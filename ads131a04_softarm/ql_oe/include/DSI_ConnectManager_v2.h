/**
  @file
  DSI_ConnectManager_v2.h

  @brief
  This file provides the definitions for adc, and declares the
  API functions.

*/
/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------
27/09/2019  Tinker.Sun     init
=============================================================================*/
#ifndef __DSI_CONNECTMANAGER_V2_H__
#define __DSI_CONNECTMANAGER_V2_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define QL_WAN_SUCCESS                      (0)
#define QL_WAN_ERROR                        (-1)


typedef enum {
    QL_DSI_IP_VERSION_4 = 4,
    QL_DSI_IP_VERSION_6 = 6,
    QL_DSI_IP_VERSION_4_6 = 10
} QL_DSI_IP_VERSION_T;



/**
 * DSI event define
 */
typedef enum {
    QL_DSI_EVT_INVALID = 0x0,              /**< Invalid event. */
    QL_DSI_EVT_NET_IS_CONN,                /**< Call is connected. */
    QL_DSI_EVT_NET_NO_NET,                 /**< Call is disconnected. */
    QL_DSI_EVT_PHYSLINK_DOWN_STATE,        /**< Physlink becomes dormant. */
    QL_DSI_EVT_PHYSLINK_UP_STATE,          /**< Physlink becomes active. */
    QL_DSI_EVT_NET_RECONFIGURED,           /**< Interface is reconfigured. */
    QL_DSI_EVT_QOS_STATUS_IND,             /**< A status for the associated QoS has changed. */
    QL_DSI_EVT_NET_NEWADDR,                /**< New address is generated. */
    QL_DSI_EVT_NET_DELADDR,                /**< An address for the interface has been deleted. */
    QL_DSI_EVT_NET_PARTIAL_CONN,           /**< Address is available for either IPv4
                                            or IPv6 only. @newpage */
    QL_DSI_NET_TMGI_ACTIVATED,
    QL_DSI_NET_TMGI_DEACTIVATED,
    QL_DSI_NET_TMGI_ACTIVATED_DEACTIVATED,
    QL_DSI_NET_TMGI_LIST_CHANGED,
    QL_DSI_NET_SAI_LIST_CHANGED,
    QL_DSI_NET_CONTENT_DESC_CONTROL,
    QL_DSI_NET_TMGI_SERVICE_INTERESTED,

    QL_DSI_EVT_NET_HANDOFF,
    QL_DSI_EVT_WDS_CONNECTED,
    QL_DSI_EVT_MAX
} QL_DSI_NET_EVT_T;



typedef enum {
    QL_PDP_TYPE_E_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    QL_PDP_TYPE_IPV4_V01 = 0,
    QL_PDP_TYPE_PPP_V01 = 1,
    QL_PDP_TYPE_IPV6_V01 = 2,
    QL_PDP_TYPE_IPV4V6_V01 = 3,
    QL_PDP_TYPE_INVALID_V01 = 0xff,
    QL_PDP_TYPE_E_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
} DATA_CALL_PDP_TYPE;
/**
 * DSI auth pref define
*/
typedef enum {
    QL_DSI_AUTH_PREF_NULL = 0,
    QL_DSI_AUTH_PREF_ONLY_PAP,
    QL_DSI_AUTH_PREF_ONLY_CHAP,
    QL_DSI_AUTH_PREF_BOTH_PAP_CHAP
} QL_DSI_AUTH_PREF_T;

typedef struct {
    uint32_t profile_index;
    QL_DSI_IP_VERSION_T ip_version;
    uint8_t  default_network;
    uint8_t  reconnect_enable;
    uint32_t reconnect_sec[5];
    uint32_t switch_cfun;
    uint8_t  apn_valid;
    struct {
        char apn_name[150];
        char username[127];
        char password[127];
        QL_DSI_AUTH_PREF_T auth;
        DATA_CALL_PDP_TYPE pdp_type;
    } apn;
} QL_NETCALL_INFO_T;

/**
 * callback function used in  ql_wan_start, when data call status changed,
 * Use callback functions for notifications, see enum QL_DSI_NET_EVT_T
*/
typedef void (*nw_status_cb)(int status);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief data call init function, must call first.
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_v2_init(QL_NETCALL_INFO_T *info);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief start data call, must call ql_wan_init first.
  @param[in] nw_cb this is a callback function, when data call status changed, Use callback
                   functions for notifications
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_v2_start(nw_status_cb nw_cb);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to destory wan environment.

  @return none
  */
/*-----------------------------------------------------------------------------------------------*/
void ql_wan_v2_deinit();


#ifdef __cplusplus
}
#endif

#endif
