// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "ESP_01_AP_Name"
#define REMOTEXY_WIFI_PASSWORD "ESP_01_AP_PS"
#define REMOTEXY_SERVER_PORT 6377

#define DEBUG true
String mySSID = "Router_AP_Name";       // WiFi SSID
String myPWD = "Router_AP_PS"; // WiFi Password
String myAPI = "Your_thingspeak_API";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD1 = "field1";
String myFIELD2 = "field2";
String myFIELD3 = "field3";
String myFIELD4 = "field4";
String myFIELD5 = "field5";
String myFIELD6 = "field6";
int sendVal;

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   
  { 255,1,0,33,0,47,0,16,177,1,2,1,23,38,22,11,2,26,31,31,
  79,78,0,79,70,70,0,67,4,3,16,24,8,2,26,11,67,4,29,16,
  17,8,2,26,11,67,4,48,16,12,8,2,26,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_router; // =1 if switch ON and =0 if OFF 

    // output variables
  char text_label[11];  // string UTF8 end zero 
  char text_sensor_value[11];  // string UTF8 end zero 
  char text_measuring_unit[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)


//custom settings
#define PIN_SWITCH_ROUTER 12
unsigned long prevDisplayMillis = 0;
unsigned long prevDisplayMillis1 = 0;
int count =1;
int count2 =1;
int sec =0;
int sec2 =0;




void setup() {
  Serial.begin(115200);
  RemoteXY_Init (); 
  pinMode (PIN_SWITCH_ROUTER, OUTPUT);  

  espSendData("AT+RST", 1000, DEBUG);      //Reset the ESP8266 
  espSendData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);

  delay(1000);
}



void loop() { 
  unsigned long currentMillis = millis();
  digitalWrite(PIN_SWITCH_ROUTER, (RemoteXY.switch_router==0)?LOW:HIGH);

  
  if(currentMillis - prevDisplayMillis1 >= 60000){  
    //this loop send data to server every one minute adjust 60000 as you need.

    prevDisplayMillis1 = currentMillis;
    Serial.println("From First Loop");

    sendVal = random(10); // Send a random number between 1 and 1000
    
    // Please adjust using your thingspeak total field.
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD1 +"="+String(sendVal)+"&"+ myFIELD2 +"="+String(sendVal)+"&"+ myFIELD3 +"="+String(sendVal)+"&"+ myFIELD4 +"="+String(sendVal)+"&"+ myFIELD5 +"="+String(sendVal)+"&"+ myFIELD6 +"="+String(sendVal);

 
    espSendData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espSendData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    Serial.find(">"); 
    Serial.println(sendData);
     
    espSendData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
  }
  else{
    //this loop runs if the first loop isn't running. and manages remoteXY app data.
    RemoteXY_Handler ();

    if (currentMillis - prevDisplayMillis >= 1000){
      prevDisplayMillis = currentMillis;
      if(count==1){
        sprintf(RemoteXY.text_label,"Out_1");
        dtostrf(sec, 0, 0, RemoteXY.text_sensor_value);
        sprintf(RemoteXY.text_measuring_unit, "V"); 
        sec++;
        count++; 
      }
      else if(count==2){
        sprintf(RemoteXY.text_label,"Out_2");
        dtostrf(sec2, 0, 0, RemoteXY.text_sensor_value);
        sprintf(RemoteXY.text_measuring_unit, "V");  
        sec2+=5;
        count++; 
      }
      else if(count==3){
        sprintf(RemoteXY.text_label,"Out_3");
        dtostrf(sec, 0, 0, RemoteXY.text_sensor_value);
        sprintf(RemoteXY.text_measuring_unit, "V");  
        sec++;
        count++; 
      }
      else if(count==4){
        sprintf(RemoteXY.text_label,"Cell_1");
        dtostrf(sec2, 0, 0, RemoteXY.text_sensor_value);
        sprintf(RemoteXY.text_measuring_unit, "V");  
        sec2+=5;
        count++; 
      }
      else if(count==5){
        sprintf(RemoteXY.text_label,"Cell_2");
        dtostrf(sec, 0, 0, RemoteXY.text_sensor_value);
        sprintf(RemoteXY.text_measuring_unit, "V");  
        sec++;
        count++; 
      }
      else if(count==6){
        sprintf(RemoteXY.text_label,"Cell_3");
        dtostrf(sec2, 0, 0, RemoteXY.text_sensor_value);
        sprintf(RemoteXY.text_measuring_unit, "V");
        sec2+=5;
        count=1;
      }
    } 
  }
}


String espSendData(String command, const int timeout, boolean debug){ 
  String response = "";
  Serial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis()){
    while (Serial.available()){
      char c = Serial.read();
      response += c;
    }
  }
  return response;
}
