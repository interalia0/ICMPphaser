/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MyColours.h"

//==============================================================================
ICMPphaserAudioProcessorEditor::ICMPphaserAudioProcessorEditor (ICMPphaserAudioProcessor& p,
                                                                juce::AudioProcessorValueTreeState& treeState,
                                                                juce::UndoManager& um)
    : AudioProcessorEditor (&p), audioProcessor (p), undoManager (um), editorContent (treeState, um)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    juce::ignoreUnused (audioProcessor);

    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable (false, true);
    getConstrainer()->setFixedAspectRatio (ratio);
    getConstrainer()->setSizeLimits (defaultWidth / 2, defaultHeight / 2,
                                     defaultWidth * 2, defaultHeight * 2);
    setSize (defaultWidth * 1.5, defaultHeight * 1.5);
    editorContent.setSize (defaultWidth * 1.5, defaultHeight * 1.5);
    
    addAndMakeVisible (editorContent);
    
}

ICMPphaserAudioProcessorEditor::~ICMPphaserAudioProcessorEditor()
{
}

//==============================================================================
void ICMPphaserAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
}

void ICMPphaserAudioProcessorEditor::resized()
{
    const auto factor = static_cast<float> (getWidth()) / defaultWidth;
    editorContent.setTransform (juce::AffineTransform::scale (factor));
}

bool ICMPphaserAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    const auto cmdZ = juce::KeyPress { 'z', juce::ModifierKeys::commandModifier, 0 };

    if (key == cmdZ && undoManager.canUndo())
    {
        undoManager.undo();
        return true;
    }

    const auto cmdShiftZ = juce::KeyPress { 'z', juce::ModifierKeys::commandModifier
                                                 | juce::ModifierKeys::shiftModifier, 0 };

    if (key == cmdShiftZ && undoManager.canRedo())
    {
        undoManager.redo();
        return true;
    }

    return false;
}
