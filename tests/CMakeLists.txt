include(CTest)
enable_testing()

file(GLOB V_GLOB LIST_DIRECTORIES true "*")
foreach(item ${V_GLOB})
	if(EXISTS "${item}/CMakeLists.txt")
		add_subdirectory("${item}")
	endif()
	get_filename_component(target_name "${item}" NAME)
	set(target_name "${target_name}-test")
	if(TARGET ${target_name})
		add_test(
			NAME ${target_name}
			COMMAND $<TARGET_FILE:${target_name}>
		)
	endif()
endforeach()