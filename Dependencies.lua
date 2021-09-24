
-- Tracer Dependencies

-- VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Tracer/vendor/stb_image"
IncludeDir["stb_image_write"] = "%{wks.location}/Tracer/vendor/stb_image_write"
-- IncludeDir["vma"] = "%{wks.location}/Tracer/vendor/vma"
IncludeDir["GLFW"] = "%{wks.location}/Tracer/vendor/GLFW/include"
-- IncludeDir["ImGui"] = "%{wks.location}/Tracer/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/Tracer/vendor/glm"
IncludeDir["Glad"] = "%{wks.location}/Tracer/vendor/Glad/include"
-- IncludeDir["shaderc"] = "%{wks.location}/Tracer/vendor/shaderc/include"
-- IncludeDir["SPIRV_Cross"] = "%{wks.location}/Tracer/vendor/SPIRV-Cross"
-- IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

-- LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
-- LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Tracer/vendor/VulkanSDK/Lib"
-- LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/Tracer/vendor/VulkanSDK/Bin"

Library = {}
-- Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
-- Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

-- Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
-- Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
-- Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
-- Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

-- Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
-- Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
-- Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
