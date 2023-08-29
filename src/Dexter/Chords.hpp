//
// Chords.hpp
// Author: Dale Johnson
// Contact: valley.audio.soft@gmail.com
// Date: 18/3/2018
//
// Credit to Maddie to helping me with the octave jump code
// for calculating new inversion
//

#pragma once
#include <string>
#include <vector>

const int kMaxChordSize = 8;

using Notes = std::vector<float>;

class Chord {
public:
    Chord(const std::string& name = "", const Notes& notes = {});

    std::size_t size() const;
    Notes getNotes() const;
    std::string getName() const;

private:
    std::string name_;
    std::vector<float> notes_;
    float detuneAmount_ = 0.f;
};

enum Chords {
    CHORD_ONE_NOTE,
    CHORD_MINOR_2ND,
    CHORD_MAJOR_2ND,
    CHORD_MINOR_3RD,
    CHORD_MAJOR_3RD,
    CHORD_4TH,
    CHORD_TRITONE,
    CHORD_5TH,
    CHORD_AUG_5TH,
    CHORD_6TH,
    CHORD_MINOR_7TH_INT,
    CHORD_MAJOR_7TH_INT,
    CHORD_OCTAVE,
    CHORD_SUB_OCTAVE,
    CHORD_OCT_2OCT,
    CHORD_MINOR_TRIAD,
    CHORD_MAJOR_TRIAD,
    CHORD_SUS_TRIAD,
    CHORD_AUG_TRIAD,
    CHORD_DIM_TRIAD,
    CHORD_MAJOR_6TH,
    CHORD_MAJOR_7TH,
    CHORD_DOM_7TH,
    CHORD_MINOR_7TH,
    CHORD_HALFDIM_7TH,
    CHORD_DIM_7TH,
    CHORD_SUS_7TH,
    CHORD_DOM_9TH,
    CHORD_DOM_MIN_9TH,
    CHORD_MAJOR_9TH,
    CHORD_MINOR_9TH,
    CHORD_MAJOR_6_9,
    CHORD_MINOR_6_9,
    CHORD_9TH_FLAT_5TH,
    CHORD_9TH_SHARP_5TH,
    CHORD_DOM_11TH,
    CHORD_MINOR_11TH,
    CHORD_UNISON4,
    CHORD_UNISON8,
    NUM_CHORDS
};

const Chord chord_oneNote     ("Single", {0});
const Chord chord_minor2nd    ("Min 2", {0, 1});
const Chord chord_major2nd    ("Maj 2", {0, 2});
const Chord chord_minor3rd    ("Min 3", {0, 3});
const Chord chord_major3rd    ("Maj 3", {0, 4});
const Chord chord_4th         ("4th", {0, 5});
const Chord chord_tritone     ("Tritone", {0, 6});
const Chord chord_5th         ("5th", {0, 7});
const Chord chord_aug5th      ("Aug5 Int", {0, 8});
const Chord chord_6th         ("6th", {0, 9});
const Chord chord_minor7thInt ("Min7 Int", {0, 10});
const Chord chord_major7thInt ("Maj7 Int", {0, 11});
const Chord chord_octave      ("Octave", {0, 12});
const Chord chord_subOctave   ("SubOct", {0, -12});
const Chord chord_oct2Octave  ("2 Oct", {0, 12, 24});
const Chord chord_minorTriad  ("MinTriad", {0, 3, 7});
const Chord chord_majorTriad  ("MajTriad", {0, 4, 7});
const Chord chord_susTriad    ("SusTriad", {0, 5, 7});
const Chord chord_augTriad    ("AugTriad", {0, 4, 8});
const Chord chord_dimTriad    ("DimTriad", {0, 3, 6});
const Chord chord_major6th    ("Maj 6th", {0, 4, 7, 9});
const Chord chord_major7th    ("Maj 7th", {0, 4, 7, 11});
const Chord chord_dom7th      ("Dom 7th", {0, 4, 7, 10});
const Chord chord_minor7th    ("Min 7th", {0, 3, 7, 10});
const Chord chord_halfDim7th  ("HalfDim7", {0, 3, 6, 10});
const Chord chord_dim7th      ("Dim 7th", {0, 3, 6, 9});
const Chord chord_sus7th      ("Sus 7th", {0, 5, 7, 10});
const Chord chord_dom9th      ("Dom 9th", {0, 4, 7, 10, 14});
const Chord chord_domMin9th   ("Dom m9", {0, 4, 7, 10, 13});
const Chord chord_major9th    ("Maj 9th", {0, 4, 7, 11, 14});
const Chord chord_minor9th    ("Min 9th", {0, 3, 7, 10, 14});
const Chord chord_major69     ("Maj6th9", {0, 4, 7, 9, 14});
const Chord chord_minor69     ("m6th9", {0, 3, 7, 9, 14});
const Chord chord_9thFlat5th  ("9b5th", {0, 4, 6, 10, 14});
const Chord chord_9thSharp5th ("9#5th", {0, 4, 8, 10, 14});
const Chord chord_dom11th     ("Dom 11th", {0, 7, 10, 14, 18});
const Chord chord_minor11th   ("Min 11th", {0, 3, 7, 10, 14, 17});
const Chord chord_unison4     ("Uni 5", {0, 0, 0, 0, 0});
const Chord chord_unison8     ("Uni 7", {0, 0, 0, 0, 0, 0, 0});

static const Chord chords[NUM_CHORDS] = {
    chord_oneNote, chord_minor2nd, chord_major2nd, chord_minor3rd,
    chord_major3rd, chord_4th, chord_tritone, chord_5th, chord_aug5th,
    chord_6th, chord_minor7thInt, chord_major7thInt, chord_octave,
    chord_subOctave, chord_oct2Octave, chord_minorTriad, chord_majorTriad,
    chord_susTriad, chord_augTriad, chord_dimTriad, chord_major6th,
    chord_major7th, chord_dom7th, chord_minor7th, chord_halfDim7th,
    chord_dim7th, chord_sus7th, chord_dom9th, chord_domMin9th, chord_major9th,
    chord_minor9th, chord_major69, chord_minor69, chord_9thFlat5th,
    chord_9thSharp5th, chord_dom11th, chord_minor11th, chord_unison4,
    chord_unison8
};

void invertNotes(Notes& notes,
                int inversions,
                bool fullInversion = false);

void detuneNotes(Notes& notes, float detuneAmount);

