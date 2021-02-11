package com.example.myffmpegandroid;

import android.util.Log;

public class FFmpegHelper {

    static {
        System.loadLibrary("avutil");
        System.loadLibrary("swscale");
        System.loadLibrary("swresample");
        System.loadLibrary("avcodec");
        System.loadLibrary("avformat");
        System.loadLibrary("avfilter");
        System.loadLibrary("avdevice");
        System.loadLibrary("ffmpegjni");
    }

    public static native int executeCmd(String[] commandArgs);

    // success 0, error 1
    public static int runCommand(String command) {
        if(command == null || command.length() == 0) {
            return -1;
        }

        String[] args = command.split(" ");
        return executeCmd(args);
    }
}
