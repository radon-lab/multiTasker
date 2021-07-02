#include "multiTasker.h"

void setup(void) {
  Serial.begin(9600);
  newTask(mainLoop);
  newTask(task1, 1000);
  newTask(task2, 5000);
  newTask(task3, 9000);
}

void mainLoop(void) {
  Serial.println("loop");
}

void task1(void) {
  Serial.println("task1");
}

void task2(void) {
  Serial.println("task2");
}

void task3(void) {
  Serial.println("task3");
}
