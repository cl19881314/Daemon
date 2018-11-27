//
// Created by Administrator on 2018/11/26.
//
#ifndef NDKTEST_NATIVE_LIB_H
#define NDKTEST_NATIVE_LIB_H
#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"tag",__VA_ARGS__)
#define LOGE2(...) __android_log_print(ANDROID_LOG_ERROR, "JNITag","string From Java To C : %s", str);
void child_do_work();

int child_create_channel();

void child_listen_msg();

#endif //NDKTEST_NATIVE_LIB_H
