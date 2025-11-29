local m = {}
m.define_suffix = "STB"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "stb") }
end
return m