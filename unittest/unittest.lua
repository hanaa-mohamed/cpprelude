local catch_path = "deps/Catch"

project "unittest"
	language "C++"
	kind "ConsoleApp"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/unittest/")

	files {"inc/**.h", "src/**.cpp"}

	includedirs {
		"inc/",
		cpprelude_path .. "/inc/",
		catch_path .. "/include/"
	}

	links {"cpprelude"}

	filter "action:gmake"
		buildoptions {"-std=c++14"}
		linkoptions {"-pthread"}

	filter "configurations:debug"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x86_64"
		architecture "x64"