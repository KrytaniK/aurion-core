module;

export module Aurion.FileSystem:WindowsDirectory;

import :Entry;
import :Directory;
import :Descriptor;

#ifdef AURION_PLATFORM_WINDOWS
export namespace Aurion
{
    class FSDirectory_WindowsImpl : public FSDirectoryImpl
    {
    public:
        FSDirectory_WindowsImpl();
        ~FSDirectory_WindowsImpl() override;

        const FSDescriptor& GetDescriptor() override;

        const FSMetadata& GetMetadata(const char* path, bool follow_links) override;

        void Open(const char* path, const FSFileOpenParams& params) override;
        void Close() override;

        bool Delete(const char* path) override;
        bool DeleteAll(const char* path) override;

        bool Exists(const char* path) override;

        FSCollection List(const char* path) override;

    private:
        FSDescriptor m_descriptor;
        FSMetadata m_metadata;
    };
}
#endif