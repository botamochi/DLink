//======================================================================
// ライブラリ使用例2
// ランダムな相手とバトル(Arduino側が必ず負ける)
// バトルの仕様は初代のもの
// PCのシリアルモニタから任意の文字を送信するとバトル開始
// 2015/1/25 作成
// 2020/6/28 DLinkOriginクラスを使用
//======================================================================
#include <DLink.h>

DLinkOrigin dlink(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  DataSetOrigin senddata, recvdata;
  // PCから任意の文字を受信
  while (Serial.available() < 1);
  Serial.read();
  // 対戦相手をランダムに生成
  randomSeed(millis());
  senddata.field.species = (unsigned char)random(3, 15);
  senddata.field.effort  = 0;
  senddata.field.victory = DLinkOrigin::LOSE;
  senddata.field.version = DLinkOrigin::VER1;
  // 育成ギアと通信
  recvdata = dlink.battle(senddata);
  // 受信フレームをPCに送信
  Serial.println("===== Battle Success =====");
  Serial.print("frame[0] : 0b");
  Serial.println(recvdata.frame[0], BIN);
  Serial.print("frame[1] : 0b");
  Serial.println(recvdata.frame[1], BIN);
}
