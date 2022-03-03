#include <Dps368.h>

// Dps368 Opject
Dps368 Dps368PressureSensor = Dps368();

void setup()
{
  //pin number of your slave select line
  //XMC2GO
  int16_t pin_cs = 3;
  //for XMC 1100 Bootkit  & XMC4700 Relax Kit uncomment the following line
  //int16_t pin_cs = 10;

  Serial.begin(9600);
  while (!Serial);


  //Call begin to initialize Dps368PressureSensor
  //The parameter pin_nr is the number of the CS pin on your Microcontroller
  Dps368PressureSensor.begin(SPI, pin_cs);

  Serial.println("Init complete!");
}



void loop()
{
  float temperature;
  float pressure;
  uint8_t oversampling = 7;
  int16_t ret;
  Serial.println();

  //lets the Dps368 perform a Single temperature measurement with the last (or standard) configuration
  //The result will be written to the paramerter temperature
  //ret = Dps368PressureSensor.measureTempOnce(temperature);
  //the commented line below does exactly the same as the one above, but you can also config the precision
  //oversampling can be a value from 0 to 7
  //the Dps 368 will perform 2^oversampling internal temperature measurements and combine them to one result with higher precision
  //measurements with higher precision take more time, consult datasheet for more information
  ret = Dps368PressureSensor.measureTempOnce(temperature, oversampling);

  if (ret != 0)
  {
    //Something went wrong.
    //Look at the library code for more information about return codes
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" degrees of Celsius");
  }


  //ret = Dps368PressureSensor.measurePressureOnce(pressure);
  ret = Dps368PressureSensor.measurePressureOnce(pressure, oversampling);
  if (ret != 0)
  {
    //Something went wrong.
    //Look at the library code for more information about return codes
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pascal");
  }

  //Wait some time
  delay(1000);
}
