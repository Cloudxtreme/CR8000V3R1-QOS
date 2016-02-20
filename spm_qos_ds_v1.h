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

#ifndef SPM_QOS_DS_H
#define SPM_QOS_DS_H

/*LOG�����С��ض���*/
#define DS_MSG_INFO_LEN    300 

/*PHB��ض���*/
#define MAX_PHB_NUM         8
#define PHB_BE              0
#define PHB_AF1             1
#define PHB_AF2             2
#define PHB_AF3             3
#define PHB_AF4             4
#define PHB_EF              5
#define PHB_CS6             6
#define PHB_CS7             7

/*��ɫ��ض���*/
#define MAX_COLOR_NUM       3
#define QOS_COLOR_GREEN     0
#define QOS_COLOR_YELLOW    1
#define QOS_COLOR_RED       2

/*�û����ȼ���ض���*/
#define MAX_VLAN_PRI_NUM    8
#define MAX_DSCP_PRI_NUM    64
#define MAX_EXP_PRI_NUM     8

/*����dsptrƫ����ض���*/
#define PRI2PHB_ASSIGN_OFFSET      16
#define PHB2PRI_ASSIGN_OFFSET      16




/* QoSģ�������Ϣ */
typedef struct spm_qos_pri2phb_map
{
    NBB_BYTE phb;

    NBB_BYTE color;

}SPM_QOS_PRI2PHB_MAP;



NBB_LONG spm_ds_init(void);


/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_set_l3_intf_node(
    SUB_PORT               *sub_port,
    NBB_ULONG  log_key,
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve); //INTF ���ÿ���Ŀ��ֵ




/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_ds_set_l3out_uni(NBB_ULONG logic_key,NBB_ULONG posid,
            ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve);





/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_flow_node
   ��������  : �߼��˿�flow���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿�flow��key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_set_l2_intf_node(SUB_PORT *sub_port, 
            NBB_ULONG log_key,ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff_serve);







/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_flow_vpuni_node
   ��������  : �߼��˿�flow���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿�flow��key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_set_univp_node(NBB_ULONG logic_index, 
     NBB_ULONG vpid, ATG_DCI_LOG_PORT_DS_L2_DATA *diff_serve);







/*****************************************************************************
   �� �� ��  : spm_ds_add_vc_node
   ��������  : vc���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,vc key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_set_vc_node(ATG_DCI_VC_KEY *vc_key, 
    ATG_DCI_VC_DIFF_SERV_DATA *diff_serve, NBB_ULONG posid,NBB_USHORT flag);  




/*****************************************************************************
   �� �� ��  : spm_ds_add_vc_node
   ��������  : vc���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,vc key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_set_vrf_node(SPM_QOS_VRF_INSTANSE_KEY *vrf_key, 
    ATG_DCI_VRF_INSTANCE_DIFF_SERV *diff_serve,NBB_ULONG posid);




NBB_LONG spm_ds_add_tx_lsp_node (SPM_QOS_TUNNEL_KEY *lspkey,
        ATG_DCI_LSP_TX_PROT_DIFF_SERV *diff_serve,
        NBB_ULONG posid,NBB_BYTE type);//type:(0:inlsp;1:outlsp)  




//type:
//0:RXPW;1: RxLspEgrInExp;2:RxLspEgrBackupInExp;
//3:RxLspEgrOutExp;4:RxLspEgrBackupOutExp
//5:RxLspEcmpInExp;6:RxLspEcmpOutExp
NBB_LONG spm_add_ilm_ds_node(NBB_ULONG label,ATG_DCI_ILM_DIFF_SERV_DATA *diff,NBB_ULONG posid,
                                NBB_ULONG eposid,NBB_BYTE type) ;



#endif 
