/**INC+***********************************************************************/
/* Header:    spm.h                                                          */
/*                                                                           */
/* Purpose:   Internal data strutures and constants for SPM                  */
/*                                                                           */
/* FiberHome                             2003-2012                           */
/*                                                                           */
/* $Revision: 1.107.4.3 $ $Date: 2012/01/29 09:17:52 $                       */
/*                                                                           */
/**INC-***********************************************************************/

#ifndef SPM_INCLUDED
#define SPM_INCLUDED


#undef OS_PRINTF

#define OS_PRINTF(...)  do { \
            if (0)\
            {\
                printf(__VA_ARGS__); \
            } \
        }while(0)
        

#define SPM_L2_ERROR_PRINT_LOG(message,...)do{\
		{\
			OS_PRINTF(__VA_ARGS__);\
			OS_SPRINTF(message,__VA_ARGS__);\
			BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR,message);\
		}\
	}while(0)

#define SPM_L2_INFO_PRINT_LOG(message,...)do{\
		{\
			OS_PRINTF(__VA_ARGS__);\
			OS_SPRINTF(message,__VA_ARGS__);\
			BMU_SLOG(BMU_CRIT, SPM_L2_LOG_DIR,message);\
		}\
	}while(0)






/*****************************************************************************/
/* GROUP SPM_MIB_TYPE                                                        */
/* spm send mib message to sbm.                                              */
/*****************************************************************************/
#define SPM_START_SBM                        (PCT_SBM | ((NBB_LONG)0x00000100))
#define SPM_STOP_SBM                         (PCT_SBM | ((NBB_LONG)0x00000200))
#define SPM_START_SCK                        (PCT_SBM | ((NBB_LONG)0x00000400))
#define SPM_STOP_SCK                         (PCT_SBM | ((NBB_LONG)0x00000800))

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP SPM_PROTO_RET_TYPE                                                  */
/*****************************************************************************/
#define SPM_PROT_RET_UNSUPPORT                    (1 << 5)
#define SPM_PROT_RET_ERROR                        (1 << 7)
/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/


/*****************************************************************************/
/* GROUP THREAD_ID                                                           */
/* external thread ID.                                          */
/*****************************************************************************/
#define SPM_INIT_HARDW_THREAD       0x1

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP FUNCTION_PARAMETER_INOUT                                            */
/* Function parameter in out gourp.                                          */
/*****************************************************************************/
/*****************************************************************************/
/* Parameter in(read only, input).                                           */
/*****************************************************************************/
#define IN

/*****************************************************************************/
/* Parameter out(writeable, output only)                                     */
/*****************************************************************************/
#define OUT

/*****************************************************************************/
/* Parameter in & out(writeable, input & output).                            */
/*****************************************************************************/
#define INOUT

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP SPM_TCP_DATA_PARAM_GROUP                                            */
/* socket parameter data gourp.                                              */
/*****************************************************************************/
#define SPM_TCP_PORT                                                    6000
#define SPM_TCP_RECV_BUF_SIZE                                    512 * 1024
#define SPM_TCP_SEND_BUF_SIZE                                    512 * 1024
#define SPM_TCP_DATA_POOL_SIZE                                           32
#define SPM_TCP_DATA_BUFFER_SIZE                                 512 * 1024
#define SPM_MTL_DATA_BUFFER_SIZE                                1024 * 1024

#define SPM_TCP_RP_HEAD_DATA_SIZE                                 24
#define SPM_TCP_RP_DATA_SIZE_OFFSET                               20
#define SPM_TCP_RQ_HEAD_DATA_SIZE                                 18
#define SPM_TCP_HEADE_FLAG_L3VN_SIZE                              4
#define SPM_TCP_PORT_SUBCARD1                                      30001
#define SPM_TCP_PORT_SUBCARD2                                      30002
#define SPM_TCP_HEAD_FLAG_CMD_TYPE                                 4
#define SPM_DATA_TO_NMU_HEAD_SIZE                                  28

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP SPM_THREAD_INSTRUCT_GROUP                                            */
/*****************************************************************************/
#define SPM_INSTRUCT_TYPE_BOOT                                 0X101200
#define SPM_INSTRUCT_TYPE_BFD                                  0X101201
#define SPM_INSTRUCT_TYPE_HWINITOK                             0X101202
#define SPM_INSTRUCT_TYPE_BOARDSTATE                           0X123457

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* 最大子卡个数，目前系统只支持2块子卡.                                      */
/*****************************************************************************/
#define MAX_SUB_CARD_NUM                                       2






#define MAX_PHYSIC_PORT_NUM                             128

/*HQOS最大LSP条目数*/
#define HQOS_MAX_LSP_ID 4096

/*HQOS最大pw条目数*/
#define HQOS_MAX_PW_ID   4096

#define MAX_ACL_CFG_NUM          6000


#define MAX_FLOW_CLASSIFY_PRI         5000
#define MAX_FLOW_CLASSIFY_RULE_ID         16000

#define MAX_CLASSIFY_PORT_ACL_ID_NUM  99
#define MAX_CLASSIFY_HIGH_ACL_ID_NUM  1000
#define MAX_CLASSIFY_ETH_ACL_ID_NUM  99


#define SIGNAL_NUM  46



#define SPM_SLOT_MAX_NUM              10    /* 整机业务盘槽位个数 */
#define SPM_PORT_MAX_NUM              40    /* 每个槽位面板口的最大个数 */

#define SPM_VC_NHI_ID_NUM             65536 //(1024 * 64)

//#define FOR_TIME_TEST

#define SPM_SUB_CONF_TYPE_SIZE              2
#define SPM_SUB_CONF_LENGTH_SIZE            2

#define SPM_L2_LOG_DIR     "log/L2.log"
#define SPM_L3_LOG_DIR     "log/L3.log"
#define SPM_OAM_LOG_DIR     "log/tpoam.log"
#define SPM_QOS_LOG_DIR     "log/Qos.log"

typedef NBB_LONG (*SPM_PROC_DATA_FUNC)(NBB_CONST NBB_BYTE *rq_buf, NBB_CONST NBB_ULONG len, NBB_BYTE *buf, NBB_BYTE *ucCharacter);

/*****************************************************************************/
/* 常用控制命令中子配置的类型值.                                             */
/*****************************************************************************/
#define SPM_CTRL_TYPE_LASER_SWITCH          1
#define SPM_CTRL_TYPE_LINE_OUT_RING         2
#define SPM_CTRL_TYPE_SYS_IN_RING           3
#define SPM_CTRL_TYPE_BOARD_RESET           4
#define SPM_CTRL_TYPE_SUBCARD_LINE_OUT_RING            5
#define SPM_CTRL_TYPE_SUBCARD_SYS_IN_RING              6
#define SPM_CTRL_TYPE__ALL_LINE_OUT_RING               7
#define SPM_CTRL_TYPE__ALL_LINE_SYS_IN_RING            8
#define SPM_CTRL_TYPE__BRANCH_LINE_SYS_IN_RING         9
#define SPM_CTRL_TYPE__FE1600_UNSHUTDOWN               18
#define SPM_CTRL_TYPE__CFG_CLEARALL                    19/* 配置清除 */
#define SPM_CTRL_TYPE__PWPING_SEND_ENABLE               31
#define SPM_CTRL_TYPE_E1_LINE                           32 
#define SPM_CTRL_TYPE_E1_SYS                            33 

/*****************************************************************************/
/* 单盘状态通信协议-状态通信类型T.                                           */
/*****************************************************************************/
#define SPM_STATUS_RQ_TYPE_SYSSTATE       102
#define SPM_STATUS_RQ_TYPE_ETH_OPTICALMODULE 103
#define SPM_STATUS_RQ_TYPE_CS1A8_GLOABALST  105
#define SPM_STATUS_RQ_TYPE_CS1A8_OVERHEAD   106
#define SPM_STATUS_RQ_TYPE_CS1A8_LINE_SIMULATION   107
#define SPM_STATUS_RQ_TYPE_CS1A8_FIFOST     108
#define SPM_STATUS_RQ_TYPE_CS1A8_CESCLOCK   109
#define SPM_STATUS_RQ_TYPE_CS1A8_J2STATUS   110

#define SPM_STATUS_RQ_TYPE_BFD              111
#define SPM_STATUS_RQ_TYPE_BOARDSTATE       112
#define SPM_STATUS_RQ_TYPE_VPLS_MAC_TABLE   113
#define SPM_STATUS_RQ_TYPE_VPLS_MAC_TABLEGATHER   124 /* vpls mac表汇总信息 add by gaos 2013.12.5*/
#define SPM_STATUS_RQ_TYPE_PROTECT_STATE_INFO        4
#define SPM_STATUS_RQ_TYPE_PHY_PORT         2
#define SPM_STATUS_RQ_TYPE_TPOAM            141

#define SPM_STATUS_RQ_TYPE_CLOCK_STATE                  120
#define SPM_STATUS_RQ_TYPE_CLOCKSOURCE_STATE            121
#define SPM_STATUS_RQ_TYPE_TIME_SYNCHRONOUS_PORTST      122
#define SPM_STATUS_RQ_TYPE_TIME_SYNCHRONOUS_GLOABALST   123
#define SPM_STATUS_RQ_TYPE_FANCONTROL                   125

#define SPM_STATUS_RQ_TYPE_BOARD_SHOW_CMD   200

/*****************************************************************************/
/* 控制字使能：0/1=不使能/使能。              */
/*****************************************************************************/
#define ENABLE  1
#define DISABLE 0

/*****************************************************************************/
/* C3芯片选择。                                                              */
/*****************************************************************************/
#define UNIT_0    0x0
#define UNIT_1    0x1

/*****************************************************************************/
/* 突发长度，暂赋值为640，即80kbyte                                        */
/*****************************************************************************/
#define KBITS_BURST     640

/***************************************************************************/
/* 指明主用备用，主/备=1/2.                                       */
/***************************************************************************/
#define VC_MAIN     1
#define VC_BACK     2

/***************************************************************************/
/* 条目增加/更新=0/1.                                       */
/***************************************************************************/
#define SPM_OPER_ADD    0
#define SPM_OPER_UPD    1
#define SPM_OPER_DEL    2

/***************************************************************************/
/* 条目增加/更新=0/1.                                       */
/***************************************************************************/
#define SPM_PRINT_CFG       	1
#define SPM_PRINT_DETAIL    	2
#define SPM_PRINT_ALL       	3

#define L2VPN_VPWS              1
#define L2VPN_VPLS              2
#define L2VPN_MSPW              4

/*****************************************************************************/
/*VPLS__MAC 状态查询113回调状态MAC_NUM的最大值  */
/*****************************************************************************/
#define VPLS_MAC_STA_MAX_NUM     6551

/*****************************************************************************/
/*VPLS__MAC 状态查询113回调状态MAC_NUM的C3一次上报的最大值  */
/*****************************************************************************/
#define VPLS_MAC_STA_C3_MAX_NUM     6551

/*****************************************************************************/
/*保护 状态查询4回调状态最大值(包含查询消息)  */
/*****************************************************************************/
#define PROT_STA_INFO_MAX_NUM     100

/***************************************************************************/
/* 直连物理端口速率 1=STM1                                      */
/***************************************************************************/

#define DERECT_PHY_PORT_RATE              1
#define SPM_VP_NNI              1
#define SPM_VP_UNI              2

//调试信息msg数组长度
//2015.1.10 200->500防止打印信息超长时越界
#define SPM_MSG_INFO_LEN    	500 

//组播Arad固定端口
#define SPM_MULTICAST_PORT_0		109
#define SPM_MULTICAST_PORT_1		110

//组播IP地址下发时，VE口计算port_id的偏移
#define SPM_MC_VE_PORT_OFFSET	0x800

/*****************************************************************************/
/* GROUP FUNCTION_RETURN_CODES                                               */
/* Function return codes.                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* RETURN SUCCESS.                                                           */
/*****************************************************************************/
#define SUCCESS                    0

/*****************************************************************************/
/* RETURN ERROR.                                                             */
/*****************************************************************************/
#define ERROR                      -1

/*****************************************************************************/
/* RETURN MEMORY ERROR.                                                      */
/*****************************************************************************/
#define ERROR_MEMORY              -2

/*****************************************************************************/
/* RETURN DATA ERROR.                                                        */
/*****************************************************************************/
#define ERROR_DATA                -3

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP BOOLEAN VALUES                                                      */
/* boolean values.                                                           */
/*****************************************************************************/
#define TRUE                       1
#define FALSE                      0

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP SPM_SJ_STATUS                                                       */
/* Slave-side join status.                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* Initial state on activation - no join.                                    */
/*****************************************************************************/
#define SPM_SJ_INIT                     0x00

/*****************************************************************************/
/* Activating (will become ACTIVE if join activation succeeds or INIT        */
/* otherwise).                                                               */
/*****************************************************************************/
#define SPM_SJ_ACTIVATING               0x01

/*****************************************************************************/
/* Active.                                                                   */
/*****************************************************************************/
#define SPM_SJ_ACTIVE                   0x02

/*****************************************************************************/
/* Deactivating (will become INIT when join deactivation completes).         */
/*****************************************************************************/
#define SPM_SJ_DEACTIVATING             0x03

/*****************************************************************************/
/* Failing over (will recover with replicated status).                       */
/*****************************************************************************/
#define SPM_SJ_FOVER                    0x04

/*****************************************************************************/
/* Failed (will recover without replicated status).                          */
/*****************************************************************************/
#define SPM_SJ_FAILED                   0x05

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* GROUP SPM_MJ_STATUS                                                       */
/* Master-side join status.                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Initial state on activation - no join.                                    */
/*****************************************************************************/
#define SPM_MJ_INIT                    0x00

/*****************************************************************************/
/* Activating (will become ACTIVE if join activation succeeds or INIT        */
/* otherwise).                                                               */
/*****************************************************************************/
#define SPM_MJ_ACTIVATING              0x01

/*****************************************************************************/
/* Active.                                                                   */
/*****************************************************************************/
#define SPM_MJ_ACTIVE                  0x02

/*****************************************************************************/
/* Deactivating (will become INIT when join deactivation completes).         */
/*****************************************************************************/
#define SPM_MJ_DEACTIVATING            0x03

/*****************************************************************************/
/* Failing over (will recover with replicated status).                       */
/*****************************************************************************/
#define SPM_MJ_FOVER                   0x04

/*****************************************************************************/
/* Failed (will recover without replicated status).                          */
/*****************************************************************************/
#define SPM_MJ_FAILED                  0x05

/*****************************************************************************/
/* Unavailable (will not recover).                                           */
/*****************************************************************************/
#define SPM_MJ_UNAVAIL                 0x06

/*****************************************************************************/
/* END GROUP                                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* SPM_PD INFO LIST                                                          */
/*****************************************************************************/
#define PHYSICAL_PORT_PD    11
#define LOGICAL_PORT_PD     12
#define LAG_PD              13
#define VC_PD               14
#define VPWS_PD             15
#define VPLS_PD             16
#define ARP_PD              17
#define NCACHE_PD           18
#define PORT_MIRROR_PD      19
#define GLOBAL_PD           20
#define VPLS_MC_PD          21
#define VPLS_UC_PD          22
#define SWITCH_VC_PD        23
#define MAC_RECYCLE_PD      24
#define BYPASS_PD           27
#define PUB_FUN_PD          29
#define ASSOCIATE_IF_PD     30
#define IP_NF_PD            33
#define MSP_PD              34
#define CMD_SWITCH_PD       42
#define VPLS_L3_MC_PD       43


/*****************************************************************************/
/* SPM_PD ERROR LIST     CONFIG_ID * 100 + SUB_ERROR_CODE = ERROR_CODE       */
/*****************************************************************************/
//PUBLIC
#define FUNC_IN_PARAM_IS_NULL       -101    //函数传入参数为NULL
#define CALL_FUNC_ERROR             -102    //调用C3驱动出错
#define CALL_C3_FUNC_ERROR          -103    //调用C3驱动出错
#define CALL_ARAD_FUNC_ERROR        -104    //调用Arad驱动出错
#define DEL_UNEXIST_CONFIG_ERROR    -105    //要删除的条目并不存在
#define BASIC_CONFIG_NOT_EXIST      -106  //基本或重要的配置并不存在
#define LOGICAL_PORT_NOT_EXIST      -107   //逻辑端口不存在
#define PHYSICAL_PORT_NOT_EXIST     -108   //逻辑端口不存在
#define VPN_NOT_VPWS_NOR_VPLS       -109
#define VC_NOT_MASTER_NOR_SLAVE     -110
#define VP_NOT_NNI_NOR_UNI          -111
#define CONFIG_NUM_IS_FULL          -112
#define VPWS_VP_NUM_IS_FULL         -113
#define CONFIG_NUM_IS_OUT_RANGE     -114
#define ALLOC_MEMORY_ERROR			-115
#define CONFIG_IS_IN_USE            -116

//PHYSICAL_PORT_PD
#define PHYSICAL_PORT_PD_PORT_UNEXIST   -131    //要删除的PORT_INDEX=%ld 的接口物理配置并不存在
#define UNABLE_GET_C3_PORT   -132    //子卡%d 面板口%d 获取芯片口失败!退出处理

//ARP_PD
#define ARP_PD_LPORT_UNEXIST   -133    //INDEX=%ld IP_ADDR=%s ARP配置所在的逻辑端口不存在

//VC_PD
#define VC_PD_NOT_MASTER_NOR_SLAVE  -134    //要修改VC配置既不是主也不是备

struct sysinfo s_info1;
struct sysinfo s_info2;

/**STRUCT+********************************************************************/
/* Structure: SPM_JOIN_CB                                                    */
/*                                                                           */
/* Description: Interface join control block.                                */
/*                                                                           */
/*            There is one control block for each SBI Interface              */
/*            join.  Each is held in its own list off SHARED data.           */
/*                                                                           */
/*****************************************************************************/

typedef struct spm_sj_join_cb
{
    /***************************************************************************/
    /* Entry in the list of joins.                                             */
    /***************************************************************************/
    NBB_LQE spm_sj_join_list_entry;

    /***************************************************************************/
    /* Join status.  Status SPM_SJ_INIT means that this control block is not   */
    /* in use.                                                                 */
    /***************************************************************************/
    NBB_BYTE spm_join_status;                  /* One of: SPM_SJ_STATUS;       */

    /***************************************************************************/
    /* HAF slave join CB (including part n er_data_pid, part n er_data_handle, */
    /* and other public fields).                                               */
    /***************************************************************************/
    AHL_SJ_CB spm_join_haf_sj_cb;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_sj_join_handle;

    /***************************************************************************/
    /* sbi接口注册完毕的标志。                                                 */
    /***************************************************************************/
    NBB_BOOL sbi_reg_over;

    /***************************************************************************/
    /* The following fields are received on the REGISTER message.              */
    /***************************************************************************/

    /***************************************************************************/
    /* Master process ID that the provider (this stub) should send interface   */
    /* signals to.                                                             */
    /***************************************************************************/
    NBB_PROC_ID spm_join_appl_pid;

    /***************************************************************************/
    /* Master queue ID that the provider (this stub) should send interface     */
    /* signals to.                                                             */
    /***************************************************************************/
    NBB_QUEUE_ID spm_join_appl_qid;

    /***************************************************************************/
    /* Master join handle that the provider (this stub) should include in      */
    /* interface signals sent to the master.                                   */
    /***************************************************************************/
    NBB_HANDLE spm_join_appl_handle;

    /***************************************************************************/
    /* 子卡槽位 slot 数据.                                                     */
    /***************************************************************************/
    NBB_ULONG slot;

} SPM_SJ_JOIN_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MJ_JOIN_CB                                                 */
/*                                                                           */
/* Description: Interface master join control block.                         */
/*                                                                           */
/*            There is one control block for dci interface join.             */
/*                                                                           */
/*****************************************************************************/

typedef struct spm_mj_join_cb
{
    /***************************************************************************/
    /* HAF master join control block.                                          */
    /***************************************************************************/
    AHL_MJ_CB spm_haf_cb;

    /***************************************************************************/
    /* Join status.  The status of SPM_MJ_INIT means that the control block is */
    /* not in use.                                                             */
    /***************************************************************************/
    NBB_BYTE spm_join_status;                        /* One of: SPM_MJ_STATUS; */

    /***************************************************************************/
    /* 是否和dci注册完毕的标志。                                               */
    /***************************************************************************/
    NBB_BOOL dci_reg_over;

} SPM_MJ_JOIN_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_SCK_JOIN_CB                                                */
/*                                                                           */
/* Description: SPM SOCKET master join control block.                        */
/*                                                                           */
/*            There is one control block for SCK interface join.             */
/*            Note that the NMP only supports one master join for each       */
/*            interface.                                                     */
/*                                                                           */
/*****************************************************************************/

typedef struct spm_sck_join_cb
{
    /***************************************************************************/
    /* HAF master join control block.                                          */
    /***************************************************************************/
    AHL_MJ_CB haf_mj_cb;

    /***************************************************************************/
    /* Join status.  The status of NMP_MJ_INIT means that the control block is */
    /* not in use.                                                             */
    /***************************************************************************/
    NBB_BYTE join_status;                   /* One of: SPM_MJ_STATUS; */

    /***************************************************************************/
    /* The register message for the master join interface.                     */
    /***************************************************************************/
    NBB_IPS *spm_sck_register_ips;

} SPM_SCK_JOIN_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MTL_JOIN_CB                                               */
/*                                                                           */
/* Description: SPM MTL master join control block.                          */
/*                                                                           */
/*            There is one control block for MTL interface join.             */
/*                                                                           */
/*****************************************************************************/

typedef struct spm_mtl_join_cb
{
    /***************************************************************************/
    /* HAF master join control block.                                          */
    /***************************************************************************/
    AHL_MJ_CB haf_mj_cb;

    /***************************************************************************/
    /* Join status.  The status of NMP_MJ_INIT means that the control block is */
    /* not in use.                                                             */
    /***************************************************************************/
    NBB_BYTE join_status;                   /* One of: SPM_MJ_STATUS; */

} SPM_MTL_JOIN_CB;

/**STRUCT-********************************************************************/

typedef struct shash_node
{

    NBB_VOID *data;                  /*TLV值中的V,    数据开始存放KEY值的原始数据*/

    NBB_ULONG key;                 /*HASH算法计算后的KEY值*/

    struct shash_node *next;

}SHASH_NODE;

typedef struct shash_table
{

    SHASH_NODE **index;

    NBB_ULONG (*hash_key)();

    NBB_ULONG (*hash_cmp)();

    NBB_ULONG size;

    NBB_ULONG count;

}SHASH_TABLE;

/*****************************************************************************/
/* 定义迭代结果*/
/*****************************************************************************/
#if 1

typedef struct dc_uni
{
    NBB_ULONG ntip[4];        /* 迭代结果链接ARP*/
    NBB_ULONG nextport;      /* 迭代结果链接ARP*/
}DC_UNI;

typedef struct dc_lsp
{
    NBB_ULONG label3;
    NBB_ULONG label2;
    NBB_ULONG label1;

    NBB_ULONG nextip;
    NBB_ULONG nextport;

    NBB_ULONG index;
    NBB_ULONG ftnidx;

}DC_LSP;

#endif

/*****************************************************************************/
/* VRF UROUTE  */
/*****************************************************************************/
#if 1

#define         VRFUROUTEKEYLGTH     20      /*key  length*/

typedef struct vrfuroute_key
{
    NBB_USHORT vrfid;
    NBB_BYTE addrtype;
    NBB_BYTE mask;
    NBB_ULONG dip[4];

}VRFUROUTE_KEY;

typedef struct vrfuroute_base
{
    NBB_BYTE routetype;
    NBB_BYTE prmode;
    NBB_BYTE ecmpmode;
    NBB_BYTE ecmpnum;

    NBB_BYTE res[4];

}VRFUROUTE_BASE;

typedef struct vrfuroute_next
{
    NBB_BYTE res1[3];
    NBB_BYTE nexttype;

    NBB_ULONG nextip[4];

    NBB_ULONG nextport;
    NBB_ULONG label;

    NBB_BYTE nexttypeb;
    NBB_BYTE res2[1];
    NBB_USHORT bfdid;

}VRFUROUTE_NEXT;


typedef struct vrfuroute
{
    AVLL_NODE route_node;

    VRFUROUTE_KEY key;

    VRFUROUTE_BASE base;
    VRFUROUTE_NEXT nexthop[8];
	
    DC_UNI dchop[8];
    NBB_ULONG nextindex[8];
	
    NBB_BYTE ipv4num;
    NBB_BYTE ipv6num;
    NBB_BYTE flag;
    NBB_BYTE memflag;

    NBB_BYTE frrtype;
    NBB_BYTE ecmpflag;
    NBB_USHORT frrecmpid;
	
    NBB_ULONG memindex;
    struct vrfuroute *next;

}VRFUROUTE;

typedef struct ucroute_info
{
    NBB_ULONG delipsnum;
    NBB_ULONG addipsnum;

    NBB_ULONG delipsok;
    NBB_ULONG addipsok;
}UCROUTE_INFO;


#endif

/**STRUCT+********************************************************************/
/* Structure: SPM_SOCKET_CB                                                  */
/*                                                                           */
/* Description: Basic Control block used for the FH-SPM sockets.             */
/*                                                                           */
/*****************************************************************************/

typedef struct spm_socket_cb
{
    /***************************************************************************/
    /* Application socket handle.                                              */
    /***************************************************************************/
    NBB_HANDLE appl_sock_handle;

    /***************************************************************************/
    /* Stub process ID to send data signals for this socket to.                */
    /***************************************************************************/
    NBB_PROC_ID stub_data_pid;

    /***************************************************************************/
    /* Stub queue ID to send data signals for this socket to.                  */
    /***************************************************************************/
    NBB_QUEUE_ID stub_data_qid;

    /***************************************************************************/
    /* Stub socket root list. thread_socket_cb.                                */
    /***************************************************************************/
    NBB_LQE thread_list;

} SPM_SOCKET_CB;

/**STRUCT-********************************************************************/

/**STRUCT*********************************************************************/
/* Structure: SPM_THREAD_SOCKET_CB                                           */
/*                                                                           */
/* Description: SPM communicate with every thread by the socket_cb.          */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_thread_socket_cb
{

    /***************************************************************************/
    /* Stub socket self list.                                                  */
    /***************************************************************************/
    NBB_LQE lqe_list;

    /***************************************************************************/
    /* Stub join handle to use in data signals for this socket.                */
    /***************************************************************************/
    NBB_HANDLE stub_data_join_handle;

    /***************************************************************************/
    /* Stub socket handle.                                                     */
    /***************************************************************************/
    NBB_HANDLE stub_sock_handle;

    /***************************************************************************/
    /* Request: Remote port.  This is the port used by the remote endpoint     */
    /* that opened the incoming connection.                                    */
    /***************************************************************************/
    NBB_USHORT remote_port;

    /***************************************************************************/
    /*  recv data buffer                                                       */
    /***************************************************************************/
    NBB_ULONG recv_len;

    /***************************************************************************/
    /*  thread socket communicate recv data buffe                              */
    /***************************************************************************/
    NBB_BYTE recv_buffer[SPM_TCP_DATA_BUFFER_SIZE];

    /***************************************************************************/
    /*  send data buffer                                                       */
    /***************************************************************************/
    NBB_ULONG send_len;

    /***************************************************************************/
    /*  thread socket communicate send data buffer.                            */
    /***************************************************************************/
    NBB_BYTE send_buffer[SPM_TCP_DATA_BUFFER_SIZE];

}  SPM_THREAD_SOCKET_CB;

/**STRUCT+********************************************************************/
/* Structure: SPM_GLOBAL_CB                                                    */
/*                                                                           */
/* Description: 全局配置（条目数1）。                                                       */
/*                                                                           */
/* Added by xiaoxiang, 2013/04/11                                             */
/*****************************************************************************/
typedef struct spm_global_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_global_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_global_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    NBB_ULONG global_id;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_BASIC_DATA basic_cfg_cb;

    /***************************************************************************/
    /* CFM配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_CFM_DATA cfm_cfg_data;

    /***************************************************************************/
    /* 门限配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_THRES_DATA thres_data;

    /***************************************************************************/
    /* OAM配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_OAM_DATA oam_data;

    /***************************************************************************/
    /* IP FRR 配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_IPFRR_DATA ip_frr_data;

    /***************************************************************************/
    /* VPN FRR 配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_VPNFRR_DATA vpn_frr_data;
} SPM_GLOBAL_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_CIR_EMU_CB                                                  */
/*                                                                           */
/* Description: 电路仿真盘全局配置（条目数1）。                              */
/*                                                                           */
/* Added by xiaoxiang, 2013/06/24                                            */
/*****************************************************************************/
typedef struct spm_cir_emu_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_cir_emu_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_cir_emu_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_KEY stCirEmuKey;

    /***************************************************************************/
    /* 公共门限配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_PUB_LIMIT *pub_limit_cb;

    /***************************************************************************/
    /* 同步时钟源输出配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_SYN_CLK *syn_clk_cb;

    /***************************************************************************/
    /* 电口盘配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_ELTRC_BOARD *eltrc_board_cb;

    /***************************************************************************/
    /* 再生段门限配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_REBTH_LIMIT *rebth_limit_cb;

    /***************************************************************************/
    /* 复用段门限配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_MU_LIMIT *mu_limit_cb;

    /***************************************************************************/
    /* 高阶通道门限配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_HIGH_ORD_PASS *high_ord_pass_cb;

    /***************************************************************************/
    /* 低阶通道门限配置。                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_LOW_ORD_PASS *low_ord_pass_cb;
    
} SPM_CIR_EMU_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MAC_RECYCLE_CB                                             */
/*                                                                           */
/* Description: MAC地址撤销命令（条目数1）。                                 */
/*                                                                           */
/* Added by xiaoxiang, 2013/04/12                                            */
/*****************************************************************************/
typedef struct spm_mac_recycle_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_mac_recycle_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_mac_recycle_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    NBB_ULONG mac_recycle_id;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_MAC_WITHDRAW_DATA basic_cfg_cb;

} SPM_MAC_RECYCLE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_NF_CTRL_CB                                             */
/*                                                                           */
/* Description:流采样控制命令                                 */
/*                                                                           */
/* Added by fansongbo, 2013/08/12                                            */
/*****************************************************************************/
typedef struct spm_nf_ctrl_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_nf_ctrl_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_nf_ctrl_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    NBB_ULONG nf_ctrl_id;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_MAC_WITHDRAW_DATA basic_cfg_cb;

} SPM_NF_CTRL_CB;

/**STRUCT-********************************************************************/


/**STRUCT*********************************************************************/
/* Structure: SPM_QOS_PORT_CB                                           */
/*                                                                           */
/* Description: qos物理端口        */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_qos_port_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_port_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_qos_port_handle;

    /* key值: 物理端口*/
    NBB_USHORT port_key;

    /* LSP policy索引*/
    AVLL_TREE lsp_tree;

    /*pw policy索引*/
    AVLL_TREE pw_vrf_tree;

    /* pw policy索引*/
    AVLL_TREE pw_vc_tree;

    AVLL_TREE group_tree;

} SPM_QOS_PORT_CB;






/**STRUCT-********************************************************************/


/**STRUCT*********************************************************************/
/* Structure: SPM_CLK_CB                                           */
/*                                                                           */
/* Description: 时钟全局配置        */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_clk_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    //AVLL_NODE spm_clk_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_clk_handle;

    /***************************************************************************/
    /* key值: vpws_id.                                                         */
    /***************************************************************************/
    NBB_ULONG clk_id_key;

    /***************************************************************************/
    /* 时钟基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_GLOBAL_CLK_BASIC basic_clk_cb;

    /***************************************************************************/
    /* 外时钟配置。                                                           */
    /***************************************************************************/
    ATG_DCI_EXT_CLK  ext_clk_cb;

    /***************************************************************************/
    /* 定时源时钟输入配置。                                                           */
    /***************************************************************************/
    NBB_USHORT clk_pri_num; 
    ATG_DCI_TIMER_SRCCLK  clk_pri_cb[ATG_DCI_G_CLK_TIMER_SRCCLK_NUM];
    
    /***************************************************************************/
    /* 外时钟导出配置。                                                           */
    /***************************************************************************/
    NBB_USHORT t4_clk_num;
    ATG_DCI_EXTCLK_EXPORT  t4_clk_cb[ATG_DCI_G_CLK_EXTCLK_EXPORT_NUM];
    
     /***************************************************************************/
    /* 输出源QL 配置。                                                           */
    /***************************************************************************/
     NBB_USHORT ql_out_num;
    ATG_DCI_OUTSRC_QL  ql_out_cb[ATG_DCI_G_CLK_OUTSRC_QL_NUM];  
     
} SPM_CLK_CB;

/**STRUCT*********************************************************************/
/* Structure: SPM_PTP_CB                                           */
/*                                                                           */
/* Description: 时间同步基本配置        */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_ptp_cb
{
    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_ptp_handle;

    /***************************************************************************/
    /* key值: ptp_id_key.                                                         */
    /***************************************************************************/
    NBB_ULONG ptp_id_key;

    /***************************************************************************/
    /* 同步参数配置。                                                              */
    /***************************************************************************/
    ATG_DCI_CLK_SYNC_PARA sync_para_ptp_cb;

    /***************************************************************************/
    /* 本地时钟配置。                                                           */
    /***************************************************************************/
    ATG_DCI_LOCAL_CLK  local_clk_ptp_cb;

    /***************************************************************************/
    /* 1PPS+TOD接口配置。                                                           */
    /***************************************************************************/
    ATG_DCI_PPS_TOD_IF_DATA  pps_tod_if_ptp_cb;
} SPM_PTP_CB;

/**STRUCT+********************************************************************/
/* Structure: SPM_CMD_SWITCH_CB                                            */
/*                                                                           */
/* Description:倒换命令配置块                     */
/*                                                                           */
/* Added by zhangsen 2014-4-3                                            */
/*****************************************************************************/
typedef struct spm_cmd_switch_cb
{
    /***************************************************************************/
    /*key值: ID:始终为1                                                        */
    /***************************************************************************/
    NBB_ULONG key;

    /***************************************************************************/
    /*DNI-PW倒换命令                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_DNIPW dni_pw_switch_cfg_cb;
    
    /***************************************************************************/
    /*MSP倒换命令                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_MSP msp_switch_cfg_cb;
    
    /***************************************************************************/
    /*MSRP倒换命令                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_MSRP msrp_switch_cfg_cb;
    
    /***************************************************************************/
    /*PW冗余倒换命令                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_PW1TO1 pw1To1_switch_cfg_cb;
    
    /***************************************************************************/
    /*BYPASS倒换命令                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_BYPASS bypass_switch_cfg_cb;

}SPM_CMD_SWITCH_CB;
/**STRUCT-********************************************************************/

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_SHARED_LOCAL                                               */
/*                                                                           */
/* Description: SPM shared local data.                                       */
/*                                                                           */
/*            The SPM keeps all 'local' data in                              */
/*            SHARED local rather than in LOCAL data, and calls              */
/*            NBS_GET_SHARED_DATA / NBS_RELEASE_SHARED_DATA around every     */
/*            entry point into the stub.  This is simpler than keeping some  */
/*            fields in LOCAL data and some in SHARED data and locking       */
/*            SHARED data only when accessing it.  There is no significant   */
/*            performance impact from using the simpler SHARED-only          */
/*            approach.                                                      */
/*                                                                           */
/*****************************************************************************/

typedef struct spm_shared_local
{
    /***************************************************************************/
    /* Local mtl path.                                                        */
    /***************************************************************************/
    NBB_ULONG local_frame;
    NBB_ULONG local_slot;
    NBB_ULONG local_cpu;

    /***************************************************************************/
    /* Our N-BASE process id.                                                  */
    /***************************************************************************/
    NBB_PROC_ID spm_our_pid;

    /***************************************************************************/
    /* SM's process identifier.                                                */
    /***************************************************************************/
    NBB_PROC_ID sm_pid;

    /***************************************************************************/
    /* SM's instance handle.                                                   */
    /***************************************************************************/
    NBB_HANDLE sm_handle;

    /***************************************************************************/
    /* Product instance index.                                                 */
    /***************************************************************************/
    NBB_ULONG spm_index;

    /***************************************************************************/
    /* SPM HAF data (including sm_pid).                                        */
    /***************************************************************************/
    AHL_DATA spm_haf_data;

    /***************************************************************************/
    /* MASTER一方的Join控制块                                                  */
    /***************************************************************************/
    SPM_MJ_JOIN_CB spm_dci_join_cb;

    SPM_SCK_JOIN_CB sck_join_cb;

    SPM_MTL_JOIN_CB mtl_join_cb;

    SPM_SOCKET_CB sck_cb;

    /***************************************************************************/
    /* List of SPM joins.  Each entry is an SPM_SJ_JOIN_CB.                    */
    /* 由于存在多个Master使用一个Slave的情况，                                 */
    /*因此对于每个Master，都要建立一个                                         */
    /* SPM_SJ_JOIN_CB，这些SPM_SJ_JOIN_CB形成一个链表                          */
    /***************************************************************************/
    NBB_LQE spm_sbi_join_list;
    
    /***************************************************************************/
    /* proc_timer是否处于active状态。                                          */
    /***************************************************************************/
    NBB_BOOL proc_timer_active;

    /***************************************************************************/
    /* 用于多消息处理的定时器。                                                */
    /***************************************************************************/
#define PROC_SPM_TIMER_TIMEOUT  60000
    NBB_TIMER_DATA proc_timer;
    
    /***************************************************************************/
    /* proc_timer是否处于active状态。                                          */
    /***************************************************************************/
    NBB_BOOL proc_timer_clearcfg_active;

    /***************************************************************************/
    /* 用于多消息处理的定时器。                                                */
    /***************************************************************************/
#define PROC_TIMER_CLEARCFG_TIMEOUT  180000
    NBB_TIMER_DATA proc_timer_clearcfg;

    /***************************************************************************/
    /* 用于状态处理的回调函数集合.                                             */
    /***************************************************************************/
#define SPM_STATE_TLV_MAXNUM 0xFF
    SPM_PROC_DATA_FUNC state_func[SPM_STATE_TLV_MAXNUM];

    /***************************************************************************/
    /* AMBL global data.                                                       */
    /***************************************************************************/
    AMBL_DATA spm_ambl_data;

    /***************************************************************************/
    /* Flag : Whether the HAF library is initialized.                          */
    /***************************************************************************/
    NBB_BYTE spm_haf_initialized;                       /* One of: ATG_YES_NO; */

    /***************************************************************************/
    /* Flag : Whether the MIB product support library is initialized.          */
    /***************************************************************************/
    NBB_BYTE spm_ambl_initialized;                      /* One of: ATG_YES_NO; */

    /***************************************************************************/
    /* Flag : Whether the top level MIB has been registered.  This is only     */
    /* necessary if the stub is being used to emulate interfaces, since the    */
    /* top level MIB is sufficient for normal stub operation.                  */
    /***************************************************************************/
    NBB_BYTE spm_top_mib_registered;                    /* One of: ATG_YES_NO; */

    /***************************************************************************/
    /* Data about the state of the MIB row.                                    */
    /***************************************************************************/
    AMBL_ROW_DATA spm_row_data;

    /***************************************************************************/
    /* Top Mib row data.                                                       */
    /***************************************************************************/
    AMB_CUST_SPM spm_top_mib_data;

    /***************************************************************************/
    /* Data pointer for MIB definitions in AMBL.                               */
    /***************************************************************************/
    NBB_VOID *spm_prod_mib_data;

    /***************************************************************************/
    /* ATG_YES if stub supports Fault Tolerance, ATG_NO otherwise.             */
    /***************************************************************************/
    NBB_BYTE ft_capable;                               /* One of: ATG_YES_NO;  */

    /***************************************************************************/
    /* 指令流水号。                                                            */
    /***************************************************************************/
    NBB_ULONG instr_number;

    /***************************************************************************/
    /* 子卡槽位对应的 join_cb. 目前只有两块子卡.                               */
    /***************************************************************************/
    SPM_SJ_JOIN_CB *sub_card_cb[MAX_SUB_CARD_NUM];

    /***************************************************************************/
    /*  mtl communicate data buffer.                                          */
    /***************************************************************************/
    NBB_BYTE data_buffer[SPM_MTL_DATA_BUFFER_SIZE];

    /***************************************************************************/
    /* 接口物理配置树    Added by xiaoxiang, 2012/9/11                         */
    /***************************************************************************/
    AVLL_TREE physical_port_tree;

    /***************************************************************************/
    /* 端口逻辑配置树    Added by xiaoxiang, 2012/9/11                         */
    /***************************************************************************/
    AVLL_TREE logical_port_tree;

    /***************************************************************************/
    /* 链路聚合树    Added by xiaoxiang, 2012/12/1                             */
    /***************************************************************************/
    AVLL_TREE lag_tree;

    /***************************************************************************/
    /* VC配置树        Added by xiaoxiang, 2012/9/18                           */
    /***************************************************************************/
    AVLL_TREE vc_tree;

    /***************************************************************************/
    /* VPWS配置树        Added by xiaoxiang, 2012/9/18                         */
    /***************************************************************************/
    AVLL_TREE vpws_tree;

    /***************************************************************************/
    /* VPLS配置树        Added by xiaoxiang, 2012/9/18                         */
    /***************************************************************************/
    AVLL_TREE vpls_tree;

    /***************************************************************************/
    /* OAM配置树       Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyoamid;

    /***************************************************************************/
    /* OAM rsvp发方向配置为key值存储的配置      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyrsvptx;

    /***************************************************************************/
    /* OAM rsvp收方向配置为key值存储的配置      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyrsvprx;

    /***************************************************************************/
    /* OAM pw配置为key存储的配置      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebypw; 

    /***************************************************************************/
    /* OAM vs配置为key存储的配置      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyindex;
    
    /***************************************************************************/
    /* OAM wrapping配置为key存储的配置      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebywrapping;

    /***************************************************************************/
    /* VPLS组播配置树        Added by xiaoxiang, 2013/1/12                     */
    /***************************************************************************/
    AVLL_TREE vpls_mc_tree;

    /***************************************************************************/
    /* VPLS静态单播配置树        Added by xiaoxiang, 2013/4/18                 */
    /***************************************************************************/
    AVLL_TREE vpls_uc_tree;

	/***************************************************************************/
    /* IGMP        Added by zhangzhm	, 2016/1/8                 */
    /***************************************************************************/
	AVLL_TREE vpls_l3_mc_tree;
    
    /***************************************************************************/
    /* ARP配置树        Added by xiaoxiang, 2012/11/10                         */
    /***************************************************************************/
    AVLL_TREE arp_tree;

    /***************************************************************************/
    /* IPv6 邻居缓存树        Added by xiaoxiang, 2013/4/15                    */
    /***************************************************************************/
    AVLL_TREE ncache_tree;
    
    /***************************************************************************/
    /* 端口镜像树        Added by xiaoxiang, 2013/03/15                        */
    /***************************************************************************/
    AVLL_TREE port_mirror_tree;

    /***************************************************************************/
    /* 全局配置（条目数1）        Added by xiaoxiang, 2013/04/11               */
    /***************************************************************************/
    SPM_GLOBAL_CB global_cb;

    /***************************************************************************/
    /* 电路仿真盘全局配置（条目数1）        Added by xiaoxiang, 2013/06/24     */
    /*改为树结构Added by fansongbo, 2015/1/27*/
    /***************************************************************************/
    //SPM_CIR_EMU_CB cir_emu_cb;
    AVLL_TREE cir_emu_tree;
    
    /***************************************************************************/
    /* MAC地址撤销命令（条目数1）        Added by xiaoxiang, 2013/04/18        */
    /***************************************************************************/
    SPM_MAC_RECYCLE_CB mac_recycle_cb;

    /***************************************************************************/
    /* 流采样控制命令        Added by fansongbo, 2013/08/12        */
    /***************************************************************************/
    SPM_MAC_RECYCLE_CB nf_ctrl_cb;
    
    /***************************************************************************/
    /* Bypass双归保护树        Added by xiaoxiang, 2013/07/16                  */
    /***************************************************************************/
    AVLL_TREE bypass_tree;

    /***************************************************************************/
    /* SWITCH VC表树        Added by xiaoxiang, 2013/03/26                     */
    /***************************************************************************/
    AVLL_TREE switch_vc_tree;

    /***************************************************************************/
    /* MSP保护表树        Added by xiaoxiang, 2014/04/10                     */
    /***************************************************************************/
    AVLL_TREE msp_tree;
    
    /***************************************************************************/
    /* FTN                        */
    /***************************************************************************/
    AVLL_TREE ftn_tree;

    /***************************************************************************/
    /* ILM                        */
    /***************************************************************************/
    SHASH_TABLE *ilmhash;

    /***************************************************************************/
    /* CR LSP RX                        */
    /***************************************************************************/
    SHASH_TABLE *crlsprxhash;

    /***************************************************************************/
    /* CR LSP TX                        */
    /***************************************************************************/
    SHASH_TABLE *crlsptxhash;

    /***************************************************************************/
    /* 存放直连路由*/
    /***************************************************************************/
    AVLL_TREE dcroute_tree;

    /***************************************************************************/
    /* 存放loopback*/
    /***************************************************************************/
    AVLL_TREE lbroute_tree;

    /***************************************************************************/
    /* 存放单播路由*/
    /***************************************************************************/
    AVLL_TREE vrfuroute_tree;

    /***************************************************************************/
    /* TUNNEL CHOOSE         */
    /***************************************************************************/
    SHASH_TABLE *tunnelchhash;

    /***************************************************************************/
    /* LSP PROT         */
    /***************************************************************************/
    SHASH_TABLE *lspprothash;

    /***************************************************************************/
    /* 路由UNI 出口*/
    /***************************************************************************/
    AVLL_TREE outuni;

    /***************************************************************************/
    /* 路由内存*/
    /***************************************************************************/
    VRFUROUTE *routemem;

    //VRFUROUTE   g_uroute[200000];

    /***************************************************************************/
    /*组播 路由*/
    /***************************************************************************/
    AVLL_TREE  mcroute_tree;

    /***************************************************************************/
    /*VRF 实例*/
    /***************************************************************************/
    AVLL_TREE  vrfins_tree;

    /***************************************************************************/
    /* 引用计数*/
    /***************************************************************************/
    AVLL_TREE rsvpcite;
    AVLL_TREE ldpcite;
	
    /***************************************************************************/
    /*下发IPS消息数目统计*/
    /***************************************************************************/
    UCROUTE_INFO  ipsroute;

    /***************************************************************************/
    /*VPN FRR */
    /***************************************************************************/
    AVLL_TREE  peer_tree;

    /***************************************************************************/
    /*ECMP ID*/
    /***************************************************************************/
    AVLL_TREE  ecmp_tree;

    /***************************************************************************/
    /*PEER FTN*/
    /***************************************************************************/
    AVLL_TREE  peftn_tree;

    /***************************************************************************/
    /*PEER ECMP*/
    /***************************************************************************/
    AVLL_TREE  peecmp_tree;

    /***************************************************************************/
    /*BFD FRR*/
    /***************************************************************************/
    AVLL_TREE  bfdfrr_tree;

    /***************************************************************************/
    /*IP FRR*/
    /***************************************************************************/
    AVLL_TREE  ipfrr_tree;

	/***************************************************************************/
    /*RSVP LSP*/
    /***************************************************************************/
    AVLL_TREE  rsvplsp_tree;

    /***************************************************************************/
    /* spm module communicate with outside thread socket list.                 */
    /***************************************************************************/
    NBB_LQE spm_thread_socket_list;

    /***************************************************************************/
    /* 存放ds_domain路由*/
    /***************************************************************************/
    AVLL_TREE ds_domain_tree;

    /***************************************************************************/
    /* 存放ds路由*/
    /***************************************************************************/
    AVLL_TREE ds_tree;

    /***************************************************************************/
    /* 存放逻辑端口L3 INTF ds配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_logic_intf_tree;

    /***************************************************************************/
    /* 存放逻辑端口L2 FLOW ds配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_logic_flow_tree;

    /***************************************************************************/
    /* 存放L2 FLOW VPUNI ds配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_logic_univp_tree;

    /***************************************************************************/
    /* 存放用户组配置*/
    /***************************************************************************/
    AVLL_TREE qos_user_group_tree;
    
     /***************************************************************************/
    /* 存放ip nf配置*/
    /***************************************************************************/
    AVLL_TREE ip_nf_tree;

    /***************************************************************************/
    /* 存放nf aggr配置*/
    /***************************************************************************/
    AVLL_TREE nf_aggr_tree;
    /***************************************************************************/
    /* 存放VC ds配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_vc_tree;

    /***************************************************************************/
    /* 存放VRF ds配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_vrf_tree;

    /***************************************************************************/
    /* 存放ILM ds配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_ilm_tree;

    /***************************************************************************/
    /* 存放FTN  qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_ftn_tree;

    /***************************************************************************/
    /* 存放rx_lsp  qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_rx_lsp_tree;

    /***************************************************************************/
    /* 存放tx_lsp  qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_tx_lsp_tree;

    /***************************************************************************/
    /* 存放lsp  qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_ds_tunnel_tree;

    /***************************************************************************/
    /* 存放物理端口qos配置*/
    /***************************************************************************/
    //SPM_QOS_PORT_CB qos_port_cb[MAX_SLOT_NUM][MAX_PHYSIC_PORT_NUM];

    /***************************************************************************/
    /* 存放acl qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_acl_tree;

    /***************************************************************************/
    /* 存放meter配置*/
    /***************************************************************************/
    //NBB_ULONG flow_meter_id[MAX_METER_NUM];
#ifdef PTN690

    /***************************************************************************/
    /* 存放meter桶配置*/
    /***************************************************************************/
    //NBB_ULONG meter_bukect_id[MAX_METER_BUKECT_NUM];

    /***************************************************************************/
    /* 存放color action配置*/
    /***************************************************************************/
    //NBB_ULONG color_action_id[MAX_COLOR_ACTION_NUM];

    /***************************************************************************/
    /* 存放txlsp_car qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_txlsp_car_tree;

    /***************************************************************************/
    /* 存放pw_car qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_pw_car_tree;
#endif  
    /***************************************************************************/
    /* 存放复杂流分类全局ruleID配置*/
    /***************************************************************************/
    //NBB_ULONG flow_rule_id[MAX_FLOW_CLASSIFY_RULE_ID];


    /***************************************************************************/
    /* 为复杂流分类分配匹配端口acl id */
    /***************************************************************************/
    //NBB_ULONG g_classify_port_acl_id[MAX_CLASSIFY_PORT_ACL_ID_NUM];

    /***************************************************************************/
    /* 为复杂流分类分配高级acl id */
    /***************************************************************************/
    //NBB_ULONG g_classify_high_acl_id[MAX_CLASSIFY_HIGH_ACL_ID_NUM];

    /***************************************************************************/
    /* 为复杂流分类分配匹配以太网acl id */
    /***************************************************************************/
    //NBB_ULONG g_classify_eth_acl_id[MAX_CLASSIFY_ETH_ACL_ID_NUM];


    /***************************************************************************/
    /* 存放twamp_ipv4树的key值                                                 */
    /***************************************************************************/
    AVLL_TREE twamp_ipv4_tree;

    /***************************************************************************/
    /* 存放twamp_ipv6树的key值                                                 */
    /***************************************************************************/
    AVLL_TREE twamp_ipv6_tree;
    
    /***************************************************************************/
    /* 存放filter*/
    /***************************************************************************/
    AVLL_TREE qos_filter_tree;

    /***************************************************************************/
    /* 存放上话复杂流分类 qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_flow_classify_tree;

    /***************************************************************************/
    /* 存放上话用户 qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_up_usr_tree;

    /***************************************************************************/
    /* 存放上话流相关 qos策略*/
    /***************************************************************************/
    AVLL_TREE qos_flow_up_usr_tree;

    /***************************************************************************/
    /* 存放classify qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_classify_tree;

    /***************************************************************************/
    /* 存放action qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_action_tree;

    /***************************************************************************/
    /* 存放policy qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_policy_tree;

    /***************************************************************************/
    /* 存放policy qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_defend_policy_tree;

    /***************************************************************************/
    /* 存放policy qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_defend_tree;

    /***************************************************************************/
    /* 存放wred qos配置*/
    /***************************************************************************/
    AVLL_TREE qos_wred_tree;

    /***************************************************************************/
    /* 存放wred qos配置*/
    /***************************************************************************/
    AVLL_TREE port_wred_tree;

    /*开启HQOS的槽位*/
    AVLL_TREE hqos_tree;

    /*只支持4个槽位的HQOS*/
    NBB_BYTE hqos_port_index[4];

    /***************************************************************************/
    /* 存放vrf qos 实例配置*/
    /***************************************************************************/
    AVLL_TREE qos_vrf_tree;

    /***************************************************************************/
    /* 存放Hqos LSPID*/
    /***************************************************************************/
    NBB_ULONG hqos_lspId;

    /***************************************************************************/
    /* 存放Hqos LSP 资源*/
    /***************************************************************************/
    NBB_BYTE hqos_lsp_pool[HQOS_MAX_LSP_ID];

    /***************************************************************************/
    /* 存放Hqos pwId*/
    /***************************************************************************/
    NBB_ULONG hqos_pwId;

    /***************************************************************************/
    /* 存放Hqos PW 资源*/
    /***************************************************************************/
    NBB_BYTE hqos_pw_pool[HQOS_MAX_PW_ID];

    /***************************************************************************/
    /* 存放防攻击的每个协议的rule个数*/
    /***************************************************************************/
    NBB_BYTE qos_defend_num[SIGNAL_NUM + 1];
    
    /***************************************************************************/
    /* 存放防攻击的每个协议的rule偏移*/
    /***************************************************************************/
    NBB_USHORT qos_defend_offset[SIGNAL_NUM + 1];

    /*************定时器********************/

    NBB_TIMER_DATA proc_bfd_timer;
    NBB_TIMER_DATA proc_bfd_timer1;
    NBB_TIMER_DATA proc_bfd_timer2;
    NBB_TIMER_DATA proc_bfd_syncheck;

    /***************************************************************************/
    /* proc_bfd_syncheck是否处于active状态。                                          */
    /***************************************************************************/
    NBB_BOOL proc_bfdsyncheck_active;

    /***************************************************************************/
    /* VPLS组播CUD值，全盘唯一，64K。                                   */
    /***************************************************************************/
    NBB_INT mc_cud[ATG_DCI_CUD_NUM];

    /***************************************************************************/
    /* VC_NHI_ID 全盘唯一，64K。                                   */
    /***************************************************************************/
    NBB_UINT vc_nhi_id[SPM_VC_NHI_ID_NUM];

    /***************************************************************************/
    /* 本盘槽位号。                                   */
    /***************************************************************************/
    NBB_BYTE local_slot_id;

    /***************************************************************************/
    /* C3个数。                                   */
    /***************************************************************************/
    NBB_BYTE c3_num;

    SPM_CLK_CB clk_glob_cb;

	/***************************************************************************/
    /* 时间同步基本配置（条目数1）        Added by zengliang, 2013/06/28               */
    /***************************************************************************/
    SPM_PTP_CB ptp_cb;
    
    /***************************************************************************/
    /* 整机端口信息。0xFF表示无端口信息                                   */
    /***************************************************************************/
    NBB_BYTE dev_port_info[SPM_PORT_MAX_NUM][SPM_SLOT_MAX_NUM];

    NBB_BYTE rcu_stat;

	/***************************************************************************/
    /* ASSOCIATE_IF树       Added by zhangzhm, 2013/11/20                    */
    /***************************************************************************/
    AVLL_TREE associate_if_tree;
    
    /***************************************************************************/
    /* 存放113 mac vpls 状态查询收到的状态上报信息*/
    /*包含T /L /V*/
    /***************************************************************************/
    NBB_BYTE mac_vpls_sta_info[20 + 10 * VPLS_MAC_STA_MAX_NUM];
    
    /***************************************************************************/
    /* 存放124 mac vpls by mac状态查询收到的状态上报信息*/
    /*包含T /L /V*/
    /***************************************************************************/
    NBB_BYTE mac_vpls_mac_sta_info[12];   
     
    /***************************************************************************/
    /* 为MSP保护预申请64个frr group id*/
    /***************************************************************************/
    NBB_USHORT msp_frr_group_id[ATG_DCI_MSP_NUM];

    /***************************************************************************/
    /* 倒换命令配置（条目数1）        Added by xiaoxiang, 2014/04/3           */
    /***************************************************************************/
    SPM_CMD_SWITCH_CB cmd_switch_cb;
    
    /***************************************************************************/
    /* 存放4 保护状态查询收到的状态上报信息*/
    /*包含T /L /V*/
    /***************************************************************************/
    NBB_BYTE prot_sta_info[PROT_STA_INFO_MAX_NUM];
    
    
} SPM_SHARED_LOCAL;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* N-BASE priority.                                                          */
/*****************************************************************************/
#define SPM_NBASE_PRIORITY               0x40

/*****************************************************************************/
/* 定义内存类型，包括组件的SHARED_LOCAL和所有控制块                          */
/*****************************************************************************/
#define MEM_SPM_SHARED                          ((NBB_LONG)(PCT_SPM | 0x00000001))
#define MEM_SPM_SBI_JOIN_CB                     ((NBB_LONG)(PCT_SPM | 0x00000002))

#define MEM_SPM_PHYSICAL_PORT_CB                ((NBB_LONG)(PCT_SPM | 0x00000100))
#define MEM_SPM_PHY_PORT_BASIC_CB               ((NBB_LONG)(PCT_SPM | 0x00000101))
#define MEM_SPM_PHY_PORT_PHY_CB                 ((NBB_LONG)(PCT_SPM | 0x00000102))
#define MEM_SPM_PHY_PORT_POS_CB                 ((NBB_LONG)(PCT_SPM | 0x00000103))
#define MEM_SPM_PHY_PORT_SDH_SOVHD_CB           ((NBB_LONG)(PCT_SPM | 0x00000104))
#define MEM_SPM_PHY_PORT_CES_CB                 ((NBB_LONG)(PCT_SPM | 0x00000105))
#define MEM_SPM_PHY_PORT_CEP_CB                 ((NBB_LONG)(PCT_SPM | 0x00000106))
#define MEM_SPM_PHY_PORT_EFM_OAM_CB             ((NBB_LONG)(PCT_SPM | 0x00000107))
#define MEM_SPM_PHY_PORT_TIME_SYNC_CB           ((NBB_LONG)(PCT_SPM | 0x00000108))
#define MEM_SPM_PHY_PORT_MSG_SUPPR_CB           ((NBB_LONG)(PCT_SPM | 0x00000109))
#define MEM_SPM_PHY_PORT_FLOW_SHAPE_CB          ((NBB_LONG)(PCT_SPM | 0x0000010A))
#define MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB      ((NBB_LONG)(PCT_SPM | 0x0000010B))
#define MEM_SPM_PHY_PORT_CONGEST_POLICY_CB      ((NBB_LONG)(PCT_SPM | 0x0000010C))
#define MEM_SPM_PHY_PORT_IPV4_NF_CB             ((NBB_LONG)(PCT_SPM | 0x0000010D))
#define MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB         ((NBB_LONG)(PCT_SPM | 0x0000010E))
#define MEM_SPM_PHY_PORT_IPV6_NF_CB             ((NBB_LONG)(PCT_SPM | 0x0000010F))
#define MEM_SPM_PHY_PORT_IPV6_SAMPLE_CB         ((NBB_LONG)(PCT_SPM | 0x00000110))
#define MEM_SPM_PHY_PORT_COMMON_CB              ((NBB_LONG)(PCT_SPM | 0x00000111))
#define MEM_SPM_PHY_PORT_SDH_TOVHD_CB           ((NBB_LONG)(PCT_SPM | 0x00000112))
#define MEM_SPM_PHY_PORT_ALS_CB           ((NBB_LONG)(PCT_SPM | 0x00000113))
#define MEM_SPM_PHY_PORT_POWER_THRESHOLD_CB           ((NBB_LONG)(PCT_SPM | 0x00000114))

#define MEM_SPM_LOGICAL_PORT_CB                 ((NBB_LONG)(PCT_SPM | 0x00000200))
#define MEM_SPM_LOG_PORT_BASIC_CB               ((NBB_LONG)(PCT_SPM | 0x00000201))
#define MEM_SPM_LOG_PORT_PHY_CB                 ((NBB_LONG)(PCT_SPM | 0x00000202))
#define MEM_SPM_LOG_PORT_L3_CB                  ((NBB_LONG)(PCT_SPM | 0x00000203))
#define MEM_SPM_LOG_PORT_L2_CB                  ((NBB_LONG)(PCT_SPM | 0x00000204))
#define MEM_SPM_LOG_PORT_CES_CB                 ((NBB_LONG)(PCT_SPM | 0x00000205))
#define MEM_SPM_LOG_PORT_FLOW_CB                ((NBB_LONG)(PCT_SPM | 0x00000206))
#define MEM_SPM_LOG_PORT_IPV4_CB                ((NBB_LONG)(PCT_SPM | 0x00000207))
#define MEM_SPM_LOG_PORT_IPV6_CB                ((NBB_LONG)(PCT_SPM | 0x00000208))
#define MEM_SPM_LOG_PORT_MC_IPV4_CB             ((NBB_LONG)(PCT_SPM | 0x00000209))
#define MEM_SPM_LOG_PORT_VIP_VMAC_CB            ((NBB_LONG)(PCT_SPM | 0x0000020A))
#define MEM_SPM_LOG_PORT_VE_MAC_CB              ((NBB_LONG)(PCT_SPM | 0x0000020B))
#define MEM_SPM_LOG_PORT_DIFF_SERV_CB           ((NBB_LONG)(PCT_SPM | 0x0000020C))
#define MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB      ((NBB_LONG)(PCT_SPM | 0x0000020D))
#define MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB    ((NBB_LONG)(PCT_SPM | 0x0000020E))
#define MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB      ((NBB_LONG)(PCT_SPM | 0x0000020F))
#define MEM_SPM_LOG_PORT_UP_USER_QOS_CB         ((NBB_LONG)(PCT_SPM | 0x00000210))
#define MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB        ((NBB_LONG)(PCT_SPM | 0x00000211))
#define MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB       ((NBB_LONG)(PCT_SPM | 0x00000212))
#define MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB      ((NBB_LONG)(PCT_SPM | 0x00000213))
#define MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB      ((NBB_LONG)(PCT_SPM | 0x00000214))
#define MEM_SPM_LOG_PORT_TERMINAL_IF_CB      	((NBB_LONG)(PCT_SPM | 0x00000215))
#define MEM_SPM_LOG_PORT_MC_IPV6_CB             ((NBB_LONG)(PCT_SPM | 0x00000216))
#define MEM_SPM_LOG_PORT_DS_L2_CB               ((NBB_LONG)(PCT_SPM | 0x00000217))
#define MEM_SPM_LOG_PORT_VIPV6_VMAC_CB            ((NBB_LONG)(PCT_SPM | 0x00000219))

#define MEM_SPM_LAG_CB                          ((NBB_LONG)(PCT_SPM | 0x00000300))
#define MEM_SPM_LAG_GLOBAL_CB                   ((NBB_LONG)(PCT_SPM | 0x00000301))
#define MEM_SPM_LAG_CONF_CB                     ((NBB_LONG)(PCT_SPM | 0x00000302))
#define MEM_SPM_LAG_PORT_CB                     ((NBB_LONG)(PCT_SPM | 0x00000303))
#define MEM_SPM_LAG_NLB_PORT_CB                 ((NBB_LONG)(PCT_SPM | 0x00000304))
#define MEM_SPM_LAG_IPV4_NF_CB                  ((NBB_LONG)(PCT_SPM | 0x00000305))
#define MEM_SPM_LAG_IPNF_SAMPLER                ((NBB_LONG)(PCT_SPM | 0x00000306))
#define MEM_SPM_LAG_IPV6_NF_CB                  ((NBB_LONG)(PCT_SPM | 0x00000307))

#if 1
#define MEM_SPM_ACTIVE_OAM_CB                   ((NBB_LONG)(PCT_SPM | 0x00001B00))
#define MEM_SPM_ACTIVE_OAM_BASEIC_CB            ((NBB_LONG)(PCT_SPM | 0x00001B01))
#define MEM_SPM_ACTIVE_OAM_PW_CB                ((NBB_LONG)(PCT_SPM | 0x00001B02))
#define MEM_SPM_ACTIVE_OAM_VS_CB                ((NBB_LONG)(PCT_SPM | 0x00001B03))
#define MEM_SPM_ACTIVE_OAM_APS_CB               ((NBB_LONG)(PCT_SPM | 0x00001B04))
#define MEM_SPM_ACTIVE_OAM_CV_CB                ((NBB_LONG)(PCT_SPM | 0x00001B05))
#define MEM_SPM_ACTIVE_OAM_CSF_CB               ((NBB_LONG)(PCT_SPM | 0x00001B06))
#define MEM_SPM_ACTIVE_OAM_FDI_CB               ((NBB_LONG)(PCT_SPM | 0x00001B07))
#define MEM_SPM_ACTIVE_OAM_RSVP_CB              ((NBB_LONG)(PCT_SPM | 0x00001B08))
#define MEM_SPM_ACTIVE_OAM_WRAPPING_CB          ((NBB_LONG)(PCT_SPM | 0x00001B09))
#define MEM_SPM_ACTIVE_OAM_ADJUST_CB            ((NBB_LONG)(PCT_SPM | 0x00001B0A))
#define MEM_SPM_ACTIVE_OAM_ALARM_CB             ((NBB_LONG)(PCT_SPM | 0x00001B0C))
#define MEM_SPM_TPOAM_TREE_CB                   ((NBB_LONG)(PCT_SPM | 0x00001B0B))
#define MEM_SPM_OD_OAM_LCK_CB                   ((NBB_LONG)(PCT_SPM | 0x00001C04))
#define MEM_SPM_OD_OAM_LM_CB                    ((NBB_LONG)(PCT_SPM | 0x00001C05))
#define MEM_SPM_OD_OAM_DM_CB                    ((NBB_LONG)(PCT_SPM | 0x00001C06))
#define MEM_SPM_OD_OAM_MIP_CB                   ((NBB_LONG)(PCT_SPM | 0x00001C07))
#define MEM_SPM_EFM_OAM_CB                      ((NBB_LONG)(PCT_SPM | 0x00006A00))
#define MEM_SPM_OD_OAM_CB                       ((NBB_LONG)(PCT_SPM | 0x00001C00))
#define MEM_SPM_OD_OAM_BASIC_CB                 ((NBB_LONG)(PCT_SPM | 0x00001C01))
#define MEM_SPM_OD_OAM_LB_CB                    ((NBB_LONG)(PCT_SPM | 0x00001C02))
#define MEM_SPM_OD_OAM_TST_CB                   ((NBB_LONG)(PCT_SPM | 0x00001C03))
#define MEM_SPM_OAM_LIST                        ((NBB_LONG)(PCT_SPM | 0x00001C04))
#define MEM_SPM_OAM_SEARCH_TREE                 ((NBB_LONG)(PCT_SPM | 0x00001C05))
#define MEM_SPM_TPOAM_YBEND_TREE                ((NBB_LONG)(PCT_SPM | 0x00001C06))
#define MEM_SPM_OAM_SD_LIST                     ((NBB_LONG)(PCT_SPM | 0x00001C07))
#define MEM_SPM_OAM_WRAPPING_LIST               ((NBB_LONG)(PCT_SPM | 0x00001C08))
#define MEM_SPM_OAM_PUC_SYN_ID_INFO_CB          ((NBB_LONG)(PCT_SPM | 0x00001C09))
#endif



#define MEM_SPM_VC_CB                           ((NBB_LONG)(PCT_SPM | 0x00000400))
#define MEM_SPM_VC_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00000401))
#define MEM_SPM_VC_DIFF_SERV_CB                 ((NBB_LONG)(PCT_SPM | 0x00000402))
#define MEM_SPM_VC_UP_VPN_CB                    ((NBB_LONG)(PCT_SPM | 0x00000403))

#define MEM_SPM_VPWS_CB                         ((NBB_LONG)(PCT_SPM | 0x00000500))
#define MEM_SPM_VPWS_BASIC_CB                   ((NBB_LONG)(PCT_SPM | 0x00000501))
#define MEM_SPM_VPWS_NNI_CB                     ((NBB_LONG)(PCT_SPM | 0x00000502))
#define MEM_SPM_VPWS_UNI_CB                     ((NBB_LONG)(PCT_SPM | 0x00000503))
#define MEM_SPM_VPWS_BYPASS_CB                  ((NBB_LONG)(PCT_SPM | 0x00000504))

#define MEM_SPM_VPLS_CB                         ((NBB_LONG)(PCT_SPM | 0x00000600))
#define MEM_SPM_VPLS_BASIC_CB                   ((NBB_LONG)(PCT_SPM | 0x00000601))
#define MEM_SPM_VPLS_NNI_CB                     ((NBB_LONG)(PCT_SPM | 0x00000602))
#define MEM_SPM_VPLS_UNI_CB                     ((NBB_LONG)(PCT_SPM | 0x00000603))

#define MEM_SPM_ARP_CB                          ((NBB_LONG)(PCT_SPM | 0x00000D00))
#define MEM_SPM_ARP_BASIC_CB                    ((NBB_LONG)(PCT_SPM | 0x00000D01))

#define MEM_SPM_NCACHE_BASIC_CB                 ((NBB_LONG)(PCT_SPM | 0x00000E00))
#define MEM_SPM_NCACHE_CB                       ((NBB_LONG)(PCT_SPM | 0x00000E01))

#define MEM_SPM_PORT_MIRROR_CB                  ((NBB_LONG)(PCT_SPM | 0x00001200))
#define MEM_SPM_PORT_MIRROR_BASIC_CB            ((NBB_LONG)(PCT_SPM | 0x00001201))

#define MEM_SPM_CIR_EMU_CB                         ((NBB_LONG)(PCT_SPM | 0x00001500))
#define MEM_SPM_CIR_EMU_PUB_LIMIT_CB                   ((NBB_LONG)(PCT_SPM | 0x00001501))
#define MEM_SPM_CIR_EMU_SYN_CLK_CB                     ((NBB_LONG)(PCT_SPM | 0x00001502))
#define MEM_SPM_CIR_EMU_ELTRC_BOARD_CB                     ((NBB_LONG)(PCT_SPM | 0x00001503))
#define MEM_SPM_CIR_EMU_REBTH_LIMIT_CB                  ((NBB_LONG)(PCT_SPM | 0x00001504))
#define MEM_SPM_CIR_EMU_MU_LIMIT_CB                     ((NBB_LONG)(PCT_SPM | 0x00001505))
#define MEM_SPM_CIR_EMU_HIGH_ORD_PASS_CB                     ((NBB_LONG)(PCT_SPM | 0x00001506))
#define MEM_SPM_CIR_EMU_LOW_ORD_PASS_CB                  ((NBB_LONG)(PCT_SPM | 0x00001507))

#define MEM_SPM_VPLS_MC_CB                      ((NBB_LONG)(PCT_SPM | 0x00001D00))
#define MEM_SPM_VPLS_MC_BASIC_CB                ((NBB_LONG)(PCT_SPM | 0x00001D01))
#define MEM_SPM_VPLS_MC_NNI_CB                  ((NBB_LONG)(PCT_SPM | 0x00001D02))
#define MEM_SPM_VPLS_MC_UNI_CB                  ((NBB_LONG)(PCT_SPM | 0x00001D03))

#define MEM_SPM_VPLS_UC_CB                      ((NBB_LONG)(PCT_SPM | 0x00001E00))
#define MEM_SPM_VPLS_UC_BASIC_CB                ((NBB_LONG)(PCT_SPM | 0x00001E01))

#define MEM_SPM_SWITCH_VC_CB                    ((NBB_LONG)(PCT_SPM | 0x00002B00))
#define MEM_SPM_SWITCH_VC_VC1_CB                ((NBB_LONG)(PCT_SPM | 0x00002B01))
#define MEM_SPM_SWITCH_VC_VC2_CB                ((NBB_LONG)(PCT_SPM | 0x00002B02))

#define MEM_SPM_BYPASS_CB                       ((NBB_LONG)(PCT_SPM | 0x00002F00))
#define MEM_SPM_BYPASS_BASIC_CB                 ((NBB_LONG)(PCT_SPM | 0x00002F01))

#define MEM_SPM_MSP_CB                          ((NBB_LONG)(PCT_SPM | 0x00003500))
#define MEM_SPM_MSP_BASIC_CB                    ((NBB_LONG)(PCT_SPM | 0x00003501))

#define MEM_SPM_OAM_CFG_CB                      ((NBB_LONG)(PCT_SPM | 0x00000036))

#define MEM_SPM_ASSOCIATE_IF_CB                     ((NBB_LONG)(PCT_SPM | 0x00001800))
#define MEM_SPM_ASSOCIATE_BASIC_CB                  ((NBB_LONG)(PCT_SPM | 0x00001801))
#define MEM_SPM_ASSOCIATE_TRIGGER_IF_CB             ((NBB_LONG)(PCT_SPM | 0x00001802))
#define MEM_SPM_ASSOCIATE_ACT_IF_CB                 ((NBB_LONG)(PCT_SPM | 0x00001803))

#define MEM_SPM_VPLS_L3_MC_CFG                  ((NBB_LONG)(PCT_SPM | 0x00004C00))
#define MEM_SPM_VPLS_L3_MC_BASIC_CFG            ((NBB_LONG)(PCT_SPM | 0x00004C01))
#define MEM_SPM_VPLS_L3_MC_NNI_CFG              ((NBB_LONG)(PCT_SPM | 0x00004C02))
#define MEM_SPM_VPLS_L3_MC_UNI_CFG              ((NBB_LONG)(PCT_SPM | 0x00004C03))

#if 1 /* BFD配置块内存宏 */
#define MEM_SPM_OAM_TREE_CB                     ((NBB_LONG)(PCT_SPM | 0x00001301))
#define MEM_SPM_BFD_PUL_TREE_BASE_CB            ((NBB_LONG)(PCT_SPM | 0x00001302))
#define MEM_SPM_BFD_PUC_SYN_ID_INFO_CB          ((NBB_LONG)(PCT_SPM | 0x00001303))
#define MEM_SPM_BFD_PUC_ALARM_INFO_CB           ((NBB_LONG)(PCT_SPM | 0x00001304))
#define MEM_SPM_BFD_PUL_FWD_TREE_BASE_CB        ((NBB_LONG)(PCT_SPM | 0x00001305))
#define MEM_SPM_BFD_GLOBAL_CB                   ((NBB_LONG)(PCT_SPM | 0x00001306))
#define MEM_SPM_BFD_FWD_GLOBAL_CB               ((NBB_LONG)(PCT_SPM | 0x00001307))
#define MEM_SPM_BFD_IP_GLOBAL_CB                ((NBB_LONG)(PCT_SPM | 0x00001308))
#define MEM_SPM_BFD_IPV6_GLOBAL_CB              ((NBB_LONG)(PCT_SPM | 0x00001309))
//#define MEM_SPM_BFD_BASIC_DATA_CB               ((NBB_LONG)(PCT_SPM | 0x0000130A))
#define MEM_SPM_BFD_LDP_LSP_DATA_CB             ((NBB_LONG)(PCT_SPM | 0x0000130B))
#define MEM_SPM_BFD_PW_DATA_CB                  ((NBB_LONG)(PCT_SPM | 0x0000130C))
#define MEM_SPM_BFD_RSVP_LSP_DATA_CB            ((NBB_LONG)(PCT_SPM | 0x0000130D))
#define MEM_SPM_BFD_ALARM_FAIL_TREE_CB          ((NBB_LONG)(PCT_SPM | 0x0000130E))
#define MEM_SPM_BFD_PUL_NNI_TREE_BASE_CB        ((NBB_LONG)(PCT_SPM | 0x0000130F))
#endif

#if 1
/*QOS配置块内存宏*/
#define MEM_SPM_DEFEND_POLICY_CB               ((NBB_LONG)(PCT_SPM | 0x00003100))
#define MEM_SPM_DEFEND_APPERC_CB               ((NBB_LONG)(PCT_SPM | 0x000031004))
#define MEM_SPM_DEFEND_USR_DEF_CB              ((NBB_LONG)(PCT_SPM | 0x00003105))

#define MEM_SPM_DEFEND_CB               ((NBB_LONG)(PCT_SPM | 0x00003000))

#define MEM_SPM_HQOS_CB                     ((NBB_LONG)(PCT_SPM | 0x00001100))
#define MEM_SPM_HQOS_SLOT_CB                     ((NBB_LONG)(PCT_SPM | 0x00001102))

#define MEM_SPM_WRED_CB                     ((NBB_LONG)(PCT_SPM | 0x00001f00))
#define MEM_SPM_WRED_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00001f01))

#define MEM_SPM_TWAMP_IPV4_CB               ((NBB_LONG)(PCT_SPM | 0x00003200))
#define MEM_SPM_TWAMP_IPV4_BASIC_CB         ((NBB_LONG)(PCT_SPM | 0x00003201))
#define MEM_SPM_TWAMP_IPV6_CB               ((NBB_LONG)(PCT_SPM | 0x00003300))
#define MEM_SPM_TWAMP_IPV6_BASIC_CB         ((NBB_LONG)(PCT_SPM | 0x00003301))


#define MEM_SPM_DS_DOMAIN_CB                     ((NBB_LONG)(PCT_SPM | 0x00002100))
#define MEM_SPM_DS_DOMAIN_D2P_CB                     ((NBB_LONG)(PCT_SPM | 0x00002101))
#define MEM_SPM_DS_DOMAIN_TC2P_CB                     ((NBB_LONG)(PCT_SPM | 0x00002102))
#define MEM_SPM_DS_DOMAIN_VLAN2P_CB                     ((NBB_LONG)(PCT_SPM | 0x00002103))
#define MEM_SPM_DS_DOMAIN_EXP2P_CB                     ((NBB_LONG)(PCT_SPM | 0x00002104))
#define MEM_SPM_DS_DOMAIN_P2D_CB                     ((NBB_LONG)(PCT_SPM | 0x00002105))
#define MEM_SPM_DS_DOMAIN_P2TC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002106))
#define MEM_SPM_DS_DOMAIN_P2VLAN_CB                     ((NBB_LONG)(PCT_SPM | 0x00002107))
#define MEM_SPM_DS_DOMAIN_P2EXP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002108))

#define MEM_SPM_DS_CB                             ((NBB_LONG)(PCT_SPM | 0x00002000))
#define MEM_SPM_DS_PRI2PHB_CB                     ((NBB_LONG)(PCT_SPM | 0x00002001))
#define MEM_SPM_DS_PHB2PRI_CB                     ((NBB_LONG)(PCT_SPM | 0x00002002))

#define MEM_SPM_ACL_CB                           ((NBB_LONG)(PCT_SPM | 0x00002300))
#define MEM_SPM_ACL_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002301))
#define MEM_SPM_ACL_PORT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002302))
#define MEM_SPM_ACL_ETH_CB                     ((NBB_LONG)(PCT_SPM | 0x00002303))
#define MEM_SPM_ACL_IPV4_CB                     ((NBB_LONG)(PCT_SPM | 0x00002304))
#define MEM_SPM_ACL_IPTCP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002305))
#define MEM_SPM_ACL_IPV6_CB                     ((NBB_LONG)(PCT_SPM | 0x00002306))
#define MEM_SPM_ACL_NOIP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002307))
#define MEM_SPM_ACL_IPUDP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002308))

#define MEM_SPM_POLICY_CB                     ((NBB_LONG)(PCT_SPM | 0x00002400))
#define MEM_SPM_POLICY_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002401))
#define MEM_SPM_POLICY_C2B_CB                     ((NBB_LONG)(PCT_SPM | 0x00002402))

#define MEM_SPM_CLASSIFY_CB                     ((NBB_LONG)(PCT_SPM | 0x00002500))
#define MEM_SPM_CLASSIFY_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002501))
#define MEM_SPM_CLASSIFY_PORT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002502))
#define MEM_SPM_CLASSIFY_ETH_CB                     ((NBB_LONG)(PCT_SPM | 0x00002503))
#define MEM_SPM_CLASSIFY_IPV4_CB                     ((NBB_LONG)(PCT_SPM | 0x00002504))
#define MEM_SPM_CLASSIFY_IPTCP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002505))
#define MEM_SPM_CLASSIFY_IPV6_CB                     ((NBB_LONG)(PCT_SPM | 0x00002506))
#define MEM_SPM_CLASSIFY_NOIP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002507))
#define MEM_SPM_CLASSIFY_IPUDP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002508))
#define MEM_SPM_CLASSIFY_POLICY_CB                     ((NBB_LONG)(PCT_SPM | 0x00002509))

#define MEM_SPM_ACTION_CB                     ((NBB_LONG)(PCT_SPM | 0x00002600))
#define MEM_SPM_ACTION_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002601))
#define MEM_SPM_ACTION_SHAPING_CB                     ((NBB_LONG)(PCT_SPM | 0x00002602))
#define MEM_SPM_ACTION_SUPERVISE_CB                     ((NBB_LONG)(PCT_SPM | 0x00002603))
#define MEM_SPM_ACTION_SCHEDULE_CB                     ((NBB_LONG)(PCT_SPM | 0x00002604))
#define MEM_SPM_ACTION_FLOW_ACT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002606))
#define MEM_SPM_ACTION_FLOW_SAMPL_CB                     ((NBB_LONG)(PCT_SPM | 0x00002607))
#define MEM_SPM_ACTION_REDIRECT_CB                     ((NBB_LONG)(PCT_SPM | 0x000026008))
#define MEM_SPM_ACTION_POLCY_RT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002609))
#define MEM_SPM_ACTION_PHB_CB                     ((NBB_LONG)(PCT_SPM | 0x0000260a))
#define MEM_SPM_ACTION_DOMAIN_CB                     ((NBB_LONG)(PCT_SPM | 0x0000260b))
#define MEM_SPM_ACTION_URPF_CB                     ((NBB_LONG)(PCT_SPM | 0x0000260c))
#define MEM_SPM_ACTION_POLICY_SUPERVISE_CB         ((NBB_LONG)(PCT_SPM | 0x0000260e))

#define MEM_SPM_USER_GROUP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002700))
#define MEM_SPM_BASIC_USER_GROUP_CB                ((NBB_LONG)(PCT_SPM | 0x00002701))
#define MEM_SPM_UP_USER_GROUP_CB                ((NBB_LONG)(PCT_SPM | 0x00002702))
#define MEM_SPM_DOWN_USER_GROUP_CB                ((NBB_LONG)(PCT_SPM | 0x00002703))



/*QOS子定义内存宏*/
#define MEM_SPM_PORT_WRED_CB                     ((NBB_LONG)(PCT_SPM | 0x00001110))

#define MEM_SPM_QOS_DS_LOGIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00001111))
#define MEM_SPM_QOS_DS_LOGIC_FLOW_CB                 ((NBB_LONG)(PCT_SPM | 0x00001112))
#define MEM_SPM_QOS_DS_VC_CB                     ((NBB_LONG)(PCT_SPM | 0x00001113))
#define MEM_SPM_QOS_DS_VRF_CB                     ((NBB_LONG)(PCT_SPM | 0x0001114))
#define MEM_SPM_QOS_DS_ILM_CB                     ((NBB_LONG)(PCT_SPM | 0x00001115))
#define MEM_SPM_QOS_DS_FTN_CB                     ((NBB_LONG)(PCT_SPM | 0x00001116))
#define MEM_SPM_QOS_DS_RX_LSP_CB                     ((NBB_LONG)(PCT_SPM | 0x00001117))
#define MEM_SPM_QOS_DS_TX_LSP_CB                     ((NBB_LONG)(PCT_SPM | 0x00001118))
#define MEM_SPM_QOS_DS_TUNNEL_CB                     ((NBB_LONG)(PCT_SPM | 0x00001119))

#define MEM_SPM_QOS_ACL_LOGIC_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111A))
#define MEM_SPM_QOS_FLOW_CLASSIFY_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111B))
#define MEM_SPM_QOS_UP_USR_CB                       ((NBB_LONG)(PCT_SPM | 0x0000111C))

#define MEM_SPM_HQOS_LSP_TX_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111D))
#define MEM_SPM_HQOS_VRF_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111E))
#define MEM_SPM_HQOS_VC_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111F))
#define MEM_SPM_HQOS_LOG_GROUP_CB                     ((NBB_LONG)(PCT_SPM | 0x00001120))
#define MEM_SPM_HQOS_LOG_USR_CB                     ((NBB_LONG)(PCT_SPM | 0x00001121))
#define MEM_SPM_QOS_FLOW_UP_USR_CB                  ((NBB_LONG)(PCT_SPM | 0x00001122))
#define MEM_SPM_QOS_DS_LOGIC_UNIVP_CB          ((NBB_LONG)(PCT_SPM | 0x00001123))
#define MEM_SPM_QOS_TXLSP_CAR_CB               ((NBB_LONG)(PCT_SPM | 0x00001124))
#define MEM_SPM_QOS_PW_CAR_CB                  ((NBB_LONG)(PCT_SPM | 0x00001125))
#define MEM_SPM_QOS_INTF_PW_CB                 ((NBB_LONG)(PCT_SPM | 0x00001126))
#endif
#if 1
/*NF配置块内存宏*/
#define MEM_SPM_IP_NF_CB                  ((NBB_LONG)(PCT_SPM | 0x00002C00))
#define MEM_SPM_IP_NF_IPV4_DSTHOST_CB     ((NBB_LONG)(PCT_SPM | 0x00002C01))
#define MEM_SPM_IP_NF_IPV4_SRCIP_CB       ((NBB_LONG)(PCT_SPM | 0x00002C02))
#define MEM_SPM_IP_NF_IPV4_VERSION_CB     ((NBB_LONG)(PCT_SPM | 0x00002C03))
#define MEM_SPM_IP_NF_IPV6_DSTHOST_CB     ((NBB_LONG)(PCT_SPM | 0x00002C04))
#define MEM_SPM_IP_NF_IPV6_SRCIP_CB       ((NBB_LONG)(PCT_SPM | 0x00002C05))
#define MEM_SPM_IP_NF_IPV6_VERSION_CB     ((NBB_LONG)(PCT_SPM | 0x00002C06))
#define MEM_SPM_IP_NF_CHECK_TCP_CB        ((NBB_LONG)(PCT_SPM | 0x00002C07))
#define MEM_SPM_IP_NF_TIMEOUT_CB          ((NBB_LONG)(PCT_SPM | 0x00002C08))
#define MEM_SPM_IP_NF_START_CB            ((NBB_LONG)(PCT_SPM | 0x00002C09))
#define MEM_SPM_IP_NF_EXPORT_INFO_CB      ((NBB_LONG)(PCT_SPM| 0x00002C0A))
#define MEM_SPM_IP_NF_ENABLE_CB            ((NBB_LONG)(PCT_SPM | 0x00002C0B))
#define MEM_SPM_IP_NF_FLEXNFKEY_CB      ((NBB_LONG)(PCT_SPM| 0x00002C0C))
#define MEM_SPM_NF_AGGR_CB                ((NBB_LONG)(PCT_SPM | 0x00002D00))
#define MEM_SPM_NF_AGGR_AGGREGATION_CB    ((NBB_LONG)(PCT_SPM | 0x00002D01))
#define MEM_SPM_NF_AGGR_EXPORT_VERSION_CB    ((NBB_LONG)(PCT_SPM | 0x00002D02))
#define MEM_SPM_NF_AGGR_IPV4_MASK_CB           ((NBB_LONG)(PCT_SPM | 0x00002D03))
#define MEM_SPM_NF_AGGR_IPV6_MASK_CB           ((NBB_LONG)(PCT_SPM | 0x00002D04))
#define MEM_SPM_NF_AGGR_ENABLE_CB         ((NBB_LONG)(PCT_SPM | 0x00002D05))

#endif

/*****************************************************************************/
/* 定义句柄类型，包括所有组件用到的句柄。                                    */
/*****************************************************************************/
#define HDL_SPM_DCI_JOIN_CB                     ((NBB_LONG)(PCT_SPM | 0x00000001))
#define HDL_SPM_SBI_JOIN_CB                     ((NBB_LONG)(PCT_SPM | 0x00000002))
#define HDL_SPM_PHYSICAL_PORT_CB                ((NBB_LONG)(PCT_SPM | 0x00000003))
#define HDL_SPM_LOGICAL_PORT_CB                 ((NBB_LONG)(PCT_SPM | 0x00000004))
#define HDL_SPM_VC_CB                           ((NBB_LONG)(PCT_SPM | 0x00000005))
#define HDL_SPM_VPWS_CB                         ((NBB_LONG)(PCT_SPM | 0x00000006))
#define HDL_SPM_VPLS_CB                         ((NBB_LONG)(PCT_SPM | 0x00000007))
#define HDL_SPM_IPV4_NODE_CB                    ((NBB_LONG)(PCT_SPM | 0x00000008))
#define HDL_SPM_IPV6_NODE_CB                    ((NBB_LONG)(PCT_SPM | 0x00000009))
#define HDL_SPM_FLOW_NODE_CB                    ((NBB_LONG)(PCT_SPM | 0x0000000A))
#define HDL_SPM_MC_IPV4_NODE_CB                 ((NBB_LONG)(PCT_SPM | 0x0000000B))
#define HDL_SPM_VIP_VMAC_NODE_CB                ((NBB_LONG)(PCT_SPM | 0x0000000C))
#define HDL_SPM_ARP_CB                          ((NBB_LONG)(PCT_SPM | 0x0000000D))
#define HDL_SPM_LAG_CB                          ((NBB_LONG)(PCT_SPM | 0x0000000E))

#define HDL_SPM_SOCKET_JOIN_CB                  ((NBB_LONG)(PCT_SPM | 0x00000010))
#define HDL_SPM_SOCKET_CB                       ((NBB_LONG)(PCT_SPM | 0x00000011))
#define HDL_SPM_MTL_JOIN_CB                    ((NBB_LONG)(PCT_SPM | 0x00000012))

#define HDL_SPM_VPLS_MC_CB                      ((NBB_LONG)(PCT_SPM | 0x00000013))
#define HDL_SPM_PORT_MIRROR_CB                  ((NBB_LONG)(PCT_SPM | 0x00000014))
#define HDL_SPM_SWITCH_VC_CB                    ((NBB_LONG)(PCT_SPM | 0x00000015))
#define HDL_SPM_NCACHE_CB                       ((NBB_LONG)(PCT_SPM | 0x00000016))
#define HDL_SPM_VPLS_UC_CB                      ((NBB_LONG)(PCT_SPM | 0x00000017))
#define HDL_SPM_BYPASS_CB                       ((NBB_LONG)(PCT_SPM | 0x00000018))
#define HDL_SPM_TERMINAL_IF_CB                  ((NBB_LONG)(PCT_SPM | 0x00000019))
#define HDL_SPM_ASSOCIATE_IF_CB                 ((NBB_LONG)(PCT_SPM | 0x0000001A))
#define HDL_SPM_MSP_CB                          ((NBB_LONG)(PCT_SPM | 0x0000001B))
#define HDL_SPM_VPLS_L3_MC_CB                   ((NBB_LONG)(PCT_SPM | 0x00000025))
#define HDL_SPM_VPLS_NNI_CB                     ((NBB_LONG)(PCT_SPM | 0x00000026))
#define HDL_SPM_VPLS_UNI_CB                     ((NBB_LONG)(PCT_SPM | 0x00000027))



/*****************************************************************************/
/* Pointer to local data - used for debugging.                               */
/*****************************************************************************/
extern SPM_SHARED_LOCAL *v_spm_shared;
#ifdef SPM_DEFINE_VARS
SPM_SHARED_LOCAL *v_spm_shared = NULL;
#endif

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_NODE_CB                                                    */
/*                                                                           */
/* Description: 流节点配置。                                                     */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                             */
/*****************************************************************************/
typedef struct spm_flow_node_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE flow_tree_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE flow_tree_handle;

    /***************************************************************************/
    /* key值：流ID。                                                           */
    /***************************************************************************/
    NBB_USHORT flow_id_key;

    /***************************************************************************/
    /* 流配置。                                                                */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_FLOW_DATA flow_cfg_cb;

} SPM_FLOW_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_IPV4_NODE_CB                                                    */
/*                                                                           */
/* Description: ipv4点配置。                                                     */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                             */
/*****************************************************************************/
typedef struct spm_ipv4_node_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE ipv4_tree_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE ipv4_tree_handle;

    /***************************************************************************/
    /* key值：ip地址。                                                         */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV4_DATA ipv4_key;

    /***************************************************************************/
    /* Ipv4地址配置L3。                                                        */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV4_DATA ipv4_cfg_cb;

} SPM_IPV4_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_IPV6_NODE_CB                                                    */
/*                                                                           */
/* Description: ipv6点配置。                                                     */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                             */
/*****************************************************************************/
typedef struct spm_ipv6_node_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE ipv6_tree_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE ipv6_tree_handle;

    /***************************************************************************/
    /* key值：ip地址。                                                         */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV6_DATA ipv6_key;

    /***************************************************************************/
    /* Ipv4地址配置L3。                                                        */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV6_DATA ipv6_cfg_cb;

} SPM_IPV6_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MC_MAC_NODE_CB                                                    */
/*                                                                           */
/* Description: 组播组地址L3。                                                     */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                             */
/*****************************************************************************/
typedef struct spm_mc_mac_node_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE mc_mac_tree_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE mc_mac_tree_handle;

    /***************************************************************************/
    /* key值：mac地址。                                                        */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_MC_MAC_DATA mac_key;

    /***************************************************************************/
    /* Ipv4地址配置L3。                                                        */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_MC_MAC_DATA mc_mac_cfg_cb;

} SPM_MC_MAC_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VIP_VMAC_NODE_CB                                           */
/*                                                                           */
/* Description: 虚拟MAC和IP地址配置L3。                                      */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                            */
/*****************************************************************************/
typedef struct spm_vip_vmac_node_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE vip_vmac_tree_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE vip_vmac_tree_handle;

    /***************************************************************************/
    /* key值：ip+mac地址。                                                     */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA ip_mac_key;

    /***************************************************************************/
    /* Ipv4地址配置L3。                                                        */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA vip_vmac_cfg_cb;

} SPM_VIP_VMAC_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_INFO_CB                                           */
/*                                                                           */
/* Description: 流的驱动返回值和是否配置了QoS等信息                       */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                            */
/*****************************************************************************/
typedef struct spm_port_drv_info_cb
{
    /***************************************************************************/
    /* 芯片物理口.        */
    /***************************************************************************/
    NBB_INT port_phy_id;

    /***************************************************************************/
    /* 槽位号.        */
    /***************************************************************************/
    NBB_INT slot_id;

    /***************************************************************************/
    /* 端口MAC.        */
    /***************************************************************************/
    NBB_BYTE mac[ATG_DCI_MAC_LEN];

    /***************************************************************************/
    /* 端口TPID.        */
    /***************************************************************************/
    NBB_INT tpid;

    /***************************************************************************/
    /* 端口Link状态.        */
    /***************************************************************************/
    NBB_INT link_state;

    /***************************************************************************/
    /* 端口开/关状态.        */
    /***************************************************************************/
    NBB_INT on_off_en;

    /***************************************************************************/
    /* 端口的入口速控.        */
    /***************************************************************************/
    NBB_INT ingress_rc_kbps;

    /***************************************************************************/
    /* 端口的出口速控.        */
    /***************************************************************************/
    NBB_INT egress_rc_kbps;

    /***************************************************************************/
    /* 端口速率.        */
    /***************************************************************************/
    NBB_INT speed;

    /***************************************************************************/
    /* 端口自协商使能.        */
    /***************************************************************************/
    NBB_INT nego_en;

    /***************************************************************************/
    /* 端口双工.        */
    /***************************************************************************/
    NBB_INT duplex;

    /***************************************************************************/
    /* 端口PHY LOOPBACK.        */
    /***************************************************************************/
    NBB_INT phy_local_lb;
    NBB_INT phy_remote_lb;

    /***************************************************************************/
    /* 端口MAC LOOPBACK.        */
    /***************************************************************************/
    NBB_INT mac_local_lb;
    NBB_INT mac_remote_lb;
    
} SPM_PORT_DRV_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_INFO_CB                                           */
/*                                                                           */
/* Description: 流的驱动返回值和是否配置了QoS等信息                       */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                            */
/*****************************************************************************/
typedef struct spm_flow_info_cb
{
    /***************************************************************************/
    /* 驱动返回的INTF pos_id, 如果为L2业务指流INTF.        */
    /***************************************************************************/
    NBB_USHORT pos_id;

    /***************************************************************************/
    /* 是否配置了QoS.        */
    /***************************************************************************/
    NBB_BYTE if_qos;

} SPM_FLOW_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_MC_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPLS组播保存的一些配置                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/2/5                                            */
/*****************************************************************************/
typedef struct spm_vpls_mc_port_info_cb
{
    /***************************************************************************/
    /* 组播组里面NNI/UNI的驱动pos_id.        */
    /***************************************************************************/
    NBB_ULONG vp_pos_id;

    /***************************************************************************/
    /* 组播组里面为NNI/UNI分配的cud.        */
    /***************************************************************************/
    //NBB_INT cud;

    /***************************************************************************/
    /* 组播组里面NNI/UNI所在的槽位号.        */
    /***************************************************************************/
    NBB_BYTE slot_id;
} SPM_VPLS_MC_PORT_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPN_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPN保存的一些组播配置和Bypass需要的配置                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/14                                            */
/*****************************************************************************/
typedef struct spm_phy_port_info_cb
{
    /*************************************************************************/
    /* 接口类型:0/1/2/3/4/5/6=面板以太网接口/仿真控制口/POS/CPOS/E-CPOS/仿真接口/同步串口。 */
    /*************************************************************************/
    NBB_BYTE port_type;                  /* One of: ATG_DCI_INTERFACE_TYPE;  */

    /*************************************************************************/
    /* 槽位号=1-10。                                                         */
    /*************************************************************************/
    NBB_BYTE slot;

    /*************************************************************************/
    /* 子卡号：0/1/2=无子卡/子卡1/子卡2。                                    */
    /*************************************************************************/
    NBB_BYTE sub_board;

    /*************************************************************************/
    /* 端口号(在本卡上从1开始排序)(对于直连物理接口该字段有效，从属物理接口无效)。 */
    /*************************************************************************/
    NBB_BYTE port;

} SPM_PHY_PORT_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPN_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPN保存的一些组播配置和Bypass需要的配置                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/14                                            */
/*****************************************************************************/
typedef struct spm_vpn_port_info_cb
{
    /***************************************************************************/
    /* VP的pos id.                                       */
    /***************************************************************************/
    NBB_ULONG vp_idx;
    
    /***************************************************************************/
    /* 组播组里面NNI/UNI的驱动pos_id.        */
    /***************************************************************************/
    //NBB_ULONG vp_mc_pos_id;

    /***************************************************************************/
    /* 为SWITCH_VC两段VC分配的next_hop_id.        */
    /* 也用作VPWS/VPLS的next_hop_id.    fansongbo 2015.9.17 解决mantis 84426.        */
    /***************************************************************************/
    NBB_UINT next_hop_id;

    /***************************************************************************/
    /* 组播组里面NNI/UNI所在的槽位号.        */
    /***************************************************************************/
    NBB_BYTE slot_id;

    /***************************************************************************/
    /* 组播组里面NNI/UNI的端口类型.        */
    /***************************************************************************/
    NBB_BYTE port_type;

    /***************************************************************************/
    /* C3里面NNI/UNI所对应的PORT_INDEX.        */
    /***************************************************************************/
    NBB_ULONG port_index;
    
    /***************************************************************************/
    /* 组播组里所有的出口index. 例如: ECMP最多8个出口；FRR有主备2个出口       */
    /***************************************************************************/
    NBB_ULONG port_index_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
    /* 组播组里面所有NNI/UNI所在的槽位号.        */
    /***************************************************************************/
    NBB_BYTE slot_id_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
    /* 组播组里面所有NNI/UNI的端口类型.        */
    /***************************************************************************/
    NBB_BYTE port_type_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
   /* 组播组里面所有NNI/UNI的端口.        */
    /***************************************************************************/
    NBB_USHORT port_id_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
    /* 组播组里所有NNI出口index的个数       */
    /***************************************************************************/
    NBB_BYTE port_num;
    
    /***************************************************************************/
    /* 保护组id                                       */
    /***************************************************************************/
    NBB_USHORT frr_group_id;
    
    /***************************************************************************/
    /* 用作VPWS/VPLS的next_hop_id_p.    fansongbo 2015.9.17 解决mantis 84426.        */
    /***************************************************************************/
    NBB_UINT next_hop_id_p; 
	NBB_UINT next_hop_counter_id;
	NBB_UINT next_hop_counter_id_p;
	
} SPM_VPN_PORT_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_VC_INFO_CB                                           */
/*                                                                           */
/* Description: VC维护的一些配置，包括驱动返回                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/2/5                                            */
/*****************************************************************************/
typedef struct spm_vc_info_cb
{
    /***************************************************************************/
    /* VPN类型：1/2=VPWS/VPLS.                                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /* VPN ID：VPWS=[1-16K] VPLS=[1-4K].                                       */
    /***************************************************************************/
    NBB_USHORT vpn_id;

    /***************************************************************************/
    /* 指明主用备用，主/备=1/2.                                       */
    /***************************************************************************/
    NBB_USHORT if_main;

    /***************************************************************************/
    /* VC所在VP的pos id.                                       */
    /***************************************************************************/
    NBB_ULONG vp_idx;

    /***************************************************************************/
    /*  VP_TABLE index.                                       */
    /***************************************************************************/
    //NBB_ULONG vp_api_idx;

    /***************************************************************************/
    /*  port_id.                                       */
    /***************************************************************************/
    NBB_USHORT port_id;

    /***************************************************************************/
    /*  slot_id.                                       */
    /***************************************************************************/
    NBB_BYTE slot_id;
	
    /***************************************************************************/
    /*  逻辑端口index.                                       */
    /***************************************************************************/
    NBB_ULONG port_index;

    /***************************************************************************/
    /*  是否配置了QoS.                                       */
    /***************************************************************************/
    NBB_BYTE if_qos;

    /***************************************************************************/
    /*  VC对应的下一跳ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_id;

    /***************************************************************************/
    /*  VC对应的下一跳计数器ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_counter_id;

    NBB_USHORT frr_group_id;
    
    NBB_ULONG drv_tunnel_id;
    
    /*************************************************************************/
    /* LSP选择：0/1=FTN/CR-LSP.                                              */
    /*************************************************************************/
    NBB_BYTE lsp_option;                        /* One of: ATG_DCI_LSP_TYPE; */ 
    
} SPM_VC_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_LOGICAL_PORT_INFO_CB                                       */
/*                                                                           */
/* Description: 逻辑端口维护的一些配置，包括驱动返回                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/13                                            */
/*****************************************************************************/
typedef struct spm_logical_port_info_cb
{

    /***************************************************************************/
    /* 驱动返回的VP pos_id，如果为0说明此端口未被加入到某VPWS/VPLS.            */
    /***************************************************************************/
    NBB_ULONG vp_idx;

    /***************************************************************************/
    /* vp_idx. 微码的index                                      */
    /***************************************************************************/
    //NBB_ULONG vp_api_idx;

    /***************************************************************************/
    /* VPN类型：1/2=VPWS/VPLS.                                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /* VPN ID：VPWS=[1-16K] VPLS=[1-4K].                                       */
    /***************************************************************************/
    NBB_USHORT vpn_id;

    /***************************************************************************/
    /* 驱动返回的INTF pos_id, 如果为L3指三层INTF.        */
    /***************************************************************************/
    NBB_USHORT intf_pos_id;

    /***************************************************************************/
    /*  UNI对应的下一跳ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_id;

    /***************************************************************************/
    /*  MSP 保护时UNI对应的备用下一跳ID.                                       */
    /***************************************************************************/
    NBB_ULONG p_next_hop_id;    

    /***************************************************************************/
    /*  UNI对应的下一跳counter ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_counter_id;

    /***************************************************************************/
    /* 流的驱动返回值和是否配置了QoS等信息.        */
    /***************************************************************************/
    SPM_FLOW_INFO_CB flow_info_cb;
	
    /*  VRRP的MAC对应的pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG ulVrrpMacPosId[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /*  VRRP ipv6的MAC对应的pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG vrrp_ipv6_posid[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];    

    /*  VRRP的MAC对应的pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG vrrp_ipv4_posid2[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /*  VRRP ipv6的MAC对应的pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG vrrp_ipv6_posid2[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];     
} SPM_LOGICAL_PORT_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: PORT属性                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_port_info_cb
{
    /***************************************************************************/
    /*  C3芯片号.                                       */
    /***************************************************************************/
    NBB_INT unit_id;
    
    /***************************************************************************/
    /* 端口类型：0/1/2/3=接口/子接口/LAG/虚拟以太网口.                         */
    /***************************************************************************/
    NBB_BYTE port_type;

    /***************************************************************************/
    /*  port_id.                                       */
    /***************************************************************************/
    NBB_USHORT port_id;
    
    /***************************************************************************/
    /*  物理端口号.                                       */
    /***************************************************************************/
    NBB_BYTE phy_port;
    
    /***************************************************************************/
    /*  card_id.                                       */
    /***************************************************************************/
    NBB_USHORT card_id;
    /***************************************************************************/
    /*  slot_id.                                       */
    /***************************************************************************/
    NBB_BYTE slot_id;

    /***************************************************************************/
    /*  记录LAG口第一个成员端口的槽位，用于组播时FE1600增删槽位.               */
    /***************************************************************************/
    NBB_BYTE lag_slot_id;

    NBB_USHORT lag_port_id;

    /***************************************************************************/
    /*  VE口的槽位.                                       */
    /***************************************************************************/
    NBB_BYTE ve_slot_id;
    
    /***************************************************************************/
    /*  s-vlan.                                       */
    /***************************************************************************/
    NBB_USHORT svlan;

    /***************************************************************************/
    /*  c-vlan.                                       */
    /***************************************************************************/
    NBB_USHORT cvlan;

    /***************************************************************************/
    /*  UNI VP posid.                                       */
    /***************************************************************************/
    NBB_ULONG vp_pos_id;

} SPM_PORT_INFO_CB;

/**STRUCT+********************************************************************/
/* Structure: SPM_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: PORT属性                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_vp_info_cb
{
    /***************************************************************************/
    /* 业务类型 1/2=VPWS/VPLS.                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /*  驱动返回的vp_api_idx.                                       */
    /***************************************************************************/
    NBB_USHORT vp_id;

    /***************************************************************************/
    /*  UNI 测的逻辑端口index.                                       */
    /***************************************************************************/
    NBB_ULONG uni_port_index;

    /***************************************************************************/
    /*  uni_slot.                                       */
    /***************************************************************************/
    NBB_BYTE uni_slot;

    /***************************************************************************/
    /*  uni_port_id.                                       */
    /***************************************************************************/
    NBB_USHORT uni_port_id;
    
    /***************************************************************************/
    /*  标签.                                       */
    /***************************************************************************/
    NBB_ULONG pw_rx_label;
    NBB_ULONG pw_tx_label;
    NBB_ULONG lsp_rx_label;
    NBB_ULONG lsp_tx_label;

    /***************************************************************************/
    /*  lsp类型: 1/2=tunnel/lsp.                                       */
    /***************************************************************************/
    NBB_BYTE lsp_type;

    /***************************************************************************/
    /*  VC对应的下一跳ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_id;
                                           
} SPM_VP_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VP_INFO_REFRESH_CB                                           */
/*                                                                           */
/* Description: PORT属性                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_vp_info_refresh_cb
{
    /***************************************************************************/
    /* 业务类型 1/2/4=VPWS/VPLS/MS_PW.                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /* 指明VC主用备用，主/备=1/2.                                       */
    /***************************************************************************/
    NBB_BYTE if_main;

    /***************************************************************************/
    /* VPN ID：VPWS=[1-16K] VPLS=[1-4K].                                       */
    /***************************************************************************/
    NBB_USHORT vpn_id;

    /***************************************************************************/
    /* VC的key值，里面包含peer_ip.                                       */
    /***************************************************************************/
    ATG_DCI_VC_KEY vc_key;

	/***************************************************************************/
    /* vc出口index.                                       */
    /***************************************************************************/
	NBB_ULONG port_index;

	/***************************************************************************/
    /* vpls中VP组播ID.                                       */
    /***************************************************************************/
	NBB_USHORT mc_id;
} SPM_VP_INFO_REFRESH_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MSP_PROT_INFO_CB                                       */
/*                                                                           */
/* Description: MSP保护主备端口的时隙信息，包括KEY值，是否保护               */
/*                                                                           */
/* Added by fansongbo , 2014/12/27                                            */
/*****************************************************************************/
typedef struct spm_msp_prot_info_cb
{

    /***************************************************************************/
    /* 是否有保护：0/1=无保护/有保护.                                         */
    /***************************************************************************/
    NBB_BYTE if_prot;

    /***************************************************************************/
    /* 2M时隙对应的物理接口配置块KEY值                                      */
    /***************************************************************************/
    NBB_ULONG ces_key;

} SPM_MSP_PROT_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VP_INFO_REFRESH_CB                                           */
/*                                                                           */
/* Description: PORT属性                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_vpws_nni_info_cb
{
    /***************************************************************************/
    /* VC的key值，里面包含peer_ip.                                       */
    /***************************************************************************/
    ATG_DCI_VC_KEY vc_key;

    /***************************************************************************/
    /*  逻辑端口index.                                       */
    /***************************************************************************/
    NBB_ULONG port_index;

    /***************************************************************************/
    /* vc对应的CRLSPTX的key值.                                       */
    /***************************************************************************/
    ATG_DCI_CR_LSP_KEY rsvpkey;

    NBB_USHORT frr_group_id;

    /***************************************************************************/
    /*NNI的VP_Idx.                                       */
    /***************************************************************************/
    NBB_ULONG vp_idx;
} SPM_VPWS_NNI_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PHYSICAL_PORT_CB                                           */
/*                                                                           */
/* Description: 接口物理配置。                                               */
/*                                                                           */
/* Added by xiaoxiang, 2012/9/11                                             */
/*****************************************************************************/
typedef struct spm_physical_port_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_physical_port_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_physical_port_handle;

    /* key值: index */
    NBB_ULONG port_index_key;

    /* 基本配置。*/
    ATG_DCI_PHY_PORT_BASIC_DATA *basic_cfg_cb;

    /* 以太网接口物理配置。*/
    ATG_DCI_PHY_PORT_PHY_DATA *phy_cfg_cb;

    /* POS接口链路层配置。 */
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pos_link_cfg_cb;

    /* SDH段开销配置。 */
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *sdh_sovhd_cfg_cb;

    /* CES接口配置。 */
    ATG_DCI_PHY_PORT_CES_DATA *ces_cfg_cb;

    /* CEP接口开销配置。 */
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *cep_ovhd_cfg_cb;

    /***************************************************************************/
    /* 以太网EFM OAM.                                                         */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_EFM_OAM_DATA *efm_oam_cfg_cb;

    /***************************************************************************/
    /* 时间同步端口配置.                                                       */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *time_sync_cfg_cb;

    /***************************************************************************/
    /* 以太网接口报文抑制.                                                     */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *msg_suppr_cfg_cb;

    /***************************************************************************/
    /* 以太网接口流量整形.                                                     */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_FLOW_SHAPE *flow_shape_cfg_cb;

    /***************************************************************************/
    /* 以太网出口队列调度策略.                                                 */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *schedule_policy_cfg_cb;

    /***************************************************************************/
    /* 以太网出口队列拥塞策略.                                                 */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_CONGEST_POLICY *congest_policy_cfg_cb;

    /***************************************************************************/
    /* IPV4流采样使能配置IPV4 NetFlow.                                         */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_IPV4NF_DATA *ipv4_nf_cfg_cb;

    /***************************************************************************/
    /* IPV4流采样镜像 IPV4 NetFlow sampler.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *ip_sample_cfg_cb;

    /***************************************************************************/
    /* IPV6流采样使能配置 IPV6 NetFlow.                                         */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_IPV6NF_DATA *ipv6_nf_cfg_cb;

    /***************************************************************************/
    /* 接口通用配置.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_COMMON_DATA *common_cfg_cb;

    /***************************************************************************/
    /* SDH通道开销配置.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *sdh_tovhd_cfg_cb;

    /***************************************************************************/
    /* ALS配置.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_ALS_DATA *als_cfg_cb;

    /***************************************************************************/
    /* 光功率门限配置.                                    */
    /***************************************************************************/
    //ATG_DCI_PHY_PORT_POWER_THRESHOLD *threshold_cfg_cb;    
    
} SPM_PHYSICAL_PORT_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: BMU_TERMINAL_IF_CB                                             */
/*                                                                           */
/* Description: 终结子接口配置块。                                           */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_terminal_if_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_terminal_if_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_terminal_if_handle;

    /* 终结子接口。 */
    ATG_DCI_LOG_PORT_VLAN terminal_if_cfg;

    /* 驱动返回值 ，非VE和LAG时可能为任意片C3的驱动返回值*/
    NBB_ULONG intf_pos_id;

      /* VE或者LAG时  第二片C3驱动返回值，其它情况下无此信息 */
    NBB_ULONG intf2_pos_id;  

} SPM_TERMINAL_IF_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_DIFF_SERV_CB                                             */
/*                                                                           */
/* Description:DIFF SERV配置块。                                           */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_diffl_serv_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_diff_serv_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    //NBB_HANDLE spm_diff_serv_handle;

    /* DIFF_SERV */
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA diff_serv_cfg;

    /* 驱动返回值 */
    //NBB_ULONG pos_id;

} SPM_DIFF_SERV_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_DIFF_SERV_CB                                             */
/*                                                                           */
/* Description:FLOW_DIFF SERV配置块。                                           */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_flow_diff_serv_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_flow_diff_serv_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    //NBB_HANDLE spm_diff_serv_handle;
     /* FLOW_DIFF_SERV  key*/
    //ATG_DCI_LOG_PORT_VLAN key;

    /* FLOW_DIFF_SERV */
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV flow_diff_serv_cfg;

    /* 驱动返回值 */
    //NBB_ULONG pos_id;

} SPM_FLOW_DIFF_SERV_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_INCLASSIFY_QOS_CB                                             */
/*                                                                           */
/* Description:INCLASSIFY_QOS配置块。                                           */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_inclassify_qos_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_inclassify_qos_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    //NBB_HANDLE spm_diff_serv_handle;
     /* FLOW_DIFF_SERV  key*/
    //ATG_DCI_LOG_PORT_VLAN key;
    
    /* INCLASSIFY_QOS */
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS inclassify_qos_cfg;

    /* 驱动返回值 */
    //NBB_ULONG pos_id;

} SPM_INCLASSIFY_QOS_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_TRAFFIC_FILTER_CB                                             */
/*                                                                           */
/* Description:TRAFFIC_FILTER配置块。                                           */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_traffic_filter_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_traffic_filter_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    //NBB_HANDLE spm_diff_serv_handle;
     /* FLOW_DIFF_SERV  key*/
    //ATG_DCI_LOG_PORT_VLAN key;
    
    /* TRAFFIC_FILTER */
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER traffic_filter_cfg;

    /* 驱动返回值 */
    //NBB_ULONG pos_id;

} SPM_TRAFFIC_FILTER_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PHYSICAL_PORT_CB                                           */
/*                                                                           */
/* Description: 端口逻辑配置。                                                 */
/*                                                                           */
/* Added by xiaoxiang, 2012/9/11                                             */
/*****************************************************************************/
typedef struct spm_logical_port_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_logical_port_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_logical_port_handle;

    /* key值: index */
    NBB_ULONG port_index_key;

    /* 基本配置。*/
    ATG_DCI_LOG_PORT_BASIC_DATA *basic_cfg_cb;

    /* 物理配置。*/
    ATG_DCI_LOG_PORT_PHY_DATA *phy_cfg_cb;

    /* 逻辑配置L3 */
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *logic_l3_cfg_cb;

    /* 逻辑配置L2 */
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *logic_l2_cfg_cb;

    /* 流相关配置L2 */
    //ATG_DCI_LOG_PORT_FLOW_DATA *flow_cfg_cb;
    //NBB_USHORT flow_num;
    //ATG_DCI_LOG_PORT_FLOW_DATA *flow_cfg_cb[ATG_DCI_LOG_PORT_FLOW_NUM];

    /* 逻辑配置CES */
    ATG_DCI_LOG_PORT_CES_DATA *ces_cfg_cb;

    /* Ipv4地址配置L3 */
    NBB_USHORT ipv4_num;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_cfg_cb[ATG_DCI_LOG_PORT_IPV4_NUM];

    /* IPv6地址配置L3 */
    NBB_USHORT ipv6_num;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_cfg_cb[ATG_DCI_LOG_PORT_IPV6_NUM];

    /* 组播组地址L3 */
    NBB_USHORT mc_ipv4_num;
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *mc_ipv4_cfg_cb[ATG_DCI_LOG_PORT_MC_IPV4_NUM];

    /* 虚拟MAC和IP地址配置L3 */
    NBB_USHORT vip_vmac_num;
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *vip_vmac_cfg_cb[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /***************************************************************************/
    /* VE MAC地址配置.                                                         */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_VE_MAC_DATA *ve_mac_cfg_cb;

    /***************************************************************************/
    /* Diff-Serv配置.                                                          */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_cfg_cb;

    /***************************************************************************/
    /* Diff-Serv配置.                                              */
    /***************************************************************************/
    NBB_USHORT diff_serv_num;
    AVLL_TREE diff_serv_tree;

    /***************************************************************************/
    /* 流相关Diff-Serv配置L2.                                                  */
    /***************************************************************************/
    //NBB_USHORT flow_diff_serv_num;
    //ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_cfg_cb[ATG_DCI_LOG_FLOW_DIFF_SERV_NUM];
    //ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_cfg_cb;

    /***************************************************************************/
    /* 流相关Diff-Serv配置L2.                                                  */
    /***************************************************************************/
    NBB_USHORT flow_diff_serv_num;
    AVLL_TREE flow_diff_serv_tree;

    /***************************************************************************/
    /* 上话复杂流分类QOS策略配置L3.                                                  */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_INCLASSIFY_QOS *inclassify_qos_cfg_cb;

    /***************************************************************************/
    /* 上话复杂流分类QOS策略配置                                                 */
    /***************************************************************************/
    NBB_USHORT inclassify_qos_num;
    AVLL_TREE inclassify_qos_tree;

    /***************************************************************************/
    /* 上话用户QOS策略配置L3.                                                  */
    /***************************************************************************/
    //ATG_DCI_LOG_UP_USER_QOS_POLICY *up_user_qos_cfg_cb;

    /***************************************************************************/
    /* 上话用户组QOS策略配置.                                                  */
    /***************************************************************************/
    ATG_DCI_LOG_UP_USER_GROUP_QOS *up_user_group_qos_cfg_cb;

    /***************************************************************************/
    /* 下话用户队列QOS策略配置L3.                                              */
    /***************************************************************************/
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *down_user_qos_cfg_cb;

    /***************************************************************************/
    /* 下话用户组QOS配置L3.                                                    */
    /***************************************************************************/
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *down_user_group_qos_cfg_cb;

    /***************************************************************************/
    /* 流相关上话用户QOS策略配置L2.                                            */
    /***************************************************************************/
    //ATG_DCI_LOG_FLOW_UP_USER_QOS *flow_up_user_qos_cfg_cb;

    /***************************************************************************/
    /* IPV6组播组地址L3.                                              */
    /***************************************************************************/
	NBB_USHORT mc_ipv6_num;
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *mc_ipv6_cfg_cb[ATG_DCI_LOG_PORT_MC_IPV6_NUM];

    /***************************************************************************/
    /* 端口逻辑配置-包过滤器。                                                  */
    /***************************************************************************/
    NBB_USHORT traffic_filter_num;
    AVLL_TREE traffic_filter_tree;

    /***************************************************************************/
    /* Diff-Serv配置L2（VP下话UNI侧）.                                              */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_DS_L2_DATA *ds_l2_cfg_cb;

    /***************************************************************************/
    /* 终结子接口。.                                              */
    /***************************************************************************/
    NBB_USHORT terminal_if_num;
    //ATG_DCI_LOG_PORT_TERMINAL_IF *terminal_if_cfg_cb[ATG_DCI_LOG_TERMINAL_IF_NUM];
    AVLL_TREE terminal_if_tree;

    /* 虚拟MAC和IP地址配置L3 */
    NBB_USHORT vipv6_vmac_num;
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *vipv6_vmac_cfg_cb[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];    

    /***************************************************************************/
    /*  C3芯片号.                                       */
    /***************************************************************************/
    NBB_INT unit_id;

    /***************************************************************************/
    /* 表示C3物理端口，VE口index，LAG端口ID.                                 */
    /***************************************************************************/
    NBB_USHORT port_id;

    /***************************************************************************/
    /* 槽位号. VE和平LAG没有槽位                                */
    /***************************************************************************/
    NBB_BYTE slot_id;

    SPM_LOGICAL_PORT_INFO_CB logic_port_info_cb;

} SPM_LOGICAL_PORT_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_LAG_CB                                                    */
/*                                                                           */
/* Description: 链路聚合                                                     */
/*                                                                           */
/* Added by xiaoxiang, 2012/12/1                                             */
/*****************************************************************************/
typedef struct spm_lag_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_lag_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_lag_handle;

    /***************************************************************************/
    /* key值: lag_id.                                                         */
    /***************************************************************************/
    NBB_ULONG lag_id_key;

    /***************************************************************************/
    /* 全局属性。                                                              */
    /***************************************************************************/
    ATG_DCI_LINK_AGGR_GLOB_ATTRIB *global_cfg_cb;

    /***************************************************************************/
    /* LAG配置。                                                               */
    /***************************************************************************/
    ATG_DCI_LINK_AGGR_LAG_CONF *lag_cfg_cb;

    /***************************************************************************/
    /* 成员端口配置。                                                          */
    /***************************************************************************/
    ATG_DCI_LINK_AGGR_MEMBER_PORT *port_cfg_cb[ATG_DCI_LAG_MEMBER_PORT_NUM];

    /***************************************************************************/
    /* 非负载分担成员端口配置。                                                */
    /***************************************************************************/
    ATG_DCI_LAG_NLB_MEMBER_PORT *nlb_port_cfg_cb;
    /***************************************************************************/
    /* IPV4流采样配置                                                */
    /***************************************************************************/
    ATG_DCI_LAG_IPV4NF_DATA *ipv4_nf_data;
     /***************************************************************************/
    /* IP流采样镜像                                                */
    /***************************************************************************/
    ATG_DCI_LAG_IPNF_SAMPLER_DATA *ip_sample_data;
    /***************************************************************************/
    /* IPV6流采样配置                                                */
    /***************************************************************************/
    ATG_DCI_LAG_IPV6NF_DATA *ipv6_nf_data;

    NBB_ULONG port_num;

	//第1个成员口的port_id
    NBB_USHORT lag_port_id;

	//第1个成员口所在的槽位
    NBB_BYTE lag_slot_id;

    NBB_BYTE if_lag_exist;

    NBB_INT ifLagConfiged;

}SPM_LAG_CB;
/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_ASSOCIATE_IF_CB                                                    */
/*                                                                           */
/* Description: 关联接口配置。                                                     */
/*                                                                           */
/* Added by zhangzhm, 2013/11/20                                           */
/*****************************************************************************/

typedef struct spm_associate_if_cb
{
	/***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
	AVLL_NODE spm_associate_if_node;
	/***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
	NBB_HANDLE spm_associate_if_handle;
	/***************************************************************************/
    /* key值: Monitor_Group_ID.                                                         */
    /***************************************************************************/
	NBB_BYTE key;
	/***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
	ATG_DCI_ASSOCIATE_IF_BASIC *basic_cfg_cb;
	/***************************************************************************/
    /* 触发接口数目                                                              */
    /***************************************************************************/
	NBB_USHORT trigger_if_num;
	/***************************************************************************/
    /*动作接口数目                                                             */
    /***************************************************************************/
	NBB_USHORT act_if_num;
	/***************************************************************************/
    /*触发接口配置                                                              */
    /***************************************************************************/
	ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *trigger_if_cfg_cb[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM];
	/***************************************************************************/
    /*触发接口对应逻辑接口是否存在的标志位   0/1=不存在/存在                                                           */
    /***************************************************************************/
    NBB_BYTE trigger_if_exist[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM];
    /* 动作接口配置                                                             */
    /***************************************************************************/
	ATG_DCI_ASSOCIATE_IF_ACT_IF *act_if_cfg_cb[ATG_DCI_ASSOCIATE_IF_ACT_NUM];
    /***************************************************************************/
    /* 动作接口对应逻辑接口是否存在标志位 0/1=不存在/存在                                                           */
    /***************************************************************************/
    NBB_BYTE act_if_exist[ATG_DCI_ASSOCIATE_IF_ACT_NUM];
	
}SPM_ASSOCIATE_IF_CB;


/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_L3_MC_CB                                                 */
/*                                                                           */
/* Description: VPLS_L3_MC配置。                                                */
/*                                                                           */
/* Added by xiaoxiang, 2012/9/18                                             */
/*****************************************************************************/
typedef struct spm_vpls_l3_mc_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_vpls_l3_mc_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_vpls_l3_mc_handle;

    /***************************************************************************/
    /* key值: vpls_mc_id.                                                      */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_KEY vpls_l3_mc_key;

    /***************************************************************************/
    /* NNI个数.                                                                */
    /***************************************************************************/
    NBB_USHORT nni_num;

    /***************************************************************************/
    /* UNI个数.                                                                */
    /***************************************************************************/
    NBB_USHORT uni_num;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* NNI接口配置。                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_NNI_DATA *nni_cfg_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB nni_info_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    /***************************************************************************/
    /* UNI接口配置。                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_UNI_DATA *uni_cfg_cb[ATG_DCI_VPLS_MC_UNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB uni_info_cb[ATG_DCI_VPLS_MC_UNI_NUM];

    /***************************************************************************/
    /* 驱动返回值。                                                           */
    /***************************************************************************/
    NBB_ULONG ulIgmpSnoopPosId;
} SPM_VPLS_L3_MC_CB;


/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_MC_CB                                                 */
/*                                                                           */
/* Description: VPLS_MC配置。                                                */
/*                                                                           */
/* Added by xiaoxiang, 2012/9/18                                             */
/*****************************************************************************/
typedef struct spm_vpls_mc_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_vpls_mc_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_vpls_mc_handle;

    /***************************************************************************/
    /* key值: vpls_mc_id.                                                      */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_KEY vpls_mc_key;

    /***************************************************************************/
    /* NNI个数.                                                                */
    /***************************************************************************/
    NBB_USHORT nni_num;

    /***************************************************************************/
    /* UNI个数.                                                                */
    /***************************************************************************/
    NBB_USHORT uni_num;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* NNI接口配置。                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_NNI_DATA *nni_cfg_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB nni_info_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    /***************************************************************************/
    /* UNI接口配置。                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_UNI_DATA *uni_cfg_cb[ATG_DCI_VPLS_MC_UNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB uni_info_cb[ATG_DCI_VPLS_MC_UNI_NUM];

} SPM_VPLS_MC_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_UC_CB                                                 */
/*                                                                           */
/* Description: VPLS_UC配置。                                                */
/*                                                                           */
/* Added by xiaoxiang, 2013/4/18                                             */
/*****************************************************************************/
typedef struct spm_vpls_uc_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_vpls_uc_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_vpls_uc_handle;

    /***************************************************************************/
    /* key值: vpls_mc_id.                                                      */
    /***************************************************************************/
    ATG_DCI_VPLS_UC_KEY vpls_uc_key;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_VPLS_UC_BASIC_DATA *basic_cfg_cb;

} SPM_VPLS_UC_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_ARP_CB                                                    */
/*                                                                           */
/* Description: ARP表。                                                       */
/*                                                                           */
/* Added by xiaoxiang, 2012/11/10                                             */
/*****************************************************************************/
typedef struct spm_arp_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_arp_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_arp_handle;

    /***************************************************************************/
    /* key值: Index+IP.                                         */
    /***************************************************************************/
    ATG_DCI_ARP_KEY arp_key;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_ARP_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* 建ARP时在驱动中的位置。                                                 */
    /***************************************************************************/
    NBB_USHORT arp_pos_id;

} SPM_ARP_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_NCACHE_CB                                                  */
/*                                                                           */
/* Description: IPv6 邻居缓存。                                              */
/*                                                                           */
/* Added by xiaoxiang, 2013/4/15                                            */
/*****************************************************************************/
typedef struct spm_ncache_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_ncache_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_ncache_handle;

    /***************************************************************************/
    /* key值: Index+IPv6.                                         */
    /***************************************************************************/
    ATG_DCI_IPV6_CACHE_KEY ncache_key;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_IPV6_CACHE_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* 建ARP时在驱动中的位置。                                                 */
    /***************************************************************************/
    NBB_USHORT ncache_pos_id;

} SPM_NCACHE_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PORT_MIRROR_CB                                                    */
/*                                                                           */
/* Description: 端口镜像。                                                       */
/*                                                                           */
/* Added by xiaoxiang, 2013/03/15                                             */
/*****************************************************************************/
typedef struct spm_port_mirror_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_port_mirror_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_port_mirror_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    NBB_ULONG port_mirror_key;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_PORT_MIRROR_BASIC_DATA *basic_cfg_cb;

} SPM_PORT_MIRROR_CB;

/**STRUCT-********************************************************************/



/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_BYPASS_CB                                                  */
/*                                                                           */
/* Description: Bypass双归保护配置。                                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/07/16                                            */
/*****************************************************************************/
typedef struct spm_bypass_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_bypass_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_bypass_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    NBB_USHORT bypass_key;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_BYPASS_BASIC_DATA *basic_cfg_cb;

} SPM_BYPASS_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MSP_CB                                                    */
/*                                                                           */
/* Description: MSP保护协议。                                                       */
/*                                                                           */
/* Added by xiaoxiang, 2014/04/10                                             */
/*****************************************************************************/
typedef struct spm_msp_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_msp_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_msp_handle;

    /***************************************************************************/
    /* key值: ID.                                         */
    /***************************************************************************/
    NBB_ULONG msp_key;

    /***************************************************************************/
    /* 基本配置。                                                              */
    /***************************************************************************/
    ATG_DCI_MSP_BASIC *basic_cfg_cb;

    /***************************************************************************/
    /* 主用端口信息                                                            */
    /***************************************************************************/
    SPM_MSP_PROT_INFO_CB main_info[63 * DERECT_PHY_PORT_RATE];

    /***************************************************************************/
    /* 备用端口信息                                                            */
    /***************************************************************************/
    SPM_MSP_PROT_INFO_CB bak_info[63 * DERECT_PHY_PORT_RATE];        

} SPM_MSP_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VC_MOD_INFO_CB                                             */
/*                                                                           */
/* Description: 在VC修改时，保存修改后驱动返回的信息。                       */
/*                                                                           */
/* Added by xiaoxiang, 2013/03/15                                            */
/*****************************************************************************/
typedef struct spm_vc_mod_info_cb
{
    /***************************************************************************/
    /* 改变的是主用还是备用.   主/备=1/2  in                                     */
    /***************************************************************************/
    NBB_USHORT if_main;

    /***************************************************************************/
    /* vp_idx.    in/out                                     */
    /***************************************************************************/
    NBB_UINT vp_idx_main;
    NBB_UINT vp_idx_back;

    /***************************************************************************/
    /*  VC对应的下一跳ID.   out                                    */
    /***************************************************************************/
    NBB_ULONG next_hop_counter_id_main;
    NBB_ULONG next_hop_counter_id_back;
    
    /***************************************************************************/
    /*  VC对应的下一跳ID.   out                                    */
    /***************************************************************************/
    NBB_ULONG next_hop_id_main;
    NBB_ULONG next_hop_id_back;

    /***************************************************************************/
    /* VC走的LSP所对应的出口index.   out                                       */
    /***************************************************************************/
    NBB_ULONG port_index_main;
    NBB_ULONG port_index_back;
    
    /***************************************************************************/
    /*  FRR保护的group_id.         in/out                              */
    /***************************************************************************/
    NBB_USHORT frr_group_id;
} SPM_VC_MOD_INFO_CB;

/**STRUCT+********************************************************************/
/* Structure: SPM_LAG_PORT_INFO_CB                                            */
/*                                                                           */
/* Description:记录lag成员端口的port_id 和协商状态                      */
/*                                                                           */
/* Added by zhangzhm 20131119                                            */
/*****************************************************************************/

typedef struct spm_lag_port_info_cb
{
	/***************************************************************************/
    /*  成员端口对应的物理口port_id                                  */
    /***************************************************************************/
	NBB_USHORT port_id;
	/***************************************************************************/
    /*  成员端口对应的物理口slot_id                                  */
    /***************************************************************************/
	NBB_BYTE slot_id;
	/***************************************************************************/
    /*  lacp协商状态 1/0='成功'/'不成功'                                  */
    /***************************************************************************/
	NBB_BYTE port_consult;
    /***************************************************************************/
    /* 子卡信息:0/1/2 = 无子卡/子卡1/子卡2                                 */
    /***************************************************************************/
    NBB_USHORT card_id;
}SPM_LAG_PORT_INFO_CB;



/**STRUCT-********************************************************************/

#define         BCMC31          0
#define         BCMC32          1

#define         SHASH
#define         PUBLIC
#define         ITERATION
#define     FOR_OAM

#define FTN_T
#define ILM_T
#define CR_LSP_RX
#define         CR_LSP_TX
#define VRF_U_ROUTE
#define TUNNEL_CHOOSE
#define LSP_PROT
#define VRF_M_ROUTE
#define VRF_VRF_INSTANCE

#define PHYSICAL_PORT_CFG
#define LOGICAL_PORT_CFG
#define LAG_CFG
#define VC_CFG
#define VC_DRV
#define VPWS_CFG
#define VPLS_CFG
#define ARP_CFG
#define NCACHE_CFG
#define PORT_MIRROR_CFG
#define GLOBAL_CFG
#define VPLS_MC_CFG
#define VPLS_UC_CFG
#define SWITCH_VC_CFG
#define BYPASS_CFG
#define MAC_RECYCLE_CFG
#define PTP_CFG
#define ASSOCIATE_IF_CFG
#define MSP_CFG
#define CMD_SWITCH_CFG
#define VPLS_L3_MC_CFG


/************************************
 * L3 ERROR CODE DEFINITION         *
 ************************************/
/*
#define SPM_ERR_NULL_PTR        -1
#define SPM_ERR_OUT_OF_RANGE    -2
#define SPM_ERR_GET_FAILED      -3
#define SPM_ERR_SET_FAILED      -4
#define SPM_ERR_DATA_ERROR      -5
#define SPM_ERR_SEND_FAILED     -6
#define SPM_ERR_PROCESS_FAILED  -7
#define SPM_ERR_NO_BOARD        -8
#define SPM_ERR_MALLOC_FAILED   -9
#define SPM_ERR_DATA_EXISTED    -10
*/

/* 113 114 124 状态查询块使用*/
#define MAC_VPLS_VP_STA

/* 4 状态查询块使用*/
#define PROT_STA_INFO

typedef enum
{
    e_protect_type_lsp          = 2,
    e_protect_type_sncp         = 3, 
    e_protect_type_vc           = 4, 
    e_protect_type_msrp         = 5, 
    e_protect_type_dni_pw       = 6, 
    e_protect_type_dni_pw_vpws  = 7, 
    e_protect_type_bypass       = 8, 
    e_protect_type_bypass_vpws  = 9, 
    e_protect_type_msp          = 10, 
    e_protect_type_vpn_frr      = 11, 
    e_protect_type_ip_frr       = 12, 
    e_protect_type_max
} e_protect_type;


#define STATUS_RQ_TOTAL_CONTENT_LEN 68
#define STATUS_RQ_SINGLE_STATE_CONTET_LEN 64
#define SPM_STAT_MSG_SZ_HEAD        24
#define SPM_STAT_MSG_SZ_T           2
#define SPM_STAT_MSG_SZ_CONT_LEN    2
#define SPM_STAT_MSG_SZ_SUB_T       2
#define SPM_STAT_MSG_SZ_SUB_T_LEN   2
#define SPM_STAT_MSG_SZ_RES         2
#define SPM_STAT_MSG_SZ_NUM         2
#define SPM_STAT_MSG_SZ_ITEM        60
#define SPM_STAT_MSG_SZ (SPM_STAT_MSG_SZ_HEAD   \
    + SPM_STAT_MSG_SZ_T                         \
    + SPM_STAT_MSG_SZ_CONT_LEN                  \
    + SPM_STAT_MSG_SZ_SUB_T                     \
    + SPM_STAT_MSG_SZ_SUB_T_LEN                 \
    + SPM_STAT_MSG_SZ_RES                       \
    + SPM_STAT_MSG_SZ_NUM                       \
    + SPM_STAT_MSG_SZ_ITEM)

#define SPM_STAT_MSG_AD_ITEM_START  (SPM_STAT_MSG_SZ - SPM_STAT_MSG_SZ_ITEM)

#define SPM_STAT_MSG_AD_KEY_START   8
#define SPM_STAT_MSG_AD_KEY_END     24
#define SPM_STAT_MSG_AD_STAT_START  24
#define SPM_STAT_MSG_AD_STAT_END    60
#define SPM_STAT_MSG_SZ_STAT        (           \
    SPM_STAT_MSG_AD_STAT_END - SPM_STAT_MSG_AD_STAT_START)

#define SPM_L3_WR_U16(_u16, _buf)       \
    *(unsigned short *)(_buf) = (_u16); \
    (_buf) += 2;
#define SPM_L3_WR_STRING(_ibuf, _ibuf_len, _obuf)                           \
    memcpy((unsigned char *)(_obuf), (unsigned char *)(_ibuf), (_ibuf_len));\
    (_obuf) += (_ibuf_len);

/*****************************************************************************/
/*  定义内存类型  ,对应每个需要分配内存的结构体*/
/*****************************************************************************/
#define MEM_SPM_THREAD_SOCKET_CB             ((NBB_LONG)(PCT_SPM | 0x00000001))

#define MEM_SPM_HASH_TABLE_CB                   ((NBB_LONG)(PCT_SPM | 0x00005001))
#define MEM_SPM_HASH_NODE_CB                    ((NBB_LONG)(PCT_SPM | 0x00005002))
#define MEM_SPM_HASH_INDEX_CB                    ((NBB_LONG)(PCT_SPM | 0x00005003))

#define MEM_SPM_FTN_TAB_NODE_CB                 ((NBB_LONG)(PCT_SPM | 0x00000701))
#define MEM_SPM_FTN_TAB_PEER_CB         		((NBB_LONG)(PCT_SPM | 0x00000702))
#define MEM_SPM_FTN_TAB_VC_CB         		((NBB_LONG)(PCT_SPM | 0x00000703))
#define MEM_SPM_FTN_TAB_VRF_CB         		((NBB_LONG)(PCT_SPM | 0x00000704))
#define MEM_SPM_FTN_TAB_MC_CB         		((NBB_LONG)(PCT_SPM | 0x00000705))
#define MEM_SPM_FTN_TAB_PEECMP_CB         	((NBB_LONG)(PCT_SPM | 0x00000706))
#define MEM_SPM_FTN_TAB_ECMPNODE_CB        ((NBB_LONG)(PCT_SPM | 0x00000707))
#define MEM_SPM_CR_LSP_TAB_RSVP_CB         ((NBB_LONG)(PCT_SPM | 0x00000708))
#define MEM_SPM_CR_LSP_TAB_VPN_CB         ((NBB_LONG)(PCT_SPM | 0x00000709))
#define MEM_SPM_ILM_TAB_CB                              ((NBB_LONG)(PCT_SPM | 0x00000801))

#define MEM_SPM_CR_LSP_RX_CB                    ((NBB_LONG)(PCT_SPM | 0x00000901))

#define MEM_SPM_CR_LSP_TX_CB                    ((NBB_LONG)(PCT_SPM | 0x00001001))

#define MEM_SPM_VRFUROUTE_NODE_CB               ((NBB_LONG)(PCT_SPM | 0x00001101))
#define MEM_SPM_VRFUROUTE_IPV4_CB               ((NBB_LONG)(PCT_SPM | 0x00001103))
#define MEM_SPM_VRFUROUTE_IPV6_CB               ((NBB_LONG)(PCT_SPM | 0x00001104))
#define MEM_SPM_VRFUROUTE_UNI_CB                ((NBB_LONG)(PCT_SPM | 0x00001105))
#define MEM_SPM_VRFUROUTE_ROUTNODE_CB      ((NBB_LONG)(PCT_SPM | 0x00001106))
#define MEM_SPM_VRFUROUTE_PWNODE_CB         ((NBB_LONG)(PCT_SPM | 0x00001107))
#define MEM_SPM_VRFUROUTE_MPEER_CB         ((NBB_LONG)(PCT_SPM | 0x00001108))
#define MEM_SPM_VRFUROUTE_BPEER_CB         ((NBB_LONG)(PCT_SPM | 0x00001109))
#define MEM_SPM_VRFUROUTE_ECMP_CB         ((NBB_LONG)(PCT_SPM | 0x0000110A))
#define MEM_SPM_VRFUROUTE_BFDFRR_CB      ((NBB_LONG)(PCT_SPM | 0x0000110B))
#define MEM_SPM_VRFUROUTE_IPFRR_CB      ((NBB_LONG)(PCT_SPM | 0x0000110C))


#define MEM_SPM_VRFMROUTE_NODE_CB               ((NBB_LONG)(PCT_SPM | 0x00001201))
#define MEM_SPM_VRFMROUTE_BASE_CB               ((NBB_LONG)(PCT_SPM | 0x00001202))

#define MEM_SPM_TUNNELCH_CB                     ((NBB_LONG)(PCT_SPM | 0x00001501))
#define MEM_SPM_TUNNELCH_PWNODE_CB                ((NBB_LONG)(PCT_SPM | 0x00001503))

#define MEM_SPM_LSPPROT_CB                              ((NBB_LONG)(PCT_SPM | 0x00001601))
#define MEM_SPM_LSPPROT_RSVPCITE_CB              ((NBB_LONG)(PCT_SPM | 0x00001604))

#define MEM_SPM_VRFINS_CB                              ((NBB_LONG)(PCT_SPM | 0x00004001))
/*****************************************************************************/
/* 定义句柄类型*/
/*****************************************************************************/

/*****************************************************************************/
/*  定义最大条目数*/
/*****************************************************************************/
#define SPM_L3_VRF_MAX  8196

/*****************************************************************************/
/* 定义HASH结构*/
/*****************************************************************************/
#ifdef SHASH

#define KEYOFFSET(node, offset)   ((NBB_ULONG *)(((NBB_BYTE *)(node)) + offset))          /*取数据的KEY*/

#endif

/*****************************************************************************/
/* FTN */
/*****************************************************************************/

//#ifdef FTN_T

#define         FTNKEYLGTH     16      /*key  length*/

typedef struct ftn_key
{
    NBB_USHORT vrfid;
    NBB_BYTE res;
    NBB_BYTE mask;

    NBB_ULONG fec;

}FTN_KEY;

typedef struct ftn_base
{
    NBB_BYTE almtype;
    NBB_BYTE nexttype;
    NBB_BYTE res[6];

}FTN_BASE;

typedef struct ftn_nhop
{
    NBB_ULONG nextip;
    NBB_ULONG nextport;
    NBB_ULONG outlabel;

    NBB_BYTE nexttypeb;
    NBB_BYTE res[3];

}FTN_NHOP;

typedef struct ftn_difser
{
    NBB_ULONG dserid;

}FTN_DIFSER;
typedef struct ftn_tab
{
    AVLL_NODE ftn_node;

    FTN_KEY key;
    FTN_BASE  base;
    FTN_NHOP  nexthop[8];
    //NBB_ULONG dserid;

    NBB_USHORT hopnum;
    NBB_USHORT ecmpid;

    DC_LSP dclsp[8];

    NBB_ULONG tunnelid;
    NBB_ULONG index[8];
    NBB_ULONG posid[8];

    NBB_ULONG pwposid;
    NBB_ULONG pwnhi;

    NBB_USHORT bfdnum;
    NBB_BYTE oamnum;
    NBB_BYTE bntype;
    NBB_ULONG l2num;
    NBB_ULONG l3num;
    NBB_BYTE  memflag;
    NBB_BYTE  delflag;
    NBB_BYTE  res[2];

}FTN_TAB;

//#endif

/*****************************************************************************/
/* ILM */
/*****************************************************************************/

//#ifdef ILM_T

#define         ILMKEYLGTH     4      /*key  length*/

typedef struct ilm_key
{
    NBB_ULONG inlabel;

}ILM_KEY;

typedef struct ilm_base
{
    NBB_USHORT vrfid;
    NBB_BYTE feclgth;
    NBB_BYTE res1;

    NBB_ULONG fec;

    NBB_BYTE almtype;
    NBB_BYTE nextype;
    NBB_BYTE labelaction;
    NBB_BYTE res2;

}ILM_BASE;

typedef struct ilm_nhop
{
    NBB_ULONG nextip;
    NBB_ULONG nextport;
    NBB_ULONG outlabel;

    NBB_BYTE nextypeb;
    NBB_BYTE action;
    NBB_BYTE res[6];

}ILM_NHOP;

typedef struct ilm_difser
{
    NBB_ULONG nextip;
    NBB_ULONG nextport;
    NBB_ULONG dserid;
    NBB_BYTE res[4];

}ILM_DIFSER;

typedef struct ilm_tab
{
    ILM_KEY key;
    ILM_BASE base;
    ILM_NHOP nexthop[8];
    NBB_ULONG dserid;

    NBB_BYTE nhopnum;
    NBB_BYTE ntype;
    NBB_USHORT frrid;
	
    NBB_ULONG posid;
	
    NBB_BYTE	 res[2];
    NBB_USHORT ecmpid;	

}ILM_TAB;

//#endif

/*****************************************************************************/
/* CR  LSP  RX */
/*****************************************************************************/

#ifdef CR_LSP_RX

#define         CRRXLSPKEYLGTH     16      /*key  length*/

typedef struct crrxlsp_key
{
    NBB_ULONG ingress;
    NBB_ULONG egress;
    NBB_ULONG tunnelid;
    NBB_ULONG lspid;

}CRRXLSP_KEY;

typedef struct crrxlsp_base
{
    NBB_USHORT vrfid;
    NBB_USHORT res1;

    NBB_ULONG inportindex;
    NBB_ULONG inlabel;
    NBB_ULONG outlabel;
    NBB_ULONG nextip;
    NBB_ULONG nextport;

    NBB_BYTE action;
    NBB_BYTE almtype;
    NBB_USHORT tefrrtype;

    NBB_BYTE tefrrnode;
    NBB_BYTE res2[3];

}CRRXLSP_BASE;

typedef struct crrxlsp_tefrr
{
    NBB_ULONG inportindex;
    NBB_ULONG inlabel;
    NBB_ULONG nextip;
    NBB_ULONG nextport;
    NBB_ULONG outlabel;

    NBB_BYTE action;
    NBB_BYTE res[3];

}CRRXLSP_TEFRR;

typedef struct crrxlsp_difser
{
    NBB_ULONG res1;
    NBB_ULONG dserid;
    NBB_BYTE res2[8];

}CRRXLSP_DIFSER;

typedef struct crrxlsp
{
    CRRXLSP_KEY 		key;
    CRRXLSP_BASE 		base;
    CRRXLSP_TEFRR 	tefrr;

    NBB_ULONG 		mdserid;
    NBB_ULONG 		pdserid;

    NBB_USHORT 		wraplogid;
    NBB_USHORT 		frrid;

    NBB_ULONG 		posid;

    DC_LSP 			dclsp;

}CRRXLSP;

#endif

/*****************************************************************************/
/* CR LSP TX */
/*****************************************************************************/
#ifdef CR_LSP_TX

#define         CRTXLSPKEYLGTH     16

typedef struct crtxlsp_key
{
    NBB_ULONG ingress;
    NBB_ULONG egress;
    NBB_ULONG tunnelid;
    NBB_ULONG lspid;

}CRTXLSP_KEY;

typedef struct crtxlsp_base
{
    NBB_USHORT vrfid;
    NBB_USHORT res1;

    NBB_ULONG outlabel;
    NBB_ULONG nextip;
    NBB_ULONG nextport;

    NBB_USHORT tefrrtype;
    NBB_BYTE tefrrnode;
    NBB_BYTE almtype;

    NBB_BYTE res2[4];

}CRTXLSP_BASE;

typedef struct crtxlsp_tefrr
{
    NBB_ULONG nextip;
    NBB_ULONG nextport;
    NBB_ULONG outlabel;

    NBB_BYTE action;
    NBB_BYTE res[7];

}CRTXLSP_TEFRR;

typedef struct crtxlsp_difser
{
    NBB_ULONG res;
    NBB_ULONG dserid;
}CRTXLSP_DIFSER;

typedef struct crtxlsp_upqos
{
    NBB_BYTE mode;
    NBB_BYTE res1[3];
    NBB_ULONG nodeindex;
    NBB_ULONG qospid;
    NBB_ULONG cir;
    NBB_ULONG pir;
    NBB_ULONG cbs;
    NBB_ULONG pbs;
    NBB_ULONG res2;
}CRTXLSP_UPQOS;
typedef struct crtxlsp
{
    CRTXLSP_KEY key;
    CRTXLSP_BASE  base;
    CRTXLSP_TEFRR  tefrr;

    NBB_ULONG mdserid;
    NBB_ULONG pdserid;

    CRTXLSP_UPQOS upqos;

    NBB_USHORT wraplogid;
    NBB_USHORT res;

    NBB_BYTE porttype;
    NBB_BYTE slot;
    NBB_USHORT port;

    DC_LSP dclsp;

}CRTXLSP;

#endif

/*****************************************************************************/
/* VRF UROUTE  */
/*****************************************************************************/
#if 0

#define         VRFUROUTEKEYLGTH     20      /*key  length*/

typedef struct vrfuroute_key
{
    NBB_USHORT vrfid;
    NBB_BYTE addrtype;
    NBB_BYTE mask;
    NBB_BYTE dip[16];

}VRFUROUTE_KEY;

typedef struct vrfuroute_base
{
    NBB_BYTE routetype;
    NBB_BYTE prmode;
    NBB_BYTE ecmpmode;
    NBB_BYTE ecmpnum;

    NBB_BYTE res[4];

}VRFUROUTE_BASE;

typedef struct ipv4_nexthopaddr
{
    NBB_ULONG nextip;
    NBB_ULONG nextport;
    NBB_ULONG label;

    NBB_BYTE nexttypeb;
    NBB_BYTE res[3];

}IPV4_NEXTHOPADDR;

typedef struct vrfuroute_ipv4
{
    IPV4_NEXTHOPADDR nexthop[8];
}VRFUROUTE_IPV4;

typedef struct ipv6_nexthopaddr
{
    NBB_BYTE res1[3];
    NBB_BYTE nexttype;

    NBB_BYTE nextip[16];

    NBB_ULONG nextport;
    NBB_ULONG label;

    NBB_BYTE nexttypeb;
    NBB_BYTE res2[3];

}IPV6_NEXTHOPADDR;

typedef struct vrfuroute_ipv6
{
    IPV6_NEXTHOPADDR nexthop[8];
}VRFUROUTE_IPV6;

typedef struct vrfuroute
{
    AVLL_NODE route_node;

    VRFUROUTE_KEY key;

    VRFUROUTE_BASE base;
    VRFUROUTE_IPV4 ipv4;
    VRFUROUTE_IPV6 ipv6;

    NBB_USHORT ipv4num;
    NBB_USHORT ipv6num;
    DC_UNI dchop[8];
    NBB_ULONG nextindex[8];

    NBB_ULONG memindex;
    NBB_ULONG memflag;

    //struct vrfuroute          *prev;
    struct vrfuroute *next;

}VRFUROUTE;

#endif

/*****************************************************************************/
/* VRF MROUTE  */
/*****************************************************************************/
#ifdef VRF_M_ROUTE

#define         MCROUTEKEYLGTH     8      /*key  length*/

typedef struct mcroute_key
{
    NBB_USHORT vrfid;
    NBB_BYTE addrtype;
    NBB_BYTE res;
	
    NBB_ULONG ips[4];
    NBB_ULONG ipg[4];
}MCROUTE_KEY;

typedef struct mcroute_base
{
    NBB_USHORT mcid;
    NBB_BYTE res;
    NBB_BYTE type;
    NBB_ULONG inport;
    NBB_ULONG upip[4];
    NBB_ULONG pelabel;
    NBB_ULONG portnumm;	
    NBB_ULONG portnumn;
    NBB_ULONG outport[16];

}MCROUTE_BASE;

typedef struct mcroute_pmsi
{
    NBB_ULONG downip[4];
    NBB_ULONG outlabel;

}MCROUTE_PMSI;

typedef struct mcroute
{
    AVLL_NODE route_node;

    MCROUTE_KEY key;
    MCROUTE_BASE base;
    MCROUTE_PMSI pmsi[16];

    NBB_ULONG nhi[32];
    NBB_ULONG outcud[32];
    NBB_ULONG posid[32];
    NBB_BYTE outslot[32];


}MCROUTE;

#endif

#ifdef TUNNEL_CHOOSE

#define         TUNNELCHKEYLGTH     8      /*key  length*/

typedef struct tunnelch_key
{
    NBB_USHORT res;
    NBB_USHORT vrfid;
    NBB_ULONG peerip;

}TUNNELCH_KEY;

typedef struct tunnelch_base
{
    NBB_ULONG res;
    NBB_ULONG ingress;
    NBB_ULONG egress;
    NBB_ULONG tunnelid;

}TUNNELCH_BASE;

typedef struct pw_key
{
	NBB_ULONG label;
	
}PW_KEY;

typedef struct pwtxnode
{
	AVLL_NODE pw_node;

	PW_KEY key;

	NBB_ULONG posid;

	NBB_ULONG nhi;

	NBB_ULONG routenum;

}PWTXNODE;

typedef struct tunnelch
{

    TUNNELCH_KEY key;

    TUNNELCH_BASE base;

    NBB_USHORT res;
    NBB_USHORT delflag;
	
    NBB_ULONG tunindex;

    NBB_ULONG pwnum;

    AVLL_TREE pwtree;

}TUNNELCH;

#endif

/*****************************************************************************/
/* LSP PROT */
/*****************************************************************************/
#ifdef LSP_PROT

#define         LSPPROTKEYLGTH     12      /*key  length*/

typedef struct lspprot_key
{
    NBB_ULONG ingress;
    NBB_ULONG egress;
    NBB_ULONG tunnelid;

}LSPPROT_KEY;

typedef struct lspprot_base
{
    NBB_BYTE prtype;
    NBB_BYTE returntype;
    NBB_USHORT delaytime;

    NBB_BYTE waittime;
    NBB_BYTE res[3];

}LSPPROT_BASE;

typedef struct lspprot_1b1
{
    NBB_ULONG lspid;

    NBB_BYTE state;
    NBB_BYTE res[3];

}LSPPROT_1B1;

typedef struct lspprot_b
{
    LSPPROT_1B1 lsp[2];
}LSPPROT_B;

typedef struct lspprot
{

    LSPPROT_KEY key;

    LSPPROT_BASE base;

    LSPPROT_B lspb;

    NBB_USHORT lspnum;
    NBB_USHORT frrid;

    DC_LSP dclsp[2];

    NBB_ULONG index;
    NBB_ULONG hwposid[2];
    NBB_BYTE reverseflag;
    NBB_BYTE hwstate;
    
    CRTXLSP_KEY hwlspkey[2];
    CRTXLSP_KEY oldhwlspkey[2];
    CRTXLSP_KEY worklspkey;
    NBB_BYTE    memflag;
    NBB_BYTE    delflag;
    NBB_BYTE    res[2];
    NBB_ULONG   l2num;
    NBB_ULONG   l3num;

}LSPPROT;

#endif

/*****************************************************************************/
/* VRF VRF INSTANCE  */
/*****************************************************************************/
#ifdef VRF_VRF_INSTANCE

#define         MCROUTEKEYLGTH     8      /*key  length*/

typedef struct vrfins_key
{
    NBB_USHORT res;
    NBB_USHORT vrfid;
	
    NBB_ULONG peerip;
}VRFINS_KEY;

typedef struct vrfins_difser
{
    NBB_ULONG res;
    NBB_ULONG dserid;
}VRFINS_DIFSER;

typedef struct vrfins_upqos
{
    NBB_ULONG res;
    NBB_ULONG nodeindex;
    NBB_ULONG qospid;
    NBB_ULONG qeqospid;
}VRFINS_UPQOS;

typedef struct vrfins
{
    AVLL_NODE ins_node;

    VRFINS_KEY key;
    VRFINS_DIFSER difs;
    VRFINS_UPQOS upqos;

    CRTXLSP_KEY	txlspkey;

    NBB_BYTE porttype;
    NBB_BYTE slot;
    NBB_USHORT port;


}VRFINS;

#endif

/*****************************************************************************/
/* UROUTE-->UNI PORT*/
/*****************************************************************************/

typedef struct uni_key
{
    NBB_ULONG nextip[4];
    NBB_ULONG nextport;
}UNI_KEY;

typedef struct uni_port
{
    NBB_BYTE slot;
    NBB_BYTE port;
    NBB_USHORT vlan;

}UNI_PORT;

typedef struct uniport
{
    AVLL_NODE uni_node;
    UNI_KEY key;
    UNI_PORT port;

    NBB_ULONG posid;
    NBB_ULONG nhid;

    NBB_ULONG routenum;

}UNIPORT;


/*****************************************************************************/
/* PEERIP-->FRR ID*/
/*****************************************************************************/

typedef struct peer_key
{
    NBB_ULONG peer;
}PEER_KEY;

typedef struct bpeerfrr
{
    AVLL_NODE bp_node;
	
    PEER_KEY key;

    NBB_USHORT frrid;
    NBB_BYTE	state;
    NBB_BYTE flag;

    NBB_ULONG frrnum;
}BPEERFRR;

typedef struct mpeerfrr
{
    AVLL_NODE mp_node;
	
    PEER_KEY key;

    NBB_ULONG bpeernum;

    AVLL_TREE bpeer_tree;
}MPEERFRR;

/*****************************************************************************/
/* BFD ID-->FRR ID*/
/*****************************************************************************/

typedef struct bfdfrr_key
{
    NBB_USHORT res;
    NBB_USHORT bfdid;
}BFDFRR_KEY;

typedef struct bfdfrr
{
    AVLL_NODE bf_node;
	
    BFDFRR_KEY key;

    NBB_USHORT res;
    NBB_USHORT frrid;
	
    NBB_ULONG citenum;
	
}BFDFRR;

/*****************************************************************************/
/* NEXT IP-->FRR ID*/
/*****************************************************************************/

typedef struct ipfrr_key
{
    NBB_USHORT res;
    NBB_USHORT vrfid;
    
    NBB_ULONG mpeer;
    NBB_ULONG bpeer;
    
}IPFRR_KEY;

typedef struct ipfrr
{
    AVLL_NODE ipf_node;
	
    IPFRR_KEY key;

    NBB_USHORT res;
    NBB_USHORT frrid;
	
    NBB_ULONG citenum;
	
}IPFRR;

/*****************************************************************************/
/* PEERIP-->PW VC*/
/*****************************************************************************/

typedef struct peip_key
{
    NBB_ULONG peer;
}PEIP_KEY;

typedef struct pwvc_key
{
    NBB_ULONG vpid;
    NBB_ULONG vcid;
}PWVC_KEY;

typedef struct pwvc
{
    AVLL_NODE vc_node;
	
    PWVC_KEY key;

    SPM_VP_INFO_REFRESH_CB l2vp;

}PWVC;

typedef struct pwvrf_key
{
    NBB_USHORT res1;
    NBB_USHORT vrfid;
}PWVRF_KEY;

typedef struct pwvrf
{
    AVLL_NODE vrf_node;
	
    PWVRF_KEY key;

    NBB_USHORT labelnum;
    NBB_BYTE	state;
    NBB_BYTE flag;

}PWVRF;

typedef struct pwmc_key
{
    NBB_USHORT vplsid;
    NBB_USHORT mcid;
}PWMC_KEY;

typedef struct pwmc
{
    AVLL_NODE mc_node;
	
    PWMC_KEY key;

    NBB_BYTE    type;
    NBB_BYTE	  res1;
    NBB_USHORT portnum;

    NBB_ULONG port;

}PWMC;

typedef struct peerftn
{
    AVLL_NODE pe_node;
	
    PEIP_KEY 	key;

    NBB_ULONG tunnel;
	
    NBB_ULONG vpnum;
    AVLL_TREE vp_tree;

    NBB_ULONG vrfnum;
    AVLL_TREE vrf_tree;

    NBB_ULONG mcnum;
    AVLL_TREE mcid_tree;
	
}PEERFTN;

typedef struct pw_vpn
{
    AVLL_NODE      vrf_node;	
    TUNNELCH_KEY   key;
    NBB_USHORT     labelnum;
    NBB_BYTE	   state;
    NBB_BYTE       flag;

}PWVPN;
typedef struct rsvpcrlsp
{
    AVLL_NODE    rsvp_node;
	LSPPROT_KEY  key;             /*tunnel key值     */ 
	NBB_ULONG    tunlid;          /*驱动返tunlid值   */
	NBB_ULONG    vpnum;           /*L2VPN数目        */
    AVLL_TREE    vp_tree;         /*下挂的VP树       */
	NBB_ULONG    vrfpeernum;      /*L3 VPN条目       */
    AVLL_TREE    vrfpeer_tree;    /*下挂的L3VPN 树   */
	NBB_ULONG    mcnum;           /*L2层组播条目     */
    AVLL_TREE    mcid_tree;       /*下挂的组播树     */
}RSVPCRLSP;
/*****************************************************************************/
/* ECMP ID*/
/*****************************************************************************/

typedef struct ecmp_key
{
    NBB_ULONG nhi1;
    NBB_ULONG nhi2;
    NBB_ULONG nhi3;
    NBB_ULONG nhi4;
}ECMP_KEY;

typedef struct ecmptab
{
    AVLL_NODE ep_node;
	
    ECMP_KEY key;

    NBB_ULONG ecmpid;

    NBB_BYTE pflag;
    NBB_BYTE ecmpnum;
    NBB_USHORT  res;
	
    NBB_ULONG peerip[4];

    NBB_ULONG num;
}ECMPTAB;

/*****************************************************************************/
/* PEER-->ECMP ID*/
/*****************************************************************************/
typedef struct peerep_key
{
    NBB_ULONG peer;

}PEEREP_KEY;

typedef struct ecmpen_key
{
    NBB_USHORT 	res;
    NBB_USHORT 	ecmpid;

}ECMPEN_KEY;

typedef struct ecmpnode
{
    AVLL_NODE en_node;
	
    ECMPEN_KEY key;

    NBB_USHORT 	flag;

    NBB_USHORT  num;
	
    NBB_ULONG peerip[4];

    NBB_ULONG nhi[4];

    NBB_ULONG citenum;

}ECMPNODE;

typedef struct peereptab
{
    AVLL_NODE pp_node;
	
    PEEREP_KEY key;
	
    NBB_ULONG num;

    AVLL_TREE ecmpnode_tree;

}PEEREPTAB;
/*****************************************************************************/
/* TUNNEL-->PW-->ROUTE*/
/*****************************************************************************/

typedef struct lsprsvp
{
    AVLL_NODE lsp_node;

    LSPPROT_KEY key;

    NBB_ULONG pwnum;

    AVLL_TREE pw_tree;

}LSPRSVP;

typedef struct lspldp
{

    AVLL_NODE lsp_node;

    FTN_KEY key;

    NBB_ULONG pwnum;

    AVLL_TREE pw_tree;

}LSPLDP;

typedef struct pwnode
{

    AVLL_NODE pw_node;

    TUNNELCH_KEY key;

    NBB_ULONG routenum;

    AVLL_TREE rout_tree;

    LSPRSVP	*lspcite;

}PWNODE;

typedef struct routnode
{

    AVLL_NODE rout_node;

    VRFUROUTE_KEY key;

     PWNODE	*pwcite;

}ROUTNODE;

/*
extern unsigned char *g_spm_l3_debug_level[];
extern unsigned char g_spm_l3_debug;
extern unsigned int g_spm_l3_module;

#define LEVEL_MASK      ((unsigned int)0x0000ffff)
#define SPM_L3_ERROR    ((unsigned int)0x00000000)
#define SPM_L3_DEBUG    ((unsigned int)0x00000001)
#define SPM_L3_INFO     ((unsigned int)0x00000002)

#define MOD_MASK            ((unsigned int)0xffff0000)
#define SPM_L3_MOD_ALM      ((unsigned int)0x00010000)
#define SPM_L3_MOD_STAT     ((unsigned int)0x00020000)


#define SPM_L3_PRINTF(_level, _format, _arg...)                         \
    if (((_level) & LEVEL_MASK) <= g_spm_l3_debug                       \
        && (((_level) & MOD_MASK) == 0                                  \
            || (((_level) & MOD_MASK) & g_spm_l3_module) != 0))         \
    {                                                                   \
        printf("%s(%d) : %s() :\n", __FILE__, __LINE__, __FUNCTION__);  \
        printf("%s : " _format "\n\n",                                  \
            g_spm_l3_debug_level[((_level) & LEVEL_MASK)], ##_arg);     \
    }                                                                   \
    if (((_level) & MOD_MASK) != SPM_L3_MOD_ALM)                        \
    {                                                                   \
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "[%s] " _format "\n",        \
            g_spm_l3_debug_level[((_level) & LEVEL_MASK)], ##_arg);     \
    }
 */

#endif
