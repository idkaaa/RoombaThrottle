int DutyCycleMax = 155;
int DutyCycleMin = 50;
int DutyCycleCurrent = 0;
//throttle mapped to range of duty cycles
int ThrottleIntputMax = 160;
int ThrottleInputMin = 0;
int ThrottleInputCurrent = 0;
//actual throttle voltage low = 0.82 volts
//actual throttle voltage high = 3.58 volts
//sensor low = 185
//sensor high = 858

bool IsThrottleActiveLeft = false;
bool IsThrottleActiveRight = false;

int ThrottleSensorValueLeft = 0;
int ThrottleSensorValueRight = 0;

int LeftThrottlePwmPin = 5;
int RightThrottlePwmPin = 10;

int LeftThrottleAnalogSensorPin = A0; 
int RightThrottleAnalogSensorPin = A5;

int ThrottleSensorValue = 0;
int ThrottleSensorMinValue = 185;
int ThrottleSensorMaxValue = 858;

float ThrottleSensorVoltageMin = 0.90;
float ThrottleSensorVoltageMax = 4.17;



void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(57600);
  while (!Serial) {
    
  }
}

void loop() 
{  
   // put your main code here, to run repeatedly:
    do
    {
      f_SetActivityThrottle();
      f_ResetThrottlePwmOutputs();
      delay(20);
    }while(
      (IsThrottleActiveLeft == false) 
    && (IsThrottleActiveRight == false)); //no new signals
    //Serial.print("Sensor value passed threshold.");
    f_OutputRemappedThrottleValues();
}

///send output to motor controllers
void f_OutputRemappedThrottleValues()
{
  if(IsThrottleActiveLeft == true)
  {
    int OutputLeft = f_GetRemappedThrottlePwmOutput(ThrottleSensorValueLeft);
    analogWrite(LeftThrottlePwmPin, OutputLeft);
  }
  if(IsThrottleActiveRight == true)
  {
    int OutputRight = f_GetRemappedThrottlePwmOutput(ThrottleSensorValueRight);
    analogWrite(RightThrottlePwmPin, OutputRight);
  }
}

///Sets whether throttles are active or not.
void f_SetActivityThrottle()
{
  ThrottleSensorValueLeft = analogRead(LeftThrottleAnalogSensorPin);
  if(ThrottleSensorValueLeft > ThrottleSensorMinValue)
  {
    IsThrottleActiveLeft = true;
  }
  else
  {
    IsThrottleActiveLeft = false;
  }

  ThrottleSensorValueRight = analogRead(RightThrottleAnalogSensorPin);
  if(ThrottleSensorValueRight > ThrottleSensorMinValue)
  {
    IsThrottleActiveRight = true;
  }
  else
  {
    IsThrottleActiveRight = false;
  }
}


//resets the pwm to 0
void f_ResetThrottlePwmOutputs()
{
  if(IsThrottleActiveLeft == false)
  {
    analogWrite(LeftThrottlePwmPin, 0);
  }
  if(IsThrottleActiveRight == false)
  {
    analogWrite(RightThrottlePwmPin, 0);
  }
}

int f_GetRemappedThrottlePwmOutput(int SensorValue)
{
  int ReMappedSensorValue = SensorValue;
//  int SlowSpeedRangeMax = 500;
//  if(SensorValue < SlowSpeedRangeMax)
//  {
//    ReMappedSensorValue = 
//    map(
//      SensorValue, 
//      ThrottleSensorMinValue, 
//      SlowSpeedRangeMax,
//      ThrottleSensorMinValue,
//      0.5*SlowSpeedRangeMax
//      );
//  }
    return map(
      ReMappedSensorValue, 
      ThrottleSensorMinValue, 
      ThrottleSensorMaxValue,
      DutyCycleMin,
      DutyCycleMax
      );
}
