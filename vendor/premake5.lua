project "zlib"
  language "C"
  kind "StaticLib"
  targetdir "lib"
  targetprefix ""
  defines { "N_FSEEKO" }
  files {
    'zlib/inffast.c',
    'zlib/deflate.c',
    'zlib/gzwrite.c',
    'zlib/infback.c',
    'zlib/compress.c',
    'zlib/gzclose.c',
    'zlib/inflate.c',
    'zlib/trees.c',
    'zlib/zutil.c',
    'zlib/inftrees.c',
    'zlib/adler32.c',
    'zlib/gzread.c',
    'zlib/crc32.c',
    'zlib/gzlib.c',
    'zlib/uncompr.c'
  }

project "libpng"
  language "C"
  kind "StaticLib"
  targetdir "lib"
  targetprefix ""
  includedirs {"misc"}

  files {
    "libpng/png.c",
    "libpng/pngerror.c",
    "libpng/pngget.c",
    "libpng/pngmem.c",
    "libpng/pngpread.c",
    "libpng/pngread.c",
    "libpng/pngrio.c",
    "libpng/pngrtran.c",
    "libpng/pngrutil.c",
    "libpng/pngset.c",
    "libpng/pngtrans.c",
    "libpng/pngwio.c",
    "libpng/pngwrite.c",
    "libpng/pngwtran.c",
    "libpng/pngwutil.c",
  }

  -- TODO: Configuration/Definitions?
  -- TODO: Add optimizations for architecture/system

project "msgpack"
  language "C"
  kind "StaticLib"
  targetdir "lib"
  targetprefix ""

  includedirs {"msgpack-c/include"}
  files {
    "msgpack-c/src/vrefbuffer.c",
    "msgpack-c/src/version.c",
    "msgpack-c/src/zone.c",
    "msgpack-c/src/objectc.c",
    "msgpack-c/src/unpack.c",
  }

project "zlib-test"
  language "C"
  kind "ConsoleApp"
  targetdir "bin/%{cfg.buildcfg}"

  dependson {"zlib"}
  links {"zlib"}
  files {"zlib/test/example.c"}

  postbuildcommands {
      "./bin/%{cfg.buildcfg}/zlib-test",
      "{DELETE} foo.gz"
  }

project "libpng-test"
  language "C"
  kind "ConsoleApp"
  targetdir "bin/%{cfg.buildcfg}"
  linkgroups "On"

  dependson {"zlib", "libpng"}
  links {"m", "zlib", "libpng"}
  includedirs {"misc"}
  files {"libpng/pngtest.c"}

  postbuildcommands {
      "{COPY} libpng/pngtest.png pngtest.png",
      "./bin/%{cfg.buildcfg}/libpng-test",
      "{DELETE} pngtest.png",
      "{DELETE} pngout.png"
  }

project "msgpack-test"
  language "C"
  kind "ConsoleApp"
  targetdir "bin/%{cfg.buildcfg}"
  dependson {"msgpack"}
  links {"msgpack"}
  includedirs {"msgpack-c/include"}

  files {"msgpack-c/example/c/simple_c.c"}
  postbuildcommands {
    "./bin/%{cfg.buildcfg}/msgpack-test"
  }

