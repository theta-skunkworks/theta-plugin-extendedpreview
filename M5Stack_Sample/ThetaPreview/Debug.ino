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


unsigned long calcDiffTime(unsigned long ulBeforTime, unsigned long uCurTime)
{
  unsigned long ulResult;
  if ( uCurTime > ulBeforTime ) {
    ulResult = uCurTime - ulBeforTime;
  } else {
    ulResult = (4294967295 - ulBeforTime) + uCurTime + 1 ;
  }
  return ulResult;
}

void  Debug_OutputInFps(void)
{
  unsigned long ulCurMs = millis();
  unsigned long ulElapsedMs = calcDiffTime(ulInFpsCntStart, ulCurMs);
  if ( ulElapsedMs >= 1000 ) {
    float fInFps = (float)(iInFps*1000)/(float)ulElapsedMs;
    Serial.printf( "###################### Rcv Frame : %2.1f[fps]\n", fInFps);
    iInFps=0;
    ulInFpsCntStart = millis();
  }
}

void  Debug_OutputOutFps(void)
{
  unsigned long ulCurMs = millis();
  unsigned long ulElapsedMs = calcDiffTime(ulOutFpsCntStart, ulCurMs);
  if ( ulElapsedMs >= 1000 ) {
    float fOutFps = (float)(iOutFps*1000)/(float)ulElapsedMs;
    Serial.printf( "********************** Disp Frame : %2.1f[fps]\n", fOutFps);
    iOutFps=0;
    ulOutFpsCntStart = millis();
  }
}
