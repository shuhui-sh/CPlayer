package com.csh.cplayer;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;


public class CPlayer implements SurfaceHolder.Callback {
    private SurfaceHolder surfaceHolder;
    //直播地址或媒体文件路径
    private String dataSource;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("cplayer");
    }

    public void setSurfaceView(SurfaceView surfaceView) {
        if (this.surfaceHolder != null) {
            this.surfaceHolder.removeCallback(this);
        }

        this.surfaceHolder = surfaceView.getHolder();
        this.surfaceHolder.addCallback(this);
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    /**
     * 画布刷新
     * @param holder
     * @param format
     * @param width
     * @param height
     */
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        setSurfaceNative(holder.getSurface());
    }

    /**
     * 画布销毁
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }
    /**
     * 播放准备工作
     */
    public void prepare() {
        prepareNative(dataSource);
    }

    /**
     * 开始播放
     */
    public void start() {
        startNative();
    }

    /**
     * 供native反射调用
     * 表示播放器准备好了可以开始播放了
     */
    public void onPrepared() {
        if (onpreparedListener != null) {
            onpreparedListener.onPrepared();
        }
    }

    void setOnpreparedListener(OnpreparedListener onpreparedListener) {
        this.onpreparedListener = onpreparedListener;
    }

    interface OnpreparedListener {
        void onPrepared();
    }

    private OnpreparedListener onpreparedListener;

    private native void prepareNative(String dataSource);
    private native void startNative();
    private native void setSurfaceNative(Surface surface);
}
