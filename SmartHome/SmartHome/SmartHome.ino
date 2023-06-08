#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

const int YanginSensor = A0; // Yangın sensörünün bulunduğu pin
const int buzzer = 21; // Buzzerın bullunduğu pin

const int HareketSensor = A1; // Hareket sensörünün bulunduğu pin
const int led = 18; // Hareket sensörü ledinin bağlıolduğu pin

const int sensor = A2; // Sıcaklık sensörünün bağlı olduğu pin
const int rs = 8, e = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13; //Ekranın takılı olduğu pinler
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

const int led_y = 19; // Yeşil ledin bağlı olduğu pin
const int led_k = 20; // Kırmızı ledin bağlı olduğu pin

char girilensifre[4]; 
char sifre[5] = {'1', '2', '3', '4'};
int i = 0;
char key_pressed = 0;

const byte ROWS = 4;
const byte COLS = 3;

char tuslar[ROWS][COLS] = {

  {'1', '2', '3'},

  {'4', '5', '6'},

  {'7', '8', '9'},

  {'*', '0', '#'}
};

byte rowPins[ROWS] = {A8, A9, A10, A11};
byte colPins[COLS] = {A5, A6, A7};

Keypad Tuslar = Keypad(makeKeymap(tuslar), rowPins, colPins, ROWS, COLS);



void setup()
{
  pinMode (YanginSensor, INPUT);
  pinMode (buzzer, OUTPUT);
  pinMode (HareketSensor, INPUT);
  pinMode (led, OUTPUT);
  pinMode (led_y, OUTPUT);
  pinMode (led_k, OUTPUT);
  lcd.begin(16, 2);
}

void loop()
{

  ////////// YANGIN SENSÖRÜ /////////
  
  int YanginSensorState = digitalRead (YanginSensor);
  if (YanginSensorState == HIGH) 
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }

  ///////////// HAREKET SENSÖRÜ /////////////
  
  int HareketSensorState = digitalRead (HareketSensor);
  if (HareketSensorState == HIGH)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }


  ////// SICAKLIK HESABI /////////
  
  int sicaklik;
  float vol;
  vol = analogRead(sensor);
  vol = (vol / 1023) * 5000; //değeri mV'a dönüştürür lm35 yapısı gereği bu işlemi yaptık
  vol = vol / 10, 0; // her 10mV 1 derece sıcaklık yaptığı için. 10 a bölerek sıcaklığı bulduk.
  sicaklik = vol + 0.3;
  lcd.setCursor(0, 0);
  lcd.print("SICAKLIK: ");
  lcd.setCursor(10, 0);
  lcd.print(sicaklik);
  lcd.setCursor(12, 0);
  lcd.print("C");
  if (sicaklik < 20) {
    lcd.setCursor(0, 1);
    lcd.print("SICAKLIK DUSUK");
  }
  else if (sicaklik > 30) {
    lcd.setCursor(0, 1);
    lcd.print("SICAKLIK YUKSEK");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  /////// ŞİFRELEME ///////
  
  key_pressed = Tuslar.getKey();
  if (key_pressed)
  {
    girilensifre[i++] = key_pressed;
  }
  if (i == 4) // If 4 keys are completed
  {
    if (!(strncmp(girilensifre, sifre, 4)))
    {
      digitalWrite(led_y, HIGH);
      digitalWrite(led_k, LOW);
      i = 0;
      //delay(300);
      //digitalWrite(led_y, LOW);
    }
    else
    {
      digitalWrite(led_k, HIGH);
      digitalWrite(led_y, LOW);
      i = 0;
      //delay(300);
      //digitalWrite(led_k, LOW);
    }
}
}
