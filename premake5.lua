-- Workspace Declarations
workspace "AurionCore"
    architecture "x64"
    startproject "Sandbox"

    configurations { "Debug", "Release", "Dist"}

    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

-- Function to generate core solution project
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

-- Function to generate sandbox project
function GenerateSandboxProject()
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

        print("[Premake5] Done!")
end

-- Generate Core solution
GenerateCoreSolution()

-- Generate sandbox testing project
GenerateSandboxProject()