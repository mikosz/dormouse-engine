structure.library_project(
	"configuration",
	function()
		links { "exceptions", "system" }
	end
	)

project "configuration-unit-test"
	includedirs(ponder_include_dir())
	link_ponder_libs()
	add_gmock_files()
