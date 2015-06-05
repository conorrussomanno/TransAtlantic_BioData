/*
 * Baseed on Chat Server 
 * by Tom Igoe. 
 * Modified by Conor R. & Yannick R.
 * Press the mouse to stop the server.
 */
 

import processing.net.*;

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

  fill(255,255,255);
  
  rect(0,0, width, height);
    
  fill(0,0,255, (int)map(intensityFromClient, 0, 100, 0, 255));    
  
  rect(0,0, width, height);
    
/*  
  if(isActive){
    println("a");
    fill(0,0,255, map(intensityFromClient, 0, 100);
  } else {
    fill(255,255,255);
  }
  
  rect(0,0, width, height);
 */
}
