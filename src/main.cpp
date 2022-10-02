#include <Arduino.h>

/**
 * 
 */
#define FIRE_BUTTON_PIN 4

#define FIRE_COOLOFF_MILLIS 500

/**
 * If present, this will default to HIGH when off, and LOW when on.
 */
#define FIRE_LED_PIN 5

#define FIRE_LED_DURATION_MILLIS 100

long lastFireTime = 0L;


void setup() {
  Serial.begin(9600);
  Serial.println("Setting up arduino-laser-tag with the following pin configurations:");
  Serial.print(printf(" FIRE_BUTTON_PIN: %d", FIRE_BUTTON_PIN));

  #ifdef FIRE_LED_PIN
  Serial.print(printf(" FIRE_LED_PIN: %d", FIRE_LED_PIN));
  #endif

  pinMode(FIRE_BUTTON_PIN, INPUT_PULLUP);

  #ifdef FIRE_LED_PIN
  pinMode(FIRE_LED_PIN, OUTPUT);
  digitalWrite(FIRE_LED_PIN, HIGH);
  #endif
}

void loop() {
  if (
    digitalRead(FIRE_BUTTON_PIN) == LOW
    && millis() - lastFireTime > FIRE_COOLOFF_MILLIS
  ) {

    Serial.println("Fire button pressed");
    lastFireTime = millis();

    #ifdef FIRE_LED_PIN
    digitalWrite(FIRE_LED_PIN, LOW);
    #endif

  }

  delay(20);

  #ifdef FIRE_LED_PIN
  if (millis() - lastFireTime > FIRE_LED_DURATION_MILLIS) {
    digitalWrite(FIRE_LED_PIN, HIGH);
  }
  #endif
}