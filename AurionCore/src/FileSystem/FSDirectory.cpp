module Aurion.FileSystem;

namespace Aurion
{
    FSDirectory::FSDirectory(const char* path)
        : FSEntry(path), m_impl(nullptr)
    {
#ifdef AURION_PLATFORM_WINDOWS
        m_impl = new FSDirectory_WinImpl();
#elifdef AURION_PLATFORM_LINUX
        m_impl = new FSDirectory_LinuxImpl();
#else
        m_impl = nullptr;
#endif
    }

    FSDirectory::~FSDirectory()
    {
        if (!m_impl) return;

        // Close the directory handle (if open)
        m_impl->Close();
        delete m_impl;
    }

    FSDirectory::FSDirectory(FSDirectory&& other) noexcept
        : FSEntry(static_cast<FSEntry&&>(other))
    {
        m_impl = other.m_impl;
        other.m_impl = nullptr;
    }

    FSDirectory& FSDirectory::operator=(FSDirectory&& other) noexcept
    {
        FSEntry::operator=(static_cast<FSEntry&&>(other));
        if (this == &other) return *this;

        if (m_impl) delete m_impl;
        m_impl = other.m_impl;
        other.m_impl = nullptr;

        return *this;
    }

    const FSMetadata& FSDirectory::GetMetadata(bool follow_links)
    {
        return m_impl->GetMetadata(m_path, follow_links);
    }

    void FSDirectory::Open(u32 flags, u32 access)
    {
        m_impl->Open(m_path, static_cast<int>(flags), static_cast<int>(access));
    }

    void FSDirectory::Close()
    {
        m_impl->Close();
    }

    bool FSDirectory::Delete()
    {
        return m_impl->Delete(m_path);
    }

    bool FSDirectory::DeleteAll()
    {
        return m_impl->DeleteAll(m_path);
    }

    bool FSDirectory::Exists()
    {
        return m_impl->Exists(m_path);
    }

    bool FSDirectory::IsOpen()
    {
#ifdef AURION_PLATFORM_WINDOWS
        return m_impl->GetDescriptor().handle != nullptr;
#elifdef AURION_PLATFORM_LINUX
        return m_impl->GetDescriptor().handle >= 0;
#else
        return false;
#endif
    }

    void FSDirectory::List(FSCollection* entries, u64& count)
    {
        m_impl->List(m_path, entries, count);
    }
}
