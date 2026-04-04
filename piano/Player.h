#pragma once
#include "Device.h"
#include "lib/miniaudio.h"

#include <functional>

class Player
{
public:
	using DataCallback = std::function<void(void* output, ma_uint32 frameCount)>;

	Player(ma_format format, ma_uint32 channels, ma_uint32 sampleRate = 48000);

	void Start();
	void Stop();

	[[nodiscard]] ma_format GetFormat() const noexcept;
	[[nodiscard]] ma_uint32 GetChannels() const noexcept;
	[[nodiscard]] ma_uint32 GetSampleRate() const noexcept;

	void SetDataCallback(DataCallback callback);

private:
	static ma_device_config CreateConfig(ma_format format, ma_uint32 channels, ma_uint32 sampleRate);

	DataCallback m_dataCallback;
	Device m_device;
};
