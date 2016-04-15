/*****************************************************************************

                  Copyright(C)1999-2013 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_oam_dbg.c
  版 本 号   : V1.1
  负 责 人   : huxi
  生成日期   : 2013年3月8日
  最近修改   :
  文件描述   : 调试函数
  函数列表   :
  修改历史   :
  1.日    期   : 2013年3月8日
    作    者   : huxi
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include <math.h>
#include <bmuOsHdrs.h>

/*****************************************************************************
 函 数 名  : spm_rsvp_key_cmp
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_rsvp_key_cmp(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    OAM_RSVP_LSP_KEY *oam_key1 = NULL;
    OAM_RSVP_LSP_KEY *oam_key2 = NULL;

    /* 入参判断 */
    NBB_ASSERT(NULL != key1);
    NBB_ASSERT(NULL != key2);

    oam_key1 = (OAM_RSVP_LSP_KEY *)key1;
    oam_key2 = (OAM_RSVP_LSP_KEY *)key2;

    /***********************************************************************/
    /* 比较key值大小                   */
    /***********************************************************************/
    rc = compare_ulong(&oam_key1->Egress, &oam_key2->Egress);

    if (0 == rc)
    {
        rc = compare_ulong(&oam_key1->Ingress, &oam_key2->Ingress);

        if (0 == rc)
        {
            rc = compare_ulong(&oam_key1->LspId, &oam_key2->LspId);

            if (0 == rc)
            {
                rc = compare_ulong(&oam_key1->TunnelId, &oam_key2->TunnelId);
            }
        }
    }

    return(rc);
}

/*****************************************************************************
 函 数 名  : spm_pw_key_cmp
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_pw_key_cmp(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    SPM_PW_KEY *oam_key1 = NULL;
    SPM_PW_KEY *oam_key2 = NULL;

    /* 入参判断 */
    NBB_ASSERT(NULL != key1);
    NBB_ASSERT(NULL != key2);

    oam_key1 = (SPM_PW_KEY *)key1;
    oam_key2 = (SPM_PW_KEY *)key2;

    /***********************************************************************/
    /* 比较key值大小                   */
    /***********************************************************************/
    rc = compare_ulong(&oam_key1->vc_id, &oam_key2->vc_id);

    if (0 == rc)
    {
        rc = compare_ushort(&oam_key1->vc_type, &oam_key2->vc_type);

        if (0 == rc)
        {
            rc = compare_ulong(&oam_key1->peer_ip, &oam_key2->peer_ip);
        }
    }

    return(rc);
}

/*****************************************************************************
 函 数 名  : spm_init_wrapping_cb_tree
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_wrapping_cb_tree()
{
    AVLL_INIT_TREE(v_spm_shared->g_oamavltreebywrapping, compare_ulong,
                   NBB_OFFSETOF(SPM_OAM_WRAPPING_CB, oamid),
                   NBB_OFFSETOF(SPM_OAM_WRAPPING_CB, spm_wrapping_node));
    return;
}


/*****************************************************************************
 函 数 名  : spm_init_vs_cb_tree
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vs_cb_tree()
{
    AVLL_INIT_TREE(v_spm_shared->g_oamavltreebyindex, compare_ulong,
                   NBB_OFFSETOF(SPM_OAM_VS_CB, vs_index),
                   NBB_OFFSETOF(SPM_OAM_VS_CB, spm_vs_node));
    return;
}

/*****************************************************************************
 函 数 名  : spm_init_rsvp_tx_cb_tree
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_rsvp_tx_cb_tree()
{
    AVLL_INIT_TREE(v_spm_shared->g_oamavltreebyrsvptx,
                   spm_rsvp_key_cmp,
                   NBB_OFFSETOF(SPM_OAM_RSVP_CB_TX, stRsvplspCfg),
                   NBB_OFFSETOF(SPM_OAM_RSVP_CB_TX, spm_oam_rsvp_node));
    return;
}

/*****************************************************************************
 函 数 名  : spm_init_rsvp_rx_cb_tree
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_rsvp_rx_cb_tree()
{
    AVLL_INIT_TREE(v_spm_shared->g_oamavltreebyrsvprx,
                   spm_rsvp_key_cmp,
                   NBB_OFFSETOF(SPM_OAM_RSVP_CB_RX, stRsvplspCfg),
                   NBB_OFFSETOF(SPM_OAM_RSVP_CB_RX, spm_oam_rsvp_node));
    return;
}

/*****************************************************************************
 函 数 名  : spm_init_pw_cb_tree
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_pw_cb_tree()
{
    AVLL_INIT_TREE(v_spm_shared->g_oamavltreebypw,
                   spm_pw_key_cmp,
                   NBB_OFFSETOF(SPM_OAM_PW_CB, stpwcfg),
                   NBB_OFFSETOF(SPM_OAM_PW_CB, spm_oam_pw_node));
    return;
}

/*****************************************************************************
 函 数 名  : spm_init_oam_cb_tree
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_oam_cb_tree()
{
    AVLL_INIT_TREE(v_spm_shared->g_oamavltreebyoamid,
                   compare_ulong,
                   NBB_OFFSETOF(SPM_OAM_CB, oam_id),
                   NBB_OFFSETOF(SPM_OAM_CB, spm_oam_node));
    return;
}

/*****************************************************************************
 函 数 名  : spm_init_channeltype_fpga
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_channeltype_fpga(NBB_USHORT uschanneltype)
{
    SD_LIST_NODE *pstroll = NULL;
    NBB_INT iret = ERROR;
    
    /* ACH寄存器 */
//    spm_get_global_channel_type(&g_channel_type);
    g_channel_type = uschanneltype;

    if ((uschanneltype != CHANNEL_TYPE_7FFA) && (uschanneltype != CHANNEL_TYPE_8902))
    {
        uschanneltype = CHANNEL_TYPE_7FFA;
    }

    spm_oam_16write(OAM_REG_BASE1_ADDRESS, OAM_GLOBAL_ACH_OFFSET, uschanneltype);
    spm_oam_16write(OAM_REG_BASE2_ADDRESS, OAM_GLOBAL_ACH_OFFSET, uschanneltype);
    pstroll = g_oam_wrappinglist;

    while (pstroll != NULL)
    {
        iret = spm_oam_wrapping_build_acl(pstroll->oamLoop);

        if (SUCCESS != iret)
        {
            printf("refresh acl for wrapping failed! oamloop=%d\n", pstroll->oamLoop);
        }

        pstroll = pstroll->next;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_init_sd_cfg
 功能描述  : sd cfg初始化
 输入参数  : NBB_ULONG threshold  
             NBB_ULONG windows    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_sd_cfg(NBB_ULONG threshold, NBB_ULONG windows)
{
    windows = 5;//因为网管windows暂时下不下来，暂定为5s
    g_sd_threshold = threshold;
    g_sd_windows = windows;
    g_sd_windowticks = g_sd_windows * 200;
    return;
}

/*****************************************************************************
 函 数 名  : redirecte_mpls_data_to_fpga_for_lsp
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG intfposid      
             NBB_BYTE fpgaport        
             NBB_ULONG ruleidcount    
             NBB_ULONG customdata[6]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT redirecte_mpls_data_to_fpga_for_lsp(NBB_ULONG intfposid, NBB_BYTE fpgaport,
        NBB_ULONG ruleidcount, NBB_ULONG customdata[6])
{
    ACL_T pstacl = {0};
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    OS_SPRINTF(g_uc_message, "Enter Function --- %s,%d : intfposid=%d,  fpgaport=%d.\n",
               __FUNCTION__, __LINE__, intfposid, fpgaport);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    if (0 == intfposid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : intfposid is 0.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&pstacl, 0, sizeof(ACL_T));
    pstacl.mAclId  = 4411;
    pstacl.mRuleId = ruleidcount;
    pstacl.tAclRule.tAclKey.ethType  = 0x8847;
    pstacl.tAclRule.tAclMask.ethType = 0xffff;
    pstacl.tAclRule.tAclKey.customerData6  = customdata[0];
    pstacl.tAclRule.tAclKey.customerData5  = customdata[1];
    pstacl.tAclRule.tAclKey.customerData4  = customdata[2];
    pstacl.tAclRule.tAclKey.customerData3  = customdata[3];
    pstacl.tAclRule.tAclKey.customerData2  = customdata[4];
    pstacl.tAclRule.tAclKey.customerData1  = customdata[5];
    pstacl.tAclRule.tAclMask.customerData6 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData5 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData4 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData3 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData2 = 0x00000000;
    pstacl.tAclRule.tAclMask.customerData1 = 0x000000ff;
    pstacl.eAclAction = ACL_ACTION_REDIRECTION;
    pstacl.slot = v_spm_shared->local_slot_id;
    pstacl.cardPort = fpgaport;

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetAcl(unit, &pstacl);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetIntfIngressAcl(unit, intfposid, 1, pstacl.mAclId);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetIntfIngressAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ret;
}

/*****************************************************************************
 函 数 名  : redirecte_mpls_data_to_fpga_for_lsp2
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG intfposid      
             NBB_BYTE fpgaport        
             NBB_ULONG ruleidcount    
             NBB_ULONG customdata[6]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT redirecte_mpls_data_to_fpga_for_lsp2(NBB_ULONG intfposid, NBB_BYTE fpgaport,
        NBB_ULONG ruleidcount, NBB_ULONG customdata[6])
{
    ACL_T pstacl = {0};
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    OS_SPRINTF(g_uc_message, "Enter Function --- %s,%d : intfposid=%d,  fpgaport=%d.\n",
               __FUNCTION__, __LINE__, intfposid, fpgaport);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    if (0 == intfposid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : intfposid is 0.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&pstacl, 0, sizeof(ACL_T));
    pstacl.mAclId  = 4414;
    pstacl.mRuleId = ruleidcount;
    pstacl.tAclRule.tAclKey.ethType  = 0x8847;
    pstacl.tAclRule.tAclMask.ethType = 0xffff;
    pstacl.tAclRule.tAclKey.customerData6  = customdata[0];
    pstacl.tAclRule.tAclKey.customerData5  = customdata[1];
    pstacl.tAclRule.tAclKey.customerData4  = customdata[2];
    pstacl.tAclRule.tAclKey.customerData3  = customdata[3];
    pstacl.tAclRule.tAclKey.customerData2  = customdata[4];
    pstacl.tAclRule.tAclKey.customerData1  = customdata[5];
    pstacl.tAclRule.tAclMask.customerData6 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData5 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData4 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData3 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData2 = 0x00000000;
    pstacl.tAclRule.tAclMask.customerData1 = 0x000000ff;
    pstacl.eAclAction = ACL_ACTION_REDIRECTION;
    pstacl.slot = v_spm_shared->local_slot_id;
    pstacl.cardPort = fpgaport;

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetAcl(unit, &pstacl);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetIntfIngressAcl(unit, intfposid, 1, pstacl.mAclId);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetIntfIngressAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ret;
}

/*****************************************************************************
 函 数 名  : redirecte_mpls_data_to_fpga_for_wrapping
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG intfposid      
             NBB_BYTE fpgaport        
             NBB_ULONG ruleidcount    
             NBB_ULONG customdata[6]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT redirecte_mpls_data_to_fpga_for_wrapping(NBB_ULONG intfposid, NBB_BYTE fpgaport,
        NBB_ULONG ruleidcount, NBB_ULONG customdata[3])
{
    ACL_T pstacl = {0};
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    OS_SPRINTF(g_uc_message, "Enter Function --- %s,%d : intfposid=%d,  fpgaport=%d.\n",
               __FUNCTION__, __LINE__, intfposid, fpgaport);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    if (0 == intfposid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : intfposid is 0.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&pstacl, 0, sizeof(ACL_T));
    pstacl.mAclId  = 4412;
    pstacl.mRuleId = ruleidcount;
    pstacl.tAclRule.tAclKey.ethType  = 0x8847;
    pstacl.tAclRule.tAclMask.ethType = 0xffff;
    pstacl.tAclRule.tAclKey.customerData6  = customdata[0];
    pstacl.tAclRule.tAclKey.customerData5  = customdata[1];
    pstacl.tAclRule.tAclKey.customerData4  = customdata[2];
    pstacl.tAclRule.tAclMask.customerData6 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData5 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData4 = 0xffffffff;
    pstacl.eAclAction = ACL_ACTION_REDIRECTION;
    pstacl.slot = v_spm_shared->local_slot_id;
    pstacl.cardPort = fpgaport;

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetAcl(unit, &pstacl);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetIntfIngressAcl(unit, intfposid, 1, pstacl.mAclId);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetIntfIngressAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ret;
}


/*****************************************************************************
 函 数 名  : redirecte_mpls_data_to_fpga_for_pw
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG intfposid      
             NBB_BYTE fpgaport        
             NBB_ULONG ruleidcount    
             NBB_ULONG customdata[6]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT redirecte_mpls_data_to_fpga_for_pw(NBB_ULONG intfposid, NBB_BYTE fpgaport,
        NBB_ULONG ruleidcount, NBB_ULONG customdata[5])
{
    ACL_T pstacl = {0};
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    OS_SPRINTF(g_uc_message, "Enter Function --- %s,%d : intfposid=%d,  fpgaport=%d.\n",
               __FUNCTION__, __LINE__, intfposid, fpgaport);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    if (0 == intfposid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : intfposid is 0.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&pstacl, 0, sizeof(ACL_T));
    pstacl.mAclId  = 4413;
    pstacl.mRuleId = ruleidcount;
    pstacl.tAclRule.tAclKey.ethType  = 0x8847;
    pstacl.tAclRule.tAclMask.ethType = 0xffff;
    pstacl.tAclRule.tAclKey.customerData5  = customdata[0];
    pstacl.tAclRule.tAclKey.customerData4  = customdata[1];
    pstacl.tAclRule.tAclKey.customerData3  = customdata[2];
    pstacl.tAclRule.tAclKey.customerData2  = customdata[3];
    pstacl.tAclRule.tAclKey.customerData1  = customdata[4];
    pstacl.tAclRule.tAclMask.customerData5 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData4 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData3 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData2 = 0x00000000;
    pstacl.tAclRule.tAclMask.customerData1 = 0x000000ff;
    pstacl.eAclAction = ACL_ACTION_REDIRECTION;
    pstacl.slot = v_spm_shared->local_slot_id;
    pstacl.cardPort = fpgaport;

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetAcl(unit, &pstacl);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetIntfIngressAcl(unit, intfposid, 1, pstacl.mAclId);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetIntfIngressAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ret;
}


/*****************************************************************************
 函 数 名  : redirecte_mpls_data_to_fpga_for_pw2
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG intfposid      
             NBB_BYTE fpgaport        
             NBB_ULONG ruleidcount    
             NBB_ULONG customdata[6]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT redirecte_mpls_data_to_fpga_for_pw2(NBB_ULONG intfposid, NBB_BYTE fpgaport,
        NBB_ULONG ruleidcount, NBB_ULONG customdata[5])
{
    ACL_T pstacl = {0};
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    OS_SPRINTF(g_uc_message, "Enter Function --- %s,%d : intfposid=%d,  fpgaport=%d.\n",
               __FUNCTION__, __LINE__, intfposid, fpgaport);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    if (0 == intfposid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : intfposid is 0.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&pstacl, 0, sizeof(ACL_T));
    pstacl.mAclId  = 4415;
    pstacl.mRuleId = ruleidcount;
    pstacl.tAclRule.tAclKey.ethType  = 0x8847;
    pstacl.tAclRule.tAclMask.ethType = 0xffff;
    pstacl.tAclRule.tAclKey.customerData5  = customdata[0];
    pstacl.tAclRule.tAclKey.customerData4  = customdata[1];
    pstacl.tAclRule.tAclKey.customerData3  = customdata[2];
    pstacl.tAclRule.tAclKey.customerData2  = customdata[3];
    pstacl.tAclRule.tAclKey.customerData1  = customdata[4];
    pstacl.tAclRule.tAclMask.customerData5 = 0xfffff000;
    pstacl.tAclRule.tAclMask.customerData4 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData3 = 0xffffffff;
    pstacl.tAclRule.tAclMask.customerData2 = 0x00000000;
    pstacl.tAclRule.tAclMask.customerData1 = 0x000000ff;
    pstacl.eAclAction = ACL_ACTION_REDIRECTION;
    pstacl.slot = v_spm_shared->local_slot_id;
    pstacl.cardPort = fpgaport;

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetAcl(unit, &pstacl);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
#ifdef SPU
        ret = ApiC3SetIntfIngressAcl(unit, intfposid, 1, pstacl.mAclId);
#endif

        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(g_uc_message, "**FATAL ERROR**%s,%d : ret =%d ApiC3SetIntfIngressAcl.\n"
                       "acl_id=%d,fpgaport=%d,intfposid=%d",
                       __FUNCTION__, __LINE__, ret, pstacl.mAclId, pstacl.cardPort, intfposid);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ret;
}

/*****************************************************************************
 函 数 名  : spm_odindex_by_activeindex
 功能描述  : 根据主动的本地索引得出按需在全局数组的索引
 输入参数  : NBB_USHORT usloop
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年1月24日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_odindex_by_activeindex(IN NBB_USHORT usactindex, OUT NBB_BYTE *ucodindex)
{
    NBB_BYTE odiloop = 0;
    NBB_INT iret = ERROR;

    if (NULL == ucodindex)
    {
        goto EXIT_LABEL;
    }

    if (USELESS == g_stoam_local[usactindex].ulActiveOamkey)
    {
        goto EXIT_LABEL;
    }

    for (odiloop = 0 ; odiloop < OAM_MAX_NUM_OD; odiloop++)
    {
        if (g_stoam_local[usactindex].ulActiveOamkey == g_stoam_odlocal[odiloop].ulOamActiveKey)
        {
            *ucodindex = odiloop;
            iret = SUCCESS;
            goto EXIT_LABEL;
        }
        else
        {
            continue;
        }
    }

    if (OAM_MAX_NUM_OD == odiloop)
    {
        goto EXIT_LABEL;
    }

EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_to_bmu_return_code_ok
 功能描述  : 返回ok给bmu
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_to_bmu_return_code_ok(ATG_DCI_SET_TPOAM_ACTIVE *pstactiveoam_set)
{
    if (NULL == pstactiveoam_set)
    {
        return;
    }

    pstactiveoam_set->return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->basic_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->pw_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->wrapping_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->vs_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->aps_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->cv_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->fdi_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->csf_return_code = ATG_DCI_RC_OK;
    pstactiveoam_set->rsvp_lsp_return_code = ATG_DCI_RC_OK;
    return;
}

/*****************************************************************************
 函 数 名  : spm_to_bmu_return_code_unsuccessful
 功能描述  : 返回UNSUCCESSFUL给bmu
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID  spm_to_bmu_return_code_unsuccessful(ATG_DCI_SET_TPOAM_ACTIVE *pstactiveoam_set)
{
    if (NULL == pstactiveoam_set)
    {
        return;
    }

#if 0
    pstactiveoam_set->return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->wrapping_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->vs_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->aps_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->cv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->fdi_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->csf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstactiveoam_set->adjust_return_code = ATG_DCI_RC_UNSUCCESSFUL;
#endif
    return;
}

/*****************************************************************************
 函 数 名  : spm_to_bmu_return_code_od_ok
 功能描述  : 按需配置返回给bmu的return code
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_to_bmu_return_code_od_ok(IN ATG_DCI_SET_TPOAM_ON_DEMAND *pstodoam_set)
{
    if (NULL == pstodoam_set)
    {
        return;
    }

    pstodoam_set->return_code = ATG_DCI_RC_OK;
    pstodoam_set->basic_return_code = ATG_DCI_RC_OK;
    pstodoam_set->loop_frame_return_code = ATG_DCI_RC_OK;
    pstodoam_set->tst_frame_return_code = ATG_DCI_RC_OK;
    pstodoam_set->lck_frame_return_code = ATG_DCI_RC_OK;
    pstodoam_set->lm_frame_return_code = ATG_DCI_RC_OK;
    pstodoam_set->dm_frame_return_code = ATG_DCI_RC_OK;
    pstodoam_set->mip_return_code = ATG_DCI_RC_OK;
    return;
}

/*****************************************************************************
 函 数 名  : spm_to_bmu_return_code_od_unsuccessful
 功能描述  : 按需配置返回给bmu的return code
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_to_bmu_return_code_od_unsuccessful(IN ATG_DCI_SET_TPOAM_ON_DEMAND *pstodoam_set)
{
    if (NULL == pstodoam_set)
    {
        return;
    }

    pstodoam_set->return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->loop_frame_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->tst_frame_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->lck_frame_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->lm_frame_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->dm_frame_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    pstodoam_set->mip_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    return;
}

/*****************************************************************************
 函 数 名  : spm_get_aps_enable_by_oamid
 功能描述  : 通过oamid值获取aps是否使能
 输入参数  : NBB_ULONG uloamid
 输出参数  : aps的使能值，0为不只能，1为使能
 返 回 值  : error/success
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月12日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_aps_enable_by_oamid(NBB_ULONG uloamid, NBB_BYTE *aps_enable)
{
    NBB_INT iret = ERROR;
    SPM_OAM_CB *pstoamcfg = NULL;

    if (NULL == aps_enable)
    {
        goto EXIT_LABEL;
    }

    /* 判断入参 */
    if (uloamid > OAM_ID_MAX)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "usOamID is illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    pstoamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &uloamid);

    if (NULL == pstoamcfg)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "OamID can not be found in g_oamavltreebyoamid\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    *aps_enable = pstoamcfg->aps_enable;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_csf_alarm_by_oamid
 功能描述  : 通过oamid值获取csf是否有告警
 输入参数  : NBB_ULONG uloamid
 输出参数  : csf的告警值，0为无告警，1为有告警
 返 回 值  : error/success
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月18日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_csf_alarm_by_oamid(NBB_ULONG uloamid, NBB_BYTE *csf_alarm)
{
    NBB_INT iret = ERROR;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_USHORT usloop = 0;
    NBB_BYTE module = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT alarm_temp = 0;

    if (NULL == csf_alarm)
    {
        goto EXIT_LABEL;
    }

    /* 判断入参 */
    if ((uloamid == 0) || (uloamid > OAM_ID_MAX))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "usOamID is illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    pstoamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &uloamid);

    if (NULL == pstoamcfg)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "OamID can not be found in g_oamavltreebyoamid\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    usloop = g_stoam_local[pstoamcfg->oamindex].module_index;
    module = g_stoam_local[pstoamcfg->oamindex].bModuleSwitch;
    spm_get_moudle_base_addr(pstoamcfg->oamindex, &base_addr);
    alarm_temp = spm_oam_16read(base_addr, ALARM_ADDR + usloop);
    *csf_alarm = ((alarm_temp >> 11) & 0x1);
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_delete_avll_tree_by_txrsvp
 功能描述  : 删除avlltree
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_delete_avll_tree_by_txrsvp(SPM_OAM_CB *pstoamcfg)
{
    OAM_RSVP_LSP_KEY strsvpkey_tx;
    SPM_OAM_RSVP_CB_TX *prsvpcfgtx = NULL;
    OS_MEMSET(&strsvpkey_tx, 0, sizeof(OAM_RSVP_LSP_KEY));

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pRsvplspCfg))
    {
        return;
    }

    strsvpkey_tx.Ingress = pstoamcfg->pRsvplspCfg->tx_ingress;
    strsvpkey_tx.Egress = pstoamcfg->pRsvplspCfg->tx_egress;
    strsvpkey_tx.LspId = pstoamcfg->pRsvplspCfg->tx_lsp_id;
    strsvpkey_tx.TunnelId = pstoamcfg->pRsvplspCfg->tx_tunnel_id;
    prsvpcfgtx = AVLL_FIND(v_spm_shared->g_oamavltreebyrsvptx, &strsvpkey_tx);

    if (NULL != prsvpcfgtx)
    {
        AVLL_DELETE(v_spm_shared->g_oamavltreebyrsvptx, prsvpcfgtx->spm_oam_rsvp_node);
        NBB_MM_FREE(prsvpcfgtx, MEM_SPM_ACTIVE_OAM_RSVP_CB);
        prsvpcfgtx = NULL;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_delete_avll_tree_by_rxrsvp
 功能描述  : 删除avlltree
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_delete_avll_tree_by_rxrsvp(SPM_OAM_CB *pstoamcfg)
{
    OAM_RSVP_LSP_KEY strsvpkeyrx;
    SPM_OAM_RSVP_CB_RX *prsvpcfgrx = NULL;
    OS_MEMSET(&strsvpkeyrx, 0, sizeof(OAM_RSVP_LSP_KEY));

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pRsvplspCfg))
    {
        return;
    }

    strsvpkeyrx.Ingress = pstoamcfg->pRsvplspCfg->rx_ingress;
    strsvpkeyrx.Egress = pstoamcfg->pRsvplspCfg->rx_egress;
    strsvpkeyrx.LspId = pstoamcfg->pRsvplspCfg->rx_lsp_id;
    strsvpkeyrx.TunnelId = pstoamcfg->pRsvplspCfg->rx_tunnel_id;
    prsvpcfgrx = AVLL_FIND(v_spm_shared->g_oamavltreebyrsvprx, &strsvpkeyrx);

    if (NULL != prsvpcfgrx)
    {
        AVLL_DELETE(v_spm_shared->g_oamavltreebyrsvprx, prsvpcfgrx->spm_oam_rsvp_node);
        NBB_MM_FREE(prsvpcfgrx, MEM_SPM_ACTIVE_OAM_RSVP_CB);
        prsvpcfgrx = NULL;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_delete_avll_tree_by_pw
 功能描述  : 删除avlltree
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_delete_avll_tree_by_pw(SPM_OAM_CB *pstoamcfg)
{
    SPM_OAM_PW_CB *pstpwcfg = NULL;
    SPM_PW_KEY stpwkey;
    OS_MEMSET(&stpwkey, 0, sizeof(SPM_PW_KEY));

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pPwCfg))
    {
        return;
    }

    stpwkey.peer_ip = pstoamcfg->pPwCfg->peer_ip;
    stpwkey.vc_id = pstoamcfg->pPwCfg->vc_id;
    stpwkey.vc_type = pstoamcfg->pPwCfg->vc_type;
    pstpwcfg = AVLL_FIND(v_spm_shared->g_oamavltreebypw, &stpwkey);

    if (NULL != pstpwcfg)
    {
        AVLL_DELETE(v_spm_shared->g_oamavltreebypw, pstpwcfg->spm_oam_pw_node);
        NBB_MM_FREE(pstpwcfg, MEM_SPM_ACTIVE_OAM_PW_CB);
        pstpwcfg = NULL;
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_delete_avll_tree_by_index
 功能描述  : 删除avlltree
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_delete_avll_tree_by_index(SPM_OAM_CB *pstoamcfg)
{
    SPM_OAM_VS_CB *pstvscfg = NULL;

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pVsCfg))
    {
        return;
    }

    pstvscfg = AVLL_FIND(v_spm_shared->g_oamavltreebyindex, &(pstoamcfg->pVsCfg->index));

    if (NULL != pstvscfg)
    {
        AVLL_DELETE(v_spm_shared->g_oamavltreebyindex, pstvscfg->spm_vs_node);
        NBB_MM_FREE(pstvscfg, MEM_SPM_ACTIVE_OAM_VS_CB);
        pstvscfg = NULL;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_delete_avll_tree_by_wrapping
 功能描述  : 删除avlltree
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_delete_avll_tree_by_wrapping(SPM_OAM_CB *pstoamcfg)
{
    SPM_OAM_WRAPPING_CB *pstwrapping = NULL;

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pWrapping))
    {
        return;
    }

    pstwrapping = AVLL_FIND(v_spm_shared->g_oamavltreebywrapping,
                           &(pstoamcfg->oam_id));

    if (NULL != pstwrapping)
    {
        AVLL_DELETE(v_spm_shared->g_oamavltreebywrapping,
                    pstwrapping->spm_wrapping_node);
        NBB_MM_FREE(pstwrapping, MEM_SPM_ACTIVE_OAM_WRAPPING_CB);
        pstwrapping = NULL;
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_get_process_slot_by_vsindex
 功能描述  : vs层获取处理盘槽位
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_process_slot_by_vsindex(NBB_ULONG index, NBB_BYTE uclocalslot,
                                        SPM_PORT_INFO_CB *portinfo)
{
    NBB_INT iret = ERROR;

    if (NULL == portinfo)
    {
        goto EXIT_LABEL;
    }

    /* 迭代出portid作为pwlable */
    iret = spm_get_portid_from_physical_port_index(index, portinfo);

    /* 判断迭代结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nGet portid failed,logical port index = %d ret=%d\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 若处理盘为本槽位，才存配置 */
    if (uclocalslot != portinfo->slot_id)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   " local slot=%d process slot=%d\n"
                   "the slot to deal with is not match with local slot\n"
                   "logical_index=%ld\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uclocalslot,
                   portinfo->slot_id, index);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = ERROR;
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_label_by_txlsp
 功能描述  : 获取出标签
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_label_by_txlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                               NBB_ULONG *ullabel_tx)
{
    CRTXLSP_KEY lsp_key_tx;
    NBB_INT iret = ERROR;
    OS_MEMSET(&lsp_key_tx, 0, sizeof(CRTXLSP_KEY));

    if ((NULL ==  pstrsvplsp) || (NULL ==  ullabel_tx))
    {
        goto EXIT_LABEL;
    }

    /* 根据txlsp的key值获取lsp出标签 */
    lsp_key_tx.egress = pstrsvplsp->tx_egress;
    lsp_key_tx.ingress = pstrsvplsp->tx_ingress;
    lsp_key_tx.lspid = pstrsvplsp->tx_lsp_id;
    lsp_key_tx.tunnelid = pstrsvplsp->tx_tunnel_id;
    iret = spm_l3_txlabelforoam(&lsp_key_tx, ullabel_tx);

    if (1 == iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "egress=%#x, ingress=%#x, lsp id=%#x, tunnel id=%#x, ret=%d\n"
                   "Cannot find the tx label\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                   lsp_key_tx.egress, lsp_key_tx.ingress,
                   lsp_key_tx.lspid, lsp_key_tx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    else if (2 == iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "egress=%#x, ingress=%#x, lsp id=%#x, tunnel id=%#x, ret=%d\n"
                   "Cannot find the tunnelid\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                   lsp_key_tx.egress, lsp_key_tx.ingress,
                   lsp_key_tx.lspid, lsp_key_tx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_out_slot_by_txlsp
 功能描述  : 获取出口槽位
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_out_slot_by_txlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                                  NBB_BYTE uclocalslot, NBB_BYTE *ucslot, NBB_USHORT *portid)
{
    CRTXLSP_KEY lsp_key_tx;
    NBB_INT iret = ERROR;
    NBB_BYTE ucporttype = 0;
    NBB_ULONG ullsp = 0;
    OS_MEMSET(&lsp_key_tx, 0, sizeof(CRTXLSP_KEY));

    if ((NULL == pstrsvplsp) || (NULL == ucslot) || (NULL == portid))
    {
        goto EXIT_LABEL;
    }

    lsp_key_tx.egress = pstrsvplsp->tx_egress;
    lsp_key_tx.ingress = pstrsvplsp->tx_ingress;
    lsp_key_tx.lspid = pstrsvplsp->tx_lsp_id;
    lsp_key_tx.tunnelid = pstrsvplsp->tx_tunnel_id;

    //迭代出口槽位
    iret = spm_get_portid_from_lsptx(lsp_key_tx, portid, ucslot, &ucporttype, &ullsp);

    if (ERROR == iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "Get out slot id through txlsp failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                   lsp_key_tx.egress, lsp_key_tx.ingress,
                   lsp_key_tx.lspid, lsp_key_tx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (uclocalslot != *ucslot)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "the slot to deal with do not match with local slot\n"
                   "local slot=%d, process slot=%d\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uclocalslot,
                   *ucslot, lsp_key_tx.egress, lsp_key_tx.ingress,
                   lsp_key_tx.lspid, lsp_key_tx.tunnelid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = -2;
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_lspid_by_txlsp
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT  spm_get_lspid_by_txlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                                NBB_ULONG *lspid, NBB_INT *uni_id, NBB_ULONG oamid)
{

    NBB_INT iret = ERROR;
    NBB_ULONG ulportindex = 0;
    CRTXLSP_KEY lsp_key_tx;
    SPM_PORT_INFO_CB stportinfo;
    OS_MEMSET(&lsp_key_tx, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&stportinfo, 0, sizeof(SPM_PORT_INFO_CB));

    if ((NULL == pstrsvplsp) || (NULL == lspid) || (NULL == uni_id))
    {
        goto EXIT_LABEL;
    }

    lsp_key_tx.egress = pstrsvplsp->tx_egress;
    lsp_key_tx.ingress = pstrsvplsp->tx_ingress;
    lsp_key_tx.lspid = pstrsvplsp->tx_lsp_id;
    lsp_key_tx.tunnelid = pstrsvplsp->tx_tunnel_id;

    /*迭代lspid*/
    iret = spm_l3_rsvpforoam(&lsp_key_tx, &ulportindex, lspid);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "Get LspId failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_tx.egress, lsp_key_tx.ingress,
                   lsp_key_tx.lspid, lsp_key_tx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 获取出端口C3芯片号 */
    iret = spm_get_portid_from_logical_port_index(ulportindex, &stportinfo);

    if (SUCCESS != iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get chip id failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "logical port index", "", "", oamid, ulportindex, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "Get chip id failed,logical port=%d\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid, ulportindex);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    *uni_id = stportinfo.unit_id;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_label_by_rxlsp
 功能描述  : 获取入标签
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_label_by_rxlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                               NBB_ULONG *ullabel_rx, NBB_ULONG oamid)
{
    CRTXLSP_KEY lsp_key_rx;
    NBB_INT iret = ERROR;
    OS_MEMSET(&lsp_key_rx, 0, sizeof(CRTXLSP_KEY));

    if ((NULL == pstrsvplsp) || (NULL == ullabel_rx))
    {
        goto EXIT_LABEL;
    }

    /* 获取lsp入标签,根据RXLSP获取入标签 */
    lsp_key_rx.egress = pstrsvplsp->rx_egress;
    lsp_key_rx.ingress = pstrsvplsp->rx_ingress;
    lsp_key_rx.lspid = pstrsvplsp->rx_lsp_id;
    lsp_key_rx.tunnelid = pstrsvplsp->rx_tunnel_id;
    iret = spm_l3_rxlabelforoam(&lsp_key_rx, ullabel_rx);

    if (1 == iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get rx label failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "egress", "ingress", "lspid", "tunnelid",
//                       lsp_key_rx.egress, lsp_key_rx.ingress,
//                       lsp_key_rx.lspid, lsp_key_rx.tunnelid));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "Cannot find the rx label\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_rx.egress, lsp_key_rx.ingress,
                   lsp_key_rx.lspid, lsp_key_rx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    else if (2 == iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get rx label failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "egress", "ingress", "lspid", "tunnelid",
//                       lsp_key_rx.egress, lsp_key_rx.ingress,
//                       lsp_key_rx.lspid, lsp_key_rx.tunnelid));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "Cannot find the rx label\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_rx.egress, lsp_key_rx.ingress,
                   lsp_key_rx.lspid, lsp_key_rx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_label_by_rxlsp
 功能描述  : 获取入标签
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_label_by_mip_txlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                                   NBB_ULONG *ullabel_rx, NBB_ULONG oamid)
{
    CRTXLSP_KEY lsp_key_rx;
    NBB_INT iret = ERROR;
    OS_MEMSET(&lsp_key_rx, 0, sizeof(CRTXLSP_KEY));

    if ((NULL == pstrsvplsp) || (NULL == ullabel_rx))
    {
        goto EXIT_LABEL;
    }

    /* 获取lsp入标签,根据RXLSP获取入标签 */
    lsp_key_rx.egress = pstrsvplsp->tx_egress;
    lsp_key_rx.ingress = pstrsvplsp->tx_ingress;
    lsp_key_rx.lspid = pstrsvplsp->tx_lsp_id;
    lsp_key_rx.tunnelid = pstrsvplsp->tx_tunnel_id;
    iret = spm_l3_rxlabelforoam(&lsp_key_rx, ullabel_rx);

    if (1 == iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get rx label failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "egress", "ingress", "lspid", "tunnelid",
//                       lsp_key_rx.egress, lsp_key_rx.ingress,
//                       lsp_key_rx.lspid, lsp_key_rx.tunnelid));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "Cannot find the rx label\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_rx.egress, lsp_key_rx.ingress,
                   lsp_key_rx.lspid, lsp_key_rx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    else if (2 == iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get rx label failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "egress", "ingress", "lspid", "tunnelid",
//                       lsp_key_rx.egress, lsp_key_rx.ingress,
//                       lsp_key_rx.lspid, lsp_key_rx.tunnelid));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "Cannot find the rx label\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_rx.egress, lsp_key_rx.ingress,
                   lsp_key_rx.lspid, lsp_key_rx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_inport_by_rxlsp
 功能描述  : 根据rxlsp的值来获取入端口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_inport_by_rxlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                                NBB_ULONG *lspid, NBB_BYTE *inport, NBB_ULONG *inport_index,
                                NBB_ULONG oamid)
{
    CRTXLSP_KEY lsp_key_rx;
    NBB_INT iret = ERROR;
    NBB_ULONG outport = 0;
    OS_MEMSET(&lsp_key_rx, 0, sizeof(CRTXLSP_KEY));

    if ((NULL == pstrsvplsp) || (NULL == lspid) || (NULL == inport) || (NULL == inport_index))
    {
        goto EXIT_LABEL;
    }

    lsp_key_rx.egress = pstrsvplsp->rx_egress;
    lsp_key_rx.ingress = pstrsvplsp->rx_ingress;
    lsp_key_rx.lspid = pstrsvplsp->rx_lsp_id;
    lsp_key_rx.tunnelid = pstrsvplsp->rx_tunnel_id;

    /* 根据rxlsp的key值获取入端口 */
#if 0
    iret = spm_l3_rxportlspidforoam(&lsp_key_rx, inport, inport_index, &outport, lspid);

    if (SUCCESS != iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get the inport failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "egress", "ingress", "lspid", "tunnelid",
//                       lsp_key_rx.egress, lsp_key_rx.ingress,
//                       lsp_key_rx.lspid, lsp_key_rx.tunnelid));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "get the inport failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_rx.egress, lsp_key_rx.ingress,
                   lsp_key_rx.lspid, lsp_key_rx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
#endif
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_inport_by_txlsp
 功能描述  : 根据rxlsp的值来获取入端口
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_inport_by_txlsp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                                NBB_ULONG *lspid, NBB_BYTE *inport, NBB_ULONG *inport_index,
                                NBB_ULONG oamid)
{
    CRTXLSP_KEY lsp_key_rx;
    NBB_INT iret = ERROR;
    NBB_ULONG outport = 0;
    OS_MEMSET(&lsp_key_rx, 0, sizeof(CRTXLSP_KEY));

    if ((NULL == pstrsvplsp) || (NULL == lspid) || (NULL == inport) || (NULL == inport_index))
    {
        goto EXIT_LABEL;
    }

    lsp_key_rx.egress = pstrsvplsp->tx_egress;
    lsp_key_rx.ingress = pstrsvplsp->tx_ingress;
    lsp_key_rx.lspid = pstrsvplsp->tx_lsp_id;
    lsp_key_rx.tunnelid = pstrsvplsp->tx_tunnel_id;

    /* 根据rxlsp的key值获取入端口 */
#if 0
    iret = spm_l3_rxportlspidforoam(&lsp_key_rx, inport, inport_index, &outport, lspid);

    if (SUCCESS != iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get the inport failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "egress", "ingress", "lspid", "tunnelid",
//                       lsp_key_rx.egress, lsp_key_rx.ingress,
//                       lsp_key_rx.lspid, lsp_key_rx.tunnelid));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "egress=%#x ingress=%#x lsp id=%#x tunnel id=%#x ret=%d\n"
                   "get the inport failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   lsp_key_rx.egress, lsp_key_rx.ingress,
                   lsp_key_rx.lspid, lsp_key_rx.tunnelid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
#endif
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_delete_old_cfg_when_update
 功能描述  : 配置更新
 输入参数  : SPM_OAM_CB *pstoamcfg  
             NBB_BYTE oamtype      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_delete_old_cfg_when_update(SPM_OAM_CB *pstoamcfg, NBB_BYTE oamtype)
{
    if (NULL == pstoamcfg)
    {
        return ERROR;
    }

    switch (oamtype)
    {
        case VS_OAM:
            spm_delete_avll_tree_by_index(pstoamcfg);
            break;

        case VP_OAM:
            spm_delete_avll_tree_by_txrsvp(pstoamcfg);
            spm_delete_avll_tree_by_rxrsvp(pstoamcfg);
            break;

        case VC_OAM:
            spm_delete_avll_tree_by_pw(pstoamcfg);
            break;

        case WRAPPING_OAM:
            spm_delete_avll_tree_by_wrapping(pstoamcfg);
            break;

        default:
            break;
    }

    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_set_from_bmu_active
 功能描述  : 从ips消息获取配置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : ERROR/SUCCESS
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_from_bmu_active(ATG_DCI_SET_TPOAM_ACTIVE *pstactiveoam_set,
                                SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;
    NBB_BYTE *pipsmegaddr = NULL;

    if ((NULL == pstactiveoam_set) || (NULL == pstoamcfg))
    {
        goto EXIT_LABEL;
    }

    /* basic */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->basic_data));

    if (NULL ==  pipsmegaddr)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM oam basic cfg is null"
//                       "rcv oam cfg failed", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "", "", "", pstactiveoam_set->key, 0, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\n basic_data is empty,oamid = %ld\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstactiveoam_set->key);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    pstoamcfg->pBasiCfg = (ATG_DCI_TPOAM_ACTIVE_BASIC_DATA *)
                         NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_BASIC_DATA),
                                      NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_BASEIC_CB);

    if (NULL == pstoamcfg->pBasiCfg)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nalloc memory failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    NBB_MEMCPY(pstoamcfg->pBasiCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_BASIC_DATA));

    /* pw */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->pw_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pPwCfg = (ATG_DCI_TPOAM_ACTIVE_PW *)
                           NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_PW),
                                        NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_PW_CB);

        if (NULL != pstoamcfg->pPwCfg)
        {
            NBB_MEMCPY(pstoamcfg->pPwCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_PW));
        }
    }

    /*wrapping*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->wrapping_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pWrapping = (ATG_DCI_TPOAM_ACTIVE_WRAPPING *)
                              NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_WRAPPING),
                                           NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_WRAPPING_CB);

        if (NULL != pstoamcfg->pWrapping)
        {
            NBB_MEMCPY(pstoamcfg->pWrapping, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_WRAPPING));
        }
    }

    /* vs */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->vs_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pVsCfg = (ATG_DCI_TPOAM_ACTIVE_VS *)
                           NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_VS),
                                        NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_VS_CB);

        if (NULL != pstoamcfg->pVsCfg)
        {
            NBB_MEMCPY(pstoamcfg->pVsCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_VS));
        }
    }

    /* APS */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->aps_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pApscfg = (ATG_DCI_TPOAM_ACTIVE_APS *)
                            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_APS),
                                         NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_APS_CB);

        if (NULL != pstoamcfg->pApscfg)
        {
            NBB_MEMCPY(pstoamcfg->pApscfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_APS));
        }
    }

    /* CV */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->cv_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pCvcfg = (ATG_DCI_TPOAM_ACTIVE_CV *)
                           NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_CV),
                                        NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_CV_CB);

        if (NULL != pstoamcfg->pCvcfg)
        {
            NBB_MEMCPY(pstoamcfg->pCvcfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_CV));
        }
    }

    /* CSF */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->csf_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pCsfcfg = (ATG_DCI_TPOAM_ACTIVE_CSF *)
                            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_CSF),
                                         NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_CSF_CB);

        if (NULL != pstoamcfg->pCsfcfg)
        {
            NBB_MEMCPY(pstoamcfg->pCsfcfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_CSF));
        }
    }

    /* FDI */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->fdi_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pFdicfg = (ATG_DCI_TPOAM_ACTIVE_FDI *)
                            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_FDI),
                                         NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_FDI_CB);

        if (NULL != pstoamcfg->pFdicfg)
        {
            NBB_MEMCPY(pstoamcfg->pFdicfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_FDI));
        }
    }

    /* rsvplsp */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->rsvp_lsp_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pRsvplspCfg = (ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *)
                                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP),
                                             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_RSVP_CB);

        if (NULL != pstoamcfg->pRsvplspCfg)
        {
            NBB_MEMCPY(pstoamcfg->pRsvplspCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP));
        }
    }

    /* adjust手工调整指标 */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->adjust_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pAdjust = (ATG_DCI_TPOAM_ACTIVE_ADJUST *)
                            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_ADJUST),
                                         NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_ADJUST_CB);

        if (NULL != pstoamcfg->pAdjust)
        {
            NBB_MEMCPY(pstoamcfg->pAdjust, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_ADJUST));
        }
    }

    /* alarm告警屏蔽 */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->alarm_data));

    if (NULL !=  pipsmegaddr)
    {
        pstoamcfg->pAlarm = (ATG_DCI_TPOAM_ACTIVE_ALARM *)
                           NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_ALARM),
                                        NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_ALARM_CB);

        if (NULL != pstoamcfg->pAlarm)
        {
            NBB_MEMCPY(pstoamcfg->pAlarm, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_ALARM));
        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_set_from_bmu_active_update
 功能描述  : 从ips消息获取配置(更新)
 输入参数  : 无
 输出参数  : 无
 返 回 值  : ERROR/SUCCESS
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_from_bmu_active_update(ATG_DCI_SET_TPOAM_ACTIVE *pstactiveoam_set,
                                       SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;
    NBB_BYTE *pipsmegaddr = NULL;

    if ((NULL == pstactiveoam_set) || (NULL == pstoamcfg))
    {
        goto EXIT_LABEL;
    }

    //更新时，p_Oamcfg配置不为空
    /* basic */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->basic_data));

    if (NULL ==  pipsmegaddr)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n basic_data is empty,oamid = %ld\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstactiveoam_set->key);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (NULL == pstoamcfg->pBasiCfg)
    {
        pstoamcfg->pBasiCfg = (ATG_DCI_TPOAM_ACTIVE_BASIC_DATA *)
                             NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_BASIC_DATA),
                                          NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_BASEIC_CB);

        if (NULL == pstoamcfg->pBasiCfg)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%dalloc memory for basic cfg failed\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstactiveoam_set->key);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    NBB_MEMCPY(pstoamcfg->pBasiCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_BASIC_DATA));

    /* pw */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->pw_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pPwCfg)
        {
            pstoamcfg->pPwCfg = (ATG_DCI_TPOAM_ACTIVE_PW *)
                               NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_PW),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_PW_CB);
        }

        if (0 != NBB_MEMCMP(pstoamcfg->pPwCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_PW)))
        {
            spm_delete_old_cfg_when_update(pstoamcfg, VC_OAM);
        }

        NBB_MEMCPY(pstoamcfg->pPwCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_PW));
    }

    /*wrapping*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->wrapping_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pWrapping)
        {
            pstoamcfg->pWrapping = (ATG_DCI_TPOAM_ACTIVE_WRAPPING *)
                                  NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_WRAPPING),
                                               NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_WRAPPING_CB);
        }

        if (0 != NBB_MEMCMP(pstoamcfg->pWrapping, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_WRAPPING)))
        {
            spm_delete_old_cfg_when_update(pstoamcfg, WRAPPING_OAM);
        }

        NBB_MEMCPY(pstoamcfg->pWrapping, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_WRAPPING));
    }

    /* vs */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->vs_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pVsCfg)
        {
            pstoamcfg->pVsCfg = (ATG_DCI_TPOAM_ACTIVE_VS *)
                               NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_VS),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_VS_CB);
        }

        if (0 != NBB_MEMCMP(pstoamcfg->pVsCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_VS)))
        {
            spm_delete_old_cfg_when_update(pstoamcfg, VS_OAM);
        }

        NBB_MEMCPY(pstoamcfg->pVsCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_VS));
    }

    /* APS */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->aps_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pApscfg)
        {
            pstoamcfg->pApscfg = (ATG_DCI_TPOAM_ACTIVE_APS *)
                                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_APS),
                                             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_APS_CB);
        }

        NBB_MEMCPY(pstoamcfg->pApscfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_APS));
    }

    /* CV */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->cv_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pCvcfg)
        {
            pstoamcfg->pCvcfg = (ATG_DCI_TPOAM_ACTIVE_CV *)
                               NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_CV),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_CV_CB);
        }

        NBB_MEMCPY(pstoamcfg->pCvcfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_CV));
    }

    /* CSF */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->csf_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pCsfcfg)
        {
            pstoamcfg->pCsfcfg = (ATG_DCI_TPOAM_ACTIVE_CSF *)
                                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_CSF),
                                             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_CSF_CB);
        }

        NBB_MEMCPY(pstoamcfg->pCsfcfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_CSF));
    }

    /* FDI */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->fdi_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pFdicfg)
        {
            pstoamcfg->pFdicfg = (ATG_DCI_TPOAM_ACTIVE_FDI *)
                                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_FDI),
                                             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_FDI_CB);
        }

        NBB_MEMCPY(pstoamcfg->pFdicfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_FDI));
    }

    /* rsvplsp */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->rsvp_lsp_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pRsvplspCfg)
        {
            pstoamcfg->pRsvplspCfg = (ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *)
                                    NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP),
                                                 NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_RSVP_CB);
        }

        if (0 != NBB_MEMCMP(pstoamcfg->pRsvplspCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP)))
        {
            spm_delete_old_cfg_when_update(pstoamcfg, VP_OAM);
        }

        NBB_MEMCPY(pstoamcfg->pRsvplspCfg, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP));
    }

    /* adjust手工调整指标 */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->adjust_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pAdjust)
        {
            pstoamcfg->pAdjust = (ATG_DCI_TPOAM_ACTIVE_ADJUST *)
                                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_ADJUST),
                                             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_ADJUST_CB);
        }

        NBB_MEMCPY(pstoamcfg->pAdjust, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_ADJUST));
    }

    /* alarm告警屏蔽 */
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstactiveoam_set, &(pstactiveoam_set->alarm_data));

    if (NULL !=  pipsmegaddr)
    {
        if (NULL == pstoamcfg->pAlarm)
        {
            pstoamcfg->pAlarm = (ATG_DCI_TPOAM_ACTIVE_ALARM *)
                               NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_ACTIVE_ALARM),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_ALARM_CB);
        }

        NBB_MEMCPY(pstoamcfg->pAlarm, pipsmegaddr, sizeof(ATG_DCI_TPOAM_ACTIVE_ALARM));
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_set_from_bmu_od
 功能描述  : 从ips消息获取配置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : ERROR/SUCCESS
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_from_bmu_od(IN ATG_DCI_SET_TPOAM_ON_DEMAND *pstodoam_set,
                            OAM_ON_DEMAND_LOCAL_STRU *podoam)
{
    NBB_INT iret = ERROR;
    NBB_BYTE *pipsmegaddr = NULL;
    NBB_BYTE ucloop = 0;

    if ((NULL == pstodoam_set) || (NULL == podoam))
    {
        goto EXIT_LABEL;
    }

    if (g_ucod_oamnum >= OAM_MAX_NUM_OD)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n od oam num is full,oam_num = %d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, g_ucod_oamnum);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->basic_data));

    if (NULL == pipsmegaddr)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n od oam basic cfg is null,oam_id = %d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstodoam_set->key);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    else
    {
        podoam->pBasicData = (ATG_DCI_TPOAM_OD_BASIC_DATA *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_BASIC_CB);

        if (NULL == podoam->pBasicData)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\nalloc memory failed\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }

        NBB_MEMCPY(podoam->pBasicData, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA));
    }

    /*dm*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->dm_frame_data));

    if (NULL != pipsmegaddr)
    {
        podoam->pDmFrame = (ATG_DCI_TPOAM_OD_DM_FRAME *)
                           NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_DM_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_DM_CB);

        if (NULL != podoam->pDmFrame)
        {
            NBB_MEMCPY(podoam->pDmFrame, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_DM_FRAME));
        }
    }

    /*lck*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->lck_frame_data));

    if (NULL !=  pipsmegaddr)
    {
        podoam->pLckFrame = (ATG_DCI_TPOAM_OD_LCK_FRAME *)
                            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LCK_CB);

        if (NULL != podoam->pLckFrame)
        {
            NBB_MEMCPY(podoam->pLckFrame, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME));
        }
    }

    /*tst*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->tst_frame_data));

    if (NULL !=  pipsmegaddr)
    {
        podoam->pTestFrame = (ATG_DCI_TPOAM_OD_TST_FRAME *)
                             NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_TST_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_TST_CB);

        if (NULL != podoam->pTestFrame)
        {
            NBB_MEMCPY(podoam->pTestFrame, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_TST_FRAME));
        }
    }

    /*lm*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->lm_frame_data));

    if (NULL != pipsmegaddr)
    {
        podoam->pLmFrame = (ATG_DCI_TPOAM_OD_LM_FRAME *)
                           NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LM_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LM_CB);

        if (NULL != podoam->pLmFrame)
        {
            NBB_MEMCPY(podoam->pLmFrame, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_LM_FRAME));
        }
    }

    /*lb*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->loop_frame_data));

    if (NULL !=  pipsmegaddr)
    {
        podoam->pLoopFrame = (ATG_DCI_TPOAM_OD_LOOP_FRAME *)
                             NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LB_CB);

        if (NULL != podoam->pLoopFrame)
        {
            NBB_MEMCPY(podoam->pLoopFrame, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME));
        }
    }

    /*mip*/
    pipsmegaddr = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstodoam_set, &(pstodoam_set->mip_data));

    if (NULL !=  pipsmegaddr)
    {
        podoam->pMip = (ATG_DCI_TPOAM_OD_MIP *)
                       NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_MIP), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_MIP_CB);

        if (NULL != podoam->pMip)
        {
            NBB_MEMCPY(podoam->pMip, pipsmegaddr, sizeof(ATG_DCI_TPOAM_OD_MIP));
        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_init_vscfg_node
 功能描述  : 段层获取处理槽位
 输入参数  : 无
 输出参数  : 无
 返 回 值  : ERROR/SUCCESS
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月13日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_init_vscfg_node(SPM_OAM_VS_CB *pstvscfg, SPM_OAM_CB *pstoamcfg, NBB_BYTE uclocalslot)
{
    NBB_INT iret = ERROR;
    SPM_PORT_INFO_CB stportinfo;
    OS_MEMSET(&stportinfo, 0, sizeof(SPM_PORT_INFO_CB));

    if ((NULL == pstvscfg) || (NULL == pstoamcfg) || (NULL == pstoamcfg->pVsCfg))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\ninput pointer are NULL\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 判断本盘与vsindex获取的处理盘是否相等 */
    iret = spm_get_process_slot_by_vsindex(pstoamcfg->pVsCfg->index, uclocalslot, &stportinfo);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    pstvscfg->ip = pstoamcfg->pVsCfg->nxhop_ip;
    pstvscfg->vs_portid = stportinfo.port_id;
    pstvscfg->vs_slotid = stportinfo.slot_id;
    pstoamcfg->vs_portid = stportinfo.port_id;
    pstoamcfg->vs_slotid = stportinfo.slot_id;

    pstoamcfg->ulTunnelLabel_tx = 3;//vs层tunnellable固定为3
    pstoamcfg->ulPwLable_tx = 13;
    pstoamcfg->ulTunnelLabel_rx = 13;
    pstoamcfg->ulPwLable_rx = stportinfo.port_id;
    pstoamcfg->chip_id = stportinfo.unit_id;

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_init_rsvp_node
 功能描述  : rsvp 节点初始化
 输入参数  : ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp  
             SPM_OAM_RSVP_CB_RX *prsvpcfgrx           
             SPM_OAM_RSVP_CB_TX *prsvpcfgtx           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_rsvp_node(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                            SPM_OAM_RSVP_CB_RX *prsvpcfgrx, SPM_OAM_RSVP_CB_TX *prsvpcfgtx)
{
    if ((NULL == pstrsvplsp)
        || (NULL == prsvpcfgrx) || (NULL == prsvpcfgtx))
    {
        return;
    }

    prsvpcfgtx->stRsvplspCfg.Egress = pstrsvplsp->tx_egress;
    prsvpcfgtx->stRsvplspCfg.Ingress = pstrsvplsp->tx_ingress;
    prsvpcfgtx->stRsvplspCfg.LspId = pstrsvplsp->tx_lsp_id;
    prsvpcfgtx->stRsvplspCfg.TunnelId = pstrsvplsp->tx_tunnel_id;

    prsvpcfgrx->stRsvplspCfg.Egress = pstrsvplsp->rx_egress;
    prsvpcfgrx->stRsvplspCfg.Ingress = pstrsvplsp->rx_ingress;
    prsvpcfgrx->stRsvplspCfg.LspId = pstrsvplsp->rx_lsp_id;
    prsvpcfgrx->stRsvplspCfg.TunnelId = pstrsvplsp->rx_tunnel_id;
    NBB_MEMCPY(&(prsvpcfgtx->rsvp_cfg_rx), &(prsvpcfgrx->stRsvplspCfg),
               4 * sizeof(NBB_ULONG));
    NBB_MEMCPY(&(prsvpcfgrx->rsvp_cfg_tx), &(prsvpcfgtx->stRsvplspCfg),
               4 * sizeof(NBB_ULONG));
    return;
}

/*****************************************************************************
 函 数 名  : spm_init_pw_node
 功能描述  : pw 节点初始化
 输入参数  : ATG_DCI_TPOAM_ACTIVE_PW *pstpw  
             SPM_OAM_CB *pstoamcfg            
             SPM_OAM_PW_CB *pstpwcfg          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_pw_node(ATG_DCI_TPOAM_ACTIVE_PW *pstpw,
                          SPM_OAM_CB *pstoamcfg, SPM_OAM_PW_CB *pstpwcfg)
{
    if ((NULL == pstpw) || (NULL == pstoamcfg) || (NULL == pstpwcfg))
    {
        return;
    }

    pstoamcfg->ulTunnelLabel_tx =  0x0;//vc层发方向的外层标签根据C3的要求必须为0
    pstoamcfg->ulTunnelLabel_rx = 0xFFFFF;

    pstpwcfg->stpwcfg.peer_ip = pstpw->peer_ip;
    pstpwcfg->stpwcfg.vc_id = pstpw->vc_id;
    pstpwcfg->stpwcfg.vc_type = pstpw->vc_type;
    return;
}

/*****************************************************************************
 函 数 名  : spm_set_mep_rsvp
 功能描述  : 设置mep的rsvp
 输入参数  : ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp  
             SPM_OAM_CB *pstoamcfg                       
             SPM_OAM_RSVP_CB_RX *prsvpcfgrx           
             SPM_OAM_RSVP_CB_TX *prsvpcfgtx           
             NBB_BYTE uclocalslot                       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mep_rsvp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                         SPM_OAM_CB *pstoamcfg, SPM_OAM_RSVP_CB_RX *prsvpcfgrx,
                         SPM_OAM_RSVP_CB_TX *prsvpcfgtx, NBB_BYTE uclocalslot)
{
    if ((NULL == pstrsvplsp) || (NULL == pstoamcfg)
        || (NULL == prsvpcfgrx) || (NULL == prsvpcfgtx))
    {
        goto EXIT_LABEL;
    }

    NBB_INT iret = ERROR;
    NBB_ULONG ullabel_tx = 0;
    NBB_USHORT usportid = 0;
    NBB_BYTE ucslot = 0;
    NBB_INT unit_id = 0;
    NBB_ULONG ullspid = 0;
    NBB_ULONG ullabel_rx = 0;
    NBB_ULONG lspid = 0;
    NBB_ULONG inport_index = 0;
    NBB_BYTE inport = 0;

    /* 获取出标签 */
    iret = spm_get_label_by_txlsp(pstrsvplsp, &ullabel_tx);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    //C3根据外层标签为0，来判断标签动作，解析OAM报文
//    pstoamcfg->ulTunnelLabel_tx = ullabel_tx;
    pstoamcfg->ulTunnelLabel_tx = 0;
    pstoamcfg->ulPwLable_tx = 13;

    /*获取出口lspid*/
    iret = spm_get_lspid_by_txlsp(pstrsvplsp,
                                  &ullspid, &unit_id, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    pstoamcfg->chip_id = (NBB_BYTE)unit_id;
    pstoamcfg->mcFlag = 0x0;
    prsvpcfgtx->vp_Lspid = ullspid;
    pstoamcfg->lsp_id_tx = prsvpcfgtx->vp_Lspid;

    /* 获取入标签 */
    iret = spm_get_label_by_rxlsp(pstrsvplsp, &ullabel_rx, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    pstoamcfg->ulTunnelLabel_rx = ullabel_rx;
    pstoamcfg->ulPwLable_rx = 13;

    /* 通过rxlsp获取入端口 */
    iret = spm_get_inport_by_rxlsp(pstrsvplsp,
                                   &lspid, &inport, &inport_index, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    prsvpcfgrx->vp_Lspid = lspid;
    prsvpcfgrx->vp_PortId = inport;//rxlsp的入端口
    prsvpcfgrx->inport_index = inport_index;
    pstoamcfg->lsp_id_rx = lspid;
    pstoamcfg->vp_PortId_rx = inport;
    pstoamcfg->inport_index_rx = inport_index;

    /* 获取出口槽位 */
    iret = spm_get_out_slot_by_txlsp(pstrsvplsp,
                                     uclocalslot, &ucslot, &usportid);

    if (ERROR == iret)
    {
        goto EXIT_LABEL;
    }

    if (-2 == iret)
    {
        iret = ERROR;
    }

    prsvpcfgtx->vp_Slot = ucslot;
    prsvpcfgtx->vp_PortId = usportid;
    prsvpcfgrx->vp_Slot = ucslot;
    pstoamcfg->slotMcid = ucslot;
    pstoamcfg->vp_PortId_tx = usportid;

    //所有单盘都写上送槽位表项
    iret = spm_set_mpls_flag(pstoamcfg);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_set_mip_rsvp
 功能描述  : 设置mip的rsvp
 输入参数  : ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp  
             SPM_OAM_CB *pstoamcfg                       
             SPM_OAM_RSVP_CB_RX *prsvpcfgrx           
             SPM_OAM_RSVP_CB_TX *prsvpcfgtx           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mip_rsvp(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP *pstrsvplsp,
                         SPM_OAM_CB *pstoamcfg, SPM_OAM_RSVP_CB_RX *prsvpcfgrx,
                         SPM_OAM_RSVP_CB_TX *prsvpcfgtx)
{
    NBB_INT iret = ERROR;
    NBB_ULONG lspid1 = 0;
    NBB_ULONG inport_index1 = 0;
    NBB_BYTE inport1 = 0;
    NBB_ULONG lspid2 = 0;
    NBB_ULONG inport_index2 = 0;
    NBB_BYTE inport2 = 0;
    NBB_ULONG ullabel_tx = 0;
    NBB_ULONG ullabel_rx = 0;

    if ((NULL == pstrsvplsp) || (NULL == pstoamcfg)
        || (NULL == prsvpcfgrx) || (NULL == prsvpcfgtx))
    {
        goto EXIT_LABEL;
    }

    /* 通过rxlsp获取入端口 */
    //MIP节点的正向rxlsp放在rsvptx树里，反向rxlsp放在rsvprx树里
    iret = spm_get_inport_by_txlsp(pstrsvplsp, &lspid1, &inport1, &inport_index1, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    prsvpcfgtx->vp_Lspid = lspid1;
    prsvpcfgtx->vp_PortId = inport1;
    prsvpcfgtx->vp_Slot = 0;
    prsvpcfgtx->inport_index = inport_index1;
    pstoamcfg->lsp_id_tx = lspid1;
    pstoamcfg->vp_PortId_tx = inport1;
    pstoamcfg->inport_index_tx = inport_index1;


    /* 获取入标签 */
    iret = spm_get_label_by_mip_txlsp(pstrsvplsp, &ullabel_tx, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    pstoamcfg->ulTunnelLabel_rx = ullabel_tx;
    pstoamcfg->ulPwLable_rx = 13;

    /* 获取入标签 */
    /* mip获取入标签，建立acl用 */
    iret = spm_get_label_by_rxlsp(pstrsvplsp, &ullabel_rx, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    //C3根据外层标签为0，来判断标签动作，解析OAM报文
//    pstoamcfg->ulTunnelLabel_tx = ullabel_tx;
    pstoamcfg->ulTunnelLabel_tx = 0;
    pstoamcfg->ulPwLable_tx = 13;
    prsvpcfgrx->mip_second_ulTunnelLabel_rx = ullabel_rx;
    pstoamcfg->mip_second_ulTunnelLabel_rx = ullabel_rx;

    /* 通过rxlsp获取入端口 */
    iret = spm_get_inport_by_rxlsp(pstrsvplsp, &lspid2, &inport2, &inport_index2, pstoamcfg->oam_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    prsvpcfgrx->vp_Lspid = lspid2;
    prsvpcfgrx->vp_PortId = inport2;//rxlsp的入端口
    prsvpcfgrx->vp_Slot = 0;
    prsvpcfgrx->inport_index = inport_index2;
    pstoamcfg->lsp_id_rx = lspid2;
    pstoamcfg->vp_PortId_rx = inport2;
    pstoamcfg->inport_index_rx = inport_index2;

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_set_mep_pw
 功能描述  : set mep pw
 输入参数  : ATG_DCI_TPOAM_ACTIVE_PW *pstpw  
             SPM_OAM_PW_CB *pstpwcfg          
             SPM_OAM_CB *pstoamcfg            
             NBB_BYTE uclocalslot            
             NBB_ULONG oamid                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mep_pw(ATG_DCI_TPOAM_ACTIVE_PW *pstpw,
                       SPM_OAM_PW_CB *pstpwcfg, SPM_OAM_CB *pstoamcfg,
                       NBB_BYTE uclocalslot, NBB_ULONG oamid)
{
#if 0
    NBB_INT iret = ERROR;
    NBB_INT unit_id = 0;
    SPM_VP_INFO_CB stVpInfo;
    ATG_DCI_VC_KEY stVcKey;
    NBB_BYTE ucMainBoradSlot = 0;
    OS_MEMSET(&stVpInfo, 0, sizeof(SPM_VP_INFO_CB));
    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    if ((NULL == pstpw) || (NULL == pstoamcfg) || (NULL == pstpwcfg))
    {
        goto EXIT_LABEL;
    }

    /* 迭代出lsplable,pwlable */
    stVcKey.peer_ip = pstpw->peer_ip;
    stVcKey.vc_id = pstpw->vc_id;
    stVcKey.vc_type = pstpw->vc_type;

    /* 获取vc_no */
    iret = spm_get_out_slot_by_pw(&stVcKey, uclocalslot, oamid, &stVpInfo);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    /* 增加支持dni-pw，当uni属性为VE时，OAM建在nni侧 */
    if (ATG_DCI_VE_PORT == stVpInfo.uni_port_type)
    {
        ucMainBoradSlot = stVpInfo.nni_slot;
        pstpwcfg->uni_slot = stVpInfo.nni_slot;
    }
    else
    {
        ucMainBoradSlot = stVpInfo.uni_slot;
        pstpwcfg->uni_slot = stVpInfo.uni_slot;
    }

    pstpwcfg->vc_no_section1 = stVpInfo.next_hop_id;
    pstpwcfg->vc_PortId_section1 = stVpInfo.uni_port_id;
    pstpwcfg->uni_port_type = stVpInfo.uni_port_type;
    pstpwcfg->nni_slot = stVpInfo.nni_slot;
    pstpwcfg->vpn_type = stVpInfo.vpn_type;
    pstpwcfg->vp_id = stVpInfo.vp_id;

    pstoamcfg->vc_no = pstpwcfg->vc_no_section1;

    /* 外层标签大于16，FPGA才认为是pw层报文，否则就丢弃，这里固定为全F */
    pstoamcfg->ulPwLable_rx = stVpInfo.pw_rx_label;

    /* vc层，pw标签固定为1，为私有标签，C3根据这个来解析oam报文 */
    pstoamcfg->ulPwCesLable_tx = stVpInfo.pw_tx_label;
    pstoamcfg->ulPwLable_tx = 1;
    iret = spm_get_unit_no_by_vckey(&stVcKey, oamid, &unit_id);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    pstoamcfg->lag_id = stVpInfo.lag_id;
    pstoamcfg->chip_id = unit_id;

    if (0x0 != stVpInfo.mc_id)//是仿真业务
    {
        pstoamcfg->slotMcid = (stVpInfo.mc_id & 0xff);
        pstoamcfg->mcFlag = 0x1;
    }
    else
    {
        pstoamcfg->slotMcid = pstpwcfg->uni_slot;//mantis 62855
        pstoamcfg->mcFlag = 0x0;
    }


    pstoamcfg->uni_slot = pstpwcfg->uni_slot;
    pstoamcfg->vc_no_section1 = pstpwcfg->vc_no_section1;
    pstoamcfg->vc_PortId_section1 = pstpwcfg->vc_PortId_section1;
    pstoamcfg->uni_port_type = pstpwcfg->uni_port_type;
    pstoamcfg->nni_slot = pstpwcfg->nni_slot;
    pstoamcfg->vpn_type = pstpwcfg->vpn_type;
    pstoamcfg->vp_id = pstpwcfg->vp_id;

    //所有单盘均写上送槽位表项
    iret = spm_set_mpls_flag(pstoamcfg);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    /* 判断是否本盘处理，本盘槽位号与UNI出口slot是否相等 */
    if (ucMainBoradSlot != uclocalslot)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "local slot=%d process slot=%d\n "
                   "the slot to deal with is not match with local slot\n"
                   "vc id=%d, vc type=%d, peer ip=%#x\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid, uclocalslot,
                   ucMainBoradSlot, stVcKey.vc_id, stVcKey.vc_type, stVcKey.peer_ip);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = ERROR;
    }

EXIT_LABEL:
    return iret;
#endif
    return SUCCESS;
}



/*****************************************************************************
 函 数 名  : spm_set_mip_pw
 功能描述  : 设置mip pw
 输入参数  : ATG_DCI_TPOAM_ACTIVE_PW *pstpw  
             SPM_OAM_PW_CB *pstpwcfg          
             SPM_OAM_CB *pstoamcfg            
             NBB_ULONG oamid                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mip_pw(ATG_DCI_TPOAM_ACTIVE_PW *pstpw,
                       SPM_OAM_PW_CB *pstpwcfg, SPM_OAM_CB *pstoamcfg, NBB_ULONG oamid)
{
#if 0
    NBB_INT iret = ERROR;
    ATG_DCI_VC_KEY stVcKey;
    SPM_SWITCH_VC_INFO swvcInfo;
    OS_MEMSET(&swvcInfo, 0, sizeof(SPM_SWITCH_VC_INFO));
    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    if ((NULL == pstpw) || (NULL == pstoamcfg) || (NULL == pstpwcfg))
    {
        goto EXIT_LABEL;
    }

    stVcKey.peer_ip = pstpw->peer_ip;
    stVcKey.vc_id = pstpw->vc_id;
    stVcKey.vc_type = pstpw->vc_type;
    iret = spm_get_out_vcno_by_switchvc(&stVcKey, oamid, &swvcInfo);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    pstpwcfg->vc_no_section1 = swvcInfo.vc_no1; //第一段vc的vc_no
    pstpwcfg->vc_PortId_section1 = swvcInfo.port_id1; //第一段vc出端口
    pstpwcfg->vc_no_section2 = swvcInfo.vc_no2;//第二段vc的vc_no
    pstpwcfg->vc_PortId_section2 = swvcInfo.port_id2; //第二段vc出端口
    pstpwcfg->vc_logical_port_index1 = swvcInfo.logical_port1;
    pstpwcfg->vc_logical_port_index2 = swvcInfo.logical_port2;
    pstpwcfg->swtich_pw_label_rx1 = swvcInfo.pw_rx1;
    pstpwcfg->swtich_pw_label_rx2 = swvcInfo.pw_rx2;
    pstpwcfg->uni_slot = 0;
    pstpwcfg->uni_port_type = 0;
    pstpwcfg->vpn_type = VPWS_TYPE;
    pstpwcfg->vp_id = 0;


    pstoamcfg->vc_no_section1 = pstpwcfg->vc_no_section1; //第一段vc的vc_no
    pstoamcfg->vc_PortId_section1 = pstpwcfg->vc_PortId_section1; //第一段vc出端口
    pstoamcfg->vc_no_section2 = pstpwcfg->vc_no_section2;//第二段vc的vc_no
    pstoamcfg->vc_PortId_section2 = pstpwcfg->vc_PortId_section2; //第二段vc出端口
    pstoamcfg->vc_logical_port_index1 = pstpwcfg->vc_logical_port_index1;
    pstoamcfg->vc_logical_port_index2 = pstpwcfg->vc_logical_port_index2;
    pstoamcfg->swtich_pw_label_rx1 = pstpwcfg->swtich_pw_label_rx1;
    pstoamcfg->swtich_pw_label_rx2 = pstpwcfg->swtich_pw_label_rx2;
    pstoamcfg->uni_slot = pstpwcfg->uni_slot;
    pstoamcfg->uni_port_type = pstpwcfg->uni_port_type;
    pstoamcfg->vpn_type = pstpwcfg->vpn_type;
    pstoamcfg->vp_id = pstpwcfg->vp_id;

    /* vc层，pw标签固定为1，为私有标签，C3根据这个来解析oam报文 */
    pstoamcfg->ulPwLable_tx = 0x1;

    /* 外层标签大于16，FPGA才认为是pw层报文，否则就丢弃，这里固定为全F */
    pstoamcfg->ulPwLable_rx = swvcInfo.pw_rx1;
    pstoamcfg->chip_id = swvcInfo.chip_no1;
    pstoamcfg->chip_id_second = swvcInfo.chip_no2;
    pstpwcfg->mip_nni_slot1 = swvcInfo.slot_id1;
    pstpwcfg->mip_nni_slot2 = swvcInfo.slot_id2;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
#endif
    return SUCCESS;

}

/*****************************************************************************
 函 数 名  : spm_get_out_slot_by_pw
 功能描述  : acl 规则下发
 输入参数  : ATG_DCI_VC_KEY *pvckey   
             NBB_BYTE uclocalslot     
             NBB_ULONG oamid          
             SPM_VP_INFO_CB *pvpinfo  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_out_slot_by_pw(ATG_DCI_VC_KEY *pvckey,
                               NBB_BYTE uclocalslot, NBB_ULONG oamid, SPM_VP_INFO_CB *pvpinfo)
{
    NBB_INT iret = ERROR;

    if ((NULL == pvckey) || (NULL == pvpinfo))
    {
        goto EXIT_LABEL;
    }

    iret = spm_get_vpinfo_from_vckey(pvckey, pvpinfo);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "vc id=%d vc type=%d peer ip=%x ret=%d\n"
                   "Get VpInfo failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   pvckey->vc_id, pvckey->vc_type, pvckey->peer_ip, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (VPWS_TYPE != pvpinfo->vpn_type)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "SPU cannot build vpls\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = ERROR;
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_get_unit_no_by_vckey
 功能描述  : acl 规则下发
 输入参数  : ATG_DCI_VC_KEY *pvckey  
             NBB_ULONG oamid         
             NBB_INT *unit_id        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_unit_no_by_vckey(ATG_DCI_VC_KEY *pvckey, NBB_ULONG oamid,
                                 NBB_INT *unit_id)
{
    NBB_INT iret = ERROR;
    NBB_ULONG outport_index = 0;
    SPM_PORT_INFO_CB stportinfo;

    if ((NULL == pvckey) || (NULL == unit_id))
    {
        goto EXIT_LABEL;
    }

    /* 获取出端口逻辑端口index */
    iret = spm_get_port_index_from_vc_key(pvckey, &outport_index);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid = %d\n"
                   "vc id=%d vc type=%d peer ip=%#x ret=%d\n"
                   "Get out port logical index failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   pvckey->vc_id, pvckey->vc_type, pvckey->peer_ip, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 根据逻辑端口的index获取C3芯片号 */
    iret = spm_get_portid_from_logical_port_index(outport_index, &stportinfo);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "outport_index=%#x ret=%d\n"
                   "Get chip id failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   outport_index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

#if 0
NBB_INT spm_get_out_vcno_by_switchvc(ATG_DCI_VC_KEY *pvckey,
                                     NBB_ULONG oamid, SPM_SWITCH_VC_INFO *pswvcInfo)
{
    NBB_INT iret = ERROR;

    if ((NULL == pvckey) || (NULL == pswvcInfo))
    {
        goto EXIT_LABEL;
    }

    /* 获取vc_no,第一段vc的出端口 */
#if 0
    iret = spm_get_info_from_switch_vc_key(pvckey, pswvcInfo);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "vc id=%d vc type=%d peer ip=%#x ret=%d\n"
                   "mspw get vc no from switch vckey failed\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamid,
                   pvckey->vc_id, pvckey->vc_type, pvckey->peer_ip, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
#endif
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}
#endif

/*****************************************************************************
 函 数 名  : spm_memfree_oam_cfg
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_CB *pstoamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_memfree_oam_cfg(SPM_OAM_CB *pstoamcfg)
{
    if ((NULL == pstoamcfg))
    {
        return;
    }

    if (NULL != pstoamcfg->pBasiCfg)
    {
        NBB_MM_FREE(pstoamcfg->pBasiCfg, MEM_SPM_ACTIVE_OAM_BASEIC_CB);
        pstoamcfg->pBasiCfg = NULL;
    }

    if (NULL != pstoamcfg->pCvcfg)
    {
        NBB_MM_FREE(pstoamcfg->pCvcfg, MEM_SPM_ACTIVE_OAM_CV_CB);
        pstoamcfg->pCvcfg = NULL;
    }

    if (NULL != pstoamcfg->pPwCfg)
    {
        NBB_MM_FREE(pstoamcfg->pPwCfg, MEM_SPM_ACTIVE_OAM_PW_CB);
        pstoamcfg->pPwCfg = NULL;
    }

    if (NULL != pstoamcfg->pCsfcfg)
    {
        NBB_MM_FREE(pstoamcfg->pCsfcfg, MEM_SPM_ACTIVE_OAM_CSF_CB);
        pstoamcfg->pCsfcfg = NULL;
    }

    if (NULL != pstoamcfg->pApscfg)
    {
        NBB_MM_FREE(pstoamcfg->pApscfg, MEM_SPM_ACTIVE_OAM_APS_CB);
        pstoamcfg->pApscfg = NULL;
    }

    if (NULL != pstoamcfg->pWrapping)
    {
        NBB_MM_FREE(pstoamcfg->pWrapping, MEM_SPM_ACTIVE_OAM_WRAPPING_CB);
        pstoamcfg->pWrapping = NULL;
    }

    if (NULL != pstoamcfg->pRsvplspCfg)
    {
        NBB_MM_FREE(pstoamcfg->pRsvplspCfg, MEM_SPM_ACTIVE_OAM_RSVP_CB);
        pstoamcfg->pRsvplspCfg = NULL;
    }

    if (NULL != pstoamcfg->pVsCfg)
    {
        NBB_MM_FREE(pstoamcfg->pVsCfg, MEM_SPM_ACTIVE_OAM_VS_CB);
        pstoamcfg->pVsCfg = NULL;
    }

    if (NULL != pstoamcfg->pFdicfg)
    {
        NBB_MM_FREE(pstoamcfg->pFdicfg, MEM_SPM_ACTIVE_OAM_FDI_CB);
        pstoamcfg->pFdicfg = NULL;
    }

    if (NULL != pstoamcfg->pAdjust)
    {
        NBB_MM_FREE(pstoamcfg->pAdjust, MEM_SPM_ACTIVE_OAM_ADJUST_CB);
        pstoamcfg->pAdjust = NULL;
    }

    if (NULL != pstoamcfg->pAlarm)
    {
        NBB_MM_FREE(pstoamcfg->pAlarm, MEM_SPM_ACTIVE_OAM_ALARM_CB);
        pstoamcfg->pAlarm = NULL;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_memfree_od_oam_cfg
 功能描述  : acl 规则下发
 输入参数  : OAM_ON_DEMAND_LOCAL_STRU *podoam  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_memfree_od_oam_cfg(OAM_ON_DEMAND_LOCAL_STRU *podoam)
{
    if ((NULL == podoam))
    {
        return;
    }

    if (NULL != podoam->pBasicData)
    {
        NBB_MM_FREE(podoam->pBasicData, MEM_SPM_OD_OAM_BASIC_CB);
        podoam->pBasicData = NULL;
    }

    if (NULL != podoam->pDmFrame)
    {
        NBB_MM_FREE(podoam->pDmFrame, MEM_SPM_OD_OAM_DM_CB);
        podoam->pDmFrame = NULL;
    }

    if (NULL != podoam->pLckFrame)
    {
        NBB_MM_FREE(podoam->pLckFrame, MEM_SPM_OD_OAM_LCK_CB);
        podoam->pLckFrame = NULL;
    }

    if (NULL != podoam->pLmFrame)
    {
        NBB_MM_FREE(podoam->pLmFrame, MEM_SPM_OD_OAM_LM_CB);
        podoam->pLmFrame = NULL;
    }

    if (NULL != podoam->pLoopFrame)
    {
        NBB_MM_FREE(podoam->pLoopFrame, MEM_SPM_OD_OAM_LB_CB);
        podoam->pLoopFrame = NULL;
    }

    if (NULL != podoam->pMip)
    {
        NBB_MM_FREE(podoam->pMip, MEM_SPM_OD_OAM_MIP_CB);
        podoam->pMip = NULL;
    }

    if (NULL != podoam->pTestFrame)
    {
        NBB_MM_FREE(podoam->pTestFrame, MEM_SPM_OD_OAM_TST_CB);
        podoam->pTestFrame = NULL;
    }

    return;
}



/*****************************************************************************
 函 数 名  : spm_get_enable
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_CB *pstoamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_enable(SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pBasiCfg))
    {
        goto EXIT_LABEL;
    }

    pstoamcfg->oamtype = pstoamcfg->pBasiCfg->oam_type;

    if (NULL != pstoamcfg->pCsfcfg)
    {
        pstoamcfg->csf_enable = pstoamcfg->pCsfcfg->csf_enable;
    }

    if (NULL != pstoamcfg->pApscfg)
    {
        pstoamcfg->aps_enable = pstoamcfg->pApscfg->aps_enable;
    }

    if (NULL != pstoamcfg->pCvcfg)
    {
        pstoamcfg->cv_cycle = pstoamcfg->pCvcfg->cv_cycle;
        pstoamcfg->cv_enable = pstoamcfg->pCvcfg->cv_enable;
    }

    if (NULL != pstoamcfg->pFdicfg)
    {
        pstoamcfg->fdi_enable = pstoamcfg->pFdicfg->fdi_enable;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_update_loop_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                          
             ATG_DCI_TPOAM_OD_LOOP_FRAME *pLoopFrame  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_update_loop_cfg(NBB_BYTE odloop, ATG_DCI_TPOAM_OD_LOOP_FRAME *pLoopFrame)
{
    if (NULL == pLoopFrame)
    {
        return;
    }
    else
    {
        if (NULL == g_stoam_odlocal[odloop].pLoopFrame)
        {
            g_stoam_odlocal[odloop].pLoopFrame = (ATG_DCI_TPOAM_OD_LOOP_FRAME *)
                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LB_CB);
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLoopFrame, pLoopFrame, sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME));
        }
        else
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLoopFrame, pLoopFrame, sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME));
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_update_lck_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                        
             ATG_DCI_TPOAM_OD_LCK_FRAME *pLckFrame  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_update_lck_cfg(NBB_BYTE odloop, ATG_DCI_TPOAM_OD_LCK_FRAME *pLckFrame)
{
    if (NULL == pLckFrame)
    {
        return;
    }
    else
    {
        if (NULL == g_stoam_odlocal[odloop].pLckFrame)
        {
            g_stoam_odlocal[odloop].pLckFrame = (ATG_DCI_TPOAM_OD_LCK_FRAME *)
                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LCK_CB);
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLckFrame, pLckFrame, sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME));
        }
        else
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLckFrame, pLckFrame, sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME));
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_update_dm_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                      
             ATG_DCI_TPOAM_OD_DM_FRAME *pDmFrame  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_update_dm_cfg(NBB_BYTE odloop, ATG_DCI_TPOAM_OD_DM_FRAME *pDmFrame)
{
    NBB_BYTE ucodloop = 0;
    NBB_BYTE i = 0;
    NBB_ULONG baseaddr;
    NBB_ULONG usoamloopi = 0;

    if (NULL == pDmFrame)
    {
        return;
    }
    else
    {
        if (NULL == g_stoam_odlocal[odloop].pDmFrame)
        {
            g_stoam_odlocal[odloop].pDmFrame = (ATG_DCI_TPOAM_OD_DM_FRAME *)
                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_DM_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_DM_CB);
            NBB_MEMCPY(g_stoam_odlocal[odloop].pDmFrame, pDmFrame, sizeof(ATG_DCI_TPOAM_OD_DM_FRAME));
        }
        else
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pDmFrame, pDmFrame, sizeof(ATG_DCI_TPOAM_OD_DM_FRAME));

            if (NULL != g_stoam_odlocal[odloop].pDmFrame)
            {
                if (g_stoam_odlocal[odloop].pDmFrame->byte1.send_enable == 0)
                {
                    ucodloop =  g_stoam_odlocal[odloop].ucOdLoop - 1;
                    usoamloopi = g_stoam_odlocal[odloop].usOamLoop;
                    spm_get_moudle_base_addr(usoamloopi, &baseaddr);

                    for (i = 0 ; i < 16 ; i++)
                    {
                        spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DMR_TIME_HEAD_OFFSET + ucodloop * 0x10 + i,
                                        0x0000);
                    }

                }
            }
        }

    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_update_lm_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                      
             ATG_DCI_TPOAM_OD_LM_FRAME *pLmFrame  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_update_lm_cfg(NBB_BYTE odloop, ATG_DCI_TPOAM_OD_LM_FRAME *pLmFrame)
{
    if (NULL == pLmFrame)
    {
        return;
    }
    else
    {
        if (NULL == g_stoam_odlocal[odloop].pLmFrame)
        {
            g_stoam_odlocal[odloop].pLmFrame = (ATG_DCI_TPOAM_OD_LM_FRAME *)
                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LM_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LM_CB);
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLmFrame, pLmFrame, sizeof(ATG_DCI_TPOAM_OD_LM_FRAME));
        }
        else
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLmFrame, pLmFrame, sizeof(ATG_DCI_TPOAM_OD_LM_FRAME));
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_update_tst_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                         
             ATG_DCI_TPOAM_OD_TST_FRAME *pTestFrame  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_update_tst_cfg(NBB_BYTE odloop, ATG_DCI_TPOAM_OD_TST_FRAME *pTestFrame)
{
    if (NULL == pTestFrame)
    {
        return;
    }
    else
    {
        if (NULL == g_stoam_odlocal[odloop].pTestFrame)
        {
            g_stoam_odlocal[odloop].pTestFrame = (ATG_DCI_TPOAM_OD_TST_FRAME *)
                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_TST_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_TST_CB);
            NBB_MEMCPY(g_stoam_odlocal[odloop].pTestFrame, pTestFrame, sizeof(ATG_DCI_TPOAM_OD_TST_FRAME));
        }
        else
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pTestFrame, pTestFrame, sizeof(ATG_DCI_TPOAM_OD_TST_FRAME));
        }
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_update_mip_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop             
             ATG_DCI_TPOAM_OD_MIP *pMip  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_update_mip_cfg(NBB_BYTE odloop, ATG_DCI_TPOAM_OD_MIP *pMip)
{
    if (NULL == pMip)
    {
        return;
    }
    else
    {
        if (NULL == g_stoam_odlocal[odloop].pMip)
        {
            g_stoam_odlocal[odloop].pMip = (ATG_DCI_TPOAM_OD_MIP *)
                NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_MIP), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_MIP_CB);
            NBB_MEMCPY(g_stoam_odlocal[odloop].pMip, pMip, sizeof(ATG_DCI_TPOAM_OD_MIP));
        }
        else
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pMip, pMip, sizeof(ATG_DCI_TPOAM_OD_MIP));
        }
    }

    return;
}



/*****************************************************************************
 函 数 名  : spm_od_oam_update_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                     
             OAM_ON_DEMAND_LOCAL_STRU *pstOdOam  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_oam_update_cfg(NBB_BYTE odloop, OAM_ON_DEMAND_LOCAL_STRU *pstOdOam)
{
    if (NULL == pstOdOam)
    {
        return;
    }

    if ((NULL != pstOdOam->pBasicData)
        && (NULL != g_stoam_odlocal[odloop].pBasicData))
    {
        NBB_MEMCPY(g_stoam_odlocal[odloop].pBasicData, pstOdOam->pBasicData,
                   sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA));
    }
    else
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nod basic cfg is null\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    spm_update_loop_cfg(odloop, pstOdOam->pLoopFrame);

    spm_update_tst_cfg(odloop, pstOdOam->pTestFrame);

    spm_update_lck_cfg(odloop, pstOdOam->pLckFrame);

    spm_update_lm_cfg(odloop, pstOdOam->pLmFrame);

    spm_update_dm_cfg(odloop, pstOdOam->pDmFrame);

    spm_update_mip_cfg(odloop, pstOdOam->pMip);
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_oam_add_cfg
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE odloop                     
             OAM_ON_DEMAND_LOCAL_STRU *pstOdOam  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_oam_add_cfg(NBB_BYTE odloop, OAM_ON_DEMAND_LOCAL_STRU *pstOdOam)
{

    if (NULL == pstOdOam)
    {
        return;
    }

    if ((NULL != pstOdOam->pBasicData))
    {
        g_stoam_odlocal[odloop].pBasicData = (ATG_DCI_TPOAM_OD_BASIC_DATA *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_BASIC_CB);

        if (NULL == g_stoam_odlocal[odloop].pBasicData)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\ninput parameter illegal\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            return;
        }

        NBB_MEMCPY(g_stoam_odlocal[odloop].pBasicData, pstOdOam->pBasicData,
                   sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA));
    }
    else
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n od oam basic cfg is null,oam_id = %d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstOdOam->ulOamActiveKey);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    if ((NULL != pstOdOam->pLoopFrame))
    {
        g_stoam_odlocal[odloop].pLoopFrame = (ATG_DCI_TPOAM_OD_LOOP_FRAME *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LB_CB);

        if (NULL != g_stoam_odlocal[odloop].pLoopFrame)
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLoopFrame, pstOdOam->pLoopFrame, sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME));
        }
    }

    if ((NULL != pstOdOam->pTestFrame))
    {
        g_stoam_odlocal[odloop].pTestFrame = (ATG_DCI_TPOAM_OD_TST_FRAME *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_TST_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_TST_CB);

        if (NULL != g_stoam_odlocal[odloop].pTestFrame)
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pTestFrame, pstOdOam->pTestFrame, sizeof(ATG_DCI_TPOAM_OD_TST_FRAME));
        }
    }

    if ((NULL != pstOdOam->pLckFrame))
    {
        g_stoam_odlocal[odloop].pLckFrame = (ATG_DCI_TPOAM_OD_LCK_FRAME *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LCK_CB);

        if (NULL != g_stoam_odlocal[odloop].pLckFrame)
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLckFrame, pstOdOam->pLckFrame, sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME));
        }
    }

    if ((NULL != pstOdOam->pLmFrame))
    {
        g_stoam_odlocal[odloop].pLmFrame = (ATG_DCI_TPOAM_OD_LM_FRAME *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_LM_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_LM_CB);

        if (NULL != g_stoam_odlocal[odloop].pLmFrame)
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pLmFrame, pstOdOam->pLmFrame, sizeof(ATG_DCI_TPOAM_OD_LM_FRAME));
        }
    }

    if ((NULL != pstOdOam->pDmFrame))
    {
        g_stoam_odlocal[odloop].pDmFrame = (ATG_DCI_TPOAM_OD_DM_FRAME *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_DM_FRAME), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_DM_CB);

        if (NULL != g_stoam_odlocal[odloop].pDmFrame)
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pDmFrame, pstOdOam->pDmFrame, sizeof(ATG_DCI_TPOAM_OD_DM_FRAME));
        }
    }

    if ((NULL != pstOdOam->pMip))
    {
        g_stoam_odlocal[odloop].pMip = (ATG_DCI_TPOAM_OD_MIP *)
            NBB_MM_ALLOC(sizeof(ATG_DCI_TPOAM_OD_MIP), NBB_NORETRY_ACT, MEM_SPM_OD_OAM_MIP_CB);

        if (NULL != g_stoam_odlocal[odloop].pMip)
        {
            NBB_MEMCPY(g_stoam_odlocal[odloop].pMip, pstOdOam->pMip, sizeof(ATG_DCI_TPOAM_OD_MIP));
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_lb_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_lb_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;
    NBB_BYTE offset = 0;

    /* 填写帧头MAC内容 */
    for (ucloop = 0 ; ucloop < 8 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodloopi * 8 + ucloop,
                        0x0000);
    }

    /* 标签寄存器内容 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_LABLE_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_LABLE_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);
    
    /* gal寄存器 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_GAL_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_GAL_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);

    /* meg/mep寄存器 */
    for (offset = 0 ; offset < 0x10 ; offset++)
    {
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + offset,
                        0x0000);
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_lck_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_lck_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;

    /* 填写帧头MAC内容 */
    for (ucloop = 0 ; ucloop < 8 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucodloopi * 8 + ucloop,
                        0x0000);
    }

    /* 标签寄存器内容 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LCK_LABLE_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LCK_LABLE_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);
    
    /* gal寄存器 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LCK_GAL_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LCK_GAL_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);
    return;
}

/*****************************************************************************
 函 数 名  : spm_lm_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_lm_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;
    
    //LMM
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0x0,
                    0x0000 NBB_CCXT_T NBB_CCXT);
    
    /*保留*/
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0x1,
                    0x0000 NBB_CCXT_T NBB_CCXT);
    
    /*长度*/
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0x2,
                    0x0000 NBB_CCXT_T NBB_CCXT);

    /* 填写帧头MAC内容 */
    for (ucloop = 0 ; ucloop < 8 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucodloopi * 8 + ucloop,
                        0x0000);
    }

    /* 标签寄存器内容 */
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_LABLE_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_LABLE_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);
    
    /* gal寄存器 */
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_GAL_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_GAL_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);

    /*保留*/
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0xd,
                    0x0000 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0xf,
                    0x0000 NBB_CCXT_T NBB_CCXT);

    //删除DMR
    for (ucloop = 0 ; ucloop < 10 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucodloopi * 8 + ucloop,
                        0x0000);
    }

    //oam_release_fpga_lm(ucodloopi);
    return;
}

/*****************************************************************************
 函 数 名  : spm_dm_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_dm_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;
    
    //DMM
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0x0,
                    0x0000 NBB_CCXT_T NBB_CCXT);
    
    /*保留*/
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0x1,
                    0x0000 NBB_CCXT_T NBB_CCXT);
    
    /*长度*/
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0x2,
                    0x0000 NBB_CCXT_T NBB_CCXT);

    /* 填写帧头MAC内容 */
    for (ucloop = 3 ; ucloop < 9 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + ucloop,
                        0x0000);
    }

    /* 标签寄存器内容 */
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_LABLE_OFFSET + ucodloopi * 10 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_LABLE_OFFSET + ucodloopi * 10 + 0x1,
                    0x0000);
    
    /* gal寄存器 */
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_GAL_OFFSET + ucodloopi * 10 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_GAL_OFFSET + ucodloopi * 10 + 0x1,
                    0x0000);

    /*保留*/
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0xd,
                    0x0000 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + 0xf,
                    0x0000 NBB_CCXT_T NBB_CCXT);

    //删除DMR
    for (ucloop = 0 ; ucloop < 10 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucodloopi * 0x10 + ucloop,
                        0x0000);
    }

    //删除DMR时戳
    for (ucloop = 0 ; ucloop < 16 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET + DMR_TIME_HEAD_OFFSET + ucodloopi * 0x10 + ucloop,
                        0x0000);
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_tst_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_tst_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;

    /* 填写帧头MAC内容 */
    for (ucloop = 0 ; ucloop < 8 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucodloopi * 8 + ucloop,
                        0x0000);
    }

    /* 标签寄存器内容 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + TST_LABLE_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + TST_LABLE_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);
    
    /* gal寄存器 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + TST_GAL_OFFSET + ucodloopi * 2 + 0x0,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + TST_GAL_OFFSET + ucodloopi * 2 + 0x1,
                    0x0000);
    return;
}


/*****************************************************************************
 函 数 名  : spm_mep_meg_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_mep_meg_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;

    for (ucloop = 0 ; ucloop < 0x10 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                        + MEP_MEG_BASE + ucodloopi * 0x10 + ucloop,
                        0x0000);
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_rx_label_delete_fpga
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG baseaddr  
             NBB_BYTE ucodloopi  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rx_label_delete_fpga(NBB_ULONG baseaddr, NBB_BYTE ucodloopi)
{
    NBB_BYTE ucloop = 0;

    for (ucloop = 0 ; ucloop < 0x3 ; ucloop++)
    {
        spm_oam_16write(baseaddr, DEMAND_RX_LSP_CFG_OFFSET
                        + ucodloopi * 0x3 + ucloop,
                        0x0000);
        spm_oam_16write(baseaddr, DEMAND_LM_RX_LSP_OFFSET
                        + ucodloopi * 3 + ucloop, 0x0 NBB_CCXT_T NBB_CCXT);
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_build_acl_for_wrapping
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG inport_index   
             NBB_ULONG customdata[3]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_build_acl_for_wrapping(NBB_ULONG inport_index, NBB_ULONG customdata[3])
{
    NBB_INT iret = ERROR;
    NBB_ULONG posid = 0;
    NBB_BYTE fpgaport = 105;

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    iret = spm_get_poscnt_from_intf(inport_index, 0, 0, &posid);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n get posid from intf failed\n"
                   "inport index=%#x, ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, inport_index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    printf("inport_index=%d,posid=%d\n", inport_index, posid);
    iret = redirecte_mpls_data_to_fpga_for_wrapping(posid, fpgaport, g_usoam_num, customdata);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n build acl for wrapping failed\n"
                   "posid=%#x,out_index=%#x,ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, posid, inport_index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_mip_build_acl_for_lbm_vp
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG inport_index   
             NBB_ULONG customdata[6]  
             NBB_BYTE OneOrTwo        
             NBB_BYTE fpgaport        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_mip_build_acl_for_lbm_vp(NBB_ULONG inport_index,
                                     NBB_ULONG customdata[6], NBB_BYTE OneOrTwo, NBB_BYTE fpgaport)
{
    NBB_INT iret = ERROR;
    NBB_ULONG posid = 0;

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    iret = spm_get_poscnt_from_intf(inport_index, 0, 0, &posid);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n lb,mip node get posid from intf failed\n"
                   "inport index=%#x ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, inport_index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x1 == OneOrTwo)  //中间节点要建两条acl的第一条
    {
        iret = redirecte_mpls_data_to_fpga_for_lsp(posid, fpgaport, g_usoam_num, customdata);

        if (SUCCESS != iret)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n lb,mip node build acl failed\n"
                       "posid=%#x ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, posid, iret);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }
    else if (0x2 == OneOrTwo)  //第二条acl
    {
        iret = redirecte_mpls_data_to_fpga_for_lsp2(posid, fpgaport, g_usoam_num, customdata);

        if (SUCCESS != iret)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n lb,mip node build acl failed\n"
                       "posid=%#x ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, posid, iret);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_mip_build_acl_for_lbm_pw
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG inport_index   
             NBB_ULONG customdata[5]  
             NBB_BYTE OneOrTwo        
             NBB_BYTE fpgaport        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_mip_build_acl_for_lbm_pw(NBB_ULONG inport_index,
                                     NBB_ULONG customdata[5], NBB_BYTE OneOrTwo, NBB_BYTE fpgaport)
{
    NBB_INT iret = ERROR;
    NBB_ULONG posid = 0;

    if (NULL == customdata)
    {
        goto EXIT_LABEL;
    }

    iret = spm_get_poscnt_from_intf(inport_index, 0, 0, &posid);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n lb,mip node get posid from intf failed\n"
                   "inport index=%#x ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, inport_index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x1 == OneOrTwo)  //中间节点要建两条acl的第一条
    {
        iret = redirecte_mpls_data_to_fpga_for_pw(posid, fpgaport, g_usoam_num, customdata);

        if (SUCCESS != iret)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n lb,mip node build acl failed\n"
                       "posid=%#x ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, posid, iret);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }
    else if (0x2 == OneOrTwo)  //第2条acl
    {
        iret = redirecte_mpls_data_to_fpga_for_pw2(posid, fpgaport, g_usoam_num, customdata);

        if (SUCCESS != iret)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n lb,mip node build acl failed\n"
                       "posid=%#x ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, posid, iret);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_get_od_tmpls
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE ucodloop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_od_tmpls(NBB_BYTE ucodloop)
{
    CV_CFH_HEAD_LABLE1_STRU sttmplslable;
    NBB_ULONG ultmplslable = 0;
    NBB_USHORT usloop = 0;
    OS_MEMSET(&sttmplslable, 0, sizeof(CV_CFH_HEAD_LABLE1_STRU));
    usloop = g_stoam_odlocal[ucodloop].usOamLoop;

    if (NULL != g_stoam_odlocal[ucodloop].pBasicData)
    {
        switch (g_stoam_local[usloop].oam_type)
        {
            case VS_OAM:
                sttmplslable.Tc = g_stoam_odlocal[ucodloop].pBasicData->gal_exp;
                sttmplslable.Ttl = g_stoam_odlocal[ucodloop].pBasicData->gal_ttl;
                break;

            case VP_OAM:
            case VC_OAM:
                sttmplslable.Tc = g_stoam_odlocal[ucodloop].pBasicData->lsp_exp;
                sttmplslable.Ttl = g_stoam_odlocal[ucodloop].pBasicData->lsp_ttl;
                break;

            default:
                OS_SPRINTF(g_uc_message, "%s %s,%d\noam type illegal\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                return;
        }

        sttmplslable.TunnelLable = ((g_stoam_odlocal[ucodloop].ulTunnelLabel_tx) & 0xFFFFF);
        sttmplslable.Zero = 0;
        ultmplslable = *(NBB_ULONG *)(&sttmplslable);
        g_stoam_odlocal[ucodloop].tmpls_label_fpga = ultmplslable;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_get_od_gal
 功能描述  : acl 规则下发
 输入参数  : NBB_BYTE ucodloop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_od_gal(NBB_BYTE ucodloop)
{
    CV_CFH_HEAD_LABLE2_STRU stgallable;
    NBB_USHORT usloop = 0;
    NBB_ULONG ulgallable = 0;
    OS_MEMSET(&stgallable, 0, sizeof(CV_CFH_HEAD_LABLE2_STRU));
    usloop = g_stoam_odlocal[ucodloop].usOamLoop;

    if (NULL != g_stoam_odlocal[ucodloop].pBasicData)
    {
        switch (g_stoam_local[usloop].oam_type)
        {
            case VS_OAM:
            case VP_OAM:
                stgallable.Ttl = g_stoam_odlocal[ucodloop].pBasicData->gal_ttl;
                stgallable.Tc = g_stoam_odlocal[ucodloop].pBasicData->gal_exp;
                break;

            case VC_OAM:
                stgallable.Ttl = g_stoam_odlocal[ucodloop].pBasicData->pw_ttl;
                stgallable.Tc = g_stoam_odlocal[ucodloop].pBasicData->pw_exp;
                break;

            default:
                OS_SPRINTF(g_uc_message, "%s %s,%d\noam type illegal\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                return;
        }

        stgallable.Gal = g_stoam_local[usloop].ulPwLable_tx;
        stgallable.Zero = 1;
        ulgallable = *(NBB_ULONG *)(&stgallable);
        g_stoam_odlocal[ucodloop].gal_label_fpga = ulgallable;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_wrapping_mac
 功能描述  : acl 规则下发
 输入参数  : IN NBB_USHORT usloop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_wrapping_mac(IN NBB_USHORT usloop)
{
    CV_CFH_SLOTID_STRU cvslotid;
    CV_CFH_PORTID_STRU cvportid;
    NBB_USHORT usportiddata;
    NBB_USHORT usslotiddata;
    NBB_USHORT usvanid = 0;
    NBB_ULONG ip = 0;
    NBB_USHORT utemp = 0;
    NBB_ULONG baseaddr = 0;
    NBB_USHORT uslocalindex = 0;
    OS_MEMSET((&cvslotid), 0, sizeof(CV_CFH_SLOTID_STRU));
    OS_MEMSET((&cvportid), 0, sizeof(CV_CFH_PORTID_STRU));

    /* 帧头各部分赋值 */
    cvslotid.Unit = 0;
    cvslotid.SlotId = g_stoam_local[usloop].vs_Slotid;
    cvportid.DType = 3; /* 代表是vs层oam */
    cvportid.IsLagId = 0;
    cvportid.chip_id = g_stoam_local[usloop].chip_id;
    cvportid.PortId = g_stoam_local[usloop].vs_Portid;
    usslotiddata = *(NBB_USHORT *)(&cvslotid);
    usportiddata = *(NBB_USHORT *)(&cvportid);
    ip = g_stoam_local[usloop].ip;
    spm_get_moudle_base_addr(usloop, &baseaddr);
    uslocalindex = g_stoam_local[usloop].module_index;

    /* 填写帧头MAC内容 */
    spm_oam_16write(baseaddr, CV_TX_DMAC_OFFSET1 + uslocalindex ,
                    0x0000);
    spm_oam_16write(baseaddr, CV_TX_DMAC_OFFSET1 + uslocalindex,
                    usvanid);
    spm_oam_16write(baseaddr, CV_TX_DMAC_OFFSET1 + uslocalindex,
                    usslotiddata);
    spm_oam_16write(baseaddr, CV_TX_DMAC_OFFSET1 + uslocalindex,
                    usportiddata);
    spm_oam_16write(baseaddr, CV_TX_DMAC_OFFSET1 + uslocalindex,
                    (ip >> 16) & 0xFFFF);
    spm_oam_16write(baseaddr, CV_TX_DMAC_OFFSET1 + uslocalindex,
                    ip & 0xFFFF);

    /* 对等MEP ID写入寄存器 */
    printf("%s,", __FUNCTION__);
    printf("dst_mep_id=%d,oamid=%d,usloop=%d\n", g_stoam_local[usloop].dst_mep_id,
           g_stoam_local[usloop].ulActiveOamkey, usloop);
    printf("baseaddr=%#x,offsetAddr=%#x,uslocalindex=%d\n", baseaddr,
           CV_EXPECT_MEP_OFFSET, uslocalindex);
    spm_oam_16write(baseaddr, CV_EXPECT_MEP_OFFSET + uslocalindex, g_stoam_local[usloop].dst_mep_id);
    utemp = spm_oam_16read(baseaddr, CV_EXPECT_MEP_OFFSET + uslocalindex);
    printf("mep_id_in_fpga=%d\n", utemp);
    return;
}

/*****************************************************************************
 函 数 名  : spm_wrapping_label
 功能描述  : acl 规则下发
 输入参数  : IN NBB_USHORT usloop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_wrapping_label(IN NBB_USHORT usloop)
{
    NBB_ULONG ulwrappinglabel = 0;
    CV_CFH_HEAD_LABLE1_STRU cvheadlable1;
    CV_CFH_HEAD_LABLE2_STRU cvheadlable2;
    FRAM_TX_ENABLE1 frametxlableenable1;
    FRAM_TX_ENABLE2 frametxlableenable2;
    NBB_ULONG ullableda1;
    NBB_ULONG ullableda2;
    NBB_USHORT usframtxenabledata1 = 0;
    NBB_USHORT usframtxenabledata2 = 0;
    NBB_ULONG baseaddr;
    NBB_USHORT uslocalindex = 0; //模块本地索引

    OS_MEMSET((&frametxlableenable1), 0, sizeof(FRAM_TX_ENABLE1));
    OS_MEMSET((&frametxlableenable2), 0, sizeof(FRAM_TX_ENABLE2));
    OS_MEMSET((&cvheadlable1), 0, sizeof(CV_CFH_HEAD_LABLE1_STRU));
    OS_MEMSET((&cvheadlable2), 0, sizeof(CV_CFH_HEAD_LABLE2_STRU));
    
    /* 判断所在模块 */
    spm_get_moudle_base_addr(usloop, &baseaddr);
    uslocalindex = g_stoam_local[usloop].module_index;
    ulwrappinglabel = g_stoam_local[usloop].ulTunnelLabel_tx;
    
    /* 两层标签字段赋值 */
    cvheadlable1.TunnelLable = ulwrappinglabel;
    cvheadlable1.Tc = g_stoam_local[usloop].lsp_exp;
    cvheadlable1.Ttl = g_stoam_local[usloop].lsp_ttl;
    cvheadlable1.Zero = 0;

    cvheadlable2.Gal = g_stoam_local[usloop].ulPwLable_tx;
    cvheadlable2.Tc = g_stoam_local[usloop].gal_exp;
    cvheadlable2.Ttl = g_stoam_local[usloop].gal_ttl;
    cvheadlable2.Zero = 1;
    ullableda1 = *(NBB_ULONG *)(&cvheadlable1);
    ullableda2 = *(NBB_ULONG *)(&cvheadlable2);
    
    /* 条目N的帧发送标签赋值 */
    frametxlableenable1.CvEnable = g_stoam_local[usloop].cv_enable;
    frametxlableenable1.LspApsEnable = g_stoam_local[usloop].aps_enable;
    frametxlableenable2.apsOpcode = WRAPPING_TX;
    frametxlableenable2.apsPort = g_stoam_local[usloop].vs_Portid - 1;
    frametxlableenable2.mel = g_stoam_local[usloop].mel;
    usframtxenabledata1 = *(NBB_USHORT *)(&frametxlableenable1);
    usframtxenabledata2 = *(NBB_USHORT *)(&frametxlableenable2);
    
    /* 条目N的发送标签使能写寄存器 */
    spm_oam_16write(baseaddr, CV_TXVLAN_LABEL_OFFSET + uslocalindex * 4 + 0x0,
                    (ullableda1 >> 16) & 0xFFFF);
    spm_oam_16write(baseaddr, CV_TXVLAN_LABEL_OFFSET + uslocalindex * 4 + 0x1,
                    ullableda1 & 0xFFFF);
    spm_oam_16write(baseaddr, CV_TXVLAN_LABEL_OFFSET + uslocalindex * 4 + 0x2,
                    (ullableda2 >> 16) & 0xFFFF);
    spm_oam_16write(baseaddr, CV_TXVLAN_LABEL_OFFSET + uslocalindex * 4 + 0x3,
                    ullableda2 & 0xFFFF);
    
    /* 发送的mepid写入寄存器 */
    spm_oam_16write(baseaddr, CV_TXMEP_OFFSET + uslocalindex,
                    (g_stoam_local[usloop].src_mep_id));
    spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + uslocalindex * 1 + 0x0,
                    usframtxenabledata1);
    spm_oam_16write(baseaddr, CV_TXMEL_OFFSET + uslocalindex * 1 + 0x0,
                    usframtxenabledata2);
    return;
}

/*****************************************************************************
 函 数 名  : spm_get_lsp_pw_exp_ttl
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT usloop  
             NBB_BYTE *lsp_exp  
             NBB_BYTE *pw_exp   
             NBB_BYTE *lsp_ttl  
             NBB_BYTE *pw_ttl   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_lsp_pw_exp_ttl(NBB_USHORT usloop, NBB_BYTE *lsp_exp,
                                NBB_BYTE *pw_exp, NBB_BYTE *lsp_ttl, NBB_BYTE *pw_ttl)
{
    if ((NULL == lsp_exp) || (NULL == pw_exp) || (NULL == lsp_ttl) || (NULL == pw_ttl))
    {
        return;
    }

    switch (g_stoam_local[usloop].oam_type)
    {
        case VP_OAM:
            *lsp_exp = g_stoam_local[usloop].lsp_exp;
            *pw_exp = g_stoam_local[usloop].gal_exp;
            *lsp_ttl = g_stoam_local[usloop].lsp_ttl;
            *pw_ttl = g_stoam_local[usloop].gal_ttl;
            break;

        case VC_OAM:
            *lsp_exp = g_stoam_local[usloop].lsp_exp;
            *pw_exp = g_stoam_local[usloop].pw_exp;
            *lsp_ttl = g_stoam_local[usloop].lsp_ttl;
            *pw_ttl = g_stoam_local[usloop].pw_ttl;
            break;

        default:
            *lsp_exp = g_stoam_local[usloop].gal_exp;
            *pw_exp = g_stoam_local[usloop].gal_exp;
            *lsp_ttl = g_stoam_local[usloop].gal_ttl;
            *pw_ttl = g_stoam_local[usloop].gal_ttl;
            break;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_set_mpls_and_gal_label_for_acl
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT usoamloop     
             NBB_ULONG *mplslabele  
             NBB_ULONG *mpls_label_i  
             NBB_ULONG *pw_label      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_set_mpls_and_gal_label_for_acl(NBB_USHORT usoamloop,
        NBB_ULONG *mplslabele, NBB_ULONG *mpls_label_i, NBB_ULONG *pw_label)
{
    NBB_BYTE lsp_exp;
    NBB_BYTE pw_exp;
    NBB_BYTE lsp_ttl;
    NBB_BYTE pw_ttl;
    CV_CFH_HEAD_LABLE1_STRU external_label;
    CV_CFH_HEAD_LABLE2_STRU inner_label;
    OS_MEMSET(&external_label, 0, sizeof(CV_CFH_HEAD_LABLE1_STRU));
    OS_MEMSET(&inner_label, 0, sizeof(CV_CFH_HEAD_LABLE2_STRU));

    if ((NULL == mplslabele) || (NULL == mpls_label_i) || (NULL == pw_label))
    {
        return;
    }

    spm_get_lsp_pw_exp_ttl(usoamloop, &lsp_exp, &pw_exp, &lsp_ttl, &pw_ttl);

    external_label.TunnelLable = g_stoam_local[usoamloop].mip_second_tunnel_rx;
    external_label.Tc = 7;
    external_label.Ttl = 255;
    external_label.Zero = 0;
    *mplslabele = *(NBB_ULONG *)(&external_label);

    external_label.TunnelLable = g_stoam_local[usoamloop].ulTunnelLabel_rx;
    *mpls_label_i = *(NBB_ULONG *)(&external_label);

    inner_label.Gal = g_stoam_local[usoamloop].ulPwLable_rx;
    inner_label.Tc = 7;
    inner_label.Ttl = 255;
    inner_label.Zero = 1;
    *pw_label = *(NBB_ULONG *)(&inner_label);
    return;
}


/*****************************************************************************
 函 数 名  : spm_get_pw_label_for_swtichvc_acl
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT usoamloop     
             NBB_ULONG *mplslabele  
             NBB_ULONG *mpls_label_i  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_pw_label_for_swtichvc_acl(NBB_USHORT usoamloop,
        NBB_ULONG *mplslabele, NBB_ULONG *mpls_label_i)
{
    NBB_BYTE lsp_exp;
    NBB_BYTE pw_exp;
    NBB_BYTE lsp_ttl;
    NBB_BYTE pw_ttl;
    CV_CFH_HEAD_LABLE2_STRU inner_label;
    OS_MEMSET(&inner_label, 0, sizeof(CV_CFH_HEAD_LABLE2_STRU));

    if ((NULL == mplslabele) || (NULL == mpls_label_i))
    {
        return;
    }

    inner_label.Gal = g_stoam_local[usoamloop].swtich_pw_label_rx1;
    inner_label.Tc = 7;
    inner_label.Ttl = 255;
    inner_label.Zero = 1;
    *mplslabele = *(NBB_ULONG *)(&inner_label);

    inner_label.Gal = g_stoam_local[usoamloop].swtich_pw_label_rx2;
    *mpls_label_i = *(NBB_ULONG *)(&inner_label);
    return;
}

/*****************************************************************************
 函 数 名  : spm_set_acl_for_lb_pw
 功能描述  : acl 规则下发
 输入参数  : IN NBB_USHORT usoamloop  
             IN NBB_BYTE ucodloop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_acl_for_lb_pw(IN NBB_USHORT usoamloop, IN NBB_BYTE ucodloop)
{
    NBB_INT iret = ERROR;
    NBB_ULONG inport_index1 = 0;
    NBB_ULONG inport_index2 = 0;
    NBB_ULONG ulpwlabel1;
    NBB_ULONG ulpwlabel2;
    NBB_ULONG customdata[5] = {0};
    NBB_ULONG ach_32bit = 0;
    NBB_BYTE fpgaport = 0;
    CUSTOMO_DATA opcode_data;
    TARGET_TLV_TYPE_FOUR_BYTE tlv_type;
    OS_MEMSET(&tlv_type, 0, sizeof(TARGET_TLV_TYPE_FOUR_BYTE));
    OS_MEMSET(&opcode_data, 0, sizeof(CUSTOMO_DATA));

    if (0x0 == g_stoam_local[usoamloop].bModuleSwitch)
    {
        fpgaport = 105;
    }
    else
    {
        fpgaport = 106;
    }

    inport_index1 = g_stoam_local[usoamloop].vc_logical_port_index1;
    inport_index2 = g_stoam_local[usoamloop].vc_logical_port_index2;
    spm_get_pw_label_for_swtichvc_acl(usoamloop, &ulpwlabel1, &ulpwlabel2);
    opcode_data.mel = g_stoam_odlocal[ucodloop].pBasicData->mel;
    opcode_data.flag = 0;
    opcode_data.opcode = 0x03;
    opcode_data.version = 0;
    opcode_data.tlv_offset = 4;

    tlv_type.type = 34;
    tlv_type.length = 25;
    tlv_type.id_subtype = 0x3;

    ach_32bit = ACH_32BIT;
    ach_32bit = ((ach_32bit << 16) + g_channel_type);
    customdata[0] = ulpwlabel1;
    customdata[1] = ach_32bit;
    customdata[2] = *((NBB_ULONG *)(&opcode_data));
    customdata[3] = 0;
    customdata[4] = *((NBB_ULONG *)(&tlv_type));
    spm_mip_build_acl_for_lbm_pw(inport_index1, customdata, 0x1, fpgaport);

    customdata[0] = ulpwlabel2;
    spm_mip_build_acl_for_lbm_pw(inport_index2, customdata, 0x2, fpgaport);

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_set_acl_for_lb_lsp
 功能描述  : acl 规则下发
 输入参数  : IN NBB_USHORT usoamloop  
             IN NBB_BYTE ucodloop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_acl_for_lb_lsp(IN NBB_USHORT usoamloop, IN NBB_BYTE ucodloop)
{
    NBB_INT iret = ERROR;
    NBB_ULONG inport_index1 = 0;
    NBB_ULONG inport_index2 = 0;
    NBB_ULONG ulsplabel1;
    NBB_ULONG ullsplabel2;
    NBB_ULONG ulpwlabel;
    NBB_ULONG customdata[6] = {0};
    NBB_ULONG ach_32bit = 0;
    NBB_BYTE fpgaport = 0;
    CUSTOMO_DATA opcode_data;
    TARGET_TLV_TYPE_FOUR_BYTE tlv_type;
    OS_MEMSET(&tlv_type, 0, sizeof(TARGET_TLV_TYPE_FOUR_BYTE));
    OS_MEMSET(&opcode_data, 0, sizeof(CUSTOMO_DATA));

    if (0x0 == g_stoam_local[usoamloop].bModuleSwitch)
    {
        fpgaport = 105;
    }
    else
    {
        fpgaport = 106;
    }

    inport_index1 = g_stoam_local[usoamloop].inport_index1;
    inport_index2 = g_stoam_local[usoamloop].inport_index2;
    spm_set_mpls_and_gal_label_for_acl(usoamloop, &ullsplabel2,
                                       &ulsplabel1, &ulpwlabel);
    opcode_data.mel = g_stoam_odlocal[ucodloop].pBasicData->mel;
    opcode_data.flag = 0;
    opcode_data.opcode = 0x03;
    opcode_data.version = 0;
    opcode_data.tlv_offset = 4;

    tlv_type.type = 34;
    tlv_type.length = 25;
    tlv_type.id_subtype = 0x3;

    ach_32bit = ACH_32BIT;
    ach_32bit = ((ach_32bit << 16) + g_channel_type);
    customdata[0] = ulsplabel1;
    customdata[1] = ulpwlabel;
    customdata[2] = ach_32bit;
    customdata[3] = *((NBB_ULONG *)(&opcode_data));
    customdata[4] = 0;
    customdata[5] = *((NBB_ULONG *)(&tlv_type));
    spm_mip_build_acl_for_lbm_vp(inport_index1, customdata, 0x1, fpgaport);
    customdata[0] = ullsplabel2;
    spm_mip_build_acl_for_lbm_vp(inport_index2, customdata, 0x2, fpgaport);
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_add_tree
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG uloamid     
             SPM_OAM_CB *pstoamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_tree(NBB_ULONG uloamid, SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;

    if (NULL == pstoamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    
    /* 全局配置树插入结点 */
    iret = spm_oam_add_tree(pstoamcfg);

    if (SUCCESS != iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM rcv oam cfg failed", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "", "", "", uloamid, 0, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld,iret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    g_usoam_num++;
    pstoamcfg->IsaddSuccess = OAM_ADD;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_set_wrapping
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_CB *pstoamcfg             
             SPM_OAM_WRAPPING_CB *pstwrapping  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_wrapping(SPM_OAM_CB *pstoamcfg, SPM_OAM_WRAPPING_CB *pstwrapping)
{
#if 0

    NBB_INT iret = ERROR;
    MSRP_OAM_INFO stMsrpOamInfo;
    SPM_PORT_INFO_CB stportinfo;
    OS_MEMSET(&stMsrpOamInfo, 0, sizeof(MSRP_OAM_INFO));
    OS_MEMSET(&stportinfo, 0, sizeof(SPM_PORT_INFO_CB));

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pWrapping)
        || (NULL == pstwrapping))
    {
        goto EXIT_LABEL;
    }
    iret = spm_get_msrp_oam_info(pstoamcfg->oam_id, pstoamcfg->pWrapping->ring_id,
                                 pstoamcfg->pWrapping->direction_select, &stMsrpOamInfo);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\noam id =%d\n"
                   "get oam info from msrp failed ret=%d\n"
                   "ring_id=%d, direction_select=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstoamcfg->oam_id, iret,
                   pstoamcfg->pWrapping->ring_id, pstoamcfg->pWrapping->direction_select);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    printf("out_index=%d\n", stMsrpOamInfo.out_index);

    /* 根据逻辑端口的index获取C3芯片号 */
    iret = spm_get_portid_from_logical_port_index(stMsrpOamInfo.out_index, &stportinfo);

    if (SUCCESS != iret)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM get chip id from logical port index failed"
//                       "rcv oam cfg failed", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "logical port index", "", "",
//                       pstoamcfg->oam_id, stMsrpOamInfo.out_index, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "outport_index=%#x ret=%d\n"
                   "Get chip id failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstoamcfg->oam_id,
                   stMsrpOamInfo.out_index, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    pstwrapping->logical_port_index = stMsrpOamInfo.out_index;
    pstwrapping->ip = stMsrpOamInfo.out_ip;
    pstwrapping->out_portid = stportinfo.port_id;
    pstwrapping->slot_id = stportinfo.slot_id;
    pstoamcfg->logical_port_index = stMsrpOamInfo.out_index;
    pstoamcfg->nhop_ip = stMsrpOamInfo.out_ip;
    pstoamcfg->out_portid = stportinfo.port_id;
    pstoamcfg->slot_id = stportinfo.slot_id;


    pstoamcfg->chip_id = stportinfo.unit_id;
    pstoamcfg->ulTunnelLabel_tx = stMsrpOamInfo.work_out_label;
    pstoamcfg->ulPwLable_tx = 13;
    pstoamcfg->ulTunnelLabel_rx = stMsrpOamInfo.work_in_label;
    pstoamcfg->ulPwLable_rx = 13;
    
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
#endif
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_init_wrapping_pointer
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_WRAPPING_CB **pp_wrapping  
             SPM_OAM_CB *pstoamcfg               
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_init_wrapping_pointer(SPM_OAM_WRAPPING_CB **pp_wrapping,
                                  SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstoamcfg) || (NULL == pp_wrapping) || (NULL == pstoamcfg->pWrapping))
    {
        goto EXIT_LABEL;
    }

    *pp_wrapping = (SPM_OAM_WRAPPING_CB *)NBB_MM_ALLOC(sizeof(SPM_OAM_WRAPPING_CB),
                   NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_WRAPPING_CB);

    if (NULL == *pp_wrapping)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM alloc memory for wrapping cfg failed"
//                       "rcv oam cfg failed", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "", "", "", pstoamcfg->oam_id, 0, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\n memory is not enough for SPM_OAM_WRAPPING_CB oamid = %ld\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstoamcfg->oam_id);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET(*pp_wrapping, 0, sizeof(SPM_OAM_WRAPPING_CB));
    AVLL_INIT_NODE((*pp_wrapping)->spm_wrapping_node);
    (*pp_wrapping)->direction_select = pstoamcfg->pWrapping->direction_select;
    (*pp_wrapping)->ring_id = pstoamcfg->pWrapping->ring_id;
    (*pp_wrapping)->oamid = pstoamcfg->oam_id;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_init_rsvp_pointer
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_RSVP_CB_TX **pp_tx  
             SPM_OAM_RSVP_CB_RX **pp_rx  
             NBB_ULONG uloamid           
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_init_rsvp_pointer(SPM_OAM_RSVP_CB_TX **pp_tx,
                              SPM_OAM_RSVP_CB_RX **pp_rx, NBB_ULONG uloamid)
{
    NBB_INT iret = ERROR;

    if ((NULL == pp_tx) || (NULL == pp_rx))
    {
        goto EXIT_LABEL;
    }

    /* 以rsvp配置为key值的树插入结点 */
    *pp_tx = (SPM_OAM_RSVP_CB_TX *)NBB_MM_ALLOC(sizeof(SPM_OAM_RSVP_CB_TX),
             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_RSVP_CB);

    if (NULL == *pp_tx)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM alloc memory for rsvp tx cfg failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "", "", "", uloamid, 0, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "memory is not enough for SPM_OAM_RSVP_CB_TX\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    *pp_rx = (SPM_OAM_RSVP_CB_RX *)NBB_MM_ALLOC(sizeof(SPM_OAM_RSVP_CB_RX),
             NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_RSVP_CB);

    if (NULL == *pp_rx)
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM alloc memory for rsvp tx cfg failed"
//                       "rcv oam cfg failed\n", ATG_DCI_RC_UNSUCCESSFUL,
//                       "oam id", "", "", "", uloamid, 0, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\noamid = %ld\n"
                   "memory is not enough for SPM_OAM_RSVP_CB_RX\n"
                   "rcv oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_MEMSET((*pp_tx), 0, sizeof(SPM_OAM_RSVP_CB_TX));
    OS_MEMSET((*pp_rx), 0, sizeof(SPM_OAM_RSVP_CB_RX));
    AVLL_INIT_NODE((*pp_tx)->spm_oam_rsvp_node);
    AVLL_INIT_NODE((*pp_rx)->spm_oam_rsvp_node);
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_add_info_to_list_lag
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_CB *pstoamcfg  
             NBB_BYTE portid       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_info_to_list_lag(SPM_OAM_CB *pstoamcfg, NBB_BYTE portid)
{
    NBB_INT iret = ERROR;
    CSF_RELATED_OAMID csf_info;

    if (NULL == pstoamcfg)
    {
        goto EXIT_LABEL;
    }

//    printf("%s\nlag_id=%d,oamid=%d\n",__FUNCTION__,pstoamcfg->lag_id,pstoamcfg->oam_id);
    if (0x0 == pstoamcfg->module)
    {
        csf_info.base_addr = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        csf_info.base_addr = OAM_REG_BASE2_ADDRESS;
    }

    csf_info.lag_id = pstoamcfg->lag_id;
    csf_info.module_index = pstoamcfg->module_index;
    csf_info.oamid = pstoamcfg->oam_id;
    csf_info.port_id = portid;
    csf_info.oamindex = pstoamcfg->oamindex;
    iret = list_add_node(&g_oamid_encsflist, csf_info);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\add info to list failed,oam id=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstoamcfg->oam_id);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}




/*****************************************************************************
 函 数 名  : spm_record_lag_port_for_send_csf
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_PW_CB *pstpwcfg  
             SPM_OAM_CB *pstoamcfg    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_record_lag_port_for_send_csf(SPM_OAM_PW_CB *pstpwcfg, SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstpwcfg) || (NULL == pstoamcfg))
    {
        return;
    }

    //端口属性为lag口时记录lag口成员状态，以触发csf帧
    if (TRUE == pstoamcfg->csf_enable)
    {
        if (ATG_DCI_LAG == pstpwcfg->uni_port_type)
        {
            iret = spm_add_info_to_list_lag(pstoamcfg, pstpwcfg->vc_PortId_section1);

            if (SUCCESS != iret)
            {
                return;
            }
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_delete_lag_port_from_list
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_CB *pstoamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_delete_lag_port_from_list(SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;
    NBB_ULONG uloamid = 0;

    if (NULL == pstoamcfg)
    {
        return;
    }

    uloamid = pstoamcfg->oam_id;

    iret = list_search_node(g_oamid_encsflist, uloamid, pstoamcfg->lag_id);

    if (SUCCESS != iret)
    {
        return;
    }

    iret = list_delete_node(&g_oamid_encsflist, uloamid, pstoamcfg->lag_id);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "delete oamid in oamid&lag list failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_add_wrapping_all
 功能描述  : acl 规则下发
 输入参数  : SPM_OAM_WRAPPING_CB *pstwrappingcfg  
             SPM_OAM_CB *pstoamcfg                
             NBB_BYTE uclocalslot                
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_wrapping_all(SPM_OAM_WRAPPING_CB *pstwrappingcfg,
                             SPM_OAM_CB *pstoamcfg, NBB_BYTE uclocalslot)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstwrappingcfg) || (NULL == pstoamcfg))
    {
        goto EXIT_LABEL;
    }

    iret = spm_set_wrapping(pstoamcfg, pstwrappingcfg);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    if (pstwrappingcfg->slot_id != uclocalslot)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "local slot=%d process slot=%d\n"
                   "the slot to deal with is not match with local slot\n"
                   "ring_id=%d,direction_select=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uclocalslot,
                   pstwrappingcfg->slot_id, pstwrappingcfg->ring_id,
                   pstwrappingcfg->direction_select);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = ERROR;
        goto EXIT_LABEL;
    }


    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_add_vs_all
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月24日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_vs_all(SPM_OAM_VS_CB *pstvscfg,
                       SPM_OAM_CB *pstoamcfg, NBB_BYTE uclocalslot)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstvscfg) || (NULL == pstoamcfg))
    {
        goto EXIT_LABEL;
    }

    /* 存储vs avll tree和oam avll tree的相关配置 */
    iret = spm_init_vscfg_node(pstvscfg, pstoamcfg, uclocalslot);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    
    iret = spm_set_mpls_flag(pstoamcfg);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_add_lsp_all
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月24日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_lsp_all(SPM_OAM_CB *pstoamcfg, SPM_OAM_RSVP_CB_RX *prsvpcfgrx,
                        SPM_OAM_RSVP_CB_TX *prsvpcfgtx, NBB_BYTE uclocalslot)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstoamcfg) || (NULL == prsvpcfgrx) || (NULL == prsvpcfgtx)
        || (NULL == pstoamcfg->pBasiCfg))
    {
        goto EXIT_LABEL;
    }

    if (0x01 ==  pstoamcfg->pBasiCfg->vp_attrib)
    {
        /* 只有源节点才根据配置信息来查找发送标签，MIP节点没有txlsp */
        iret = spm_set_mep_rsvp(pstoamcfg->pRsvplspCfg, pstoamcfg,
                                prsvpcfgrx, prsvpcfgtx, uclocalslot);

        if (SUCCESS != iret)
        {
            goto EXIT_LABEL;
        }
    }
    else
    {
        iret = spm_set_mip_rsvp(pstoamcfg->pRsvplspCfg, pstoamcfg,
                                prsvpcfgrx, prsvpcfgtx);

        if (SUCCESS != iret)
        {
            goto EXIT_LABEL;
        }
    }


    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_add_pw_all
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月24日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_pw_all(SPM_OAM_CB *pstoamcfg, SPM_OAM_PW_CB *pstpwcfg,
                       NBB_BYTE uclocalslot, NBB_ULONG uloamid)
{
    NBB_INT iret = ERROR;

    if ((NULL == pstoamcfg) || (NULL == pstoamcfg->pBasiCfg)
        || (NULL == pstpwcfg))
    {
        goto EXIT_LABEL;
    }

    if (TRUE == pstoamcfg->pBasiCfg->vp_attrib)
    {
        /* MEP */
        iret = spm_set_mep_pw(pstoamcfg->pPwCfg, pstpwcfg, pstoamcfg, uclocalslot, uloamid);

        if (SUCCESS != iret)
        {
            goto EXIT_LABEL;
        }
    }
    else /*MIP,多段mspw支持FDI帧*/
    {
        iret = spm_set_mip_pw(pstoamcfg->pPwCfg, pstpwcfg, pstoamcfg, uloamid);

        if (SUCCESS != iret)
        {
            goto EXIT_LABEL;
        }
    }


    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_decide_lck_or_tst
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月24日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_decide_lck_or_tst(NBB_BYTE ucodloop, NBB_BYTE *lckEnable)
{
    if (ucodloop >= OAM_MAX_NUM_OD)
    {
        return;
    }

    if (NULL != g_stoam_odlocal[ucodloop].pTestFrame)
    {
        if (TRUE == g_stoam_odlocal[ucodloop].pTestFrame->byte1.send_enable)
        {
            if (OAM_TEST_OFFLINE ==
                g_stoam_odlocal[ucodloop].pTestFrame->byte1.send_method)
            {
                *lckEnable = 1;
                return;
            }
            else
            {
                *lckEnable = 0;
                return;
            }
        }
        else
        {
            if (NULL != g_stoam_odlocal[ucodloop].pLckFrame)
            {
                *lckEnable = g_stoam_odlocal[ucodloop].pLckFrame->lck_frame_send_enable;
                return;
            }
            else
            {
                *lckEnable = 0;
                return;
            }
        }
    }
    else if (NULL != g_stoam_odlocal[ucodloop].pLckFrame)
    {
        *lckEnable = g_stoam_odlocal[ucodloop].pLckFrame->lck_frame_send_enable;
        return;
    }
}

/*****************************************************************************
 函 数 名  : spm_delete_wrong_cfg
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月24日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_delete_wrong_cfg(SPM_OAM_CB **pstoamcfg)
{
    NBB_USHORT usloop = 0;

    if ((NULL == pstoamcfg) || (NULL == (*pstoamcfg)))
    {
        return ERROR;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d oamid=%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__, (*pstoamcfg)->oam_id);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    for (usloop = 0 ; usloop < OAM_MAX_NUM_ACTIVE_SPU - 1; usloop++)
    {
        if (0x0 == g_stoam_local[usloop].ulActiveOamkey)
        {
            continue;
        }

        if (((*pstoamcfg)->oam_id) == g_stoam_local[usloop].ulActiveOamkey)
        {
            NBB_MEMSET(&g_stoam_local[usloop], 0, sizeof(OAM_ACTIVE_LOCAL_STRU));
            break;
        }
    }

    spm_recycle_oam_index((*pstoamcfg)->oamindex);
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_add_tp_oam_cfg
 功能描述  :
    由保护模块调用，标识本条OAM是否绑定了保护，FLAG为1表示绑定，FLAG为0未绑定
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月24日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_tp_oam_cfg(NBB_ULONG uloamid, NBB_BYTE flag)
{
    SPM_OAM_CB *poamcfg = NULL;
    NBB_USHORT usloop = 0;
    NBB_INT iret = ERROR;
    poamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &uloamid);

    if (NULL == poamcfg)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "cannot find the oamid in g_oamavltreebyoamid\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    usloop = poamcfg->oamindex;
    g_stoam_local[usloop].bIsRelateProt = flag;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 Prototype    : spm_get_label_action
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_get_label_action(SPM_OAM_CB *poamcfg, NBB_BYTE *labelaction)
{
    NBB_INT iret = ERROR;
    NBB_ULONG uloamid = 0;
    CRRXLSP_KEY key;
    OS_MEMSET(&key, 0, sizeof(CRRXLSP_KEY));

    if ((NULL == labelaction) || (NULL == poamcfg))
    {
        goto EXIT_LABEL;
    }

    uloamid = poamcfg->oam_id;

    if (NULL == poamcfg->pRsvplspCfg)
    {
        goto EXIT_LABEL;
    }

    key.egress = poamcfg->pRsvplspCfg->rx_egress;
    key.ingress = poamcfg->pRsvplspCfg->rx_ingress;
    key.tunnelid = poamcfg->pRsvplspCfg->rx_tunnel_id;
    key.lspid = poamcfg->pRsvplspCfg->rx_lsp_id;
#if 0
    iret = spm_l3_get_rxlabelaction(&key, labelaction);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }
#endif
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : spm_oam_get_local_oamindex_for_wrapping
 功能描述  : acl 规则下发
 输入参数  : IN NBB_ULONG uloamid                              
             OUTNBB_USHORT *usLocalOamLoop NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_get_local_oamindex_for_wrapping(IN NBB_ULONG uloamid, OUT
        NBB_USHORT *usLocalOamLoop NBB_CCXT_T NBB_CXT)
{
    NBB_INT iret = ERROR;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_USHORT usloop = 0;

    /* 判断入参 */
    if ((uloamid == 0) || (uloamid > OAM_ID_MAX))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "usOamID is illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    pstoamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &uloamid);

    if (NULL == pstoamcfg)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "OamID can not be found in g_oamavltreebyoamid\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, uloamid);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    *usLocalOamLoop = pstoamcfg->oamindex;

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

#ifdef SPU

/*****************************************************************************
 函 数 名  : spm_write_table_for_active_lm
 功能描述  : acl 规则下发
 输入参数  : NBB_INT unit         
             NBB_ULONG label      
             NBB_ULONG vpVcOamId  
             NBB_USHORT iloop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_write_table_for_active_lm(NBB_INT unit,
                                      NBB_ULONG label, NBB_ULONG vpVcOamId, NBB_USHORT iloop)
{
    NBB_INT iret = ERROR;
#if 0
    switch (g_stoam_local[iloop].oam_type)
    {
        case VS_OAM:
            iret = ApiC3SetIngVsOamId(unit, label, vpVcOamId);

            if (SUCCESS != iret)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\nunit=%d,label=%d,vpVcOamId=%d\n"
                           "call ApiC3SetIngVsOamId() error\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                           unit, label, vpVcOamId);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            break;

        case VP_OAM:
        case VC_OAM:
            iret = ApiC3SetMplsVpvcOamId(unit, label, vpVcOamId);

            if (SUCCESS != iret)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\nunit=%d,label=%d,vpVcOamId=%d\n"
                           "call ApiC3SetMplsVpvcOamId() error\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                           unit, label, vpVcOamId);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            break;

        default:
            goto EXIT_LABEL;
    }
#endif
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_tst_offline_lock_flow
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT usoamloop  
             NBB_BYTE ucodloop     
             NBB_BYTE enb          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_tst_offline_lock_flow(NBB_USHORT usoamloop, NBB_BYTE ucodloop,
                                  NBB_BYTE enb)
{
    NBB_INT iret = ERROR;

    if (usoamloop > OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }

    if (ucodloop > OAM_MAX_NUM_OD)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\nOamLoop=%d,OdLoop=%d,enb=%d\n",
               ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
               usoamloop, ucodloop, enb);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    switch (g_stoam_local[usoamloop].oam_type)
    {

#if 0
        case VS_OAM:
            iret = ApiC3SetPortVsLck(g_stoam_local[usoamloop].chip_id,
                                     g_stoam_local[usoamloop].vs_Portid,
                                     enb);

            if (SUCCESS != iret)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                           "call ApiC3SetPortVsLck() failed\n"
                           "chip_id=%d,portid=%d,iret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                           g_stoam_local[usoamloop].chip_id,
                           g_stoam_local[usoamloop].vs_Portid, iret);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            break;

        case VP_OAM:
            iret = ApiC3SetLspVpLck(g_stoam_local[usoamloop].chip_id,
                                    g_stoam_local[usoamloop].vp_Lspid,
                                    enb);

            if (SUCCESS != iret)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                           "call ApiC3SetLspVpLck() failed\n"
                           "chip_id=%d,lspid=%d,iret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                           g_stoam_local[usoamloop].chip_id,
                           g_stoam_local[usoamloop].vp_Lspid, iret);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            break;

        case VC_OAM:
            iret = ApiC3SetVpEgrVcLck(g_stoam_local[usoamloop].chip_id,
                                      g_stoam_local[usoamloop].vc_no_section1,
                                      enb);

            if (SUCCESS != iret)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                           "call ApiC3SetVpEgrVcLck() failed\n"
                           "chip_id=%d,vpid=%d,iret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                           g_stoam_local[usoamloop].chip_id,
                           g_stoam_local[usoamloop].vc_no_section1, iret);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            break;
#endif
        default:
            iret = SUCCESS;
            break;
    }

EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 Prototype    : spm_pw_oam_send_packedge
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_pw_oam_send_packedge(SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;
    NBB_USHORT usloop = 0;
    NBB_USHORT module_index = 0;
    NBB_ULONG baseaddr = 0;

    if (NULL == pstoamcfg)
    {
        return iret;
    }

    usloop = pstoamcfg->oamindex;
    module_index = g_stoam_local[usloop].module_index;

    if (NULL != pstoamcfg->pCvcfg)
    {
        spm_oam_tx_vpws_mac_fpga(usloop, ACTIVE_OAM);
    }

    if (NULL != pstoamcfg->pCsfcfg)
    {
        if (0x1 == g_stoam_local[usloop].vp_attrib)
        {
            /* 判断csf是否使能 */
            spm_oam_csf(usloop);
        }
    }

    if (NULL != pstoamcfg->pApscfg)
    {
        spm_oam_tx_vpws_mac_fpga(usloop, ACTIVE_OAM);
        
        /* APS成帧高16位和低16位 */
        /* 判断所在模块 */
        spm_get_moudle_base_addr(usloop, &baseaddr);
    }

    if (NULL != pstoamcfg->pFdicfg)
    {
        if (0 == g_stoam_local[usloop].vp_attrib)
        {
            spm_oam_tx_vpws_mac_fpga(usloop, FDI_OAM);
            g_stoam_local[usloop].bHasFdi = TRUE;

            if (TRUE == pstoamcfg->pFdicfg->fdi_enable)
            {
                (NBB_VOID)spm_tmc_send_fdi_by_loc(usloop);
            }
        }
        else
        {
            if (TRUE == pstoamcfg->pFdicfg->fdi_enable)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n vp attribute is MEP\n"
                           "send fdi failed\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }
        }
    }
}

#endif

/*****************************************************************************
 函 数 名  : spm_add_od_num
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT usoamloop  
             NBB_BYTE ucodloop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_od_num(NBB_USHORT usoamloop, NBB_BYTE ucodloop)
{
//    NBB_BYTE LoopTemp = 0;
    NBB_INT iret = ERROR;

    //判断对应主动TP所在FPGA模块
    if (FALSE == g_stoam_local[usoamloop].bModuleSwitch)
    {
        if (g_odoam_cnt1 > OAM_MAX_NUM_OD)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\nod oam is full in moudel 1\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
        else
        {
            g_odoam_cnt1++;
            g_ucod_oamnum++;
            g_stoam_odlocal[ucodloop].ucOdLoop = g_odoam_cnt1;
            iret = SUCCESS;
            goto EXIT_LABEL;
        }
    }
    else
    {
        if (g_odoam_cnt2 > OAM_MAX_NUM_OD)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\nod oam is full in moudel 2\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
        }
        else
        {
            g_odoam_cnt2++;
            g_ucod_oamnum++;
            g_stoam_odlocal[ucodloop].ucOdLoop = g_odoam_cnt2;
            iret = SUCCESS;
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_delete_od_num
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT usoamloop  
             NBB_BYTE ucodloop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_delete_od_num(NBB_USHORT usoamloop, NBB_BYTE ucodloop)
{
    NBB_INT iret = ERROR;
    NBB_BYTE *podoamcnt = NULL;
    
    //判断对应主动TP所在FPGA模块

    if (FALSE == g_stoam_odlocal[ucodloop].bModuleSwitch)
    {
        podoamcnt = &g_odoam_cnt1;
    }
    else
    {
        podoamcnt = &g_odoam_cnt2;
    }

    if (*podoamcnt <= 0x0)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nod oam is empty in moudel %d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                   g_stoam_odlocal[ucodloop].bModuleSwitch);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    else
    {
        (*podoamcnt)--;
        g_ucod_oamnum--;
        iret = SUCCESS;
        goto EXIT_LABEL;
    }

EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 Prototype    : spm_mip_lb_tmp_mac_fpga
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_VOID spm_mip_lb_tmp_mac_fpga(IN NBB_USHORT usoamloop)
{
    CV_CFH_PORTID_STRU cvdtype;
    NBB_USHORT usdtypedata;
    NBB_USHORT uslspid2;
    NBB_USHORT usvanid = 0;
    NBB_INT iret = ERROR;
    NBB_ULONG baseaddr;
    NBB_ULONG ullocaltunnelable;
    NBB_ULONG ullocalpwlable;
    NBB_ULONG ulgallable = 0;
    NBB_USHORT uslocalindex;
    NBB_BYTE ucodloop = 0;
    NBB_BYTE ucodindex = 0;
    OS_MEMSET((&cvdtype), 0, sizeof(CV_CFH_PORTID_STRU));

    uslspid2 = g_stoam_local[usoamloop].lspid2 & 0xFFFF;
    
    /* Dtype赋值 */
    cvdtype.chip_id = g_stoam_local[usoamloop].chip_id;
    cvdtype.DType = 1; /* 代表是vp层oam */
    usdtypedata = *(NBB_USHORT *)(&cvdtype);
    
    /*判断该条目存在哪个模块*/
    spm_get_moudle_base_addr(usoamloop, &baseaddr);
    uslocalindex = g_stoam_local[usoamloop].module_index;

    iret = spm_odindex_by_activeindex(usoamloop, &ucodloop);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    ucodindex = g_stoam_odlocal[ucodloop].ucOdLoop - 1;

    /* 填写帧头MAC内容 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x1,
                    0x0000);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x2,
                    usvanid);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x3,
                    uslspid2);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x4,
                    usdtypedata);
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x5,
                    0x0000);  /* IPADDRESS 需修改 */
    spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x6,
                    0x0000);  /* IPADDRESS 需修改 */
EXIT_LABEL:
    return;
}


/*****************************************************************************
 Prototype    : spm_mip_lb_tmc_mac_fpga
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_VOID spm_mip_lb_tmc_mac_fpga(IN NBB_USHORT usoamloop)
{
    CV_CFG_VPWS_DTYPE_STRU stvpwsdtype;
    NBB_USHORT usvpwsdtype;
    NBB_INT iret = ERROR;
    NBB_USHORT usvanid = 0;
    NBB_ULONG baseaddr;
    NBB_ULONG ulgallable = 0;
    NBB_USHORT uslocalindex = 0; //模块本地索引
    NBB_BYTE uclocalslot;
    NBB_BYTE ucodloop = 0;
    NBB_BYTE ucodindex = 0;
    OS_MEMSET((&stvpwsdtype), 0, sizeof(CV_CFG_VPWS_DTYPE_STRU));

    if (usoamloop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        goto EXIT_LABEL;
    }

    //判断该条目存在哪个模块
    spm_get_moudle_base_addr(usoamloop, &baseaddr);
    uslocalindex = g_stoam_local[usoamloop].module_index;
    iret = spm_odindex_by_activeindex(usoamloop, &ucodloop);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

//    spm_get_od_gal(ucodloop);
//    ulgallable = g_stoam_odlocal[ucodloop].gal_label_fpga;
    ucodindex = g_stoam_odlocal[ucodloop].ucOdLoop - 1;

    /* 判断vpn的类型是VPWS/VPLS */
    /* 若是VPWS */
    if (VPWS_TYPE == g_stoam_local[usoamloop].vpn_type)
    {
        /* Dtype字段赋值 */
        stvpwsdtype.PktT = 0; /*  代表vc层vpws的oam帧 */
        stvpwsdtype.Dtype = 4; /* 代表vpws vc层oam帧 */
        stvpwsdtype.Reserved = 0;
        stvpwsdtype.chip_id = g_stoam_local[usoamloop].chip_id;
        usvpwsdtype = *(NBB_USHORT *)(&stvpwsdtype);

        /* 迭代出pwlable值 */
        /* 条目N的发送标签使能,填入tunnel和pw标签 */
        /* 填写帧头MAC内容 */
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x1,
                        0x0000);
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x2,
                        usvanid);
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x3,
                        g_stoam_local[usoamloop].vc_no_section1);
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x4,
                        usvpwsdtype);
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x5,
                        0x0000);
        spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucodindex * 8 + 0x6,
                        0x0000);
    }
    else
    {
//        NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
//                       "TPOAM vpn type is vpls", ATG_DCI_RC_UNSUCCESSFUL,
//                       "", "", "", "", 0, 0, 0, 0));
        OS_SPRINTF(g_uc_message, "%s %s,%d\n vpn type is vpls,EXIT! vpn_type=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, g_stoam_local[usoamloop].vpn_type);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

EXIT_LABEL:
    return;
}



/*****************************************************************************
 函 数 名  : spm_tmp_fdi_get_related_tms_oam_tx
 功能描述  : acl 规则下发
 输入参数  : OAM_RSVP_LSP_KEY *pstkey_tx  
             NBB_USHORT *poamindex1       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_tmp_fdi_get_related_tms_oam_tx(OAM_RSVP_LSP_KEY *pstkey_tx,
        NBB_USHORT *poamindex1)
{
    NBB_ULONG logical_port_index = 0;
    NBB_USHORT oamindex = 0;
    SPM_OAM_RSVP_CB_TX *prsvptxcfg = NULL;
    NBB_INT iret = ERROR;

    if ((NULL == pstkey_tx) || (NULL == poamindex1))
    {
        goto EXIT_LABEL;
    }

    prsvptxcfg = (SPM_OAM_RSVP_CB_TX *)AVLL_FIND(v_spm_shared->g_oamavltreebyrsvptx,
                 pstkey_tx);

    if ((NULL == prsvptxcfg))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nRX:"
                   "ingress=%x egress=%x tunnelid=%x lspid=%x\n"
                   "cannot find cfg in g_oamavltreebyrsvptx\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                   pstkey_tx->Ingress, pstkey_tx->Egress,
                   pstkey_tx->TunnelId, pstkey_tx->LspId);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    logical_port_index = prsvptxcfg->inport_index;
    iret = spm_find_tms_index_form_logical_port_index(logical_port_index,
            &oamindex);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    *poamindex1 = oamindex;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_find_tms_index_form_logical_port_index
 功能描述  : acl 规则下发
 输入参数  : NBB_ULONG port_index   
             NBB_USHORT *poamindex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_find_tms_index_form_logical_port_index(NBB_ULONG port_index,
        NBB_USHORT *poamindex)
{
    NBB_ULONG oamid = 0;
    NBB_INT iret = ERROR;
    NBB_BYTE uclocalslot = 0;
    SPM_OAM_VS_CB *pvscfg = NULL;

    if (NULL == poamindex)
    {
        goto EXIT_LABEL;
    }

    pvscfg = (SPM_OAM_VS_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyindex, &port_index);

    if (NULL == pvscfg)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n do not find oam cfg "
                   "in g_oamavltreebyindex,portindex=%#x\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, port_index);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    oamid = pvscfg->oamid;
    iret = spm_hardw_getslot(&uclocalslot);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n get local slot error\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }


    *poamindex = pvscfg->oamindex;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 Prototype    : spm_tmp_monitor_tms_loc
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_tmp_monitor_tms_loc(int oamloop)
{
    NBB_USHORT moduleindex = 0;
    static NBB_BYTE s_localarmlast[OAM_MAX_NUM_ACTIVE_SPU] = {0};
    NBB_BYTE localarm = 0;
    NBB_ULONG baseaddr = 0;
    NBB_ULONG baseaddr2 = 0;
    NBB_USHORT loc_module_index = 0;
    NBB_USHORT loc_loop = 0;
    NBB_USHORT tempdata = 0;

    if (oamloop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        return 0;
    }

    if (TRUE == g_stoam_local[oamloop].fdi_enable)
    {
        loc_loop = g_stoam_local[oamloop].fdi_loop;
        loc_module_index = g_stoam_local[loc_loop].module_index;
        moduleindex = g_stoam_local[oamloop].module_index;
        spm_get_moudle_base_addr(oamloop, &baseaddr);
        spm_get_moudle_base_addr(loc_loop, &baseaddr2);
        localarm = spm_oam_16read(baseaddr2, ALARM_ADDR + loc_module_index);
        localarm = ((localarm >> 4) & 0x1);

        if (s_localarmlast[oamloop] != localarm)
        {
            tempdata = spm_oam_16read(baseaddr, FDI_TX_CFG_OFFSET + moduleindex);

            if (TRUE == localarm)
            {
                tempdata &= 0xfbff;
            }
            else
            {
                tempdata |= 0x0400;
            }

            spm_oam_16write(baseaddr, FDI_TX_CFG_OFFSET + moduleindex,
                            tempdata);
            s_localarmlast[oamloop] = localarm;
        }
    }

    return 0;
}



/*****************************************************************************
 Prototype    : spm_tmp_send_fdi_by_loc_task
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_VOID spm_tmp_send_fdi_by_loc_task(NBB_USHORT oamloop)
{
    OAM_RSVP_LSP_KEY stkey_tx;
    NBB_USHORT oamidex = 0;
    NBB_INT iret = ERROR;
    OS_MEMSET(&stkey_tx, 0, sizeof(OAM_RSVP_LSP_KEY));

    if (BmuWdStart(g_stoam_local[oamloop].fdi_monitor, BmuWd5MSTickBase(200),
                   (int(*)(int))spm_tmp_send_fdi_by_loc_task, oamloop) == ERROR)         /*不能用printf*/
    {
        printf("spm_tmp_send_fdi_by_loc_task BmuWdStart FAIL  !\n");
    }

    stkey_tx.Egress = g_stoam_local[oamloop].stRsvpLsp.tx_egress;
    stkey_tx.Ingress = g_stoam_local[oamloop].stRsvpLsp.tx_ingress;
    stkey_tx.TunnelId = g_stoam_local[oamloop].stRsvpLsp.tx_tunnel_id;
    stkey_tx.LspId = g_stoam_local[oamloop].stRsvpLsp.tx_lsp_id;
    iret = spm_tmp_fdi_get_related_tms_oam_tx(&stkey_tx, &oamidex);

    if (SUCCESS != iret)
    {
        return;
    }

    g_stoam_local[oamloop].fdi_loop = oamidex;
    spm_tmp_monitor_tms_loc(oamloop);
    return;
}



/*****************************************************************************
 Prototype    : spm_tmp_send_fdi_by_loc
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_VOID spm_tmp_send_fdi_by_loc(NBB_USHORT oamloop)
{
    if (oamloop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        return;
    }

    g_stoam_local[oamloop].fdi_monitor = BmuWdCreate();

    if (g_stoam_local[oamloop].fdi_monitor == NULL)
    {
        printf("BmuWdCreate FAIL because memory is insufficient !\n");
    }

    if (BmuWdStart(g_stoam_local[oamloop].fdi_monitor, BmuWd5MSTickBase(200),
                   (int(*)(int))spm_tmp_send_fdi_by_loc_task, oamloop) == ERROR)         /*不能用printf*/
    {
        printf("spm_tmp_send_fdi_by_loc_task BmuWdStart FAIL  !\n");
    }

    return;
}


/*****************************************************************************
 Prototype    : spm_tmc_send_fdi_by_loc
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_VOID spm_tmc_send_fdi_by_loc(NBB_USHORT oamloop)
{
    if (oamloop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        return;
    }

    g_stoam_local[oamloop].fdi_monitor = BmuWdCreate();

    if (g_stoam_local[oamloop].fdi_monitor == NULL)
    {
        printf("BmuWdCreate FAIL because memory is insufficient !\n");
    }

    if (BmuWdStart(g_stoam_local[oamloop].fdi_monitor, BmuWd5MSTickBase(200),
                   (int(*)(int))spm_tmc_send_fdi_by_loc_task, oamloop) == ERROR)         /*不能用printf*/
    {
        printf("spm_tmc_send_fdi_by_loc_task BmuWdStart FAIL  !\n");
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_tmc_fdi_get_related_tmp_oam
 功能描述  : acl 规则下发
 输入参数  : NBB_USHORT oamloop    
             NBB_USHORT *lsp_loop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_tmc_fdi_get_related_tmp_oam(NBB_USHORT oamloop, NBB_USHORT *lsp_loop)
{
#if 0
    SPM_OAM_RSVP_CB_TX *pRsvpLspCfg = NULL;
    ATG_DCI_VC_KEY stVcKey;
    SPM_SWITCH_VC_INFO stSwitchVckey;
    CRTXLSP_KEY stTxKey;
    NBB_INT iret = ERROR;
    OS_MEMSET(&stSwitchVckey, 0, sizeof(SPM_SWITCH_VC_INFO));
    OS_MEMSET(&stVcKey, 0, sizeof(stVcKey));

    if ((oamloop >= OAM_MAX_NUM_ACTIVE_SPU) || (NULL == lsp_loop))
    {
        goto EXIT_LABEL;
    }

    stVcKey.peer_ip = g_stoam_local[oamloop].stPw.peer_ip;
    stVcKey.vc_id = g_stoam_local[oamloop].stPw.vc_id;
    stVcKey.vc_type = g_stoam_local[oamloop].stPw.vc_type;

    /* 从vckey获取lsp的key值 */
#if 0
    iret = spm_get_info_from_switch_vc_key(&stVcKey, &stSwitchVckey);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    NBB_MEMCPY(&stTxKey, &stSwitchVckey.stCrLspKey1, 4 * sizeof(NBB_ULONG));
    pRsvpLspCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyrsvptx, &stTxKey);

    if (NULL == pRsvpLspCfg)
    {
        goto EXIT_LABEL;
    }

    *lsp_loop = pRsvpLspCfg->oamindex;
#endif
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
#endif 
    return SUCCESS;
}



/*****************************************************************************
 Prototype    : spm_tmc_send_fdi_by_loc_task
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_VOID spm_tmc_send_fdi_by_loc_task(NBB_USHORT oamloop)
{
    NBB_INT iret = ERROR;
    NBB_USHORT lsp_loop = 0;

    if (BmuWdStart(g_stoam_local[oamloop].fdi_monitor, BmuWd5MSTickBase(200),
                   (int(*)(int))spm_tmc_send_fdi_by_loc_task, oamloop) == ERROR)         /*不能用printf*/
    {
        printf("spm_tmp_send_fdi_by_loc_task BmuWdStart FAIL  !\n");
    }

    iret = spm_tmc_fdi_get_related_tmp_oam(oamloop, &lsp_loop);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    g_stoam_local[oamloop].fdi_loop = lsp_loop;
    spm_tmc_monitor_tmp_loc(oamloop);
EXIT_LABEL:
    return;
}


/*****************************************************************************
 Prototype    : spm_tmc_monitor_tmp_loc
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_tmc_monitor_tmp_loc(int oamloop)
{
    NBB_USHORT moduleindex = 0;
    NBB_BYTE localarm = 0;
    static NBB_BYTE s_localarmlast[OAM_MAX_NUM_ACTIVE_SPU] = {0};
    NBB_ULONG baseaddr = 0;
    NBB_ULONG baseaddr2 = 0;
    NBB_USHORT vp_loop = 0;
    NBB_USHORT vp_module_index = 0;
    NBB_USHORT tempdata = 0;

    if (oamloop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        return 0;
    }

    if (TRUE == g_stoam_local[oamloop].fdi_enable)
    {
        vp_loop = g_stoam_local[oamloop].fdi_loop;
        vp_module_index = g_stoam_local[vp_loop].module_index;
        moduleindex = g_stoam_local[oamloop].module_index;
        spm_get_moudle_base_addr(oamloop, &baseaddr);
        spm_get_moudle_base_addr(vp_loop, &baseaddr2);
        localarm = spm_oam_16read(baseaddr2, ALARM_ADDR + vp_module_index);
        localarm = ((localarm >> 4) & 0x1);

        if (s_localarmlast[oamloop] != localarm)
        {
            tempdata = spm_oam_16read(baseaddr, FDI_TX_CFG_OFFSET + moduleindex);

            if (TRUE == localarm)
            {
                tempdata &= 0xfbff;
            }
            else
            {
                tempdata |= 0x0400;
            }

            spm_oam_16write(baseaddr, FDI_TX_CFG_OFFSET + moduleindex,
                            tempdata);
            s_localarmlast[oamloop] = localarm;
        }
    }

    return 0;
}




/*****************************************************************************
 函 数 名  : spm_get_vc_key
 功能描述  : acl 规则下发
 输入参数  : OAM_VC_KEY *oamvckey  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_get_vc_key(OAM_VC_KEY *oamvckey)
{
    SPM_OAM_PW_CB *pstpwcfg = NULL;
    NBB_INT iret = ERROR;
    NBB_BYTE uclocalslot = 0;

    iret = spm_hardw_getslot(&uclocalslot);

    if (oamvckey == NULL)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n the oamvckey is NULL\n"
                   "rev oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    for (pstpwcfg = (SPM_OAM_PW_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebypw);
         pstpwcfg != NULL;
         pstpwcfg = (SPM_OAM_PW_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebypw,
                   pstpwcfg->spm_oam_pw_node))
    {
        if ((pstpwcfg->stpwcfg.vc_id != 0) && (pstpwcfg->stpwcfg.vc_type != 0) && (pstpwcfg->stpwcfg.peer_ip != 0))
        {
            oamvckey->VcId = pstpwcfg->stpwcfg.vc_id;
            oamvckey->VcType = pstpwcfg->stpwcfg.vc_type;
            oamvckey->PeerIp = pstpwcfg->stpwcfg.peer_ip;
            iret = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 Prototype    : spm_record_sd_oamloop
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_record_sd_oamloop(NBB_USHORT oamloop)
{
    NBB_INT iret = ERROR;
    iret = sd_list_searchnode(g_oam_sdlist, oamloop);

    if (SUCCESS == iret)//已存在，不添加
    {
        goto EXIT_LABEL;
    }

    iret = sd_list_addnode(&g_oam_sdlist, oamloop,SD_LIST);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nadd node to SD list failed,oamloop=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamloop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 Prototype    : spm_delete_record_sd_oamloop
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_delete_record_sd_oamloop(NBB_USHORT oamloop)
{
    NBB_INT iret = ERROR;
    iret = sd_list_searchnode(g_oam_sdlist, oamloop);

    if (SUCCESS != iret)//不存在，不删除
    {
        goto EXIT_LABEL;
    }

    /* 删除SD标志位  */
    g_stoam_local[oamloop].bSdAlarm = FALSE;

#ifdef SPU
    (NBB_VOID)fhdrv_port_set_ing_port_sd_en(g_stoam_local[oamloop].chip_id, g_stoam_local[oamloop].vs_Portid, FALSE);
#endif
    
    iret = sd_list_deletenode(&g_oam_sdlist, oamloop,SD_LIST);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\ndelete node to SD list failed,oamloop=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamloop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 Prototype    : spm_record_wrapping_oamloop
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_record_wrapping_oamloop(NBB_USHORT oamloop)
{
    NBB_INT iret = ERROR;
    iret = sd_list_searchnode(g_oam_wrappinglist, oamloop);

    if (SUCCESS == iret)//已存在，不添加
    {
        goto EXIT_LABEL;
    }

    iret = sd_list_addnode(&g_oam_wrappinglist, oamloop,WRAPPING_LIST);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nadd node to wrapping list failed,oamloop=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamloop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 Prototype    : spm_delete_record_wrapping_oamloop
 Description  : 
 Input        : NBB_USHORT oamloop
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_delete_record_wrapping_oamloop(NBB_USHORT oamloop)
{
    NBB_INT iret = ERROR;
    iret = sd_list_searchnode(g_oam_wrappinglist, oamloop);

    if (SUCCESS != iret)//不存在，不删除
    {
        goto EXIT_LABEL;
    }

    iret = sd_list_deletenode(&g_oam_wrappinglist, oamloop,WRAPPING_LIST);

    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\ndelete node to wrapping list failed,oamloop=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, oamloop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 Prototype    : spm_lsp_oam_send_packedge
 Description  : 
 Input        : SPM_OAM_CB *poamcfg
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_lsp_oam_send_packedge(SPM_OAM_CB *pstoamcfg)
{
    NBB_INT iret = ERROR;
    NBB_USHORT usloop = 0;
    NBB_USHORT module_index = 0;
    NBB_ULONG baseaddr = 0;

    if (NULL == pstoamcfg)
    {
        return iret;
    }

    usloop = pstoamcfg->oamindex;
    module_index = g_stoam_local[usloop].module_index;

    /* 判断cv帧是否使能 */
    if (NULL != pstoamcfg->pCvcfg)
    {
        spm_oam_tx_tmp_mac_fpga(usloop, ACTIVE_OAM);
    }

    if (NULL != pstoamcfg->pApscfg)
    {
        spm_oam_tx_tmp_mac_fpga(usloop, ACTIVE_OAM);

        /* APS成帧高16位和低16位 */
        /* 判断所在模块 */
        spm_get_moudle_base_addr(usloop, &baseaddr);
    }

    if (NULL != pstoamcfg->pFdicfg)
    {
        if (0 == g_stoam_local[usloop].vp_attrib)
        {
            spm_oam_tx_tmp_mac_fpga(usloop, FDI_OAM);
            g_stoam_local[usloop].bHasFdi = TRUE;

            if (TRUE == pstoamcfg->pFdicfg->fdi_enable)
            {
                spm_tmp_send_fdi_by_loc(usloop);
            }
        }
        else
        {
            if (TRUE == pstoamcfg->pFdicfg->fdi_enable)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n vp attribute is MEP\n"
                           "send fdi failed\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }
        }
    }

    iret = SUCCESS;
    return iret;
}



/*****************************************************************************
 Prototype    : spm_put_cfg_into_array
 Description  : 将配置树里的配置写入本地数组
 Input        : NBB_VOID *poamcfg
 Output       : None
 Return Value : NBB_INT
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/4
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_put_cfg_into_array(SPM_OAM_CB *pstoamcfg)
{
    NBB_USHORT iloop = 0;

    if (NULL == pstoamcfg)
    {
        return SUCCESS;
    }

    /* 将树的配置放入本地数组，方便发送报文 */
    iloop = pstoamcfg->oamindex;
    g_stoam_local[iloop].global_index = pstoamcfg->oamindex;

    g_stoam_local[iloop].module_index = pstoamcfg->module_index;
    g_stoam_local[iloop].aps_enable = pstoamcfg->aps_enable;
    g_stoam_local[iloop].csf_enable = pstoamcfg->csf_enable;
    g_stoam_local[iloop].cv_cycle = pstoamcfg->cv_cycle;
    g_stoam_local[iloop].cv_enable = pstoamcfg->cv_enable;
    g_stoam_local[iloop].fdi_enable = pstoamcfg->fdi_enable;

    g_stoam_local[iloop].oam_type = pstoamcfg->oamtype;
    g_stoam_local[iloop].chip_id = pstoamcfg->chip_id;
    g_stoam_local[iloop].vp_attrib = pstoamcfg->pBasiCfg->vp_attrib;
    NBB_MEMCPY(g_stoam_local[iloop].meg_icc, pstoamcfg->pBasiCfg->meg_icc, 6 * sizeof(NBB_BYTE));
    NBB_MEMCPY(g_stoam_local[iloop].meg_umc, pstoamcfg->pBasiCfg->meg_umc, 6 * sizeof(NBB_BYTE));
    g_stoam_local[iloop].dst_mep_id = pstoamcfg->pBasiCfg->dst_mep_id;
    g_stoam_local[iloop].src_mep_id = pstoamcfg->pBasiCfg->src_mep_id;
    g_stoam_local[iloop].mel = pstoamcfg->pBasiCfg->mel;
    g_stoam_local[iloop].lsp_exp = pstoamcfg->pBasiCfg->lsp_exp;
    g_stoam_local[iloop].pw_exp = pstoamcfg->pBasiCfg->pw_exp;
    g_stoam_local[iloop].gal_exp = pstoamcfg->pBasiCfg->gal_exp;
    g_stoam_local[iloop].gal_ttl = pstoamcfg->pBasiCfg->gal_ttl;
    g_stoam_local[iloop].lsp_ttl = pstoamcfg->pBasiCfg->lsp_ttl;
    g_stoam_local[iloop].pw_ttl = pstoamcfg->pBasiCfg->pw_ttl;

    g_stoam_local[iloop].ulActiveOamkey = pstoamcfg->oam_id;
    g_stoam_local[iloop].ulTunnelLabel_tx = pstoamcfg->ulTunnelLabel_tx;
    g_stoam_local[iloop].ulPwLable_tx = pstoamcfg->ulPwLable_tx;
    g_stoam_local[iloop].ulTunnelLabel_rx = pstoamcfg->ulTunnelLabel_rx;
    g_stoam_local[iloop].ulPwLable_rx = pstoamcfg->ulPwLable_rx;

    if (NULL != pstoamcfg->pAdjust)
    {
        g_stoam_local[iloop].nearpklos = pstoamcfg->pAdjust->near_pklos;
        g_stoam_local[iloop].farpklos = pstoamcfg->pAdjust->far_pklos;
        g_stoam_local[iloop].delay = pstoamcfg->pAdjust->delay;
        g_stoam_local[iloop].vardelay = pstoamcfg->pAdjust->vardelay;
    }

    if (NULL != pstoamcfg->pAlarm)
    {
        g_stoam_local[iloop].ais_alarm_flag = pstoamcfg->pAlarm->ais_alarm_flag;
        g_stoam_local[iloop].rdi_alarm_flag = pstoamcfg->pAlarm->rdi_alarm_flag;
    }

    if (NULL != pstoamcfg->pRsvplspCfg)
    {
        NBB_MEMCPY(&g_stoam_local[iloop].stRsvpLsp, pstoamcfg->pRsvplspCfg,
                   sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP));
    }

    if (NULL != pstoamcfg->pPwCfg)
    {
        NBB_MEMCPY(&g_stoam_local[iloop].stPw, pstoamcfg->pPwCfg,
                   sizeof(ATG_DCI_TPOAM_ACTIVE_PW));
    }

    return SUCCESS;
}
