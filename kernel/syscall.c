﻿#include <unios/syscall.h>
#include <type.h>
#include <global.h>
#include <assert.h>

#define SYSCALL_ENTRY(name) [NR_##name] = sys_##name

#define SYSCALL_ARGS1(t1) (t1) get_syscall_argument(0)
#define SYSCALL_ARGS2(t1, t2) \
 (t1) get_syscall_argument(0), (t2)get_syscall_argument(1)
#define SYSCALL_ARGS3(t1, t2, t3)                           \
 (t1) get_syscall_argument(0), (t2)get_syscall_argument(1), \
     (t3)get_syscall_argument(2)
#define SYSCALL_ARGS4(t1, t2, t3, t4)                       \
 (t1) get_syscall_argument(0), (t2)get_syscall_argument(1), \
     (t3)get_syscall_argument(2), (t4)get_syscall_argument(3)
#define SYSCALL_ARGS5(t1, t2, t3, t4, t5)                      \
 (t1) get_syscall_argument(0), (t2)get_syscall_argument(1),    \
     (t3)get_syscall_argument(2), (t4)get_syscall_argument(3), \
     (t5)get_syscall_argument(4)
#define SYSCALL_ARGS6(t1, t2, t3, t4, t5, t6)                  \
 (t1) get_syscall_argument(0), (t2)get_syscall_argument(1),    \
     (t3)get_syscall_argument(2), (t4)get_syscall_argument(3), \
     (t5)get_syscall_argument(4), (t6)get_syscall_argument(5)

typedef u32 (*syscall_t)();

static u32 get_syscall_argument(int index) {
    //! FIXME: syscall maybe from kernel space
    //! FIXME: p_proc_current may not from the caller proc?
    switch (index) {
        case 0: {
            return p_proc_current->task.regs.ebx;
        } break;
        case 1: {
            return p_proc_current->task.regs.ecx;
        } break;
        case 2: {
            return p_proc_current->task.regs.edx;
        } break;
        case 3: {
            return p_proc_current->task.regs.esi;
        } break;
        case 4: {
            return p_proc_current->task.regs.edi;
        } break;
        case 5: {
            return p_proc_current->task.regs.ebp;
        } break;
    }
    panic("syscall argument out of range");
}

static u32 sys_get_ticks() {
    return do_get_ticks();
}

static u32 sys_kmalloc() {
    return (u32)do_kmalloc(SYSCALL_ARGS1(int));
}

static u32 sys_kmalloc_4k() {
    return (u32)do_kmalloc_4k();
}

static u32 sys_malloc() {
    return (u32)do_malloc(SYSCALL_ARGS1(int));
}

static u32 sys_malloc_4k() {
    return (u32)do_malloc_4k();
}

static u32 sys_free() {
    return do_free(SYSCALL_ARGS1(void *));
}

static u32 sys_pthread() {
    return do_pthread(SYSCALL_ARGS1(void *));
}

static u32 sys_exec() {
    return do_exec(SYSCALL_ARGS1(char *));
}

static u32 sys_fork() {
    return do_fork();
}

static u32 sys_exit() {
    do_exit(SYSCALL_ARGS1(int));
    return 0;
}

static u32 sys_get_pid() {
    return do_get_pid();
}

static u32 sys_yield() {
    do_yield();
    return 0;
}

static u32 sys_sleep() {
    do_sleep(SYSCALL_ARGS1(int));
    return 0;
}

static u32 sys_wakeup() {
    do_wakeup(SYSCALL_ARGS1(void *));
    return 0;
}

static u32 sys_open() {
    return do_open(SYSCALL_ARGS2(const char *, int));
}

static u32 sys_close() {
    return do_close(SYSCALL_ARGS1(int));
}

static u32 sys_read() {
    return do_read(SYSCALL_ARGS3(int, char *, int));
}

static u32 sys_write() {
    return do_write(SYSCALL_ARGS3(int, const char *, int));
}

static u32 sys_lseek() {
    return do_lseek(SYSCALL_ARGS3(int, int, int));
}

static u32 sys_unlink() {
    return do_unlink(SYSCALL_ARGS1(const char *));
}

static u32 sys_create() {
    return do_create(SYSCALL_ARGS1(const char *));
}

static u32 sys_delete() {
    return do_delete(SYSCALL_ARGS1(const char *));
}

static u32 sys_opendir() {
    return do_opendir(SYSCALL_ARGS1(const char *));
}

static u32 sys_createdir() {
    return do_createdir(SYSCALL_ARGS1(const char *));
}

static u32 sys_deletedir() {
    return do_deletedir(SYSCALL_ARGS1(const char *));
}

syscall_t syscall_table[NR_SYSCALLS] = {
    SYSCALL_ENTRY(get_ticks), SYSCALL_ENTRY(get_pid),
    SYSCALL_ENTRY(kmalloc),   SYSCALL_ENTRY(kmalloc_4k),
    SYSCALL_ENTRY(malloc),    SYSCALL_ENTRY(malloc_4k),
    SYSCALL_ENTRY(free),      SYSCALL_ENTRY(fork),
    SYSCALL_ENTRY(pthread),   SYSCALL_ENTRY(exec),
    SYSCALL_ENTRY(yield),     SYSCALL_ENTRY(sleep),
    SYSCALL_ENTRY(wakeup),    SYSCALL_ENTRY(open),
    SYSCALL_ENTRY(close),     SYSCALL_ENTRY(read),
    SYSCALL_ENTRY(write),     SYSCALL_ENTRY(lseek),
    SYSCALL_ENTRY(unlink),    SYSCALL_ENTRY(create),
    SYSCALL_ENTRY(delete),    SYSCALL_ENTRY(opendir),
    SYSCALL_ENTRY(createdir), SYSCALL_ENTRY(deletedir),
    SYSCALL_ENTRY(exit),
};
