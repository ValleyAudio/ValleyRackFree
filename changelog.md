# ValleyRackFree Change Log

## 2.0.3

    - [Fixed] Crash when Terrorform is loaded in a recent DAW project.
    - [Update] Replace raw text drawing with DigitalDisplay object in Terrorform.
    - [Added] Digital displays stay lit when 'Room brightness' is reduced.

## 2.0.2

    - [Fixed] Revert Plateau DSP to original V1 code.
    - [Added] Bypass mode to Plateau.
    - [Added] Compatibility for Terrorform in Cardinal (thanks Filipe).

## 2.0.1

    - [Fixed] Fonts are now loaded and cached during draw()

## 2.0.0
    
    - [Update] Overhauled codebase to work with Rack V2
    - [Fixed] Corrected text overwrite bug in TFormNumberField that would cause cursor to overshoot text size
    - [Update] Plateau now features a block processing mode to help with efficiency on older systems.
    - [Fixed] Corrected the output saturation and clipping modes on Plateau.
    - [Update] Updated graphics to v2 style 

## 1.1.1

    - [Update] Add the ability to load larger wavelength tables into Terrorform

## 1.1.0

    - [New Module] Terrorform, a shapeshifting, complex wavetable voice

## 0.6.9

    - [Fixed] Corrected a mistake where the modulation depth and shape input jacks were mixed up.

## 0.6.8

    - [Fixed] Corrected the dry gain staging mistake that 0.6.7 introduced. We never speak of 0.6.7 again....

## 0.6.7

    - [Fixed] "Diffuse input" bypass switch correctly taps from after the pre-delay

## 0.6.6

    - [Update] Plateau now features a CV input for Predelay. The sensitivity for this input is set in the context menu.

## 0.6.5

    - [Fix] Fixed stereo input bug on Plateau

## 0.6.4

    - [New Module] Plateau reverb! A plate reverb with a twist. Can add large expansive textures to your sounds, as well as be able to be tuned and excited at very short reverb times.
    - [Update] Performance optimisations to Topograph and µGraph.
    - [Update] All modules now have a dark jack look.

## 0.6.3

    - [Fixed] Dexter sample rate bug. If Dexter is inserted at a sample rate other than 44.1K, the tuning would be incorrect until the sample rate was changed.

## 0.6.2

### [New Module] µGraph

    - µGraph is a micro version of Topograph.

### Topograph

    - [Update] Fixed hanging gate issue when using external clock in gate mode.

### Dexter

    - [New] Full inversion mode for chords that span more than 1 octave. Notes can now be optionally inverted above the highest note rather than exactly 1 octave.

## 0.5.7

    - [Major Update] Warning, this update will break old patches. Open "Please READ ME first.pdf" for a guide on how to fix your old patches.

## 0.5.6

    - [Fixed] Dexter is now tuned to C not A. Please adjust Dexter to be in tune with your current patches.

## 0.5.5

Say hello to Dexter ;)

## 0.5.4

### Topograph

    - [New] CV trigger for Run
    - [New] Run can either be Toggled (default) or Momentary.
    - [New] Dynamic text that displays BPM and lengths for each channel in Euclidean mode.

## 0.5.3

### Topograph

    - [New] Swing!
    - [New] Alternate, light panel graphic. Accessible from the right click menu.
    - [Update] Graphical tweaks.

## 0.5.2

### Topograph

    - [New] Two new drums modes: Olivier and Euclidean. Current drum modes are Henri, Olivier and Euclidean.
    - [New] Three ext clock resolution modes. Module can respond to 4, 8 and 24 ppqn clocks.
    - [New] Gate modes. Toggle between 1ms pulses and 50% duty cycle gate mode on the trigger outputs.
    - [New] Accent out alt modes. Can toggle between independent accents or [Accent / Clock / Reset] modes.
    - [Update] Brighter button graphics.
    - [Update] Ext clock input trigger sensitivity improved. User complained that some modules could not trigger it with certain gate signals.

## 0.5.1

    - [Update] Fixed CV control.
