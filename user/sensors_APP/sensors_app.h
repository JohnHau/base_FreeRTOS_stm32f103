#ifndef __SENSORS_APP_H__
#define __SENSORS_APP_H__


#include "stm32f10x.h"

extern DHT11_Data_TypeDef sensor_DHT11_Data;

extern void sensors_thread(void *para);
#endif /* __SENSORS_APP_H__*/