local m = {}
m.defineSuffix = "SDL"
function m.AddDependency()
	cp.AddDependencyDefine(m)
	includedirs { path.join(cp.externPath, "SDL3/include") }
	cp.AddDependency("SDL")
	filter "kind:WindowedApp"
		--postbuildcommands { "{COPY} \"../libs/SDL3.dll\" \"%{cfg.targetdir}\"", "{ECHO} 'Copied SDL3.dll to %{cfg.targetdir}'" }
	filter {}
end
function m.AddProject()
	externalproject "SDL"
   	location(path.join(cp.externPath, "SDL3/VisualC/SDL"))
   	uuid "81CE8DAF-EBB2-4761-8E45-B71ABCCA8C68"
   	kind "SharedLib"
   	language "C++"
   	architecture "x64"
   	objdir(cp.objPath)
	targetdir(cp.targetPath)
end
return m
