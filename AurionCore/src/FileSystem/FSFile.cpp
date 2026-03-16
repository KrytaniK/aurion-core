module;

#include <AurionLog.h>

module Aurion.FileSystem;

namespace Aurion
{
    FSFile::FSFile()
        : FSEntry(nullptr), m_impl(nullptr)
    {

    }

    FSFile::FSFile(const char* path)
        : FSEntry(path), m_impl(nullptr)
    {
#ifdef AURION_PLATFORM_WINDOWS
        m_impl = new FSFile_WindowsImpl();
#elifdef AURION_PLATFORM_LINUX
        m_impl = new FSFile_LinuxImpl();
#else
        m_impl = nullptr;
#endif
    }

    FSFile::~FSFile()
    {
        if (!m_impl) return;

        // Close the file (if open)
        m_impl->Close();
        delete m_impl;
    }

    FSFile::FSFile(FSFile&& other) noexcept
        : FSEntry(static_cast<FSEntry&&>(other))
    {
        m_impl = other.m_impl;
        other.m_impl = nullptr;
    }

    FSFile& FSFile::operator=(FSFile&& other) noexcept
    {
        if (this == &other) return *this;
        FSEntry::operator=(static_cast<FSEntry&&>(other));

        if (m_impl) delete m_impl;
        m_impl = other.m_impl;
        other.m_impl = nullptr;

        return *this;
    }

    const FSMetadata& FSFile::GetMetadata(bool follow_links)
    {
        return m_impl->GetMetadata(m_path, follow_links);
    }

    void FSFile::Open(const FSFileOpenParams& params)
    {
        if (this->IsOpen())
        {
            AURION_WARN("Attempted to open '%s', but it is already open!", m_path);
            return;
        }

        m_impl->Open(m_path, params);
    }

    void FSFile::Close()
    {
        m_impl->Close();
    }

    bool FSFile::Exists()
    {
        return m_impl->Exists(m_path);
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
        m_impl->Read(buffer, size);
    }

    void FSFile::Write(const void* buffer, u64 size)
    {
        m_impl->Write(buffer, size);
    }

    bool FSFile::Unlink()
    {
        return m_impl->Unlink(m_path);
    }

    bool FSFile::Delete()
    {
        // Close the file if it's open
        if (IsOpen())
            Close();

        // Then remove from OS FileSystem
        return m_impl->Unlink(m_path);
    }

    void FSFile::Seek(i64 offset, int whence)
    {
        m_impl->Seek(offset, whence);
    }

    u64 FSFile::Tell()
    {
        return m_impl->Tell();
    }
}
