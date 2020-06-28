//======================================================================
// デジモンギアとの通信
// DLink.cpp
// 2015/1/23 作成
//======================================================================
#include "Arduino.h"
#include "DLink.h"

//--------------------------------------------------
// コンストラクタ
// inputPin  : データ読み取り用のピン
// outputPin : データ出力用のピン
//--------------------------------------------------
DLink::DLink(int inputPin, int outputPin)
  : pIn(inputPin), pOut(outputPin)
{
  pinMode(pIn, INPUT);
  pinMode(pOut, INPUT); // 送信時にOUTPUTにする
  digitalWrite(pOut, HIGH);
  isBegin = false;
}

//--------------------------------------------------
// データ読み取り
// timeout : Startパルスを受信までのタイムアウト時間(ms)
// 戻り値  : 受信したデータフレーム, 失敗した場合は0
//--------------------------------------------------
dframe DLink::receive(unsigned int timeout)
{
  dframe frame = 0;
  unsigned long tm, thr;

  //------------------------------
  // Start
  //------------------------------
  if (timeout == 0) {
    while (digitalRead(pIn) == HIGH);
  } else {
    while (digitalRead(pIn) == HIGH) {
      delay(1);
      timeout--;
      if (timeout == 0) return 0;
    }
  }
  //------------------------------
  // Head
  //------------------------------
  if ((thr = pulseIn(pIn, HIGH, 100000)) == 0) return 0;
  //------------------------------
  // Data
  //------------------------------
  for (int i = 0; i < 16; i++) {
    if ((tm = pulseIn(pIn, HIGH, 8000)) == 0) return 0;
    if (tm > thr) frame |= (1<<i);
  }
  while (digitalRead(pIn) == LOW);
  return frame;
}

//--------------------------------------------------
// 送信開始
// Startパルスを送信する
// 先に送信しておくことで60msの間に処理が行える
//--------------------------------------------------
void DLink::begin()
{
  if (isBegin == true) return;
  pinMode(pOut, OUTPUT);
  digitalWrite(pOut, LOW);
  isBegin = true;
  beginTime = millis();
}

//--------------------------------------------------
// 1フレーム送信
// frame : 送信するデータフレーム
//--------------------------------------------------
void DLink::send(dframe frame)
{
  //------------------------------
  // Start
  //------------------------------
  begin();
  while (millis() - beginTime < 60);
  //------------------------------
  // Head
  //------------------------------
  digitalWrite(pOut, HIGH);
  delayMicroseconds(2000);
  digitalWrite(pOut, LOW);
  delayMicroseconds(1000);
  //------------------------------
  // Data
  //------------------------------
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
  pinMode(pOut, INPUT);
  isBegin = false;
  delayMicroseconds(1000);
}


//--------------------------------------------------
// 初代ギアのバトル
// mode : true->自分からバトル開始, false->バトル待ち
// 戻り値 : true->通信成功, false->通信失敗
//--------------------------------------------------
DataSetOrigin DLinkOrigin::battle(bool mode, unsigned int timeout)
{
  int i;
  DataSetOrigin recvdata;

  if (mode == true) {
    dataset.field.padding1 = ~((unsigned char)dataset.field.effort << 4 | dataset.field.species);
    dataset.field.padding2 = ~((unsigned char)dataset.field.version << 4 | dataset.field.victory);
    for (i = 0; i < 2; i++) {
      send(dataset.frame[i]);
      recvdata.frame[i] = receive(timeout);
      if (recvdata.frame[i] == 0) {
        recvdata.frame[0] = 0;
        recvdata.frame[1] = 0;
        return recvdata;
      }
    }
  } else {
    dataset.field.padding1 = ~((unsigned char)dataset.field.effort << 4 | dataset.field.species);
    for (i = 0; i < 2; i++) {
      recvdata.frame[i] = receive(timeout);
      if (recvdata.frame[i] == 0) {
        recvdata.frame[0] = 0;
        recvdata.frame[1] = 0;
        return recvdata;
      }
      begin();
      if (i == 1) {
        dataset.field.victory = recvdata.field.victory == WIN ? LOSE : WIN;
        dataset.field.padding2 = ~((unsigned char)dataset.field.version << 4 | dataset.field.victory);
      }
      send(dataset.frame[i]);
    }
  }
  return recvdata;
}
