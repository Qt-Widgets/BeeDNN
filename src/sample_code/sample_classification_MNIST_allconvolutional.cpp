// all convolutional MNIST classification with a conv2d (no poolmax layers)
// accuracy > 99% after 20 epochs, 25s/epochs
// conv2d speed is not fully optimized yet.

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
#include "LayerDropout.h"
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
    iEpoch = 0;

	cout << "all convolutional MNIST classification with a conv2d (no poolmax layers)" << endl;
	cout << "accuracy> 99% after 20 epochs, 25s/epochs" << endl;
	cout << "conv2d speed is not fully optimized yet" << endl;

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

	//create a all convolutional net:
	Net net;
	net.add(new LayerConvolution2D(28, 28, 1, 3, 3, 8));
	net.add(new LayerChannelBias(26,26,8));
	net.add(new LayerActivation("Relu"));

	net.add(new LayerConvolution2D(26, 26, 8, 3, 3, 8, 2, 2));
	net.add(new LayerChannelBias(12,12,8));
	net.add(new LayerActivation("Relu"));
	net.add(new LayerDropout(0.3f));

	net.add(new LayerConvolution2D(12, 12, 8, 3, 3, 8));
	net.add(new LayerChannelBias(10,10,8));
	net.add(new LayerActivation("Relu"));
	net.add(new LayerDropout(0.3f));

	net.add(new LayerDense(10 * 10 * 8, 128));

	net.add(new LayerActivation("Relu"));
	net.add(new LayerDense(128, 10));
	net.add(new LayerSoftmax());

	//set train options
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
	if (crTest.accuracy < 99.f)
	{
		cout << "Test failed! accuracy=" << crTest.accuracy << endl;
		return -1;
	}

	cout << "Test succeded." << endl;
    return 0;
}
