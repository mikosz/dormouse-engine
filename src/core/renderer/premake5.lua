structure.library_project("renderer")

project "renderer-unit-test"
	kind "WindowedApp"
	flags { "WinMain" }
	links { "wm", "graphics", "tester" }
project "*"
