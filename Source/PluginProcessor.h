/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Phaser.h"

//==============================================================================
/**
*/
class ICMPphaserAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ICMPphaserAudioProcessor();
    ~ICMPphaserAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParamLayout();
    juce::AudioProcessorValueTreeState treeState {*this, &undoManager, "params", createParamLayout()};
    juce::UndoManager undoManager;
    
    float getRmsValue(const int channel); /// Function to get the current RMS value for updating the UI.
private:
    float calculateRms(const int channel, juce::dsp::AudioBlock<float>& block) const; /// Function for calculating the rms value from a AudioBlock object.
    juce::LinearSmoothedValue<float> rmsL, rmsR;
    
    juce::dsp::DryWetMixer<float> drywet;
    Phaser phaserL;
    Phaser phaserR;
    
    void updatePhaser();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICMPphaserAudioProcessor)
};
