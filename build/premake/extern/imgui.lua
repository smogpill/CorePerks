local m = {}
m.define_suffix = "IMGUI"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "imgui") }
end
function m.add_project()
	cp.cpp_project("imgui")
	location(path.join(cp.extern_path, "imgui"))
	includedirs { path.join(cp.extern_path, "imgui") }
	removefiles {"**"}
	files { path.join(cp.extern_path, "imgui/*.h") }
	files { path.join(cp.extern_path, "imgui/*.cpp") }
	if cp.module_used(cp.sdl) then
		files { path.join(cp.extern_path, "imgui/backends/imgui_impl_sdl3.*") }
	end
	if cp.module_used(cp.vulkan) then
		files { path.join(cp.extern_path, "imgui/backends/imgui_impl_vulkan.*") }
	end
end
return m
