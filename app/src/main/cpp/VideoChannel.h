//
// Created by g10295 on 2021/8/16.
//

#ifndef CPLAYER_VIDEOCHANNEL_H
#define CPLAYER_VIDEOCHANNEL_H

#include "BaseChannel.h"

class VideoChannel : public BaseChannel {

public:
    VideoChannel(int id);


    ~VideoChannel();

    virtual void start();

    virtual void stop();

};

#endif //CPLAYER_VIDEOCHANNEL_H
