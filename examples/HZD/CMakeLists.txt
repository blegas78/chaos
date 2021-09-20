
project( hzd )

include_directories( include )

add_executable( hzd	src/main.cpp )

target_link_libraries( hzd ${LIBS} zmqpp zmq )



