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

#ifndef SPM_QOS_PM_H
#define SPM_QOS_PM_H


#define MEM_SPM_TWAMP_IPV4_CB               ((NBB_LONG)(PCT_SPM | 0x00003200))
#define MEM_SPM_TWAMP_IPV4_BASIC_CB         ((NBB_LONG)(PCT_SPM | 0x00003201))
#define MEM_SPM_TWAMP_IPV6_CB               ((NBB_LONG)(PCT_SPM | 0x00003300))
#define MEM_SPM_TWAMP_IPV6_BASIC_CB         ((NBB_LONG)(PCT_SPM | 0x00003301))


/* QoS模块相关信息 */
typedef struct spm_twamp_ipv4_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_twamp_ipv4_node;

    ATG_DCI_TWAMP_IPV4_KEY key;

    ATG_DCI_TWAMP_IPV4_BASIC data;
    
    /***************************************************************************/
    /* Driver return code.                                                     */
    /***************************************************************************/
    NBB_ULONG posId;         /*posId*/
    
    NBB_ULONG groupId;       /*groupId*/
    
    NBB_ULONG entryId;       /*entryId*/   
} SPM_TWAMP_IPV4_CB;

/* QoS模块相关信息 */
typedef struct spm_twamp_ipv6_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_twamp_ipv6_node;

    ATG_DCI_TWAMP_IPV6_KEY key;

    ATG_DCI_TWAMP_IPV6_BASIC *data;

} SPM_TWAMP_IPV6_CB;

NBB_VOID spm_qos_clear_all_twamp_ipv4_cb();

NBB_VOID spm_qos_clear_all_twamp_ipv6_cb();

NBB_VOID spm_rcv_dci_set_twamp_ipv6(ATG_DCI_SET_TWAMP_IPV6* pstSetIps);

NBB_VOID spm_rcv_dci_set_twamp_ipv4(ATG_DCI_SET_TWAMP_IPV4* pstSetIps);

NBB_VOID spm_init_twamp();

NBB_VOID spm_twamp_ipv4_cb_verfify();

NBB_VOID spm_twamp_ipv6_cb_verfify();
#endif
