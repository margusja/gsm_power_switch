#include <Arduino.h>

#include <SoftwareSerial.h>

SoftwareSerial gsm(7,8);
String msg;
const int LED = 13;
String message = "";

void send_message(String message)
{
  // gsm.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  // delay(100);  
  gsm.println("AT+CMGS=\"+3725148780\""); // Replace it with your mobile number
  delay(100);
  gsm.println(message);   // The SMS text you want to send
  delay(100);
  gsm.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  gsm.println();
  delay(5000);  
}

void showSMS()
{
 delay(1000);
 msg = "";
 while(gsm.available() > 0)
 {
  msg = gsm.readString();
  Serial.println(msg);
 }

}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void checkATCommand(String errorMessage, String okMessage) {
    if (gsm.find("OK"))
    Serial.println(okMessage);
  else {
    Serial.println(errorMessage);
    Serial.println("Going to reset controller...");
    delay(1000);
    resetFunc();
  }
}

void setup() {
  gsm.begin(9600);
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // set SMS mode to text mode
  gsm.print("AT+CMGF=1\r");
  checkATCommand("Setting GSM module to text mode is failed", "GSM module is in text mode");
 
  
  // set gsm module to tp show the output on serial out
  gsm.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100); 
  checkATCommand("Setting GSM module to serial out mode is failed", "GSM module is out serial mode");

  // delete all sms's
  gsm.print("AT+CMGD=1,4\r");
  delay(100);
  checkATCommand("Deleting SMS's is failed", "ALL SMS's deleted");
}


void loop() {

  showSMS();

  if(msg.indexOf("on")>=0)
  {
    digitalWrite(LED, HIGH);
    message = "Led is turned ON";
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
  } 
  if (msg.indexOf("off")>=0)
  {
    digitalWrite(LED, LOW);
    message = "Led is turned OFF";
    // Send a sms back to confirm that the relay is turned off
    send_message(message);
  }


}