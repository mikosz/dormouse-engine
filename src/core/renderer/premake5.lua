structure.library_project("renderer", function()
		links { "essentials", "graphics", "math" }
		includedirs { ponder_include_dir() };
	end
	)

project "renderer-unit-test"
	kind "WindowedApp"
	flags { "WinMain" }
	links { "tester" }
project "*"
