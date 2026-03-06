module;

#include <AurionExport.h>

export module Aurion.FileSystem:Entry;

import :Descriptor;

import Aurion.Types;

export namespace Aurion
{
    class AURION_API FSEntry
    {
    public:
        explicit FSEntry(const char* path);
        virtual ~FSEntry();

        FSEntry(FSEntry&& other) noexcept;
        FSEntry& operator=(FSEntry&& other) noexcept;

        FSEntry(const FSEntry&) = delete;
        FSEntry& operator=(const FSEntry&) = delete;

        const char* GetPath();

        virtual const FSMetadata& GetMetadata(bool follow_links) = 0;

        virtual void Open(u32 flags, u32 access) = 0;
        virtual void Close() = 0;

        virtual bool Exists() = 0;

    protected:
        char* m_path;
    };

    class AURION_API FSEntryImpl
    {
    public:
        virtual ~FSEntryImpl() = default;

        virtual const FSDescriptor& GetDescriptor() = 0;
        virtual const FSMetadata& GetMetadata(const char* path, bool follow_links) = 0;

        virtual void Open(const char* path, u32 flags, u32 access) = 0;
        virtual void Close() = 0;

        virtual bool Exists(const char* path) = 0;
    };
}