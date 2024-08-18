public enum EyeState {
  Neutral(
    getSimpleTransforms(110, 0, 80, 0, 1.2, 1),
    Util.getSimpleQuad(-15, -22, 5, -22, 15, 22, -5, 22)
  ),
  Happy(
    getSimpleTransforms(105, -20, 80, 0, 1.5, 1),
    new float[][] {
      {-15, 8, 0, 0, 0, 0},
      {-15, -8, 15, -8, 15, 8},
      {15, 8, 7, 8, 7, 8},
      {7, 1, -7, 1, -7, 8},
      {-7, 8, -15, 8, -15, 8}
    }
  ),
  Upset(
    getSimpleTransforms(105, -20, 80, 0, 1.5, 1),
    Util.getSimpleQuad(-15, -5, 5, 0, 15, 5, -5, 5)
  );
  
  public final float transforms[][];
  public final float points[][];
  private EyeState(float t[][], float p[][]) {
    transforms = t;
    points = p;
  }
}
  
private static float[][] getSimpleTransforms(float x0, float y0, float x1, float y1, float scale0, float scale1) {
  return new float[][] {
    {-x0, y0, scale0, scale0},
    {-x1, y1, scale1, scale1},
    {x1, y1, -scale1, scale1},
    {x0, y0, -scale0, scale0}
  };
}

public class Eyes {
  private EyeState state;
  private PGraphics g;
  
  private float points[][][];
  private float targetPoints[][][];
  private float transforms[][];
  private float targetTransforms[][];
  
  private long nextBlink = millis() + int(random(3500));
  private float blinkLevel = 1;
  
  public Eyes(PGraphics graphics, EyeState initial) {
    g = graphics;

    points = new float[4][16][6];
    targetPoints = new float[4][16][6];
    transforms = new float[4][4];
    targetTransforms = new float[4][4];
    
    setState(initial, true);
  }
  
  public void setState(EyeState newState, boolean setCurrent) {
    if (newState == state) return;
    
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 16; j++) {
          for (int k = 0; k < 6; k++) {
            if (j < newState.points.length) {
              targetPoints[i][j][k] = newState.points[j][k];
            } else {
              targetPoints[i][j][k] = newState.points[0][k % 2];
            }
            
            if (setCurrent) {
              points[i][j][k] = targetPoints[i][j][k];
            }
          }
      }
    }
    
    for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 4; j++) {
         targetTransforms[i][j] = newState.transforms[i][j];
         if (setCurrent) {
           transforms[i][j] = targetTransforms[i][j];
         }
       }
    }
    
    state = newState;
  }
  
  public void setState(EyeState newState) {
    setState(newState, false);
  }

  void draw() {
    for (int i = 0; i < 4; i++) {
      Util.drawPointSet(
        g,
        points[i],
        transforms[i][2],
        transforms[i][3] * Math.min(1, Math.abs(blinkLevel)),
        transforms[i][0],
        transforms[i][1]
      );
    }
  }
  
  void tick(float delta) {
    if (blinkLevel < 1) {
      blinkLevel += 20 * delta;
      if (blinkLevel >= 1) {
        blinkLevel = 1;
        nextBlink = millis() + int(random(6500));
      }
    } else if (millis() > nextBlink) {
      blinkLevel = -1;
    }
    
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 16; j++) {
        for (int k = 0; k < 6; k++) {
          float diff = targetPoints[i][j][k] - points[i][j][k];
          if (Math.abs(diff) > 0.5) {
            points[i][j][k] += diff * Math.min(20 * delta, 1);
          } else {
            points[i][j][k] = targetPoints[i][j][k];
          }
        }
      }
    }
    
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        float diff = targetTransforms[i][j] - transforms[i][j];
        if (Math.abs(diff) > 0.1 || (j > 1 && Math.abs(diff) > 0.001)) {
          transforms[i][j] += diff * Math.min(10 * delta, 1);
        } else {
          transforms[i][j] = targetTransforms[i][j];
        }
      }
    }
  }
};
