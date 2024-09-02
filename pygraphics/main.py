import cairo
import math
import time
import platform
import os

from expr_standard import StandardExpressions
from output import Matrix, Win
import state

def running_on_pi():
    plat = platform.machine().lower()
    if plat.find('86') >= 0 or plat.find('amd') >= 0:
        print('Platform is x86/AMD64 based. Assuming dev mode.')
        return False
    elif plat.find('arm'):
        print('Platform is ARM. Assuming headless mode.')
        return True
    else:
        print(f'Machine "{plat}" is not recognized! Falling back to socket detection.')
        return os.access('/tmp/mbeast.socket')


def main():
    last_update = time.time()
    update_times = list()
    use_socket = running_on_pi()

    surface = cairo.ImageSurface(cairo.Format.RGB24, Matrix.WIDTH, Matrix.HEIGHT)
    ctx = cairo.Context(surface)
    ctx.set_antialias(cairo.ANTIALIAS_NONE)
    ctx.translate(Matrix.WIDTH/2, Matrix.HEIGHT/2)

    pat = cairo.LinearGradient(-128.0, -32.0, 128.0, 32.0)
    pat.add_color_stop_rgb(1, 0.7, 0, 0)
    pat.add_color_stop_rgb(0, 0.9, 0.7, 0.2)
    ctx.set_source(pat)
    ctx.set_line_width(3)

    output = None
    if not use_socket:
        output = Win()

    expr = StandardExpressions()

    while state.running:
        now = time.time()
        update_times.append(now)
        update_times = list([t for t in update_times if t > now - 1])

        ctx.save()
        ctx.rectangle(-Matrix.WIDTH/2, -Matrix.HEIGHT/2, Matrix.WIDTH, Matrix.HEIGHT)
        ctx.set_source_rgb(0, 0, 0)
        ctx.fill()
        ctx.restore()
        
        expr.draw(ctx, update_times[-1] - update_times[-2] if len(update_times) >= 2 else 0)

        output.draw(surface.get_data())

        if (last_update + .5 < now):
            print(f'FPS: {len(update_times)}')
            last_update = now

if __name__ == '__main__':
    exit(main())