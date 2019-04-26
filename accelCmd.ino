//Accelerator Command variables
int accelPin=10;//pin must be a PWM pin, PWM on uno 3,5,6,9,10,11  
int accelSwitch=9;
float Vcc=5;//V high output on PWM signal
float Vmax=3.75;//Max expected voltage from accelerator 
int dutyCycle=0;
int previousDutyCycle = 0;
float period=5;//Sets the time it takes to ramp up to Vmax in seconds
float timeStep;//variable for time quantization 
float DigitalVoltage;//Digital level of Vmax 
boolean maxVoltage= false;
long previousMillis = 0;
String inputVoltage="0";//Variable to manually set voltage
float DigitalInput=0;

//Serial Read Variables 
String dummy="0";
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

//Big Red Button Variables 
int BRBPin=2;
int BRBValue=0;

//Drive Mode Variables
int modeSwitch=3;

void setup() {
  Serial.begin(9600);//being communication with serial port
  pinMode(accelPin,OUTPUT);
  pinMode(accelSwitch,OUTPUT);
  pinMode(modeSwitch,OUTPUT);
  pinMode(BRBPin,INPUT_PULLUP);

}

void loop() {
  BRBValue = digitalRead(BRBPin); // Sets BRBValue to the value at pin 2, BRB up = 0; BRB down =1;
  if(BRBValue)// If BRB is up, put car into automatic mode and will push serial commands to actuators
  {
   digitalWrite(modeSwitch,LOW);
  }
  else
  {
    digitalWrite(modeSwitch,HIGH);
  }
  recvWithStartEndMarkers();//program to read from serial port, start reading when startMarker is seen, stop reading when end Marker is seen
  showNewData();//parses out serial input
  acceleratorCommand();
  
  
}
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();//reads first char from serial port

        if (recvInProgress == true) 
        {
            if (rc != endMarker) 
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else 
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) //if Start Marker is seen, begins reading from; else that byte is ignored
        {
            recvInProgress = true;//Sets recvInProgress=true to being reading 
        }
    }
}
void showNewData() {
    if (newData == true) {
        inputVoltage=receivedChars;
        newData = false;
    }
}
void acceleratorCommand(){
  DigitalVoltage=Vmax/Vcc*255;//Analog write expects a number from 0 to 255, 0 is 0V, 255 is 5V
  timeStep=period*1000/DigitalVoltage;//sets the delta t to reach desired voltage at desired time 

  if(inputVoltage.toFloat()>Vmax){
    DigitalInput=Vmax/Vcc*255;
    digitalWrite(accelSwitch,HIGH);
  }
  else if(inputVoltage.toFloat()<=0){
    DigitalInput=0;
    digitalWrite(accelSwitch,LOW);
    
  }
  else{
    DigitalInput=inputVoltage.toFloat()/Vcc*255;//Gets digitial value for desired input voltage
    digitalWrite(accelSwitch,HIGH);
  }
  
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > timeStep){//performs this statement every timeStep
    previousMillis = currentMillis;
      if(dutyCycle<DigitalInput){
        dutyCycle++;
      }
      else{
        dutyCycle=DigitalInput;
      }
  }
  analogWrite(accelPin,dutyCycle);
}
