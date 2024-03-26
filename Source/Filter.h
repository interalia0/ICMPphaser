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

    void reset ();
    float processSample (float inputSample);
    
    __attribute__((deprecated("not implemented, use processSample instead")))
    float processSampleStereo (int channel, float inputSample);
    __attribute__((deprecated("not implemented, use getS_value instead")))
    float getS_valueSt(int channel);
private:
    void updateCoefficents();

    const float mPI = juce::MathConstants<float>::pi;
    double mFs = 44100;
    float mFc = 20000.f;
    float mQ = 0.7;
    float alpha = 0.f, omega = 0.f;
    float a0 = 1.f, a1 = 0.f, a2 = 0.f, b0 = 0.f, b1 = 0.f, b2 = 0.f;
    float y0;
    
    std::array<float, 2> xn_1stereo;
    std::array<float, 2> xn_2stereo;
    std::array<float, 2> yn_1stereo;
    std::array<float, 2> yn_2stereo;
    
    float xz_1 = 0;
    float xz_2 = 0;
    float yz_1 = 0;
    float yz_2 = 0;

    FilterType mFilterType = FilterType::LPF;
};
