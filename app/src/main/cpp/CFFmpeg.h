//
// Created by g10295 on 2021/8/16.
//

#ifndef CPLAYER_CFFMPEG_H
#define CPLAYER_CFFMPEG_H


#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"
#include <cstring>
#include "macro.h"
#include "pthread.h"

extern "C" {
#include <libavformat/avformat.h>
};

class CFFmpeg {

public:
    CFFmpeg(JavaCallHelper *javaCallHelper, char *dataSource);

    ~CFFmpeg();

    void prepare();

    void _prepare();

    void start();

    void _start();


private:
    JavaCallHelper *javaCallHelper = 0;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    char *dataSource;
    pthread_t pid_prepare;
    pthread_t pid_start;
    bool isPlaying;
    AVFormatContext *avFormatContext = 0;
};

#endif //CPLAYER_CFFMPEG_H
