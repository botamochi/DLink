//--------------------------------------------------
// デジモンギアとの通信
// DLink.h
// 2015/1/23 作成
//--------------------------------------------------
#ifndef _DIGIMON_DLINK_H_
#define _DIGIMON_DLINK_H_

// データ型
typedef unsigned int dframe;

// DLinkクラス
class DLink
{
public:
  DLink(int inputPin, int outputPin);
  dframe receive(void);
  void   begin(void);
  void   send(dframe frame);
private:
  const int     pIn, pOut;
  bool          isBegin;
  unsigned long beginTime;
};

#endif
