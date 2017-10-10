function ponder_include_dir()
	return path.join(PONDER_INSTALL_DIR, "include")
end

function link_ponder_libs()
	libdirs { path.join(PONDER_INSTALL_DIR, "lib") }
	
	filter { "not Release*", "platforms:Win64" }
		links { "ponderd" }
	filter { "Release*", "platforms:Win64" }
		links { "ponder" }
	filter {}
end
