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

  private var mGLESVersion: Int = 0
  private var mSupportGL3x: Boolean = false
  private var nativeApp: Long = 0

  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_main)

    nativeApp = nativeOnCreate()

    checkGLVersionCompatibility()
    gl_surface_view.setEGLContextClientVersion(if (mSupportGL3x) 3 else 2)

    gl_surface_view.setRenderer(Renderer())
    gl_surface_view.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
  }

  override fun onPause() {
    super.onPause()
    nativeOnPause(nativeApp)
  }

  override fun onResume() {
    super.onResume()
    nativeOnResume(nativeApp)
  }

  override fun onDestroy() {
    super.onDestroy()
    nativeOnDestroy(nativeApp)
    nativeApp = 0
  }

  private fun checkGLVersionCompatibility() {
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
      Log.d(TAG, "String: ${configInfo.glEsVersion}")
      configInfo.reqGlEsVersion
    } else {
      1 shl 16 // Lack of property means OpenGL ES version 1
    }
  }

  private inner class Renderer : GLSurfaceView.Renderer {
    override fun onDrawFrame(gl: GL10?) {
      nativeOnDrawFrame(nativeApp)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
      nativeOnSurfaceChanged(nativeApp, width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
      nativeOnSurfaceCreated(nativeApp)
    }

  }

  companion object {

    // Used to load the 'native-lib' library on application startup.
    init {
      System.loadLibrary("native-lib")
    }
  }

  external fun nativeOnCreate(): Long

  external fun nativeOnPause(nativeApp: Long)

  external fun nativeOnResume(nativeApp: Long)

  external fun nativeOnDestroy(nativeApp: Long)

  external fun nativeOnSurfaceCreated(nativeApp: Long)

  external fun nativeOnSurfaceChanged(nativeApp: Long, width: Int, height: Int)

  external fun nativeOnDrawFrame(nativeApp: Long)
}
