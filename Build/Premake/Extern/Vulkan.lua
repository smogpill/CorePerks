local m = {}
m.defineSuffix = "VULKAN"
function m.AddDependency()
	cp.AddDependencyDefine(m)
	includedirs "%VULKAN_SDK%/Include"
	filter "kind:WindowedApp"
		libdirs "%VULKAN_SDK%/Lib"
		links "vulkan-1"
	filter {}
end
return m
