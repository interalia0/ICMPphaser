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
    xz_1=0;
    xz_2=0;
    yz_1=0;
    yz_2=0;
    
    std::fill(xn_1stereo.begin(), xn_1stereo.end(), 0.f);
    std::fill(xn_2stereo.begin(), xn_2stereo.end(), 0.f);
    std::fill(yn_1stereo.begin(), yn_1stereo.end(), 0.f);
    std::fill(yn_2stereo.begin(), yn_2stereo.end(), 0.f);
}

float Filter::getG_value() const {
    return a0;
}

float Filter::getS_Value() {
    float storageComponent = 0.f;
    storageComponent = a1 * xz_1 + a2 * xz_2 - b1 * yz_1 - b2 * yz_2;
    return storageComponent;
}

/// Old method for getting the S values for stereo processing.

float Filter::getS_valueSt(int channel) {
    float storageComponent = 0.f;
    storageComponent = a1 * xn_1stereo[channel] + a2 * xn_2stereo[channel] - b1 * yn_1stereo[channel] - b2 * yn_2stereo[channel];
    return storageComponent;
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

float Filter::processSample(float inputSample) {
    float x0 = inputSample;
    y0 = (b0/a0)*x0 + (b1/a0)*xz_1 + (b2/a0)*xz_2 - (a1/a0)*yz_1 - (a2/a0)*yz_2;
    auto outputSample = y0;

    xz_2 = xz_1;
    xz_1 = x0;
    yz_2 = yz_1;
    yz_1 = y0;
    
    return outputSample;
}

/// Old method for stereo processing. Use two instances and process sample instead.

float Filter::processSampleStereo(int channel, float inputSample) {
    float x0 = inputSample;
    y0 = (b0/a0)*x0 + (b1/a0)*xn_1stereo[channel] + (b2/a0)*xn_2stereo[channel] - (a1/a0)*yn_1stereo[channel] - (a2/a0)*yn_2stereo[channel];
    auto outputSample = y0;

    xn_2stereo[channel] = xn_1stereo[channel];
    xn_1stereo[channel] = x0;
    yn_2stereo[channel] = yn_1stereo[channel];
    yn_1stereo[channel] = y0;
    
    return outputSample;
}
