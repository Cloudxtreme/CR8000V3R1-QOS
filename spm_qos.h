/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos.h
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS处理
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件
******************************************************************************/
#ifndef SPM_QOS_INCLUDE
#define SPM_QOS_INCLUDE

#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>

/* QoS模块相关信息 */
typedef struct spm_qos_logic_intf_key
{
    NBB_ULONG       index;
    NBB_USHORT      svlan;
    NBB_USHORT      cvlan;
}SPM_QOS_LOGIC_INTF_KEY;

/* QoS模块相关信息 */
typedef struct spm_qos_logic_flow_key
{
    NBB_ULONG       index;
    NBB_USHORT      svlan;
    NBB_USHORT      cvlan;
}SPM_QOS_LOGIC_FLOW_KEY;

/* QoS模块相关信息 */
typedef struct spm_qos_logic_univp
{
    NBB_ULONG      vp_type;
    NBB_ULONG      vpn_id;
    NBB_ULONG      vp_posid;
}SPM_QOS_LOGIC_UNIVP;

/* QoS模块相关信息 */
typedef struct tx_uni_port
{
    NBB_CHAR        sysPort;    //传递给C3的SYS PORT  
    NBB_USHORT      port;       //出端口
    NBB_USHORT      vid;        //内层VID
}TX_UNI_PORT;









/* QoS模块相关信息 */
typedef struct spm_te_shaping
{
    NBB_ULONG   cir;   
    NBB_ULONG   pir;
    NBB_USHORT   cbs;
    NBB_USHORT   pbs;
}SPM_TE_SHAPING;

/* QoS模块相关信息 */
typedef struct sub_port
{
    NBB_BYTE    unit;  
    NBB_BYTE   	slot;
    NBB_USHORT 	port;
    NBB_USHORT    ovlan;
    NBB_USHORT    ivlan;

    /*******驱动返回值*******/
    NBB_LONG posid;/*ACL包过滤用*/
}SUB_PORT;


#if 1/*接口函数*/

NBB_LONG spm_set_port_flood_control(SUB_PORT *sub_port,
    NBB_BYTE bc_swtich,NBB_BYTE mc_swtich,NBB_BYTE fd_swtich,
    NBB_ULONG bc_bandwith,NBB_ULONG mc_bandwith,NBB_ULONG fd_bandwith NBB_CCXT_T);

NBB_LONG spm_set_port_cir_pir(SUB_PORT *sub_port,
                             NBB_LONG cos, ATG_DCI_PHY_FLOW_SHAPE_QUEUE *shape NBB_CCXT_T);


/*IP流采样镜像IP NetFlow sampler*/
NBB_LONG spm_set_ipv4_sampler(SUB_PORT *sub_port,
                                ATG_DCI_PHY_PORT_IPNF_SAMPLER *cfg NBB_CCXT_T);

NBB_LONG spm_set_lag_sampler(SUB_PORT *sub_port,
                                ATG_DCI_LAG_IPNF_SAMPLER_DATA *cfg NBB_CCXT_T);

NBB_LONG spm_set_port_sp_wfq(SUB_PORT *sub_port,NBB_BYTE cos,
    ATG_DCI_PHY_PORT_SCHEDULE_QUEUE *port_schedule NBB_CCXT_T);


NBB_LONG spm_set_port_wred(SPM_PORT_WRED_KEY *key,
                            SUB_PORT *sub_port,
                            ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg NBB_CCXT_T);



/*包过滤*/
NBB_LONG spm_add_logic_acl_node(
                NBB_ULONG key,                          //逻辑端口KEY
                ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg,
                SUB_PORT *sub_port NBB_CCXT_T);

/*清除逻辑接口包过滤配置*/
NBB_LONG spm_qos_clear_logic_acl(NBB_LONG logic_key NBB_CCXT_T);


/*上话复杂流分类策略*/
NBB_LONG spm_add_logic_flow_classify_node(
                NBB_ULONG key,                          //逻辑端口KEY
                ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, 
                SUB_PORT *sub_port NBB_CCXT_T);

/*清除逻辑接口上话复杂流分类配置*/
NBB_LONG spm_qos_clear_logic_classify(NBB_LONG logic_key NBB_CCXT_T);


/*用户上话QOS*/
NBB_LONG spm_add_logic_up_usr_qos_node(
     NBB_ULONG key,                 //逻辑接口的key
     SUB_PORT *sub_port,
     ATG_DCI_LOG_UP_USER_QOS_POLICY *up_usr NBB_CCXT_T);           //子接口信息
    

/*用户流相关QOS*/
NBB_LONG spm_add_flow_up_usr_qos_node(
     NBB_ULONG key,                 
     SUB_PORT *sub_port,
     ATG_DCI_LOG_FLOW_UP_USER_QOS *up_usr NBB_CCXT_T);


NBB_LONG spm_hqos_set_l3uni(NBB_ULONG logic_key,NBB_ULONG posid,
                                ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data 
                                NBB_CCXT_T);


/*下话用户HQOS*/
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
                        NBB_BYTE proflag,/*1/2=主用/备用*/
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





#if 2/*内存校验函数*/

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


/***************************清除QOS配置函数***********************************/
NBB_VOID spm_qos_clear_all_cfg(NBB_CXT_T NBB_CXT);


/***************************性能告警***********************************/
NBB_LONG spm_get_poscnt_from_aclkey(NBB_ULONG ulAcl_Id, NBB_ULONG ulRule_Id,
    NBB_ULONG *posid NBB_CCXT_T);

#endif

