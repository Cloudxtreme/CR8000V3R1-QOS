/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_group.c
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

/* 全局打印变量qos_log_cfg_print */
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
NBB_LONG spm_disconnect_usr_group_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_USER_GROUP_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_usr_group_cnt err : ret = %ld, "
                  "usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_usr_group_cnt err : ret = %ld, "
                   "usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_user_group_tree, &index);
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_usr_group_cnt err : can't find usr group cfg. "
                  "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_usr_group_cnt err : can't find usr group cfg. "
                   "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    if (0 == pcb->cnt)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_usr_group_cnt err : usr group cnt is 0. "
                  "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_usr_group_cnt err : usr group cnt is 0. "
                   "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        goto EXIT_LABEL;
    }
    (pcb->cnt)--;

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
NBB_LONG spm_set_up_user_group_cfg(NBB_ULONG oper, ATG_DCI_USER_GROUP_UP_QOS **pcb, 
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_USER_GROUP_UP_QOS *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* ips消息内容为空 */
        if (NULL == buf)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }

        /* 不为空 */
        else
        {
            ptemp = (ATG_DCI_USER_GROUP_UP_QOS *)buf;

            /***************************************************************************/
            /*                          存配置                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_USER_GROUP_UP_QOS *)NBB_MM_ALLOC(sizeof(ATG_DCI_USER_GROUP_UP_QOS),
                        NBB_NORETRY_ACT, MEM_SPM_UP_USER_GROUP_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_USER_GROUP_UP_QOS));
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
NBB_VOID spm_rcv_dci_set_user_group(ATG_DCI_SET_USER_GROUP *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_USER_GROUP_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;

    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucUpDataStart = NULL;
    NBB_BYTE *pucDownDataStart = NULL;

    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperDown = ATG_DCI_OPER_NULL;

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
    pstCb = AVLL_FIND(SHARED.qos_user_group_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    ulOperBasic = pstSetIps->oper_basic;
    ulOperUp = pstSetIps->oper_up_qos;
    ulOperDown = pstSetIps->oper_down_qos;
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucUpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->up_qos_data);
    pucDownDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->down_qos_data);

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
            AVLL_DELETE(SHARED.qos_user_group_tree, pstCb->spm_qos_user_group_node);
            spm_free_user_group_cb(pstCb NBB_CCXT);
        }
    }

    /* 更新或者增加 */
    else
    {
        /* 如果条目不存在,新申请内存空间保存数据 */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstCb = spm_alloc_user_group_cb(ulkey NBB_CCXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

        /* 被引用 */
        if ((NULL != pstCb) && (0 != pstCb->cnt))
        {
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }

/*
        if(NULL == pstCb)
        {
            goto EXIT_LABEL;
        }
*/

        /***************************************************************************/
        /*                                 上话用户组配置                          */
        /***************************************************************************/
        ret = spm_set_up_user_group_cfg(ulOperUp, &(pstCb->up_cfg_cb), pucUpDataStart NBB_CCXT);
        pstSetIps->up_qos_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                              上话用户组配置                             */
        /***************************************************************************/
        ret = spm_set_down_user_group_cfg(ulOperDown, pstCb, pucDownDataStart NBB_CCXT);
        pstSetIps->down_qos_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        pstSetIps->return_code = ips_ret;

        /* 如果是新增的条目,插入到树中 */
        if (QOS_UNEXIST == ucIfExist)
        {
            pstCb->user_group_key = ulkey;
            
            //coverity[no_effect_test]
            rv = AVLL_INSERT(SHARED.qos_user_group_tree, pstCb->spm_qos_user_group_node);
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
NBB_VOID spm_qos_clear_all_usr_group(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_USER_GROUP_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_USER_GROUP_CB*) AVLL_FIRST(v_spm_shared->qos_user_group_tree);cfg_cb != NULL;
        cfg_cb = (SPM_QOS_USER_GROUP_CB*) AVLL_FIRST(v_spm_shared->qos_user_group_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_user_group_tree, cfg_cb->spm_qos_user_group_node);
        spm_free_user_group_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}
