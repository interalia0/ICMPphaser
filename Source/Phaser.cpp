/*
  ==============================================================================

    Phaser.cpp
    Created: 25 Mar 2024 2:16:54pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#include "Phaser.h"

void Phaser::reset() {
    lfo.reset();
    for (auto& filter : apf) {
        filter.reset();
    }
}

void Phaser::prepare(juce::dsp::ProcessSpec &spec, double sampleRate) {
//    spec.sampleRate = sampleRate / 2;
    lfo.prepare(spec);
    lfo.initialise([](float x) {return std::sin(x);}, 128);
  
    for (auto& filter : apf) {
        filter.setType(FilterType::APF);
        filter.setSampleRate(sampleRate);
    }    
}

void Phaser::setRate(float rate) {
    if (!juce::approximatelyEqual(mRate, rate)) {
        this->mRate = rate;
        lfo.setFrequency(mRate);
    }
}

void Phaser::setDepth(float depth) {
    if (!juce::approximatelyEqual(mDepth, depth)) {
        this->mDepth = depth;
    }
}

void Phaser::setFeedback(float feedback) {
    if (!juce::approximatelyEqual(mFeedback, feedback)) {
        this->mFeedback = feedback;
    }
}

void Phaser::setResonance(float resonance) {
    auto mappedResonance = juce::jmap(resonance, 0.f, 1.f, 3.f, 0.61f);
    if (!juce::approximatelyEqual(mResonance, mappedResonance)) {
        this->mResonance = mappedResonance;
    }
}

void Phaser::setPhaseReversal(bool phase) {
    this->mIsPhaseFlipped = phase;
}

float Phaser::processSample(float inputSample) {
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

/// Old function for multi channel processing.

float Phaser::processSampleStereo(int channel, float inputSample) {
    auto lfoOut = lfo.processSample(0.f);
    auto depth = juce::jmap(mDepth, 0.f, 100.f, 0.f, 1.f);
    auto modValue = lfoOut * depth;
    auto modHz = juce::jmap(modValue, -1.f, 1.f, 20.f, 20000.f);
    
    for (int i=0; i<numFilters; ++i) {
        apf[i].setCutoff(juce::jmap(modHz, 20.f, 20000.f, apfMinFreq[i], apfMaxFreq[i]));
    }
    
    float gamma1 = apf[5].getG_value();
    float gamma2 = apf[4].getG_value()*gamma1;
    float gamma3 = apf[3].getG_value()*gamma2;
    float gamma4 = apf[2].getG_value()*gamma3;
    float gamma5 = apf[1].getG_value()*gamma4;
    float gamma6 = apf[0].getG_value()*gamma5;
    
    float K = mFeedback / 1000;
    float alpha0 = 1.f / (1.f + K*gamma6);
    
    float Sn = gamma5*apf[0].getS_value(channel) +
               gamma4*apf[1].getS_value(channel) +
               gamma3*apf[2].getS_value(channel) +
               gamma2*apf[3].getS_value(channel) +
               gamma1*apf[4].getS_value(channel) +
               apf[5].getS_value(channel);
    
    float u = alpha0*(inputSample - K*Sn);

    auto apf1 = apf[0].processSample(channel, u);
    auto apf2 = apf[1].processSample(channel, apf1);
    auto apf3 = apf[2].processSample(channel, apf2);
    auto apf4 = apf[3].processSample(channel, apf3);
    auto apf5 = apf[4].processSample(channel, apf4);
    auto apf6 = apf[5].processSample(channel, apf5);

    float output = 0.303*inputSample + 0.707*apf6;
    return output;
    
}

