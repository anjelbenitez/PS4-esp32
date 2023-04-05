
#ifndef PS4CONTROLLER_H
#define PS4CONTROLLER_H

#include <stdbool.h>
#include <stdlib.h>

typedef void (*callback_t)();

bool begin(const char* mac);
void end(void);
bool isConnected(void);
void setLed(uint8_t r, uint8_t g, uint8_t b);
void setRumble(uint8_t small, uint8_t large);
void setFlashRate(uint8_t onTime, uint8_t offTime);
void sendToController();
void attach(callback_t callback);
void attachOnConnect(callback_t callback);
void attachOnDisconnect(callback_t callback);
#endif
