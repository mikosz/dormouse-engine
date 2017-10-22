function add_imgui_files()
	files {
		path.join(_MAIN_SCRIPT_DIR, "external/imgui/imconfig.h"),
		path.join(_MAIN_SCRIPT_DIR, "external/imgui/imgui.h"),
		path.join(_MAIN_SCRIPT_DIR, "external/imgui/imgui.cpp"),
		path.join(_MAIN_SCRIPT_DIR, "external/imgui/imgui_draw.cpp"),
		path.join(_MAIN_SCRIPT_DIR, "external/imgui/imgui_internal.h")
	}
	
	vpaths { [ ".imgui" ] = path.join(_MAIN_SCRIPT_DIR, "external/imgui/**.*") }
end

function imgui_include_dir()
	return path.join(_MAIN_SCRIPT_DIR, "external/imgui")
end
