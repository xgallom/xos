//
// Created by xgallom on 5/22/20.
//

#ifndef XOS_PAUSE_H
#define XOS_PAUSE_H

inline void pause() { asm volatile("pause"); }

#endif //XOS_PAUSE_H
