-- Workspace Declarations
workspace "AurionCore"
    architecture "x64"
    startproject "Sandbox"

    configurations { "Debug", "Release", "Dist"}

    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

-- Function to generate core solution
function GenerateCoreSolution()
    print("Generating Solution: AurionCore")

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
        files { "core/**.h", "core/**.ixx", "core/**.cpp" } 

        -- Include Directories
        includedirs { "core", "third_party/GLFW/include" }

        -- Library Directories 
        libdirs { "third_party/GLFW/lib" }

        -- Link Directories
        links { "glfw3.lib" }

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

        filter "configurations:Release"
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            runtime "Release"
            optimize "On"
end

-- Function to scan and generate projects for each plugin in the SDK
function GeneratePluginProjects()
    local projects = {}
    local names = {}

    local pluginDir = "plugins/"
    local pluginFolders = os.matchdirs(pluginDir .. "*")

    for _, folder in ipairs(pluginFolders) do
        local pluginName = path.getname(folder)

        print("[Premake5] Adding Project: " .. pluginName .. "Plugin")

        project(pluginName .. "Plugin")
            kind "SharedLib"
            language "C++"
            cppdialect "C++20"
            staticruntime "Off"
            location(folder)

            -- C++ Module Support
            allmodulespublic "On"
            scanformoduledependencies "true"

            -- Build Directories
            targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/plugins/" .. pluginName)
            objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/plugins/" .. pluginName)

            -- Files types to compile
            files { folder .. "/core/**.ixx", folder .. "/core/**.cpp" }

            -- Retrieve all plugin dependencies
            pluginIncludeDirs = { "%{wks.location}/core" }
            pluginLibDirs = {}
            pluginLinks = { "AurionCore" }

            -- Subdirectories in the 'third_party' directory indicate a dependency
            print("[Premake5] -- Checking for dependencies...")
            for _, depFolder in ipairs(os.matchdirs(folder .. "/third_party/".. "*")) do
                print("[Premake5] ---- Dependency Found: " .. path.getname(depFolder))
                -- Only two subdirectories should exist within the dependency directory:
                --  /include - Header files for the dependency
                --  /lib - Library files for the dependency
                for _, depSubfolder in ipairs(os.matchdirs(depFolder .. "/*")) do
                    if path.getname(depSubfolder) == "include" then
                        print("[Premake5] ------ Include Directory Found!")
                        table.insert(pluginIncludeDirs, depFolder .. "/include")
                    end
                    if path.getname(depSubfolder) == "lib" then
                        print("[Premake5] ------ Library folder found!")
                        table.insert(pluginLibDirs, depFolder .. "/lib")
                        table.insert(pluginLinks, depFolder .. "/lib/*.lib")
                    end
                end
            end

            print("[Premake5] Linking plugin dependencies...")
            -- Map Plugin Dependencies
            includedirs(pluginIncludeDirs)
            libdirs(pluginLibDirs)
            links(pluginLinks)

            print("[Premake5] Defining global macros...")

            -- Global project defines
            defines { "AURION_DLL" }

            print("[Premake5] Setting post-build commands...")

            postbuildcommands {
                "{MKDIR} %{wks.location}/Sandbox/plugins",
                "{COPYFILE} %{wks.location}/build/bin/" .. outputdir .. "/plugins/" .. pluginName .. "/*.dll %{wks.location}/Sandbox/plugins/"
            }

            print("[Premake5] Adding filters...")

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
            table.insert(projects, pluginName .. "Plugin")
            table.insert(names, pluginName)
    end

    print("[Premake5] Done!")

    return projects, names
end

function GenerateSandboxProject(pluginProjects, pluginNames)
    print("[Premake5] Adding Project: Sandbox")

    -- Sandbox project for testing
    project "Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "Off"

        -- C++ Module Support
        allmodulespublic "On"
        scanformoduledependencies "true"
    
        location("Sandbox")

        targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	    objdir ("%{wks.location}/build/bin-int/" .. outputdir .. "/%{prj.name}")

        files { "Sandbox/**.ixx", "Sandbox/**.h", "Sandbox/**.cpp" }

        includedirs { "%{wks.location}/core" }

        print("[Premake5] Linking project dependencies...")

        -- Link Core Framework
        links { "AurionCore" }

        -- Link All plugins
        for _, plugin in ipairs(pluginProjects) do
            links { plugin }
        end

        print("[Premake5] Adding filters...")

        -- Platform (OS) Filters
        filter "system:windows"
		    systemversion "latest"

            defines { "AURION_PLATFORM_WINDOWS" }

            postBuildCmds = {}

            print("[Premake5] Applying Windows post-build commands...")

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

        print("[Premake5] Done!")
end

-- Generate Core solution
GenerateCoreSolution()

-- Generate sandbox testing project, linking all plugins
GenerateSandboxProject(GeneratePluginProjects())