structure.library_project("reflection", function()
		print("including "..ponder_include_dir())
		includedirs(ponder_include_dir())
		link_ponder_libs()
	end
	)
