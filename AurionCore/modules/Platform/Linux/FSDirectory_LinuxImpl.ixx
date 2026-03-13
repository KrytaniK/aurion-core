module;

export module Aurion.FileSystem:LinuxDirectory;

import :Entry;
import :Directory;
import :Descriptor;

#ifdef AURION_PLATFORM_LINUX
#include <dirent.h>
export namespace Aurion
{
    class FSDirectory_LinuxImpl : public FSDirectoryImpl
    {
    public:
        FSDirectory_LinuxImpl();
        ~FSDirectory_LinuxImpl() override;

        const FSDescriptor& GetDescriptor() override;

        const FSMetadata& GetMetadata(const char* path, bool follow_links) override;

        void Open(const char* path, const FSFileOpenParams& params) override;
        void Close() override;

        bool Delete(const char* path) override;
        bool DeleteAll(const char* path) override;

        bool Exists(const char* path) override;

        FSCollection List(const char* path, bool counts_only) override;

    private:
        FSDescriptor m_descriptor;
        FSMetadata m_metadata;
        DIR* m_dirp;
    };
}
#endif