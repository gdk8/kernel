/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * author:
 *	Alpha Lin, alpha.lin@rock-chips.com
 *	Randy Li, randy.li@rock-chips.com
 *	Ding Wei, leo.ding@rock-chips.com
 *
 */
#ifndef __ROCKCHIP_MPP_COMMON_H__
#define __ROCKCHIP_MPP_COMMON_H__

#include <linux/cdev.h>
#include <linux/clk.h>
#include <linux/dma-buf.h>
#include <linux/kfifo.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/workqueue.h>
#include <linux/reset.h>
#include <linux/irqreturn.h>
#include <linux/poll.h>

#define MHZ			(1000 * 1000)

#define MPP_MAX_MSG_NUM			(16)
#define MPP_MAX_REG_TRANS_NUM		(60)
/* define flags for mpp_request */
#define MPP_FLAGS_MULTI_MSG		(0x00000001)
#define MPP_FLAGS_LAST_MSG		(0x00000002)
#define MPP_FLAGS_REG_FD_NO_TRANS	(0x00000004)
#define MPP_FLAGS_SCL_FD_NO_TRANS	(0x00000008)
#define MPP_FLAGS_SECURE_MODE		(0x00010000)

/**
 * Device type: classified by hardware feature
 */
enum MPP_DEVICE_TYPE {
	MPP_DEVICE_VDPU1	= 0, /* 0x00000001 */
	MPP_DEVICE_VDPU2	= 1, /* 0x00000002 */
	MPP_DEVICE_VDPU1_PP	= 2, /* 0x00000004 */
	MPP_DEVICE_VDPU2_PP     = 3, /* 0x00000008 */

	MPP_DEVICE_HEVC_DEC	= 8, /* 0x00000100 */
	MPP_DEVICE_RKVDEC	= 9, /* 0x00000200 */
	MPP_DEVICE_AVSPLUS_DEC	= 12, /* 0x00001000 */
	MPP_DEVICE_JPGDEC	= 13, /* 0x00002000 */

	MPP_DEVICE_RKVENC	= 16, /* 0x00010000 */
	MPP_DEVICE_VEPU1	= 17, /* 0x00020000 */
	MPP_DEVICE_VEPU2	= 18, /* 0x00040000 */
	MPP_DEVICE_VEPU22	= 24, /* 0x01000000 */

	MPP_DEVICE_IEP2		= 28, /* 0x10000000 */
	MPP_DEVICE_BUTT,
};

/**
 * Driver type: classified by driver
 */
enum MPP_DRIVER_TYPE {
	MPP_DRIVER_NULL = 0,
	MPP_DRIVER_VDPU1,
	MPP_DRIVER_VEPU1,
	MPP_DRIVER_VDPU2,
	MPP_DRIVER_VEPU2,
	MPP_DRIVER_VEPU22,
	MPP_DRIVER_RKVDEC,
	MPP_DRIVER_RKVENC,
	MPP_DRIVER_IEP,
	MPP_DRIVER_IEP2,
	MPP_DRIVER_JPGDEC,
	MPP_DRIVER_RKVDEC2,
	MPP_DRIVER_BUTT,
};

/**
 * Command type: keep the same as user space
 */
enum MPP_DEV_COMMAND_TYPE {
	MPP_CMD_QUERY_BASE		= 0,
	MPP_CMD_QUERY_HW_SUPPORT	= MPP_CMD_QUERY_BASE + 0,
	MPP_CMD_QUERY_HW_ID		= MPP_CMD_QUERY_BASE + 1,
	MPP_CMD_QUERY_CMD_SUPPORT	= MPP_CMD_QUERY_BASE + 2,
	MPP_CMD_QUERY_BUTT,

	MPP_CMD_INIT_BASE		= 0x100,
	MPP_CMD_INIT_CLIENT_TYPE	= MPP_CMD_INIT_BASE + 0,
	MPP_CMD_INIT_DRIVER_DATA	= MPP_CMD_INIT_BASE + 1,
	MPP_CMD_INIT_TRANS_TABLE	= MPP_CMD_INIT_BASE + 2,
	MPP_CMD_INIT_BUTT,

	MPP_CMD_SEND_BASE		= 0x200,
	MPP_CMD_SET_REG_WRITE		= MPP_CMD_SEND_BASE + 0,
	MPP_CMD_SET_REG_READ		= MPP_CMD_SEND_BASE + 1,
	MPP_CMD_SET_REG_ADDR_OFFSET	= MPP_CMD_SEND_BASE + 2,
	MPP_CMD_SEND_BUTT,

	MPP_CMD_POLL_BASE		= 0x300,
	MPP_CMD_POLL_HW_FINISH		= MPP_CMD_POLL_BASE + 0,
	MPP_CMD_POLL_BUTT,

	MPP_CMD_CONTROL_BASE		= 0x400,
	MPP_CMD_RESET_SESSION		= MPP_CMD_CONTROL_BASE + 0,
	MPP_CMD_TRANS_FD_TO_IOVA	= MPP_CMD_CONTROL_BASE + 1,
	MPP_CMD_RELEASE_FD		= MPP_CMD_CONTROL_BASE + 2,
	MPP_CMD_SEND_CODEC_INFO		= MPP_CMD_CONTROL_BASE + 3,
	MPP_CMD_CONTROL_BUTT,

	MPP_CMD_BUTT,
};

enum MPP_CLOCK_MODE {
	CLK_MODE_BASE		= 0,
	CLK_MODE_DEFAULT	= CLK_MODE_BASE,
	CLK_MODE_DEBUG,
	CLK_MODE_REDUCE,
	CLK_MODE_NORMAL,
	CLK_MODE_ADVANCED,
	CLK_MODE_BUTT,
};

enum MPP_RESET_TYPE {
	RST_TYPE_BASE		= 0,
	RST_TYPE_A		= RST_TYPE_BASE,
	RST_TYPE_H,
	RST_TYPE_NIU_A,
	RST_TYPE_NIU_H,
	RST_TYPE_CORE,
	RST_TYPE_CABAC,
	RST_TYPE_HEVC_CABAC,
	RST_TYPE_BUTT,
};

enum ENC_INFO_TYPE {
	ENC_INFO_BASE		= 0,
	ENC_INFO_WIDTH,
	ENC_INFO_HEIGHT,
	ENC_INFO_FORMAT,
	ENC_INFO_FPS_IN,
	ENC_INFO_FPS_OUT,
	ENC_INFO_RC_MODE,
	ENC_INFO_BITRATE,
	ENC_INFO_GOP_SIZE,
	ENC_INFO_FPS_CALC,
	ENC_INFO_PROFILE,

	ENC_INFO_BUTT,
};

enum ENC_INFO_FLAGS {
	ENC_INFO_FLAG_NULL		= 0,
	ENC_INFO_FLAG_NUMBER,
	ENC_INFO_FLAG_STRING,

	ENC_INFO_FLAG_BUTT,
};

/* data common struct for parse out */
struct mpp_request {
	__u32 cmd;
	__u32 flags;
	__u32 size;
	__u32 offset;
	void __user *data;
};

/* struct use to collect task set and poll message */
struct mpp_task_msgs {
	u32 flags;
	u32 req_cnt;
	struct mpp_request reqs[MPP_MAX_MSG_NUM];
	u32 set_cnt;
	u32 poll_cnt;
};

struct mpp_grf_info {
	u32 offset;
	u32 val;
	struct regmap *grf;
};

/**
 * struct for hardware info
 */
struct mpp_hw_info {
	/* register number */
	u32 reg_num;
	/* hardware id */
	int reg_id;
	u32 hw_id;
	/* start index of register */
	u32 reg_start;
	/* end index of register */
	u32 reg_end;
	/* register of enable hardware */
	int reg_en;
};

struct mpp_trans_info {
	const int count;
	const u16 * const table;
};

struct reg_offset_elem {
	u32 index;
	u32 offset;
};

struct reg_offset_info {
	u32 cnt;
	struct reg_offset_elem elem[MPP_MAX_REG_TRANS_NUM];
};

struct codec_info_elem {
	__u32 type;
	__u32 flag;
	__u64 data;
};

struct mpp_clk_info {
	struct clk *clk;

	/* debug rate, from debug */
	u32 debug_rate_hz;
	/* normal rate, from dtsi */
	u32 normal_rate_hz;
	/* high performance rate, from dtsi */
	u32 advanced_rate_hz;

	u32 default_rate_hz;
	u32 reduce_rate_hz;
	/* record last used rate */
	u32 used_rate_hz;
};

struct mpp_dev_var {
	enum MPP_DEVICE_TYPE device_type;

	/* info for each hardware */
	struct mpp_hw_info *hw_info;
	struct mpp_trans_info *trans_info;
	struct mpp_hw_ops *hw_ops;
	struct mpp_dev_ops *dev_ops;
};

struct mpp_mem_region {
	struct list_head reg_link;
	/* address for iommu */
	dma_addr_t iova;
	unsigned long len;
	u32 reg_idx;
	void *hdl;
};

struct mpp_dma_session;

struct mpp_taskqueue;

struct mpp_dev {
	struct device *dev;
	const struct mpp_dev_var *var;
	struct mpp_hw_ops *hw_ops;
	struct mpp_dev_ops *dev_ops;

	int irq;
	u32 irq_status;

	void __iomem *reg_base;
	struct mpp_grf_info *grf_info;
	struct mpp_iommu_info *iommu_info;

	atomic_t reset_request;
	atomic_t task_count;
	/* task for work queue */
	struct workqueue_struct *workq;
	/* current task in running */
	struct mpp_task *cur_task;
	/* set session max buffers */
	u32 session_max_buffers;
	struct mpp_taskqueue *queue;
	struct mpp_reset_group *reset_group;
	/* point to MPP Service */
	struct platform_device *pdev_srv;
	struct mpp_service *srv;
};

struct mpp_task;

struct mpp_session {
	enum MPP_DEVICE_TYPE device_type;
	/* the session related device private data */
	struct mpp_service *srv;
	struct mpp_dev *mpp;
	struct mpp_dma_session *dma;

	/* lock for session task register list */
	struct mutex reg_lock;
	/* lock for session task pending list */
	struct mutex pending_lock;
	/* task pending list in session */
	struct list_head pending_list;
	/* lock for session task done list */
	struct mutex done_lock;
	/* task done list in session */
	struct list_head done_list;

	/* event for session wait thread */
	wait_queue_head_t wait;
	pid_t pid;
	atomic_t task_count;
	atomic_t release_request;
	/* trans info set by user */
	int trans_count;
	u16 trans_table[MPP_MAX_REG_TRANS_NUM];
	/* link to mpp_service session_list */
	struct list_head session_link;
	/* private data */
	void *priv;
};

/* task state in work thread */
enum mpp_task_state {
	TASK_STATE_PENDING	= 0,
	TASK_STATE_RUNNING	= 1,
	TASK_STATE_START	= 2,
	TASK_STATE_HANDLE	= 3,
	TASK_STATE_IRQ		= 4,
	TASK_STATE_FINISH	= 5,
	TASK_STATE_TIMEOUT	= 6,
	TASK_STATE_DONE		= 7,
};

/* The context for the a task */
struct mpp_task {
	/* context belong to */
	struct mpp_session *session;

	/* link to pending list in session */
	struct list_head pending_link;
	/* link to done list in session */
	struct list_head done_link;
	/* link to list in taskqueue */
	struct list_head queue_link;
	/* The DMA buffer used in this task */
	struct list_head mem_region_list;

	/* state in the taskqueue */
	unsigned long state;
	atomic_t abort_request;
	/* delayed work for hardware timeout */
	struct delayed_work timeout_work;
	struct kref ref;

	/* record context running start time */
	struct timeval start;
	/* hardware info for current task */
	struct mpp_hw_info *hw_info;
	u32 *reg;
};

struct mpp_taskqueue {
	/* lock for trigger work */
	struct mutex work_lock;
	/* work for taskqueue */
	struct work_struct work;

	/* lock for pending list */
	struct mutex pending_lock;
	struct list_head pending_list;
	/* lock for running list */
	struct mutex running_lock;
	struct list_head running_list;

	/* point to MPP Service */
	struct mpp_service *srv;
	/* lock for mmu list */
	struct mutex mmu_lock;
	struct list_head mmu_list;
};

struct mpp_reset_group {
	/* the flag for whether use rw_sem */
	u32 rw_sem_on;
	struct rw_semaphore rw_sem;
	struct reset_control *resets[RST_TYPE_BUTT];
	/* for set rw_sem */
	struct mpp_taskqueue *queue;
};

struct mpp_service {
	struct class *cls;
	struct device *dev;
	dev_t dev_id;
	struct cdev mpp_cdev;
	struct device *child_dev;
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *procfs;
#endif
	unsigned long hw_support;
	atomic_t shutdown_request;
	/* follows for device probe */
	struct mpp_grf_info grf_infos[MPP_DRIVER_BUTT];
	struct platform_driver *sub_drivers[MPP_DRIVER_BUTT];
	/* follows for attach service */
	struct mpp_dev *sub_devices[MPP_DEVICE_BUTT];
	u32 taskqueue_cnt;
	struct mpp_taskqueue *task_queues[MPP_DEVICE_BUTT];
	u32 reset_group_cnt;
	struct mpp_reset_group *reset_groups[MPP_DEVICE_BUTT];
	/* lock for session list */
	struct mutex session_lock;
	struct list_head session_list;
};

/*
 * struct mpp_hw_ops - context specific operations for device
 * @init	Do something when hardware probe.
 * @exit	Do something when hardware remove.
 * @clk_on	Enable clocks.
 * @clk_off	Disable clocks.
 * @get_freq	Get special freq for setting.
 * @set_freq	Set freq to hardware.
 * @reduce_freq	Reduce freq when hardware is not running.
 * @reset	When error, reset hardware.
 */
struct mpp_hw_ops {
	int (*init)(struct mpp_dev *mpp);
	int (*exit)(struct mpp_dev *mpp);
	int (*clk_on)(struct mpp_dev *mpp);
	int (*clk_off)(struct mpp_dev *mpp);
	int (*get_freq)(struct mpp_dev *mpp,
			struct mpp_task *mpp_task);
	int (*set_freq)(struct mpp_dev *mpp,
			struct mpp_task *mpp_task);
	int (*reduce_freq)(struct mpp_dev *mpp);
	int (*reset)(struct mpp_dev *mpp);
	int (*set_grf)(struct mpp_dev *mpp);
};

/*
 * struct mpp_dev_ops - context specific operations for task
 * @alloc_task	Alloc and set task.
 * @prepare	Check HW status for determining run next task or not.
 * @run		Start a single {en,de}coding run. Set registers to hardware.
 * @irq		Deal with hardware interrupt top-half.
 * @isr		Deal with hardware interrupt bottom-half.
 * @finish	Read back processing results and additional data from hardware.
 * @result	Read status to userspace.
 * @free_task	Release the resource allocate which alloc.
 * @ioctl	Special cammand from userspace.
 * @open	Open a instance for hardware when set client.
 * @release	Specific instance release operation for hardware.
 * @free	Specific instance free operation for hardware.
 */
struct mpp_dev_ops {
	void *(*alloc_task)(struct mpp_session *session,
			    struct mpp_task_msgs *msgs);
	void *(*prepare)(struct mpp_dev *mpp, struct mpp_task *task);
	int (*run)(struct mpp_dev *mpp, struct mpp_task *task);
	int (*irq)(struct mpp_dev *mpp);
	int (*isr)(struct mpp_dev *mpp);
	int (*finish)(struct mpp_dev *mpp, struct mpp_task *task);
	int (*result)(struct mpp_dev *mpp, struct mpp_task *task,
		      struct mpp_task_msgs *msgs);
	int (*free_task)(struct mpp_session *session,
			 struct mpp_task *task);
	int (*ioctl)(struct mpp_session *session, struct mpp_request *req);
	int (*init_session)(struct mpp_session *session);
	int (*free_session)(struct mpp_session *session);
	int (*dump_session)(struct mpp_session *session, struct seq_file *seq);
};

int mpp_taskqueue_init(struct mpp_taskqueue *queue,
		       struct mpp_service *srv);

struct mpp_mem_region *
mpp_task_attach_fd(struct mpp_task *task, int fd);
int mpp_translate_reg_address(struct mpp_session *session,
			      struct mpp_task *task, int fmt,
			      u32 *reg, struct reg_offset_info *off_inf);

int mpp_check_req(struct mpp_request *req, int base,
		  int max_size, u32 off_s, u32 off_e);
int mpp_extract_reg_offset_info(struct reg_offset_info *off_inf,
				struct mpp_request *req);
int mpp_query_reg_offset_info(struct reg_offset_info *off_inf,
			      u32 index);
int mpp_translate_reg_offset_info(struct mpp_task *task,
				  struct reg_offset_info *off_inf,
				  u32 *reg);
int mpp_task_init(struct mpp_session *session,
		  struct mpp_task *task);
int mpp_task_finish(struct mpp_session *session,
		    struct mpp_task *task);
int mpp_task_finalize(struct mpp_session *session,
		      struct mpp_task *task);
int mpp_task_dump_mem_region(struct mpp_dev *mpp,
			     struct mpp_task *task);
int mpp_task_dump_reg(struct mpp_dev *mpp,
		      struct mpp_task *task);
int mpp_task_dump_hw_reg(struct mpp_dev *mpp,
			 struct mpp_task *task);

int mpp_dev_probe(struct mpp_dev *mpp,
		  struct platform_device *pdev);
int mpp_dev_remove(struct mpp_dev *mpp);

int mpp_power_on(struct mpp_dev *mpp);
int mpp_power_off(struct mpp_dev *mpp);
int mpp_dev_reset(struct mpp_dev *mpp);

irqreturn_t mpp_dev_irq(int irq, void *param);
irqreturn_t mpp_dev_isr_sched(int irq, void *param);

struct reset_control *mpp_reset_control_get(struct mpp_dev *mpp,
					    enum MPP_RESET_TYPE type,
					    const char *name);

u32 mpp_get_grf(struct mpp_grf_info *grf_info);
int mpp_set_grf(struct mpp_grf_info *grf_info);

int mpp_time_record(struct mpp_task *task);
int mpp_time_diff(struct mpp_task *task);

int mpp_write_req(struct mpp_dev *mpp, u32 *regs,
		  u32 start_idx, u32 end_idx, u32 en_idx);
int mpp_read_req(struct mpp_dev *mpp, u32 *regs,
		 u32 start_idx, u32 end_idx);

int mpp_get_clk_info(struct mpp_dev *mpp,
		     struct mpp_clk_info *clk_info,
		     const char *name);
int mpp_set_clk_info_rate_hz(struct mpp_clk_info *clk_info,
			     enum MPP_CLOCK_MODE mode,
			     unsigned long val);
unsigned long mpp_get_clk_info_rate_hz(struct mpp_clk_info *clk_info,
				       enum MPP_CLOCK_MODE mode);
int mpp_clk_set_rate(struct mpp_clk_info *clk_info,
		     enum MPP_CLOCK_MODE mode);

static inline int mpp_write(struct mpp_dev *mpp, u32 reg, u32 val)
{
	int idx = reg / sizeof(u32);

	mpp_debug(DEBUG_SET_REG,
		  "write reg[%03d]: %04x: 0x%08x\n", idx, reg, val);
	writel(val, mpp->reg_base + reg);

	return 0;
}

static inline int mpp_write_relaxed(struct mpp_dev *mpp, u32 reg, u32 val)
{
	int idx = reg / sizeof(u32);

	mpp_debug(DEBUG_SET_REG,
		  "write reg[%03d]: %04x: 0x%08x\n", idx, reg, val);
	writel_relaxed(val, mpp->reg_base + reg);

	return 0;
}

static inline u32 mpp_read(struct mpp_dev *mpp, u32 reg)
{
	u32 val = 0;
	int idx = reg / sizeof(u32);

	val = readl(mpp->reg_base + reg);
	mpp_debug(DEBUG_GET_REG,
		  "read reg[%03d]: %04x: 0x%08x\n", idx, reg, val);

	return val;
}

static inline u32 mpp_read_relaxed(struct mpp_dev *mpp, u32 reg)
{
	u32 val = 0;
	int idx = reg / sizeof(u32);

	val = readl_relaxed(mpp->reg_base + reg);
	mpp_debug(DEBUG_GET_REG,
		  "read reg[%03d] %04x: 0x%08x\n", idx, reg, val);

	return val;
}

static inline int mpp_safe_reset(struct reset_control *rst)
{
	if (rst)
		reset_control_assert(rst);

	return 0;
}

static inline int mpp_safe_unreset(struct reset_control *rst)
{
	if (rst)
		reset_control_deassert(rst);

	return 0;
}

static inline int mpp_clk_safe_enable(struct clk *clk)
{
	if (clk)
		clk_prepare_enable(clk);

	return 0;
}

static inline int mpp_clk_safe_disable(struct clk *clk)
{
	if (clk)
		clk_disable_unprepare(clk);

	return 0;
}

static inline int mpp_reset_down_read(struct mpp_reset_group *group)
{
	if (group && group->rw_sem_on)
		down_read(&group->rw_sem);

	return 0;
}

static inline int mpp_reset_up_read(struct mpp_reset_group *group)
{
	if (group && group->rw_sem_on)
		up_read(&group->rw_sem);

	return 0;
}

static inline int mpp_reset_down_write(struct mpp_reset_group *group)
{
	if (group && group->rw_sem_on)
		down_write(&group->rw_sem);

	return 0;
}

static inline int mpp_reset_up_write(struct mpp_reset_group *group)
{
	if (group && group->rw_sem_on)
		up_write(&group->rw_sem);

	return 0;
}

#ifdef CONFIG_PROC_FS
struct proc_dir_entry *
mpp_procfs_create_u32(const char *name, umode_t mode,
		      struct proc_dir_entry *parent, void *data);
#else
static inline struct proc_dir_entry *
mpp_procfs_create_u32(const char *name, umode_t mode,
		      struct proc_dir_entry *parent, void *data)
{
	return 0;
}
#endif

#ifdef CONFIG_PROC_FS
extern const char *mpp_device_name[MPP_DEVICE_BUTT];
extern const char *enc_info_item_name[ENC_INFO_BUTT];
#endif

/* workaround according hardware */
int px30_workaround_combo_init(struct mpp_dev *mpp);
int px30_workaround_combo_switch_grf(struct mpp_dev *mpp);

extern const struct file_operations rockchip_mpp_fops;

extern struct platform_driver rockchip_rkvdec_driver;
extern struct platform_driver rockchip_rkvenc_driver;
extern struct platform_driver rockchip_vdpu1_driver;
extern struct platform_driver rockchip_vepu1_driver;
extern struct platform_driver rockchip_vdpu2_driver;
extern struct platform_driver rockchip_vepu2_driver;
extern struct platform_driver rockchip_vepu22_driver;
extern struct platform_driver rockchip_iep2_driver;
extern struct platform_driver rockchip_jpgdec_driver;
extern struct platform_driver rockchip_rkvdec2_driver;

#endif
