export module Aurion.Input:Event;

import <macros/AurionExport.h>;

import Aurion.Types;
import Aurion.Events;

export namespace Aurion
{
	typedef enum AURION_API InputEventTypes : u16
	{
		AC_INPUT_EVENT_NONE					= 0,
		AC_INPUT_EVENT_TOUCH				= 1 << 0, // Touch input event (for touchscreens, touchpads, etc.)
		AC_INPUT_EVENT_VALUE				= 1 << 1, // 1-dimensional input value (for sliders, triggers, etc.)
		AC_INPUT_EVENT_BUTTON				= 1 << 2, // Generic button/key event (for gamepads, keyboards, etc.)
		AC_INPUT_EVENT_AXIS					= 1 << 3, // Generic axis movement (for joysticks, mice, etc.)
		AC_INPUT_EVENT_AXIS_2D				= 1 << 4, // 2-dimensional axis movement (for dual-axis joysticks, touchpads, etc.)
		AC_INPUT_EVENT_AXIS_3D				= 1 << 5, // 3-dimensional axis movement (for 3D controllers, motion sensors, etc.)
		AC_INPUT_EVENT_AXIS_4D				= 1 << 6, // 4-dimensional axis movement (for advanced 3D controllers, etc.)
	} InputEventTypes;

	typedef enum AURION_API InputButtonStates : u8
	{
		AC_BUTTON_STATE_NONE		= 0,
		AC_BUTTON_STATE_PRESSED		= 1,
		AC_BUTTON_STATE_RELEASED	= 2,
		AC_BUTTON_STATE_REPEATED	= 3,
	} InputButtonStates;

	struct AURION_API InputEvent : public EventBase
	{
		InputEvent() { this->category = AC_EVENT_CATEGORY_INPUT; };
		InputEvent(const InputEventTypes& type) : InputEvent() { this->type = type; };

		u64 context_id = 0; // Context ID (could be a window, application, etc.)
		u64 device_id = 0; // Device ID (which device generated the event)
		u64 control_id = 0; // ID of the control (which control on the device)
	};

	struct AURION_API InputTouchEvent : public InputEvent
	{
		InputTouchEvent() : InputEvent(AC_INPUT_EVENT_TOUCH) {};

		u32 touch_id; // Unique ID for the touch point (for multi-touch support)
		double x = 0.0; // X position of the touch point
		double y = 0.0; // Y position of the touch point
		float pressure = 0.0f; // Pressure of the touch (if supported)
	};

	struct AURION_API InputValueEvent : public InputEvent
	{
		InputValueEvent() : InputEvent(AC_INPUT_EVENT_VALUE) {};

		float value = 0.0f; // Value of the control (normalized between 0.0 and 1.0)
	};

	struct AURION_API InputButtonEvent : public InputEvent
	{
		InputButtonEvent() : InputEvent(AC_INPUT_EVENT_BUTTON) {};

		u64 button = 0; // Button code (could be a keycode, mouse button, gamepad button, etc.)
		u64 scan_code = 0; // Physical scan code (if applicable)
		InputButtonStates state = AC_BUTTON_STATE_NONE; // State of the button (pressed, released, repeated)
	};

	struct AURION_API InputAxisEvent : public InputEvent
	{
		InputAxisEvent() : InputEvent(AC_INPUT_EVENT_AXIS) {};
		float value = 0.0f; // Value of the axis movement (normalized between -1.0 and 1.0)
	};

	struct AURION_API InputAxis2DEvent : public InputEvent
	{
		InputAxis2DEvent() : InputEvent(AC_INPUT_EVENT_AXIS_2D) {};

		float value_x = 0.0f; // Value of the X axis movement (normalized between -1.0 and 1.0)
		float value_y = 0.0f; // Value of the Y axis movement (normalized between -1.0 and 1.0)
	};

	struct AURION_API InputAxis3DEvent : public InputEvent
	{
		InputAxis3DEvent() : InputEvent(AC_INPUT_EVENT_AXIS_3D) {};

		float value_x = 0.0f; // Value of the X axis movement (normalized between -1.0 and 1.0)
		float value_y = 0.0f; // Value of the Y axis movement (normalized between -1.0 and 1.0)
		float value_z = 0.0f; // Value of the Z axis movement (normalized between -1.0 and 1.0)
	};

	struct AURION_API InputAxis4DEvent : public InputEvent
	{
		InputAxis4DEvent() : InputEvent(AC_INPUT_EVENT_AXIS_4D) {};

		float value_x = 0.0f; // Value of the X axis movement (normalized between -1.0 and 1.0)
		float value_y = 0.0f; // Value of the Y axis movement (normalized between -1.0 and 1.0)
		float value_z = 0.0f; // Value of the Z axis movement (normalized between -1.0 and 1.0)
		float value_w = 0.0f; // Value of the W axis movement (normalized between -1.0 and 1.0)
	};
}