/*
    Copyright (c) 2019, Etienne de Foras and the respective contributors
    All rights reserved.

    Use of this source code is governed by a MIT-style license that can be found
    in the LICENSE.txt file.
*/

#include "LayerGlobalBias.h"

///////////////////////////////////////////////////////////////////////////////
LayerGlobalBias::LayerGlobalBias() :
    Layer(0 , 0, "GlobalBias")
{
    _weight.resize(1,1);
    LayerGlobalBias::init();
}
///////////////////////////////////////////////////////////////////////////////
LayerGlobalBias::~LayerGlobalBias()
{ }
///////////////////////////////////////////////////////////////////////////////
Layer* LayerGlobalBias::clone() const
{
    LayerGlobalBias* pLayer=new LayerGlobalBias();
	pLayer->weights() = _weight;

    return pLayer;
}
///////////////////////////////////////////////////////////////////////////////
void LayerGlobalBias::init()
{
    _weight.setZero(); // by default

    Layer::init();
}
///////////////////////////////////////////////////////////////////////////////
void LayerGlobalBias::forward(const MatrixFloat& mMatIn,MatrixFloat& mMatOut) const
{
    mMatOut = mMatIn.array() + _weight(0); //WIP
}
///////////////////////////////////////////////////////////////////////////////
void LayerGlobalBias::backpropagation(const MatrixFloat &mInput,const MatrixFloat &mDelta, MatrixFloat &mNewDelta)
{
    (void)mInput;

    mNewDelta = mDelta; //WIP
    _gradientWeight = mDelta; //WIP
}
///////////////////////////////////////////////////////////////////////////////
float LayerGlobalBias::bias() const
{
    return _weight(0);
}
///////////////////////////////////////////////////////////////
