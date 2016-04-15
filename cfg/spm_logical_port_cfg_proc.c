/******************************************************************************

                  版权所有 (C), 烽火通信科技股份有限公司

******************************************************************************

  文 件 名   : spm_logical_port_cfg_proc.c
  版 本 号   : 初稿
  作    者   : lish
  生成日期   : 2015年8月10日
  最近修改   :
  功能描述   : 逻辑端口配置处理文件
  函数列表   :
  修改历史   :
  1.日    期 : 2015年8月10日
    作    者 : lish
    修改内容 : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef LOGICAL_PORT_CFG

extern unsigned char g_logical_port_cfg_print_setting;
extern char *port_route_type_string[];
extern char *cfg_oper_type_string[];

/* vpws max_num，供设备规格注册使用 (hdjia) */
NBB_ULONG g_l2_vpws_maxnum = 0;

/* vpls max_num，供设备规格注册使用 (hdjia) */
NBB_ULONG g_l2_vpls_maxnum = 0;

/*****************************************************************************
 函 数 名  : spm_logical_port_vpbindvpws_proc
 功能描述  : vp逻辑口和vpws的绑定关系处理，分为添加绑定，和删除解除
 输入参数  : NBB_ULONG ul_port_key  
             SPM_VPWS_CB *pst_vpws  
             NBB_BYTE    proc_type  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月5日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_vpbindvpws_proc(
    NBB_ULONG ul_port_key,
    SPM_VPWS_CB *pst_vpws,
    NBB_BYTE    proc_type   /* 处理的模式，分为添加和删除 (hdjia) */
    )
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
   

    NBB_TRC_ENTRY("spm_logical_port_vpbindvpws_proc");

    if (NULL == pst_vpws)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_vpws NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_vpws NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }    
    
    /* 删除类型，解绑定*/
    if(ATG_DCI_OPER_DEL == proc_type)
    {
        if(ul_port_key == pst_vpws->port_index)
        {                    
            pst_vpws->port_index = 0;

            /* 备用port为0，则清除uni_no，uni_num个数减一 (hdjia) */
            if (0 == pst_vpws->port_index_bak)
            {
                pst_vpws->uni_no = 0;                    
                pst_vpws->uni_num--;
            }
        }
        else if(ul_port_key == pst_vpws->port_index_bak)
        {                    
            pst_vpws->port_index_bak = 0;

            /* 主用port为0，则清除uni_no，uni_num个数减一 (hdjia) */
            if (0 == pst_vpws->port_index)
            {
                pst_vpws->uni_no = 0;                    
                pst_vpws->uni_num--;
            }
        }
        else if(ul_port_key == pst_vpws->port_index_2)
        {                    
            pst_vpws->port_index_2 = 0;

            /* 备用port为0，则清除uni_no，uni_num个数减一 (hdjia) */
            if (0 == pst_vpws->port_index_bak_2)
            {
                pst_vpws->uni_no_2 = 0;                    
                pst_vpws->uni_num--;
            }
        }
        else if(ul_port_key == pst_vpws->port_index_bak_2)
        {                    
            pst_vpws->port_index_bak_2 = 0;

            /* 主用port为0，则清除uni_no，uni_num个数减一 (hdjia) */
            if (0 == pst_vpws->port_index_2)
            {
                pst_vpws->uni_no_2 = 0;                    
                pst_vpws->uni_num--;
            }
        }                
        
    }
    else if (ATG_DCI_OPER_ADD == proc_type)     /* 添加，绑定关系 (hdjia) */
    {

    }
           


    
   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}



/*****************************************************************************
 函 数 名  : spm_logical_port_del_l2vpn_cfg
 功能描述  : 逻辑口中删除l2vpn配置时，1、置驱动vp为无效；2、解除vp与vpn绑定
             关系；3、释放l2vpn子配置内存，避免更新其他子配置又重建业务
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月5日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_del_l2vpn_cfg(
    SPM_LOGICAL_PORT_CB *pst_logical_port
    )
{
    NBB_INT         ret = SUCCESS;
    
    NBB_CHAR        uc_message[SPM_MSG_INFO_LEN];

    SPM_VPWS_CB     *pst_vpws = NULL;    
    
    SPM_VPLS_CB     *pst_vpls = NULL;    

    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;

    NBB_USHORT      vpn_id = 0;

    NBB_TRC_ENTRY("spm_logical_port_del_l2vpn_cfg");

    if ((NULL == pst_logical_port) || (NULL == pst_logical_port->l2vpn_cfg_cb))
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port or l2vpn NULL!\n",
            __FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port or l2vpn NULL!\n",
            __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    /* 置驱动vp为无效处理 (hdjia) */
    ret = spm_logical_port_drv_set_vp_invalid(pst_logical_port);

    /* ********************************* (hdjia) */
    /* 解绑vp与vpws关系 (hdjia) */
    /* ********************************** (hdjia) */
    vpn_id = pst_logical_port->l2vpn_cfg_cb->vpn_id;

    if (L2VPN_VPWS == pst_logical_port->l2vpn_cfg_cb->vpn_type)  /* vpws类型 (hdjia) */    
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &vpn_id);
        
        if (NULL == pst_vpws)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPWS_ID=%ld does not exist !\n",
                __FUNCTION__, __LINE__, vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPWS_ID=%ld does not exist !\n",
                __FUNCTION__, __LINE__, vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else    /* 找到vpws 解绑 (hdjia) */
        {
            ret = spm_logical_port_vpbindvpws_proc(
                pst_logical_port->port_index_key, pst_vpws, ATG_DCI_OPER_DEL);            
        }

    }
    else if (L2VPN_VPLS == pst_logical_port->l2vpn_cfg_cb->vpn_type)  /* vpls类型 (hdjia) */    
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &vpn_id);
        
        if (NULL == pst_vpls)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPLS_ID=%ld does not exist \n",
                __FUNCTION__, __LINE__, vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPLS_ID=%ld does not exist \n",
                __FUNCTION__, __LINE__, vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else    /* 找到vpls (hdjia) */
        {  
            pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree, 
                &pst_logical_port->l2vpn_cfg_cb->uni_no);

            if (NULL != pst_vpls_uni_cb)
            {
                AVLL_DELETE(pst_vpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);
                
                //释放内存空间
                spm_free_vpls_uni_cb(pst_vpls_uni_cb NBB_CCXT);             
                pst_vpls->uni_num--;    

            } 
            else
            {
                OS_PRINTF(" ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> not find uni_cb!\n",
                    __FUNCTION__, __LINE__,pst_logical_port->port_index_key, vpn_id);
                OS_SPRINTF(uc_message, 
                    " ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> not find uni_cb!\n",
                    __FUNCTION__, __LINE__,pst_logical_port->port_index_key, vpn_id);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_NOT_FOUND;
                goto EXIT_LABEL;

            }

        }

    }

    
    /* ********************************* (hdjia) */
    /* 释放l2vpn子配置 (hdjia) */
    /* ********************************** (hdjia) */
    NBB_MM_FREE(pst_logical_port->l2vpn_cfg_cb, MEM_SPM_LOG_PORT_L2VPN_CB);
    pst_logical_port->l2vpn_cfg_cb = NULL;

    
    EXIT_LABEL:
        NBB_TRC_EXIT();
        return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_terminal_if_cb
   功能描述  : 端口逻辑配置终结子接口结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_TERMINAL_IF_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年9月5日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_TERMINAL_IF_CB *spm_logical_port_alloc_terminal_if_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_terminal_if_cb");

    /* 分配一个新的配置条目。*/
    pst_terminal_if_cb = (SPM_TERMINAL_IF_CB *)NBB_MM_ALLOC(sizeof(SPM_TERMINAL_IF_CB), NBB_NORETRY_ACT, 
        MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    
    if (pst_terminal_if_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_terminal_if_cb, 0, sizeof(SPM_TERMINAL_IF_CB));

    /* 建立用于该配置条目的句柄，作为异步消息交换的相关器。*/
    pst_terminal_if_cb->spm_terminal_if_handle = NBB_CREATE_HANDLE(pst_terminal_if_cb, HDL_SPM_TERMINAL_IF_CB);

    /* 成功分配一个新的配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_TERMINAL_IF_CB allocated at %p with handle %#lx",
            pst_terminal_if_cb, pst_terminal_if_cb->spm_terminal_if_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_terminal_if_cb->spm_terminal_if_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_terminal_if_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_terminal_if_cb
   功能描述  : 释放端口逻辑配置终结子接口节点的内存空间
   输入参数  : (SPM_TERMINAL_IF_CB *pstTerminalIf
             NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年9月5日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_terminal_if_cb(SPM_TERMINAL_IF_CB *pst_terminal_if_cb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_logical_port_free_terminal_if_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_terminal_if_cb, sizeof(SPM_TERMINAL_IF_CB), MEM_SPM_LOG_PORT_TERMINAL_IF_CB);

    if (pst_terminal_if_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_terminal_if_cb->terminal_if_cfg, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
	
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_terminal_if_cb->spm_terminal_if_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_terminal_if_cb, MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    pst_terminal_if_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_terminal_if_key_compare
 功能描述  : 终结子接口Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年9月5日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_terminal_if_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    ATG_DCI_LOG_PORT_VLAN *terminal_if_key1;
    ATG_DCI_LOG_PORT_VLAN *terminal_if_key2;

    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    terminal_if_key1 = (ATG_DCI_LOG_PORT_VLAN *) key1;
    terminal_if_key2 = (ATG_DCI_LOG_PORT_VLAN *) key2;
     
    /***********************************************************************/
    /* 比较key值大小                   */
    /***********************************************************************/
    rc = compare_ushort(&terminal_if_key1->svlan_id, &terminal_if_key2->svlan_id NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ushort(&terminal_if_key1->cvlan_id, &terminal_if_key2->cvlan_id NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}
    
    
	EXIT_LABEL:

    return(rc);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_diff_serv_cb
   功能描述  : 端口逻辑配置diff_serv结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月22日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_DIFF_SERV_CB *spm_logical_port_alloc_diff_serv_cb(NBB_CXT_T NBB_CXT)
{

    SPM_DIFF_SERV_CB *pst_diff_serv_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_diff_serv_cb");

    /* 分配一个新的配置条目。*/
    pst_diff_serv_cb = (SPM_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    
    if (pst_diff_serv_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_diff_serv_cb, 0, sizeof(SPM_DIFF_SERV_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_diff_serv_cb->spm_diff_serv_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_diff_serv_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_diff_serv_cb
  功能描述  : 释放端口逻辑配置diff_serv节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月22日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_diff_serv_cb(SPM_DIFF_SERV_CB *pst_diff_serv_cb NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_logical_port_free_diff_serv_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_diff_serv_cb, sizeof(SPM_DIFF_SERV_CB), MEM_SPM_LOG_PORT_DIFF_SERV_CB);

    if (pst_diff_serv_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_diff_serv_cb->diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));    

    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_diff_serv_cb, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    pst_diff_serv_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_diff_serv_key_compare
 功能描述  : diff_serv Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_diff_serv_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_key1;
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    diff_serv_key1 = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *) key1;
    diff_serv_key2 = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *) key2;
    
    /***********************************************************************/
    /* 比较key值大小                   */
    /***********************************************************************/
    rc = compare_ushort(&diff_serv_key1->svlan, &diff_serv_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&diff_serv_key1->cvlan, &diff_serv_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    return(rc);
    
}

/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_flow_diff_serv_cb
   功能描述  : 端口逻辑配置flow_diff_serv结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_FLOW_DIFF_SERV_CB *spm_logical_port_alloc_flow_diff_serv_cb(NBB_CXT_T NBB_CXT)
{

    SPM_FLOW_DIFF_SERV_CB *pst_flow_diff_serv_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_flow_diff_serv_cb");

    /* 分配一个新的配置条目。*/
    pst_flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    
    if (pst_flow_diff_serv_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_flow_diff_serv_cb, 0, sizeof(SPM_FLOW_DIFF_SERV_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_flow_diff_serv_cb->spm_flow_diff_serv_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_flow_diff_serv_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_flow_diff_serv_cb
  功能描述  : 释放端口逻辑配置flow_diff_serv节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_flow_diff_serv_cb(SPM_FLOW_DIFF_SERV_CB *pst_flow_diff_serv_cb NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_logical_port_free_flow_diff_serv_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_flow_diff_serv_cb, sizeof(SPM_FLOW_DIFF_SERV_CB), MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);

    if (pst_flow_diff_serv_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_flow_diff_serv_cb->flow_diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
    
    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_flow_diff_serv_cb, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    pst_flow_diff_serv_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_flow_diff_serv_key_compare
 功能描述  : diff_serv Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月6日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_flow_diff_serv_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_key1;
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    flow_diff_serv_key1 = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *) key1;
    flow_diff_serv_key2 = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *) key2;
    
    /***********************************************************************/
    /* 比较key值大小                                                       */
    /***********************************************************************/
    rc = compare_ushort(&flow_diff_serv_key1->svlan, &flow_diff_serv_key2->svlan NBB_CCXT);
      
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&flow_diff_serv_key1->cvlan, &flow_diff_serv_key2->cvlan NBB_CCXT);   
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }


    EXIT_LABEL:
    
    return(rc);
    
}

/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_inclassify_qos_cb
   功能描述  : 端口逻辑配置inclassify_qos结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pst_logical_port->inclassify_qos_tree,

*****************************************************************************/
SPM_INCLASSIFY_QOS_CB *spm_logical_port_alloc_inclassify_qos_cb(NBB_CXT_T NBB_CXT)
{

    SPM_INCLASSIFY_QOS_CB *pst_tlv_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_inclassify_qos_cb");

    /* 分配一个新的配置条目。*/
    pst_tlv_cfg_cb = (SPM_INCLASSIFY_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_INCLASSIFY_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    
    if (pst_tlv_cfg_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_tlv_cfg_cb, 0, sizeof(SPM_INCLASSIFY_QOS_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tlv_cfg_cb->spm_inclassify_qos_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tlv_cfg_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_inclassify_qos_cb
  功能描述  : 释放端口逻辑配置inclassify_qos节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_inclassify_qos_cb(SPM_INCLASSIFY_QOS_CB *pst_tlv_cfg_cb)
{

    NBB_TRC_ENTRY("spm_logical_port_free_inclassify_qos_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tlv_cfg_cb, sizeof(SPM_INCLASSIFY_QOS_CB), MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);

    if (pst_tlv_cfg_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_tlv_cfg_cb->inclassify_qos_cfg, 0, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
    
    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_tlv_cfg_cb, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    pst_tlv_cfg_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_inclassify_qos_key_compare
 功能描述  : diff_serv Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月6日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_inclassify_qos_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg_key1;
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    pst_tlv_cfg_key1 = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *) key1;
    pst_tlv_cfg_key2 = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *) key2;
    
    /***********************************************************************/
    /* 比较key值大小                                                       */
    /***********************************************************************/
    rc = compare_ushort(&pst_tlv_cfg_key1->svlan, &pst_tlv_cfg_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&pst_tlv_cfg_key1->cvlan, &pst_tlv_cfg_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
   
    return(rc);
    
}

/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_up_user_qos_cb
   功能描述  : 端口逻辑配置inclassify_qos结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pst_logical_port->inclassify_qos_tree,

*****************************************************************************/
SPM_UP_USER_QOS_CB *spm_logical_port_alloc_up_user_qos_cb(NBB_CXT_T NBB_CXT)
{

    SPM_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_up_user_qos_cb");

    /* 分配一个新的配置条目。*/
    pst_tlv_cfg_cb = (SPM_UP_USER_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_UP_USER_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
    
    if (NULL == pst_tlv_cfg_cb)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_tlv_cfg_cb, 0, sizeof(SPM_UP_USER_QOS_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tlv_cfg_cb->spm_up_user_qos_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tlv_cfg_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_up_user_qos_cb
  功能描述  : 释放端口逻辑配置inclassify_qos节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_up_user_qos_cb(SPM_UP_USER_QOS_CB *pst_tlv_cfg_cb)
{

    NBB_TRC_ENTRY("spm_logical_port_free_up_user_qos_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tlv_cfg_cb, sizeof(SPM_UP_USER_QOS_CB), MEM_SPM_LOG_PORT_UP_USER_QOS_CB);

    if (NULL == pst_tlv_cfg_cb)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_tlv_cfg_cb->up_user_qos_cfg, 0, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
    
    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_tlv_cfg_cb, MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
    pst_tlv_cfg_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_up_user_qos_key_compare
 功能描述  : diff_serv Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月6日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_up_user_qos_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg_key1;
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    pst_tlv_cfg_key1 = (ATG_DCI_LOG_UP_USER_QOS_POLICY *) key1;
    pst_tlv_cfg_key2 = (ATG_DCI_LOG_UP_USER_QOS_POLICY *) key2;
    
    /***********************************************************************/
    /* 比较key值大小                                                       */
    /***********************************************************************/
    rc = compare_ushort(&pst_tlv_cfg_key1->svlan, &pst_tlv_cfg_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&pst_tlv_cfg_key1->cvlan, &pst_tlv_cfg_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
   
    return(rc);
    
}

/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_flow_up_user_qos_cb
   功能描述  : 端口逻辑配置块流 19)相关上话用户QOS策略配置L2配置处理
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pst_logical_port->inclassify_qos_tree,

*****************************************************************************/
SPM_FLOW_UP_USER_QOS_CB *spm_logical_port_alloc_flow_up_user_qos_cb()
{

    SPM_FLOW_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_flow_up_user_qos_cb");

    /* 分配一个新的配置条目。*/
    pst_tlv_cfg_cb = (SPM_FLOW_UP_USER_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_UP_USER_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);
    
    if (NULL == pst_tlv_cfg_cb)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_tlv_cfg_cb, 0, sizeof(SPM_FLOW_UP_USER_QOS_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tlv_cfg_cb->spm_flow_up_user_qos_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tlv_cfg_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_flow_up_user_qos_cb
  功能描述  : 释放端口逻辑配置inclassify_qos节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_flow_up_user_qos_cb(SPM_FLOW_UP_USER_QOS_CB *pst_tlv_cfg_cb)
{

    NBB_TRC_ENTRY("spm_logical_port_free_flow_up_user_qos_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tlv_cfg_cb, sizeof(SPM_FLOW_UP_USER_QOS_CB), MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);

    if (NULL == pst_tlv_cfg_cb)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_tlv_cfg_cb->flow_up_user_qos_cfg, 0, sizeof(SPM_FLOW_UP_USER_QOS_CB));
    
    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_tlv_cfg_cb, MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);
    pst_tlv_cfg_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_flow_up_user_qos_key_compare
 功能描述  : diff_serv Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月6日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_flow_up_user_qos_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg_key1;
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    pst_tlv_cfg_key1 = (ATG_DCI_LOG_FLOW_UP_USER_QOS *) key1;
    pst_tlv_cfg_key2 = (ATG_DCI_LOG_FLOW_UP_USER_QOS *) key2;
    
    /***********************************************************************/
    /* 比较key值大小                                                       */
    /***********************************************************************/
    rc = compare_ushort(&pst_tlv_cfg_key1->svlan, &pst_tlv_cfg_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&pst_tlv_cfg_key1->cvlan, &pst_tlv_cfg_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
   
    return(rc);
    
}


/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_traffic_filter_cb
   功能描述  : 端口逻辑配置traffic_filter结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :(SPM_TRAFFIC_FILTER_CB*) AVLL_NEXT(pst_logical_port->traffic_filter_tree,

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_TRAFFIC_FILTER_CB *spm_logical_port_alloc_traffic_filter_cb(NBB_CXT_T NBB_CXT)
{

    SPM_TRAFFIC_FILTER_CB *pst_traffic_filter_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_traffic_filter_cb");

    /* 分配一个新的配置条目。*/
    pst_traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB *)NBB_MM_ALLOC(sizeof(SPM_TRAFFIC_FILTER_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    
    if (pst_traffic_filter_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_traffic_filter_cb, 0, sizeof(SPM_TRAFFIC_FILTER_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_traffic_filter_cb->spm_traffic_filter_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_traffic_filter_cb);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_free_traffic_filter_cb
  功能描述  : 释放端口逻辑配置traffic_filter节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_free_traffic_filter_cb(SPM_TRAFFIC_FILTER_CB *pst_traffic_filter_cb NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_logical_port_free_traffic_filter_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_traffic_filter_cb, sizeof(SPM_TRAFFIC_FILTER_CB), MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

    if (pst_traffic_filter_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_TRAFFIC_FILTER_CB at %p", pst_traffic_filter_cb));
    

    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_traffic_filter_cb->traffic_filter_cfg, 0, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));

    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_traffic_filter_cb, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    pst_traffic_filter_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_traffic_filter_key_compare
 功能描述  : diff_serv Key值比较函数
 输入参数  : NBB_VOID *key1  
             NBB_VOID *key2  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月6日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_traffic_filter_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *traffic_filter_key1;
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *traffic_filter_key2;   

    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    traffic_filter_key1 = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *) key1;
    traffic_filter_key2 = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *) key2;
        
    rc = compare_ushort(&traffic_filter_key1->svlan, &traffic_filter_key2->svlan NBB_CCXT);
       
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }
    
    rc = compare_ushort(&traffic_filter_key1->cvlan, &traffic_filter_key2->cvlan NBB_CCXT);
    
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    return(rc);
    
}

/*****************************************************************************
   函 数 名  : spm_logical_port_if_intf_cfg_acceptd
   功能描述  : 如果盘子是混合盘，但是端口对应的物理口类型不为pve，intf标志无效
   输入参数  : NBB_ULONG logical_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_if_intf_cfg_acceptd(NBB_ULONG logical_index)
{
    NBB_ULONG physical_port_index = 0;
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;

    pst_logical_port = AVLL_FIND(v_spm_shared->logical_port_tree,&logical_index);

    if((NULL != pst_logical_port)
        && (NULL != pst_logical_port->phy_cfg_cb))
    {
        physical_port_index = pst_logical_port->phy_cfg_cb->phy_port_index;

        pst_physical_port = AVLL_FIND(v_spm_shared->physical_port_tree,&physical_port_index);

        if((NULL != pst_physical_port)
            && (NULL != pst_physical_port->basic_cfg_cb))
        {
           /* if(MIX1LP4 == g_stHardwBoardInf.enBoardName 
            || MIX10LP2 == g_stHardwBoardInf.enBoardName
            || MIX1OLP2 == g_stHardwBoardInf.enBoardName
            || MSA1LN4 == g_stHardwBoardInf.enBoardName)
            {
                if(ATG_DCI_PVE != pst_physical_port->basic_cfg_cb->port_type)
                {
                    return ERROR;
                }
            } */
        }
    }

    return SUCCESS;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_ipv4_key_compare
   功能描述  : ipv4做为key值的比较函数
   输入参数  : NBB_VOID *key1                     
               NBB_VOID *key2 NBB_CCXT_T NBB_CXT  
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_ipv4_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_key1 = NULL;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_key2 = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv4_key_compare");
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    ipv4_key1 = (ATG_DCI_LOG_PORT_IPV4_DATA *) key1;
    ipv4_key2 = (ATG_DCI_LOG_PORT_IPV4_DATA *) key2;
     
    /***********************************************************************/
    /* 比较key值大小                   */
    /***********************************************************************/
    rc = compare_ulong(&ipv4_key1->ip_addr, &ipv4_key2->ip_addr NBB_CCXT);
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_byte(&ipv4_key1->mask_len, &ipv4_key2->mask_len NBB_CCXT);
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }
        
    EXIT_LABEL:
    return(rc);
}

/*****************************************************************************
   函 数 名  : spm_logical_port_ipv4_key_compare
   功能描述  : ipv6做为key值的比较函数
   输入参数  : NBB_VOID *key1                     
               NBB_VOID *key2 NBB_CCXT_T NBB_CXT  
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_ipv6_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_key1 = NULL;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_key2 = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv6_key_compare");
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    ipv6_key1 = (ATG_DCI_LOG_PORT_IPV6_DATA *) key1;
    ipv6_key2 = (ATG_DCI_LOG_PORT_IPV6_DATA *) key2;
     
    /***********************************************************************/
    /* 比较key值大小                   */
    /***********************************************************************/
    rc = compare_ulong(&ipv6_key1->ip_addr[0], &ipv6_key2->ip_addr[0] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ulong(&ipv6_key1->ip_addr[1], &ipv6_key2->ip_addr[1] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ulong(&ipv6_key1->ip_addr[2], &ipv6_key2->ip_addr[2] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ulong(&ipv6_key1->ip_addr[3], &ipv6_key2->ip_addr[3] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_byte(&ipv6_key1->mask_len, &ipv6_key2->mask_len NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}
	
	EXIT_LABEL:
    return(rc);
}

/*****************************************************************************
   函 数 名   : spm_logical_port_check_if_ipv4_exist
   功能描述   : 检查一个逻辑端口中ipv4是否有重复
   输入参数   : 
   输出参数   : 无
   返 回 值   : 0 ：无重复
                n : 有重复，n=数组位置+1
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_check_if_ipv4_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_IPV4_DATA *pst_ipv4)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_ipv4_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_ipv4)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_IPV4_NUM; loop++)
    {
        if (NULL == pst_logical_port->ipv4_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //如果IPV4相等，返回存储位置
            if (0 == (NBB_MEMCMP(pst_ipv4, pst_logical_port->ipv4_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名   : spm_logical_port_check_if_ipv6_exist
   功能描述   : 检查一个逻辑端口中ipv4是否有重复
   输入参数   : 
   输出参数   : 无
   返 回 值   : 0 ：无重复
                n : 有重复，n=数组位置+1
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_check_if_ipv6_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_IPV6_DATA *pst_ipv6)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_ipv6_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_ipv6)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_IPV6_NUM; loop++)
    {
        if (NULL == pst_logical_port->ipv6_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //如果IPV6相等，返回存储位置
            if (0 == (NBB_MEMCMP(pst_ipv6, pst_logical_port->ipv6_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名   : spm_logical_port_check_if_mc_ipv4_exist
   功能描述   : 检查一个逻辑端口中ipv4是否有重复
   输入参数   : 
   输出参数   : 无
   返 回 值   : 0 ：无重复
                n : 有重复，n=数组位置+1
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_check_if_mc_ipv4_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *pst_mc_ipv4)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_mc_ipv4_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_mc_ipv4)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_MC_IPV4_NUM; loop++)
    {
        if (NULL == pst_logical_port->mc_ipv4_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //如果IPV4相等，返回存储位置
            if (0 == (NBB_MEMCMP(pst_mc_ipv4, pst_logical_port->mc_ipv4_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名   : spm_logical_port_check_if_mc_ipv6_exist
   功能描述   : 检查一个逻辑端口中ipv4是否有重复
   输入参数   : 
   输出参数   : 无
   返 回 值   : 0 ：无重复
                n : 有重复，n=数组位置+1
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_check_if_mc_ipv6_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *pst_mc_ipv6)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_mc_ipv6_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_mc_ipv6)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_MC_IPV6_NUM; loop++)
    {
        if (NULL == pst_logical_port->mc_ipv6_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //如果IPV4相等，返回存储位置
            if (0 == (NBB_MEMCMP(pst_mc_ipv6, pst_logical_port->mc_ipv6_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名   : spm_logical_port_check_if_vip_vmac_exist
   功能描述   : 检查一个逻辑端口中ipv4是否有重复
   输入参数   : 
   输出参数   : 无
   返 回 值   : 0 ：无重复
                n : 有重复，n=数组位置+1
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_check_if_vip_vmac_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pst_vip_vmac)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_vip_vmac_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_vip_vmac)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; loop++)
    {
        if (NULL == pst_logical_port->vip_vmac_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //如果IPV4相等，返回存储位置
            if (0 == (NBB_MEMCMP(pst_vip_vmac, pst_logical_port->vip_vmac_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名   : spm_logical_port_check_if_vipv6_vmac_exist
   功能描述   : 检查一个逻辑端口中ipv4是否有重复
   输入参数   : 
   输出参数   : 无
   返 回 值   : 0 ：无重复
                n : 有重复，n=数组位置+1
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_check_if_vipv6_vmac_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_vipv6_vmac_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_vipv6_vmac)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; loop++)
    {
        if (NULL == pst_logical_port->vipv6_vmac_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //如果IPV4相等，返回存储位置
            if (0 == (NBB_MEMCMP(pst_vipv6_vmac, pst_logical_port->vipv6_vmac_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_intf
   功能描述  : 删除端口逻辑配置的intf
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2016年1月22日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_intf(SPM_INTF_CB *st_intf_cb)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit = 0; 
    
    NBB_TRC_ENTRY("spm_logical_port_add_intf");
       
    if (NULL == st_intf_cb)
    { 
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,st_intf_cb->svlan_id,st_intf_cb->cvlan_id);       
        goto EXIT_LABEL;
    }
   
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;
        
        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == st_intf_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == st_intf_cb->port_sub_type)
            || ((ATG_DCI_LAG != st_intf_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != st_intf_cb->port_sub_type) 
            && (c3_unit == st_intf_cb->unit_id)))
        {
            if(ATG_DCI_L2 == st_intf_cb->port_route_type)
            {
                func_ret = spm_logical_port_drv_add_l2_intf(c3_unit,st_intf_cb);
            } 
            else if(ATG_DCI_L3 == st_intf_cb->port_route_type)
            {
                func_ret = spm_logical_port_drv_add_l3_intf(c3_unit,st_intf_cb);
            }   
            
            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_two_value(ret,c3_unit,func_ret); 
                goto EXIT_LABEL;
            }        
        }
    }
       
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_logical_port_del_cfg
   功能描述  : 删除端口逻辑配置
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_del_cfg(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_INT loop = 0;
    NBB_ULONG posid = 0; 
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_del_cfg");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);  
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);  
        goto EXIT_LABEL;
    }

    //删除intf
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
        spm_logical_port_apply_del_vlan(pst_logical_port,pst_terminal_if_cb);
    }
        
    //如果是三层接口，删除intf
    if (pst_logical_port->basic_cfg_cb->port_route_type == ATG_DCI_L3)
    {
        for (loop = 0; loop < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; loop++)
        {
            if (pst_logical_port->vip_vmac_cfg_cb[loop])
            {
                posid = pst_logical_port->logic_port_info_cb.vip_vmac_posid[loop][0];
                func_ret = spm_logical_port_drv_del_acl_vrrp(posid);
                
                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
                    spm_l2_record_err_code(ret);  
                }
            }
        }    
    }
    else if (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
    {
        //删除vp
        if (NULL != pst_logical_port->l2vpn_cfg_cb)
        {
        	if(L2VPN_VPWS == pst_logical_port->l2vpn_cfg_cb->vpn_type)
        	{
        		func_ret = spm_logical_port_apply_del_l2vpn(
                	pst_logical_port,
                	pst_logical_port->l2vpn_cfg_cb);
        	}

			else if(L2VPN_VPLS == pst_logical_port->l2vpn_cfg_cb->vpn_type)
			{
				func_ret = spm_logical_Port_apply_del_vpls_uni(
					pst_logical_port,
                	pst_logical_port->l2vpn_cfg_cb);
			}
            
            
            if (SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
                spm_l2_record_err_code(ret); 
            }
        }
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_set_intf_vrrp
   功能描述  : 删除端口逻辑配置
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数
   修改历史      :
   1.日    期   : 2016年1月25日
    作    者   : fansongbo
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT spm_logical_port_set_intf_vrrp(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    NBB_BYTE c3_unit =0; 
    
    NBB_TRC_ENTRY("spm_logical_port_set_intf_vrrp");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        goto EXIT_LABEL;
    }
    
    //设置intf VRRP
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {        
            if(0 != pst_terminal_if_cb->intf_pos_id[c3_unit])
            {
                func_ret = SUCCESS;
                func_ret = spm_logical_port_drv_set_intf_vrrp(c3_unit,pst_terminal_if_cb->intf_pos_id[c3_unit]);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,
                        pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                        pst_terminal_if_cb->terminal_if_cfg.cvlan_id,
                        func_ret);
                    break;
                }         
            }
        }    
    }   
    
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_get_vpws_vc_key_from_uni_index;
   功能描述  : 从逻辑端口index判断对应的VPWS的vc key
   输入参数  : NBB_ULONG ulPortIndex
   输出参数  : 无
   返 回 值  :  0/-1 = 参数错误/正常获取
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年6月9日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_vpws_vc_key_from_uni_index(NBB_ULONG index,ATG_DCI_VC_KEY *vc_key)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    SPM_LOGICAL_PORT_CB *pstLogicalport = NULL;
    SPM_VPWS_CB *pstVpws = NULL;
    NBB_USHORT vpws_key = 0;

    if(NULL == vc_key)
    {
        OS_PRINTF("	***ERROR***:(%s:%d)pointer is NULL\n", __FILE__, __LINE__);

        OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)pointer is NULL\n", __FILE__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }

    pstLogicalport = AVLL_FIND(v_spm_shared->logical_port_tree,&index);
    
    if((NULL == pstLogicalport)
    || (NULL == pstLogicalport->basic_cfg_cb))
    {
        OS_PRINTF("	***ERROR***:(%s:%d)INDEX=%ld的逻辑端口配置为空\n", 
            __FILE__, __LINE__, index);

        OS_SPRINTF(ucMessage,"	***ERROR***:(%s:%d)INDEX=%ld的逻辑端口配置为空\n",
            __FILE__,__LINE__,index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }

    if((L2VPN_VPWS != pstLogicalport->logic_port_info_cb.vpn_type)
    || (0 == pstLogicalport->logic_port_info_cb.vpn_id))
    {
        OS_PRINTF("	***ERROR***:(%s:%d)INDEX=%ld的逻辑口不是VPWS_UNI口\n", 
            __FILE__, __LINE__, index);

        OS_SPRINTF(ucMessage,"	***ERROR***:(%s:%d)INDEX=%ld的逻辑口不是VPWS_UNI口\n",
            __FILE__,__LINE__,index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }

    vpws_key = pstLogicalport->logic_port_info_cb.vpn_id;

    pstVpws = AVLL_FIND(v_spm_shared->vpws_tree,&vpws_key);

    if(NULL == pstVpws)
    {
        OS_PRINTF("	***ERROR***:(%s:%d)VPWS_ID=%d的VPWS的NNI配置为空\n", 
            __FILE__, __LINE__, vpws_key);

        OS_SPRINTF(ucMessage,"	***ERROR***:(%s:%d)VPWS_ID=%d的VPWS的NNI配置为空\n",
            __FILE__,__LINE__,vpws_key);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }
    else if(0 != pstVpws->vckey_main.vc_id)
    {
        vc_key->vc_id = pstVpws->vckey_main.vc_id;
        vc_key->vc_type = pstVpws->vckey_main.vc_type;
        vc_key->peer_ip = pstVpws->vckey_main.peer_ip;       
    }
    else if(0 != pstVpws->vckey_back.vc_id)
    {
        vc_key->vc_id = pstVpws->vckey_back.vc_id;
        vc_key->vc_type = pstVpws->vckey_back.vc_type;
        vc_key->peer_ip = pstVpws->vckey_back.peer_ip;       
    }

    return SUCCESS;

    
}


/*****************************************************************************
   函 数 名   : spm_logical_port_get_portid
   功能描述   : 从逻辑端口配置的index获取对应的物理芯片号
   输入参数   : NBB_ULONG ulPortIndex, SPM_PORT_INFO_CB *pstPortInfo
   输出参数   : 无
   返 回 值   :
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_get_portid(NBB_ULONG ul_port_index, SPM_PORT_INFO_CB *pst_port_info)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    SPM_PORT_INFO_CB st_logic_port_info;
    NBB_BYTE uc_lag_slot_id = 0;
    NBB_USHORT us_lag_port_id = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_logical_port_get_portid");

    if (NULL == pst_port_info)
    {
        OS_PRINTF(" ***ERROR***:(%s:%d)pst_port_info is NULL\n",__FUNCTION__, __LINE__);

        OS_SPRINTF(uc_message, " ***ERROR***:(%s:%d)pst_port_info is NULL\n",__FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&st_logic_port_info, 0, sizeof(SPM_PORT_INFO_CB));

    pst_logical_port = AVLL_FIND(v_spm_shared->logical_port_tree, &ul_port_index);

    //逻辑端口不存在
    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld logical port does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);

        OS_SPRINTF(uc_message, " ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld logical port does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);

        ret = SPM_LOGICAL_E_NOT_FOUND;
        goto EXIT_LABEL;
    }

    //端口逻辑配置存在，但子配置：基本配置/物理配置不存在的话，也无法获取PortId
    if ((NULL == pst_logical_port->basic_cfg_cb) || (NULL == pst_logical_port->phy_cfg_cb))
    {
        OS_PRINTF(" ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:basic config or physical config does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);

        OS_SPRINTF(uc_message, 
            " ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:basic config or physical config does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);

        ret = SPM_LOGICAL_E_BASIC_CFG_IS_FAIL;
        goto EXIT_LABEL;
    }

    //接口、子接口、pve口
    if (((pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_SUB_INTFACE)
            || (pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
            || (pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_PVE_PORT))
        && (pst_logical_port->phy_cfg_cb->phy_port_index != 0))
    {
        func_ret = spm_get_portid_from_physical_port_index(pst_logical_port->phy_cfg_cb->phy_port_index,
            &st_logic_port_info NBB_CCXT);
        if (func_ret == SUCCESS)
        {
            pst_port_info->port_id = st_logic_port_info.port_id;
            pst_port_info->slot_id = st_logic_port_info.slot_id;
            pst_port_info->card_id = st_logic_port_info.card_id;
            pst_port_info->unit_id = st_logic_port_info.unit_id;
            pst_port_info->phy_port_type = st_logic_port_info.phy_port_type;
            pst_port_info->port_type = pst_logical_port->basic_cfg_cb->port_sub_type;
            pst_port_info->pve_backup1_slot_id = st_logic_port_info.pve_backup1_slot_id;
            pst_port_info->pve_backup1_port_id = st_logic_port_info.pve_backup1_port_id;
            pst_port_info->pve_backup2_slot_id = st_logic_port_info.pve_backup2_slot_id;
            pst_port_info->pve_backup2_port_id = st_logic_port_info.pve_backup2_port_id;
            pst_port_info->pve_backup3_slot_id = st_logic_port_info.pve_backup3_slot_id;
            pst_port_info->pve_backup3_port_id = st_logic_port_info.pve_backup3_port_id;

        }
        else
        {
            OS_PRINTF(" ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:"
                "call spm_get_portid_from_physical_port_index failed,ret=%d!\n",
                __FUNCTION__, __LINE__,ul_port_index,func_ret);

            OS_SPRINTF(uc_message, 
                " ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:"
                "call spm_get_portid_from_physical_port_index failed,ret=%d!\n", 
                __FUNCTION__, __LINE__,ul_port_index,func_ret);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);

            ret = SPM_LOGICAL_E_CALL_PHYSICAL;
            goto EXIT_LABEL;
        }
    }

    //LAG口
    else if ((pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
        && (pst_logical_port->phy_cfg_cb->lag_id != 0))
    {
        //func_ret = spm_link_aggr_get_lag_min_slot_port(pst_logical_port->phy_cfg_cb->lag_id, &uc_lag_slot_id, 
            //&us_lag_port_id NBB_CCXT);

        pst_port_info->port_id = pst_logical_port->phy_cfg_cb->lag_id + ATG_DCI_LAG_OFFSET;
        pst_port_info->slot_id = 0;
        pst_port_info->unit_id = UNIT_0;  
        pst_port_info->port_type = ATG_DCI_LAG;
        pst_port_info->lag_slot_id = uc_lag_slot_id;
        pst_port_info->lag_port_id = us_lag_port_id;
    }

    //L2 L3桥接口
    else if ((pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)
        && (pst_logical_port->phy_cfg_cb->ve_group_id != 0))
    {
            pst_port_info->port_id = pst_logical_port->phy_cfg_cb->ve_group_id;
            pst_port_info->slot_id = 0;
            pst_port_info->unit_id = UNIT_0;  
            pst_port_info->port_type = ATG_DCI_VE_PORT;

            //区分L2ve和L3ve
            if (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                pst_port_info->ve_type = L2_VE;
            }
            else if (ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                pst_port_info->ve_type = L3_VE;
            }
    }

    if (pst_logical_port->basic_cfg_cb != NULL)
    {
        pst_port_info->svlan = pst_logical_port->basic_cfg_cb->control_vid;

    }

    pst_port_info->vp_pos_id = pst_logical_port->logic_port_info_cb.vp_idx;

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名   : spm_vpn_del_vp_uni
   功能描述   : 删除vp、释放nhi、清除应用层存储驱动信息
   输入参数   : 
   输出参数   : 无
   返 回 值   :
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpn_del_vp_uni(SPM_LOGICAL_PORT_CB *pst_logical_port) 
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_UINT vp_idx = 0;
    NBB_BYTE uc_vpn_type;
    NBB_USHORT us_vpn_id;   

    /* 逻辑接口的键值 */
    NBB_ULONG ul_port_key = 0;

    NBB_TRC_ENTRY("spm_vpn_del_vp_uni");

    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pointer is NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pointer is NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    vp_idx = pst_logical_port->logic_port_info_cb.vp_idx;
    uc_vpn_type = pst_logical_port->logic_port_info_cb.vpn_type;
    us_vpn_id = pst_logical_port->logic_port_info_cb.vpn_id;
    ul_port_key = pst_logical_port->port_index_key;

#if 0       
    if (0 == pst_l2vpn_data->uni_no)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld from VPN_TYPE=%d VPN_ID=%d error:uni_no ==0!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        OS_SPRINTF(uc_message, 
            " ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld from VPN_TYPE=%d VPN_ID=%d error:uni_no ==0!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_NOT_FOUND;
        goto EXIT_LABEL;

    }
#endif

    if (0 != vp_idx)
    {
        func_ret = spm_logical_port_drv_del_port_to_vpn(uc_vpn_type,us_vpn_id,vp_idx);
        if (SUCCESS != func_ret)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:"
                "call spm_logical_port_del_port_to_vpn failed,ret=%d!\n",
                __FUNCTION__, __LINE__,ul_port_key,func_ret);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:"
                "call spm_logical_port_del_port_to_vpn failed,ret=%d!\n", 
                __FUNCTION__, __LINE__,ul_port_key,func_ret);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_CALL_DRV;
        }

    }
    else
    {
        OS_PRINTF(" ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld  from VPN_TYPE=%d VPN_ID=%d repeatedly!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        OS_SPRINTF(uc_message, 
            " ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld  from VPN_TYPE=%d VPN_ID=%d repeatedly!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_NOT_FOUND;
        goto EXIT_LABEL;
    }

    
    if (SUCCESS == ret)
    {
    	//回收next_hop_id资源
		if (pst_logical_port->logic_port_info_cb.next_hop_id != 0)
		{
	    	func_ret =  spm_vc_free_nhi_id(pst_logical_port->logic_port_info_cb.next_hop_id NBB_CCXT);
             if (SUCCESS != func_ret)
             {
                OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,call spm_vc_free_nhi_id error,ret=%d!\n", 
                    __FUNCTION__, __LINE__,ul_port_key,func_ret);
                OS_SPRINTF(uc_message, 
                    " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,call spm_vc_free_nhi_id error,ret=%d!\n", 
                    __FUNCTION__, __LINE__,ul_port_key,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_VC;
             }
	    }

        //清空逻辑接口配置中的驱动返回值
        OS_MEMSET(&pst_logical_port->logic_port_info_cb, 0, sizeof(SPM_LOGICAL_PORT_INFO_CB));

    }
    else
    {
        OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:call spm_logical_port_del_port_to_vpn failed!\n",
            __FUNCTION__, __LINE__,ul_port_key);
        OS_SPRINTF(uc_message, 
            " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:call spm_logical_port_del_port_to_vpn failed!\n", 
            __FUNCTION__, __LINE__,ul_port_key);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_DRV;
        goto EXIT_LABEL;
    }
        

   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}

/*****************************************************************************
   函 数 名  : spm_logical_port_del_uni_from_vpn
   功能描述  : 为vpn删除一个VP(UNI),解除vp与vpn绑定关系，若l2vpn为空，则说明
                之前已做过关系解除处理，只删除驱动、释放nhi资源即可
   输入参数  : NBB_BYTE uc_vpn_type
               NBB_USHORT us_vpn_id
               NBB_VOID *pst_uni_data
               SPM_VPN_PORT_INFO_CB *pst_vpn_port_info
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_del_uni_from_vpn(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;

    /* 逻辑接口的键值 */
    NBB_ULONG ul_port_key = 0;


    NBB_TRC_ENTRY("spm_logical_port_del_uni_from_vpn");

    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    /* 若l2vpn属性为空，则说明vp vpn绑定关系已解除，只需删驱动vp释放nhi资源 (hdjia) */
    if (NULL == pst_logical_port->l2vpn_cfg_cb)
    {
        ret = spm_vpn_del_vp_uni(pst_logical_port);
        if (SUCCESS != ret)
        {    
            OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,spm_vpn_del_vp_uni, ret=%d!\n",
                __FUNCTION__, __LINE__,pst_logical_port->port_index_key,ret);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,spm_vpn_del_vp_uni, ret=%d!\n",
                __FUNCTION__, __LINE__,pst_logical_port->port_index_key,ret);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_CONFIG;
        }

        /* 操作完成后，退出即可 (hdjia) */
        goto EXIT_LABEL;
    }

    /*  *************************************************************** */
    /* 如下操作是在l2vpn属性存在，删除逻辑口时，删除vp的处理 (hdjia) */
    /*  *************************************************************** */
    ul_port_key = pst_logical_port->port_index_key;
    pst_l2vpn_data = pst_logical_port->l2vpn_cfg_cb;
    
    /* vpn类型为VPWS*/
    if(L2VPN_VPWS == pst_l2vpn_data->vpn_type)
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpws)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPWS_ID=%ld does not exist,can not del uni!\n",
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPWS_ID=%ld does not exist,can not del uni!\n", 
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else
        {
            func_ret = spm_vpn_del_vp_uni(pst_logical_port);
            
            //如果成功，处理ac vp与vpn绑定关系
            if (SUCCESS == func_ret)
            {
                ret = spm_logical_port_vpbindvpws_proc(ul_port_key, pst_vpws, ATG_DCI_OPER_DEL);
            }
            else //如果增加UNI口失败记录错误状态
            {
                OS_PRINTF("***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPWS_ID=%d>"
                    " spm_vpn_del_vp_uni() VPWS error,ret=%d! \n",
                    __FUNCTION__, __LINE__, ul_port_key, pst_vpws->vpws_id_key,func_ret);
                OS_SPRINTF(uc_message,
                    "***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPWS_ID=%d> spm_vpn_del_vp_uni() VPWS error,ret=%d！\n",
                    __FUNCTION__, __LINE__, ul_port_key, pst_vpws->vpws_id_key,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_LOGICAL;
            }

        }

    }

    /* vpn类型为VPLS*/
    else if(L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpls)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPLS_ID=%ld does not exist,can not del uni!\n",
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPLS_ID=%ld does not exist,can not del uni!\n",
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else
        {
            func_ret = spm_vpn_del_vp_uni(pst_logical_port);
            
            //如果成功，删除数据
            if (SUCCESS == func_ret)
            {
                pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree, &pst_l2vpn_data->uni_no);

                if (NULL != pst_vpls_uni_cb)
                {
    	            AVLL_DELETE(pst_vpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);
                    
    	            //释放内存空间
    	            spm_free_vpls_uni_cb(pst_vpls_uni_cb NBB_CCXT);	            
    				pst_vpls->uni_num--;	

                } 
                else
                {
                    OS_PRINTF(" ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> del uni data error!\n",
                        __FUNCTION__, __LINE__,ul_port_key,pst_l2vpn_data->vpn_id);
                    OS_SPRINTF(uc_message, 
                        " ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> del uni data error!\n",
                        __FUNCTION__, __LINE__,ul_port_key,pst_l2vpn_data->vpn_id);
                    BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                    ret = SPM_LOGICAL_E_NOT_FOUND;
                    goto EXIT_LABEL;

                }
            }
            else //如果增加UNI口失败记录错误状态
            {
                OS_PRINTF("***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d>"
                    " spm_vpn_del_vp_uni() VPLS error,ret=%d! \n",
                    __FUNCTION__, __LINE__,ul_port_key,pst_vpls->vpls_id_key,func_ret);
                OS_SPRINTF(uc_message,
                    "***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> spm_vpn_del_vp_uni() VPLS error,ret=%d！\n",
                    __FUNCTION__, __LINE__,ul_port_key,pst_vpls->vpls_id_key,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_LOGICAL;
            }


        }

    }
    
   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}


/*****************************************************************************
   函 数 名   : spm_vpls_add_mc_uni_vp
   功能描述   : vpls组播添加uni vp
   输入参数   : ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port
   输出参数   : 无
   返 回 值   :
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_add_mc_uni_vp(SPM_LOGICAL_PORT_CB *pst_logical_port,
    SPM_VPN_PORT_INFO_CB *pst_vpls_port_info NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT vp_id = 0;
    
    NBB_TRC_ENTRY("spm_vpls_add_mc_uni_vp");
    
    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vpls_port_info)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_vpls_port_info NULL\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_vpls_port_info NULL\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    //根据逻辑端口配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600


   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}


/*****************************************************************************
 函 数 名  : spm_l2_set_vpn_maxnum_value
 功能描述  : 设置vpws vpls所支持的最大规格参数
 输入参数  : NBB_ULONG vpws_maxnum  
             NBB_ULONG vpls_maxnum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月21日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_l2_set_vpn_maxnum_value(
    NBB_ULONG vpws_maxnum,  /* 设备所支持的最大vpws规格 (hdjia) */
    NBB_ULONG vpls_maxnum   /* 设备所支持的最大vpls规格 (hdjia) */
    )
{
    g_l2_vpws_maxnum = vpws_maxnum;
    g_l2_vpls_maxnum = vpls_maxnum;

    return 0;
}


/*****************************************************************************
 函 数 名  : spm_l2_create_vpn_maxnum
 功能描述  : 初始化调用驱动创建最大数量的vpws vpls
 输入参数  : NBB_ULONG vpws_maxnum  
             NBB_ULONG vpls_maxnum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月21日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_l2_create_vpn_maxnum(
    NBB_ULONG vpws_maxnum,  /* 设备所支持的最大vpws规格 (hdjia) */
    NBB_ULONG vpls_maxnum   /* 设备所支持的最大vpls规格 (hdjia) */
    )
{
    NBB_INT         ret = SUCCESS;
    NBB_ULONG       vpn_id = 0; 

    NBB_TRC_ENTRY("spm_l2_create_vpn_maxnum_init");

    /* vpws create  (hdjia) */
    for (vpn_id = 1; vpn_id <= g_l2_vpws_maxnum; vpn_id++)
    {
        ret = spm_drv_create_mpls_vpn(L2VPN_VPWS, vpn_id);
    }

    /* VPLS create  (hdjia) */
    for (vpn_id = 1; vpn_id <= g_l2_vpws_maxnum; vpn_id++)
    {
        ret = spm_drv_create_mpls_vpn(L2VPN_VPLS, vpn_id);
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}
    

/*****************************************************************************
 函 数 名  : spm_l2_vpn_maxnum_init
 功能描述  : 1、设置设备所支持的vpws、vpls最大数目；2、创建vpn
 输入参数  : NBB_ULONG vpws_maxnum  
             NBB_ULONG vpls_maxnum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月21日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_l2_vpn_maxnum_init(
    NBB_ULONG vpws_maxnum,  /* 设备所支持的最大vpws规格 (hdjia) */
    NBB_ULONG vpls_maxnum   /* 设备所支持的最大vpls规格 (hdjia) */
    )
{
    spm_l2_set_vpn_maxnum_value(vpws_maxnum, vpls_maxnum);

    spm_l2_create_vpn_maxnum(vpws_maxnum, vpls_maxnum);
}

/*****************************************************************************
   函 数 名    : spm_logical_port_basic_data_proc
   功能描述    : 端口逻辑配置块基本配置处理 1)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      本地数据库指针 
               : NBB_ULONG                  ul_oper_basic          操作码
                                   
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数
   2.日    期 : 2016年1月19日
     作    者 : fansongbo
     修改内容 : 8000解耦   
*****************************************************************************/
NBB_INT spm_logical_port_basic_data_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_BASIC_DATA *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;    
    NBB_INT ret = SUCCESS; 
        
    NBB_TRC_ENTRY("spm_logical_port_basic_data_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_BASIC_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_BASIC_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*增加*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->basic_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = LOG_PORT_BASIC_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_BASIC_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  1）Basic Config\n");
                    spm_dbg_print_logical_port_basic_cfg(pst_tlv_cfg);
                }
                              
                OS_SPRINTF(uc_message,"  1）Basic Config\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_basic_cfg(pst_tlv_cfg);
                }
                
                spm_logical_port_apply_set_basic_data(pst_logical_port,pst_tlv_cfg);
                
                /* 保存数据 */
                if (NULL == pst_logical_port->basic_cfg_cb)
                {
                    pst_logical_port->basic_cfg_cb =
                        (ATG_DCI_LOG_PORT_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_BASIC_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_BASIC_CB);

                    if(NULL == pst_logical_port->basic_cfg_cb)
                    {
                        ret = LOG_PORT_BASIC_PROC_ALLOC_CB_FAIL;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }              
                OS_MEMCPY(pst_logical_port->basic_cfg_cb, pst_tlv_cfg, 
                    sizeof(ATG_DCI_LOG_PORT_BASIC_DATA));             
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = LOG_PORT_BASIC_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = LOG_PORT_BASIC_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_BASIC_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名    : spm_logical_port_phy_data_proc
   功能描述    : 端口逻辑配置块物理配置处理 2)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
               : NBB_ULONG              ul_oper_phy            操作码
               : SPM_LOGICAL_PORT_CB    *pst_logical_port      本地数据库指针                                    
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_phy_data_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_PHY_DATA *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 

    NBB_TRC_ENTRY("spm_logical_port_phy_data_proc");
      
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_PHY_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_PHY_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->physical_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = LOG_PORT_PHY_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret); 
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_PHY_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  2）Physical Config\n");
                    spm_dbg_print_logical_port_phy_cfg(pst_tlv_cfg);
                }

                OS_SPRINTF(uc_message,"  2）Physical Config\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                               
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_phy_cfg(pst_tlv_cfg);
                }
                
                spm_logical_port_apply_set_phy_data(pst_logical_port,pst_tlv_cfg);
                
                /* 保存数据 */
                if (NULL == pst_logical_port->phy_cfg_cb)
                {
                    pst_logical_port->phy_cfg_cb =
                        (ATG_DCI_LOG_PORT_PHY_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_PHY_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_PHY_CB);

                    if(NULL == pst_logical_port->phy_cfg_cb)
                    {
                        ret = LOG_PORT_PHY_PROC_ALLOC_CB_FAIL;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }              
                OS_MEMCPY(pst_logical_port->phy_cfg_cb, pst_tlv_cfg, 
                    sizeof(ATG_DCI_LOG_PORT_PHY_DATA));         
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = LOG_PORT_PHY_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = LOG_PORT_PHY_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_PHY_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    EXIT_LABEL :NBB_TRC_EXIT();
    
    if (NULL != pst_logical_port)
    {
        pst_logical_port->phy_cfg_flag = ret;
    }
    
    return ret;
}

/*****************************************************************************
   函 数 名    : spm_logical_port_logic_l3_proc
   功能描述    : 端口逻辑配置块逻辑配置L3配置处理 3)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
               : NBB_ULONG                   uloperbasic           操作码
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      本地数据库指针                                    
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年11月3日
     作    者  : lish
     修改内容  : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_logic_l3_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper_logic_l3)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *pst_logic_l3_data = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_logic_l3_data_start = NULL;
    NBB_BYTE c3_unit = 0; 
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    SPM_PORT_INFO_CB st_port_info;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    NBB_ULONG intf_pos_id;
    
    NBB_TRC_ENTRY("spm_logical_port_logic_l3_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_LOGIC_L3_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_LOGIC_L3_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    switch(ul_oper_logic_l3)
    { 
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
            /* 计算第entry的地址。*/
            puc_logic_l3_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->logical_l3_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_logic_l3_data_start)
            {
                ret = LOG_PORT_LOGIC_L3_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);
            }
            else
            {
                pst_logic_l3_data = (ATG_DCI_LOG_PORT_LOGIC_L3_DATA *)puc_logic_l3_data_start;

                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  3）Logical Config L3\n");
                    spm_dbg_print_logical_port_logic_l3_cfg(pst_logic_l3_data);
                }

                OS_SPRINTF(uc_message, "  3）Logical Config L3\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_logic_l3_cfg(pst_logic_l3_data);
                }                
                
                spm_logical_port_drv_set_logic_l3(pst_logical_port,pst_logic_l3_data);
                
                /* 保存数据 */
                if (NULL == pst_logical_port->logic_l3_cfg_cb)
                {
                    pst_logical_port->logic_l3_cfg_cb =
                        (ATG_DCI_LOG_PORT_LOGIC_L3_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_L3_CB);

                    if (NULL == pst_logical_port->logic_l3_cfg_cb)
                    {
                        ret = LOG_PORT_LOGIC_L3_PROC_ALLOC_CB_FAIL;    
                        spm_l2_record_err_code(ret);       
                        goto EXIT_LABEL;           			
                    }
                }

                OS_MEMCPY(pst_logical_port->logic_l3_cfg_cb, pst_logic_l3_data,
                    sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA));
            }
                        
            break; 
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = LOG_PORT_LOGIC_L3_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = LOG_PORT_LOGIC_L3_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = LOG_PORT_LOGIC_L3_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret);
            break;
    }

    EXIT_LABEL :NBB_TRC_EXIT();
    if (NULL != pst_logical_port)
    {
        pst_logical_port->logical_l3_cfg_flag = ret;
    }
    return ret;
}


/*****************************************************************************
   函 数 名    : spm_logical_port_phy_proc
   功能描述    : 端口逻辑配置块逻辑配置L2配置处理 4)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息                   
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      本地数据库指针
               : NBB_ULONG                  uloperbasic            操作码
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数
   2.日    期 : 2016年1月14日
     作    者 : fansongbo
     修改内容 : 8000解耦    
*****************************************************************************/
NBB_INT spm_logical_port_logic_l2_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper_logic_l2)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_logic_l2_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_logic_l2_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_LOGIC_L2_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_LOGIC_L2_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    switch(ul_oper_logic_l2)
    {
        case ATG_DCI_OPER_ADD:                     /*增加*/
            
           /* 计算第entry的地址。*/
            puc_logic_l2_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->logical_l2_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_logic_l2_data_start)
            {
                ret = LOG_PORT_LOGIC_L2_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret);              
            }
            else    /*  配置正常下发*/
            {
                pst_logic_l2_data = (ATG_DCI_LOG_PORT_LOGIC_L2_DATA *)puc_logic_l2_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  4）Logical Config L2\n");
                    spm_dbg_print_logical_port_logic_l2_cfg(pst_logic_l2_data);
                }

                OS_SPRINTF(uc_message, "  4）Logical Config L2\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_logic_l2_cfg(pst_logic_l2_data);
                }                   
                
                spm_logical_port_apply_set_logic_l2(pst_logical_port,pst_logic_l2_data);
                
                /* 保存数据 */
                if (NULL == pst_logical_port->logic_l2_cfg_cb)
                {
                    pst_logical_port->logic_l2_cfg_cb =
                        (ATG_DCI_LOG_PORT_LOGIC_L2_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_L2_CB);

                    if (NULL == pst_logical_port->logic_l2_cfg_cb)
                    {
                        ret = LOG_PORT_LOGIC_L2_PROC_ALLOC_CB_FAIL;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->logic_l2_cfg_cb, pst_logic_l2_data,
                    sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));               
            }
            break;
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = LOG_PORT_LOGIC_L2_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = LOG_PORT_LOGIC_L2_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_LOGIC_L2_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名    : spm_logical_port_l2vpn_proc
   功能描述    : 端口逻辑配置块L2VPN配置处理 5)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息                   
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      本地数据库指针
               : NBB_ULONG                   ul_oper_l2vpn            操作码
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_l2vpn_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper_l2vpn)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_l2vpn_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_BYTE ul_oper_mode = 0;  
	SPM_VPLS_CB *pst_vpls = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_l2vpn_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_L2VPN_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_L2VPN_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
	
    switch(ul_oper_l2vpn)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL: 
            
           /* 计算第entry的地址。*/
            puc_l2vpn_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->l2vpn_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_l2vpn_data_start)
            {
                ret = LOG_PORT_L2VPN_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret); 
            }
            else
            {				
                pst_l2vpn_data = (ATG_DCI_LOG_PORT_L2VPN_DATA *)puc_l2vpn_data_start;
                
                ul_oper_mode = ul_oper_l2vpn - 1;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  5）L2VPN Config %s\n",*(cfg_oper_type_string + ul_oper_mode));                 
                    spm_dbg_print_logical_port_l2vpn_cfg(pst_l2vpn_data);
                }

                OS_SPRINTF(uc_message, "  5）L2VPN Config %s\n",*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_l2vpn_cfg(pst_l2vpn_data);
                }
                
                if(ATG_DCI_OPER_ADD == ul_oper_l2vpn)
                {
                    spm_logical_port_apply_add_l2vpn(pst_logical_port,pst_l2vpn_data);
					
                    
                    /* 保存数据 */
                    if (NULL == pst_logical_port->l2vpn_cfg_cb)
                    {
                        pst_logical_port->l2vpn_cfg_cb =
                            (ATG_DCI_LOG_PORT_L2VPN_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_L2VPN_DATA),
                            NBB_NORETRY_ACT,
                            MEM_SPM_LOG_PORT_L2VPN_CB);

                        if (NULL == pst_logical_port->l2vpn_cfg_cb)
                        {
                            ret = LOG_PORT_L2VPN_PROC_ALLOC_CB_FAIL;
                            spm_l2_record_err_code(ret);
                            goto EXIT_LABEL;
                        }
                    }
                    
                    OS_MEMCPY(pst_logical_port->l2vpn_cfg_cb, pst_l2vpn_data,
                        sizeof(ATG_DCI_LOG_PORT_L2VPN_DATA));    
                }               
                else
                {
                    spm_logical_port_apply_del_l2vpn(pst_logical_port,pst_l2vpn_data);
                    NBB_MM_FREE(pst_logical_port->l2vpn_cfg_cb, MEM_SPM_LOG_PORT_L2VPN_CB);
                    pst_logical_port->l2vpn_cfg_cb = NULL;
                }                
            }            
            break;   
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = LOG_PORT_L2VPN_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_L2VPN_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    
    if (NULL != pst_logical_port)
    {
        pst_logical_port->l2vpn_cfg_flag = ret;
    }
    
    return ret;
}


/*****************************************************************************
   函 数 名     : spm_logical_port_ces_data_proc
   功能描述    : 端口逻辑配置块逻辑配置	仿真配置处理 6)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_ces_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ces NBB_CCXT_T NBB_CXT)
{
}


/*****************************************************************************
   函 数 名     : spm_logical_port_ipv4_data_proc
   功能描述    : 端口逻辑配置块Ipv4地址配置L3配置处理 7)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_ipv4_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ipv4)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_IPV4_DATA *pst_ipv4_data = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_ipv4_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv4_data_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    switch(ul_oper_ipv4)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_ipv4_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv4_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_ipv4_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  7) Ipv4 Address Config L3 (ipv4_num = %d) ADD\n", 
                        pst_set_logical_port->ipv4_num);
                }

                OS_SPRINTF(uc_message, "  7) Ipv4 Address Config L3 (ipv4_num = %d) ADD\n", 
                    pst_set_logical_port->ipv4_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv4_num; loop++)
                {
                    pst_ipv4_data = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                        (puc_ipv4_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv4_cfg(pst_ipv4_data);
                    }
                    
                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv4_cfg(pst_ipv4_data);
                    }

                    //检查ipv4是否配置过
                    icfg_pos = spm_logical_port_check_if_ipv4_exist(pst_logical_port, pst_ipv4_data);

                    //新增
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV4_NUM; j++)
                        {
                            //如果为NULL，说明这个位置为无数据，可以存储数据
                            if (NULL == pst_logical_port->ipv4_cfg_cb[j])
                            {
                                pst_logical_port->ipv4_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_IPV4_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_IPV4_CB);
                                
                                if (NULL == pst_logical_port->ipv4_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->ipv4_cfg_cb[j], pst_ipv4_data,
                                    sizeof(ATG_DCI_LOG_PORT_IPV4_DATA));
                                pst_logical_port->ipv4_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //有重复，更新配置
                    {
                        OS_MEMCPY(pst_logical_port->ipv4_cfg_cb[icfg_pos - 1], pst_ipv4_data,
                            sizeof(ATG_DCI_LOG_PORT_IPV4_DATA));
                    }
                }               
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*删除*/
            
           /* 计算第entry的地址。*/
            puc_ipv4_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv4_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_ipv4_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  7) Ipv4 Address Config L3 (ipv4_num = %d) DEL\n", pst_set_logical_port->ipv4_num);
                }

                OS_SPRINTF(uc_message, "  7) Ipv4 Address Config L3 (ipv4_num = %d) DEL\n", 
                    pst_set_logical_port->ipv4_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv4_num; loop++)
                {
                    pst_ipv4_data = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                        (puc_ipv4_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv4_cfg(pst_ipv4_data);
                    }

                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv4_cfg(pst_ipv4_data);
                    }
                    
                    //检查ipv4是否配置过
                    icfg_pos = spm_logical_port_check_if_ipv4_exist(pst_logical_port, pst_ipv4_data);

                    if(0 < icfg_pos)    //有配置，可以删除
                    {
                        NBB_MM_FREE(pst_logical_port->ipv4_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_IPV4_CB);
                        pst_logical_port->ipv4_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->ipv4_num--;                        
                    }                     
                }               
            } 
            break;
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_ipv6_data_proc
   功能描述    : 端口逻辑配置块Ipv6地址配置L3配置处理 8)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_ipv6_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ipv6)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_IPV6_DATA *pst_ipv6_data = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_ipv6_data_start = NULL;

    /* 逻辑接口的键值 */
    NBB_INT ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv6_data_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper_ipv6)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_ipv6_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv6_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_ipv6_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  8) Ipv6 Address Config L3 (ipv6_num = %d) ADD\n", 
                        pst_set_logical_port->ipv6_num);
                }

                OS_SPRINTF(uc_message, "  8) Ipv6 Address Config L3 (ipv6_num = %d) ADD\n", 
                    pst_set_logical_port->ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv6_num; loop++)
                {
                    pst_ipv6_data = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                        (puc_ipv6_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv6_cfg(pst_ipv6_data);
                    }
                    
                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv6_cfg(pst_ipv6_data);
                    }

                    //检查ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_ipv6_exist(pst_logical_port, pst_ipv6_data);

                    //新增
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV6_NUM; j++)
                        {
                            //如果为NULL，说明这个位置为无数据，可以存储数据
                            if (NULL == pst_logical_port->ipv6_cfg_cb[j])
                            {
                                pst_logical_port->ipv6_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_IPV6_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_IPV6_CB);
                                
                                if (NULL == pst_logical_port->ipv6_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->ipv6_cfg_cb[j], pst_ipv6_data,
                                    sizeof(ATG_DCI_LOG_PORT_IPV6_DATA));
                                pst_logical_port->ipv6_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //有重复，更新配置
                    {
                        OS_MEMCPY(pst_logical_port->ipv6_cfg_cb[icfg_pos - 1], pst_ipv6_data,
                            sizeof(ATG_DCI_LOG_PORT_IPV6_DATA));
                    }
                }               
            }   
            
            break;       
        case ATG_DCI_OPER_DEL:                            /*删除*/
            
           /* 计算第entry的地址。*/
            puc_ipv6_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv6_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_ipv6_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  8) Ipv6 Address Config L3 (ipv6_num = %d) DEL\n", pst_set_logical_port->ipv6_num);
                }

                OS_SPRINTF(uc_message, "  8) Ipv6 Address Config L3 (ipv6_num = %d) DEL\n", 
                    pst_set_logical_port->ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv6_num; loop++)
                {
                    pst_ipv6_data = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                        (puc_ipv6_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv6_cfg(pst_ipv6_data);
                    }

                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv6_cfg(pst_ipv6_data);
                    }
                    
                    //检查ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_ipv6_exist(pst_logical_port, pst_ipv6_data);

                    if(0 < icfg_pos)    //有配置，可以删除
                    {
                        NBB_MM_FREE(pst_logical_port->ipv6_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_IPV6_CB);
                        pst_logical_port->ipv6_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->ipv6_num--;                        
                    }                     
                }               
            }   
            break;
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名     : spm_logical_port_mc_ipv4_proc
   功能描述    : 端口逻辑配置块IPV4组播组地址L3配置处理 9)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_mc_ipv4_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_mc_ipv4)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *pst_mc_ipv4 = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_mc_ipv4_start = NULL;

    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_mc_ipv4_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    switch(ul_oper_mc_ipv4)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_mc_ipv4_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv4_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_mc_ipv4_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) ADD\n", 
                        pst_set_logical_port->mc_ipv4_num);
                }

                //OS_SPRINTF(uc_message, "  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) ADD\n", 
                    //pst_set_logical_port->mc_ipv4_num);
                //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv4_num; loop++)
                {
                    pst_mc_ipv4 = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                        (puc_mc_ipv4_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv4_cfg(pst_mc_ipv4);
                    }

                    spm_logical_port_apply_set_mc_ipv4(pst_logical_port,pst_mc_ipv4);
                    
                    //检查mc ipv4是否配置过
                    icfg_pos = spm_logical_port_check_if_mc_ipv4_exist(pst_logical_port, pst_mc_ipv4);

                    //新增
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV4_NUM; j++)
                        {
                            //如果为NULL，说明这个位置为无数据，可以存储数据
                            if (NULL == pst_logical_port->mc_ipv4_cfg_cb[j])
                            {
                                pst_logical_port->mc_ipv4_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_MC_IPV4_CB);
                                
                                if (NULL == pst_logical_port->mc_ipv4_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->mc_ipv4_cfg_cb[j], pst_mc_ipv4,
                                    sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA));
                                pst_logical_port->mc_ipv4_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //有重复，更新配置
                    {
                        OS_MEMCPY(pst_logical_port->mc_ipv4_cfg_cb[icfg_pos - 1], pst_mc_ipv4,
                            sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA));
                    }
                }               
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*删除*/
            
           /* 计算第entry的地址。*/
            puc_mc_ipv4_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv4_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_mc_ipv4_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) DEL\n", 
                        pst_set_logical_port->mc_ipv4_num);
                }

                //OS_SPRINTF(uc_message, "  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) DEL\n", 
                    //pst_set_logical_port->mc_ipv4_num);
                //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv4_num; loop++)
                {
                    pst_mc_ipv4 = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                        (puc_mc_ipv4_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv4_cfg(pst_mc_ipv4);
                    }
                    
                    //检查mc ipv4是否配置过
                    icfg_pos = spm_logical_port_check_if_mc_ipv4_exist(pst_logical_port, pst_mc_ipv4);

                    if(0 < icfg_pos)    //有配置，可以删除
                    {
                        NBB_MM_FREE(pst_logical_port->mc_ipv4_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_MC_IPV4_CB);
                        pst_logical_port->mc_ipv4_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->mc_ipv4_num--;                        
                    }                     
                }               
            } 

            break;
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_mc_ipv6_proc
   功能描述    : 端口逻辑配置块IPV4组播组地址L3配置处理 22)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_mc_ipv6_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_mc_ipv6)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *pst_mc_ipv6 = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_mc_ipv6_start = NULL;

    /* 逻辑接口的键值 */
    NBB_USHORT mc_ve_portid = 0;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_mc_ipv6_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    switch(ul_oper_mc_ipv6)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_mc_ipv6_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv6_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_mc_ipv6_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                        pst_set_logical_port->mc_ipv6_num);
                }

                OS_SPRINTF(uc_message, "  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                    pst_set_logical_port->mc_ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv6_num; loop++)
                {
                    pst_mc_ipv6 = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                        (puc_mc_ipv6_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv6_cfg(pst_mc_ipv6);
                    }

                    //调用dk接口
                    spm_logical_port_apply_set_mc_ipv4(pst_logical_port,pst_mc_ipv6);
                        
                    //检查mc ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_mc_ipv6_exist(pst_logical_port, pst_mc_ipv6);

                    //新增
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV6_NUM; j++)
                        {
                            //如果为NULL，说明这个位置为无数据，可以存储数据
                            if (NULL == pst_logical_port->mc_ipv6_cfg_cb[j])
                            {
                                pst_logical_port->mc_ipv6_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_MC_IPV6_CB);
                                
                                if (NULL == pst_logical_port->mc_ipv6_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->mc_ipv6_cfg_cb[j], pst_mc_ipv6,
                                    sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA));
                                pst_logical_port->mc_ipv6_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //有重复，更新配置
                    {
                        OS_MEMCPY(pst_logical_port->mc_ipv6_cfg_cb[icfg_pos - 1], pst_mc_ipv6,
                            sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA));
                    }
                }               
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*删除*/
            
           /* 计算第entry的地址。*/
            puc_mc_ipv6_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv6_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_mc_ipv6_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                        pst_set_logical_port->mc_ipv6_num);
                }

                OS_SPRINTF(uc_message, "  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                    pst_set_logical_port->mc_ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv6_num; loop++)
                {
                    pst_mc_ipv6 = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                        (puc_mc_ipv6_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv6_cfg(pst_mc_ipv6);
                    }
                    
                    //检查mc ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_mc_ipv6_exist(pst_logical_port, pst_mc_ipv6);

                    if(0 < icfg_pos)    //有配置，可以删除
                    {
                        NBB_MM_FREE(pst_logical_port->mc_ipv6_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_MC_IPV6_CB);
                        pst_logical_port->mc_ipv6_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->mc_ipv6_num--;                        
                    }                     
                }               
            }
            
            break;
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_vip_vmac_proc
   功能描述    : 端口逻辑配置块VRRP虚MAC和IP地址配置L3配置处理 10)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_vip_vmac_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_vip_vmac)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pst_vip_vmac = NULL;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_vip_vmac_start = NULL;
    SPM_VRRP_CB st_vrrp_cb;
  
    /* 逻辑接口的键值 */
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    NBB_ULONG terminal_intf_num = 0;  
    NBB_ULONG long_mac[2] = {0};  
    
    NBB_TRC_ENTRY("spm_logical_port_vip_vmac_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    switch(ul_oper_vip_vmac)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_vip_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vip_vmac_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_vip_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vip_vmac_num);
                }

                OS_SPRINTF(uc_message, "  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vip_vmac_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                //找到vlan属性第一个配置
                pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);

                //没有vlan属性，无法绑定，打印错误，直接退出
                if(NULL == pst_terminal_if_cb)
                {
                    //此次下发的所有VRRP配置均返回失败
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                }
                else
                {
                    spm_logical_port_set_intf_vrrp(pst_logical_port);                
                }
                
                //初始化Vrrp的公共成员
                st_vrrp_cb.port_id = pst_logical_port->port_id;/*lint !e613 */
                st_vrrp_cb.slot_id = pst_logical_port->slot_id;/*lint !e613 */

                //VE口只需置VE标志，非VE口需要填vlan信息
                if(ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)/*lint !e613 */
                {
                    st_vrrp_cb.ve_flag = 1;
                }
                
                for (loop = 0; loop < pst_set_logical_port->vip_vmac_num; loop++)
                {
                    pst_vip_vmac = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                        (puc_vip_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * loop);

                    /* 配置处理，todo */
                    if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_logical_port_vip_vmac_cfg(pst_vip_vmac);
                    }

                    st_vrrp_cb.dmac[0] = pst_vip_vmac->mac[0];
                    st_vrrp_cb.dmac[1] = pst_vip_vmac->mac[1];
                    st_vrrp_cb.dmac[2] = pst_vip_vmac->mac[2];
                    st_vrrp_cb.dmac[3] = pst_vip_vmac->mac[3];
                    st_vrrp_cb.dmac[4] = pst_vip_vmac->mac[4];
                    st_vrrp_cb.dmac[5] = pst_vip_vmac->mac[5];   
                    spm_l2_convert_mac_to_ulong(&(pst_vip_vmac->mac[0]),long_mac);                  
                    spm_logical_port_drv_set_acl_vrrp(pst_logical_port,&st_vrrp_cb,&terminal_intf_num); 
                    
                    if(pst_logical_port->terminal_if_num != terminal_intf_num)
                    {
                        pst_set_logical_port->vip_vmac_return_code[loop] = ATG_DCI_RC_UNSUCCESSFUL;                      
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_three_value(ret,long_mac[0],long_mac[1],func_ret);
                        break;
                    }    
                    
                    //检查mc ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_vip_vmac_exist(pst_logical_port, pst_vip_vmac);                        

                    //新增
                    if ((0 == icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                        {
                            //如果为NULL，说明这个位置为无数据，可以存储数据
                            if (NULL == pst_logical_port->vip_vmac_cfg_cb[j])
                            {
                                pst_logical_port->vip_vmac_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_VIP_VMAC_CB);
                                
                                if (NULL == pst_logical_port->vip_vmac_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->vip_vmac_cfg_cb[j], pst_vip_vmac,
                                    sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA));
                                pst_logical_port->logic_port_info_cb.vip_vmac_posid[j][0] = st_vrrp_cb.posid[0];
                                pst_logical_port->logic_port_info_cb.vip_vmac_posid[j][1] = st_vrrp_cb.posid[1];
                                pst_logical_port->vip_vmac_num++;
                                break;
                            }
                        }
                    }
                    else if ((0 < icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))    //有重复，更新配置
                    {
                        OS_MEMCPY(pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1], pst_vip_vmac,
                            sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA));
                    }
                }                                   
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*删除*/
            
           /* 计算第entry的地址。*/
            puc_vip_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vip_vmac_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_vip_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) DEL\n", 
                        pst_set_logical_port->vip_vmac_num);
                }

                OS_SPRINTF(uc_message, "  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) DEL\n", 
                        pst_set_logical_port->vip_vmac_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->vip_vmac_num; loop++)
                {
                    pst_vip_vmac = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                        (puc_vip_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * loop);

                    /* 配置处理，todo */
                    if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_logical_port_vip_vmac_cfg(pst_vip_vmac);
                    }
                                            
                    //检查mc ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_vip_vmac_exist(pst_logical_port, pst_vip_vmac);                        

                    //存在，需要删除驱动
                    if (0 < icfg_pos)
                    {
                        func_ret = SUCCESS;
                        func_ret = spm_logical_port_drv_del_acl_vrrp(pst_logical_port,icfg_pos - 1);

                        if(SUCCESS != func_ret)
                        {
                            pst_set_logical_port->vip_vmac_return_code[loop] = ATG_DCI_RC_UNSUCCESSFUL;                      
                        } 
                        
                        if (NULL != pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1])
                        {
                            NBB_MM_FREE(pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1],
                                MEM_SPM_LOG_PORT_VIP_VMAC_CB);
                            pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1] = NULL;                               
                        }
                        
                        pst_logical_port->vip_vmac_num--;       
                    }
                    else
                    {
                        pst_set_logical_port->vip_vmac_return_code[loop] = ATG_DCI_RC_UNSUCCESSFUL; 
                        ret = SPM_LOGICAL_E_PARAM_NULL;    
                        spm_l2_record_err_code(ret);  
                    }
                }                                   
            }            
            break;    
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_vipv6_vmac_proc
   功能描述    : 端口逻辑配置块VRRP虚MAC和IP地址配置L3配置处理25)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_vipv6_vmac_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_vipv6_vmac)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac = NULL;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL; 
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_vipv6_vmac_start = NULL;
    SPM_VRRP_CB st_vrrp_cb;
  
    /* 逻辑接口的键值 */
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    NBB_ULONG terminal_intf_num = 0;  
    NBB_ULONG long_mac[2] = {0};  
    
    NBB_TRC_ENTRY("spm_logical_port_vipv6_vmac_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    OS_MEMSET(&st_vrrp_cb, 0, sizeof(SPM_VRRP_CB));   

    switch(ul_oper_vipv6_vmac)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_vipv6_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vrrp_ipv6_mac_l3_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_vipv6_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                }

                OS_SPRINTF(uc_message, "  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                //找到vlan属性第一个配置
                pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);

                //没有vlan属性，无法绑定，打印错误，直接退出
                if(NULL == pst_terminal_if_cb)
                {
                    //此次下发的所有VRRP配置均返回失败
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                }
                else
                {
                    spm_logical_port_set_intf_vrrp(pst_logical_port);                
                }
                
                //初始化Vrrp的公共成员
                st_vrrp_cb.port_id = pst_logical_port->port_id;/*lint !e613 */
                st_vrrp_cb.slot_id = pst_logical_port->slot_id;/*lint !e613 */

                //VE口只需置VE标志，非VE口需要填vlan信息
                if(ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)/*lint !e613 */
                {
                    st_vrrp_cb.ve_flag = 1;
                }
                
                for (loop = 0; loop < pst_set_logical_port->vrrp_ipv6_mac_l3_num; loop++)
                {
                    pst_vipv6_vmac = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                        (puc_vipv6_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * loop);

                    /* 配置处理，todo */
                    if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac);
                    }

                    st_vrrp_cb.dmac[0] = pst_vipv6_vmac->mac_address[0];
                    st_vrrp_cb.dmac[1] = pst_vipv6_vmac->mac_address[1];
                    st_vrrp_cb.dmac[2] = pst_vipv6_vmac->mac_address[2];
                    st_vrrp_cb.dmac[3] = pst_vipv6_vmac->mac_address[3];
                    st_vrrp_cb.dmac[4] = pst_vipv6_vmac->mac_address[4];
                    st_vrrp_cb.dmac[5] = pst_vipv6_vmac->mac_address[5];   
                    spm_l2_convert_mac_to_ulong(&(pst_vipv6_vmac->mac_address[0]),long_mac);                  
                    spm_logical_port_drv_set_acl_vrrp(pst_logical_port,&st_vrrp_cb,&terminal_intf_num); 
                    
                    if(pst_logical_port->terminal_if_num != terminal_intf_num)
                    {                     
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_three_value(ret,long_mac[0],long_mac[1],func_ret);
                        break;
                    }    
                    
                    //检查mc ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_vipv6_vmac_exist(pst_logical_port, pst_vipv6_vmac);                        

                    //新增
                    if ((0 == icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                        {
                            //如果为NULL，说明这个位置为无数据，可以存储数据
                            if (NULL == pst_logical_port->vipv6_vmac_cfg_cb[j])
                            {
                                pst_logical_port->vipv6_vmac_cfg_cb[j] =
                                    (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
                                
                                if (NULL == pst_logical_port->vipv6_vmac_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->vipv6_vmac_cfg_cb[j], pst_vipv6_vmac,
                                    sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3));
                                pst_logical_port->logic_port_info_cb.vipv6_vmac_posid[j][0] = st_vrrp_cb.posid[0];
                                pst_logical_port->logic_port_info_cb.vipv6_vmac_posid[j][1] = st_vrrp_cb.posid[1];
                                pst_logical_port->vipv6_vmac_num++;
                                break;
                            }
                        }
                    }
                    else if ((0 < icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))    //有重复，更新配置
                    {
                        OS_MEMCPY(pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1], pst_vipv6_vmac,
                            sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3));
                    }
                }                                   
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*删除*/
            
           /* 计算第entry的地址。*/
            puc_vipv6_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vrrp_ipv6_mac_l3_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_vipv6_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                }

                OS_SPRINTF(uc_message, "  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->vrrp_ipv6_mac_l3_num; loop++)
                {
                    pst_vipv6_vmac = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                        (puc_vipv6_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * loop);

                    /* 配置处理，todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        //spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac);
                    }
                                            
                    //检查mc ipv6是否配置过
                    icfg_pos = spm_logical_port_check_if_vipv6_vmac_exist(pst_logical_port, pst_vipv6_vmac);                        

                    //存在，需要删除驱动
                    if (0 < icfg_pos)
                    {
                        func_ret = SUCCESS;
                        func_ret = spm_logical_port_drv_del_acl_vrrp(pst_logical_port,icfg_pos - 1);

                        if(SUCCESS != func_ret)
                        {
                            pst_set_logical_port->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;                      
                        } 
                        
                        if (NULL != pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1])
                        {
                            NBB_MM_FREE(pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1],
                                MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
                            pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1] = NULL;                               
                        }
                        
                        pst_logical_port->vipv6_vmac_num--;       
                    }
                    else
                    {
                        pst_set_logical_port->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL; 
                        ret = SPM_LOGICAL_E_PARAM_NULL;    
                        spm_l2_record_err_code(ret);  
                    }
                }                                   
            }            
            break;    
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名     : spm_logical_port_ve_mac_proc
   功能描述    : 端口逻辑配置块VE MAC地址配置配置处理
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_ve_mac_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ve_mac)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_VE_MAC_DATA *pst_ve_mac = NULL;
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_ve_mac_start = NULL;
  
    /* 逻辑接口的键值 */
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_ve_mac_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    
    
    if (NULL == pst_logical_port->phy_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    switch(ul_oper_ve_mac)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
            
           /* 计算第entry的地址。*/
            puc_ve_mac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ve_mac_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_ve_mac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                pst_ve_mac = (ATG_DCI_LOG_PORT_VE_MAC_DATA *)puc_ve_mac_start;

                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  11）Ve Mac Config\n");
                    spm_dbg_print_logical_port_ve_mac_cfg(pst_ve_mac);
                }

                OS_SPRINTF(uc_message, "  11）Ve Mac Config\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                spm_logical_port_drv_set_ve_port_mac(pst_logical_port,pst_ve_mac);
                
                /* 保存数据 */
                if (NULL == pst_logical_port->ve_mac_cfg_cb)
                {
                    pst_logical_port->ve_mac_cfg_cb =
                        (ATG_DCI_LOG_PORT_VE_MAC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_VE_MAC_CB);
                    
                    if (NULL == pst_logical_port->ve_mac_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_PARAM_NULL;
                        spm_l2_record_err_code(ret);   
                        goto EXIT_LABEL;                                   
                    }
                }

                OS_MEMCPY(pst_logical_port->ve_mac_cfg_cb, pst_ve_mac, sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA));                           
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;  
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;            
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_diff_serv_proc
   功能描述    : 端口逻辑配置块,12）Diff-Serv配置 L3专用
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_diff_serv_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_DIFF_SERV_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;         
    
    NBB_TRC_ENTRY("spm_logical_port_diff_serv_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL:                           /*删除*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->diff_serv_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {     
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  12）Diff-serv Config L3 (num = %d) %s\n",
                        pst_set_logical_port->diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  12）Diff-serv Config L3 (num = %d) %s\n",
                    pst_set_logical_port->diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->diff_serv_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_diff_serv_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //如果vlan配置不存在，退出
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->diff_serv_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_diff_serv_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                            AVLL_INSERT(pst_logical_port->diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_diff_serv_node);
                            pst_logical_port->diff_serv_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                        }                        
                    }
                    else
                    {
                        //待讨论 fansongbo
                        pst_tlv_cfg->l3_in_diff_serv_id = 0;
                        pst_tlv_cfg->l3_out_diff_serv_id = 0;
                        func_ret = spm_logical_port_drv_add_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->diff_serv_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_diff_serv_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_diff_serv_cb(pst_tlv_cfg_cb);
                            pst_logical_port->diff_serv_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_flow_diff_serv_proc
   功能描述    : 端口逻辑配置块流相关Diff-Serv配置L2配置处理 13)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_flow_diff_serv_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_FLOW_DIFF_SERV_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;     
    
    NBB_TRC_ENTRY("spm_logical_port_flow_diff_serv_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL:                           /*删除*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->flow_diff_serv_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {      
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  13）Flow Diff-serv Config L2 (num = %d) %s\n",
                        pst_set_logical_port->flow_diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  13）Flow Diff-serv Config L2 (num = %d) %s\n",
                        pst_set_logical_port->flow_diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->flow_diff_serv_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_flow_diff_serv_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //如果vlan配置不存在，退出
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_flow_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_diff_serv_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_flow_diff_serv_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                            AVLL_INSERT(pst_logical_port->flow_diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_flow_diff_serv_node);
                            pst_logical_port->flow_diff_serv_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->l2_in_diff_serv_id = 0;
                        func_ret = spm_logical_port_drv_add_flow_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_diff_serv_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->flow_diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_flow_diff_serv_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_flow_diff_serv_cb(pst_tlv_cfg_cb);
                            pst_logical_port->flow_diff_serv_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名     : spm_logical_port_inclassify_qos_proc
   功能描述    : 端口逻辑配置块14号子TLV(L2 L3)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_inclassify_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_INCLASSIFY_QOS_CB *pst_tlv_cfg_cb = NULL;   
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE port_route_type = 0;
    NBB_BYTE ul_oper_mode = 0;     
    
    NBB_TRC_ENTRY("spm_logical_port_inclassify_qos_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL:                           /*删除*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->inclassify_qos_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  14）Inclassify Qos Config %s (num = %d) %s\n",
                        *(port_route_type_string + port_route_type),
                        pst_set_logical_port->inclassify_qos_num,
                        *(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  14）Inclassify Qos Config %s (num = %d) %s\n",
                    *(port_route_type_string + port_route_type),
                    pst_set_logical_port->inclassify_qos_num,
                    *(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->inclassify_qos_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_inclassify_qos_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //如果vlan配置不存在，退出
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_classify_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->inclassify_qos_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_inclassify_qos_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->inclassify_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                            AVLL_INSERT(pst_logical_port->inclassify_qos_tree, 
                                pst_tlv_cfg_cb->spm_inclassify_qos_node);
                            pst_logical_port->inclassify_qos_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->inclassify_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->qos_policy_index = 0;
                        func_ret = spm_logical_port_drv_add_classify_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->inclassify_qos_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->inclassify_qos_tree, 
                                pst_tlv_cfg_cb->spm_inclassify_qos_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_inclassify_qos_cb(pst_tlv_cfg_cb);
                            pst_logical_port->inclassify_qos_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_up_user_qos_policy_proc
   功能描述    : 端口逻辑配置块15）上话用户QOS策略配置 L3专用
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_up_user_qos_policy_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;      
    
    NBB_TRC_ENTRY("spm_logical_port_up_user_qos_policy_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL:                           /*删除*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->up_user_qos_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {   
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  15）UP User Qos Policy Config L3 (num = %d) %s\n",
                        pst_set_logical_port->up_user_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  15）UP User Qos Policy Config L3 (num = %d) %s\n",
                    pst_set_logical_port->up_user_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->up_user_qos_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_UP_USER_QOS_POLICY *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_up_user_qos_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //如果vlan配置不存在，退出
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->up_user_qos_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_up_user_qos_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
                            AVLL_INSERT(pst_logical_port->up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_up_user_qos_node);
                            pst_logical_port->up_user_qos_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->qos_policy_index = 0;
                        func_ret = spm_logical_port_drv_add_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->up_user_qos_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_up_user_qos_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_up_user_qos_cb(pst_tlv_cfg_cb);
                            pst_logical_port->up_user_qos_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_up_user_group_qos_proc
   功能描述    : 端口逻辑配置块上话用户组QOS策略配置配置处理 16)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_up_user_group_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_user_group_qos NBB_CCXT_T NBB_CXT)
{

}

/*****************************************************************************
   函 数 名     : spm_logical_port_down_user_queue_qos_proc
   功能描述    : 端口逻辑配置块 17)下话用户队列QOS策略配置 L2 L3 共用 放18)后面运行
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      本地数据库指针 
               : NBB_ULONG                  ul_oper_basic          操作码
                                   
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数
   2.日    期 : 2016年1月19日
     作    者 : fansongbo
     修改内容 : 8000解耦   
*****************************************************************************/
NBB_INT spm_logical_port_down_user_queue_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_BYTE port_route_type = 0;   
    
    NBB_TRC_ENTRY("spm_logical_port_down_user_queue_qos_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*增加*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->down_user_queue_qos_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)puc_tlv_cfg_data_start;
                
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  17）Down User Qos Policy Config %s ADD\n",
                        *(port_route_type_string + port_route_type));
                    spm_dbg_print_logical_port_down_user_queue_qos_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  17）Down User Qos Policy Config %s ADD\n",
                    *(port_route_type_string + port_route_type));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                func_ret = spm_logical_port_drv_add_down_user_queue_qos(pst_logical_port,pst_tlv_cfg);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_E_CALL_QOS;
                    spm_l2_record_err_code(ret);   
                    goto EXIT_LABEL;
                }
                
                /* 保存数据 */
                if (NULL == pst_logical_port->down_user_qos_cfg_cb)
                {
                    pst_logical_port->down_user_qos_cfg_cb = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);

                    if(NULL == pst_logical_port->down_user_qos_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->down_user_qos_cfg_cb, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));          
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名     : spm_logical_port_down_user_group_qos_proc
   功能描述    : 端口逻辑配置块 18)下话用户组QOS配置配置处理  L2 l3共用
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      本地数据库指针 
               : NBB_ULONG                  ul_oper_basic          操作码
                                   
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数
   2.日    期 : 2016年1月19日
     作    者 : fansongbo
     修改内容 : 8000解耦   
*****************************************************************************/
NBB_INT spm_logical_port_down_user_group_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_down_user_group_qos_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*增加*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->down_user_queue_qos_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  18）Down User Group Qos Config ADD\n");
                    spm_dbg_print_logical_port_down_user_group_qos_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  18）Down User Group Qos Config ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                func_ret = spm_logical_port_drv_add_down_user_group_qos(pst_logical_port,pst_tlv_cfg);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_E_CALL_QOS;
                    spm_l2_record_err_code(ret);   
                    goto EXIT_LABEL;
                }
                
                /* 保存数据 */
                if (NULL == pst_logical_port->down_user_group_qos_cfg_cb)
                {
                    pst_logical_port->down_user_group_qos_cfg_cb = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);

                    if(NULL == pst_logical_port->down_user_group_qos_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->down_user_group_qos_cfg_cb, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS));
                
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名     : spm_logical_port_flow_up_user_qos_proc
   功能描述    : 端口逻辑配置块流 19)相关上话用户QOS策略配置L2配置处理
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_flow_up_user_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_FLOW_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;  
    
    NBB_TRC_ENTRY("spm_logical_port_flow_up_user_qos_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL:                           /*删除*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->flow_upuser_qos_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {    
                ul_oper_mode = ul_oper - 1;
            
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  19) Flow Up User Qos Config L2 (num = %d) %s\n",
                        pst_set_logical_port->flow_upuser_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  19) Flow Up User Qos Config L2 (num = %d) %s\n",
                    pst_set_logical_port->flow_upuser_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (loop = 0; loop < pst_set_logical_port->flow_upuser_qos_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_FLOW_UP_USER_QOS *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_flow_upuser_qos(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //如果vlan配置不存在，退出
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_flow_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_up_user_qos_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_flow_up_user_qos_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS));
                            AVLL_INSERT(pst_logical_port->flow_up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_flow_up_user_qos_node);
                            pst_logical_port->flow_up_user_qos_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->qos_policy_index = 0;
                        func_ret = spm_logical_port_drv_add_flow_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_up_user_qos_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->flow_up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_flow_up_user_qos_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_flow_up_user_qos_cb(pst_tlv_cfg_cb);
                            pst_logical_port->flow_up_user_qos_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_traffic_filter_proc
   功能描述    : 端口逻辑配置块包过滤器traffic_filter配置处理 20) L2 L3共用
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_traffic_filter_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_TRAFFIC_FILTER_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE port_route_type = 0;
    NBB_BYTE ul_oper_mode = 0;    
    
    NBB_TRC_ENTRY("spm_logical_port_traffic_filter_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*增加*/
        case ATG_DCI_OPER_DEL:                           /*删除*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->traffic_filter_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {  
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  20) Traffic Filter Qos Config %s (num = %d) %s\n",
                        *(port_route_type_string + port_route_type),
                        pst_set_logical_port->traffic_filter_num,
                        *(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  20) Traffic Filter Qos Config %s (num = %d) %s\n",
                    *(port_route_type_string + port_route_type),
                    pst_set_logical_port->traffic_filter_num,
                    *(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->traffic_filter_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_traffic_filter_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //如果vlan配置不存在，退出
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_traffic_filter(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->traffic_filter_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_traffic_filter_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->traffic_filter_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                            AVLL_INSERT(pst_logical_port->traffic_filter_tree, 
                                pst_tlv_cfg_cb->spm_traffic_filter_node);
                            pst_logical_port->traffic_filter_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->traffic_filter_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->acl_id = 0;
                        func_ret = spm_logical_port_drv_add_traffic_filter(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->traffic_filter_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->traffic_filter_tree, 
                                pst_tlv_cfg_cb->spm_traffic_filter_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_traffic_filter_cb(pst_tlv_cfg_cb);
                            pst_logical_port->traffic_filter_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名    : spm_logical_port_vlan_proc
   功能描述    : 端口逻辑配置块VLAN属性L2配置处理
   输入参数    : ATG_DCI_SET_LOGICAL_PORT  *pst_set_logical_port  IPS消息
               : SPM_LOGICAL_PORT_CB       *pst_logical_port      本地数据库指针         
   输出参数    : 无
   返 回 值    :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期  : 2015年8月6日
     作    者  : lish
     修改内容  : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_vlan_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_VLAN *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    SPM_TERMINAL_IF_CB *pst_tlv_cfg_cb = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_ULONG loop = {0};
    NBB_BOOL avll_ret_code;
    NBB_BYTE port_route_type = 0;
    NBB_BYTE ul_oper_mode = 0;    
     
    NBB_TRC_ENTRY("spm_logical_port_vlan_proc"); 
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_VLAN_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_VLAN_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_VLAN_PROC_LOCAL_BASIC_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*增加*/
        case ATG_DCI_OPER_DEL:                      /*删除*/
            
            /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(
                pst_set_logical_port,
                &pst_set_logical_port->vlan_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = LOG_PORT_VLAN_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
            }
            else
            {       
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                ul_oper_mode = ul_oper - 1;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  21）VLAN %s (num = %d) %s\n", 
                        port_route_type_string [port_route_type],
                        pst_set_logical_port->vlan_num,
                        *(cfg_oper_type_string + ul_oper_mode));
                    
                }
                
                OS_SPRINTF(uc_message, "  21）VLAN %s (num = %d) %s\n", 
                    *(port_route_type_string + port_route_type),
                    pst_set_logical_port->vlan_num,
                    *(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (loop = 0; loop < pst_set_logical_port->vlan_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_VLAN*)
                        (puc_tlv_cfg_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * loop);
                        
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_terminal_if_cfg(pst_tlv_cfg);
                    }
                    
                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_terminal_if_cfg(pst_tlv_cfg);
                    }  
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        spm_logical_port_apply_add_vlan(pst_logical_port,pst_tlv_cfg);
                        
                        pst_tlv_cfg_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_terminal_if_cb(NBB_CXT);

                            if(NULL == pst_tlv_cfg_cb)
                            {
                                ret = LOG_PORT_VLAN_PROC_ALLOC_CB_FAIL;
                                spm_l2_record_err_code_and_two_value(ret,
                                    pst_tlv_cfg->svlan_id,pst_tlv_cfg->cvlan_id);
                                goto EXIT_LABEL;
                            }

                            //保存数据并插入树中
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->terminal_if_cfg), pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_VLAN));
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->intf_pos_id), &(pst_logical_port->temp_intf_pos_id), 2);
                            avll_ret_code = AVLL_INSERT(pst_logical_port->terminal_if_tree, 
                                pst_tlv_cfg_cb->spm_terminal_if_node);
                            NBB_ASSERT(avll_ret_code);
                            pst_logical_port->terminal_if_num++;
                        }
                        
                        NBB_MEMCPY(&(pst_tlv_cfg_cb->terminal_if_cfg), pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_VLAN));
                        NBB_MEMCPY(&(pst_tlv_cfg_cb->intf_pos_id), &(pst_logical_port->temp_intf_pos_id), 8);
                    }
                    else
                    {
                        pst_tlv_cfg_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, pst_tlv_cfg);

                        //如果不存在，申请树节点，插入树中，计数加1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = LOG_PORT_VLAN_PROC_FIND_DEL_CB_NULL;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            spm_logical_port_apply_del_vlan(pst_logical_port,pst_tlv_cfg_cb);
                            
                            AVLL_DELETE(pst_logical_port->terminal_if_tree, pst_tlv_cfg_cb->spm_terminal_if_node);

                            //释放端口逻辑配置节点的内存空间
                            spm_logical_port_free_terminal_if_cb(pst_tlv_cfg_cb);

                            pst_logical_port->terminal_if_num--;	
                        }   
                    }
                }            
            }
            break;
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = LOG_PORT_VLAN_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_VLAN_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;           
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名     : spm_logical_port_ds_l2_data_proc
   功能描述    : 端口逻辑配置块Diff-Serv配置L2（VP下话UNI侧）配置处理 23)
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_ds_l2_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_DS_L2_DATA *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_ds_l2_data_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*增加*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ds_l2_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_DS_L2_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  23）Diff-serv Config L2 ADD\n");
                    spm_dbg_print_logical_port_ds_l2_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  23）Diff-serv Config L2 ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                spm_logical_port_drv_add_ds_l2(pst_logical_port,pst_tlv_cfg);

                /* 保存数据 */
                if (NULL == pst_logical_port->ds_l2_cfg_cb)
                {
                    pst_logical_port->ds_l2_cfg_cb = (ATG_DCI_LOG_PORT_DS_L2_DATA *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_DS_L2_CB);

                    if(NULL == pst_logical_port->ds_l2_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->ds_l2_cfg_cb, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA));
                
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名     : spm_logical_port_car_data_proc
   功能描述    : 端口逻辑配置块car配置配置处理 24) L2
   输入参数    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS消息
                   : NBB_ULONG                              uloperbasic             操作码
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        本地数据库指针                                    
   输出参数    : 无
   返 回 值     :
   调用函数    :
   被调函数    :
   修改历史    :
   1.日    期   : 2015年8月6日
      作    者   : fansongbo
      修改内容 : 新生成函数

*****************************************************************************/
NBB_VOID spm_logical_port_car_data_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_LOG_PORT_CAR_DATA *pst_tlv_cfg = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_car_data_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*增加*/
            
           /* 计算第entry的地址。*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->car_data);   

            /* 首地址为NULL，异常 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_CAR_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  24）car Config L2 ADD\n");
                    spm_dbg_print_logical_port_car_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  24）car Config L2 ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                spm_logical_port_drv_add_car(pst_logical_port,pst_tlv_cfg);
    
                /* 保存数据 */
                if (NULL == pst_logical_port->car_cfg)
                {
                    pst_logical_port->car_cfg = (ATG_DCI_LOG_PORT_CAR_DATA *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_PORT_CAR_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_CAR_CB);

                    if(NULL == pst_logical_port->car_cfg)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->car_cfg, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_PORT_CAR_DATA));               
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_init_vp_tree
   功能描述  : 初始化存储VPWS配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vp_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vp_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_VP_CB, vp_idx);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vp_tree, compare_ushort,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_VP_CB, spm_vp_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_free_vp_cb
   功能描述  : 释放VPWS结构体的内存
   输入参数  : SPM_VPWS_CB *pstVpws NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_vp_cb(SPM_VPWS_CB *pst_vp)
{
    NBB_TRC_ENTRY("spm_free_vp_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_vp, sizeof(SPM_VP_CB), MEM_SPM_VP_CB);

    if (NULL == pst_vp)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vp, MEM_SPM_VP_CB);
    pst_vp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_alloc_vp_cb
   功能描述  : VPWS结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_VPWS_CB
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_VP_CB *spm_alloc_vp_cb()
{
    SPM_VP_CB *pst_vp = NULL;

    NBB_TRC_ENTRY("spm_alloc_vp_cb");

    /* 分配一个新的VPWS配置条目。*/
    pst_vp = (SPM_VP_CB *)NBB_MM_ALLOC(sizeof(SPM_VP_CB), NBB_NORETRY_ACT, MEM_SPM_VP_CB);
    
    if (NULL == pst_vp)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pst_vp, 0, sizeof(SPM_VP_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vp->spm_vp_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_vp);
}


/*****************************************************************************
   函 数 名  : spm_logical_port_alloc_logical_port_cb
   功能描述  : 端口逻辑配置结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_LOGICAL_PORT_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年10月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_LOGICAL_PORT_CB *spm_alloc_logical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    NBB_BUF_SIZE avll_key_offset;
    NBB_INT i = 0;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_alloc_logical_port_cb");

    /* 分配一个新的逻辑接口配置条目。*/
    pst_logical_port = (SPM_LOGICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_LOGICAL_PORT_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_LOGICAL_PORT_CB);
    
    if (NULL == pst_logical_port)
    {    
        goto EXIT_LABEL;
    }

    /* 初始化 */
    OS_MEMSET(pst_logical_port, 0, sizeof(SPM_LOGICAL_PORT_CB));

    /* 初始化指针 */
    pst_logical_port->basic_cfg_cb = NULL;
    pst_logical_port->phy_cfg_cb = NULL;
    pst_logical_port->logic_l3_cfg_cb = NULL;
    pst_logical_port->logic_l2_cfg_cb = NULL;
    pst_logical_port->l2vpn_cfg_cb = NULL;
    pst_logical_port->ces_cfg_cb = NULL;
    pst_logical_port->ve_mac_cfg_cb = NULL;
    pst_logical_port->up_user_group_qos_cfg_cb = NULL;
    pst_logical_port->down_user_qos_cfg_cb = NULL;
    pst_logical_port->down_user_group_qos_cfg_cb = NULL;
    pst_logical_port->ds_l2_cfg_cb = NULL;
    pst_logical_port->car_cfg = NULL;

    /***************************************************************************/
    /* 初始化IPV4组播组地址L3配置                                              */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        pst_logical_port->ipv4_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* 初始化IPv6地址配置L3配置                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        pst_logical_port->ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* 初始化IPV4组播配置                                                      */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        pst_logical_port->mc_ipv4_cfg_cb[i] = NULL;
    }

	/***************************************************************************/
    /* 初始化IPV6组播配置                                                      */
    /***************************************************************************/
	for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        pst_logical_port->mc_ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* 初始化VRRP虚MAC和IP地址配置L3配置                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pst_logical_port->vip_vmac_cfg_cb[i] = NULL;
    }
    
    /***************************************************************************/
    /* 初始化VRRP虚MAC和IPV6地址配置L3配置                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pst_logical_port->vipv6_vmac_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* 初始化用于存储SPM_TERMINAL_IF_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->terminal_if_tree, spm_logical_port_terminal_if_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, terminal_if_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, spm_terminal_if_node));


    /***************************************************************************/
    /* 初始化用于存储SPM_DIFF_SERV_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->diff_serv_tree, spm_logical_port_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, diff_serv_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, spm_diff_serv_node));

    /***************************************************************************/
    /* 初始化用于存储SPM_FLOW_DIFF_SERV_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->flow_diff_serv_tree, spm_logical_port_flow_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, flow_diff_serv_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, spm_flow_diff_serv_node));

    /***************************************************************************/
    /* 初始化用于存储SPM_INCLASSIFY_QOS_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->inclassify_qos_tree, spm_logical_port_inclassify_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, inclassify_qos_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, spm_inclassify_qos_node));
    
    /***************************************************************************/
    /* 初始化用于存储SPM_UP_USER_QOS_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->up_user_qos_tree, spm_logical_port_up_user_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_UP_USER_QOS_CB, up_user_qos_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_UP_USER_QOS_CB, spm_up_user_qos_node));

    /***************************************************************************/
    /* 初始化用于存储SPM_UP_USER_QOS_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->flow_up_user_qos_tree, spm_logical_port_flow_up_user_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_UP_USER_QOS_CB, flow_up_user_qos_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_UP_USER_QOS_CB, spm_flow_up_user_qos_node));

    /***************************************************************************/
    /* 初始化用于存储SPM_TRAFFIC_FILTER_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->traffic_filter_tree, spm_logical_port_traffic_filter_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, traffic_filter_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, spm_traffic_filter_node));  

    
    /* 建立用于该逻辑接口配置条目的句柄，作为异步消息交换的相关器。*/
    pst_logical_port->spm_logical_port_handle = NBB_CREATE_HANDLE(pst_logical_port,
        HDL_SPM_LOGICAL_PORT_CB);

    /* 成功分配一个新的逻辑接口配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_LOGICAL_PORT_CB allocated at %p with handle %#lx",
            pst_logical_port, pst_logical_port->spm_logical_port_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_logical_port->spm_logical_port_node);
    
	EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_logical_port);
}


/*****************************************************************************
   函 数 名  : spm_logical_port_free_logical_port_cb
   功能描述  : 释放端口逻辑配置节点的内存空间
   输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port
               NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月8日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_logical_port_cb(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_DIFF_SERV_CB *pst_diff_serv_cb = NULL;   
    SPM_FLOW_DIFF_SERV_CB *pst_flow_diff_serv_cb = NULL; 
    SPM_INCLASSIFY_QOS_CB *pst_inclassify_qos_cb = NULL; 
    SPM_UP_USER_QOS_CB *pst_up_user_qos_cb = NULL;  
    SPM_FLOW_UP_USER_QOS_CB *pst_flow_up_user_qos_cb = NULL; 
    SPM_TRAFFIC_FILTER_CB *pst_traffic_filter_cb = NULL;     
    AVLL_NODE node = {0};

    NBB_TRC_ENTRY("spm_free_logical_port_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_logical_port, sizeof(SPM_LOGICAL_PORT_CB), MEM_SPM_LOGICAL_PORT_CB);

    if (pst_logical_port == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    if (pst_logical_port->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->basic_cfg_cb, MEM_SPM_LOG_PORT_BASIC_CB);
        pst_logical_port->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放物理配置数据块。                                                    */
    /***************************************************************************/
    if (pst_logical_port->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->phy_cfg_cb, MEM_SPM_LOG_PORT_PHY_CB);
        pst_logical_port->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放逻辑配置L3数据块。                                                  */
    /***************************************************************************/
    if (pst_logical_port->logic_l3_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->logic_l3_cfg_cb, MEM_SPM_LOG_PORT_L3_CB);
        pst_logical_port->logic_l3_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放逻辑配置L2数据块。                                                  */
    /***************************************************************************/
    if (pst_logical_port->logic_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->logic_l2_cfg_cb, MEM_SPM_LOG_PORT_L2_CB);
        pst_logical_port->logic_l2_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放L2VPN数据块。                                                  */
    /***************************************************************************/
    if (pst_logical_port->l2vpn_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->l2vpn_cfg_cb, MEM_SPM_LOG_PORT_L2VPN_CB);
        pst_logical_port->l2vpn_cfg_cb = NULL;
    }
	
    /***************************************************************************/
    /* 释放逻辑配置CES/CEP数据块。                                             */
    /***************************************************************************/
    if (pst_logical_port->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->ces_cfg_cb, MEM_SPM_LOG_PORT_CES_CB);
        pst_logical_port->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放VE MAC地址数据块。                                             */
    /***************************************************************************/
    if (pst_logical_port->ve_mac_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->ve_mac_cfg_cb, MEM_SPM_LOG_PORT_VE_MAC_CB);
        pst_logical_port->ve_mac_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放上话用户组QOS策略配置配置处理配置数据块。                               */
    /***************************************************************************/
    if (pst_logical_port->up_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->up_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB);
        pst_logical_port->up_user_group_qos_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放下话用户队列QOS策略配置L3配置数据块。                               */
    /***************************************************************************/
    if (pst_logical_port->down_user_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->down_user_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
        pst_logical_port->down_user_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放下话用户组QOS配置L3配置数据块。                                     */
    /***************************************************************************/
    if (pst_logical_port->down_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->down_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
        pst_logical_port->down_user_group_qos_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放Diff-Serv配置L2（VP下话UNI侧）配置数据块。                          */
    /***************************************************************************/
    if (pst_logical_port->ds_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->ds_l2_cfg_cb, MEM_SPM_LOG_PORT_DS_L2_CB);
        pst_logical_port->ds_l2_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放CAR配置数据块。                                         */
    /***************************************************************************/
    if (pst_logical_port->car_cfg != NULL)
    {
        NBB_MM_FREE(pst_logical_port->car_cfg, MEM_SPM_LOG_PORT_CAR_CB);
        pst_logical_port->car_cfg = NULL;
    }
    
    /***************************************************************************/
    /* 释放Ipv4地址配置L3数据块。                                              */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        if (pst_logical_port->ipv4_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->ipv4_cfg_cb[i], MEM_SPM_LOG_PORT_IPV4_CB);
            pst_logical_port->ipv4_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* 释放Ipv6地址配置L3数据块。                                              */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        if (pst_logical_port->ipv6_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->ipv6_cfg_cb[i], MEM_SPM_LOG_PORT_IPV6_CB);
            pst_logical_port->ipv6_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* 释放组播组地址L3数据块。                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        if (pst_logical_port->mc_ipv4_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->mc_ipv4_cfg_cb[i], MEM_SPM_LOG_PORT_MC_IPV4_CB);
            pst_logical_port->mc_ipv4_cfg_cb[i] = NULL;
        }
    }
    
    /***************************************************************************/
    /* 释放IPV6组播组地址L3数据块。                                            */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        if (pst_logical_port->mc_ipv6_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->mc_ipv6_cfg_cb[i], MEM_SPM_LOG_PORT_MC_IPV6_CB);
            pst_logical_port->mc_ipv6_cfg_cb[i] = NULL;
        }
    }
    
    /***************************************************************************/
    /* 释放虚拟MAC和IP地址配置L3数据块。                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pst_logical_port->vip_vmac_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->vip_vmac_cfg_cb[i], MEM_SPM_LOG_PORT_VIP_VMAC_CB);
            pst_logical_port->vip_vmac_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* 释放VRRP虚MAC和IPV6地址配置L3配置                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pst_logical_port->vipv6_vmac_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->vipv6_vmac_cfg_cb[i], MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
            pst_logical_port->vipv6_vmac_cfg_cb[i] = NULL;
        }        
    }

    /***************************************************************************/
    /* 释放终结子接口配置数据块。                                              */
    /***************************************************************************/
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree))
    {
        AVLL_DELETE(pst_logical_port->terminal_if_tree, pst_terminal_if_cb->spm_terminal_if_node);
        spm_logical_port_free_terminal_if_cb(pst_terminal_if_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* 释放diff_serv配置数据块。                                               */
    /***************************************************************************/
    for (pst_diff_serv_cb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->diff_serv_tree);
        pst_diff_serv_cb != NULL;
        pst_diff_serv_cb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->diff_serv_tree))
    {
        AVLL_DELETE(pst_logical_port->diff_serv_tree, pst_diff_serv_cb->spm_diff_serv_node);
        spm_logical_port_free_diff_serv_cb(pst_diff_serv_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* 释放流相关Diff-Serv配置L2数据块。                                       */
    /***************************************************************************/
    for (pst_flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->flow_diff_serv_tree);
        pst_flow_diff_serv_cb != NULL;
        pst_flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->flow_diff_serv_tree))
    {
        AVLL_DELETE(pst_logical_port->flow_diff_serv_tree, pst_flow_diff_serv_cb->spm_flow_diff_serv_node);
        spm_logical_port_free_flow_diff_serv_cb(pst_flow_diff_serv_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* 释放上话复杂流分类QOS策略配置L3配置数据块。                             */
    /***************************************************************************/
    for (pst_inclassify_qos_cb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(pst_logical_port->inclassify_qos_tree);
         pst_inclassify_qos_cb != NULL;
         pst_inclassify_qos_cb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(pst_logical_port->inclassify_qos_tree))
    {
    	AVLL_DELETE(pst_logical_port->inclassify_qos_tree, pst_inclassify_qos_cb->spm_inclassify_qos_node);
    	spm_logical_port_free_inclassify_qos_cb(pst_inclassify_qos_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* 释放上话用户QOS策略配置配置数据块。                              */
    /***************************************************************************/
    for (pst_up_user_qos_cb = (SPM_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->up_user_qos_tree);
         pst_up_user_qos_cb != NULL;
         pst_up_user_qos_cb = (SPM_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->up_user_qos_tree))
    {
    	AVLL_DELETE(pst_logical_port->up_user_qos_tree, pst_up_user_qos_cb->spm_up_user_qos_node);
    	spm_logical_port_free_up_user_qos_cb(pst_up_user_qos_cb NBB_CCXT);
    }
         
    /***************************************************************************/
    /* 释放流 19)相关上话用户QOS策略配置配置数据块。                             */
    /***************************************************************************/
    for (pst_flow_up_user_qos_cb = (SPM_FLOW_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->flow_up_user_qos_tree);
         pst_flow_up_user_qos_cb != NULL;
         pst_flow_up_user_qos_cb = (SPM_FLOW_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->flow_up_user_qos_tree))
    {
    	AVLL_DELETE(pst_logical_port->flow_up_user_qos_tree, pst_flow_up_user_qos_cb->spm_flow_up_user_qos_node);
    	spm_logical_port_free_flow_up_user_qos_cb(pst_flow_up_user_qos_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* 释放流包过滤器traffic_filterL3配置数据块。                              */
    /***************************************************************************/
    for (pst_traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(pst_logical_port->traffic_filter_tree);
         pst_traffic_filter_cb != NULL;
         pst_traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(pst_logical_port->traffic_filter_tree))
    {
    	AVLL_DELETE(pst_logical_port->traffic_filter_tree, pst_traffic_filter_cb->spm_traffic_filter_node);
    	spm_logical_port_free_traffic_filter_cb(pst_traffic_filter_cb NBB_CCXT);
    }
         
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_logical_port->spm_logical_port_handle, HDL_SPM_LOGICAL_PORT_CB);

    /***************************************************************************/
    /* 现在释放端口逻辑配置控制块。                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_logical_port, MEM_SPM_LOGICAL_PORT_CB);
    pst_logical_port = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_init_logical_port_tree
   功能描述  : 初始化存储端口逻辑配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月24日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_logical_port_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_logical_port_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_LOGICAL_PORT_CB, port_index_key);
    AVLL_INIT_TREE(SHARED.logical_port_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_LOGICAL_PORT_CB, spm_logical_port_node));

    NBB_TRC_EXIT();
}


/*****************************************************************************
   函 数 名  : spm_logical_port_free_all_logical_port
   功能描述  : 释放端口逻辑配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年11月6日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_logical_port(NBB_CXT_T NBB_CXT)
{
	SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
	
    for (pst_logical_port = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
         pst_logical_port != NULL;
         pst_logical_port = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree))
    {
    	AVLL_DELETE(SHARED.logical_port_tree, pst_logical_port->spm_logical_port_node);
    	spm_free_logical_port_cb(pst_logical_port NBB_CCXT);
    }
}

/*****************************************************************************
   函 数 名   : spm_logical_port_process
   功能描述   : 端口逻辑配置处理
   输入参数   : ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port
   输出参数   : 无
   返 回 值   :
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数
   2.日    期 : 2016年1月14日
     作    者 : fansongbo
     修改内容 : 8000解耦    

*****************************************************************************/
NBB_VOID spm_logical_port_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port)
{
    //NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    /*记录下配置打印时间*/
    //struct timeval logic_tv_begin = {0};
    //struct timeval logic_tv_end = {0};
    //gettimeofday(&logic_tv_begin, NULL);

    /*记录开始处理配置时间*/
    //OS_SPRINTF(uc_message, "time_test logic_port_proc begin = %ld\n", logic_tv_begin.tv_usec);
    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);  

    /* 子配置的操作模式 */
    NBB_ULONG ul_oper_basic = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_phy = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_logic_l2 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_logic_l3 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_l2vpn = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_vlan = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_ipv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_ipv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_mc_ipv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_mc_ipv6 = ATG_DCI_OPER_NULL;    
    NBB_ULONG ul_oper_vip_vmac = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_vipv6_vmac = ATG_DCI_OPER_NULL;      
    NBB_ULONG ul_oper_ve_mac = ATG_DCI_OPER_NULL;       
    NBB_ULONG ul_oper_diff_serv = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_flow_diff_serv = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_inclassify_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_up_user_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_up_user_group_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_down_user_queue_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_down_user_group_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_flow_up_user_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_traffic_filter = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_ds_l2 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_car = ATG_DCI_OPER_NULL;
     
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    NBB_ULONG ul_port_key = 0;
    NBB_BYTE uc_if_exist = ATG_DCI_EXIST;
    NBB_BOOL avll_ret_code;
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_logical_port_process");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pst_set_logical_port != NULL);

    if (NULL == pst_set_logical_port)
    {      
        ret = LOG_PORT_PROC_IPS_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    ul_port_key = pst_set_logical_port->key;
    pst_logical_port = AVLL_FIND(SHARED.logical_port_tree, &ul_port_key);

    if (NULL == pst_logical_port) /* 如果条目不存在*/
    {
        uc_if_exist = ATG_DCI_UNEXIST;
    }

    /* 获取子配置操作 */
    ul_oper_basic = pst_set_logical_port->oper_basic;
    ul_oper_phy = pst_set_logical_port->oper_physical;
    ul_oper_logic_l2 = pst_set_logical_port->oper_logical_l2;
    ul_oper_logic_l3 = pst_set_logical_port->oper_logical_l3;
    ul_oper_l2vpn = pst_set_logical_port->oper_l2vpn;
    ul_oper_vlan = pst_set_logical_port->oper_vlan;
    ul_oper_ipv4 = pst_set_logical_port->oper_ipv4;
    ul_oper_ipv6 = pst_set_logical_port->oper_ipv6;
    ul_oper_mc_ipv4 = pst_set_logical_port->oper_mc_ipv4;
    ul_oper_mc_ipv6 = pst_set_logical_port->oper_mc_ipv6;    
    ul_oper_vip_vmac = pst_set_logical_port->oper_vip_vmac;
    ul_oper_vipv6_vmac = pst_set_logical_port->oper_vrrp_ipv6_mac_l3;     
    ul_oper_ve_mac = pst_set_logical_port->oper_ve_mac;     
    ul_oper_diff_serv = pst_set_logical_port->oper_diff_serv;
    ul_oper_flow_diff_serv = pst_set_logical_port->oper_flow_diff_serv;
    ul_oper_inclassify_qos = pst_set_logical_port->oper_inclassify_qos;
    ul_oper_up_user_qos = pst_set_logical_port->oper_up_user_qos;
    ul_oper_up_user_group_qos = pst_set_logical_port->oper_upuser_group_qos;
    ul_oper_down_user_queue_qos = pst_set_logical_port->oper_down_user_queue_qos;
    ul_oper_down_user_group_qos = pst_set_logical_port->oper_down_user_group_qos;
    ul_oper_flow_up_user_qos = pst_set_logical_port->oper_flow_upuser_qos;
    ul_oper_traffic_filter = pst_set_logical_port->oper_traffic_filter;
    ul_oper_ds_l2 = pst_set_logical_port->oper_ds_l2;
    ul_oper_car = pst_set_logical_port->oper_car;
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pst_set_logical_port->return_code = ATG_DCI_RC_OK;

    /* 删除整个条目 */
    if (TRUE == pst_set_logical_port->delete_struct)
    {
        /* 如果条目不存在，不删除 */
        if (ATG_DCI_UNEXIST == uc_if_exist)
        {        
            ret = LOG_PORT_PROC_DEL_NULL_LOCAL_CFG;
            spm_l2_record_err_code(ret);         
            goto EXIT_LABEL;
        }                
        else/* 存在删除 */
        {
            spm_dbg_print_logical_port_head(ul_port_key, SPM_OPER_DEL);        
            spm_dbg_record_logical_port_head(ul_port_key, SPM_OPER_DEL);

            //删除端口逻辑配置驱动配置
            spm_logical_port_del_cfg(pst_logical_port);

            if (NULL != pst_logical_port)
            {
                AVLL_DELETE(SHARED.logical_port_tree, pst_logical_port->spm_logical_port_node);

                //释放端口逻辑配置节点的内存空间
                spm_free_logical_port_cb(pst_logical_port NBB_CCXT);
            }
        }

        /*记录删除配置结束时间*/
        //gettimeofday(&logic_tv_end, NULL);
        /*记录开始处理配置时间*/
        //OS_SPRINTF(uc_message, "time_test logic_port_proc delete end = %ld\n", logic_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message); 
        //OS_SPRINTF(uc_message, "time_test logic_interval = %ld\n", logic_tv_end.tv_usec - logic_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
    }

    /* 增加或更新条目 */
    else
    {
        /* 如果条目不存在，树中要增加条目 */
        if (uc_if_exist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_logical_port_head(ul_port_key, SPM_OPER_ADD);
            spm_dbg_record_logical_port_head(ul_port_key, SPM_OPER_ADD);
            pst_logical_port = spm_alloc_logical_port_cb(NBB_CXT);

            if(NULL == pst_logical_port)
            {
                ret = LOG_PORT_PROC_ALLOC_CB_FAIL;
                spm_l2_record_err_code(ret);   
                goto EXIT_LABEL;
            }

            if (pst_logical_port != NULL)
            {
                pst_logical_port->port_index_key = ul_port_key;
                avll_ret_code = AVLL_INSERT(SHARED.logical_port_tree, pst_logical_port->spm_logical_port_node);
                NBB_ASSERT(avll_ret_code);
            }
        }
        else
        {
            spm_dbg_print_logical_port_head(ul_port_key, SPM_OPER_UPD);
            spm_dbg_record_logical_port_head(ul_port_key, SPM_OPER_UPD);
        }

        /************************************ 1) 基本配置 **************************************/
        if(ATG_DCI_OPER_NULL != ul_oper_basic)
        {
             spm_logical_port_basic_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_basic); 
        }

        /************************************2) 物理配置 ***************************************/
        if(ATG_DCI_OPER_NULL != ul_oper_phy)
        {
             spm_logical_port_phy_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_phy); 
        }

        /************************************************************************************/
        /* L2端口                                                                                                     */
        /************************************************************************************/
        if ((NULL != pst_logical_port) && (NULL != pst_logical_port->basic_cfg_cb) 
            && (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type))
        {
            /************************************4) 逻辑配置L2 **********************************/
            if(ATG_DCI_OPER_NULL != ul_oper_logic_l2)
            {
                 spm_logical_port_logic_l2_proc(pst_set_logical_port, pst_logical_port,ul_oper_logic_l2); 
            }

            /************************************5) L2VPN *************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_l2vpn)
            {
                 spm_logical_port_l2vpn_proc(pst_set_logical_port, pst_logical_port,ul_oper_l2vpn); 
            }

            /************************************21) VLAN属性***********************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vlan)
            {
                 spm_logical_port_vlan_proc(pst_set_logical_port, pst_logical_port,ul_oper_vlan);  
            }

            /************************************ 13）流相关Diff-Serv配置L2************************/
            if(ATG_DCI_OPER_NULL != ul_oper_flow_diff_serv)
            {
                spm_logical_port_flow_diff_serv_proc(pst_set_logical_port, pst_logical_port,ul_oper_flow_diff_serv);
            }
            
            /************************************ 19）流相关上话用户QOS策略配置L2 ***********************/
            if(ATG_DCI_OPER_NULL != ul_oper_flow_up_user_qos)
            {
                spm_logical_port_flow_up_user_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_flow_up_user_qos);
            } 
            
            /************************************ 14）上话复杂流分类QOS策略配置L2***************************/
            if(ATG_DCI_OPER_NULL != ul_oper_inclassify_qos)
            {
                spm_logical_port_inclassify_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_inclassify_qos);
            } 
            
            /************************************ 18）下话用户组QOS配置L2*************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_group_qos)
            {
                spm_logical_port_down_user_group_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_group_qos);
            }
            
            /************************************ 17）下话用户队列QOS策略配置L2******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_queue_qos)
            {
                spm_logical_port_down_user_queue_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_queue_qos);
            }
            
            /************************************ 20）包过滤器traffic_filterL2******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_traffic_filter)
            {
                spm_logical_port_traffic_filter_proc(pst_set_logical_port, pst_logical_port,ul_oper_traffic_filter);
            }  
            
            /************************************ 23) diff-Serv配置L2（VP下话UNI侧）******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ds_l2)
            {
                spm_logical_port_ds_l2_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_ds_l2);
            }
            
            /************************************ 24）car配置******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_car)
            {
                spm_logical_port_car_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_car);
            }       
        }

        /**************************************************************************************************************/
        /* L3端口                                                                                                     */
        /**************************************************************************************************************/
        else if((NULL != pst_logical_port) && (NULL != pst_logical_port->basic_cfg_cb) 
            && (ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type))
        {
        
            /************************************3) 逻辑配置L3 **********************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_logic_l3)
            {
                 spm_logical_port_logic_l3_proc(pst_set_logical_port, pst_logical_port, ul_oper_logic_l3);

            }
            
            /************************************7) IPv4地址配置L3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ipv4)
            {
                 spm_logical_port_ipv4_data_proc(pst_set_logical_port, pst_logical_port, ul_oper_ipv4);
            }

            /************************************8) IPv6地址配置L3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ipv6)
            {
                 spm_logical_port_ipv6_data_proc(pst_set_logical_port, pst_logical_port, ul_oper_ipv6);
            }
            
            /************************************9) 组播组地址L3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_mc_ipv4)
            {
                 spm_logical_port_mc_ipv4_proc(pst_set_logical_port, pst_logical_port, ul_oper_mc_ipv4);
            }
            
            /************************************22) IPv6组播组地址L3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_mc_ipv6)
            {
                 spm_logical_port_mc_ipv6_proc(pst_set_logical_port, pst_logical_port, ul_oper_mc_ipv6);
            } 
            
            /************************************10)VRRP虚拟MAC和IP地址配置L3 *******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vip_vmac)
            {
                 spm_logical_port_vip_vmac_proc(pst_set_logical_port, pst_logical_port, ul_oper_vip_vmac);
            } 
            
            /************************************25) VRRP虚拟MAC和IPv6地址配置L3 *******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vipv6_vmac)
            {
                 spm_logical_port_vipv6_vmac_proc(pst_set_logical_port, pst_logical_port, ul_oper_vipv6_vmac);
            } 
            
            /************************************11）VE MAC地址配置*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ve_mac)
            {
                 spm_logical_port_ve_mac_proc(pst_set_logical_port, pst_logical_port, ul_oper_ve_mac);
            } 
            
            /************************************21) VLAN属性*************************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vlan)
            {
                 spm_logical_port_vlan_proc(pst_set_logical_port, pst_logical_port,ul_oper_vlan);  
            }

            /************************************ 12）Diff-Serv配置****************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_flow_diff_serv)
            {
                spm_logical_port_diff_serv_proc(pst_set_logical_port, pst_logical_port,ul_oper_flow_diff_serv);
            }

            /************************************ 14）上话复杂流分类QOS策略配置L3***************************/
            if(ATG_DCI_OPER_NULL != ul_oper_inclassify_qos)
            {
                spm_logical_port_inclassify_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_inclassify_qos);
            }
            
            /************************************ 15）上话用户QOS策略配置 *********************************/
            if(ATG_DCI_OPER_NULL != ul_oper_up_user_qos)
            {
                spm_logical_port_up_user_qos_policy_proc(pst_set_logical_port, pst_logical_port,ul_oper_up_user_qos);
            }
        
            /************************************ 18）下话用户组QOS配置L3*************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_group_qos)
            {
                spm_logical_port_down_user_group_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_group_qos);
            }
            
            /************************************ 17）下话用户队列QOS策略配置L3******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_queue_qos)
            {
                spm_logical_port_down_user_queue_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_queue_qos);
            }
            
            /************************************ 20）包过滤器traffic_filterL3******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_traffic_filter)
            {
                spm_logical_port_traffic_filter_proc(pst_set_logical_port, pst_logical_port,ul_oper_traffic_filter);
            }        
        }
        else //既非L2也非L3
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_two_value(ret,ul_port_key,pst_logical_port->logic_cvt_id);
            goto EXIT_LABEL;
        }
        
        /************************************ 16）上话用户组QOS策略配置(无效)**************************/
        if(ATG_DCI_OPER_NULL != ul_oper_up_user_group_qos)
        {
            spm_logical_port_up_user_group_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_up_user_group_qos);
        }

        /*记录删除配置结束时间*/
        //gettimeofday(&logic_tv_end, NULL);
        /*记录开始处理配置时间*/
        //OS_SPRINTF(uc_message, "time_test logic_port_proc add end = %ld\n", logic_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        //OS_SPRINTF(uc_message, "time_test logic_interval = %ld\n", logic_tv_end.tv_usec - logic_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();
}


#endif

