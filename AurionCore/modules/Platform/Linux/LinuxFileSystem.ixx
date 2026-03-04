module;

#include <AurionExport.h>

export module Aurion.FileSystem:Linux;

import :Interface;
import :Handle;

export namespace Aurion {
    class AURION_API LinuxFileSystem : IFileSystem {
    public:
        LinuxFileSystem();
        ~LinuxFileSystem() override;

        FSHandle GetSystemHandle(const char *path) override;

        FSMetadata GetMetadata(const char *path) override;
        FSMetadata GetMetadata(const FSHandle &sys_handle) override;
    };
}