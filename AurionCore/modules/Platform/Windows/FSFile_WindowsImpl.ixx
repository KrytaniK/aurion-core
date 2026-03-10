module;

#include <AurionExport.h>

#ifdef AURION_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

export module Aurion.FileSystem:WindowsFile;

import :File;
import :Descriptor;

#ifdef AURION_PLATFORM_WINDOWS
export namespace Aurion
{
    constexpr auto ToQuad = [](DWORD low, DWORD high) -> u64
    {
        ULARGE_INTEGER li{};
        li.LowPart = low;
        li.HighPart = high;
        return li.QuadPart;
    };

    constexpr auto FileTimeToQuad = [](const FILETIME& filetime) -> u64
    {
        ULARGE_INTEGER ull{};
        ull.LowPart = filetime.dwLowDateTime;
        ull.HighPart = filetime.dwHighDateTime;
        return ull.QuadPart;
    };

    class AURION_API FSFile_WindowsImpl : public FSFileImpl
    {
    public:
        FSFile_WindowsImpl();
        ~FSFile_WindowsImpl() override;

        const FSDescriptor& GetDescriptor() override;

        const FSMetadata& GetMetadata(const char* path, bool follow_links) override;

        void Open(const char* path, const FSFileOpenParams& params) override;
        void Close() override;

        bool Exists(const char* path) override;

        void Read(void* buffer, u64 size) override;
        void Write(const void* buffer, u64 size) override;

        bool Unlink(const char* path) override;

        void Seek(i64 offset, int whence) override;

        u64 Tell() override;

    private:
        FSDescriptor m_descriptor;
        FSMetadata m_metadata;
    };
}
#endif
