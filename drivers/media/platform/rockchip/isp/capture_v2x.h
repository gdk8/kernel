/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2020 Rockchip Electronics Co., Ltd. */

#ifndef _RKISP_CAPTURE_V2X_H
#define _RKISP_CAPTURE_V2X_H

extern struct stream_config rkisp2_dmatx0_stream_config;
extern struct stream_config rkisp2_dmatx1_stream_config;
extern struct stream_config rkisp2_dmatx2_stream_config;
extern struct stream_config rkisp2_dmatx3_stream_config;

struct rkisp_dummy_buffer *hdr_dqbuf(struct list_head *q);
void hdr_qbuf(struct list_head *q, struct rkisp_dummy_buffer *buf);
int hdr_config_dmatx(struct rkisp_device *dev);
int hdr_update_dmatx_buf(struct rkisp_device *dev);
void hdr_stop_dmatx(struct rkisp_device *dev);
void hdr_destroy_buf(struct rkisp_device *dev);
void rkisp_config_dmatx_valid_buf(struct rkisp_device *dev);

int rkisp_register_stream_v20(struct rkisp_device *dev);
void rkisp_unregister_stream_v20(struct rkisp_device *dev);
void rkisp_mi_v20_isr(u32 mis_val, struct rkisp_device *dev);
void rkisp_mipi_v20_isr(u32 phy, u32 packet, u32 overflow, u32 state, struct rkisp_device *dev);

int rkisp_register_stream_v21(struct rkisp_device *dev);
void rkisp_unregister_stream_v21(struct rkisp_device *dev);
void rkisp_mi_v21_isr(u32 mis_val, struct rkisp_device *dev);
void rkisp_mipi_v21_isr(u32 phy, u32 packet, u32 overflow, u32 state, struct rkisp_device *dev);
#endif
