-- Workspace Declarations
workspace "AurionCore"
    architecture "x64"
    startproject "AurionCore"

    configurations { "Debug", "Release", "Dist"}

    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

-- Project Declaration
project "AurionCore"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    -- Build Directories
    targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")

    -- C++ Module Support
    allmodulespublic "On"
    scanformoduledependencies "true"

    -- File Locations
    files { "macros/**.h", "modules/**.ixx", "src/**.cpp" } 

    -- Include Directories
    includedirs {}

    -- Library Directories 
    libdirs {}

    -- Link Directories
    links {}

    -- Global Project defines
    defines { "AURION_DLL" }

    -- Platform (OS) Filters
    filter "system:windows"
		systemversion "latest"

        defines { "AURION_PLATFORM_WINDOWS" }

    -- Build Configuration Filters
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

        defines { "AURION_CORE_DEBUG" }

        links {}

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

        links {}

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"

        links {}