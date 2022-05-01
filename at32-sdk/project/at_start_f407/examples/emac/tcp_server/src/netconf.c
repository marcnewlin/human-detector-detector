/**
  **************************************************************************
  * @file     netconf.c
  * @version  v2.0.8
  * @date     2022-04-02
  * @brief    network connection configuration
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "netconf.h"
#include "stdio.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define MAC_ADDR_LENGTH                  (6)
#define ADDR_LENGTH                      (4)
#define SYSTEMTICK_PERIOD_MS             10
/* Private variables ---------------------------------------------------------*/
extern volatile uint32_t local_time;
struct netif netif;
volatile uint32_t tcp_timer = 0;
volatile uint32_t arp_timer = 0;

static uint8_t mac_address[MAC_ADDR_LENGTH] = {0, 0, 0x44, 0x45, 0x56, 1};;
#if LWIP_DHCP
volatile uint32_t dhcp_fine_timer = 0;
volatile uint32_t dhcp_coarse_timer = 0;
#else
static uint8_t local_ip[ADDR_LENGTH]   = {192, 168, 81, 37};
static uint8_t local_gw[ADDR_LENGTH]   = {192, 168, 81, 187};
static uint8_t local_mask[ADDR_LENGTH] = {255, 255, 255, 0};
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  initializes the lwip stack
  * @param  none
  * @retval none
  */
void tcpip_stack_init(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

  /* Initializes the dynamic memory heap defined by MEM_SIZE.*/
  mem_init();

  /* Initializes the memory pools defined by MEMP_NUM_x.*/
  memp_init();


#if LWIP_DHCP  //need DHCP server
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;

#else
  IP4_ADDR(&ipaddr, local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
  IP4_ADDR(&netmask, local_mask[0], local_mask[1], local_mask[2], local_mask[3]);
  IP4_ADDR(&gw, local_gw[0], local_gw[1], local_gw[2], local_gw[3]);
#endif

  lwip_set_mac_address(mac_address);

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))

   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/

  if(netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &netif_input) == NULL)
  {
    while(1);
  }

  /*  Registers the default network interface.*/
  netif_set_default(&netif);

#if LWIP_DHCP
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.*/
  dhcp_start(&netif);
#endif

  /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&netif);
}

/**
  * @brief  called when a frame is received
  * @param  none
  * @retval none
  */
void lwip_pkt_handle(void)
{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
  if(ethernetif_input(&netif) != ERR_OK)
  {
    while(1);
  }
}

/**
  * @brief  updates the system local time
  * @param  none
  * @retval none
  */
void time_update(void)
{
  local_time += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  lwip periodic tasks
  * @param  localtime the current localtime value
  * @retval none
  */
void lwip_periodic_handle(volatile uint32_t localtime)
{

  /* TCP periodic process every 250 ms */
  if (localtime - tcp_timer >= TCP_TMR_INTERVAL)
  {
    tcp_timer =  localtime;
    tcp_tmr();
  }
  /* ARP periodic process every 5s */
  if (localtime - arp_timer >= ARP_TMR_INTERVAL)
  {
    arp_timer =  localtime;
    etharp_tmr();
  }

#if LWIP_DHCP
  /* Fine DHCP periodic process every 500ms */
  if (localtime - dhcp_fine_timer >= DHCP_FINE_TIMER_MSECS)
  {
    dhcp_fine_timer =  localtime;
    dhcp_fine_tmr();
  }
  /* DHCP Coarse periodic process every 60s */
  if (localtime - dhcp_coarse_timer >= DHCP_COARSE_TIMER_MSECS)
  {
    dhcp_coarse_timer =  localtime;
    dhcp_coarse_tmr();
  }
#endif
}
