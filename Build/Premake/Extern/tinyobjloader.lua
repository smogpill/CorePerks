local m = {}
m.defineSuffix = "TINYOBJLOADER"
function m.AddDependency()
	cp.AddDependencyDefine(m)
end
return m
