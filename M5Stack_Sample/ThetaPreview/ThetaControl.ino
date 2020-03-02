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


//-------------------------------------------
// HTTP protocol functions
//-------------------------------------------
String  HttpPostGetString(const char* sPostGet, char* sUrl, String strData, unsigned int uiTimeoutSec)
{
  int           iReadSize;
  String        strJson = String("") ;
  char          caJsonBuff[4096];

  iReadSize = HttpPostGet(sPostGet, sUrl, strData, (unsigned char*)caJsonBuff, 4096, uiTimeoutSec);
  if ( iReadSize > 0 ) {
    caJsonBuff[iReadSize] = 0x00;
    strJson = String(caJsonBuff);
    //Serial.println("receive : JSON[" + strJson + "], Length:" + String(iReadSize));
  }
  return strJson;
}

int     HttpPostGet(const char* sPostGet, char* sUrl, String strData, unsigned char* pucaBuff, int iBuffSize, unsigned int uiTimeoutSec)
{
  int           iReadSize=0;
  unsigned long ulStartMs;
  unsigned long ulCurMs;
  unsigned long ulElapsedMs;
  unsigned long ulTimeoutMs;
  
  //Serial.printf("[%s],URL[%s],",sPostGet, sUrl, );  //debug 
  //Serial.println("str[" + strData + "]"); //debug

  ulTimeoutMs = (unsigned long)uiTimeoutSec * 1000;
  client.flush();   //clear response buffer

  //Use WiFiClient class to create TCP connections
  if (!client.connect(sHost, iHttpPort)) {
    Serial.println("connection failed." );
    delay(1000);
    WiFi.disconnect(true);
    return -1;
  }
  
  int status_code;
  String strMethod = String(sPostGet);
  String strHost = String(sHost);
  HTTPClient http;
  http.begin( strHost, iHttpPort, String(sUrl) );
  if ( strMethod.equals("POST") ) {    
    //Serial.println("http.POST:[" + strData + "]" );
    http.addHeader("Content-Type", "application/json;charset=utf-8");
    status_code = http.POST(strData);    
  } else {
    status_code = http.GET();    
  }

  ulStartMs = millis();
  
  if (status_code == HTTP_CODE_OK) {
    int timeoutCnt=0;
    int chunkSize = 0;
    int chunkBufPos=0;
    char chunkBuf[1024];

    int chunkJudg = http.getSize();
    //Serial.println("chunkJudg:[" + String(chunkJudg) + "]" ); //debug
    if ( chunkJudg == -1 ) { // chunked
      chunkSize = 0;
      chunkBufPos = 0;
    } else { // not chunked
      chunkSize = chunkJudg;
      chunkBufPos = -1;
    }
    WiFiClient *stream = http.getStreamPtr();
    while(http.connected() && (chunkJudg > 0 || chunkJudg == -1)){
      size_t size = stream->available();
      if (size) {
        timeoutCnt=0;
        
        if (0<=chunkBufPos) {
          chunkBuf[chunkBufPos] = stream->read();
          chunkBufPos++;
          if ( chunkBuf[(chunkBufPos-1)] == '\n' ) {
            chunkBuf[chunkBufPos]=0x00;
            String sHexLenStr = String(chunkBuf);
            sHexLenStr.trim();
            chunkSize = (int)strtol(chunkBuf, 0, 16);
            if ( chunkBufPos==2 ) {
              //Serial.println("chunk End : Hex Str = 0x" + sHexLenStr +"[hex], HexStrLen=" + String(chunkBufPos) + ", dec=" + String(chunkSize)); //debug
              chunkBufPos=0;
            } else if ( chunkSize == 0 ) {
              //Serial.println("Data End : Hex Str = 0x" + sHexLenStr +"[hex], HexStrLen=" + String(chunkBufPos) + ", dec=" + String(chunkSize)); //debug
              break;
            } else {
              //Serial.println("read chunk Size : Hex Str = 0x" + sHexLenStr +"[hex], HexStrLen=" + String(chunkBufPos) + ", dec=" + String(chunkSize)); //debug
              chunkBufPos=-1;
            }
          }
        } else {
          if( (iReadSize+size) > iBuffSize ) {
            Serial.println("buffer OVF !");
            break;
          }
          int iBufPos = stream->readBytes( (pucaBuff+iReadSize), chunkSize );
          iReadSize += iBufPos;
          //Serial.println("cur read size:[" + String(iReadSize) + "]" ); //debug
          chunkSize -= iBufPos;
          if ( chunkJudg == -1 ) {
            if ( chunkSize <= 0 ) {
              chunkSize=0;
              chunkBufPos=0;//switch
            }
          } else {
            if ( chunkSize <= 0 ) {
              break;
            }
          }
        }
      } else {
        ulCurMs = millis();
        ulElapsedMs = calcDiffTime(ulStartMs, ulCurMs);
        if ( (ulTimeoutMs!=0) && (ulElapsedMs>ulTimeoutMs) ) {
          break;
        }
      }
    }
    
  } else {
    Serial.println("HTTP status_code:[" + String(status_code) + "]" );
    if ( status_code < 0 ) {
      return -1;
    }
  }
  http.end();
  
  ulCurMs = millis();
  ulElapsedMs = calcDiffTime(ulStartMs, ulCurMs);
  Serial.println( "HTTP response read time=" + String(ulElapsedMs) + "[ms], len=" + String(iReadSize) );
  
  return iReadSize;
}


//-------------------------------------------
// THETA API functions
//-------------------------------------------
int     ThetaAPI_Post_State(void)
{
  int iRet=0;

  String strJson = HttpPostGetString("POST", (char*)sUrlState, "", HTTP_TIMEOUT_STATE );  
  //Serial.println("ThetaAPI_Post_State() : " + strJson);
  
  iRet = strJson.length();
  if ( iRet != 0 ) {
    char sJson[iRet+1];
    strJson.toCharArray(sJson,iRet+1);
    DynamicJsonDocument root(350);
    DeserializationError error = deserializeJson(root, sJson);
    if (error) {
      Serial.println("ThetaAPI_Post_State() : deserializeJson() failed.: inStr[" + strJson + "], error=[" + error.c_str() + "]");    
      iRet=-1;
    } else {

      double dBattLevel = root["state"]["batteryLevel"];
      iBattLevel = dBattLevel * 100;
      
      const char* _captureStatus  = root["state"]["_captureStatus"];
      strCaptureStatus  = String(_captureStatus);

      iRecordedTime = root["state"]["_recordedTime"];
      iRecordableTime = root["state"]["_recordableTime"];
      
      if ( strCaptureStatus.equals("idle") ) {
        iMoveStat = MOVE_STAT_STOP;
        iIntExpStat = INT_EXP_STAT_STOP;
      } else {
        if ( (iRecordedTime==0) && (iRecordableTime==0) ) {
          iMoveStat = MOVE_STAT_STOP;
          iIntExpOnOff= INT_EXP_ON;
          iIntExpStat = INT_EXP_STAT_RUN ;
        } else {
          iMoveStat = MOVE_STAT_REC;
          iIntExpStat = INT_EXP_STAT_STOP;
        }
      }
      iRet=1;
    }
  }
  
  return iRet;
}


int     ThetaAPI_Post_takePicture(void)
{
  int iRet=0;
  
  iTakePicStat = TAKE_PIC_STAT_BUSY;
  
  String strSendData = String("{\"name\": \"camera.takePicture\" }");
  String strJson = HttpPostGetString("POST", (char*)sUrlCmdExe, strSendData, HTTP_TIMEOUT_NORMAL );
    
  iRet = strJson.length();
  if ( iRet != 0 ) {
    char sJson[iRet+1];
    strJson.toCharArray(sJson,iRet+1);
    DynamicJsonDocument root(200);
    DeserializationError error = deserializeJson(root, sJson);
    if (error) {
      Serial.println("ThetaAPI_Post_takePicture() : deserializeJson() failed.: inStr[" + strJson + "], error=[" + error.c_str() + "]");    
      iRet=-1;
    } else {
      const char* sState  = root["state"];
      String strState = String(sState);
      //Serial.print("ThetaAPI_Post_takePicture() : state[" + strState + "], " );
      if ( strState.equals("error") ) {
        const char* sErrorCode = root["error"]["code"];
        const char* sErrorMessage = root["error"]["message"];
        //Serial.println("Code[" + String(sErrorCode) + "], Message[" + String(sErrorMessage) + "]");
        iTakePicStat = TAKE_PIC_STAT_DONE;
        iRet=-1;
      } else {  //inProgress
        const char* sId = root["id"];
        strTakePicLastId = String(sId);
        //Serial.println("id[" + strTakePicLastId + "]");
        iRet=1;
      }
    }
  }
  
  return iRet;
}


int     ThetaAPI_Post_commnads_status(void)
{
  int iRet=0;
  if ( strTakePicLastId == "0" ) {
    return iRet;
  }
  
  String strSendData = "{\"id\":\"" + strTakePicLastId + "\"}" ;
  String strJson = HttpPostGetString("POST", (char*)sUrlCmdStat, strSendData, HTTP_TIMEOUT_CMDSTAT );
  String strDbg = "";
  
  iRet = strJson.length();
  if ( iRet != 0 ) {
    char sJson[iRet+1];
    strJson.toCharArray(sJson,iRet+1);
    DynamicJsonDocument root(200);
    DeserializationError error = deserializeJson(root, sJson);
    if (error) {
      Serial.println("ThetaAPI_Post_commnads_status() : deserializeJson() failed.: inStr[" + strJson + "], error=[" + error.c_str() + "]");    
      iRet=-1;
    } else {
      const char* sState  = root["state"];
      String strState = String(sState);
      if ( strState.equals("error") ) {
        const char* sErrorCode = root["error"]["code"];
        const char* sErrorMessage = root["error"]["message"];
        strDbg = "Code[" + String(sErrorCode) + "], Message[" + String(sErrorMessage) + "]";
        iTakePicStat = TAKE_PIC_STAT_DONE;
        iRet=-1;
      } else if ( strState.equals("done") ) {
        const char* sFileUri = root["results"]["fileUri"];
        strDbg = "done : fileUri[" + String(sFileUri) + "]";
        iTakePicStat = TAKE_PIC_STAT_DONE;        
        iRet=1;
      } else {  // inProgress
        const char* sId = root["id"];
        strDbg = "inProgress : id[" + String(sId) + "]";
        iRet=1;
      }
    }
  }
  
  return iRet;
}
