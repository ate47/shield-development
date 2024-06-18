newoption {
   trigger = "ci-build",
   description = "CI build define"
}


workspace "DynamicBinaryFile"
    startproject "DynamicBinaryFileTest"
    location "./build"
    configurations { 
        "Debug",
        "Release"
    }

    architecture "x86_64"
    platforms "x64"

    filter { "options:ci-build" }
        defines { "DBFLIB_CI_BUILD" }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Size"
        
    filter {} -- Reset filters

project "DynamicBinaryFileLibrary"
    kind "None"
    language "C++"
    cppdialect "C++20"
    targetdir "%{wks.location}/bin/"
    objdir "%{wks.location}/obj/"

    targetname "dbflib"
    
    files {
        "./src/lib/**.hpp",
        "./src/lib/**.cpp",
    }

    includedirs {
        "src/lib"
    }

    vpaths {
        ["*"] = "*"
    }


project "DynamicBinaryFileTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "%{wks.location}/bin/"
    objdir "%{wks.location}/obj/"

    targetname "dbftest"
    
    files {
        "./src/test/**.hpp",
        "./src/test/**.cpp",
    }

    includedirs {
        "src/lib",
        "src/test"
    }

    vpaths {
        ["*"] = "*"
    }
    links { "DynamicBinaryFileLibrary" }
    dependson "DynamicBinaryFileLibrary"
