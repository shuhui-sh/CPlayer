//
// Created by g10295 on 2021/8/16.
//

#ifndef CPLAYER_JAVACALLHELPER_H
#define CPLAYER_JAVACALLHELPER_H

#include <jni.h>
#include "macro.h"

class JavaCallHelper {

public:
    JavaCallHelper(JavaVM *javaVm_, JNIEnv *env_, jobject instance_);

    ~JavaCallHelper();

    void onPrepared(int threadMode);

    void onError(int threadMode, int errorCode);

    void onProgress(int threadMode, int errorCode);

private:
    JavaVM *javaVm;
    JNIEnv *env;
    jobject instance;
    jmethodID jmd_prepared;
    jmethodID jmd_onError;
    jmethodID jmd_onProgress;

};

#endif //CPLAYER_JAVACALLHELPER_H
