#include "swcontrol.h"

const unsigned int ID_FM_STATUS = 0x8001;
const unsigned int ID_FM_FAILS = 0x8002;
const unsigned int ID_FM_BLOCK = 0x8003;
const unsigned int ID_FAILS1 = 0x8004;
const unsigned int ID_FAILS2 = 0x8005;
const unsigned int ID_FAILS3 = 0x8006;
const unsigned int ID_FAILS4 = 0x8007;
const unsigned int ID_FAILS5 = 0x8008;
const unsigned int ID_FAILS6 = 0x8009;
const unsigned int ID_FAILS7 = 0x800a;
const unsigned int ID_FAILS8 = 0x800b;
const unsigned int ID_ECC = 0x800c;
const unsigned int ID_INIT = 0x800f;
const unsigned int ID_DATA = 0x8010;
const unsigned int ID_READ_ID = 0x8011;
const unsigned int ID_STATUS = 0x8012;
const unsigned int ID_READ_SCAN = 0x8013;
const unsigned int ID_READ_BLOCK = 0x8014;
const unsigned int ID_READ_BLOCKS = 0x8015;
const unsigned int ID_L1_RESULT = 0x8016;
const unsigned int ID_READ_ECC = 0x8017;
const unsigned int ID_READ_IO = 0x8018;
const unsigned int ID_READ_DISPLAY = 0x8019;
const unsigned int ID_PRETEST_DATA = 0x8020;
const unsigned int ID_PRETEST_RESULT = 0x8021;
const unsigned int ID_BITEST_DATA = 0x8022;
const unsigned int ID_BITEST_RESULT = 0x8023;
const unsigned int ID_LATCH = 0x8024;
const unsigned int ID_READ_CHIP_ID = 0x8025;
const unsigned int ID_CHIP_ID_RESULT = 0x8026;
const unsigned int ID_FM_READ_DATA = 0x8027;
const unsigned int ID_DQ_DATA = 0x8028;
const unsigned int ID_DQ_RESULT = 0x8029;
const unsigned int ID_BIST_IO_LATCH = 0x8100;
const unsigned int ID_BIST_DATA = 0x8101;
const unsigned int ID_BIST_RESULT = 0x8102;
const unsigned int ID_BIST2_RESULT = 0x8103;
const unsigned int ID_EMMC_IO_LATCH = 0x8110;
const unsigned int ID_EMMC_DATA = 0x8111;
const unsigned int ID_EMMC_INIT = 0x8112;
const unsigned int ID_EMMC_RESULT = 0x8113;
const unsigned int ID_EMMC_BLOCK_RESULT = 0x8114;
const unsigned int ID_EMMC_CHIP_ID = 0x8115;
const unsigned int ID_EMMC_READ_DATA = 0x8116;
const unsigned int ID_AGING = 0x8201;

SWControl::SWControl() : HWControl(),
	m_init_test(false),
	m_eng_mode(false),
	m_dq_mode(false),
	m_dq_use(false),
	m_app_value(false),
	m_hichp_use(false),
	m_read_nand_chipid(false),
	m_aging_use(false),
	m_aging_end(false),
	m_skip_status(false),
	m_all_skip(false),
	m_use_mask(false),
	m_use_excl_mask(true),
	m_use_setmap(false),
	m_pf_state(false),
	m_tempinfo_mode(false),
	m_emmc_bit_value(1),
	m_emmc_init_display_count(0),
	m_emmc_test_display_count(0),
	m_init_count(0),
	m_test_count(0),
	m_cat_chipid_ver(0),
	m_mcp(1),
	m_nchip(1),
	m_start_mcp(0),
	m_end_mcp(0),
	m_emmc_ce(0),
	m_emmc_chipid_unit_count(0),
	m_emmc_fm_display_count(0),
	m_device_mode(0),
	m_device_code(0),
	m_test_mode(0),
	m_ready_bit(0),
	m_read_rdq_count(0),
	m_set_rdq_count(1),
	m_rdq_index(0),
	m_hic_length(0),
	m_hic_count(0),
	m_hic_size(0),
	m_dram_read_chipid_count(0),
	m_step_type(0),
	m_use_io_count(0),
	m_dev_os_type(0),
	m_block_limit(0x1000),
	m_block_fail_limit(80),
	m_block_shift(13),
	m_page_shift(0),
	m_page_div(528),
	m_test_dut_count(0),
	m_block_add_limit(10),
	m_block_add_limit_step(-1),
	m_fail_mode(NORMAL),
	m_judge_mode(JUD_NORMAL),
	m_ecc_index(0),
	m_ecc_page(0),
	m_ecc_io(0),
	m_ecc_block(0),
	m_emmc_compare_bit(0),
	m_emmc_expect_bit(0),
	_test_mode(0),
	m_aging_stime(0),
	m_aging_etime(0),
	m_fm_read_buf_index(0),
	//	m_fm_read_scan_count(0),
	m_fm_read_data_index(0),
	m_board_index(-1),
	m_step_no(0),
	m_old_step_no(0),
	m_now_step_count(0),
	m_set_step_count(1),
	m_step_merge_type(MULTI_AND),
	m_bib_scan(14),
	m_bib_io_size(8),
	m_bib_io_count_per_1scan(8),
	m_bib_x(14),
	m_bib_y(8),
	m_scan(0),
	m_count(0),
	m_sync_index(0),
	m_use_mode(0),
	m_base_cat_step(0),
	m_bist_errno_no(0),
	m_bist_exception_cate(99),
	m_mpat_limit_time(0),
	m_emmc_compare_value(0llu),
	m_emmc_expect_value(0llu),
	m_init_fail(0),
	m_pre_init_fail(0),
	m_cum_fail(0),
	m_mask_fail(0),
	m_step_fail(0),
	m_step_io_fail(0),
	m_cum_fail_chip(0),
	m_step_fail_chip(0),
	m_test_fail(0),
	m_test_data(0),
        m_test_io_data(0),
	m_cum_fail_block(0),
	m_now_step_fail_block(0),
	m_get_step_fail_block(0),
	m_init_fail_block(0),
	m_read_fm_data(0),
	m_chipid(0),
	m_waferid(0),
	m_xpos(0),
	m_ypos(0),
	m_emmc_offset(0),
	m_index_map(0),
	m_index_si_map(0),
	m_si_index_map(0),
	m_ecc_fail(0),
	m_cat_limit(0),
	m_cat_action_cate(0),
	m_dq_value(0),
	m_dq_data(0),
	m_failblock(0),
	m_patternCompiler(0),
	m_timeCompiler(0),
	m_16io_mode(false),
	m_ps1(false),
	m_ps2(false),
	m_ps4(false),
	m_ps5(false),
	m_fm_page(0),
	m_imprint_fail_count(0),
	m_display_count(0),
	m_page_mask(0),
	m_ecc_fly_count(0),
	m_ps1_voltage(0.0),
	m_ps2_voltage(0.0),
	m_ps4_voltage(0.0),
	m_ps5_voltage(0.0),
	m_ps1_high(0.0),
	m_ps2_high(0.0),
	m_ps4_high(0.0),
	m_ps5_high(0.0),
	m_ps1_low(0.0),
	m_ps2_low(0.0),
	m_ps4_low(0.0),
	m_ps5_low(0.0),
	m_ps1_limit(0.0),
	m_ps2_limit(0.0),
	m_ps3_limit(0.0),
	m_ps4_limit(0.0),
	m_ps5_limit(0.0),
	m_ecc_block_fail(0),
	m_ecc_fly_fail(0),
	m_fault_timer(10*1000000)
{
	m_comp_io_mask.da = 0xffffffffffffllu;
	m_comp_io_mask.db = 0xffffffffffffllu;
	memset(m_hic_type, 0, sizeof(m_hic_type));
	memset(m_fail_cate, 0, sizeof(m_fail_cate));
	memset(m_pass_cate, 0, sizeof(m_pass_cate));
	memset(m_bin, 0, sizeof(m_bin));
	memset(m_force_bin, 0, sizeof(m_force_bin));
	memset(m_bin_mark, 0, sizeof(m_bin_mark));
	m_shmoo_init = m_shmoo_test = false;
	m_shmoo_save_ch[0] = m_shmoo_save_ch[1] = 10;
	m_shmoo_save_data[0] = m_shmoo_save_data[1] = 0.0;
	memset(_bi_find, 0, sizeof(_bi_find));
	memset(m_icount, 0, sizeof(m_icount));
	memset(m_tcount, 0, sizeof(m_tcount));
	memset(m_ocount, -1, sizeof(m_ocount));
	

	for (unsigned int i = 0; i < NSLOT; i++) {
		for (unsigned int j = 0; j < MAX_NAND_CHIP; j++) {
			m_bist[i][j] = new char[MAX_BIST_SIZE + 2];
			memset(m_bist[i][j], 0xff, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	m_cat_action_cate = new unsigned int[200];
	m_cat_limit = new unsigned int[64];
	memset(m_cat_action_cate, 0, sizeof(unsigned int) * 200);
	memset(m_cat_limit, 0, sizeof(unsigned int) * 64);
	memset(m_tmp_fm_data, 0, sizeof(m_tmp_fm_data));
	SET_IO_DEFINE(1, 2, 3, 4, 5, 6, 7, 8);
	Write(0x406a, 0);
	SCAN_INV(0);
	SET_IOMASK(0);
	m_tsCount = 0;

	readPSRev();

	if(m_isnew == true)
	{
		i2c_init();
	}
	else
	{
		readBDRev();
	}
	SET_PS_COMMON(true);
	RESET_TESTER();
	SET_REV();
}

SWControl::~SWControl()
{
	for (POSITION pos = m_tgGroup.GetHeadPosition(); pos;) {
		delete m_tgGroup.GetNext(pos);
	}

	m_tgGroup.RemoveAll();

	for (POSITION pos = m_pgGroup.GetHeadPosition(); pos;) {
		delete m_pgGroup.GetNext(pos);
	}

	m_pgGroup.RemoveAll();

	if (m_init_fail) {
		delete[] m_init_fail;
		m_init_fail = 0;
	}

	if (m_pre_init_fail) {
		delete[] m_pre_init_fail;
		m_pre_init_fail = 0;
	}

	if (m_cum_fail) {
		delete[] m_cum_fail;
		m_cum_fail = 0;
	}

	if (m_mask_fail) {
		delete[] m_mask_fail;
		m_mask_fail = 0;
	}

	if (m_step_fail) {
		delete[] m_step_fail;
		m_step_fail = 0;
	}

        if (m_step_io_fail) {
                delete[] m_step_io_fail;
                m_step_io_fail = 0;
        }

	if (m_cum_fail_chip) {
		delete[] m_cum_fail_chip;
		m_cum_fail_chip = 0;
	}

	if (m_step_fail_chip) {
		delete[] m_step_fail_chip;
		m_step_fail_chip = 0;
	}

	if (m_test_fail) {
		delete[] m_test_fail;
		m_test_fail = 0;
	}

	if (m_test_data) {
		delete[] m_test_data;
		m_test_data = 0;
	}

        if (m_test_io_data) {
                delete[] m_test_io_data;
                m_test_io_data = 0;
        }
	
	if (m_cum_fail_block) {
		delete[] m_cum_fail_block;
		m_cum_fail_block = 0;
	}

	if (m_now_step_fail_block) {
		delete[] m_now_step_fail_block;
		m_now_step_fail_block = 0;
	}

	if (m_get_step_fail_block) {
		delete[] m_get_step_fail_block;
		m_get_step_fail_block = 0;
	}

	if (m_init_fail_block) {
		delete[] m_init_fail_block;
		m_init_fail_block = 0;
	}

	if (m_read_fm_data) {
		delete[] m_read_fm_data;
		m_read_fm_data = 0;
	}

	if (m_dq_value) {
		delete[] m_dq_value;
		m_dq_value = 0;
	}

	if (m_dq_data) {
		delete[] m_dq_data;
		m_dq_data = 0;
	}

	//	if (m_block_fail) {
	//		delete[] m_block_fail;
	//		m_block_fail = 0;
	//	}

	if (m_ecc_block_fail) {
		delete[] m_ecc_block_fail;
		m_ecc_block_fail = 0;
	}

	if (m_ecc_fail) {
		delete[] m_ecc_fail;
		m_ecc_fail = 0;
	}

	if (m_ecc_fly_fail) {
		delete[] m_ecc_fly_fail;
		m_ecc_fly_fail = 0;
	}

	if (m_failblock) {
		delete[] m_failblock;
		m_failblock = 0;
	}

	if (m_cat_limit) {
		delete m_cat_limit;
		m_cat_limit = 0;
	}
}

BIB_FIND *SWControl::FIND_BOARD(int bib)
{
	unsigned short serial, code;

	_bi_find[bib].find_drv = 0;
	_bi_find[bib].name[0] = 0;

#ifdef BIB_DEBUG
	_bi_find[bib].find_drv = 1;
	sprintf(_bi_find[bib].name, "%02x%x%x%05d", rand() & 0xf, rand() & 0xf, rand() & 0xf, rand() & 0xffff);

	return _bi_find;
#endif

	sysWrite(bib * 0x100, 0xff);

	if (sysRead(bib * 0x100) != 0xff) {
		return _bi_find;
	}

	if (!(sysRead(bib * 0x100 + 0xa) & 0x100)) {
		return _bi_find;
	}

	sysWrite(0x8, 1);
	serial = sysRead(0xa + bib * 0x100) & 0xff;

	sysWrite(0x8, 2);
	serial |= (sysRead(0xa + bib * 0x100) & 0xff) << 8;

	sysWrite(0x8, 8);
	code = sysRead(0xa + bib * 0x100) & 0xff;

	sysWrite(0x8, 4);
	code |= (sysRead(0xa + bib * 0x100) & 0xff) << 8;

	_bi_find[bib].find_drv = 1;
	sprintf(_bi_find[bib].name, "%02x%x%x%05d", code & 0xf, (code >> 12) & 0xf, (code >> 8) & 0xf, serial);
	return _bi_find;
}

String SWControl::GET_PART_NAME()
{
	char temp[30] = { 0 };
	Send(BC_GET_PART_NAME, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

String SWControl::GET_LOT_NAME( )
{
	char temp[30] = { 0 };
	Send(BC_GET_LOT_NAME, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

int SWControl::GET_LOT_NAME(int &no, String *name)
{
	char temp[300] = { 0 }, tmp[30], *p;
	int i;

	Send(BC_GET_LOT_NO, 0, 0);
	Recv((char *) temp, sizeof(temp));
	p = temp;

	for (i = 0; i < 36; i++) {
		sscanf(p, "%s", tmp);
		name[i] = tmp;

		if ((p = strchr(p, ' ')) == NULL) {
			break;
		}

		p++;
	}

	no = i;
	return no;
}

String SWControl::GET_PGM_NAME()
{
	char temp[30] = { 0 };
	Send(BC_GET_PGM_NAME, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

String SWControl::GET_SYSTEM_NAME()
{
	char temp[30] = { 0 };
	Send(BC_GET_SYSTEM_NAME, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

String SWControl::GET_START_TIME()
{
	char temp[20] = { 0 };
	Send(BC_GET_START_TIME, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

String SWControl::GET_END_TIME()
{
	char temp[20] = { 0 };
	Send(BC_GET_END_TIME, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

char* SWControl::GET_OWNER()
{
	char temp[13] = { 0 };
	Send(BC_GET_OWNER, 0, 0);
	Recv((char *) temp, sizeof(temp));
	return temp;
}

/*------- coordinate -------*/
int SWControl::mapindex_to_si_raw(int index, unsigned int &scan, unsigned int &io)
{
	unsigned int i, j;

	if (index == -1) {
		return -1;
	}

	for (i = 0; i < m_bib_io_count_per_1scan; i++) {
		for (j = 0; j < m_bib_scan; j++) {
			if ((unsigned int) index == i * m_bib_scan + j) {
				scan = j;
				io = i;
				return 0;
			}
		}
	}

	return -1;
}

int SWControl::index_to_si(int index, unsigned int &scan, unsigned int &io)
{
	scan = m_index_si_map[index] & 0xffff;
	io = m_index_si_map[index] >> 16;
	return 0;
}

int SWControl::index_to_xy(int index, unsigned int &x, unsigned int &y)
{
	x = m_index_map[index] & 0xffff;
	y = m_index_map[index] >> 16;
	return 0;
}

int SWControl::si_to_index(unsigned int scan, unsigned int io, unsigned int &index)
{
	index = m_si_index_map[io * m_bib_scan + scan];
	return (int) index >= 0 ? 0 : -1;
}

int SWControl::xy_to_index(unsigned int x, unsigned int y, unsigned int &index)
{
	index = y * m_bib_x + x;
	return 0;
}

int SWControl::si_to_xy(unsigned int scan, unsigned int io, unsigned int &x, unsigned int &y)
{
	unsigned int index;

	if (si_to_index(scan, io, index) < 0) {
		x = y = -1;
		return -1;
	}

	return index_to_xy(index, x, y);
}

int SWControl::xy_to_si(unsigned int x, unsigned int y, unsigned int &scan, unsigned int &io)
{
	unsigned int index;
	xy_to_index(x, y, index);
	return index_to_si(index, scan, io);
}
/*--------------------------*/

/*------- GET / SET DATA -------*/
bool SWControl::IS_PRE_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_PRE_FAIL(board, dut_index);
}

bool SWControl::IS_PRE_FAIL(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_init_fail == 0 ? true : m_init_fail[board * m_bib_x * m_bib_y + dut] & 0x1 ? true : false;
}

bool SWControl::IS_CUM_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_CUM_FAIL(board, dut_index);
}

bool SWControl::IS_CUM_FAIL(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_cum_fail == 0 ? true : m_cum_fail[board * m_bib_x * m_bib_y + dut] & 0x1 ? true : false;
}

bool SWControl::IS_CUM_FAIL_MASK(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_CUM_FAIL_MASK(board, dut_index);
}

bool SWControl::IS_CUM_FAIL_MASK(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_mask_fail == 0 ? true : m_mask_fail[board * m_bib_x * m_bib_y + dut] & 0x1 ? true : false;
}

bool SWControl::IS_STEP_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_STEP_FAIL(board, dut_index);
}

bool SWControl::IS_STEP_FAIL(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_step_fail == 0 ? true : m_step_fail[board * m_bib_x * m_bib_y + dut] & 0x1 ? true : false;
}

bool SWControl::IS_STEP_IO_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int io)
{
        unsigned int dut_index;
        x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
        y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
        xy_to_index(x_pos, y_pos, dut_index);
        return IS_STEP_IO_FAIL(board, dut_index, io);
}

bool SWControl::IS_STEP_IO_FAIL(unsigned int board, unsigned int dut, unsigned int io)
{
        board = board >= NSLOT ? NSLOT - 1 : board;
        dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
        io = io > m_bib_io_size ? m_bib_io_size - 1 : io;
        return m_step_io_fail == 0 ? true : m_step_io_fail[board * m_bib_x * m_bib_y + dut] & (0x1 << io) ? true : false;
}

unsigned int SWControl::GET_STEP_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
    unsigned int dut_index;
    x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
    y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
    xy_to_index(x_pos, y_pos, dut_index);
    return GET_STEP_FAIL(board, dut_index);
}

unsigned int SWControl::GET_STEP_FAIL(unsigned int board, unsigned int dut)
{
    board = board >= NSLOT ? NSLOT - 1 : board;
    dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
    return m_step_io_fail[board * m_bib_x * m_bib_y + dut];
}

bool SWControl::IS_CUM_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_CUM_FAIL_CHIP(board, dut_index, chip_index);
}

bool SWControl::IS_CUM_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	return m_cum_fail_chip == 0 ? true : m_cum_fail_chip[bd_index + dut_index + chip_index] & 0x1 ? true : false;
}

bool SWControl::IS_STEP_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_STEP_FAIL_CHIP(board, dut_index, chip_index);
}

bool SWControl::IS_STEP_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	return m_step_fail_chip == 0 ? true : m_step_fail_chip[bd_index + dut_index + chip_index] & 0x1 ? true : false;
}

bool SWControl::IS_TEST_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_TEST_FAIL(board, dut_index);
}

bool SWControl::IS_TEST_FAIL(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_test_fail == 0 ? true : m_test_fail[board * m_bib_x * m_bib_y + dut] & 0x1 ? true : false;
}

bool SWControl::IS_CUM_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_CUM_BLOCK_FAIL(board, dut_index, block);
}

bool SWControl::IS_CUM_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	return m_cum_fail_block == 0 ? true : GET_BIT2(m_cum_fail_block[bd_index + dut_index + block / 8], block % 8);
}

char SWControl::IS_CUM_BLOCK_FAIL8(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_CUM_BLOCK_FAIL8(board, dut_index, block);
}

char SWControl::IS_CUM_BLOCK_FAIL8(unsigned int board, unsigned int dut, unsigned int block8)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	return (m_cum_fail_block == 0) ? 0x0 : m_cum_fail_block[bd_index + dut_index + block8 / 8];
}

bool SWControl::IS_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_STEP_BLOCK_FAIL(board, dut_index, block);
}

bool SWControl::IS_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	return m_now_step_fail_block == 0 ? true : GET_BIT2(m_now_step_fail_block[bd_index + dut_index + block / 8], block % 8);
}

char SWControl::IS_STEP_BLOCK_FAIL8(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_STEP_BLOCK_FAIL8(board, dut_index, block);
}

char SWControl::IS_STEP_BLOCK_FAIL8(unsigned int board, unsigned int dut, unsigned int block)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	return m_now_step_fail_block == 0 ? 0x0 : m_now_step_fail_block[bd_index + dut_index + block / 8];
}

bool SWControl::IS_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_GT_STEP_BLOCK_FAIL(board, dut_index, block);
}

bool SWControl::IS_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	return m_get_step_fail_block == 0 ? true : GET_BIT2(m_get_step_fail_block[bd_index + dut_index + block / 8], block % 8);
}

bool SWControl::IS_INIT_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return IS_INIT_BLOCK_FAIL(board, dut_index, block);
}

bool SWControl::IS_INIT_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	return m_init_fail_block == 0 ? true : GET_BIT2(m_init_fail_block[bd_index + dut_index + block / 8], block % 8);
}

void SWControl::SET_PRE_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_PRE_FAIL(board, dut_index, value);
}

void SWControl::SET_PRE_FAIL(unsigned int board, unsigned int dut, bool value)
{
	unsigned int index = board * m_bib_x * m_bib_y + dut;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;

	if (m_init_fail) {
		m_init_fail[index] = value ? m_init_fail[index] | 0x1 : m_init_fail[index] & ~0x1;
	}
}

void SWControl::SET_CUM_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_CUM_FAIL(board, dut_index, value);
}

void SWControl::SET_CUM_FAIL(unsigned int board, unsigned int dut, bool value)
{
	unsigned int index = board * m_bib_x * m_bib_y + dut;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	m_use_mode |= 0x1;
	//	m_use_mode = m_use_mode == 0 ? 1 : m_use_mode;

	if (m_cum_fail) {
		m_cum_fail[index] = value ? m_cum_fail[index] | 0x1 : m_cum_fail[index] & ~0x1;
	}
}

void SWControl::SET_CUM_FAIL_MASK(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_CUM_FAIL_MASK(board, dut_index, value);
}

void SWControl::SET_CUM_FAIL_MASK(unsigned int board, unsigned int dut, bool value)
{
	unsigned int index = board * m_bib_x * m_bib_y + dut;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;

	if (m_mask_fail) {
		m_mask_fail[index] = value ? m_mask_fail[index] | 0x1 : m_mask_fail[index] & ~0x1;
	}
}

void SWControl::SET_STEP_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_STEP_FAIL(board, dut_index, value);
}

void SWControl::SET_STEP_FAIL(unsigned int board, unsigned int dut, bool value)
{
	unsigned int index = board * m_bib_x * m_bib_y + dut;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	m_use_mode |= 0x1;

	if (m_step_fail) {
		m_step_fail[index] = value ? m_step_fail[index] | 0x1 : m_step_fail[index] & ~0x1;
	}
}

void SWControl::SET_STEP_IO_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int value)
{
        unsigned int dut_index;
        x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
        y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
        xy_to_index(x_pos, y_pos, dut_index);
        SET_STEP_IO_FAIL(board, dut_index, value);
}

void SWControl::SET_STEP_IO_FAIL(unsigned int board, unsigned int dut, unsigned int value)
{
        unsigned int index = board * m_bib_x * m_bib_y + dut;
        board = board >= NSLOT ? NSLOT - 1 : board;
        dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
        if (m_step_io_fail) {
                m_step_io_fail[index] = m_step_io_fail[index] | value;
        }
}

void SWControl::SET_CUM_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_CUM_FAIL_CHIP(board, dut_index, chip_index, value);
}

void SWControl::SET_CUM_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index, bool value)
{
	unsigned int bd_index, dut_index, index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	index = bd_index + dut_index + chip_index;

	if (m_cum_fail_chip) {
		m_cum_fail_chip[index] = value ? m_cum_fail_chip[index] | 0x1 : m_cum_fail_chip[index] & ~0x1;
	}
}

void SWControl::SET_STEP_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_STEP_FAIL_CHIP(board, dut_index, chip_index, value);
}

void SWControl::SET_STEP_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index, bool value)
{
	unsigned int bd_index, dut_index, index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	index = bd_index + dut_index + chip_index;

	if (m_step_fail_chip) {
		m_step_fail_chip[index] = value ? m_step_fail_chip[index] | 0x1 : m_step_fail_chip[index] & ~0x1;
	}
}

void SWControl::SET_TEST_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_TEST_FAIL(board, dut_index, value);
}

void SWControl::SET_TEST_FAIL(unsigned int board, unsigned int dut, bool value)
{
	unsigned int index = board * m_bib_x * m_bib_y + dut;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;

	if (m_test_fail) {
		m_test_fail[index] = value ? m_test_fail[index] | 0x1 : m_test_fail[index] & ~0x1;
	}
}

void SWControl::SET_CUM_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_CUM_BLOCK_FAIL(board, dut_index, block, value);
}

void SWControl::SET_CUM_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	m_use_mode |= 0x2;

	if (m_cum_fail_block) {
		SET_BIT2(m_cum_fail_block[bd_index + dut_index + block / 8], block % 8, value);
	}
}

void SWControl::SET_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_STEP_BLOCK_FAIL(board, dut_index, block, value);
}

void SWControl::SET_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);
	m_use_mode |= 0x2;

	if (m_now_step_fail_block) {
		SET_BIT2(m_now_step_fail_block[bd_index + dut_index + block / 8], block % 8, value);
	}
}

void SWControl::SET_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_GT_STEP_BLOCK_FAIL(board, dut_index, block, value);
}

void SWControl::SET_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);

	if (m_get_step_fail_block) {
		SET_BIT2(m_get_step_fail_block[bd_index + dut_index + block / 8], block % 8, value);
	}
}

void SWControl::SET_INIT_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_INIT_BLOCK_FAIL(board, dut_index, block, value);
}

void SWControl::SET_INIT_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	block = block >= m_block_limit ? m_block_limit - 1 : block;
	bd_index = board * m_bib_x * m_bib_y * (m_block_limit / 8 + 1);
	dut_index = dut * (m_block_limit / 8 + 1);

	if (m_init_fail_block) {
		SET_BIT2(m_init_fail_block[bd_index + dut_index + block / 8], block % 8, value);
	}
}

int SWControl::CUM_BLOCK_FAIL_COUNT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return CUM_BLOCK_FAIL_COUNT(board, dut_index, mcp);
}

int SWControl::CUM_BLOCK_FAIL_COUNT(unsigned int board, unsigned int dut, unsigned int mcp)
{
	unsigned int block, count;
	unsigned int mcp_block_limit = m_block_limit / (m_mcp == 0 ? 1 : m_mcp);

	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	mcp = mcp >= m_mcp ? m_mcp - 1 : mcp;

	for (count = 0, block = 0; block < mcp_block_limit; block++) {
		unsigned int mcp_block = block + mcp_block_limit * mcp;

		if (IS_CUM_BLOCK_FAIL(board, dut, mcp_block)) {
			count++;
		}
	}

	return count;
}

int SWControl::STEP_BLOCK_FAIL_COUNT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return STEP_BLOCK_FAIL_COUNT(board, dut_index, mcp);
}

int SWControl::STEP_BLOCK_FAIL_COUNT(unsigned int board, unsigned int dut, unsigned int mcp)
{
	unsigned int block, count;
	unsigned int mcp_block_limit = m_block_limit / (m_mcp == 0 ? 1 : m_mcp);

	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	mcp = mcp >= m_mcp ? m_mcp - 1 : mcp;

	for (count = 0, block = 0; block < mcp_block_limit; block++) {
		unsigned int mcp_block = block + mcp_block_limit * mcp;

		if (IS_STEP_BLOCK_FAIL(board, dut, mcp_block)) {
			count++;
		}
	}

	return count;
}

int SWControl::CUM_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp,
		unsigned int index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return CUM_BLOCK_FAIL_ADDRESS(board, dut_index, mcp, index);
}

int SWControl::CUM_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int dut, unsigned int mcp, unsigned int index)
{
	unsigned int block, count;
	unsigned int mcp_block_limit = m_block_limit / (m_mcp == 0 ? 1 : m_mcp);

	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	mcp = mcp >= m_mcp ? m_mcp - 1 : mcp;

	for (count = 0, block = 0; block < mcp_block_limit; block++) {
		unsigned int mcp_block = block + mcp_block_limit * mcp;

		if (IS_CUM_BLOCK_FAIL(board, dut, mcp_block)) {
			count++;
		}

		if (index + 1 == count) {
			break;
		}
	}

	return block;
}

int SWControl::STEP_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp,
		unsigned int index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return STEP_BLOCK_FAIL_ADDRESS(board, dut_index, mcp, index);
}

int SWControl::STEP_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int dut, unsigned int mcp, unsigned int index)
{
	unsigned int block, count;
	unsigned int mcp_block_limit = m_block_limit / (m_mcp == 0 ? 1 : m_mcp);

	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	mcp = mcp >= m_mcp ? m_mcp - 1 : mcp;

	for (count = 0, block = 0; block < mcp_block_limit; block++) {
		unsigned int mcp_block = block + mcp_block_limit * mcp;

		if (IS_STEP_BLOCK_FAIL(board, dut, mcp_block)) {
			count++;
		}

		if (index + 1 == count) {
			break;
		}
	}

	return block;
}
/*------------------------------*/

/*------- READ FM DATA -------*/
unsigned char SWControl::TMP_FM_DATA(unsigned int board, unsigned int io, unsigned int index)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	io = io >= m_bib_io_count_per_1scan ? m_bib_io_count_per_1scan - 1 : io;
	return m_tmp_fm_data[board][io][index];
}

unsigned char SWControl::FM_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index,
		unsigned int byte_index)
{
	unsigned int dut_index = 0;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return FM_DATA(board, dut_index, chip_index, byte_index);
}

unsigned char SWControl::FM_DATA(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int byte_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	byte_index = byte_index >= MAX_NBYTE ? MAX_NBYTE - 1 : byte_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip * MAX_NBYTE;
	dut_index = dut * m_mcp * m_nchip * MAX_NBYTE;
	chip_index = chip_index * MAX_NBYTE;
	return m_read_fm_data == 0 ? 0 : m_read_fm_data[bd_index + dut_index + chip_index + byte_index];
}

void SWControl::SET_FM_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index,
		unsigned int byte_index, unsigned char value)
{
	unsigned int dut_index = 0;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_FM_DATA(board, dut_index, chip_index, byte_index, value);
}

void SWControl::SET_FM_DATA(unsigned int board, unsigned int dut, unsigned int chip_index,
		unsigned int byte_index, unsigned char value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	byte_index = byte_index >= MAX_NBYTE ? MAX_NBYTE - 1 : byte_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip * MAX_NBYTE;
	dut_index = dut * m_mcp * m_nchip * MAX_NBYTE;
	chip_index = chip_index * MAX_NBYTE;

	if (m_read_fm_data) {
		m_read_fm_data[bd_index + dut_index + chip_index + byte_index] = value;
	}
}
/*----------------------------*/

/*------- READ BIST DATA -------*/
char *SWControl::BIST_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return BIST_DATA(board, dut_index);
}

char *SWControl::BIST_DATA(unsigned int board, unsigned int dut)
{
	return m_bist[board][dut];
}
/*------------------------------*/

/*------- READ DQ DATA -------*/
int SWControl::DQ_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int dq_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return DQ_DATA(board, dut_index, dq_index);
}

int SWControl::DQ_DATA(unsigned int board, unsigned int dut, unsigned int dq_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	dq_index = dq_index >= MAX_DQ_VALUE ? MAX_DQ_VALUE - 1 : dq_index;
	bd_index = board * m_bib_x * m_bib_y * MAX_DQ_VALUE;
	dut_index = dut * MAX_DQ_VALUE;
	return m_dq_value == 0 ? 0 : m_dq_value[bd_index + dut_index + dq_index];
}
/*----------------------------*/

/*------- GET / SET CHIPID -------*/
char *SWControl::CHIPID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return CHIPID_V(board, dut_index, chip_index);
}

char *SWControl::CHIPID_V(unsigned int board, unsigned int dut, unsigned int chip_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	chip_index = chip_index * 64;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip * 64;
	dut_index = dut * m_mcp * m_nchip * 64;
	return m_chipid == 0 ? 0 : &m_chipid[bd_index + dut_index + chip_index];
}

int SWControl::WAFERID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return WAFERID_V(board, dut_index, chip_index);
}

int SWControl::WAFERID_V(unsigned int board, unsigned int dut, unsigned int chip_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	return m_waferid == 0 ? 0 : m_waferid[bd_index + dut_index + chip_index];
}

int SWControl::XPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return XPOS_V(board, dut_index, chip_index);
}

int SWControl::XPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	return m_xpos == 0 ? 0 : m_xpos[bd_index + dut_index + chip_index];
}

int SWControl::YPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return YPOS_V(board, dut_index, chip_index);
}

int SWControl::YPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;
	return m_ypos == 0 ? 0 : m_ypos[bd_index + dut_index + chip_index];
}

void SWControl::SET_CHIPID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, char *value)
{
	unsigned int dut_index = 0;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_CHIPID_V(board, dut_index, chip_index, value);
}

void SWControl::SET_CHIPID_V(unsigned int board, unsigned int dut, unsigned int chip_index, char *value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	chip_index = chip_index * 64;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip * 64;
	dut_index = dut * m_mcp * m_nchip * 64;

	if (m_chipid) {
		snprintf(&m_chipid[bd_index + dut_index + chip_index], 64, "%s", value);
	}
}

void SWControl::SET_WAFERID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, unsigned int value)
{
	unsigned int dut_index = 0;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_WAFERID_V(board, dut_index, chip_index, value);
}

void SWControl::SET_WAFERID_V(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;

	if (m_waferid) {
		m_waferid[bd_index + dut_index + chip_index] = value;
	}
}

void SWControl::SET_XPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, unsigned int value)
{
	unsigned int dut_index = 0;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_XPOS_V(board, dut_index, chip_index, value);
}

void SWControl::SET_XPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;

	if (m_xpos) {
		m_xpos[bd_index + dut_index + chip_index] = value;
	}
}

void SWControl::SET_YPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, unsigned int value)
{
	unsigned int dut_index = 0;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_YPOS_V(board, dut_index, chip_index, value);
}

void SWControl::SET_YPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int value)
{
	unsigned int bd_index, dut_index;
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	chip_index = chip_index >= (unsigned int) (m_mcp * m_nchip) ? m_mcp * m_nchip - 1 : chip_index;
	bd_index = board * m_bib_x * m_bib_y * m_mcp * m_nchip;
	dut_index = dut * m_mcp * m_nchip;

	if (m_ypos) {
		m_ypos[bd_index + dut_index + chip_index] = value;
	}
}
/*--------------------------------*/

/*------- CATEGORY & SORT -------*/

void SWControl::JUDGE_FORCE(unsigned int mode, unsigned int bin)
{
	bool is_fail = false;
	unsigned int dut_index;
	for(unsigned int board = 0; board < NSLOT; board++)
	{
		for(unsigned int x = 0; x < m_bib_x; x++)
		{
			for(unsigned int y = 0; y < m_bib_y; y++)
			{
				xy_to_index(x, y, dut_index);

				if(m_force_bin[board][dut_index]  == true)
				{
					continue;
				}

				is_fail = IS_STEP_FAIL(board, x, y);


				if(is_fail)
				{
					if(mode == FAIL)
					{
						SET_BIN(board, x, y, bin);
						m_force_bin[board][dut_index] = true;
					}
				}
				else
				{

					if(mode == PASS)
					{
						SET_BIN(board, x, y, bin);
						m_force_bin[board][dut_index] = true;
					}
				}
			}
		}
	}
}

bool SWControl::FCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int fail_cate)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return FCAT(board, dut_index, fail_cate);
}

bool SWControl::FCAT(unsigned int board, unsigned int dut, unsigned int fail_cate)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	fail_cate = fail_cate >= MAX_CATEGORY ? MAX_CATEGORY - 1 : fail_cate;
	return GET_BIT(m_fail_cate[board][dut], fail_cate);
}

bool SWControl::PCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int pass_cate)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return PCAT(board, dut_index, pass_cate);
}

bool SWControl::PCAT(unsigned int board, unsigned int dut, unsigned int pass_cate)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	pass_cate = pass_cate >= MAX_CATEGORY ? MAX_CATEGORY - 1 : pass_cate;
	return GET_BIT(m_pass_cate[board][dut], pass_cate);
}

unsigned int SWControl::BIN(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return BIN(board, dut_index);
}

unsigned int SWControl::BIN(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_bin[board][dut];
}

unsigned int SWControl::BIN_MARK(unsigned int board, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	return BIN_MARK(board, dut_index);
}

unsigned int SWControl::BIN_MARK(unsigned int board, unsigned int dut)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	return m_bin_mark[board][dut];
}

void SWControl::SET_FCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int fail_cate)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_FCAT(board, dut_index, fail_cate);
}

void SWControl::SET_FCAT(unsigned int board, unsigned int dut, unsigned int fail_cate)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	fail_cate = fail_cate >= MAX_CATEGORY ? MAX_CATEGORY - 1 : fail_cate;
	SET_BIT(m_fail_cate[board][dut], fail_cate, true);
}

void SWControl::SET_PCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int pass_cate)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_PCAT(board, dut_index, pass_cate);
}

void SWControl::SET_PCAT(unsigned int board, unsigned int dut, unsigned int pass_cate)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	pass_cate = pass_cate >= MAX_CATEGORY ? MAX_CATEGORY - 1 : pass_cate;
	SET_BIT(m_pass_cate[board][dut], pass_cate, true);
}

void SWControl::SET_BIN(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int bin)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_BIN(board, dut_index, bin);
}

void SWControl::SET_BIN(unsigned int board, unsigned int dut, unsigned int bin)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	if(m_force_bin[board][dut]  == true)
	{
		return;
	}
	m_bin[board][dut] = bin;
}

void SWControl::SET_BIN_MARK(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int value)
{
	unsigned int dut_index;
	x_pos = x_pos >= m_bib_x ? m_bib_x - 1: x_pos;
	y_pos = y_pos >= m_bib_y ? m_bib_y - 1: y_pos;
	xy_to_index(x_pos, y_pos, dut_index);
	SET_BIN_MARK(board, dut_index, value);
}

void SWControl::SET_BIN_MARK(unsigned int board, unsigned int dut, unsigned int value)
{
	board = board >= NSLOT ? NSLOT - 1 : board;
	dut = dut >= m_bib_x * m_bib_y ? m_bib_x * m_bib_y - 1 : dut;
	m_bin_mark[board][dut] = value;
}

void SWControl::SET_FAULTBIN(unsigned int bin)
{
	Send(BC_SETFAULTBIN, &bin, sizeof(unsigned int));
}

void SWControl::SET_STEP_FAULTBIN(unsigned int bin)
{
	TFAULTBIN fbin = {0, m_cur_step.step_no, bin, -1};
	Send(BC_SETSTEPFAULTBIN, &fbin, sizeof(TFAULTBIN));
}
/*-------------------------------*/

/*------- PPN (CAT) Chip -------*/
void SWControl::CAT_ACTION_CATE(int c, ...)
{
	va_list	ap;
	int p = c;

	memset(m_cat_action_cate, 0, sizeof(unsigned int) * 200);
	va_start(ap, c);

	for (unsigned int i = 0; i < 200 && p; i++) {
		m_cat_action_cate[i] = p;
		p = va_arg(ap, unsigned int);
	}

	va_end(ap);
	Send(BC_CAT_ACTION_CATE, m_cat_action_cate, sizeof(unsigned int) * 200);
}

void SWControl::CAT_LIMIT(int l, ...)
{
	va_list	ap;
	int p = l;

	memset(m_cat_limit, 0, sizeof(unsigned int) * 64);
	va_start(ap, l);

	for (unsigned int i = 0; i < 64 && p; i++) {
		m_cat_limit[i] = p;
		p = va_arg(ap, unsigned int);
	}

	va_end(ap);
	Send(BC_CAT_LIMIT, m_cat_limit, sizeof(unsigned int) * 64);
}

void SWControl::CAT_DECODE(unsigned int v, unsigned char bit, char *s, unsigned int len)
{
	unsigned int i, j;
	unsigned int tmp = 0;

	memset(s, 0, len);

	for (i = 0, j = 0; i < (unsigned int) bit; i += 6, j++) {
		tmp = (v & (0x3f << i)) >> i;
		s[bit / 6 - 1 - j] = m_cat_cdec[tmp];
	}
}

void SWControl::CAT_CHIP_DATA(unsigned int libno, unsigned int length, const char *data_type)
{
	unsigned int i, j;
	int x_value = 0, y_value = 0, wafer_value = 0;
	unsigned int x_pos = 0, y_pos = 0, wafer = 0;
	unsigned int lotid1 = 0, lotid2 = 0;
	char tbuf[32];
	String lot;
	//	char msg[256];

	m_cat_chipid_ver = libno;

	for (i = 0; i < NSLOT; i++) {
		if (!_bi_find[i].read) {
			continue;
		}

		for (j = 0; j < (unsigned int) (m_bib_x * m_bib_y); j++) {
			if ((m_init_fail[i * m_bib_x * m_bib_y + j] & 1)) {
				continue;
			}

			x_pos = y_pos = wafer = lotid1 = lotid2 = 0;
			//	sprintf(msg, "b : %d, index : %d, v : %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
			//			i, j,
			//			m_bist[i][j][0], m_bist[i][j][1], m_bist[i][j][2], m_bist[i][j][3], m_bist[i][j][4],
			//			m_bist[i][j][5], m_bist[i][j][6], m_bist[i][j][7], m_bist[i][j][8], m_bist[i][j][9],
			//			m_bist[i][j][10], m_bist[i][j][11], m_bist[i][j][12], m_bist[i][j][13], m_bist[i][j][14],
			//			m_bist[i][j][15], m_bist[i][j][16], m_bist[i][j][17]);
			//	Send(BC_PRINTF, msg, strlen(msg));

			if (m_cat_chipid_ver == 0) {
				y_pos |= (m_bist[i][j][7] & 0xf);
				y_pos |= (m_bist[i][j][8] & 0xff) << 4;
				y_pos |= (m_bist[i][j][9] & 0x3f) << 12;
				x_pos |= (m_bist[i][j][9] & 0xc0) >> 6;
				x_pos |= (m_bist[i][j][10] & 0xff) << 2;
				x_pos |= (m_bist[i][j][11] & 0xff) << 10;
				wafer |= (m_bist[i][j][12] & 0xff);
				wafer |= (m_bist[i][j][13] & 0xf) << 8;
				lotid1 |= (m_bist[i][j][13] & 0xf0) >> 4;
				lotid1 |= (m_bist[i][j][14] & 0xff) << 4;
				lotid2 |= (m_bist[i][j][15] & 0xff);
				lotid2 |= (m_bist[i][j][16] & 0xff) << 8;
				lotid2 |= (m_bist[i][j][17] & 0xff) << 16;
				CAT_DECODE(y_pos, 18, tbuf, sizeof(tbuf));
				y_value = CID_Y_CAL(atoi(tbuf));
				CAT_DECODE(x_pos, 18, tbuf, sizeof(tbuf));
				x_value = CID_X_CAL(atoi(tbuf));
				CAT_DECODE(wafer, 12, tbuf, sizeof(tbuf));
				wafer_value = CID_WAFER_CAL(atoi(tbuf));
				CAT_DECODE(lotid1, 12, tbuf + 4, 2);
				CAT_DECODE(lotid2, 24, tbuf, 4);
				lot = CID_LOTID_CAL(tbuf);
			}
			else if (m_cat_chipid_ver == 1) {
				y_pos |= (m_bist[i][j][8] & 0xff);
				y_pos |= (m_bist[i][j][9] & 0xff) << 8;
				y_pos |= 0x3 << 16;
				x_pos |= (m_bist[i][j][10] & 0xff);
				x_pos |= (m_bist[i][j][11] & 0xff) << 8;
				x_pos |= 0x3 << 16;
				wafer |= (m_bist[i][j][12] & 0xff);
				wafer |= (m_bist[i][j][13] & 0xf) << 8;
				lotid1 |= (m_bist[i][j][13] & 0xf0) >> 4;
				lotid1 |= (m_bist[i][j][14] & 0xff) << 4;
				lotid2 |= (m_bist[i][j][15] & 0xff);
				lotid2 |= (m_bist[i][j][16] & 0xff) << 8;
				lotid2 |= (m_bist[i][j][17] & 0xff) << 16;
				CAT_DECODE(y_pos, 18, tbuf, sizeof(tbuf));
				y_value = CID_Y_CAL(atoi(tbuf));
				CAT_DECODE(x_pos, 18, tbuf, sizeof(tbuf));
				x_value = CID_X_CAL(atoi(tbuf));
				CAT_DECODE(wafer, 12, tbuf, sizeof(tbuf));
				wafer_value = CID_WAFER_CAL(atoi(tbuf));
				CAT_DECODE(lotid1, 12, tbuf + 4, 2);
				CAT_DECODE(lotid2, 24, tbuf, 4);
				lot = CID_LOTID_CAL(tbuf);
			}

			*(unsigned int *) &m_bist[i][j][512] = m_cat_chipid_ver;
			*(unsigned int *) &m_bist[i][j][516] = length;
			snprintf(&m_bist[i][j][520], 64, "%s", data_type);
			snprintf(&m_bist[i][j][1024], 64, "%s", (const char *) lot);
			*(int *) &m_bist[i][j][1088] = x_value;
			*(int *) &m_bist[i][j][1092] = y_value;
			*(int *) &m_bist[i][j][1096] = wafer_value;
			//	sprintf(msg, "x : %d, y : %d, w : %d, l : %s", x_value, y_value, wafer_value, (const char *) lot);
			//	Send(BC_PRINTF, msg, strlen(msg));
			//	sprintf(msg, "x : %d, y : %d, w : %d, l : %s",
			//			*(int *) &m_bist[i][j][1088], *(int *) &m_bist[i][j][1092],
			//			*(int *) &m_bist[i][j][1096], &m_bist[i][j][1024]);
			//	Send(BC_PRINTF, msg, strlen(msg));
			Send(BC_BISTDATA3, m_bist[i][j], sizeof(char) * (MAX_BIST_SIZE + 2));
		}

		for (j = 0; j < MAX_NAND_CHIP; j++) {
			memset(m_bist[i][j], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}
}
/*------------------------------*/

/*------- IO_DEFINE -------*/
unsigned int SWControl::IO_DEFINE_FAIL_DEVICE(unsigned int fail_device)
{
	unsigned int ret = 0;

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		if (m_io_define[i] == 0) {
			continue;
		}

		ret |= (fail_device >> i & 1) << (m_io_define[i] - 1);
	}

	return ret;
}

unsigned int SWControl::IO_DEFINE_FAIL_DEVICE_REVERSE(unsigned int fail_device)
{
	unsigned int ret = 0;

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		if (m_io_define[i] == 0) {
			continue;
		}

		ret |= (fail_device >> (m_io_define[i] - 1) & 1) << i;
	}

	return ret;
}

unsigned long long SWControl::IO_DEFINE_FAIL_LATCH(unsigned long long fail_dq)
{
	unsigned long long ret = 0;
	unsigned int size = 64 / IO_DEFINE_BUFFER_LENGTH;

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		if (m_io_define[i] == 0) {
			continue;
		}

		ret |= (fail_dq >> (i * size) & 0xff) << ((m_io_define[i] - 1) * size);
	}

	return ret;
}

void SWControl::IO_DEFINE_FAIL_DATA(unsigned char *result, unsigned char *fail_data)
{
	memset(result, 0, IO_DEFINE_BUFFER_LENGTH);

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		if (m_io_define[i] == 0) {
			continue;
		}

		result[m_io_define[i] - 1] = fail_data[i];
	}
}

void SWControl::IO_DEFINE_FAIL_DATA_ADDR(unsigned char *result_data, unsigned char *fail_data,
		unsigned short *result_addr, unsigned short *fail_addr)
{
	memset(result_data, 0, IO_DEFINE_BUFFER_LENGTH);
	memset(result_addr, 0, sizeof(unsigned short) * IO_DEFINE_BUFFER_LENGTH);

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		if (m_io_define[i] == 0) {
			continue;
		}

		result_data[m_io_define[i] - 1] = fail_data[i];
		result_addr[m_io_define[i] - 1] = fail_addr[i];
	}
}

void SWControl::IO_DEFINE_FAIL_COUNTER(unsigned int *result, unsigned int *fail_counter)
{
	memset(result, 0, IO_DEFINE_BUFFER_LENGTH * sizeof(unsigned int));

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		if (m_io_define[i] == 0) {
			continue;
		}

		result[m_io_define[i] - 1] = fail_counter[i];
	}
}

void SWControl::SET_IO_DEFINE(unsigned int io_group0, unsigned int io_group1, unsigned int io_group2, unsigned int io_group3,
		unsigned int io_group4, unsigned int io_group5, unsigned int io_group6, unsigned int io_group7)
{
	m_io_define[0] = io_group0;
	m_io_define[1] = io_group1;
	m_io_define[2] = io_group2;
	m_io_define[3] = io_group3;
	m_io_define[4] = io_group4;
	m_io_define[5] = io_group5;
	m_io_define[6] = io_group6;
	m_io_define[7] = io_group7;
	m_use_io_count = 0;

	for (unsigned int i = 0; i < IO_DEFINE_BUFFER_LENGTH; i++) {
		m_use_io_count = m_io_define[i] > 0 ? m_use_io_count + 1 : m_use_io_count;
	}
}
/*-------------------------*/

void SWControl::BIB(int d, ...)
{
	va_list	ap;
	int p = d;
	int x, y;
	unsigned int scan, io;
	unsigned int value;

	memset(m_index_si_map, 0, m_bib_x * m_bib_y * sizeof(int));
	memset(m_si_index_map, -1, m_bib_scan * m_bib_io_count_per_1scan * sizeof(int));
	va_start(ap, d);

	for (y = m_bib_y - 1; y >= 0; y--) {
		for (x = 0; (unsigned int) x < m_bib_x; x++) {
			if (p == 0) {
				break;
			}

			scan = (p >> 16) & 0xff;
			io = (p >> 24) & 0xff;
			value = (p & 0xffff) - 1;

			if (scan == 0 && io == 0) {
				mapindex_to_si_raw(value, scan, io);
			}
			else {
				io--;
				scan--;
			}

			if (m_bib_x * m_bib_y > y * m_bib_x + x) {
				m_index_si_map[y * m_bib_x + x] = io << 16 | scan;
			}

			if (m_bib_scan * m_bib_io_count_per_1scan > io * m_bib_scan + scan) {
				m_si_index_map[io * m_bib_scan + scan] = y * m_bib_x + x;
			}

			p = va_arg(ap, int);
		}
	}

	va_end(ap);
	int size[5] = { m_bib_scan, m_bib_io_count_per_1scan, m_bib_x, m_bib_y, m_16io_mode };
	Send(BC_GETBIB, &size, sizeof(size));
	Send(BC_INDEX_SI_MAP, m_index_si_map, sizeof(int) * m_bib_x * m_bib_y);
	Send(BC_SI_INDEX_MAP, m_si_index_map, sizeof(int) * m_bib_scan * m_bib_io_count_per_1scan);
}

void SWControl::SET_BIB(unsigned int scan, unsigned int io, unsigned int bibx, unsigned int biby, bool f_16mode)
{
	SET_BIB_EX(scan, io, 64u / io, bibx, biby, f_16mode);
}

void SWControl::SET_BIB_EX(unsigned int scan, unsigned int io, unsigned int io_count_per_1scan,
		unsigned int bibx, unsigned int biby, bool f_16mode)
{
	unsigned int b;
	unsigned int x;
	unsigned int y;
	unsigned int set;

	m_bib_scan = scan;
	m_bib_io_size = io;
	m_bib_io_count_per_1scan = io_count_per_1scan;
	m_bib_x = bibx == 0 ? scan : bibx;
	m_bib_y = biby == 0 ? io_count_per_1scan : biby;
	m_16io_mode = f_16mode;

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].find_drv) {
			FIND_BOARD(b);
		}
	}

	if (m_index_map) {
		delete[] m_index_map;
		m_index_map = 0;
	}

	if (m_index_si_map) {
		delete[] m_index_si_map;
		m_index_si_map = 0;
	}

	if (m_si_index_map) {
		delete[] m_si_index_map;
		m_si_index_map = 0;
	}

	m_index_map = new int[m_bib_x * m_bib_y];
	m_index_si_map = new int[m_bib_x * m_bib_y];
	m_si_index_map = new int[m_bib_scan * m_bib_io_count_per_1scan];
	memset(m_index_map, 0, m_bib_x * m_bib_y * sizeof(int));
	memset(m_index_si_map, 0, m_bib_x * m_bib_y * sizeof(int));
	memset(m_si_index_map, -1, m_bib_scan * m_bib_io_count_per_1scan * sizeof(int));

	for (set = 0, y = 0; y < m_bib_io_count_per_1scan; y++) {
		for (x = 0; x < m_bib_scan; x++) {
			if (m_bib_x * m_bib_y > set) {
				m_index_si_map[set] = y << 16 | x;
			}

			set++;
		}
	}

	for (set = 0, y = 0; y < m_bib_y; y++) {
		for (x = 0; x < m_bib_x; x++) {
			index_to_si(set, scan, io);

			if (m_bib_scan * m_bib_io_count_per_1scan > io * m_bib_scan + scan) {
				m_si_index_map[io * m_bib_scan + scan] = set;
			}

			m_index_map[set] = y << 16 | x;
			set++;
		}
	}

	if (m_init_fail == 0) {
		m_init_fail = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
	}

	if (m_pre_init_fail == 0) {
		m_pre_init_fail = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_pre_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
	}

	if (m_cum_fail == 0) {
		m_cum_fail = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_cum_fail, 0, m_bib_x * m_bib_y * NSLOT);
	}

	if (m_mask_fail == 0) {
		m_mask_fail = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_mask_fail, 0, m_bib_x * m_bib_y * NSLOT);
	}

	if (m_step_fail == 0) {
		m_step_fail = new char[m_bib_x * m_bib_y * NSLOT + 5];
		memset(m_step_fail, 0, m_bib_x * m_bib_y * NSLOT + 5);
	}

	if (m_step_io_fail == 0) {
                m_step_io_fail = new unsigned int[m_bib_x * m_bib_y * NSLOT + 5];
		memset(m_step_io_fail, 0, sizeof(unsigned int) * (m_bib_x * m_bib_y * NSLOT + 5));
        }
	
	if (m_cum_fail_chip != 0) {
		delete[] m_cum_fail_chip;
	}

	m_cum_fail_chip = new char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
	memset(m_cum_fail_chip, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);

	/*	if (m_cum_fail_chip == 0) {
		m_cum_fail_chip = new char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
		memset(m_cum_fail_chip, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);
		}*/

	if (m_step_fail_chip != 0) {
		delete[] m_step_fail_chip;
	}

	m_step_fail_chip = new char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
	memset(m_step_fail_chip, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);

	/*	if (m_step_fail_chip == 0) {
		m_step_fail_chip = new char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
		memset(m_step_fail_chip, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);
		}*/

	if (m_test_fail == 0) {
		m_test_fail = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_test_fail, 0, m_bib_x * m_bib_y * NSLOT);
	}

	if (m_test_io_data == 0) {
		m_test_io_data = new unsigned int[m_bib_x * m_bib_y * NSLOT];
		memset(m_test_io_data, 0, sizeof(unsigned int) * m_bib_x * m_bib_y * NSLOT);
	}

	if (m_cum_fail_block == 0) {
		m_cum_fail_block = new char[m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1)];
		memset(m_cum_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
	}

	if (m_now_step_fail_block == 0) {
		m_now_step_fail_block = new char[m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1)];
		memset(m_now_step_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
	}

	if (m_get_step_fail_block == 0) {
		m_get_step_fail_block = new char[m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1)];
		memset(m_get_step_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
	}

	if (m_init_fail_block == 0) {
		m_init_fail_block = new char[m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1)];
		memset(m_init_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
	}

	/*	if (m_read_fm_data != 0) {
		delete[] m_read_fm_data;
		}

		m_read_fm_data = new unsigned char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * MAX_NBYTE];
		memset(m_read_fm_data, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * MAX_NBYTE);
		*/	
	if (m_read_fm_data == 0) {
		m_read_fm_data = new unsigned char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * MAX_NBYTE];
		memset(m_read_fm_data, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * MAX_NBYTE);
	}

	if (m_dq_value == 0) {
		m_dq_value = new unsigned int[m_bib_x * m_bib_y * NSLOT * MAX_DQ_VALUE];
		memset(m_dq_value, 0, m_bib_x * m_bib_y * NSLOT * MAX_DQ_VALUE * sizeof(unsigned int));
	}

	if (m_dq_data != 0) {
		delete[] m_dq_data;
	}

	m_dq_data = new unsigned long long[m_bib_scan * NSLOT];
	memset(m_dq_data, 0, m_bib_scan * sizeof(unsigned long long) * NSLOT);

	if (m_emmc_offset == 0) {
		m_emmc_offset = new unsigned int[m_bib_x * m_bib_y * NSLOT];
		memset(m_emmc_offset, 0, m_bib_x * m_bib_y * NSLOT * sizeof(unsigned int));
	}

	if (m_ecc_block_fail == 0) {
		m_ecc_block_fail = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_ecc_block_fail, 0, m_bib_x * m_bib_y * NSLOT);
	}

	if (m_failblock == 0) {
		m_failblock = new FailBlock[m_bib_x * m_bib_y * NSLOT];
		memset(m_failblock, 0, m_bib_x * m_bib_y * NSLOT);
	}

	int size[5] = { m_bib_scan, m_bib_io_count_per_1scan, m_bib_x, m_bib_y, m_16io_mode };
	Send(BC_GETBIB, &size, sizeof(size));
	Send(BC_INDEX_SI_MAP, m_index_si_map, sizeof(int) * m_bib_x * m_bib_y);
	Send(BC_SI_INDEX_MAP, m_si_index_map, sizeof(int) * m_bib_scan * m_bib_io_count_per_1scan);
}

void SWControl::SET_IOMASK(long long a, long long b)
{
	if (b == -1) {
		b = a;
	}

	m_comp_io_mask.da = a;
	m_comp_io_mask.db = b;

	Write(0x60, a & 0xffff);
	Write(0x61, (a >> 16) & 0xffff);
	Write(0x62, (a >> 32) & 0xffff);

	Write(0x68, b & 0xffff);
	Write(0x69, (b >> 16) & 0xffff);
	Write(0x6a, (b >> 32) & 0xffff);
}

void SWControl::SET_LIMIT(unsigned int bl, unsigned int bfl, unsigned int bal)
{
	unsigned int l[3] = { bl, bfl, bal };
	m_block_limit = bl > 0x30000 ? 0x30000 : bl;
	m_block_fail_limit = bfl;
	m_block_add_limit = bal;
	Send(BC_FAILLIMIT, l, sizeof(l));
}

void SWControl::SET_ADD_SIZE(unsigned int block_shift, unsigned int page_shift, unsigned int page_div,
		unsigned int mcp, unsigned int nchip)
{
	unsigned int tmp[5] = { block_shift, page_shift, page_div, mcp, nchip };
	m_block_shift = block_shift;
	m_page_shift = page_shift;
	m_page_div = page_div;
	m_mcp = mcp > 8 ? 8 : mcp;
	m_nchip = nchip;
	Send(BC_ADDSIZE, tmp, sizeof(tmp));
}

void SWControl::SET_JUDGE(int rack)
{
	unsigned int b;

	for (b = 0; b < NSLOT; b++) {
		if ((rack >> b) & 1) {
			_bi_find[b].read = 1;
		}
		else {
			_bi_find[b].read = 0;
		}
	}
}
	
int SWControl::confirm_fail()
{
	int b, i, tmp[8] = { 0 };

	memset(m_icount, 0, sizeof(m_icount));
	memset(m_tcount, 0, sizeof(m_tcount));
	for (b = 0; b < 8; b++) 
	{
		for (i = 0; i < m_bib_x * m_bib_y; i++) 
		{
			if (!_bi_find[b].read)
			{
				continue;
			}

			if (!(m_init_fail[b * m_bib_x * m_bib_y + i] & 1))
			{
				m_icount[b]++;
			}
		}
	}

	for (b = 0; b < 8; b++) 
	{
		for (i = 0; i < m_bib_x * m_bib_y; i++) 
		{
			if (!_bi_find[b].read)
			{
				continue;
			}

			if (!(m_init_fail[b * m_bib_x * m_bib_y + i] & 1) && !(m_test_fail[b * m_bib_x * m_bib_y + i] & 1))
			{
				m_tcount[b]++;
			}
		}
	}

	if (m_ocount[0] == -1)
	{
		memmove(m_ocount, m_icount, sizeof(m_icount));
	}

	memmove(tmp, m_tcount, sizeof(m_tcount));
	memmove(m_tcount, m_ocount, sizeof(m_ocount));
	memmove(m_ocount, tmp, sizeof(tmp));

	if (!m_use_mask)
	{
		memmove(m_tcount, m_icount, sizeof(m_icount));
	}

	return 0;
}



void SWControl::SET_DEVICE(int mode, unsigned short device)
{
	sysMuxMode(mode);
	m_device_mode = mode;
	m_device_code = device;

	if (mode == EMMC) {
		for (unsigned int i = 0; i < NSLOT; i++) {
			for (unsigned int j = 0; j < MAX_NAND_CHIP; j++) {
				if (m_bist[i][j] == 0) {
					m_bist[i][j] = new char[MAX_BIST_SIZE + 2];
				}

				memset(m_bist[i][j], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
			}
		}
	}

	int m[2] = { mode, device };
	Send(BC_SETDEVICE, m, sizeof(m));
}

bool SWControl::SET_BOARD(int b, bool keep_level)
{
	unsigned short board = 0xff;
	unsigned short level = 0;

	if (b != -1) {
		board = 1 << b;
	}

	for (unsigned int i = 0; i < NSLOT; i++) {
		setReadyBusy(i, 0);

		if (!_bi_find[i].read) {
			board &= ~(1 << i);
		}
	}

	for (unsigned int i = 0; i < NSLOT; i++) {
		if ((board >> i) & 1) {
			setReadyBusy(i, m_ready_bit);
		}
	}

	if (keep_level) {
		level = Read(0x5);
	}

	Write(0x4, ~board);
	setDriver(board);
	m_board_index = b;

	if (keep_level) {
		setLevel(0x1);
		Write(6, 0x400);
		sysDelay(10 MS);
	}
	if(m_isnew)
	{
		for(unsigned int i = 0; i < NSLOT; i++)
		{
			if((board >> i) & 0x1)
			{
				m_PSManager.setGLOBALSLOTUse(i, true);
				m_bdbit |= 0x1 << i;
			}	
			else
			{
				m_PSManager.setGLOBALSLOTUse(i, false);
				m_bdbit &= ~(0x1 << i);
			}

		}
	}

	return board ? true : false;
} 

void SWControl::SET_VLOOP_COUNT(unsigned int index, unsigned int v, ...)
{
	va_list ap;
	int p = v;
	int set = index;

	va_start(ap, v);

	while (p != -1) {
		setVloop(set++, p);
		p = va_arg(ap, unsigned int);
	}

	va_end(ap);
}

void SWControl::SET_VLOOP_COUNT_DATA(unsigned int *s, unsigned int size, unsigned int index)
{
	unsigned int i;

	for (i = 0; i < size / sizeof(unsigned int); i++) {
		setVloop(i + index, s[i]);
	}
}

void SWControl::SET_VECTOR(unsigned int index, unsigned v, ...)
{
	va_list ap;
	int p = v;
	int set = index;
	unsigned short val = Read(0x4062);

	Write(0x4062, (val & 0xf) | 0x10);
	va_start(ap, v);

	while (p != -1) {
		setVector(set++, p);
		p = va_arg(ap, unsigned int);
	}

	va_end(ap);
}

void SWControl::SET_VECTOR_DATA(unsigned int *s, unsigned int size, unsigned int index)
{
	unsigned int i;
	unsigned short val = Read(0x4062);

	Write(0x4062, (val & 0xf) | 0x10);

	for (i = 0; i < size / sizeof(unsigned int); i++) {
		setVector(i + index, s[i]);
	}
}

void SWControl::SET_VECTOR_OFF()
{
	unsigned short val = Read(0x4062);
	Write(0x4062, (val & 0xf) | 0x0);
}

void SWControl::SET_VECTOR_EMUL(unsigned int v, ...)
{
	va_list ap;
	int p = v;
	int set = 0, mode;
	unsigned short val = Read(0x4003);

	val = val & 0xff00;
	sysWrite(0x4003, val);
	mode = sysRead(0x4022);
	sysWrite(0x4022, (mode & ~0x20) | 1);
	mode = sysRead(0x4042);
	sysWrite(0x4042, (mode & ~0x20) | 1);
	va_start(ap, v);

	while (p != -1) {
		setVectorEmul(set++, p);
		p = va_arg(ap, unsigned int);
	}

	va_end(ap);
}

void SWControl::SET_VECTOR_EMUL_DATA(unsigned int *s, unsigned int size)
{
	int mode, i;
	unsigned short val = Read(0x4003);

	val = val & 0xff00;
	sysWrite(0x4003, val);
	mode = sysRead(0x4022);
	sysWrite(0x4022, (mode & ~0x20) | 1);
	mode = sysRead(0x4042);
	sysWrite(0x4042, (mode & ~0x20) | 1);

	for (i = 0; i < (int) (size / sizeof(unsigned int)); i++) {
		setVectorEmul(i, s[i]);
	}
}

void SWControl::SET_VECTOR_EMUL_OFF()
{
}

void SWControl::SET_SCAN(int scan, ...)
{
	va_list	ap;
	int p = scan;
	int all_scan = 0;
	int set = 0;

	va_start(ap, scan);

	while (p) {
		all_scan |= p;
		setScanVector(set++, p);
		p = va_arg(ap, int);
	}

	setScanMode(0, all_scan);
	va_end(ap);
	setScanLoop(set - 1);
}

void SWControl::SET_READ_FAULT_TIMER(unsigned long long sec)
{
	m_fault_timer = sec * 1000000;
}

void SWControl::SET_REV()
{
	int rev = 0;
	rev |= ((int)_MAJOR_REV1 << 8);
	rev |= ((int)_MAJOR_REV2 << 4);
	rev |= (int)_MINOR_REV;
	Write(0x4000, rev);
}

void SWControl::SET_PS(int ch, double voltage, double delay, double high, double low, double c_limit)
{	
	if( m_shmoo_test == true ) {
		if( ch == m_shmoo_save_ch[0] )
			voltage = (double) m_shmoo_save_data[0];
		else if( ch == m_shmoo_save_ch[1] )
			voltage = (double) m_shmoo_save_data[1];
	}
	
	unsigned int i;
	bool benable = false, boff = false;
	bool bcommon = false;
	bool bcommon2 = false;
	char msg[40];
	readPSCommon(bcommon, bcommon2);
	//printf("SET_PS : ch : %d voltage : %f high : %f low : %f c_limit %f", ch, voltage, high, low, c_limit);
	if(m_isnew)
	{
		if (((bcommon == true) && (ch == 3)) || ((bcommon2 == true) && (ch == 4)))
		{
			sprintf(msg, "Invalid PS Channel!!! PS[%d]", ch + 1);
			runtimeError(msg);
			return;
		}
	}
	else
	{
		if (bcommon == true && (ch == 3 || ch == 4))
		{
			sprintf(msg, "Invalid PS Channel!!! PS[%d]", ch + 1);
			runtimeError(msg);
			return;
		}
	}

	if (ch == 2) {
		SET_PS3(voltage, 600 MS, delay, high, low, c_limit);
		return;
	}

	if (m_ps1 == false && m_ps2 == false && m_ps4 == false && m_ps5 == false) {
		SET_VIL(VIL_CLK, 0.0 V, 000 MS);
		SET_VIL(VIL_CLK6, 0.0 V, 000 MS);
		SET_VIL(VIL_CLK7, 0.0 V, 000 MS);
		SET_VIL(VIL_ADD8, 0.0 V, 000 MS);
		SET_VIL(VIL_ADD9, 0.0 V, 000 MS);
		SET_VIL(VIL_SCAN0, 0.0 V, 000 MS);
		SET_VIL(VIL_SCAN1, 0.0 V, 000 MS);
	}

	sort_channel(ch, voltage, high, low, c_limit);
	int enbit = 0;
	int flag_bit = check_psflag(bcommon, bcommon2, enbit);

	if(flag_bit & 0x1)
	{
		benable = true;
	}
	if(flag_bit & 0x2)
	{
		boff = true;
	}

	//printf("benable : %d boff : %d enbit :0x%x", benable, boff,enbit);

	if (benable == false) {
		VOLTINFO vi = { ch, voltage, delay, high, low, c_limit };
		Send(BC_SETVOLT, &vi, sizeof(VOLTINFO));
		sysDelay(delay);
		return;
	}

	m_ps1 = m_ps2 = m_ps4 = m_ps5 = false;
	sysDelay(600 MS);	// HW 요청으로 변경

	if (boff == false) {
		for (i = 0; i < NSLOT; i++) {
			if (!_bi_find[i].read) {
				continue;
			}

			Write(0x5 + i * 0x100, enbit);
			sysDelay(5 MS);

			if(!m_isnew)
			{
				Write(0x5 + i * 0x100, 0x400);
				sysDelay(20 MS);
			}
		}
	}
	else {

		if(!m_isnew)
		{
			for (i = 0; i < NSLOT; i++) {
			Write(0x6 + i * 0x100, 0x400);
			sysDelay(5 MS);

			Write(0x6 + i * 0x100, enbit);
			sysDelay(5 MS);
			}
		}
	}
	set_da();

	VOLTINFO vi = { ch, voltage, delay, high, low, c_limit };
	Send(BC_SETVOLT, &vi, sizeof(VOLTINFO));
	sysDelay(delay);
}

void SWControl::write_i2cid(int ps, bool Merge)
{
	int read_v = 0;
	for(unsigned int i = 0; i < NSLOT; i++)
	{
		if(_bi_find[i].find_drv)
		{
			read_v = Read(0x2 + i * 0x100) & 0xc000;
			break;
		}
	}
	//printf("ps[0x%x] i2cid[0x%x]", ps, i2cid);

	Write(0x2, read_v | ps);
	m_PSManager.setPSUse(ps);
	m_PSManager.setPSCHMergeUse(Merge);
	int iRet = m_PSManager.setPSSelect();
	if( !iRet)
	{
		printf( "[ERROR] I2C INIT setI2CID Error : [0x%x] %s\n", iRet,  m_PSManager.error());
		return;
	}

}

void SWControl::i2c_init()
{
	if(m_isnew == false)
	{
		return;
	}

	unsigned int islot;

	m_PSManager.setGLOBALWRITEUse( true );
	m_PSManager.setGLOBALSLOTCount( 8 );
	m_PSManager.setBaseAdd( 0x0 );

	for(islot = 0; islot < NSLOT; islot++ )
	{
		if(!_bi_find[islot].find_drv)
		{
			continue;
		}
		m_PSManager.setGLOBALSLOTUse(islot, true);
		m_PSManager.setGLOBALSLOTADDBit( islot, islot * 0x100 );
	}

	for(int ps = 0; ps < 5; ps++)
	{
		write_i2cid(0x1 << ps, false);
		m_PSManager.setVOUTMODE(0x14);
	}

	/*
	   for( i = 0; i < NSLOT; i++ )
	   {
	   m_pmbus
	   .setSLOT( i );

	   if( m_pmbus.readREVISION( usData ) != PMUBS_CMD_SUCCESS )
	   {
	   printf( "[ERROR] SLOT[%d] PS[%d] READ REVISION Error!\n", i + 1, PS );
	   }
	   }
	   */
}
void SWControl::set_i2c(double voltage, bool is_up, int ps, double high, double low, double c_limit)
{
	if(m_isnew == false)
	{
		return;
	}
	float volt_set = (float) voltage;
	int ps_index = readPSIndex(ps);
	high = high == 0 ? voltage * 1.35 : high;
	low = low == 0? 0.3 : low;
	c_limit = c_limit == 0? 25 : c_limit; 
	int iRet;
	char msg[100];

	write_i2cid(ps, true);

	iRet =  m_PSManager.setOCFAULTLIMIT((float) c_limit); 
	if( iRet != PMBUS_CMD_SUCCESS )
	{
		printf("[ERROR] PS[%d] setOCFAULTLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
	}

	write_i2cid(ps, true);
	iRet = m_PSManager.setOVWARNLIMIT((float)high - 0.1);
	if( iRet != PMBUS_CMD_SUCCESS )
	{
		printf("[ERROR] PS[%d] setOVWARNLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
	}

	write_i2cid(ps, true);
	iRet = m_PSManager.setUVWARNLIMIT((float)low + 0.1);
	if( iRet != PMBUS_CMD_SUCCESS )
	{
		printf("[ERROR] PS[%d] setUVWARNLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
	}
	//if( m_pmbus.setVOUTMHIGH(0, volt_set * 1.05) != PMBUS_CMD_SUCCESS )
	//    printf( "[ERROR] PS[%d] setVOUTMHIGH Error\n", ps_index );

	//if( m_pmbus.setVOUTMLOW(0, volt_set * 0.9) != PMBUS_CMD_SUCCESS )
	//  printf( "[ERROR] PS[%d] setVOUTMLOW Error\n", ps_index );


	if ( is_up == true ) {  //vbump set!!!
		write_i2cid(ps, true);
		iRet = m_PSManager.setOVFAULTLIMIT((float)high);
		if( iRet != PMBUS_CMD_SUCCESS )
		{
			printf("[ERROR] PS[%x] setOVFAULTLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
		}

		write_i2cid(ps, true);
		iRet = m_PSManager.setVOUTCOMMAND(volt_set);
		if( iRet != PMBUS_CMD_SUCCESS )
		{
			printf("[ERROR] PS[%x] setOVOUTCOMMAND Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
		}

		write_i2cid(ps, true);
		iRet = m_PSManager.setUVFAULTLIMIT((float)low);
		if( iRet != PMBUS_CMD_SUCCESS )
		{
			printf("[ERROR] PS[%x] setUVFAULTLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
		}
	}

	else {
		write_i2cid(ps, true);
		iRet = m_PSManager.setUVFAULTLIMIT((float)low);
		if( iRet != PMBUS_CMD_SUCCESS )
		{
			printf("[ERROR] PS[%d] setUVFAULTLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
		}

		write_i2cid(ps, true);
		iRet = m_PSManager.setVOUTCOMMAND(volt_set);
		if( iRet != PMBUS_CMD_SUCCESS )
		{
			printf("[ERROR] PS[%d] setOVOUTCOMMAND Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
		}

		write_i2cid(ps, true);
		iRet = m_PSManager.setOVFAULTLIMIT((float)high);
		if( iRet != PMBUS_CMD_SUCCESS )
		{
			printf("[ERROR] PS[%d] setOVFAULTLIMIT Error : [0x%x] %s\n", ps_index, iRet, m_PSManager.error() );
		}
	}
}
/*
   void SWControl::verify_i2c(int ps)
   {
   int ps2 = 0;
   int ps_index = 0;

   if(ps == (0x1|0x2))
   {
   ps = 0x1;
   ps2 = 0x2;
   }
   else if(ps == (0x4|0x8))
   {
   ps = 0x4;
   ps2 = 0x8;
   }

   for(int ch = 0; ch < 2; ch++)
   {
   ps_index = readPSIndex(ps);

   write_i2cid(ps, 0x44);

   for( int iCH = 0; iCH < 2; iCH++ )
   {
   if( m_pmbus.verifyGLOBALWriteData(iCH) != PMBUS_CMD_SUCCESS)
   {
   printf( "[ERROR] PS[%d] CH[%d] VERIFY ERROR\n", ps_index, iCH );
   }
   }
   if(ps2 == 0 )
   {
   break;
   }
   ps = ps2;
   ps2 = 0;
   }
   }*/

void SWControl::sort_channel(int ch, double voltage, double high, double low, double c_limit)
{
	bool is_up = false;
	bool bcommon = false;
	bool bcommon2 = false;
	int ps = 0;
	double c_range = 36.0;
	int enbit = 0;
	int dv = 2;

	readPSCommon(bcommon, bcommon2);

	double tmpv = 0.25;
	if(m_isnew)
	{
		tmpv = 0.0375;
	}

	double limit = c_limit * tmpv;
	limit = limit < 0? 0 : limit;

	if (ch == 0) {
		if( m_ps1_voltage < voltage)
		{
			is_up = true;
		}
		ps = 0x1;
		m_ps1 = true;
		m_ps1_voltage = voltage;
		m_ps1_high = high;
		m_ps1_low = low;
		m_ps1_limit = limit;
		m_cur_step.ps[PS1] = voltage;
		enbit = 0x2;
		setDAC(9, 25 A * tmpv);
		if(!m_isnew)
		{
			setDAC(3, 8.0 V);
			setDAC(6, 0.0 V);
			setDAC(0, voltage);
		}
		if(bcommon && m_isnew)
		{
			setDAC(6, 25 A * tmpv);
			ps = 0x1 | 0x2;
			c_range = 72.0;
			dv = 4;
			enbit = 0x802;
		}
	}
	else if (ch == 1) {
		if( m_ps2_voltage < voltage)
		{
			is_up = true;
		}
		ps = 0x4;
		m_ps2 = true;
		m_ps2_voltage = voltage;
		m_ps2_high = high;
		m_ps2_low = low;
		m_ps2_limit = limit;
		enbit = 0x4;
		m_cur_step.ps[PS2] = voltage;
		setDAC(10, 25 A * tmpv);
		if(!m_isnew)
		{
			setDAC(4, 8.0 V);
			setDAC(7, 0.0 V);
			setDAC(1, voltage);
		}
		if(bcommon2 && m_isnew)
		{
			setDAC(7, 25 A * tmpv);
			ps = 0x4|0x8;
			c_range = 72.0;
			dv = 4;
			enbit = 0x1004;
		}
	}
	else if(ch == 2)
	{
		ps = 0x10;
		if(m_ps3_volt_save < voltage)
		{
			is_up = true;
		}
		m_ps3_volt_save = voltage;
		m_cur_step.ps[PS3] = voltage;
		m_ps3_limit = limit;
		enbit = 0x8;
		setDAC(11, limit);
		if(!m_isnew)
		{
			setDAC(5, 8.0 V);
			setDAC(8, 0.0 V);
			setDAC(2, voltage);
		}
	}
	else if (ch == 3) {
		if( m_ps4_voltage < voltage)
		{
			is_up = true;
		}
		ps = 0x2;
		m_ps4 = true;
		m_ps4_voltage = voltage;
		m_ps4_high = high;
		m_ps4_low = low;
		m_ps4_limit = limit;
		m_cur_step.ps[PS4] = voltage;
		enbit = 0x800;
		if(!m_isnew)
		{
			setDAC(28, 25 A * tmpv);
			setDAC(24, 8.0 V);
			setDAC(26, 0.0 V);
			setDAC(22, voltage);
		}
		else
		{
			setDAC(3, 25 A * tmpv);
		}

	}
	else if (ch == 4) {
		if( m_ps5_voltage < voltage)
		{
			is_up = true;
		} 
		ps = 0x8;
		m_ps5 = true;
		m_ps5_voltage = voltage;
		m_ps5_high = high;
		m_ps5_low = low;
		m_ps5_limit = limit;
		enbit = 0x1000;
		m_cur_step.ps[PS5] = voltage;
		if(!m_isnew)
		{
			setDAC(29, 25 A * tmpv);
			setDAC(25, 8.0 V);
			setDAC(27, 0.0 V);
			setDAC(23, voltage);
		}
		else
		{
			setDAC(4, 25 A * tmpv);
		}
	}

	if(m_isnew)
	{
		if(c_limit > c_range)
		{
			runtimeError("Current Limit is out of range !!!!");
			return;
		}
		if(voltage != 0)
		{
			set_i2c(voltage + 0.01, is_up, ps, high, low, c_limit / dv);
		}
		else
		{/*
		    for (unsigned int i = 0; i < NSLOT; i++) {
		    int value = Read(0x5 +i *0x100);

		    if(value & 0x180e)
		    {    
		    Write(0x6 + i * 0x100, value);
			//printf("VOLT OFF addr ; 0x%x value : 0x%x", 0x6 + i * 0x100, value);
			sysDelay(5 MS);
			}
			}*/

			set_i2c(0.5, is_up, ps, 8.0, 0.3, 25);
			for (int i = 0; i < NSLOT; i++) {

				Write(0x6 + i * 0x100, enbit);
				sysDelay(5 MS);
			}

		}
	}

}

void SWControl::readPSCommon(bool &bcommon, bool &bcommon2)
{
	for(unsigned int i = 0; i < NSLOT; i++)
	{
		if(_bi_find[i].find_drv)
		{
			if(m_isnew)
			{
				bcommon = !((Read(0x2 + i * 0x100) >> 14) & 0x1);
				bcommon2 = !((Read(0x2 + i * 0x100) >> 15) & 0x1);
				break;
			}
			else
			{
				if(m_is1540)
				{
					bcommon = true;
					bcommon2 = false;
					break;
				}
				else
				{
					bcommon = !(Read(0x2 + i * 0x100) & 0x1);
					bcommon2 = false;
					break;
				}
			}

		}
	}
}

int SWControl::check_psflag(bool bcommon, bool bcommon2, int &enbit)
{
	int cvalue = 0;
	if(m_isnew == true)
	{
		if(bcommon == true)
		{
			if(bcommon2 == true)
			{
				if(m_ps1 == true && m_ps2 == true)
				{
					cvalue |= 0x1;
				}

				if(m_ps1_voltage == 0.0 && m_ps2_voltage == 0.0)
				{
					cvalue |= 0x1 << 1;
				}
			}
			else
			{
				if(m_ps1 == true && m_ps2 == true && m_ps5 == true)
					// if(m_ps1 == true && m_ps2 == true && m_ps5)
				{
					cvalue |= 0x1;
				}

				if(m_ps1_voltage == 0.0 && m_ps2_voltage == 0.0 && m_ps5_voltage == 0.0)
				{
					cvalue |= 0x1 << 1;
				}

			}
		}
		else
		{
			if(bcommon2 == true)
			{
				if(m_ps1 == true && m_ps2 == true && m_ps4 == true)
				{
					cvalue |= 0x1;
				}

				if(m_ps1_voltage == 0.0 && m_ps2_voltage == 0.0 && m_ps4_voltage == 0.0)
				{
					cvalue |= 0x1 << 1;
				}
			}
			else
			{
				if(m_ps1 == true && m_ps2 == true && m_ps4 == true && m_ps5 == true)
				{
					cvalue |= 0x1;
				}
				if(m_ps1_voltage == 0.0 && m_ps2_voltage == 0.0 && m_ps4_voltage == 0.0 && m_ps5_voltage == 0.0)
				{
					cvalue |= 0x1 << 1;
				}

			}
		}
		enbit = 0x1806;
	}
	else // only PS1, PS2
	{
		if(bcommon)
		{

			enbit = 0x6;
			if(m_ps1 == true && m_ps2 == true)
			{
				cvalue |= 0x1;
			}

			if(m_ps1_voltage == 0.0 && m_ps2_voltage == 0.0)
			{
				cvalue |= 0x1 << 1;
			}
		}
		else
		{
			enbit = 0x1806;
			if(m_ps1 == true && m_ps2 == true && m_ps4 == true && m_ps5 == true)
			{
				cvalue |= 0x1;
			}
			if(m_ps1_voltage == 0.0 && m_ps2_voltage == 0.0 && m_ps4_voltage == 0.0 && m_ps5_voltage == 0.0)
			{
				cvalue |= 0x1 << 1;
			}
		}
	}

	return cvalue;
}

void SWControl::SET_PS3(double voltage, double delay1, double delay2, double high, double low, double c_limit)
{
	unsigned int i;
	bool bon = voltage? true : false;

	sort_channel(2, voltage, high, low, c_limit);
	sysDelay(delay1);

	if(bon)
	{

		for (i = 0; i < NSLOT; i++) {
			Write(0x5 + i * 0x100, 0x8);
			sysDelay(5 MS);
		}

	}
	else
	{
		if(!m_isnew)
		{
			for (i = 0; i < NSLOT; i++) {
				Write(0x6 + i * 0x100, 0x8);
				sysDelay(5 MS);
			}
		}

	}

	double factor = m_isnew == true? 0.0375: 0.25 * 1.2;
	double limit = c_limit * factor;
	limit = limit < 0 ? 0 : limit;

	if(!m_isnew)
	{
		setDAC(5, high);
		setDAC(8, low);
		setDAC(11, limit);
	}
	else
	{
		setDAC(11, limit);
	}

	VOLTINFO vi = { 2, voltage, delay2, high, low, c_limit };
	Send(BC_SETVOLT, &vi, sizeof(VOLTINFO));
	sysDelay(delay2);
}

void SWControl::readPSRev()
{    
	int rev_prev = 0, rev = 0, cnt = 0, sdut = 0;
	char msg[100];

	for(unsigned int i = 0; i < NSLOT; i++)
	{
		FIND_BOARD(i);
		if(!_bi_find[i].find_drv)
		{
			continue;
		}

		sdut |= 1 << i;
	}

	SET_JUDGE(sdut);


	int bd_flag = 0xff;

	for(unsigned int i = 0; i < NSLOT; i++)
	{
		sysWrite(i * 0x100, 0xff);

		if (sysRead(i * 0x100) != 0xff) 
		{
			bd_flag &= ~(0x1<<i); 
		}	
	}

	for(unsigned int i = 0; i < NSLOT; i++)
	{

		if (((bd_flag >> i) & 0x1) == 0) 
		{
			continue;
		}

		rev = Read(0xd +(i << 8)) >> 8;
		cnt++;
		//printf(" i : %d addr : 0x%x read valule : 0x%x rev : 0x%x", i, 0xd + (i << 8), Read(0xd + (i << 8)), rev);
		if((cnt > 1) && (rev_prev != rev))
		{
			sprintf(msg, "PS REVISION IS NOT MATCHED SLOT %d : 0x%x SLOT %d : 0x%x", i, rev_prev, i + 1, rev);
			Send(BC_RUNTIMEERROR, msg, strlen(msg) + 1);
			return;
		}
		rev_prev = rev;
	}

	if(rev == 0x26)
	{
		m_isnew = true;
	}
	else
	{
		m_isnew = false;
	}

}

void SWControl::readBDRev()
{
	int bd_flag = 0xff;
	int rev_prev = 0, rev = 0, cnt = 0;
	char msg[100];

	for(unsigned int i = 0; i < NSLOT; i++)
	{
		sysWrite(i * 0x100, 0xff);

		if (sysRead(i * 0x100) != 0xff) 
		{
			bd_flag &= ~(0x1<<i); 
		}	
	}

	for(unsigned int i = 0; i < NSLOT; i++)
	{

		if (((bd_flag >> i) & 0x1) == 0) 
		{
			continue;
		}

		rev = Read(0x1 + (i << 8));
		cnt++;
		if((cnt > 1) && (rev_prev != rev))
		{
			sprintf(msg, "DRV REVISION IS NOT MATCHED SLOT %d : 0x%x SLOT %d : 0x%x", i, rev_prev, i + 1, rev);
			Send(BC_RUNTIMEERROR, msg, strlen(msg) + 1);
			return;
		}
		rev_prev = rev;
	}

	if(rev == 0x0110)
	{
		m_is1540 = true;
	}
	else
	{
		m_is1540 = false;
	}

}
void SWControl::SET_PSA(VOLTINFO *vi, unsigned int length)
{
	bool boff = true;
	bool bcommon = false;
	bool bcommon2 = false;
	unsigned int i, j;
	unsigned int value = 0;
	char msg[40];

	readPSCommon(bcommon, bcommon2);

	if(m_isnew == true)
	{
		if(length <= 0)
		{
			if(bcommon == true)
			{
				length = bcommon2 == true? 3 : 4;
			}
			else
			{
				length = bcommon2 == true? 4 : 5;
			}
		}

	}
	else
	{
		if(length <= 0)
		{
			length = bcommon ? 3 : 5;
		}
	}

	SET_VIL(VIL_CLK, 0.0 V, 000 MS);
	SET_VIL(VIL_CLK6, 0.0 V, 000 MS);
	SET_VIL(VIL_CLK7, 0.0 V, 000 MS);
	SET_VIL(VIL_ADD8, 0.0 V, 000 MS);
	SET_VIL(VIL_ADD9, 0.0 V, 000 MS);
	SET_VIL(VIL_SCAN0, 0.0 V, 000 MS);
	SET_VIL(VIL_SCAN1, 0.0 V, 000 MS);

	for (i = 0; i < length; i++) {

		if(m_isnew == true)
		{
			if (((bcommon == true) && (vi[i].ch == 3)) || ((bcommon2 == true) && (vi[i].ch == 4)))
			{
				sprintf(msg, "Invalid PS Channel!!! PS[%d]", vi[i].ch + 1);
				runtimeError(msg);
				return;
			}
		}
		sort_channel(vi[i].ch, vi[i].voltage, vi[i].high, vi[i].low, vi[i].c_limit);
	}

		sysDelay(600 MS);

	for (i = 0; i < length; i++) {
		if (vi[i].ch == 3 && bcommon == true) {
			continue;
		}

		if (vi[i].ch == 4 && bcommon2 == true) {
			continue;
		}

		if (vi[i].voltage != 0.0) {
			boff = false;
		}
	}

	if (boff == false) {
		unsigned short enable_value_common[] = { 0x802, 0x1004, 0x8 };
		unsigned short enable_value_common1[] = { 0x802, 0x4, 0x8, 0, 0x1000 };
		unsigned short enable_value_common2[] = { 0x2, 0x1004, 0x8, 0x800 };
		unsigned short enable_value_notcommon[] = { 0x2, 0x4, 0x8, 0x800, 0x1000 };


		for (i = 0; i < NSLOT; i++) {
			if (!_bi_find[i].read) {
				continue;
			}

			double delay = 0.0;

			for (j = 0; j < length; j++) {
				if(m_isnew)
				{
					if(bcommon)
					{
						value |= (bcommon2 == true? enable_value_common[vi[j].ch] : enable_value_common1[vi[j].ch]);
					}
					else
					{
						value |= (bcommon2 == true? enable_value_common2[vi[j].ch] : enable_value_notcommon[vi[j].ch]);
					}
				}
				else
				{

					value |= enable_value_common[vi[j].ch];
				}
				Write(0x5 + i * 0x100, value);

				if (vi[j].delay == 0.0) {
					continue;
				}

				sysDelay(vi[j].delay);
				delay += vi[j].delay;
			}

			if (5 MS - delay > 0) {
				sysDelay(5 MS - delay);
			}
		}
		if(!m_isnew)
		{

			for (i = 0; i < NSLOT; i++) {
				if (!_bi_find[i].read) {
					continue;
				}

				Write(0x5 + i * 0x100, 0x400);
				sysDelay(5 MS);
			}
		}
	}
	else {

		if(!m_isnew)
		{
			for (i = 0; i < NSLOT; i++) {
				Write(0x6 + i * 0x100, 0x400);
				sysDelay(5 MS);
			}

			for (i = 0; i < NSLOT; i++) {
				Write(0x6 + i * 0x100, 0x180e);
				sysDelay(5 MS);
			}
		}
	}

	m_ps1 = m_ps2 = m_ps4 = m_ps5 = false;

	set_da();

	//	VOLTINFO vi = { vi[0].ch, vi[0].voltage, vi[0].delay, vi[0].high, vi[0].low, vi[0].c_limit };
	Send(BC_SETVOLT, &vi[0], sizeof(VOLTINFO));
}

void SWControl::set_da()
{
	bool bcommon = false;
	bool bcommon2 = false;

	if(!m_isnew)
	{
		setDAC(3, m_ps1_high);
		setDAC(4, m_ps2_high);
		setDAC(24, m_ps4_high);
		setDAC(25, m_ps5_high);
		setDAC(6, m_ps1_low);
		setDAC(7, m_ps2_low);
		setDAC(26, m_ps4_low);
		setDAC(27, m_ps5_low);
		setDAC(9, m_ps1_limit);
		setDAC(10, m_ps2_limit);
		setDAC(28, m_ps4_limit);
		setDAC(29, m_ps5_limit);
	}
	else
	{
		if(bcommon)
		{
			if(bcommon2)
			{
				setDAC(6, m_ps1_limit);
				setDAC(7, m_ps2_limit);
				setDAC(11, m_ps3_limit);
			}
			else
			{
				setDAC(6, m_ps1_limit);
				setDAC(10, m_ps2_limit*2);
				setDAC(4, m_ps5_limit*2);
				setDAC(11, m_ps3_limit);
			}
		}
		else
		{
			if(bcommon2)
			{
				setDAC(9, m_ps1_limit*2);
				setDAC(7, m_ps2_limit);
				setDAC(3, m_ps4_limit*2);
				setDAC(11, m_ps3_limit);
			}
			else
			{
				setDAC(9, m_ps1_limit*2);
				setDAC(10, m_ps2_limit*2);
				setDAC(3, m_ps4_limit*2);
				setDAC(4, m_ps5_limit*2);
				setDAC(11, m_ps3_limit);
			}
		}
	}

}

void SWControl::SET_VIH(int ch, double voltage, double delay)
{	
	if( m_shmoo_test == true ) {
		if( m_shmoo_save_ch[0] == 5 )
			voltage = (double) m_shmoo_save_data[0];
		else if( m_shmoo_save_ch[1] == 5 )
			voltage = (double) m_shmoo_save_data[1];
	}

	/*	unsigned int i;
		int measch[] = { 0x0b, 0x20, 0x21, 0x22, 0x23, 0x24, 0x30, 0x31 };

		setDAC(12 + ch, voltage);

		if (ch == 0) {
		setDAC(12 + VIH_CLK, voltage);
		setDAC(12 + VIH_CLK6, voltage);
		setDAC(12 + VIH_CLK7, voltage);
		setDAC(12 + VIH_ADD8, voltage);
		setDAC(12 + VIH_ADD9, voltage);
		setDAC(12 + VIH_SCAN0, voltage);
		setDAC(12 + VIH_SCAN1, voltage);

		if (voltage) {
		for (i = 0; i < NSLOT; i++) {
		Write(0x5 + i * 0x100, 0x3e0);
		sysDelay(5 MS);
		}

		if (!(Read(0x4062) & FLASH_DEVICE) || m_ready_bit == 0) {
		for (i = 0; i < NSLOT; i++) {
		Write(0x6 + i * 0x100, 0x200);
		}
		}
		}
		else {
		for (i = 0; i < NSLOT; i++) {
		Write(0x6 + i * 0x100, 0x10);
		sysDelay(5 MS);
		}
		}
		}

		sysDelay(delay);
		voltage = measVoltage(measch[ch]);
		m_cur_step.vih[ch] = voltage;*/
	bool benable = voltage ? true : false;
	if(m_isnew && (voltage < 0.5))
	{
		voltage = 0.5;
		benable = false;
	}

	setDAC(12 + ch, voltage);

	if (ch == 0) {
		setDAC(12 + VIH_CLK, voltage);
		setDAC(12 + VIH_CLK6, voltage);
		setDAC(12 + VIH_CLK7, voltage);
		setDAC(12 + VIH_ADD8, voltage);
		setDAC(12 + VIH_ADD9, voltage);
		setDAC(12 + VIH_SCAN0, voltage);
		setDAC(12 + VIH_SCAN1, voltage);


		if (benable) {
			for(int i = 0; i < 8; i++)
			{
				Write(0x5 + i * 0x100, 0x3e0);
				sysDelay(1 MS);

				if (!(Read(0x4062) & FLASH_DEVICE) || m_ready_bit == 0) {
					Write(0x6 +  i * 0x100, 0x200);
				}
			}
		}
		else {
			for(int i = 0; i < 8; i++)
			{
				Write(0x6 + i * 0x100, 0x3e0);
				sysDelay(1 MS);
			}
		}

	}

	    sysDelay(delay);
	//    voltage = measVoltage(0x11 + ch);
	//	char msg[100];
	//	sprintf( msg, "voltage : %f, ch : %d ", voltage, ch );
	//	Send( BC_PRINTF, msg, strlen( msg ));
	m_cur_step.vih[ch] = voltage;


}

void SWControl::SET_VIL(int ch, double voltage, double delay)
{
	int measch[] = { 0x25, 0x26, 0x27, 0x28, 0x29, 0x32, 0x33 };
	setDAC(32 + ch, voltage);
	sysDelay(delay);
	//    voltage = measVoltage(measch[ch]);
	m_cur_step.vil[ch] = voltage;
}

void SWControl::SET_VTH(double voltage, double delay)
{	
	if( m_shmoo_save_ch[0] == 6 )
		voltage = (double) m_shmoo_save_data[0];
	else if( m_shmoo_save_ch[1] == 6 )
		voltage = (double) m_shmoo_save_data[1];

	/*setDAC(20, voltage);
	  sysDelay(delay);

	  voltage = measVoltage(0x34);
	  m_cur_step.vth = voltage;

	  voltage = measVoltage(0x0c + PS1);
	  m_cur_step.ps[PS1] = voltage;

	  voltage = measVoltage(0x0c + PS2);
	  m_cur_step.ps[PS2] = voltage;

	  voltage = measVoltage(0x0c + PS3);
	  m_cur_step.ps[PS3] = voltage;

	  voltage = measVoltage(0x0c + PS4);
	  m_cur_step.ps[PS4] = voltage;

	  voltage = measVoltage(0x0c + PS5);
	  m_cur_step.ps[PS5] = voltage;*/


	setDAC(20, voltage);
	sysDelay(delay);

	m_cur_step.vth = voltage;
	/*voltage = measVoltage(0x16);
	  m_cur_step.vth = voltage;

	  voltage = measVoltage(0x05);
	  m_cur_step.ps[PS1] = voltage;

	  voltage = measVoltage(0x06);
	  m_cur_step.ps[PS2] = voltage;

	  voltage = measVoltage(0x07);
	  m_cur_step.ps[PS3] = voltage; //have to check exact address
	  */


	//1540
	/*	setDAC(17, voltage);
		sysDelay(delay);

		voltage = measVoltage(0x16);
		m_cur_step.vth = voltage;

		voltage = measVoltage(5 + PS1);
		m_cur_step.ps[PS1] = voltage;

		voltage = measVoltage(5 + PS2);
		m_cur_step.ps[PS2] = voltage;

		voltage = measVoltage(5 + PS3);
		m_cur_step.ps[PS3] = voltage;
		*/

}

void SWControl::MEAS_MPAT(const char *pattern_name, double wait, short xa, short ya, int da, char sa)
{
	m_now_step_count++;

	if (START_MPAT(pattern_name, 0, xa, ya, da, sa)) {
		WAIT_MPAT(wait);
	}

	for (POSITION pos = m_patternCompiler->patternData()->GetHeadPosition(); pos;) {
		TPGDATA	*pg = m_patternCompiler->patternData()->GetNext(pos);

		if (strcmp(pattern_name, (const char *) pg->name) == 0) {
			if (!m_diagnostic) {
				if (!pg->catch_use) {
					if (m_device_mode == FLASH_DEVICE || m_device_mode == EMMC) {
						_APP(((m_step_no == 0) || (m_hottemp == true)) ? ID_INIT : ID_DATA);
					}
					else {
						if((m_hottemp) || (m_step_no == 0))
						{
							_APP(ID_PRETEST_RESULT);
						}
						else
						{
							_APP(ID_BITEST_RESULT);
						}
					}
				}
				else if (m_dq_use == true) {
					_APP(ID_DQ_RESULT);
				}
			}

			break;
		}
	}

	if( m_shmoo_test == false) {     
	unsigned int iFault[NSLOT];
	int ret = 0;
	unsigned int i = 0;

	if(m_isnew == true)
	{
		write_i2cid(0x1f, false);
		ret = readFault();

		for(i = 0; i < NSLOT; i++)
		{
			iFault[i] = 0;
			iFault[i] = m_fault[i];
			iFault[i] |= readFault(i);
			if (iFault[i] != 0)
			{
				_bi_find[i].read = 0;
				//printf("ifalut occured! slot : %d iFault : 0x%x", i, iFault[i]);
				Write(0x6 + i * 0x100, 0x1ffe);
				sysDelay(5 MS);
			}

		}

	}
	else
	{
		for (i = 0; i < NSLOT; i++)
		{
			if(!_bi_find[i].read)
			{
				continue;
			}
			iFault[i] = readFault(i);

			if (iFault[i] != 0) {
				_bi_find[i].read = 0;
			}
		}
	}

	Send(BC_DRV_FAULT, iFault, sizeof(iFault));
	}
}

bool SWControl::START_MPAT(const char *pattern_name, double wait, short xa, short ya, int da, char sa)
{
	char msg[100];

	m_display_count = 0;
	sysResetLatch();	// latched data clear
	Write(0x3f, 0);

	m_cur_step.mpat_time = wait;
	strcpy(m_cur_step.tcs, pattern_name);
	Send(BC_STEPINFO, &m_cur_step, sizeof(m_cur_step));

	for (POSITION pos = m_patternCompiler->patternData()->GetHeadPosition(); pos;) {
		TPGDATA	*pg = m_patternCompiler->patternData()->GetNext(pos);

		if (strcmp(pattern_name, pg->name) == 0) {
			if (!pg->catch_use) {
				_APP(ID_FM_BLOCK);
			}

			break;
		}
	}

	if (!startPattern(*m_patternCompiler, pattern_name, xa, ya, da ,sa)) {
		sprintf(msg, "INVALIDE PATTERN NAME %s\n", pattern_name);
		runtimeError((const char *) msg);
		return false;
	}

	m_cur_pattern = pattern_name;

	if (wait) {
		WAIT_MPAT(wait);
	}

	return true;
}

void SWControl::STOP_MPAT()
{
	sysStopPattern();
}

void SWControl::WAIT_MPAT(double time)
{
	if (DoPgrunWork(time) != 0) {
		STOP_MPAT();
	}
}

int SWControl::WAIT_PAUSE(double time)
{
	return DoPgrunWork(time, true);
}

void SWControl::CONTINUE_MPAT()
{
	sysPGContinue();
}

void SWControl::RESET_TESTER()
{
	bool bcommon = false, bcommon2 = false;
	sysStopPattern();

	SET_PS(PS1, 0 V, 10 MS, 0 V, 0 V, 0 V);
	SET_PS(PS2, 0 V, 10 MS, 0 V, 0 V, 0 V);
	SET_PS(PS3, 0 V, 10 MS, 0 V, 0 V, 0 V);

	readPSCommon(bcommon, bcommon2);


	if(bcommon == false)
	{
		SET_PS(PS4, 0 V, 10 MS, 0 V, 0 V, 0 V);
		if((m_isnew == false) || (bcommon2 == false))
		{
			SET_PS(PS5, 0 V, 10 MS, 0 V, 0 V, 0 V);
		}
	}

	SET_VIH(VIH, 0, 10 MS);
	SET_VIH(VIH_CLK6, 0, 10 MS);
	SET_VIH(VIH_CLK7, 0, 10 MS);
	SET_VIH(VIH_ADD8, 0, 10 MS);
	SET_VIH(VIH_ADD9, 0, 10 MS);

	RESET_DRIVER();

	for(unsigned int i = 0; i < NSLOT; i++)
	{
		Write(0x6 + i * 0x100, 0x1ffe);
		sysDelay(5 MS);
	}
	Write(0x40c4, 0);
	Write(0x40c5, 0);

	if(m_isnew)
	{
		for(int i = 0; i < 5; i++)
		{
			Write(0x2, 0x1 << i);
			int iRet =  m_PSManager.setCLEARFAULTS(); 
			if(iRet != PMBUS_CMD_SUCCESS)
				//if(m_PSManager.setCLEARFAULTS() != PMBUS_CMD_SUCCESS)
			{
				printf("[ERROR] PS[%d] CLEAR FAULT Error : [0x%x] %s\n", readPSIndex(0x1<<i), iRet, m_PSManager.error());
			}
		}
	}
}

void SWControl::SET_TIME(const char *group)
{
	char msg[132];

#ifdef _TPG_GROUP
	m_timeCompiler = NULL;

	for (POSITION pos = m_tgGroup.GetHeadPosition(); pos;) {
		m_timeCompiler = m_tgGroup.GetNext(pos);

		if (m_timeCompiler->name() == group) {
			break;
		}
	}
#endif
	if (m_timeCompiler == NULL) {
		sprintf(msg, "TIMESET DOWNLOAD ERROR : %s\n", group);
		runtimeError((const char *) msg);
		return;
	}

	//	int down_time = Read(0x4000);
	//	down_time |= Read(0x4020) << 16;

	//	if (m_timeCompiler->compTime() != down_time) {
	if(m_shmoo_test == false)  {	//could be changed
		writeTimeset(*m_timeCompiler);
	}
	//		Write(0x4000, m_timeCompiler->compTime() & 0xffff);
	//		Write(0x4020, m_timeCompiler->compTime() >> 16);
	//	}
}

void SWControl::SET_MPAT(const char *group)
{
	char msg[132];

#ifdef _TPG_GROUP
	m_patternCompiler = NULL;

	for (POSITION pos = m_pgGroup.GetHeadPosition(); pos;) {
		m_patternCompiler = m_pgGroup.GetNext(pos);

		if (m_patternCompiler->name() == group) {
			break;
		}
	}
#endif
	if (m_patternCompiler == NULL) {
		sprintf(msg, "PATTERN DOWNLOAD ERROR : %s\n", group);
		runtimeError((const char *) msg);
		return;
	}

	//	int down_time = Read(0x4080);
	//	down_time |= Read(0x8600) << 16;

	//	if (m_patternCompiler->compTime() != down_time) {
	writePattern(*m_patternCompiler);
	//		Write(0x4080, m_patternCompiler->compTime() & 0xffff);
	//		Write(0x8600, m_patternCompiler->compTime() >> 16);
	//	}
}

void SWControl::SET_SCR(const char *group, Scramble &scr)
{
	//	int down_time = Read(0x4040);
	//	down_time |= Read(0x4060) << 16;

	//	if (scr.comp_time != down_time) {
	writeScramble(scr);
	//		scr.comp_time = sysDelay(0);
	//		Write(0x4040, scr.comp_time & 0xffff);
	//		Write(0x4060, scr.comp_time >> 16);
	//	}
}

void SWControl::SET_AGING(int agingtime)
{
	m_aging_stime = ::time(0);
	m_aging_etime = agingtime;
	m_aging_use = true;
	Send(BC_SET_AGING_TIME, &agingtime, sizeof(agingtime));
}

void SWControl::SET_EMMC_COMPARE(unsigned int bit)
{
	m_emmc_compare_bit = bit | 0x80000000;
	m_emmc_compare_value = 0llu;
}

void SWControl::SET_EMMC_COMPARE(unsigned int bit, unsigned long long value)
{
	m_emmc_compare_bit = bit;
	m_emmc_compare_value = value;
}

void SWControl::SET_EMMC_EXPECT(unsigned int bit, unsigned long long value)
{
	m_emmc_expect_bit = bit;
	m_emmc_expect_value = value;
}

void SWControl::SET_READ_NAND_CHIPID()
{
	m_read_nand_chipid = true;
	INIT_READ_CHIPID();
}

void SWControl::RATE(double r0, double r1, double r2, double r3, double r4, double r5, double r6, double r7,
		double  r8, double  r9, double r10, double r11, double r12, double r13, double r14, double r15,
		double r16, double r17, double r18, double r19, double r20, double r21, double r22, double r23,
		double r24, double r25, double r26, double r27, double r28, double r29, double r30, double r31)
{
#ifdef _TPG_GROUP
	m_timeCompiler = NULL;

	for (POSITION pos = m_tgGroup.GetHeadPosition(); pos;) {
		m_timeCompiler = m_tgGroup.GetNext(pos);

		if (m_timeCompiler->name() == "internel") {
			break;
		}
	}
#else
	if (m_timeCompiler) {
		delete m_timeCompiler;
		m_timeCompiler = NULL;
	}
#endif

	if (m_timeCompiler == NULL) {
		m_timeCompiler = new TGCompiler();
		//	m_timeCompiler->setModel(sysModel);
		m_timeCompiler->setName("internel");
#ifdef _TPG_GROUP
		m_tgGroup.append(m_timeCompiler);
#endif
	}

	TTIMESET *timeSet = m_timeCompiler->ts();
	TTGDATA *tg;

	for (POSITION pos = timeSet->list.GetHeadPosition(); pos;) {
		tg = timeSet->list.GetNext(pos);
	}

	m_timeCompiler->setCompTime(sysDelay(0));

	int nts = timeSet->list.GetCount();
	int rate = 2;

#define addTS() \
	{ \
		TTGDATA *tg = new TTGDATA; \
		memset(tg, 0, sizeof(TTGDATA)); \
		tg->rate = rate; \
		timeSet->list.AddTail(tg); \
	}

	if (r0 && nts < 1)	addTS();
	if (r1 && nts < 2)	addTS();
	if (r2 && nts < 3)	addTS();
	if (r3 && nts < 4)	addTS();
	if (r4 && nts < 5)	addTS();
	if (r5 && nts < 6)	addTS();
	if (r6 && nts < 7)	addTS();
	if (r7 && nts < 8)	addTS();
	if (r8 && nts < 9)	addTS();
	if (r9 && nts < 10)	addTS();
	if (r10 && nts < 11)	addTS();
	if (r11 && nts < 12)	addTS();
	if (r12 && nts < 13)	addTS();
	if (r13 && nts < 14)	addTS();
	if (r14 && nts < 15)	addTS();
	if (r15 && nts < 16)	addTS();
	if (r16 && nts < 17)	addTS();
	if (r17 && nts < 18)	addTS();
	if (r18 && nts < 19)	addTS();
	if (r19 && nts < 20)	addTS();
	if (r20 && nts < 21)	addTS();
	if (r21 && nts < 22)	addTS();
	if (r22 && nts < 23)	addTS();
	if (r23 && nts < 24)	addTS();
	if (r24 && nts < 25)	addTS();
	if (r25 && nts < 26)	addTS();
	if (r26 && nts < 27)	addTS();
	if (r27 && nts < 28)	addTS();
	if (r28 && nts < 29)	addTS();
	if (r29 && nts < 30)	addTS();
	if (r30 && nts < 31)	addTS();
	if (r31 && nts < 32)	addTS();

#undef addTS

	int ts = 0;

	for (POSITION pos = timeSet->list.GetHeadPosition(); pos; ts++) {
		tg = timeSet->list.GetNext(pos);

		if (r0 && ts == 0)	tg->width = (int) (r0 * 1000 / 0.000001);
		if (r1 && ts == 1)	tg->width = (int) (r1 * 1000 / 0.000001);
		if (r2 && ts == 2)	tg->width = (int) (r2 * 1000 / 0.000001);
		if (r3 && ts == 3)	tg->width = (int) (r3 * 1000 / 0.000001);
		if (r4 && ts == 4)	tg->width = (int) (r4 * 1000 / 0.000001);
		if (r5 && ts == 5)	tg->width = (int) (r5 * 1000 / 0.000001);
		if (r6 && ts == 6)	tg->width = (int) (r6 * 1000 / 0.000001);
		if (r7 && ts == 7)	tg->width = (int) (r7 * 1000 / 0.000001);
		if (r8 && ts == 8)	tg->width = (int) (r8 * 1000 / 0.000001);
		if (r9 && ts == 9)	tg->width = (int) (r9 * 1000 / 0.000001);
		if (r10 && ts == 10)	tg->width = (int) (r10 * 1000 / 0.000001);
		if (r11 && ts == 11)	tg->width = (int) (r11 * 1000 / 0.000001);
		if (r12 && ts == 12)	tg->width = (int) (r12 * 1000 / 0.000001);
		if (r13 && ts == 13)	tg->width = (int) (r13 * 1000 / 0.000001);
		if (r14 && ts == 14)	tg->width = (int) (r14 * 1000 / 0.000001);
		if (r15 && ts == 15)	tg->width = (int) (r15 * 1000 / 0.000001);
		if (r16 && ts == 16)	tg->width = (int) (r16 * 1000 / 0.000001);
		if (r17 && ts == 17)	tg->width = (int) (r17 * 1000 / 0.000001);
		if (r18 && ts == 18)	tg->width = (int) (r18 * 1000 / 0.000001);
		if (r19 && ts == 19)	tg->width = (int) (r19 * 1000 / 0.000001);
		if (r20 && ts == 20)	tg->width = (int) (r20 * 1000 / 0.000001);
		if (r21 && ts == 21)	tg->width = (int) (r21 * 1000 / 0.000001);
		if (r22 && ts == 22)	tg->width = (int) (r22 * 1000 / 0.000001);
		if (r23 && ts == 23)	tg->width = (int) (r23 * 1000 / 0.000001);
		if (r24 && ts == 24)	tg->width = (int) (r24 * 1000 / 0.000001);
		if (r25 && ts == 25)	tg->width = (int) (r25 * 1000 / 0.000001);
		if (r26 && ts == 26)	tg->width = (int) (r26 * 1000 / 0.000001);
		if (r27 && ts == 27)	tg->width = (int) (r27 * 1000 / 0.000001);
		if (r28 && ts == 28)	tg->width = (int) (r28 * 1000 / 0.000001);
		if (r29 && ts == 29)	tg->width = (int) (r29 * 1000 / 0.000001);
		if (r30 && ts == 30)	tg->width = (int) (r30 * 1000 / 0.000001);
		if (r31 && ts == 31)	tg->width = (int) (r31 * 1000 / 0.000001);

		if (tg->width > 60000) {
			tg->width = 60000;
		}

		tg->data[0x58] = tg->width - 30;
	}
}

void SWControl::CLK(int pin, char *c0, char *c1, char *c2, char *c3, char *c4, char *c5, char *c6, char *c7,
		char  *c8, char  *c9, char *c10, char *c11, char *c12, char *c13, char *c14, char *c15,
		char *c16, char *c17, char *c18, char *c19, char *c20, char *c21, char *c22, char *c23,
		char *c24, char *c25, char *c26, char *c27, char *c28, char *c29, char *c30, char *c31)
{
	if (m_timeCompiler == NULL) {
		return;
	}

	double arg_ad = (double) (int) c0 - (double) (int) c1 * 0.1 -
		(double) (int)  c2 * 0.2    - (double) (int)  c3 * 0.3    - (double) (int)  c4 * 0.4    -
		(double) (int)  c5 * 0.5    - (double) (int)  c6 * 0.6    - (double) (int)  c7 * 0.7    -
		(double) (int)  c8 * 0.8    - (double) (int)  c9 * 0.9    - (double) (int) c10 * 0.01   -
		(double) (int) c11 * 0.02   - (double) (int) c12 * 0.03   - (double) (int) c13 * 0.04   -
		(double) (int) c14 * 0.05   - (double) (int) c15 * 0.06   - (double) (int) c16 * 0.07   -
		(double) (int) c17 * 0.08   - (double) (int) c18 * 0.09   - (double) (int) c19 * 0.001  -
		(double) (int) c20 * 0.002  - (double) (int) c21 * 0.003  - (double) (int) c22 * 0.004  -
		(double) (int) c23 * 0.005  - (double) (int) c24 * 0.006  - (double) (int) c25 * 0.007  -
		(double) (int) c26 * 0.008  - (double) (int) c27 * 0.009  - (double) (int) c28 * 0.0001 -
		(double) (int) c29 * 0.0002 - (double) (int) c30 * 0.0003 - (double) (int) c31 * 0.0004;

	int from, to, spin = 0;
	char wave[255];
	int ts = 0;
	char *p;
	long long arg_add = (long long) arg_ad;
	int  magic = (int) ((arg_ad - arg_add / 1000 * 1000) * 10000);

	pin %= 32;
	pin += 1;

	TTIMESET *timeSet = m_timeCompiler->ts();
	TTGDATA *tg;

	if (timeSet->magic[pin - 1] == magic) {
		return;
	}

	timeSet->magic[pin - 1] = magic;

	int pins[] = {
		16, 12,  8, 10,  0,  2, 14, 18,  4,  6, 20,
		22,  0,  0,  0,  0, 24, 28, 32, 36, 40, 44,
		48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88
	};

	pin = pins[pin - 1];

	for (POSITION pos = timeSet->list.GetHeadPosition(); pos; ts++) {
		tg = timeSet->list.GetNext(pos);

		if (ts == 0 && c0)		strcpy(wave, c0);
		else if (ts == 1 && c1)		strcpy(wave, c1);
		else if (ts == 2 && c2)		strcpy(wave, c2);
		else if (ts == 3 && c3)		strcpy(wave, c3);
		else if (ts == 4 && c4)		strcpy(wave, c4);
		else if (ts == 5 && c5)		strcpy(wave, c5);
		else if (ts == 6 && c6)		strcpy(wave, c6);
		else if (ts == 7 && c7)		strcpy(wave, c7);
		else if (ts == 8 && c8)		strcpy(wave, c8);
		else if (ts == 9 && c9)		strcpy(wave, c9);
		else if (ts == 10 && c10)	strcpy(wave, c10);
		else if (ts == 11 && c11)	strcpy(wave, c11);
		else if (ts == 12 && c12)	strcpy(wave, c12);
		else if (ts == 13 && c13)	strcpy(wave, c13);
		else if (ts == 14 && c14)	strcpy(wave, c14);
		else if (ts == 15 && c15)	strcpy(wave, c15);
		else if (ts == 16 && c16)	strcpy(wave, c16);
		else if (ts == 17 && c17)	strcpy(wave, c17);
		else if (ts == 18 && c18)	strcpy(wave, c18);
		else if (ts == 19 && c19)	strcpy(wave, c19);
		else if (ts == 20 && c20)	strcpy(wave, c20);
		else if (ts == 21 && c21)	strcpy(wave, c21);
		else if (ts == 22 && c22)	strcpy(wave, c22);
		else if (ts == 23 && c23)	strcpy(wave, c23);
		else if (ts == 24 && c24)	strcpy(wave, c24);
		else if (ts == 25 && c25)	strcpy(wave, c25);
		else if (ts == 26 && c26)	strcpy(wave, c26);
		else if (ts == 27 && c27)	strcpy(wave, c27);
		else if (ts == 28 && c28)	strcpy(wave, c28);
		else if (ts == 29 && c29)	strcpy(wave, c29);
		else if (ts == 30 && c30)	strcpy(wave, c30);
		else if (ts == 31 && c31)	strcpy(wave, c31);

		p = strtok(wave, " ");
		spin = pin;

		while (p) {
			from = atoi(p);
			to = tg->width;

			p = strtok(NULL, " ");

			if (p) {
				to = atoi(p);
				p = strtok(NULL, " ");
			}

			tg->data[spin] = from | 0x8000;

			if (to != tg->width) {
				tg->data[spin + 1] = to | 0x8000;
			}

			if (pin >= 24) {
				spin = pin + 2;
			}
		}
	}
}

void SWControl::TS(char *n0, char *n1, char *n2, char *n3, char *n4, char *n5, char *n6, char *n7,
		char  *n8, char  *n9, char *n10, char *n11, char *n12, char *n13, char *n14, char *n15,
		char *n16, char *n17, char *n18, char *n19, char *n20, char *n21, char *n22, char *n23,
		char *n24, char *n25, char *n26, char *n27, char *n28, char *n29, char *n30, char *n31)
{

	if (m_timeCompiler == NULL) {
		return;
	}

	TTIMESET *timeSet = m_timeCompiler->ts();
	m_timeCompiler->setCompTime(sysDelay(0));

	TTGDATA *tg;
	POSITION pos = timeSet->list.GetHeadPosition();

	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n0); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n1); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n2); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n3); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n4); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n5); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n6); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n7); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n8); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n9); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n10); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n11); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n12); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n13); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n14); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n15); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n16); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n17); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n18); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n19); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n20); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n21); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n22); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n23); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n24); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n25); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n26); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n27); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n28); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n29); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n30); }
	if (pos) { tg = timeSet->list.GetNext(pos); strcpy(tg->name, n31); }
}
/*
   int SWControl::SET_PG(const char *name, int offset, int data, int which)
   {
   int pc = getPC(name);

   if (pc) {
   offset += pc;
   sysWrite(which + offset, data & 0xffff);

   if (which == PC_LOOP) {
   sysWrite(PC_LOOP + 0x200 + offset, data >> 16);
   }

   return offset;
   }

   return 0;
   }

   bool SWControl::SET_TG(const char *name, const char *clk, int edge, ...)
   {
   va_list ap;
   int i, from = edge;
   unsigned short data;
   int nts = 0;
   TTGDATA *tg = 0;
   TCHANNEL *ch = 0;

   for (POSITION pos = m_timeCompiler->ts()->list.GetHeadPosition(); pos; nts++) {
   tg = m_timeCompiler->ts()->list.GetNext(pos);

   if (strcmp(name, tg->name) == 0) {
   break;
   }
   }

   if (tg == 0) {
   return false;
   }

   for (POSITION pos = m_timeCompiler->ts()->chlist.GetHeadPosition(); pos;) {
   ch = m_timeCompiler->ts()->chlist.GetNext(pos);

   if (strcmp(ch->name, clk) == 0) {
   break;
   }
   }

   if (ch == 0) {
   return false;
   }

   nts = nts * USE_BIT;
   va_start(ap, edge);

   int to = va_arg(ap, int);

   for (i = 0; i < from / tg->rate; i++) {
   data = tg->data[i];
   data &= ~(1 << ch->sys);

   if (ch->sys < 16) {
   Write(0x40000 + nts + 1, data);
   }
   else {
   Write(0x60000 + nts + 1, data);
   }
   }

   while (from) {
   if (to == 0) {
to = tg->width;
}

for (i = from / tg->rate; i < to / tg->rate; i++) {
	data = tg->data[i];
	data &= ~(1 << ch->sys);
	data |= 1 << ch->sys;

	if (ch->sys < 16) {
		Write(0x40000 + nts + 1, data);
	}
	else {
		Write(0x60000 + nts + 1, data);
	}
}

from = va_arg(ap, int);
to = va_arg(ap, int);
}

va_end(ap);
return true;
}
	*/
int SWControl::FM_READ_RESULT()
{
	Write(0x3f, 0);
	m_scan = 0;
	Send(BC_FM_DATA, m_read_fm_data, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * MAX_NBYTE);
	return 0;
}

int SWControl::GET_BLOCK_DATA(char *step_name)
{
	unsigned int length = m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1);
	memset(m_get_step_fail_block, 0, length);
	Send(BC_GET_BLOCK_DATA, step_name, strlen(step_name));
	return Recv(m_get_step_fail_block, length);
}

int SWControl::GET_NBYTE_MATCH(double &result, char *base, unsigned int chip_index, unsigned int byte_index, unsigned int max_byte)
{
	char buf[MAX_NBYTE + sizeof(unsigned int) * 3 + 20];
	unsigned int i;

	memset(buf, 0, sizeof(buf));

	for (i = 0; i < MAX_NBYTE && base[i] != 0; i++) {
		buf[i + 12] = base[i];
	}

	*(unsigned int *) &buf[0] = chip_index;
	*(unsigned int *) &buf[4] = byte_index;
	*(unsigned int *) &buf[8] = max_byte;

	if (RUNMODE) {
		Send(BC_GET_NBYTE_MATCH, buf, MAX_NBYTE + sizeof(unsigned int) * 3 + 1);
		recv(m_sockfd, (char *) &i, 4, 0);
		result = i / 100.0;
	}

	return 0;
}

int SWControl::getPC(const char *name)
{
	TPGDATA *pg;

	if (m_patternCompiler == 0) {
		return 0;
	}

	for (POSITION pos = m_patternCompiler->patternData()->GetHeadPosition(); pos;) {
		pg = m_patternCompiler->patternData()->GetNext(pos);

		if (strcmp(name, pg->name) == 0) {
			return pg->pc;
		}
	}

	return 0;
}

void SWControl::tsCompile(const char *group, const char *name, const char *source)
{
	char msg[132];
	TTGDATA *tg;
	
	if( m_shmoo_init == true ) {
		
		if( m_tsCount == 0 )
			strcpy( ts_name, name );
		else
			strcat( ts_name, name );

		strcat( ts_name, "/" );
		m_tsCount++;
	}
#ifdef _TPG_GROUP
	m_timeCompiler = NULL;

	for (POSITION pos = m_tgGroup.GetHeadPosition(); pos;) {
		m_timeCompiler = m_tgGroup.GetNext(pos);

		if (m_timeCompiler->name() == group) {
			break;
		}
	}
#else
	if (m_timeCompiler && m_timeCompiler->name() != group) {
		delete m_timeCompiler;
		m_timeCompiler = NULL;
	}
	else if (m_timeCompiler && m_timeCompiler->error()) {
		return;
	}
#endif

	if (m_timeCompiler == NULL) {
		m_timeCompiler = new TGCompiler();
		//	m_timeCompiler->setModel(sysModel);
		m_timeCompiler->setName(group);
		m_timeCompiler->setCompTime(sysDelay(0));
#ifdef _TPG_GROUP
		m_tgGroup.AddTail(m_timeCompiler);
#endif
	}

	if (!m_timeCompiler) {
		return;
	}

	for (POSITION pos = m_timeCompiler->ts()->list.GetHeadPosition(); pos;) {
		tg = m_timeCompiler->ts()->list.GetNext(pos);

		if (strcmp(name, tg->name) == 0) {
			return;
		}
	}

	String strSource = source;

	m_timeCompiler->splitSyntex((char *) (const char *) strSource);

	if (m_timeCompiler->error()) {
		sprintf(msg, "TIMESET COMPILE ERROR : %s\n", name);
		//	runtimeError((const char *) msg);
		Send(BC_COMPILEERROR, (char *) m_timeCompiler->getError(), strlen((char *) m_timeCompiler->getError()) + 1);
		return;
	}
}

void SWControl::subCompile(const char *group, const char *name, const char *source)
{
	int old_pc = 0;
	char msg[132];
	TTGDATA *tg;
	TPGDATA *pg;

#ifdef _TPG_GROUP
	m_patternCompiler = NULL;

	for (POSITION pos = m_pgGroup.GetHeadPosition(); pos;) {
		m_patternCompiler = m_pgGroup.GetNext(pos);

		if (m_patternCompiler->name() == group) {
			old_pc = m_patternCompiler->pc();
			break;
		}
	}
#else
	if (m_patternCompiler && m_patternCompiler->name() != group) {
		delete m_patternCompiler;
		m_patternCompiler = NULL;
	}
	else if (m_patternCompiler && m_patternCompiler->error()) {
		return;
	}

	if (m_patternCompiler) {
		old_pc = m_patternCompiler->pc();
	}
#endif

	if (m_patternCompiler == NULL) {
		old_pc = 0;
		m_patternCompiler = new PGCompiler();
		//	m_patternCompiler->setModel(sysModel);
		m_patternCompiler->setName(group);
		m_patternCompiler->setCompTime(sysDelay(0));

		if (m_timeCompiler) {
			int timeset = 0;

			for (POSITION pos = m_timeCompiler->ts()->list.GetHeadPosition(); pos; timeset++) {
				tg = m_timeCompiler->ts()->list.GetNext(pos);
				m_patternCompiler->addTsGroup(tg->name, timeset);
			}
		}
#ifdef _TPG_GROUP
		m_pgGroup.AddTail(m_patternCompiler);
#endif
	}

	if (!m_patternCompiler) {
		return;
	}

	for (POSITION pos = m_patternCompiler->patternData()->GetHeadPosition(); pos;) {
		pg = m_patternCompiler->patternData()->GetNext(pos);

		if (strcmp(name, pg->name) == 0) {
			return;
		}
	}

	m_patternCompiler->setPC(old_pc == 0 ? 1 : old_pc);
	String strSource = source;
	m_patternCompiler->splitSyntex((char *) (const char *) strSource);

	if (m_patternCompiler->error()) {
		sprintf(msg, "PATTERN COMPILE ERROR : %s\n", name);
		//	runtimeError((const char *) msg);
		Send(BC_COMPILEERROR, (char *) m_patternCompiler->getError(), strlen((char *) m_patternCompiler->getError()) + 1);
		return;
	}
}

int SWControl::Recv(char *buf, unsigned int length)
{
	unsigned int total;
	int ret;

	for (total = 0; total < length;) {
		if ((ret = recv(m_sockfd, buf + total, length - total, 0)) < 0) {
			if (errno == EWOULDBLOCK || errno == EINTR) {
				continue;
			}

			return errno == ECONNRESET ? 0 : -1;
		}
		else if (ret == 0) {
			return 0;
		}

		total += ret;
	}

	return total;
}

int SWControl::Sync()
{
	unsigned int buf;
	unsigned int length;
	int ret;
	fd_set rfds;
	struct timeval tv;
	char *p;

	for (;;) {
		Send(BC_SYNC, &m_sync_index, sizeof(m_sync_index));
		FD_ZERO(&rfds);
		FD_SET(m_sockfd, &rfds);
		tv.tv_sec = 10;
		tv.tv_usec = 0;

		if ((ret = select(m_sockfd + 1, &rfds, NULL, NULL, &tv)) <= 0) {
			continue;
		}

		if (FD_ISSET(m_sockfd, &rfds)) {
			break;
		}
	}

	p = (char *) &buf;
	length = sizeof(buf);
	ret = Recv(p, length);
	m_sync_index = buf;
	return 0;
}

void SWControl::GET_LOT_YIELD( char *item_name, float lot_yield[32] )
{
	if (!m_all_skip) {
		char data[NRACK*NSLOT][15];

		Send(BC_GET_LOTYIELD, item_name, strlen(item_name) + 1);

		Recv((char *) data, sizeof( data ));

		for( unsigned int i = 0 ; i < 32 ; i++ ) {
			if( 0 == strcmp( data[i], "" ) )
				lot_yield[i] = -1;
			else
				lot_yield[i] = atof( data[i] );
		}
	}
}

void SWControl::GET_CUM_LOT_YIELD( float lot_yield[32] )
{
	if (!m_all_skip) {
		char data[NRACK*NSLOT][15];

		Send(BC_GET_CUMLOTYIELD, 0, 0);

		Recv((char *) data, sizeof( data ));

		for( unsigned int i = 0 ; i < 32 ; i++ ) {
			if( 0 == strcmp( data[i], "" ) )
				lot_yield[i] = -1;
			else
				lot_yield[i] = atof( data[i] );
		}
	}
}

void SWControl::SET_LOTBINSET( int select_lot, int select_bin )
{
	int temp[2];

	temp[0] = select_lot;
	temp[1] = select_bin;
	Send(BC_SET_LOTFAIL, (char *) temp, sizeof(temp));
}

void SWControl::SET_BOARD_DISABLE( int select_lot, bool state )
{
	int slot_data = 0;
	int temp[1];
	temp[0] = select_lot;

	Send(BC_SET_BOARD_DISABLE, (char*)temp, sizeof(temp));
	Recv((char *) &slot_data, 4);

	unsigned int b;

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		if( state == true ) {
			if ((slot_data >> b) & 1) {
				_bi_find[b].read = 0;
			}
		}
		else {
			if (!((slot_data >> b) & 1)) {
				_bi_find[b].read = 0;
			}
		}
	}
}

void SWControl::SET_BOARD_ENABLE()
{
	int iFault[NSLOT];

	for (unsigned int i = 0; i < NSLOT; i++) {
		if (!_bi_find[i].find_drv) {
			continue;
		}

		iFault[i] = readFault(i);

		if (iFault[i] != 0) {
			_bi_find[i].read = 0;
		}
		else {
			_bi_find[i].read = 1;
		}
	}
}

void SWControl::MULTIBIN_SET(int bin10, int bin11, int bin12, int bin13, int bin14, int bin15)
{
	int temp[6];

	temp[0] = bin10;
	temp[1] = bin11;
	temp[2] = bin12;
	temp[3] = bin13;
	temp[4] = bin14;
	temp[5] = bin15;

	Send(BC_MULTIBIN_SET, (char *) temp, sizeof(temp));
}

void SWControl::MULTIBIN_SET(int bin1, int bin2, int bin3, int bin4, int bin5, int bin6, int bin7, int bin8, int bin9, int bin10, int bin11, int bin12, int bin13, int bin14, int bin15)
{
	int temp[15];

	temp[0] = bin1;
	temp[1] = bin2;
	temp[2] = bin3;
	temp[3] = bin4;
	temp[4] = bin5;
	temp[5] = bin6;
	temp[6] = bin7;
	temp[7] = bin8;
	temp[8] = bin9;
	temp[9] = bin10;
	temp[10] = bin11;
	temp[11] = bin12;
	temp[12] = bin13;
	temp[13] = bin14;
	temp[14] = bin15;

	Send(BC_MULTIBIN_SET1, (char *) temp, sizeof(temp));
}

void SWControl::SET_WAFER_PDA( char *RUNNAME, int QTY_CNT, bool PDA_STATE )
{
	char data[100];
	snprintf( data, sizeof(data), "%s %d %d", RUNNAME, QTY_CNT, PDA_STATE );
	Send(BC_WAFERPDASET, data, sizeof(data));
}

void SWControl::SET_BIN_REF1(double binref1, double binref2, double binref3, double binref4, double binref5, double binref6, double binref7, double binref8, double binref9, double binref10, double binref11, double binref12, double binref13, double binref14, double binref15)
{
	char data[100];
	snprintf( data, sizeof(data), "%3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f", binref1, binref2, binref3, binref4, binref5, binref6, binref7, binref8, binref9, binref10, binref11, binref12, binref13, binref14, binref15 );
	Send(BC_SETBIN_REF1, data, sizeof(data));
}

void SWControl::SET_BIN_REF2(double binref1, double binref2, double binref3, double binref4, double binref5, double binref6, double binref7, double binref8, double binref9, double binref10, double binref11, double binref12, double binref13, double binref14, double binref15)
{
	char data[100];
	snprintf( data, sizeof(data), "%3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f", binref1, binref2, binref3, binref4, binref5, binref6, binref7, binref8, binref9, binref10, binref11, binref12, binref13, binref14, binref15 );
	Send(BC_SETBIN_REF2, data, sizeof(data));
}

void SWControl::SET_BIN_REF3(double binref1, double binref2, double binref3, double binref4, double binref5, double binref6, double binref7, double binref8, double binref9, double binref10, double binref11, double binref12, double binref13, double binref14, double binref15)
{
	char data[100];
	snprintf( data, sizeof(data), "%3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f", binref1, binref2, binref3, binref4, binref5, binref6, binref7, binref8, binref9, binref10, binref11, binref12, binref13, binref14, binref15 );
	Send(BC_SETBIN_REF3, data, sizeof(data));
}

void SWControl::InterLock_Time1(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {0, onoff, time};
	Send(BC_TIME_INTERLOCK, (char *)&t, sizeof(t));
}

void SWControl::InterLock_Time2(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {1, onoff, time};
	Send(BC_TIME_INTERLOCK, (char *)&t, sizeof(t));
}

void SWControl::InterLock_Time3(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {2, onoff, time};
	Send(BC_TIME_INTERLOCK, (char *)&t, sizeof(t));
}

void SWControl::InterLock_Time4(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {3, onoff, time};
	Send(BC_TIME_INTERLOCK, (char *)&t, sizeof(t));
}

void SWControl::InterLock_Time_AllStop()
{
	Send(BC_TIME_INTERLOCK_OFF,  0, 4);
}

void SWControl::INTERLOCK_TIME_MAX1(int onoff, int time)
{   
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {0, onoff, time}; 
	Send(BC_TIME_INTERLOCK_MAX1, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MAX2(int onoff, int time)
{   
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {1, onoff, time}; 
	Send(BC_TIME_INTERLOCK_MAX2, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MAX3(int onoff, int time)
{   
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {2, onoff, time}; 
	Send(BC_TIME_INTERLOCK_MAX3, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MAX4(int onoff, int time)
{   
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {3, onoff, time}; 
	Send(BC_TIME_INTERLOCK_MAX4, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MIN1(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {0, onoff, time};
	Send(BC_TIME_INTERLOCK_MIN1, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MIN2(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {1, onoff, time};
	Send(BC_TIME_INTERLOCK_MIN2, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MIN3(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {2, onoff, time};
	Send(BC_TIME_INTERLOCK_MIN3, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_MIN4(int onoff, int time)
{
	if( onoff < 0 ) onoff = 0;
	else if(onoff > 1 ) onoff = 1;

	int t[3] = {3, onoff, time};
	Send(BC_TIME_INTERLOCK_MIN4, (char *)&t, sizeof(t));
}

void SWControl::INTERLOCK_TIME_ALLSTOP()
{
	Send(BC_TIME_INTERLOCK_OFF, 0, 0);
}

bool SWControl::_APP(unsigned int id)
{
	if (id == ID_FM_STATUS) {
		m_scan = m_count = 0;	// BIST MODE
		Write(0x22, 1 | (m_16io_mode ? 8 : 0));
		Write(0x3f, 0);
	}
	else if (id == ID_FM_FAILS) {
		m_fm_page = 0;
		Write(0x22, 2 | (m_16io_mode ? 8 : 0));
		Write(0x23, 0x40);
		Write(0x3f, 0);
	}
	else if (id == ID_FM_BLOCK) {
		Write(0x22, 4 | (m_16io_mode ? 8 : 0));
		Write(0x3f, 0);
		Write(0x2a, 0);
	}
	else if (id >= ID_FAILS1 && id <= ID_FAILS8) {
		m_imprint_fail_count = id - ID_FAILS1;
		m_fm_page = 0;
		Write(0x22, 2 | (m_16io_mode ? 8 : 0));
		Write(0x23, 0x00 | (m_imprint_fail_count << 8));
		Write(0x3f, 0);
	}
	else if (id == ID_ECC) {
		app_id_ecc();
	}
	else if (id == ID_INIT) {
		app_id_init();
	}
	else if (id == ID_DATA) {
		app_id_data();
	}
	else if (id == ID_READ_ID) {
		app_id_read_id();
	}
	else if (id == ID_STATUS) {
		app_id_status();
	}
	else if (id == ID_READ_SCAN) {
		app_id_read_scan();
	}
	else if (id == ID_READ_BLOCK) {
		app_id_read_block();
	}
	else if (id == ID_READ_BLOCKS) {
		app_id_read_blocks();
	}
	else if (id == ID_L1_RESULT) {
		app_id_l1_result();
	}
	else if (id == ID_READ_ECC) {
		app_id_read_ecc();
	}
	else if (id == ID_READ_IO) {
		app_id_read_io();
	}
	else if (id == ID_READ_DISPLAY) {
		app_id_read_display();
	}
	else if (id == ID_PRETEST_DATA) {
		app_id_pretest_data();
	}
	else if (id == ID_PRETEST_RESULT) {
		app_id_pretest_result();
	}
	else if (id == ID_BITEST_DATA) {
		app_id_bitest_data();
	}
	else if (id == ID_BITEST_RESULT) {
		app_id_bitest_result();
	}
	else if (id == ID_FM_READ_DATA) {
		app_id_fm_read_data();
	}
	else if (id == ID_DQ_DATA) {
		app_id_dq_data();
	}
	else if (id == ID_DQ_RESULT) {
		app_id_dq_result();
	}
	else if (id == ID_BIST_IO_LATCH) {
		app_id_bist_io_latch();
	}
	else if (id == ID_BIST_DATA) {
		app_id_bist_data();
	}
	else if (id == ID_BIST_RESULT) {
		app_id_bist_result();
	}
	else if (id == ID_BIST2_RESULT) {
		app_id_bist2_result();
	}
	else if (id == ID_EMMC_IO_LATCH) {
		app_id_emmc_io_latch();
	}
	else if (id == ID_EMMC_DATA) {
		app_id_emmc_data();
	}
	else if (id == ID_EMMC_INIT) {
		app_id_emmc_init();
	}
	else if (id == ID_EMMC_RESULT) {
		app_id_emmc_result();
	}
	else if (id == ID_EMMC_BLOCK_RESULT) {
		app_id_emmc_block_result();
	}
	else if (id == ID_EMMC_CHIP_ID) {
		app_id_emmc_chip_id();
	}
	else if (id == ID_EMMC_READ_DATA) {
		app_id_emmc_read_data();
	}
	else if (id == ID_AGING) {
		if (m_aging_use) {
			int ct = ::time(0);

			if ((ct - m_aging_stime) >= m_aging_etime) {
				m_aging_end = true;
			}
		}
	}
	else {
		APP(id);
	}

	return false;
}

bool SWControl::APP(int /*value*/)
{
	return false;
}

int SWControl::SET_PF(unsigned int mode)
{
	if( m_pf_state == true ) {
		m_pf_state = false;
		return 0;
	}

	unsigned int b, x, y, n, mcp, block;
	unsigned int mcp_block_limit = m_block_limit / (m_mcp == 0 ? 1 : m_mcp);

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < (unsigned int) m_bib_x; x++) {
			for (y = 0; y < (unsigned int) m_bib_y; y++) {
				bool dut_fail = false;
				unsigned int fail_limit, add_limit;

				add_limit = m_block_add_limit_step >= 0 ? m_block_add_limit_step : m_block_add_limit;
				fail_limit = m_fail_mode == ADD_FAIL ? add_limit : m_block_fail_limit;

				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				for (mcp = 0; mcp < (unsigned int) m_mcp; mcp++) {
					//	printf(" mcp loop : %d ", mcp );
					bool chip_fail = false;
					unsigned int fail_n = 0, cum_fail_n = 0;
					char data=0x0;

					for (block = 0; block < mcp_block_limit; block+=8) {
						if( chip_fail ) //for time reduce
							break;

						unsigned int mcp_block = block + mcp_block_limit * mcp;

						//	printf("mcp : %d / block : %d / mcp_block_limit : %d / mcp_block : %d", mcp, block, mcp_block_limit, mcp_block );

						data = IS_STEP_BLOCK_FAIL8(b, x, y, mcp_block);

						if( data ) {
							//	printf("data : %x", data );
							for( int i=0; i<8;i++){ //8bit loop
								if( (data >>i) & 0x1 ){
									//	printf(" mcp : %d mcp_block(fail)1 : %x ", mcp, mcp_block+i );
									if( mcp_block+i < mcp_block_limit*m_mcp ){
										//	printf(" mcp : %d mcp_block(fail)2 : %x ", mcp, mcp_block+i );
										if (IS_STEP_BLOCK_FAIL(b, x, y, mcp_block+i)){
											if (mode == BLOCK) {
												dut_fail |= true;
												chip_fail |= true;
											}
											else {
												if (m_fail_mode == ADD_FAIL) {
													if (!IS_INIT_BLOCK_FAIL(b, x, y, mcp_block+i)) {
														fail_n++;
													}
												}
												else {
													fail_n++;
													//	printf("fail_n : %d ", fail_n );
												}
											}
										}
									}
								}
							}
						}

						chip_fail |= fail_n > fail_limit;
						dut_fail |= fail_n > fail_limit;

						data = IS_CUM_BLOCK_FAIL8(b, x, y, mcp_block);
						if (data) {
							for( int i=0; i<8;i++){ //8bit loop
								if( (data >>i) & 0x1 ){
									if( mcp_block+i < mcp_block_limit*m_mcp ){
										//	printf(" mcp : %d cum mcp_block(fail) : %x ",mcp,  mcp_block+i );
										if (IS_CUM_BLOCK_FAIL(b, x, y, mcp_block+i)){
											cum_fail_n++;
										}
									}
								}
							}
						}
					}

					if (m_fail_mode != ADD_FAIL && cum_fail_n > fail_limit) {
						if (!IS_CUM_FAIL_MASK(b, x, y)) {
							dut_fail |= true;
							chip_fail |= true;
							SET_CUM_FAIL_MASK(b, x, y, true);
						}
					}

					if ((m_fail_mode == MUST_FAIL && !chip_fail) || chip_fail) {
						for (n = 0; n < (unsigned int) m_nchip; n++) {
							unsigned int chip_index = mcp * m_nchip + n;
							SET_CUM_FAIL_CHIP(b, x, y, chip_index, true);
							SET_STEP_FAIL_CHIP(b, x, y, chip_index, true);
						}
					}
				}

				if (m_fail_mode == MUST_FAIL) {
					dut_fail = !dut_fail;
				}

				if (dut_fail) {
					SET_CUM_FAIL(b, x, y, true);
					SET_STEP_FAIL(b, x, y, true);
				}
			}
		}
	}

	return 0;
}

int SWControl::SET_FM_READ_DATA(unsigned int base, unsigned int i_block, unsigned int index)
{
	return 0;
}

int SWControl::SET_FM(int board, unsigned int fm_mode, int s_scan, int e_scan)
{
	unsigned int io;
	int fail;
	int s, scan;
	int sscan, escan;
	int scr_addr = 0x10000;	// *0x10000*xy
	int vloop_addr = 0x9000;	// *100*xy;
	int xscr_addr = 0x10000;
	int yscr_addr = 0x20000;
	int set = 0;		// 0=count
	short fail_count;
	unsigned int data;
	unsigned int mode;
	unsigned int start_mcp, end_mcp;
	unsigned short v = Read(0x4003);

	start_mcp = (m_start_mcp == 0) ? 0 : m_start_mcp - 1;
	end_mcp = (m_end_mcp == 0) ? m_mcp : m_end_mcp;
	v = v & 0xff00;
	sysWrite(0x4003, v);
	mode = sysRead(0x4022);
	sysWrite(0x4022, (mode & ~0x20) | 1);
	mode = sysRead(0x4042);
	sysWrite(0x4042, (mode & ~0x20) | 1);

	if (s_scan == -1) {
		sscan = 0;
	}
	else {
		sscan = s_scan;
	}

	if (e_scan == -1) {
		escan = m_bib_scan;
	}
	else {
		escan = e_scan;
	}

	s = 0;

	for (scan = sscan; scan <= escan; scan++) {
		for (io = 0; io < m_bib_io_count_per_1scan; io++) {
			unsigned int dut;
			unsigned int dut_index;

			if (si_to_index(scan, io, dut_index) < 0) {
				continue;
			}

			dut = board * m_bib_x * m_bib_y + dut_index;

			if (fm_mode == MEMORY) {
				fail_count = (short) m_failblock[dut][FAIL_BLOCK_LIMIT + 2];

				if (m_init_fail[dut] & 1) {
					fail_count = 0xffff;
				}

				if (fail_count == 0xff || fail_count == -1) {
					fail_count = 0;

					if (m_app_value == true) {
						setScanVector(s, 0);
					}
				}

				for (fail = 0; fail < fail_count; fail++) {
					data = (m_failblock[dut][fail]);
					Write(scr_addr + set++, data);
				}

				if (fail_count) {
					fail_count = 1;
				}

				Write(vloop_addr + scan * m_bib_io_count_per_1scan + io, fail_count);
				s++;
				continue;
			}

			for (unsigned int mcp = start_mcp; mcp < end_mcp; mcp++) {
				int mcp_size = m_block_fail_limit;

				fail_count = CUM_BLOCK_FAIL_COUNT(board, dut_index, mcp);

				if (BIN(board, dut_index) != 1) {
					fail_count = 0xffff;
				}
				else if (BIN(board, dut_index) == 1 && fail_count > mcp_size) {
					fail_count = mcp_size;
				}

				if (m_init_fail[dut] & 1) {
					fail_count = 0xffff;
				}

				if (fm_mode == CD_WRITE) {
					if (m_test_mode == 0 && m_16io_mode) {
						Write(scr_addr + set++, fail_count);
					}
					else if (m_test_mode == 1) {
						Write(scr_addr + set++, fail_count | 0xff00);
						Write(scr_addr + set++, fail_count | 0xff00);
					}
					else if (m_test_mode == 2) {
						Write(scr_addr + set++, (fail_count & 0xff) << 8 | 0x00ff);
						Write(scr_addr + set++, (fail_count & 0xff) << 8 | 0x00ff);
					}
					else {
						Write(scr_addr + set++, fail_count | 0xff00);
						Write(scr_addr + set++, fail_count | 0xff00);
					}
				}

				if (fail_count == 0xff || fail_count == -1) {
					fail_count = 0;

					if (m_app_value == true) {
						setScanVector(s, 0);
					}
				}

				if (fm_mode == CD_WRITE) {
					for (fail = 0; fail < fail_count; fail++) {
						data = CUM_BLOCK_FAIL_ADDRESS(board, dut_index, mcp, fail);

						if (m_test_mode == 0 && m_16io_mode) {
							Write(scr_addr + set++, data);
						}
						else if (m_test_mode == 1) {
							Write(scr_addr + set++, (data >> 8) | 0xff00);
							Write(scr_addr + set++, (data & 0xff) | 0xff00);
						}
						else if (m_test_mode == 2) {
							Write(scr_addr + set++, data & 0xff00 | 0x00ff);
							Write(scr_addr + set++, (data & 0xff) << 8 | 0x00ff);
						}
						else {
							Write(scr_addr + set++, (data >> 8) | 0xff00);
							Write(scr_addr + set++, (data & 0xff) | 0xff00);
						}
					}
				}
				else if (fm_mode == IMPRINT) {
					for (fail = 0; fail < fail_count; fail++) {
						data = CUM_BLOCK_FAIL_ADDRESS(board, dut_index, mcp, fail) << m_block_shift;
						Write(xscr_addr + set, (data) & 0xffff);
						Write(yscr_addr + set, (data >> 16) & 0xffff);
						set++;
					}
				}

				Write(vloop_addr + ((scan - sscan) *
							m_bib_io_count_per_1scan + io) *
						(end_mcp - start_mcp) + mcp - start_mcp, fail_count);
				s++;
			}
		}
	}

	return set;
}

int SWControl::SET_EMMC_READ_DATA(unsigned int index)
{
	return 0;
}

int SWControl::SET_DRAM_READ_CHIPID(unsigned int index)
{
	return 0;
}

void SWControl::SET_BLOCK(int addr, int s, int &address, int &scan)
{
	address = (unsigned int) addr >> m_block_shift;
	scan = s / m_mcp;
}

bool SWControl::RUNTIME_ERROR(const char *code)
{
	return false;
}

bool SWControl::BURST(const char *pattern_name, int nBurst)
{
	return false;
}

bool SWControl::PAUSE(const char *pattern_name)
{
	return false;
}

int SWControl::OPTION_CALL(int Command, int wParam, int lParam)
{
	return 0;
}

void SWControl::INIT_READ_CHIPID()
{
	if (m_chipid == 0) {
		m_chipid = new char[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * 64];
		memset(m_chipid, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * 64);
	}

	if (m_waferid == 0) {
		m_waferid = new unsigned int[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
		memset(m_waferid, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
	}

	if (m_xpos == 0) {
		m_xpos = new unsigned int[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
		memset(m_xpos, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
	}

	if (m_ypos == 0) {
		m_ypos = new unsigned int[m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip];
		memset(m_ypos, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
	}
}

void SWControl::getHICInfo(unsigned short chip_count, unsigned short data_length, unsigned short chip_size, const char *data_type)
{
	char buf[1024] = { 0 };
	m_hic_count = chip_count;
	m_hic_length = data_length;
	m_hic_size = chip_size;
	strcpy(m_hic_type, data_type);

	if (m_read_nand_chipid == false) {
		return;
	}

	snprintf(buf, sizeof(buf), "%08u,%s", chip_count, data_type);
	Send(BC_EMMC_CHIPID_DATA, buf, sizeof(buf));
}

void SWControl::DRAM_READ_CHIP_DATA(unsigned short chip_count, const char *data_type)
{
	char buf[1024] = { 0 };
	m_dram_read_chipid_count = chip_count;
	//	strcpy(m_hic_type, data_type);
	snprintf(buf, sizeof(buf), "%08u,%s", chip_count, data_type);
	Send(BC_DRAM_CHIPID_DATA, buf, sizeof(buf));
}

void SWControl::EMMC_READ_CHIP_DATA(unsigned short chip_count, const char *data_type)
{
	char buf[1024] = { 0 };
	m_hic_count = chip_count;
	strcpy(m_hic_type, data_type);
	snprintf(buf, sizeof(buf), "%08u,%s", chip_count, data_type);
	Send(BC_EMMC_CHIPID_DATA, buf, sizeof(buf));
}

void SWControl::CALC_HICHIP()
{
	const unsigned int _DATA_LENGTH = m_hic_length;
	unsigned int scan, io, max_val = 0, max_cnt = 0;
	unsigned int dut_index;
	unsigned int index, ddp_pos;
	unsigned short buf;
	bool bMatch;

	GET_MEMORYDATA(NORMAL);

	for (unsigned int board = 0; board < NSLOT; board++) {
		if (!_bi_find[board].read) {
			continue;
		}

		for (scan = 0; scan < m_bib_scan; scan++) {
			for (io = 0; io < m_bib_io_count_per_1scan; io++) {
				int cnt[_DATA_LENGTH];
				unsigned short median[_DATA_LENGTH];
				unsigned short buffer[_DATA_LENGTH][256];

				if (si_to_index(scan, io, dut_index) < 0) {
					continue;
				}

				dut_index = board * m_bib_x * m_bib_y + dut_index;

				for (unsigned int ddp = 0; ddp < m_hic_count; ddp++) {
					ddp_pos = (0x100 / m_hic_count) * ddp;

					for (unsigned int j = 0; j < _DATA_LENGTH; j++) {
						for (int y = 0; y < 256; y++) {
							buffer[j][y] = 0;
						}

						cnt[j] = 0;
					}

					for (index = ddp_pos; index < ((0x100 / m_hic_count) * (ddp + 1)); index++) {
						bMatch = false;

						if (index == ddp_pos) {
							buffer[index - ddp_pos][0] = m_failblock[dut_index][index] & 0xff | (0x1 << 8);
							cnt[index - ddp_pos] = 1;
						}
						else if (index - ddp_pos < _DATA_LENGTH) {
							buffer[index - ddp_pos][0] = m_failblock[dut_index][index] & 0xff | (0x1 << 8);
							cnt[index - ddp_pos] = 1;
						}
						else {
							for (int c = 0; c < cnt[(index - ddp_pos) % _DATA_LENGTH]; c++) {
								if ((buffer[(index - ddp_pos) % _DATA_LENGTH][c] & 0xff) ==
										(m_failblock[dut_index][index] & 0xff)) {
									buf = (buffer[(index - ddp_pos) % _DATA_LENGTH][c] >> 8) &
										0xff;
									buffer[(index - ddp_pos) % _DATA_LENGTH][c] =
										((buffer[(index - ddp_pos) %
										  _DATA_LENGTH][c]) & 0xff) |
										((buf + 1) << 8);
									bMatch = true;
								}
							}

							if (!bMatch) {
								int a = cnt[(index - ddp_pos) % _DATA_LENGTH];
								int b = (m_failblock[dut_index][index] & 0xff) | (0x1 << 8);
								buffer[(index - ddp_pos) % _DATA_LENGTH][a] = b;
							}
						}
					}

					for (unsigned int jj = 0; jj < _DATA_LENGTH; jj++) {
						for (int c = 0; c < cnt[jj]; c++) {
							if (c == 0) {
								max_val = (buffer[jj][c] >> 8) & 0xff;
								max_cnt = c;
							}
							else {
								if (max_val < (buffer[jj][c] >> 8) & 0xff) {
									max_val = (buffer[jj][c] >> 8) & 0xff;
									max_cnt = c;
								}
							}
						}

						median[jj] = buffer[jj][max_cnt];
					}

					for (unsigned int jjj = 0; jjj < _DATA_LENGTH; jjj++) {
						m_failblock[dut_index][(ddp * _DATA_LENGTH) + jjj] = median[jjj] & 0xff;
					}
				}
			}
		}
	}

	SAVE_HICHIP(m_hic_type);
}

unsigned char SWControl::find_max_value(unsigned char *buf, unsigned int len)
{
	unsigned int i;
	unsigned int j;
	unsigned int index;
	unsigned int max_val;
	unsigned int max_index;
	unsigned int v[len];
	unsigned int count[len];

	memset(v, 0xffffffff, sizeof(v));
	memset(count, 0, sizeof(count));

	for (index = 0, i = 0; i < len; i++) {
		bool found = false;

		for (j = 0; j < index && j < len; j++) {
			if (buf[i] == v[j]) {
				count[j]++;
				found = true;
				break;
			}
		}

		if (found == false) {
			v[index] = buf[i];
			index++;
		}
	}

	for (max_val = 0, max_index = 0, i = 0; i < index; i++) {
		if (count[i] > max_val) {
			max_val = count[i];
			max_index = i;
		}
	}

	return v[max_index];
}

int SWControl::do_bistdata_fmt_1(unsigned int b, unsigned int x, unsigned int y)
{
	unsigned int dut_index;
	unsigned int i;
	unsigned short status;
	unsigned short last_fail_testnumber;
	unsigned char *p;
	bool bfind = false;
	bool fail = false;

	xy_to_index(x, y, dut_index);
	p = (unsigned char *) (m_bist[b][dut_index] + 2);
	m_base_cat_step = m_base_cat_step == 0 ? m_step_no : m_base_cat_step;
	m_bist_errno_no = m_bist_errno_no < m_base_cat_step ? m_base_cat_step : m_bist_errno_no;
	status = p[10] << 8 | p[11];
	last_fail_testnumber = p[12] << 8 | p[13];

	if (last_fail_testnumber < m_base_cat_step || last_fail_testnumber > 200 || status >= 0xff) {
		status = 1;
		last_fail_testnumber = m_bist_errno_no;
	}
	else {
		for (bfind = false, i = 0; i < 200 && m_cat_action_cate[i]; i++) {
			if (last_fail_testnumber == m_cat_action_cate[i]) {
				bfind = true;
				break;
			}
		}

		last_fail_testnumber = bfind == false ? m_bist_exception_cate : last_fail_testnumber;
	}

	fail = (bool) status;

	if (fail) {
		SET_CUM_FAIL(b, x, y, true);
		SET_FCAT(b, x, y, last_fail_testnumber);
	}

	m_use_mode |= 0x4;
	Send(BC_BISTDATA, m_bist[b][dut_index], sizeof(char) * (MAX_BIST_SIZE + 2));
	return 0;
}

int SWControl::do_bistdata_fmt_2(unsigned int b, unsigned int x, unsigned int y)
{
	unsigned int dut_index;
	xy_to_index(x, y, dut_index);
	m_base_cat_step = m_base_cat_step == 0 ? m_step_no : m_base_cat_step;
	m_use_mode |= 0x4;
	Send(BC_BISTDATA2, m_bist[b][dut_index], sizeof(char) * (MAX_BIST_SIZE + 2));
	return 0;
}

int SWControl::do_emmcdata_fmt_0(unsigned int b, unsigned int x, unsigned int y)
{
	unsigned int dut_index;
	unsigned char *p;
	unsigned int mode = 0;
	bool fail = false;
	unsigned long long value = 0llu;

	xy_to_index(x, y, dut_index);
	p = (unsigned char *) (m_bist[b][dut_index] + 2);
	mode = m_emmc_compare_bit == 0 && m_emmc_compare_value == 0llu ? 0 : (m_emmc_compare_bit & 0x80000000) ? 1 : 2;

	if (mode == 1) {
		fail = (bool) ((p[m_emmc_compare_bit / 8] >> (7 - m_emmc_compare_bit % 8)) & 1);
		fail = !fail;
	}
	else if (mode == 2) {
		unsigned int v_index, b_index;

		for (unsigned int i = 0; i < m_emmc_compare_bit; i++) {
			v_index = i / 8;
			b_index = 7 - i % 8;
			value |= (p[v_index] >> b_index & 0x1llu) << (m_emmc_compare_bit - i - 1);
		}

		if (value != m_emmc_compare_value) {
			fail = true;
		}
	}

	if (fail == true) {
		SET_PRE_FAIL(b, x, y, true);
	}

	if (m_emmc_init_display_count > 0) {
		char buf[1024] = { 0 };
		snprintf(buf, sizeof(buf), "b : %d, x : %2d, y : %2d, bit : %d, v : %llx, value : %llx, ",
				b, x, y, m_emmc_compare_bit, m_emmc_compare_value, value);

		for (unsigned int i = 0; i < m_emmc_init_display_count; i++) {
			snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "0x%02x ", p[i]);
		}

		Send(BC_PRINTF, buf, strlen(buf));
	}

	return 0;
}

int SWControl::do_emmcdata_fmt_1(unsigned int b, unsigned int x, unsigned int y)
{
	unsigned int dut_index;
	unsigned char *p;
	unsigned int mode = 0;
	bool fail = false;
	unsigned long long value = 0llu;

	xy_to_index(x, y, dut_index);
	p = (unsigned char *) (m_bist[b][dut_index] + 2);
	mode = m_emmc_compare_bit == 0 && m_emmc_compare_value == 0llu ? 0 : (m_emmc_compare_bit & 0x80000000) ? 1 : 2;

	if (mode == 1) {
		fail = (bool) ((p[m_emmc_compare_bit / 8] >> (7 - m_emmc_compare_bit % 8)) & 1);
		fail = !fail;
	}
	else if (mode == 2) {
		unsigned int v_index, b_index;

		for (unsigned int i = 0; i < m_emmc_compare_bit; i++) {
			v_index = i / 8;
			b_index = 7 - i % 8;
			value |= (p[v_index] >> b_index & 0x1llu) << (m_emmc_compare_bit - i - 1);
		}

		if (value != m_emmc_compare_value) {
			fail = true;
		}
	}

	fail = m_fail_mode == MUST_FAIL ? !fail: fail;

	if (fail == true) {
		SET_STEP_FAIL(b, x, y, true);
		SET_CUM_FAIL(b, x, y, true);

		for (unsigned int mcp = 0; mcp < (unsigned int) m_mcp; mcp++) {
			for (unsigned int n = 0; n < (unsigned int) m_nchip; n++) {
				unsigned int chip_index = mcp * m_nchip + n;
				SET_CUM_FAIL_CHIP(b, x, y, chip_index, true);
				SET_STEP_FAIL_CHIP(b, x, y, chip_index, true);
			}
		}
	}

	if (m_emmc_test_display_count > 0) {
		char buf[1024] = { 0 };
		snprintf(buf, sizeof(buf), "b : %d, x : %2d, y : %2d, bit : %d, v : %llx, value : %llx, ",
				b, x, y, m_emmc_compare_bit, m_emmc_compare_value, value);

		for (unsigned int i = 0; i < m_emmc_test_display_count; i++) {
			snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "0x%02x ", p[i]);
		}

		Send(BC_PRINTF, buf, strlen(buf));
	}

	return 0;
}

int SWControl::do_emmcdata_fmt_2(unsigned int b, unsigned int x, unsigned int y)
{
	unsigned int block_index = 0;
	unsigned int dut_index;
	bool set_block = true;
	unsigned char *p;

	xy_to_index(x, y, dut_index);
	p = (unsigned char *) (m_bist[b][dut_index] + 2);

	for (unsigned int i = 0; i < MAX_BIST_SIZE; i++) {
		for (unsigned int j = 0; j < 8; j++, block_index++) {
			bool fail = (p[i] >> j) & 1 ? true : false;

			if (fail == false) {
				continue;
			}

			if (set_block) {
				SET_STEP_BLOCK_FAIL(b, x, y, block_index, true);
				SET_CUM_BLOCK_FAIL(b, x, y, block_index, true);
			}

			if (m_fail_mode == INIT_FAIL) {
				SET_INIT_BLOCK_FAIL(b, x, y, block_index, true);
			}
		}
	}

	if (m_emmc_test_display_count > 0) {
		char buf[1024] = { 0 };
		snprintf(buf, sizeof(buf), "b : %d, x : %2d, y : %2d, bit : %d, v : %llx, ",
				b, x, y, m_emmc_compare_bit, m_emmc_compare_value);

		for (unsigned int i = 0; i < m_emmc_test_display_count; i++) {
			snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "0x%02x ", p[i]);
		}

		Send(BC_PRINTF, buf, strlen(buf));
	}

	return 0;
}

int SWControl::do_emmcdata_fmt_3(unsigned int b, unsigned int x, unsigned int y)
{
	unsigned int MAX_COUNT = m_emmc_chipid_unit_count == 0 ? 32 : m_emmc_chipid_unit_count;
	unsigned int dut_index;
	unsigned char *p;
	unsigned char chipid0[MAX_COUNT];
	unsigned char chipid1[MAX_COUNT];
	unsigned char chipid2[MAX_COUNT];
	unsigned char chipid3[MAX_COUNT];
	unsigned char wfid[MAX_COUNT];
	unsigned char xpos[MAX_COUNT];
	unsigned char ypos[MAX_COUNT];
	char chipid[64] = { 0 };

	xy_to_index(x, y, dut_index);
	p = (unsigned char *) (m_bist[b][dut_index] + 2);
	memset(chipid0, 0, sizeof(MAX_COUNT));
	memset(chipid1, 0, sizeof(MAX_COUNT));
	memset(chipid2, 0, sizeof(MAX_COUNT));
	memset(chipid3, 0, sizeof(MAX_COUNT));
	memset(wfid, 0, sizeof(MAX_COUNT));
	memset(xpos, 0, sizeof(MAX_COUNT));
	memset(ypos, 0, sizeof(MAX_COUNT));

	for (unsigned int i = 0; i < MAX_COUNT; i++) {
		chipid0[i] = p[0 + i * 8];
		chipid1[i] = p[1 + i * 8];
		chipid2[i] = p[2 + i * 8];
		chipid3[i] = p[3 + i * 8];
		wfid[i] = p[4 + i * 8];
		xpos[i] = p[5 + i * 8];
		ypos[i] = p[6 + i * 8];
	}

	chipid[0] = find_max_value(chipid0, MAX_COUNT);
	chipid[1] = find_max_value(chipid1, MAX_COUNT);
	chipid[2] = find_max_value(chipid2, MAX_COUNT);
	chipid[3] = find_max_value(chipid3, MAX_COUNT);

	unsigned int bd_index = b * m_bib_x * m_bib_y * m_mcp * m_nchip;
	unsigned int chip_index = m_emmc_ce * m_nchip;
	unsigned char aa, bb, cc;
	dut_index = dut_index * m_mcp * m_nchip;
	snprintf(m_chipid + bd_index * 64 + dut_index * 64 + chip_index * 64, 64, "%s", chipid);
	m_waferid[bd_index + dut_index + chip_index] = aa = find_max_value(wfid, MAX_COUNT);
	m_xpos[bd_index + dut_index + chip_index] = bb = find_max_value(xpos, MAX_COUNT);
	m_ypos[bd_index + dut_index + chip_index] = cc = find_max_value(ypos, MAX_COUNT);

	if (m_emmc_test_display_count > 0) {
		char buf[1024] = { 0 };
		snprintf(buf, sizeof(buf), "b : %d, x : %2d, y : %2d, %s %2x %2d %2d\t", b + 1, x + 1, y + 1, chipid, aa, bb, cc);

		for (unsigned int i = 0; i < m_emmc_test_display_count; i++) {
			snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "0x%02x ", p[i]);
		}

		Send(BC_PRINTF, buf, strlen(buf));
	}

	return 0;
}

int SWControl::do_emmcdata_comp_expect(char *buf)
{
	if (m_emmc_expect_bit == 0 || m_emmc_expect_value == 0llu) {
		return 0;
	}

	bool find = false;
	unsigned int i, j;

	for (i = 0; i < 256; i++) {
		unsigned long long value = 0llu;

		for (j = 0; j < m_emmc_expect_bit; j++) {
			value |= (buf[i + j] & 0x1llu) << (m_emmc_expect_bit - j - 1);
		}

		if (value == m_emmc_expect_value) {
			find = true;
			break;
		}
	}

	return find == false ? -1 : i + m_emmc_expect_bit;
}

int SWControl::do_get_cum_data(unsigned int mode)
{
	int ret;

	if (mode & 0x1) {
		memset(m_cum_fail, 0, m_bib_x * m_bib_y * NSLOT);
		Send(BC_GET_DUT_PF_DATA_CUM, 0, 0);

		if ((ret = Recv((char *) m_cum_fail, m_bib_x * m_bib_y * NSLOT)) < 0) {
			return ret;
		}
	}

	if (mode & 0x2) {
		memset(m_cum_fail_chip, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);
		Send(BC_GET_CHIP_PF_DATA_CUM, 0, 0);

		if ((ret = Recv((char *) m_cum_fail_chip, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip)) < 0) {
			return ret;
		}
	}

	if (mode & 0x4) {
		memset(m_cum_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
		Send(BC_GET_BLOCK_PF_DATA_CUM, 0, 0);

		if ((ret = Recv((char *) m_cum_fail_block, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1))) < 0) {
			return ret;
		}
	}

	return 0;
}

int SWControl::do_get_cate_bin_data(unsigned int mode)
{
	int ret;

	if (mode & 0x1) {
		memset(m_fail_cate, 0, sizeof(m_fail_cate));
		Send(BC_GET_FCATE_DATA, 0, 0);

		if ((ret = Recv((char *) m_fail_cate, sizeof(m_fail_cate))) < 0) {
			return ret;
		}
	}

	if (mode & 0x2) {
		memset(m_pass_cate, 0, sizeof(m_pass_cate));
		Send(BC_GET_PCATE_DATA, 0, 0);

		if ((ret = Recv((char *) m_pass_cate, sizeof(m_pass_cate))) < 0) {
			return ret;
		}
	}

	if (mode & 0x4) {
		memset(m_bin, 0, sizeof(m_bin));
		Send(BC_GET_BIN_DATA, 0, 0);

		if ((ret = Recv((char *) m_bin, sizeof(m_bin))) < 0) {
			return ret;
		}
	}

	return 0;
}

int SWControl::STEP_START()
{
	unsigned int length;
	unsigned int buf = 0;
	char *p;

	if (m_diagnostic == true) {
		return 1;
	}

	if (Read(0x406a) == 0 && (m_scan_mode & 0xffff) != 0) {
		Write(0x406a, m_scan_mode & 0xffff);
	}

	m_pf_state = false;
	m_dq_mode = false;
	m_dq_use = false;
	m_hottemp = false;
	m_read_nand_chipid = false;
	m_use_excl_mask = true;
	m_test_mode = 0;
	m_use_mode = 0;
	m_test_count = 0;
	m_fail_mode = NORMAL;
	m_judge_mode = JUD_NORMAL;
	m_mpat_limit_time = 0;
	m_now_step_count = 0;
	m_set_step_count = 1;
	m_fm_read_buf_index = 1;
	//	m_fm_read_scan_count = 1;
	m_fm_read_data_index = 0;
	m_block_add_limit_step = -1;
	m_rdq_index = 0;
	m_start_mcp = 0;
	m_end_mcp = 0;
	m_emmc_bit_value = 1;
	m_emmc_init_display_count = 0;
	m_emmc_test_display_count = 0;
	m_emmc_chipid_unit_count = 0;
	m_emmc_fm_display_count = 0;
	m_emmc_compare_bit = 0;
	m_emmc_expect_bit = 0;
	m_emmc_compare_value = 0;
	m_emmc_expect_value = 0;
	m_tsCount = 0;
	memset(ts_name, 0, sizeof(ts_name));
	memset(m_step_fail, 0, m_bib_x * m_bib_y * NSLOT + 5);
	memset(m_step_io_fail, 0, sizeof(unsigned int) * (m_bib_x * m_bib_y * NSLOT + 5));
	memset(m_step_fail_chip, 0, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);
	memset(m_now_step_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
	Send(BC_STEPSTART, &m_cur_step, sizeof(m_cur_step));

	for (;;) {
		p = (char *) &buf;
		length = sizeof(buf);
		Recv(p, length);

		if (buf == BC_STEPSTART || m_diagnostic == true) {
			break;
		}
	}

	return 1;
}

int SWControl::STEP_END()
{
	if (m_diagnostic == true) {
		return 1;
	}

	if (m_init_test == true) {
		if( m_use_setmap ) {
			m_use_setmap = false;
			Send(BC_PREMAP_DATA, m_pre_init_fail, m_bib_x * m_bib_y * NSLOT);
		}

		Send(BC_INITDATA, m_init_fail, m_bib_x * m_bib_y * NSLOT);
		Send(BC_SCAN_SET, &m_scan_set, sizeof(unsigned int));
		
		if( m_shmoo_init == true ) {
			Send(BC_SHMOO_TIMESET_CNT, &m_tsCount, 4);
			Send(BC_SHMOO_TIMESET_LIST, ts_name, sizeof(ts_name));
		}
	}
	else {
		unsigned int *p = (unsigned int *) &m_step_fail[m_bib_x * m_bib_y * NSLOT + 1];
		p[0] = m_test_dut_count;
		m_step_fail[m_bib_x * m_bib_y * NSLOT] = m_use_mode;
		Send(BC_FCATE_DATA, m_fail_cate, sizeof(m_fail_cate));
		Send(BC_PCATE_DATA, m_pass_cate, sizeof(m_pass_cate));
		Send(BC_BIN_DATA, m_bin, sizeof(m_bin));
		Send(BC_STEP_TYPE, &m_step_type, sizeof(m_step_type));

		if (m_use_mode & 0x1) {
			Send(BC_DUT_PF_DATA_STEP, m_step_fail, m_bib_x * m_bib_y * NSLOT + 5);
			Send(BC_CHIP_PF_DATA_STEP, m_step_fail_chip, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip);
		}

		if (m_use_mode & 0x2) {
			Send(BC_BLOCK_PF_DATA_STEP, m_now_step_fail_block, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));

			if (m_fail_mode == INIT_FAIL) {
				Send(BC_INIT_PF_DATA, m_init_fail_block, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1));
			}
		}

		if (m_use_mode & 0x4) {
			do_get_cum_data(0x6);
		}

		if (m_read_nand_chipid == true) {
			RESULT_CHIPID_DATA(0);
		}
	}

	Sync();
	//	m_fail_index = 0;
	m_ecc_index = 0;
	m_dq_mode = false;
	Send(BC_STEPEND, &m_cur_step, sizeof(m_cur_step));
	return 1;
}

int SWControl::RESULT_CHIPID_DATA(unsigned int mode)
{
	if (mode == 0) {
		Send(BC_CHIPID, m_chipid, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * 64);
		Send(BC_WAFERID, m_waferid, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
		Send(BC_XPOS, m_xpos, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
		Send(BC_YPOS, m_ypos, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
	}
	else if (mode == 1) {
		Send(BC_CHIPID_DRAM, m_chipid, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * 64);
		Send(BC_WAFERID_DRAM, m_waferid, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
		Send(BC_XPOS_DRAM, m_xpos, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
		Send(BC_YPOS_DRAM, m_ypos, m_bib_x * m_bib_y * NSLOT * m_mcp * m_nchip * sizeof(unsigned int));
	}

	return 0;
}

int SWControl::app_id_ecc()
{
	m_fm_page = 0;

	if (m_ecc_fail != 0) {
		delete[] m_ecc_fail;
		m_ecc_fail = 0;
	}

	if (m_ecc_fail == 0) {
		m_ecc_fail = new unsigned int[0x10000];
	}

	if (m_ecc_fly_fail != 0) {
		delete[] m_ecc_fly_fail;
		m_ecc_fly_fail = 0;
	}

	if (m_ecc_fly_fail == 0) {
		m_ecc_fly_fail = new unsigned int[0x10000];
	}

	memset(m_ecc_block_fail, 0, m_bib_x * m_bib_y * NSLOT);
	m_ecc_index = 0;
	m_ecc_fly_count = 0;

	Write(0x22, 2 | (m_16io_mode ? 8 : 0));
	Write(0x23, 0x00 | (m_ecc_io << 8));
	Write(0x3f, 0);
	return 0;
}

int SWControl::app_id_init()
{
	unsigned int i;
	unsigned int mask = 0;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int fail_device;
	int block;
	int scan;
	int use_scan;
	bool fail;
	unsigned char ios[8];

	use_scan = Read(0x3a) >> 8;
	memset(m_dq_data, 0, m_bib_scan * sizeof(unsigned long long) * NSLOT);

	if (m_init_count++ == 0) {
		memset(m_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
		memset(m_pre_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
	}

	for (i = 0; i < io; i++) {
		mask |= 1 << i;
	}

	for (int s = 0; s < use_scan; s++) {
		unsigned long long tmpdq;

		Write(0x3a, s);
		SET_BLOCK(0, Read(0x40cf), block, scan);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			if (m_dq_mode == true) {
				getFailData(b, 0xff, s, 0, ios);

				for (i = 0, tmpdq = 0; i < 8; i++) {
					unsigned long long tmp = ios[i] & 0xff;
					tmpdq |= tmp << (i * 8);
				}

				m_dq_data[b * m_bib_scan + s] = tmpdq;
			}

			fail_device = Read(0x3c | (b << 8)) & 0xf0f;
			fail_device |= (fail_device >> 4);
			fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				fail = (fail_device >> i * io) & mask;
				fail = m_fail_mode == MUST_FAIL ? !fail : fail;
				m_init_fail[b * m_bib_x * m_bib_y + dut_index] |= fail;
				m_pre_init_fail[b * m_bib_x * m_bib_y + dut_index] = fail;

				if (!fail) {
					m_scan_set = s + 1;
				}
			}
		}
	}

	Write(0x3f, 0);

	if (m_dq_mode == true) {
		Send(BC_DQDATA, m_dq_data, m_bib_scan * sizeof(unsigned long long) * NSLOT);
	}

	if( m_use_setmap ) {
		m_use_setmap = false;
		Send(BC_PREMAP_DATA, m_pre_init_fail, m_bib_x * m_bib_y * NSLOT);
	}

	Send(BC_INITDATA, m_init_fail, m_bib_x * m_bib_y * NSLOT);
	Send(BC_SCAN_SET, &m_scan_set, sizeof(unsigned int));
	return 0;
}

int SWControl::app_id_data()
{
	unsigned int i;
	unsigned int mask = 0;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int mcp;
	unsigned int n;
	unsigned int fail_device;
	int block;
	int scan;
	int use_scan;
	bool v;
	bool fail;
	unsigned char ios[8];
	//	char test_data[m_bib_x * m_bib_y * NSLOT];

	m_pf_state = true;

	use_scan = Read(0x400e) + 1;

	if (m_test_count++ == 0) {
		if (m_test_data) {
			delete[] m_test_data;
			m_test_data = 0;
		}

		m_test_data = new char[m_bib_x * m_bib_y * NSLOT];
		memset(m_test_data, (m_fail_mode << 4), m_bib_x * m_bib_y * NSLOT);
	}

	memset(m_dq_data, 0, m_bib_scan * sizeof(unsigned long long) * NSLOT);
	m_use_mode |= 0x1;

	for (i = 0; i < io; i++) {
		mask |= 1 << i;
	}

	for (int s = 0; s < use_scan; s++) {
		unsigned long long tmpdq;

		Write(0x3a, s);
		SET_BLOCK(0, Read(0x40cf), block, scan);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			if (m_dq_mode == true) {
				getFailData(b, 0xff, s, 0, ios);

				for (i = 0, tmpdq = 0; i < 8; i++) {
					unsigned long long tmp = ios[i] & 0xff;
					tmpdq |= tmp << (i * 8);
				}

				m_dq_data[b * m_bib_scan + s] = tmpdq;

			}

			fail_device = Read(0x3c | (b << 8)) & 0xf0f;
			fail_device |= (fail_device >> 4);
			fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;
				unsigned int index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				index = b * m_bib_x * m_bib_y + dut_index;

				if (IS_PRE_FAIL(b, dut_index)) {
					continue;
				}

				fail = (fail_device >> i * io) & mask;
				fail = m_fail_mode == MUST_FAIL ? !fail : fail;
				//      m_test_data[index] |= fail;
				//      v = m_test_data[index] & 1;
				v = fail;

				if (v) {
					SET_STEP_FAIL(b, dut_index, v);
					SET_CUM_FAIL(b, dut_index, v);

					for (mcp = 0; mcp < (unsigned int) m_mcp; mcp++) {
						for (n = 0; n < (unsigned int) m_nchip; n++) {
							unsigned int chip_index = mcp * m_nchip + n;
							SET_CUM_FAIL_CHIP(b, dut_index, chip_index, true);
							SET_STEP_FAIL_CHIP(b, dut_index, chip_index, true);
						}
					}
				}
			}
		}
	}

	Write(0x3f, 0);

	if (m_dq_mode == true) {
		Send(BC_DQDATA, m_dq_data, m_bib_scan * sizeof(unsigned long long) * NSLOT);
	}

	return 0;
}

int SWControl::app_id_read_id()
{
	unsigned int i;
	unsigned int io = m_bib_io_size >> 3;
	int block;
	int scan;
	int use_scan;
	bool fail;
	unsigned char ios[8];
	unsigned char ios_tmp[8];
	unsigned short id[NSLOT * m_bib_x * m_bib_y];
	//	char msg[256];

	use_scan = Read(0x400e) + 1;
	memset(id, 0, sizeof(id));

	if (m_init_count++ == 0) {
		memset(m_init_fail, 3, m_bib_x * m_bib_y * NSLOT);
		memset(m_pre_init_fail, 3, m_bib_x * m_bib_y * NSLOT);
	}

	for (int s = 0; s < use_scan; s++) {
		Write(0x3a, s);
		SET_BLOCK(0, Read(0x40cf), block, scan);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			getFailData(b, 0xff, s * 2, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				id[b * m_bib_x * m_bib_y + dut_index] = ios[i * io] << 8;
			}

			getFailData(b, 0xff, s * 2 + 1, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				id[b * m_bib_x * m_bib_y + dut_index] |= ios[i * io];
				fail = (bool) (id[b * m_bib_x * m_bib_y + dut_index] != m_device_code);
				fail = m_fail_mode == MUST_FAIL ? !fail : fail;

				if (m_init_count == 1) {
					m_init_fail[b * m_bib_x * m_bib_y + dut_index] = fail | 0x2;
					m_pre_init_fail[b * m_bib_x * m_bib_y + dut_index] = fail | 0x2;
				}
				else {
					m_init_fail[b * m_bib_x * m_bib_y + dut_index] |= fail | 0x2;
					m_pre_init_fail[b * m_bib_x * m_bib_y + dut_index] = fail | 0x2;
				}

				if (m_init_fail[b * m_bib_x * m_bib_y + dut_index] != 3) {
					m_scan_set = s + 1;
				}
			}
		}
	}

	if( m_use_setmap ) {
		m_use_setmap = false;
		Send(BC_PREMAP_DATA, m_pre_init_fail, m_bib_x * m_bib_y * NSLOT);
	}

	Send(BC_DEVICEID, id, sizeof(id));
	Send(BC_INITDATA, m_init_fail, m_bib_x * m_bib_y * NSLOT);
	Send(BC_SCAN_SET, &m_scan_set, sizeof(unsigned int));
	setFMClear();
	return 0;
}

int SWControl::app_id_status()
{
	unsigned int i;
	unsigned int io = m_bib_io_size >> 3;
	int block;
	int scan;
	int use_scan;
	int i_block;
	int set = 0;
	unsigned int tvalue;
	unsigned char ios[8];
	unsigned char ios_tmp[8];

	use_scan = Read(0x3a) >> 8;
	Write(0x3a, 0);
	i_block = (Read(0x402f) | (Read(0x404f) << 16));
	SET_BLOCK(i_block, Read(0x40cf), block, scan);
	m_hichp_use = true;

	unsigned int *mem = new unsigned int[NSLOT * m_bib_io_count_per_1scan * use_scan + 1];
	memset(mem, 0, sizeof(int) * (NSLOT * m_bib_io_count_per_1scan * use_scan + 1));

	if (m_test_mode == 0) {
		tvalue = i_block;
	}
	else {
		tvalue = block / (m_block_limit / m_mcp) * (0x100 / (m_mcp * m_nchip));
	}

	mem[0] = scan | (tvalue << 16) | (use_scan << 24);

	for (int s = 0; s < use_scan; s++) {
		Write(0x3a, s);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			getFailData(b, 0xff, s, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				if (!(m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) {
					if (m_test_mode == 0) {
						if (io == 2) {
							mem[++set] = (ios[i] | (ios[i + 1] << 8)) |
								(i << 16) | (b << 19) | (s << 24) | 0x800000;
						}
						else {
							mem[++set] = (ios[i]) | (i << 16) | (b << 19) | (s << 24) | 0x800000;
						}
					}
					else {
						unsigned int tmp = m_test_mode == 0 || (m_test_mode & 0x1) == 1 ? 0 : 1;
						mem[++set] = (ios[i * io + tmp]) | (i << 16) | (b << 19) | (s << 24) | 0x800000;
					}
				}
			}
		}
	}

	Write(0x3f, 0);
	Send(BC_SETMEMORY, mem, sizeof(int) * set);
	delete mem;
	return 0;
}

int SWControl::app_id_read_scan()
{
	unsigned int i;
	unsigned int index;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int fail_device;
	int block;
	int scan;
	bool fail;
	bool set_block = true;

	m_use_mode |= 0x3;
	Write(0x3a, 0);
	SET_BLOCK((Read(0x402f) | (Read(0x404f) << 16)), Read(0x40cf), block, scan);

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		fail_device = Read(0x38 | (b << 8)) & 0xf;
		fail_device |= (Read(0x58 | (b << 8)) & 0xf) << 4;
		fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);
		fail = 0;

		for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
			unsigned int dut_index;

			if (si_to_index(scan, i, dut_index) < 0) {
				continue;
			}

			index = b * m_bib_x * m_bib_y + dut_index;

			if (!((m_init_fail[index]) & 1) && ((fail_device >> i * io) & 1)) {
				if (set_block) {
					SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
					SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
				}

				if (m_fail_mode == INIT_FAIL) {
					SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
				}
			}
		}
	}

	Write(0x2a, 0);
	Write(0x3f, 0);

	//	if (m_fail_index > 60000) {
	//		SEND_FAIL_DATA();
	//	}

	return 0;
}

int SWControl::app_id_read_block()
{
	unsigned int i;
	unsigned int index;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int fail_device;
	int block;
	int scan;
	int use_scan;
	int i_block;
	bool set_block = true;
	//	char tmpbuf[8192] = { 0 };

	use_scan = Read(0x400e) + 1;
	m_use_mode |= 0x3;
	Write(0x3a, 0);
	i_block = (Read(0x402f) | (Read(0x404f) << 16));

	for (int s = 0; s < use_scan; s++) {
		Write(0x3a, s);
		SET_BLOCK(i_block, Read(0x40cf), block, scan);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			fail_device = Read(0x3c | (b << 8)) & 0xf0f;
			/*	snprintf(tmpbuf + strlen(tmpbuf), sizeof(tmpbuf) - strlen(tmpbuf),
				"use_scan: %3d, i_block: 0x%04x, s: %3d, fail_device: 0x%x\r\n",
				use_scan, i_block, s, fail_device);*/
			fail_device |= (fail_device >> 4);
			fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				index = b * m_bib_x * m_bib_y + dut_index;

				if ((!(m_init_fail[index] & 1)) && ((fail_device >> i * io) & 1)) {
					if (set_block) {
						SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
						SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
					}

					if (m_fail_mode == INIT_FAIL) {
						SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
					}
				}
			}
		}
	}

	//	Send(BC_PRINTF, tmpbuf, strlen(tmpbuf));
	Write(0x3f, 0);
	return 0;
}

int SWControl::app_id_read_blocks()
{
	unsigned int i;
	unsigned int index;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int fail_device;
	int block;
	int scan;
	int addr = (m_fm_page & 1) * 128, blocks;
	bool set_block = true;

	m_use_mode |= 0x3;
	Write(0x4002, 7);

	for (blocks = 0; blocks < 128; blocks++) {
		Write(0x3a, addr + blocks);
		SET_BLOCK((Read(0x402f) | (Read(0x404f) << 16)), Read(0x40cf) , block, scan);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			fail_device = Read(0x38 | (b << 8)) & 0xf;
			fail_device |= (Read(0x58 | (b << 8)) & 0xf) << 4;
			fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);

			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				unsigned int dut_index;

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				index = b * m_bib_x * m_bib_y + dut_index;

				if (!((m_init_fail[index]) & 1) && ((fail_device >> i * io) & 1)) {
					if (set_block) {
						SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
						SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
					}

					if (m_fail_mode == INIT_FAIL) {
						SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
					}
				}
			}
		}
	}

	m_fm_page++;
	return 0;
}

int SWControl::app_id_l1_result()
{
	unsigned int i;
	unsigned int index;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int max_fail;
	unsigned int tmp = m_test_mode == 0 || (m_test_mode & 0x1) == 1 ? 0 : 1;
	unsigned int fail_device;
	int block;
	int scan;
	int pscan;
	bool set_block = true;
	unsigned int fail_block[8] = { 0 };
	unsigned int fails[8];
	unsigned int fails_tmp[8];
	unsigned char ios[8];
	unsigned char ios_tmp[8];

	m_use_mode |= 0x3;
	Write(0x3a, 0);
	pscan = Read(0x40cf);
	SET_BLOCK(0, pscan, block, scan);

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		Write(0x3a, 0);
		fail_device = getFailDevice(b, (int *) fails_tmp);
		fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);
		IO_DEFINE_FAIL_COUNTER(fails, fails_tmp);

		// 추가로 더 Read 가능하도록 LIMIT 변경
		max_fail = m_block_fail_limit * m_mcp * m_mcp;

		if (m_mcp == 1) {
			max_fail = max_fail * 2;
		}

		if ((fail_device & 0xff) == 0) {
			continue;
		}

		for (i = 0; i < max_fail; i++) {
			getFailData(b, 0xff, i, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);

			for (unsigned int j = 0; j < m_bib_io_count_per_1scan; j++) {
				unsigned int dut_index;

				if (si_to_index(scan, j, dut_index) < 0) {
					continue;
				}

				index = b * m_bib_x * m_bib_y + dut_index;

				if (m_test_mode == 0 && m_16io_mode) { // yet
					if (((fail_device >> j * io) & 1) &&
							!((m_init_fail[index] & 1)) &&
							(fails[j * io] < 63 ? fails[j * io] : max_fail * 4) >= i) {
						fail_block[j] = ios[j * io] | ios[j * io + 1] << 8;

						if (fail_block[j] < (unsigned) m_block_limit) {
							SET_BLOCK(fail_block[j] << m_block_shift, pscan, block, scan);

							if (set_block) {
								SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
								SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
							}

							if (m_fail_mode == INIT_FAIL) {
								SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
							}
						}
					}

					continue;
				}

				if (((fail_device >> j * io) & 1) &&
						!((m_init_fail[index] & 1))
						&& (fails[j * io + tmp] < 63 ? fails[j * io + tmp] : max_fail * 4) >= i) {
					if ((i & 1)) {
						fail_block[j] |= ios[j * io + tmp];

						if (fail_block[j] < (unsigned) m_block_limit) {
							if (m_test_mode == 0) {
								SET_BLOCK(fail_block[j] << m_block_shift, pscan, block, scan);
							}
							else if ((m_test_mode & 0x1) == 1) {
								SET_BLOCK(fail_block[j], pscan, block, scan);
							}
							else if ((m_test_mode & 0x1) == 0) {
								SET_BLOCK(fail_block[j], pscan, block, scan);
							}

							if (set_block) {
								SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
								SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
							}

							if (m_fail_mode == INIT_FAIL) {
								SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
							}
						}
					}
					else {
						fail_block[j] = (ios[j * io + tmp]) << 8;

						if (ios[j * io + tmp] == 0xff) {
							fails[j * io + tmp] = 0;
						}
					}
				}
			}
		}
	}

	Write(0x3f, 0);

	//	if (m_fail_index > 60000) {
	//		SEND_FAIL_DATA();
	//	}

	return 0;
}

int SWControl::app_id_read_ecc()
{
	unsigned int i;
	unsigned int io = m_bib_io_size >> 3;
	unsigned int add;
	unsigned int max_fail;
	unsigned int dut_index;
	unsigned int fail_device;
	int block;
	int scan;
	int ecc, j;
	int page_addr;
	int dut, iob;
	bool ecc_block;
	bool set_block = true;
	unsigned int fails[8];
	unsigned int fails_tmp[8];
	unsigned short addr[8];
	unsigned short addr_tmp[8];
	unsigned char ios[8];
	unsigned char ios_tmp[8];

	m_use_mode |= 0x3;

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		if (m_page_mask == 0) {
			for (unsigned int i = 0; i < m_block_shift - m_page_shift; i++) {
				m_page_mask |= (1 << i);
			}
		}

		if ((m_fm_page % m_ecc_page) != m_ecc_page - 1) {
			goto block_check;
		}

		Write(0x3a, 0);
		add = (Read(0x402f) | (Read(0x404f) << 16));

		if (m_page_mask == 0) {
			for (unsigned int i = 0; i < m_block_shift - m_page_shift; i++) {
				m_page_mask |= (1 << i);
			}
		}

		page_addr = (add >> m_page_shift) & m_page_mask;
		SET_BLOCK(add, Read(0x40cf), block, scan);
		fail_device = getFailDevice(b, (int *) fails_tmp);
		fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);
		IO_DEFINE_FAIL_COUNTER(fails, fails_tmp);
		ecc = (fail_device & 0xff) ^ (fail_device >> 8);

		for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
			if (si_to_index(scan, i, dut_index) < 0) {
				continue;
			}

			iob = i * io;
			dut = b * m_bib_x * m_bib_y + dut_index;

			if (!((m_init_fail[dut] & 1)) && ((fail_device >> iob) & 1) &&
					!((ecc >> iob) & 1) && !(m_ecc_block_fail[dut] & 0x80)) {
				m_ecc_block_fail[dut] |= 0x80;

				if (set_block) {
					SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
					SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
				}

				if (m_fail_mode == INIT_FAIL) {
					SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
				}
			}
		}

		fail_device = 0;

		for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
			if (si_to_index(scan, i, dut_index) < 0) {
				continue;
			}

			iob = i * io;
			dut = b * m_bib_x * m_bib_y + dut_index;

			if (!(m_init_fail[dut] & 1)) {
				if (((ecc >> iob) & 1)) {
					if (!(m_ecc_block_fail[dut] & 0x40)) {
						m_ecc_block_fail[dut] |= 0x40;
						m_ecc_fly_fail[m_ecc_fly_count++] =
							block | (i << 20) | (b << 23) | (scan << 26) |
							0x80000000;
					}

					fail_device |= 0x101 << iob;
				}
			}
		}

		if (m_bib_io_count_per_1scan == 4) {
			fail_device |= (fail_device << 1) & 0xff;
		}

		fail_device = IO_DEFINE_FAIL_DEVICE_REVERSE(fail_device);
		max_fail = 0;

		if ((fail_device & 0xff)) {
			for (i = 0; (unsigned int) i < m_bib_io_count_per_1scan; i++) {
				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				iob = i * io;

				if (!((m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) &&
						(max_fail < (unsigned int) fails[iob])) {
					max_fail = fails[iob];
				}
			}

			for (i = 0; i <= max_fail; i++) {
				getFailData(b, fail_device, i, addr_tmp, ios_tmp);
				IO_DEFINE_FAIL_DATA_ADDR(ios, ios_tmp, addr, addr_tmp);

				for (j = 0; (unsigned int) j < m_bib_io_count_per_1scan; j++) {
					if (si_to_index(scan, j, dut_index) < 0) {
						continue;
					}

					iob = j * io;
					dut = b * m_bib_x * m_bib_y + dut_index;

					if ((!(m_init_fail[dut] & 1)) &&
							(unsigned int) fails[iob] >= i &&
							((ecc >> iob) & 1)) {
						if ((m_ecc_block_fail[dut] & 0x3f) > m_ecc_block) {
							continue;
						}

						m_ecc_block_fail[dut]++;
						m_ecc_fly_fail[m_ecc_fly_count++] = (addr[iob] % m_page_div) |
							(page_addr + addr[iob] / m_page_div) << 16;
						m_ecc_fly_fail[m_ecc_fly_count++] = (ios[iob] << 8) |
							(ios[iob + 1] << 16) | j;
					}
				}
			}
		}

		Write( 0x3f , 0 );
block_check:
		if (m_fm_page == m_page_mask) {
			for (j = 0; j < m_ecc_fly_count; j++) {
				ecc_block = (bool) (m_ecc_fly_fail[j] & 0x80000000);

				if (ecc_block) {
					i = (m_ecc_fly_fail[j] >> 20) & 7;
				}
				else {
					i = m_ecc_fly_fail[j + 1] & 7;
				}

				if (si_to_index(scan, i, dut_index) < 0) {
					continue;
				}

				dut = b * m_bib_x * m_bib_y + dut_index;

				if (m_ecc_block_fail[dut] & 0x40) {
					if ((m_ecc_block_fail[dut] & 0x3f) > m_ecc_block) {
						m_ecc_block_fail[dut] = 0x80;
						//	m_block_fail[m_fail_index++] =
						//			block | (i << 20) | (b << 23) | (scan << 26);

						if (set_block) {
							SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
							SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
						}

						if (m_fail_mode == INIT_FAIL) {
							SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
						}
					}
					else {
						m_ecc_fail[m_ecc_index++] = m_ecc_fly_fail[j];

						if (!ecc_block) {
							m_ecc_fail[m_ecc_index++] = m_ecc_fly_fail[j + 1];
						}
					}
				}

				if (!ecc_block) {
					j++;
				}
			}

			memset(m_ecc_block_fail, 0, m_bib_x * m_bib_y * NSLOT);
			m_ecc_fly_count = 0;
			m_fm_page = 0;
			Write(0x3f, 0);
		}
		else {
			m_fm_page++;
		}
	}

	return 0;
}

int SWControl::app_id_read_io()
{
	unsigned int i;
	unsigned int index;
	unsigned int io = m_bib_io_size >> 3;
	int block;
	int scan;
	int fail_device;
	int add, ecc;
	int address;
	int _prev_Scan;
	bool block_mode = (Read(0x22) & 0x4) ? 1 : 0;
	bool set_block = true;
	unsigned int fails[8];
	unsigned int fails_tmp[8];

	m_use_mode |= 0x3;
	Write(0x3a, 0);
	add = (Read(0x402f) | (Read(0x404f) << 16));
	address = add >> m_page_shift;
	_prev_Scan = Read(0x40cf);

	SET_BLOCK(add, (_prev_Scan / (m_mcp * m_bib_io_count_per_1scan) * m_mcp + _prev_Scan % m_mcp), block, scan);
	sysWrite(0x3b, 0);
	i = (_prev_Scan / m_mcp) % m_bib_io_count_per_1scan;

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		if (m_board_index != -1 && (unsigned int) m_board_index != b) {
			continue;
		}

		unsigned int dut_index;

		if (si_to_index(scan, i, dut_index) < 0) {
			continue;
		}

		index = b * m_bib_x * m_bib_y + dut_index;

		if (block_mode) {
			fail_device = Read(0x3c | (b << 8)) & 0xf0f;
			fail_device |= (fail_device >> 4);
			fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);

			if ((!(m_init_fail[index] & 1)) && ((fail_device >> i * io) & 1)) {
				if (set_block) {
					SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
					SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
				}

				if (m_fail_mode == INIT_FAIL) {
					SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
				}
			}
		}
		else {
			fail_device = getFailDevice(b, (int *) fails_tmp);
			fail_device = IO_DEFINE_FAIL_DEVICE(fail_device);
			IO_DEFINE_FAIL_COUNTER(fails, fails_tmp);
			ecc = (fail_device & 0xff) ^ (fail_device >> 8);

			if (fails[i * io] && !((m_init_fail[index] & 1)) &&
					((fail_device >> i * io) & 1) && !((ecc >> i * io) & 1)) {
				if (set_block) {
					SET_STEP_BLOCK_FAIL(b, dut_index, block, true);
					SET_CUM_BLOCK_FAIL(b, dut_index, block, true);
				}

				if (m_fail_mode == INIT_FAIL) {
					SET_INIT_BLOCK_FAIL(b, dut_index, block, true);
				}
			}
		}
	}

	Write(0x3f, 0);
	return 0;
}

int SWControl::app_id_read_display()
{
	const unsigned int IO_SIZE = 8;
	unsigned int i;
	unsigned int rows = (Read(0x3a) >> 8);
	int block;
	int scan;
	int i_block;
	int page = 0;
	unsigned char ios[8];

	if (rows == 0) {
		rows = 0x100;
	}

	char test_data[IO_SIZE * rows + 24];

	memset(test_data, 0, IO_SIZE * rows + 24);
	i_block = (Read(0x402f) | (Read(0x404f) << 16));
	SET_BLOCK(i_block, Read(0x40cf), block, scan);

	if (!_test_mode) {
		int bits = m_block_shift - m_page_shift;
		int mask = 0;

		for (int i = 0; i < bits; i++) {
			mask |= (1 << i);
		}

		page = (i_block >> m_page_shift) & mask;
	}
	else {
		block = i_block;
	}

	*(int *) (test_data + 4) = scan;
	*(unsigned int *) (test_data + 8) = (unsigned int) block;
	*(int *) (test_data + 12) = page;
	*(int *) (test_data + 16) = rows;
	*(int *) (test_data + 20) = m_display_count;
	m_display_count += rows;

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		*(int *) test_data = b;

		for (int s = 0; s < (int) rows; s++) {
			getFailData(b, 0xff, s, 0, ios);

			for (i = 0;i < IO_SIZE; i++) {
				test_data[24 + IO_SIZE * s + i] = ios[i];
			}
		}

		break;
	}

	Write(0x3f, 0);
	Send(BC_DISPDATA, test_data, IO_SIZE * rows + 24);
	return 0;
}

int SWControl::app_id_pretest_data()
{
	unsigned int i;
	int mask = (1llu << m_bib_io_size) - 1;
	int _value[4];
	bool fail;
	unsigned long long tmpdq;
	unsigned long long _tmpdq;

	Write(0x22, 4);

	if (m_scan == 0) {
		memset(m_dq_data, 0, m_bib_scan * sizeof(unsigned long long) * NSLOT);	// dq data

		if (m_init_count++ == 0) {
			memset(m_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
			memset(m_pre_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
		}
	}

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		_value[0] = Read(0x2a | (b << 8));
		_value[1] = Read(0x2b | (b << 8));
		_value[2] = Read(0x4a | (b << 8));
		_value[3] = Read(0x4b | (b << 8));

		for (i = 0, _tmpdq = 0; i < 4; i++) {
			tmpdq = _value[i];
			_tmpdq |= tmpdq << (i << 4);
		}

		if (m_dq_mode == true) {
			m_dq_data[b * m_bib_scan + m_scan] = _tmpdq;
		}

		tmpdq = IO_DEFINE_FAIL_LATCH(_tmpdq);

		for (i = 0, _tmpdq = tmpdq; i < m_bib_io_count_per_1scan; i++) {
			unsigned int dut_index;

			if (si_to_index(m_scan, i, dut_index) < 0) {
				continue;
			}

			fail = (int) (_tmpdq & mask) ? 1 : 0;
			_tmpdq >>= m_bib_io_size;
			m_init_fail[b * m_bib_x * m_bib_y + dut_index] |= fail;
			m_pre_init_fail[b * m_bib_x * m_bib_y + dut_index] = fail;
		}
	}

	m_scan++;
	return 0;
}

int SWControl::app_id_pretest_result()
{
	m_scan_set = m_scan;
	m_scan = 0;

	if (m_dq_mode == true) {
		Send(BC_DQDATA, m_dq_data, m_bib_scan * sizeof(unsigned long long) * NSLOT);
	}

	if( m_use_setmap ) {
		m_use_setmap = false;
		Send(BC_PREMAP_DATA, m_pre_init_fail, m_bib_x * m_bib_y * NSLOT);
	}

	Send(BC_INITDATA, m_init_fail, m_bib_x * m_bib_y * NSLOT);
	Send(BC_SCAN_SET, &m_scan_set, sizeof(unsigned int));
	return 0;
}

int SWControl::app_id_bitest_data()
{
	unsigned int i;
	int mask = (1llu << m_bib_io_size) - 1;
	int _value[4];
	bool fail;
	bool v;
	unsigned int dut_index;
	unsigned int data_index;
        unsigned int tmpfail;
        unsigned int _tmpfail;
	unsigned long long tmpdq;
	unsigned long long _tmpdq;

	m_use_mode |= 0x1;

	if (m_scan == 0 && m_now_step_count == 1) {
		if (m_test_data) {
			delete[] m_test_data;
			m_test_data = 0;
		}
		
		if(m_test_io_data)
                {
                    delete[] m_test_io_data;
                    m_test_io_data = 0;
                }
		
		m_test_data = new char[m_bib_x * m_bib_y * NSLOT];
                m_test_io_data = new unsigned int[m_bib_x * m_bib_y * NSLOT];
		memset(m_test_data, 0, m_bib_x * m_bib_y * NSLOT);
		memset(m_test_io_data, 0, sizeof(unsigned int) * m_bib_x * m_bib_y * NSLOT);
		memset(m_dq_data, 0, m_bib_scan * sizeof(unsigned long long) * NSLOT);	// dq data
	}

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		_value[0] = Read(0x2a | (b << 8));
		_value[1] = Read(0x2b | (b << 8));
		_value[2] = Read(0x4a | (b << 8));
		_value[3] = Read(0x4b | (b << 8));

		for (i = 0, _tmpdq = 0; i < 4; i++) {
			tmpdq = _value[i];
			_tmpdq |= tmpdq << (i << 4);
		}

		if (m_dq_mode == true) {
			m_dq_data[b * m_bib_scan + m_scan] = _tmpdq;
		}

		tmpdq = IO_DEFINE_FAIL_LATCH(_tmpdq);

		for (i = 0, _tmpdq = tmpdq; i < m_bib_io_count_per_1scan;) {
			if (si_to_index(m_scan, i, dut_index) < 0) {
				i++;
				continue;
			}

			fail = (int) (_tmpdq & mask) ? 1 : 0;
                        tmpfail = (unsigned int)_tmpdq & mask;
			_tmpdq >>= m_bib_io_size;
			data_index = b * m_bib_x * m_bib_y + dut_index;

			if (IS_PRE_FAIL(b, dut_index)) {
				i++;
				continue;
			}

			if (m_use_mask == true && IS_TEST_FAIL(b, dut_index)) {
				i++;
				continue;
			}

			if (fail) {
				if (m_step_merge_type == MULTI_OR) {
					m_test_data[data_index] &= true;
                                        m_test_io_data[data_index] &= tmpfail;
				}
				else {
					m_test_data[data_index] |= true;
                                        m_test_io_data[data_index] |= tmpfail;
				}
			}
			else {
				if (m_step_merge_type == MULTI_OR) {
					m_test_data[data_index] &= true;
                                        m_test_io_data[data_index] &= tmpfail;
				}
			}

			if (m_now_step_count < m_set_step_count) {
				i++;
				continue;
			}

			v = m_fail_mode == MUST_FAIL ? !(m_test_data[data_index] & 1) : m_test_data[data_index] & 1;
                        _tmpfail = m_fail_mode == MUST_FAIL ? ~(m_test_io_data[data_index] & mask) :  m_test_io_data[data_index] & mask;

			if (v) {
				SET_STEP_FAIL(b, dut_index, v);
				SET_CUM_FAIL(b, dut_index, v);

				if (m_use_mask && m_use_excl_mask) {
					SET_TEST_FAIL(b, dut_index, v);
				}
			}
			if(_tmpfail)
                        {
                            SET_STEP_IO_FAIL(b, dut_index, _tmpfail);
                        }

			i++;
		}
	}

	m_scan = Read(0x400e) > (unsigned int) m_scan ? m_scan + 1 : m_scan;	// 배열 index가 벗어나는 것 방지
	return 0;
}

int SWControl::app_id_bitest_result()
{
	m_scan = 0;
	m_test_dut_count = 0;

	if (m_now_step_count < m_set_step_count) {
		return 0;
	}

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (unsigned int x = 0; x < m_bib_x; x++) {
			for (unsigned int y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				if (!(IS_TEST_FAIL(b, x, y))) {
					m_test_dut_count++;
				}
			}
		}
	}

	if (m_dq_mode == true) {
		Send(BC_DQDATA, m_dq_data, m_bib_scan * sizeof(unsigned long long) * NSLOT);
	}

	m_fail_mode = NORMAL;

	if (m_test_data) {
		delete[] m_test_data;
		m_test_data = 0;
	}

	if (m_test_io_data) {
		delete[] m_test_io_data;
		m_test_io_data = 0;
	}
	
	return 0;
}

int SWControl::app_id_fm_read_data()
{
	unsigned int i, j, base;
	int i_block;
	int use_scan = Read(0x3a) >> 8;
	unsigned char ios[8];
	unsigned char ios_tmp[8];

	Write(0x3a, 0);
	memset(m_tmp_fm_data, 0, sizeof(m_tmp_fm_data));
	//	Write(0x3a, m_scan / m_fm_read_scan_count);
	base = Read(0x40cf);
	i_block = (Read(0x402f) | (Read(0x404f) << 16));
	//	SET_BLOCK(i_block, base, block, m_scan);

	for (i = 0; i < (unsigned int) use_scan; i++) {
		Write(0x3a, i);

		for (unsigned int b = 0; b < NSLOT; b++) {
			if (!_bi_find[b].read) {
				continue;
			}

			getFailData(b, 0xff, i, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);
			//		printf("i : %2d, %2d %2d %2d %2d %2d %2d %2d %2d", i,
			//				ios[0], ios[1], ios[2], ios[3], ios[4], ios[5], ios[6], ios[7]);

			for (j = 0; j < m_bib_io_count_per_1scan; j++) {
				m_tmp_fm_data[b][j][i] = ios[j];
			}
		}
	}

	Write(0x3f, 0);
	SET_FM_READ_DATA(base, i_block, m_fm_read_data_index);
	return 0;
}

int SWControl::app_id_dq_data()
{
	unsigned int i;
	int mask = (1llu << m_bib_io_size) - 1;
	int _value[4];
	unsigned int tindex = m_read_rdq_count + m_rdq_index;
	unsigned long long tmpdq, _tmpdq;

	tindex = tindex >= MAX_DQ_VALUE - 1 ? MAX_DQ_VALUE - 1 : tindex;
	m_dq_use = true;

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		_value[0] = Read(0x2a | (b << 8));
		_value[1] = Read(0x2b | (b << 8));
		_value[2] = Read(0x4a | (b << 8));
		_value[3] = Read(0x4b | (b << 8));

		for (i = 0, _tmpdq = 0; i < 4; i++) {
			_tmpdq |= (unsigned long long) _value[i] << (i << 4);
		}

		tmpdq = IO_DEFINE_FAIL_LATCH(_tmpdq);

		for (i = 0, _tmpdq = tmpdq; i < m_bib_io_count_per_1scan; i++) {
			unsigned int dut_index;
			unsigned int dq = (int) (_tmpdq & mask);

			if (si_to_index(m_scan, i, dut_index) < 0) {
				continue;
			}

			_tmpdq >>= m_bib_io_size;
			m_dq_value[b * m_bib_x * m_bib_y * MAX_DQ_VALUE + dut_index * MAX_DQ_VALUE + tindex] = dq;
		}

		if (m_dq_mode == true) {
			m_dq_data[b * m_bib_scan + m_scan] = tmpdq;
		}
	}

	Write(0x2a, 0);
	m_read_rdq_count++;

	if (m_read_rdq_count == m_set_rdq_count) {
		m_read_rdq_count = 0;
		m_scan = Read(0x400e) > (unsigned int) m_scan ? m_scan + 1 : m_scan;	// 배열 index가 벗어나는 것 방지
	}

	return 0;
}

int SWControl::app_id_dq_result()
{
	m_scan = 0;
	m_read_rdq_count = 0;
	m_set_rdq_count = 1;

	if (m_dq_mode == true) {
		Send(BC_DQDATA, m_dq_data, m_bib_scan * sizeof(unsigned long long) * NSLOT);
	}

	INIT_READ_CHIPID();
	SET_DRAM_READ_CHIPID(m_fm_read_data_index);
	RESULT_CHIPID_DATA(1);
	return 0;
}

int SWControl::app_id_bist_io_latch()
{
	int i, j;
	unsigned int dut_index;
	unsigned char ios[8];
	unsigned char ios_tmp[8];
	char buf[8][256];
	//	char msg[1024];

	memset(buf, 0xff, sizeof(buf));

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read)
			continue;

		//	for (i = 0; i < 16; i++) {
		//		getFailData(b, 0xff, i, 0, ios);
		//		sprintf(msg, "s : %2d, i : %3d, %4x %4x %4x %4x %4x %4x %4x %4x",
		//				m_scan, i, ios[0], ios[1], ios[2], ios[3], ios[4], ios[5], ios[6], ios[7]);
		//		Send(BC_PRINTF, msg, strlen(msg));
		//	}

		for (i = 0; i < 256; i++) {
			getFailData(b, 0xff, i, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);

			for (j = 0; j < 8; j++) {
				if (si_to_index(m_scan, j, dut_index) < 0) {
					continue;
				}

				if (!(m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) {
					buf[j][i] = ios[j];
				}
			}
		}

		for (j = 0; j < 8; j++) {
			if (si_to_index(m_scan, j, dut_index) < 0) {
				continue;
			}

			if (!(m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1) &&
					MAX_BIST_SIZE >= 256 * ((unsigned int) m_count + 1)) {
				memcpy(m_bist[b][dut_index] + 2 + m_count * 256, buf[j], sizeof(buf[j]));
			}
		}
	}

	m_count++;
	return 0;
}

int SWControl::app_id_bist_data()
{
	int j;
	//	char msg[1024];
	//	sprintf(msg, "use_scan : 0x%x, m_scan : 0x%x, Read(0x400e) : 0x%x", use_scan, m_scan, Read(0x400e));
	//	Send(BC_PRINTF, msg, strlen(msg));

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (j = 0; j < 8; j++) {
			unsigned int dut_index;

			if (si_to_index(m_scan, j, dut_index) < 0) {
				continue;
			}

			if (!(m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) {
				m_bist[b][dut_index][0] = b;
				m_bist[b][dut_index][1] = dut_index;
			}
		}
	}

	m_scan = Read(0x400e) > (unsigned int) m_scan ? m_scan + 1 : m_scan;	// 배열 index가 벗어나는 것 방지
	m_count = 0;
	Write(0x3f, 0);
	return 0;
}

int SWControl::app_id_bist_result()
{
	unsigned int b, x, y;

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < m_bib_x; x++) {
			for (y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				do_bistdata_fmt_1(b, x, y);
			}
		}
	}

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

int SWControl::app_id_bist2_result()
{
	unsigned int b, x, y;

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < m_bib_x; x++) {
			for (y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				do_bistdata_fmt_2(b, x, y);
			}
		}
	}

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

int SWControl::app_id_emmc_io_latch()
{
	unsigned int i, j;
	unsigned int mul_index = m_bib_io_size;
	unsigned int mask_v = 0;
	unsigned int len = 256;
	unsigned char ios[8];
	unsigned char ios_tmp[8];
	char buf[64][256];

	if (m_device_mode != EMMC) {
		return 0;
	}

	for (i = 0; (unsigned int) i < mul_index; i++) {
		mask_v |= (1 << i);
	}

	memset(buf, 0xff, sizeof(buf));

	if (m_count == 0) {
		memset(m_emmc_offset, 0, m_bib_x * m_bib_y * NSLOT * sizeof(unsigned int));
	}

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (i = 0; i < 256; i++) {
			getFailData(b, 0xff, i, 0, ios_tmp);
			IO_DEFINE_FAIL_DATA(ios, ios_tmp);

			if (m_emmc_fm_display_count > 0) {
				char msg[1024];

				if (i < m_emmc_fm_display_count) {
					sprintf(msg, "s : %2d, i : %3d, %4x %4x %4x %4x %4x %4x %4x %4x",
							m_scan, i,
							ios_tmp[0], ios_tmp[1], ios_tmp[2], ios_tmp[3],
							ios_tmp[4], ios_tmp[5], ios_tmp[6], ios_tmp[7]);
					Send(BC_PRINTF, msg, strlen(msg));
				}
			}

			for (j = 0; j < m_bib_io_count_per_1scan; j++) {
				unsigned int dut_index;

				if (si_to_index(m_scan, j, dut_index) < 0) {
					continue;
				}

				if ((m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) {
					continue;
				}

				unsigned int v_index = j / (m_bib_io_count_per_1scan / m_use_io_count);
				unsigned int b_index = j % (m_bib_io_count_per_1scan / m_use_io_count);
				buf[j][i] = (ios[v_index] >> b_index * mul_index) & mask_v & m_emmc_bit_value;

				for (unsigned int k = 0; k < mul_index; k++) {
					if (((m_emmc_bit_value >> k) & 0x1) == 1) {
						break;
					}

					buf[j][i] >>= 1;
				}
			}
		}

		for (j = 0; j < m_bib_io_count_per_1scan; j++) {
			unsigned int dut_index;
			unsigned int offset;
			int ret = 0;

			if (si_to_index(m_scan, j, dut_index) < 0) {
				continue;
			}

			offset = m_emmc_offset[b * m_bib_x * m_bib_y + dut_index];

			if ((m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) {
				continue;
			}

			if (MAX_BIST_SIZE * 8 <= offset) {
				continue;
			}

			if (m_count == 0) {
				ret = do_emmcdata_comp_expect(buf[j]);
				ret = ret < 0 ? 0 : ret;
				len = 256 - ret;

				for (i = ret; i < 256; i++) {
					buf[j][i - ret] = buf[j][i];
				}
			}

			bool index_over = false;
			char *p = m_bist[b][dut_index] + 2;

			for (i = 0; i < len; i++) {
				unsigned int v_index = (i + offset) / 8;
				unsigned int b_index = (i + offset) % 8;
				unsigned int v = (buf[j][i] & mask_v) << (7 - b_index);

				if (v_index >= MAX_BIST_SIZE) {
					index_over = true;
					break;
				}

				p[v_index] = b_index == 0 ? v : p[v_index] | v;
			}

			if (index_over == false) {
				m_emmc_offset[b * m_bib_x * m_bib_y + dut_index] += len;
			}
		}
	}

	m_count++;
	return 0;
}

int SWControl::app_id_emmc_data()
{
	if (m_device_mode != EMMC) {
		return 0;
	}

	for (unsigned int b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (unsigned int j = 0; j < m_bib_io_count_per_1scan; j++) {
			unsigned int dut_index;

			if (si_to_index(m_scan, j, dut_index) < 0) {
				continue;
			}

			if (!(m_init_fail[b * m_bib_x * m_bib_y + dut_index] & 1)) {
				m_bist[b][dut_index][0] = b;
				m_bist[b][dut_index][1] = dut_index;
			}
		}
	}

	m_scan = Read(0x400e) > (unsigned int) m_scan ? m_scan + 1 : m_scan;	// 배열 index가 벗어나는 것 방지
	m_count = 0;
	Write(0x3f, 0);
	return 0;
}

int SWControl::app_id_emmc_init()
{
	unsigned int b, x, y;

	if (m_device_mode != EMMC) {
		return 0;
	}

	m_scan_set = m_scan + 1;
	m_scan = 0;

	if (m_init_count++ == 0) {
		memset(m_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
		memset(m_pre_init_fail, 2, m_bib_x * m_bib_y * NSLOT);
	}

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < m_bib_x; x++) {
			for (y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				do_emmcdata_fmt_0(b, x, y);
			}
		}
	}

	if( m_use_setmap ) {
		m_use_setmap = false;
		Send(BC_PREMAP_DATA, m_pre_init_fail, m_bib_x * m_bib_y * NSLOT);
	}

	Send(BC_INITDATA, m_init_fail, m_bib_x * m_bib_y * NSLOT);
	Send(BC_SCAN_SET, &m_scan_set, sizeof(unsigned int));

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

int SWControl::app_id_emmc_result()
{
	unsigned int b, x, y;

	if (m_device_mode != EMMC) {
		return 0;
	}

	m_scan = 0;
	m_use_mode |= 0x1;

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < m_bib_x; x++) {
			for (y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				do_emmcdata_fmt_1(b, x, y);
			}
		}
	}

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

int SWControl::app_id_emmc_block_result()
{
	unsigned int b, x, y;

	if (m_device_mode != EMMC) {
		return 0;
	}

	m_scan = 0;
	m_use_mode |= 0x3;

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < m_bib_x; x++) {
			for (y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				do_emmcdata_fmt_2(b, x, y);
			}
		}
	}

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

int SWControl::app_id_emmc_chip_id()
{
	unsigned int b, x, y;

	if (m_device_mode != EMMC) {
		return false;
	}

	INIT_READ_CHIPID();

	for (b = 0; b < NSLOT; b++) {
		if (!_bi_find[b].read) {
			continue;
		}

		for (x = 0; x < m_bib_x; x++) {
			for (y = 0; y < m_bib_y; y++) {
				if (IS_PRE_FAIL(b, x, y)) {
					continue;
				}

				do_emmcdata_fmt_3(b, x, y);
			}
		}
	}

	RESULT_CHIPID_DATA(0);

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

int SWControl::app_id_emmc_read_data()
{
	unsigned int b, x;

	if (m_device_mode != EMMC) {
		return 0;
	}

	m_scan = 0;
	m_use_mode |= 0x3;
	SET_EMMC_READ_DATA(m_fm_read_data_index);

	for (b = 0; b < NSLOT; b++) {
		for (x = 0; x < MAX_NAND_CHIP; x++) {
			memset(m_bist[b][x], 0, sizeof(char) * (MAX_BIST_SIZE + 2));
		}
	}

	return 0;
}

bool SWControl::runtimeError(const char *code)
{
	Send(BC_RUNTIMEERROR, code, strlen(code) + 1);
	return 0; /*stop_test */
}

int SWControl::DoPgrunWork(double wait, bool pause)
{
	unsigned long long start_time, end_time, fault_start_time;
	long mpat_start_time, mpat_end_time;
	unsigned short flag = 0;
	unsigned int pause_count = 0;

	enum { PGF_START = 1, PGF_RUN = 2, PGF_PAUSE = 4, PGF_STOP = 8, PGF_BUSY = 16, PGF_BURST = 32, };

	unsigned long long delay = (unsigned long long) (wait / 0.000001);

	end_time = start_time = fault_start_time = Counter();
	mpat_start_time = mpat_end_time = ::time(0);

	do {
		flag = Read(0x4002);

		if (flag & PGF_PAUSE) {
			pause_count = Read(0x4004);

			if (pause) {
				break;
			}

			if (pause_count == ID_LATCH) {
				pause_count =( m_step_no == 0 || m_hottemp == true) ? ID_PRETEST_DATA : ID_BITEST_DATA;
			}

			if (BURST(m_cur_pattern, pause_count)) {
				Write(0x4002, 7);
			}
			else if (!_APP(pause_count)) {
				Write(0x4002, 7);
			}

			if (m_aging_end) {
				Write(0x4002, 0);
				m_aging_stime = m_aging_etime = 0;
				m_aging_use = m_aging_end = false;
				break;
			}
		}
		else if (flag & PGF_STOP) {
			if (m_fail_mode == AGING && end_time - start_time < delay) {
				Write(0x4002, 3);
				continue;
			}

			Write(0x4002, 0);
			
			if( m_shmoo_test == true )
				Send(BC_SHMOO_ENDINFO, 0, 0);

			break;
		}
		else if (!(flag & PGF_START)) {
			break;
		}

		end_time = Counter();
		mpat_end_time = ::time(0);

		if (m_mpat_limit_time && mpat_end_time - mpat_start_time > m_mpat_limit_time) {
			char tmp[100] = { 0 };
			snprintf(tmp, sizeof(tmp), "PATTERN TIME LIMIT OVER [LIMIT TIME : %ld SEC]", m_mpat_limit_time);
			Send(BC_MPAT_TIME_LIMIT_OVER, tmp, strlen(tmp));
			::exit(0);
		}
		if(((end_time - fault_start_time) >= m_fault_timer) && m_isnew)
		{
			unsigned int iFault[NSLOT];
			unsigned int i = 0;

			write_i2cid(0x1f, false);
			readFault();

			for(i = 0; i < NSLOT; i++)
			{
				iFault[i] = 0;
				iFault[i] = m_fault[i];
				iFault[i] |= readFault(i);
				if (iFault[i] != 0)
				{
					_bi_find[i].read = 0;
					//printf("ifalut occured! slot : %d iFault : 0x%x", i, iFault[i]);
					Write(0x6 + i * 0x100, 0x1ffe);
					sysDelay(5 MS);
				}

			}

			Send(BC_DRV_FAULT, iFault, sizeof(iFault));
			fault_start_time = Counter();
		}

	} while (wait == 0 || end_time - start_time < delay);

	if (pause == false) {
		return flag;
	}
	else if (flag & PGF_PAUSE) {
		return pause_count;
	}

	return -1;
}

void SWControl::SET_USERALARM()
{
	Send(BC_USERALARM, 0, 0);
}

void SWControl::SET_USERSYSTEMDOWN()
{
	Send(BC_USERSYSTEMDOWN, 0, 0);
}

int SWControl::SET_USERPOPUP(char *str)
{
    unsigned int buf;
    unsigned int length;
    int ret;
    fd_set rfds;
    struct timeval tv;
    char *p;

    for (int i = 0 ; i < 1 ; i++) {
        Send(BC_USERWAITTEST, str, strlen(str));
        FD_ZERO(&rfds);
        FD_SET(m_sockfd, &rfds);
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        if ((ret = select(m_sockfd + 1, &rfds, NULL, NULL, &tv)) <= 0) {
            continue;
        }
        if (FD_ISSET(m_sockfd, &rfds)) {
            break;
        }
    }

    p = (char *) &buf;
    length = sizeof(buf);
    ret = Recv(p, length);
    return ret;
}

void SWControl::PAUSE()
{

	int ret;
	fd_set rfds;
	struct timeval tv;


	for (;;) {
		Send( BC_PAUSE, 0, 0 );
		FD_ZERO(&rfds);
		FD_SET(m_sockfd, &rfds);

		tv.tv_sec = 10;
		tv.tv_usec = 0;
		if ((ret = select(m_sockfd + 1, &rfds, NULL, NULL, &tv)) <= 0) {
			continue;
		}
		if (FD_ISSET(m_sockfd, &rfds)) {
			break;
		}
	}
	
	recv( 0, &ret, 4, 0 );

	if( ret == 0 )
	{
		printf( "PAUSE continue...[%d]\n",ret );
	}
	else
	{
		printf( "PAUSE END...[%d]\n", ret );

		printf( "Send BC_TESTEND...\n" );
	//	Send( BC_TESTEND, 0, 0 );

		_exit( 0 );
	}
}

void SWControl::SET_USERSENDMONITORING( const char *strMESSAGE, const char *strCOMMENT )
{
	TSENDMONITORING dataSENDMONITORING;
	memset( &dataSENDMONITORING, 0, sizeof( TSENDMONITORING ) );
	snprintf( dataSENDMONITORING.MESSAGE, 10, "%s", strMESSAGE );
	snprintf( dataSENDMONITORING.COMMENT, 250, "%s", strCOMMENT );

	Send(BC_SENDMONITORING, &dataSENDMONITORING, sizeof( TSENDMONITORING ));
}
