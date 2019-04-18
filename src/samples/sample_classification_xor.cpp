// this sample shows how to do a simple classification, the usecase is to learn a XOR gate
// the output of this network is a Sigmoid, so the we can use the loss BinaryCrossEntropy and 0 -> class_0 ; 1 -> class_1

#include <iostream>
using namespace std;

#include "Net.h"
#include "NetTrain.h"

int main()
{
    //contruct layer
    Net net;
    net.add_dense_layer(2,3);
	net.add_activation_layer("Tanh");
	net.add_dense_layer(3, 1);
	net.add_activation_layer("Sigmoid");

    //set the train data
    float dSamples[]={ 0,0 , 0,1 , 1,0 , 1,1 };
    float dTruths[]={ 0 , 1 , 1, 0 };
    const MatrixFloat mSamples=fromRawBuffer(dSamples,4,2);
    const MatrixFloat mTruth=fromRawBuffer(dTruths,4,1);

    //optimize
    TrainOption tOpt;
    tOpt.learningRate=0.05f;
	tOpt.epochs = 1000;
	NetTrain netFit;
	netFit.set_loss("BinaryCrossEntropy");
	netFit.fit(net,mSamples,mTruth,tOpt);

    //predict and show results
    MatrixFloat m00,m01,m10,m11;
    net.forward(mSamples.row(0),m00);
    net.forward(mSamples.row(1),m01);
    net.forward(mSamples.row(2),m10);
    net.forward(mSamples.row(3),m11);
    cout << "0_xor_0=" << m00(0) << "  0_xor_1=" <<m01(0) << "  1_xor_0=" << m10(0) << "  1_xor_1=" << m11(0) << endl;

    //compute total loss
    float fLoss=netFit.compute_loss(net,mSamples,mTruth);
    cout << "Loss=" << fLoss << endl;

    return 0;
}
