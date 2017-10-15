structure.library_project("tester", function()
		includedirs(ponder_include_dir())
		links { "wm", "engine" }
	end
	)
