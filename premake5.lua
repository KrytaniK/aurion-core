-- Workspace Declarations
workspace "AurionCore"
    architecture "x64"
    startproject "AurionCore"

    configurations { "Debug", "Release", "Dist"}

    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

-- Core Project Declaration
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
    files { "/**.h", "/**.ixx", "/**.cpp" } 

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

-- Function to scan and generate projects for each plugin in the SDK
function GeneratePluginProjects()
    local pluginDir = "plugins/"
    local pluginFolders = os.matchdirs(pluginDir .. "*")

    for _, folder in ipairs(pluginFolders) do
        local pluginName = path.getname(folder)

        print("Adding Project: " .. pluginName .. "Plugin")

        project(pluginName .. "Plugin")
            kind "SharedLib"
            language "C++"
            cppdialect "C++20"
            staticruntime "Off"
            location(folder)

        -- Build Directories
        targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/" .. pluginName)
        objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/" .. pluginName)

        -- Files types to compile
        files { folder .. "modules/**.ixx", folder .. "src/**.cpp" }

        -- Directories to compile from
        includedirs { "modules", "src" }

        -- Library linkage
        links { "AurionCore" }

        -- Platform (OS) Filters
        filter "system:windows"
		    systemversion "latest"

            defines { "AURION_PLATFORM_WINDOWS" }

        -- Build Configuration Filters
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            runtime "Release"
            optimize "On"
    end
end

-- Automatically create plugin projects
GeneratePluginProjects()