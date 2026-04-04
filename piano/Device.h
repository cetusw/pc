#pragma once
#include "lib/miniaudio.h"
#include <functional>
#include <system_error>

inline const std::error_category& ErrorCategory()
{
	class AudioErrorCategory : public std::error_category
	{
	public:
		[[nodiscard]] const char* name() const noexcept override
		{
			return "Audio error category";
		}

		[[nodiscard]] std::string message(int errorCode) const override
		{
			return ma_result_description(static_cast<ma_result>(errorCode));
		}
	};

	static AudioErrorCategory errorCategory;
	return errorCategory;
}

class Device
{
public:
	using DataCallback = std::function<void(void*, const void*, ma_uint32)>;

	explicit Device(ma_device_config config);

	void Start();
	void Stop();

	ma_device* operator->() noexcept;
	const ma_device* operator->() const noexcept;

	void SetDataCallback(DataCallback dataCallback);

	Device(const Device&) = delete;
	Device& operator=(const Device&) = delete;

	~Device();

private:
	void OnDataCallback(void* output, const void* input, ma_uint32 frameCount) const;

	ma_device m_device{};
	DataCallback m_dataCallback;
};
