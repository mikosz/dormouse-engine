structure.library_project("reflection", function()
		includedirs(ponder_include_dir())
		link_ponder_libs()
		links { "essentials" }
	end
	)
