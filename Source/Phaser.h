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
    Phaser();
    void reset();
    void prepare(juce::dsp::ProcessSpec& spec, double sampleRate);
    void setRate(float rate);
    void setDepth(float depth);
    void setFeedback(float feedback);
    void setResonance(float resonance);
    void setPhaseReversal(bool phase);
    virtual float processSample(float inputSample) = default;
private:
    float mDepth = 0.f;
    float mRate = 0.5f;
    float mFeedback = 0.f;
    float mResonance = 0.7f;
    bool mIsPhaseFlipped = false;
    
    int numFilters = 6;
    std::vector<float> apfMinFreq = {32, 68, 96, 212, 320, 636};
    std::vector<float> apfMaxFreq = {1500, 3400, 4800, 10000, 16000, 20480};
    
    int samplesSinceLastUpdate = 5;

    std::vector<Filter> apf;
    std::vector<float> apfOutputs;
    juce::dsp::Oscillator<float> lfo;
    float modHz = 0.f;
       
    /// Old function for multi channel processing.
    __attribute__((deprecated("use processSample instead")))
    float processSampleStereo(int channel, float inputSample);  
};
