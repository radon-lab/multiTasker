uint8_t newTask(void (*task)(), uint32_t timer = 0);

void taskLaunch(void (*inLoop)()) {
  inLoop();
}

volatile uint32_t _millis_now = 0;
uint16_t* taskManager;
uint8_t taskNum = 0;
uint8_t task = 0;

boolean chekTimer(uint32_t timer, uint32_t* lastTime) {
  if ((*lastTime < millis() && *lastTime - timer < millis()) || *lastTime - timer > millis()) {
    *lastTime = millis() + timer;
  }
  else return 0;
  return 1;
}

void loop(void) {
  if (taskNum) {
    if (chekTimer(*(uint32_t*)(taskManager + task * 10 + 2), (uint32_t*)(taskManager + task * 10 + 6))) { //получаем время таймера процедуры
      taskLaunch((void(*)()) * (taskManager + task * 10)); //получаем адресс процедуры
    }
    if (++task >= taskNum) task = 0;
  }
}

uint8_t newTask(void (*task)(), uint32_t time) {
  if (taskNum < 255) {
    for (uint8_t i = 0; i < taskNum; i++) {
      if (*(taskManager + i * 10) == (uint16_t)task) return 1;
    }
    uint16_t* mem = taskNum ? (uint16_t*)realloc(taskManager, (taskNum + 1) * 10) : (uint16_t*)malloc(10);
    if (mem != NULL) {
      taskManager = mem;
      *(taskManager + taskNum * 10) = (uint16_t)task;
      *(uint32_t*)(taskManager + taskNum * 10 + 2) = time;
      *(uint32_t*)(taskManager + taskNum++ * 10 + 6) = millis() + time;
      return 0;
    }
  }
  return 2;
}

boolean killTask(void (*task)()) {
  if (taskNum) {
    for (uint8_t i = 0; i < taskNum; i++) {
      if (*(taskManager + i * 10) == (uint16_t)task) {
        for (uint8_t t = i; t < taskNum; t++) {
          *(taskManager + t * 10) = (*taskManager + (t + 1) * 10);
          *(uint32_t*)(taskManager + t * 10 + 2) = *(uint32_t*)(taskManager + (t + 1) * 10 + 2);
          *(uint32_t*)(taskManager + t * 10 + 6) = *(uint32_t*)(taskManager + (t + 1) * 10 + 6);
        }
        if (--taskNum) taskManager = (uint16_t*)realloc(taskManager, taskNum * 10);
        else free(taskManager);
        return 0;
      }
    }
  }
  return 1;
}

boolean killTask(uint8_t task) {
  if (taskNum > task) {
    for (uint8_t i = task; i < taskNum; i++) {
      *(taskManager + i * 10) = (*taskManager + (i + 1) * 10);
      *(uint32_t*)(taskManager + i * 10 + 2) = *(uint32_t*)(taskManager + (i + 1) * 10 + 2);
      *(uint32_t*)(taskManager + i * 10 + 6) = *(uint32_t*)(taskManager + (i + 1) * 10 + 6);
    }
    if (--taskNum) taskManager = (uint16_t*)realloc(taskManager, taskNum * 10);
    else free(taskManager);
    return 0;
  }
  return 1;
}

boolean killAllTasks(void) {
  if (taskNum) {
    taskNum = 0;
    free(taskManager);
    return 0;
  }
  return 1;
}

uint8_t getTask(void (*task)()) {
  if (taskNum) {
    for (uint8_t i = 0; i < taskNum; i++) {
      if (*(taskManager + i * 10) == (uint16_t)task) return i;
    }
  }
  return 255;
}

void nowTask(uint16_t task) {
  taskLaunch((void(*)())task);
}

boolean nowTask(uint8_t task) {
  if (taskNum > task) {
    taskLaunch((void(*)()) * (taskManager + task * 10));
    return 0;
  }
  return 1;
}

boolean timerTask(uint8_t task, uint32_t time) {
  if (taskNum > task) {
    *(uint32_t*)(taskManager + task * 10 + 2) = time;
    *(uint32_t*)(taskManager + task * 10 + 6) = millis() + time;
    return 0;
  }
  return 1;
}

boolean timerTask(void (*task)(), uint32_t time) {
  for (uint8_t i = 0; i < taskNum; i++) {
    if ((*taskManager + i * 10) == (uint16_t)task) {
      *(uint32_t*)(taskManager + i * 10 + 2) = time;
      *(uint32_t*)(taskManager + i * 10 + 6) = millis() + time;
      return 0;
    }
  }
  return 1;
}

boolean resetTask(uint8_t task) {
  if (taskNum > task) {
    *(uint32_t*)(taskManager + task * 10 + 6) = millis() + *(uint32_t*)(taskManager + task * 10 + 2);
    return 0;
  }
  return 1;
}

boolean resetTask(void (*task)()) {
  for (uint8_t i = 0; i < taskNum; i++) {
    if (*(taskManager + i * 10) == (uint16_t)task) {
      *(uint32_t*)(taskManager + i * 10 + 6) = millis() + *(uint32_t*)(taskManager + i * 10 + 2);
      return 0;
    }
  }
  return 1;
}

void reboot(void) {
  taskNum = 0;
  free(taskManager);
  asm volatile("jmp 0x0000"); //прыгаем в начало
}
