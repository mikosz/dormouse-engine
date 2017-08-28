structure.library_project(
	"logger",
	function()
		links { "configuration", "system" }
	end
	)

project "logger-unit-test"
	add_gmock_files()
project "*"
	