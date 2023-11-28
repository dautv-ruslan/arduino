#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <Key.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'4','1','7','*'},
  {'5','2','8','0'},
  {'6','3','9','#'},
  {'B','A','C','D'}
};
byte rowPins[ROWS] = {5,4,3,2};
byte colPins[COLS] = {11,10,7,6};
String state = "A";
int width = 0;
int iterations = 0;
boolean width_flag = false;
boolean iterations_flag = false;
Keypad customKeypad = Keypad(makeKeymap(hexaKeys),rowPins,colPins, ROWS,COLS);

LiquidCrystal_I2C lcd(0x27,16,2);

int echoPin = 9;
int trigPin = 8;
int downPin = 40;
int upPin = 12;
int motorPin = 31;
int inPin = 22;
int datchik1 = 24;
int datchik2 = 32;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("(A)  B   C   D "); 
  
  //lcd.setCursor(0,1);
  //lcd.print(width);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  char customKey = customKeypad.getKey();
  if (customKey){
    //Serial.println(customKey);
    switch (customKey) {
      case 'A':
      state = "A";
      width_flag = true;
      lcd.setCursor(0,0);
      lcd.print("(A)  B   C   D ");
      lcd.setCursor(0,1);
      lcd.print(width);
      lcd.print("             ");
      break;
      case 'B':
      state = "B";
      iterations_flag = true;
      lcd.setCursor(0,0);
      lcd.print(" A  (B)  C   D ");
      lcd.setCursor(0,1);
      lcd.print(iterations);
      lcd.print("               ");
      break;
      case 'C':
      if (state == "C") {
        lcd.clear();
      } else {
      state = "C";
      lcd.setCursor(0,0);
      lcd.print(" A   B  (C)  D ");
      lcd.setCursor(0,1);
      lcd.print("Continue? -> 'C'");
      }
      break;
      case 'D':
      if (state == "D") {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Starting...");
        work();
        lcd.setCursor(0,0);
        lcd.print("Finished!  ");
        
      } else {
      state = "D";
      lcd.setCursor(0,0);
      lcd.print(" A   B   C  (D)");
      lcd.setCursor(0,1);
      lcd.print("Start? -> 'D'   ");
      }
      break;
      case '*':
      case '#':
      break;
      default:
      if (state == "A") {
        if (width_flag) { width = 0;width_flag = false;}
        if (width < 10) {
          width = width * 10 + int(customKey) - 48;
        }
        lcd.setCursor(0,1);
        lcd.print(width);
        lcd.print("   ");
      }
      if (state == "B") {
        if (iterations_flag) {iterations = 0;iterations_flag = false;}
        if (iterations < 3) {
        iterations = iterations * 10 + int(customKey) - 48;
        }
        lcd.setCursor(0,1);
        lcd.print(iterations);
        lcd.print("        ");
      }
    }   
    //lcd.setCursor(15,1);
    //lcd.print(customKey);
  }
}

void work() {
  //pinMode(40, OUTPUT);
  //pinMode(13, OUTPUT);
  //pinMode(14, OUTPUT);
  //pinMode(15, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(downPin, OUTPUT);
  pinMode(upPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(inPin, OUTPUT);
  pinMode(datchik1, INPUT);
  pinMode(26, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(datchik2, INPUT);
  
  int w = width;
  int n = 1;
  
  while (n <= iterations) {
  
  setRuler(w); // 3 downPin = 40
  
  digitalWrite(motorPin, HIGH);
  delay(1000);
  digitalWrite(motorPin, LOW); // 4 motorPin = 31
  
  digitalWrite(inPin, HIGH);
  delay(1000);
  digitalWrite(inPin, LOW); // 5 inPin = 22
  
  while (digitalRead(datchik2) == LOW) {
    delay(100);
  } // datchik1 = 32
  
  digitalWrite(26, HIGH);
  delay(1000);
  digitalWrite(26, LOW);
  
  upRuler(); // upPin = 12
  
  digitalWrite(28, HIGH);
  delay(1000);
  digitalWrite(28, LOW); // 6

  while(digitalRead(datchik1) == LOW) {
    delay(100);  
  } // datchik2 = 24

  digitalWrite(26, HIGH);
  delay(1000);
  digitalWrite(26, LOW); // 7
  
  n++; // 1, 2, 3, 4, 5, ..
  w += width; // 25, 50, 75, ..
  
  }
}   
  
void setRuler(int cantimeter) {
  int duration;
  int cm = 0;
  lcd.clear();
  while (cm != cantimeter) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = duration / 5.8;

  //Serial.println(cm);
 
  if (cm<0) {
    cm = 0;
    continue;  
  }
  
  delay(100);
  
  if (cm<cantimeter) {
    digitalWrite(downPin, HIGH);
  }
  if (cm==cantimeter) {
    digitalWrite(downPin, LOW);
  }
  
  lcd.setCursor(0,0);
  lcd.print(cm);
  lcd.println("   ");
  
  }
}

void upRuler() {
  int duration;
  int cm = 26;
  lcd.clear();
  while (cm >25) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = duration / 5.8;

  //Serial.println(cm);
 
  if (cm<0) {
    cm = 26;
    continue;
  }
 
  delay(100);
  
  if (cm>25) {
    digitalWrite(upPin, HIGH);
  }
  if (cm<=25) {
    digitalWrite(upPin, LOW);
  }
  
  lcd.setCursor(1,1);
  lcd.print(cm);
  lcd.println("   ");
  
  }
}
