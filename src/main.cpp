#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial gsm(7,8);
String msg;
const int RELAY = 12;
const int STATUSLED = 13;
String message = "";

void send_message(String message)
{
  gsm.println("AT+CMGS=\"+3725148780\""); // Replace it with your mobile number
  delay(1000);
  gsm.println(message);   // The SMS text you want to send
  delay(1000);
  gsm.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
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
  delay(100);
}

void setup() {
  delay(15000);
  gsm.begin(9600);
  Serial.begin(9600);
  delay(1000);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  
  pinMode(STATUSLED, OUTPUT);
  digitalWrite(STATUSLED, LOW);

  // set SMS mode to text mode
  gsm.print("AT+CMGF=1\r");
  checkATCommand("Setting GSM module to text mode is failed", "GSM module is in text mode");
 
  // set gsm module to tp show the output on serial out
  gsm.print("AT+CNMI=2,2,0,0,0\r"); 
  checkATCommand("Setting GSM module to serial out mode is failed", "GSM module is out serial mode");

  // delete all sms's
  gsm.print("AT+CMGD=1,4\r");
  checkATCommand("Deleting SMS's is failed", "ALL SMS's deleted");

  digitalWrite(STATUSLED, HIGH);
  send_message("Relay is ready");
}


void loop() {

  showSMS();

  if(msg.indexOf("on")>=0)
  {
    digitalWrite(RELAY, LOW);
    // Send a sms back to confirm that the relay is turned on
    send_message("Relay is turned ON");
  } 
  if (msg.indexOf("off")>=0)
  {
    digitalWrite(RELAY, HIGH);
    // Send a sms back to confirm that the relay is turned off
    send_message("Relay is turned OFF");
  }


}