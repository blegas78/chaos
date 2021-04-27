cmake_minimum_required(VERSION 3.0)

project( chaos )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
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
						src/sequence.cpp
						src/menuing.cpp
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

add_executable( chaos	src/main.cpp )
add_executable( passthrough-hid	src/passthroughHID.cpp )

add_executable( hid-example src/hid-example.c )
add_executable( gadget src/gadget.c )

find_package (Threads REQUIRED)

find_package (LibJsoncpp REQUIRED)
include_directories(${LIBJSONCPP_INCLUDE_DIRS})
set(LIBS ${LIBS} jsoncpp)

#find_package (LibMogi REQUIRED)
#include_directories(${LIBMOGI_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${LIBMOGI_LIBRARIES})
set(LIBS ${LIBS} pthread mogi)

find_package (Libusb1 REQUIRED)
include_directories(${LIBUSB1_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LIBUSB1_LIBRARIES})

#set(LIBS ${LIBS} pthread )

if( NOT APPLE )
	#set(LIBS ${LIBS} wiringPi crypt rt )
	set(LIBS ${LIBS} crypt rt )
endif()

target_link_libraries( chaos ${LIBS} zmqpp zmq )
target_link_libraries( passthrough-hid ${LIBS} zmqpp zmq )
						
#add_executable( zmq_client src/zmq_client.cpp )
#add_executable( zmq_server src/zmq_server.cpp )
#target_link_libraries( zmq_client zmqpp zmq )
#target_link_libraries( zmq_server zmqpp zmq )

