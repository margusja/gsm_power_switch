#include <Arduino.h>

#include <SoftwareSerial.h>
SoftwareSerial gsm(7,8);
String msg;
const int LED = 13;
String message = "";

void send_message(String message)
{
  gsm.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  gsm.println("AT+CMGS=\"+3725148780\""); // Replace it with your mobile number
  delay(100);
  gsm.println(message);   // The SMS text you want to send
  delay(100);
  gsm.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  gsm.println();
  delay(1000);  
}

void showSMS()
{
 gsm.print("AT+CMGF=1\r");
 gsm.print("AT+CNMI=2,2,0,0,0\r");
 delay(1000);
 msg = "";
 while(gsm.available() > 0)
 {
  msg = gsm.readString();
  Serial.println(msg);
 }
}

void setup() {
  gsm.begin(9600);
  Serial.begin(9600);
  Serial.println("PUMP CONTROL");
  Serial.println("t : to receive text");
  delay(1000);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

    // set SMS mode to text mode
  gsm.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to tp show the output on serial out
  gsm.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);
}

void loop() {

  showSMS();


  while(gsm.available() > 0)
  {
    Serial.write(gsm.read());
  }

  if(msg.indexOf("turn_on")>=0)
  {
    digitalWrite(LED, HIGH);
    message = "Led is turned ON";
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
  } 
  if (msg.indexOf("turn_off")>=0)
  {
    digitalWrite(LED, LOW);
    message = "Led is turned OFF";
    // Send a sms back to confirm that the relay is turned off
    send_message(message);
  }


}