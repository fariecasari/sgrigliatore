//FarieCasari
//Progetto di un Sgrigliatore a catena per la pulizia di una piccola turbina da 5KW situata in una roggia con 1M di salto
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>
#include <EEPROM.h>

#define IN_SWITCH_ENCODER 10
#define IN_FINECORSA 5

#define IN_START_MANUALE 7
#define OUT_GUASTO 6

int lastFinecorsa = HIGH;
int lastSwitch = HIGH;
boolean didReset = false;
// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

float h; //VARIABILE UTILIZZATA PER LE ORE
float m; //VARIABILE UTILIZZATA PER I MINUTI
float s; //VARIABILE UTILIZZATA PER I SECONDI
float dp; //PARTE DECIMALE DELLE OPERAZIONI
int conta = 0;
int addr = 0;
int newPos = 10;
int OUT_SGRIGLIATORE =  9;
int STATO_SGRIGLIATORE = LOW;
unsigned long previousMillis = 0;        // will store last time LED was updated
long OnTime = 180000; //250;           // milliseconds of on-time
// long OffTime = (15*(float) 60 *1000 );
//long OffTime = 2;
// long OffTime = 600000; //1800000000; //750;          // milliseconds of off-time
unsigned long currentMillis ;
unsigned long Secondi ;
void setup()
{
  pinMode(IN_FINECORSA, INPUT);
  pinMode(IN_START_MANUALE, INPUT);
  pinMode(IN_SWITCH_ENCODER, INPUT);
  pinMode(OUT_GUASTO, OUTPUT);
  pinMode(OUT_SGRIGLIATORE, OUTPUT);
  digitalWrite(IN_SWITCH_ENCODER, HIGH); // set internal pull-up resistor
  digitalWrite(IN_START_MANUALE, HIGH); // set internal pull-up resistor
  digitalWrite(IN_FINECORSA, HIGH); // set internal pull-up resistor
  lcd.init();                      // initialize the lcd
  lcd.backlight();

  // ------- Quick 3 blinks of backlight  -------------
  for (int i = 0; i < 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();
  lcd.setCursor(0, 0); //Start at character 4 on line 0
  lcd.print("FarieCasari");
  delay(1000);
}

void start()
{
  STATO_SGRIGLIATORE = HIGH;  // turn it on
  previousMillis = currentMillis;   // Remember the time
  digitalWrite(OUT_SGRIGLIATORE, STATO_SGRIGLIATORE);
}
void guasto()
{
  digitalWrite(OUT_GUASTO, HIGH);
  lcd.setCursor(1, 1);
  lcd.print("GUASTO  ");
  for (int i = 0; i < 30; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();
  int riprendi = 0;
  while (riprendi == 0) {
    int currentSwitch = digitalRead(IN_SWITCH_ENCODER);
    if (currentSwitch == LOW && lastSwitch != currentSwitch ) {
      //  if (digitalRead(IN_SWITCH_ENCODER) == HIGH) {
      digitalWrite(OUT_GUASTO, LOW);
      riprendi = 1;
      lcd.setCursor(1, 1);
      lcd.print("        ");
    }
  }
}

void loop()
{
  static int pos = 0;
  encoder.tick();
  //    lcd.setCursor(0, 0);
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    lcd.setCursor(10, 1);
    lcd.print(newPos);
    lcd.print(" ");
    pos = newPos;
  } // if

  int val = EEPROM.read(0);
  lcd.setCursor(13, 1);
  lcd.print(val);
  long OffTime = (val * (float) 60 * 1000 );
  //    lcd.print(OffTime);
  int currentSwitch = digitalRead(IN_SWITCH_ENCODER);
  if (currentSwitch == LOW && lastSwitch != currentSwitch ) {
    EEPROM.write(addr, newPos);
    lcd.setCursor(10, 1);
    lcd.print("   ");
  }

  // stampa il numero di secondi dall'ultimo reset
  // check to see if it's time to change the state of the LED
  currentMillis = millis();
  Secondi = (currentMillis / 1000);
  //   unsigned long currentMillis = millis();
  //   unsigned long Secondi = (currentMillis / 1000);
  lcd.setCursor(0, 1);
  // lcd.print( Secondi);

  int currentFinecorsa = digitalRead(IN_FINECORSA);
  if (( currentFinecorsa != lastFinecorsa) && (STATO_SGRIGLIATORE == HIGH))
  { if ( currentFinecorsa == HIGH)
    { conta = conta + 1;
      lcd.setCursor(13, 0);
      lcd.print(conta);

      delay(250);
    }
  }// DebounceTime
  lastFinecorsa = currentFinecorsa;

  if ((STATO_SGRIGLIATORE == HIGH) && (conta >= 2))
  {
    STATO_SGRIGLIATORE = LOW;  // Turn it off
    previousMillis = currentMillis;  // Remember the time
    digitalWrite(OUT_SGRIGLIATORE, STATO_SGRIGLIATORE);  // Update the actual LED
    conta = 0;
    lcd.setCursor(13, 0);
    lcd.print("  ");
    lcd.setCursor(1, 1);
    lcd.print("        ");
  }

  if ((STATO_SGRIGLIATORE == HIGH) && (currentMillis - previousMillis >= OnTime))
  {
    STATO_SGRIGLIATORE = LOW;  // Turn it off
    previousMillis = currentMillis;  // Remember the time
    digitalWrite(OUT_SGRIGLIATORE, STATO_SGRIGLIATORE);  // Update the actual LED
    guasto();
  }
  else if ((STATO_SGRIGLIATORE == LOW) && (currentMillis - previousMillis >= OffTime))
  {
    start();
  }
  int StartManuale = digitalRead(IN_START_MANUALE);

  if ((STATO_SGRIGLIATORE == LOW) && (StartManuale == HIGH))
  {
    lcd.setCursor(1, 1);
    lcd.print("StartMan");

    start();
    //   delay(20);
  }
}



