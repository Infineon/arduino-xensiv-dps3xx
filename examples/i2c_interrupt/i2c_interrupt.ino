
#include <Dps368.h>

// Dps368 Opject
Dps368 Dps368PressureSensor = Dps368();
void onFifoFull();


void setup()
{
  Serial.begin(9600);
  while (!Serial);

  //Call begin to initialize Dps368PressureSensor
  //The parameter 0x76 is the bus address. The default address is 0x77 and does not need to be given.
  //Dps368PressureSensor.begin(Wire, 0x76);
  //Use the commented line below instead to use the default I2C address.
  
  Dps368PressureSensor.begin(Wire); 
  int16_t val = Dps368PressureSensor.setInterruptSources(4, 0);
  //clear interrupt flag by reading
  Dps368PressureSensor.getIntStatusFifoFull();

  //initialization of Interrupt for Controller unit
  //SDO pin of Dps368 has to be connected with interrupt pin
  int16_t interruptPin = 9;
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onFifoFull, RISING);

  

  //temperature measure rate (value from 0 to 7)
  //2^temp_mr temperature measurement results per second
  int16_t temp_mr = 5;
  //temperature oversampling rate (value from 0 to 7)
  //2^temp_osr internal temperature measurements per result
  //A higher value increases precision
  int16_t temp_osr = 1;
  //pressure measure rate (value from 0 to 7)
  //2^prs_mr pressure measurement results per second
  int16_t prs_mr = 5;
  //pressure oversampling rate (value from 0 to 7)
  //2^prs_osr internal pressure measurements per result
  //A higher value increases precision
  int16_t prs_osr = 1;
  //startMeasureBothCont enables background mode
  //temperature and pressure ar measured automatically
  //High precision and hgh measure rates at the same time are not available.
  //Consult Datasheet (or trial and error) for more information
  int16_t ret = Dps368PressureSensor.startMeasureBothCont(temp_mr, temp_osr, prs_mr, prs_osr);



  if (ret != 0)
  {
    Serial.print("Init FAILED! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("Init complete!");
  }
}



void loop()
{
  uint8_t pressureCount = 20;
  float pressure[pressureCount];
  uint8_t temperatureCount = 20;
 float temperature[temperatureCount];
 
 int16_t val = Dps368PressureSensor.getIntStatusFifoFull();


  //This function writes the results of continuous measurements to the arrays given as parameters
  //The parameters temperatureCount and pressureCount should hold the sizes of the arrays temperature and pressure when the function is called
  //After the end of the function, temperatureCount and pressureCount hold the numbers of values written to the arrays
  //Note: The Dps368 cannot save more than 32 results. When its result buffer is full, it won't save any new measurement results
 int16_t ret = Dps368PressureSensor.getContResults(temperature, temperatureCount, pressure, pressureCount);

 if (ret != 0)
  {
    Serial.println();
    Serial.println();
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.println();
    Serial.println();
    Serial.print(temperatureCount);
    Serial.println(" temperature values found: ");
    for (int16_t i = 0; i < temperatureCount; i++)
    {
      Serial.print(temperature[i]);
      Serial.println(" degrees of Celsius");
    }

    Serial.println();
    Serial.print(pressureCount);
    Serial.println(" pressure values found: ");
    for (int16_t i = 0; i < pressureCount; i++)
    {
      Serial.print(pressure[i]);
      Serial.println(" Pascal");
    }
  }

  //Wait some time, so that the Dps368 can refill its buffer
  delay(10000);
}

void onFifoFull()
{
  //message for debugging
  Serial.println("Interrupt handler called");
  Dps368PressureSensor.getIntStatusFifoFull();

}
