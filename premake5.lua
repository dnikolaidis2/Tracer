include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Tracer"
	architecture "x86_64"
	startproject "Tracer"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Tracer/vendor/GLFW"
	include "Tracer/vendor/Glad"
group ""

include "Tracer"
