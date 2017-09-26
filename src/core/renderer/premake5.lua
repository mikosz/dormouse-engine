structure.library_project("renderer", function()
		links { "essentials", "graphics", "math" }
	end
	)

project "renderer-unit-test"
	kind "WindowedApp"
	flags { "WinMain" }
	links { "wm", "tester" }
project "*"
