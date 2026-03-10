module;

#ifdef AURION_PLATFORM_LINUX
#include <AurionLog.h>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

module Aurion.FileSystem;

#ifdef AURION_PLATFORM_LINUX
namespace Aurion
{
    FSFile_LinuxImpl::~FSFile_LinuxImpl()
    {
        FSFile_LinuxImpl::Close();
    }

    const FSDescriptor& FSFile_LinuxImpl::GetDescriptor()
    {
        return m_descriptor;
    }

    const FSMetadata& FSFile_LinuxImpl::GetMetadata(const char* path, bool follow_links)
    {
        struct stat info{};
        if (stat(path, &info) != 0)
        {
            AURION_ERROR("Cannot Access File '%s'", path);
            return m_metadata;
        }

        // Bail if not a regular file
        if (!S_ISREG(info.st_mode))
        {
            AURION_ERROR("'%s' is not a regular file", path);
            return m_metadata;
        }

        // Fill out metadata otherwise
        m_metadata.type = FS_TYPE_FILE;
        m_metadata.size = info.st_size;
        m_metadata.created_at = 0; // Linux doesn't reliably track creation time across distros
        m_metadata.accessed_at = info.st_atime;
        m_metadata.modified_at = info.st_mtime;

        return m_metadata;
    }

    void FSFile_LinuxImpl::Open(const char* const FSFileOpenParams& params)
    {
        if (m_descriptor.handle != -1)
            return;

        // Open the file, opt for owner read/write, others read
        m_descriptor.handle = open(path, static_cast<int>(flags), static_cast<int>(access));

        if (m_descriptor.handle == -1)
            AURION_ERROR("Cannot Open File '%s'", path);
    }

    void FSFile_LinuxImpl::Close()
    {
        if (m_descriptor.handle == -1)
            return;

        if (close(m_descriptor.handle) != 0)
            AURION_ERROR("Failed to close File Descriptor %d: %s", m_descriptor.handle, strerror(errno));

        m_descriptor.handle = -1;
    }

    bool FSFile_LinuxImpl::Exists(const char* path)
    {
        return access(path, F_OK) == 0;
    }

    void FSFile_LinuxImpl::Read(void* buffer, u64 size)
    {
        read(m_descriptor.handle, buffer, size);
    }

    void FSFile_LinuxImpl::Write(const void* buffer, u64 size)
    {
        if (write(m_descriptor.handle, buffer, size) != size)
            AURION_ERROR("Write operation failed: %s", strerror(errno));
    }

    bool FSFile_LinuxImpl::Unlink(const char* path)
    {
        if (unlink(path) != 0)
        {
            AURION_ERROR("FSFile Unlink operation failed: %s", strerror(errno));
            return false;
        }

        return true;
    }

    void FSFile_LinuxImpl::Seek(i64 offset, int whence)
    {
        lseek(m_descriptor.handle, offset, whence);
    }

    u64 FSFile_LinuxImpl::Tell()
    {
        return lseek(m_descriptor.handle, 0l, SEEK_CUR);
    }
}
#endif

