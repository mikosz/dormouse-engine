structure.library_project("essentials", function()
		includedirs(ponder_include_dir())
		link_ponder_libs()
		links { "exceptions" }
	end
	)

project "essentials-unit-test"
	add_gmock_files()
	