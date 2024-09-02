import cairo
import time
import random
import state
import util
import math
from collections.abc import MutableSequence

class StandardExpressions:
    def __init__(self):
        self._eyes = Eyes()
        self._nose_shape = util.get_simple_quad((-3, -2), (1, -1), (3, 2), (-2, 2))
        self._mouth_shape = util.TShape([
            [-90, 31],
            [-70, 40, -50, 30, -30, 28],
            [-16, 20],
            [ 0, 25]])
        


    def draw(self, g: cairo.Context, delta: float = 1/60):
        g.save()

        g.translate(0, math.sin(time.time() * (math.pi / 2)))

        self._eyes.draw(g, delta)
        self._mouth_shape.draw(g, (0, 0, 1, 1))
        g.stroke()
        self._mouth_shape.draw(g, (0, 0, -1, 1))
        g.stroke()
        self._nose_shape.draw(g, (-25, -20, 2, 1.25))
        g.fill()
        self._nose_shape.draw(g, (25, -20, -2, 1.25))
        g.fill()

        g.restore()

class Eyes:
    def __init__(self):
        self.next_blink = time.time() + random.random() * 3.5
        self.blink_level = 1

        self.last_state: state.self.target_tform = None

        self.current_eyes: MutableSequence[util.TShape] = []
        self.target_eyes: MutableSequence[util.TShape] = []
        self.current_tform: MutableSequence[[float, float, float, float]] = []
        self.target_tform: MutableSequence[[float, float, float, float]] = []

        self._update_targets()
    
    def draw(self, g: cairo.Context, delta: float = 1/60):
        self._update_targets()
        self._update_currents(delta)
        for i in range(4):
            self.current_eyes[i].copy(self.current_tform[i]).draw(g)
            g.fill()
    
    def _update_targets(self):
        if self.last_state == state.standard_expr:
            return

        if state.standard_expr == state.Standard.Neutral:
            self.target_eyes = [util.get_simple_quad((-15, -22), (5, -22), (15, 22), (-5, 22))] * 4
            self.target_tform = _get_simple_transforms(110, 0, 80, 0, 1.2, 1)
        elif state.standard_expr == state.Standard.Happy:
            self.target_eyes = [util.TShape([
                [-15, 8, 0, 0, 0, 0],
                [-15, -8, 15, -8, 15, 8],
                [15, 8, 7, 8, 7, 8],
                [7, 1, -7, 1, -7, 8],
                [-7, 8, -15, 8, -15, 8]])] * 4
            self.target_tform = _get_simple_transforms(105, -20, 80, 0, 1.5, 1)
        else:
            # Upset
            self.target_eyes = [util.get_simple_quad((-15, -5), (5, 0), (15, 5), (-5, 5))] * 4
            self.target_tform = _get_simple_transforms(105, -20, 80, 0, 1.5, 1)
        
        # Change length of current eyes to match the length of the target ones
        for i in range(4):
            if i >= len(self.current_eyes):
                self.current_eyes.append(self.target_eyes[i].copy())
            else:
                diff = len(self.target_eyes[i].points) - len(self.current_eyes[i].points)
                if diff > 0:
                    self.current_eyes[i].points += [self.current_eyes[i].points[-1]] * 4 if len(self.current_eyes[i].points) > 1 else self.target_eyes[i].copy().points
                elif diff < 0:
                    self.current_eyes[i].points = self.current_eyes[i].points[:len(self.target_eyes[i].points)]
        
            if i >= len(self.current_tform):
                c = []
                for j in range(len(self.target_tform[i])):
                    c.append(self.target_tform[i][j])
                self.current_tform.append(c)
        
        self.last_state = state.standard_expr
    
    def _update_currents(self, delta: float):
        if self.blink_level < 1:
            self.blink_level += 20 * delta
            if self.blink_level >= 1:
                self.blink_level = 1
                self.next_blink = time.time() + random.random() * 6.5
        elif time.time() > self.next_blink:
            self.blink_level = -1
        
        for i in range(4):
            self.current_eyes[i].approach(self.target_eyes[i], delta)
            for j in range(4):
                diff = self.target_tform[i][j] - self.current_tform[i][j]
                if abs(diff) > 0.1 or (j > 1 and abs(diff) > 0.001):
                    self.current_tform[i][j] += diff * min(10 * delta, 1)
                else:
                    self.current_tform[i][j] = self.target_tform[i][j]


def _get_simple_transforms(x0, y0, x1, y1, scale0, scale1):
    return [
        [-x0, y0, scale0, scale0],
        [-x1, y1, scale1, scale1],
        [x1, y1, -scale1, scale1],
        [x0, y0, -scale0, scale0]
    ]