module;

#include <AurionExport.h>

export module Aurion.FileSystem:LinuxFile;

import :File;
import :Descriptor;

#ifdef AURION_PLATFORM_LINUX
export namespace Aurion
{
    class AURION_API FSFile_LinuxImpl : public FSFileImpl
    {
    public:
        ~FSFile_LinuxImpl() override;

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
