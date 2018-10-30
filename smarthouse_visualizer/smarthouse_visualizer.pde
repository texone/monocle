import processing.serial.*;

Serial myPort;  // Create object from Serial class
PrintWriter output;
void setup() 
{
  size(1800, 800);
  frameRate(30);
  // print out all ports find the arduino port
  for (String portName : Serial.list()) {
    println(portName);
  }
  textFont(loadFont("Monospaced-48.vlw"), 24);
  myPort = new Serial(this, "COM3", 115200);
  //output = createWriter("log.txt"); 
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
ArrayList<Float> targetBuffer = new ArrayList();
ArrayList<Float> stepBuffer = new ArrayList();
ArrayList<Float> speedBuffer = new ArrayList();
ArrayList<Float> difBuffer = new ArrayList();
float target = 0;

String myAnimation = "HOMING";
String[] animationNames = {
  "BASE_STILL",
  "RANDOM_STILL",
  "JITTER_STILL",
  "RANDOM_MOVE",
  "JITTER_MOVE",
  "FULL_ROLL",
  "RANDOM_ROLL",
  "TRANSITION"
};

float lastInput;

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
   // output.println(myDataString);
    //output.flush();
    readBuffer.delete(0, indexNewLine + 1);
  
    String[] myData = myDataString.split(",");
    try {
      int myTargetSteps = Integer.parseInt(myData[0]);
      int myCurrentSteps = Integer.parseInt(myData[1]);
      float speed = myCurrentSteps - lastInput;
      lastInput = myCurrentSteps;
      int myIndex = Integer.parseInt(myData[2]);
      int myTransition = Integer.parseInt(myData[3]);
      
      int diff = myCurrentSteps - myTargetSteps;
      println(diff);
      difBuffer.add(diff / 4000f);
      target = myTargetSteps / 4000f;
      float current = myCurrentSteps / 4000f;
      targetBuffer.add(target);
      stepBuffer.add(current);
      speedBuffer.add(speed * 0.04f);
      while (targetBuffer.size() > width) {
        targetBuffer.remove(0);
      }
      while (stepBuffer.size() > width) {
        stepBuffer.remove(0);
      }
      while (speedBuffer.size() > width) {
        speedBuffer.remove(0);
      }
      while (difBuffer.size() > width) {
        difBuffer.remove(0);
      }
      
      myAnimation = animationNames[myIndex];
      if(myTransition == 1){
        myAnimation = "TRANSITION";
      }
    }
    catch(Exception e) {
    }
  }
}

void drawCurve(ArrayList<Float> theData, int r, int g, int b){
  stroke(r, g, b);
  beginShape();
  for (int i = 0; i < theData.size(); i++) {
    vertex(i, theData.get(i) * height/ 2);
  }
  endShape();
}

void drawCurves() {
  if (!debug)return;
  noFill();
  pushMatrix();
  translate(0, 400);
  
  drawCurve(difBuffer, 130,0,0);
  drawCurve(targetBuffer, 130,130,0);
  drawCurve(stepBuffer, 130,130,130);
  drawCurve(speedBuffer, 255,0,0);
  
  
  stroke(0, 255, 0);
  beginShape();
  for (int i = 0; i < speedBuffer.size(); i++) {
    vertex(i, speedBuffer.get(i) * height/ 2);
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
  
  text(myAnimation + " " + frameRate,20,30);
  
  pushMatrix();
  scale(height, height);
  fill(255);
  ellipse(0.5, 0.5, 0.9, 0.9);

  pushMatrix();
  translate(0.5, 0.5);
  rotate(target * radians(175f / 2));

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
