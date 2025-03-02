

import Aurion.Window;

namespace Aurion
{
	WindowContext::WindowContext()
		: m_driver(nullptr)
	{

	}
	
	WindowContext::~WindowContext()
	{

	}

	void WindowContext::SetDriver(IWindowDriver* driver)
	{
		m_driver = driver;
	}

	IWindowDriver* WindowContext::GetDriver()
	{
		return m_driver;
	}

}