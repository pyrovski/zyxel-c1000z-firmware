/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#if defined(CONFIG_MIPS_BRCM)
#include <linux/if_vlan.h>
#include <linux/timer.h>
#include <linux/igmp.h>
#include <linux/ip.h>
#include <linux/blog.h>
#include <linux/ktime.h>
#endif
#include "br_private.h"
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
#include "br_igmp.h"
#endif
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
#include "br_mld.h"
#endif

/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev, *brdev = br->dev;

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
	blog_link(IF_DEVICE, blog_ptr(skb), (void*)br->dev, DIR_RX, skb->len);
#endif

	brdev->stats.rx_packets++;
	brdev->stats.rx_bytes += skb->len;

	indev = skb->dev;
	skb->dev = brdev;

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		netif_receive_skb);
}

/* note: already called with rcu_read_lock (preempt_disabled) */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct sk_buff *skb2;
#if defined(CONFIG_MIPS_BRCM)
	struct iphdr *pip = ip_hdr(skb);
#endif

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

#if defined(CONFIG_MIPS_BRCM)
	if (pip->protocol == IPPROTO_IGMP)
   {
#if defined(CONFIG_BCM_GPON_MODULE)
		__u8 pos = (pip->ihl << 2);
		struct igmphdr *ih = (struct igmphdr *)&skb->data[pos];

		/* drop IGMP v1 packets */
		if ((ih->type == IGMP_HOST_MEMBERSHIP_QUERY) ||
		    (ih->type == IGMP_HOST_MEMBERSHIP_REPORT))
		{
			goto drop;
		}

		/* drop IGMP leave packets for group 0.0.0.0 */
		if ((ih->type == IGMP_HOST_LEAVE_MESSAGE) &&
          (0 == ih->group) )
		{
			goto drop;
		}
#endif
		/* rate limit IGMP */
		br = p->br;
		if ( br->igmp_rate_limit )
		{
			ktime_t      curTime;
			u64          diffUs;
			unsigned int usPerPacket;
			unsigned int temp32;
			unsigned int burstLimit;

			/* add tokens to the bucket - compute in microseconds */
			curTime     = ktime_get();
			usPerPacket = (1000000 / br->igmp_rate_limit);
			diffUs      = ktime_to_us(ktime_sub(curTime, br->igmp_rate_last_packet));
			diffUs     += br->igmp_rate_rem_time;

			/* allow 25% burst */
			burstLimit = br->igmp_rate_limit >> 2;
			if ( 0 == burstLimit)
			{
				burstLimit = 1;
			}

			if ( diffUs > 1000000 )
			{
				br->igmp_rate_bucket   = burstLimit;
				br->igmp_rate_rem_time = 0;
			}
			else
			{
				temp32 = (unsigned int)diffUs / usPerPacket;
				br->igmp_rate_bucket += temp32;
				if (temp32)
				{
					br->igmp_rate_rem_time = diffUs - (temp32 * usPerPacket);
				}
			}
			if (br->igmp_rate_bucket > burstLimit)
			{
				br->igmp_rate_bucket   = burstLimit;
				br->igmp_rate_rem_time = 0;
			}

			/* if bucket is empty drop the packet */
			if (0 == br->igmp_rate_bucket)
			{
				goto drop;
			}
			br->igmp_rate_bucket--;
			br->igmp_rate_last_packet.tv64 = curTime.tv64;
		}
	}
#endif

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;
	br_fdb_update(br, p, eth_hdr(skb)->h_source);

	if (p->state == BR_STATE_LEARNING)
		goto drop;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	if((BR_MLD_MULTICAST_MAC_PREFIX == dest[0]) && 
	   (BR_MLD_MULTICAST_MAC_PREFIX == dest[1])) {
		br->statistics.multicast++;
		skb2 = skb;
		if (br_mld_mc_forward(br, skb, 1, 0))
		{
			skb = NULL;
			skb2 = NULL;
		}
	} else 
#endif
	if (is_multicast_ether_addr(dest)) {
		br->dev->stats.multicast++;
		skb2 = skb;
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_IGMP_SNOOP)
		if (br_igmp_mc_forward(br, skb, 1, 0)) 
		{
			skb = NULL;
			skb2 = NULL;
		}
#endif
	} else 
	{
		dst = __br_fdb_get(br, dest);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
		blog_link(BRIDGEFDB, blog_ptr(skb),
					(void*)__br_fdb_get(br, eth_hdr(skb)->h_source),
					BLOG_PARAM1_SRCFDB, 0);
		blog_link(BRIDGEFDB, blog_ptr(skb), (void*)dst, BLOG_PARAM1_DSTFDB, 0);
#endif
		if ((dst != NULL) && dst->is_local) 
		{
			skb2 = skb;
			/* Do not forward the packet since it's local. */
			skb = NULL;
		}
	}

	if((skb != NULL) && (skb2 == skb))
	{
		skb2 = skb_clone(skb, GFP_ATOMIC);
	}

	if (skb2)  
		br_pass_frame_up(br, skb2);

	if (skb) {
		if (dst)
			br_forward(dst->dst, skb);
		else
			br_flood_forward(br, skb);
	}

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock (preempt_disabled) */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);

	if (p)
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source);
	return 0;	 /* process further */
}

/* Does address match the link local multicast address.
 * 01:80:c2:00:00:0X
 */
static inline int is_link_local(const unsigned char *dest)
{
	__be16 *a = (__be16 *)dest;
	static const __be16 *b = (const __be16 *)br_group_address;
	static const __be16 m = cpu_to_be16(0xfff0);

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
}

/*
 * Called via br_handle_frame_hook.
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock (preempt_disabled)
 */
struct sk_buff *br_handle_frame(struct net_bridge_port *p, struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	int (*rhook)(struct sk_buff *skb);

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return NULL;

	if (unlikely(is_link_local(dest))) {
		/* Pause frames shouldn't be passed up by driver anyway */
		if (skb->protocol == htons(ETH_P_PAUSE))
			goto drop;

		/* If STP is turned off, then forward */
		if (p->br->stp_enabled == BR_NO_STP && dest[5] == 0)
			goto forward;

		if (NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish))
			return NULL;	/* frame consumed by filter */
		else
			return skb;	/* continue processing */
	}

forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook != NULL) {
			if (rhook(skb))
				return skb;
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return NULL;
}
