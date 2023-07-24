#include <stdio.h>
#include <stdlib.h>
#include "candles.h"
#include "indicators.h"

#define TASK_MAX 1000
#define DATA_MAX 4

typedef struct {
  int enabled;
  int target_index;
  int is_outputs;
  int data_index;
} MapInfo;

typedef struct {
  int used;
  int indicator_index;
  int size;
  TI_REAL options[DATA_MAX];
  int inputs_offset;
  TI_REAL * inputs[DATA_MAX];
  int outputs_offset;
  TI_REAL * outputs[DATA_MAX];
  MapInfo inputs_map[DATA_MAX];
} Task;

int main() {
  printf("你好，世界\n");
  return 0;
}
