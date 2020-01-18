package com.example.openglandroid

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

import android.opengl.GLSurfaceView
import android.util.Log

import kotlinx.android.synthetic.main.activity_main.*
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {

    private val TAG = MainActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        gl_surface_view.setEGLContextClientVersion(2)
        gl_surface_view.setRenderer(Renderer())
        gl_surface_view.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
    }

    private class Renderer : GLSurfaceView.Renderer {
        override fun onDrawFrame(gl: GL10?) {

        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {

        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            gl?.glGetString(GL10.GL_VERSION).also {
                Log.d(this::class.java.simpleName, "GL Version: $it")
            }
        }

    }

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
