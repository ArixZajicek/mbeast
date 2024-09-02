import cairo
from collections.abc import MutableSequence

class TShape:
    def __init__(self, 
            points: MutableSequence[(float, float, float, float, float, float)],
            transform = (0, 0, 1, 1)):
        self.points = []
        for p in points:
            if len(p) == 2:
                self.points.append([
                    p[0] * transform[2] + transform[0],
                    p[1] * transform[3] + transform[1],
                ])
            else:
                self.points.append([
                    p[0] * transform[2] + transform[0],
                    p[1] * transform[3] + transform[1],
                    p[2] * transform[2] + transform[0],
                    p[3] * transform[3] + transform[1],
                    p[4] * transform[2] + transform[0],
                    p[5] * transform[3] + transform[1],
                ])
    
    def approach(self, target: 'TShape', factor = 0.05, threshold = 0.5):
        for i in range(min(len(self.points), len(target.points))):
            for j in range(min(len(self.points[i]), len(target.points[i]))):
                diff = target.points[i][j] - self.points[i][j]
                if (abs(diff) > threshold):
                    self.points[i][j] += diff * Math.min(factor, 1)
                else:
                    self.points[i][j] = target.points[i][j]
    
    def copy(self, transform = (0, 0, 1, 1)):
        return TShape(self.points, transform)
    
    def draw(self, g: cairo.Context, transform = (0, 0, 1, 1)):
        first = True
        for p in self.points:
            if first:
                g.move_to(
                    p[0] * transform[2] + transform[0],
                    p[1] * transform[3] + transform[1])
                first = False
            elif len(p) == 2:
                g.line_to(
                    p[0] * transform[2] + transform[0],
                    p[1] * transform[3] + transform[1])
            else:
                g.curve_to(
                    p[0] * transform[2] + transform[0],
                    p[1] * transform[3] + transform[1],
                    p[2] * transform[2] + transform[0],
                    p[3] * transform[3] + transform[1],
                    p[4] * transform[2] + transform[0],
                    p[5] * transform[3] + transform[1])



def get_simple_quad(
        p0: (float, float),
        p1: (float, float),
        p2: (float, float),
        p3: (float, float)) -> TShape:
    return TShape([
        [p0[0], p0[1], 0, 0, 0, 0],
        [p0[0], p0[1], p1[0], p1[1], p1[0], p1[1]],
        [p1[0], p1[1], p2[0], p2[1], p2[0], p2[1]],
        [p2[0], p2[1], p3[0], p3[1], p3[0], p3[1]],
        [p3[0], p3[1], p0[0], p0[1], p0[0], p0[1]]
    ])