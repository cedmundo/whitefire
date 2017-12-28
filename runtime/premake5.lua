project "wferuntime"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    defines {"WFE_USE_STDINT", "WFE_USE_C11_ALIGNOF"}
    targetdir "lib/%{cfg.buildcfg}"

    includedirs {"include"}
    files {"src/**.c", "src/**.h"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

project "wferuntime-test"
    dependson {"wferuntime"}

    kind "ConsoleApp"
    language "C"
    cdialect "C11"
    defines {"WFE_USE_STDINT", "WFE_USE_C11_ALIGNOF"}
    targetdir "bin/%{cfg.buildcfg}/Tests"

    includedirs {"include"}
    files {"tests/**.c", "tests/**.h"}
    links {"wferuntime"}

    postbuildcommands {
        "./bin/%{cfg.buildcfg}/Tests/wferuntime-test"
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
