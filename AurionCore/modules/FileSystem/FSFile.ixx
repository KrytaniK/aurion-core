module;

#include <AurionExport.h>

export module Aurion.FileSystem:File;

import Aurion.Types;
import :Entry;

export namespace Aurion
{
    class AURION_API FSFile : public FSEntry
    {
    public:
        explicit FSFile(const char* path);
        ~FSFile() override;

        FSFile(FSFile&& other) noexcept;
        FSFile& operator=(FSFile&& other) noexcept;

        FSFile(const FSFile&) = delete;
        FSFile& operator=(const FSFile&) = delete;

        const FSMetadata& GetMetadata(bool follow_links) override;

        void Open(u32 flags, u32 access) override;
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
        FSEntryImpl* m_impl;
    };
}