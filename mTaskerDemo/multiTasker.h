void mainSetup(void);
void mainLoop(void);

void taskLoop(void (*inLoop)()) {
  inLoop();
}
uint16_t* taskManager;
uint8_t taskNum = 0;

int main(void) {
  mainSetup();
  for (;;) {
    mainLoop();
    for (uint8_t task = 0; task < taskNum; task++) {
      taskLoop((void(*)())taskManager[task]);
    }
  }
  return 0;
}

class multiTasker
{
  public:
    uint8_t newTask(void (*task)());
    boolean killTask(void (*task)());
    boolean killTask(uint8_t task);
    boolean killAllTasks(void);
    uint8_t getTask(void (*task)());
    void nowTask(uint16_t task);
    boolean nowTask(uint8_t task);
    void reboot(void);

  private:

};

uint8_t multiTasker::newTask(void (*task)()) {
  if (taskNum < 254) {
    for (uint8_t i = 0; i < taskNum; i++) {
      if (taskManager[i] == (uint16_t)task) return 1;
    }
    uint16_t* mem = taskNum ? (uint16_t*)realloc(taskManager, (taskNum + 1) * sizeof(uint16_t)) : (uint16_t*)malloc(sizeof(uint16_t));
    if (mem != NULL) {
      taskManager = mem;
      taskManager[taskNum++] = (uint16_t)task;
      return 0;
    }
  }
  return 2;
}

boolean multiTasker::killTask(void (*task)()) {
  if (taskNum) {
    for (uint8_t i = 0; i < taskNum; i++) {
      if (taskManager[i] == (uint16_t)task) {
        for (uint8_t t = i; t < taskNum; t++) {
          taskManager[t] = taskManager[t + 1];
        }
        if (--taskNum) taskManager = (uint16_t*)realloc(taskManager, taskNum * sizeof(uint16_t));
        else free(taskManager);
        return 0;
      }
    }
  }
  return 1;
}

boolean multiTasker::killTask(uint8_t task) {
  if (taskNum > task) {
    for (uint8_t i = task; i < taskNum; i++) {
      taskManager[i] = taskManager[i + 1];
    }
    taskManager = (uint16_t*)realloc(taskManager, taskNum * sizeof(uint16_t));
    if (--taskNum) taskManager = (uint16_t*)realloc(taskManager, taskNum * sizeof(uint16_t));
    else free(taskManager);
    return 0;
  }
  return 1;
}

boolean multiTasker::killAllTasks(void) {
  if (taskNum) {
    taskNum = 0;
    free(taskManager);
    return 0;
  }
  return 1;
}

uint8_t multiTasker::getTask(void (*task)()) {
  if (taskNum) {
    for (uint8_t i = 0; i < taskNum; i++) {
      if (taskManager[i] == (uint16_t)task) return i;
    }
  }
  return 255;
}

void multiTasker::nowTask(uint16_t task) {
  taskLoop((void(*)())task);
}

boolean multiTasker::nowTask(uint8_t task) {
  if (taskNum > task) {
    taskLoop((void(*)())taskManager[task]);
    return 0;
  }
  return 1;
}

void multiTasker::reboot(void) {
  taskNum = 0;
  free(taskManager);
  asm volatile("jmp 0x0000"); //прыгаем в начало
}
