#include <stdio.h>
#include <stdlib.h>
#include "candles.h"
#include "indicators.h"

#define DATA_MAX 4
#define TASK_MAX 1000

typedef struct {
  int enabled;
  int target_index;
  int is_inputs;
  int data_index;
} InputMap;

typedef struct {
  int size;
  int start_task;
  int indic_index;
  TI_REAL options[DATA_MAX];
  int inputs_offset;
  TI_REAL * inputs[DATA_MAX];
  int outputs_offset;
  TI_REAL * outputs[DATA_MAX];
  InputMap inputs_map[DATA_MAX];
} Task;

int next = 0;
Task tasks[TASK_MAX];

void init(int task_index) {
  Task * task = &tasks[task_index];
  for (int i = 0; i < DATA_MAX; ++i) {
    task->inputs[i] = NULL;
    task->outputs[i] = NULL;
    task->inputs_map[i].enabled = 0;
  }
}

void erase(int task_index) {
  Task * task = &tasks[task_index];
  for (int i = 0; i < DATA_MAX; ++i) {
    if (task->inputs[i] != NULL) {
      free(task->inputs[i]);
      task->inputs[i] = NULL;
    }
    if (task->outputs[i] != NULL) {
      free(task->outputs[i]);
      task->outputs[i] = NULL;
    }
    task->inputs_map[i].enabled = 0;
  }
}

void push(int indic_index, int size, int start_task) {
  task_free(next);
  Task * task = &tasks[next];
  task->indic_index = indic_index;
  task->size = size;
  task->start_task = start_task;
  ti_indicator_info * indic = &ti_indicators[indic_index];
  if (!start_task) {
    for (int i = 0; i < indic->outputs; ++i)
      task->outputs[i] = malloc(sizeof(TI_REAL) * size);
  }
}

TI_REAL * inputs(int task_index, int data_index) {
  Task * task = &tasks[task_index];
  if (task->inputs[data_index] == NULL)
    task->inputs[data_index] = malloc(sizeof(TI_REAL) * task->size);
  return task->inputs[data_index];
}

TI_REAL * options(int task_index) {
  return tasks[task_index].options;
}

TI_REAL * outputs(int task_index, int data_index) {
  return tasks[task_index].outputs[data_index];
}

void inputs_map(
  int task_index,
  int input_index,
  int enabled,
  int target_index,
  int is_inputs,
  int data_index
) {
  InputMap * map = &tasks[task_index].inputs_map[input_index];
  map->enabled = enabled;
  map->target_index = target_index;
  map->is_inputs = is_inputs;
  map->data_index = data_index;
}

int inputs_offset(int task_index) {
  return tasks[task_index].inputs_offset;
}

int outputs_offset(int task_index) {
  return tasks[task_index].outputs_offset;
}

void link(int task_index) {
  Task * task = &tasks[task_index];
  ti_indicator_info * indic = &ti_indicators[task->indic_index];
  int inputs_offset = 0;
  for (int i = 0; i < indic->inputs; ++i) {
    InputMap * map = &task->inputs_map[i];
    if (map->enabled) {
      Task * target = &tasks[map->target_index];
      const int offset = map->is_inputs ? target->inputs_offset :
        target->outputs_offset;
      task->inputs[i] = map->is_inputs ? target->inputs[map->data_index] :
        target->outputs[map->data_index];
      if (offset > inputs_offset) inputs_offset = offset;
    }
  }
  task->inputs_offset = inputs_offset;
}

void run(int task_index) {
  Task * task = &tasks[task_index];
  ti_indicator_info * indic = &ti_indicators[task->indic_index];
  if (task->start_task) {
    task->outputs_offset = indic->start(task->options);
    return;
  }
  //
  const TI_REAL * inputs[DATA_MAX];
  TI_REAL * outputs[DATA_MAX];
  for (int i = 0; i < indic->inputs; ++i)
    inputs[i] = &task->inputs[i][task->inputs_offset];
  for (int i = 0; i < indic->outputs; ++i)
    outputs[i] = &task->outputs[i][task->outputs_offset];
  indic->indicator(
    task->size - task->inputs_offset,
    inputs,
    task->options,
    outputs
  );
}

int main() {
  printf("你好，世界\n");
  return 0;
}
