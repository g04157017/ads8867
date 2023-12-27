/**  
  @file
  DSI_ConnectManager.h

  @brief
  This file provides the definitions for adc, and declares the 
  API functions.

*/
/*============================================================================
  Copyright (c) 2017 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------
27/04/2019  Quectel        init
13/05/2019  Baron.qian     Optimize part of macro definition, add interface definition
24/09/2019  Baron.qian     add set ip version function
05/12/2019  Baron.qian     add set profile index and pdp type interface, optimize
                           other interface
=============================================================================*/
#ifndef __DSI_CONNECTMANAGER_H__
#define __DSI_CONNECTMANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define QL_WAN_SUCCESS                      (0)
#define QL_WAN_ERROR                        (-1)

#define QL_DSI_IP_VERSION_4                    4
#define QL_DSI_IP_VERSION_6                    6
#define QL_DSI_IP_VERSION_4_6                  10




/**
 * DSI event define
 */ 
typedef enum
{
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



typedef enum
{
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
typedef enum
{
  QL_DSI_AUTH_PREF_NULL = 0,
  QL_DSI_AUTH_PREF_ONLY_PAP,
  QL_DSI_AUTH_PREF_ONLY_CHAP,
  QL_DSI_AUTH_PREF_BOTH_PAP_CHAP
} QL_DSI_AUTH_PREF_T;

/****
 * dsi packets stats define
 */ 
typedef struct
{
  unsigned long pkts_tx;           /**< Number of packets transmitted. */
  unsigned long pkts_rx;           /**< Number of packets received. */
  long long     bytes_tx;          /**< Number of bytes transmitted. */
  long long     bytes_rx;          /**< Number of bytes received. */
  unsigned long pkts_dropped_tx;   /**< Number of transmit packets dropped. */
  unsigned long pkts_dropped_rx;   /**< Number of receive packets dropped. */
} ql_dsi_data_pkt_stats;

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
int ql_wan_init();


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief data call auto connect set, include auto status and interval(seconds).
  @param[in] auto_status  When it is 0, the network cannot be automatically reconnected 
                                after disconnection; when it is 1, it can be automatically reconnected
  @param[in] interval_sec data call status check interval
                                the max value is QL_WAN_NETWORK_STATUS_MAX_TIMEOUT
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_set_autoconnect(int auto_status, int interval_sec);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief start data call, must call ql_wan_init first.
  @param[in] nw_cb this is a callback function, when data call status changed, Use callback 
                   functions for notifications
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_start(nw_status_cb nw_cb);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief data call stop function.
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_stop();


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Queries the packet data transfer statistics.
  @param
    stats: memory to hold the queried statistics details
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_get_pkt_stats(ql_dsi_data_pkt_stats *stats);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Resets the packet data transfer statistics

  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_reset_pkt_stats();
/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to set apn.
  @param[in] apn        apn name want to set
  @param[in] username   user name of apn
  @param[in] password   password of apn
  @param[in] auth       auth type of apn, see enum QL_DSI_AUTH_PREF_T
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_setapn(const char *apn, const char *userName, const char *password, QL_DSI_AUTH_PREF_T auth);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to get first apn infomation.
  @param[out] apn       apn name to return
  @param[in ] apnLen    apn name buffer size
  @param[out] userName  username of apn to return
  @param[in ] userLen   username buffer size
  @param[out] password  password to return
  @param[in ] pwdLen    password buffer size 
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_getapn(char *apn, int apnLen, char *userName, int userLen, char *password, int pwdLen);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to set ip version to dial.
  @param[in ] version    ip version which want to set
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_set_ip_version(int version);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to set profile index which used to dial.
  @param[in ] profile_idx    profile index
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_set_profile_index(int profile_idx);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to set pdp type. must use this interface before call ql_wan_set_apn to set pdp type.
  @param[in ] pdp_type    pdp type
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_wan_set_pdp_type(int pdp_type);
/*-----------------------------------------------------------------------------------------------*/
/**
  @brief function to destory wan environment.
  
  @return if success return QL_WAN_SUCCESS, else return QL_WAN_ERROR
  */
/*-----------------------------------------------------------------------------------------------*/
void ql_wan_deinit();



#ifdef __cplusplus
}
#endif

#endif
