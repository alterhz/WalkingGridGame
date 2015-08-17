cmake_minimum_required (VERSION 2.8)

#src
set(LIB_LIBRAY_SRC
	
	${CMAKE_SOURCE_DIR}/public/include/netio.h
	${CMAKE_SOURCE_DIR}/libray/netio.cpp

	${CMAKE_SOURCE_DIR}/public/include/autostr.h
	${CMAKE_SOURCE_DIR}/public/include/debug.h
	${CMAKE_SOURCE_DIR}/libray/network/debug.cpp

	${CMAKE_SOURCE_DIR}/public/include/vector2.h
	${CMAKE_SOURCE_DIR}/libray/src/vector2.cpp

	${CMAKE_SOURCE_DIR}/libray/network/service.h
	${CMAKE_SOURCE_DIR}/libray/network/service.cpp
	${CMAKE_SOURCE_DIR}/libray/network/acceptor.h
	${CMAKE_SOURCE_DIR}/libray/network/acceptor.cpp
	${CMAKE_SOURCE_DIR}/libray/network/connector.h
	${CMAKE_SOURCE_DIR}/libray/network/connector.cpp
	${CMAKE_SOURCE_DIR}/libray/network/socket.h
	${CMAKE_SOURCE_DIR}/libray/network/socket.cpp

	${CMAKE_SOURCE_DIR}/libray/boostinc.h
	${CMAKE_SOURCE_DIR}/libray/boostinc.cpp
	${CMAKE_SOURCE_DIR}/libray/boostsrc.cpp
	${CMAKE_SOURCE_DIR}/libray/include/ringbuffer.h
	${CMAKE_SOURCE_DIR}/libray/include/ringbuffer.cpp


	${CMAKE_SOURCE_DIR}/public/include/event.h
	${CMAKE_SOURCE_DIR}/libray/src/eventmanager.h
	${CMAKE_SOURCE_DIR}/libray/src/eventmanager.cpp

	${CMAKE_SOURCE_DIR}/public/include/db.h
	${CMAKE_SOURCE_DIR}/libray/src/dbmysql.h
	${CMAKE_SOURCE_DIR}/libray/src/dbmysql.cpp

	${CMAKE_SOURCE_DIR}/public/include/thread.h
	${CMAKE_SOURCE_DIR}/libray/src/thread.cpp
)

foreach(src ${LIB_LIBRAY_SRC})
	file(APPEND ${src})
endforeach(src)

# source_group begin
source_group("Public\\Include" REGULAR_EXPRESSION public/include)
source_group("NetWork" REGULAR_EXPRESSION libray/network)
source_group("Include" REGULAR_EXPRESSION libray/include)
source_group("Src" REGULAR_EXPRESSION libray/src)
# source_group end

set(BOOST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/boost_1_55_0)
set(MYSQL_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/mysql_5_6_20/include)
set(MYSQLPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/mysql++_3_2_1/include)


#include path
include_directories(
	${PLATFORM_PATH_DIR}
	${BOOST_SOURCE_DIR}
	${MYSQL_SOURCE_DIR}
	${MYSQLPP_SOURCE_DIR}

	${CMAKE_SOURCE_DIR}/public/include
	${CMAKE_SOURCE_DIR}/libray
	${CMAKE_SOURCE_DIR}/libray/network
	${CMAKE_SOURCE_DIR}/libray/include
	${CMAKE_SOURCE_DIR}/libray/src
)

#output
add_library(libray STATIC ${LIB_LIBRAY_SRC})

if(WIN32)
	target_link_libraries(libray)
	set_target_properties(libray
		PROPERTIES
		COMPILE_FLAGS "/wd4819 /wd4996"
	)
else()
	target_link_libraries(libray)
	set_target_properties(libray
		PROPERTIES
		COMPILE_FLAGS "-std=c++11 -g"
	)
endif()
