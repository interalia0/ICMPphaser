/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ICMPphaserAudioProcessor::ICMPphaserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ICMPphaserAudioProcessor::~ICMPphaserAudioProcessor()
{
}

//==============================================================================
const juce::String ICMPphaserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ICMPphaserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ICMPphaserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ICMPphaserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ICMPphaserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ICMPphaserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ICMPphaserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ICMPphaserAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ICMPphaserAudioProcessor::getProgramName (int index)
{
    return {};
}

void ICMPphaserAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ICMPphaserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    drywet.prepare(spec);
    drywet.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    
    phaserL.prepare(spec, sampleRate);
    phaserL.reset();
    phaserR.prepare(spec, sampleRate);
    phaserR.reset();
    
    rmsL.reset(sampleRate, 0.5f);
    rmsR.reset(sampleRate, 0.5f);
    rmsL.setCurrentAndTargetValue(-100.f);
    rmsR.setCurrentAndTargetValue(-100.f);
}

void ICMPphaserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ICMPphaserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ICMPphaserAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    auto input = context.getInputBlock();
    auto output = context.getOutputBlock();
    
    auto* inL = input.getChannelPointer(0);
    auto* inR = input.getChannelPointer(1);
    auto* outL = output.getChannelPointer(0);
    auto* outR = output.getChannelPointer(1);
    
    auto gainValuedB = treeState.getRawParameterValue("gainIn")->load();
    auto gain = juce::Decibels::decibelsToGain(gainValuedB);
    buffer.applyGain(0, buffer.getNumSamples(), gain);
    
    rmsL.skip(numSamples);
    rmsR.skip(numSamples);

    auto mix = treeState.getRawParameterValue("mix")->load();
    drywet.pushDrySamples(input);
    drywet.setWetMixProportion(mix);
    updatePhaser();

    for (int sample = 0; sample < numSamples; ++sample) {
        outL[sample] = phaserL.processSample(inL[sample]);
        outR[sample] = phaserR.processSample(inR[sample]);
    }
    {
        const auto valueL = juce::Decibels::gainToDecibels(calculateRms(0, output));
            if (valueL < rmsL.getCurrentValue())
                rmsL.setTargetValue(valueL);
            else
                rmsL.setCurrentAndTargetValue(valueL);
    }
    {
        const auto valueR = juce::Decibels::gainToDecibels(calculateRms(1, output));
            if (valueR < rmsR.getCurrentValue())
                rmsR.setTargetValue(valueR);
            else
                rmsR.setCurrentAndTargetValue(valueR);
    }
    drywet.mixWetSamples(output);
}

//==============================================================================
bool ICMPphaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ICMPphaserAudioProcessor::createEditor()
{
    return new ICMPphaserAudioProcessorEditor (*this, treeState, undoManager);
//    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void ICMPphaserAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    treeState.state.writeToStream (mos);
}

void ICMPphaserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, static_cast<size_t> (sizeInBytes));

    if (tree.isValid())
        treeState.replaceState (tree);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ICMPphaserAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout ICMPphaserAudioProcessor::createParamLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    using pID = juce::ParameterID;
    using range = juce::NormalisableRange<float>;
    
    layout.add(std::make_unique<juce::AudioParameterFloat> (pID{"gainIn", 1}, "Gain", range{-60.f, 12.f, 0.1f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (pID{"mix", 1}, "Mix", range{0.f, 1.f, 0.01f}, 1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (pID{"depth", 1}, "Lfo Depth", range{0.f, 100.f, 1.f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (pID{"rate", 1}, "Lfo Rate", range{0.02f, 5.f, 0.01, 0.3}, 1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (pID{"q", 1}, "Q", range{0.f, 1.f, 0.01f}, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (pID{"feedback", 1}, "Feedback", range{0.f, 100.f, 0.1f}, 0.f));
    layout.add(std::make_unique<juce::AudioParameterBool>  (pID{"phaseOffset", 1}, "Offset", false));

    return layout;
}

void ICMPphaserAudioProcessor::updatePhaser() {
    auto rate = treeState.getRawParameterValue("rate")->load();
    auto depth = treeState.getRawParameterValue("depth")->load();
    auto feedback = treeState.getRawParameterValue("feedback")->load();
    auto phaseOffset = treeState.getRawParameterValue("phaseOffset")->load();
    auto q = treeState.getRawParameterValue("q")->load();
    
    phaserL.setRate(rate);
    phaserL.setDepth(depth);
    phaserL.setFeedback(feedback);
    phaserL.setResonance(q);
    phaserR.setRate(rate);
    phaserR.setDepth(depth);
    phaserR.setFeedback(feedback);
    phaserR.setResonance(q);
    phaserR.setPhaseReversal(phaseOffset);
}

float ICMPphaserAudioProcessor::calculateRms(const int channel, juce::dsp::AudioBlock<float>& block) const
{
    jassert(block.getNumSamples() >= 0);    
    auto* data = block.getChannelPointer(channel);
    double sum = 0.f;
    
    for (int i = 0; i < block.getNumSamples(); ++i)
    {
        auto sample = data[i];
        sum += sample * sample;
    }
    return std::sqrt (sum / block.getNumSamples());
}

float ICMPphaserAudioProcessor::getRmsValue(const int channel)
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0) {
        return rmsL.getNextValue();
    }
    if (channel == 1) {
        return rmsR.getNextValue();
    }
    else return 0;
}
