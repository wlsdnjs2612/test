
#ifndef _HWCONTROL_H
#define _HWCONTROL_H

#include "if.h"
#include "sysdeclar.h"
#include "pgcompiler.h"
#include "tgcompiler.h"
#include "scramble.h"

#include <pmbus.h>
#include <psmanager.h>

#define SET_QUEUE(a)	setQueue(a)
#define _v2bit(v, r)	(unsigned short) ((v) == (r) ? 0xfff : ((double) (v) / (r) * 4096.0))
#define _bit2v(s, r)	(double) (((double) (s)) * ((r) / 4096.0))

class HWControl : public CIF
{
public:
    HWControl();
    virtual ~HWControl();

    int send_data(unsigned int cmd, const void *data, unsigned int len, unsigned int start_addr);
    int Send(unsigned int cmd, const void *data, unsigned int len);
    int printf(const char *format, ...);

    virtual void LIBRARY_TYPE(unsigned int type);
    void SET_PS_COMMON(bool b);
    void SET_PS_COMMON(int ps1, int ps2 = 0);
    //void SET_PS_COMMON(int ps, ...);
    int sysDelay(double s);
    int sysGetPGRunTime();
    void sysWrite(unsigned int add, unsigned short data);
    unsigned short sysRead(unsigned int add);
    void setVloop(unsigned int i, unsigned int value);
    void setVector(unsigned int i, unsigned int vec);
    void setVectorEmul(unsigned int i, unsigned int vec);
    void setScanVector(unsigned int i, unsigned int scan);
    void setScanMode(bool mode, unsigned int scan = 0);
    void setScanMode2(unsigned int bit);
    void setScan(unsigned int scan);
    void setPDS(int ch = -1, int pin = 0);
    void setLevel(unsigned short ch);
    void resetLevel(unsigned short ch);
    void setDriver(unsigned short board);
    void resetDriver(unsigned short board);
    int readFault(int board);
    int readFault();
    int readPSIndex(int ps);
    void sysResetLatch();
    void setFM(int mode, int size = 0,int nbit = 0);
    void setFMClear();
    int getAddress(bool scan,int addr = -1);
    int getFailDevice(int board, int *nfail);
    void getFailData(int board, int device, int page, unsigned short address[8], unsigned char data[8]);
    void setReadyBusy(int bd, unsigned short busy_code);
    unsigned short getMux(int which);
    unsigned long long sysReadComp(int board, bool db = false);
    unsigned long long sysReadLatch(int dut, bool db = false);
    void setScanLoop(int loop);
    void setExtLoop(int loop);
    void setDAC(int ch, double volt);
    double measVoltage(int ch);
    double measVoltage(int board, int  ch);
    bool startPattern(PGCompiler &pattern, const char *name, int xa = 0, int ya = 0, int da = 0, char sa = 0);
    void writePattern(PGCompiler &pattern);
    void readPattern(PGCompiler &pattern);
    void sysStopPattern();
    void writeTimeset(TGCompiler &timeset);
    //	void readTimeset(TGCompiler &timeset);
    void writeScramble(Scramble &scramble);
    void writeScramble(unsigned short *xscr, unsigned short *yscr, unsigned int *dscr);
    void readScramble(Scramble &scramble);
    void readScramble(unsigned short *xscr, unsigned short *yscr, unsigned int *dscr);
    int sysReadPGStatus(unsigned short &flag);
    void sysPGContinue();
    bool isPowerOff();
    void sysMuxMode(int mode);
    void sysFMMode(bool set);
    void scanInv(unsigned int bit);
    void setFault(bool fault);
    void setFMSel(bool xy);
    void setClkInv(int);
    int  m_bdbit;
    unsigned int  m_fault[8];
    CPSManager	   m_PSManager;

protected:
    bool m_diagnostic;
    bool m_common;
    bool m_isnew;
    bool m_is1540;
    int m_sockfd;
    int m_sysModel;
    unsigned int m_lib_type;
    unsigned int m_scan_mode;

private:
    int m_last_pc;
};

#endif
