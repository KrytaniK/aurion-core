module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Window:Events;

import Aurion.Events;

export namespace Aurion
{

	typedef enum AURION_API WindowEventTypes : uint16_t
	{
		AC_WIN_EVENT_CREATE		= 1 << 0,
		AC_WIN_EVENT_CLOSE		= 1 << 1,
		AC_WIN_EVENT_RESIZE		= 1 << 2,
		AC_WIN_EVENT_FOCUS		= 1 << 3,
		AC_WIN_EVENT_MOVE		= 1 << 4,
	} WindowEventTypes;

	// Window Category Base Event
	struct AURION_API WindowEvent : EventBase
	{
		WindowEvent() { this->category = AC_EVENT_CATEGORY_WINDOW; };
		WindowEvent(const WindowEventTypes& type) : WindowEvent() { this->type = type; };
		int window_id = -1;
	};

	struct AURION_API WindowCreateEvent : WindowEvent
	{
		WindowCreateEvent() : WindowEvent(AC_WIN_EVENT_CREATE) {};
	};

	struct AURION_API WindowCloseEvent : WindowEvent
	{
		WindowCloseEvent() : WindowEvent(AC_WIN_EVENT_CLOSE) {};
	};

	struct AURION_API WindowResizeEvent : WindowEvent
	{
		WindowResizeEvent() : WindowEvent(AC_WIN_EVENT_RESIZE) {};
		uint16_t width = 0;
		uint16_t height = 0;
	};

	struct AURION_API WindowFocusEvent : WindowEvent
	{
		WindowFocusEvent() : WindowEvent(AC_WIN_EVENT_FOCUS) {};
		bool focused = false;
	};

	struct AURION_API WindowMoveEvent : WindowEvent
	{
		WindowMoveEvent() : WindowEvent(AC_WIN_EVENT_MOVE) {};
		uint16_t x = 0;
		uint16_t y = 0;
	};
}