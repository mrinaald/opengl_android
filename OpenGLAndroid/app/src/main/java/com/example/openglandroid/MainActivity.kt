package com.example.openglandroid

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

import android.opengl.GLSurfaceView
import android.util.Log

import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import android.content.pm.ConfigurationInfo
import android.app.ActivityManager
import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Matrix
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.View
import android.view.Window
import android.widget.Button
import androidx.core.view.MotionEventCompat


class MainActivity : AppCompatActivity() {

  private val TAG = MainActivity::class.java.simpleName

  enum class GestureMode(val mode: Int) {
    MODE_ROTATE(1),
    MODE_PAN(2)
  }

  private var mGLESVersion: Int = 0
  private var mSupportGL3x: Boolean = false

  private var nativeApp: Long = 0

  private lateinit var mGLSurfaceView: GLSurfaceView

  private val mRenderer = object : GLSurfaceView.Renderer {
    override fun onDrawFrame(gl: GL10?) {
      nativeOnDrawFrame(nativeApp)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
      nativeOnSurfaceChanged(nativeApp, width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
      nativeOnSurfaceCreated(nativeApp)
      loadTextureFiles()
    }
  }

  private lateinit var mSwitchButton: Button
  private var mGestureMode = GestureMode.MODE_ROTATE

  private lateinit var mResetButton: Button

  private var mActivePointerId = MotionEvent.INVALID_POINTER_ID
  private var mLastTouchX: Float = 0f
  private var mLastTouchY: Float = 0f


  private val mScaleGestureListener =
    object : ScaleGestureDetector.SimpleOnScaleGestureListener() {

      private var lastSpanX: Float = 0f
      private var lastSpanY: Float = 0f

      override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
        lastSpanX = detector.currentSpanX
        lastSpanY = detector.currentSpanY
        return true

      }

      override fun onScale(detector: ScaleGestureDetector): Boolean {
        val spanX = detector.currentSpanX
        val spanY = detector.currentSpanY

        val scaleX = spanX - lastSpanX
        val scaleY = spanY - lastSpanY

        val focusX = detector.focusX
        val focusY = detector.focusY

        // do something
        nativeOnScaleGesture(nativeApp, scaleX, scaleY)

        lastSpanX = spanX
        lastSpanY = spanY
        return true
      }
  }

  private lateinit var mScaleGestureDetector: ScaleGestureDetector


  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    window.requestFeature(Window.FEATURE_NO_TITLE)  // hide title
    setContentView(R.layout.activity_main)
    supportActionBar?.hide()    // hide title bar

    nativeApp = nativeOnCreate()

    checkGLVersionCompatibility()
    mGLSurfaceView = findViewById(R.id.gl_surface_view)
    mGLSurfaceView.setEGLContextClientVersion(if (mSupportGL3x) 3 else 2)

    mGLSurfaceView.setRenderer(mRenderer)
    mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY

    mScaleGestureDetector = ScaleGestureDetector(this, mScaleGestureListener)
    mGLSurfaceView.setOnTouchListener { view, event -> glSurfaceViewTouchListener(view, event) }

    // Setting Switch Button Properties
    mSwitchButton = findViewById(R.id.switch_pan_rotate_button)
    if ( mGestureMode == GestureMode.MODE_ROTATE ) {
      mSwitchButton.text = modeToTextMap[GestureMode.MODE_PAN]
    }
    else {
      mSwitchButton.text = modeToTextMap[GestureMode.MODE_ROTATE]
    }

    mSwitchButton.setOnClickListener { switchGestureMode(it) }

    // Setting Reset Button Properties
    mResetButton = findViewById(R.id.reset_button)
    mResetButton.setOnClickListener { nativeOnViewReset(nativeApp) }
  }


  private fun glSurfaceViewTouchListener(view: View, event: MotionEvent): Boolean {
    var isEventHandled = false

    when (MotionEventCompat.getActionMasked(event)) {

      MotionEvent.ACTION_DOWN -> {
        MotionEventCompat.getActionIndex(event).also { pointerIndex ->
          // Remember where we started (for dragging)
          mLastTouchX = MotionEventCompat.getX(event, pointerIndex)
          mLastTouchY = MotionEventCompat.getY(event, pointerIndex)
        }

        // Save the ID of this pointer (for dragging)
        mActivePointerId = MotionEventCompat.getPointerId(event, 0)
      }


      MotionEvent.ACTION_MOVE -> {
        // Find the index of the active pointer and fetch its position
        val (x: Float, y: Float) =
          MotionEventCompat.findPointerIndex(event, mActivePointerId).let { pointerIndex ->
            // Calculate the distance moved
            MotionEventCompat.getX(event, pointerIndex) to
                    MotionEventCompat.getY(event, pointerIndex)
          }

        val diffX = x - mLastTouchX
        val diffY = y - mLastTouchY

        if (MotionEventCompat.getPointerCount(event) == 1) {
          isEventHandled = true
          nativeOnDragGesture(nativeApp, diffX, diffY)
        }

        // Remember this touch position for the next move event
        mLastTouchX = x
        mLastTouchY = y
      }


      MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
        mActivePointerId = MotionEvent.INVALID_POINTER_ID
      }


      MotionEvent.ACTION_POINTER_UP -> {
        MotionEventCompat.getActionIndex(event).also { pointerIndex ->
          MotionEventCompat.getPointerId(event, pointerIndex)
            .takeIf { it == mActivePointerId }
            ?.run {
              // This was our active pointer going up. Choose a new
              // active pointer and adjust accordingly.
              val newPointerIndex = if (pointerIndex == 0) 1 else 0
              mLastTouchX = MotionEventCompat.getX(event, newPointerIndex)
              mLastTouchY = MotionEventCompat.getY(event, newPointerIndex)
              mActivePointerId = MotionEventCompat.getPointerId(event, newPointerIndex)
            }
        }
      }
    }

    return mScaleGestureDetector.onTouchEvent(event) || isEventHandled
  }


  private fun switchGestureMode(v: View) {
    when ( mGestureMode ) {
      GestureMode.MODE_ROTATE -> {
        // Switching to PAN Mode
        mGestureMode = GestureMode.MODE_PAN
        mSwitchButton.text = modeToTextMap[GestureMode.MODE_ROTATE]
        nativeSetGestureMode(nativeApp, mGestureMode.mode)
      }

      GestureMode.MODE_PAN -> {
        // Switching to ROTATE Mode
        mGestureMode = GestureMode.MODE_ROTATE
        mSwitchButton.text = modeToTextMap[GestureMode.MODE_PAN]
        nativeSetGestureMode(nativeApp, mGestureMode.mode)
      }
    }
  }


  override fun onPause() {
    Log.d(TAG, "onPause()")
    nativeOnPause(nativeApp)
    super.onPause()
  }

  override fun onResume() {
    Log.d(TAG, "onResume()")
    super.onResume()
    nativeOnResume(nativeApp)
  }

  override fun onDestroy() {
    Log.d(TAG, "onDestroy()")
    nativeOnDestroy(nativeApp)
    nativeApp = 0
    super.onDestroy()
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


  private fun loadTextureFiles() {
    val options = BitmapFactory.Options()
    options.inScaled = false

    var bitmap = BitmapFactory.decodeStream(assets.open("container.jpg"))
    nativeLoadTextureFromBitmap(nativeApp, bitmap.verticalFlip(), 0)

    bitmap = BitmapFactory.decodeStream(assets.open("awesomeface.png"))
    nativeLoadTextureFromBitmap(nativeApp, bitmap.verticalFlip(), 1)
  }


  private fun Bitmap.verticalFlip(): Bitmap {
    val cx = this.width / 2f
    val cy = this.height / 2f
    val mat = Matrix().apply{ postScale(1f, -1f, cx, cy) }
    return Bitmap.createBitmap(this, 0, 0, width, height, mat, true)
  }


  companion object {

    private const val SWITCH_TO_PAN = "Switch to PAN Mode"
    private const val SWITCH_TO_ROTATE = "Switch to ROTATE Mode"

    private val modeToTextMap = mapOf(
                              (GestureMode.MODE_PAN to SWITCH_TO_PAN),
                              (GestureMode.MODE_ROTATE to SWITCH_TO_ROTATE)
                            )

    // Used to load the 'native-lib' library on application startup.
    init {
      System.loadLibrary("native-lib")
    }
  }

  private external fun nativeOnCreate(): Long

  private external fun nativeOnPause(nativeApp: Long)

  private external fun nativeOnResume(nativeApp: Long)

  private external fun nativeOnDestroy(nativeApp: Long)

  external fun nativeOnSurfaceCreated(nativeApp: Long)

  external fun nativeOnSurfaceChanged(nativeApp: Long, width: Int, height: Int)

  external fun nativeOnDrawFrame(nativeApp: Long)

  private external fun nativeLoadTextureFromBitmap(nativeApp: Long, bitmap: Bitmap, newActiveTextureId: Int)

  private external fun nativeOnViewReset(nativeApp: Long)

  private external fun nativeSetGestureMode(nativeApp: Long, mode: Int)

  private external fun nativeOnDragGesture(nativeApp: Long, diffX: Float, diffY: Float)

  private external fun nativeOnScaleGesture(nativeApp: Long, scaleX: Float, scaleY: Float)
}
