#include "DNNEngineTestDnn.h"

#include "Net.h"
#include "NetTrain.h"
#include "LayerActivation.h"
#include "LayerDense.h"
#include "NetUtil.h"

//////////////////////////////////////////////////////////////////////////////
DNNEngineTestDnn::DNNEngineTestDnn()
{
    _pNet=new Net;
}
//////////////////////////////////////////////////////////////////////////////
DNNEngineTestDnn::~DNNEngineTestDnn()
{
    _pNet->clear();
}
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::clear()
{
    _pNet->clear();
}
//////////////////////////////////////////////////////////////////////////////
string DNNEngineTestDnn::to_string()
{
    return NetUtil::to_string(_pNet);
}
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::init()
{
    _pNet->init();
    DNNEngine::init();
}
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::add_dense_layer(int inSize, int outSize, bool bWithBias)
{
    _pNet->add_dense_layer(inSize,outSize,bWithBias);
}
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::add_activation_layer(string sActivation)
{
    _pNet->add_activation_layer(sActivation);
}
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::add_dropout_layer(int inSize,float fRatio)
{
    _pNet->add_dropout_layer(inSize,fRatio);
}
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::predict(const MatrixFloat& mIn, MatrixFloat& mOut)
{
    _pNet->forward(mIn,mOut);
}
//////////////////////////////////////////////////////////////////////////////
/*int DNNEngineTestDnn::classify(const MatrixFloat& mIn)
{
    return _pNet->classify(mIn);
}
*/
//////////////////////////////////////////////////////////////////////////////
void DNNEngineTestDnn::learn_epochs(const MatrixFloat& mSamples,const MatrixFloat& mTruth,const DNNTrainOption& dto)
{
    TrainOption tOpt;
    tOpt.epochs=dto.epochs;
    tOpt.learningRate=dto.learningRate;
    tOpt.batchSize=dto.batchSize;
    tOpt.optimizer=dto.optimizer;
    tOpt.decay=dto.decay;
    tOpt.momentum=dto.momentum;
    //tOpt.observer=nullptr;//dto.observer;
    tOpt.testEveryEpochs=dto.testEveryEpochs;

    NetTrain netTrain;
    if(_bClassification)
        netTrain.train(*_pNet,mSamples,mTruth,tOpt);
    else
        netTrain.fit(*_pNet,mSamples,mTruth,tOpt);

    const auto& l=netTrain.loss();
    _vdLoss.insert(end(_vdLoss),begin(l),end(l)); //temp
}
//////////////////////////////////////////////////////////////////////////////
float DNNEngineTestDnn::compute_loss(const MatrixFloat & mSamples, const MatrixFloat& mTruth)
{
    NetTrain netTrain;
    return netTrain.compute_loss(*_pNet,mSamples,mTruth);
}
//////////////////////////////////////////////////////////////////////////////
