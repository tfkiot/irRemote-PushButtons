#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#include <EEPROM.h>

#define IR_PIN D4

#define BUTTON_PIN_1 5
#define BUTTON_PIN_2 4
#define BUTTON_PIN_3 14
#define BUTTON_PIN_4 12

#define LED_PIN_1 13
#define LED_PIN_2 15
#define LED_PIN_3 3
#define LED_PIN_4 1

bool led1State = false;
bool led2State = false;
bool led3State = false;
bool led4State = false;

IRrecv irrecv(IR_PIN);
decode_results results;

void setup() {
  EEPROM.begin(512);
  Serial.begin(115200);

  led1State = EEPROM.read(1);
  led2State = EEPROM.read(2);
  led3State = EEPROM.read(3);
  led4State = EEPROM.read(4);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);

  updateLED();

  irrecv.enableIRIn();
}


void loop() {
  readButton();
  irRemote();
  delay(100); // Adjust the delay as needed
}

//////
void updateLED() {
  digitalWrite(LED_PIN_1, led1State);
  digitalWrite(LED_PIN_2, led2State);
  digitalWrite(LED_PIN_3, led3State);
  digitalWrite(LED_PIN_4, led4State);
  updateEEPROM();
}

void updateEEPROM(){
  EEPROM.put(1, led1State);
  EEPROM.put(2, led2State);
  EEPROM.put(3, led3State);
  EEPROM.put(4, led4State);
  EEPROM.commit(); 
}

/////////
void readButton() {
  if (digitalRead(BUTTON_PIN_1) == LOW) {
    led1State = !led1State;
    updateLED();
    delay(50);
    while (digitalRead(BUTTON_PIN_1) == LOW);
    Serial.println(led1State);
  }

  if (digitalRead(BUTTON_PIN_2) == LOW) {
    led2State = !led2State;
    updateLED();
    delay(50);
    while (digitalRead(BUTTON_PIN_2) == LOW)
      ;
    Serial.println(led2State);
  }

  if (digitalRead(BUTTON_PIN_3) == LOW) {
    led3State = !led3State;
    updateLED();
    delay(50);
    while (digitalRead(BUTTON_PIN_3) == LOW)
      ;
    Serial.println(led3State);
  }

  if (digitalRead(BUTTON_PIN_4) == LOW) {
    led4State = !led4State;
    updateLED();
    delay(50);
    while (digitalRead(BUTTON_PIN_4) == LOW)
      ;
   // Serial.println(led4State);
  }
}

//////
void irRemote() {
  if (irrecv.decode(&results)) {
    unsigned long int decCode = results.value;
    Serial.println(decCode, HEX);

    if (decCode == 0x1FE50AF) {
      led1State = !led1State;
      Serial.println(led1State);
      updateLED();
    }

    if (decCode == 0x1FED827) {
      led2State = !led2State;
      Serial.println(led2State);
      updateLED();
    }

    if (decCode == 0x1FEF807) {
      led3State = !led3State;
      Serial.println(led3State);
      updateLED();
    }

    if (decCode == 0x1FE30CF) {
      led4State = !led4State;
      Serial.println(led4State);
      updateLED();
    }

    irrecv.resume();
  }
}