module;

#include <AurionExport.h>

export module Aurion.FileSystem:Collection;

import :File;
import :Directory;

export namespace Aurion
{
    struct AURION_API FSCollection
    {
        u64 file_count = 0;
        u64 directory_count = 0;
        FSFile* files = nullptr;
        FSDirectory* directories = nullptr;
    };
}