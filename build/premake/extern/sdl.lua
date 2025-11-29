local m = {}
m.define_suffix = "SDL"
function m.add_dependency()
	includedirs { path.join(cp.extern_path, "SDL3/include") }
	cp.add_dependency("SDL")
	filter "kind:WindowedApp"
		--postbuildcommands { "{COPY} \"../libs/SDL3.dll\" \"%{cfg.targetdir}\"", "{ECHO} 'Copied SDL3.dll to %{cfg.targetdir}'" }
	filter {}
end
function m.add_project()
	externalproject "SDL"
   	location(path.join(cp.extern_path, "SDL3/VisualC/SDL"))
   	uuid "81CE8DAF-EBB2-4761-8E45-B71ABCCA8C68"
   	kind "SharedLib"
   	language "C++"
   	architecture "x64"
   	objdir(cp.obj_path)
	targetdir(cp.target_path)
end
return m
