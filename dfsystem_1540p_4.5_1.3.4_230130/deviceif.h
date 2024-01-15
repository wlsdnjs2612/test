
#ifndef _DEVICEIF_H
#define _DEVICEIF_H

#include "swcontrol.h"

#define SEQUENCE(a) \
void SEQ_##a() \
{ \
	snprintf(m_deviceName, sizeof(m_deviceName), #a); \
	seq_##a(); \
} \
void seq_##a()

#define SEQUENCE_ITEM(a) \
	addFuncItem("_seq_"#a, (ItemCall) &CDevice::SEQ_##a)

#define MANUAL_ITEM(a) \
	addFuncItem(#a, (ItemCall) &CDevice::STEP_##a)

#define STEP(a, b) \
bool STEP_##a() \
{ \
	m_step_no = m_init_test == true ? 0 : m_step_no; \
	if (!isSkip(b, m_step_no)) { \
		STEP_START(); \
		step_##a(); \
		STEP_END(); \
		return false; \
	} \
	else { \
		return true; \
	} \
} \
void step_##a()

#define STEP_RUN(a) \
do { \
	static unsigned short _step_run_step_no = 0; \
	static unsigned short _step_run_step_index = 0; \
	if (m_all_skip == true) { \
		if (_step_run_step_no == 0) { \
			_step_run_step_no = m_step_no; \
		} \
		else { \
			unsigned short _tmpv = _step_run_step_no; \
			_tmpv = _tmpv > MAX_STEP ? MAX_STEP : _tmpv; \
			for (unsigned int _tmpi = 0; _tmpi < _step_run_step_index; _tmpi++) { \
				_tmpv = m_step_index[_tmpv]; \
				_tmpv = _tmpv > MAX_STEP ? MAX_STEP : _tmpv; \
			} \
			m_step_index[_tmpv] = m_step_no; \
			_step_run_step_index++; \
		} \
		m_step_no++; \
		if (m_step == true) { \
			STEP_##a(); \
		} \
	} \
	else { \
		m_old_step_no = m_step_no; \
		m_step_no = _step_run_step_no; \
		while (m_step_no > 0 && m_step_no < m_old_step_no) { \
			_step_run_step_no = m_step_index[_step_run_step_no]; \
			m_step_no = _step_run_step_no; \
		} \
		_step_run_step_no = m_step_index[_step_run_step_no]; \
		bool _tmp_skip = STEP_##a(); \
		m_skip_status = m_step_no >= 0 ? _tmp_skip : m_skip_status; \
	} \
} while (0)

#define START_DEVICE(a) \
class CDevice : public DeviceIF \
{ \
public: \
	CDevice() : DeviceIF() \
	{ \
		snprintf(m_deviceName, sizeof(m_deviceName), #a); \
	}

#define END_DEVICE() \
}; \
int main(int argc, char **argv) \
{ \
	CDevice d; \
	d.check_diagnostic(argv[0]); \
	return d.parseCommand(argc, argv); \
}

class DeviceIF : public SWControl
{
public:
	DeviceIF();
	virtual ~DeviceIF();

	typedef void (DeviceIF::*ItemCall)(void);

	typedef struct tagITEMLIST
	{
		char name[32];
		ItemCall pfnTest;
	} TITEMLIST;

	typedef List<TITEMLIST *, TITEMLIST *>	CallItems;

	typedef struct tagCommand
	{
		unsigned long length;
		unsigned long cmd;
		char *data;
	} TCommand;

	void LIBRARY_TYPE(unsigned int type);
	unsigned int TEST_START();
	unsigned int ITEM_TEST(const char *name);

	virtual int setOption(int command, int wparam = 0, int lparam = 0);
	virtual int parseCommand(int cmd_c, char **cmd_v);
	virtual void programStart();
	virtual void testEntry();
	virtual void initTest();
	virtual void display();
	virtual void program();
	virtual void erase();
	virtual char *getSource();
	virtual void readChipId();
	void check_diagnostic(char *s);

	const char *name()	{ return (const char *) m_deviceName; }
	const char *version()	{ return (const char *) m_strVersion; }
	const char *libname()	{ return ( const char * ) m_strLibName; }
	const char *libversion(){ return ( const char * ) m_strLibVersion; }
	const char *builddate()	{ return ( const char * ) m_strBuildDate; }
	const char *libremark()	{ return ( const char * ) m_strLibRemark; }
	void setVersion(const char *str);
	StringList *funcItems();

	char	m_strLibName[64];
	char	m_strLibVersion[64];
	char	m_strBuildDate[64];
	char	m_strLibRemark[2048];
    char m_strInitSaveFile[1024];

protected:
	int testStart(const char *name);
	int testEnd();
	bool funcItem(const char *name);
	bool addFuncItem(const char *name, ItemCall func);
	bool isFuncItem(const char *name);
	bool isSkip(const char *name, int testno);

	void setInitData_filename(const char *str);
    bool saveInitData();
    bool readInitData();

	bool m_step;
//	bool m_all_skip;
	int _start_block;
	int _end_block;
	int _write_data;
	int _select_scan;
	int m_Zone;
	char m_deviceName[64];
	char m_strVersion[64];
	char partName[30];
	unsigned short m_step_index[MAX_STEP + 1];

private:
	void testing();
	bool isChangeDetect();

	bool m_lib_send;
	unsigned int m_testTime;
	int m_manual_step;

	CallItems m_funcList;
	StringList m_strFuncList;
	StepList m_SkipList;
	ItemCall m_testFunc;
};

#endif
