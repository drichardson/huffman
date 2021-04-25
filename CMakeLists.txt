cmake_minimum_required (VERSION 3.4)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")
project (huffman)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
	add_definitions (-D_WIN32_WINNT=0x0400)
	add_definitions (-D_SCL_SECURE_NO_WARNINGS=1)
	add_definitions (-D_CRT_SECURE_NO_WARNINGS=1)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
endif()
if(UNIX)
	set(CMAKE_C_FLAGS "$ENV{CFLAGS} ${CMAKE_C_FLAGS} -Wall -Werror -D_POSIX_C_SOURCE=2")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_library(huffman huffman.c
)

if(WIN32)
	target_link_libraries(huffman ws2_32)
endif()

INSTALL(TARGETS huffman
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

set(HEADERS huffman.h
)

INSTALL(FILES ${HEADERS} DESTINATION include/huffman)

if (UNIX AND NOT APPLE AND NOT ANDROID)
	add_executable(huffcode huffcode.c)
	target_link_libraries(huffcode huffman)

	enable_testing()

	add_test(NAME check
		COMMAND ./run_tests.sh --huffcode ${CMAKE_BINARY_DIR}/huffcode
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)

	add_test(NAME valgrind_check
		COMMAND ./run_tests.sh --huffcode ${CMAKE_BINARY_DIR}/huffcode --use-valgrind
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
endif()
