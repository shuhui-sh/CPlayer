//
// Created by g10295 on 2021/8/16.
//

#ifndef CPLAYER_AUDIOCHANNEL_H
#define CPLAYER_AUDIOCHANNEL_H

#include "BaseChannel.h"

class AudioChannel : public BaseChannel {
public:
    AudioChannel(int id);

    ~AudioChannel();

    void start();

    void stop();
};

#endif //CPLAYER_AUDIOCHANNEL_H
