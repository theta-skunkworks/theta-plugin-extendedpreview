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


int     updateLivePreview(void)
{
  int iRet=0;

  volatile int iWP = pstLvBuff->iWP;
  volatile int iTmpSize = pstLvBuff->staLvFrame[iWP].iSize;      
  if( iTmpSize == 0 ) {
    int iSize = ExtAPI_Post_getPreview(iPreviewQuality, pstLvBuff->staLvFrame[iWP].ucaBuff, LV_BSIZE_MAX);
    if (iSize>0) {
      pstLvBuff->staLvFrame[iWP].iSize = iSize;
      iWP++;
      if (iWP>=LV_FRAME_BUF_NUM) {
        iWP=0;
      }
      pstLvBuff->iWP = iWP;
      iRet=1;
    } else if ( iSize < 0 ) {
      Serial.println( "HTTP client errors");
      iRet = -1;
    } else {
      //NOP
    }
  }
  
  return iRet;
}

int     DisplayLivePreview(void)
{
  int iRet=0;
  
  volatile int iRP = pstLvBuff->iRP;
  volatile int iTmpSize;
  iTmpSize = pstLvBuff->staLvFrame[iRP].iSize;
  if ( iTmpSize != 0 ) {
    
    M5.Lcd.drawJpg( pstLvBuff->staLvFrame[iRP].ucaBuff, iTmpSize );
    pstLvBuff->staLvFrame[iRP].iSize = 0;
    iRP++;
    if (iRP>=LV_FRAME_BUF_NUM) {
      iRP=0;
    }
    pstLvBuff->iRP = iRP;
    iRet=1;
    
  }

  return iRet;
}


int     ExtAPI_Post_getPreviewStat(void)
{
  int iRet=0;
  
  String strSendData = "{\"name\":\"camera.getPreviewStat\"}" ;
  String strJson = HttpPostGetString("POST", (char*)sUrlExtCmdExe, strSendData, HTTP_TIMEOUT_NORMAL );

  iRet = strJson.length();
  if ( iRet != 0 ) {
    char sJson[iRet+1];
    strJson.toCharArray(sJson,iRet+1);
    DynamicJsonDocument root(200);
    DeserializationError error = deserializeJson(root, sJson);
    if (error) {
      Serial.println("deserializeJson() failed.: inStr[" + strJson + "], error=[" + error.c_str() + "]");    
      iRet=-1;
    } else {
      const char* sState  = root["state"];
      String strState = String(sState);
      if ( strState.equals("error") ) {
        Serial.println("camera.startPreview:" + strJson);
        iRet=-1;
      } else if ( strState.equals("done") ) {
        const char* sResults  = root["results"];
        String strResult = String(sResults);
        Serial.println("camera.getPreviewStat : results=[" + strResult + "]");
        if ( strResult.equals("on") ) {
          iRet=1;
        } else {
          iRet=0;
        }
      } else {
        Serial.println("camera.startPreview:" + strJson);
        iRet=-1;
      }
    }
  } else {
    Serial.println("JSON string length = 0");
    iRet=-1;
  }
  
  return iRet;
}

int     ExtAPI_Post_startPreview(void)
{
  int iRet=0;
  
  String strSendData = "{\"name\":\"camera.startPreview\", \"parameters\":{\"formatNo\":2}}" ;
  String strJson = HttpPostGetString("POST", (char*)sUrlExtCmdExe, strSendData, HTTP_TIMEOUT_NORMAL );

  iRet = strJson.length();
  if ( iRet != 0 ) {
    char sJson[iRet+1];
    strJson.toCharArray(sJson,iRet+1);
    DynamicJsonDocument root(200);
    DeserializationError error = deserializeJson(root, sJson);
    if (error) {
      Serial.println("deserializeJson() failed.: inStr[" + strJson + "], error=[" + error.c_str() + "]");    
      iRet=-1;
    } else {
      const char* sState  = root["state"];
      String strState = String(sState);
      if ( strState.equals("error") ) {
        Serial.println("camera.startPreview:" + strJson);
        iRet=-1;
      } else if ( strState.equals("done") ) {
        iRet=0;
      } else {
        Serial.println("camera.startPreview:" + strJson);
        iRet=-1;
      }
    }
  } else {
    Serial.println("JSON string length = 0");
    iRet=-1;
  }
  
  return iRet;
}

int     ExtAPI_Post_stopPreview(void)
{
  int iRet=0;
  
  String strSendData = "{\"name\":\"camera.stopPreview\"}" ;
  String strJson = HttpPostGetString("POST", (char*)sUrlExtCmdExe, strSendData, HTTP_TIMEOUT_NORMAL );
  
  iRet = strJson.length();
  if ( iRet != 0 ) {
    char sJson[iRet+1];
    strJson.toCharArray(sJson,iRet+1);
    DynamicJsonDocument root(200);
    DeserializationError error = deserializeJson(root, sJson);
    if (error) {
      Serial.println("deserializeJson() failed.: inStr[" + strJson + "], error=[" + error.c_str() + "]");    
      iRet=-1;
    } else {
      const char* sState  = root["state"];
      String strState = String(sState);
      if ( strState.equals("error") ) {
        Serial.println("camera.startPreview:" + strJson);
        iRet=-1;
      } else if ( strState.equals("done") ) {
        iRet=0;
      } else {
        Serial.println("camera.startPreview:" + strJson);
        iRet=-1;
      }
    }
  } else {
    Serial.println("JSON string length = 0");
    iRet=-1;
  }
  
  return iRet;
}

int     ExtAPI_Post_getPreview(int previewQuality, unsigned char* pucaLvBuff, int iLvBuffSize)
{  
  String  strSendData = "{\"name\":\"camera.getPreviewFrame\", \"parameters\":{\"resizeWidth\":320, \"quality\":" + String(previewQuality) + "}}" ;
  int     iReadSize = HttpPostGet("POST", (char*)sUrlExtCmdExe, strSendData, pucaLvBuff, iLvBuffSize, 1000);
  return  iReadSize;
}
