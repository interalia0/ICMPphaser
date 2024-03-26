/*
  ==============================================================================

    Phaser.h
    Created: 25 Mar 2024 2:16:54pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include "Filter.h"
#include <JuceHeader.h>

class Phaser {
    
public:
    void reset();
    void prepare(juce::dsp::ProcessSpec& spec, double sampleRate);
    void setRate(float rate);
    void setDepth(float depth);
    void setFeedback(int feedback);
    float processSample(float inputSample);
    
    /// Old function for multi channel processing.
    __attribute__((deprecated("not implemented, use processSample instead")))
    float processSampleStereo(int channel, float inputSample);    
private:
    float mDepth = 0.f;
    float mRate = 0.5f;
    float mFeedback = 0.f;
    
    static constexpr int numFilters = 6;
    const float apfMinFrequencies[6] = {32, 68, 96, 212, 320, 636};
    const float apfMaxFrequencies[6] = {1500, 3400, 4800, 10000, 16000, 20480};

    std::array<Filter, numFilters> apf;
    std::array<float, numFilters> apfOutputs;
    juce::dsp::Oscillator<float> lfo;
};
