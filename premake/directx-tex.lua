function directx_tex_include_dir()
	return DIRECTX_TEX_ROOT
end

function link_directx_tex_libs()
	local directx_tex_lib = path.join(DIRECTX_TEX_ROOT, "DirectXTex/Bin")
	
	if _ACTION == "vs2017" then
		directx_tex_lib = path.join(directx_tex_lib, "Desktop_2017")
	end
	
	filter { "Debug*", "platforms:Win64" }
		libdirs { path.join(directx_tex_lib, "x64/Debug") }
	
	filter { "Release*", "platforms:Win64" }
		libdirs { path.join(directx_tex_lib, "x64/Release") }
	
	filter {}
	
	links { "DirectXTex" }
end
