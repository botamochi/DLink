//--------------------------------------------------
// デジモンギアとの通信
// DLink.cpp
// 2015/1/23 作成
//--------------------------------------------------
#include "Arduino.h"
#include "DLink.h"

// コンストラクタ
DLink::DLink(int inputPin, int outputPin)
  : pIn(inputPin), pOut(outputPin)
{
  pinMode(pIn, INPUT);
  pinMode(pOut, OUTPUT);
  digitalWrite(pOut, HIGH);
  isBegin = false;
}

// 受信
// 引数:Startパルスを受信する際のタイムアウト(ms)
// 失敗すると0を返す
dframe DLink::receive(unsigned int timeout)
{
  dframe frame = 0;
  unsigned long tm, thr;

  // Start
  if (timeout == 0) {
    while (digitalRead(pIn) == HIGH);
  } else {
    while (digitalRead(pIn) == HIGH) {
      delay(1);
      timeout--;
      if (timeout == 0) return 0;
    }
  }
  // Head
  if ((thr = pulseIn(pIn, HIGH, 100000)) == 0) return 0;
  // Data
  for (int i = 0; i < 16; i++) {
    if ((tm = pulseIn(pIn, HIGH, 8000)) == 0) return 0;
    if (tm > thr) frame |= (1<<i);
  }
  while (digitalRead(pIn) == LOW);
  return frame;
}

// 送信開始
// Startパルスを送信する
// 先に送信しておくことで60msの間に処理が行える
void DLink::begin()
{
  if (isBegin == true) return;
  digitalWrite(pOut, LOW);
  isBegin = true;
  beginTime = millis();
}

// 送信
void DLink::send(dframe frame)
{
  // Start
  begin();
  while (millis() - beginTime < 60);
  // Head
  digitalWrite(pOut, HIGH);
  delayMicroseconds(2000);
  digitalWrite(pOut, LOW);
  delayMicroseconds(1000);
  // Data
  for (int i = 0; i < 16; i++) {
    if (frame & (1<<i)) {
      digitalWrite(pOut, HIGH);
      delayMicroseconds(2500);
      digitalWrite(pOut, LOW);
      delayMicroseconds(1500);
    } else {
      digitalWrite(pOut, HIGH);
      delayMicroseconds(1000);
      digitalWrite(pOut, LOW);
      delayMicroseconds(3000);
    }
  }
  digitalWrite(pOut, HIGH);
  isBegin = false;
  delayMicroseconds(1000);
}
