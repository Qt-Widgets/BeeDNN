/*
    Copyright (c) 2019, Etienne de Foras and the respective contributors
    All rights reserved.

    Use of this source code is governed by a MIT-style license that can be found
    in the LICENSE.txt file.
*/

#include "LayerGlobalGain.h"

///////////////////////////////////////////////////////////////////////////////
LayerGlobalGain::LayerGlobalGain(int iInSize) :
    Layer(iInSize, iInSize, "GlobalGain")
{
    _weight.resize(1,1);
    LayerGlobalGain::init();
}
///////////////////////////////////////////////////////////////////////////////
LayerGlobalGain::~LayerGlobalGain()
{ }
///////////////////////////////////////////////////////////////////////////////
Layer* LayerGlobalGain::clone() const
{
    LayerGlobalGain* pLayer=new LayerGlobalGain(_iInSize);
	pLayer->weights() = _weight;

    return pLayer;
}
///////////////////////////////////////////////////////////////////////////////
void LayerGlobalGain::init()
{
    _weight.setOnes(); // by default

    Layer::init();
}
///////////////////////////////////////////////////////////////////////////////
void LayerGlobalGain::forward(const MatrixFloat& mMatIn,MatrixFloat& mMatOut) const
{
    mMatOut = mMatIn * _weight(0);
}
///////////////////////////////////////////////////////////////////////////////
void LayerGlobalGain::backpropagation(const MatrixFloat &mInput,const MatrixFloat &mDelta, MatrixFloat &mNewDelta)
{
    _gradientWeight = (mInput.transpose())*mDelta;

    mNewDelta = mDelta * _weight(0);
}
///////////////////////////////////////////////////////////////////////////////
float LayerGlobalGain::gain() const
{
    return _weight(0);
}
///////////////////////////////////////////////////////////////
