#include "Piano.h"
#include <cmath>
#include <memory>

Piano::Piano(const ma_uint32 sampleRate)
	: m_sampleRate(sampleRate)
{
	m_voiceNoteMap.fill(-1);
}

void Piano::NoteOn(const int noteIndex)
{
	std::lock_guard lock(m_mutex);
	for (int i = 0; i < MAX_VOICES; ++i)
	{
		if (m_voiceNoteMap[i] == noteIndex && m_voices[i].IsActive())
		{
			return;
		}
		if (m_voiceNoteMap[i] == -1 || !m_voices[i].IsActive())
		{
			m_voiceNoteMap[i] = -1;
			const ma_float frequency = 440.0f * std::pow(2.0f, static_cast<float>(noteIndex) / 12.0f);
			m_voices[i].Reset(m_sampleRate, frequency);
			m_voiceNoteMap[i] = noteIndex;
			return;
		}
	}
}

void Piano::NoteOff(const int noteIndex)
{
	std::lock_guard lock(m_mutex);
	for (int i = 0; i < MAX_VOICES; ++i)
	{
		if (m_voiceNoteMap[i] == noteIndex)
		{
			m_voices[i].Release();
		}
	}
}

void Piano::Mix(float* output, const ma_uint32 frameCount)
{
	std::lock_guard lock(m_mutex);
	std::fill_n(output, frameCount, 0.0f);

	for (ma_uint32 i = 0; i < frameCount; ++i)
	{
		float sum = 0.0f;
		for (int v = 0; v < MAX_VOICES; ++v)
		{
			if (m_voiceNoteMap[v] != -1)
			{
				sum += m_voices[v].GetNextSample();
				if (!m_voices[v].IsActive())
				{
					m_voiceNoteMap[v] = -1;
				}
			}
		}
		output[i] = std::tanh(sum);
	}
}
