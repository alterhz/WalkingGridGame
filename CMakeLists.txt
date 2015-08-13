cmake_minimum_required (VERSION 2.8)

project (GameMD)

add_definitions(-D_UNICODE -DUNICODE)

if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501)
else()
	add_definitions(-DLINUX_ENGINE -DNDEBUG)
endif()

#platform
if(WIN32)
	set(PLATFORM_PATH_DIR ${CMAKE_SOURCE_DIR}/public/include/win32)
	set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/public/lib/win32)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/win32)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/public/lib/win32)
else()
	set(PLATFORM_PATH_DIR ${CMAKE_SOURCE_DIR}/public/include/linux)
	set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/public/lib/linux)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/linux)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/public/lib/linux)
endif()

enable_testing()

#add_subdirectory(gather)
add_subdirectory(libray)
add_subdirectory(game)