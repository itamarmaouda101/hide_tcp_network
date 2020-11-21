#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>

struct nf_hook_ops hook_ops;
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
        if(src_port == 51941)
            return NF_STOLEN;
    }

    return NF_ACCEPT;
}