module;

#include <AurionExport.h>

export module Aurion.FileSystem:Directory;

import :Entry;
import :File;

import Aurion.Types;
import Aurion.Memory;

export namespace Aurion
{
    struct FSCollection;

    class AURION_API FSDirectoryImpl : public FSEntryImpl
    {
    public:
        ~FSDirectoryImpl() override = default;
        const FSDescriptor& GetDescriptor() override = 0;

        const FSMetadata& GetMetadata(const char* path, bool follow_links) override = 0;

        void Open(const char* path, u32 flags, u32 access) override = 0;
        void Close() override = 0;

        virtual bool Delete(const char* path) = 0;
        virtual bool DeleteAll(const char* path) = 0;

        bool Exists(const char* path) override = 0;

        virtual void List(const char* path, FSCollection* entries, u64& count) = 0;
    };

    class AURION_API FSDirectory : public FSEntry
    {
    public:
        explicit FSDirectory(const char* path);
        ~FSDirectory() override;

        FSDirectory(FSDirectory&& other) noexcept;
        FSDirectory& operator=(FSDirectory&& other) noexcept;

        FSDirectory(const FSDirectory&) = delete;
        FSDirectory& operator=(const FSDirectory&) = delete;

        const FSMetadata& GetMetadata(bool follow_links) override;

        void Open(u32 flag, u32 access) override;
        void Close() override;

        bool Delete();
        bool DeleteAll();

        bool Exists() override;

        bool IsOpen();

        void List(FSCollection* entries, u64& count);

    private:
        FSDirectoryImpl* m_impl;
    };

    struct AURION_API FSCollection
    {
        Vector<FSFile> files;
        Vector<FSDirectory> directories;
    };
}
