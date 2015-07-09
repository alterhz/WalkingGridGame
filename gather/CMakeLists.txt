cmake_minimum_required (VERSION 2.8)

#src
set(LIB_GATHER_SRC

	${CMAKE_SOURCE_DIR}/protocol/packet/ProtocolFactory.h
	${CMAKE_SOURCE_DIR}/protocol/packet/ProtocolFactory.cpp

	${CMAKE_SOURCE_DIR}/protocol/message/gpid.pb.h
	${CMAKE_SOURCE_DIR}/protocol/message/gpid.pb.cc
	${CMAKE_SOURCE_DIR}/protocol/message/gmsg.pb.h
	${CMAKE_SOURCE_DIR}/protocol/message/gmsg.pb.cc
	${CMAKE_SOURCE_DIR}/protocol/message/msginc.h

	${CMAKE_SOURCE_DIR}/gather/msgdispatch/msgdispatch.h
	${CMAKE_SOURCE_DIR}/gather/msgdispatch/msgdispatch.cpp

    ${CMAKE_SOURCE_DIR}/gather/net/client.h
    ${CMAKE_SOURCE_DIR}/gather/net/client.cpp
    ${CMAKE_SOURCE_DIR}/gather/net/clientmanager.h
    ${CMAKE_SOURCE_DIR}/gather/net/clientmanager.cpp
    ${CMAKE_SOURCE_DIR}/gather/net/connector.h
    ${CMAKE_SOURCE_DIR}/gather/net/connector.cpp

    ${CMAKE_SOURCE_DIR}/gather/public/app.h
    ${CMAKE_SOURCE_DIR}/gather/public/app.cpp

    ${CMAKE_SOURCE_DIR}/gather/db/dbevent.h
    ${CMAKE_SOURCE_DIR}/gather/db/dbevent.cpp
    ${CMAKE_SOURCE_DIR}/gather/db/dbconnection.h
    ${CMAKE_SOURCE_DIR}/gather/db/dbconnection.cpp

    ${CMAKE_SOURCE_DIR}/gather/main.cpp
)

foreach(src ${LIB_GATHER_SRC})
	file(APPEND ${src})
endforeach(src)

source_group("Base" REGULAR_EXPRESSION ../public/gamebase)
source_group("Message" REGULAR_EXPRESSION protocol/message)
source_group("Packet" REGULAR_EXPRESSION protocol/packet)
source_group("Net" REGULAR_EXPRESSION gather/net)
source_group("Public" REGULAR_EXPRESSION gather/public)
source_group("Db" REGULAR_EXPRESSION gather/db)
source_group("MsgDispatch" REGULAR_EXPRESSION gather/msgdispatch)


set(MYSQL_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../3rd/mysql_5_6_20/include)
set(MYSQLPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../3rd/mysql++_3_2_1/include)

#include path
include_directories(
	${PLATFORM_PATH_DIR}
	${MYSQL_SOURCE_DIR}
	${MYSQLPP_SOURCE_DIR}

	${CMAKE_SOURCE_DIR}/../3rd/tinyxml2
	${CMAKE_SOURCE_DIR}/../public/net
	${CMAKE_SOURCE_DIR}/../public/src
	${CMAKE_SOURCE_DIR}/../public/common
	${CMAKE_SOURCE_DIR}/protocol/message
	${CMAKE_SOURCE_DIR}/protocol/packet
	${CMAKE_SOURCE_DIR}/gather
	${CMAKE_SOURCE_DIR}/gather/net
	${CMAKE_SOURCE_DIR}/gather/public
	${CMAKE_SOURCE_DIR}/gather/msgdispatch
	${CMAKE_SOURCE_DIR}/gather/db
)

#link path
if(WIN32)
	link_directories(
		${CMAKE_SOURCE_DIR}/../3rd/mysql++_3_2_1/lib/win32/${CMAKE_BUILD_TYPE}/
		${CMAKE_SOURCE_DIR}/../3rd/mysql_5_6_20/lib/win32/${CMAKE_BUILD_TYPE}/
	)
else()
	link_directories(
		${CMAKE_SOURCE_DIR}/../3rd/mysql++_3_2_1/lib/linux/
		${CMAKE_SOURCE_DIR}/../3rd/mysql_5_6_20/lib/linux/
	)
endif()

if(WIN32)
	add_definitions(-DMYSQLPP_NO_DLL)
endif()

#output
add_executable(Gather ${LIB_GATHER_SRC})

if(WIN32)
	target_link_libraries(Gather mysqlpp mysqlclient libprotobuf libray)
	set_target_properties(Gather
		PROPERTIES
		COMPILE_FLAGS "/wd4819 /wd4996 /Zi" LINK_FLAGS "/DEBUG /SAFESEH:NO"
	)
else()
	#!!! mysqlpp need before mysqlclient.
	target_link_libraries(Gather mysqlpp mysqlclient protobuf libray dl pthread)
	set_target_properties(Gather
		PROPERTIES
		#COMPILE_FLAGS "-std=c++11 -g -Wall -O2"
		COMPILE_FLAGS "-std=c++11 -Wall -g"
	)
endif()
