/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_private.h
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS�ڲ�������������
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : zenglu
    �޸�����   : �����ļ�
******************************************************************************/
#ifndef SPM_QOS_PRIVATE_INCLUDE
#define SPM_QOS_PRIVATE_INCLUDE


/* QoSģ�������Ϣ */
typedef struct spm_qos_ilm_key
{
    NBB_ULONG inlabel;//���ǩ
    NBB_BYTE flag;//��һ����־λ
    NBB_ULONG nextip;
    NBB_ULONG nextport;
}SPM_QOS_ILM_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_port_wred_key
{
    NBB_ULONG index;//����˿�INDEX
    NBB_BYTE cos;   //�������ȼ�
}SPM_PORT_WRED_KEY;



/*##################���ϲ���Ҫ�Ľṹ��Ϊ�˱���ͨ��######################*/


#define QOS_MSG_INFO_LEN    300 
#define QOS_PD              25
#define QOS_RCU_PORT    (104)



#define QOS_CFG_STRING       "QOS CFG"
#define HQOS_LSP_CFG         "HQOS LSP CFG"
#define HQOS_LSP_ERROR       "***HQOS LSP ERROR***"
#define HQOS_LSP_WARNING     "***HQOS LSP WARNING***"
#define HQOS_VRF_CFG         "HQOS VRF CFG"
#define HQOS_VRF_ERROR       "***HQOS VRF ERROR***"
#define HQOS_VC_CFG         "HQOS VC CFG"
#define HQOS_VC_ERROR       "***HQOS VC ERROR***"
#define HQOS_USR_CFG         "HQOS USR CFG"
#define HQOS_USR_ERROR       "***HQOS USR ERROR***"
#define HQOS_GROUP_CFG         "HQOS GROUP CFG"
#define HQOS_GROUP_ERROR       "***HQOS GROUP ERROR***"
#define HQOS_ARAD_ERROR       "***HQOS ARAD ERROR***"
#define HQOS_C3_ERROR       "***HQOS C3 ERROR***"
#define HQOS_BMU_ERROR       "***HQOS BMU ERROR***"
#define QOS_ERROR_STRING     "**Qos ERROR**"



#define QOS_EXIST 1
#define QOS_UNEXIST 0

#define  PTN_690_PORT_OFFSET 0

#ifdef PTN690
#define MAX_SLOT_NUM        42
#define MAX_MSP_ID          64
#define MAX_METER_NUM     (32 * 1024)
#else
#define MAX_SLOT_NUM        13
#define MAX_METER_NUM     (8 * 1024)
#endif

#define MAX_METER_BUKECT_NUM                        (256)
#define MAX_PHYSIC_PORT                             128
#define MIN_PHYSIC_PORT                             0
#define MAX_PHYSIC_PORT_NUM                             128
#define VOQ_OFFSET          40
#define NUM_COS             8
#define HQOS_OFFSET          (MAX_SLOT_NUM * NUM_COS * MAX_PHYSIC_PORT_NUM + VOQ_OFFSET)
#define MAX_NODE_ID          2048
#define MAX_HQOS_SLOT_NUM    4

#define MATCH_ERROR         0
#define MATCH_PORT          1
#define MATCH_ETH_HEADER    2   
#define MATCH_IPV4          3
#define MATCH_IP_TCP        9   /*�߼�ACL*/
#define MATCH_IP_UDP        10
#define MATCH_IPV6          19
#define MATCH_PORT_VLAN     39
#define MATCH_NOIP          40
#define MATCH_ACL_ID        50

 /*�ӽӿ������Ϣ*/
typedef struct sub_port
{
    NBB_BYTE    unit;  
    NBB_BYTE   	slot;
    NBB_USHORT 	port;
    NBB_USHORT    ovlan;
    NBB_USHORT    ivlan;

    /*******��������ֵ*******/
    NBB_LONG posid;/*ACL��������*/
}SUB_PORT;

/* QoSģ�������Ϣ */
typedef struct spm_qos_vrf_instanse_key
{
    NBB_ULONG  label;
    NBB_USHORT vrf_id; 
    NBB_ULONG peer_ip; 
}SPM_QOS_VRF_INSTANSE_KEY;


/* QoSģ�������Ϣ */
typedef struct spm_qos_tunnel_key
{
    NBB_USHORT      type;       //type = 0 FTN; type = 1 CR-LSP Tx; �����Ƿ�
    NBB_USHORT      flag;       // 1Ϊ���ã�0Ϊ����
    FTN_KEY         ftn;        //type = 0 FTN
    CRTXLSP_KEY     tx_lsp;     //type = 1 CR-LSP Tx
}SPM_QOS_TUNNEL_KEY;























#endif

