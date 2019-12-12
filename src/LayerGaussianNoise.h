/*
    Copyright (c) 2019, Etienne de Foras and the respective contributors
    All rights reserved.

    Use of this source code is governed by a MIT-style license that can be found
    in the LICENSE.txt file.
*/

#ifndef LayerGaussianNoise_
#define LayerGaussianNoise_

#include "Layer.h"
#include "Matrix.h"

#include <string>
#include <random>
using namespace std;

class Activation;

class LayerGaussianNoise : public Layer
{
public:
    LayerGaussianNoise(int iSize,float fStd);
    virtual ~LayerGaussianNoise() override;

    virtual Layer* clone() const override;

    virtual void forward(const MatrixFloat& mIn, MatrixFloat &mOut) override;

    virtual void backpropagation(const MatrixFloat &mIn,const MatrixFloat &mGradientOut, MatrixFloat &mGradientIn) override;

    float get_std() const;

private:
    float _fStd;
	normal_distribution<float> _distNormal;
};

#endif
