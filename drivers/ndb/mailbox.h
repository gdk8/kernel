#ifndef __MAILBOX_FILE__
#define __MAILBOX_FILE__
#ifndef _WIN32
#include <linux/types.h>
#endif

#define  NM_MAX_FILE_PATH   260
// ndb mail interface
typedef enum _nm_task_type_t
{
    nt_readfile = 1,
}nm_task_type_t;
typedef struct _nm_file_para_t
{
    char full_path[NM_MAX_FILE_PATH];
}nm_file_para_t, * ptr_nm_file_para_t;
typedef struct _nm_file_result_t
{
    int         status_;// 0 stands for success, else error no of fopen
    int         reserved_;
    uint64_t    file_length_; // file length in bytes 
    // followed by file contents in binary
}nm_file_result_t;
typedef enum _nm_state_t
{
    nm_idle = 0, // nothing to do
    nm_request,
    nm_reply,
}nm_state_t;
typedef struct _ndb_mailbox_t
{
    nm_state_t          state_; // state of the mailbox
    int                 total_size_; // in bytes
    int                 total_tasks_; //
    nm_task_type_t      task_type_;// type of the first task
    nm_file_para_t      file_para_;
    nm_file_result_t    file_result;
    // para/result of the first task, size depends on type
    // the second task type
    // para/result of the second task 
    // ...
}ndb_mailbox_t;
typedef struct _ndb_mod_dep_t
{
    char*   file_buf;
    long    file_size;
}ndb_mod_dep_t;
#define  NDB_MAX_PATH  260
//////////////////////////////////////////////////////////////////////////////////////////////////
long            ndm_status(void);
void            ndm_set(const char* file_name);
void            ndm_exit(void);
int             ndm_init(void);
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif
