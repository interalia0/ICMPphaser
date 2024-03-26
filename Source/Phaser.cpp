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
    lfo.prepare(spec);
    lfo.initialise([](float x) {return std::sin(x);}, 128);
  
    for (auto& filter : apf) {
        filter.setType(FilterType::APF);
        filter.setSampleRate(sampleRate);
    }    
}

void Phaser::setRate(float rate) {
    jassert (juce::isPositiveAndBelow (rate, static_cast<float> (100.f)));
    this->mRate = rate;
    lfo.setFrequency(mRate);
}

void Phaser::setDepth(float depth) {
    jassert(juce::isPositiveAndBelow(depth, static_cast<float>(101.f)));
    this->mDepth = depth;
}

float Phaser::processSampleMono(float inputSample) {
    auto lfoOut = lfo.processSample(0.f);
    auto depth = juce::jmap(mDepth, 0.f, 100.f, 0.f, 1.f);
    auto modValue = lfoOut * depth;
    auto modHz = juce::jmap(modValue, -1.f, 1.f, 20.f, 20000.f);
    
    for (int i=0; i<numFilters; ++i) {
        apf[i].setCutoff(juce::jlimit(apfMinFrequencies[i], apfMaxFrequencies[i], modHz));
    }
                         
    auto apf1 = apf[0].processSampleMono(inputSample);
    auto apf2 = apf[1].processSampleMono(apf1);
    auto apf3 = apf[2].processSampleMono(apf2);
    auto apf4 = apf[3].processSampleMono(apf3);
    auto apf5 = apf[4].processSampleMono(apf4);
    auto apf6 = apf[5].processSampleMono(apf5);
    
    float output = 0.707*inputSample + 0.707*apf6;
    return output;
}

float Phaser::processSampleStereo(int channel, float inputSample) {
    auto lfoOut = lfo.processSample(0.f);
    auto depth = juce::jmap(mDepth, 0.f, 100.f, 0.f, 1.f);
    auto modValue = lfoOut * depth;
    auto modHz = juce::jmap(modValue, -1.f, 1.f, 20.f, 20000.f);
    
    for (int i=0; i<numFilters; ++i) {
        apf[i].setCutoff(juce::jlimit(apfMinFrequencies[i], apfMaxFrequencies[i], modHz));
    }
                         
    auto apf1 = apf[0].processSampleStereo(channel, inputSample);
    auto apf2 = apf[1].processSampleStereo(channel, apf1);
    auto apf3 = apf[2].processSampleStereo(channel, apf2);
    auto apf4 = apf[3].processSampleStereo(channel, apf3);
    auto apf5 = apf[4].processSampleStereo(channel, apf4);
    auto apf6 = apf[5].processSampleStereo(channel, apf5);
    
    float output = 0.707*inputSample + 0.707*apf6;
    return output;
}

