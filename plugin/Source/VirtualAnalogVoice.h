#pragma once

#include <JuceHeader.h>
#include "Cfg.h"

class VirtualAnalogAudioProcessor;

//==============================================================================
class VirtualAnalogVoice : public gin::SynthesiserVoice,
                           public gin::ModVoice
{
public:
    VirtualAnalogVoice (VirtualAnalogAudioProcessor& p, gin::BandLimitedLookupTables& bandLimitedLookupTables);
    
    void noteStarted() override;
    void noteRetriggered() override;
    void noteStopped (bool allowTailOff) override;

    void notePressureChanged() override;
    void noteTimbreChanged() override;
    void notePitchbendChanged() override    {}
    void noteKeyStateChanged() override     {}
    
    void setCurrentSampleRate (double newRate) override;

    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    bool isVoiceActive() override;

    float getFilterCutoffNormalized (int idx);

private:
    void updateParams (int blockSize);

    VirtualAnalogAudioProcessor& proc;
    gin::BandLimitedLookupTables& bandLimitedLookupTables;

    gin::VoicedStereoOscillator oscillators[Cfg::numOSCs] =
    {
        bandLimitedLookupTables,
        bandLimitedLookupTables,
        bandLimitedLookupTables,
        bandLimitedLookupTables,
    };

    gin::Filter filters[Cfg::numFilters];
    gin::ADSR filterADSRs[Cfg::numFilters];
    
    gin::ADSR modADSRs[Cfg::numENVs];
    gin::LFO modLFOs[Cfg::numLFOs];
    gin::StepLFO modStepLFO;

    gin::AnalogADSR adsr;

    float currentMidiNotes[Cfg::numOSCs];
    gin::VoicedStereoOscillator::Params oscParams[Cfg::numOSCs];
    
    gin::EasedValueSmoother<float> noteSmoother;
    
    float ampKeyTrack = 1.0f;
};
