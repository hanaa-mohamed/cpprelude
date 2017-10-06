bin_path 		= path.getabsolute("bin")
build_path 		= path.getabsolute("build")
cpprelude_path 	= path.getabsolute("cpprelude")

workspace "cpprelude"
	configurations {"debug", "release"}
	platforms {"x86", "x64"}
	location "build"
	startproject "scratch"

	include "cpprelude/cpprelude.lua"
	include "scratch/scratch.lua"
	include "unittest/unittest.lua"