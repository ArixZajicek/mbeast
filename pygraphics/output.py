import pygame

import state

class Matrix:
    WIDTH, HEIGHT = 256, 64

    def __init__(self):
        self

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