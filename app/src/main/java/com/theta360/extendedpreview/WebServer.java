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

import android.content.ContentResolver;
import android.content.Context;
import android.content.res.AssetManager;
import android.net.Uri;
import android.util.Log;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import com.theta360.pluginapplication.task.GetLiveViewTask;
import com.theta360.pluginapplication.network.HttpConnector;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayInputStream;
import java.util.Map;
import java.util.HashMap;
import fi.iki.elonen.NanoHTTPD;
import org.json.JSONException;
import org.json.JSONObject;

public class WebServer extends NanoHTTPD {
    private static final String TAG = "WebServer";

    private Callback mCallback;

    private static final int PORT = 8888;
    private Context context;
    private int previewFormatNo=-1;

    public WebServer(Context context, WebServer.Callback callback) {
        super(PORT);
        this.context = context;
        mCallback = callback;
    }

    @Override
    public Response serve(IHTTPSession session) {
        Method method = session.getMethod();
        String uri = session.getUri();
        String postData = getPostData(session);
        //Log.d(TAG, "serve:[method]=" + method + ", [uri]=" + uri + ", [postData]=" + postData );

        if( uri.startsWith("/osc/") ) {
            //Divert THETA webAPI(OSC) commands.
            return serveOsc(method, uri, postData);
        } else if (uri.equals("/preview/commands/execute")) {
            //Process custom commands
            return servePreviewCommnand(method, uri, postData);
        } else if (uri.equals("/favicon.ico")) {
            //Returns the icon image requested by the browser. (Not required.)
            return serveAssetFiles("/img/theta_logo.jpg");
        } else {
            //If the requested file is in the asset folder, send it.（Skip POST / GET identification）
            return serveAssetFiles(uri);
        }
    }

    private String getPostData(IHTTPSession session ) {
        String postData="";
        Map<String, String> tmpRequestFile = new HashMap<>();
        try {
            session.parseBody(tmpRequestFile);
            postData = tmpRequestFile.get("postData");
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ResponseException e) {
            e.printStackTrace();
        }
        return postData;
    }

    private Response servePreviewCommnand(Method method, String uri, String strJson) {

        try {
            JSONObject command = new JSONObject(strJson);
            String name = "";
            if (command.isNull("name")) {
                name ="";
            } else {
                name = command.getString("name");
            }

            if ( name.equals("camera.getPreviewFrame") ) {
                int resizeWidth;
                int quality;
                if (command.isNull("parameters")) {
                    resizeWidth = 0; //Non Resize
                    quality = 100;
                } else {
                    JSONObject parameters = command.getJSONObject("parameters");

                    if (parameters.isNull("resizeWidth")) {
                        resizeWidth = 0; //Non Resize
                    } else {
                        resizeWidth = parameters.getInt("resizeWidth");
                    }

                    if (parameters.isNull("quality")) {
                        quality = 100;
                    } else {
                        quality = parameters.getInt("quality");
                        if ( (quality<0) || (100<quality) ) {
                            quality = 100;
                        }
                    }
                }
                return serveFrame(resizeWidth, quality);

            } else if ( name.equals("camera.getPreviewStat") ) {
                String result;
                if (mCallback.execGetPreviewStat()) {
                    result="on";
                } else {
                    result="off";
                }
                return newFixedLengthResponse(Response.Status.OK, "text/plain", "{\"name\":\"camera.getPreviewStat\", \"state\":\"done\", \"results\":\"" + result + "\"}");

            } else if ( name.equals("camera.startPreview") ) {
                int formatNo;
                if (command.isNull("parameters")) {
                    formatNo = GetLiveViewTask.FORMAT_NO_1024_30FPS;
                } else {
                    JSONObject parameters = command.getJSONObject("parameters");
                    if (parameters.isNull("formatNo")) {
                        formatNo = GetLiveViewTask.FORMAT_NO_1024_30FPS;
                    } else {
                        formatNo = parameters.getInt("formatNo");
                    }
                }

                mCallback.execStartPreview(formatNo);

                return newFixedLengthResponse(Response.Status.OK, "text/plain", "{\"name\":\"camera.startPreview\", \"state\":\"done\"}");

            } else if ( name.equals("camera.stopPreview") ) {
                mCallback.execStopPreview();
                return newFixedLengthResponse(Response.Status.OK, "text/plain", "{\"name\":\"camera.stopPreview\", \"state\":\"done\"}");

            } else {
                return newFixedLengthResponse(Response.Status.BAD_REQUEST, "text/plain", "Invalid command is issued");
            }

        } catch (JSONException e) {
            e.printStackTrace();
            return newFixedLengthResponse(Response.Status.BAD_REQUEST, "text/plain", e.getMessage());
        }
    }

    private Response serveFrame(int resizeWidth, int quality) {
        byte[] frame;

        frame = mCallback.getLatestFrame();
        if (frame==null) {
            frame = setBlackJpegByteArray(previewFormatNo);
        }

        if (resizeWidth<2) {
            InputStream is = new ByteArrayInputStream( frame );
            return newChunkedResponse(Response.Status.OK, "image/jpeg", is);
        } else {
            Bitmap bitmap = BitmapFactory.decodeByteArray(frame, 0, frame.length);
            Bitmap resizeBmp = Bitmap.createScaledBitmap(bitmap, resizeWidth,  (resizeWidth/2), true);

            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            resizeBmp.compress(Bitmap.CompressFormat.JPEG, quality, baos);
            byte[] newFrame = baos.toByteArray();

            InputStream is = new ByteArrayInputStream( newFrame );
            return newChunkedResponse(Response.Status.OK, "image/jpeg", is);
        }
    }

    private Response serveAssetFiles(String uri) {
        if (uri.equals("/")){
            uri = "/index.html";
        }

        ContentResolver contentResolver = context.getContentResolver();
        String mimeType = contentResolver.getType(Uri.parse(uri));
        String filePath = uri.replaceAll("(^\\/)(.+)","$2");

        try {
            AssetManager assetManager = context.getAssets();
            InputStream is = assetManager.open(filePath);
            return newChunkedResponse(Response.Status.OK, mimeType, is);
        } catch (IOException e ) {
            e.printStackTrace();
            return newFixedLengthResponse(Response.Status.INTERNAL_ERROR, "text/plain", e.getMessage());
        }
    }

    private Response serveOsc(Method method, String uri, String strJson) {
        if (strJson==null) {
            strJson ="{}";
        }

        try {
            JSONObject command = new JSONObject(strJson);
            String name = "";
            if (command.isNull("name")) {
                name ="";
            } else {
                name = command.getString("name");
            }

            if (name.equals("camera.getLivePreview")) {
                //Prohibit the use of the original getLivePreview.
                return newFixedLengthResponse(Response.Status.BAD_REQUEST, "text/plain", "Invalid command is issued");
            } else {
                if (name.equals("camera.takePicture") || name.equals("camera.startCapture")) {
                    mCallback.execStopPreview();
                }

                HttpConnector camera = new HttpConnector("127.0.0.1:8080");
                String strResult = "";
                strResult = camera.httpExec( method.toString(), uri, strJson );
                try {
                    InputStream is = new ByteArrayInputStream( strResult.getBytes("utf-8") );
                    return newChunkedResponse(Response.Status.OK, "text/html", is);
                } catch (IOException e ) {
                    e.printStackTrace();
                    return newFixedLengthResponse(Response.Status.INTERNAL_ERROR, "text/plain", e.getMessage());
                }
            }

        } catch (JSONException e) {
            e.printStackTrace();
            return newFixedLengthResponse(Response.Status.BAD_REQUEST, "text/plain", e.getMessage());
        }
    }

    private byte[] setBlackJpegByteArray(int previewFormatNo) {
        int width;
        switch (previewFormatNo){
            case GetLiveViewTask.FORMAT_NO_1920_8FPS :
                width = 1920;
                break;
            case GetLiveViewTask.FORMAT_NO_1024_30FPS :
            case GetLiveViewTask.FORMAT_NO_1024_8FPS :
                width = 1024;
                break;
            case GetLiveViewTask.FORMAT_NO_640_30FPS :
            case GetLiveViewTask.FORMAT_NO_640_8FPS :
                width = 640;
                break;
            default:
                width = 1024;
                break;
        }
        Bitmap blackBmp = Bitmap.createBitmap(width, width/2, Bitmap.Config.ARGB_8888);
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        blackBmp.compress(Bitmap.CompressFormat.JPEG, 100, baos);

        return baos.toByteArray();
    }

    /**
     * Interface of Callback.
     */
    public interface Callback {
        void execStartPreview(int format);
        void execStopPreview();
        boolean execGetPreviewStat();
        byte[] getLatestFrame();
    }

}
