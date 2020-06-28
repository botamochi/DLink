//======================================================================
// ライブラリ使用例2
// ランダムな相手とバトル(Arduino側が必ず負ける)
// バトルの仕様は初代のもの
// PCのシリアルモニタから任意の文字を送信するとバトル開始
// 2015/1/25 作成
// 2020/6/28 入出力同一ピン使用に変更
//======================================================================
#include <DLink.h>

DLink gear(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  dframe dat[2];
  // PCから任意の文字を受信
  while (Serial.available() < 1);
  Serial.read();
  // 対戦相手をランダムに生成
  randomSeed(millis());
  word pos = (word)random(3, 15);
  // 育成ギアと通信
  gear.send((~pos << 8) | pos);
  dat[0] = gear.receive();
  gear.send(0b1111110100000010);
  dat[1] = gear.receive();
  // 受信フレームをPCに送信
  Serial.println(dat[0], BIN);
  Serial.println(dat[1], BIN);
  Serial.println();
}
