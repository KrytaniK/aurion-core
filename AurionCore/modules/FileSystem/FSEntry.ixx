module;

#include <AurionExport.h>

export module Aurion.FileSystem:Entry;

import :Descriptor;

export namespace Aurion
{
    class AURION_API FSEntry
    {
    public:
        explicit FSEntry(const char* path);
        virtual ~FSEntry() = 0;

        const char* GetPath();

        virtual const FSMetadata& GetMetadata(bool follow_links) = 0;

        virtual void Open(FSFlags flags) = 0;
        virtual void Close() = 0;

        virtual bool Exists() = 0;

    protected:
        char* m_path;
    };

    class AURION_API FSEntryImpl
    {
    public:
        virtual ~FSEntryImpl() = 0;

        virtual const FSDescriptor& GetDescriptor() = 0;
        virtual const FSMetadata& GetMetadata(const char* path, bool follow_links) = 0;

        virtual void Open(const char* path, FSFlags flags) = 0;
        virtual void Close() = 0;

        virtual bool Exists(const char* path) = 0;
    };
}