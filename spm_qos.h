/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos.h
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS����
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : zenglu
    �޸�����   : �����ļ�
******************************************************************************/
#ifndef SPM_QOS_INCLUDE
#define SPM_QOS_INCLUDE

#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>

/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_intf_key
{
    NBB_ULONG       index;
    NBB_USHORT      svlan;
    NBB_USHORT      cvlan;
}SPM_QOS_LOGIC_INTF_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_flow_key
{
    NBB_ULONG       index;
    NBB_USHORT      svlan;
    NBB_USHORT      cvlan;
}SPM_QOS_LOGIC_FLOW_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_univp
{
    NBB_ULONG      vp_type;
    NBB_ULONG      vpn_id;
    NBB_ULONG      vp_posid;
}SPM_QOS_LOGIC_UNIVP;

/* QoSģ�������Ϣ */
typedef struct tx_uni_port
{
    NBB_CHAR        sysPort;    //���ݸ�C3��SYS PORT  
    NBB_USHORT      port;       //���˿�
    NBB_USHORT      vid;        //�ڲ�VID
}TX_UNI_PORT;

/* QoSģ�������Ϣ */
typedef struct spm_qos_tunnel_key
{
    NBB_USHORT      type;       //type = 0 FTN; type = 1 CR-LSP Tx; �����Ƿ�
    NBB_USHORT      flag;       // 1Ϊ���ã�0Ϊ����
    FTN_KEY         ftn;        //type = 0 FTN
    CRTXLSP_KEY     tx_lsp;     //type = 1 CR-LSP Tx
}SPM_QOS_TUNNEL_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_qos_vrf_instanse_key
{
    NBB_ULONG  label;
    NBB_USHORT vrf_id; 
    NBB_ULONG peer_ip; 
}SPM_QOS_VRF_INSTANSE_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_port_wred_key
{
    NBB_ULONG index;//����˿�INDEX
    NBB_BYTE cos;   //�������ȼ�
}SPM_PORT_WRED_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_qos_ilm_key
{
    NBB_ULONG inlabel;//���ǩ
    NBB_BYTE flag;//��һ����־λ
    NBB_ULONG nextip;
    NBB_ULONG nextport;
}SPM_QOS_ILM_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_te_shaping
{
    NBB_ULONG   cir;   
    NBB_ULONG   pir;
    NBB_USHORT   cbs;
    NBB_USHORT   pbs;
}SPM_TE_SHAPING;

/* QoSģ�������Ϣ */
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

#if 21
NBB_LONG spm_ds_set_l3uni(NBB_ULONG logic_key,NBB_ULONG posid,
            ATG_DCI_LOG_PORT_DIFF_SERV_DATA *data NBB_CCXT_T);

NBB_LONG spm_ds_add_logic_intf_node(
    SUB_PORT                *sub_port,
    SPM_QOS_LOGIC_INTF_KEY *key,
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve NBB_CCXT_T);

/*********ɾ��L3�߼��ӿ����е�ds����********************/
NBB_LONG spm_qos_clear_ds_logic_intf(NBB_LONG logic_key NBB_CCXT_T);

NBB_LONG spm_ds_add_logic_flow_node(SUB_PORT *sub_port, 
            SPM_QOS_LOGIC_FLOW_KEY *key,
            ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff_serv NBB_CCXT_T);

/*********ɾ��l2�߼��ӿ����е�ds����********************/
NBB_LONG spm_qos_clear_logic_ds_flow(NBB_LONG logic_key NBB_CCXT_T);

NBB_LONG spm_ds_add_vc_node(
            NBB_ULONG               ds_index,   //VC�� DSģ��ID
            ATG_DCI_VC_KEY          *key,       //VC�� ���ÿ���Ŀ��ֵ 
            NBB_ULONG               posid,      //��������ֵ
            NBB_USHORT              flag NBB_CCXT_T);     //VC����1Ϊ��0Ϊ��
NBB_LONG spm_ds_add_vrf_node(
            NBB_ULONG               ds_index,   //VRFʵ�� DSģ��ID
            SPM_QOS_VRF_INSTANSE_KEY *key,      //VRFʵ�� ���ÿ���Ŀ��ֵ
            NBB_ULONG               nhiId NBB_CCXT_T);     //����VRFʵ��ʱ����������


NBB_LONG spm_ds_add_tx_lsp_node(
            NBB_ULONG               ds_index,   //FTN DSģ��ID
            SPM_QOS_TUNNEL_KEY      *key,       //FTN ���ÿ���Ŀ��ֵ
            NBB_ULONG               posid,      //����FTN��Ŀʱ����������
            NBB_ULONG               ds_index2,  //��װ�ڶ���Tunnel��ǩ��DSģ��ID   
            SPM_QOS_TUNNEL_KEY      *key_tun2 NBB_CCXT_T); //��װ�ڶ���Tunnel��ǩ����Ŀ��ֵ

NBB_LONG spm_ds_add_rx_lsp_node(
            NBB_ULONG ds_index,  //ILM DSģ��ID
            CRRXLSP_KEY *key,      //CRRXLSP_KEY key
            NBB_ULONG posid,     //����ILM��Ŀʱ����������
            NBB_BYTE node_action, //�ڵ㶯����Ϣ
            NBB_USHORT flag,      //������ʶ1/2:��/��
            NBB_ULONG ds_index2, //��װ�ڶ���Tunnel��ǩ��DSģ��ID
            SPM_QOS_TUNNEL_KEY *key_tun2 NBB_CCXT_T); //��װ�ڶ���Tunnel��ǩ����Ŀ��ֵ


//type:
//0:RXPW;1: RxLspEgrInExp;2:RxLspEgrBackupInExp;
//3:RxLspEgrOutExp;4:RxLspEgrBackupOutExp
//5:RxLspEcmpInExp;6:RxLspEcmpOutExp
NBB_LONG spm_add_ilm_ds_node(NBB_ULONG label,
    ILM_DIFSER* diff,NBB_ULONG posid, NBB_ULONG eposid, 
    NBB_BYTE type) ;

/*�����PW��ǩ����ΪIPOP,node_action=LABEL_ACTION_PW_POP_GO*/
NBB_LONG spm_ds_add_ilm_node(
            ILM_DIFSER *ilm_ds, 
            SPM_QOS_ILM_KEY *key, //ilm KEY
            NBB_ULONG posid, //����ILM��Ŀʱ�����ǩ��Ϣ
            NBB_BYTE node_action, //�ڵ㶯����Ϣ
            NBB_ULONG ds_index2, //��װ�ڶ���Tunnel��ǩ��DSģ��ID
            SPM_QOS_TUNNEL_KEY *key_tun2 NBB_CCXT_T); //��װ�ڶ���Tunnel��ǩ����Ŀ��ֵ

#endif

#if 1/*�ӿں���*/

NBB_LONG spm_set_port_flood_control(SUB_PORT *sub_port,
    NBB_BYTE bc_swtich,NBB_BYTE mc_swtich,NBB_BYTE fd_swtich,
    NBB_ULONG bc_bandwith,NBB_ULONG mc_bandwith,NBB_ULONG fd_bandwith NBB_CCXT_T);

NBB_LONG spm_set_port_cir_pir(SUB_PORT *sub_port,
                             NBB_LONG cos, ATG_DCI_PHY_FLOW_SHAPE_QUEUE *shape NBB_CCXT_T);


/*IP����������IP NetFlow sampler*/
NBB_LONG spm_set_ipv4_sampler(SUB_PORT *sub_port,
                                ATG_DCI_PHY_PORT_IPNF_SAMPLER *cfg NBB_CCXT_T);

NBB_LONG spm_set_lag_sampler(SUB_PORT *sub_port,
                                ATG_DCI_LAG_IPNF_SAMPLER_DATA *cfg NBB_CCXT_T);

NBB_LONG spm_set_port_sp_wfq(SUB_PORT *sub_port,NBB_BYTE cos,
    ATG_DCI_PHY_PORT_SCHEDULE_QUEUE *port_schedule NBB_CCXT_T);


NBB_LONG spm_set_port_wred(SPM_PORT_WRED_KEY *key,
                            SUB_PORT *sub_port,
                            ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg NBB_CCXT_T);



/*������*/
NBB_LONG spm_add_logic_acl_node(
                NBB_ULONG key,                          //�߼��˿�KEY
                ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg,
                SUB_PORT *sub_port NBB_CCXT_T);

/*����߼��ӿڰ���������*/
NBB_LONG spm_qos_clear_logic_acl(NBB_LONG logic_key NBB_CCXT_T);


/*�ϻ��������������*/
NBB_LONG spm_add_logic_flow_classify_node(
                NBB_ULONG key,                          //�߼��˿�KEY
                ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, 
                SUB_PORT *sub_port NBB_CCXT_T);

/*����߼��ӿ��ϻ���������������*/
NBB_LONG spm_qos_clear_logic_classify(NBB_LONG logic_key NBB_CCXT_T);


/*�û��ϻ�QOS*/
NBB_LONG spm_add_logic_up_usr_qos_node(
     NBB_ULONG key,                 //�߼��ӿڵ�key
     SUB_PORT *sub_port,
     ATG_DCI_LOG_UP_USER_QOS_POLICY *up_usr NBB_CCXT_T);           //�ӽӿ���Ϣ
    

/*�û������QOS*/
NBB_LONG spm_add_flow_up_usr_qos_node(
     NBB_ULONG key,                 
     SUB_PORT *sub_port,
     ATG_DCI_LOG_FLOW_UP_USER_QOS *up_usr NBB_CCXT_T);


NBB_LONG spm_hqos_set_l3uni(NBB_ULONG logic_key,NBB_ULONG posid,
                                ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data 
                                NBB_CCXT_T);


/*�»��û�HQOS*/
NBB_LONG spm_hqos_add_usr_node(
                                    SUB_PORT *sub_port, 
                                    
                                    //NBB_ULONG posid,
                                    NBB_ULONG logic_key,
                                    NBB_ULONG group_index,
                                    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS  *data
                                    NBB_CCXT_T);


NBB_LONG spm_hqos_add_group_node(NBB_USHORT slot,
                            NBB_LONG port, 
                            NBB_ULONG group_index,
                            NBB_ULONG log_key NBB_CCXT_T);         

NBB_LONG spm_hqos_add_lsp_node(NBB_USHORT slot,
                            NBB_LONG port,
                            SPM_QOS_TUNNEL_KEY *pkey,
                            ATG_DCI_LSP_TX_UP_TNNL_QOS *pstUpTnnlQos,
                            NBB_ULONG nhi NBB_CCXT_T);

NBB_LONG spm_hqos_add_vc_node(NBB_USHORT slot,
                        NBB_LONG port,
                        NBB_ULONG posid,
                        NBB_BYTE proflag,/*1/2=����/����*/
                        SPM_QOS_TUNNEL_KEY *skey,
                        ATG_DCI_VC_KEY *pkey,
                        ATG_DCI_VC_UP_VPN_QOS_POLICY *data NBB_CCXT_T);


NBB_LONG spm_hqos_add_vrf_node(NBB_USHORT slot,
                        NBB_LONG port,
                        NBB_ULONG posid,
                        SPM_QOS_TUNNEL_KEY *skey,
                        SPM_QOS_VRF_INSTANSE_KEY *pkey,
                        ATG_DCI_VRF_INSTANCE_UP_VPN_QOS *data NBB_CCXT_T);
#endif





#if 2/*�ڴ�У�麯��*/

/****************VERIFY************************************/
NBB_VOID spm_cfg_ds_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_domain_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_acl_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_policy_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_classify_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_action_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_hqos_slot_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_group_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_wred_cb_verify(NBB_CXT_T NBB_CXT);



NBB_VOID spm_cfg_ds_intf_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_flow_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_vc_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_vrf_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_ilm_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_rxlsp_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_ds_tunnel_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_logic_acl_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_logic_classify_cb_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_up_usr_verify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_cfg_hqos_cb_verify(NBB_CXT_T NBB_CXT);


NBB_VOID spm_qos_cfg_cb_verfify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_twamp_ipv4_cb_verfify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_twamp_ipv6_cb_verfify(NBB_CXT_T NBB_CXT);
#endif


/***************************���QOS���ú���***********************************/
NBB_VOID spm_qos_clear_all_cfg(NBB_CXT_T NBB_CXT);


/***************************���ܸ澯***********************************/
NBB_LONG spm_get_poscnt_from_aclkey(NBB_ULONG ulAcl_Id, NBB_ULONG ulRule_Id,
    NBB_ULONG *posid NBB_CCXT_T);

#endif

