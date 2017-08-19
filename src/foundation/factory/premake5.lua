structure.library_project(
	"factory",
	function()
		links { "exceptions" }
	end
	)

project "factory-unit-test"
	add_gmock_files()
	