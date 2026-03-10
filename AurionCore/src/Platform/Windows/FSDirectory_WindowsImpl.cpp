module;

#ifdef AURION_PLATFORM_WINDOWS
#include <AurionLog.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <new>

#endif
module Aurion.FileSystem;

#ifdef AURION_PLATFORM_WINDOWS
namespace Aurion
{
    FSDirectory_WindowsImpl::FSDirectory_WindowsImpl()
        : m_descriptor({}), m_metadata({})
    {
    }

    FSDirectory_WindowsImpl::~FSDirectory_WindowsImpl()
    {
        FSDirectory_WindowsImpl::Close();
    }

    const FSDescriptor& FSDirectory_WindowsImpl::GetDescriptor()
    {
        return m_descriptor;
    }

    const FSMetadata& FSDirectory_WindowsImpl::GetMetadata(const char* path, bool follow_links)
    {
        WIN32_FILE_ATTRIBUTE_DATA info{};
        if (!GetFileAttributesExA(path, GetFileExInfoStandard, &info))
        {
            AURION_ERROR("Failed to get file attributes for file path '%s'", path);
            return m_metadata;
        }

        // Bail if not a directory
        if (!(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            AURION_ERROR("'%s' is not a directory", path);
            return m_metadata;
        }

        m_metadata.type = FS_TYPE_FILE;
        m_metadata.size = ToQuad(info.nFileSizeLow, info.nFileSizeHigh);
        m_metadata.created_at = FileTimeToQuad(info.ftCreationTime);
        m_metadata.accessed_at = FileTimeToQuad(info.ftLastAccessTime);
        m_metadata.modified_at = FileTimeToQuad(info.ftLastWriteTime);

        return m_metadata;
    }

    void FSDirectory_WindowsImpl::Open(const char* path, const FSFileOpenParams& params)
    {
        if (m_descriptor.handle) return;

        m_descriptor.handle = CreateFileA(
            path,
            params.dwAccess,
            params.dwShareMode,
            params.lpSecurityAttr,
            params.dwCreateDisposition,
            params.dwFlagsAndAttr | FILE_FLAG_BACKUP_SEMANTICS, // backup_semantics required for directories
            params.hTemplateFile
        );
    }

    void FSDirectory_WindowsImpl::Close()
    {
        if (m_descriptor.handle == nullptr) return;

        CloseHandle(m_descriptor.handle);
        m_descriptor.handle = nullptr;
    }

    bool FSDirectory_WindowsImpl::Delete(const char* path)
    {
        if (RemoveDirectoryA(path) == FALSE)
        {
            AURION_ERROR("Failed to remove directory '%s': Error code (%ul)", path, GetLastError());
            return false;
        }

        return true;
    }

    bool FSDirectory_WindowsImpl::DeleteAll(const char* path)
    {
        FSCollection entries = List(path);

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

        // entries destructor handles cleanup
        return Delete(path);
    }

    bool FSDirectory_WindowsImpl::Exists(const char* path)
    {
        return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
    }

    FSCollection FSDirectory_WindowsImpl::List(const char* path)
    {
        FSCollection entries{
            .file_count = 0,
            .directory_count = 0,
            .files = nullptr,
            .directories = nullptr
        };

        HANDLE entry = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATA efData{};

        char search_path[MAX_PATH];
        strcpy_s(search_path, MAX_PATH, path);
        strcat_s(search_path, MAX_PATH, "\\*");

        entry = FindFirstFileA(search_path, &efData);
        if (entry == INVALID_HANDLE_VALUE)
        {
            AURION_ERROR("FindFirstFile failed for '%s'. Error: %lu", search_path, GetLastError());
            return FSCollection{};
        }

        // First pass: count files and directories
        do
        {
            if (efData.cFileName[0] == '.')
            {
                if (efData.cFileName[1] == '\0' ||
                    (efData.cFileName[1] == '.' && efData.cFileName[2] == '\0'))
                    continue;
            }

            if (efData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                entries.directory_count++;
            else
                entries.file_count++;
        } while (FindNextFileA(entry, &efData) != 0);

        // Heap allocate storage in the collection
        entries.files = new FSFile[entries.file_count];
        entries.directories = new FSDirectory[entries.directory_count];

        // Second pass: construct entries via placement new
        FindClose(entry);
        entry = FindFirstFileA(search_path, &efData);
        if (entry == INVALID_HANDLE_VALUE)
        {
            AURION_ERROR("FindFirstFile failed on second pass for '%s'. Error: %lu", search_path, GetLastError());
            delete[] entries.files;
            delete[] entries.directories;
            return FSCollection{};
        }

        u64 fi = 0;
        u64 di = 0;
        do
        {
            if (efData.cFileName[0] == '.')
            {
                if (efData.cFileName[1] == '\0' ||
                    (efData.cFileName[1] == '.' && efData.cFileName[2] == '\0'))
                    continue;
            }

            char full_path[MAX_PATH];
            strcpy_s(full_path, MAX_PATH, path);
            strcat_s(full_path, MAX_PATH, "\\");
            strcat_s(full_path, MAX_PATH, efData.cFileName);

            if (efData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                entries.directories[di++] = FSDirectory(full_path);
            else
                entries.files[fi++] = FSFile(full_path);
        } while (FindNextFileA(entry, &efData) != 0);

        DWORD dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES)
            AURION_ERROR("List failed for path '%s': Error code %lu", search_path, dwError);

        FindClose(entry);
        return entries;
    }

}
#endif
