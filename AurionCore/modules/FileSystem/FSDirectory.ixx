module;

#include <AurionExport.h>

export module Aurion.FileSystem:Directory;

import :Entry;

namespace Aurion
{
    class AURION_API FSDirectoryImpl : public FSEntryImpl
    {
    public:
        virtual ~FSDirectoryImpl() = 0;
        virtual const FSDescriptor& GetDescriptor() = 0;

        virtual const FSMetadata& GetMetadata(const char* path, bool follow_links) = 0;

        virtual void Open(const char* path, FSFlags flags) = 0;
        virtual void Close() = 0;

        virtual bool Exists(const char* path) = 0;

        virtual void List(const char* path, FSEntry** entries, u64& count, const FSFlags& flags) = 0;
    };

    class AURION_API FSDirectory : public FSEntry
    {
    public:
        explicit FSDirectory(const char* path);
        ~FSDirectory() override;

        const FSMetadata& GetMetadata(bool follow_links) override;

        void Open(FSFlags flags) override;
        void Close() override;

        bool Exists() override;

        bool IsOpen();

        void List(FSEntry** entries, u64& count, const FSFlags& flags);

    private:
        FSDirectoryImpl* m_impl;
    };
}