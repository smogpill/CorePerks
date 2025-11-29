local m = {}
m.define_suffix = "VULKAN"
function m.add_dependency()
	includedirs "%VULKAN_SDK%/Include"
	filter "kind:WindowedApp"
		libdirs "%VULKAN_SDK%/Lib"
		links "vulkan-1"
	filter {}
end
return m
