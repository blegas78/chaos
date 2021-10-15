
project( twitch-plays )


add_executable( twitch-plays	src/main.cpp )

target_link_libraries( twitch-plays ${LIBS} zmqpp zmq )



