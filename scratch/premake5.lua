workspace "cpprelude"
	configurations {"debugShared", "releaseShared", "debugStatic", "releaseStatic"}
	platforms {"x86", "x86_64"}
	location "../build"

project "scratch"
	language "C++"
	targetdir "../bin/%{cfg.platform}/%{cfg.buildcfg}"
	kind "ConsoleApp"
	location "../build/scratch"

	files {"inc/**.h", "src/**.cpp"}

	includedirs {"inc/", "../cpprelude/inc/"}

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
