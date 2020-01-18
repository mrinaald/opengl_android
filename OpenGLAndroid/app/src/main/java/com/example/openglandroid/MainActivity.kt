package com.example.openglandroid

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

import android.opengl.GLSurfaceView
import android.util.Log

import kotlinx.android.synthetic.main.activity_main.*
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import android.content.pm.ConfigurationInfo
import android.app.ActivityManager
import android.content.Context



class MainActivity : AppCompatActivity() {

    private val TAG = MainActivity::class.java.simpleName
    var mGLESVersion : Int = 0
    var mSupportGL3x : Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        checkGLVersionCompatibility()
        gl_surface_view.setEGLContextClientVersion(if (mSupportGL3x) 3 else 2)

        gl_surface_view.setRenderer(Renderer())
        gl_surface_view.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
    }

    private fun checkGLVersionCompatibility ()  {
        mGLESVersion = getVersionFromActivityManager(this)

        val majorVersion = ((mGLESVersion and 0xFFFF0000.toInt())) shr 16
        val minorVersion = (mGLESVersion and 0xFFFF)
        Log.d(TAG, "OpenGL Version from Activity Manager: ${majorVersion}.${minorVersion}")

        mSupportGL3x = majorVersion == 3
    }

    /**
     * This function is taken directly from
     * https://android.googlesource.com/platform/cts/+/2b87267/tests/tests/graphics/src/android/opengl/cts/OpenGlEsVersionTest.java
     */
    private fun getVersionFromActivityManager(context: Context): Int {
        val activityManager = context.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val configInfo = activityManager.deviceConfigurationInfo
        return if (configInfo.reqGlEsVersion != ConfigurationInfo.GL_ES_VERSION_UNDEFINED) {
            configInfo.reqGlEsVersion
        } else {
            1 shl 16 // Lack of property means OpenGL ES version 1
        }
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
