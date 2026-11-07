#ifndef TEMPERATURE_H
#define TEMPERATURE_H

void raw_calcuation(float *hum, float *temp);
void average_calculation(float hum,float temp,float *hum_average,float *temp_average);
void print_data(float hum_average,float temp_average);

# endif