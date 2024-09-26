import pygame
import socket
import time

import state

class Matrix:
    WIDTH, HEIGHT = 256, 64

    def connect(self):
        while not self.connected:
            try:
                self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
                self.sock.connect(state.socket_path)
                self.connected = True
            except e:
                # Failed to connect
                print(f'Could not connect to {state.socket_path}. Trying again in 500ms...')
                time.sleep(0.5)

    def __init__(self):
        self.next_cmd = bytearray([32]) + bytearray(self.WIDTH * self.HEIGHT * 3)
        self.connected = False
        self.connect()

    def draw(self, data):
        if not self.connected:
            self.connect()

        i_dest = 1
        for i_src in range(self.WIDTH * self.HEIGHT * 4):
            if i_src % 4 != 3:
                self.next_cmd[i_dest] = data[i_src]
                i_dest = i_dest + 1
                
        self.sock.sendall(self.next_cmd)
    
    def cleanup(self):
        sock.close()
        


class Win:
    bg_color = (0, 0, 0)

    def __init__(self):
        self.screen = pygame.display.set_mode((Matrix.WIDTH * 4, Matrix.HEIGHT * 4))
        pygame.display.set_caption('mbeast')
        self.screen.fill(self.bg_color)
        pygame.display.flip()
    
    def draw(self, data):
        self.screen.fill(self.bg_color)
        tmp = pygame.image.frombuffer(data, (Matrix.WIDTH, Matrix.HEIGHT), 'RGBX')
        tmp = pygame.transform.scale(tmp, (Matrix.WIDTH * 4, Matrix.HEIGHT * 4), self.screen)
        pygame.display.flip()

        # Check for quit
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                state.running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RIGHT:
                    state.standard_expr = state.Standard.Angry
                elif event.key == pygame.K_DOWN:
                    state.standard_expr = state.Standard.Neutral
                elif event.key == pygame.K_UP:
                    state.standard_expr = state.Standard.Happy
                
                elif event.key == pygame.K_q:
                    state.running = False

    def cleanup(self):
        self
        # nothing to do
