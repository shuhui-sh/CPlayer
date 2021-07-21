package com.csh.cplayer;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;


public class CPlayer implements SurfaceHolder.Callback {
    private SurfaceHolder surfaceHolder;

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

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        this.surfaceHolder = holder;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void playVideo(String path) {
        native_play_video(path, surfaceHolder.getSurface());
    }

    public native void native_play_video(String path, Surface surface);
}
