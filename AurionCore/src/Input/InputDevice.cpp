module Aurion.Input;

import Aurion.Types;

namespace Aurion
{
	InputDevice::InputDevice(const InputDeviceSpec& spec, const InputDeviceCapabilities& caps, const InputDeviceMemReqs& mem_reqs)
		: m_specs(spec), m_caps(caps), m_mem_reqs(mem_reqs), InputState(mem_reqs)
	{

	}

	const InputDeviceSpec& InputDevice::GetSpecification()
	{
		return m_specs;
	}

	const InputDeviceMemReqs& InputDevice::GetMemoryRequirements()
	{
		return m_mem_reqs;
	}

	const InputDeviceCapabilities& InputDevice::GetCapabilities()
	{
		return m_caps;
	}
}