module;
#ifdef AURION_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <AurionLog.h>
#include <windows.h>

#endif
module Aurion.FileSystem;

#ifdef AURION_PLATFORM_WINDOWS

namespace Aurion
{
    FSFile_WindowsImpl::FSFile_WindowsImpl()
        : m_descriptor({}), m_metadata({})
    {
    }

    FSFile_WindowsImpl::~FSFile_WindowsImpl()
    {
        FSFile_WindowsImpl::Close();
    }

    const FSDescriptor& FSFile_WindowsImpl::GetDescriptor()
    {
        return m_descriptor;
    }

    const FSMetadata& FSFile_WindowsImpl::GetMetadata(const char* path, bool follow_links)
    {
        WIN32_FILE_ATTRIBUTE_DATA info{};
        if (!GetFileAttributesExA(path, GetFileExInfoStandard, &info))
        {
            AURION_ERROR("Failed to get file attributes for file path '%s'", path);
            return m_metadata;
        }

        // Bail if not a regular file
        if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            AURION_ERROR("'%s' is a directory", path);
            return m_metadata;
        }

        m_metadata.type = FS_TYPE_FILE;
        m_metadata.size = ToQuad(info.nFileSizeLow, info.nFileSizeHigh);
        m_metadata.created_at = FileTimeToQuad(info.ftCreationTime);
        m_metadata.accessed_at = FileTimeToQuad(info.ftLastAccessTime);
        m_metadata.modified_at = FileTimeToQuad(info.ftLastWriteTime);

        return m_metadata;
    }

    void FSFile_WindowsImpl::Open(const char* path, const FSFileOpenParams& params)
    {
        if (m_descriptor.handle != nullptr && m_descriptor.handle != INVALID_HANDLE_VALUE)
            return;

        m_descriptor.handle = CreateFileA(
            path,
            params.dwAccess,
            params.dwShareMode,
            params.lpSecurityAttr,
            params.dwCreateDisposition,
            params.dwFlagsAndAttr,
            params.hTemplateFile
        );

        if (m_descriptor.handle == INVALID_HANDLE_VALUE)
            AURION_ERROR("Cannot Open File '%s'", path);
    }

    void FSFile_WindowsImpl::Close()
    {
        if (m_descriptor.handle == nullptr) return;

        CloseHandle(m_descriptor.handle);
        m_descriptor.handle = nullptr;
    }

    bool FSFile_WindowsImpl::Exists(const char* path)
    {
        return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
    }

    void FSFile_WindowsImpl::Read(void* buffer, u64 size)
    {
        DWORD bytesRead{};
        ReadFile(m_descriptor.handle, buffer, size, &bytesRead, nullptr);
        // Null terminate the buffer
        static_cast<char*>(buffer)[bytesRead] = '\0';
    }

    void FSFile_WindowsImpl::Write(const void* buffer, u64 size)
    {
        WriteFile(m_descriptor.handle, buffer, size, nullptr, nullptr);
    }

    bool FSFile_WindowsImpl::Unlink(const char* path)
    {
        return DeleteFileA(path) != FALSE;
    }

    void FSFile_WindowsImpl::Seek(i64 offset, int whence)
    {
        LARGE_INTEGER li{};
        li.QuadPart = offset;
        SetFilePointerEx(m_descriptor.handle, li, nullptr, whence);
    }

    u64 FSFile_WindowsImpl::Tell()
    {
        constexpr LARGE_INTEGER zero{};
        LARGE_INTEGER pos{};
        SetFilePointerEx(m_descriptor.handle, zero, &pos, FILE_CURRENT);
        return static_cast<u64>(pos.QuadPart);
    }
}
#endif

