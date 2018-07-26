import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup() 
{
  size(800, 800);
  // print out all ports find the arduino port
  for(String portName : Serial.list()){
    println(portName);
  }
  
  myPort = new Serial(this, "/dev/cu.usbmodem1411", 115200);
}


StringBuffer readBuffer = new StringBuffer();
ArrayList<Float> angleBuffer = new ArrayList();
ArrayList<Float> stepBuffer = new ArrayList();
float angle = 0;

void handleInput(){
  println(readBuffer.length());
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      readBuffer.append(inBuffer);
     // println(readBuffer);
    }
  }
  int indexNewLine = 0;
  
  while(indexNewLine >= 0){
    indexNewLine = readBuffer.indexOf("\n");
    if(indexNewLine <= 0)continue;
    String myDataString = readBuffer.substring(0, indexNewLine);
    readBuffer.delete(0, indexNewLine + 1);
    
    String[] myData = myDataString.split(",");
    try{
      int mySteps = Integer.parseInt(myData[0]);
      int mySteps2 = Integer.parseInt(myData[1]);
      angle = mySteps / 4000f;
      float angle2 = mySteps2 / 4000f;
      angleBuffer.add(angle);
      stepBuffer.add(angle2);
      while(angleBuffer.size() > width){
        angleBuffer.remove(0);
      }
      while(stepBuffer.size() > width){
        stepBuffer.remove(0);
      }
    }catch(Exception e){
      
    }
  }
}

void draw()
{
  handleInput();
  
  background(0);
  fill(255);
  ellipse(400,400,700,700);
  
  pushMatrix();
  translate(400,400);
  rotate(angle * radians(175f / 2));
  
  fill(0);
  ellipse(0,250,200,200);
  popMatrix();
  
  pushMatrix();
  translate(0,400);
  noFill();
  stroke(255,0,0);
  beginShape();
  for(int i = 0; i < angleBuffer.size();i++){
    vertex(i, angleBuffer.get(i) * height/ 2);
  }
  endShape();
  
  stroke(0,255,0);
  beginShape();
  for(int i = 0; i < stepBuffer.size();i++){
    vertex(i, stepBuffer.get(i) * height/ 2);
  }
  endShape();
  noStroke();
  popMatrix();
}