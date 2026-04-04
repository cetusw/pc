#include "SineWaveGenerator.h"
#include <cmath>

SineWaveGenerator::SineWaveGenerator(
	const ma_uint32 sampleRate,
	const ma_float frequency,
	const ma_float amplitude,
	const ma_float startPhase)
	: m_sampleRate(sampleRate)
	, m_frequency(frequency)
	, m_amplitude(amplitude)
	, m_amplitudePerSample(0.0f)
	, m_phase(startPhase)
	, m_phaseShift(static_cast<ma_float>(2.f * std::numbers::pi * m_frequency / m_sampleRate))
	, m_state(State::PLAYING)
{
}

ma_float SineWaveGenerator::GetNextSample()
{
	if (m_state == State::OFF)
	{
		return 0.0f;
	}

	constexpr auto twoPi = static_cast<ma_float>(2.f * std::numbers::pi);

	const auto sample = m_amplitude * std::sin(m_phase);
	m_phase = std::fmod(m_phase + m_phaseShift, twoPi);

	if (m_state == State::RELEASING)
	{
		HandleReleasing();
	}

	return sample;
}

void SineWaveGenerator::Release()
{
	m_state = State::RELEASING;

	constexpr ma_float decayTime = 0.5f;
	const ma_float samplesCount = decayTime * static_cast<ma_float>(m_sampleRate);

	m_amplitudePerSample = m_amplitude / samplesCount;
}

bool SineWaveGenerator::IsActive() const
{
	return m_state != State::OFF;
}

void SineWaveGenerator::HandleReleasing()
{
	m_amplitude -= m_amplitudePerSample;
	if (m_amplitude <= 0.0f)
	{
		m_amplitude = 0.0f;
		m_state = State::OFF;
	}
}
