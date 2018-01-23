project "wferuntime"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    targetdir "lib/%{cfg.buildcfg}"

    includedirs {"include", "../vendor", "../vendor/msgpack-c/include"}
    files {"src/**.c", "src/**.h", "msgpack"}

    filter "platforms:Linux"
        defines {"WFE_USE_STDDEF", "WFE_USE_STDINT"}
        removefiles {"src/game_glfm.c"}

    filter "platforms:Windows"
        defines {"WFE_USE_MSVSCDEF", "WFE_USE_MSVSCDEF"}
        removefiles {"src/game_glfm.c"}

    filter {}

project "wferuntime-test"
    dependson {"wferuntime"}

    kind "ConsoleApp"
    language "C"
    cdialect "C11"
    targetdir "bin/%{cfg.buildcfg}/Tests"

    includedirs {"include", "../vendor", "../vendor/msgpack-c/include"}
    files {"tests/**.c", "tests/**.h"}
    links {"wferuntime", "glfw", "msgpack"}

    postbuildcommands {
        "./bin/%{cfg.buildcfg}/Tests/wferuntime-test"
    }

    filter "platforms:Linux"
        defines {"WFE_USE_STDDEF", "WFE_USE_STDINT"}
        removefiles {"src/game_glfm.c"}

    filter "platforms:Windows"
        defines {"WFE_USE_MSVSCDEF", "WFE_USE_MSVSCDEF"}
        removefiles {"src/game_glfm.c"}

    filter {}

