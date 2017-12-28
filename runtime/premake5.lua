project "wferuntime"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    targetdir "lib/%{cfg.buildcfg}"

    includedirs {"include"}
    files {"src/**.c", "src/**.h"}

    filter "platforms:Linux"
        defines {"WFE_USE_STDDEF", "WFE_USE_STDINT"}

    filter "platforms:Windows"
        defines {"WFE_USE_MSVSCDEF", "WFE_USE_MSVSCDEF"}

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
    targetdir "bin/%{cfg.buildcfg}/Tests"

    includedirs {"include"}
    files {"tests/**.c", "tests/**.h"}
    links {"wferuntime"}

    postbuildcommands {
        "./bin/%{cfg.buildcfg}/Tests/wferuntime-test"
    }

    filter "platforms:Linux"
        defines {"WFE_USE_STDDEF", "WFE_USE_STDINT"}

    filter "platforms:Windows"
        defines {"WFE_USE_MSVSCDEF", "WFE_USE_MSVSCDEF"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
