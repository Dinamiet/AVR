cmake_minimum_required(VERSION 3.20.0)

add_library(
	GPIO
)

target_compile_options(
	GPIO
	PRIVATE
		"-Wall" # Enable all common warnings
		"-Wextra" # Enable more warnings
		"-Wpedantic" # Warn about non-standard code
		"-Werror" # Warnings are treated as errors
		"-Wshadow" # Warn about shadow variables
		"-Wdouble-promotion" # Warn when promoting floats to doubles
		"-Wformat=2" # printf and scanf warnings
		"-Wformat-security" # printf and scanf warnings
		"-Wformat-truncation" # printf and scanf warnings
		"-Wundef" # Warn when evaluating not defined flags
		"-fno-common" # Warn about multiple global variables with the same name
		# "-Wconversion" # Warn about implicit conversions
		"-Wuninitialized" # Warn about uninitialized’ variables
		"-Wpadded" # Warn about padded structures
		"-flto" # Link time optimization
		"-fno-fat-lto-objects" # Link time optimization
		"-ffunction-sections" # Link time optimization help
		"-fdata-sections" # Link time optimization help
		"-fanalyzer" # Static analysis of program flow
)

target_link_options(
	GPIO
	PRIVATE
		"-Wl,--gc-sections" # Remove unwanted/unused sections
		"-flto" # Link time optimization
		"-fno-fat-lto-objects" # Link time optimization
)

target_include_directories(
	GPIO
	PUBLIC
		include
)

file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c")

target_sources(
	GPIO
	PUBLIC
		${INCLUDE_FILES}
	PRIVATE
		${SOURCE_FILES}
)
