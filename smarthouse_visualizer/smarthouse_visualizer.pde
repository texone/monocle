import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup() 
{
  size(800, 800);
  frameRate(30);
  // print out all ports find the arduino port
  for (String portName : Serial.list()) {
    println(portName);
  }
  textFont(loadFont("Monospaced-48.vlw"), 24);
  myPort = new Serial(this, "/dev/cu.usbmodem1411", 115200);
}

boolean debug = true;

boolean saveOneFrame = false;
boolean saveFrames = false;

void keyPressed() {
  if (key == 'd' || key == 'D') {
    debug = !debug;
  }
  if (key == 'f' || key == 'F') {
    saveOneFrame = true;
  }
  if (key == 's' || key == 'S') {
    saveFrames = !saveFrames;
  }
}


StringBuffer readBuffer = new StringBuffer();
ArrayList<Float> angleBuffer = new ArrayList();
ArrayList<Float> stepBuffer = new ArrayList();
float angle = 0;

String myAnimation = "UNDEFINED";
String[] animationNames = {
  "BASE_STILL",
  "RANDOM_STILL",
  "JITTER_STILL",
  "RANDOM_MOVE",
  "JITTER_MOVE",
  "FULL_ROLL",
  "RANDOM_ROLL"
};

void handleInput() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      readBuffer.append(inBuffer);
      // println(readBuffer);
    }
  }
  int indexNewLine = 0;

  while (indexNewLine >= 0) {
    indexNewLine = readBuffer.indexOf("\n");
    if (indexNewLine <= 0)continue;
    String myDataString = readBuffer.substring(0, indexNewLine);
    println(myDataString);
    readBuffer.delete(0, indexNewLine + 1);
  
    String[] myData = myDataString.split(",");
    try {
      int mySteps = Integer.parseInt(myData[0]);
      int mySteps2 = Integer.parseInt(myData[1]);
      int myIndex = Integer.parseInt(myData[2]);
      
      angle = mySteps / 4000f;
      float angle2 = mySteps2 / 4000f;
      angleBuffer.add(angle);
      stepBuffer.add(angle2);
      while (angleBuffer.size() > width) {
        angleBuffer.remove(0);
      }
      while (stepBuffer.size() > width) {
        stepBuffer.remove(0);
      }
      
      myAnimation = animationNames[myIndex];
    }
    catch(Exception e) {
    }
  }
}

void drawCurves() {
  if (!debug)return;
  pushMatrix();
  translate(0, 400);
  noFill();
  stroke(255, 0, 0);
  beginShape();
  for (int i = 0; i < angleBuffer.size(); i++) {
    vertex(i, angleBuffer.get(i) * height/ 2);
  }
  endShape();

  stroke(0, 255, 0);
  beginShape();
  for (int i = 0; i < stepBuffer.size(); i++) {
    vertex(i, stepBuffer.get(i) * height/ 2);
  }
  endShape();
  noStroke();
  popMatrix();
}

void draw()
{
  handleInput();
  
  background(0);
  fill(255);
  
  text(myAnimation,20,30);
  
  pushMatrix();
  scale(width, height);
  fill(255);
  ellipse(0.5, 0.5, 0.9, 0.9);

  pushMatrix();
  translate(0.5, 0.5);
  rotate(angle * radians(175f / 2));

  fill(0);
  ellipse(0, 0.32, 0.25, 0.25);
  popMatrix();
  popMatrix();
  
  drawCurves();
  
  if(saveOneFrame){
    saveOneFrame = false;
    saveFrame("monocle.gif");
  }
  
  if(saveFrames){
    saveFrame("monocle-######.gif");
    fill(255,0,0);
    rect(10,10,40,40);
  }
  
}