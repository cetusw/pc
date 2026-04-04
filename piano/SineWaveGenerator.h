#pragma once
#include "lib/miniaudio.h"
#include <numbers>

class SineWaveGenerator
{
public:
	enum class State
	{
		PLAYING,
		RELEASING,
		OFF
	};

	SineWaveGenerator();
	SineWaveGenerator(
		ma_uint32 sampleRate,
		ma_float frequency,
		ma_float amplitude = 0.3f,
		ma_float startPhase = 0.f);

	ma_float GetNextSample();

	void Release();
	[[nodiscard]] bool IsActive() const;
	void Reset(ma_uint32 sampleRate, ma_float frequency, ma_float amplitude = 0.3f);

private:
	void HandleReleasing();
	ma_uint32 m_sampleRate;
	ma_float m_frequency;
	ma_float m_amplitude;
	ma_float m_amplitudePerSample;
	ma_float m_phase;
	ma_float m_phaseShift;
	State m_state;
};
