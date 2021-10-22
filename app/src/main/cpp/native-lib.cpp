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


//1，data;2，linesize；3，width; 4， height
void renderFrame(uint8_t *src_data, int src_lineSize, int width, int height) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    ANativeWindow_setBuffersGeometry(window, width,
                                     height,
                                     WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }
    //把buffer中的数据进行赋值（修改）
    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    int dst_lineSize = window_buffer.stride * 4;//ARGB
    //逐行拷贝
    for (int i = 0; i < window_buffer.height; ++i) {
        memcpy(dst_data + i * dst_lineSize, src_data + i * src_lineSize, dst_lineSize);
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, 0);
    javaCallHelper = new JavaCallHelper(javaVm, env, thiz);
    cfFmpeg = new CFFmpeg(javaCallHelper, const_cast<char *>(dataSource));
    cfFmpeg->setRenderCallback(renderFrame);
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


extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_stopNative(JNIEnv *env, jobject thiz) {
    if (cfFmpeg) {
        cfFmpeg->stop();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_releaseNative(JNIEnv *env, jobject thiz) {
    pthread_mutex_lock(&mutex);
    if (window) {
        //把老的释放
        ANativeWindow_release(window);
        window = 0;
    }
    pthread_mutex_unlock(&mutex);
    DELETE(cfFmpeg);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_csh_cplayer_CPlayer_getDurationNative(JNIEnv *env, jobject thiz) {
    if (cfFmpeg) {
        return cfFmpeg->getDuration();
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_seekToNative(JNIEnv *env, jobject thiz, jint playProgress) {
    if (cfFmpeg) {
        cfFmpeg->seekTo(playProgress);
    }
}