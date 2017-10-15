structure.library_project("engine", function()
		links { "essentials", "wm" }
		add_imgui_files()
		includedirs { ponder_include_dir(), imgui_include_dir() };
	end
	)
