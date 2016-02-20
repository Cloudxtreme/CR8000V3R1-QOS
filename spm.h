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
/* ����ӿ�������Ŀǰϵͳֻ֧��2���ӿ�.                                      */
/*****************************************************************************/
#define MAX_SUB_CARD_NUM                                       2






#define MAX_PHYSIC_PORT_NUM                             128

/*HQOS���LSP��Ŀ��*/
#define HQOS_MAX_LSP_ID 4096

/*HQOS���pw��Ŀ��*/
#define HQOS_MAX_PW_ID   4096

#define MAX_ACL_CFG_NUM          6000


#define MAX_FLOW_CLASSIFY_PRI         5000
#define MAX_FLOW_CLASSIFY_RULE_ID         16000

#define MAX_CLASSIFY_PORT_ACL_ID_NUM  99
#define MAX_CLASSIFY_HIGH_ACL_ID_NUM  1000
#define MAX_CLASSIFY_ETH_ACL_ID_NUM  99


#define SIGNAL_NUM  46



#define SPM_SLOT_MAX_NUM              10    /* ����ҵ���̲�λ���� */
#define SPM_PORT_MAX_NUM              40    /* ÿ����λ���ڵ������� */

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
/* ���ÿ��������������õ�����ֵ.                                             */
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
#define SPM_CTRL_TYPE__CFG_CLEARALL                    19/* ������� */
#define SPM_CTRL_TYPE__PWPING_SEND_ENABLE               31
#define SPM_CTRL_TYPE_E1_LINE                           32 
#define SPM_CTRL_TYPE_E1_SYS                            33 

/*****************************************************************************/
/* ����״̬ͨ��Э��-״̬ͨ������T.                                           */
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
#define SPM_STATUS_RQ_TYPE_VPLS_MAC_TABLEGATHER   124 /* vpls mac�������Ϣ add by gaos 2013.12.5*/
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
/* ������ʹ�ܣ�0/1=��ʹ��/ʹ�ܡ�              */
/*****************************************************************************/
#define ENABLE  1
#define DISABLE 0

/*****************************************************************************/
/* C3оƬѡ��                                                              */
/*****************************************************************************/
#define UNIT_0    0x0
#define UNIT_1    0x1

/*****************************************************************************/
/* ͻ�����ȣ��ݸ�ֵΪ640����80kbyte                                        */
/*****************************************************************************/
#define KBITS_BURST     640

/***************************************************************************/
/* ָ�����ñ��ã���/��=1/2.                                       */
/***************************************************************************/
#define VC_MAIN     1
#define VC_BACK     2

/***************************************************************************/
/* ��Ŀ����/����=0/1.                                       */
/***************************************************************************/
#define SPM_OPER_ADD    0
#define SPM_OPER_UPD    1
#define SPM_OPER_DEL    2

/***************************************************************************/
/* ��Ŀ����/����=0/1.                                       */
/***************************************************************************/
#define SPM_PRINT_CFG       	1
#define SPM_PRINT_DETAIL    	2
#define SPM_PRINT_ALL       	3

#define L2VPN_VPWS              1
#define L2VPN_VPLS              2
#define L2VPN_MSPW              4

/*****************************************************************************/
/*VPLS__MAC ״̬��ѯ113�ص�״̬MAC_NUM�����ֵ  */
/*****************************************************************************/
#define VPLS_MAC_STA_MAX_NUM     6551

/*****************************************************************************/
/*VPLS__MAC ״̬��ѯ113�ص�״̬MAC_NUM��C3һ���ϱ������ֵ  */
/*****************************************************************************/
#define VPLS_MAC_STA_C3_MAX_NUM     6551

/*****************************************************************************/
/*���� ״̬��ѯ4�ص�״̬���ֵ(������ѯ��Ϣ)  */
/*****************************************************************************/
#define PROT_STA_INFO_MAX_NUM     100

/***************************************************************************/
/* ֱ������˿����� 1=STM1                                      */
/***************************************************************************/

#define DERECT_PHY_PORT_RATE              1
#define SPM_VP_NNI              1
#define SPM_VP_UNI              2

//������Ϣmsg���鳤��
//2015.1.10 200->500��ֹ��ӡ��Ϣ����ʱԽ��
#define SPM_MSG_INFO_LEN    	500 

//�鲥Arad�̶��˿�
#define SPM_MULTICAST_PORT_0		109
#define SPM_MULTICAST_PORT_1		110

//�鲥IP��ַ�·�ʱ��VE�ڼ���port_id��ƫ��
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
#define FUNC_IN_PARAM_IS_NULL       -101    //�����������ΪNULL
#define CALL_FUNC_ERROR             -102    //����C3��������
#define CALL_C3_FUNC_ERROR          -103    //����C3��������
#define CALL_ARAD_FUNC_ERROR        -104    //����Arad��������
#define DEL_UNEXIST_CONFIG_ERROR    -105    //Ҫɾ������Ŀ��������
#define BASIC_CONFIG_NOT_EXIST      -106  //��������Ҫ�����ò�������
#define LOGICAL_PORT_NOT_EXIST      -107   //�߼��˿ڲ�����
#define PHYSICAL_PORT_NOT_EXIST     -108   //�߼��˿ڲ�����
#define VPN_NOT_VPWS_NOR_VPLS       -109
#define VC_NOT_MASTER_NOR_SLAVE     -110
#define VP_NOT_NNI_NOR_UNI          -111
#define CONFIG_NUM_IS_FULL          -112
#define VPWS_VP_NUM_IS_FULL         -113
#define CONFIG_NUM_IS_OUT_RANGE     -114
#define ALLOC_MEMORY_ERROR			-115
#define CONFIG_IS_IN_USE            -116

//PHYSICAL_PORT_PD
#define PHYSICAL_PORT_PD_PORT_UNEXIST   -131    //Ҫɾ����PORT_INDEX=%ld �Ľӿ��������ò�������
#define UNABLE_GET_C3_PORT   -132    //�ӿ�%d ����%d ��ȡоƬ��ʧ��!�˳�����

//ARP_PD
#define ARP_PD_LPORT_UNEXIST   -133    //INDEX=%ld IP_ADDR=%s ARP�������ڵ��߼��˿ڲ�����

//VC_PD
#define VC_PD_NOT_MASTER_NOR_SLAVE  -134    //Ҫ�޸�VC���üȲ�����Ҳ���Ǳ�

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
    /* sbi�ӿ�ע����ϵı�־��                                                 */
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
    /* �ӿ���λ slot ����.                                                     */
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
    /* �Ƿ��dciע����ϵı�־��                                               */
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

    NBB_VOID *data;                  /*TLVֵ�е�V,    ���ݿ�ʼ���KEYֵ��ԭʼ����*/

    NBB_ULONG key;                 /*HASH�㷨������KEYֵ*/

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
/* ����������*/
/*****************************************************************************/
#if 1

typedef struct dc_uni
{
    NBB_ULONG ntip[4];        /* �����������ARP*/
    NBB_ULONG nextport;      /* �����������ARP*/
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
/* Description: ȫ�����ã���Ŀ��1����                                                       */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    NBB_ULONG global_id;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_BASIC_DATA basic_cfg_cb;

    /***************************************************************************/
    /* CFM���á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_CFM_DATA cfm_cfg_data;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_THRES_DATA thres_data;

    /***************************************************************************/
    /* OAM���á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_OAM_DATA oam_data;

    /***************************************************************************/
    /* IP FRR ���á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_IPFRR_DATA ip_frr_data;

    /***************************************************************************/
    /* VPN FRR ���á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOB_CONF_VPNFRR_DATA vpn_frr_data;
} SPM_GLOBAL_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_CIR_EMU_CB                                                  */
/*                                                                           */
/* Description: ��·������ȫ�����ã���Ŀ��1����                              */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_KEY stCirEmuKey;

    /***************************************************************************/
    /* �����������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_PUB_LIMIT *pub_limit_cb;

    /***************************************************************************/
    /* ͬ��ʱ��Դ������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_SYN_CLK *syn_clk_cb;

    /***************************************************************************/
    /* ��������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_ELTRC_BOARD *eltrc_board_cb;

    /***************************************************************************/
    /* �������������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_REBTH_LIMIT *rebth_limit_cb;

    /***************************************************************************/
    /* ���ö��������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_MU_LIMIT *mu_limit_cb;

    /***************************************************************************/
    /* �߽�ͨ���������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_HIGH_ORD_PASS *high_ord_pass_cb;

    /***************************************************************************/
    /* �ͽ�ͨ���������á�                                                          */
    /***************************************************************************/
    ATG_DCI_CIR_EMU_LOW_ORD_PASS *low_ord_pass_cb;
    
} SPM_CIR_EMU_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MAC_RECYCLE_CB                                             */
/*                                                                           */
/* Description: MAC��ַ���������Ŀ��1����                                 */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    NBB_ULONG mac_recycle_id;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_MAC_WITHDRAW_DATA basic_cfg_cb;

} SPM_MAC_RECYCLE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_NF_CTRL_CB                                             */
/*                                                                           */
/* Description:��������������                                 */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    NBB_ULONG nf_ctrl_id;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_MAC_WITHDRAW_DATA basic_cfg_cb;

} SPM_NF_CTRL_CB;

/**STRUCT-********************************************************************/


/**STRUCT*********************************************************************/
/* Structure: SPM_QOS_PORT_CB                                           */
/*                                                                           */
/* Description: qos����˿�        */
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

    /* keyֵ: ����˿�*/
    NBB_USHORT port_key;

    /* LSP policy����*/
    AVLL_TREE lsp_tree;

    /*pw policy����*/
    AVLL_TREE pw_vrf_tree;

    /* pw policy����*/
    AVLL_TREE pw_vc_tree;

    AVLL_TREE group_tree;

} SPM_QOS_PORT_CB;






/**STRUCT-********************************************************************/


/**STRUCT*********************************************************************/
/* Structure: SPM_CLK_CB                                           */
/*                                                                           */
/* Description: ʱ��ȫ������        */
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
    /* keyֵ: vpws_id.                                                         */
    /***************************************************************************/
    NBB_ULONG clk_id_key;

    /***************************************************************************/
    /* ʱ�ӻ������á�                                                              */
    /***************************************************************************/
    ATG_DCI_GLOBAL_CLK_BASIC basic_clk_cb;

    /***************************************************************************/
    /* ��ʱ�����á�                                                           */
    /***************************************************************************/
    ATG_DCI_EXT_CLK  ext_clk_cb;

    /***************************************************************************/
    /* ��ʱԴʱ���������á�                                                           */
    /***************************************************************************/
    NBB_USHORT clk_pri_num; 
    ATG_DCI_TIMER_SRCCLK  clk_pri_cb[ATG_DCI_G_CLK_TIMER_SRCCLK_NUM];
    
    /***************************************************************************/
    /* ��ʱ�ӵ������á�                                                           */
    /***************************************************************************/
    NBB_USHORT t4_clk_num;
    ATG_DCI_EXTCLK_EXPORT  t4_clk_cb[ATG_DCI_G_CLK_EXTCLK_EXPORT_NUM];
    
     /***************************************************************************/
    /* ���ԴQL ���á�                                                           */
    /***************************************************************************/
     NBB_USHORT ql_out_num;
    ATG_DCI_OUTSRC_QL  ql_out_cb[ATG_DCI_G_CLK_OUTSRC_QL_NUM];  
     
} SPM_CLK_CB;

/**STRUCT*********************************************************************/
/* Structure: SPM_PTP_CB                                           */
/*                                                                           */
/* Description: ʱ��ͬ����������        */
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
    /* keyֵ: ptp_id_key.                                                         */
    /***************************************************************************/
    NBB_ULONG ptp_id_key;

    /***************************************************************************/
    /* ͬ���������á�                                                              */
    /***************************************************************************/
    ATG_DCI_CLK_SYNC_PARA sync_para_ptp_cb;

    /***************************************************************************/
    /* ����ʱ�����á�                                                           */
    /***************************************************************************/
    ATG_DCI_LOCAL_CLK  local_clk_ptp_cb;

    /***************************************************************************/
    /* 1PPS+TOD�ӿ����á�                                                           */
    /***************************************************************************/
    ATG_DCI_PPS_TOD_IF_DATA  pps_tod_if_ptp_cb;
} SPM_PTP_CB;

/**STRUCT+********************************************************************/
/* Structure: SPM_CMD_SWITCH_CB                                            */
/*                                                                           */
/* Description:�����������ÿ�                     */
/*                                                                           */
/* Added by zhangsen 2014-4-3                                            */
/*****************************************************************************/
typedef struct spm_cmd_switch_cb
{
    /***************************************************************************/
    /*keyֵ: ID:ʼ��Ϊ1                                                        */
    /***************************************************************************/
    NBB_ULONG key;

    /***************************************************************************/
    /*DNI-PW��������                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_DNIPW dni_pw_switch_cfg_cb;
    
    /***************************************************************************/
    /*MSP��������                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_MSP msp_switch_cfg_cb;
    
    /***************************************************************************/
    /*MSRP��������                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_MSRP msrp_switch_cfg_cb;
    
    /***************************************************************************/
    /*PW���൹������                                                           */
    /***************************************************************************/
    ATG_DCI_SWITCH_OVER_PW1TO1 pw1To1_switch_cfg_cb;
    
    /***************************************************************************/
    /*BYPASS��������                                                           */
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
    /* MASTERһ����Join���ƿ�                                                  */
    /***************************************************************************/
    SPM_MJ_JOIN_CB spm_dci_join_cb;

    SPM_SCK_JOIN_CB sck_join_cb;

    SPM_MTL_JOIN_CB mtl_join_cb;

    SPM_SOCKET_CB sck_cb;

    /***************************************************************************/
    /* List of SPM joins.  Each entry is an SPM_SJ_JOIN_CB.                    */
    /* ���ڴ��ڶ��Masterʹ��һ��Slave�������                                 */
    /*��˶���ÿ��Master����Ҫ����һ��                                         */
    /* SPM_SJ_JOIN_CB����ЩSPM_SJ_JOIN_CB�γ�һ������                          */
    /***************************************************************************/
    NBB_LQE spm_sbi_join_list;
    
    /***************************************************************************/
    /* proc_timer�Ƿ���active״̬��                                          */
    /***************************************************************************/
    NBB_BOOL proc_timer_active;

    /***************************************************************************/
    /* ���ڶ���Ϣ����Ķ�ʱ����                                                */
    /***************************************************************************/
#define PROC_SPM_TIMER_TIMEOUT  60000
    NBB_TIMER_DATA proc_timer;
    
    /***************************************************************************/
    /* proc_timer�Ƿ���active״̬��                                          */
    /***************************************************************************/
    NBB_BOOL proc_timer_clearcfg_active;

    /***************************************************************************/
    /* ���ڶ���Ϣ����Ķ�ʱ����                                                */
    /***************************************************************************/
#define PROC_TIMER_CLEARCFG_TIMEOUT  180000
    NBB_TIMER_DATA proc_timer_clearcfg;

    /***************************************************************************/
    /* ����״̬����Ļص���������.                                             */
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
    /* ָ����ˮ�š�                                                            */
    /***************************************************************************/
    NBB_ULONG instr_number;

    /***************************************************************************/
    /* �ӿ���λ��Ӧ�� join_cb. Ŀǰֻ�������ӿ�.                               */
    /***************************************************************************/
    SPM_SJ_JOIN_CB *sub_card_cb[MAX_SUB_CARD_NUM];

    /***************************************************************************/
    /*  mtl communicate data buffer.                                          */
    /***************************************************************************/
    NBB_BYTE data_buffer[SPM_MTL_DATA_BUFFER_SIZE];

    /***************************************************************************/
    /* �ӿ�����������    Added by xiaoxiang, 2012/9/11                         */
    /***************************************************************************/
    AVLL_TREE physical_port_tree;

    /***************************************************************************/
    /* �˿��߼�������    Added by xiaoxiang, 2012/9/11                         */
    /***************************************************************************/
    AVLL_TREE logical_port_tree;

    /***************************************************************************/
    /* ��·�ۺ���    Added by xiaoxiang, 2012/12/1                             */
    /***************************************************************************/
    AVLL_TREE lag_tree;

    /***************************************************************************/
    /* VC������        Added by xiaoxiang, 2012/9/18                           */
    /***************************************************************************/
    AVLL_TREE vc_tree;

    /***************************************************************************/
    /* VPWS������        Added by xiaoxiang, 2012/9/18                         */
    /***************************************************************************/
    AVLL_TREE vpws_tree;

    /***************************************************************************/
    /* VPLS������        Added by xiaoxiang, 2012/9/18                         */
    /***************************************************************************/
    AVLL_TREE vpls_tree;

    /***************************************************************************/
    /* OAM������       Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyoamid;

    /***************************************************************************/
    /* OAM rsvp����������Ϊkeyֵ�洢������      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyrsvptx;

    /***************************************************************************/
    /* OAM rsvp�շ�������Ϊkeyֵ�洢������      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyrsvprx;

    /***************************************************************************/
    /* OAM pw����Ϊkey�洢������      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebypw; 

    /***************************************************************************/
    /* OAM vs����Ϊkey�洢������      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebyindex;
    
    /***************************************************************************/
    /* OAM wrapping����Ϊkey�洢������      Added by huxi, 2014/1/21                    */
    /***************************************************************************/
    AVLL_TREE g_oamavltreebywrapping;

    /***************************************************************************/
    /* VPLS�鲥������        Added by xiaoxiang, 2013/1/12                     */
    /***************************************************************************/
    AVLL_TREE vpls_mc_tree;

    /***************************************************************************/
    /* VPLS��̬����������        Added by xiaoxiang, 2013/4/18                 */
    /***************************************************************************/
    AVLL_TREE vpls_uc_tree;

	/***************************************************************************/
    /* IGMP        Added by zhangzhm	, 2016/1/8                 */
    /***************************************************************************/
	AVLL_TREE vpls_l3_mc_tree;
    
    /***************************************************************************/
    /* ARP������        Added by xiaoxiang, 2012/11/10                         */
    /***************************************************************************/
    AVLL_TREE arp_tree;

    /***************************************************************************/
    /* IPv6 �ھӻ�����        Added by xiaoxiang, 2013/4/15                    */
    /***************************************************************************/
    AVLL_TREE ncache_tree;
    
    /***************************************************************************/
    /* �˿ھ�����        Added by xiaoxiang, 2013/03/15                        */
    /***************************************************************************/
    AVLL_TREE port_mirror_tree;

    /***************************************************************************/
    /* ȫ�����ã���Ŀ��1��        Added by xiaoxiang, 2013/04/11               */
    /***************************************************************************/
    SPM_GLOBAL_CB global_cb;

    /***************************************************************************/
    /* ��·������ȫ�����ã���Ŀ��1��        Added by xiaoxiang, 2013/06/24     */
    /*��Ϊ���ṹAdded by fansongbo, 2015/1/27*/
    /***************************************************************************/
    //SPM_CIR_EMU_CB cir_emu_cb;
    AVLL_TREE cir_emu_tree;
    
    /***************************************************************************/
    /* MAC��ַ���������Ŀ��1��        Added by xiaoxiang, 2013/04/18        */
    /***************************************************************************/
    SPM_MAC_RECYCLE_CB mac_recycle_cb;

    /***************************************************************************/
    /* ��������������        Added by fansongbo, 2013/08/12        */
    /***************************************************************************/
    SPM_MAC_RECYCLE_CB nf_ctrl_cb;
    
    /***************************************************************************/
    /* Bypass˫�鱣����        Added by xiaoxiang, 2013/07/16                  */
    /***************************************************************************/
    AVLL_TREE bypass_tree;

    /***************************************************************************/
    /* SWITCH VC����        Added by xiaoxiang, 2013/03/26                     */
    /***************************************************************************/
    AVLL_TREE switch_vc_tree;

    /***************************************************************************/
    /* MSP��������        Added by xiaoxiang, 2014/04/10                     */
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
    /* ���ֱ��·��*/
    /***************************************************************************/
    AVLL_TREE dcroute_tree;

    /***************************************************************************/
    /* ���loopback*/
    /***************************************************************************/
    AVLL_TREE lbroute_tree;

    /***************************************************************************/
    /* ��ŵ���·��*/
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
    /* ·��UNI ����*/
    /***************************************************************************/
    AVLL_TREE outuni;

    /***************************************************************************/
    /* ·���ڴ�*/
    /***************************************************************************/
    VRFUROUTE *routemem;

    //VRFUROUTE   g_uroute[200000];

    /***************************************************************************/
    /*�鲥 ·��*/
    /***************************************************************************/
    AVLL_TREE  mcroute_tree;

    /***************************************************************************/
    /*VRF ʵ��*/
    /***************************************************************************/
    AVLL_TREE  vrfins_tree;

    /***************************************************************************/
    /* ���ü���*/
    /***************************************************************************/
    AVLL_TREE rsvpcite;
    AVLL_TREE ldpcite;
	
    /***************************************************************************/
    /*�·�IPS��Ϣ��Ŀͳ��*/
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
    /* ���ds_domain·��*/
    /***************************************************************************/
    AVLL_TREE ds_domain_tree;

    /***************************************************************************/
    /* ���ds·��*/
    /***************************************************************************/
    AVLL_TREE ds_tree;

    /***************************************************************************/
    /* ����߼��˿�L3 INTF ds����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_logic_intf_tree;

    /***************************************************************************/
    /* ����߼��˿�L2 FLOW ds����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_logic_flow_tree;

    /***************************************************************************/
    /* ���L2 FLOW VPUNI ds����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_logic_univp_tree;

    /***************************************************************************/
    /* ����û�������*/
    /***************************************************************************/
    AVLL_TREE qos_user_group_tree;
    
     /***************************************************************************/
    /* ���ip nf����*/
    /***************************************************************************/
    AVLL_TREE ip_nf_tree;

    /***************************************************************************/
    /* ���nf aggr����*/
    /***************************************************************************/
    AVLL_TREE nf_aggr_tree;
    /***************************************************************************/
    /* ���VC ds����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_vc_tree;

    /***************************************************************************/
    /* ���VRF ds����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_vrf_tree;

    /***************************************************************************/
    /* ���ILM ds����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_ilm_tree;

    /***************************************************************************/
    /* ���FTN  qos����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_ftn_tree;

    /***************************************************************************/
    /* ���rx_lsp  qos����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_rx_lsp_tree;

    /***************************************************************************/
    /* ���tx_lsp  qos����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_tx_lsp_tree;

    /***************************************************************************/
    /* ���lsp  qos����*/
    /***************************************************************************/
    AVLL_TREE qos_ds_tunnel_tree;

    /***************************************************************************/
    /* �������˿�qos����*/
    /***************************************************************************/
    //SPM_QOS_PORT_CB qos_port_cb[MAX_SLOT_NUM][MAX_PHYSIC_PORT_NUM];

    /***************************************************************************/
    /* ���acl qos����*/
    /***************************************************************************/
    AVLL_TREE qos_acl_tree;

    /***************************************************************************/
    /* ���meter����*/
    /***************************************************************************/
    //NBB_ULONG flow_meter_id[MAX_METER_NUM];
#ifdef PTN690

    /***************************************************************************/
    /* ���meterͰ����*/
    /***************************************************************************/
    //NBB_ULONG meter_bukect_id[MAX_METER_BUKECT_NUM];

    /***************************************************************************/
    /* ���color action����*/
    /***************************************************************************/
    //NBB_ULONG color_action_id[MAX_COLOR_ACTION_NUM];

    /***************************************************************************/
    /* ���txlsp_car qos����*/
    /***************************************************************************/
    AVLL_TREE qos_txlsp_car_tree;

    /***************************************************************************/
    /* ���pw_car qos����*/
    /***************************************************************************/
    AVLL_TREE qos_pw_car_tree;
#endif  
    /***************************************************************************/
    /* ��Ÿ���������ȫ��ruleID����*/
    /***************************************************************************/
    //NBB_ULONG flow_rule_id[MAX_FLOW_CLASSIFY_RULE_ID];


    /***************************************************************************/
    /* Ϊ�������������ƥ��˿�acl id */
    /***************************************************************************/
    //NBB_ULONG g_classify_port_acl_id[MAX_CLASSIFY_PORT_ACL_ID_NUM];

    /***************************************************************************/
    /* Ϊ�������������߼�acl id */
    /***************************************************************************/
    //NBB_ULONG g_classify_high_acl_id[MAX_CLASSIFY_HIGH_ACL_ID_NUM];

    /***************************************************************************/
    /* Ϊ�������������ƥ����̫��acl id */
    /***************************************************************************/
    //NBB_ULONG g_classify_eth_acl_id[MAX_CLASSIFY_ETH_ACL_ID_NUM];


    /***************************************************************************/
    /* ���twamp_ipv4����keyֵ                                                 */
    /***************************************************************************/
    AVLL_TREE twamp_ipv4_tree;

    /***************************************************************************/
    /* ���twamp_ipv6����keyֵ                                                 */
    /***************************************************************************/
    AVLL_TREE twamp_ipv6_tree;
    
    /***************************************************************************/
    /* ���filter*/
    /***************************************************************************/
    AVLL_TREE qos_filter_tree;

    /***************************************************************************/
    /* ����ϻ����������� qos����*/
    /***************************************************************************/
    AVLL_TREE qos_flow_classify_tree;

    /***************************************************************************/
    /* ����ϻ��û� qos����*/
    /***************************************************************************/
    AVLL_TREE qos_up_usr_tree;

    /***************************************************************************/
    /* ����ϻ������ qos����*/
    /***************************************************************************/
    AVLL_TREE qos_flow_up_usr_tree;

    /***************************************************************************/
    /* ���classify qos����*/
    /***************************************************************************/
    AVLL_TREE qos_classify_tree;

    /***************************************************************************/
    /* ���action qos����*/
    /***************************************************************************/
    AVLL_TREE qos_action_tree;

    /***************************************************************************/
    /* ���policy qos����*/
    /***************************************************************************/
    AVLL_TREE qos_policy_tree;

    /***************************************************************************/
    /* ���policy qos����*/
    /***************************************************************************/
    AVLL_TREE qos_defend_policy_tree;

    /***************************************************************************/
    /* ���policy qos����*/
    /***************************************************************************/
    AVLL_TREE qos_defend_tree;

    /***************************************************************************/
    /* ���wred qos����*/
    /***************************************************************************/
    AVLL_TREE qos_wred_tree;

    /***************************************************************************/
    /* ���wred qos����*/
    /***************************************************************************/
    AVLL_TREE port_wred_tree;

    /*����HQOS�Ĳ�λ*/
    AVLL_TREE hqos_tree;

    /*ֻ֧��4����λ��HQOS*/
    NBB_BYTE hqos_port_index[4];

    /***************************************************************************/
    /* ���vrf qos ʵ������*/
    /***************************************************************************/
    AVLL_TREE qos_vrf_tree;

    /***************************************************************************/
    /* ���Hqos LSPID*/
    /***************************************************************************/
    NBB_ULONG hqos_lspId;

    /***************************************************************************/
    /* ���Hqos LSP ��Դ*/
    /***************************************************************************/
    NBB_BYTE hqos_lsp_pool[HQOS_MAX_LSP_ID];

    /***************************************************************************/
    /* ���Hqos pwId*/
    /***************************************************************************/
    NBB_ULONG hqos_pwId;

    /***************************************************************************/
    /* ���Hqos PW ��Դ*/
    /***************************************************************************/
    NBB_BYTE hqos_pw_pool[HQOS_MAX_PW_ID];

    /***************************************************************************/
    /* ��ŷ�������ÿ��Э���rule����*/
    /***************************************************************************/
    NBB_BYTE qos_defend_num[SIGNAL_NUM + 1];
    
    /***************************************************************************/
    /* ��ŷ�������ÿ��Э���ruleƫ��*/
    /***************************************************************************/
    NBB_USHORT qos_defend_offset[SIGNAL_NUM + 1];

    /*************��ʱ��********************/

    NBB_TIMER_DATA proc_bfd_timer;
    NBB_TIMER_DATA proc_bfd_timer1;
    NBB_TIMER_DATA proc_bfd_timer2;
    NBB_TIMER_DATA proc_bfd_syncheck;

    /***************************************************************************/
    /* proc_bfd_syncheck�Ƿ���active״̬��                                          */
    /***************************************************************************/
    NBB_BOOL proc_bfdsyncheck_active;

    /***************************************************************************/
    /* VPLS�鲥CUDֵ��ȫ��Ψһ��64K��                                   */
    /***************************************************************************/
    NBB_INT mc_cud[ATG_DCI_CUD_NUM];

    /***************************************************************************/
    /* VC_NHI_ID ȫ��Ψһ��64K��                                   */
    /***************************************************************************/
    NBB_UINT vc_nhi_id[SPM_VC_NHI_ID_NUM];

    /***************************************************************************/
    /* ���̲�λ�š�                                   */
    /***************************************************************************/
    NBB_BYTE local_slot_id;

    /***************************************************************************/
    /* C3������                                   */
    /***************************************************************************/
    NBB_BYTE c3_num;

    SPM_CLK_CB clk_glob_cb;

	/***************************************************************************/
    /* ʱ��ͬ���������ã���Ŀ��1��        Added by zengliang, 2013/06/28               */
    /***************************************************************************/
    SPM_PTP_CB ptp_cb;
    
    /***************************************************************************/
    /* �����˿���Ϣ��0xFF��ʾ�޶˿���Ϣ                                   */
    /***************************************************************************/
    NBB_BYTE dev_port_info[SPM_PORT_MAX_NUM][SPM_SLOT_MAX_NUM];

    NBB_BYTE rcu_stat;

	/***************************************************************************/
    /* ASSOCIATE_IF��       Added by zhangzhm, 2013/11/20                    */
    /***************************************************************************/
    AVLL_TREE associate_if_tree;
    
    /***************************************************************************/
    /* ���113 mac vpls ״̬��ѯ�յ���״̬�ϱ���Ϣ*/
    /*����T /L /V*/
    /***************************************************************************/
    NBB_BYTE mac_vpls_sta_info[20 + 10 * VPLS_MAC_STA_MAX_NUM];
    
    /***************************************************************************/
    /* ���124 mac vpls by mac״̬��ѯ�յ���״̬�ϱ���Ϣ*/
    /*����T /L /V*/
    /***************************************************************************/
    NBB_BYTE mac_vpls_mac_sta_info[12];   
     
    /***************************************************************************/
    /* ΪMSP����Ԥ����64��frr group id*/
    /***************************************************************************/
    NBB_USHORT msp_frr_group_id[ATG_DCI_MSP_NUM];

    /***************************************************************************/
    /* �����������ã���Ŀ��1��        Added by xiaoxiang, 2014/04/3           */
    /***************************************************************************/
    SPM_CMD_SWITCH_CB cmd_switch_cb;
    
    /***************************************************************************/
    /* ���4 ����״̬��ѯ�յ���״̬�ϱ���Ϣ*/
    /*����T /L /V*/
    /***************************************************************************/
    NBB_BYTE prot_sta_info[PROT_STA_INFO_MAX_NUM];
    
    
} SPM_SHARED_LOCAL;

/**STRUCT-********************************************************************/

/*****************************************************************************/
/* N-BASE priority.                                                          */
/*****************************************************************************/
#define SPM_NBASE_PRIORITY               0x40

/*****************************************************************************/
/* �����ڴ����ͣ����������SHARED_LOCAL�����п��ƿ�                          */
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

#if 1 /* BFD���ÿ��ڴ�� */
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
/*QOS���ÿ��ڴ��*/
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



/*QOS�Ӷ����ڴ��*/
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
/*NF���ÿ��ڴ��*/
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
/* ���������ͣ�������������õ��ľ����                                    */
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
/* Description: ���ڵ����á�                                                     */
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
    /* keyֵ����ID��                                                           */
    /***************************************************************************/
    NBB_USHORT flow_id_key;

    /***************************************************************************/
    /* �����á�                                                                */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_FLOW_DATA flow_cfg_cb;

} SPM_FLOW_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_IPV4_NODE_CB                                                    */
/*                                                                           */
/* Description: ipv4�����á�                                                     */
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
    /* keyֵ��ip��ַ��                                                         */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV4_DATA ipv4_key;

    /***************************************************************************/
    /* Ipv4��ַ����L3��                                                        */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV4_DATA ipv4_cfg_cb;

} SPM_IPV4_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_IPV6_NODE_CB                                                    */
/*                                                                           */
/* Description: ipv6�����á�                                                     */
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
    /* keyֵ��ip��ַ��                                                         */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV6_DATA ipv6_key;

    /***************************************************************************/
    /* Ipv4��ַ����L3��                                                        */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_IPV6_DATA ipv6_cfg_cb;

} SPM_IPV6_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MC_MAC_NODE_CB                                                    */
/*                                                                           */
/* Description: �鲥���ַL3��                                                     */
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
    /* keyֵ��mac��ַ��                                                        */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_MC_MAC_DATA mac_key;

    /***************************************************************************/
    /* Ipv4��ַ����L3��                                                        */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_MC_MAC_DATA mc_mac_cfg_cb;

} SPM_MC_MAC_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VIP_VMAC_NODE_CB                                           */
/*                                                                           */
/* Description: ����MAC��IP��ַ����L3��                                      */
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
    /* keyֵ��ip+mac��ַ��                                                     */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA ip_mac_key;

    /***************************************************************************/
    /* Ipv4��ַ����L3��                                                        */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA vip_vmac_cfg_cb;

} SPM_VIP_VMAC_NODE_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_INFO_CB                                           */
/*                                                                           */
/* Description: ������������ֵ���Ƿ�������QoS����Ϣ                       */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                            */
/*****************************************************************************/
typedef struct spm_port_drv_info_cb
{
    /***************************************************************************/
    /* оƬ�����.        */
    /***************************************************************************/
    NBB_INT port_phy_id;

    /***************************************************************************/
    /* ��λ��.        */
    /***************************************************************************/
    NBB_INT slot_id;

    /***************************************************************************/
    /* �˿�MAC.        */
    /***************************************************************************/
    NBB_BYTE mac[ATG_DCI_MAC_LEN];

    /***************************************************************************/
    /* �˿�TPID.        */
    /***************************************************************************/
    NBB_INT tpid;

    /***************************************************************************/
    /* �˿�Link״̬.        */
    /***************************************************************************/
    NBB_INT link_state;

    /***************************************************************************/
    /* �˿ڿ�/��״̬.        */
    /***************************************************************************/
    NBB_INT on_off_en;

    /***************************************************************************/
    /* �˿ڵ�����ٿ�.        */
    /***************************************************************************/
    NBB_INT ingress_rc_kbps;

    /***************************************************************************/
    /* �˿ڵĳ����ٿ�.        */
    /***************************************************************************/
    NBB_INT egress_rc_kbps;

    /***************************************************************************/
    /* �˿�����.        */
    /***************************************************************************/
    NBB_INT speed;

    /***************************************************************************/
    /* �˿���Э��ʹ��.        */
    /***************************************************************************/
    NBB_INT nego_en;

    /***************************************************************************/
    /* �˿�˫��.        */
    /***************************************************************************/
    NBB_INT duplex;

    /***************************************************************************/
    /* �˿�PHY LOOPBACK.        */
    /***************************************************************************/
    NBB_INT phy_local_lb;
    NBB_INT phy_remote_lb;

    /***************************************************************************/
    /* �˿�MAC LOOPBACK.        */
    /***************************************************************************/
    NBB_INT mac_local_lb;
    NBB_INT mac_remote_lb;
    
} SPM_PORT_DRV_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_INFO_CB                                           */
/*                                                                           */
/* Description: ������������ֵ���Ƿ�������QoS����Ϣ                       */
/*                                                                           */
/* Added by xiaoxiang, 2012/10/11                                            */
/*****************************************************************************/
typedef struct spm_flow_info_cb
{
    /***************************************************************************/
    /* �������ص�INTF pos_id, ���ΪL2ҵ��ָ��INTF.        */
    /***************************************************************************/
    NBB_USHORT pos_id;

    /***************************************************************************/
    /* �Ƿ�������QoS.        */
    /***************************************************************************/
    NBB_BYTE if_qos;

} SPM_FLOW_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_MC_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPLS�鲥�����һЩ����                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/2/5                                            */
/*****************************************************************************/
typedef struct spm_vpls_mc_port_info_cb
{
    /***************************************************************************/
    /* �鲥������NNI/UNI������pos_id.        */
    /***************************************************************************/
    NBB_ULONG vp_pos_id;

    /***************************************************************************/
    /* �鲥������ΪNNI/UNI�����cud.        */
    /***************************************************************************/
    //NBB_INT cud;

    /***************************************************************************/
    /* �鲥������NNI/UNI���ڵĲ�λ��.        */
    /***************************************************************************/
    NBB_BYTE slot_id;
} SPM_VPLS_MC_PORT_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPN_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPN�����һЩ�鲥���ú�Bypass��Ҫ������                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/14                                            */
/*****************************************************************************/
typedef struct spm_phy_port_info_cb
{
    /*************************************************************************/
    /* �ӿ�����:0/1/2/3/4/5/6=�����̫���ӿ�/������ƿ�/POS/CPOS/E-CPOS/����ӿ�/ͬ�����ڡ� */
    /*************************************************************************/
    NBB_BYTE port_type;                  /* One of: ATG_DCI_INTERFACE_TYPE;  */

    /*************************************************************************/
    /* ��λ��=1-10��                                                         */
    /*************************************************************************/
    NBB_BYTE slot;

    /*************************************************************************/
    /* �ӿ��ţ�0/1/2=���ӿ�/�ӿ�1/�ӿ�2��                                    */
    /*************************************************************************/
    NBB_BYTE sub_board;

    /*************************************************************************/
    /* �˿ں�(�ڱ����ϴ�1��ʼ����)(����ֱ������ӿڸ��ֶ���Ч����������ӿ���Ч)�� */
    /*************************************************************************/
    NBB_BYTE port;

} SPM_PHY_PORT_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPN_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPN�����һЩ�鲥���ú�Bypass��Ҫ������                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/14                                            */
/*****************************************************************************/
typedef struct spm_vpn_port_info_cb
{
    /***************************************************************************/
    /* VP��pos id.                                       */
    /***************************************************************************/
    NBB_ULONG vp_idx;
    
    /***************************************************************************/
    /* �鲥������NNI/UNI������pos_id.        */
    /***************************************************************************/
    //NBB_ULONG vp_mc_pos_id;

    /***************************************************************************/
    /* ΪSWITCH_VC����VC�����next_hop_id.        */
    /* Ҳ����VPWS/VPLS��next_hop_id.    fansongbo 2015.9.17 ���mantis 84426.        */
    /***************************************************************************/
    NBB_UINT next_hop_id;

    /***************************************************************************/
    /* �鲥������NNI/UNI���ڵĲ�λ��.        */
    /***************************************************************************/
    NBB_BYTE slot_id;

    /***************************************************************************/
    /* �鲥������NNI/UNI�Ķ˿�����.        */
    /***************************************************************************/
    NBB_BYTE port_type;

    /***************************************************************************/
    /* C3����NNI/UNI����Ӧ��PORT_INDEX.        */
    /***************************************************************************/
    NBB_ULONG port_index;
    
    /***************************************************************************/
    /* �鲥�������еĳ���index. ����: ECMP���8�����ڣ�FRR������2������       */
    /***************************************************************************/
    NBB_ULONG port_index_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
    /* �鲥����������NNI/UNI���ڵĲ�λ��.        */
    /***************************************************************************/
    NBB_BYTE slot_id_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
    /* �鲥����������NNI/UNI�Ķ˿�����.        */
    /***************************************************************************/
    NBB_BYTE port_type_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
   /* �鲥����������NNI/UNI�Ķ˿�.        */
    /***************************************************************************/
    NBB_USHORT port_id_mc[ATG_DCI_ECMP_PORT_NUM];

    /***************************************************************************/
    /* �鲥��������NNI����index�ĸ���       */
    /***************************************************************************/
    NBB_BYTE port_num;
    
    /***************************************************************************/
    /* ������id                                       */
    /***************************************************************************/
    NBB_USHORT frr_group_id;
    
    /***************************************************************************/
    /* ����VPWS/VPLS��next_hop_id_p.    fansongbo 2015.9.17 ���mantis 84426.        */
    /***************************************************************************/
    NBB_UINT next_hop_id_p; 
	NBB_UINT next_hop_counter_id;
	NBB_UINT next_hop_counter_id_p;
	
} SPM_VPN_PORT_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_VC_INFO_CB                                           */
/*                                                                           */
/* Description: VCά����һЩ���ã�������������                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/2/5                                            */
/*****************************************************************************/
typedef struct spm_vc_info_cb
{
    /***************************************************************************/
    /* VPN���ͣ�1/2=VPWS/VPLS.                                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /* VPN ID��VPWS=[1-16K] VPLS=[1-4K].                                       */
    /***************************************************************************/
    NBB_USHORT vpn_id;

    /***************************************************************************/
    /* ָ�����ñ��ã���/��=1/2.                                       */
    /***************************************************************************/
    NBB_USHORT if_main;

    /***************************************************************************/
    /* VC����VP��pos id.                                       */
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
    /*  �߼��˿�index.                                       */
    /***************************************************************************/
    NBB_ULONG port_index;

    /***************************************************************************/
    /*  �Ƿ�������QoS.                                       */
    /***************************************************************************/
    NBB_BYTE if_qos;

    /***************************************************************************/
    /*  VC��Ӧ����һ��ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_id;

    /***************************************************************************/
    /*  VC��Ӧ����һ��������ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_counter_id;

    NBB_USHORT frr_group_id;
    
    NBB_ULONG drv_tunnel_id;
    
    /*************************************************************************/
    /* LSPѡ��0/1=FTN/CR-LSP.                                              */
    /*************************************************************************/
    NBB_BYTE lsp_option;                        /* One of: ATG_DCI_LSP_TYPE; */ 
    
} SPM_VC_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_LOGICAL_PORT_INFO_CB                                       */
/*                                                                           */
/* Description: �߼��˿�ά����һЩ���ã�������������                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/13                                            */
/*****************************************************************************/
typedef struct spm_logical_port_info_cb
{

    /***************************************************************************/
    /* �������ص�VP pos_id�����Ϊ0˵���˶˿�δ�����뵽ĳVPWS/VPLS.            */
    /***************************************************************************/
    NBB_ULONG vp_idx;

    /***************************************************************************/
    /* vp_idx. ΢���index                                      */
    /***************************************************************************/
    //NBB_ULONG vp_api_idx;

    /***************************************************************************/
    /* VPN���ͣ�1/2=VPWS/VPLS.                                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /* VPN ID��VPWS=[1-16K] VPLS=[1-4K].                                       */
    /***************************************************************************/
    NBB_USHORT vpn_id;

    /***************************************************************************/
    /* �������ص�INTF pos_id, ���ΪL3ָ����INTF.        */
    /***************************************************************************/
    NBB_USHORT intf_pos_id;

    /***************************************************************************/
    /*  UNI��Ӧ����һ��ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_id;

    /***************************************************************************/
    /*  MSP ����ʱUNI��Ӧ�ı�����һ��ID.                                       */
    /***************************************************************************/
    NBB_ULONG p_next_hop_id;    

    /***************************************************************************/
    /*  UNI��Ӧ����һ��counter ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_counter_id;

    /***************************************************************************/
    /* ������������ֵ���Ƿ�������QoS����Ϣ.        */
    /***************************************************************************/
    SPM_FLOW_INFO_CB flow_info_cb;
	
    /*  VRRP��MAC��Ӧ��pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG ulVrrpMacPosId[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /*  VRRP ipv6��MAC��Ӧ��pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG vrrp_ipv6_posid[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];    

    /*  VRRP��MAC��Ӧ��pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG vrrp_ipv4_posid2[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /*  VRRP ipv6��MAC��Ӧ��pos_id.                                       */
    /***************************************************************************/
    NBB_ULONG vrrp_ipv6_posid2[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];     
} SPM_LOGICAL_PORT_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: PORT����                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_port_info_cb
{
    /***************************************************************************/
    /*  C3оƬ��.                                       */
    /***************************************************************************/
    NBB_INT unit_id;
    
    /***************************************************************************/
    /* �˿����ͣ�0/1/2/3=�ӿ�/�ӽӿ�/LAG/������̫����.                         */
    /***************************************************************************/
    NBB_BYTE port_type;

    /***************************************************************************/
    /*  port_id.                                       */
    /***************************************************************************/
    NBB_USHORT port_id;
    
    /***************************************************************************/
    /*  ����˿ں�.                                       */
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
    /*  ��¼LAG�ڵ�һ����Ա�˿ڵĲ�λ�������鲥ʱFE1600��ɾ��λ.               */
    /***************************************************************************/
    NBB_BYTE lag_slot_id;

    NBB_USHORT lag_port_id;

    /***************************************************************************/
    /*  VE�ڵĲ�λ.                                       */
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
/* Description: PORT����                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_vp_info_cb
{
    /***************************************************************************/
    /* ҵ������ 1/2=VPWS/VPLS.                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /*  �������ص�vp_api_idx.                                       */
    /***************************************************************************/
    NBB_USHORT vp_id;

    /***************************************************************************/
    /*  UNI ����߼��˿�index.                                       */
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
    /*  ��ǩ.                                       */
    /***************************************************************************/
    NBB_ULONG pw_rx_label;
    NBB_ULONG pw_tx_label;
    NBB_ULONG lsp_rx_label;
    NBB_ULONG lsp_tx_label;

    /***************************************************************************/
    /*  lsp����: 1/2=tunnel/lsp.                                       */
    /***************************************************************************/
    NBB_BYTE lsp_type;

    /***************************************************************************/
    /*  VC��Ӧ����һ��ID.                                       */
    /***************************************************************************/
    NBB_ULONG next_hop_id;
                                           
} SPM_VP_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VP_INFO_REFRESH_CB                                           */
/*                                                                           */
/* Description: PORT����                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_vp_info_refresh_cb
{
    /***************************************************************************/
    /* ҵ������ 1/2/4=VPWS/VPLS/MS_PW.                         */
    /***************************************************************************/
    NBB_BYTE vpn_type;

    /***************************************************************************/
    /* ָ��VC���ñ��ã���/��=1/2.                                       */
    /***************************************************************************/
    NBB_BYTE if_main;

    /***************************************************************************/
    /* VPN ID��VPWS=[1-16K] VPLS=[1-4K].                                       */
    /***************************************************************************/
    NBB_USHORT vpn_id;

    /***************************************************************************/
    /* VC��keyֵ���������peer_ip.                                       */
    /***************************************************************************/
    ATG_DCI_VC_KEY vc_key;

	/***************************************************************************/
    /* vc����index.                                       */
    /***************************************************************************/
	NBB_ULONG port_index;

	/***************************************************************************/
    /* vpls��VP�鲥ID.                                       */
    /***************************************************************************/
	NBB_USHORT mc_id;
} SPM_VP_INFO_REFRESH_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MSP_PROT_INFO_CB                                       */
/*                                                                           */
/* Description: MSP���������˿ڵ�ʱ϶��Ϣ������KEYֵ���Ƿ񱣻�               */
/*                                                                           */
/* Added by fansongbo , 2014/12/27                                            */
/*****************************************************************************/
typedef struct spm_msp_prot_info_cb
{

    /***************************************************************************/
    /* �Ƿ��б�����0/1=�ޱ���/�б���.                                         */
    /***************************************************************************/
    NBB_BYTE if_prot;

    /***************************************************************************/
    /* 2Mʱ϶��Ӧ������ӿ����ÿ�KEYֵ                                      */
    /***************************************************************************/
    NBB_ULONG ces_key;

} SPM_MSP_PROT_INFO_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VP_INFO_REFRESH_CB                                           */
/*                                                                           */
/* Description: PORT����                         */
/*                                                                           */
/* Added by xiaoxiang, 2013/3/18                                            */
/*****************************************************************************/
typedef struct spm_vpws_nni_info_cb
{
    /***************************************************************************/
    /* VC��keyֵ���������peer_ip.                                       */
    /***************************************************************************/
    ATG_DCI_VC_KEY vc_key;

    /***************************************************************************/
    /*  �߼��˿�index.                                       */
    /***************************************************************************/
    NBB_ULONG port_index;

    /***************************************************************************/
    /* vc��Ӧ��CRLSPTX��keyֵ.                                       */
    /***************************************************************************/
    ATG_DCI_CR_LSP_KEY rsvpkey;

    NBB_USHORT frr_group_id;

    /***************************************************************************/
    /*NNI��VP_Idx.                                       */
    /***************************************************************************/
    NBB_ULONG vp_idx;
} SPM_VPWS_NNI_INFO_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PHYSICAL_PORT_CB                                           */
/*                                                                           */
/* Description: �ӿ��������á�                                               */
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

    /* keyֵ: index */
    NBB_ULONG port_index_key;

    /* �������á�*/
    ATG_DCI_PHY_PORT_BASIC_DATA *basic_cfg_cb;

    /* ��̫���ӿ��������á�*/
    ATG_DCI_PHY_PORT_PHY_DATA *phy_cfg_cb;

    /* POS�ӿ���·�����á� */
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pos_link_cfg_cb;

    /* SDH�ο������á� */
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *sdh_sovhd_cfg_cb;

    /* CES�ӿ����á� */
    ATG_DCI_PHY_PORT_CES_DATA *ces_cfg_cb;

    /* CEP�ӿڿ������á� */
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *cep_ovhd_cfg_cb;

    /***************************************************************************/
    /* ��̫��EFM OAM.                                                         */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_EFM_OAM_DATA *efm_oam_cfg_cb;

    /***************************************************************************/
    /* ʱ��ͬ���˿�����.                                                       */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *time_sync_cfg_cb;

    /***************************************************************************/
    /* ��̫���ӿڱ�������.                                                     */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *msg_suppr_cfg_cb;

    /***************************************************************************/
    /* ��̫���ӿ���������.                                                     */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_FLOW_SHAPE *flow_shape_cfg_cb;

    /***************************************************************************/
    /* ��̫�����ڶ��е��Ȳ���.                                                 */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *schedule_policy_cfg_cb;

    /***************************************************************************/
    /* ��̫�����ڶ���ӵ������.                                                 */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_CONGEST_POLICY *congest_policy_cfg_cb;

    /***************************************************************************/
    /* IPV4������ʹ������IPV4 NetFlow.                                         */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_IPV4NF_DATA *ipv4_nf_cfg_cb;

    /***************************************************************************/
    /* IPV4���������� IPV4 NetFlow sampler.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *ip_sample_cfg_cb;

    /***************************************************************************/
    /* IPV6������ʹ������ IPV6 NetFlow.                                         */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_IPV6NF_DATA *ipv6_nf_cfg_cb;

    /***************************************************************************/
    /* �ӿ�ͨ������.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_COMMON_DATA *common_cfg_cb;

    /***************************************************************************/
    /* SDHͨ����������.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *sdh_tovhd_cfg_cb;

    /***************************************************************************/
    /* ALS����.                                    */
    /***************************************************************************/
    ATG_DCI_PHY_PORT_ALS_DATA *als_cfg_cb;

    /***************************************************************************/
    /* �⹦����������.                                    */
    /***************************************************************************/
    //ATG_DCI_PHY_PORT_POWER_THRESHOLD *threshold_cfg_cb;    
    
} SPM_PHYSICAL_PORT_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: BMU_TERMINAL_IF_CB                                             */
/*                                                                           */
/* Description: �ս��ӽӿ����ÿ顣                                           */
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

    /* �ս��ӽӿڡ� */
    ATG_DCI_LOG_PORT_VLAN terminal_if_cfg;

    /* ��������ֵ ����VE��LAGʱ����Ϊ����ƬC3����������ֵ*/
    NBB_ULONG intf_pos_id;

      /* VE����LAGʱ  �ڶ�ƬC3��������ֵ������������޴���Ϣ */
    NBB_ULONG intf2_pos_id;  

} SPM_TERMINAL_IF_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_DIFF_SERV_CB                                             */
/*                                                                           */
/* Description:DIFF SERV���ÿ顣                                           */
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

    /* ��������ֵ */
    //NBB_ULONG pos_id;

} SPM_DIFF_SERV_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_FLOW_DIFF_SERV_CB                                             */
/*                                                                           */
/* Description:FLOW_DIFF SERV���ÿ顣                                           */
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

    /* ��������ֵ */
    //NBB_ULONG pos_id;

} SPM_FLOW_DIFF_SERV_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_INCLASSIFY_QOS_CB                                             */
/*                                                                           */
/* Description:INCLASSIFY_QOS���ÿ顣                                           */
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

    /* ��������ֵ */
    //NBB_ULONG pos_id;

} SPM_INCLASSIFY_QOS_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_TRAFFIC_FILTER_CB                                             */
/*                                                                           */
/* Description:TRAFFIC_FILTER���ÿ顣                                           */
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

    /* ��������ֵ */
    //NBB_ULONG pos_id;

} SPM_TRAFFIC_FILTER_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PHYSICAL_PORT_CB                                           */
/*                                                                           */
/* Description: �˿��߼����á�                                                 */
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

    /* keyֵ: index */
    NBB_ULONG port_index_key;

    /* �������á�*/
    ATG_DCI_LOG_PORT_BASIC_DATA *basic_cfg_cb;

    /* �������á�*/
    ATG_DCI_LOG_PORT_PHY_DATA *phy_cfg_cb;

    /* �߼�����L3 */
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *logic_l3_cfg_cb;

    /* �߼�����L2 */
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *logic_l2_cfg_cb;

    /* ���������L2 */
    //ATG_DCI_LOG_PORT_FLOW_DATA *flow_cfg_cb;
    //NBB_USHORT flow_num;
    //ATG_DCI_LOG_PORT_FLOW_DATA *flow_cfg_cb[ATG_DCI_LOG_PORT_FLOW_NUM];

    /* �߼�����CES */
    ATG_DCI_LOG_PORT_CES_DATA *ces_cfg_cb;

    /* Ipv4��ַ����L3 */
    NBB_USHORT ipv4_num;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_cfg_cb[ATG_DCI_LOG_PORT_IPV4_NUM];

    /* IPv6��ַ����L3 */
    NBB_USHORT ipv6_num;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_cfg_cb[ATG_DCI_LOG_PORT_IPV6_NUM];

    /* �鲥���ַL3 */
    NBB_USHORT mc_ipv4_num;
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *mc_ipv4_cfg_cb[ATG_DCI_LOG_PORT_MC_IPV4_NUM];

    /* ����MAC��IP��ַ����L3 */
    NBB_USHORT vip_vmac_num;
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *vip_vmac_cfg_cb[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /***************************************************************************/
    /* VE MAC��ַ����.                                                         */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_VE_MAC_DATA *ve_mac_cfg_cb;

    /***************************************************************************/
    /* Diff-Serv����.                                                          */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_cfg_cb;

    /***************************************************************************/
    /* Diff-Serv����.                                              */
    /***************************************************************************/
    NBB_USHORT diff_serv_num;
    AVLL_TREE diff_serv_tree;

    /***************************************************************************/
    /* �����Diff-Serv����L2.                                                  */
    /***************************************************************************/
    //NBB_USHORT flow_diff_serv_num;
    //ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_cfg_cb[ATG_DCI_LOG_FLOW_DIFF_SERV_NUM];
    //ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_cfg_cb;

    /***************************************************************************/
    /* �����Diff-Serv����L2.                                                  */
    /***************************************************************************/
    NBB_USHORT flow_diff_serv_num;
    AVLL_TREE flow_diff_serv_tree;

    /***************************************************************************/
    /* �ϻ�����������QOS��������L3.                                                  */
    /***************************************************************************/
    //ATG_DCI_LOG_PORT_INCLASSIFY_QOS *inclassify_qos_cfg_cb;

    /***************************************************************************/
    /* �ϻ�����������QOS��������                                                 */
    /***************************************************************************/
    NBB_USHORT inclassify_qos_num;
    AVLL_TREE inclassify_qos_tree;

    /***************************************************************************/
    /* �ϻ��û�QOS��������L3.                                                  */
    /***************************************************************************/
    //ATG_DCI_LOG_UP_USER_QOS_POLICY *up_user_qos_cfg_cb;

    /***************************************************************************/
    /* �ϻ��û���QOS��������.                                                  */
    /***************************************************************************/
    ATG_DCI_LOG_UP_USER_GROUP_QOS *up_user_group_qos_cfg_cb;

    /***************************************************************************/
    /* �»��û�����QOS��������L3.                                              */
    /***************************************************************************/
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *down_user_qos_cfg_cb;

    /***************************************************************************/
    /* �»��û���QOS����L3.                                                    */
    /***************************************************************************/
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *down_user_group_qos_cfg_cb;

    /***************************************************************************/
    /* ������ϻ��û�QOS��������L2.                                            */
    /***************************************************************************/
    //ATG_DCI_LOG_FLOW_UP_USER_QOS *flow_up_user_qos_cfg_cb;

    /***************************************************************************/
    /* IPV6�鲥���ַL3.                                              */
    /***************************************************************************/
	NBB_USHORT mc_ipv6_num;
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *mc_ipv6_cfg_cb[ATG_DCI_LOG_PORT_MC_IPV6_NUM];

    /***************************************************************************/
    /* �˿��߼�����-����������                                                  */
    /***************************************************************************/
    NBB_USHORT traffic_filter_num;
    AVLL_TREE traffic_filter_tree;

    /***************************************************************************/
    /* Diff-Serv����L2��VP�»�UNI�ࣩ.                                              */
    /***************************************************************************/
    ATG_DCI_LOG_PORT_DS_L2_DATA *ds_l2_cfg_cb;

    /***************************************************************************/
    /* �ս��ӽӿڡ�.                                              */
    /***************************************************************************/
    NBB_USHORT terminal_if_num;
    //ATG_DCI_LOG_PORT_TERMINAL_IF *terminal_if_cfg_cb[ATG_DCI_LOG_TERMINAL_IF_NUM];
    AVLL_TREE terminal_if_tree;

    /* ����MAC��IP��ַ����L3 */
    NBB_USHORT vipv6_vmac_num;
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *vipv6_vmac_cfg_cb[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];    

    /***************************************************************************/
    /*  C3оƬ��.                                       */
    /***************************************************************************/
    NBB_INT unit_id;

    /***************************************************************************/
    /* ��ʾC3����˿ڣ�VE��index��LAG�˿�ID.                                 */
    /***************************************************************************/
    NBB_USHORT port_id;

    /***************************************************************************/
    /* ��λ��. VE��ƽLAGû�в�λ                                */
    /***************************************************************************/
    NBB_BYTE slot_id;

    SPM_LOGICAL_PORT_INFO_CB logic_port_info_cb;

} SPM_LOGICAL_PORT_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_LAG_CB                                                    */
/*                                                                           */
/* Description: ��·�ۺ�                                                     */
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
    /* keyֵ: lag_id.                                                         */
    /***************************************************************************/
    NBB_ULONG lag_id_key;

    /***************************************************************************/
    /* ȫ�����ԡ�                                                              */
    /***************************************************************************/
    ATG_DCI_LINK_AGGR_GLOB_ATTRIB *global_cfg_cb;

    /***************************************************************************/
    /* LAG���á�                                                               */
    /***************************************************************************/
    ATG_DCI_LINK_AGGR_LAG_CONF *lag_cfg_cb;

    /***************************************************************************/
    /* ��Ա�˿����á�                                                          */
    /***************************************************************************/
    ATG_DCI_LINK_AGGR_MEMBER_PORT *port_cfg_cb[ATG_DCI_LAG_MEMBER_PORT_NUM];

    /***************************************************************************/
    /* �Ǹ��طֵ���Ա�˿����á�                                                */
    /***************************************************************************/
    ATG_DCI_LAG_NLB_MEMBER_PORT *nlb_port_cfg_cb;
    /***************************************************************************/
    /* IPV4����������                                                */
    /***************************************************************************/
    ATG_DCI_LAG_IPV4NF_DATA *ipv4_nf_data;
     /***************************************************************************/
    /* IP����������                                                */
    /***************************************************************************/
    ATG_DCI_LAG_IPNF_SAMPLER_DATA *ip_sample_data;
    /***************************************************************************/
    /* IPV6����������                                                */
    /***************************************************************************/
    ATG_DCI_LAG_IPV6NF_DATA *ipv6_nf_data;

    NBB_ULONG port_num;

	//��1����Ա�ڵ�port_id
    NBB_USHORT lag_port_id;

	//��1����Ա�����ڵĲ�λ
    NBB_BYTE lag_slot_id;

    NBB_BYTE if_lag_exist;

    NBB_INT ifLagConfiged;

}SPM_LAG_CB;
/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_ASSOCIATE_IF_CB                                                    */
/*                                                                           */
/* Description: �����ӿ����á�                                                     */
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
    /* keyֵ: Monitor_Group_ID.                                                         */
    /***************************************************************************/
	NBB_BYTE key;
	/***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
	ATG_DCI_ASSOCIATE_IF_BASIC *basic_cfg_cb;
	/***************************************************************************/
    /* �����ӿ���Ŀ                                                              */
    /***************************************************************************/
	NBB_USHORT trigger_if_num;
	/***************************************************************************/
    /*�����ӿ���Ŀ                                                             */
    /***************************************************************************/
	NBB_USHORT act_if_num;
	/***************************************************************************/
    /*�����ӿ�����                                                              */
    /***************************************************************************/
	ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *trigger_if_cfg_cb[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM];
	/***************************************************************************/
    /*�����ӿڶ�Ӧ�߼��ӿ��Ƿ���ڵı�־λ   0/1=������/����                                                           */
    /***************************************************************************/
    NBB_BYTE trigger_if_exist[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM];
    /* �����ӿ�����                                                             */
    /***************************************************************************/
	ATG_DCI_ASSOCIATE_IF_ACT_IF *act_if_cfg_cb[ATG_DCI_ASSOCIATE_IF_ACT_NUM];
    /***************************************************************************/
    /* �����ӿڶ�Ӧ�߼��ӿ��Ƿ���ڱ�־λ 0/1=������/����                                                           */
    /***************************************************************************/
    NBB_BYTE act_if_exist[ATG_DCI_ASSOCIATE_IF_ACT_NUM];
	
}SPM_ASSOCIATE_IF_CB;


/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_L3_MC_CB                                                 */
/*                                                                           */
/* Description: VPLS_L3_MC���á�                                                */
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
    /* keyֵ: vpls_mc_id.                                                      */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_KEY vpls_l3_mc_key;

    /***************************************************************************/
    /* NNI����.                                                                */
    /***************************************************************************/
    NBB_USHORT nni_num;

    /***************************************************************************/
    /* UNI����.                                                                */
    /***************************************************************************/
    NBB_USHORT uni_num;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* NNI�ӿ����á�                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_NNI_DATA *nni_cfg_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB nni_info_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    /***************************************************************************/
    /* UNI�ӿ����á�                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_L3_MC_UNI_DATA *uni_cfg_cb[ATG_DCI_VPLS_MC_UNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB uni_info_cb[ATG_DCI_VPLS_MC_UNI_NUM];

    /***************************************************************************/
    /* ��������ֵ��                                                           */
    /***************************************************************************/
    NBB_ULONG ulIgmpSnoopPosId;
} SPM_VPLS_L3_MC_CB;


/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_MC_CB                                                 */
/*                                                                           */
/* Description: VPLS_MC���á�                                                */
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
    /* keyֵ: vpls_mc_id.                                                      */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_KEY vpls_mc_key;

    /***************************************************************************/
    /* NNI����.                                                                */
    /***************************************************************************/
    NBB_USHORT nni_num;

    /***************************************************************************/
    /* UNI����.                                                                */
    /***************************************************************************/
    NBB_USHORT uni_num;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* NNI�ӿ����á�                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_NNI_DATA *nni_cfg_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB nni_info_cb[ATG_DCI_VPLS_MC_NNI_NUM];

    /***************************************************************************/
    /* UNI�ӿ����á�                                                           */
    /***************************************************************************/
    ATG_DCI_VPLS_MC_UNI_DATA *uni_cfg_cb[ATG_DCI_VPLS_MC_UNI_NUM];

    SPM_VPLS_MC_PORT_INFO_CB uni_info_cb[ATG_DCI_VPLS_MC_UNI_NUM];

} SPM_VPLS_MC_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPLS_UC_CB                                                 */
/*                                                                           */
/* Description: VPLS_UC���á�                                                */
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
    /* keyֵ: vpls_mc_id.                                                      */
    /***************************************************************************/
    ATG_DCI_VPLS_UC_KEY vpls_uc_key;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_VPLS_UC_BASIC_DATA *basic_cfg_cb;

} SPM_VPLS_UC_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_ARP_CB                                                    */
/*                                                                           */
/* Description: ARP��                                                       */
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
    /* keyֵ: Index+IP.                                         */
    /***************************************************************************/
    ATG_DCI_ARP_KEY arp_key;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_ARP_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* ��ARPʱ�������е�λ�á�                                                 */
    /***************************************************************************/
    NBB_USHORT arp_pos_id;

} SPM_ARP_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_NCACHE_CB                                                  */
/*                                                                           */
/* Description: IPv6 �ھӻ��档                                              */
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
    /* keyֵ: Index+IPv6.                                         */
    /***************************************************************************/
    ATG_DCI_IPV6_CACHE_KEY ncache_key;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_IPV6_CACHE_BASIC_DATA *basic_cfg_cb;

    /***************************************************************************/
    /* ��ARPʱ�������е�λ�á�                                                 */
    /***************************************************************************/
    NBB_USHORT ncache_pos_id;

} SPM_NCACHE_CB;

/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_PORT_MIRROR_CB                                                    */
/*                                                                           */
/* Description: �˿ھ���                                                       */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    NBB_ULONG port_mirror_key;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_PORT_MIRROR_BASIC_DATA *basic_cfg_cb;

} SPM_PORT_MIRROR_CB;

/**STRUCT-********************************************************************/



/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_BYPASS_CB                                                  */
/*                                                                           */
/* Description: Bypass˫�鱣�����á�                                         */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    NBB_USHORT bypass_key;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_BYPASS_BASIC_DATA *basic_cfg_cb;

} SPM_BYPASS_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_MSP_CB                                                    */
/*                                                                           */
/* Description: MSP����Э�顣                                                       */
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
    /* keyֵ: ID.                                         */
    /***************************************************************************/
    NBB_ULONG msp_key;

    /***************************************************************************/
    /* �������á�                                                              */
    /***************************************************************************/
    ATG_DCI_MSP_BASIC *basic_cfg_cb;

    /***************************************************************************/
    /* ���ö˿���Ϣ                                                            */
    /***************************************************************************/
    SPM_MSP_PROT_INFO_CB main_info[63 * DERECT_PHY_PORT_RATE];

    /***************************************************************************/
    /* ���ö˿���Ϣ                                                            */
    /***************************************************************************/
    SPM_MSP_PROT_INFO_CB bak_info[63 * DERECT_PHY_PORT_RATE];        

} SPM_MSP_CB;

/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VC_MOD_INFO_CB                                             */
/*                                                                           */
/* Description: ��VC�޸�ʱ�������޸ĺ��������ص���Ϣ��                       */
/*                                                                           */
/* Added by xiaoxiang, 2013/03/15                                            */
/*****************************************************************************/
typedef struct spm_vc_mod_info_cb
{
    /***************************************************************************/
    /* �ı�������û��Ǳ���.   ��/��=1/2  in                                     */
    /***************************************************************************/
    NBB_USHORT if_main;

    /***************************************************************************/
    /* vp_idx.    in/out                                     */
    /***************************************************************************/
    NBB_UINT vp_idx_main;
    NBB_UINT vp_idx_back;

    /***************************************************************************/
    /*  VC��Ӧ����һ��ID.   out                                    */
    /***************************************************************************/
    NBB_ULONG next_hop_counter_id_main;
    NBB_ULONG next_hop_counter_id_back;
    
    /***************************************************************************/
    /*  VC��Ӧ����һ��ID.   out                                    */
    /***************************************************************************/
    NBB_ULONG next_hop_id_main;
    NBB_ULONG next_hop_id_back;

    /***************************************************************************/
    /* VC�ߵ�LSP����Ӧ�ĳ���index.   out                                       */
    /***************************************************************************/
    NBB_ULONG port_index_main;
    NBB_ULONG port_index_back;
    
    /***************************************************************************/
    /*  FRR������group_id.         in/out                              */
    /***************************************************************************/
    NBB_USHORT frr_group_id;
} SPM_VC_MOD_INFO_CB;

/**STRUCT+********************************************************************/
/* Structure: SPM_LAG_PORT_INFO_CB                                            */
/*                                                                           */
/* Description:�l��¼lag��Ա�˿ڵ�port_id ��Э��״̬                      */
/*                                                                           */
/* Added by zhangzhm 20131119                                            */
/*****************************************************************************/

typedef struct spm_lag_port_info_cb
{
	/***************************************************************************/
    /*  ��Ա�˿ڶ�Ӧ�������port_id                                  */
    /***************************************************************************/
	NBB_USHORT port_id;
	/***************************************************************************/
    /*  ��Ա�˿ڶ�Ӧ�������slot_id                                  */
    /***************************************************************************/
	NBB_BYTE slot_id;
	/***************************************************************************/
    /*  lacpЭ��״̬ 1/0='�ɹ�'/'���ɹ�'                                  */
    /***************************************************************************/
	NBB_BYTE port_consult;
    /***************************************************************************/
    /* �ӿ���Ϣ:0/1/2 = ���ӿ�/�ӿ�1/�ӿ�2                                 */
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

/* 113 114 124 ״̬��ѯ��ʹ��*/
#define MAC_VPLS_VP_STA

/* 4 ״̬��ѯ��ʹ��*/
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
/*  �����ڴ�����  ,��Ӧÿ����Ҫ�����ڴ�Ľṹ��*/
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
/* ����������*/
/*****************************************************************************/

/*****************************************************************************/
/*  ���������Ŀ��*/
/*****************************************************************************/
#define SPM_L3_VRF_MAX  8196

/*****************************************************************************/
/* ����HASH�ṹ*/
/*****************************************************************************/
#ifdef SHASH

#define KEYOFFSET(node, offset)   ((NBB_ULONG *)(((NBB_BYTE *)(node)) + offset))          /*ȡ���ݵ�KEY*/

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
	LSPPROT_KEY  key;             /*tunnel keyֵ     */ 
	NBB_ULONG    tunlid;          /*������tunlidֵ   */
	NBB_ULONG    vpnum;           /*L2VPN��Ŀ        */
    AVLL_TREE    vp_tree;         /*�¹ҵ�VP��       */
	NBB_ULONG    vrfpeernum;      /*L3 VPN��Ŀ       */
    AVLL_TREE    vrfpeer_tree;    /*�¹ҵ�L3VPN ��   */
	NBB_ULONG    mcnum;           /*L2���鲥��Ŀ     */
    AVLL_TREE    mcid_tree;       /*�¹ҵ��鲥��     */
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
