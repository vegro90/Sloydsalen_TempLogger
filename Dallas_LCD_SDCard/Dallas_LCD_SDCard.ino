#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h> // https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h> // https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 8 // Select data pin(yellow)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallas(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Set the LCD address to 0x27 for a 16 chars and 2 line display

int sensorPin = A0;
int sensorValue = 0;
int ledPin = 13;

float Celsius = 0;
float Fahrenheit = 0;

const int chipSelect = 4;

/**********************************************/
/*    SETUP                                   */
/**********************************************/
void setup() {
  
  dallas.begin();
  Serial.begin(9600); 
  while(!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }


  // initialize the LCD
  lcd.begin();
  lcd.backlight();    // Turn on the blacklight and print a message.
  lcd.setCursor(0,0);
  lcd.print("Celsius: ");
  lcd.setCursor(0,1);
  lcd.print("Fahrenheit: ");  

  // SD CARD SETUP
  lcd.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {  // see if the card is present and can be initialized:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Card failed,");
    lcd.setCursor(0,1);
    lcd.print("or not present");
    delay(5000);
    // don't do anything more:
    return;
  }
  lcd.clear();
  lcd.println("card initialized.");
}


/**********************************************/
/*    LOOP                                    */
/**********************************************/
void loop() {
  dallas.requestTemperatures();
  Celsius = dallas.getTempCByIndex(0);
  Fahrenheit = dallas.toFahrenheit(Celsius);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Celsius: ");
  lcd.setCursor(0,1);
  lcd.print("Fahrenheit: ");
  lcd.setCursor(12,0);
  lcd.print( String(Celsius));
  lcd.setCursor(12,1);
  lcd.print( String(Fahrenheit));

  // SD CARD

  String dataString = "";
  dataString += String(Celsius);
  dataString += ",";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    lcd.clear();
    lcd.print("error opening:");
    lcd.setCursor(0,1);
    lcd.print("datalog.txt");
  }
  delay(5000);
}
