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


//--------------------------------------------------
// 初代デジタルモンスター通信データセット
//--------------------------------------------------
typedef union DataSetOriginType
{
  dframe frame[2];
  struct {
    unsigned char species  : 4;
    unsigned char effort   : 4;
    unsigned char padding1 : 8;
    unsigned char victory  : 4;
    unsigned char version  : 4;
    unsigned char padding2 : 8;
  } field;
} DataSetOrigin;

//--------------------------------------------------
// 初代デジタルモンスター通信用クラス
//--------------------------------------------------
class DLinkOrigin : public DLink
{
public:
  enum {
    // Version1
    AGUMON         = 0b0011,
    BETAMON        = 0b0100,
    GREYMON        = 0b0101,
    TYRANOMON      = 0b0110,
    DEVIMON        = 0b0111,
    MERAMON        = 0b1000,
    SEADRAMON      = 0b1001,
    AIRDRAMON      = 0b1010,
    NUMEMON        = 0b1011,
    METALGREYMON   = 0b1100,
    MAMEMON        = 0b1101,
    MONZAEMON      = 0b1110
  };
  enum {
    VER1 = 0b0000,
    VER2 = 0b0001,
    VER3 = 0b0010,
    VER4 = 0b0011,
    VER5 = 0b0100
  };
  enum {
    WIN  = 0b0001,
    LOSE = 0b0010
  };
public:
  DLinkOrigin(int ioPin) : DLink(ioPin) {}
  DLinkOrigin(int inputPin, int outputPin) : DLink(inputPin, outputPin) {}
  DataSetOrigin battle(DataSetOrigin dataset, bool mode = true, unsigned int timeout = 0);
};

#endif
