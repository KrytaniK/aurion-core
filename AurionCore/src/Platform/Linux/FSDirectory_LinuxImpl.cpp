#ifdef AURION_PLATFORM_LINUX
module;

#include <AurionLog.h>
#include <cstring>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <dirent.h>
#include <cerrno>
#include <new>

module Aurion.FileSystem;

namespace Aurion
{
    FSDirectory_LinuxImpl::FSDirectory_LinuxImpl()
        : m_descriptor({}), m_metadata({}), m_dirp(nullptr)
    {
    }

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
        struct stat info{};
        if (stat(path, &info) != 0)
        {
            AURION_ERROR("Cannot Access Directory '%s'", path);
            return m_metadata;
        }

        // Bail if not a regular file
        if (!S_ISDIR(info.st_mode))
        {
            AURION_ERROR("'%s' is not a directory", path);
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

    void FSDirectory_LinuxImpl::Open(const char* path, const FSFileOpenParams& params)
    {
        if (m_descriptor.handle != -1)
            return;

        // Parameters are ignored on Linux
        m_dirp = opendir(path);
        m_descriptor.handle = dirfd(m_dirp);
    }

    void FSDirectory_LinuxImpl::Close()
    {
        if (m_descriptor.handle == -1 || m_dirp == nullptr)
            return;

        closedir(m_dirp);
        m_descriptor.handle = -1;
    }

    bool FSDirectory_LinuxImpl::Delete(const char* path)
    {
        if (rmdir(path) != 0)
        {
            AURION_ERROR("Cannot Delete Directory '%s': %s", path, strerror(errno));
            return false;
        }

        return true;
    }

    bool FSDirectory_LinuxImpl::DeleteAll(const char* path)
    {
        FSCollection entries = List(path, false);

        // Recursively call down to the leaf directory
        for (u64 i = 0; i < entries.directory_count; i++)
            if (!entries.directories[i].DeleteAll())
                return false;

        // Delete all files in this directory
        for (u64 i = 0; i < entries.file_count; i++)
            if (!entries.files[i].Delete())
                return false;

        delete[] entries.files;
        delete[] entries.directories;

        return Delete(path);
    }

    bool FSDirectory_LinuxImpl::Exists(const char* path)
    {
        return access(path, F_OK) == 0;
    }

    FSCollection FSDirectory_LinuxImpl::List(const char* path, bool counts_only)
    {
        FSCollection entries;
        entries.file_count = 0;
        entries.directory_count = 0;

        DIR* dir = m_dirp ? m_dirp : opendir(path);

        dirent* entry;

        // First pass: count files and directories
        while ((entry = readdir(dir)) != nullptr)
        {
            if (entry->d_name[0] == '.')
            {
                if (entry->d_name[1] == '\0' ||
                    (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
                    continue;
            }

            if (entry->d_type == DT_DIR)
                entries.directory_count++;
            else if (entry->d_type == DT_REG)
                entries.file_count++;
        }

        // Bail early if only the counts were requested
        if (counts_only) return entries;

        // Allocate storage in the collection
        entries.files = new FSFile[entries.file_count];
        entries.directories = new FSDirectory[entries.directory_count];

        // Second pass: construct entries
        rewinddir(dir);
        u64 fi = 0;
        u64 di = 0;
        while ((entry = readdir(dir)) != nullptr)
        {
            if (entry->d_name[0] == '.')
            {
                if (entry->d_name[1] == '\0' ||
                    (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
                    continue;
            }

            u64 path_len = strlen(path);
            u64 entry_len = strlen(entry->d_name);
            u64 full_len = path_len + entry_len + 2;

            char full_path[full_len];
            strcpy(full_path, path);

            if (path_len > 0 && path[path_len - 1] != '/')
                strcat(full_path, "/");

            strcat(full_path, entry->d_name);
            full_path[full_len - 1] = '\0';

            if (entry->d_type == DT_DIR)
                entries.directories[di++] = FSDirectory(full_path);
            else if (entry->d_type == DT_REG)
                entries.files[fi++] = FSFile(full_path);
        }

        // If the directory wasn't opened via Open(),
        //  close it.
        if (dir != m_dirp)
            closedir(dir);

        return entries;
    }
}
#endif
