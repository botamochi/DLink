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
// 失敗すると0を返す
dframe DLink::receive()
{
  dframe frame = 0;
  unsigned long tm, thr;

  // リセットパルス
  while (digitalRead(pIn) == HIGH);
  // スタートパルス
  if ((thr = pulseIn(pIn, HIGH, 100000)) == 0) return 0;
  // データパルス
  for (int i = 0; i < 16; i++) {
    if ((tm = pulseIn(pIn, HIGH, 8000)) == 0) return 0;
    if (tm > thr) frame |= (1<<i);
  }
  while (digitalRead(pIn) == LOW);
  return frame;
}

// 送信開始
// リセットパルスを送信する
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
  // リセットパルス
  begin();
  while (millis() - beginTime < 60);
  // スタートパルス
  digitalWrite(pOut, HIGH);
  delayMicroseconds(2000);
  digitalWrite(pOut, LOW);
  delayMicroseconds(1000);
  // データパルス
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
