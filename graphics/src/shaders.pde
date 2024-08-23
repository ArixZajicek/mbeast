PShader shaderHue, shaderChannels;

void setupShaders() {
  shaderHue = loadShader("shaders/hue.glsl");
  shaderChannels = loadShader("shaders/channels.glsl");
}

void setShaderParams(float hue, float glitch) {
  shaderHue.set("hue", hue * TWO_PI);
  
  shaderChannels.set("rbias", 0.0, 0.0);
  shaderChannels.set("gbias", glitch * random(-0.02, 0.02), 0.0);
  shaderChannels.set("bbias", 0.0, 0.0);
  shaderChannels.set("rmult", 1.0 + glitch * random(-0.25, 0.25), 1.0);
  shaderChannels.set("gmult", 1.0, 1.0);
  shaderChannels.set("bmult", 1.0, 1.0);
}
