//======================================================================
// デジモンギアとの通信
// DLink.h
// 2015/1/23 作成
//======================================================================
#ifndef _DIGIMON_DLINK_H_
#define _DIGIMON_DLINK_H_

//--------------------------------------------------
// データフレーム型
// 1回のデータ送信で使用する16bitのパケット
//--------------------------------------------------
typedef unsigned int dframe;

//--------------------------------------------------
// DLinkクラス
//--------------------------------------------------
class DLink
{
public:
  DLink(int ioPin) : DLink(ioPin, ioPin) {} // 入出力ピンを同じにする場合
  DLink(int inputPin, int outputPin);
  dframe receive(unsigned int timeout = 0);
  void   begin(void);
  void   send(dframe frame);
private:
  const int     pIn, pOut;
  bool          isBegin;
  unsigned long beginTime;
};

#endif
