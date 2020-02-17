// simple MNIST classification with a conv2d and poolmax2D
// accuracy> 96% after 15 epochs, 4s/epochs (data size reduced by factor 10)

#include <iostream>
#include <chrono>
using namespace std;

#include "Net.h"
#include "NetTrain.h"
#include "MNISTReader.h"
#include "ConfusionMatrix.h"

#include "LayerActivation.h"
#include "LayerConvolution2D.h"
#include "LayerChannelBias.h"
#include "LayerDense.h"
#include "LayerPoolMax2D.h"
#include "LayerSoftmax.h"

NetTrain netTrain;
MatrixFloat mRefImages, mRefLabels, mValImages, mValLabels;
int iEpoch;
chrono::steady_clock::time_point start;

//////////////////////////////////////////////////////////////////////////////
void epoch_callback()
{
	//compute epoch time
	chrono::steady_clock::time_point next = chrono::steady_clock::now();
	auto delta = chrono::duration_cast<std::chrono::milliseconds>(next - start).count();
	start = next;

    iEpoch++;
    cout << "Epoch: " << iEpoch << " duration: " << delta << " ms" << endl;
	cout << "TrainLoss: " << netTrain.get_current_train_loss() << " TrainAccuracy: " << netTrain.get_current_train_accuracy() << " %" ;
	cout << " ValidationAccuracy: " << netTrain.get_current_validation_accuracy() << " %" << endl;

	cout << endl;
}
//////////////////////////////////////////////////////////////////////////////
int main()
{
	cout << "Simple MNIST classification with a conv2d and poolmax2D" << endl;
	cout << "Validation accuracy> 96% after 15 epochs, 4s/epochs (data size reduced by factor 10)" << endl;

    iEpoch = 0;

	//load and normalize MNIST data
    cout << "Loading MNIST database..." << endl;
    MNISTReader mr;
    if(!mr.read_from_folder(".",mRefImages,mRefLabels, mValImages,mValLabels))
    {
        cout << "MNIST samples not found, please check the *.ubyte files are in the executable folder" << endl;
        return -1;
    }
	mValImages/= 256.f;
	mRefImages/= 256.f;
  
	// reduce data size for this test
	cout << "Decimating MNIST database (ratio=10) ..." << endl;
	mRefImages = decimate(mRefImages, 10);
	mRefLabels = decimate(mRefLabels, 10);
	mValImages = decimate(mValImages, 10);
	mValLabels = decimate(mValLabels, 10);
	
	//create simple toy convolution net:
	Net net;
	net.add(new LayerConvolution2D(28, 28, 1, 3, 3, 16));
	net.add(new LayerChannelBias(26,26,16));
	net.add(new LayerActivation("Relu"));

	net.add(new LayerPoolMax2D(26, 26, 16, 2, 2));
	net.add(new LayerChannelBias(13,13,16));
	net.add(new LayerActivation("Relu"));
	
	net.add(new LayerDense(13 * 13 * 16, 128));
	net.add(new LayerActivation("Relu"));
	net.add(new LayerDense(128, 10));

	net.add(new LayerSoftmax());

	//setup train options
	netTrain.set_net(net);
	netTrain.set_epochs(20);
	netTrain.set_batchsize(32);
	netTrain.set_loss("SparseCategoricalCrossEntropy");
	netTrain.set_epoch_callback(epoch_callback); //optional, show progress
	netTrain.set_train_data(mRefImages, mRefLabels);
	netTrain.set_validation_data(mValImages, mValLabels); //optional, not used for training, helps to keep the final best model

	// train net
	cout << "Training..." << endl << endl;
	start = chrono::steady_clock::now();
	netTrain.train();

	// show train results
	MatrixFloat mClassPredicted;
	net.classify(mRefImages, mClassPredicted);
	ConfusionMatrix cmRef;
	ClassificationResult crRef = cmRef.compute(mRefLabels, mClassPredicted);
	cout << "Ref accuracy: " << crRef.accuracy << " %" << endl;

	MatrixFloat mClassTest;
	net.classify(mValImages, mClassTest);
	ConfusionMatrix cmTest;
	ClassificationResult crTest = cmTest.compute(mValLabels, mClassTest);
	cout << "Val accuracy: " << crTest.accuracy << " %" << endl;
	cout << "Val confusion matrix:" << endl << crTest.mConfMat << endl;
	
	//testu function
	if (crTest.accuracy < 96.f)
	{
		cout << "Test failed! accuracy=" << crTest.accuracy << endl;
		return -1;
	}

	cout << "Test succeded." << endl;
    return 0;
}
