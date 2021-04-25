#   https://zeromq.org/get-started/?language=python&library=pyzmq#
#
#   Hello World client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "Hello" to server, expects "World" back
#

import zmq

context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server…")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5556")

#  Do 10 requests, waiting each time for a response
for request in range(10):
    print(f"Sending request {request} …")
    socket.send(b"{\"pause\":0}")

    #  Get the reply.
    message = socket.recv()
    print(f"Received reply {request} [ {message} ]")
