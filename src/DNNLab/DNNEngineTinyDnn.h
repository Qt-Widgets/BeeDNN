#ifndef DNNEngineTinyDnn_
#define DNNEngineTinyDnn_

#include "DNNEngine.h"

// forward declaration of tiny_dnn
namespace tiny_dnn {
template <typename NetType>
class network;
class sequential;
}

class DNNEngineTinyDnn : public DNNEngine
{
public:
    DNNEngineTinyDnn();
    virtual ~DNNEngineTinyDnn();
    virtual string to_string() override;

    virtual void clear() override;
    virtual void init() override;
    virtual void add_layer(int inSize,int outSize, string sLayerType) override;

    virtual void train_epochs(const MatrixFloat& mSamples,const MatrixFloat& mTruth,const DNNTrainOption& dto) override;

    virtual void predict(const MatrixFloat& mIn, MatrixFloat& mOut) override;
    virtual int classify(const MatrixFloat& mIn) override;

    virtual double compute_loss(const MatrixFloat & mSamples, const MatrixFloat& mTruth) override;

private:   
    tiny_dnn::network<tiny_dnn::sequential>* _pNet;
};

#endif
