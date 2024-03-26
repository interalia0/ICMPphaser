/*
  ==============================================================================

    Filter.h
    Created: 25 Mar 2024 2:16:43pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum class FilterType {
    LPF,
    HPF,
    BPF,
    APF
};

class Filter {
public:
    void setCutoff (float cutoff);
    void setQ (float q);
    void setType (FilterType type);
    void setSampleRate (double sampleRate);
    float getG_value() const;
    float getS_Value();
    float getS_valueSt(int channel);


    void reset ();
    float processSampleMono (float inputSample);
    float processSampleStereo (int channel, float inputSample);

private:
    void updateCoefficents();

    const float mPI = juce::MathConstants<float>::pi;
    double mFs = 44100;
    float mFc = 20000.f;
    float mQ = 0.7;
    float alpha = 0.f, omega = 0.f;
    float a0 = 1.f, a1 = 0.f, a2 = 0.f, b0 = 0.f, b1 = 0.f, b2 = 0.f;
    float y0;
    
    std::array<float, 2> xn_1;
    std::array<float, 2> xn_2;    
    std::array<float, 2> yn_1;
    std::array<float, 2> yn_2;
    
    float xn_1f = 0;
    float xn_2f = 0;
    float yn_1f = 0;
    float yn_2f = 0;


    FilterType mFilterType = FilterType::LPF;
};
