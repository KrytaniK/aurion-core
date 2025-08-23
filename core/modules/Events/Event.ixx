module;

#include <macros/AurionExport.h>

#include <cstdint>
#include <time.h>

export module Aurion.Events:Event;

export namespace Aurion
{
	// Default Event Categories (use is optional)
	// Custom categories can be defined by the user
	enum EventCategories : uint16_t
	{
		None			= 0,
		Application		= 1 << 0,
		Window			= 1 << 1,
		File			= 1 << 2,
		Plugin			= 1 << 3,
		Input			= 1 << 4,
		Generic			= 1 << 5,
		Custom			= 1 << 6
	};

	// TODO: Use a date-time utility class instead of time_t.
	// Consider using 'time since app start' instead of system/clock time.
	// ALSO: timestamps need to be accurate, down to nanoseconds at best, milliseconds at worst.

	// Base Event Interface
	struct AURION_API IEvent
	{
		uint16_t category = EventCategories::None;
		uint16_t type = 0;

		time_t timestamp = 0;

		bool propagates = true;
		bool handled = false;
	};

	// Base Event Utility Class
	class AURION_API EventBase : protected IEvent
	{
	public:
		EventBase();
		~EventBase() = default;

		uint16_t GetType() const;
		uint16_t GetCategory() const;

		const time_t& GetTimestamp() const;

		bool Propagates() const;
		bool IsHandled() const;

		void StopPropagation();
		void Handle();
	};
	
	// Example using IEvent
	/*
		Note: Events defined this way will need their members to be manually initialized.
	
		enum FileEventTypes : uint16_t
		{
			FileOpen	= 1,
			FileClose	= 2,
			FileRead	= 3,
			FileWrite	= 4
		};

		// File Category Base Event
		struct AURION_API FileEvent : IEvent
		{
			FileEvent() { this->category = EventCategories::File; };
			FileEvent(const FileEventTypes& type) : FileEvent() { this->type = type; };
			const char* path = "";
			int file_descriptor = -1;
		};

		struct AURION_API FileOpenEvent : FileEvent
		{
			FileOpenEvent() : FileEvent(FileEventTypes::FileOpen) {};
			bool read_only = true;
		};
	
	*/

	// Example Using EventBase
	/*
		Note: Events defined this way will have their base members automatically initialized.
	
		enum WindowEventTypes : uint16_t
		{
			WindowClose		= 1,
			WindowResize	= 2,
			WindowFocus		= 3,
			WindowMoved		= 4
		};

		// Window Category Base Event
		struct AURION_API WindowEvent : EventBase
		{
			WindowEvent() { this->category = EventCategories::Window; };
			WindowEvent(const WindowEventTypes& type) : WindowEvent() { this->type = type; };
			int window_id = -1;
		};

		struct AURION_API WindowResizeEvent : WindowEvent
		{
			WindowResizeEvent() : WindowEvent(WindowEventTypes::WindowResize) {};
			int width = 0;
			int height = 0;
		};
	
	*/
}