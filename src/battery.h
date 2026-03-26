#include <stdio.h>

#define BATTERY_CAPACITY_LOCATION "/sys/class/power_supply/BAT0/capacity"

int getBatteryCapacity() {

  FILE* fp;

  fp = fopen(BATTERY_CAPACITY_LOCATION, "r");

  if (fp == NULL) {
    return -1;
  }
  int capacity;

  fscanf(fp, "%d", &capacity);
  return capacity;
  
}
