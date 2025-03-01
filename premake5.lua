-- Workspace Declarations
workspace "AurionCore"
    architecture "x64"
    startproject "Sandbox"

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
    files { "macros/**.h", "modules/**.ixx", "src/**.cpp" } 

    -- Include Directories
    includedirs {}

    -- Library Directories 
    libdirs {}

    -- Link Directories
    links {}

    -- Global Project defines
    defines { "AURION_DLL" }

    postbuildcommands {
        "{MKDIR} %{wks.location}/build/bin/" .. outputdir .. "/Sandbox",
        "{COPYFILE} %{wks.location}/build/bin/" .. outputdir .. "/AurionCore/AurionCore.dll %{wks.location}/build/bin/" .. outputdir .. "/Sandbox/"
    }

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

pluginProjects = {}
pluginNames = {}

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

            scanformoduledependencies "true"

            -- Build Directories
            targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/plugins/" .. pluginName)
            objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/plugins/" .. pluginName)

            -- Files types to compile
            files { folder .. "/modules/**.ixx", folder .. "/src/**.cpp" }

            -- Directories to include
            includedirs { "%{wks.location}/modules", "%{wks.location}/src" }

            -- Library linkage
            links { "AurionCore" }

            -- Global project defins
            defines { "AURION_DLL" }

            postbuildcommands {
                "{MKDIR} %{wks.location}/Sandbox/plugins",
                "{COPYFILE} %{wks.location}/build/bin/" .. outputdir .. "/plugins/" .. pluginName .. "/*.dll %{wks.location}/Sandbox/plugins/"
            }

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
        
            -- Attach this project to the table for linking with Sandbox
            table.insert(pluginProjects, pluginName .. "Plugin")
            table.insert(pluginNames, pluginName)
    end
end

-- Automatically create plugin projects
GeneratePluginProjects()

-- Sandbox project for testing
project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"
    

    scanformoduledependencies "true"
    
    location("Sandbox")

    targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "Sandbox/**.ixx", "Sandbox/**.h", "Sandbox/**.cpp" }

    includedirs { "%{wks.location}/modules", "%{wks.location}/src" }

    -- Link Core Framework
    links { "AurionCore" }

    -- Link All plugins
    for _, plugin in ipairs(pluginProjects) do
        links { plugin }
    end

    -- Platform (OS) Filters
    filter "system:windows"
		systemversion "latest"

        defines { "AURION_PLATFORM_WINDOWS" }

        postBuildCmds = {}

        -- Add commands for copying plugin DLLs
        for _, pluginName in ipairs(pluginNames) do
            table.insert(postBuildCmds, "{COPY} %{wks.location}/build/bin/" .. outputdir .. "/plugins/".. pluginName .. "/*.dll %{wks.location}/Sandbox/plugins/")
        end

-- Apply all collected post-build commands
postbuildcommands(postBuildCmds)

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