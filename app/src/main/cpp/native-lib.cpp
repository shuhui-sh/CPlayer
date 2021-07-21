#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <zconf.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_csh_cplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ FFMpeg Version : ";
    std::string version = av_version_info();
    return env->NewStringUTF(hello.append(version).c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_csh_cplayer_CPlayer_native_1play_1video(JNIEnv *env, jobject instance,
                                                 jstring path_, jobject surface) {
    const char *path = env->GetStringUTFChars(path_, 0);
    //初始化网络模块
    avformat_network_init();
    AVFormatContext *formatContext = avformat_alloc_context();
    //1、打开URL
    AVDictionary *opts = NULL;
    //设置超时3秒
    av_dict_set(&opts, "timeout", "3000000", 0);
    //强制指定AVFormatContext中AVInputFormat的。这个参数一般情况下可以设置为NULL，这样FFmpeg可以自动检测AVInputFormat。
    //输入文件的封装格式
    //av_find_input_format("avi")
    //ret为零 表示成功
    int ret = avformat_open_input(&formatContext, path, NULL, &opts);
    avformat_find_stream_info(formatContext, NULL);
    //视频时长（单位：微秒us，转换为秒需要除以1000000）
    int video_stream_index = -1;
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }

    AVCodecParameters *avCodecParameters = formatContext->streams[video_stream_index]->codecpar;
    //找到解码器
    AVCodec *avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
    //创建上下文
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    avcodec_parameters_to_context(avCodecContext, avCodecParameters);
    //版本升级了
    avcodec_open2(avCodecContext, avCodec, NULL);
    //读取包
    AVPacket *avPacket = av_packet_alloc();
    //像素数据
    SwsContext *swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                            avCodecContext->pix_fmt, avCodecContext->width,
                                            avCodecContext->height, AV_PIX_FMT_RGBA,
                                            SWS_BILINEAR, 0, 0, 0);
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    //视频缓冲区
    ANativeWindow_Buffer nativeWindowBuffer;
    //创建新的窗口用于视频显示
    ANativeWindow_setBuffersGeometry(nativeWindow, avCodecContext->width,
                                     avCodecContext->height,
                                     WINDOW_FORMAT_RGBA_8888);
    while (av_read_frame(formatContext, avPacket) >= 0) {
        avcodec_send_packet(avCodecContext, avPacket);
        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            //队列里需要更多数据
            continue;
        } else if (ret < 0) {
            break;
        }
        uint8_t *dst_data[0];
        int dst_linesize[0];
        av_image_alloc(dst_data, dst_linesize,
                       avCodecContext->width, avCodecContext->height, AV_PIX_FMT_RGBA, 1);

        if (avPacket->stream_index == video_stream_index) {
            //非零   正在解码
            if (ret == 0) {
                //绘制之前   配置一些信息  比如宽高   格式
                //绘制
                ANativeWindow_lock(nativeWindow, &nativeWindowBuffer, NULL);
                //     h264 -> yuv  -> RGBA
                //转为指定的YUV420P
                sws_scale(swsContext,
                          reinterpret_cast<const uint8_t *const *>(frame->data),
                          frame->linesize, 0,
                          frame->height,
                          dst_data, dst_linesize);
                //rgb_frame是有画面数据
                uint8_t *dst = (uint8_t *) nativeWindowBuffer.bits;
                //  拿到一行有多少个字节 RGBA
                int destStride = nativeWindowBuffer.stride * 4;
                uint8_t *src_data = dst_data[0];
                int src_linesize = dst_linesize[0];
                uint8_t *firstWindown = static_cast<uint8_t *>(nativeWindowBuffer.bits);
                for (int i = 0; i < nativeWindowBuffer.height; ++i) {
                    memcpy(firstWindown + i * destStride, src_data + i * src_linesize,
                           destStride);
                }
                ANativeWindow_unlockAndPost(nativeWindow);
                usleep(1000 * 16);
                av_frame_free(&frame);
            }
        }
    }
    ANativeWindow_release(nativeWindow);
    avcodec_close(avCodecContext);
    avformat_free_context(formatContext);
    env->ReleaseStringUTFChars(path_, path);
}