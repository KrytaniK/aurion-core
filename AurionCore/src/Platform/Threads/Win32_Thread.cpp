module;

#include <Aurion/Platform.h>

#if defined(AURION_PLATFORM_WINDOWS)


#include <stdexcept>

#endif

module Aurion.Multithreading;

#if defined(AURION_PLATFORM_WINDOWS)
namespace Aurion
{
  unsigned __stdcall Thread_Trampoline(void* thread)
  {
    Thread* t = static_cast<Thread*>(thread);

    // TODO: Wait until thread reaches 'Created' State

    // Start the thread routine
    t->Start();

    return 0;
  }

  Thread::Thread(ThreadRoutine routine, void* arg)
    : m_desc({}), m_status(ThreadStatus::Uninitialized)
  {
    if (!routine) return;

    // TODO: Create thread with Thread_Trampoline routine
  }

  Thread::Thread(Thread&& other)
  {
    if (this == &other)
      return;

    this->m_desc = other.m_desc;
    this->m_status = other.m_status;

    other.m_status = ThreadStatus::Uninitialized;
    other.m_desc.routine = nullptr;
    other.m_desc.arg = nullptr;
  }

  Thread& Thread::operator=(Thread&& other) noexcept
  {
    if (this == &other)
      return *this;

    this->m_desc = other.m_desc;
    this->m_status = other.m_status;

    other.m_status = ThreadStatus::Uninitialized;
    other.m_desc.routine = nullptr;
    other.m_desc.arg = nullptr;

    return *this;
  }

  Thread::~Thread()
  {
    if (m_status == ThreadStatus::Joined)
      return;

    Thread::Join();
  }

  void Thread::Start()
  {
    // TODO: Thread Status checks

    m_status = ThreadStatus::Running;
    m_desc.routine(m_desc.arg);
    m_status = ThreadStatus::Finished;
  }

  void Thread::Sleep(u32 ms)
  {
    m_status = ThreadStatus::Delayed;
    // TODO: Add sleep logic
    m_status = ThreadStatus::Running;
  }

  void Thread::Join()
  {
    // TODO: Win32 Thread Join
  }

  void* Thread::GetNativeHandle()  {
    // No native handle for uninitialized/joined threads
    if (m_status == ThreadStatus::Uninitialized || m_status == ThreadStatus::Joined)
      return nullptr;

    return &m_desc.handle;
  }

  const ThreadStatus& Thread::GetStatus()
  {
    return m_status;
  }
}
#endif
