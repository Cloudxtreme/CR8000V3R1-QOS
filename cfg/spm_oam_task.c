/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_oam_task.c
  版 本 号   : 初稿
  作    者   : huxi
  生成日期   : 2013年5月14日
  最近修改   :
  功能描述   : OAM轮询任务模块
  函数列表   :
  修改历史   :
  1.日    期   : 2013年5月14日
    作    者   : huxi
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include <bmuOsHdrs.h>

/*****************************************************************************
 函 数 名  : spm_od_lm_task
 功能描述  : 按照方案进行每分钟一次计数读取并计算
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月22日
    作    者   : jwwang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lm_task(NBB_USHORT ucloop)
{
#if 0
    NBB_ULONG lmrxpcl                       = 0;
    NBB_ULONG lmtxpcf                       = 0;
    NBB_ULONG lmrxpcf                       = 0;
    NBB_ULONG lmtxpcb                       = 0;
    NBB_USHORT  lspoffset                    = 0;
    NBB_ULONG ulRegBaseAddress        = 0;
    NBB_BYTE time_flag = 0;

    /*根据按需对应的主动条目查看是哪个fpga的oam*/
    if (FALSE == g_stoam_local[g_stoam_odlocal[ucloop].usOamLoop].bModuleSwitch)
    {
        ulRegBaseAddress = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        ulRegBaseAddress = OAM_REG_BASE2_ADDRESS;
    }

    lspoffset = ucloop * 8;
    hwwritefpga(ulRegBaseAddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x2);

    while (1)
    {
        time_flag = spm_oam_16read(ulRegBaseAddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

        if (0 == (time_flag & 0x2))
        {
            break;
        }
    }

    lmrxpcl = (((uint32)spm_oam_16read(ulRegBaseAddress, LMRXPCl_OFFSET + lspoffset)) << 16)
              + (((uint32)spm_oam_16read(ulRegBaseAddress, LMRXPCl_OFFSET + 1 + lspoffset)));

    lmtxpcf = (((uint32)spm_oam_16read(ulRegBaseAddress, LMTXPC_F_OFFSET + lspoffset)) << 16)
              + (((uint32)spm_oam_16read(ulRegBaseAddress, LMTXPC_F_OFFSET + 1 + lspoffset)));

    lmrxpcf = (((uint32)spm_oam_16read(ulRegBaseAddress, LMRXPC_F_OFFSET + lspoffset)) << 16)
              + (((uint32)spm_oam_16read(ulRegBaseAddress, LMRXPC_F_OFFSET + 1 + lspoffset)));

    lmtxpcb = (((uint32)spm_oam_16read(ulRegBaseAddress, LMTXPC_B_OFFSET + lspoffset)) << 16)
              + (((uint32)spm_oam_16read(ulRegBaseAddress, LMTXPC_B_OFFSET + 1 + lspoffset)));

    g_stoam_odlmdata[ucloop].lmrxpcl = lmrxpcl;
    g_stoam_odlmdata[ucloop].lmtxpcf = lmtxpcf;
    g_stoam_odlmdata[ucloop].lmrxpcf = lmrxpcf;
    g_stoam_odlmdata[ucloop].lmtxpcb = lmtxpcb;

#if 0
    g_stoam_odlmdata[ucloop].lmrxpcl_old = g_stoam_odlmdata[ucloop].lmrxpcl_new;
    g_stoam_odlmdata[ucloop].lmtxpcf_old = g_stoam_odlmdata[ucloop].lmtxpcf_new;
    g_stoam_odlmdata[ucloop].lmrxpcf_old = g_stoam_odlmdata[ucloop].lmrxpcf_new;
    g_stoam_odlmdata[ucloop].lmtxpcb_old = g_stoam_odlmdata[ucloop].lmtxpcb_new;

    g_stoam_odlmdata[ucloop].lmrxpcl_new = lmrxpcl;
    g_stoam_odlmdata[ucloop].lmtxpcf_new = lmtxpcf;
    g_stoam_odlmdata[ucloop].lmrxpcf_new = lmrxpcf;
    g_stoam_odlmdata[ucloop].lmtxpcb_new = lmtxpcb;
#endif

#endif
}



/*****************************************************************************
 函 数 名  : list_add_node
 功能描述  : 链表表头添加节点
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月27日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT list_add_node(IN LIST_NODE **phead, IN CSF_RELATED_OAMID csf_info)
{
    NBB_INT iret = ERROR;
    LIST_NODE *pstinsert = NULL;

    if ((NULL == phead) || (NULL == *phead))
    {
        goto EXIT_LABEL;
    }

    if (g_list_length >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }

    pstinsert = (LIST_NODE *)NBB_MM_ALLOC(sizeof(LIST_NODE),
                                         NBB_NORETRY_ACT, MEM_SPM_OAM_LIST);

    if (NULL == pstinsert)
    {
        goto EXIT_LABEL;
    }

    pstinsert->lag_id = csf_info.lag_id;
    pstinsert->oamid = csf_info.oamid;
    pstinsert->module_index = csf_info.module_index;
    pstinsert->base_addr = csf_info.base_addr;
    pstinsert->port_id = csf_info.port_id;
    pstinsert->oamindex = csf_info.oamindex;

    if (NULL == *phead)//空链表
    {
        *phead = pstinsert;
        (*phead)->next = NULL;
    }
    else
    {
        pstinsert->next = (*phead)->next;
        *phead = pstinsert;
    }

    g_list_length++;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : list_delete_node
 功能描述  : 链表表头删除节点
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月27日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT list_delete_node(IN LIST_NODE **phead, IN NBB_ULONG oamid, NBB_BYTE lag_id)
{
    NBB_INT iret = ERROR;
    LIST_NODE *pstfront = NULL;
    LIST_NODE *pstdelete = NULL;

    if ((NULL == phead) || (NULL == *phead))
    {
        goto EXIT_LABEL;
    }

    if (g_list_length <= 0)
    {
        goto EXIT_LABEL;
    }

    pstdelete = *phead;
    pstfront = pstdelete;

    while (pstdelete != NULL)
    {
        if (pstdelete->oamid == oamid)
        {
            if (pstdelete->lag_id == lag_id)
            {
                if (pstdelete == *phead)
                {
                    *phead = pstdelete->next;
                }

                pstfront->next = pstdelete->next;
                NBB_MM_FREE(pstdelete, MEM_SPM_OAM_LIST);
                pstdelete = NULL;
                g_list_length--;
            }
        }
        else
        {
            pstfront = pstdelete;
            pstdelete = pstdelete->next;
        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : list_search_node
 功能描述  : 在链表中查找关键值
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 查找成功返回SUCCESS，失败返回ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月28日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT list_search_node(IN LIST_NODE *phead, NBB_ULONG oamid, NBB_BYTE lag_id)
{
    NBB_INT iret = ERROR;
    LIST_NODE *pstsearch = NULL;

    if (NULL ==  phead)
    {
        goto EXIT_LABEL;
    }

    pstsearch = phead;

    while (pstsearch != NULL)
    {
        if (pstsearch->oamid == oamid)
        {
            if (pstsearch->lag_id == lag_id)
            {
                iret = SUCCESS;
                goto EXIT_LABEL;
            }
        }

        pstsearch = pstsearch->next;
    }

EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : init_sd_list
 功能描述  : a
 输入参数  : OUT SD_LIST_NODE **root  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT init_sd_list(OUT SD_LIST_NODE **root)
{
    NBB_INT iret = ERROR;
    *root = (SD_LIST_NODE *)NBB_MM_ALLOC(sizeof(SD_LIST_NODE), 
            NBB_NORETRY_ACT, MEM_SPM_OAM_SD_LIST);

    if (NULL == (*root))
    {
        goto EXIT_LABEL;
    }

    (*root)->next = NULL;
    (*root)->oamLoop = 0;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : sd_list_addnode
 功能描述  : 链表表头添加节点
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月27日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT sd_list_addnode(IN SD_LIST_NODE **phead, IN NBB_USHORT usloop,
                      NBB_BYTE listtype)
{
    NBB_INT iret = ERROR;
    SD_LIST_NODE *pstinsert = NULL;
    NBB_USHORT *plength = 0;
    NBB_ULONG memmacro;

    switch (listtype)
    {
        case SD_LIST:
            plength = &g_sd_listlength;
            memmacro = MEM_SPM_OAM_SD_LIST;
            break;

        case WRAPPING_LIST:
            plength = &g_wrapping_listlength;
            memmacro = MEM_SPM_OAM_WRAPPING_LIST;
            break;

        default:
            goto EXIT_LABEL;
    }

    if (*plength > OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }

    pstinsert = (SD_LIST_NODE *)NBB_MM_ALLOC(sizeof(SD_LIST_NODE),
                                            NBB_NORETRY_ACT, memmacro);

    if (NULL == pstinsert)
    {
        goto EXIT_LABEL;
    }

    pstinsert->oamLoop = usloop;

    if (NULL == (*phead))//空链表
    {
        *phead = pstinsert;
        (*phead)->next = NULL;
    }
    else
    {
        pstinsert->next = (*phead)->next;
        (*phead)->next = pstinsert;
    }

    (*plength)++;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : sd_list_deletenode
 功能描述  : 链表表头删除节点
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月27日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT sd_list_deletenode(IN SD_LIST_NODE **phead, IN NBB_USHORT usloop,
                         NBB_BYTE listtype)
{
    NBB_INT iret = ERROR;
    NBB_USHORT *plength = 0;
    NBB_ULONG memmacro;
    SD_LIST_NODE *pstfront = NULL;
    SD_LIST_NODE *pstdelete = NULL;

    if ((NULL == phead) || (NULL == *phead))
    {
        goto EXIT_LABEL;
    }

    switch (listtype)
    {
        case SD_LIST:
            plength = &g_sd_listlength;
            memmacro = MEM_SPM_OAM_SD_LIST;
            break;

        case WRAPPING_LIST:
            plength = &g_wrapping_listlength;
            memmacro = MEM_SPM_OAM_WRAPPING_LIST;
            break;

        default:
            goto EXIT_LABEL;
    }

    if (*plength <= 0)
    {
        goto EXIT_LABEL;
    }

    pstdelete = *phead;
    pstfront = pstdelete;

    while (pstdelete != NULL)
    {
        if (pstdelete->oamLoop == usloop)
        {
            if (pstdelete == *phead)
            {
                *phead = pstdelete->next;
            }

            pstfront->next = pstdelete->next;
            NBB_MM_FREE(pstdelete, memmacro);
            pstdelete = NULL;
            (*plength)--;
        }
        else
        {
            pstfront = pstdelete;
            pstdelete = pstdelete->next;
        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : sd_list_searchnode
 功能描述  : 在链表中查找关键值
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 查找成功返回SUCCESS，失败返回ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月28日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT sd_list_searchnode(IN SD_LIST_NODE *phead, IN NBB_USHORT usloop)
{
    NBB_INT iret = ERROR;
    SD_LIST_NODE *pstsearch = NULL;

    if (NULL ==  phead)
    {
        goto EXIT_LABEL;
    }

    pstsearch = phead;

    while (pstsearch != NULL)
    {
        if (pstsearch->oamLoop == usloop)
        {
            iret = SUCCESS;
            goto EXIT_LABEL;
        }

        pstsearch = pstsearch->next;
    }

EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_csf_for_lag_task
 功能描述  : 轮询lag端口状态,down则发csf，up则停止发送csf
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月27日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_monitor_lag_for_csf_task()
{
    NBB_INT iret = ERROR;
    NBB_USHORT data_temp = 0;
    NBB_INT state = 1; //默认down模式
    LIST_NODE *psttemp = NULL;
    NBB_USHORT usloop = 0;

    if (NULL == g_oamid_encsflist)
    {
        return;
    }

    psttemp = g_oamid_encsflist;

    while (psttemp != NULL)
    {
        if (psttemp->oamid != USELESS)
        {
#if 0
            iret = aps_get_mclag_state(psttemp->lag_id, &state);

#if 0

            if (SUCCESS != iret)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\aps lag state failed,lag_id=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, psttemp->lag_id);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

#endif
            usloop = psttemp->oamindex;
            g_lag_state_now[usloop] = state;
#endif

            if (g_lag_state_now[usloop] != g_lag_state_last[usloop])
            {
                printf("state_now=%d,state_last=%d\n", g_lag_state_now[usloop],
                       g_lag_state_last[usloop]);

                if (0x1 == g_lag_state_now[usloop]) //down
                {
                    printf("port_id=%d,oamid=%d\n", psttemp->port_id, psttemp->oamid);
                    spm_oam_16write(psttemp->base_addr,
                                    CV_TXVLAN_LABEL_OFFSET + 0x7 + 8 * (psttemp->module_index),
                                    psttemp->port_id - 1);
                }
                else //up
                {
                    printf("port_id=0xFFFF,oamid=%d\n", psttemp->oamid);
                    spm_oam_16write(psttemp->base_addr,
                                    CV_TXVLAN_LABEL_OFFSET + 0x7 + 8 * (psttemp->module_index), 0xFFFF); //默认无告警
                }

                g_lag_state_last[usloop] = g_lag_state_now[usloop];
            }
        }

        psttemp = psttemp->next;
    }

    return;
}

#ifdef SPU

/*****************************************************************************
 函 数 名  : spm_oam_sd_active_lm_counter
 功能描述  : a
 输入参数  : NBB_INT oamloop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_sd_active_lm_counter(NBB_INT oamloop)
{
    NBB_USHORT rx_temp = 0;
    NBB_USHORT tx_temp = 0;
    NBB_ULONG base_addr = 0;
    float lossratio = 0;
    NBB_USHORT modoule_index = 0;
    NBB_INT iret = ERROR;
    NBB_USHORT sdnumindex = 0;
    NBB_USHORT sdbitindex = 0;
    NBB_USHORT sddatatemp = 0;
    NBB_USHORT mask = 0;
    NBB_USHORT usloop = 0;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    NBB_BYTE portaddrindex = 0;
    NBB_BYTE bitindex = 0;
    static NBB_BYTE s_cntinsert[OAM_MAX_NUM_ACTIVE_SPU] = {0};
    static NBB_BYTE s_cntdelete[OAM_MAX_NUM_ACTIVE_SPU] = {0};
    NBB_BYTE sdinsert = 0;
    NBB_INT ulrtn = ERROR;

    if (oamloop < 0 || oamloop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }

    usloop = (NBB_USHORT)oamloop;

    if (USELESS == g_stoam_local[usloop].ulActiveOamkey)
    {
        printf("oamkey in array is unused!\n");
        goto EXIT_LABEL;
    }

    spm_get_moudle_base_addr(usloop, &base_addr);
    modoule_index = g_stoam_local[usloop].module_index;
    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        printf("get loss ratio by active lm failed\n");
        goto EXIT_LABEL;
    }

    /*计算近端丢包率*/
    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;

    if (1 == g_sd_debug)
    {
        printf("****************oamloop :%d****************\n", usloop);
        printf("RSVP NEAR :\n");
        printf("txfcf %d\n", txfcf);
        printf("rxfci %d\n", rxfci);
    }

    temp1 = txfcf;
    temp2 = rxfci;

    if ((temp1 != 0x0))
    {
        if (temp1 < temp2) /* 如果收比发多包，FPGA存在计数瞬时差1 */
        {
            lossratio = 0;
        }
        else
        {
            lossratio = ((float)(temp1 - temp2)) / temp1 * 100;
        }
    }
    else
    {
        lossratio = 0;
    }

    if (1 == g_sd_debug)
    {
        printf("g_sd_threshold=%d\n", g_sd_threshold);
        printf("(temp1 - temp2)=%d\n", (temp1 - temp2));
        printf("temp1=%d\n", temp1);
        printf("near lossratio %f\n", lossratio);
    }

    g_sd_lossratio = lossratio;

    //if ( lossratio != 0x0 )
    {
        if (lossratio > (g_sd_threshold * 0.1))
        {
            g_sdstate_now[oamloop] = TPOAM_SD_INSERT;

            if ((g_sdstate_last[oamloop] == TPOAM_SD_INITE)
                || (g_sdstate_last[oamloop] == g_sdstate_now[oamloop]))
            {
                s_cntinsert[oamloop]++;
                g_sdstate_last[oamloop] = g_sdstate_now[oamloop];
            }
            else
            {
                s_cntinsert[oamloop] = 1;
                g_sdstate_last[oamloop] = g_sdstate_now[oamloop];
            }

            //连续3个周期有sd，则强插sd告警
            if (0x3 == s_cntinsert[oamloop])
            {
                s_cntinsert[oamloop] = 0;
                s_cntdelete[oamloop] = 0;
                sdinsert = 1;
            }

            if (1 == g_sd_debug)
            {
                printf("1\n");
                printf("s_cntinsert=%d,s_cntdelete=%d\n", s_cntinsert[oamloop], s_cntdelete[oamloop]);
                printf("g_sdstate_last=%d,g_sdstate_now=%d\n", g_sdstate_last[oamloop],
                       g_sdstate_now[oamloop]);
            }
        }
        else
        {
            g_sdstate_now[oamloop] = TPOAM_SD_DELETE;
            s_cntinsert[oamloop] = 0;

            if ((g_sdstate_last[oamloop] == TPOAM_SD_INITE)
                || (g_sdstate_last[oamloop] == g_sdstate_now[oamloop]))
            {
                s_cntdelete[oamloop]++;
                g_sdstate_last[oamloop] = g_sdstate_now[oamloop];
            }
            else
            {
                s_cntdelete[oamloop] = 1;
                g_sdstate_last[oamloop] = g_sdstate_now[oamloop];
            }

            //连续3周期没有sd，则消掉sd告警
            if (0x3 == s_cntdelete[oamloop])
            {
                s_cntdelete[oamloop] = 0;
                s_cntinsert[oamloop] = 0;
                sdinsert = 2;
            }

            if (1 == g_sd_debug)
            {
                printf("2\n");
                printf("s_cntinsert=%d,s_cntdelete=%d\n", s_cntinsert[oamloop], s_cntdelete[oamloop]);
                printf("g_sdstate_last=%d,g_sdstate_now=%d\n",
                       g_sdstate_last[oamloop], g_sdstate_now[oamloop]);
            }
        }
    }

    if (0x1 == sdinsert) //大于门限则强插sd
    {
        g_stoam_local[usloop].bSdAlarm = TRUE;

        //如果检测到SD告警，则也需要将对应出端口的告警置位,用于传递sd告警
//        spm_lps_write_sd_to_port_alm_bus(g_stoam_local[usloop].vs_Portid,0x1);
        (NBB_VOID)fhdrv_port_set_ing_port_sd_en(g_stoam_local[usloop].chip_id, g_stoam_local[usloop].vs_Portid, TRUE);
        if (1 == g_sd_debug)
        {
            printf("tempPort=%#x\n", g_stoam_local[usloop].vs_Portid);
        }
    }
    else if (0x2 == sdinsert) //清除sd
    {
        g_stoam_local[usloop].bSdAlarm = FALSE;

        //如果检测到SD告警，则也需要将对应出端口的告警置位,用于传递sd告警
//        spm_lps_write_sd_to_port_alm_bus(g_stoam_local[usloop].vs_Portid,0x0);
        (NBB_VOID)fhdrv_port_set_ing_port_sd_en(g_stoam_local[usloop].chip_id, g_stoam_local[usloop].vs_Portid, FALSE);

        if (1 == g_sd_debug)
        {
            printf("tempPort=%#x\n", g_stoam_local[usloop].vs_Portid);
        }
    }

    ulrtn = SUCCESS;
EXIT_LABEL:
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_packet_loss_ratio_by_active_lm
 功能描述  : 主动LM获取cv帧统计
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_packet_loss_ratio_by_active_lm(NBB_USHORT usloop)
{
    NBB_INT iret = ERROR;

    NBB_ULONG oamid;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfcf = 0;

    if (usloop > OAM_MAX_NUM_ACTIVE_SPU - 1)
    {
#if 0
        OS_SPRINTF(g_uc_message, "%s %s,%d\nusLoop is out of range\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
#endif
        goto EXIT_LABEL;
    }

    oamid = g_stoam_local[usloop].ulActiveOamkey;

    if (USELESS == oamid)
    {
#if 0
        OS_SPRINTF(g_uc_message, "%s %s,%d\nusLoop is unused in array\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
#endif
        goto EXIT_LABEL;
    }
    rxfcf = spm_oam_16read(OAM_REG_BASE1_ADDRESS, CV_CNT_RX_OAM + g_stoam_local[usloop].module_index);
    txfcf = spm_oam_16read(OAM_REG_BASE1_ADDRESS, CV_CNT_TX_OAM + g_stoam_local[usloop].module_index);

    g_stoam_activelmdata[usloop].lmrxpcf = rxfcf;
    g_stoam_activelmdata[usloop].lmtxpcf = txfcf;

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : spm_clear_sd_alarm
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_clear_sd_alarm(NBB_VOID)
{
    NBB_USHORT oamloop = 0;
    SD_LIST_NODE *psdnode = NULL;
    psdnode = g_oam_sdlist;

    while (NULL != psdnode)
    {
        oamloop = psdnode->oamLoop;
        psdnode = psdnode->next;
        (NBB_VOID)fhdrv_port_set_ing_port_sd_en(g_stoam_local[oamloop].chip_id, 
            g_stoam_local[oamloop].vs_Portid, FALSE);
        g_stoam_local[oamloop].bSdAlarm = FALSE;
    }

    return;
}


#endif
#ifdef PTN690_CES
NBB_INT spm_packet_loss_ratio_by_active_lm(NBB_USHORT usloop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_oam_sd_active_lm_counter
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_sd_active_lm_counter(NBB_INT oamloop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_clear_sd_alarm
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_clear_sd_alarm(NBB_VOID)
{
    return ;
}


#endif

#ifdef SRC
NBB_INT spm_packet_loss_ratio_by_active_lm(NBB_USHORT usloop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_oam_sd_active_lm_counter
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_sd_active_lm_counter(NBB_INT oamloop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_clear_sd_alarm
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_clear_sd_alarm(NBB_VOID)
{
    return;
}

#endif

#ifdef SNU
NBB_INT spm_packet_loss_ratio_by_active_lm(NBB_USHORT usloop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_oam_sd_active_lm_counter
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_sd_active_lm_counter(NBB_INT oamloop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_clear_sd_alarm
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年5月5日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_clear_sd_alarm(NBB_VOID)
{
    return;
}

#endif

/*****************************************************************************
 函 数 名  : spm_tpoam_sd_alarm_task
 功能描述  : 释放用于轮询sd告警的信号量
 输入参数  : NBB_VOID
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月17日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_tpoam_sd_alarm_task(NBB_VOID)
{
    OS_SPRINTF(g_uc_message, "%s %s,%d\ng_sdThreshold=%d\n",
               ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, g_sd_threshold);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    while (1)
    {
        if (0x0 != g_sd_threshold)
        {
            spm_tpoam_sd_alarm();
        }
        else
        {
            spm_clear_sd_alarm();//门限配置为0时清楚所有sd告警位
        }

        //(NBB_VOID)BmuTaskDelay(BmuSysClkRateGet()* 20);
        (NBB_VOID)BmuTaskDelay(g_sd_windowticks * 4);
        BmuTaskDiag();
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_tpoam_sd_alarm
 功能描述  : 获取信号量，轮询sd状态
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月17日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_tpoam_sd_alarm(NBB_VOID)
{
    NBB_USHORT oamloop = 0;
    SD_LIST_NODE *psdnode = NULL;
    psdnode = g_oam_sdlist;

    while (NULL != psdnode)
    {
        oamloop = psdnode->oamLoop;
        psdnode = psdnode->next;
        spm_oam_sd_active_lm_counter(oamloop);
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_tpoam_sd_alarm_thread
 功能描述  : �
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月17日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_tpoam_sd_alarm_thread()
{          
    g_tpoam_sd = BmuTaskCreate("g_tpoam_sd", 30, 0, (int (*)(void *))spm_tpoam_sd_alarm_task, NULL);
    if (g_tpoam_sd == ERROR)
    {
        printf("g_tpoam_sd task failed!\n");
        return -1;
    }
    
    return 0;
}

