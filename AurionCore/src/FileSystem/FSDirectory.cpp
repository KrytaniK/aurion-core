module;

module Aurion.FileSystem;

namespace Aurion
{
    FSDirectory::FSDirectory(const char* path)
        : FSEntry(path)
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
        // Close the directory handle (if open)
        m_impl->Close();
        delete m_impl;
    }

    const FSMetadata& FSDirectory::GetMetadata(bool follow_links)
    {
        return m_impl->GetMetadata(GetPath(), follow_links);
    }

    void FSDirectory::Open(FSFlags flags)
    {
        m_impl->Open(GetPath(), flags);
    }

    void FSDirectory::Close()
    {
        m_impl->Close();
    }

    bool FSDirectory::Exists()
    {
        return m_impl->Exists(GetPath());
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

    void FSDirectory::List(FSEntry** entries, u64& count, const FSFlags& flags)
    {
        m_impl->List(GetPath(), entries, count, flags);
    }
}
