package com.csh.cplayer;

public class NEPlayer {

    static {
        System.loadLibrary("cplayer");
    }

    private String dataSource;
    private MyErrorListener listener;

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void prepare() {
        prepareNative(dataSource);
    }

    public void start() {
        startNative();
    }

    /**
     * 供native反射调用
     * 表示播放器准备好了可以开始播放了
     */
    public void onPrepared() {

    }

    /**
     * 给jni回调用的
     * errorCode 从jni 反射传递过来
     * @param errorCode
     */
    public void onError(int errorCode){
        listener.onError(errorCode);
    }

    void setListener(MyErrorListener listener){
        this.listener = listener;
    }


    interface MyErrorListener {
        void onError(int errorCode);
    }

    private native void prepareNative(String dataSource);
    private native void startNative();
}
