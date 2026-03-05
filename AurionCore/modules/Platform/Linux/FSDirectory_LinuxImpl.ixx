module;

export module Aurion.FileSystem:LinuxDirectory;

import :Entry;
import :Directory;
import :Descriptor;

#ifdef AURION_PLATFORM_LINUX
export namespace Aurion
{
    class FSDirectory_LinuxImpl : public FSDirectoryImpl
    {
    public:
        ~FSDirectory_LinuxImpl() override;

        const FSDescriptor& GetDescriptor() override;

        const FSMetadata& GetMetadata(const char* path, bool follow_links) override;

        void Open(const char* path, FSFlags flags) override;
        void Close() override;

        bool Exists(const char* path) override;

        void List(const char* path, FSEntry** entries, u64& count, const FSFlags& flags) override;

    private:
        FSDescriptor m_descriptor;
        FSMetadata m_metadata;
    };
}
#endif