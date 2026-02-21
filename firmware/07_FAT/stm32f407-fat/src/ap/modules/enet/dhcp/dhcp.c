#include "dhcp.h"

#include <zephyr/kernel.h>
#include <zephyr/linker/sections.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>


LOG_MODULE_REGISTER(enet_dhcp, LOG_LEVEL_DBG);

#define DHCP_OPTION_NTP (42)


static void dhcpHandler(struct net_mgmt_event_callback *cb, uint64_t mgmt_event, struct net_if *iface);
static void dhcpStartClient(struct net_if *iface, void *user_data);
static void dhcpOptionHandler(struct net_dhcpv4_option_callback *cb, size_t length, enum net_dhcpv4_msg_type msg_type, struct net_if *iface);


static struct net_mgmt_event_callback    mgmt_cb;
static struct net_mgmt_event_callback    mgmt_cb_link;
static struct net_dhcpv4_option_callback dhcp_cb;

static uint8_t        ntp_server[4];
static bool           is_get_ip = false;
static struct net_if *iface;
static dhcp_info_t    dhcp_info;




bool dhcpInit(void)
{
  iface = net_if_get_default();
  if (iface == NULL)
  {
    logPrintf("[  ] No default network interface found!\n");
    return false;
  }

  logPrintf("[  ] link is %s\n", net_if_is_up(iface) ? "Up":"Down");


  net_mgmt_init_event_callback(&mgmt_cb, dhcpHandler,
				     NET_EVENT_IPV4_ADDR_ADD);            
	net_mgmt_add_event_callback(&mgmt_cb);

	net_mgmt_init_event_callback(&mgmt_cb_link, dhcpHandler,
            NET_EVENT_IF_UP | NET_EVENT_IF_DOWN);
	net_mgmt_add_event_callback(&mgmt_cb_link);


	net_dhcpv4_init_option_callback(&dhcp_cb, dhcpOptionHandler,
					DHCP_OPTION_NTP, ntp_server,
					sizeof(ntp_server));

	net_dhcpv4_add_option_callback(&dhcp_cb);

	net_if_foreach(dhcpStartClient, NULL);

  logPrintf("[OK] dhcpInit()\n");
	return true;
}

bool dhcpIsGetIP(void)
{
  return is_get_ip;
}

bool dhcpGetInfo(dhcp_info_t *p_info)
{
  memcpy(p_info, &dhcp_info, sizeof(dhcp_info));
  return true;
}

void dhcpStartClient(struct net_if *iface, void *user_data)
{
	ARG_UNUSED(user_data);

	LOG_INF("Start on %s: index=%d", net_if_get_device(iface)->name,
		net_if_get_by_iface(iface));
	net_dhcpv4_start(iface);
}

void dhcpHandler(struct net_mgmt_event_callback *cb,
                 uint64_t                        mgmt_event,
                 struct net_if                  *iface)
{
  int i = 0;


  if (mgmt_event == NET_EVENT_IF_DOWN)
  {
    is_get_ip = false;
  }

  if (mgmt_event != NET_EVENT_IPV4_ADDR_ADD)
  {
    return;
  }

  for (i = 0; i < NET_IF_MAX_IPV4_ADDR; i++)
  {
    char buf[NET_IPV4_ADDR_LEN];

    if (iface->config.ip.ipv4->unicast[i].ipv4.addr_type !=
        NET_ADDR_DHCP)
    {
      continue;
    }

    memcpy(dhcp_info.ip, iface->config.ip.ipv4->unicast[i].ipv4.address.in_addr.s4_addr, 4);
    memcpy(dhcp_info.sn, iface->config.ip.ipv4->unicast[i].netmask.s4_addr, 4);
    memcpy(dhcp_info.gw, iface->config.ip.ipv4->gw.s4_addr, 4);

    LOG_INF("   Address[%d]: %s", net_if_get_by_iface(iface),
            net_addr_ntop(AF_INET,
                          &iface->config.ip.ipv4->unicast[i].ipv4.address.in_addr,
                          buf, sizeof(buf)));
    LOG_INF("    Subnet[%d]: %s", net_if_get_by_iface(iface),
            net_addr_ntop(AF_INET,
                          &iface->config.ip.ipv4->unicast[i].netmask,
                          buf, sizeof(buf)));
    LOG_INF("    Router[%d]: %s", net_if_get_by_iface(iface),
            net_addr_ntop(AF_INET,
                          &iface->config.ip.ipv4->gw,
                          buf, sizeof(buf)));
    LOG_INF("Lease time[%d]: %u seconds", net_if_get_by_iface(iface),
            iface->config.dhcpv4.lease_time);

    is_get_ip = true;
  }
}

void dhcpOptionHandler(struct net_dhcpv4_option_callback *cb,
                    size_t                             length,
                    enum net_dhcpv4_msg_type           msg_type,
                    struct net_if                     *iface)
{
  char buf[NET_IPV4_ADDR_LEN];

  LOG_INF("DHCP Option %d: %s", cb->option,
          net_addr_ntop(AF_INET, cb->data, buf, sizeof(buf)));
}
