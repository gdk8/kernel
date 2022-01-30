/*
File Name: main.c
Description: the driver interface for Nano Debug Beacon (NDB) module. 
    NDB is a module of Nano Debugger. Please visit http://nanocode.cn for 
    more information.
Author: GEDU Shanghai Lab(GSL) 
*/
#include "kgb.h"
#include "mailbox.h"
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/timer.h> 
#include <linux/mm.h> 
#include <linux/uaccess.h>
#include <linux/kgdb.h>

static struct proc_dir_entry* proc_ndb_entry = NULL;
extern ndb_mailbox_t* g_ptr_mailbox;
#define NDB_VER_MAJOR 1
#define NDB_VER_MINOR 6 // add arm64 support on 2022 Jan.

static int ndb_test_percpu(void)
{
    int i = 0;
    int total_cpus = num_online_cpus();
    printk("Num online CPUS %d\n", total_cpus);
    for (i = 0; i < total_cpus; i++)
    {
        printk("percpu offset %px for cpu %d\n", (void*)__per_cpu_offset[i], i);
    }
    //printk("current task %px\n", current_task); GDNDK 2022.01.12

    return 0;
}
static int ndb_test_current(void)
{
    struct task_struct * cur = get_current();
    printk("current %px\n", cur);
    printk("\tmm %px\n", cur->mm);
    printk("\tmm offset %ld\n", offsetof(struct task_struct, mm));

    return 0;
}
static ssize_t proc_ndb_read(struct file* filp, char __user * buf, size_t count, loff_t * offp)
{
    int ret = 0,n;
    const char * msg = kgb_message();
    printk(KERN_INFO "proc_ndb_read is called: file %p, buf %p count %zu offset %llx\n",
        filp, buf, count, *offp); 
    n = strlen(msg)+1;
    if (*offp < n)
    {
        ret = copy_to_user(buf, msg, n + 1); //GDNDK 2021.01.12
        *offp = n + 1;
        ret = n + 1;
    }
    else
        ret = 0;

    return ret;
}
static ssize_t proc_ndb_write(struct file* file, const char __user * buffer,
    size_t count, loff_t * data)
{
    char cmd[100] = { 0x00 };
    int i = 0;

    printk("proc_ndb_write called legnth 0x%lx, %p\n", count, buffer);
    if (count < 1)
    {
        printk("count <=1\n");
        return -EBADMSG; /* runt */
    }
    if (count > sizeof(cmd))
    {
        printk("count > sizeof(cmd)\n");
        return -EFBIG;   /* too long */
    }

    if (copy_from_user(cmd, buffer, count))
        return -EFAULT;
    if (strncmp(cmd, "lm", 2) == 0)
    {
        kgb_list_module();
    }
    if (strncmp(cmd, "ps", 2) == 0)
    {
        kgb_list_process();
    }
    else if (strncmp(cmd, "percpu", 6) == 0)
    {
        ndb_test_percpu();
    }
    else if(strncmp(cmd,"current",7) == 0)
    {
        ndb_test_current();
    }
    else if(strncmp(cmd,"break", 5) == 0)
    {
        printk("triggering break\n");
        arch_kgdb_breakpoint(); //GDNDK 2021.01.12
    }
    else
    {
        for (i = 0; i < strlen(cmd); i++)
        {
            if (cmd[i] == '\n')
            {
               cmd[i] = '\0';
               break;
            }
        }
        printk("cmd = [%s]", cmd);
        ndm_set(cmd);
    }

    return count;
}
static const struct file_operations proc_ndb_fops = {
 .owner = THIS_MODULE,
 .read = proc_ndb_read,
 .write = proc_ndb_write,
};

static int __init ndb_init(void)
{
    int ret = 0;

    printk("ndb %d.%d inits at %px\n", NDB_VER_MAJOR, NDB_VER_MINOR, ndb_init);

    /* Create /proc/ndb as interface for NdSrv and users */
    proc_ndb_entry = proc_create("ndb", 0, NULL, &proc_ndb_fops);
    if (proc_ndb_entry == NULL)
    {
        printk(KERN_ERR "create proc entry failed\n");
        return -1;
    }
    ret = ndm_init();
    ret = kgb_init();
   
    return 0;
}

static void __exit ndb_exit(void)
{
    kgb_exit();

    if (proc_ndb_entry)
    {
        proc_remove(proc_ndb_entry);
    }

    ndm_exit();

    printk("NDB exits from 0x%p... Bye, GEDU friends\n", ndb_exit);
}

module_init(ndb_init);
module_exit(ndb_exit);
MODULE_AUTHOR("GEDU Shanghai Lab(GSL)");
MODULE_DESCRIPTION("The Beacon LKM for Nano Debugger");
MODULE_LICENSE("GPL");
