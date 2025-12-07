local m = {}
m.define_suffix = "RAPIDJSON"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "rapidjson/include") }
end
return m
