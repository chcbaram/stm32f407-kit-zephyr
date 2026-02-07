#ifndef ENET_H_
#define ENET_H_


#include "ap_def.h"


#include "dhcp/dhcp.h"


typedef struct 
{
  uint8_t mac[6];  ///< Source Mac Address
  uint8_t ip[4];   ///< Source IP Address
  uint8_t sn[4];   ///< Subnet Mask 
  uint8_t gw[4];   ///< Gateway IP Address
  uint8_t dns[4];  ///< DNS server IP Address
  bool    dhcp;    
} enet_info_t;


bool enetInit(void);
bool enetIsLink(void);
bool enetIsGetIP(void);
bool enetGetInfo(enet_info_t *p_info);

#endif