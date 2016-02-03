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

#ifndef SPM_QOS_DS_H
#define SPM_QOS_DS_H

/*LOG缓存大小相关定义*/
#define DS_MSG_INFO_LEN    300 

/*PHB相关定义*/
#define MAX_PHB_NUM         8
#define PHB_BE              0
#define PHB_AF1             1
#define PHB_AF2             2
#define PHB_AF3             3
#define PHB_AF4             4
#define PHB_EF              5
#define PHB_CS6             6
#define PHB_CS7             7

/*颜色相关定义*/
#define MAX_COLOR_NUM       3
#define QOS_COLOR_GREEN     0
#define QOS_COLOR_YELLOW    1
#define QOS_COLOR_RED       2

/*用户优先级相关定义*/
#define MAX_VLAN_PRI_NUM    8
#define MAX_DSCP_PRI_NUM    64
#define MAX_EXP_PRI_NUM     8

/*驱动dsptr偏移相关定义*/
#define PRI2PHB_ASSIGN_OFFSET      16
#define PHB2PRI_ASSIGN_OFFSET      16







/*****************************************************************************
   函 数 名  : spm_ds_add_logic_intf_node
   功能描述  : 逻辑端口优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_l3_intf_node(
    SUB_PORT               *sub_port,
    NBB_ULONG  log_key,
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve); //INTF 配置块条目键值




/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_ds_set_l3out_uni(NBB_ULONG logic_key,NBB_ULONG posid,
            ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve);





/*****************************************************************************
   函 数 名  : spm_ds_add_logic_flow_node
   功能描述  : 逻辑端口flow优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口flow的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_l2_intf_node(SUB_PORT *sub_port, 
            NBB_ULONG log_key,ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff_serve);







/*****************************************************************************
   函 数 名  : spm_ds_add_logic_flow_vpuni_node
   功能描述  : 逻辑端口flow优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口flow的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_univp_node(NBB_ULONG logic_index, 
     NBB_ULONG vpid, ATG_DCI_LOG_PORT_DS_L2_DATA *diff_serve);







/*****************************************************************************
   函 数 名  : spm_ds_add_vc_node
   功能描述  : vc优先级映射的接口函数
   输入参数  : ds模板的index  ,vc key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_vc_node(
    ATG_DCI_VC_KEY *vc_key, //VC表 配置块条目键值
    ATG_DCI_VC_DIFF_SERV_DATA *diff_serve, //VC表 DS配置
    NBB_ULONG posid, //驱动返回值
    NBB_USHORT flag);    //VC主备标识




/*****************************************************************************
   函 数 名  : spm_ds_add_vc_node
   功能描述  : vc优先级映射的接口函数
   输入参数  : ds模板的index  ,vc key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_vrf_node(
    SPM_QOS_VRF_INSTANSE_KEY *vrf_key, //VRF表 配置块条目键值
    ATG_DCI_VRF_INSTANCE_DIFF_SERV *diff_serve, //VRF表 DS配置
    NBB_ULONG posid);




NBB_LONG spm_ds_add_tx_lsp_node (SPM_QOS_TUNNEL_KEY *lspkey,
        ATG_DCI_LSP_TX_PROT_DIFF_SERV *diff_serve,
        NBB_ULONG posid,NBB_BYTE type);//type:(0:inlsp;1:outlsp)  








#endif 
