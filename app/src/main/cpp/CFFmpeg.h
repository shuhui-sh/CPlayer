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

class CFFmpeg {

public:
    CFFmpeg();

    ~CFFmpeg();

    void prepare();

    void _prepare();


private:

    JavaCallHelper *javaCallHelper = 0;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    char *dataSource;
    pthread_t pid_prepare;
};

#endif //CPLAYER_CFFMPEG_H
