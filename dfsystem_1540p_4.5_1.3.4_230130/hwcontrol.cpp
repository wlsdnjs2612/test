
#include "hwcontrol.h"

#ifndef max
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif

HWControl::HWControl():
    CIF(),
    m_diagnostic(false),
    m_sockfd(0),
    m_sysModel(33),
    m_lib_type(0),
    m_scan_mode(0),
    m_last_pc(0)
{
    memset(m_fault, 0, sizeof(m_fault));
    m_bdbit = 0 ;
}

HWControl::~HWControl()
{
}

int HWControl::send_data(unsigned int cmd, const void *data, unsigned int len, unsigned int start_addr)
{
    int ret;
    unsigned int total;
    unsigned int buf[2] = { cmd, len };
    char *p;

    if (start_addr < 8) {
        p = (char *) buf;

        for (total = start_addr; total < 8;) {
            if ((ret = send(m_sockfd, p + total, 8 - total, MSG_NOSIGNAL)) < 0) {
                if (errno == EWOULDBLOCK || errno == EINTR) {
                    continue;
                }
                else if (errno == EPIPE) {
                    return 0;
                }

                return -total;
            }

            total += ret;
        }

        start_addr = 8;
    }

    start_addr -= 8;
    p = (char *) data;

    for (total = start_addr; total < len;) {
        if ((ret = send(m_sockfd, p + total, len - total, MSG_NOSIGNAL)) < 0) {
            if (errno == EWOULDBLOCK || errno == EINTR) {
                continue;
            }
            else if (errno == EPIPE) {
                return 0;
            }

            return -(total + 8);
        }

        total += ret;
    }

    fflush(0);
    return 1;
}

int HWControl::Send(unsigned int cmd, const void *data, unsigned int len)
{
    int ret = 0;
    int flags;

    if ((flags = fcntl(m_sockfd, F_GETFL, 0)) < 0) {
        return -1;
    }

    flags &= ~O_NONBLOCK;

    if (fcntl(m_sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }

    do {
        ret = send_data(cmd, data, len, -ret);
    } while (ret < 0);

    if (ret == 0) {
        return 0;
    }

    if (fcntl(m_sockfd, F_SETFL, flags) < 0) {
        return -1;
    }

    return 1;
}

int HWControl::printf(const char *format, ...)
{
    char buf[BUFSIZ] = { 0 };
    va_list arg;
    int ret;

    /*	if (m_diagnostic == true) {
                va_start(arg, format);
                ret = vprintf(format, arg);
                va_end(arg);
        }
        else {*/
    va_start(arg, format);
    ret = vsnprintf(buf, sizeof(buf), format, arg);
    va_end(arg);
    Send(BC_PRINTF, buf, strlen(buf));
    //	}

    return ret;
}

void HWControl::LIBRARY_TYPE(unsigned int type)
{
    m_lib_type = type;
}

int HWControl::sysDelay(double s)
{
    unsigned long long us = (unsigned int) (s / 0.000001);
    return Delay(us);
}

int HWControl::sysGetPGRunTime()
{
    int runtime;
    runtime = Read(0x4005);
    runtime |= Read(0x4006) << 16;
    return runtime;
}

void HWControl::sysWrite(unsigned int add, unsigned short data)
{
    Write(add, data);
}

unsigned short HWControl::sysRead(unsigned int add)
{
    return Read(add);
}

void HWControl::SET_PS_COMMON(bool b)
{
    //printf("SET_PS_COMMON : bool %d", b);
    if(m_isnew)
    {
        m_common = b;
        int read_v = Read(0x2) & 0x3fff;
        Write(0x2, b == true? (read_v & ~(0x3 << 14)) : (read_v | (0x3 << 14)));
    }
    else
    {
        Write(0x2, !b);
    }
}

void HWControl::SET_PS_COMMON(int ps1, int ps2)
{
    if((m_isnew == false) || (m_common == true))
    {
        return;
    }
    char msg[100];
    int icommon = 0x3;
    int read_v = Read(0x2) & 0x3fff;

    if(((ps1 != 0x3) && (ps1 != 0x5) ) || ((ps2 != 0) && (ps2 != 0x3) && (ps2 != 0x5)))
    {
        sprintf(msg, "SET_PS_COMMON ARGUMENT IS NOT VALID!! \n");
        Send(BC_RUNTIMEERROR, msg, strlen(msg) + 1);
        return;
    }

    if((ps1 == 0x3) || (ps2 == 0x3))
    {
        icommon ^= 0x1;
    }
    if((ps1 == 0x5) || (ps2 == 0x5))
    {
        icommon ^= 0x2;
    }


    icommon = read_v | (icommon << 14);
    //printf("check icommon value : 0x%x", icommon);
    Write(0x2, icommon);
}
/*
   void HWControl::SET_PS_COMMON(int ps, ...)
   {
   if((m_isnew == false) || (m_common == true) || (ps == 0) || (ps == 1))
   {
                return;
        }
        char msg[100];
        int icommon = 0x3;
        int read_v = Read(0x2) & 0x3fff;

        int p = ps;
        va_list argptr;
        va_start(argptr, ps);

        while( p > 0)
        {
                if((p != 3) && (p != 5))
                {
                        sprintf(msg, "SET_PS_COMMON ARGUMENT IS NOT VALID!! value : 0x%x \n",p);
                        Send(BC_RUNTIMEERROR, msg, strlen(msg) + 1);
                        return;
                }

                if(p == 0x3)
                {
                        icommon ^= 0x1;
                }
                if(p == 0x5)
                {
                        icommon ^= 0x2;
                }
                p = va_arg(argptr, int);

        }

        icommon = read_v | (icommon << 14);
        printf("check icommon value : 0x%x", icommon);
        Write(0x2, icommon);
}*/

void HWControl::setVloop(unsigned int i, unsigned int value)
{
    i = i > 0x1ff ? 0x1ff : i;
    Write(0x9000 + i, value & 0xffff);
}

void HWControl::setVector(unsigned int i, unsigned int vec)
{
    i = i > 0x3ffff ? 0x3ffff : i;
    Write(0x80000 + i, vec & 0xffff);
}

void HWControl::setVectorEmul(unsigned int i, unsigned int vec)
{
    Write(0x10000 + i, vec & 0xffff);
}

void HWControl::setScanVector(unsigned int i, unsigned int scan)
{
    Write(0xd800 + i, scan & 0xffff);
    Write(0xd900 + i, scan >> 16);
}

void HWControl::setScanMode(bool mode, unsigned int scan)
{
    Write(0x40c2, mode);

    if (scan) {
        Write(0x40c6, scan & 0xffff);
        Write(0x40c7, scan >> 16);
    }
}

void HWControl::setScan(unsigned int scan)
{
    Write(0x40c4, scan & 0xffff);
    Write(0x40c5, scan >> 16);
}

unsigned short HWControl::getMux(int which)
{
    if (which == 0) {
        return Read(0x402a);
    }
    else if (which == 1) {
        return Read(0x403a);
    }

    return 0;
}

void HWControl::setPDS(int ch, int pin)
{
    if (ch == -1) {
        for (int i = 0; i < 16; i++) {
            Write(0x40e0 + i, 0x302 + (i + (i / 8 * 4)) * 0x202);
        }

        return;
    }

    unsigned short data = Read(0x40e0 + ch / 2);

    if (ch & 1) {
        data &= 0xff;
        data |= pin << 8;
    }
    else {
        data &= 0xff00;
        data |= pin;
    }

    Write(0x40e0 + ch / 2, data);
}

void HWControl::setLevel(unsigned short ch)
{
    Write(0x05, ch);
}

void HWControl::resetLevel(unsigned short ch)
{
    Write(0x06, ch);
}

void HWControl::setDriver(unsigned short board)
{
    Write(0x03, board);
}

void HWControl::resetDriver(unsigned short board)
{
    Write(0x04, board);
}

int HWControl::readFault(int board)
{
	if(m_isnew)
	{ 
		unsigned int bit = 0;
		unsigned int result = 0;
		if(((m_bdbit >> board) & 0x1) == 0)
		{
			return 0;
		}
		bit = Read(0xb | board * 0x100);
		result |= (bit & 0xe) << 6;
		result |= (bit & 0x20) << 10;
		return result;

	}
	return Read(0xb | board * 0x100);
}

int HWControl::readFault()
{

    unsigned short  usStatus_vout;
    unsigned short  usStatus_iout;
    unsigned short  error_flag = 0;
    int ps_index = 0;
    int fault_flag = 0;
    int iRet = 0;
    
    memset(m_fault, 0, sizeof(m_fault));
    
    for(int ch = 0; ch < 2; ch++)
    {
	    iRet = m_PSManager.readSTATUSVOUT(ch, usStatus_vout); 
	    if(iRet != PMBUS_CMD_SUCCESS)
	    //if(m_PSManager.readSTATUSVOUT(ch, usStatus_vout) != PMBUS_CMD_SUCCESS)
	    {
		    printf("[ERROR] CH [%d] readSTATUSVOUT Error : [0x%x] %s", ch, iRet, m_PSManager.error());
		    continue;
	    }

	    for(int ps = 0; ps < 5; ps++)
	    {
		    ps_index = readPSIndex(0x1<<ps);
		    for(unsigned int islot = 0; islot < NSLOT; islot++)
		    {
			    if(((m_bdbit >> islot) & 0x1) == 0)
			    {
				    continue;
			    }
			    m_PSManager.readData(ps, islot, usStatus_vout);
			    if(usStatus_vout & 0x2)
			    {
				    printf("PS[%d] CH [%d] SLOT[%d] VOUT POWER OFF TIME FAULT!! STATUS[%02x]\n", ps_index, ch, islot + 1, usStatus_vout );
				    error_flag |= 0x1 << islot;
				    fault_flag = 0x10000;
				    m_fault[islot] |= fault_flag << ps_index;
			    }
			    if(usStatus_vout & 0x4)
			    {
				    printf("PS[%d] CH [%d] SLOT[%d] VOUT POWER ON TIME FAULT!! STATUS[%02x]\n", ps_index, ch, islot + 1, usStatus_vout );
				    error_flag |= 0x1 << islot;
				    fault_flag = 0x8000;
				    m_fault[islot] |= fault_flag << ps_index;
			    }
			    if(usStatus_vout & 0x10)
			    {
				    printf("PS[%d] CH [%d] SLOT[%d] VOUT UNDER FAULT!! STATUS[%02x]\n", ps_index, ch, islot + 1, usStatus_vout );
				    error_flag |= 0x1 << islot;
				fault_flag = ps_index < 4 ? 0x08 : 0x100;
				    m_fault[islot] |= fault_flag << ps_index;
			    }
			    if(usStatus_vout & 0x80)
			    {
				    printf("PS[%d] CH [%d] SLOT[%d] VOUT OVER FAULT!! STATUS[%02x]\n", ps_index, ch, islot + 1, usStatus_vout );
				    error_flag |= 0x1 << islot;
				    fault_flag = ps_index < 4 ? 0x1 : 0x40;
				    m_fault[islot] |= fault_flag << ps_index;
			    }
		    }
	    }

	    iRet = m_PSManager.readSTATUSIOUT(ch, usStatus_iout);
	    if(iRet != PMBUS_CMD_SUCCESS)
	    //if(m_PSManager.readSTATUSIOUT(ch, usStatus_iout) != PMBUS_CMD_SUCCESS)
	    {
		    printf("[ERROR] CH [%d] readSTATUSIOUT Error : [0x%x] %s", ch, iRet, m_PSManager.error());
		    continue;
	    }
	    
	    for(int ps = 0; ps < 5; ps++)
	    {
		    ps_index = readPSIndex(0x1<<ps);
		    for(unsigned int islot = 0; islot < NSLOT; islot++)
		    {
			    if(((m_bdbit >> islot) & 0x1) == 0)
			    {
				    continue;
			    }
			    m_PSManager.readData(ps, islot, usStatus_iout);

			    if(usStatus_iout &0x80)
			    {
				    printf("PS[%d] CH [%d] SLOT[%d] IOUT OVER FAULT!! STATUS[%02x]\n", ps_index, ch, islot + 1, usStatus_iout );
				    error_flag |= 0x1 << islot;
				    fault_flag = ps_index < 4 ? 0x40 : 0x400;
				    m_fault[islot] |= fault_flag << ps_index;
			    }
		    }
	    }
    }
	
    return error_flag;
}

int HWControl::readPSIndex(int ps)
{
    int ps_index = 0;
    switch(ps)
    {
    case 0x1:
        ps_index = 1;
        break;
    case 0x2:
        ps_index = 4;
        break;
    case 0x4:
        ps_index= 2;
        break;
    case 0x8:
        ps_index = 5;
        break;
    case 0x10:
        ps_index = 3;
        break;
    case 0x3:
	ps_index = 1;
	break;
    case 0xc:
	ps_index = 2;
    }

    return ps_index;
}

void HWControl::sysResetLatch()
{
    Write(0x2a, 0);
}

void HWControl::setFM(int mode, int size, int nbit)
{
    Write(0x22, mode);
    Write(0x23, size | (nbit << 8));
}

void HWControl::setFMClear()
{
    Write(0x3f, 0);
}

int HWControl::getAddress(bool scan, int addr)
{
    if (addr != -1) {
        Write(0x3a, addr);
    }

    if (!scan) {
        return Read(0x402f) | (Read(0x404f) << 16);
    }

    return Read(0x40cf);
}

int HWControl::getFailDevice(int board, int *nfail)
{
    int fail = 0;
    unsigned int flag;

    flag = Read(0x38 | (board << 8));
    flag |= Read(0x39 | (board << 8)) << 16;

    fail = flag & 0xf;
    flag >>= 4;

    fail |= (flag & 0xf) << 8;
    flag >>= 4;

    nfail[0] = (flag & 0x3f);
    flag >>= 6;
    nfail[1] = (flag & 0x3f);
    flag >>= 6;
    nfail[2] = (flag & 0x3f);
    flag >>= 6;
    nfail[3] = (flag & 0x3f);

    flag = Read(0x58 | board * 0x100);
    flag |= Read(0x59 | board * 0x100) << 16;

    fail |= (flag & 0xf) << 4;
    flag >>= 4;

    fail |= (flag & 0xf) << 12;
    flag >>= 4;

    nfail[4] = (flag & 0x3f);
    flag >>= 6;
    nfail[5] = (flag & 0x3f);
    flag >>= 6;
    nfail[6] = (flag & 0x3f);
    flag >>= 6;
    nfail[7] = (flag & 0x3f);

    return fail;
}

void HWControl::getFailData(int board, int device, int page, unsigned short address[8], unsigned char data[8])
{
    Write(0x3a, page);

    for (int i = 0; i < 8; i++) {
        if ((device >> i) & 1) {
            data[i] = Read(0x30 + (i % 4) * 2 + 0x20 * (i / 4) | (board << 8)) & 0xff;
//	    printf("data[%d] : %x", i, data[i]);
        }

        if ((device >> (i + 8)) & 1) {
            address[i] = Read(0x31 + (i % 4) * 2 + 0x20 * (i / 4) | (board << 8));
        }
    }
}

void HWControl::setReadyBusy(int bd, unsigned short busy_code)
{
    Write(0x24 | bd * 0x100, busy_code);

    if (busy_code) {
        Write(0x5 | bd * 0x100, 0x200);
    }
    else {
        Write(0x5 | bd * 0x100, 0);
    }
}

unsigned long long HWControl::sysReadComp(int board, bool db)
{
    unsigned long long comp = 0;

    comp = Read(0x29 + board * 0x100 + db * 0x20);
    comp <<= 32;
    comp |= Read(0x28 + board * 0x100 + db * 0x20) << 16;
    comp |= Read(0x27 + board * 0x100 + db * 0x20);

    return comp;
}

unsigned long long HWControl::sysReadLatch(int board, bool db)
{
    unsigned long long comp;

    comp = Read(0x2c + board * 0x100 + db * 0x20);
    comp <<= 32;
    comp |= Read(0x2b + board * 0x100 + db * 0x20) << 16;
    comp |= Read(0x2a + board * 0x100 + db * 0x20);

    return comp;
}

void HWControl::setScanLoop(int loop)
{
    Write(0x400e, loop);
}

void HWControl::setExtLoop(int loop)
{
    Write(0x400c, loop & 0xffff);
    Write(0x400d, loop >> 16);
}

void HWControl::setDAC(int ch, double volt)
{
    unsigned int addr[] = { 0x54c0, 0x54c1, 0x54c2, 0x54c3, 0x54c4, 0x54c5, 0x54c6, 0x54c7,
                            0x54c8, 0x54c9, 0x54ca, 0x54cb, 0x54cc, 0x54cd, 0x54ce, 0x54cf,
                            0x54d0, 0x54d1, 0x54d2, 0x54d3, 0x54d4, 0x54d5, 0x54d6, 0x54d7,
                            0x54d8, 0x54d9, 0x54da, 0x54db, 0x54dc, 0x54dd, 0x54de, 0x54df,
                            0x58c0, 0x58c1, 0x58c2, 0x58c3, 0x58c4, 0x58c5, 0x58c6, 0x58df };
    //	unsigned short v = (unsigned short)((volt + 2.5) * 0x3fff / 10);
    unsigned short v;

if(!m_isnew)
{
    if ((ch >= 0 && ch <= 2) || (ch >= 22 && ch <= 23) || ch == 20) {
        volt = volt * 10 / 11.0;
    }
    else if ((ch >= 3 && ch <= 5) || (ch >= 24 && ch <= 25) || (ch >= 14 && ch <= 17)) {
        volt = volt / 2.0;
    }
}	
else{
	if(ch == 20)
	{
		volt = volt * 10 / 11.0;
	}
	else if(ch >= 14 && ch <= 17)
	{
		volt = volt / 2.0;
	}
}

    if (ch >= (int) (sizeof(addr) / sizeof(unsigned int))) {
        return;
    }

    v = (unsigned short) ((volt + 2.5) * 0x3fff / 10);
    //printf("ch : %d volt : %f v = %d", ch, volt, v);
    Write(addr[ch], v);
    sysDelay(400 US);
}

double HWControl::measVoltage(int ch)
{
    unsigned short read;
    double voltage;

    Write(0x40f2, ch);
    sysDelay(400 US);
    Write(0x40f3, 0);
    sysDelay(100 US);
    read = Read(0x40f4) & 0xffff;
    voltage = ((read ^ 0x8000) * 20.0 / 65535.0) - 10.0;
    return voltage;
}

double HWControl::measVoltage(int board, int ch)
{
    Write(0x7, (board << 8) | ch);
    return measVoltage(0x35);
}

bool HWControl::startPattern(PGCompiler &pattern, const char *name, int xa, int ya, int da, char sa)
{
    patternList *pPatternList = pattern.patternData();

    for (POSITION pos = pPatternList->GetHeadPosition(); pos;) {
        TPGDATA *pg = pPatternList->GetNext(pos);

        if (strcmp(name, pg->name)) {
            continue;
        }

        //	Write(0x40c2, pg->scan_use);

        Write(0xa000, 1);
        Write(0xa400, xa & 0xffff);
        Write(0xa800, xa >> 16);
        Write(0xb000, 1);
        Write(0xb400, ya & 0xffff);
        Write(0xb800, ya >> 16);

        Write(0xc000, 4 | ((da >> 16) & 3));
        Write(0xc400, da);
        Write(0xd400, 0x100 | sa);

        Write(0x4030, (pg->minmax.xmin >> 16) | ((pg->minmax.xmax >> 8) & 0xff00));
        Write(0x403d, ((pg->minmax.vmin >> 16) & 0x3) | ((pg->minmax.vmax >> 8) & 0x300));
        Write(0x4050, (pg->minmax.ymin >> 16) | ((pg->minmax.ymax >> 8) & 0xff00));

        Write(0x4023, pg->minmax.xmin & 0xffff);
        Write(0x4024, pg->minmax.xmax & 0xffff);
        Write(0x403b, pg->minmax.vmin & 0xffff);
        Write(0x403c, pg->minmax.vmax & 0xffff);
        Write(0x4043, pg->minmax.ymin & 0xffff);
        Write(0x4044, pg->minmax.ymax & 0xffff);

        Write(0x4063, (pg->minmax.dmin & 0xffff));
        Write(0x4064, (pg->minmax.dmax & 0xffff));
        Write(0x4065, (pg->minmax.dmin >> 16) | ((pg->minmax.dmax >> 14) & 0xc));
        Write(0x4083, pg->minmax.bmin | (pg->minmax.bmax << 4));
        Write(0x40C3, pg->minmax.smin | (pg->minmax.smax << 8));
        Write(0x4002, 0);

        sysDelay(1 US);
        Write(0x8000, 0x0100);

        Write(0x8400, pg->pc);

        Write(0x4002, 1);  // Power UP
        sysDelay(1 US);
        Write(0x40c2, pg->scan_use);
        Write(0x4002, 3);  // Pg Run

        return true;
    }

    return false;
}

void HWControl::writePattern(PGCompiler &pattern)
{
    patternList *pPatternList = pattern.patternData();
    int i = 1, j = 0, pat_base = 0, pat_index = 0;
    int ram_count = 0;

    Write(0x4002, 0);
    Write(0x8800, 0x0);
    Write(0x8c00, 0x0);

    for (POSITION pos = pPatternList->GetHeadPosition(); pos;) {
        TPGDATA *pg = pPatternList->GetNext(pos);

        i = pg->pc;
        ram_count = pg->pram->GetCount() - 1;

        for (POSITION rampos = pg->pram->GetHeadPosition(); rampos; i++) {
            TRAMDATA *ram = pg->pram->GetNext(rampos);

            if (i == pg->pc) {
                ram->d.start = 1;
            }

            if (ram_count == i - pg->pc) {
                ram->d.stop = 1;
            }

            pat_base = i >= 1024 ? 0x100000 : 0;
            pat_index = i % 1024;
            Write(pat_base + 0x8000 + pat_index, ram->opcode_ts33);
            Write(pat_base + 0x8400 + pat_index, ram->jump);
            Write(pat_base + 0x8800 + pat_index, ram->l.low_count);
            Write(pat_base + 0x8c00 + pat_index, ram->l.high_count);

            Write(pat_base + 0xa000 + pat_index, ram->x);
            Write(pat_base + 0xa400 + pat_index, ram->x_operand);
            Write(pat_base + 0xa800 + pat_index, ram->x_operdir);
            Write(pat_base + 0xac00 + pat_index, ram->x_direct);
            Write(pat_base + 0x9800 + pat_index, ram->x_fm);

            Write(pat_base + 0xb000 + pat_index, ram->y);
            Write(pat_base + 0xb400 + pat_index, ram->y_operand);
            Write(pat_base + 0xb800 + pat_index, ram->y_operdir);
            Write(pat_base + 0xbc00 + pat_index, ram->y_direct);
            Write(pat_base + 0x9c00 + pat_index, ram->y_fm);

            Write(pat_base + 0xc000 + pat_index, ram->dop.d_op);
            Write(pat_base + 0xc400 + pat_index, ram->dop.d_oper);
            Write(pat_base + 0xd000 + pat_index, ram->bank);
            Write(pat_base + 0xd400 + pat_index, ram->scan);
            Write(pat_base + 0xdc00 + pat_index, ram->s_fm);

            Write(pat_base + 0xe000 + pat_index, ram->vector);
            Write(pat_base + 0xe400 + pat_index, ram->v_source);
            Write(pat_base + 0xe800 + pat_index, ram->v_srcdir);
            Write(pat_base + 0xec00 + pat_index, ram->v_direct);
        }
    }

    j = i;

    for (i = j; i < 2048; i++) {
        pat_base = i >= 1024 ? 0x100000 : 0;
        pat_index = i % 1024;
        Write(pat_base + 0x8000 + pat_index, 0);
        Write(pat_base + 0x8400 + pat_index, 0);
        Write(pat_base + 0x8800 + pat_index, 0);
        Write(pat_base + 0x8c00 + pat_index, 0);
        Write(pat_base + 0xa000 + pat_index, 0);
        Write(pat_base + 0xa400 + pat_index, 0);
        Write(pat_base + 0xa800 + pat_index, 0);
        Write(pat_base + 0xac00 + pat_index, 0);
        Write(pat_base + 0xb000 + pat_index, 0);
        Write(pat_base + 0xb400 + pat_index, 0);
        Write(pat_base + 0xb800 + pat_index, 0);
        Write(pat_base + 0xbc00 + pat_index, 0);
        Write(pat_base + 0xc000 + pat_index, 0);
        Write(pat_base + 0xc400 + pat_index, 0);
        Write(pat_base + 0xd000 + pat_index, 0);
        Write(pat_base + 0xd400 + pat_index, 0);
        Write(pat_base + 0xe000 + pat_index, 0);
        Write(pat_base + 0xe400 + pat_index, 0);
        Write(pat_base + 0xe800 + pat_index, 0);
        Write(pat_base + 0xec00 + pat_index, 0);
    }

    m_last_pc = ram_count + 1;
}

void HWControl::readPattern(PGCompiler &pattern)
{
    int i, pat_base = 0, pat_index = 0;
    int pc_line = 2048;
    unsigned short flag;
    patternList *pPatternList = pattern.patternData();

    sysReadPGStatus(flag);

    if (flag & 3) {
        return;
    }
    pattern.init();

    TMINMAX minmax;
    TRAMDATA ram;
    TPGDATA *pg = new TPGDATA;

    memset(pg, 0, sizeof(TPGDATA));

    minmax.xmin = Read(0x4023);
    minmax.xmax = Read(0x4024);
    minmax.vmin = Read(0x403b);
    minmax.vmax = Read(0x403c);
    minmax.ymin = Read(0x4043);
    minmax.ymax = Read(0x4044);
    minmax.dmin = Read(0x4063);
    minmax.dmax = Read(0x4064);
    minmax.dmin |= (Read(0x4065) & 0x3) << 16;
    minmax.dmax |= (Read(0x4065) & 0xc) << 14;
    minmax.bmin = Read(0x4083) & 0xf;
    minmax.bmin = (Read(0x4083) & 0xf0) >> 4;
    minmax.bmin = Read(0x40C3) & 0xff;
    minmax.bmin = Read(0x40C3) >> 8;
    minmax.xmin |= (Read(0x4030) & 0xff) << 16;
    minmax.xmax |= (Read(0x4030) & 0xff00) << 8;
    minmax.vmin |= (Read(0x403d) & 0x3) << 16;
    minmax.vmax |= (Read(0x403d) & 0x300) << 8;
    minmax.ymin |= (Read(0x4050) & 0xff) << 16;
    minmax.ymax |= (Read(0x4050) & 0xff00) << 8;

    for (i = 1; i < pc_line; i++) {
        pat_base = i >= 1024 ? 0x100000 : 0;
        pat_index = i % 1024;
        ram.opcode_ts33 = Read(pat_base + 0x8000 + pat_index);
        ram.jump = Read(pat_base + 0x8400 + pat_index);
        ram.l.low_count = Read(pat_base + 0x8800 + pat_index);
        ram.l.high_count = Read(pat_base + 0x8c00 + pat_index);

        ram.x = Read(pat_base + 0xa000 + pat_index);
        ram.x_operand = Read(pat_base + 0x9400 + pat_index);
        ram.x_operdir = Read(pat_base + 0x9800 + pat_index);
        ram.x_direct = Read(pat_base + 0x9c00 + pat_index);
        //	ram.x_fm = Read(pat_base + 0x9800 + pat_index);

        ram.y = Read(pat_base + 0xb000 + pat_index);
        ram.y_operand = Read(pat_base + 0xb400 + pat_index);
        ram.y_operdir = Read(pat_base + 0xb800 + pat_index);
        ram.y_direct = Read(pat_base + 0xbc00 + pat_index);
        //	ram.y_fm = Read(pat_base + 0x9c00 + pat_index);

        ram.dop.d_op = Read(pat_base + 0xc000 + pat_index);
        ram.dop.d_oper = Read(pat_base + 0xc400 + pat_index);
        ram.bank = Read(pat_base + 0xd000 + pat_index);
        ram.scan = Read(pat_base + 0xd400 + pat_index);
        //	ram.s_fm = Read(pat_base + 0xde00 + pat_index);

        ram.vector = Read(pat_base + 0xe000 + pat_index);
        ram.v_source = Read(pat_base + 0xe400 + pat_index);
        ram.v_srcdir = Read(pat_base + 0xe800 + pat_index);
        ram.v_direct = Read(pat_base + 0xec00 + pat_index);

        if (ram.jump == 0xffff) {
            continue;
        }

        if (ram.d.start) {
            pg->pc = i;

            if (pg->pram) {
                delete pg->pram;
            }

            pg->pram = new ramList;
        }

        if (ram.o.use || ram.pg33.use) {
            if (pg->pram) {
                TRAMDATA *use_ram = new TRAMDATA;
                memcpy(use_ram, &ram, sizeof(TRAMDATA));
                pg->pram->AddTail(use_ram);
            }
        }

        if (ram.d.stop) {
            memcpy(&pg->minmax, &minmax, sizeof(TMINMAX));
            pPatternList->AddTail(pg);
            pg = new TPGDATA;
            memset(pg, 0, sizeof(TPGDATA));
        }
    }

    if (pg) {
        delete pg;
    }

    if (pPatternList->GetCount()) {
        pattern.attach();
    }
}

void HWControl::sysStopPattern()
{
    Write(0x4002, 0);
}

void HWControl::writeTimeset(TGCompiler &timeset)
{
    TTIMESET *pTimeSet = timeset.ts();
    unsigned int nTime = 0;
    unsigned int i;

    Write(0x4002, 0);

    for (POSITION pos = pTimeSet->list.GetHeadPosition(); pos; nTime++) {
        TTGDATA *tg = pTimeSet->list.GetNext(pos);

        for (i = 0; i <= 0x5a; i++) {
            Write(0x40000 + nTime + (i << 8), tg->data[i] & 0xffff);
        }

        for (i = 0; i <= 0x1f; i++) {
            Write(0x50000 + nTime + (i << 8), tg->data2[i] & 0xffff);
        }
    }

    for (; nTime < 255; nTime++) {
        for (i = 0; i <= 0x5a; i++) {
            Write(0x40000 + nTime + (i << 8), 0);
        }

        for (i = 0; i <= 0x1f; i++) {
            Write(0x50000 + nTime + (i << 8), 0);
        }
    }

    Write(0x46002, pTimeSet->clk_inv & 0xffff);
    Write(0x46003, pTimeSet->clk_inv >> 16);
    Write(0x46009, pTimeSet->clk_inv2 & 0xffff);
}
/*
void HWControl::readTimeset(TGCompiler &timeset)
{
        char *clks[13] = { "tri", "strobe", "strobe1", "amux",
                "dmux", "addh", "16addr", "scan0",
                "dinv", "scan2", "scan3", "addm", "vmux" };
        int bits33[] = { 12, 8, 10, 0, 2, 16, 18, 4, 6, 20, 22, 14, 89 };
        TTIMESET *pTimeSet = timeset.ts();
        int nts = 0;
        int nTime, i, j;
        unsigned short data;

        sysReadPGStatus(data);

        if (data & 3) {
                return;
        }

        timeset.init();

        pTimeSet->clk_inv = Read(0x46002);
        pTimeSet->clk_inv |= Read(0x46003) << 16;

        for (nTime = 0; nTime < 255; nTime++) {
                TTGDATA *tg = new TTGDATA;
                tg->rate = 2;

                for (i = 0; i <= 0x5a; i++) {
                        tg->data[i] = Read(0x40000 + nTime + (i << 8));
                }

                tg->width = tg->data[0x58] + 30;
                sprintf(tg->name, "TS%d", nts++);
                pTimeSet->list.AddTail(tg);
        }

        if (pTimeSet->list.GetCount() == 0) {
                TTGDATA *tg = new TTGDATA;

                tg->rate = 2;
                tg->width = 1000;

                strcpy(tg->name, "initialize");
                pTimeSet->list.AddTail(tg);
        }

        for (i = 0; i < 13; i++) {
                TCHANNEL *ch = new TCHANNEL;
                ch->sys = bits33[i];
                strcpy(ch->name, clks[i]);
                pTimeSet->order[i] = i;
                pTimeSet->chlist.AddTail(ch);
        }

        for (j = 0; j < 16; j++) {
                TCHANNEL *ch = new TCHANNEL;
                sprintf(ch->name, "c%d", j);
                ch->sys = 24 + j * 4;
                pTimeSet->order[i + j] = i + j;
                pTimeSet->chlist.AddTail(ch);
        }

        for (; j < 22; j++) {
                pTimeSet->order[i + j] = (char) -1;
        }
}
*/
void HWControl::writeScramble(Scramble &scramble)
{
    unsigned short read = Read(0x4062);
    //	char msg[256];

    //	sprintf(msg, "writeScramble 01");
    //	Send(BC_PRINTF, msg, strlen(msg));

    Write(0x4025, scramble.xlink33.data[0]);
    Write(0x4026, scramble.xlink33.data[1]);
    Write(0x4027, scramble.xlink33.data[2]);
    Write(0x4028, scramble.xlink33.data[3]);
    Write(0x4029, scramble.xlink33.data[4]);
    Write(0x4031, scramble.xlink33.data[5]);
    Write(0x4032, scramble.xlink33.data[6]);
    Write(0x4033, scramble.xlink33.data[7]);
    Write(0x4034, scramble.xlink33.data[8]);

    //	sprintf(msg, "writeScramble 02");
    //	Send(BC_PRINTF, msg, strlen(msg));
    Write(0x4045, scramble.ylink33.data[0]);
    Write(0x4046, scramble.ylink33.data[1]);
    Write(0x4047, scramble.ylink33.data[2]);
    Write(0x4048, scramble.ylink33.data[3]);
    Write(0x4049, scramble.ylink33.data[4]);
    Write(0x4051, scramble.ylink33.data[5]);
    Write(0x4052, scramble.ylink33.data[6]);
    Write(0x4053, scramble.ylink33.data[7]);
    Write(0x4054, scramble.ylink33.data[8]);

    //	sprintf(msg, "writeScramble 03, scramble.xscr_add33 : %x, order : %x",
    //			scramble.xscr_add33[0][1], scramble.xy_order[0][1]);
    //	Send(BC_PRINTF, msg, strlen(msg));
    Write(0x4066, scramble.dscr_add33[1] << 8 | scramble.dscr_add33[0]);
    Write(0x4067, scramble.dscr_add33[3] << 8 | scramble.dscr_add33[2]);
    Write(0x4068, scramble.dscr_add33[5] << 8 | scramble.dscr_add33[4]);
    Write(0x406b, scramble.dscr_add33[7] << 8 | scramble.dscr_add33[6]);

    for (unsigned int i = 0; i < 16; i++) {
        Write(0x4100 + i * 4, scramble.xscr_add33[i][1] << 8 | scramble.xscr_add33[i][0]);
        Write(0x4100 + i * 4 + 1, scramble.xscr_add33[i][3] << 8 | scramble.xscr_add33[i][2]);
        Write(0x4100 + i * 4 + 2, scramble.xscr_add33[i][5] << 8 | scramble.xscr_add33[i][4]);
        Write(0x4100 + i * 4 + 3, scramble.xscr_add33[i][7] << 8 | scramble.xscr_add33[i][6]);
    }

    //	sprintf(msg, "v : %x, Read(0x4100) : %x", scramble.xscr_add33[0][1] << 8 | scramble.xscr_add33[0][0], Read(0x4100));
    //	Send(BC_PRINTF, msg, strlen(msg));
    for (unsigned int i = 0; i < 16; i++) {
        Write(0x4140 + i * 4, scramble.yscr_add33[i][1] << 8 | scramble.yscr_add33[i][0]);
        Write(0x4140 + i * 4 + 1, scramble.yscr_add33[i][3] << 8 | scramble.yscr_add33[i][2]);
        Write(0x4140 + i * 4 + 2, scramble.yscr_add33[i][5] << 8 | scramble.yscr_add33[i][4]);
        Write(0x4140 + i * 4 + 3, scramble.yscr_add33[i][7] << 8 | scramble.yscr_add33[i][6]);
    }
    //	sprintf(msg, "writeScramble 04");
    //	Send(BC_PRINTF, msg, strlen(msg));

    //	sprintf(msg, "writeScramble 05");
    //	Send(BC_PRINTF, msg, strlen(msg));
    Write(0x4022, (read & FLASH_DEVICE ? 0x10 : 0) | scramble.x_mode | 0x20);
    Write(0x4042, (read & FLASH_DEVICE ? 0x10 : 0) | scramble.y_mode | 0x20);
    Write(0x4062, (read & 0x1e) | (scramble.d_set ? 1 : 0));

    //	sprintf(msg, "Read(0x4100) : %x", Read(0x4100));
    //	Send(BC_PRINTF, msg, strlen(msg));
    //	sprintf(msg, "writeScramble 06");
    //	Send(BC_PRINTF, msg, strlen(msg));
    writeScramble(scramble.xscr, scramble.yscr, scramble.dscr);
    //	sprintf(msg, "writeScramble 07");
    //	Send(BC_PRINTF, msg, strlen(msg));
}

void HWControl::writeScramble(unsigned short *xscr, unsigned short *yscr, unsigned int *dscr)
{
    int i;

    for (i = 0; i < 0x100 && (xscr || yscr); i++) {
        if (xscr) {
            Write(0x4200 + i, xscr[i]);
        }

        if (yscr) {
            Write(0x4300 + i, yscr[i]);
        }
    }

    for (i = 0; i < 0x100 && dscr; i++) {
        Write(0xc800 + i, (unsigned short) (dscr[i] & 0xffff));
        Write(0xc900 + i, (unsigned short) (((dscr[i]) >> 16) & 0x3));
    }

    //	char msg[256];
    //	sprintf(msg, "Read(0x4100) : %x", Read(0x4100));
    //	Send(BC_PRINTF, msg, strlen(msg));
}

void HWControl::readScramble(Scramble &scramble)
{
    unsigned short read;

    scramble.x_mode = Read(0x4022);
    scramble.y_mode = Read(0x4042);
    read = Read(0x4062);
    scramble.d_set = read & 1;

    scramble.xlink33.data[0] = Read(0x4025);
    scramble.xlink33.data[1] = Read(0x4026);
    scramble.xlink33.data[2] = Read(0x4027);
    scramble.xlink33.data[3] = Read(0x4028);
    scramble.xlink33.data[4] = Read(0x4029);
    scramble.xlink33.data[5] = Read(0x4031);
    scramble.xlink33.data[6] = Read(0x4032);
    scramble.xlink33.data[7] = Read(0x4033);
    scramble.xlink33.data[8] = Read(0x4034);

    scramble.ylink33.data[0] = Read(0x4045);
    scramble.ylink33.data[1] = Read(0x4046);
    scramble.ylink33.data[2] = Read(0x4047);
    scramble.ylink33.data[3] = Read(0x4048);
    scramble.ylink33.data[4] = Read(0x4049);
    scramble.ylink33.data[5] = Read(0x4051);
    scramble.ylink33.data[6] = Read(0x4052);
    scramble.ylink33.data[7] = Read(0x4053);
    scramble.ylink33.data[8] = Read(0x4054);

    //	scramble.dscr_add33.data[0] = Read(0x4066);
    //	scramble.dscr_add33.data[1] = Read(0x4067);
    //	scramble.dscr_add33.data[2] = Read(0x4068);
    //	scramble.dscr_add33.data[2] = Read(0x406b);

    readScramble(scramble.xscr, scramble.yscr, scramble.dscr);
}

void HWControl::readScramble(unsigned short *xscr, unsigned short *yscr, unsigned int *dscr)
{
    int i;
    unsigned short read;

    for (i = 0; i < 0x10000; i++) {
        xscr[i] = sysRead(0x10000 + i);
        yscr[i] = sysRead(0x20000 + i);
    }

    for (i = 0; i < 0x100; i++) {
        read = sysRead(0xc800 + i);
        dscr[i] = read;
        read = sysRead(0xc900 + i);
        dscr[i] |= (int) (read & 0x3) << 16;
    }
}

int HWControl::sysReadPGStatus(unsigned short &flag)
{
    unsigned short pause_count = 0;

    flag = Read(0x4002) & 0xf;

    if (flag & 0x4) {
        pause_count = Read(0x4004);
    }

    return (int) pause_count;
}

void HWControl::sysPGContinue()
{
    Write(0x4002, 7);
}

bool HWControl::isPowerOff()
{
    unsigned short read[2] = {};

    read[0] = Read(0x4001);
    Write(0x4000, ~read[0]);
    read[1] = Read(0x4000);

#ifdef F1500_DEBUG
    return false;
#endif

    return (read[0] == read[1]);
}

void HWControl::sysMuxMode(int mode)
{
    unsigned short data = Read(0x4062);
    mode = mode & 0xff;
    Write(0x4062, (data & 1) | mode);
    data = Read(0x4022);
    Write(0x4022, (mode == FLASH_DEVICE ? 0x10 : 0) | data);
    data = Read(0x4042);
    Write(0x4042, (mode == FLASH_DEVICE ? 0x10 : 0) | data);
}

void HWControl::sysFMMode(bool set)
{
    Write(0x4022, set);
    //	Write(0x4042, set);
}

void HWControl::scanInv(unsigned int bit)
{
    Write(0x40ca, bit & 0xffff);
    Write(0x40cb, bit >> 16);
}

void HWControl::setFault(bool fault)
{
    //	Write(0x6 - fault * 1, 0x400);
    if (fault) {
        Write(0x5, 0x400);
        Write(0x105, 0x400);
        Write(0x205, 0x400);
        Write(0x305, 0x400);
        Write(0x405, 0x400);
        Write(0x505, 0x400);
        Write(0x605, 0x400);
        Write(0x705, 0x400);
    }
    else {
        Write(0x6, 0x400);
        Write(0x106, 0x400);
        Write(0x206, 0x400);
        Write(0x306, 0x400);
        Write(0x406, 0x400);
        Write(0x506, 0x400);
        Write(0x606, 0x400);
        Write(0x706, 0x400);
    }
}

void HWControl::setScanMode2(unsigned int bit)
{
    m_scan_mode = bit;
    Write(0x4001, bit >> 16);
    Write(0x4003, bit >> 16);
    Write(0x406a, bit & 0xffff);
}

void HWControl::setFMSel(bool xy)
{
    Write(0x4003, xy);
    Write(0x4022, !xy);
    Write(0x4042, xy);
}

void HWControl::setClkInv(int clk)
{
    Write(0x46002, clk & 0xffff);
    Write(0x46003, clk >> 16);
}
