package com.example.chen.ndktest;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.support.annotation.Nullable;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

/**
 * @author Chenhong
 * @date 2018/11/26.
 * @des
 */
public class ProgressService extends Service {
    static {
        System.loadLibrary("native-lib");
    }
    int i = 0;
    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Watcher watcher = new Watcher();
//                watcher.createWatcher(String.valueOf(Process.myUid()));
//                watcher.connectMonitor();
//            }
//        }).start();
        Watcher watcher = new Watcher();
        watcher.createWatcher(String.valueOf(Process.myUid()));
        watcher.connectMonitor();
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.e("tag","服务运行中..." + i);
                i++;
            }
        }, 0,3 * 1000);
    }
}
