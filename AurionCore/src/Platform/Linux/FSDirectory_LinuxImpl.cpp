module;

#include <AurionLog.h>
#include <cstring>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <dirent.h>

module Aurion.FileSystem;

#ifdef AURION_PLATFORM_LINUX
namespace Aurion
{
    FSDirectory_LinuxImpl::~FSDirectory_LinuxImpl()
    {
        FSDirectory_LinuxImpl::Close();
    }

    const FSDescriptor& FSDirectory_LinuxImpl::GetDescriptor()
    {
        return m_descriptor;
    }

    const FSMetadata& FSDirectory_LinuxImpl::GetMetadata(const char* path, bool follow_links)
    {
        struct stat info;
        if (stat(path, &info) != 0)
        {
            AURION_ERROR("Cannot Access File '%s'", path);
            return m_metadata;
        }

        // Bail if not a regular file
        if (!S_ISDIR(info.st_mode))
        {
            AURION_ERROR("'%s' is not a regular file", path);
            return m_metadata;
        }

        // Fill out metadata otherwise
        m_metadata.type = FS_TYPE_DIRECTORY;
        m_metadata.size = info.st_size;
        m_metadata.created_at = 0; // Linux doesn't reliably track creation time across distros
        m_metadata.accessed_at = info.st_atime;
        m_metadata.modified_at = info.st_mtime;

        return m_metadata;
    }

    void FSDirectory_LinuxImpl::Open(const char* path, FSFlags flags)
    {
        if (m_descriptor.handle != -1)
            return;

        int l_flags = 0;
        if (l_flags & FS_FLAGS_CREATE_IF_MISSING)
            l_flags |= O_CREAT;

        m_descriptor.handle = open(path, l_flags |= O_DIRECTORY);

        if (m_descriptor.handle == -1)
            return;

        struct stat info;
        fstat(m_descriptor.handle, &info);

        // Bail if not a regular file
        if (!S_ISDIR(info.st_mode))
        {
            AURION_ERROR("'%s' is not a regular file", path);
            close(m_descriptor.handle);
            m_descriptor.handle = -1;
            return;
        }

        // Go ahead and fill out metadata info
        m_metadata.size = info.st_size;
        m_metadata.type = FS_TYPE_DIRECTORY;
        m_metadata.created_at = 0; // Linux doesn't reliably track creation time across distros
        m_metadata.accessed_at = info.st_atime;
        m_metadata.modified_at = info.st_mtime;
    }

    void FSDirectory_LinuxImpl::Close()
    {
        if (m_descriptor.handle == -1)
            return;

        close(m_descriptor.handle);
        m_descriptor.handle = -1;
    }

    bool FSDirectory_LinuxImpl::Exists(const char* path)
    {
        return access(path, F_OK) == 0;
    }

    void FSDirectory_LinuxImpl::List(const char* path, FSEntry** entries, u64& count, const FSFlags& flags)
    {
        count = 0;

        DIR* dir = opendir(path);
        if (!dir)
            return;

        struct dirent* entry;

        // First pass: count entries
        while ((entry = readdir(dir)) != NULL)
        {
            // skip . and ..
            if (entry->d_name[0] == '.')
            {
                if (entry->d_name[1] == '\0' ||
                    (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
                {
                    continue;
                }
            }

            count++;
        }

        // Allocate
        entries = new FSEntry*[count];

        // Second pass: populate entries
        rewinddir(dir);
        u64 index = 0;
        while ((entry = readdir(dir)) != NULL)
        {
            // skip . and ..
            if (entry->d_name[0] == '.')
            {
                if (entry->d_name[1] == '\0' ||
                    (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
                {
                    continue;
                }
            }

            // Get path length
            u64 path_len = strlen(path);
            u64 entry_len = strlen(entry->d_name);

            // Allocate
            char full_path[path_len + entry_len + 1];

            // Piece together full path
            strcpy(full_path, path);
            strcat(full_path, "/");
            strcat(full_path, entry->d_name);

            // Generate Entry
            if (entry->d_type == DT_DIR)
                entries[index++] = new FSDirectory(full_path);
            else if (entry->d_type == DT_REG)
                entries[index++] = new FSFile(full_path);
        }
    }
}
#endif
