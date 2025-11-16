local m = {}
m.defineSuffix = "VMA"
function m.AddDependency()
	cp.AddDependencyDefine(m)
	includedirs { path.join(cp.externPath, "VulkanMemoryAllocator/include") }
end
return m