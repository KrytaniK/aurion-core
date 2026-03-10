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

    void FSDirectory_LinuxImpl::Open(const char* path, const FSFileOpenParams& params)
    {
        if (m_descriptor.handle != -1)
            return;

        m_descriptor.handle = open(path, params.flags | O_DIRECTORY, params.access);

        if (m_descriptor.handle == -1)
            return;
    }

    void FSDirectory_LinuxImpl::Close()
    {
        if (m_descriptor.handle == -1)
            return;

        close(m_descriptor.handle);
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
        // Open Directory
        this->Open(path, 0, 0644);

        // Loop through contents:
            // Unlink files, recursively remove contents

        return false;
    }

    bool FSDirectory_LinuxImpl::Exists(const char* path)
    {
        return access(path, F_OK) == 0;
    }

    void FSDirectory_LinuxImpl::List(const char* path, FSCollection* entries, u64& count)
    {
        count = 0;

        DIR* dir = opendir(path);
        if (!dir)
            return;

        dirent* entry;

        // First pass: count files and directories
        u64 file_count = 0;
        u64 dir_count = 0;
        while ((entry = readdir(dir)) != nullptr)
        {
            if (entry->d_name[0] == '.')
            {
                if (entry->d_name[1] == '\0' ||
                    (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
                    continue;
            }

            if (entry->d_type == DT_DIR)
                dir_count++;
            else if (entry->d_type == DT_REG)
                file_count++;

            count++;
        }

        // Bail if only getting the count
        if (!entries)
        {
            closedir(dir);
            return;
        }

        // Allocate storage in the collection
        entries->Allocate(file_count, dir_count);

        // Second pass: construct entries via placement new
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
                new (entries->Directories() + di++) FSDirectory(static_cast<const char*>(full_path));
            else if (entry->d_type == DT_REG)
                new (entries->Files() + fi++) FSFile(static_cast<const char*>(full_path));
        }

        closedir(dir);
    }
}
#endif
