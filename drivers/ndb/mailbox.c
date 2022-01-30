#include "mailbox.h"
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <asm/fcntl.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/utsname.h>
#include <linux/uaccess.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>

#define NDM_BUF_SIZE (1024*1024)
char     g_file_path[256] = { 0 };
ndb_mailbox_t* g_ptr_mailbox = NULL;

static int remap_pfn_open(struct inode *inode, struct file *file)
{
	struct mm_struct *mm = current->mm;

	printk("client: %s (%d)\n", current->comm, current->pid);
	printk("code  section: [0x%lx   0x%lx]\n", mm->start_code, mm->end_code);
	printk("data  section: [0x%lx   0x%lx]\n", mm->start_data, mm->end_data);
	printk("brk   section: s: 0x%lx, c: 0x%lx\n", mm->start_brk, mm->brk);
	printk("mmap  section: s: 0x%lx\n", mm->mmap_base);
	printk("stack section: s: 0x%lx\n", mm->start_stack);
	printk("arg   section: [0x%lx   0x%lx]\n", mm->arg_start, mm->arg_end);
	printk("env   section: [0x%lx   0x%lx]\n", mm->env_start, mm->env_end);

	return 0;
}

static int remap_pfn_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long pfn_start = (virt_to_phys((void*)g_ptr_mailbox) >> PAGE_SHIFT) + vma->vm_pgoff;
	unsigned long virt_start = (unsigned long)g_ptr_mailbox + offset;
	unsigned long size = vma->vm_end - vma->vm_start;
	int ret = 0;

	printk("phy: 0x%lx, offset: 0x%lx, size: 0x%lx\n", pfn_start << PAGE_SHIFT, offset, size);

	ret = remap_pfn_range(vma, vma->vm_start, pfn_start, size, vma->vm_page_prot);
	if (ret)
		printk("%s: remap_pfn_range failed at [0x%lx  0x%lx]\n",
			__func__, vma->vm_start, vma->vm_end);
	else
		printk("%s: map 0x%lx to 0x%lx, size: 0x%lx\n", __func__, virt_start,
			vma->vm_start, size);

	return ret;
}
static const struct file_operations remap_pfn_fops = {
	.owner = THIS_MODULE,
	.open = remap_pfn_open,
	.mmap = remap_pfn_mmap,
};

static struct miscdevice remap_pfn_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ndbbox",
	.fops = &remap_pfn_fops,
};
int ndm_init(void)
{
    int ret = 0;

    g_ptr_mailbox = (ndb_mailbox_t*)kzalloc(NDM_BUF_SIZE, GFP_KERNEL | __GFP_ZERO);
    if (!g_ptr_mailbox)
    {
        ret = -ENOMEM;
		goto err;
    }
    printk("NBD mailbox is created at %px (&%px)\n", 
        g_ptr_mailbox, &g_ptr_mailbox);

    //register misc device
    ret = misc_register(&remap_pfn_misc);
	if (unlikely(ret)) 
    {
		pr_err("failed to register misc device!\n");
		goto err;
	}

    return 0;

err:
	return ret;
}

long    ndm_status()
{
    if (!g_ptr_mailbox)
    {
        printk("ptr_mailbox is null\n");
        return -1;
    }
    printk("Enter ndb_status_mailbox_%d...\n", g_ptr_mailbox->state_);
    if (g_ptr_mailbox->state_ == nm_idle)
    {
        printk("The current state is nm_idle\n");
        g_ptr_mailbox->state_ = nm_reply;
        return nm_idle;
    }
    if (g_ptr_mailbox->state_ == nm_request)
    {
        printk("The current state is nm_request\n");
        if (g_ptr_mailbox->task_type_ == nt_readfile)
        {
            printk("nt_readfile is path: %s\n", g_ptr_mailbox->file_para_.full_path);
            printk("kernel content: %s\n", (char*)((long)g_ptr_mailbox + sizeof(ndb_mailbox_t)));
        }
        g_ptr_mailbox->state_ = nm_reply;
        return nm_request;
    }
    printk("The current state is nm_reply\n");
    printk("kernel content: %s\n", (char*)((long)g_ptr_mailbox + sizeof(ndb_mailbox_t)));
    return nm_reply;
}

void    ndm_set(const char* file_name)
{
    if (!g_ptr_mailbox || !file_name)
    {
        printk("ptr_mailbox or file_name is null\n");
        return;
    }
    strcpy(g_ptr_mailbox->file_para_.full_path, file_name); 
    g_ptr_mailbox->task_type_ = nt_readfile;
    g_ptr_mailbox->state_ = nm_request;
    printk("file path is [%s]", g_ptr_mailbox->file_para_.full_path);
}

void ndm_exit()
{
    if (g_ptr_mailbox != NULL)
    {
        kfree(g_ptr_mailbox);
        g_ptr_mailbox = NULL;
    }
    misc_deregister(&remap_pfn_misc);
}
