cmake_minimum_required (VERSION 2.8)

#src
set(LIB_GAME_SRC
	${CMAKE_SOURCE_DIR}/public/include/observer.h
	${CMAKE_SOURCE_DIR}/public/include/macrosdef.h

	${CMAKE_SOURCE_DIR}/protocol/message/gpid.pb.h
	${CMAKE_SOURCE_DIR}/protocol/message/gpid.pb.cc
	${CMAKE_SOURCE_DIR}/protocol/message/gmsg.pb.h
	${CMAKE_SOURCE_DIR}/protocol/message/gmsg.pb.cc
	${CMAKE_SOURCE_DIR}/protocol/message/msginc.h

	${CMAKE_SOURCE_DIR}/game/configread/configread.h
	${CMAKE_SOURCE_DIR}/game/configread/configread.cpp
	${CMAKE_SOURCE_DIR}/game/configread/xmldata.h
	${CMAKE_SOURCE_DIR}/game/configread/xmldata.cpp

	${CMAKE_SOURCE_DIR}/game/entity/attribute.h
	${CMAKE_SOURCE_DIR}/game/entity/attribute.cpp
	${CMAKE_SOURCE_DIR}/game/entity/action.h
	${CMAKE_SOURCE_DIR}/game/entity/action.cpp
	${CMAKE_SOURCE_DIR}/game/entity/sprite.h
	${CMAKE_SOURCE_DIR}/game/entity/sprite.cpp
	${CMAKE_SOURCE_DIR}/game/entity/role.h
	${CMAKE_SOURCE_DIR}/game/entity/role.cpp
	${CMAKE_SOURCE_DIR}/game/entity/monster.h
	${CMAKE_SOURCE_DIR}/game/entity/monster.cpp
	${CMAKE_SOURCE_DIR}/game/entity/country.h
	${CMAKE_SOURCE_DIR}/game/entity/country.cpp

	${CMAKE_SOURCE_DIR}/game/scene/scenemanager.h
	${CMAKE_SOURCE_DIR}/game/scene/scenemanager.cpp
	${CMAKE_SOURCE_DIR}/game/scene/scene.h
	${CMAKE_SOURCE_DIR}/game/scene/scene.cpp
	${CMAKE_SOURCE_DIR}/game/scene/landscene.h
	${CMAKE_SOURCE_DIR}/game/scene/landscene.cpp
	${CMAKE_SOURCE_DIR}/game/scene/instance.h
	${CMAKE_SOURCE_DIR}/game/scene/instance.cpp
	${CMAKE_SOURCE_DIR}/game/scene/zone.h
	${CMAKE_SOURCE_DIR}/game/scene/zone.cpp

	${CMAKE_SOURCE_DIR}/game/battle/ground.h
	${CMAKE_SOURCE_DIR}/game/battle/ground.cpp
	${CMAKE_SOURCE_DIR}/game/battle/battleground.h
	${CMAKE_SOURCE_DIR}/game/battle/battleground.cpp

	${CMAKE_SOURCE_DIR}/game/msgdispatch/logindispatcher.h
	${CMAKE_SOURCE_DIR}/game/msgdispatch/logindispatcher.cpp
	${CMAKE_SOURCE_DIR}/game/msgdispatch/gamedispatcher.h
	${CMAKE_SOURCE_DIR}/game/msgdispatch/gamedispatcher.cpp

    ${CMAKE_SOURCE_DIR}/game/net/client.h
    ${CMAKE_SOURCE_DIR}/game/net/client.cpp
    ${CMAKE_SOURCE_DIR}/game/net/clientmanager.h
    ${CMAKE_SOURCE_DIR}/game/net/clientmanager.cpp

    ${CMAKE_SOURCE_DIR}/game/public/app.h
    ${CMAKE_SOURCE_DIR}/game/public/app.cpp

    ${CMAKE_SOURCE_DIR}/game/unittest/testclient.h
    ${CMAKE_SOURCE_DIR}/game/unittest/testclient.cpp

    ${CMAKE_SOURCE_DIR}/game/main.cpp
)


foreach(src ${LIB_GAME_SRC})
	file(APPEND ${src})
endforeach(src)


source_group("Message" REGULAR_EXPRESSION protocol/message)
source_group("Packet" REGULAR_EXPRESSION protocol/packet)
source_group("Net" REGULAR_EXPRESSION game/net)
source_group("Public" REGULAR_EXPRESSION game/public)
source_group("MsgDispatch" REGULAR_EXPRESSION game/msgdispatch)
source_group("UnitTest" REGULAR_EXPRESSION game/unittest)
source_group("Entity" REGULAR_EXPRESSION game/entity)
source_group("Scene" REGULAR_EXPRESSION game/scene)
source_group("Battle" REGULAR_EXPRESSION game/battle)
source_group("ConfigRead" REGULAR_EXPRESSION game/configread)


set(MYSQL_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/mysql_5_6_20/include)
set(MYSQLPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/mysql++_3_2_1/include)
set(PROTOBUF_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/protobuf-2.5.0/src)


#include path
include_directories(
	${PLATFORM_PATH_DIR}
	${MYSQL_SOURCE_DIR}
	${MYSQLPP_SOURCE_DIR}
	${PROTOBUF_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/3rd/tinyxml2
	${CMAKE_SOURCE_DIR}/public/include
	${CMAKE_SOURCE_DIR}/protocol/message
	${CMAKE_SOURCE_DIR}/game
	${CMAKE_SOURCE_DIR}/game/net
	${CMAKE_SOURCE_DIR}/game/public
	${CMAKE_SOURCE_DIR}/game/msgdispatch
	${CMAKE_SOURCE_DIR}/game/unittest
	${CMAKE_SOURCE_DIR}/game/entity
	${CMAKE_SOURCE_DIR}/game/scene
	${CMAKE_SOURCE_DIR}/game/battle
	${CMAKE_SOURCE_DIR}/game/configread
)


#link path
if(WIN32)
	link_directories(
		${CMAKE_SOURCE_DIR}/3rd/lib/win32/${CMAKE_BUILD_TYPE}/
	)
else()
	link_directories(
		${CMAKE_SOURCE_DIR}/3rd/lib/linux/
	)
endif()

if(WIN32)
	add_definitions(-DMYSQLPP_NO_DLL)
endif()

#output
add_executable(Game ${LIB_GAME_SRC})

if(WIN32)
	target_link_libraries(Game mysqlpp mysqlclient libprotobuf libray)
	set_target_properties(Game
		PROPERTIES
		COMPILE_FLAGS "/wd4819 /wd4996 /Zi" LINK_FLAGS "/DEBUG /SAFESEH:NO"
	)
else()
	#!!! mysqlpp need before mysqlclient.
	target_link_libraries(Game mysqlpp mysqlclient protobuf libray dl pthread)
	set_target_properties(Game
		PROPERTIES
		#COMPILE_FLAGS "-std=c++11 -g -Wall -O2"
		COMPILE_FLAGS "-std=c++11 -Wall -g"
	)
endif()
