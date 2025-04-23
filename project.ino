#include <LiquidCrystal_I2C.h>
#include <PacketSerial.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
PacketSerial ps;

const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 6;

const byte distLimitCM = 30;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  ps.begin(115200);
}

void loop() {
  ps.update();
  double dist = distance();
  lcd.clear();
  String SerialData = "";
  SerialData = String(dist, 2);
  lcd.setCursor(0, 1);
  lcd.print(SerialData + " cm");
  if (dist > distLimitCM) {
    //hladina je příliš daleko
    tone(buzzerPin, 1000);
    lcd.setCursor(0, 0);
    lcd.print("TOO LOW");
  }
  else {
    //hladina je ok
    noTone(buzzerPin);
    lcd.setCursor(0, 0);
    lcd.print("OK");
  }

  Serial.println(dist);
  delay(1000);
}

double distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  const double duration = pulseIn(echoPin, HIGH, 30000);

  return (duration * 0.0343) / 2;
}

void sendPacket(String packet) {
  char chars = convertStringToCharArray(packet);
  Serial.println(sizeof(chars));
  ps.send(chars, sizeof(chars));
}

uint8_t* convertStringToCharArray(String S)
{
  S = S + "\n";
  int   ArrayLength  = S.length();
  char  CharArray[ArrayLength];
  S.toCharArray(CharArray, ArrayLength);

  return (uint8_t *)CharArray;
}
