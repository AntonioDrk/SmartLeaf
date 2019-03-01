#define soilHumPinAn A4  // SoilHumidity pin (analog one)
#define waterLevelPin A3 // Water level sensors pin (analog one)
#define soilHumPinDig 2  // SoilHumidity pin (digital one)
#define pumpPin 5        // Pump activation pin (digital one)

// Defines the acceptable error of the sensors (so things don't update if the difference isn't bigger than the PRECISION_ERROR)
#define PRECISION_ERROR 20

// Defines how long will the pump be activated for (in milliseconds)
#define pumpInterval 3000

// Verifies every hour(in milliseconds) if the soil humidity is at it's target value
#define autoModeInterval 3600000

// Variables used to calculate intervals
long int previousMillisPump = 0;
long int previousMillisAutoMode = 0;

float maxMoisture = 2000; // target moisture to reach
int state = 0;            // recieved state from the phone
int soilHum = 0;          // read measured data of the soil humidity sensor
int waterLevel = 0;       // read measured data of the water level sensor
bool pumpDone = true;     // trigger for keeping the pump on and stopping it

void setup()
{
  // Setting up the pins
  pinMode(soilHumPinAn, INPUT);
  pinMode(soilHumPinDig, INPUT);
  pinMode(waterLevelPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  // Making sure no leak of voltage activates the pump by accident
  digitalWrite(pumpPin, LOW);
  // Opening the serial for communication with the bluetooth module
  Serial.begin(9600);
}

void loop()
{
  // Temporaries for reading the measured data to be verified after
  int tempSoilHum = analogRead(soilHumPinAn);
  int tempWaterLevel = analogRead(waterLevelPin);

  // If the measured data has a difference of PRECISION_ERROR with the last read, then don't change it, else change it
  if (abs((soilHum - tempSoilHum)) > PRECISION_ERROR)
  {
    soilHum = tempSoilHum;
  }
  else
  {
    soilHum = soilHum;
  }

  if (abs((waterLevel - tempWaterLevel)) > PRECISION_ERROR)
  {
    waterLevel = tempWaterLevel;
  }
  else
  {
    waterLevel = waterLevel;
  }

  // If the autoModeInterval passed we check if the moisture level we read is lower(yes, a bigger number means a lesser moist soil) than the maxMoisture set by the user
  if (((millis() - previousMillisAutoMode) > autoModeInterval) && (soilHum > maxMoisture - PRECISION_ERROR))
  {
    previousMillisAutoMode = millis();
    digitalWrite(pumpPin, HIGH);
    previousMillisPump = millis();
    pumpDone = false;
  }

  if (Serial.available() > 0) // Checks whether data is comming from the serial port
    state = Serial.read();    // Reads the data from the serial ports

  // State '1' is "interogation state", we write the read data from the sensors with a ',' between them
  // The phone app does the "decoding"
  if (state == '1')
  {
    Serial.println(String(soilHum) + "," + String(waterLevel));
    state = 0;
  }

  // State '2' is "set state", after this signal is set we wait for more data to recieve
  if (state == '2')
  {
    // Close the pump in case of recieving new data (so it doesn't stay on while we wait)
    digitalWrite(pumpPin, LOW);
    int bytesRead[2];

    while (Serial.available() != 1) // Await the next set of instruction (that being the target moisture)
    {
    }

    // Read the target moisture (it's in percentage)
    maxMoisture = (float)Serial.read();
    // Transform it in raw analog data to make comparisons more easy
    maxMoisture = ceil(1024 - ((1024 * maxMoisture) / 100));
    // Revert the state to 0 so we can read other states
    state = 0;
  }

  // State '3' is "activation" signal, here we just activate the pump pin with a high voltage so it waters the plants
  if (state == '3')
  {
    digitalWrite(pumpPin, HIGH);
    previousMillisPump = millis();
    pumpDone = false;

    state = 0;
  }

  // Check to see if the interval of the pump passed and it's running so we can close it
  if (millis() - previousMillisPump > pumpInterval && !pumpDone)
  {
    digitalWrite(pumpPin, LOW);
    pumpDone = true;
  }
}
