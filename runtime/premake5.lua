project "wferuntime"
    kind "StaticLib"
    language "C"
    targetdir "lib/%{cfg.buildcfg}"
    includedirs {"include"}
    files {"src/**.c", "src/**.h"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

    filter "platforms:linux"
        defines {"WFE_USE_STDINT", "WFE_USE_C11_ALIGNOF"}

project "wferuntime-tests"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"
    dependson {"wferuntime"}
    includedirs {"include"}
    files {"tests/**.c", "tests/**.h"}
    links {"wferuntime"}

    postbuildcommands {
        "./bin/%{cfg.buildcfg}/wferuntime-tests"
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
