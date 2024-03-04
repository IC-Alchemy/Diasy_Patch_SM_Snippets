### Quick Manual for DaisyPatchSM Audio Processing by GPT

#### Overview
 The code integrates various components such as an ADSR envelope, a looper, and a reverb effect.


#### Control Inputs
- **Control Voltages (CV)**: Several CV inputs are used to control various parameters:
  - `CV_1` and `CV_2` adjust the attack time and shape of the ADSR envelope.
  - `CV_3` sets the reverb time.
  - `CV_5` adjusts the release time of the ADSR envelope.
  - `ADC_11` and `ADC_12` control the dampening frequency of the reverb and the mix amount, respectively.
- **Gate_in_1**: Gate used to trigger the looper functionalities.
- **Gate_in_2**: Gate used to trigger the envelope functionalities.

- **Buttons**:
  - `button1` is used to trigger recording in the looper and, when held, to clear the loop.
  - `button2` switches through the looper's modes and toggles reverse playback when held.

This manual provides a concise overview of the system's capabilities and usage. For more detailed information on specific functions or components, refer to the respective documentation of the DaisyPatchSM and DaisySP libraries.
