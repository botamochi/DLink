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
  unsigned long tm1, tm2, thr;

  // リセットパルス
  while (digitalRead(pIn) == HIGH);
  if (edge(RISING, 100) == false) return 0;
  // スタートパルス
  tm1 = micros();
  if (edge(FALLING, 8) == false) return 0;
  tm2 = micros();
  thr = tm2 - tm1; // 基準値
  if (edge(RISING, 8) == false) return 0;
  // データパルス
  for (int i = 0; i < 16; i++) {
    tm1 = micros();
    if (edge(FALLING, 8) == false) return 0;
    tm2 = micros();
    if (tm2 - tm1 > thr) frame |= (1<<i);
    if (edge(RISING, 8) == false) return 0;
  }
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

// エッジ検出
// 立ち上がりと立ち下がりのみ検出
bool DLink::edge(int mode, unsigned long timeout)
{
  int val;
  unsigned long tm = millis();
  
  if (mode == RISING) {
    val = LOW;
  } else if (mode == FALLING) {
    val = HIGH;
  } else {
    return false;
  }
  while (digitalRead(pIn) == val) {
    if (millis() - tm >= timeout) return false;
  }
  return true;
}
