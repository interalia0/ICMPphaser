/*
  ==============================================================================

    ThreeStagePhaser.h
    Created: 31 Mar 2024 9:33:07pm
    Author:  Elja Markkanen

  ==============================================================================
*/

#pragma once
#include "Phaser.h"

class ThreeStagePhaser : public Phaser {
    
public:
    ThreeStagePhaser(); 
    float processSample(float inputSample) override;
};

