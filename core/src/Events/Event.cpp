

#include <time.h>
#include <stdio.h>
#include <string.h>

import Aurion.Types;
import Aurion.Events;

namespace Aurion
{
	EventBase::EventBase()
	{
		this->timestamp = time(nullptr);
	}

	u16 EventBase::GetType() const
	{
		return this->type;
	}

	u16 EventBase::GetCategory() const
	{
		return this->category;
	}

	bool EventBase::Propagates() const
	{
		return this->propagates;
	}

	bool EventBase::IsHandled() const
	{
		return this->handled;
	}

	void EventBase::StopPropagation()
	{
		this->propagates = false;
	}

	void EventBase::Handle()
	{
		this->handled = true;
	}

	const time_t& EventBase::GetTimestamp() const
	{
		return this->timestamp;
	}
}