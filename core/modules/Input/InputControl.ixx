module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Input:Control;

import Aurion.Memory;
import Aurion.Events;

export namespace Aurion
{
	// Used to determine the state of bit-sized controls.
	typedef enum InputControlState : uint8_t
	{
		INPUT_STATE_IDLE = 0,
		INPUT_STATE_ACTIVE = 1,
		INPUT_STATE_HELD = 2,
	};

	struct AURION_API InputControlInfo
	{
		uint16_t byte_offset = 0; // The byte offset of this control in the device layout.
		InputControlState input_state = INPUT_STATE_IDLE;
		uint8_t input_code = 0; // The key/mouse/device code corresponding to the physical input device key/button/axis (Shift key, Left Mouse Button, Position, etc.)
		uint8_t bit = 0; // The bit that this control maps to, if applicable.
		uint8_t size_in_bits = 0; // Total size of this control, in bits.
		uint8_t size_in_bytes = 0; // Total size of this control, in bytes.
		uint8_t child_count = 0; // Total number of child controls.
	};

	class AURION_API InputControl
	{
	public:
		InputControl();
		InputControl(const InputControlInfo& info, uint8_t* parent_state);
		~InputControl();

		const bool IsValid() const;

		const InputControlInfo& GetInfo();

		const InputControlState& GetState();

		const uint8_t& GetInputCode();

		// Helper function to write to this control from an array
		bool SetFromArray(const int*& values, const size_t& count);
		bool SetFromArray(const float*& values, const size_t& count);

		// Type conversions to keep input values bound to default types
		operator bool() const;
		operator int() const;
		operator float() const;
		operator float*() const;

		// Assignment operator overloads for default types
		InputControl& operator=(bool value);
		InputControl& operator=(int value);
		InputControl& operator=(float value);

		// Array indexing operator overload to make retrieving child controls easy.
		const InputControl& operator[](size_t index) const;

	private:
		// Control specific data
		InputControlInfo m_info;
		uint8_t* m_state_block;
	};
	
}