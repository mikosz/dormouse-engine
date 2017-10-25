structure.library_project("math", function()
		includedirs(ponder_include_dir())
		link_ponder_libs()
		links { "essentials" }
	end
	)

project "math-unit-test"
	links { "graphics" } -- TODO: ????
