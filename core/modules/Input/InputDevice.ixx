module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Input:Device;

import :Layout;
import :Control;

export namespace Aurion
{
	struct AURION_API InputDeviceInfo
	{
		const char* product_name; // Name of the product
		const char* product_manufacturer; // Name of the manufacturer
		const char* product_version; // Product version, as a string
		const char* product_interface; // Name of the interface making the device available (such as HID).
		const char* product_ext; // Extension string for interface-specific device capabilities (such as HID information)
		uint64_t classification; // Classification Identifier, as an unsigned 64-bit integer (such as gamepad, keyboard, etc.)
	};

	// An input device is a logical mapping to a physical device (such as a mouse or keyboard)
	class AURION_API InputDevice
	{
	public:
		InputDevice() = default;
		InputDevice(const InputDeviceInfo& info, const InputLayout& layout);
		~InputDevice() = default;

		const InputDeviceInfo& GetInfo();

		InputControl* GetControl(const uint16_t& input_code);
		
	private:
		InputDeviceInfo m_info;
		void* m_state;
		size_t m_control_count;
		InputControl* m_controls;
	};
}