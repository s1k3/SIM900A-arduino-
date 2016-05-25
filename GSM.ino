#include <SoftwareSerial.h>
#include <dht.h>
dht DHT;
#define DHT11_PIN 5
#define DEBUG true

SoftwareSerial gsm900a(2, 3);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  gsm900a.begin(19200);

  sendCommand("AT\r\n", 1000, DEBUG);
  sendCommand("AT+SAPBR=3,1,\"APN\",\"Internet\"\r\n", 1000, DEBUG);
  sendCommand("AT+SAPBR=1,1\r\n", 3000, DEBUG);
  sendCommand("AT+HTTPINIT\r\n", 3000, DEBUG);

}

void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT.read11(DHT11_PIN);
  String URL = "AT+HTTPPARA=\"URL\",\"http://robin.swadhinit.com/gsm.php?value=";
  URL += DHT.temperature;
  URL += "\"\r\n";
  sendCommand(URL, 2000, DEBUG);
  sendCommand("AT+HTTPACTION=0\r\n", 3000, DEBUG);
  sendCommand("AT+HTTPREAD\r\n", 4000, DEBUG);

}


String sendCommand(String command, const int timeout, boolean debug)
{
  String response = "";
  gsm900a.print(command);
  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (gsm900a.available())
    {
      char c = gsm900a.read();
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
