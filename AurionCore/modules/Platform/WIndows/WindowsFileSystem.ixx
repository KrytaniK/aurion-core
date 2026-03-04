module;

#include <AurionExport.h>

export module Aurion.FileSystem:Windows;

import :Interface;
import :Handle;

export namespace Aurion {
    class AURION_API WindowsFileSystem : IFileSystem {
    public:
        WindowsFileSystem();
        ~WindowsFileSystem() override;

        FSHandle GetSystemHandle(const char *path) override;

        FSMetadata GetMetadata(const char *path) override;
        FSMetadata GetMetadata(const FSHandle &sys_handle) override;
    };
}