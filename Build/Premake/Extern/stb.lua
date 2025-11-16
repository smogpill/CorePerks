local m = {}
m.defineSuffix = "STB"
function m.AddDependency()
	cp.AddDependencyDefine(m)
	includedirs { path.join(cp.externPath, "stb") }
end
return m