//
// Created by g10295 on 2021/8/16.
//

#ifndef CPLAYER_BASECHANNEL_H
#define CPLAYER_BASECHANNEL_H

#include "safe_queue.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
};




/**
 * VideoChannel和AudioChannel的父类
 */
class BaseChannel {

public:
    SafeQueue<AVPacket *> packets;
    SafeQueue<AVFrame *> frames;
    int id;

    BaseChannel(int id) : id(id){
        packets.setReleaseCallback(releaseAVPacket);
        frames.setReleaseCallback(releaseAVFrame);
    }

    /**
    * 释放 AVPacket
    * @param packet
    */
    static void releaseAVPacket(AVPacket **packet) {
        if (packet) {
            av_packet_free(packet);
            *packet = 0;
        }
    }

    /**
     * 释放 AVFrame
     * @param frame
     */
    static void releaseAVFrame(AVFrame **frame) {
        if (frame) {
            av_frame_free(frame);
            *frame = 0;
        }
    }

    //纯虚函数（抽象方法）
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual ~BaseChannel() {
        packets.clear();
        frames.clear();
    }

};

#endif //CPLAYER_BASECHANNEL_H
