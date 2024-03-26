/*
  ==============================================================================

    RotatedLabel.h
    Created: 26 Mar 2024 11:06:13pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RotatedLabel : public juce::Label
{
public:
    RotatedLabel()
    {
        setJustificationType(juce::Justification::centred);
    }

    void paint(juce::Graphics& g) override
    {
        g.saveState();
        g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi, static_cast<float>(getWidth() / 2.0), static_cast<float>(getHeight() / 2.0)));
        juce::Label::paint(g);
        g.restoreState();
    }
};
