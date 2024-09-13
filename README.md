# audio programming project

## references

[demo video on YouTube](https://www.youtube.com/watch?v=m27jSpFIRqA)

### Features of [Rings resonator](https://pichenettes.github.io/mutable-instruments-documentation/modules/rings/) (Three resonator models)
1. **Modal resonator** (features are below)
2. **Sympathetic strings**, modelled by a network of comb filters.
3. **String with non-linearity/dispersion** (comb filter with multimode filter and non-linearities in the loop).

#### (main)[Modal resonator](https://pichenettes.github.io/mutable-instruments-documentation/modules/elements/#modal-resonator)
- Internally uses 64 zero-delay feedback state variable filters.
- **Coarse**, fine and FM frequency controls.
- **Geometry**: Interpolates through a collection of structures, including plates, strings, tubes, bowls.
- **Brightness**. Specifies the character of the material the resonating structure is made of – from wood to glass, from nylon to steel.
- **Damping**. Adds damping to the sound – simulates a wet material or the muting of the vibrations.
- **Position**. Specifies at which point the structure is excited.
- **Space**. Creates an increasingly rich stereo output by capturing the sound at two different points of the structure, and then adds more space through algorithmic reverberation.

---
<!-- 
explaination of files in original repo that we need to use (gpt generated)
### Summary of `voice.cc` and `voice.h`(elements/dsp/)

The files `voice.cc` and `voice.h` implement a modal synthesis voice, which is a form of sound synthesis used to simulate the resonances of physical systems, such as musical instruments. The synthesis involves exciters like bow, blow, and strike exciters, which are modulated by different parameters, and a resonator to process the sound.

#### 1. **`voice.h` (Header File)**
   - This file defines the `Voice` class and its components.
   - Key elements include:
     - **Exciters**: The synthesis uses three types of exciters—bow, blow, and strike—to simulate different types of excitation methods for resonating bodies (e.g., bowing a string, blowing through a tube, or striking a surface).
     - **Resonator Models**: Three different resonator models are available:
       1. `RESONATOR_MODEL_MODAL`
       2. `RESONATOR_MODEL_STRING`
       3. `RESONATOR_MODEL_STRINGS`
     - **Components of the Voice**: 
       - `MultistageEnvelope`: An ADSR envelope for controlling amplitude over time.
       - `Resonator`: Processes sound and produces the final output.
       - `Tube`: Simulates tube-like resonators.
       - `String`: Simulates string-like resonators.
       - `Diffuser`: Adds spatial diffusion to the signal.
     - **Methods**:
       - `Init()`: Initializes the voice components.
       - `Process()`: Processes the sound input, applying the exciters and resonators to produce output.
       - `ResetResonator()`: Resets the resonator state.
       - `GetGateFlags()`: Manages gate inputs for triggering events.

#### 2. **`voice.cc` (Source File)**
   - Implements the behavior and processing logic of the `Voice` class.
   - **Initialization**:
     - The `Init()` method initializes various components like the envelope, bow, blow, strike, diffuser, and resonator. It also sets the default parameters for the exciters and the resonator.
   - **Exciter Configuration**:
     - Different exciter models and parameters (e.g., timbre, strength) are configured for the bow, blow, and strike exciters based on the current patch (user-defined parameters).
     - The envelope shape is dynamically adjusted depending on the patch settings.
   - **Resonator Processing**:
     - The resonator is set up based on the selected model (`RESONATOR_MODEL_MODAL` or `RESONATOR_MODEL_STRING`), and sound input is processed through the resonator to generate the final output.
     - The resonator modulates sound characteristics like brightness, damping, and frequency.
   - **Chords**:
     - Predefined chord intervals are used for tuning the strings, allowing for polyphonic sound generation.
   - **Strength and Zipper Noise Reduction**:
     - The strength of excitation is interpolated to reduce zipper noise, a common artifact in digital audio processing.
   - **Sound Blending**:
     - The exciter outputs (bow, blow, strike) are combined with user inputs and modulated over time to generate the final sound output. 

### Key Concepts
   - **Exciters**: These simulate different methods of generating sound (bowing, blowing, or striking an instrument).
   - **Resonators**: Act like physical bodies that respond to the exciters and modify the sound accordingly.
   - **Modal Synthesis**: A technique where a physical model is used to simulate the vibrations of objects, often used for simulating acoustic instruments.
   - **Chords**: Predefined sets of intervals for generating harmonic content in polyphonic mode.

These files are part of a real-time audio processing system, likely for a synthesizer, and focus on modeling realistic instrument sounds through complex sound synthesis techniques. -->

--- 

(add more about how to use the dsp classes in rings/dsp, which is the modal resonator part)


TODO
- midi handler
- stereo
- freqMod
- GUI
- polyphony ? 
