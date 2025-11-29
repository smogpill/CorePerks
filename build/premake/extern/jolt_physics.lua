local m = {}
m.define_suffix = "JOLT_PHYSICS"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "JoltPhysics")}
	filter "kind:WindowedApp"
	links { "Jolt" }
	filter { "kind:WindowedApp", "configurations:Debug" }
		libdirs { path.join(cp.extern_path, "JoltPhysics/Build/VS2022_CL_Double/Debug") }
	filter { "kind:WindowedApp", "configurations:Release" }
		libdirs { path.join(cp.extern_path, "JoltPhysics/Build/VS2022_CL_Double/Release") }
	filter {}
end
return m
