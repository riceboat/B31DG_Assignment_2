#define syncPin 18
#define dataPin 19
#define OUTPUT_SELECT_PIN 25
#define OUTPUT_ENABLE_PIN 26
#define DEBUG true
// WATT
// option 4
#define a 400
#define b  100
#define c 11
#define d 3500
#define sync_PRODUCTION 50


boolean enable_signal = true;

long debounce_time = 100000;
volatile unsigned long last_uS_enable;
volatile unsigned long last_uS_select;

float interval_multiplier = 1.0f;

void setup() {
  // initialize digital pin GPIO18 as an output.
  pinMode(syncPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(OUTPUT_SELECT_PIN, INPUT);
  attachInterrupt(OUTPUT_SELECT_PIN, ISR_SELECT, RISING);
  pinMode(OUTPUT_ENABLE_PIN, INPUT);
  attachInterrupt(OUTPUT_ENABLE_PIN, ISR_ENABLE, RISING);
}
void customDelayuS(int uS) {
  int multiplier = 1;
  if (DEBUG) {
    multiplier *= 100;
  }
  delayMicroseconds(uS * multiplier);
}
void ISR_SELECT() {
  if ((micros() - last_uS_select) >= debounce_time) { // debouncing pushbutton
    last_uS_select = micros();
    if (interval_multiplier == 1.0f) {
      interval_multiplier = 0.5f;
    }
    else {
      interval_multiplier = 1.0f;
    }
  }
}
void ISR_ENABLE() {
  if ((micros() - last_uS_enable) >= debounce_time) { // debouncing pushbutton
    last_uS_enable = micros();
    enable_signal = !enable_signal;
  }
}
void pinOnFor(int pin, int uS) {
  digitalWrite(pin, HIGH);
  customDelayuS(uS);
}
// the loop function runs over and over again forever
void loop() {
  pinOnFor(syncPin, sync_PRODUCTION);
  digitalWrite(syncPin, LOW);
  for (int i = 0; i < c + 1; i++) {
    if (enable_signal) {
      pinOnFor(dataPin, a * i);
    }
    digitalWrite(dataPin, LOW);
    customDelayuS(b * interval_multiplier);

  }
  customDelayuS(d * interval_multiplier);
}
