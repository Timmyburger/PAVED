#include <Servo.h>
Servo motor;

#define outputA 6
#define outputB 7

const byte numChars = 32;
char receivedChars[numChars];

String desired_position="0";
int counter =0;
int aState;
int aLastState;
float tau;
float error=0;
float sum_error=0;

long previousMillis = 0; 
long interval = 500;  
int tftrefresh = 0;


boolean newData = false;

void setup() 
{
  Serial.begin(19200);
  pinMode(LED_BUILTIN, OUTPUT);

  aLastState = digitalRead(outputA);

}

void loop() {
  recvWithStartEndMarkers();//program to read from serial port, start reading when startMarker is seen, stop reading when end Marker is seen
  showNewData();//parses out serial input
  controlLoop();
  

}
void recvWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) 
    {
        rc = Serial.read();//reads first char from serial port

        if (recvInProgress == true) 
        {
            if (rc != endMarker) //executes until endmarker is seen
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) //if the received commmand is over size(ndx) sets the ndx-1 bit of receivedChars to the last char received  
                {
                    ndx = numChars - 1;
                }
            }
            else //once endMarker is seen, terminates string and kicks out of both nested if statement and while loop
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) //Begins reading from serial once startMarker is seen, else that byte is ignored
        {
            recvInProgress = true;//Sets recvInProgress=true to being reading 
        }
    }
}
void showNewData() 
{
    if (newData == true) 
    {
        desired_position=receivedChars;
        newData = false;
    }
}
void controlLoop()
{
  float kp=.1;
  float kv=.0001;

    aState= digitalRead (outputA);
  if (aState != aLastState)
  {
    if(digitalRead(outputB) != aState)
    {
      counter++;
    }
    else
    {
      counter --;
    }
  }
  aLastState= aState;
  error=desired_position.toInt()-counter;

  if(millis() - previousMillis > interval) 
  { 
         previousMillis = millis();  
         sum_error=sum_error+error;
  }
  if (error>0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    counter++;
    delay(100);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  //tau=kp*error+kv*sum_error+90;
  //motor.write(tau);
}
