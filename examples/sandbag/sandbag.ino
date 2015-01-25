// ランダムな相手とバトル
// 必ず勝てる
#include <DLink.h>

DLink gear(3, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  dframe dat[2];
  while (Serial.available() < 1);
  Serial.read();
  randomSeed(millis());
  word pos = (word)random(3, 15);
  gear.send((~pos << 8) | pos);
  dat[0] = gear.receive();
  gear.send(0b1111110100000010);
  dat[1] = gear.receive();
  Serial.println(dat[0], BIN);
  Serial.println(dat[1], BIN);
  Serial.println();
}
