cmake_minimum_required(VERSION 2.6)
PROJECT(Labyrinth-Irrlicht)

SET(CMAKE_CXX_FLAGS_RELEASE "-O3 --std=gnu++11")
SET(CMAKE_CXX_FLAGS_DEBUG  "g --std=gnu++11")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g --std=gnu++11")

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

IF(CMAKE_CXX_FLAGS_DEBUG)
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
ELSE(CMAKE_CXX_FLAGS_DEBUG)
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
ENDIF(CMAKE_CXX_FLAGS_DEBUG)

SET(INCLUDE_LIBS Irrlicht)

INCLUDE_DIRECTORIES(
        "/usr/include/"
        "/usr/local/include/"
        "/usr/include/irrlicht"
		"./include/"
)

FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HDRS include/*.h)

LINK_DIRECTORIES(
	"/usr/lib/"
        "/usr/local/lib/"
        ${INCLUDE_DIRS}
)
ADD_EXECUTABLE(Labyrinth-Irrlicht 
	${SRCS}
	${HDRS}
	)

TARGET_LINK_LIBRARIES(Labyrinth-Irrlicht
            ${INCLUDE_LIBS}
	)
