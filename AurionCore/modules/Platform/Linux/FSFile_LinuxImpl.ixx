module;

#include <AurionExport.h>

export module Aurion.FileSystem:LinuxFile;

import :Entry;
import :Descriptor;

#ifdef AURION_PLATFORM_LINUX
export namespace Aurion
{
    class AURION_API FSFile_LinuxImpl : public FSEntryImpl
    {
    public:
        ~FSFile_LinuxImpl() override;

        const FSDescriptor& GetDescriptor() override;

        const FSMetadata& GetMetadata(const char* path, bool follow_links) override;

        void Open(const char* path, FSFlags flags) override;
        void Close() override;

        bool Exists(const char* path) override;

    private:
        int ToLinuxFlags(FSFlags flags);

    private:
        FSDescriptor m_descriptor;
        FSMetadata m_metadata;
    };
}
#endif
