#include "Chords.hpp"
#include <cmath>

Chord::Chord(const std::string& name, const Notes& notes)
    : name_(name), notes_(notes)
{ }

std::size_t Chord::size() const { return notes_.size(); }

Notes Chord::getNotes() const { return notes_; }

std::string Chord::getName() const { return name_; }

void invertNotes(Notes& notes,
                 int inversions,
                 bool fullInversion)
{
    if (inversions == 0 || notes.size() < 2)
        return;

    int globalOctave = 0;
    int localOctave = 1;
    float direction = inversions > 0.f ? 1.f : -1.f;
    auto lastNote = notes.back();

    std::size_t shifts = std::abs(inversions) % notes.size();
    globalOctave = (int)((float)inversions / (float)notes.size());
    if (fullInversion) {
        globalOctave *= 1 + (int)(lastNote / 12.f);
        localOctave = 1 + (int)(lastNote / 12.f);
    }

    for (std::size_t i = 0; i < shifts; ++i)
        notes[i] += 12.f * direction * (float)localOctave;

    for (auto& n : notes)
        n += 12 * (float)globalOctave;
}

void detuneNotes(Notes& notes, float detuneAmount) {
    if (notes.size() <= 1)
        return;

    const float detuneTable[8] = {24.f, -24.f, 18.f, -18.f,
                                  12.f, -12.f, 6.f, -6.f};

    for (std::size_t i = 0; i < notes.size(); ++i) {
        float detuneLimit = detuneTable[i];
        notes[i] += detuneLimit * detuneAmount;
    }
}

