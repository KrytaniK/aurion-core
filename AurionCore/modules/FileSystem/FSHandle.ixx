module;

#include <AurionExport.h>

export module Aurion.FileSystem:Handle;

import Aurion.Types;

export namespace Aurion {

    typedef enum AURION_API FSType : u16
    {
        FS_TYPE_UNKNOWN         = 0,
        FS_TYPE_FILE            = 1 << 0,
        FS_TYPE_DIRECTORY       = 1 << 1,
    } FSType;

    struct AURION_API FSMetadata {
        FSType type = FS_TYPE_UNKNOWN;
        u64 size = 0;
    };

#ifdef AURION_PLATFORM_WINDOWS
    typedef u64 FSHandle; // File descriptors on Linux are 64-bit unsigned integers
#else
    typedef void* FSHandle; // File descriptors are HANDLE objects (void*) on Windows
#endif
}
