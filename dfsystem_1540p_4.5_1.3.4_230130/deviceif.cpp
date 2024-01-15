
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "deviceif.h"

DeviceIF::DeviceIF() : SWControl(),
	m_step(false),
	_start_block(0),
	_end_block(0xfff),
	_write_data(0),
	_select_scan(0),
	m_Zone(0),
	m_lib_send(false),
	m_testTime(0),
	m_manual_step(0),
	m_testFunc(0)
{
	char host[100] = { 0 };
	gethostname(host, 100);
	m_Zone = atoi(host + 7);
	memset(m_deviceName, 0, sizeof(m_deviceName));
	memset(m_strVersion, 0, sizeof(m_strVersion));
	memset(m_step_index, 0, sizeof(m_step_index));

	sprintf( m_strLibName, "%s", "[LIBNAME] 1540sys" );
	sprintf( m_strLibVersion, "[VERSION] %d.%d.%d", _MAJOR_REV1, _MAJOR_REV2, _MINOR_REV);
	sprintf( m_strBuildDate, "%s", "[BUILDDATE] 2021.12.13" );
	sprintf( m_strLibRemark, "%s", "[REMARK] MULTI PGM & Add TIME Interlock functions." );
	setInitData_filename("/initSaveFile");
}

DeviceIF::~DeviceIF()
{
	for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
		delete m_funcList.GetNext(pos);
	}

	m_funcList.RemoveAll();
}

void DeviceIF::LIBRARY_TYPE(unsigned int type)
{
	m_lib_type = type;
	m_lib_send = true;
	Send(BC_LIB_TYPE, (char *) &m_lib_type, sizeof(unsigned int));
}

char *DeviceIF::getSource()
{
	return 0;
}

void DeviceIF::display()
{
}

void DeviceIF::program()
{
}

void DeviceIF::erase()
{
}

void DeviceIF::initTest()
{
}

int DeviceIF::parseCommand(int cmd_c, char **cmd_v)
{
	int i;
	char item[32] = "";
	char *s = 0;
	bool test = false;

//	RESET_TESTER();
	check_diagnostic(cmd_v[0]);

	for (i = 1; i < cmd_c; i++) {
		if (strcmp(cmd_v[i], "-step") == 0) {
			/*
			 * STEP 정보 설정
			 *
			 * m_all_skip == true 일경우 실제 TEST를 수행하지 않고 FOSMain에 넘겨줄 STEP 정보를 설정한다.
			 * m_init_test : INITIAL TEST FLAG SETTING
			 * INITIAL TEST가 하나의 STEP이 아니고 여러 STEP으로 이루어져 있을경우에
			 * FOSMain으로 STEP 정보를 넘겨줄때 여러개의 STEP중 하나의 STEP 정보만 넘겨주기 위한 처리
			 */
			m_step = true;
			m_all_skip = true;
			m_init_test = true;
			initTest();
			m_init_test = false;
			m_testFunc = &DeviceIF::testEntry;

			for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
				TITEMLIST *step = m_funcList.GetNext(pos);

				if (strcmp(step->name, item) != 0) {
					continue;
				}

				m_testFunc = step->pfnTest;
				break;
			}

			testing();
			m_step = false;
			m_all_skip = false;

			int j = 0;
			int nstep = m_SkipList.GetCount();
			GETSTEP	steps[nstep];

			for (POSITION pos = m_SkipList.GetHeadPosition(); pos; j++) {
				steps[j] = m_SkipList.GetNext(pos);
			}

			Send(BC_GETSTEP, steps, sizeof(GETSTEP) * nstep);
			//return 0;
		}
		else if (strcmp("-fm", cmd_v[i]) == 0) {
		}
		if (strcmp("-tempinfo", cmd_v[i]) == 0) {
			m_tempinfo_mode = true;
		}
		else if (strcmp("-find", cmd_v[i]) == 0) {
			for (unsigned int j = 0; j < NSLOT; j++) {
				FIND_BOARD(j);
			}

			Send(BC_BIBFIND, _bi_find, sizeof(BIB_FIND) * NSLOT);
		}
		else if (strcmp("-reset", cmd_v[i]) == 0) {
			RESET_TESTER();
		}
		else if (strcmp("-dummy", cmd_v[i]) == 0) {
			for (unsigned int j = 0; j < NSLOT; j++) {
				_bi_find[j].find_drv = 1;
				_bi_find[j].read = 1;
				sprintf(_bi_find[j].name, " BIB-%02d", j + 1);
			}

			Send(BC_BIBFIND, _bi_find, sizeof(BIB_FIND) * NSLOT);
		}
		else if (strcmp("-bib", cmd_v[i]) == 0) {
			int size[4] = { m_bib_scan, m_bib_io_count_per_1scan, m_bib_x, m_bib_y };
			Send(BC_GETBIB, &size, 16);
		}
		else if (strcmp("-init", cmd_v[i]) == 0) {
		//	int temp[16] = { 0 }, i;

			/*
			 * m_init_test : INITIAL TEST FLAG SETTING
			 * INITIAL TEST가 하나의 STEP이 아니고 여러 STEP으로 이루어져 있을경우에
			 * Step의 Step Counter를 조정하여 Initial Test는 수행하되 하나의 Step을 여러번 테스트하는 것처럼 처리
			 */
			m_init_test = true;
                        initTest();
			m_init_test = false;
			Send(BC_INITEND, 0, 0);
			saveInitData();
		}
		else if (strcmp("-tm", cmd_v[i]) == 0) {
			_test_mode = true;
		}
		else if ((s = strstr(cmd_v[i], "-display="))) {
			_start_block = strtoul(cmd_v[i] + 9, &s, 0);
			display();
			Send(BC_DISPLAY_END, 0, 0);
		}
		else if ((s = strstr(cmd_v[i], "-program="))) {
			_start_block = strtol(cmd_v[i] + 9, &s, 0);
			m_step_no = -1;
			program();
			Send(BC_DISPLAY_END, 0, 0);
		}
		else if ((s = strstr(cmd_v[i], "-erase="))) {
			_start_block = strtol(cmd_v[i] + 9, &s, 0);
			erase();
			Send(BC_DISPLAY_END, 0, 0);
		}
		else if ((s = strstr(cmd_v[i], "-scan="))) {
			_select_scan = strtol(cmd_v[i] + 6, &s, 0);
		}
		else if((strcmp(cmd_v[i], "-ver") == 0)){
			char sysver[50];
			char cmd[100];
			sprintf(sysver, "%d.%d.%d", _MAJOR_REV1, _MAJOR_REV2, _MINOR_REV);
			sprintf(cmd, "[SYSTEM LIB] [VERSION] %s\n[ PMBUS LIB] %s", sysver, m_PSManager.libversion());
			//system(cmd);
			puts(cmd);
		}
		else if ((s = strstr(cmd_v[i], "-s="))) {
			char *p = cmd_v[i] + strlen("-s=");
			GETSTEP	step;
			//	step.step_no = atoi(cmd_v[i] + 3);
			//	m_SkipList.AddTail(step);
			//	SET_BIT(m_skip_step, step.step_no, 1);

			for (unsigned int j = 0; p[j] != '\0'; j++) {
				if (p[j] == '1') {
					continue;
				}

				step.step_no = j + 1;
				m_SkipList.AddTail(step);
			//	SET_BIT(m_skip_step, step.step_no, 1);
			}

		//	for (POSITION pos = m_SkipList.GetHeadPosition(); pos;) {
		//		step = m_SkipList.GetNext(pos);
		//		printf("skip_step_no : %d", step.step_no);
		//	}
		}
		else if ((s = strstr(cmd_v[i], "-manual="))) {
			test = true;
			m_manual_step = strtol(cmd_v[i] + 8, &s, 0);
		}
		else if ((s = strstr(cmd_v[i], "-judge="))) {
			 SET_JUDGE(strtol(cmd_v[i] + 7, &s, 0));
		}
		else if ((s = strstr(cmd_v[i], "-start="))) {
			 _start_block = strtol(cmd_v[i] + 7, &s, 0);
		}
		else if ((s = strstr(cmd_v[i], "-end="))) {
			 _end_block = strtol(cmd_v[i] + 5, &s, 0);
		}
		else if ((s = strstr(cmd_v[i], "-data="))) {
			 _write_data = strtol(cmd_v[i] + 6, &s, 0);
		}
		else if ((s = strstr(cmd_v[i], "-seq="))) {
			strcpy(item, "_seq_");
			strcat(item, s + 5);
		}
		else if ((s = strstr(cmd_v[i], "-item="))) {
			strcpy(item, s + 6);
		}
		else if (strcmp("-test", cmd_v[i]) == 0) {
			test = true;
		}
		else if (strcmp("-rev", cmd_v[i]) == 0) {
			int set = 0;
			char n[m_funcList.GetCount() + 2][32];
			
			strcpy(n[set++], m_deviceName);
			strcpy(n[set++], m_strVersion);

			for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
				TITEMLIST *item = m_funcList.GetNext(pos);

				if (strstr(item->name, "_seq_") == 0) {
					continue;
				}

				strcpy(n[set++], item->name + 5);
			}

			Send(BC_VERSION, n, set * 32);
			return 0;
		}
		else if (strcmp("-backup", cmd_v[i]) == 0) {
			if (m_init_fail) {
				//Recv((char *) m_init_fail, sizeof(char) * m_bib_x * m_bib_y * NSLOT);
			}
		}
		else if ((s = strstr(cmd_v[i], "-ss="))) {
			setScanLoop(atoi(s + 4) - 1);
		}
		else if (strcmp("-start", cmd_v[i]) == 0) {
			Write(0x406a, 0);
			programStart();
			if (m_lib_send == false) {
				LIBRARY_TYPE(m_lib_type);
			}

			if (m_dev_os_type != ALL_TYPE &&
					(m_dev_os_type == T33_ONLY && m_sysModel == 10 ||
					m_dev_os_type == T10_ONLY && m_sysModel == 33)) {
				char tmp[100] = { 0 };
				snprintf(tmp, sizeof(tmp), "DEVICE & OS TYPE NOT SAME [DEVICE : %s / OS : %uM]",
						m_dev_os_type == ALL_TYPE ? "ALL TYPE" :
						m_dev_os_type == T33_ONLY ? "33M" :
						m_dev_os_type == T10_ONLY ? "10M" : "UNKNOWN",
						m_sysModel);
				Send(BC_DEV_OS_TYPE, tmp, strlen(tmp));
				::exit(0);
			}
		}
		/*
		 * fosmain 모드 정보 (양산 모드 or Eng'r 모드) : Eng'r 모드일때만 DQ Data Reading 함
		 */
		else if ((s = strstr(cmd_v[i], "-mmode="))) {
			int tmp = 0;
			sscanf(s + 7, "%d", &tmp);
			m_eng_mode = (bool) tmp;
		}
		else if ((s = strstr(cmd_v[i], "-nrack="))) {
			if (sscanf(s + 7, "%d", &NRACK) != 1) {
				NRACK = 4;
			}
		}
		else if ((s = strstr(cmd_v[i], "-nslot="))) {
			if (sscanf(s + 7, "%d", &NSLOT) != 1) {
				NSLOT = 8;
			}
		}
		else if ((s = strstr(cmd_v[i], "-part="))) {
			strcpy(partName, s + 6);
		}
	}

	if (test) {

        readInitData();

		if (m_diagnostic == false) {
			m_all_skip = true;
			m_step_no = 1;
			m_testFunc = &DeviceIF::testEntry;
			testing();
			m_all_skip = false;
		}

		m_step_no = 1;
		m_testFunc = &DeviceIF::testEntry;

		if (strstr(item, "_seq_")) {
			for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
				TITEMLIST *fi = m_funcList.GetNext(pos);

				if (strcmp(fi->name, item) == 0) {
		      			m_testFunc = fi->pfnTest;
					break;
				}
			}
		}

		Send(BC_TESTSTART, 0, 0);
		testing();

		if (!m_diagnostic) {
			if (m_hichp_use) {
				CALC_HICHIP();
			}

			unsigned int *p = (unsigned int *) &m_step_fail[m_bib_x * m_bib_y * NSLOT + 1];
			p[0] = m_test_dut_count;
			m_step_fail[m_bib_x * m_bib_y * NSLOT] = 0xff;
			Send(BC_FCATE_DATA, m_fail_cate, sizeof(m_fail_cate));
			Send(BC_PCATE_DATA, m_pass_cate, sizeof(m_pass_cate));
			Send(BC_BIN_DATA, m_bin, sizeof(m_bin));
			Send(BC_DUT_PF_DATA_STEP, m_step_fail, m_bib_x * m_bib_y * NSLOT + 5);
			Send(BC_CHIP_PF_DATA_STEP, m_step_fail_chip, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);
			Sync();
		}
		
		Send(BC_TESTEND, 0, 0);
	}
	else if (item[0]) {
		ITEM_TEST(item);
	}
	return 0;
}

int DeviceIF::setOption(int command, int wparam, int lparam)
{
	return 0;
}

void DeviceIF::setVersion(const char *str)
{
	memset(m_strVersion, 0, sizeof(m_strVersion));
	snprintf(m_strVersion, sizeof(m_strVersion), "%s", str);
}

/*
 * STEP SKIP 여부 검사
 * m_all_skip == true  : FOSMain에 넘겨줄 STEP 정보를 Setting하는 기능
 * m_all_skip == false : Step Skip 여부 검사
 */
bool DeviceIF::isSkip(const char *name, int step_no)
{
	GETSTEP	step;
	String	str;

	m_cur_step.step_no = step_no;
	m_cur_step.tcs[0] = 0;
	strcpy(m_cur_step.name, name);

	if (m_all_skip) {
		strcpy(step.name, name);
		step.step_no = step_no;

		if (m_SkipList.IsEmpty() == true || step_no != 0) {
			m_SkipList.AddTail(step);
		}
		else if (m_SkipList.IsEmpty() == false && step_no == 0) {
			m_SkipList.RemoveTail();
			m_SkipList.AddTail(step);
		}

		return true;
	}
	else if (m_manual_step && step_no != m_manual_step) {
		return true;
	}

	for (POSITION pos = m_SkipList.GetHeadPosition(); pos;) {
		step = m_SkipList.GetNext(pos);

		if (step.step_no == step_no || strcmp(name, step.name) == 0) {
			return true;
		}
	}

	return false;
}

void DeviceIF::setInitData_filename(const char *str)
{
    sprintf(m_strInitSaveFile, "%s", str);
}

bool DeviceIF::saveInitData()
{
    FILE *finitSave;

    finitSave = fopen(m_strInitSaveFile, "w+");

    if(finitSave == NULL)
    {
        return false;
    }

	fwrite(m_init_fail, sizeof(char) * m_bib_x * m_bib_y * NSLOT, 1, finitSave);

	if(finitSave != NULL)
	{
		fclose(finitSave);
	}

    return true;
}

bool DeviceIF::readInitData()
{
    FILE *finitSave;

    finitSave = fopen(m_strInitSaveFile, "r+");

    if(finitSave == NULL)
    {
        return false;
    }

    fread(m_init_fail, sizeof(char) * m_bib_x * m_bib_y * NSLOT, 1, finitSave);

    if(finitSave != NULL)
    {
        fclose(finitSave);
    }
	
    return true;
}

bool DeviceIF::addFuncItem(const char *name, ItemCall func)
{
	if (isFuncItem(name)) {
		return FALSE;
	}

	TITEMLIST *item = new TITEMLIST;
	strcpy(item->name, name);
	item->pfnTest = func;
	m_funcList.AddTail(item);
	return true;
}

StringList *DeviceIF::funcItems()
{
	if (m_strFuncList.GetCount()) {
		return &m_strFuncList;
	}

	for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
		TITEMLIST *item = m_funcList.GetNext(pos);
		m_strFuncList.AddTail(item->name);
	}

	return &m_strFuncList;
}

bool DeviceIF::isFuncItem(const char *name)
{
	for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
		TITEMLIST *item = m_funcList.GetNext(pos);

		if (strcmp(name, item->name) == 0) {
			return true;
		}
	}

	return false;
}

unsigned int DeviceIF::TEST_START()
{
//	if (!isChangeDetect())
//		return false;

	m_testTime = 0;
	m_testFunc = &DeviceIF::testEntry;
	testing();
	return m_testTime;
}

unsigned int DeviceIF::ITEM_TEST(const char *name)
{
//	if (!isChangeDetect())
//		return false;

	m_testTime = 0;

	for (POSITION pos = m_funcList.GetHeadPosition(); pos;) {
		TITEMLIST *item = m_funcList.GetNext(pos);

		if (strcmp(name, item->name) == 0) {
			m_testFunc = item->pfnTest;
			break;
		}
	}

	if (m_testFunc) {
	//	WRITE_CONSOLE("%s START\n", name);
		testing();
	//	WRITE_CONSOLE("%s END %d,%d MS\n", name, m_testTime / 1000, m_testTime % 1000);
	}

	return m_testTime;
}

void DeviceIF::testing()
{
	m_testTime = sysDelay(0);

	if (m_testFunc) {
		(this->*m_testFunc)();
		m_testFunc = 0;
	}

	m_testTime = sysDelay(0) - m_testTime;
}

int DeviceIF::testStart(const char *name)
{
	return OPTION_CALL(BC_ITEM_START, 0, (int) name);
}

int DeviceIF::testEnd()
{
	return OPTION_CALL(BC_ITEM_END);
}

void DeviceIF::programStart()
{
}

void DeviceIF::testEntry()
{
}

void DeviceIF::readChipId()
{
}

bool DeviceIF::isChangeDetect()
{
	if (isPowerOff()) {
		WRITE_CONSOLE("FATAL ERROR : SYSTEM FAULT\n");
		OPTION_CALL(BC_SYSTEM_OFF);
		return false;
	}

	return true;
}

void DeviceIF::check_diagnostic(char *s)
{
	if (strstr(s, "dfdiag")) {
		m_diagnostic = true;
	}
	else {
		m_diagnostic = false;
	}
}
