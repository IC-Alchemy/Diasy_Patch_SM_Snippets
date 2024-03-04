# Diasy_Patch_SM_Snippets
Collection of code and some compiled binaries for Patch_SM

## LoopVerblope
- - Looper with reverb, and a seperate ASDR envelope with CV out because why not with a Daisy?
- Loop can be triggered from the gate input for perfectly timed loops, or from a button
- CV control over the release time of envelope sounds great
#### Control Inputs
- **Control Voltages (CV)**: Several CV inputs are used to control various parameters:
  - `CV_1` and `CV_2` adjust the attack time and shape of the ADSR envelope.
  - `CV_3` sets the reverb time.
  - `CV_5` adjusts the release time of the ADSR envelope.
  - `ADC_11` and `ADC_12` control the dampening frequency of the reverb and the mix amount, respectively.
- **Gate Inputs**: Gate inputs are used to trigger the envelope and looper functionalities.
- **Buttons**:
  - `button1` is used to trigger recording in the looper and, when held, to clear the loop.
  - `button2` switches through the looper's modes and toggles reverse playback when held.
