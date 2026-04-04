#pragma once
#include "SineWaveGenerator.h"
#include <map>
#include <memory>

class Piano
{
public:
	explicit Piano(ma_uint32 sampleRate);

	void NoteOn(int noteIndex);
	void NoteOff(int noteIndex);
	void Mix(float* output, ma_uint32 frameCount);

private:
	ma_uint32 m_sampleRate;
	std::map<int, std::unique_ptr<SineWaveGenerator>> m_activeVoices;
	std::mutex m_mutex;
};
