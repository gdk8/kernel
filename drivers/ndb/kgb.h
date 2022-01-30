/*
File Name: kgd.h
Descroption: The header file for KGB(Kernel Geography Block). 
    KGB is a memory page to present the target landmarks to
    the host side of Nano Debugger.
Author: GEDU Shanghai Lab(GSL)     
*/
#ifndef __KGB_H__
#define __KGB_H__
#ifndef _WIN32
#include <linux/init.h>
#include <linux/module.h>
#endif

#define  NM_MAX_FILE_PATH   260
typedef struct ndb_os_struct_sym_tag
{
    short vma_size_;
    short vma_file_; // file field in VMA struct
    short module_size_;
    short module_name_;
    short module_version_;
    short module_srcversion_;
    short module_syms_;
    short module_crcs_;
    short module_num_syms_;
    short module_init_;
    short module_core_;
    short module_list_;
    short module_core_size_;
    short module_init_size_;
    short module_core_text_size_;
    short module_init_text_size_;
    short module_args_;
    short module_exit_;
    short module_sect_attrs_;
    short module_attribute_size_;
    short attribute_group_size_;
    uint32_t pcpu_current_task_;
    
    short task_size; // size of task_struct
    short task_state; // offset of .state
    short task_stack; // offset of .stack
    short task_usage; // offset of .usage
    short task_flags; // offset of .flags
    short task_tasks; // offset of .tasks // the list struct
    short task_pid;  // offset of .pid
    short task_comm; // offset of .comm, actually it is the name of the process
    short task_mm; //struct mm_struct* mm;
    short mm_size; // sizeof(mm_struct) struct mm_struct* active_mm;
    short task_real_parent;
    short task_start_time;
    short task_fs;
    short task_files;
    short task_thread;
    short mm_pgd;
    short ns_name;
    short utsname_size;
    short file_path;
    short path_dentry;
    short dentry_d_name;
    short dentry_d_iname; // inlined short name of the dentry
    short task_active_mm;
    short mm_mmap;
    short dentry_size;
    short qstr_size;
    short qstr_name;
    short vma_vm_flags;
}ndb_os_struct_sym, * ptr_ndb_os_struct_sym;


typedef struct _ndb_syspara_t
{
    uint32_t                ndp_markera_; // marker to identify the data structure in memory space
    uint32_t                ndp_markerb_; // marker to identify the data structure in memory space
    uint64_t                kgb_base_;       // kernel geo block
    uint64_t                kernel_base_;
    uint64_t                module_list_head_;
    uint64_t                module_list_head_orig_; // the original head address of Linux
    uint64_t                process_list_head_;
    uint64_t                debugger_data_list_head_;
    uint64_t                printk_buffer_addr_; // base address of the printk buffer
    uint64_t                percpu_offset_;
    uint64_t                mailbox_base_;
    uint64_t                phys_base_;
    uint64_t                init_uts_ns_;
    uint64_t                vvar_page_; 
    uint64_t                reserved[28]; 
    uint32_t                printk_buffer_length_;
    uint32_t                kernel_length_;
    uint16_t                total_cpu_;
    uint16_t                machine_type_; // the IMAGE_FILE_MACHINE_AMD64 etc in WinNT.h
    ndb_os_struct_sym       kernelstructs_;
}ndb_syspara_t, * ptr_ndb_syspara_t;

//////////////////////////////////////////////////////////////////////////////////////////////////
void kgb_list_process(void);
int kgb_init(void);
int kgb_exit(void);
void kgb_list_module(void);
const char* kgb_message(void);
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __KGB_H__
