#include "Ticker.h"
#include <B31DGMonitor.h>

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
unsigned int frame = 0;
unsigned int tFrame = 0;
B31DGCyclicExecutiveMonitor monitor;
Ticker majorCycleTicker;
void setup(void)
{
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(freqPin1, INPUT);
  pinMode(freqPin2, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(sigPin1, OUTPUT);
  pinMode(sigPin2, OUTPUT);
  majorCycleTicker.attach_us(1000, majorCycle);
  monitor.startMonitoring();
}

void performJobTask1(void) { //every 4ms
  monitor.jobStarted(1);
  digitalWrite(sigPin1, HIGH);
  delayMicroseconds(250); // Spin wait for 250us
  digitalWrite(sigPin1, LOW);;
  delayMicroseconds(50); // Spin wait for 50us
  digitalWrite(sigPin1, HIGH);
  delayMicroseconds(300); // Spin wait for 300us
  digitalWrite(sigPin1, LOW);
  monitor.jobEnded(1);
}

void performJobTask2(void) { //every 3ms
  monitor.jobStarted(2);
  digitalWrite(sigPin2, HIGH);
  delayMicroseconds(100); // Spin wait for 250us
  digitalWrite(sigPin2, LOW);
  delayMicroseconds(50); // Spin wait for 50us
  digitalWrite(sigPin2, HIGH);
  delayMicroseconds(200); // Spin wait for 300us
  digitalWrite(sigPin2, LOW);
  monitor.jobEnded(2);
}

void performJobTask3(void) { //every 10ms
  monitor.jobStarted(3);
  float measuringTime = 375.0;
  int i = micros() + measuringTime;
  bool last = digitalRead(freqPin1);
  float flips = 0;
  while (micros() < i){ 
    bool curr = digitalRead(freqPin1);
    if (curr != last){
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
}

void performJobTask4(void) { //every 10ms
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
}

void performJobTask5(void) { //every 5ms
   if (doWork){
    monitor.jobStarted(5);
    monitor.doWork(); //500us
    monitor.jobEnded(5);
  }
}

void doTask(int i){
  switch(i){
    case 1: performJobTask1(); break;
    case 2: performJobTask2(); break;
    case 3: performJobTask3(); break;
    case 4: performJobTask4(); break;
    case 5: performJobTask5(); break;
  }
}

void doTasks(int a=0,int b=0,int c=0){
  doTask(a);
  doTask(b);
  doTask(c);
}

void fillEmptyTime(int i){
}
void execFrame(unsigned int i) {
  int t = micros();
  unsigned int c = i % 60;
  switch (c) {
    case 0: doTasks(2); break;
    case 1: doTasks(1); break;
    case 2: doTasks(5); break;
    case 3: doTasks(2); break;
    case 4: doTasks(3); break;
    case 5: doTasks(1); break;
    case 6: doTasks(2); break;
    case 7: doTasks(5); break;
    case 8: doTasks(4); break;
    case 9: doTasks(1,2); break;
    case 10: break;
    case 11: break;
    case 12: doTasks(2,5); break;
    case 13: doTasks(1); break;
    case 14: doTasks(3); break;
    case 15: doTasks(2); break;
    case 16: break;
    case 17: doTasks(1, 5); break;
    case 18: doTasks(2,4); break;
    case 19: break;
    case 20: break;
    case 21: doTasks(1,2); break;
    case 22: doTasks(5); break;
    case 23: break;
    case 24: doTasks(2,3); break;
    case 25: doTasks(1); break;
    case 26: break;
    case 27: doTasks(2, 5); break;
    case 28: doTasks(4); break;;
    case 29: doTasks(1); break;
    case 30: doTasks(2); break;
    case 31: break;
    case 32: doTasks(5);break;
    case 33: doTasks(1,2); break;
    case 34: doTasks(3); break;
    case 35: break;
    case 36: doTasks(2); break;
    case 37: doTasks(1,5); break;
    case 38: doTasks(4); break;;
    case 39: doTasks(2); break;
    case 40: break;
    case 41: doTasks(1); break;
    case 42: doTasks(2,5); break;
    case 43: break;
    case 44: doTasks(3); break;
    case 45: doTasks(1,2); break;
    case 46: break;
    case 47: doTasks(5);break;
    case 48: doTasks(2,4); break;
    case 49: doTasks(1); break;
    case 50: break;
    case 51: doTasks(2); break;
    case 52: doTasks(5);break;
    case 53: doTasks(1); break;
    case 54: doTasks(2,3); break;
    case 55: break;
    case 56: break;
    case 57: doTasks(1,2,5); break;
    case 58: doTasks(4); break;
    case 59: break;
  }
  fillEmptyTime(1000 - (micros() - t));
  if (micros() - t > 10000){
    Serial.println(c);
    Serial.println(micros()-t);
  }

  digitalWrite(ledPin1,(avgFreq1 + avgFreq2 > 1500));

   bool lastButtonState = currentButtonState;      // save the last state
  currentButtonState = digitalRead(buttonPin); // read new state
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    doWork = !doWork;
  }
  digitalWrite(ledPin2, doWork);
}
void majorCycle(void)
{
  execFrame(frame);
  frame+=1;
}
void loop(void){ 
}
