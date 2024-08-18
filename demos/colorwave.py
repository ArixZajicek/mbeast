import socket
import colorsys
import random
import math
import time

# Set the path for the Unix socket
socket_path = '/tmp/mbeast_driver.socket'

mod = 10

while True:
    try:
        # Create a socket object
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

        # Connect to the socket
        sock.connect(socket_path)

        rgb = colorsys.hsv_to_rgb((time.time() % mod) / mod, 1, 1)
        ints = [17, math.floor(rgb[0] * 255), math.floor(rgb[1] * 255), math.floor(rgb[2] * 255)]
        msg = bytearray(ints)

        # Send data
        sock.sendall(msg)
        

    finally:
        # Close the socket
        sock.close()
    
    time.sleep(0.1)