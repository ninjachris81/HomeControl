#include "CurrentController.h"
#include "Pins.h"
#include "TaskIDs.h"

#include "CommController.h"

CurrentController::CurrentController() : AbstractIntervalTask(CURRENT_UPDATE_INTERVAL_MS) {
  
}

void CurrentController::init() {
  pinMode(PIN_VOLTAGE_IN, INPUT);

  mamps.init(PROP_MAMPS, 0);
  mamps.registerValueChangeListener(this);
}

void CurrentController::onPropertyValueChange(uint8_t id, uint16_t newValue, uint16_t oldValue) {
  taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendCurrent(newValue);
  lastCurr = millis();
}

void CurrentController::update() {


  float nVPP;   // Voltage measured across resistor
  float nCurrThruResistorPP; // Peak Current Measured Through Resistor
  float nCurrThruResistorRMS; // RMS current through Resistor
  float nCurrentThruWire;     // Actual RMS current in Wire

   
   nVPP = getVPP();
   
   /*
   Use Ohms law to calculate current across resistor
   and express in mA 
   */
   
   nCurrThruResistorPP = (nVPP/200.0) * 1000.0;
   
   /* 
   Use Formula for SINE wave to convert
   to RMS 
   */
   
   nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;
   
   /* 
   Current Transformer Ratio is 1000:1...
   
   Therefore current through 200 ohm resistor
   is multiplied by 1000 to get input current
   */
   
   nCurrentThruWire = nCurrThruResistorRMS * 1000;


   /*
   Serial.print("Volts Peak : ");
   Serial.println(nVPP,3);
 
   
   Serial.print("Current Through Resistor (Peak) : ");
   Serial.print(nCurrThruResistorPP,3);
   Serial.println(" mA Peak to Peak");
   
   Serial.print("Current Through Resistor (RMS) : ");
   Serial.print(nCurrThruResistorRMS,3);
   Serial.println(" mA RMS");
   
   Serial.print("Current Through Wire : ");
   Serial.print(nCurrentThruWire,3);
   Serial.println(" mA RMS");
   */

   if (nCurrentThruWire>=MIN_VALUE && nCurrentThruWire<=MAX_VALUE) {
     mamps.setValue(nCurrentThruWire);
     currIsValid = true;
   } else {
    // read sensor error
    currIsValid = false;
   }

   if (currIsValid && millis() - lastCurr > CURRENT_CONTROLLER_BROADCAST_INTERVAL_MS) {
    taskManager->getTask<CommController*>(COMM_CONTROLLER)->sendCurrent(mamps.getValue());
    lastCurr = millis();
   }
    
}



 /************************************ 
In order to calculate RMS current, we need to know
the peak to peak voltage measured at the output across the
200 Ohm Resistor

The following function takes one second worth of samples
and returns the peak value that is measured
 *************************************/
 
 
float CurrentController::getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(PIN_VOLTAGE_IN);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
   }
   
   // Convert the digital data to a voltage
   result = (maxValue * 5.0)/1024.0;
  
   return result;
 }
