module;

#include <AurionExport.h>

export module Aurion.FileSystem:Descriptor;

import Aurion.Types;

export namespace Aurion {

    struct AURION_API FSDescriptor
    {
#ifdef AURION_PLATFORM_WINDOWS
        void* handle = nullptr;
#else
        int handle = -1;
#endif
    };

    typedef enum AURION_API FSDescriptorType : u16
    {
        FS_TYPE_UNKNOWN         = 0,
        FS_TYPE_FILE            = 1 << 0,
        FS_TYPE_DIRECTORY       = 1 << 1,
        FS_TYPE_SYMLINK         = 1 << 2,
    } FSDescriptorType;

    struct AURION_API FSMetadata {
        FSDescriptorType type = FS_TYPE_UNKNOWN;
        u64 size = 0;
        u64 created_at = 0;
        u64 modified_at = 0;
        u64 accessed_at = 0;
    };
}
