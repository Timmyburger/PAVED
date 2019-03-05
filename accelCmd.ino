int accelPin=9;
float Vcc=5;
float Vmax=3.75;
int dutyCycle=0;
int previousDutyCycle = 0;

float period=5;
float timeStep;
float voltageStep;

boolean maxVoltage= false;

long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(accelPin,OUTPUT);

}

void loop() {
  voltageStep=Vmax/Vcc*255;
  timeStep=period/voltageStep*1000;
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > timeStep){
    previousMillis = currentMillis;
    if(dutyCycle<Vmax/Vcc*255 && maxVoltage == false )
    {
          dutyCycle++;
          if(dutyCycle >Vmax/Vcc*255){
            maxVoltage = true;
          }  
    }
    else {
      dutyCycle--;
      if (dutyCycle <= 0){
        maxVoltage = false;
      }
    }
  }
  analogWrite(accelPin,dutyCycle);
}
