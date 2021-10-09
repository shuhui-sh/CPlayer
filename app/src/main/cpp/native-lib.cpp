#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <zconf.h>
#include "JavaCallHelper.h"
#include "CFFmpeg.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}


JavaCallHelper *javaCallHelper = 0;
JavaVM *javaVm = 0;
CFFmpeg *cfFmpeg = 0;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVm = vm;
    return JNI_VERSION_1_4;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, 0);
    javaCallHelper = new JavaCallHelper(javaVm, env, thiz);
    cfFmpeg = new CFFmpeg(javaCallHelper, const_cast<char *>(dataSource));
    cfFmpeg->prepare();

    env->ReleaseStringUTFChars(data_source, dataSource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_startNative(JNIEnv *env, jobject thiz) {
    if (cfFmpeg) {
        cfFmpeg->start();
    }
}