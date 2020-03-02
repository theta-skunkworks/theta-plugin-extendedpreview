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

package com.theta360.pluginapplication.task;

import android.util.Log;
import android.os.AsyncTask;
import com.theta360.pluginapplication.network.HttpConnector;
import com.theta360.pluginapplication.view.MJpegInputStream;
import org.json.JSONException;

import java.io.IOException;
import java.io.InputStream;


public class GetLiveViewTask extends AsyncTask<Void, Void, Void> {
    private static final String TAG = "GetLiveViewTask";
    private Callback mCallback;

    private static final String FORMAT_1920_8FPS  = "{\"width\": 1920, \"height\": 960, \"framerate\": 8}";
    private static final String FORMAT_1024_30FPS = "{\"width\": 1024, \"height\": 512, \"framerate\": 30}";
    private static final String FORMAT_1024_8FPS  = "{\"width\": 1024, \"height\": 512, \"framerate\": 8}";
    private static final String FORMAT_640_30FPS  = "{\"width\": 640, \"height\": 320, \"framerate\": 30}";
    private static final String FORMAT_640_8FPS   = "{\"width\": 640, \"height\": 320, \"framerate\": 8}";
    private String strFormat;

    public static final int FORMAT_NO_1920_8FPS  = 5;
    public static final int FORMAT_NO_1024_30FPS = 4;
    public static final int FORMAT_NO_1024_8FPS  = 3;
    public static final int FORMAT_NO_640_30FPS  = 2;
    public static final int FORMAT_NO_640_8FPS   = 1;

    private int fps=0;

    MJpegInputStream mjis = null;

    Boolean timeoutOccurred = false;


    public GetLiveViewTask(GetLiveViewTask.Callback callback, int formatNo) {
        switch ( formatNo) {
            case FORMAT_NO_1920_8FPS :
                strFormat = FORMAT_1920_8FPS;
                break;
            case FORMAT_NO_1024_30FPS :
                strFormat = FORMAT_1024_30FPS;
                break;
            case FORMAT_NO_1024_8FPS :
                strFormat = FORMAT_1024_8FPS;
                break;
            case FORMAT_NO_640_30FPS :
                strFormat = FORMAT_640_30FPS;
                break;
            case FORMAT_NO_640_8FPS :
                strFormat = FORMAT_640_8FPS;
                break;
            default:
                strFormat = FORMAT_1024_30FPS;
                break;
        }
        this.mCallback = callback;
    }

    @Override
    protected void onPreExecute() {
    }

    @Override
    protected Void doInBackground(Void... params) {

        HttpConnector camera = new HttpConnector("127.0.0.1:8080");
        String strResult = "";

        //set liveview Format
        String strJsonLiveviewFormat = "{\"name\": \"camera.setOptions\", \"parameters\": {\"options\": {\"previewFormat\":" + strFormat +  "} } }";
        strResult = camera.httpExec(HttpConnector.HTTP_POST, HttpConnector.API_URL_CMD_EXEC, strJsonLiveviewFormat);
        //Log.d(TAG, "set previewFormat result:" + strResult);

        //start liveview
        final int MAX_RETRY_COUNT = 20;
        for (int retryCount = 0; retryCount < MAX_RETRY_COUNT; retryCount++) {
            if ( isCancelled() ) {
                break;
            }

            try {
                InputStream is = camera.getLivePreview();
                mjis = new MJpegInputStream(is);
                retryCount = MAX_RETRY_COUNT;
            } catch (IOException e) {
                Log.d(TAG, "[LvStart] IOException:" + e + " : mGetLiveViewTask()");
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e1) {
                    e1.printStackTrace();
                }
                mjis=null;
            } catch (JSONException e) {
                Log.d(TAG, "[LvStart] JSONException:" + e + " : mGetLiveViewTask()");
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e1) {
                    e1.printStackTrace();
                }
                mjis=null;
            }
        }

        //For releasing resources when Timeout occurs
        mCallback.onGetResorce(mjis);

        //Read Live Preview
        fps = 0;
        long startTime = System.currentTimeMillis();

        while (mjis != null && !isCancelled()) {
            try {

                byte[] buff = mjis.readMJpegFrame();
                if (isCancelled()) {
                    break;
                }
                mCallback.onLivePreviewFrame(buff);


            } catch (IOException e) {
                Log.d(TAG, "[LvRead] IOException:" + e);
                e.printStackTrace();
                cancel(true);
                timeoutOccurred = true;
                mjis = null;
            }

            fps++;
            long curTime = System.currentTimeMillis();
            long diffTime = curTime - startTime;
            if (diffTime >= 1000 ) {
                Log.d(TAG, "[LvRead]" + String.valueOf(fps) + "[fps]");
                startTime = curTime;
                fps =0;
            }
        }
        if (mjis != null) {
            try {
                mjis.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return null;
    }

    @Override
    protected void onCancelled() {
        //
        mCallback.onCancelled(timeoutOccurred);
    }

    /**
     * Interface of Callback.
     */
    public interface Callback {
        void onGetResorce(MJpegInputStream mjis);
        void onLivePreviewFrame(byte[] previewByteArrays);
        void onCancelled(Boolean timeoutOccurred);
    }
}
