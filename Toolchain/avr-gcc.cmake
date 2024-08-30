##########################################################################
# Executables in use
##########################################################################
find_program(AVR_CC avr-gcc REQUIRED)
find_program(AVR_CXX avr-g++ REQUIRED)
find_program(AVR_OBJCOPY avr-objcopy REQUIRED)
find_program(AVR_SIZE_TOOL avr-size REQUIRED)
find_program(AVR_OBJDUMP avr-objdump REQUIRED)
find_program(AVR_UPLOADTOOL avrdude REQUIRED)


##########################################################################
# Define CMake System
##########################################################################
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
# set(CMAKE_C_COMPILER ${AVR_CC})
# set(CMAKE_CXX_COMPILER ${AVR_CXX})

function(avr_configure MCU_NAME CPU_SPEED)
	set(MCU ${MCU_NAME} CACHE INTERNAL "CPU Type")
	set(CPU_FREQ ${CPU_SPEED} CACHE INTERNAL "CPU Frequency")

	add_compile_options(
		"-mmcu=${MCU}"
	)

	add_compile_definitions(
		"F_CPU=${CPU_FREQ}"
	)

	add_link_options(
		"-mmcu=${MCU}"
	)

endfunction()

function(avr_upload TARGET PORT BAUD)
	set(ELF ${TARGET}.elf CACHE INTERNAL "ELF file name")
	set(HEX ${TARGET}.hex CACHE INTERNAL "HEX file name")

	set_target_properties(
		${TARGET}
		PROPERTIES
			OUTPUT_NAME ${ELF}
	)

	add_custom_target(
		Size_${TARGET}
		COMMAND
			${AVR_OBJCOPY} -j .text -j .data -O ihex ${ELF} ${HEX}
		COMMAND
			${AVR_SIZE_TOOL} -C;--mcu=${MCU} ${ELF} | grep -vE "\"^\\(|^$$\""
		DEPENDS ${ELF}
		BYPRODUCTS ${HEX}
		COMMENT "ELF -> HEX"
	)

	add_custom_target(
		Upload_${TARGET}
		COMMAND
			${AVR_UPLOADTOOL} -l upload.log -p ${MCU} -c avrftdi -P ${PORT} -b ${BAUD} -U flash:w:${HEX} -v
		DEPENDS ${HEX}
		COMMENT "Upload"
	)
endfunction()
