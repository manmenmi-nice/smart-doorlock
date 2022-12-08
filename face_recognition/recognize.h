
#ifndef SMART_DOORLOCK_RECOGNIZE_H
#define SMART_DOORLOCK_RECOGNIZE_H

// 인식 성공 시 1, 인식 실패 시 0
typedef void (*RECOGNIZE_SET_ON_DONE_CALLBACK) (int);

int recognize_init();
int recognize_start();
int recognize_setOnDoneCallback(RECOGNIZE_SET_ON_DONE_CALLBACK);

#endif //SMART_DOORLOCK_RECOGNIZE_H
