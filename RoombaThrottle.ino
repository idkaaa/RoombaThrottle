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
    //Serial.println("In while loop");
    int SensorValue = 0;
    do
    {
      SensorValue = analogRead(A0);
      delay(20);
    }while(SensorValue < ThrottleSensorMinValue); //no new signals
    //Serial.print("Sensor value passed threshold.");
    float VoltageFromHandThrottle = f_GetVoltageFromSensorValue(SensorValue);
    int ThrottleOutput = f_GetLinearOutputDutyCycleToMotorController(VoltageFromHandThrottle);
    Serial.print("Motor controller linear pwm duty cycle output: ");
    Serial.println(ThrottleOutput);
    if(ThrottleOutput > DutyCycleMax)
    {
      Serial.println("Throttle output passed upper limit.");
      analogWrite(5, DutyCycleMax);
    }
    analogWrite(5, ThrottleOutput);
}

//gets the voltage coming from the hand throttles
float f_GetVoltageFromSensorValue(int SensorValue)
{
  float Voltage = SensorValue * (5.0 / 1023.0);
  //Serial.print("Voltage read from Hand Throttle: ");
  //Serial.println(Voltage);
  return Voltage;
}

//converts throttle voltage input to smoother output for the motor controller
int f_GetLinearOutputDutyCycleToMotorController(float VoltageFromHandThrottle)
{
  //equation to smooth motor response
  //0.84 volts == 50 duty cycle
  //4.17 volts == 210 duty cycle
  //3.33 volts range
  //3.33 * 100 = 333
  //Normalized sensor min 0 = 50 duty cycle
  //Normalized sensor max 333 = 210 duty cycle
  //each step = 2.08125 (range normalized sensor/range duty cycle)
  //each output = 2.08125*input

  float NormalizedVoltage = (VoltageFromHandThrottle - ThrottleSensorVoltageMin) * 100;
  int RangeSensor = (ThrottleSensorVoltageMax - ThrottleSensorVoltageMin)*100; //3.24 to 324
  int RangeDutyCycle = DutyCycleMax - DutyCycleMin;
  float ConversionConstant = RangeSensor / RangeDutyCycle;
  float LinearOutput = NormalizedVoltage / ConversionConstant;
  //Serial.print("Linear duty cycle output: ");
  //Serial.println(LinearOutput);
  return (int)LinearOutput + DutyCycleMin;
}

