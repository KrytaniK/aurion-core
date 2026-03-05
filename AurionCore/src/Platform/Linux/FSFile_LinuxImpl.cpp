module;

#include <AurionLog.h>

#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

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
        struct stat info;
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

    void FSFile_LinuxImpl::Open(const char* path, FSFlags flags)
    {
        if (m_descriptor.handle != -1)
            return;

        m_descriptor.handle = open(path, ToLinuxFlags(flags));

        if (m_descriptor.handle == -1)
            return;

        struct stat info;
        fstat(m_descriptor.handle, &info);

        // Bail if not a regular file
        if (!S_ISREG(info.st_mode))
        {
            AURION_ERROR("'%s' is not a regular file", path);
            close(m_descriptor.handle);
            m_descriptor.handle = -1;
            return;
        }

        // Go ahead and fill out metadata info
        m_metadata.size = info.st_size;
        m_metadata.type = FS_TYPE_FILE;
        m_metadata.created_at = 0; // Linux doesn't reliably track creation time across distros
        m_metadata.accessed_at = info.st_atime;
        m_metadata.modified_at = info.st_mtime;
    }

    void FSFile_LinuxImpl::Close()
    {
        if (m_descriptor.handle == -1)
            return;

        close(m_descriptor.handle);
        m_descriptor.handle = -1;
    }

    bool FSFile_LinuxImpl::Exists(const char* path)
    {
        return access(path, F_OK) == 0;
    }

    int FSFile_LinuxImpl::ToLinuxFlags(FSFlags flags)
    {
        int result = 0;

        if (flags & FS_FLAGS_READ_ONLY)
            result |= O_RDONLY;
        if (flags & FS_FLAGS_WRITE_ONLY)
            result |= O_WRONLY;
        if (flags & FS_FLAGS_READ_WRITE)
            result |= O_RDWR;
        if (flags & FS_FLAGS_APPEND)
            result |= O_APPEND;
        if (flags & FS_FLAGS_CREATE_IF_MISSING)
            result |= O_CREAT;

        return result;
    }
}
#endif
