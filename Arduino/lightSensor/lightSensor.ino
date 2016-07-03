#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

/*============================================================================== */


/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"

/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/* The service information */

int32_t beerServiceId;
int32_t beerCharId;

/*****************************************************************************************************/

// Define things for Beers Data 

// Flight 3 - A5,
// Ambient 2 - A4,
// Flight 2 - A3
// Ambient 1 - A2
// Flight 1 - A1 

int lightPins[] = {
  1, 2, 3, 4, 5
};


int pinCount = 5;

int sensorValue = 0;
int outputValue = 0;

void setup(void)
{
//    Serial.begin(9600);  //Begin serial communcation
      while (!Serial);  // required for Flora & Micro
      delay(500);

      boolean success;
    
      Serial.begin(115200);
      Serial.println(F("A Few Good Beers -- Setup process"));
      Serial.println(F("---------------------------------"));

      randomSeed(micros());

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // this line is particularly required for Flora, but is a good idea
  // anyways for the super long lines ahead!
  // ble.setInterCharWriteDelay(5); // 5 ms

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'A Few Good Beers': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=A Few Good Beers")) ) {
    error(F("Could not set device name?"));
  }

   /* Add the Beer Service definition */
  /* Service ID should be 1 */
  Serial.println(F("Adding the Beer Service definition (UUID = 0x103D): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=0x103D"), &beerServiceId);
  if (! success) {
    error(F("Could not add Beer service"));
  }

  /* Add the Beer Flight Measurement characteristic */
  /* Chars ID for Measurement should be 1 */
  Serial.println(F("Adding the Beer Flight Measurement characteristic - Flight 1 (UUID = 0x0001): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0001,PROPERTIES=0x10,MIN_LEN=1,VALUE=20"), &beerCharId);
    if (! success) {
    error(F("Could not add characteristic"));
  }

  Serial.println(F("Adding the Beer Flight Measurement characteristic - Ambient 1 (UUID = 0x0002): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0002,PROPERTIES=0x10,MIN_LEN=1,VALUE=20"), &beerCharId);
    if (! success) {
    error(F("Could not add characteristic"));
  }

  Serial.println(F("Adding the Beer Flight Measurement characteristic - Flight 2 (UUID = 0x0003): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0003,PROPERTIES=0x10,MIN_LEN=1,VALUE=20"), &beerCharId);
    if (! success) {
    error(F("Could not add characteristic"));
  }

  Serial.println(F("Adding the Beer Flight Measurement characteristic - Ambient 2  (UUID = 0x0004): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0004,PROPERTIES=0x10,MIN_LEN=1,VALUE=20"), &beerCharId);
    if (! success) {
    error(F("Could not add characteristic"));
  }

  Serial.println(F("Adding the Beer Flight Measurement characteristic - Flight 3 (UUID = 0x0005): "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x0005,PROPERTIES=0x10,MIN_LEN=1,VALUE=20"), &beerCharId);
    if (! success) {
    error(F("Could not add characteristic"));
  }
//  /* Add the Beer Service to the advertising data (needed for Nordic apps to detect the service) */
  Serial.print(F("Adding Beer Service UUID to the advertising payload: "));
  ble.sendCommandCheckOK( F("AT+GAPSETADVDATA=03-02-3D-10") );

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  Serial.println();
  
  
  Serial.println(F("******************************"));

  // loop through all the pins and intialize them 
//  for(int thisPin = 0; thisPin < pinCount; thisPin++){
//    pinMode( ledPins[thisPin], OUTPUT );
//  }
}


void loop()
{
   // loop through the lightPins and get the data, map and write it
    for(int thisPin = 0; thisPin < pinCount; thisPin++){
          int32_t uuidIndex; 

          uuidIndex = thisPin + 1; 

          Serial.print("which uuid index: ");
          Serial.println(uuidIndex);
          
          sensorValue = analogRead(lightPins[thisPin]);
          outputValue = mapSensorValue(sensorValue);

          /* Command is sent when \n (\r) or println is called */
          /* AT+GATTCHAR=CharacteristicID,value */
          ble.print( F("AT+GATTCHAR=") );
          ble.print( uuidIndex );
          ble.print( F(",") );
          ble.println(outputValue);
        
        //  /* Check if command executed OK */
          if ( !ble.waitForOK() )
          {
            Serial.println(F("Failed to get response!"));
          }
    }


//    
//   Serial.print(F("Updating HRM value to "));
//  Serial.print(outputValue);
//  Serial.println(F(" Units"));
//
//  /* Delay before next measurement update */
  delay(1000);
  
}

int mapSensorValue(int sensorValue) {
  int reMappedValue = 0;

  Serial.println(sensorValue); 

  outputValue = map(sensorValue, 1, 700, 1, 20);
  reMappedValue = map(outputValue, 1, 20, 1, 255);
  
  Serial.println(reMappedValue); 
//   Serial.print("sensor value = ");
//   Serial.print(sensorValue);
//   Serial.print("\t mapped value = ");
//   Serial.print(outputValue);
//   Serial.print("\t remapped = ");
//   Serial.println(reMappedValue);


  return reMappedValue;
}

