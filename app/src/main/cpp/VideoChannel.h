//
// Created by g10295 on 2021/8/16.
//

#ifndef CPLAYER_VIDEOCHANNEL_H
#define CPLAYER_VIDEOCHANNEL_H

#include "BaseChannel.h"

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

typedef void (*RenderCallback)(uint8_t *, int, int, int);

class VideoChannel : public BaseChannel {

public:
    VideoChannel(int id);


    ~VideoChannel();

    virtual void start();

    virtual void stop();

    void video_decode();

    void video_play();

    void setRenderCallback(RenderCallback renderCallback);

private:
    pthread_t pid_video_decode;
    pthread_t pid_video_play;
    RenderCallback renderCallback;

};

#endif //CPLAYER_VIDEOCHANNEL_H
