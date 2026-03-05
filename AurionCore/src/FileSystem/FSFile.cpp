module;

#include <unistd.h>

module Aurion.FileSystem;

namespace Aurion
{
    FSFile::FSFile(const char* path)
        : FSEntry(path)
    {
#ifdef AURION_PLATFORM_WINDOWS
        m_impl = new FSFile_WinImpl();
#elifdef AURION_PLATFORM_LINUX
        m_impl = new FSFile_LinuxImpl();
#else
        m_impl = nullptr;
#endif
    }

    FSFile::~FSFile()
    {
        // Close the file (if open)
        m_impl->Close();
        delete m_impl;
    }

    const FSMetadata& FSFile::GetMetadata(bool follow_links)
    {
        return m_impl->GetMetadata(GetPath(), follow_links);
    }

    void FSFile::Open(FSFlags flags)
    {
        m_impl->Open(GetPath(), flags);
    }

    void FSFile::Close()
    {
        m_impl->Close();
    }

    bool FSFile::Exists()
    {
        return m_impl->Exists(GetPath());
    }

    bool FSFile::IsOpen()
    {
#ifdef AURION_PLATFORM_WINDOWS
        return m_impl->GetDescriptor().handle != nullptr;
#elifdef AURION_PLATFORM_LINUX
        return m_impl->GetDescriptor().handle >= 0;
#else
        return false;
#endif
    }

    void FSFile::Read(void* buffer, u64 size)
    {
#ifdef AURION_PLATFORM_WINDOWS

#elifdef AURION_PLATFORM_LINUX
        read(m_impl->GetDescriptor().handle, buffer, size);
#else
#endif
    }

    void FSFile::Write(const void* buffer, u64 size)
    {
#ifdef AURION_PLATFORM_WINDOWS

#elifdef AURION_PLATFORM_LINUX
        write(m_impl->GetDescriptor().handle, buffer, size);
#else
#endif
    }

    void FSFile::Seek(i64 offset, FSSeekOrigin whence)
    {
#ifdef AURION_PLATFORM_WINDOWS

#elifdef AURION_PLATFORM_LINUX
        lseek(m_impl->GetDescriptor().handle, offset, whence);
#else
#endif
    }

    u64 FSFile::Tell()
    {
        return lseek(m_impl->GetDescriptor().handle, 0, FS_SEEK_CURRENT);
    }
}
