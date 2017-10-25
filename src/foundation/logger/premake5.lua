structure.library_project(
	"logger",
	function()
		links { "configuration", "system" }
	end
	)

project "logger-unit-test"
	includedirs(ponder_include_dir())
	link_ponder_libs()
	add_gmock_files()
	