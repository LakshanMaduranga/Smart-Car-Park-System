//done this is working code

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>

#define FIREBASE_HOST "car-parking-afb69-default-rtdb.asia-southeast1.firebasedatabase.app"       // Firebase HOST
#define FIREBASE_AUTH "w17FW9aVjXoYXNOKVEKSoQQ17zopeZVfhUPhHqnR"  // Firebase AUTH code
//#define WIFI_SSID "Maduranga"                                 // Enter your WIFI Name
//#define WIFI_PASSWORD "password"   
#define WIFI_SSID "POCO F3"                                 // Enter your WIFI Name
#define WIFI_PASSWORD "12345678"
//#define WIFI_SSID "Dialog 4G 995"                                 // Enter your WIFI Name
//#define WIFI_PASSWORD "4C29AAaF"
// Enter your Password

  


//Create Servo Object
Servo servoGate;

// Define Parking Slots IR Sensor Pins
int sensor1 = D0;
int sensor2 = D1;
int sensor3 = D2;

// Define Entrance and Exit Gate IR Sensor Pins
int sensorEntrance = D5;
int sensorExit = D6;

// Define Servo Motor Pin
int servoGatePin = D7;

// Variable to store the servo position
int pos = 0;
// Flag to indicate if the gate is open
bool isOpen = false;

// NOTE : IR Value 1 = Parking Slot Available | IR Value 0 = Parking Slot Not Available

void setup() {
  Serial.begin(9600);

  servoGate.attach(servoGatePin);

  // Initialize Parking Slots IR Sensor Pins
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  // Initialize Gate IR Sensor Pins
  pinMode(sensorEntrance, INPUT);
  pinMode(sensorExit, INPUT);

  // WIFI Connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to Firebase...");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Firebase Connected.");

}

void loop() {

  // If the entrance sensor is triggered
  if (digitalRead(sensorEntrance) == 0) {
    // If any of the parking sensors detect a vehicle
    if ((digitalRead(sensor1) != 0) || (digitalRead(sensor2) != 0) || (digitalRead(sensor3) != 0)) {
      // Gate Opening to Enter...
      gateOpen();
    } else {
      // If no parking slots are available
      // Parking Slots are not Available...
      gateClose();
    }

  } else if (digitalRead(sensorExit) == 0) { // If the exit sensor is triggered
    // Gate Opening to Exit...
    gateOpen();
  } else {
    // If neither entrance nor exit sensor is triggered
    //Gate Closed...
    gateClose();
  }

  // NOTE : IR Value 1 = Parking Slot Available | IR Value 0 = Parking Slot Not Available

  if (digitalRead(sensor1) != 0) {
    Firebase.setInt("/Parking_Area/Parking_Slots/Slot1", 0);
    Serial.println("Parking Slot 1 is Available...");
  } else {
    Firebase.setInt("/Parking_Area/Parking_Slots/Slot1", 1);
    Serial.println("Parking Slot 1 is Not Available...");
  }

  if (digitalRead(sensor2) != 0) {
    Firebase.setInt("/Parking_Area/Parking_Slots/Slot2", 0);
    Serial.println("Parking Slot 2 is Available...");
  } else {
    Firebase.setInt("/Parking_Area/Parking_Slots/Slot2", 1);
    Serial.println("Parking Slot 2 is Not Available...");
  }

  if (digitalRead(sensor3) != 0) {
    Firebase.setInt("/Parking_Area/Parking_Slots/Slot3", 0);
    Serial.println("Parking Slot 3 is Available...");
  } else {
    Firebase.setInt("/Parking_Area/Parking_Slots/Slot3", 1);
    Serial.println("Parking Slot 3 is Not Available...");
  }

}

void gateOpen() {
  if (!isOpen) {
    servoGate.attach(servoGatePin);
    for (pos = 90; pos >= 0; pos -= 1) {
      servoGate.write(pos);
      delay(15);
    }

    isOpen = true;
    delay(500);
  }
}

void gateClose() {
  if (isOpen) {
    for (pos = 0; pos <= 90; pos += 1) {
      servoGate.write(pos);
      delay(15);
    }

    isOpen = false;
    delay(500);
  }
}
