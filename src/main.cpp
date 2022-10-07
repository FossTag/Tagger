#include <Arduino.h>

#define DECODE_SONY

#include <IRremote.hpp>
#include <Sound.h>

/**
 * Required.
 * Used to differentiate each device, so that we can ignore IR data from our own transmitter.
 * TODO: Make this externalised via config, or more practically via a special controller that transmits commands to each device.
 */
#define MY_ADDRESS 1

/**
 * Optional (but highlighly recommended or else your device cannot fire).
 * Connected to a momentary switch, where one side is grounded and the other side is this pin.
 * Will be connected to the device via an internal pullup resistor, so no additional resistors are required.
 */
#define FIRE_BUTTON_PIN 4

/**
 * Optional (but highly recommended or else your device cannot fire).
 * Emits IR signals at 38Khz using the Sony protocol each time FIRE_BUTTON_PIN is triggered.
 */
#define FIRE_IR_SEND_PIN 8

/**
 * Required if FIRE_BUTTON_PIN is set, otherwise not required.
 * Minimum time allowed between triggering fires.
 * If the fire button is held down, then it will send signals every this often.
 */
#define FIRE_COOLOFF_MILLIS 500

/**
 * Optional (but highly recommended or else your device cannot be hit and will be indistructable).
 * Receives IR signals at 38Khz using the Sony protocol and interprets them as hit or configuration messages.
 * Can wire multiple IR Receivers in parallel all to this pin.
 * Recommended to include a capacitor in the circuit (e.g. 10uF) as per https://web.archive.org/web/20161210114703/http://www.lasertagparts.com/mtsensors.htm.
 */
#define HIT_IR_RECEIVE_PIN 9

/**
 * Optional.
 * Flashes for FIRE_STATUS_LED_DURATION_MILLIS each time FIRE_BUTTON_PIN is triggered.
 * Defauts to HIGH when off, and set to LOW to turn LED on.
 */
#define FIRE_STATUS_LED_PIN 5
#define FIRE_STATUS_LED_DURATION_MILLIS 100

/**
 * Optional.
 * Flashes for HIT_STATUS_LED_DURATION_MILLIS each time the device registers a hit on HIT_IR_RECEIVE_PIN.
 * Defauts to HIGH when off, and set to LOW to turn LED on.
 */
#define HIT_STATUS_LED_PIN 6
#define HIT_STATUS_LED_DURATION_MILLIS 200

// #define SPEAKER_PIN 7

/**
 * Used for both debouncing fires, limiting the rate of fire, and also used to only
 * show feedback of firing for a short time.
 */
long lastFireTime = 0L;

/**
 * You can get hit lots of times, but this will debounce briefly to ignore the effects
 * of reflected IR, as well as multiple IR receivers wired together from catching the
 * same burst of IR multiple times.
 * It is also used to only show feedback of being hit for a short time.
 */
long lastHitTime = 0L;

int health = 100;

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up arduino-laser-tag with the following pin configurations:");

  #ifdef FIRE_BUTTON_PIN
  Serial.print(printf(" FIRE_BUTTON_PIN: %d", FIRE_BUTTON_PIN));
  #endif

  #ifdef FIRE_STATUS_LED_PIN
  Serial.print(printf(" FIRE_LED_PIN: %d", FIRE_STATUS_LED_PIN));
  #endif

  #ifdef FIRE_BUTTON_PIN
  pinMode(FIRE_BUTTON_PIN, INPUT_PULLUP);
  #endif

  #ifdef FIRE_STATUS_LED_PIN
  pinMode(FIRE_STATUS_LED_PIN, OUTPUT);
  digitalWrite(FIRE_STATUS_LED_PIN, HIGH);
  #endif

  #ifdef HIT_STATUS_LED_PIN
  pinMode(HIT_STATUS_LED_PIN, OUTPUT);
  digitalWrite(HIT_STATUS_LED_PIN, HIGH);
  #endif

  #ifdef HIT_IR_RECEIVE_PIN
  IrReceiver.begin(HIT_IR_RECEIVE_PIN, true);
  #endif

  #ifdef FIRE_IR_SEND_PIN
  IrSender.begin(FIRE_IR_SEND_PIN, LED_BUILTIN);
  #endif
}

#ifdef HIT_IR_RECEIVE_PIN
bool wasHit() {
  bool hit = false;
  if (IrReceiver.decode()) {
    Serial.print(F("Decoded protocol: "));
    Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
    Serial.print(F(", Decoded raw data: "));
    Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.print(F(", decoded address: "));
    Serial.print(IrReceiver.decodedIRData.address, HEX);
    Serial.print(F(", decoded command: "));
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    if (IrReceiver.decodedIRData.decodedRawData == 0) {
      Serial.println("Ignoring hit becaues it doesn't seem to be a full message received.");
    } else if (IrReceiver.decodedIRData.address == MY_ADDRESS) {
      Serial.println("Ignoring hit because it was from my IR LED.");
    } else {
      hit = true;
    }
    IrReceiver.resume();
  }

  return hit;
}
#endif

#ifdef SPEAKER_PIN
RandomDescendingNoteSound hitSound = RandomDescendingNoteSound(
  30, 60,
  10,
  150, 50, 50
);

RandomDescendingNoteSound fireSound = RandomDescendingNoteSound(
  30, 60,
  5,
  300, 240, 50
);

RandomDescendingNoteSound dieSound = RandomDescendingNoteSound(
  50, 100,
  30,
  360, 0, 120
);
#endif

void loop() {

  #ifdef FIRE_BUTTON_PIN
  if (
    digitalRead(FIRE_BUTTON_PIN) == LOW
    && millis() - lastFireTime > FIRE_COOLOFF_MILLIS
  ) {

    Serial.println("Fire button pressed");
    lastFireTime = millis();

    #ifdef FIRE_IR_SEND_PIN
    IrSender.sendSony(MY_ADDRESS, 0x1, 0);
    #endif

    #ifdef FIRE_STATUS_LED_PIN
    digitalWrite(FIRE_STATUS_LED_PIN, LOW);
    #endif

    #ifdef SPEAKER_PIN
    Sound::play(&fireSound);
    #endif

  }
  #endif

  #ifdef HIT_IR_RECEIVE_PIN
  if (wasHit()) {
    Serial.println("Hit!");
    lastHitTime = millis();

    health -= 30;

    #ifdef HIT_STATUS_LED_PIN
    digitalWrite(HIT_STATUS_LED_PIN, LOW);
    #endif

    #ifdef SPEAKER_PIN
    if (health < 0) {
      Sound::play(&dieSound);
    } else {
      Sound::play(&hitSound);
    }
    #endif
  }
  #endif

  #ifdef FIRE_STATUS_LED_PIN
  if (millis() - lastFireTime > FIRE_STATUS_LED_DURATION_MILLIS) {
    digitalWrite(FIRE_STATUS_LED_PIN, HIGH);
  }
  #endif

  #ifdef HIT_STATUS_LED_PIN
  if (millis() - lastHitTime > HIT_STATUS_LED_DURATION_MILLIS) {
    digitalWrite(HIT_STATUS_LED_PIN, HIGH);
  }
  #endif

  #ifdef SPEAKER_PIN
  Sound::process(SPEAKER_PIN);
  #endif
}