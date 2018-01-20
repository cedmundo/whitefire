workspace "Whitefire"
    configurations { "Debug", "Release" }
    platforms {"Linux", "Windows"}

    filter "configurations:Debug"
        defines {"DEBUG", "_DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

    filter "system:windows"
      defines {"_WINDOWS"}

    filter "system:not windows"
      defines {"HAVE_UNISTD_H"}

    filter {}


include "vendor"
include "runtime"
-- include "tools"
-- include "editor"
