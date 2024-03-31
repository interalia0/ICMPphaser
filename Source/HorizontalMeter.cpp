/*
  ==============================================================================

    HorizontalMeter.cpp
    Created: 27 Mar 2024 9:46:14am
    Author:  Elja Markkanen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HorizontalMeter.h"

//==============================================================================
HorizontalMeter::HorizontalMeter() {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

HorizontalMeter::~HorizontalMeter() {
}

void HorizontalMeter::paint (Graphics& g) {
    auto bounds = getLocalBounds().toFloat();

    g.setColour(Colours::white.withBrightness(0.4f));
    g.fillRoundedRectangle(bounds, 5.f);

    if (level >= -15.f) {
        using namespace Colours;
        float interpolationFactor = jmap(level, -15.f, 0.f, 0.f, 1.f);
        Colour interpolatedColor = aquamarine.withBrightness(0.9f).interpolatedWith(red.withBrightness(0.8f), interpolationFactor);
        g.setColour(interpolatedColor);
    }
    else {
        using namespace juce::Colours;
        float slowInterpolationFactor = juce::jmap(level, -60.f, 0.f, 1.f, 0.f);
        juce::Colour interpolatedColor = aquamarine.withBrightness(0.9f).interpolatedWith(white.withBrightness(0.9f), slowInterpolationFactor);
        g.setColour(interpolatedColor);
    }
    const auto scaledX = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getWidth()));
    g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f); 
}

void HorizontalMeter::resized()
{
}

void HorizontalMeter::setLevel(const float value) {
    this->level = value;
}
