/*
  ==============================================================================

    HorizontalMeter.h
    Created: 27 Mar 2024 9:46:14am
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/
class HorizontalMeter  : public Component
{
public:
    HorizontalMeter();
    ~HorizontalMeter() override;

    void paint (Graphics&) override;
    void resized() override;
    void setLevel(const float value);

private:
    float level = -60.f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HorizontalMeter)
};
