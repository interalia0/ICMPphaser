/*
  ==============================================================================

    EditorContent.cpp
    Created: 26 Mar 2024 8:56:58pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#include "EditorContent.h"


EditorContent::EditorContent (ICMPphaserAudioProcessor& p, juce::AudioProcessorValueTreeState& treeState,
                              juce::UndoManager& um)
    : audioProcessor (p),
      rateDial        (*treeState.getParameter("rate"), &um),
      qDial           (*treeState.getParameter("q"), &um),
      feedbackDial    (*treeState.getParameter("feedback"), &um),
      offsetDial      (*treeState.getParameter("phaseOffset"), &um),
      iGainAttachment (*treeState.getParameter("gainIn"), iGainSlider),
      depthAttachment (*treeState.getParameter("depth"), depthSlider),
      mixAttachment   (*treeState.getParameter("mix"), mixSlider)

      
{
    setWantsKeyboardFocus (true);
    rateDial.setInterval(0.01f);
    qDial.setInterval(0.01f);
    feedbackDial.setInterval(0.1f);
    offsetDial.setInterval(1);
    
    addAndMakeVisible(rateDial);
    addAndMakeVisible(qDial);
    addAndMakeVisible(feedbackDial);
    addAndMakeVisible(offsetDial);
    
    addAndMakeVisible(iGainSlider);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(mixSlider);
    
    addAndMakeVisible(iGainLabel);
    iGainLabel.setText("Input Gain", juce::dontSendNotification);
    iGainLabel.setJustificationType(juce::Justification::centred);
    iGainLabel.setFont(juce::Font(15.f));
    iGainLabel.setColour(juce::Label::textColourId, juce::Colours::aquamarine);
    
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setFont(juce::Font(15.f));
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::aquamarine);
    
    addAndMakeVisible(depthLabel);
    depthLabel.setText("Depth", juce::dontSendNotification);
    depthLabel.setJustificationType(juce::Justification::centred);
    depthLabel.setFont(juce::Font(20.f));
    depthLabel.setColour(juce::Label::textColourId, juce::Colours::aquamarine);
    
    iGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    iGainSlider.setColour(juce::Slider::trackColourId, MyColours::blackGrey);
    iGainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    iGainSlider.setTextBoxIsEditable(false);
        
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mixSlider.setColour(juce::Slider::trackColourId, MyColours::blackGrey);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    mixSlider.setTextBoxIsEditable(false);
    
    depthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    depthSlider.setColour(juce::Slider::thumbColourId, juce::Colours::aquamarine);
    depthSlider.setColour(juce::Slider::trackColourId, MyColours::blackGrey);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    depthSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    
    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);
    
    startTimerHz(24);
    
}

EditorContent::~EditorContent()
{
}

void EditorContent::resized()
{
    rateDial.setBounds     (72, 90, 90, 90);
    qDial.setBounds        (164, 90, 90, 90);
    feedbackDial.setBounds (256, 90, 90, 90);
    offsetDial.setBounds   (348, 90, 90, 90);
    
    iGainSlider.setBounds  (0, 40, 60, 210);
    depthSlider.setBounds  (60, 30, 380, 40);
    mixSlider.setBounds    (440, 40, 60, 210);
    
    iGainLabel.setBounds   (0, 0, 60, 40);
    depthLabel.setBounds   (60, 8, 380, 20);
    mixLabel.setBounds     (440, 0, 60, 40);

    meterL.setBounds       (70, 195, 360, 20);
    meterR.setBounds       (70, 220, 360, 20);    
}


void EditorContent::dialValueChanged(Dial* dial)
{
}

void EditorContent::sliderValueChanged (juce::Slider* slider)
{
}


void EditorContent::timerCallback()
{
    meterL.setLevel(audioProcessor.getRmsValue(0));
    meterR.setLevel(audioProcessor.getRmsValue(1));
    meterL.repaint();
    meterR.repaint();
}
