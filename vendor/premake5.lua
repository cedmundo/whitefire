project "zlib"
  language "C"
  kind "StaticLib"
  warnings "off"
  targetdir "lib"
  defines { "N_FSEEKO" }
  files {"zlib/*.c", "zlib/*.h"}

  filter "system:windows"
    defines {"_WINDOWS"}

  filter "system:not windows"
    defines {"HAVE_UNISTD_H"}

project "zlib-test"
  language "C"
  kind "ConsoleApp"
  warnings "off"
  targetdir "bin/%{cfg.buildcfg}"

  dependson {"zlib"}
  links {"zlib"}

  defines { "N_FSEEKO" }
  files {"zlib/test/example.c"}

  filter "system:windows"
    defines {"_WINDOWS"}

  filter "system:not windows"
    defines {"HAVE_UNISTD_H"}

  postbuildcommands {
      "./bin/%{cfg.buildcfg}/zlib-test",
      "rm foo.gz"
  }
