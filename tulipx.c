#include <stdio.h>
#include <stdlib.h>
#include "candles.h"
#include "indicators.h"

#define DATA_MAX 4
#define TASK_MAX 1000

typedef enum {
  INPUT_NONE, INPUT_MAP, INPUT_DATA
} InputType;

typedef struct {
  InputType type;
  int target_index;
  int is_inputs;
  int data_index;
} InputInfo;

typedef struct {
  int index;
  int size;
  TI_REAL options[DATA_MAX];
  int inputs_offset;
  TI_REAL * inputs[DATA_MAX];
  int outputs_offset;
  TI_REAL * outputs[DATA_MAX];
  InputInfo inputs_info[DATA_MAX];
} Task;

int next = 0;
Task tasks[TASK_MAX];

void task_free(int index) {
  Task * task = &tasks[index];
  for (int i = 0; i < DATA_MAX; ++i) {
    if (task->inputs[i] != NULL) {
      free(task->inputs[i]);
      task->inputs[i] = NULL;
    }
    if (task->outputs[i] != NULL) {
      free(task->outputs[i]);
      task->outputs[i] = NULL;
    }
  }
}

int main() {
  printf("你好，世界\n");
  return 0;
}
