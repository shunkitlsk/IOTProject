#include <WiFi.h>
#include <SPI.h>  // Reference the SPI library
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
//#include <ThingsBoard.h>    // ThingsBoard SDK
#include <TinyGPS++.h>
#include <Arduino.h>
#include <SPI.h>  // Reference the SPI library
#include <SD.h>

File myFile;


//set up TXRX FOR GPS
#define RXD2 16
#define TXD2 17



char ssid[] = "test";
char password[] = "12345678";
// Please modify it to your own API Key, and change https to http
String url = "http://api.thingspeak.com/update?api_key=PO42QF5O5NARYBGX";
//---------------------------------------------------------
int ddt = 123, err = 0, sok;
String msg = "";          
int sww = 12, ii2 = 0, dil = 0, yy = 0, mm = 0, dd = 0, ttm = 0, mmm = 0, ssm = 0;
unsigned long wbb;
long wbb2;
//GPS setup
TinyGPSPlus gps;
/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
float headingDegrees, d_lat = 0, d_lng = 0, magx = 0, magy = 0, magz = 0, maga = 0;
sensors_event_t event;
//String SDD = "D:000.0cm H:000cm";
String SDD = "03/09/2021 12:12:12  000.00  000.00  000.00 000.00 00.000000 000.000000";
unsigned int row;

void setup()
{
  Serial.begin(115200);
  pinMode(sww, INPUT_PULLUP);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  /*



    while (1)
    {
      while (Serial2.available()) {
        if (gps.encode(Serial2.read()))
          displayInfo();
      }
    }

  */

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  err = 0;
  while (WiFi.status() != WL_CONNECTED && err <= 10) {
    //Serial.print(".");
    delay(1000);
    err++;
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);



    Serial.println("AT+CRESET");   // Restart the 4G module
    sok = 0; err = 0;
    while (sok == 0 && err <= 2000)
    {
      if (Serial.available()) { // When receiving the 4G module data
        if (Serial.find("PB DONE")) { //When the 4G module returns PB DONE, it means that the 4G module has been powered on          sok = 1;
        }
      }
      delay(10);
      err++;
    }
    if (err < 2000) Wait_CREG();







  //set up TXRX port for GPS module

  //Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  /* Initialise the sensor */
  if (!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    //Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while (1);
  }
  /* Display some basic information on this sensor */
  //displaySensorDetails();


  //SPI.begin ();                 // Start SPI connection
  if (!SD.begin(5)) {
    //Serial.print("SD init failed! ");
    delay(2000);
    return;
  }
  //Serial.print("SD init done.   ");
  delay(1000);
  myFile = SD.open("/data.txt");
  myFile.close();
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
void readInfo()
{
  if (gps.location.isValid())
  {
    d_lat = gps.location.lat();
    d_lng = gps.location.lng();
    /*
      Serial.print(d_lat, 6);
      Serial.print(F(","));
      Serial.print(d_lng, 6);
    */


  }


  if (gps.date.isValid())
  {
    dd = gps.date.day();
    mm = gps.date.month();
    yy = gps.date.year();

  }



  if (gps.time.isValid())
  {
    ttm = gps.time.hour();
    ttm = ttm + 8;
    ttm = ttm % 24;
    mmm = gps.time.minute();
    ssm = gps.time.second();

    /*
        Serial.print(ttm / 10); Serial.print(ttm % 10);
        Serial.print(F(":"));
        Serial.print(mmm / 10); Serial.print(mmm % 10);
        Serial.print(F(":"));
        Serial.print(ssm / 10); Serial.println(ssm % 10);
    */


  }

}

//gps module
void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);

  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
void Wait_CREG() {
  int ok = 0;
  delay(1000);
  while (ok == 0)
  {
    Serial.println("AT+CREG?");    //
    if ( Serial.available())
    {
      if ( Serial.find(",1") || Serial.find(",5")) 
      {
        ok = 1;      
      }
    }
    delay(500);
  }
  delay(1000);
  msg =  Serial.readString();           
}
void send_sim_m() {   //sim card to thingspeak
  Serial.println("AT+CIPMODE=1");
  delay(1000);
  Serial.println("AT+CSOCKSETPN=1");
  delay(1000);
  Serial.println("AT+CIPMODE=0");
  delay(1000);
  Serial.println("AT+NETOPEN");
  delay(1000);
  Serial.println("AT+CIPOPEN=0,\"TCP\",\"184.106.153.149\",80");//
  delay(2000);
  Serial.println("AT+CIPSEND=0,");
  delay(1000);
  String url2 = (String)"GET /update?key=PO42QF5O5NARYBGX" + (String)"&field1=" + magx + "&field2=" + magy + "&field3=" + magz + "&field4=" + maga + "&field5=" + String(d_lat, 6) + "&field6=" + String(d_lng, 6); //data to thingspeak

  Serial.println(url2);
  Serial.write(0x1A);
  delay(3000);
  Serial.println("AT+CIPCLOSE=0");
  delay(1000);
  Serial.println("AT+NETCLOSE");
}


void readmag()
{

  mag.getEvent(&event);

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  //Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  //Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  "); Serial.println("uT");
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);

  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  // Correct for when signs are reversed.
  if (heading < 0)
    heading += 2 * PI;
  // Check for wrap due to addition of declination.
  if (heading > 2 * PI)
    heading -= 2 * PI;
  // Convert radians to degrees for readability.
  headingDegrees = heading * 180 / M_PI;
  //Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
  //delay(500);
  magx = event.magnetic.x;
  magy = event.magnetic.y;
  magz = event.magnetic.z;
  maga = headingDegrees;
}


void wsdnew()
{
  unsigned int ii;
  myFile = SD.open("/data.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    row = myFile.size() / 73;       
    for (ii = 0; ii < row; ii++)
    {
      myFile.seek(ii * 70);
      if (myFile.read() == '-')
      {
        myFile.seek(ii * 73);
        break;
      }
    }
    if (ii == row)
    {
      myFile.seek(row * 73);
    }

    myFile.print(SDD);
    myFile.write(0x0d);
    myFile.write(0x0a);
    // close the file:
    myFile.close();

  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening   ");
  }

}

/*
  void wsdnew()
  {
  unsigned int ii;
  myFile = SD.open("/data.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {

    Serial.println(SDD);
    myFile.print(SDD);
    myFile.write(0x0d);
    myFile.write(0x0a);
    // close the file:
    myFile.close();

  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening   ");
  }

  }
*/

void save_data() {    

  //SDD = "03/09/2021 12:12:12 +000.00 +000.00 +000.00 000.00 00.000000 000.000000";



  SDD.setCharAt(0, (mm / 10) + 0x30);
  SDD.setCharAt(1, (mm % 10) + 0x30);
  SDD.setCharAt(3, (dd / 10) + 0x30);
  SDD.setCharAt(4, (dd % 10) + 0x30);
  SDD.setCharAt(6, yy / 1000 + 0x30);    
  SDD.setCharAt(7, (yy % 1000) / 100 + 0x30);
  SDD.setCharAt(8, (yy % 100) / 10 + 0x30);
  SDD.setCharAt(9, (yy % 10) + 0x30);

  SDD.setCharAt(11, (ttm / 10) + 0x30);
  SDD.setCharAt(12, (ttm % 10) + 0x30);
  SDD.setCharAt(14, (mmm / 10) + 0x30);
  SDD.setCharAt(15, (mmm % 10) + 0x30);
  SDD.setCharAt(17, (ssm / 10) + 0x30);
  SDD.setCharAt(18, (ssm % 10) + 0x30);

  wbb2 = magx * 100;
  if (wbb2 >= 0) SDD.setCharAt(20, ' ');
  else
  {
    SDD.setCharAt(20, '-'); wbb2 = abs(wbb2);
  }
  SDD.setCharAt(21, wbb2 / 10000 + 0x30);
  SDD.setCharAt(22, (wbb2 % 10000) / 1000 + 0x30);      SDD.setCharAt(23, (wbb2 % 1000) / 100 + 0x30);
  SDD.setCharAt(25, (wbb2 % 100) / 10 + 0x30);
  SDD.setCharAt(26, (wbb2 % 10) + 0x30);

  wbb2 = magy * 100;
  if (wbb2 >= 0) SDD.setCharAt(28, ' ');
  else
  {
    SDD.setCharAt(28, '-'); wbb2 = abs(wbb2);
  }
  SDD.setCharAt(29, wbb2 / 10000 + 0x30);
  SDD.setCharAt(30, (wbb2 % 10000) / 1000 + 0x30);      SDD.setCharAt(31, (wbb2 % 1000) / 100 + 0x30);
  SDD.setCharAt(33, (wbb2 % 100) / 10 + 0x30);
  SDD.setCharAt(34, (wbb2 % 10) + 0x30);

  wbb2 = magz * 100;
  if (wbb2 >= 0) SDD.setCharAt(36, ' ');
  else
  {
    SDD.setCharAt(36, '-'); wbb2 = abs(wbb2);
  }
  SDD.setCharAt(37, wbb2 / 10000 + 0x30);
  SDD.setCharAt(38, (wbb2 % 10000) / 1000 + 0x30);      SDD.setCharAt(39, (wbb2 % 1000) / 100 + 0x30);
  SDD.setCharAt(41, (wbb2 % 100) / 10 + 0x30);
  SDD.setCharAt(42, (wbb2 % 10) + 0x30);

  wbb = maga * 100;
  SDD.setCharAt(44, wbb / 10000 + 0x30);
  SDD.setCharAt(45, (wbb % 10000) / 1000 + 0x30);      SDD.setCharAt(46, (wbb % 1000) / 100 + 0x30);
  SDD.setCharAt(48, (wbb % 100) / 10 + 0x30);
  SDD.setCharAt(49, (wbb % 10) + 0x30);

  wbb = d_lat * 1000000;
  SDD.setCharAt(51, wbb / 10000000 + 0x30);
  SDD.setCharAt(52, (wbb % 10000000) / 1000000 + 0x30);      SDD.setCharAt(54, (wbb % 1000000) / 100000 + 0x30);      SDD.setCharAt(55, (wbb % 100000) / 10000 + 0x30);      SDD.setCharAt(56, (wbb % 10000) / 1000 + 0x30);      SDD.setCharAt(57, (wbb % 1000) / 100 + 0x30);
  SDD.setCharAt(58, (wbb % 100) / 10 + 0x30);
  SDD.setCharAt(59, (wbb % 10) + 0x30);

  wbb = d_lng * 1000000;
  SDD.setCharAt(61, wbb / 100000000 + 0x30);
  SDD.setCharAt(62, (wbb % 100000000) / 10000000 + 0x30);      SDD.setCharAt(63, (wbb % 10000000) / 1000000 + 0x30);      SDD.setCharAt(65, (wbb % 1000000) / 100000 + 0x30);      SDD.setCharAt(66, (wbb % 100000) / 10000 + 0x30);      SDD.setCharAt(67, (wbb % 10000) / 1000 + 0x30);      SDD.setCharAt(68, (wbb % 1000) / 100 + 0x30);
  SDD.setCharAt(69, (wbb % 100) / 10 + 0x30);
  SDD.setCharAt(70, (wbb % 10) + 0x30);
  /*
    SDD.setCharAt(2, alld2 / 1000 + 0x30);    
    SDD.setCharAt(3, (alld2 % 1000) / 100 + 0x30);
    SDD.setCharAt(4, (alld2 % 100) / 10 + 0x30);
    SDD.setCharAt(6, (alld2 % 10) + 0x30);
    SDD.setCharAt(12, stor[kk] / 100 + 0x30);     
    SDD.setCharAt(13, (stor[kk] % 100) / 10 + 0x30);
    SDD.setCharAt(14, (stor[kk] % 10) + 0x30);
  */

  wsdnew();


}
void loop()
{

  readmag();

  if (WiFi.status() == WL_CONNECTED) {
    
    HTTPClient http;
    save_data();

    String url1 = url + "&field1=" + magx + "&field2=" + magy + "&field3=" + magz + "&field4=" + maga + "&field5=" + String(d_lat, 6) + "&field6=" + String(d_lng, 6);

    
    http.begin(url1); //data to thingspeak
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)      {
      
      String payload = http.getString();
      
    } else {
      send_sim_m();   //sim card to thingspeak
    }
    http.end();

  }
  else {
    //Serial.println(“failed!!! !”);
    WiFi.reconnect();
    send_sim_m();   //sim card to thingspeak
  }


  for (ii2 = 0; ii2 < 10000; ii2++)
  {
    delay(2);
    if (digitalRead(sww) == LOW) {
      if (dil == 0)
      {
        dil = 1;
        Serial.print("ATD");
        //Serial.print("0921646726");   // Phone number
        Serial.print("59884450");   // Phone number
        
        Serial.println(';');
        delay(1000);
      }
      else
      {
        dil = 0; Serial.print("ATA"); delay(1000);
      }
      while (digitalRead(sww) == LOW)delay(1000);

    }

    if (Serial2.available()) {
      if (gps.encode(Serial2.read()))
        //displayInfo();
        readInfo();
    }
  }

}
