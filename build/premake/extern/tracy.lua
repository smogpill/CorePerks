local m = {}
m.define_suffix = "TRACY"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "tracy/public") }
	filter { "configurations:debug or dev or profile" }
	defines { "TRACY_ENABLE" }
	filter {}
end
function m.add_project()
	cp.cpp_project("tracy")
	location(path.join(cp.extern_path, "tracy/public"))
	removefiles {"**"}
	files { path.join(cp.extern_path, "tracy/public/TracyClient.cpp") }
end
return m