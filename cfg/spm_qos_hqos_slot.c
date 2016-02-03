/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_hqos_slot.c
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
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/* 全局打印变量qos_acl_action_print */
extern NBB_BYTE qos_acl_action_print;

/* 全局打印变量qos_acl_action_print */
extern NBB_BYTE qos_log_cfg_print;


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_HQOS_SLOT_CB *spm_alloc_hqos_slot_cb(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_SLOT_CB *pstTbl = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    pstTbl = (SPM_HQOS_SLOT_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_SLOT_CB),
              NBB_NORETRY_ACT, MEM_SPM_HQOS_CB);
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstTbl, 0, sizeof(SPM_HQOS_SLOT_CB));
    pstTbl->index = 1;
    AVLL_INIT_NODE(pstTbl->spm_hqos_slot_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstTbl;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_hqos_slot_cb(SPM_HQOS_SLOT_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_HQOS_SLOT_CB), MEM_SPM_HQOS_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    if (NULL != pstTbl->data)
    {
        NBB_MM_FREE(pstTbl->data, MEM_SPM_HQOS_SLOT_CB);
        pstTbl->data = NULL;
    }

    NBB_MM_FREE(pstTbl, MEM_SPM_HQOS_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_hqos_slot_cfg(NBB_ULONG oper, ATG_DCI_HQOS_SLOT_DATA **pcb, 
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_HQOS_SLOT_DATA *ptemp = NULL;
    NBB_BYTE i = 0;
    NBB_BYTE hqos_slot = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            ptemp = (ATG_DCI_HQOS_SLOT_DATA *)buf;

            /***************************************************************************/
            /*                          存配置                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_HQOS_SLOT_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_HQOS_SLOT_DATA),
                        NBB_NORETRY_ACT, MEM_SPM_HQOS_SLOT_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_HQOS_SLOT_DATA));
            for (i = 0; i < ATG_DCI_HQOS_SLOT_NUM; i++)
            {
                if ((0 != ptemp->enable_switch[i].enable) && (hqos_slot < MAX_HQOS_SLOT_NUM))
                {
                    g_hqos_port_index[hqos_slot++] = i + 1;
                }
            }
        }
        break;

        default:
        break;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_hqos(ATG_DCI_SET_HQOS *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_HQOS_SLOT_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE *pucDataStart = NULL;
    NBB_ULONG ulOper = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;
    ulkey = pstSetIps->key;
    if (1 != ulkey)
    {
        pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pstCb = AVLL_FIND(SHARED.hqos_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    ulOper = pstSetIps->oper_hqos_slot;
    pucDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->hqos_slot_data);

    /* ips消息为删除 */
    if (TRUE == pstSetIps->delete_struct)
    {
        /* 如果条目不存在,不删除 */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        /* 存在,删除 */
        else
        {
            if(NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            /* 被引用 */
            if (0 != pstCb->cnt)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            AVLL_DELETE(SHARED.hqos_tree, pstCb->spm_hqos_slot_node);
            ret = spm_free_hqos_slot_cb(pstCb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

    }

    /* 增加或者更新 */
    else
    {

        /* 如果条目不存在,新申请内存空间保存数据 */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstCb = spm_alloc_hqos_slot_cb(NBB_CXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

        /* 被引用 */
        if ((NULL != pstCb) && (0 != pstCb->cnt))
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        
/*
        if(NULL == pstCb)
        {
            goto EXIT_LABEL;
        }
*/

        ret = spm_set_hqos_slot_cfg(ulOper, &(pstCb->data) , pucDataStart NBB_CCXT);
        pstSetIps->hqos_slot_return_code = ret;

        /* 如果是新增的条目,插入到树中 */
        if (QOS_UNEXIST == ucIfExist)
        {
            pstCb->index = ulkey;

            rv = AVLL_INSERT(SHARED.hqos_tree, pstCb->spm_hqos_slot_node);
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_qos_clear_all_hqos_slot(NBB_CXT_T NBB_CXT)
{
    SPM_HQOS_SLOT_CB *cfg_cb = NULL;


    for(cfg_cb = (SPM_HQOS_SLOT_CB*) AVLL_FIRST(v_spm_shared->hqos_tree); cfg_cb != NULL;
        cfg_cb = (SPM_HQOS_SLOT_CB*) AVLL_FIRST(v_spm_shared->hqos_tree))
    {
        AVLL_DELETE(v_spm_shared->hqos_tree, cfg_cb->spm_hqos_slot_node);
        spm_free_hqos_slot_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}

