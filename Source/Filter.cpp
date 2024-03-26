/*
  ==============================================================================

    Filter.cpp
    Created: 25 Mar 2024 2:16:43pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#include "Filter.h"

void Filter::setCutoff(float cutoff) {
    this->mFc = cutoff;
    updateCoefficents();
}

void Filter::setQ(float q) {
    this->mQ = q;
    updateCoefficents();
}

void Filter::setType(FilterType type) {
    this->mFilterType = type;
    updateCoefficents();
}

void Filter::setSampleRate(double sampleRate) {
    this->mFs = sampleRate;
}

void Filter::reset() {
    std::fill(xn_1.begin(), xn_1.end(), 0.f);
    std::fill(xn_2.begin(), xn_2.end(), 0.f);
    std::fill(yn_1.begin(), yn_1.end(), 0.f);
    std::fill(yn_2.begin(), yn_2.end(), 0.f);
    
    xn_1f=0;
    xn_2f=0;
    yn_1f=0;
    yn_2f=0;
}


void Filter::updateCoefficents() {
    omega = (2 * mPI) * (mFc / mFs);
    alpha = sin(omega) / (2 * mQ);
    
    switch (mFilterType) {
        case FilterType::LPF:
            a0 = 1 + alpha;
            a1 = -2 * cos(omega);
            a2 = 1 - alpha;
            b0 = (1 - cos(omega)) / 2;
            b1 = 1 - cos(omega);
            b2 = (1 - cos(omega)) / 2;
            break;
        case FilterType::HPF:
            a0 = 1 + alpha;
            a1 = -2 * cos(omega);
            a2 = 1 - alpha;
            b0 = (1 + cos(omega)) / 2;
            b1 = -(1 + cos(omega));
            b2 = (1 + cos(omega)) / 2;
            break;
        case FilterType::BPF:
            a0 = 1 + alpha;
            a1 = -2 * cos(omega);
            a2 = 1 - alpha;
            b0 = alpha;
            b1 = 0.f;
            b2 = -alpha;
            break;
        case FilterType::APF:
            a0 = 1 + alpha;
            a1 = -2 * cos(omega);
            a2 = 1 - alpha;
            b0 = 1 - alpha;
            b1 = -2 * cos(omega);
            b2 = 1 + alpha;
            break;
        default:
            break;
    }
}

float Filter::processSampleMono(float inputSample) {
    float x0 = inputSample;
    float y0 = (b0/a0)*x0 + (b1/a0)*xn_1f + (b2/a0)*xn_2f - (a1/a0)*yn_1f - (a2/a0)*yn_2f;
    auto outputSample = y0;

    xn_2f = xn_1f;
    xn_1f = x0;
    yn_2f = yn_1f;
    yn_1f = y0;
    
    return outputSample;
}

float Filter::processSampleStereo(int channel, float inputSample) {
    float x0 = inputSample;
    float y0 = (b0/a0)*x0 + (b1/a0)*xn_1[channel] + (b2/a0)*xn_2[channel] - (a1/a0)*yn_1[channel] - (a2/a0)*yn_2[channel];
    auto outputSample = y0;

    xn_2[channel] = xn_1[channel];
    xn_1[channel] = x0;
    yn_2[channel] = yn_1[channel];
    yn_1[channel] = y0;
    
    return outputSample;
}
