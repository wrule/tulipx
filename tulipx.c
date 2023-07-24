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
  int used;
  int index;
  int start_task;
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

void free_task(int index) {
  Task * task = &tasks[index];
  if (!task->used) return;
  if (!task->start_task) {
    ti_indicator_info * indic = &ti_indicators[task->index];
    for (int i = 0; i < indic->inputs; ++i)
      if (!task->inputs_info[i].type == INPUT_DATA) free(task->inputs[i]);
    for (int i = 0; i < indic->outputs; ++i)
      free(task->outputs[i]);
  }
  task->used = 0;
}

int main() {
  printf("你好，世界\n");
  return 0;
}
