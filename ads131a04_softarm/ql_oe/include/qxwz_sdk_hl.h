/*
 * Copyright (c) 2018 Qianxun SI Inc. All rights reserved.
 *
 * Revision: 1.5
 */

#ifndef _QXWZ_SDK_H_
#define _QXWZ_SDK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * Definition for SDK status.
 */
typedef enum {
    QXWZ_SDK_STATUS_UNKNOWN = 0,
    QXWZ_SDK_STATUS_SYSTEM_FAILURE = 1001,
    QXWZ_SDK_STATUS_NETWORK_UNAVAILABLE = 1002,
    QXWZ_SDK_STATUS_NETWORK_NO_PERMISSION = 1003,
    QXWZ_SDK_STATUS_CONFIG_ERROR = 1004,
    QXWZ_SDK_STATUS_INVALID_PARAM = 1005,
    QXWZ_SDK_STATUS_UNKNOWN_APPKEY = 1006,
    QXWZ_SDK_STATUS_NO_ACCESS = 1007,
    QXWZ_SDK_STATUS_SERVER_ABORT = 1008,

    QXWZ_SDK_STATUS_INIT_SUCCESS = 2000,
    QXWZ_SDK_STATUS_INIT_FAILED = 2001,
    QXWZ_SDK_STATUS_START_SUCCESS = 2002,
    QXWZ_SDK_STATUS_START_FAILED = 2003,
    QXWZ_SDK_STATUS_STOP_SUCCESS = 2004,
    QXWZ_SDK_STATUS_STOP_FAILED = 2005,
    QXWZ_SDK_STATUS_CLEANUP_SUCCESS = 2006,
    QXWZ_SDK_STATUS_CLEANUP_FAILED = 2007,

    QXWZ_SDK_STATUS_GNSS_RUNTIME_ERROR = 3000,
    QXWZ_SDK_STATUS_GNSS_LOST_POSITION = 3001,
    QXWZ_SDK_STATUS_GNSS_HAVE_POSITION = 3002,

    QXWZ_SDK_STATUS_AUTH_PARAM_ERROR = 4000,
    QXWZ_SDK_STATUS_AUTH_ACCOUNT_NOT_EXIST = 4001,
    QXWZ_SDK_STATUS_AUTH_UNACTIVATED_ACCOUNT = 4002,
    QXWZ_SDK_STATUS_AUTH_NO_AVAILABLE_ACCOUNT = 4003,
    QXWZ_SDK_STATUS_AUTH_ACCOUNT_EXPIRED = 4004,
    QXWZ_SDK_STATUS_AUTH_ACCOUNT_TOEXPIRE = 4005,
    QXWZ_SDK_STATUS_AUTH_ACCOUNT_NEED_BIND = 4006,
    QXWZ_SDK_STATUS_AUTH_NO_VDR_PERMISSION = 4007,
    QXWZ_SDK_STATUS_AUTH_NO_PDR_PERMISSION = 4008,
    QXWZ_SDK_STATUS_AUTH_NO_AGNSS_PERMISSION = 4009,
    QXWZ_SDK_STATUS_AUTH_NO_RTK_PERMISSION = 4010,

    QXWZ_SDK_STATUS_AUTH_SUCCESS = 4011,
    QXWZ_SDK_STATUS_AUTH_NEED_MANUAL_ACTIVATION = 4012,
    QXWZ_SDK_STATUS_AUTH_NEED_TERMINAL_ACTIVATION = 4013,
    QXWZ_SDK_STATUS_AUTH_ACCOUNT_BEING_PROCESSED = 4014,
    QXWZ_SDK_STATUS_AUTH_ACCOUNT_FORBIDDEN = 4015,
    QXWZ_SDK_STATUS_AUTH_PARTIAL_CAPS_UNAVAILABLE = 4016,
    QXWZ_SDK_STATUS_AUTH_NO_CAPS_AVAILABLE = 4017,

    QXWZ_SDK_STATUS_OSS_CONNECT_SUCCESS = 5000,
    QXWZ_SDK_STATUS_OSS_CONNECT_FAIL = 5001,
    QXWZ_SDK_STATUS_OSS_CONNECT_TIMEOUT = 5002,
    QXWZ_SDK_STATUS_OSS_RECONNECTING = 5003,
    QXWZ_SDK_STATUS_OSS_DISCONNECT_SUCCESS = 5004,
    QXWZ_SDK_STATUS_OSS_DISCONNECT_FAIL = 5005,

    QXWZ_SDK_STATUS_NOSR_REQUEST_RTK_SUCCESS = 6000,
    QXWZ_SDK_STATUS_NOSR_REQUEST_RTK_FAIL = 6001,
    QXWZ_SDK_STATUS_NOSR_CLOSE_RTK_SUCCESS = 6002,
    QXWZ_SDK_STATUS_NOSR_CLOSE_RTK_FAIL = 6003,
    QXWZ_SDK_STATUS_NOSR_RTK_UNAVAILABLE = 6004,
    QXWZ_SDK_STATUS_NOSR_GGA_OUT_OF_SERVICE_AREA = 6005,
    QXWZ_SDK_STATUS_NOSR_INVALID_GGA = 6006,

    QXWX_SDK_STATUS_IAP_SUCCESS     = 7000,
    QXWX_SDK_STATUS_IAP_FAIL        = 7001,
    QXWX_SDK_STATUS_IAP_UPDATING     = 7002,
    QXWX_SDK_STATUS_IAP_TIMEOUT     = 7003

} QXWZSdkStatus;

/**
 * Definition for network status.
 */
typedef enum {
    /**
     * unknown network, the initial value.
     */
    QXWZ_NET_TYPE_UNKNOWN,
    /**
     * none network.
     */
    QXWZ_NET_TYPE_NONETWORK,
    /**
     * wifi network.
     */
    QXWZ_NET_TYPE_WIFI,
    /**
     * gsm network.
     */
    QXWZ_NET_TYPE_GSM
} QXWZNetworkStatus;

/**
 * Definition for position mode.
 */
typedef enum {
    /**
     * Positioning with RTK service.
     */
    QXWZ_POS_MODE_RTK = 1,
    /**
     * Positioning with RTK + DR services.
     */
    QXWZ_POS_MODE_RTK_DR = 2,
    /**
     * Positioning with CHIP service.
     */
    QXWZ_POS_MODE_CHIP = 3
} QXWZPosMode;

/**
 * Aiding data type(s) to delete
 */
typedef unsigned short                 QXGNSSAidingData;
#define QXWZ_GNSS_DEL_EPHEMERIS        0x0001
#define QXWZ_GNSS_DEL_ALMANAC          0x0002
#define QXWZ_GNSS_DEL_POSITION         0x0004
#define QXWZ_GNSS_DEL_TIME             0x0008
#define QXWZ_GNSS_DEL_IONO             0x0010
#define QXWZ_GNSS_DEL_UTC              0x0020
#define QXWZ_GNSS_DEL_HEALTH           0x0040
#define QXWZ_GNSS_DEL_SVDIR            0x0080
#define QXWZ_GNSS_DEL_SVSTEER          0x0100
#define QXWZ_GNSS_DEL_SADATA           0x0200
#define QXWZ_GNSS_DEL_RTI              0x0400
#define QXWZ_GNSS_DEL_CELLDB_INFO      0x8000
#define QXWZ_GNSS_DEL_ALL              0xFFFF


/* Application scenarios */
typedef unsigned char  QXGnssApplyScene;
#define QXWZ_GNSS_APPLY_SCENE_ANYWHERE     0   /* for anywhere */
#define QXWZ_GNSS_APPLY_SCENE_AUTOROOF     1   /* on the roof of auto */
#define QXWZ_GNSS_APPLY_SCENE_AUTOINSIDE   2   /* inside the auto */
#define QXWZ_GNSS_APPLY_SCENE_BIKE         3   /* share bike */
#define QXWZ_GNSS_APPLY_SCENE_CAMERA       4   /* monitor camera */
#define QXWZ_GNSS_APPLY_SCENE_WATCH        5   /* Watch */


/*Flags to indicate logmask.*/
typedef unsigned int QXWZLogMask;
/*LOG enable flag. */
/*Disable all kinds of logs*/
#define QXWZ_LOG_MASK_OFF  0
/*NOTE: This is Special, it means enable all kinds of logs*/
#define QXWZ_LOG_MASK_ON   1
/*Enable basic debug log*/
#define QXWZ_LOG_MASK_DEBUG 0x04
/*Enable Pedatain for rtk playback, it needs more space*/
#define QXWZ_LOG_MASK_ALGO 0x08
/*Enable NMEA Files for statistical accuracy*/
#define QXWZ_LOG_MASK_NMEA 0x10
/*Enable Raw Measurement Data*/
#define QXWZ_LOG_MASK_RAWX 0x20
/*Enable algo_nmea for DR playback, it needs more space*/
#define QXWZ_LOG_MASK_ALGO_DR 0x40

/* Authentication mode */
typedef unsigned char            QXWZAuthMode;
#define QXWZ_AUTH_MODE_APPKEY    0    /* authenticate by application key */
#define QXWZ_AUTH_MODE_DSK       1    /* authenticate by device service key */

/**
 * Configuration of SDK server info.
 */
typedef struct {
    size_t          size;
    char            auth_server[64];
    unsigned short  auth_port;
} QXWZServerConfig;

/**
 * Represent the SDK configuration.
 */
typedef struct {
    size_t  size;
    /**
     * Authentication mode.
     */
    QXWZAuthMode auth_mode;
    /**
     * Refer to authencation mode.
     * Application unique identity for QX_AUTH_MODE_APPKEY.
     * Device service key for QX_AUTH_MODE_DSK.
     */
    char    app_key[32];
    /**
     * Refer to authencation mode.
     * Secret for corresponding key.
     */
    char    app_secret[128];
    /**
     * Device id, UUID.
     */
    char    device_id[64];
    /**
     * Device type.
     */
    char    device_type[64];
    /**
     * SDK working directoy.
     */
    char    root_dir[256];

    /**
     * LOG enable flag, please refer to QXWZLogMask
     */
    QXWZLogMask    log_enable;
    
    /**
     * Separate log directoy if needed, otherwise logs also stored in the root_dir.
     */
    char    log_dir[256];

    /**
     * The dir to create AF_UNIX socket used by RTK, the dir must have all permissions(RWX).
     */
    char    socket_dir[256];

    /**
     * The path & filename of RTK's configurate file as QX supply for,
     *  since many of customers wanna to change the path.
     *  Notice, the path MUST contain the filename,etc: "/etc/testCfg.ini".
     */
    char    cfg_filename[256];

    /**
     *  see 'Application scenarios'.
     */
    QXGnssApplyScene  apply_scenario;

    /**
     * Configuration of SDK server info.
     */
    QXWZServerConfig  server_config;
} QXWZSdkConfig;


/* Fusion position flag */
typedef unsigned char                 QXGnssPosFlag;
#define POS_FUSION_FLAG_UNAVAILABLE         0
#define POS_FUSION_FLAG_GNSSONLY            1
#define POS_FUSION_FLAG_DGNSS               2
#define POS_FUSION_FLAG_RTK_FIX             4
#define POS_FUSION_FLAG_RTK_FLOAT           5
#define POS_FUSION_FLAG_FUSION              6
#define POS_FUSION_FLAG_DR                  7
#define POS_FUSION_FLAG_CHIP                8
#define POS_FUSION_FLAG_CHIP_FUSION         9
#define POS_FUSION_FLAG_BT                  11
#define POS_FUSION_FLAG_WIFI                12
#define POS_FUSION_FLAG_GNSSONLY_FUSION     13
#define POS_FUSION_FLAG_RTD_FUSION          14

typedef unsigned long long            QXGnssUtcTime;

/* Pos confidence flag*/
typedef unsigned int                  QXGnssConfidenceFlag;
#define POS_CONFIDENCE_FLAG_INVALID           0
#define POS_CONFIDENCE_FLAG_GNSSONLY          1
#define POS_CONFIDENCE_FLAG_DGNSS             2
#define POS_CONFIDENCE_FLAG_RTK_FIX           4
#define POS_CONFIDENCE_FLAG_RTK_FLOAT         5
#define POS_CONFIDENCE_FLAG_DR                60
#define POS_CONFIDENCE_FLAG_GNSS_FUSION       61
#define POS_CONFIDENCE_FLAG_RTK_FIX_FUSION    64
#define POS_CONFIDENCE_FLAG_RTK_FLOAT_FUSION  65

typedef struct {
    QXGnssConfidenceFlag  conf_type;
    float             conf_param_68[3];   /* 68% N/E/U confidence */
    float             conf_param_95[3];   /* 95% N/E/U confidence */
    float             conf_param_99[3];   /* 99.9% N/E/U confidence */
} QXWZGnssConfidenceParams;


typedef unsigned int QXWZReportFreq;
#define QXWZ_REPORT_FREQ_0_1_HZ   1
#define QXWZ_REPORT_FREQ_0_2_HZ   2
#define QXWZ_REPORT_FREQ_1_HZ     3
#define QXWZ_REPORT_FREQ_5_HZ     4
#define QXWZ_REPORT_FREQ_10_HZ    5

#define QXWZ_IOCTL_CMD_IMU_RATE         1
/**
 *indicate the output rate of  WZIMU info.
 */
typedef int QXWZIMURate;
#define QXWZ_IMU_RATE_0_HZ          0
#define QXWZ_IMU_RATE_1_HZ          1
#define QXWZ_IMU_RATE_2_HZ          2
#define QXWZ_IMU_RATE_5_HZ          5
#define QXWZ_IMU_RATE_10_HZ         10
#define QXWZ_IMU_RATE_25_HZ         25
#define QXWZ_IMU_RATE_50_HZ         50

/**
 *Set the extra device type
 */
#define QXWZ_IOCTL_CMD_SET_EXT_DEVTYPE   2

/**
 *  IOCTL TABLE
 *
 *  CMD                             DATA_TYPE           COMMENTS
 *  QXWZ_IOCTL_CMD_IMU_RATE         QXWZIMURate         configure the output rate(0/1/2/5/10/25/50 HZ) of WZIMU info.
 *  QXWZIMURate rate = QXWZ_IMU_RATE_1_HZ;
 *  int len = sizeof(QXWZIMURate);
 *  ioctl(QXWZ_IOCTL_CMD_IMU_RATE, (void *)&rate, (int *)&len));
 *
 */


/**
 * Represent the GNSS location.
 */
typedef struct {
    /**
     * Set to sizeof(QXWZGnssLocation)
     */
    size_t          size;
    /**
     * Position fusion flag
     */
    QXGnssPosFlag   posflag;
    /**
     * Represents latitude in degrees.
     */
    double          latitude;
    /**
     * Represents longitude in degrees.
     */
    double          longitude;
    /**
     * Represents altitude in meters above the WGS 84 reference ellipsoid.
     */
    double          altitude;
    /**
     * Represents speed in meters per second.
     */
    float           speed;
    /**
     * Represents heading in degrees.
     */
    float           bearing;
    /**
     * Represents expected accuracy in meters.
     */
    float           accuracy;
    /**
     * Timestamp for the location fix.
     */
    QXGnssUtcTime   timestamp;
    /**
     * Average CN0
     */
    float           avg_cno;
    /**
     * HDOP
     */
    float           hdop;
    /**
     * Number of satellites used for positioning.
     */
    unsigned char         sat_used;
    /**
     * Confidence coefficient.
     */
    QXWZGnssConfidenceParams confidence_param;
} QXWZGnssLocation;

#define QX_SENSOR_TYPE_LEN         64
/**
 * Represent DR configuration.
 */
typedef struct {
    /**
     * Set to sizeof(QXWZSensorConfig).
     */
    size_t  size;
    /**
     * Gyro type
     */
    char    gyro_type[QX_SENSOR_TYPE_LEN];
    /**
     * Accelerometer type
     */
    char    acce_type[QX_SENSOR_TYPE_LEN];
    /**
     * Magnetometer type
     */
    char    magn_type[QX_SENSOR_TYPE_LEN];
    /**
     * Barometer model
     */
    char    press_type[QX_SENSOR_TYPE_LEN];
    /**
     * Odometer type
     */
    char    odom_type[QX_SENSOR_TYPE_LEN];
    /**
     * Thermometer type
     */
    char    temp_type[QX_SENSOR_TYPE_LEN];
    /**
     * Four wheel speed type
     */
    char    ospd_type[QX_SENSOR_TYPE_LEN];
} QXWZSensorConfig;

typedef struct {
    unsigned long long       TimeStamp;
    unsigned short int       CheckSum;
    unsigned short int       MatchConfidence;
    unsigned short int       RoadID;
    unsigned short int       GridID;
    unsigned char            RoadType;         // 0:normal 1:channel  2:tunnel, 3:bridge 4:near IC
    unsigned char            RoadClass;
    signed short int         NorthDis;         //m
    signed short int         EastDis;          //m
    unsigned short int       LinkLength1;      //m
    unsigned short int       LinkLength2;      //m
    unsigned short int       LinkDirection;    // degree
    double                   MatchLat;         // degree
    double                   MatchLon;         // degree;
    unsigned char            MatchType;
    signed char              State;
    unsigned long long       Reserved;         // 0:unknown, 1:from server 2:from local 3:local ramp entry 4: local ramp exit
    float                    MatchDistance;    //m
    signed short int         Fence;            // 0:unsure,  2: ele road
    unsigned  int            ADCode;           //000000
    float                    RoadSpeedLimit;   //meters/s
    float                    RoadHeading;      // road heading, from -180~180
    unsigned short int       MapMatchedRamp;
    int                      NearMap;          // 0-NO, !0-YES
} QXWZMapMatchingPara;

typedef enum {
    QX_SENSOR_TYPE_ACCEL = 0X0002,
    QX_SENSOR_TYPE_MAGN  = 0X0004,
    QX_SENSOR_TYPE_PRESS = 0X0008,
    QX_SENSOR_TYPE_GYRO  = 0X0010,
    QX_SENSOR_TYPE_TEMP  = 0X0020,
    QX_SENSOR_TYPE_ODOM  = 0X0040,
    QX_SENSOR_TYPE_4SPD  = 0X0080
} QX_SENSOR_TYPE;

typedef struct {
    unsigned int             m_Mask;       /* valid sensor mask */
    unsigned long long       m_Time;       /* time */
    float                    m_ARate[3];   /* angular rate in radius per second */
    float                    m_SForce[3];  /* specific force in m/s2 */
    float                    m_Mag[3];     /* mag field in gauss */
    float                    m_Baro;       /* baro in Pa */
    float                    m_Temp;       /* temperature in celsius */
    float                    m_Speed;      /* auto speed in m/s */
    float                    m_4Spd[5];    /* steering and 4 speed of front-left, front-right, rear-left, rear-right respectively */
} QXWZSensorData;

typedef struct {
    int                 Valid;      // 0 means invalid; Non-zero means valid
    unsigned long long  TimeStamp;
} QXWZGNSSPPS;

typedef struct {
    int                 Valid;      // 0 means invalid; Non-zero means valid
    unsigned long long  TimeStamp;
    float               Orientation;
    float               BaseLen;
} QXWZGNSSOrt;

/**
 * Represent Lever-arm configuration.
 */
typedef struct {
    /**
     * Set to sizeof(QXWZLeverArmParams).
     */
    size_t   size;
    /**
     * Lever-arm of GNSS antenna and IMU
     */
    float    gnss2imu[3];
    /**
     * Lever-arm of IMU and the middle of vehicle's rear wheels
     */
    float    imu2rearmiddle[3];
    /**
     * misalignment angle of IMU and vehicle
     */
    float    imu2vehicle[3];
    /**
     * REAR Wheel Track
     */
    float    wheeltrack;
} QXWZLeverArmParams;

/**
 * Represent SDK callbacks.
 */
typedef struct {
    /**
     * Set to sizeof(QXWZSdkCallbacks).
     */
    size_t  size;

    /**
     * Callback utility for acquiring the system wakelock. It needs to be
     * passed in if the application layer does not manage the wakelock.
     * 1 for acquiring the system wakelock; 0 for releasing the system wakelock.
     */
    void    (*acquire_wakeLock)(int flag);
    /*
     * Notify sdk integrator to push the raw data cached in buf back to
     * QianXun rubik module for controlling.
     */
    void    (*fill_raw_data)(unsigned char *buf, int len);
    /*
     * Callback to report positioning results.
     */
    void    (*fill_position)(QXWZGnssLocation *pos);
    /*
     * Callback to report NMEA.
     */
    void    (*fill_nmea_info)(QXGnssUtcTime time, const char* nmea, int len);
    /*
     * Callback to report sdk status code.
     */
    void    (*status_response)(QXWZSdkStatus status);
} QXWZSdkCallbacks;

/**
 * Represent the SDK interface.
 */
typedef struct {
    /**
     * Set to sizeof(QXWZSdkInterface).
     */
    size_t       size;
    /**
     * Initialize the SDK, to provide sdk callbacks and configuration.
     */
    int          (*init)(QXWZSdkCallbacks *callbacks, QXWZSdkConfig *config);
    /**
     * Start the sdk.
     */
    int          (*start)(void);
    /**
     * Stop the sdk.
     */
    int          (*stop)(void);
    /**
     * Cleanup the sdk.
     */
    int          (*cleanup)(void);
    /**
     * Get sdk version.
     */
    char*        (*get_ver_info)(void);
    /**
     * Algo control.
     * Send cmd to or receive info from sdk. The corresponding action is based on the
     * first parameter ‘command’ to distinguish. params may be input or output params
     * depends on command, paramsLen may be input or out parameter, it is the length
     * of params, refer to the 'IOCTL TABLE' defined on the top for detailed CMD list.
     */
    int          (*ioctl)(int command, void* params, int* paramsLen);
    /**
     * Notify sdk when network status changes.
     */
    void         (*update_conn_status)(QXWZNetworkStatus networkType);
    /**
     * Configure positioning mode. QXWZPosMode defines the positioning mode, including rtk
     * and rtk + dr mode. frequency presents the time gap between position fix reports.
     * It will take effect immediately.
     */
    void         (*set_position_mode)(QXWZPosMode mode, QXWZReportFreq frequency);
    /**
     * Delete specific types of aiding data.
     */
    void         (*delete_aiding_data)(QXGNSSAidingData flags);
    /**
     * Inject gnss data which must include RXM-RAWX(Multi-GNSS Raw Measurement Data).
     */
    int          (*inject_gnss_data)(unsigned char *buffer, size_t len);
    /**
     * Inject pulse per second which is acquired at the time of GNSS positioned.
     */
    int    		 (*inject_gnss_pps)(QXWZGNSSPPS *pps);
    /**
     * Inject dual antenna GNSS orientation.
     */
    int   		 (*inject_gnss_ort)(QXWZGNSSOrt *ort);
    /**
     * Inject sensor configuration.
     */
    int   		 (*inject_sensor_config)(QXWZSensorConfig *config);
    /**
     * Inject senor data.
     */
    int          (*inject_sensor_data)(QXWZSensorData *sensorData);
    /**
     * Use third-party offline map matching. Insert external map match
     * to SDK instead of using the offline map inside the SDK.
     */
    int    		 (*inject_map_matching_para)(QXWZMapMatchingPara *mapParams);
    /**
     * Set lever arm configurations.
     */
    int   		 (*set_lever_arm_para)(QXWZLeverArmParams *lam);
} QXWZSdkInterface;



/**
 * Get the SDK interfaces.

 * @param void
 * @return a pointer of QXWZSdkInterface type that contains the SDK's interfaces
 */
const QXWZSdkInterface* getQXWZSdkInterface(void);




#ifdef __cplusplus
}
#endif
#endif /* _QXWZ_SDK_H_ */
