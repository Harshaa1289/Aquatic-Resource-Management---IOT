
/*
1. LCD connect
2. wifi
3. water level in head water tank - 3 level sensor
4. water level in sump with 2 level sensor
5. bore water pump  --> head water tank
6. muncipal water pump  -- > sump
7. sump water pump  --> to head water tank
8. muncipal water coming or not? 
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16,2);  

int headTankHIGH, headTankMED, headTankLOW, sumpTankHIGH, sumpTankLOW, muncipalIncoming; //variables to store digital readings
int flagSump = 0;         // 0 is full and 1 is empty
int flagHead = 0;         //flagHead shows over head tank level. 0 is full and 1 is empty

int headWatertankHIGH = 2;
int headWatertankMED = 3;
int headWatertankLOW = 4;

int sumpWatertankHIGH = 5;
int sumWatertankLOW = 6;
int muncipalWaterIncoming = 7;

int muncipalPump = 8;       //relay to turn on muncipal water incoming
int sumpPump = 9;           //relay to turn on sump water pump
int borePump = 10;          //relay to turn on bore water pump

int waterHighLED = A0;
int waterMedLED = A1;
int waterLowLED = A2;


void setup() 
{
  Serial.begin(9600);
  pinMode(headWatertankHIGH, INPUT_PULLUP);
  pinMode(headWatertankMED, INPUT_PULLUP);
  pinMode(headWatertankLOW, INPUT_PULLUP);
  pinMode(sumpWatertankHIGH, INPUT_PULLUP);
  pinMode(sumWatertankLOW, INPUT_PULLUP);
  pinMode(muncipalWaterIncoming, INPUT_PULLUP);

  pinMode(muncipalPump, OUTPUT);
  pinMode(sumpPump, OUTPUT);
  pinMode(borePump, OUTPUT);
  pinMode(waterHighLED, OUTPUT);
  pinMode(waterLowLED, OUTPUT);
  pinMode(waterMedLED, OUTPUT);

  digitalWrite(waterHighLED, LOW);
  digitalWrite(waterLowLED, LOW);
  digitalWrite(waterMedLED, LOW);
  
  lcd.begin();                 
  lcd.backlight();
  lcd.clear();
  lcd.print("    AUTOMATIC   ");
  lcd.setCursor(0,1);
  lcd.print("WATER CONTROLLER");
  Serial.println("Automatic water controller project started...");
  delay(2000);

}

void loop() 
{
  digitalWrite(sumpPump, HIGH);
  digitalWrite(muncipalPump, HIGH);
  digitalWrite(borePump, HIGH);
  lcd.clear();
  lcd.print("WATER TANK LEVEL:");
  headTankHIGH = digitalRead(headWatertankHIGH);
  headTankLOW = digitalRead(headTankLOW);
  headTankMED = digitalRead(headWatertankMED);
  sumpTankHIGH = digitalRead(sumpWatertankHIGH);
  sumpTankLOW = digitalRead(sumWatertankLOW);
  muncipalIncoming = digitalRead(muncipalWaterIncoming);

  if(muncipalIncoming == 0)                             //condition if muncipal water coming
  {
    Serial.println("Muncipal water coming...");
    delay(10);
    if(sumpTankHIGH == 1 || headTankHIGH == 1)          // condition if sump tank and over head tank are not full
    {
      Serial.println("Muncipal water pump turned on...");
      delay(10);
      digitalWrite(muncipalPump, LOW);
    }

    if(sumpTankHIGH == 0 && headTankHIGH == 0)           //condition if both tanks are full
    {
      Serial.println("Tanks full, muncipal water pump off...");
      delay(10);
      digitalWrite(muncipalPump, HIGH);
    }
  }


  if(headTankHIGH == 1 && headWatertankLOW == 1 && headWatertankMED == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("EMPTY");
    flagHead = 1;
    Serial.println("Overhead tank empty...");
    delay(10);
  }

  if(flagHead == 1 && muncipalIncoming == 1)          //check if overhead tank is completly empty and muncipal water coming or not
  {
    Serial.println("Muncipal water not available...");      //if muncipal water not coming...
    delay(10);
    if(sumpTankLOW == 0)                                    //if water available in sump tank
    {
      Serial.println("Turning on sump water pump...");      //turn on the sump tank water pump to over head tank
      delay(10);
      digitalWrite(sumpPump, LOW);
      digitalWrite(borePump, HIGH);
    }
    if(sumpTankLOW == 1)                                    //if sump tank is empty, turn on the bore water pump
    {
      Serial.println("Sump empty...");
      Serial.println("Turning on bore water pump..");
      delay(10);
      digitalWrite(sumpPump, HIGH);
      digitalWrite(borePump, LOW);
    }
  }

  if(headTankHIGH == 0)              //if over head tank is full, turn off sump and bore water pumps
  {
    lcd.setCursor(0,1);
    lcd.print("HIGH");
    digitalWrite(waterHighLED, HIGH);
    flagHead = 0;
    digitalWrite(sumpPump, HIGH);
    digitalWrite(borePump, HIGH);
    Serial.println("Overhead tank full..");
    delay(10);
  }
  if(headTankLOW == 0)
  {
    lcd.setCursor(0,1);
    lcd.print("LOW");
    digitalWrite(waterLowLED, HIGH);
  }
  if(headTankMED == 0)
  {
    lcd.setCursor(0,1);
    lcd.print("MEDIUM");
    digitalWrite(waterMedLED, HIGH);
  }
  digitalWrite(waterHighLED, LOW);
  digitalWrite(waterMedLED, LOW);
  digitalWrite(waterLowLED, LOW);
  delay(1000);
}
