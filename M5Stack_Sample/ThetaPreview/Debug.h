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

#ifndef DEBUG_H
#define DEBUG_H


#define DEBUG_PIN_1 21
#define DEBUG_PIN_2 22

int iInFps=0;
unsigned long ulInFpsCntStart;
int iOutFps=0;
unsigned long ulOutFpsCntStart;

unsigned long calcDiffTime(unsigned long ulBeforTime, unsigned long uCurTime);
void  Debug_OutputInFps(void);
void  Debug_OutputOutFps(void);



#endif  //DEBUG_H
