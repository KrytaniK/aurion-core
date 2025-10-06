module;

#include <macros/AurionExport.h>

#include <cstdint>
#include <time.h>

export module Aurion.Events:Event;

export namespace Aurion
{
	// Default Event Categories (use is optional)
	// Custom categories can be defined by the user
	typedef enum AURION_API EventCategories : uint16_t
	{
		AC_EVENT_CATEGORY_NONE = 0,
		AC_EVENT_CATEGORY_APPLICATION = 1 << 0,
		AC_EVENT_CATEGORY_WINDOW = 1 << 1,
		AC_EVENT_CATEGORY_FILE = 1 << 2,
		AC_EVENT_CATEGORY_PLUGIN = 1 << 3,
		AC_EVENT_CATEGORY_INPUT = 1 << 4,
		AC_EVENT_CATEGORY_LOG = 1 << 5,
	} EventCategories;

	// TODO: Use a date-time utility class instead of time_t.
	// Consider using 'time since app start' instead of system/clock time.
	// ALSO: timestamps need to be accurate, down to nanoseconds at best, milliseconds at worst.

	// Base Event Interface
	struct AURION_API IEvent
	{
		uint16_t category = AC_EVENT_CATEGORY_NONE;
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
}