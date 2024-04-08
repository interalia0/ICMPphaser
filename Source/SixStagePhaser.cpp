/*
  ==============================================================================

    SixStagePhaser.cpp
    Created: 31 Mar 2024 9:01:59pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#include "SixStagePhaser.h"

SixStagePhaser::SixStagePhaser() {
    numFilters = 6;
    apf.resize(numFilters);
    apfOutputs.resize(numFilters);
    apfMinFreq.resize(numFilters);
    apfMaxFreq.resize(numFilters);
    apfMinFreq = {32, 68, 96, 212, 320, 636};
    apfMaxFreq = {1500, 3400, 4800, 10000, 16000, 20480};
}

float SixStagePhaser::processSample(float inputSample) {
    if (samplesSinceLastUpdate == 0) {
        auto lfoOut = lfo.processSample(0.f);
        auto depth = juce::jmap(mDepth, 0.f, 100.f, 0.f, 1.f);
        auto modValue = lfoOut * depth;
        
        if (mIsPhaseFlipped) {
            modHz = juce::jmap(modValue, -1.f, 1.f, 20000.f, 20.f);
        } else {
            modHz = juce::jmap(modValue, -1.f, 1.f, 20.f, 20000.f);
        }
        
        for (int i = 0; i < numFilters; ++i) {
            apf[i].setCutoff(juce::jmap(modHz, 20.f, 20000.f, apfMinFreq[i], apfMaxFreq[i]));
        }
        
        for (int i = 0; i < numFilters; ++i) {
            apf[i].setQ(mResonance);
        }
        
        samplesSinceLastUpdate = 5;
    }

    float gamma1 = apf[5].getG_value();
    float gamma2 = apf[4].getG_value() * gamma1;
    float gamma3 = apf[3].getG_value() * gamma2;
    float gamma4 = apf[2].getG_value() * gamma3;
    float gamma5 = apf[1].getG_value() * gamma4;
    float gamma6 = apf[0].getG_value() * gamma5;
    
    float K = mFeedback / 1000;
    float alpha0 = 1.f / (1.f + K * gamma6);
    
    float Sn = gamma5 * apf[0].getS_Value() +
               gamma4 * apf[1].getS_Value() +
               gamma3 * apf[2].getS_Value() +
               gamma2 * apf[3].getS_Value() +
               gamma1 * apf[4].getS_Value() +
               apf[5].getS_Value();
    
    float u = alpha0 * (inputSample - K * Sn);
    float apfInput = u;
    
    for (int i = 0; i < numFilters; ++i) {
        apfOutputs[i] = apf[i].processSample(apfInput);
        apfInput = apfOutputs[i];
    }
        
    if (samplesSinceLastUpdate > 0) {
        samplesSinceLastUpdate--;
    }
    
    float finalApfOutput = apfInput;
    float output = 0.707 * inputSample + 0.707 * finalApfOutput;
    return output;
}
