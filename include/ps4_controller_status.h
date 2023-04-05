//
// Created by abenitez on 3/28/23.
//
#include <stdbool.h>
#include "ps4_context.h"

inline uint8_t Battery() { return ctx.ps4->data.status.battery; }
inline bool Charging() { return ctx.ps4->data.status.charging; }
inline bool Audio() { return ctx.ps4->data.status.audio; }
inline bool Mic() { return ctx.ps4->data.status.mic; }