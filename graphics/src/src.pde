final int 
  WIDTH = 256,
  HEIGHT = 64,
  SCALE = 4,
  FRAMERATE = 60;

final float
  BREATHE_CYCLE = 3000,
  BREATHE_STRENGTH = 1;

final boolean
  HW_ENABLED = true;

final String
  SOCKET_PATH = "/tmp/mbeast_driver.socket";

PGraphics g;
int mx = 0, my = 0;
int lastMillis;

Eyes eyes;
final float[][] noseShape = Util.getSimpleQuad(-3, -2, 1, -1, 3, 2, -2, 2);

Socket socket;

void setup() {
  size(640, 480, P2D);
  noSmooth();
  frameRate(FRAMERATE);
  windowResize(WIDTH * SCALE, HEIGHT * SCALE);
  
  g = createGraphics(WIDTH, HEIGHT, P2D);
  ((PGraphicsOpenGL) g).textureSampling(2);
  g.noSmooth();
  
  eyes = new Eyes(g, EyeState.Neutral);
  
  if (HW_ENABLED) {
    socket = new Socket(this);
    while (!socket.connect()) delay(1000);
  }
  
  lastMillis = millis();
}

void draw() {
  mx = mouseX / SCALE - WIDTH / 2;
  my = mouseY / SCALE;
  
  background(0);

  g.beginDraw();
  g.clear();
  g.translate(WIDTH / 2, HEIGHT / 2 + BREATHE_STRENGTH * sin(millis() / BREATHE_CYCLE * 2 * PI));
  g.noStroke();
  
  g.fill(100, 255, 200);
  eyes.tick((millis() - lastMillis) / 1000.0);
  eyes.draw();
  Util.drawPointSet(g, noseShape, 2, 1.25, -25, -20);
  Util.drawPointSet(g, noseShape, -2, 1.25, 25, -20);
  
  g.endDraw();
  image(g, 0, 0, WIDTH * SCALE, HEIGHT * SCALE);
  
  if(HW_ENABLED && !socket.connected) {
    delay(500);
    socket.connect();
  }
  
  lastMillis = millis();
}

void keyPressed() {
  switch (key) {
    case '1':
      eyes.setState(EyeState.Neutral);
      break;
    case '2':
      eyes.setState(EyeState.Happy);
      break;
    case '3':
      eyes.setState(EyeState.Upset);
      break;
  }
}
