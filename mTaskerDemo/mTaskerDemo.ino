#include "multiTasker.h"
#include "util/delay.h"

multiTasker Tasker;

void task1(void) {
  Serial.println("task1");
  _delay_ms(1000);
}

void task2(void) {
  Serial.println("task2");
  _delay_ms(1000);
}

void task3(void) {
  Serial.println("task3");
  Serial.println(Tasker.getTask(&task1));
  _delay_ms(1000);
}

void mainSetup(void) {
  Serial.begin(9600);
  Tasker.newTask(&task1);
  Tasker.newTask(&task2);
  Tasker.newTask(&task3);
}
void mainLoop(void) {

}
