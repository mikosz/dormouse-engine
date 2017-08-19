structure.library_project(
	"configuration",
	function()
		links { "exceptions", "system" }
	end
	)

project "configuration-unit-test"
	add_gmock_files()
