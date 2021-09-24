project "Tracer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "default"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- pchheader "vkcpch.h"
	-- pchsource "src/vkcpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/stb_image_write/**.h",
		"vendor/stb_image_write/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	defines
	{
	}

	includedirs
	{
		"src",
		-- "vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		-- "%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.stb_image_write}",
		-- "%{IncludeDir.vma}",
		-- "%{IncludeDir.VulkanSDK}"
	}

	links
	{
		"GLFW",
		-- "ImGui",
	}

	-- filter "files:vendor/tracy/TracyClient.cpp"
 	--    	flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "TC_DEBUG"
		runtime "Debug"
		symbols "on"
		editAndContinue "Off"

		-- links
		-- {
		-- 	"%{Library.ShaderC_Debug}",
		-- 	"%{Library.SPIRV_Cross_Debug}",
		-- 	"%{Library.SPIRV_Cross_GLSL_Debug}"
		-- }

	filter "configurations:Release"
		defines "TC_RELEASE"
		runtime "Release"
		optimize "on"

		-- links
		-- {
		-- 	"%{Library.ShaderC_Release}",
		-- 	"%{Library.SPIRV_Cross_Release}",
		-- 	"%{Library.SPIRV_Cross_GLSL_Release}"
		-- }

	filter "configurations:Dist"
		defines "TC_DIST"
		runtime "Release"
		optimize "on"

		-- links
		-- {
		-- 	"%{Library.ShaderC_Release}",
		-- 	"%{Library.SPIRV_Cross_Release}",
		-- 	"%{Library.SPIRV_Cross_GLSL_Release}"
		-- }
