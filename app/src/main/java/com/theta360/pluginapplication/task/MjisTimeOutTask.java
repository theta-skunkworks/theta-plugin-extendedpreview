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

import android.os.AsyncTask;
import android.util.Log;

public class MjisTimeOutTask extends AsyncTask<Void, Void, Void> {
    private static final String TAG = "MjisTimeOutTask";

    private MjisTimeOutTask.Callback mCallback;
    private long timeoutMs;
    private long timeoutCnt;

    public MjisTimeOutTask(MjisTimeOutTask.Callback callback, long timeoutMs) {
        this.mCallback = callback;
        this.timeoutMs = timeoutMs;
        timeoutCnt = 0;
    }

    @Override
    protected void onPreExecute() {
    }

    @Override
    protected Void doInBackground(Void... params) {
        while (!isCancelled()) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }

            timeoutCnt += 10;
            if ( timeoutCnt >= timeoutMs ) {
                Log.d(TAG, "#### TIME OUT ! ####");
                mCallback.onTimeoutExec();
                break;
            }
        }

        return null;
    }

    /**
     * Interface of Callback.
     */
    public interface Callback {
        void onTimeoutExec();
    }
}
