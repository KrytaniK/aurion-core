module;

#include <unistd.h>
#include <AurionLog.h>
#include <cstring>

#include <fcntl.h>
#include <cerrno>

module Aurion.FileSystem;

namespace Aurion
{
    FSFile::FSFile(const char* path)
        : FSEntry(path), m_impl(nullptr)
    {
#ifdef AURION_PLATFORM_WINDOWS

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
        FSEntry::operator=(static_cast<FSEntry&&>(other));
        if (this == &other) return *this;

        if (m_impl) delete m_impl;
        m_impl = other.m_impl;
        other.m_impl = nullptr;

        return *this;
    }

    const FSMetadata& FSFile::GetMetadata(bool follow_links)
    {
        return m_impl->GetMetadata(m_path, follow_links);
    }

    void FSFile::Open(u32 flags, u32 access)
    {
        if (this->IsOpen())
        {
            AURION_WARN("Attempted to open '%s', but it is already open!", m_path);
            return;
        }

        m_impl->Open(m_path, static_cast<int>(flags), static_cast<int>(access));
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
        if (write(m_impl->GetDescriptor().handle, buffer, size) != size)
            AURION_ERROR("Write operation failed: %s", strerror(errno));
#else
#endif
    }

    bool FSFile::Unlink()
    {
#ifdef AURION_PLATFORM_WINDOWS

#elifdef AURION_PLATFORM_LINUX
        if (unlink(GetPath()) != 0)
        {
            AURION_ERROR("FSFile Unlink operation failed: %s", strerror(errno));
            return false;
        }
#else
#endif

        return true;
    }

    bool FSFile::Delete()
    {
        // Close the file if it's open
        if (this->IsOpen())
            this->Close();

        // Then unlink
        #ifdef AURION_PLATFORM_WINDOWS

        #elifdef AURION_PLATFORM_LINUX
                if (unlink(m_path) != 0)
                {
                    AURION_ERROR("FSFile Delete operation failed: %s", strerror(errno));
                    return false;
                }
        #else
        #endif

        return true;
    }

    void FSFile::Seek(i64 offset, int whence)
    {
#ifdef AURION_PLATFORM_WINDOWS

#elifdef AURION_PLATFORM_LINUX
        lseek(m_impl->GetDescriptor().handle, offset, whence);
#else
#endif
    }

    u64 FSFile::Tell()
    {
        return lseek(m_impl->GetDescriptor().handle, 0l, SEEK_CUR);
    }
}
