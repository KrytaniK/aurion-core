module;

#include <AurionExport.h>

export module Aurion.FileSystem:Interface;

import :Handle;

export namespace Aurion  {

    class AURION_API IFileSystem {
    public:
        virtual ~IFileSystem();

        virtual FSHandle GetSystemHandle(const char* path) = 0;

        virtual FSMetadata GetMetadata(const char* path) = 0;
        virtual FSMetadata GetMetadata(const FSHandle& sys_handle) = 0;

        // virtual FSFile GetFile(const char* path) = 0;
        // virtual FSFile GetFile(const FSHandle& sys_handle) = 0;
        //
        // virtual FSDirectory GetDir(const char* path) = 0;
        // virtual FSDirectory GetDir(const FSHandle& sys_handle) = 0;
    };

}
