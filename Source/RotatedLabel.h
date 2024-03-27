/*
  ==============================================================================

    RotatedLabel.h
    Created: 26 Mar 2024 11:33:09pm
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
        updateTransform();
        updateFont();
    }
    
    void setLetterSpacing(float spacing) {
        letterSpacing = spacing;
        updateFont();
    }

private:
    
    void updateTransform() {
        transform = juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi, static_cast<float>(getWidth() / 2.0), static_cast<float>(getHeight() / 2.0));
    }

    void paint(juce::Graphics& g) override {
        g.saveState();
        g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi, static_cast<float>(getWidth() / 2.0), static_cast<float>(getHeight() / 2.0)));
        juce::Label::paint(g);
        g.restoreState();
    }
    
    void updateFont() {
        juce::Font newFont(getFont());
        newFont.setExtraKerningFactor(letterSpacing);
        setFont(newFont);
    }
    

    
    juce::AffineTransform transform;
    float letterSpacing = 0.4f;
};
