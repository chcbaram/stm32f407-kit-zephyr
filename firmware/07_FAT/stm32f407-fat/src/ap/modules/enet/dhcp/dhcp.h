#ifndef ENET_DHCP_H_
#define ENET_DHCP_H_


#include "ap_def.h"


typedef struct 
{
  uint8_t mac[6];  ///< Source Mac Address
  uint8_t ip[4];   ///< Source IP Address
  uint8_t sn[4];   ///< Subnet Mask 
  uint8_t gw[4];   ///< Gateway IP Address
  uint8_t dns[4];  ///< DNS server IP Address
  bool    dhcp;    
} dhcp_info_t;

bool dhcpInit(void);
bool dhcpIsGetIP(void);
bool dhcpGetInfo(dhcp_info_t *p_info);

#endif