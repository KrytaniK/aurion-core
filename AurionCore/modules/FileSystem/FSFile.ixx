module;

#include <AurionExport.h>

export module Aurion.FileSystem:File;

import Aurion.Types;
import :Descriptor;
import :Entry;

export namespace Aurion
{
    class AURION_API FSFileImpl : public FSEntryImpl
    {
    public:
        ~FSFileImpl() override = default;

        const FSDescriptor& GetDescriptor() override = 0;
        const FSMetadata& GetMetadata(const char* path, bool follow_links) override = 0;

        void Open(const char* path, const FSFileOpenParams& params) override = 0;
        void Close() override = 0;

        bool Exists(const char* path) override = 0;

        virtual void Read(void* buffer, u64 size) = 0;
        virtual void Write(const void* buffer, u64 size) = 0;

        virtual bool Unlink(const char* path) = 0;

        virtual void Seek(i64 offset, int whence) = 0;

        virtual u64 Tell() = 0;
    };

    class AURION_API FSFile : public FSEntry
    {
    public:
        FSFile();
        explicit FSFile(const char* path);
        ~FSFile() override;

        FSFile(FSFile&& other) noexcept;
        FSFile& operator=(FSFile&& other) noexcept;

        FSFile(const FSFile&) = delete;
        FSFile& operator=(const FSFile&) = delete;

        const FSMetadata& GetMetadata(bool follow_links) override;

        void Open(const FSFileOpenParams& params) override;
        void Close() override;

        bool Exists() override;

        bool IsOpen();

        void Read(void* buffer, u64 size);
        void Write(const void* buffer, u64 size);

        bool Unlink();
        bool Delete();

        void Seek(i64 offset, int whence);

        u64 Tell();

    private:
        FSFileImpl* m_impl;
    };
}