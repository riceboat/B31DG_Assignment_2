#include "Ticker.h"
#include <B31DGMonitor.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const int ledPin1 = 25;
const int ledPin2 = 26;
const int sigPin1 = 12;
const int sigPin2 = 14;
const int buttonPin = 27;
const int freqPin1 = 18;
const int freqPin2 = 19;

float avgFreq1 = 500;
float avgFreq2 = 500;
int c1 = 0;
int c2 = 0;
bool doWork = true;
bool currentButtonState = false;

B31DGCyclicExecutiveMonitor monitor;

void setup(void)
{
  monitor.startMonitoring();
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(freqPin1, INPUT);
  pinMode(freqPin2, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(sigPin1, OUTPUT);
  pinMode(sigPin2, OUTPUT);
  digitalWrite(ledPin2, HIGH);
  xTaskCreate(performJobTask1, "performJobTask1", 2400, NULL, 3, NULL);
  xTaskCreate(performJobTask2, "performJobTask2", 2400, NULL, 3, NULL);
  xTaskCreate(performJobTask3, "performJobTask3", 2500, NULL, 4, NULL);
  xTaskCreate(performJobTask4, "performJobTask4", 2500, NULL, 4, NULL);
  xTaskCreate(performJobTask5, "performJobTask5", 2500, NULL, 5, NULL);
  xTaskCreate(buttonTask, "ButtonTask", 1400, NULL, 3, NULL);
  xTaskCreate(ledTask, "LedTask", 1400, NULL, 3, NULL);
}

void performJobTask1(void *pvParameters) { // every 4ms
  for (;;) {
    monitor.jobStarted(1);
    unsigned long startTime = micros();
    digitalWrite(sigPin1, HIGH);
    while (micros() - startTime < 250);
    startTime = micros();
    digitalWrite(sigPin1, LOW);
    startTime = micros();
    while (micros() - startTime < 50);
    startTime = micros();
    digitalWrite(sigPin1, HIGH);
    startTime = micros();
    while (micros() - startTime < 300);
    digitalWrite(sigPin1, LOW);
    monitor.jobEnded(1);

    vTaskDelay(pdMS_TO_TICKS(4)); // Delay 4ms
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("1: ");
//    Serial.println(maxStackSize);
  }
}

void performJobTask2(void *pvParameters) { // every 3ms
  for (;;) {
    monitor.jobStarted(2);
    unsigned long startTime = micros();
    digitalWrite(sigPin2, HIGH);
    while (micros() - startTime < 100);
    digitalWrite(sigPin2, LOW);
    startTime = micros();
    while (micros() - startTime < 50);
    digitalWrite(sigPin2, HIGH);
    startTime = micros();
    while (micros() - startTime < 200);
    digitalWrite(sigPin2, LOW);
    monitor.jobEnded(2);
    vTaskDelay(pdMS_TO_TICKS(3)); // Delay 3ms
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("2: ");
//    Serial.println(maxStackSize);    
  }
}

void performJobTask3(void *pvParameters) { // every 10ms
  for (;;) {
    monitor.jobStarted(3);
    float measuringTime = 375.0;
    unsigned long i = micros() + measuringTime;
    bool last = digitalRead(freqPin1);
    float flips = 0;
    while (micros() < i) {
      bool curr = digitalRead(freqPin1);
      if (curr != last) {
        last = curr;
        flips = flips + 1.0;
      }
    }
    float period = measuringTime / 1000000;
    float lastFreq = (flips / period) * 0.5;
    c1++;
    avgFreq1 -= avgFreq1 / c1;
    avgFreq1 += lastFreq / c1;
    monitor.jobEnded(3);
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay 10ms
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("3: ");
//    Serial.println(maxStackSize);    
  }
}

void performJobTask4(void *pvParameters) { // every 10ms
  for (;;) {
    monitor.jobStarted(4);
    float measuringTime = 300.0;
    int i = micros() + measuringTime;
    bool last = digitalRead(freqPin2);
    float flips = 0;
    while (micros() < i){ 
      bool curr = digitalRead(freqPin2);
      if (curr != last){
        last = curr;
        flips = flips + 1.0;
      }  
    }
    float period = measuringTime / 1000000;
    float lastFreq = (flips / period) * 0.5;
    c2++;
    avgFreq2 -= avgFreq2 / c2;
    avgFreq2 += lastFreq / c2;
    monitor.jobEnded(4);
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay 10ms
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("4: ");
//    Serial.println(maxStackSize);
  }
}

void performJobTask5(void *pvParameters) { // every 5ms
  for (;;) {
    if (doWork) {
      monitor.jobStarted(5);
      monitor.doWork();
      monitor.jobEnded(5);
    }
    vTaskDelay(pdMS_TO_TICKS(5)); // Delay 5ms
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("5: ");
//    Serial.println(maxStackSize);
  }
}

void buttonTask(void *pvParameters) {
  while (1) {
    bool lastButtonState = currentButtonState;
    currentButtonState = digitalRead(buttonPin);
    if (lastButtonState == HIGH && currentButtonState == LOW) {
      doWork = !doWork;
      digitalWrite(ledPin2, doWork);
    }
    vTaskDelay(pdMS_TO_TICKS(50)); // Check button every 50ms
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("button: ");
//    Serial.println(maxStackSize);
  }
}

void ledTask(void *pvParameters) {
  while(1){
    digitalWrite(ledPin1, (avgFreq1 + avgFreq2 > 1500));
    vTaskDelay(pdMS_TO_TICKS(50));
//    UBaseType_t maxStackSize = uxTaskGetStackHighWaterMark(NULL);
//    Serial.println("led: ");
//    Serial.println(maxStackSize);
  }
}

void loop(void){ 
}
