
project( hzd )

include_directories( include )

add_executable( hzd	src/mainHZD.cpp )

target_link_libraries( hzd ${LIBS} zmqpp zmq )

install(TARGETS hzd DESTINATION bin)

