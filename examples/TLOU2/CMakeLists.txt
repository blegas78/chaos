
project( tlou2 )

include_directories( include )

add_executable( tlou2	src/main.cpp
			src/menuing.cpp
			include/menuing.h )

target_link_libraries( tlou2 ${LIBS} zmqpp zmq )



