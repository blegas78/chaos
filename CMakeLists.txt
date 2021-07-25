cmake_minimum_required(VERSION 3.0)

project( chaos )

# Extract current engine version
file(READ "${CMAKE_CURRENT_LIST_DIR}/include/version.h" file_contents)
string(REGEX MATCH "CHAOS_VERSION_MAJOR ([0-9]+)" _ "${file_contents}")
if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(FATAL_ERROR "Could not extract major version number from include/version.h")
endif()
set(CHAOS_VERSION_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "CHAOS_VERSION_MINOR ([0-9]+)" _ "${file_contents}")
if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(FATAL_ERROR "Could not extract minor version number from include/version.h")
endif()
set(CHAOS_VERSION_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "CHAOS_VERSION_PATCH ([0-9]+)" _ "${file_contents}")
if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(FATAL_ERROR "Could not extract patch version number from spdlog/version.h")
endif()
set(CHAOS_VERSION_PATCH ${CMAKE_MATCH_1})

set(CHAOS_VERSION "${CHAOS_VERSION_MAJOR}.${CHAOS_VERSION_MINOR}.${CHAOS_VERSION_PATCH}")

include(GNUInstallDirs)

# Compiler configuration

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory( nico/edisonLibmogiPackage/libmogi )
include_directories( nico/edisonLibmogiPackage/libmogi/include )

add_subdirectory(zmqpp)
include_directories( zmqpp/src )

add_subdirectory(usb-sniffify)
include_directories( usb-sniffify/include )

include_directories( include )

add_library( chaos-engine src/interface.cpp
	     		  src/command-sender.cpp
			  src/command-listener.cpp
			  src/device.cpp
			  src/controller.cpp
			  src/controller-gpio.cpp
			  src/controller-raw.cpp
			  src/controller-state.cpp
			  src/controller-state-dualsense.cpp
			  src/controller-state-dualshock.cpp
			  src/mouse.cpp
			  src/joystick.cpp
			  src/chaosEngine.cpp
			  src/modifier.cpp
			  include/modifier.h
			  src/sequence.cpp
			  include/sequence.h
			  src/chaos-uhid.cpp
			  include/interface.h
			  include/device.h
			  include/controller.h
			  include/controller-state.h
			  include/ps4-generated.h
			  include/ps5-generated.h
			  include/magicpro-generated.h
			  include/deviceTypes.h
			  include/chaos-uhid.h
			  include/chaos.h )

set(LIBS ${LIBS} chaos-engine sniffify)

add_executable( passthrough-hid	src/passthroughHID.cpp )

find_package (Threads REQUIRED)
find_package (LibJsoncpp REQUIRED)
include_directories(${LIBJSONCPP_INCLUDE_DIRS})
set(LIBS ${LIBS} jsoncpp)

set(LIBS ${LIBS} pthread mogi)

find_package (Libusb1 REQUIRED)
include_directories(${LIBUSB1_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LIBUSB1_LIBRARIES})

if( NOT APPLE )
	set(LIBS ${LIBS} crypt rt )
endif()

target_link_libraries( passthrough-hid ${LIBS} zmqpp zmq )
						
add_subdirectory(games)


