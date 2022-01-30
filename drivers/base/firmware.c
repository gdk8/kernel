// SPDX-License-Identifier: GPL-2.0
/*
 * firmware.c - firmware subsystem hoohaw.
 *
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 * Copyright (c) 2007 Greg Kroah-Hartman <gregkh@suse.de>
 * Copyright (c) 2007 Novell Inc.
 */
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

#include "base.h"

struct kobject *firmware_kobj;
EXPORT_SYMBOL_GPL(firmware_kobj);

//fox.luo@2015.02.08  memtest flag
struct kobject *memtest_kobj;
EXPORT_SYMBOL_GPL(memtest_kobj);
//liao@2016/8/16
struct kobject *kodi_kobj;
EXPORT_SYMBOL_GPL(kodi_kobj);

int __init firmware_init(void)
{
	firmware_kobj = kobject_create_and_add("firmware", NULL);
	if (!firmware_kobj)
		return -ENOMEM;
	return 0;
}

//fox.luo@2015.02.08  memtest flag
int __init memtest_init(void)
{	 
	memtest_kobj = kobject_create_and_add("skykirin", NULL);
	if (!memtest_kobj)
		return -ENOMEM;
	kodi_kobj = kobject_create_and_add("cyxtech", NULL);
	if (!kodi_kobj)
		return -ENOMEM;
	return 0;
}
