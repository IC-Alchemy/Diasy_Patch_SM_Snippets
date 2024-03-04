#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace patch_sm;
using namespace daisysp;

DaisyPatchSM hw;
Switch       button1;
Switch       button2;
Adsr         env1;

ReverbSc reverb;

#define kBuffSize 48000 * 60 // 60 seconds at 48kHz

// Loopers and the buffers they'll use
Looper              looper;
float DSY_SDRAM_BSS buffer_l[kBuffSize];

void EnvelopeCallback(uint16_t **output, size_t size)
{
    bool env_state = hw.gate_in_2.State();


    float knob_attack = hw.GetAdcValue(CV_1);
    float knob_shape  = hw.GetAdcValue(CV_2);

    env1.SetAttackTime(0.001 + (knob_attack / 6), 1.0 - knob_shape);


    float knob_decay = knob_shape;

    env1.SetDecayTime(knob_decay);
    env1.SetSustainLevel(0.9);

    float knob_release = hw.GetAdcValue(CV_5);
    env1.SetReleaseTime(knob_release);


    for(size_t i = 0; i < size; i++)
    {
        // convert to 12-bit integer (0-4095)
        uint16_t value = (env1.Process(env_state) * 4095.0);
        output[0][i]   = value;
        output[1][i]   = value;
    }
}
void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    hw.ProcessAnalogControls();

    button1.Debounce();
    button2.Debounce();

    /** The first control will set the reverb time */
    float rev_time = 0.3 + (0.67 * hw.GetAdcValue(CV_3));
    reverb.SetFeedback(rev_time);

    /** The second control will be the dampening frequency of the reverb
     *  There are several ways to get a good feeling knob for this, but
     *  a log-curve usually feels pretty good for frequency-based sweeps
     */
    const float kDampFreqMin = log(1200.f);
    const float kDampFreqMax = log(20000.f);
    float       damp_control = hw.GetAdcValue(ADC_11);

    // Calculate the damping frequency by using an exponential scale mapped from the
    // range defined by the minimum and maximum damping frequencies. This scales the
    // linear input from the ADC to an exponential frequency value.
    float damping
        = exp(kDampFreqMin + (damp_control * (kDampFreqMax - kDampFreqMin)));

    // Set the low-pass filter frequency of the reverb effect to the calculated damping value.
    reverb.SetLpFreq(damping);

    float sendamt = hw.GetAdcValue(ADC_12);


    bool state1 = hw.gate_in_1.Trig();

    if(state1
       || button1.RisingEdge()) //  trigger looper if button or gate goes high
        looper.TrigRecord();


    if(button1.TimeHeldMs() >= 1000.f) //  hold button one to clear loop
        looper.Clear();


    if(button2.RisingEdge()) //  press button2 to switch through the 4 modes
        looper.IncrementMode();
    else if(button2.TimeHeldMs() >= 1000.f) // hold button2 to toggle reverse
        looper.ToggleReverse();


    // Process audio
    for(size_t i = 0; i < size; i++)
    {
        // store the inputs * the input gain factor
        float in_l = IN_L[i];

        float sig_l = looper.Process(in_l) + in_l;

        float wetr;
        float dryl  = sig_l;
        float sendl = sig_l * sendamt;
        float wetl;

        reverb.Process(sendl, sendl, &wetl, &wetr);

        /** Add the dry and the wet together, and assign those to the output */
        OUT_L[i] = dryl + wetl;
    }
}

int main(void)
{
    // Initialize the hardware
    hw.Init();
    env1.Init(hw.AudioSampleRate());
    looper.Init(buffer_l, kBuffSize);
    reverb.Init(hw.AudioSampleRate());

    hw.StartDac(EnvelopeCallback);

    // Init the button
    button1.Init(hw.D1);
    button2.Init(hw.D10);


    // Start the audio callback
    hw.StartAudio(AudioCallback);

    // loop forever
    while(1) {}
}
