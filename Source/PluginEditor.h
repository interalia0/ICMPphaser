/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EditorLnf.h"
#include "EditorContent.h"
#include "Dial.h"
#include "MyColours.h"
#include "RotatedLabel.h"

//==============================================================================
/**
*/
class ICMPphaserAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ICMPphaserAudioProcessorEditor (ICMPphaserAudioProcessor&,
                                    juce::AudioProcessorValueTreeState& treeState,
                                    juce::UndoManager& undoManager);
    ~ICMPphaserAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool keyPressed (const juce::KeyPress& key) override;
private:
    ICMPphaserAudioProcessor& audioProcessor;
    juce::UndoManager& undoManager;
    EditorContent editorContent;
    
    
    static constexpr int defaultWidth  {500};
    static constexpr int defaultHeight {200};

    struct SharedLnf
    {
        SharedLnf()  { juce::LookAndFeel::setDefaultLookAndFeel (&editorLnf); }
        ~SharedLnf() { juce::LookAndFeel::setDefaultLookAndFeel (nullptr); }

        EditorLnf editorLnf;
    };

    juce::SharedResourcePointer<SharedLnf> lnf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICMPphaserAudioProcessorEditor)
};
