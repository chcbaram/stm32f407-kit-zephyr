#include "enet.h"

#include <zephyr/kernel.h>
#include <zephyr/linker/sections.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>


LOG_MODULE_REGISTER(enet, LOG_LEVEL_DBG);



static struct net_if *iface;
static bool is_init = false;

MODULE_DEF(enet) 
{
  .name = "enet",
  .priority = MODULE_PRI_LOW,
  .init = enetInit
};




bool enetInit(void)
{
  bool ret = true;

  logPrintf("[  ] enetInit()\n");

  iface = net_if_get_default();
  if (iface == NULL)
  {
    logPrintf("[  ] No default network interface found!\n");
    ret = false;
  }

  logPrintf("[  ] link is %s\n", net_if_is_up(iface) ? "Up":"Down");

  logPrintf("[%s] enetInit()\n", ret ? "OK":"E_");

  dhcpInit();

  is_init = ret;

	return true;
}

bool enetIsLink(void)
{
  if (iface == NULL)
    return false;

  return net_if_is_up(iface);
}

bool enetIsGetIP(void)
{
  return dhcpIsGetIP();
}

bool enetGetInfo(enet_info_t *p_info)
{
  dhcp_info_t dhcp_info;


  dhcpGetInfo(&dhcp_info);

  p_info->dhcp = dhcpIsGetIP();
  memcpy(p_info->ip, dhcp_info.ip, sizeof(dhcp_info.ip));
  memcpy(p_info->sn, dhcp_info.sn, sizeof(dhcp_info.sn));
  memcpy(p_info->gw, dhcp_info.gw, sizeof(dhcp_info.gw));

  return true;
}