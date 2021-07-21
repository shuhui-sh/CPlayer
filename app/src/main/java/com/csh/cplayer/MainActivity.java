package com.csh.cplayer;


import android.os.Bundle;
import android.os.Environment;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;

public class MainActivity extends AppCompatActivity {


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

    public void open(View view) {
        File file = new File(Environment.getExternalStorageDirectory(), "input.mp4");
        if (!file.exists()) {
            Toast.makeText(getApplication(), "找不到指定input.mp4", Toast.LENGTH_SHORT);
            return;
        }
        mPlayer.playVideo(file.getAbsolutePath());
    }
}
