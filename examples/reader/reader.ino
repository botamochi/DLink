// 読み取ってパソコンに送る
#include <DLink.h>

DLink gear(3, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  dframe frame = gear.receive();
  Serial.println(frame, BIN);
}
