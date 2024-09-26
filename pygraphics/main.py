import cairo
import math
import time
import platform
import os
from colorsys import hsv_to_rgb 

from expr_standard import StandardExpressions
from output import Matrix, Win
import state

TARGET_FPS = 120

def main():
    plat = platform.machine().lower()
    if plat.find('86') >= 0 or plat.find('amd') >= 0:
        print('Platform is x86/AMD64 based. Assuming dev mode.')
        state.use_socket = False
    elif plat.find('arm'):
        print('Platform is ARM. Assuming headless mode.')
        state.use_socket = True
    else:
        print(f'Machine "{plat}" is not recognized! Falling back to socket detection.')
        state.use_socket = os.access('/tmp/mbeast.socket')

    last_update = time.time()
    update_times = list()

    # cairo initialization
    surface = cairo.ImageSurface(cairo.Format.RGB24, Matrix.WIDTH, Matrix.HEIGHT)
    ctx = cairo.Context(surface)
    ctx.set_antialias(cairo.ANTIALIAS_NONE)
    ctx.translate(Matrix.WIDTH/2, Matrix.HEIGHT/2)


    ctx.set_line_width(3)

    output = None
    if state.use_socket:
        output = Matrix()
    else:
        output = Win()

    expr = StandardExpressions()

    while state.running:
        now = time.time()
        update_times.append(now)
        update_times = list([t for t in update_times if t >= now - 1])
        delta = now - update_times[-2] if len(update_times) >= 2 else 0

        ctx.save()
        ctx.rectangle(-Matrix.WIDTH/2, -Matrix.HEIGHT/2, Matrix.WIDTH, Matrix.HEIGHT)
        ctx.set_source_rgb(0, 0, 0)
        ctx.fill()
        ctx.restore()
        
        expr.draw(ctx, delta)

        output.draw(surface.get_data())

        sync_delay = 1 / TARGET_FPS - (time.time() - now)
        if sync_delay > 0:
            time.sleep(sync_delay)

        if (last_update + 1 < now):
            if state.use_socket:
                print(f'FPS: {len(update_times)}')
            last_update = now

if __name__ == '__main__':
    exit(main())