cmake_minimum_required(VERSION 3.20.0)

project(
	ATMega328P
)

# Doxygen document generation
find_package(Doxygen)
if (DOXYGEN_FOUND AND NOT DOXYGEN_IN)

	# Set input configuration
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

	# Add documentation target
	add_custom_target(
		docs
		COMMAND
			${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
		WORKING_DIRECTORY
			${CMAKE_CURRENT_BINARY_DIR}
		COMMENT
			"Generate documentation with Doxygen"
		VERBATIM
	)
endif()

add_subdirectory(lib)
add_subdirectory(GPIO)
add_subdirectory(UART)
add_subdirectory(I2C)
