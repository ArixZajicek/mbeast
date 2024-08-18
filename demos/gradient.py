import socket
import colorsys
import random
import math
import time

# Set the path for the Unix socket
socket_path = '/tmp/mbeast_driver.socket'

img = bytearray(256 * 64 * 3)
counter = 0

while True:
    if (counter + 2 >= 256 * 64 * 3):
        counter = 0
        img = bytearray(256 * 64 * 3)
    
    rgb = colorsys.hsv_to_rgb((counter % (256 * 3)) / 255, 1, 1)
    img[counter] = math.floor(rgb[0] * 255)
    img[counter + 1] = math.floor(rgb[1] * 255)
    img[counter + 2] = math.floor(rgb[2] * 255)
    counter = counter + 3
    
    try:
        # Create a socket object
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

        # Connect to the socket
        sock.connect(socket_path)

        # Send data
        sock.sendall(bytearray([32]) + img)
        
    finally:
        # Close the socket
        sock.close()
    
    # time.sleep(0.01)