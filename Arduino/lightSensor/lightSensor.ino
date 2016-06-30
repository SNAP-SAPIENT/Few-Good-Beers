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

    #define MANUFACTURER_APPLE         "0x004C"
    #define MANUFACTURER_NORDIC        "0x0059"

    #define BEACON_MANUFACTURER_ID     MANUFACTURER_APPLE
    #define BEACON_UUID                "01-12-23-34-45-56-67-78-89-9A-AB-BC-CD-DE-EF-F0"
    #define BEACON_MAJOR               "0x0000"
    #define BEACON_MINOR               "0x0000"
    #define BEACON_RSSI_1M             "-54"
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

/*****************************************************************************************************/

// Define things for Beers Data 
int lightPins[] = {
  0
};

int ledPins[] = {
  10
};

// nominal sensor = A5; 

int pinCount = 1;

int sensorValue = 0;
int outputValue = 0;

void setup()
{
//    Serial.begin(9600);  //Begin serial communcation
      while (!Serial);  // required for Flora & Micro
      delay(500);
    
      Serial.begin(115200);
      Serial.println(F("Adafruit Bluefruit Beacon Example"));
      Serial.println(F("---------------------------------"));
    
      /* Initialise the module */
      Serial.print(F("Initialising the Bluefruit LE module: "));
    
      if ( !ble.begin(VERBOSE_MODE) )
      {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
      }
      Serial.println( F("OK!") );
    
      if ( FACTORYRESET_ENABLE )
      {
        /* Perform a factory reset to make sure everything is in a known state */
        Serial.println(F("Performing a factory reset: "));
        if ( ! ble.factoryReset() ){
          error(F("Couldn't factory reset"));
        }
      }
    
      /* Disable command echo from Bluefruit */
      ble.echo(false);
    
      Serial.println("Requesting Bluefruit info:");
      /* Print Bluefruit information */ 
      ble.info();
      
    
      Serial.println(F("Setting beacon configuration details: "));
    
      // AT+BLEBEACON=0x004C,01-12-23-34-45-56-67-78-89-9A-AB-BC-CD-DE-EF-F0,0x0000,0x0000,-54
      ble.print("AT+BLEBEACON="        );
      ble.print(BEACON_MANUFACTURER_ID ); ble.print(',');
      ble.print(BEACON_UUID            ); ble.print(',');
      ble.print(BEACON_MAJOR           ); ble.print(',');
      ble.print(BEACON_MINOR           ); ble.print(',');
      ble.print(BEACON_RSSI_1M         );
      ble.println(); // print line causes the command to execute
    
      // check response status
      if (! ble.waitForOK() ) {
        error(F("Didn't get the OK"));
      }
    
      Serial.println();
      Serial.println(F("Open your beacon app to test"));
  
  Serial.println(F("******************************"));

  // loop through all the pins and intialize them 
  for(int thisPin = 0; thisPin < pinCount; thisPin++){
    pinMode( ledPins[thisPin], OUTPUT );
  }
}


void loop()
{
  
   // loop through the lightPins and get the data, map and write it
    for(int thisPin = 0; thisPin < pinCount; thisPin++){
          sensorValue = analogRead(lightPins[thisPin]);
          outputValue = mapSensorValue(sensorValue);
          analogWrite(ledPins[thisPin], outputValue);
          sendCalculatedData(outputValue);
    }
    
   delay(500);
}

void recieveData(){
  // Echo received data
  while ( ble.available() )
  {
//    int c = ble.read();
//
//    Serial.print((char)c);
//
//    // Hex output too, helps w/debugging!
//    Serial.print(" [0x");
//    if (c <= 0xF) Serial.print(F("0"));
//    Serial.print(c, HEX);
//    Serial.print("] ");
  }

}

void sendCalculatedData(int value){
  char n, inputs[BUFSIZE+1];
  
//  if (Serial.available())
//  {
//    n = Serial.readBytes(inputs, BUFSIZE);
//    inputs[n] = 0;
//    // Send characters to Bluefruit
    Serial.print("Sending: ");
    Serial.println(value);
//
//    // Send input data to host via Bluefruit
//    ble.print(value);
//  }

//    ble.print(value);
}

int mapSensorValue(int sensorValue) {
  int reMappedValue = 0;

  Serial.println(sensorValue); 

  outputValue = map(sensorValue, 1, 700, 1, 20);
  reMappedValue = map(outputValue, 1, 20, 1, 255);
  
  
//   Serial.print("sensor value = ");
//   Serial.print(sensorValue);
//   Serial.print("\t mapped value = ");
//   Serial.print(outputValue);
//   Serial.print("\t remapped = ");
//   Serial.println(reMappedValue);


  return reMappedValue;
}

