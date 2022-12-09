//
// Created by dfkdream on 22. 12. 9.
//

#ifndef SMART_DOORLOCK_BLUETOOTH_H
#define SMART_DOORLOCK_BLUETOOTH_H

#include <pthread.h>

typedef void (* BLUETOOTH_ONMESSAGE_CB) (const char*);

pthread_t bluetooth_init();
void bluetooth_setOnMessageCallback(BLUETOOTH_ONMESSAGE_CB);

#endif //SMART_DOORLOCK_BLUETOOTH_H
