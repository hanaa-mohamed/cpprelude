project "cpprelude"
	language "C++"
	kind "SharedLib"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/cpprelude/")

	files {"inc/**.h", "src/**.cpp"}

	includedirs {"inc/"}

	filter "action:gmake"
		buildoptions {"-std=c++14"}
		linkoptions {"-pthread"}

	filter "configurations:debug"
		targetsuffix "d"
		defines {"DEBUG", "COMPILE_DYNAMIC_LIB"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG", "COMPILE_DYNAMIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x86_64"
		architecture "x64"