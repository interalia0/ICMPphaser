/*
  ==============================================================================

    SixStagePhaser.h
    Created: 31 Mar 2024 9:01:59pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include "Phaser.h"

class SixStagePhaser : public Phaser {

public:
    SixStagePhaser();
    float processSample(float inputSample) override;
};
