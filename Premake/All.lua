abRootDir = path.getabsolute("..")
abExternDir = path.join(abRootDir, "Extern")
abSourceDir = path.join(abRootDir, "Source")
abBuildDir = path.join(abRootDir, "Build")
abProjectsDir = path.join(abBuildDir, "Projects")
abObjDir = path.join(abBuildDir, "Obj")
abTargetDir = path.join(buildDir, "Bin/$(Configuration)")
abWorkspacesDir = path.join(abBuildDir, "Workspaces")
abConfigurations = { "Debug", "Dev", "Profile", "Release" }
abPCH = "Precompiled"

function abSetProjectDefaults(name)
	project(name)
	location(abProjectsDir)
	architecture "x64"
	kind "StaticLib"
	objdir(abObjDir)
	targetdir("$(SolutionDir)$(Platform)/$(Configuration)")
	libdirs { "$(OutDir)" }
	includedirs("..")
	includedirs(abExternDir)

	debugdir "$(OutDir)"
	configurations(abConfigurations)

	filter{"configurations:Debug"}
		defines {"AB_DEBUG", "AB_DEBUG_OR_DEV"}
	filter{"configurations:Development"}
		defines {"AB_DEV", "AB_DEBUG_OR_DEV"}
	filter{"configurations:Profile"}
		defines {"AB_PROFILE"}
	filter{"configurations:Release"}
		defines {"AB_RELEASE"}
	filter {}
end

function abSetCppProjectDefaults(name)
	SetProjectDefaults(name)
	rtti "Off"
	language "C++"
	exceptionhandling "Off"
	vectorextensions "AVX"
	symbols "On"
	cppdialect "C++20"
	fatalwarnings { "All"}
	flags { "MultiProcessorCompile" }
	files { "**.cpp", "**.h", "**.hpp", "**.inl", "**.natvis", "**.lua" }

	if os.isfile(abPCH..".h") then
		pchheader(abPCH..".h")
		pchsource(abPCH..".cpp")
	end

	filter { "action:vs*" }
		files { "*.natvis"}
		defines { "_HAS_EXCEPTIONS=0" }
		buildoptions {"/Zc:preprocessor"} -- Required for C++20's __VA_OPT__() for some reason
		--flags { "StaticRuntime" }
		--linkoptions { "/ENTRY:mainCRTStartup" } -- TODO: Not working with DLL, should avoid that case somehow.
		linkoptions {"/ignore:4221"} -- warns when .cpp are empty depending on the order of obj linking.
		
	filter { "configurations:Release" }
		optimize "On"
		omitframepointer "On"
	filter {}
end

function abSetWorkspaceDefaults(name)
	workspace(name)
	configurations(abConfigurations)
	location "Workspaces"
end

function abSDL3()
	externalproject "SDL"
	   	location(abExternDir.."/SDL3/VisualC/SDL")
	   	uuid "81CE8DAF-EBB2-4761-8E45-B71ABCCA8C68"
	   	kind "SharedLib"
	   	language "C++"
	   	architecture "x64"
	   	objdir(abObjDir)
		targetdir(abTargetDir)
end