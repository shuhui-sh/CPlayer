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
ANativeWindow *window = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//静态初始化mutex

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

extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_setSurfaceNative(JNIEnv *env, jobject thiz, jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window) {
        ANativeWindow_release(window);
        window = 0;
    }

    window = ANativeWindow_fromSurface(env, surface);
    pthread_mutex_unlock(&mutex);
}
