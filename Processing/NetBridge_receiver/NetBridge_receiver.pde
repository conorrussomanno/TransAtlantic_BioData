/*
 * Baseed on Chat Server 
 * by Tom Igoe. 
 * Modified by Conor R. & Yannick R.
 * Press the mouse to stop the server.
 */
 

import processing.net.*;
import processing.serial.*;  //for serial communication to Arduino & HHI Shield

String netBridge_portName = "REPLACE_THIS_WITH_YOUR_SERIAL_PORT_NAME";  //must edit this based on the name of the serial/COM port your Arduino w/ HHI
Serial netBridge_serial;
int netBridge_baud = 9600; //baud rate from the Arduino / HHI

int port = 12345;
boolean myServerRunning = true;
int bgColor = 0;
int direction = 1;
int textLine = 60;

Server myServer;

void setup()
{
  size(displayWidth, displayHeight);
  textFont(createFont("SanSerif", 16));
  myServer = new Server(this, port); // Starts a myServer on port 10002
  background(0);
  
  //attempt to establish serial connection to Arduino board with HHI
  try {
    println("OpenBCI_GUI:  attempting to open serial port for HHI using name = " + netBridge_portName);
    netBridge_serial = new Serial(this, netBridge_portName, netBridge_baud); //open the com port
    netBridge_serial.clear(); // clear anything in the com port's buffer
  } 
  catch (RuntimeException e) {
    println("OpenBCI_GUI: *** ERROR ***: Could not open " + netBridge_portName);
  }
  
}

void mousePressed()
{
  // If the mouse clicked the myServer stops
  myServer.stop();
  myServerRunning = false;
}

boolean isActive = false;
String messageFromClient;
int intensityFromClient;

void draw()
{
  if (myServerRunning == true)
  {
    text("server", 15, 45);
    Client thisClient = myServer.available();
    if (thisClient != null) {
      if (thisClient.available() > 0) {
        messageFromClient = thisClient.readString();
        messageFromClient = messageFromClient.substring(0, messageFromClient.indexOf("\n"));
        intensityFromClient = Integer.parseInt(messageFromClient);
        println("mesage from: " + thisClient.ip() + " : " + messageFromClient, 15, textLine);
        if(messageFromClient.indexOf("off") >= 0)
        {
           isActive = false;
           println("If Off");
           println(messageFromClient);
        }
        else
        {
          isActive = true;
          println("If On");
          println(messageFromClient);
        }
        
        textLine = textLine + 35;
      }
    }
  } 
  else 
  {
    text("server", 15, 45);
    text("stopped", 15, 65);
  }

  // Clear background (draw it white)
  fill(255,255,255);
  rect(0,0, width, height);  
  
  // draw blue with opacity proportional to strength of incoming signal  
  fill(0,0,255, (int)map(intensityFromClient, 0, 100, 0, 255));    
  rect(0,0, width, height);
  
  netBridge_serial.write(intensityFromClient);  //send a value from 0-100 over the serial port to the Arduino w/ HHI shield
  println("Value To HHI: | " + intensityFromClient + " |");

//  // For BINARY ON/OFF
//  if(isActive){
//    println("a");
//    fill(0,0,255, map(intensityFromClient, 0, 100);
//  } else {
//    fill(255,255,255);
//  }
//  rect(0,0, width, height);
}
