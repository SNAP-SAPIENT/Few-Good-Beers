
int lightPin = 0;  //define a pin for Photo resistor
int ledPin=10;     //define a pin for LED

int sensorValue = 0;
int outputValue = 0;

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    pinMode( ledPin, OUTPUT );
}


void loop()
{
   sensorValue = analogRead(lightPin);
   outputValue = mapSensorValue(sensorValue);
   analogWrite(ledPin, outputValue);

   delay(100);
}

int mapSensorValue(int sensorValue) {

  outputValue = map(sensorValue, 0, 1023, 0, 255);

   Serial.print("sensor value = ");
   Serial.print(sensorValue);
   Serial.print("\t output = ");
   Serial.println(outputValue);


  return outputValue;
}

