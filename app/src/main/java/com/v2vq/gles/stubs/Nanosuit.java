package com.v2vq.gles.stubs;

import android.app.NativeActivity;

public class Nanosuit extends NativeActivity {
	static {
	       System.loadLibrary("assimp");
	       System.loadLibrary("Nanosuit");
	    }
}