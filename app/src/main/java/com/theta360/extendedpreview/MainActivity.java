/**
 * Copyright 2018 Ricoh Company, Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.theta360.extendedpreview;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.AsyncTask;
import android.util.Log;
import android.os.Bundle;
import java.io.IOException;
import android.view.KeyEvent;

import com.theta360.pluginlibrary.activity.PluginActivity;
import com.theta360.pluginlibrary.callback.KeyCallback;
import com.theta360.pluginlibrary.receiver.KeyReceiver;
import com.theta360.pluginapplication.task.TakePictureTask;
import com.theta360.pluginapplication.task.TakePictureTask.Callback;
import com.theta360.pluginapplication.task.GetLiveViewTask;
import com.theta360.pluginapplication.task.MjisTimeOutTask;
import com.theta360.pluginapplication.view.MJpegInputStream;
import com.theta360.pluginapplication.oled.Oled;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;


public class MainActivity extends PluginActivity {
    private static final String TAG = "ExtendedPreview";

    //Button Resorce
    private boolean onKeyDownModeButton = false;
    private boolean onKeyLongPressWlan = false;
    private boolean onKeyLongPressFn = false;

    //Preview Resorce
    private int previewFormatNo;
    GetLiveViewTask mGetLiveViewTask;
    private byte[]		latestLvFrame;

    //Preview Timeout Resorce
    private static final long FRAME_READ_TIMEOUT_MSEC  = 1000;
    MjisTimeOutTask mTimeOutTask;
    MJpegInputStream mjis;

    //WebServer Resorce
    private Context context;
    private WebServer webServer;

    //OLED Dislay Resorce
    Oled oledDisplay = null;
    private boolean mFinished;

    private static final int DISP_MODE_BIN = 0;
    private static final int DISP_MODE_EDGE = 1;
    private static final int DISP_MODE_MOTION = 2;
    int dispMode = DISP_MODE_BIN;


    private TakePictureTask.Callback mTakePictureTaskCallback = new Callback() {
        @Override
        public void onTakePicture(String fileUrl) {
            startPreview(mGetLiveViewTaskCallback, previewFormatNo);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Set enable to close by pluginlibrary, If you set false, please call close() after finishing your end processing.
        setAutoClose(true);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        //init OLED
        oledDisplay = new Oled(getApplicationContext());
        oledDisplay.brightness(100);
        oledDisplay.clear(oledDisplay.black);
        oledDisplay.draw();

        // Set a callback when a button operation event is acquired.
        setKeyCallback(new KeyCallback() {
            @Override
            public void onKeyDown(int keyCode, KeyEvent event) {
                switch (keyCode) {
                    case KeyReceiver.KEYCODE_CAMERA :
                        stopPreview();
                        new TakePictureTask(mTakePictureTaskCallback).execute();
                        break;
                    case KeyReceiver.KEYCODE_MEDIA_RECORD :
                        // Disable onKeyUp of startup operation.
                        onKeyDownModeButton = true;
                        break;
                    default:
                        break;
                }
            }

            @Override
            public void onKeyUp(int keyCode, KeyEvent event) {

                switch (keyCode) {
                    case KeyReceiver.KEYCODE_WLAN_ON_OFF :
                        if (onKeyLongPressWlan) {
                            onKeyLongPressWlan=false;
                        } else {

                            dispMode++;
                            if ( dispMode > DISP_MODE_MOTION ) {
                                dispMode= DISP_MODE_BIN;
                            }

                        }

                        break;
                    case KeyReceiver.KEYCODE_MEDIA_RECORD :
                        if (onKeyDownModeButton) {
                            if (mGetLiveViewTask!=null) {
                                stopPreview();
                            } else {
                                startPreview(mGetLiveViewTaskCallback, previewFormatNo);
                            }
                            onKeyDownModeButton = false;
                        }
                        break;
                    case KeyEvent.KEYCODE_FUNCTION :
                        if (onKeyLongPressFn) {
                            onKeyLongPressFn=false;
                        } else {

                            //NOP : KEYCODE_FUNCTION

                        }

                        break;
                    default:
                        break;
                }

            }

            @Override
            public void onKeyLongPress(int keyCode, KeyEvent event) {
                switch (keyCode) {
                    case KeyReceiver.KEYCODE_WLAN_ON_OFF:
                        onKeyLongPressWlan=true;

                        //NOP : KEYCODE_WLAN_ON_OFF

                        break;
                    case KeyEvent.KEYCODE_FUNCTION :
                        onKeyLongPressFn=true;

                        //NOP : KEYCODE_FUNCTION

                        break;
                    default:
                        break;
                }

            }
        });

        this.context = getApplicationContext();
        this.webServer = new WebServer(this.context, mWebServerCallback);
        try {
            this.webServer.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (isApConnected()) {

        }

        //Start LivePreview
        previewFormatNo = GetLiveViewTask.FORMAT_NO_640_30FPS;
        startPreview(mGetLiveViewTaskCallback, previewFormatNo);

        //Start OLED thread
        mFinished = false;
        drawOledThread();
    }

    @Override
    protected void onPause() {
        // Do end processing
        //close();

        //Stop Web server
        this.webServer.stop();

        //Stop LivePreview
        stopPreview();

        //Stop OLED thread
        mFinished = true;

        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (this.webServer != null) {
            this.webServer.stop();
        }
    }

    private void startPreview(GetLiveViewTask.Callback callback, int formatNo){
        if (mGetLiveViewTask!=null) {
            stopPreview();

            try {
                Thread.sleep(400);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        mGetLiveViewTask = new GetLiveViewTask(callback, formatNo);
        mGetLiveViewTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void stopPreview(){
        //At the intended stop, timeout monitoring also stops.
        if (mTimeOutTask!=null) {
            mTimeOutTask.cancel(false);
            mTimeOutTask=null;
        }

        if (mGetLiveViewTask!=null) {
            mGetLiveViewTask.cancel(false);
            mGetLiveViewTask = null;
        }
    }


    /**
     * GetLiveViewTask Callback.
     */
    private GetLiveViewTask.Callback mGetLiveViewTaskCallback = new GetLiveViewTask.Callback() {

        @Override
        public void onGetResorce(MJpegInputStream inMjis) {
            mjis = inMjis;
        }

        @Override
        public void onLivePreviewFrame(byte[] previewByteArray) {
            latestLvFrame = previewByteArray;

            //Update timeout monitor
            if (mTimeOutTask!=null) {
                mTimeOutTask.cancel(false);
                mTimeOutTask=null;
            }
            mTimeOutTask = new MjisTimeOutTask(mMjisTimeOutTaskCallback, FRAME_READ_TIMEOUT_MSEC);
            mTimeOutTask.execute();
        }

        @Override
        public void onCancelled(Boolean inTimeoutOccurred) {
            mGetLiveViewTask = null;
            latestLvFrame = null;

            if (inTimeoutOccurred) {
                startPreview(mGetLiveViewTaskCallback, previewFormatNo);
            }
        }

    };


    /**
     * MjisTimeOutTask Callback.
     */
    private MjisTimeOutTask.Callback mMjisTimeOutTaskCallback = new MjisTimeOutTask.Callback() {
        @Override
        public void onTimeoutExec(){
            if (mjis!=null) {
                try {
                    // Force an IOException to `mjis.readMJpegFrame()' in GetLiveViewTask()
                    mjis.close();
                } catch (IOException e) {
                    Log.d(TAG, "[timeout] mjis.close() IOException");
                    e.printStackTrace();
                }
                mjis=null;
            }
        }
    };

    /**
     * WebServer Callback.
     */
    private WebServer.Callback mWebServerCallback = new WebServer.Callback() {
        @Override
        public void execStartPreview(int format) {
            previewFormatNo = format;
            startPreview(mGetLiveViewTaskCallback, format);
        }

        @Override
        public void execStopPreview() {
            stopPreview();
        }

        @Override
        public boolean execGetPreviewStat() {
            if (mGetLiveViewTask==null) {
                return false;
            } else {
                return true;
            }
        }

        @Override
        public byte[] getLatestFrame() {
            return latestLvFrame;
        }
    };

    //==============================================================
    // OLED Thread
    //==============================================================
    public void drawOledThread() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                int outFps=0;
                long startTime = System.currentTimeMillis();
                Bitmap beforeBmp = null;

                while (mFinished == false) {

                    byte[] jpegFrame = latestLvFrame;
                    if ( jpegFrame != null ) {

                        //JPEG -> Bitmap
                        Bitmap bitmap = BitmapFactory.decodeByteArray(jpegFrame, 0, jpegFrame.length);
                        //Resize Bitmap : width=128pix, height=64pix.
                        Bitmap smallBmp = Bitmap.createScaledBitmap(bitmap, 128,  64, true);

                        Bitmap resultBmp=null;
                        switch (dispMode) {
                            case DISP_MODE_EDGE :
                                resultBmp= edgeImage(smallBmp, 25);
                                break;
                            case DISP_MODE_MOTION :
                                if (beforeBmp!=null){
                                    resultBmp = motionImage(smallBmp, beforeBmp, 30);
                                } else {
                                    resultBmp = smallBmp;
                                }
                                beforeBmp =smallBmp;
                                break;
                            default:
                                resultBmp=smallBmp;
                                try {
                                    Thread.sleep(22);
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                        }
                        //[Output to OLED]
                        // After creating the 128x24 image by cutting the top and bottom 20pix each, it is output.)
                        //-- The code in the comments below is slow. --
                        //oledDisplay.setBitmap(0,0, Oled.OLED_WIDTH, Oled.OLED_HEIGHT, 0, 20, 128, resultBmp);
                        //oledDisplay.draw();
                        //-- The following code is fast. That's because it doesn't process the brightness threshold. --
                        Bitmap cutBmp = Bitmap.createBitmap(resultBmp, 0, 20, 128, 24);
                        Intent imageIntent = new Intent("com.theta360.plugin.ACTION_OLED_IMAGE_SHOW");
                        imageIntent.putExtra("bitmap", cutBmp);
                        sendBroadcast(imageIntent);


                        outFps++;
                    } else {
                        try {
                            Thread.sleep(33);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                    long curTime = System.currentTimeMillis();
                    long diffTime = curTime - startTime;
                    if (diffTime >= 1000 ) {
                        Log.d(TAG, "[OLED]" + String.valueOf(outFps) + "[fps]" );
                        startTime = curTime;
                        outFps =0;
                    }

                }
            }
        }).start();
    }

    private Bitmap motionImage(Bitmap inBitmap1, Bitmap inBitmap2, int threshold){
        int black = 0xFF000000 ;
        int white = 0xFFFFFFFF ;

        int imgWidth1 = inBitmap1.getWidth();
        int imgHeight1 = inBitmap1.getHeight();
        int imgWidth2 = inBitmap2.getWidth();
        int imgHeight2 = inBitmap2.getHeight();

        Bitmap moveBmp = Bitmap.createBitmap(imgWidth1, imgHeight1, Bitmap.Config.ARGB_8888 );

        if ( (imgWidth1==imgWidth2) && (imgHeight1==imgHeight2) ) {
            for (int width=0; width<imgWidth1; width++) {
                for (int height=0; height<imgHeight1; height++) {
                    int Y1 = getY(inBitmap1.getPixel(width, height ));
                    int Y2 = getY(inBitmap2.getPixel(width, height ));

                    int absDiffY = Math.abs( Y1 - Y2 );
                    if ( absDiffY >= threshold ) {
                        moveBmp.setPixel(width, height, white);
                    } else {
                        moveBmp.setPixel(width, height, black);
                    }
                }
            }

        } else {
            Log.d(TAG, "input size error!" );
        }

        return moveBmp;
    }

    private Bitmap edgeImage(Bitmap inBitmap, int threshold){
        int black = 0xFF000000 ;
        int white = 0xFFFFFFFF ;

        int imgWidth = inBitmap.getWidth();
        int imgHeight = inBitmap.getHeight();

        Bitmap diffBmp = Bitmap.createBitmap(imgWidth, imgHeight, Bitmap.Config.ARGB_8888 );

        for (int width=0; width<imgWidth; width++) {
            for (int height=0; height<imgHeight; height++) {

                int Y1 = getY(inBitmap.getPixel(width, height ));
                int diffWidth;
                if ( width == (imgWidth-1) ) {
                    diffWidth=0;
                } else {
                    diffWidth=width+1;
                }
                int Y2 = getY(inBitmap.getPixel(diffWidth, height ));

                int absDiffY = Math.abs( Y1 - Y2 );
                if ( absDiffY >= threshold ) {
                    diffBmp.setPixel(width, height, white);
                } else {
                    diffBmp.setPixel(width, height, black);
                }

            }
        }

        return diffBmp;
    }

    private int getY(int inBmpColor){
        // int color = (A & 0xff) << 24 | (B & 0xff) << 16 | (G & 0xff) << 8 | (R & 0xff);
        //Y =  0.299 x R + 0.587  x G + 0.114  x B
        double dY = 0.299*(inBmpColor&0x000000FF) + 0.587*((inBmpColor&0x0000FF00)>>8) + 0.114*((inBmpColor&0x00FF0000)>>16);
        int Y = (int)(dY+0.5);
        return Y;
    }

}

