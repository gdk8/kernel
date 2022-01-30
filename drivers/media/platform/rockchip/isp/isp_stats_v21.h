/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2020 Rockchip Electronics Co., Ltd. */

#ifndef _RKISP_ISP_STATS_V21_H
#define _RKISP_ISP_STATS_V21_H

#include <linux/rkisp1-config.h>
#include <linux/interrupt.h>
#include <linux/kfifo.h>
#include "common.h"

//#define RKISP_RD_STATS_FROM_DDR
#define RKISP_RD_STATS_BUF_SIZE		0x35000

struct rkisp_isp_stats_vdev;
struct rkisp_stats_v21_ops {
	void (*get_rawawb_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawaf_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
			       struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawae0_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawhst0_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				 struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawae1_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawhst1_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				 struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawae2_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawhst2_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				 struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawae3_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_rawhst3_meas)(struct rkisp_isp_stats_vdev *stats_vdev,
				 struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_bls_stats)(struct rkisp_isp_stats_vdev *stats_vdev,
			      struct rkisp_isp21_stat_buffer *pbuf);
	void (*get_dhaz_stats)(struct rkisp_isp_stats_vdev *stats_vdev,
			       struct rkisp_isp21_stat_buffer *pbuf);
};

void rkisp_stats_first_ddr_config_v21(struct rkisp_isp_stats_vdev *stats_vdev);
void rkisp_init_stats_vdev_v21(struct rkisp_isp_stats_vdev *stats_vdev);
void rkisp_uninit_stats_vdev_v21(struct rkisp_isp_stats_vdev *stats_vdev);

#endif /* _RKISP_ISP_STATS_V21_H */