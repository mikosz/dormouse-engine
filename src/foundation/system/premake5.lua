structure.library_project(
	"system",
	function()
		includedirs(ponder_include_dir())
		link_ponder_libs()
		link_boost_libs { "filesystem" }
		links { "exceptions" }
	end	
	)
