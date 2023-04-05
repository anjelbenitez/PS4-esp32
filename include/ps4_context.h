//
// Created by abenitez on 3/28/23.
//
#ifndef ESP_BT_CONTROLLER_RC_PS4_CONTEXT_H
#define ESP_BT_CONTROLLER_RC_PS4_CONTEXT_H

#include "ps4.h"


typedef struct {
  ps4_t data;
  ps4_event_t event;
  ps4_cmd_t output;
  callback_t _callback_event;
  callback_t _callback_connect;
  callback_t _callback_disconnect;
} ps4_controller_t;

typedef struct proj_context_s {
  ps4_controller_t * ps4;
} proj_context_t;

extern proj_context_t ctx;

#endif // ESP_BT_CONTROLLER_RC_PS4_CONTEXT_H
