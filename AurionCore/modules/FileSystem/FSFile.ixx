module;

#include <AurionExport.h>

export module Aurion.FileSystem:File;

import Aurion.Types;
import :Entry;

namespace Aurion
{
    class AURION_API FSFile : public FSEntry
    {
    public:
        explicit FSFile(const char* path);
        ~FSFile() override;

        const FSMetadata& GetMetadata(bool follow_links) override;

        void Open(FSFlags flags) override;
        void Close() override;

        bool Exists() override;

        bool IsOpen();

        void Read(void* buffer, u64 size);
        void Write(const void* buffer, u64 size);

        void Seek(i64 offset, FSSeekOrigin whence);

        u64 Tell();

    private:
        FSEntryImpl* m_impl;
    };
}