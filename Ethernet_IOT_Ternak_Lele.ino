//INI YANG FIX

#include <Ethernet.h>
#include <SPI.h>
#include <OneWire.h> //suhu
#include <DallasTemperature.h> //suhu
#include <Wire.h> //tbd

#define ONE_WIRE_BUS 5 //suhu
OneWire oneWire(ONE_WIRE_BUS); //suhu
DallasTemperature sensorSuhu(&oneWire); //suhu

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x2A, 0x8D };
byte ip[] = { 192, 168, 1,  3 };
byte gw[] = {192, 168, 1, 1};
byte subnet[] = { 255, 255, 255, 0 };

byte server[] = { 192, 168, 1, 5}; // Server IP
int data;

//suhu
float suhuSekarang;

void setup()
{
  Serial.begin(9600);               // Used for serial debugging
  sensorSuhu.begin();      //suhu
}
void loop()
{
  senddata();                                 // Data is sent every 5 seconds
}
void senddata()
{
  Ethernet.begin(mac, ip, gw, subnet);
  EthernetClient client;//(server, 80);
  suhuSekarang = ambilSuhu();
  Serial.println(suhuSekarang);
  delay(700);
  //---------------------------------- BATAS SUHU
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1000);
  //-----------------------BATAS AKHIR TBD
  
  if (client.connect(server, 80)) {
    Serial.println("Connected");
    client.print("GET /iot/load.php?suhu=");
    client.print(suhuSekarang);
    client.print("&kekeruhan=");
    client.print(sensorValue);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.1.5");
    client.println();
    Serial.println();
  }

  else
  {
    Serial.println("Connection unsuccesful");
  }
  //}
  //stop client
  client.stop();
  while (client.status() != 0)
  {
    delay(5);
  }
}
float ambilSuhu(){
    sensorSuhu.requestTemperatures();
   float suhu = sensorSuhu.getTempCByIndex(0);
   return suhu;
  }
