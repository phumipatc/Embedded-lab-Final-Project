#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

EspSoftwareSerial::UART testSerial;

// WiFi credentials
const char* ssid = "Pun mai ngub";
const char* password = "punpunpun3";

// Set your Firebase project's unique identifier
const char* firebaseHost = "embbeded-final-project-water-default-rtdb.asia-southeast1.firebasedatabase.app";

// Set your Firebase project's authentication credentials
const char* firebaseAuth = "WtZpws8GzsHQx41SxqoM3wqTODSeMAiDHoGbCzDa";

// Create an instance of the Firebase Data object
FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // testSerial.begin(115200, EspSoftwareSerial::SWSERIAL_8N1, D7, D8, false, 95, 11);
  
  // Initialize Firebase
  Firebase.begin(firebaseHost, firebaseAuth);
}
char buffer[10];
void loop() {

  if(testSerial.available() > 0){
    int cnt = 0;
    while(cnt != 12){
      while(testSerial.available() > 0){
        buffer[cnt++] = testSerial.read();
        // Serial.print("Received: ");
        // Serial.println(receivedData);
        // Serial.println(buffer[cnt-1]);
      }
    }
    buffer[cnt] = NULL;
    Serial.println(buffer);

    int TDS_Value = 0;
    for(int i=1;i<=3;i++){
      TDS_Value*=10;
      TDS_Value+=buffer[i]-'0';
    }
    int Thermo_Value = 0;
    for(int i=5;i<=7;i++){
      Thermo_Value*=10;
      Thermo_Value+=buffer[i]-'0';
    }
    int DissolvedO2_Value = 0;
    for(int i=9;i<=11;i++){
      DissolvedO2_Value*=10;
      DissolvedO2_Value+=buffer[i]-'0';
    }
    // Send the data to Firestore
    if (Firebase.setInt(firebaseData, "/TDS_Value", TDS_Value)) {
      Serial.println("TDS_Value sent to Firestore successfully");
    } else {
      Serial.println("Failed to send data to Firestore");
      Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setInt(firebaseData, "/Thermo_Value", Thermo_Value)) {
      Serial.println("Thermo_Value sent to Firestore successfully");
    } else {
      Serial.println("Failed to send data to Firestore");
      Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setInt(firebaseData, "/DissolvedO2_Value", DissolvedO2_Value)) {
      Serial.println("DissolvedO2_Value sent to Firestore successfully");
    } else {
      Serial.println("Failed to send data to Firestore");
      Serial.println(firebaseData.errorReason());
    }
    // Delay before sending the next data
    // delay(5000);
  }
}
