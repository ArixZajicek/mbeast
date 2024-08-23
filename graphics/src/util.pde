
static class Util {
  static void drawPointSet(PGraphics g, float points[][]) {
    g.beginShape();
    boolean first = true;
    for (float v[] : points) {
      if (first || v.length == 2) {
        g.vertex(v[0], v[1]);
        first = false;
      } else {
        g.bezierVertex(v[0], v[1], v[2], v[3], v[4], v[5]);
      }
    }
    g.endShape();
  }
  
  static void drawPointSet(PGraphics g, float points[][], float scaleX, float scaleY, float translateX, float translateY) {
    g.beginShape();
    boolean first = true;
    for (float v[] : points) {
      if (first || v.length == 2) {
        g.vertex(v[0] * scaleX + translateX, v[1] * scaleY + translateY);
        first = false;
      } else {
        g.bezierVertex(
          v[0] * scaleX + translateX,
          v[1] * scaleY + translateY,
          v[2] * scaleX + translateX,
          v[3] * scaleY + translateY,
          v[4] * scaleX + translateX,
          v[5] * scaleY + translateY
        );
      }  
    }
    g.endShape();
  }
  
  static float[][] getSimpleQuad(
    float x0, float y0,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3
  ) {
    return new float[][] {
      {x0, y0, 0, 0, 0, 0},
      {x0, y0, x1, y1, x1, y1},
      {x1, y1, x2, y2, x2, y2},
      {x2, y2, x3, y3, x3, y3},
      {x3, y3, x0, y0, x0, y0}
    };
  }
}
