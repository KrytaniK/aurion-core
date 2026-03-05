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

    typedef enum AURION_API FSSeekOrigin : u8
    {
        FS_SEEK_BEGIN,
        FS_SEEK_CURRENT,
        FS_SEEK_END
    };

    typedef enum AURION_API FSDescriptorType : u16
    {
        FS_TYPE_UNKNOWN         = 0,
        FS_TYPE_FILE            = 1 << 0,
        FS_TYPE_DIRECTORY       = 1 << 1,
        FS_TYPE_SYMLINK         = 1 << 2,
    } FSDescriptorType;

    typedef enum AURION_API FSFlags : u16
    {
        FS_FLAGS_NONE               = 0,
        FS_FLAGS_FOLLOW_LINKS       = 1 << 0,
        FS_FLAGS_RECURSIVE          = 1 << 1,
        FS_FLAGS_CREATE_IF_MISSING  = 1 << 2,
        FS_FLAGS_READ_ONLY          = 1 << 3,
        FS_FLAGS_WRITE_ONLY         = 1 << 4,
        FS_FLAGS_READ_WRITE         = FS_FLAGS_READ_ONLY | FS_FLAGS_WRITE_ONLY,
        FS_FLAGS_APPEND             = 1 << 5
    } FSFlags;

    struct AURION_API FSMetadata {
        FSDescriptorType type = FS_TYPE_UNKNOWN;
        u64 size = 0;
        u64 created_at = 0;
        u64 modified_at = 0;
        u64 accessed_at = 0;
    };
}
