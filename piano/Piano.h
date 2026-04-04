#pragma once
#include "SineWaveGenerator.h"

#include <array>
#include <map>
#include <memory>

constexpr int MAX_VOICES = 16;

class Piano
{
public:
	explicit Piano(ma_uint32 sampleRate);

	void NoteOn(int noteIndex);
	void NoteOff(int noteIndex);
	void Mix(float* output, ma_uint32 frameCount);

private:
	ma_uint32 m_sampleRate;
	std::array<SineWaveGenerator, MAX_VOICES> m_voices;
	std::array<int, MAX_VOICES> m_voiceNoteMap;
	std::mutex m_mutex;
};
