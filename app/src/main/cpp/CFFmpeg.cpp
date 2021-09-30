//
// Created by g10295 on 2021/8/16.
//

#include "CFFmpeg.h"


CFFmpeg::CFFmpeg(JavaCallHelper *javaCallHelper, char *dataSource) {
    this->javaCallHelper = javaCallHelper;
//    this->dataSource = dataSource;//?
    //这里的dataSource是从Java传过来的字符串，通过jni接口转成了c++字符串。
    //在jni方法中被释放掉了，导致 dataSource 变成悬空指针的问题（指向一块已经释放了的内存）
    //？？
    //内存拷贝，自己管理它的内存
    //strlen 获取字符串长度，strcpy：拷贝字符串

    //java: "hello"
    //c字符串以 \0 结尾 : "hello\0"
    this->dataSource = new char[strlen(dataSource) + 1];
    strcpy(this->dataSource, dataSource);

}

/**
 * 准备线程pid_prepare真正执行的函数
 * @param args
 * @return
 */
void *task_prepare(void *args) {
    //参数就是调用的时候传进来的this,强转成CFFmpeg
    CFFmpeg *cfFmpeg = static_cast<CFFmpeg *>(args);
    //因为要使用私有的dataSource成员变量，这个线程执行方法不能直接访问，所以写一个public成员函数来访问
    cfFmpeg->_prepare();
    //一定一定一定要返回0！！！
    return 0;
};

void CFFmpeg::_prepare() {
    avFormatContext = avformat_alloc_context();
    AVDictionary *dictionary = 0;
    //设置超时时间为10秒，这里的单位是微秒
    av_dict_set(&dictionary, "timeout", "10000000", 0);
    int ret = avformat_open_input(&avFormatContext, dataSource, 0, &dictionary);
    if (ret) {
        //失败 ，回调给java
        LOGE("打开媒体失败：%s", av_err2str(ret));
        //失败 ，回调给java
        //        LOGE("打开媒体失败：%s", av_err2str(ret));
        ////        javaCallHelper jni 回调java方法
        ////        javaCallHelper->onError(ret);
        //          //可能java层需要根据errorCode来更新UI!
        //          //2019.8.12 作业自行尝试实现
        //TODO 作业:反射通知java
        // 1 反射Java 属性（成员/静态）
        // 2 反射Java 方法 （成员/静态）
        // 3 子线程反射
        return;
    }

    //2 查找媒体中的流信息
    ret = avformat_find_stream_info(avFormatContext, 0);
    if (ret < 0) {
        LOGE("查找媒体中的流信息失败：%s", av_err2str(ret));
        //TODO 作业:反射通知java
        return;
    }

    //这里的 i 就是后面 166行的 packet->stream_index
    for(int i = 0; i < avFormatContext->nb_streams; ++i) {
        //3获取媒体流（音频或视频）
        AVStream *stream = avFormatContext->streams[i];
        //4获取编解码这段流的参数
        AVCodecParameters *codecParameters = stream->codecpar;
        //5 通过参数中的id（编解码的方式），来查找当前流的解码器
        AVCodec *codec = avcodec_find_decoder(codecParameters->codec_id);
        if (!codec) {
            LOGE("查找当前流的解码器失败");
            //TODO 作业:反射通知java
            return;
        }

        //6 创建解码器上下文
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        //7 设置解码器上下文的参数
        ret = avcodec_parameters_to_context(codecContext, codecParameters);
        if (ret < 0) {
            //TODO 作业:反射通知java
            LOGE("设置解码器上下文的参数失败：%s", av_err2str(ret));
            return;
        }
        //8 打开解码器
        ret = avcodec_open2(codecContext, codec, 0);
        if (ret) {
            //TODO 作业:反射通知java
            LOGE("打开解码器失败：%s", av_err2str(ret));
            return;
        }
        //判断流类型（音频还是视频？）
        if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            //音频
            audioChannel = new AudioChannel(i);
        } else if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            //视频
            videoChannel = new VideoChannel(i);
        }
    }//end for
    if (!audioChannel && !videoChannel) {
        //既没有音频也没有视频
        //TODO 作业:反射通知java
        LOGE("没有音视频");
        return;
    }

    //准备好了，反射通知java
    if (javaCallHelper) {
        javaCallHelper->onPrepared(THREAD_CHILD);
    }
}

void CFFmpeg::prepare() {
//    pthread_create： 创建子线程
//task_prepare是函数指针，后面的this是其参数
    pthread_create(&pid_prepare, 0, task_prepare, this);
}

CFFmpeg::~CFFmpeg() {
    DELETE(dataSource);
    DELETE(javaCallHelper)
}


