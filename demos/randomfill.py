import socket
import colorsys
import random
import math
import time

# Set the path for the Unix socket
socket_path = '/tmp/mbeast_driver.socket'

while True:
    try:
        # Create a socket object
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

        # Connect to the socket
        sock.connect(socket_path)

        msg = bytearray([32]) + random.randbytes(256 * 64 * 3)

        # Send data
        sock.sendall(msg)

    finally:
        # Close the socket
        sock.close()
    
    time.sleep(0.01)