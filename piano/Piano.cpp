#include "Piano.h"
#include <cmath>
#include <memory>

Piano::Piano(const ma_uint32 sampleRate)
	: m_sampleRate(sampleRate)
{
}

void Piano::NoteOn(const int noteIndex)
{
	std::lock_guard lock(m_mutex);
	const ma_float frequency = 440.0f * std::pow(2.0f, static_cast<float>(noteIndex) / 12.0f);
	m_activeVoices[noteIndex] = std::make_unique<SineWaveGenerator>(m_sampleRate, frequency);
}

void Piano::NoteOff(const int noteIndex)
{
	std::lock_guard lock(m_mutex);
	if (m_activeVoices.contains(noteIndex))
	{
		m_activeVoices[noteIndex]->Release();
	}
}

void Piano::Mix(float* output, const ma_uint32 frameCount)
{
	std::lock_guard lock(m_mutex);
	for (ma_uint32 i = 0; i < frameCount; ++i)
	{
		float sum = 0;
		for (auto it = m_activeVoices.begin(); it != m_activeVoices.end();)
		{
			if (!it->second->IsActive())
			{
				it = m_activeVoices.erase(it);
				continue;
			}
			sum += it->second->GetNextSample();
			++it;
		}
		output[i] = std::tanh(sum);
	}
}
