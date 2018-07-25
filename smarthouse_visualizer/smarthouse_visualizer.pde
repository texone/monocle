import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup() 
{
  size(800, 800);
  // print out all ports find the arduino port
  for(String portName : Serial.list()){
    println(portName);
  }
  
  myPort = new Serial(this, "/dev/cu.usbmodem1411", 9600);
}


StringBuffer readBuffer = new StringBuffer();
float angle = 0;

void handleInput(){
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      readBuffer.append(inBuffer);
     // println(readBuffer);
    }
    int indexNewLine = readBuffer.indexOf("\n");
    println(indexNewLine);
    if(indexNewLine <= 0)continue;
    String myDataString = readBuffer.substring(0, indexNewLine);
    readBuffer.delete(0, indexNewLine + 1);
    
    String[] myData = myDataString.split(",");
    try{
      int mySteps = Integer.parseInt(myData[0]);
      angle = mySteps / 60000f;
      println(indexNewLine + ", " + mySteps);
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
  
  translate(400,400);
  rotate(angle * radians(175f / 2));
  
  fill(0);
  ellipse(0,250,200,200);
  
}