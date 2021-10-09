package com.csh.cplayer;


import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    private CPlayer mPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager
                .LayoutParams.FLAG_KEEP_SCREEN_ON);

        SurfaceView surfaceView = findViewById(R.id.surfaceView);
        mPlayer = new CPlayer();

        mPlayer.setSurfaceView(surfaceView);


    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    public void open(View view) {
        mPlayer.setDataSource(new File(
                Environment.getExternalStorageDirectory() + File.separator + "demo.mp4").getAbsolutePath());
        mPlayer.setOnpreparedListener(new CPlayer.OnpreparedListener() {
            @Override
            public void onPrepared() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Log.e(TAG, "开始播放");
                        Toast.makeText(MainActivity.this, "开始播放！", Toast.LENGTH_SHORT).show();
                    }
                });
                //播放 调用到native去
                //start play
                //mPlayer.start();
            }
        });

        mPlayer.prepare();
    }
}
