cmake_minimum_required(VERSION 3.20.0)

project(
	UART
)

add_library(
	${PROJECT_NAME}
)

target_compile_options(
	${PROJECT_NAME}
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
		"-Wconversion" # Warn about implicit conversions
		"-Wuninitialized" # Warn about uninitialized’ variables
		"-Wpadded" # Warn about padded structures
		"-flto" # Link time optimization
		"-fno-fat-lto-objects" # Link time optimization
		"-ffunction-sections" # Link time optimization help
		"-fdata-sections" # Link time optimization help
		"-fanalyzer" # Static analysis of program flow
)

target_link_options(
	${PROJECT_NAME}
	PRIVATE
		"-Wl,--gc-sections" # Remove unwanted/unused sections
		"-flto" # Link time optimization
		"-fno-fat-lto-objects" # Link time optimization
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		include
)

target_sources(
	${PROJECT_NAME}
	PUBLIC
		include/uart.h
	PRIVATE
		src/structure.h
		src/instance.c
		src/configure.c
		src/transfer.c
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		Buffers
)
