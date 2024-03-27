/*
  ==============================================================================

    EditorContent.h
    Created: 26 Mar 2024 8:56:58pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Dial.h"
#include "EditorLnf.h"
#include "MyColours.h"
#include "RotatedLabel.h"

class EditorContent  : public juce::Component,
                       public Dial::Listener,
                       public juce::Slider::Listener
{
public:
    EditorContent (juce::AudioProcessorValueTreeState& treeState,
                   juce::UndoManager& um);
    
    ~EditorContent();

    void resized() override;
    void dialValueChanged(Dial* dial) override;
    void sliderValueChanged (juce::Slider* slider) override;
    
private:    
    Dial rateDial, qDial, feedbackDial, offsetDial;
    juce::Slider iGainSlider, depthSlider, mixSlider;
    juce::Label iGainLabel, depthLabel, mixLabel;
    
    juce::SliderParameterAttachment iGainAttachment, depthAttachment, mixAttachment;
    
    RotatedLabel rotatedLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorContent)
};
