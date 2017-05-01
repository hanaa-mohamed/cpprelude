workspace "cpprelude"
	configurations {"debugShared", "releaseShared", "debugStatic", "releaseStatic"}
	platforms {"x86", "x86_64"}
	location "build"
	startproject "scratch"

project "cpprelude"
	language "C++"
	targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
	location "build/cpprelude"

	files {"cpprelude/inc/**.h", "cpprelude/src/**.cpp"}

	includedirs {"cpprelude/inc/"}

	filter "action:gmake"
		buildoptions {"-std=c++14"}
		linkoptions {"-pthread"}

	filter "configurations:debugShared"
		kind "SharedLib"
		defines {"DEBUG", "COMPILE_DYNAMIC_LIB"}
		symbols "On"

	filter "configurations:releaseShared"
		kind "SharedLib"
		defines {"NDEBUG", "COMPILE_DYNAMIC_LIB"}
		optimize "On"

	filter "configurations:debugStatic"
		kind "StaticLib"
		defines {"DEBUG", "STATIC_LIB"}
		symbols "On"

	filter "configurations:releaseStatic"
		kind "StaticLib"
		defines {"NDEBUG", "STATIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x86_64"
		architecture "x64"

project "scratch"
	language "C++"
	targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
	kind "ConsoleApp"
	location "build/scratch"

	files {"scratch/inc/**.h", "scratch/src/**.cpp"}

	includedirs {"scratch/inc/", "cpprelude/inc/"}

	links {"cpprelude"}

	filter "action:gmake"
		buildoptions {"-std=c++14"}
		linkoptions {"-pthread"}

	filter "configurations:debugShared"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:releaseShared"
		defines {"NDEBUG"}
		optimize "On"

	filter "configurations:debugStatic"
		defines {"DEBUG", "STATIC_LIB"}
		symbols "On"

	filter "configurations:releaseStatic"
		defines {"NDEBUG", "STATIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x86_64"
		architecture "x64"

project "unittest"
	language "C++"
	targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
	kind "ConsoleApp"
	location "build/unittest"

	files {"unittest/inc/**.h", "unittest/src/**.cpp"}

	includedirs {"unittest/inc/", "cpprelude/inc/", "unittest/deps/catch/include"}

	links {"cpprelude"}

	filter "action:gmake"
		buildoptions {"-std=c++14"}
		linkoptions {"-pthread"}

	filter "configurations:debugShared"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:releaseShared"
		defines {"NDEBUG"}
		optimize "On"

	filter "configurations:debugStatic"
		defines {"DEBUG", "STATIC_LIB"}
		symbols "On"

	filter "configurations:releaseStatic"
		defines {"NDEBUG", "STATIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x86_64"
		architecture "x64"