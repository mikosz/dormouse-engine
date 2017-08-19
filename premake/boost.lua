function link_boost_libs(names)
	for _, name in pairs(names) do
		name = "boost_" .. name

		local prefix = ""
		if BOOST_USE_STATIC_LIBS then
			prefix = "lib"
		end
		
		local mt_part = ""
		if BOOST_MULTITHREADED then
			mt_part = "-mt"
		end
		
		local toolset_part = ""
		if BOOST_TOOLSET then
			toolset_part = "-" .. BOOST_TOOLSET
		end

		local version_part = ""
		if BOOST_VERSION then
			version_part = "-" .. BOOST_VERSION
		end
		
		filter "configurations:Debug*"
			links(prefix .. name .. toolset_part .. mt_part .. "-gd" .. version_part)
		filter "configurations:Release*"
			links(prefix .. name .. toolset_part .. mt_part .. version_part)
		filter {}
	end
end
