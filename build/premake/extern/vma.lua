local m = {}
m.define_suffix = "VMA"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "VulkanMemoryAllocator/include") }
end
return m