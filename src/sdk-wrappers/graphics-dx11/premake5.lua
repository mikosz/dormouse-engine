include "directx-tex.lua"

structure.library_project(
	"graphics",
	function()
		includedirs { directx_tex_include_dir(), ponder_include_dir() }
		links { "exceptions", "system", "logger" }
		link_directx_tex_libs()
	end
	)

project "graphics-unit-test"
	add_gmock_files()
