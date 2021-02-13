package com.example.myffmpegandroid;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.loader.content.AsyncTaskLoader;
import androidx.navigation.fragment.NavHostFragment;

import com.example.myffmpegandroid.databinding.FragmentFirstBinding;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class FirstFragment extends Fragment {

    private FragmentFirstBinding binding;

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState
    ) {

        binding = FragmentFirstBinding.inflate(inflater, container, false);
        return binding.getRoot();

    }

    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        if (ContextCompat.checkSelfPermission(getActivity(), Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] { Manifest.permission.WRITE_EXTERNAL_STORAGE }, 1);
        }

        binding.buttonFirst.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                File file = new File("/sdcard/demoout.mp4");
                if (file.exists()) {
                    file.delete();
                }

                ExecutorService executorService = Executors.newFixedThreadPool(1);
                executorService.submit(new Runnable() {
                    @Override
                    public void run() {

                        String testCmd = "ffmpeg -ss 0.0 -i /sdcard/demo.mp4 -t 4 /sdcard/demoout.mp4";
                        Log.e("ffmpegjnitest", "FFmpeg test cmd : " + testCmd);
                        Log.e("ffmpegjnitest", "FFmpeg return code : " + FFmpegHelper.runCommand(testCmd));
                    }
                });
            }
        });

    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

}