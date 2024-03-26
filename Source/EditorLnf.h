/*
  ==============================================================================

    EditorLnf.h
    Created: 26 Mar 2024 8:57:19pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../Source/FuturaMedium.h"
#include "MyColours.h"

class EditorLnf : public juce::LookAndFeel_V4
{
public:
    EditorLnf();

    void drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;
};
