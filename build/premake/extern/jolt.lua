local m = {}
m.define_suffix = "JOLT"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "JoltPhysics")}
	filter "kind:WindowedApp"
		links { "Jolt" }
		filter { "kind:WindowedApp", "configurations:Debug" }
			libdirs { path.join(cp.extern_path, "JoltPhysics/Build/VS2026_CL_Double/Debug") }
		filter { "kind:WindowedApp", "configurations:Dev" }
			libdirs { path.join(cp.extern_path, "JoltPhysics/Build/VS2026_CL_Double/Release") }
		filter { "kind:WindowedApp", "configurations:Release" }
			libdirs { path.join(cp.extern_path, "JoltPhysics/Build/VS2026_CL_Double/Distribution") }
	filter {}
end
return m
