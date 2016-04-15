/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火科技有限公司

 ******************************************************************************
  文 件 名   : spm_oam_pm.c
  版 本 号   : 初稿
  作    者   : huxi
  生成日期   : 2013年10月10日
  最近修改   :
  功能描述   : oam性能告警上报
  函数列表   :
  修改历史   :
  1.日    期   : 2013年10月10日
    作    者   : huxi
    修改内容   : 创建文件

******************************************************************************/
#include <nbase.h>
#include <spmincl.h>
BMU_WDOG_ID g_stoam_walarmdog; /* 看门狗定时器 */
uint64 g_tx_vs_total_pack = 0;/* VS TX PACK */
uint64 g_rx_vs_total_pack = 0;/* VS RX PACK */
uint64 g_tx_vc_total_pack = 0;/* VC TX PACK */
uint64 g_rx_vc_total_pack = 0;/* VC RX PACK */
uint64 g_tx_vp_total_pack = 0;/* VP TX PACK */
uint64 g_rx_vp_total_pack = 0;/* VP RX PACK */

/*****************************************************************************
 函 数 名  : oam_vs_find_active_oamid
 功能描述  : VS层根据key查找主动oam对应条目号
 输入参数  : oamvskey
 输出参数  :
 返 回 值     : 条目号 ;
                          0xffff表示未查找到与输入相同端口配置的条目
                          其他值表示查到了对应条目
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_USHORT oam_vs_find_active_oamid(OAM_VS_KEY oamvskey)
{
    NBB_USHORT usoamindex                  = 0XFFFF;
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_BYTE     ulrtn                        = ERROR;

    /*判断输入参数是否正确*/
    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于主动帧告警，直接查主动数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_ACTIVE_SPU; usloop++)
    {
        if (g_stoam_local[usloop].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usloop].vs_Portid == uslogportid)
            && (g_stoam_local[usloop].vs_Slotid == oamvskey.SlotId))
        {
            usoamindex = usloop;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return usoamindex;
}


/*****************************************************************************
 函 数 名  : oam_vs_sd_alarm
 功能描述  : 获取vs层sd告警
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月20日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_sd_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *sd_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;
    NBB_USHORT alarmtemp = 0;

    /*判断输入参数是否正确*/
    if (sd_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "sd_alarm null error!"));
        goto EXIT_LABEL;
    }

    *sd_alarm = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if ((g_stoam_local[usoamindex].vp_attrib == 1) && (g_stoam_local[usoamindex].cv_enable != 0))
    {
#if 0

        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            alarmtemp = spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                       SD_OVERRIDE_OAM + g_stoam_local[usoamindex].module_index / 16);
            *sd_alarm = (alarmtemp >> g_stoam_local[usoamindex].module_index % 16)
                        & 0x1;
            *sd_alarm = (*sd_alarm == 0) ? 0 : 1;
        }
        else
        {
            alarmtemp = spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                       SD_OVERRIDE_OAM + g_stoam_local[usoamindex].module_index / 16);
            *sd_alarm = (alarmtemp >> g_stoam_local[usoamindex].module_index % 16)
                        & 0x1;
            *sd_alarm = (*sd_alarm == 0) ? 0 : 1;
        }

#endif
        *sd_alarm = g_stoam_local[usoamindex].bSdAlarm;

        if (1 == g_sd_debug)
        {
            printf("sd_alarm = %d\n", *sd_alarm);
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}




/*****************************************************************************
 函 数 名  : oam_vs_loc_alarm
 功能描述  : oam    VS层上报LOC告警
 输入参数  : oamvskey
 输出参数  : loc_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_loc_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *loc_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE rdi_alarm = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_loc_alarm"));

    /*判断输入参数是否正确*/
    if (loc_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "loc_alarm null error!"));
        goto EXIT_LABEL;
    }

    *loc_alarm = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);
    oam_vs_rdi_alarm(oamvskey, &rdi_alarm);
    oam_vs_lck_alarm(oamvskey, &lck_alarm);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if ((g_stoam_local[usoamindex].vp_attrib == 1) && (g_stoam_local[usoamindex].cv_enable != 0))
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            *loc_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x10);
            *loc_alarm = (*loc_alarm == 0) ? 0 : 1;
        }
        else
        {
            *loc_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x10);
            *loc_alarm = (*loc_alarm == 0) ? 0 : 1;
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_lck_alarm //在vs层有没有lck帧?
 功能描述  : oam    VS层上报LCK告警
 输入参数  : oamvskey
 输出参数  : lck_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_lck_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *lck_alarm)
{
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_USHORT temp = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odloop = 0;
    NBB_BYTE ucodloop = 0;


    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_lck_alarm"));

    /*判断输入参数是否正确*/
    if (lck_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *lck_alarm = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    usloop = oam_vs_find_active_oamid(oamvskey);
    spm_get_moudle_base_addr(usloop, &base_addr);
    ulrtn = spm_odindex_by_activeindex(usloop, &odloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ucodloop = g_stoam_odlocal[odloop].ucOdLoop - 1;
    temp = spm_oam_16read(base_addr, OAM_LCK_ALARM_OFFSET);
    *lck_alarm = (temp >> ucodloop) & 0x1;
#if 0
    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
                指的就是按需条目号*/
                *lck_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                                       LCK_ALARM_OFFSET) & (0x1 << usloop));
                *lck_alarm = (*lck_alarm == 0) ? 0 : 1;

                if (1 == g_alarm_debug)
                {
                    printf("VS LCK module 1 lck_alarm = %d usloop = %d\n", *lck_alarm, usloop);
                }
            }
            else
            {
                /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
                指的就是按需条目号*/
                *lck_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                                       LCK_ALARM_OFFSET) & (0x1 << usloop));
                *lck_alarm = (*lck_alarm == 0) ? 0 : 1;

                if (1 == g_alarm_debug)
                {
                    printf("VS LCK module 2 lck_alarm = %d usloop = %d\n", *lck_alarm, usloop);
                }
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

#endif
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_mmg_alarm
 功能描述  : oam    VS层上报CVMEG不匹配告警
 输入参数  : oamvskey
 输出参数  : mmg_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_mmg_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *mmg_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE lck_alarm = 0;

    /*判断输入参数是否正确*/
    if (mmg_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "mmg_alarm null error!"));
        goto EXIT_LABEL;
    }

    *mmg_alarm = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);
    oam_vs_lck_alarm(oamvskey, &lck_alarm);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *mmg_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x4);

        if (1 == lck_alarm)
        {
            *mmg_alarm = 0;
        }
        else
        {
            *mmg_alarm = (*mmg_alarm == 0) ? 0 : 1;

        }
    }
    else
    {
        *mmg_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
            OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x4);

        if (1 == lck_alarm)
        {
            *mmg_alarm = 0;
        }
        else
        {
            *mmg_alarm = (*mmg_alarm == 0) ? 0 : 1;

        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_unm_alarm
 功能描述  : oam    VS层上报CVMEP不匹配告警
 输入参数  : oamvskey
 输出参数  : unm_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_unm_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *unm_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE lck_alarm = 0;
    NBB_BYTE mmg_alarm = 0;

    /*判断输入参数是否正确*/
    if (unm_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unm_alarm null error!"));
        goto EXIT_LABEL;
    }

    *unm_alarm = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);
    oam_vs_lck_alarm(oamvskey, &lck_alarm);
    oam_vs_mmg_alarm(oamvskey, &mmg_alarm);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *unm_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x2);

        if (1 == (mmg_alarm || lck_alarm))
        {
            *unm_alarm = 0;
        }
        else
        {
            *unm_alarm = (*unm_alarm == 0) ? 0 : 1;
        }
    }
    else
    {
        *unm_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x2);

        if (1 == (mmg_alarm || lck_alarm))
        {
            *unm_alarm = 0;
        }
        else
        {
            *unm_alarm = (*unm_alarm == 0) ? 0 : 1;
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_unp_alarm
 功能描述  : oam    VS层上报CV周期不匹配告警
 输入参数  : oamvskey
 输出参数  : unp_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_unp_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *unp_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE unm_alarm = 0;
    NBB_BYTE mmg_alarm = 0; 
    NBB_BYTE lck_alarm = 0;

    /*判断输入参数是否正确*/
    if (unp_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unp_alarm null error!"));
        goto EXIT_LABEL;
    }

    *unp_alarm = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);
    oam_vs_lck_alarm(oamvskey, &lck_alarm);
    oam_vs_mmg_alarm(oamvskey, &mmg_alarm);
    oam_vs_unm_alarm(oamvskey, &unm_alarm);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *unp_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x8);

        if (1 == (mmg_alarm || lck_alarm || unm_alarm))
        {
            *unp_alarm = 0;
        }
        else
        {
            *unp_alarm = (*unp_alarm == 0) ? 0 : 1;
        }
    }
    else
    {
        *unp_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x8);

        if (1 == (mmg_alarm || lck_alarm || unm_alarm))
        {
            *unp_alarm = 0;
        }
        else
        {
            *unp_alarm = (*unp_alarm == 0) ? 0 : 1;
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_rdi_alarm
 功能描述  : oam    VS层上报RDI告警
 输入参数  : oamvskey
 输出参数  : rdi_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_rdi_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *rdi_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE lck_alarm = 0;

    /*判断输入参数是否正确*/
    if (rdi_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "rdi_alarm null error!"));
        goto EXIT_LABEL;
    }

    *rdi_alarm = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);
    oam_vs_lck_alarm(oamvskey, &lck_alarm);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (1 == g_stoam_local[usoamindex].rdi_alarm_flag)
    {
        *rdi_alarm = 0;
        goto EXIT_LABEL;
    }
    else
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            *rdi_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                    OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x1);

            if (1 == lck_alarm)
            {
                *rdi_alarm = 0;
            }
            else
            {
                *rdi_alarm = (*rdi_alarm == 0) ? 0 : 1;
            }

        }
        else
        {
            *rdi_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                    OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x1);

            if (1 == lck_alarm)
            {
                *rdi_alarm = 0;
            }
            else
            {
                *rdi_alarm = (*rdi_alarm == 0) ? 0 : 1;
            }
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_lblos_alarm
 功能描述  : oam    VS层上报lblos告警
 输入参数  : oamvpkey
 输出参数  : lblos_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_lblos_alarm(OAM_VS_KEY oamvskey, NBB_BYTE *lblos_alarm)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odloop = 0;
    NBB_BYTE temp;
    NBB_BYTE ucodloop = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_lblos_alarm"));

    /*判断输入参数是否正确*/
    if (lblos_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lblos_alarm null error!"));
        goto EXIT_LABEL;
    }

    *lblos_alarm = 0;
    usloop = oam_vs_find_active_oamid(oamvskey);
    spm_get_moudle_base_addr(usloop, &base_addr);
    ulrtn = spm_odindex_by_activeindex(usloop, &odloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ucodloop = g_stoam_odlocal[odloop].ucOdLoop - 1;

    if (1 == g_oam_debug)
    {
        printf("ucodloop %d odloop %d\n", ucodloop, odloop);
    }

    if (NULL != g_stoam_odlocal[odloop].pLoopFrame)
    {
        if (1 == g_stoam_odlocal[odloop].pLoopFrame->byte1.send_enable)
        {
            if (1 == g_oam_debug)
            {
                printf("lb send enable\n");
            }

            temp = spm_oam_16read(base_addr, OAM_LBLOS_ALARM_OFFSET);
            *lblos_alarm = (temp >> ucodloop) & 0x1;
        }

        if (1 == g_oam_debug)
        {
            printf("lblos_alarm %d\n", *lblos_alarm);
        }

    }

    ulrtn = SUCCESS;
#if 0

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;
        }

        //ulrtn = oam_vs_find_active_oamid(oamvskey);
        /*if ( SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }*/


        /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
        if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
        {
            if (NULL == g_stoam_odlocal[usloop].pLoopFrame)
            {
                goto EXIT_LABEL;
            }

            *lblos_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                      LBLOS_ALARM_OFFSET) & (0x1 << (g_stoam_odlocal[usloop].ucOdLoop - 1)));
        }
        else
        {
            if (NULL == g_stoam_odlocal[usloop].pLoopFrame)
            {
                goto EXIT_LABEL;
            }

            *lblos_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                      LBLOS_ALARM_OFFSET) & (0x1 << (g_stoam_odlocal[usloop].ucOdLoop - 1)));
        }

        ulrtn = SUCCESS;
        *lblos_alarm = (*lblos_alarm == 0) ? 0 : 1;
        goto EXIT_LABEL;

    }

#endif
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_packdelay
 功能描述  : oam    VS层时延s
 输入参数  : oamvskey
 输出参数  : packdelay
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_packdelay(OAM_VS_KEY oamvskey, uint64 * packdelay)
{
#if 0
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;

    /*判断输入参数是否正确*/
    if (packdelay == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelay null error!"));
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            delaycfg = g_stoam_local[usoamactindex].delay;
            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspoffset
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延s，高32位是s，低32位是ns,一起组成当前时戳
            将读取到数值全部转换为ns计算*/
            //*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //           - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000) ;
            delaycfg = g_stoam_local[usoamactindex].delay;

            if (0 == delaycfg)
            {
                *packdelay = rxstampb - txtimestampf;
                *packdelay = (*packdelay) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980); //取值范围为正负0.2以内
                *packdelay = (*packdelay) / 1000000000;
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif   
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_vs_packdelay_ns
功能描述  : oam    VS层时延ns
输入参数  : oamvskey
输出参数  : packdelay
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                     SUCCESS:根据key值查找到了对应的信息
调用函数  :
被调函数  :

修改历史      :
1.日    期   : 2013年10月14日
  作    者   : 胡晓露
  审 核 人   : #
  修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_packdelay_ns(OAM_VS_KEY oamvskey, uint64 * packdelay)
{
#if 0
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_packdelay_ns"));

    /*判断输入参数是否正确*/
    if (packdelay == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelay null error!"));
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspoffset
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延s，高32位是s，低32位是ns,一起组成当前时戳
            将读取到数值全部转换为ns计算*/
            //*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //           - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000) ;
            delaycfg = g_stoam_local[usoamactindex].delay;

            if ((NULL != g_stoam_odlocal[usloop].pDmFrame) && (0 == delaycfg))
            {
                *packdelay = rxstampb - txtimestampf - 30000 - 
                    (g_stoam_odlocal[usloop].pDmFrame->frame_length + 30) * 32;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980);
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_vs_packdelayvar
功能描述  : oam    VS层时延变化量s
输入参数  : oamvskey
输出参数  : packdelayvar
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                      SUCCESS:根据key值查找到了对应的信息
调用函数  :
被调函数  :

修改历史      :
 1.日    期   : 2013年10月14日
   作    者   : 胡晓露
   审 核 人   : #
   修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_packdelayvar(OAM_VS_KEY oamvskey, uint64 * packdelayvar)
{
#if 0
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    static uint64    oldTimevar               = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_USHORT timeflag = 0;
    NBB_USHORT vardelaycfg = 0;
    uint64 temp = 0;

    // NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_packdelayvar"));

    /*判断输入参数是否正确*/
    if (packdelayvar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelay null error!"));
        goto EXIT_LABEL;
    }

    *packdelayvar = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspoffset
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   RX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + (uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                    RX_TIMESTPF_OFFSET + 3 + lspoffset);

            txstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                               TX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf );*/
            /* 计算帧时延s，高32位是s，低32位是ns,一起组成当前时戳
            将读取到数值全部转换为ns计算*/
            //*packdelayvar = ((rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000))
            //        - ((txstampb&TIME_STAMP_MASK + ((txstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (rxtimestampf&TIME_STAMP_MASK + ((rxtimestampf>>32)&TIME_STAMP_MASK)*1000000000) );
            //*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf);
            vardelaycfg = g_stoam_local[usoamactindex].vardelay;

            if (0 == vardelaycfg)
            {
                if ((rxstampb - txtimestampf) > oldTimevar)
                {
                    *packdelayvar = (rxstampb - txtimestampf) - oldTimevar;
                }
                else
                {
                    *packdelayvar =  oldTimevar - (rxstampb - txtimestampf);
                }

                oldTimevar = rxstampb - txtimestampf;
                *packdelayvar = (*packdelayvar) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelayvar = rand() % (40 * vardelaycfg + 1) + (vardelaycfg * 980);
                *packdelayvar = (*packdelayvar) / 1000000000;
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_vs_packdelayvar_ns
功能描述  : oam    VS层时延变化量ns
输入参数  : oamvskey
输出参数  : packdelayvar
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                     SUCCESS:根据key值查找到了对应的信息
调用函数  :
被调函数  :

修改历史      :
1.日    期   : 2013年10月14日
  作    者   : 胡晓露
  审 核 人   : #
  修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_packdelayvar_ns(OAM_VS_KEY oamvskey, uint64 * packdelayvar)
{
#if 0
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    static uint64    oldTimevar               = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_USHORT timeflag = 0;
    NBB_USHORT vardelaycfg = 0;
    uint64 temp = 0;

    /*判断输入参数是否正确*/
    if (packdelayvar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelay null error!"));
        goto EXIT_LABEL;
    }

    *packdelayvar = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspoffset
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   RX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + (uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                    RX_TIMESTPF_OFFSET + 3 + lspoffset);

            txstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                               TX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf );*/
            /* 计算帧时延s，高32位是s，低32位是ns,一起组成当前时戳
            将读取到数值全部转换为ns计算*/
            //*packdelayvar = ((rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000))
            //        - ((txstampb&TIME_STAMP_MASK + ((txstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (rxtimestampf&TIME_STAMP_MASK + ((rxtimestampf>>32)&TIME_STAMP_MASK)*1000000000) );
            //*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf);
            vardelaycfg = g_stoam_local[usoamactindex].vardelay;

            if (0 == vardelaycfg)
            {
                if ((rxstampb - txtimestampf) > oldTimevar)
                {
                    *packdelayvar = (rxstampb - txtimestampf) - oldTimevar;
                }
                else
                {
                    *packdelayvar =  oldTimevar - (rxstampb - txtimestampf);
                }

                oldTimevar = rxstampb - txtimestampf;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelayvar = rand() % (40 * vardelaycfg + 1) + (vardelaycfg * 980);
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif 
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : oam_vs_pktlos_near
 功能描述  : oam    VS层丢包测量近端丢包率
 输入参数  : oamvskey
 输出参数  : PktLosNear
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_pktlos_near(OAM_VS_KEY oamvskey,  float * PktLosNear)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG txpcf = 0;
    NBB_ULONG rxpcf = 0;
    NBB_ULONG txpcb = 0;
    NBB_ULONG rxpcl = 0;
    static NBB_ULONG s_old_txpcf = 0;
    static NBB_ULONG s_old_rxpcf = 0;
    static NBB_ULONG s_old_txpcb = 0;
    static NBB_ULONG s_old_rxpcl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosNear == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosNear null error!"));
        goto EXIT_LABEL;
    }

    *PktLosNear = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            printf("usloop %d ulOamActiveKey %d", usloop, g_stoam_odlocal[usloop].ulOamActiveKey);
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            printf("usoamactindex %d ulActiveOamkey %d", usoamactindex, g_stoam_local[usoamactindex].ulActiveOamkey);
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            spm_od_lm_task(usloop);
            
            /*LMM 发送*/
            txpcf = g_stoam_odlmdata[usloop].lmtxpcf;
            
            /*LMM 接收*/
            rxpcf = g_stoam_odlmdata[usloop].lmrxpcf;
            
            /*LMR 发送*/
            txpcb = g_stoam_odlmdata[usloop].lmtxpcb;
            
            /*LMR 接收*/
            rxpcl = g_stoam_odlmdata[usloop].lmrxpcl;

            //*PktLosNear = (float)(SUB_ABS(txpcb, s_old_txpcb) - 
            //SUB_ABS(rxpcl, s_old_rxpcl))/SUB_ABS(txpcb, s_old_txpcb) * 100;
            //if(1 ==g_alarm_debug)
            {
                printf("VS ONDEMAN NEAR:\n");
                printf("txpcb : %d\n", txpcb);
                printf("s_old_txpcb %d\n", s_old_txpcb);
                printf("rxpcl %d\n", rxpcl);
                printf("s_old_rxpcl %d\n", s_old_rxpcl);
            }

            if (txpcb > s_old_txpcb)
            {
                temp1 = txpcb - s_old_txpcb;
            }
            else
            {
                temp1 = s_old_txpcb - txpcb;
            }

            if (rxpcl > s_old_rxpcl)
            {
                temp2 = rxpcl - s_old_rxpcl;
            }
            else
            {
                temp2 = s_old_rxpcl - rxpcl;
            }

            *PktLosNear = (float)(temp1 - temp2) / temp1 * 100;
            
            //if(1 == g_alarm_debug)
            {
                printf("PktLosNear : %f\n", *PktLosNear);
            }
            s_old_txpcb = txpcb;
            s_old_rxpcl = rxpcl;
            
            /*计算近端丢包率*/
            //* PktLosNear = SUB_ABS(g_stoam_activelmdata[usodoamid].lmtxpcb_new,
            //g_stoam_activelmdata[usodoamid].lmtxpcb_old)
            //                      -SUB_ABS(g_stoam_activelmdata[usodoamid].lmrxpcl_new,
            //g_stoam_activelmdata[usodoamid].lmrxpcl_old);
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_pktlos_far
 功能描述  : oam    VS层丢包测量远端丢包率
 输入参数  : oamvskey
 输出参数  : PktLosFar
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_pktlos_far(OAM_VS_KEY oamvskey,  float * PktLosFar)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG txpcf = 0;
    NBB_ULONG rxpcf = 0;
    NBB_ULONG txpcb = 0;
    NBB_ULONG rxpcl = 0;
    static NBB_ULONG s_old_txpcf = 0;
    static NBB_ULONG s_old_rxpcf = 0;
    static NBB_ULONG s_old_txpcb = 0;
    static NBB_ULONG s_old_rxpcl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_pktlos_far"));

    /*判断输入参数是否正确*/
    if (PktLosFar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosFar null error!"));
        goto EXIT_LABEL;
    }

    *PktLosFar = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SubBoardId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "SlotId error!"));
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PhyPortId error!"));
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            spm_od_lm_task(usloop);

            /*LMM 发送*/
            txpcf = g_stoam_odlmdata[usloop].lmtxpcf;
            
            /*LMM 接收*/
            rxpcf = g_stoam_odlmdata[usloop].lmrxpcf;
            
            /*LMR 发送*/
            txpcb = g_stoam_odlmdata[usloop].lmtxpcb;
            
            /*LMR 接收*/
            rxpcl = g_stoam_odlmdata[usloop].lmrxpcl;
            
            //if(1 == g_alarm_debug)
            {
                printf("VS ONDEMAN FAR:\n");
                printf("txpcf : %d\n", txpcf);
                printf("s_old_txpcf : %d\n", s_old_txpcf);
                printf("rxpcf : %d \n", rxpcf);
                printf("s_old_rxpcf : %d\n", s_old_rxpcf);
            }

            if (txpcf > s_old_txpcf)
            {
                temp1 = txpcf - s_old_txpcf;
            }
            else
            {
                temp1 = s_old_txpcf - txpcf;
            }

            if (rxpcf > s_old_rxpcf)
            {
                temp2 = rxpcf - s_old_rxpcf;
            }
            else
            {
                temp2 = s_old_rxpcf - rxpcf;
            }

            *PktLosFar = (float)(temp1 - temp2) / temp1 * 100;
            
            //if(1 == g_alarm_debug)
            {
                printf("PktLosFar : %f", *PktLosFar);
            }
            
            //*PktLosFar =(float)(SUB_ABS(txpcf, s_old_txpcf) - 
            //SUB_ABS(rxpcf, s_old_rxpcf))/SUB_ABS(txpcf, s_old_txpcf) * 100;
            s_old_txpcf = txpcf;
            s_old_rxpcf = rxpcf;
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_active_pktlos_near
 功能描述  : oam    VS层丢包测量近端丢包率
 输入参数  : oamvskey
 输出参数  : PktLosNear
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_active_pktlos_near(OAM_VS_KEY oamvskey,  float * PktLosNear)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG txfcb = 0;
    NBB_ULONG rxfcb = 0;
    static NBB_ULONG s_old_txfcf = 0;
    static NBB_ULONG s_old_rxfci = 0;
    static NBB_ULONG s_old_txfcb = 0;
    static NBB_ULONG s_old_rxfcb = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    float temppktlos1 = 0;
    float temppktlos2 = 0;
    NBB_USHORT nearpktloscfg = 0;
    float temp3 = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosNear == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosNear null error!"));
        goto EXIT_LABEL;
    }

    *PktLosNear = 0;
    usloop = oam_vs_find_active_oamid(oamvskey);

    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    /*计算近端丢包率*/
    /*计算近端丢包率*/
    /*LMM 发送*/
    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    
    /*LMM 接收*/
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;
    
    /*LMR 发送*/
    txfcb = g_stoam_activelmdata[usloop].lmtxpcb;
    
    /*LMR 接收*/
    rxfcb = g_stoam_activelmdata[usloop].lmrxpcl;
    nearpktloscfg = g_stoam_local[usloop].nearpklos;

    if (1 == g_alarm_debug)
    {
        printf("VS NEAR :\n");
        printf("txfcf %d\n", txfcf);
        printf("s_old_txfcf %d\n", s_old_txfcf);
        printf("rxfci %d\n", rxfci);
        printf("s_old_rxfci %d\n", s_old_rxfci);
    }

    //*PktLosNear = (float)(SUB_ABS(txpcb, s_old_txpcb) - 
    //SUB_ABS(rxpcl, s_old_rxpcl))/SUB_ABS(txpcb, s_old_txpcb) * 100;
    if (txfcf > s_old_txfcf)
    {
        temp1 = txfcf - s_old_txfcf;
    }
    else
    {
        temp1 = s_old_txfcf - txfcf;
    }

    if (rxfci > s_old_rxfci)
    {
        temp2 = rxfci - s_old_rxfci;
    }
    else
    {
        temp2 = s_old_rxfci - rxfci;
    }

    if (0 == temp1)
    {
        temppktlos1 = 0;
    }
    else
    {
        temppktlos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    temppktlos2 = rand() % (40 * nearpktloscfg + 1) + (nearpktloscfg * 980);
    temppktlos2 = temppktlos2 / 1000;

    if (0 != nearpktloscfg)
    {
        *PktLosNear = temppktlos2;
        g_vsnear_pklos = temppktlos2;
    }
    else
    {
        *PktLosNear =  temppktlos1;
    }

    if (1 == g_alarm_debug)
    {
        printf("near PktLosNear %f\n", *PktLosNear);
    }

    s_old_txfcf = txfcf;
    s_old_rxfci = rxfci;
    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_active_pktlos_far
 功能描述  : oam    VS层丢包测量远端丢包率
 输入参数  : oamvskey
 输出参数  : PktLosFar
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_active_pktlos_far(OAM_VS_KEY oamvskey,  float * PktLosFar)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG txfcb = 0;
    NBB_ULONG rxfcb = 0;
    static NBB_ULONG s_old_txfcf = 0;
    static NBB_ULONG s_old_rxfci = 0;
    static NBB_ULONG s_old_txfcb = 0;
    static NBB_ULONG s_old_rxfcb = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    float temppktlos1 = 0;
    float temppktlos2 = 0;
    NBB_USHORT farpktloscfg = 0;
    float temp3 = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_pktlos_far"));

    /*判断输入参数是否正确*/
    if (PktLosFar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosFar null error!"));
        goto EXIT_LABEL;
    }

    *PktLosFar = 0;
    usloop = oam_vs_find_active_oamid(oamvskey);

    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    /*计算远端丢包率*/
    /*LMM 发送*/
    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    
    /*LMM 接收*/
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;
    
    /*LMR 发送*/
    txfcb = g_stoam_activelmdata[usloop].lmtxpcb;
    
    /*LMR 接收*/
    rxfcb = g_stoam_activelmdata[usloop].lmrxpcl;

    farpktloscfg = g_stoam_local[usloop].farpklos;

    if (1 == g_alarm_debug)
    {
        printf("VS FAR :\n");
        printf("txfcb %d\n", txfcb);
        printf("s_old_txfcb %d\n", s_old_txfcb);
        printf("rxfcb %d\n", rxfcb);
        printf("s_old_rxfcb %d\n", s_old_rxfcb);
    }

    //*PktLosFar =(float)(SUB_ABS(txpcf, s_old_txpcf) - SUB_ABS(rxpcf, s_old_rxpcf))/SUB_ABS(txpcf, s_old_txpcf) * 100;
    if (txfcb > s_old_txfcb)
    {
        temp1 = txfcb - s_old_txfcb;
    }
    else
    {
        temp1 = s_old_txfcb - txfcb;
    }

    if (rxfcb > s_old_rxfcb)
    {
        temp2 = rxfcb - s_old_rxfcb;
    }
    else
    {
        temp2 = s_old_rxfcb - rxfcb;
    }

#if 0

    if (0 == temp2)
    {
        *PktLosFar = 0;
    }
    else
    {
        *PktLosFar = (float)(temp1 - temp2) / temp1 * 100;
    }

#endif

    if (0 == temp1)
    {
        temppktlos1 = 0;
    }
    else
    {
        temppktlos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    temppktlos2 = rand() % (40 * farpktloscfg + 1) + (farpktloscfg * 980);
    temppktlos2 = temppktlos2 / 1000;

    if (0 != farpktloscfg)
    {
        *PktLosFar = temppktlos2;
        g_vsfar_pklos = temppktlos2;
    }
    else
    {
        *PktLosFar =  temppktlos1;
    }

    if (1 == g_alarm_debug)
    {
        printf("far PktLosFar %f\n", *PktLosFar);
    }

    s_old_txfcb = txfcb;
    s_old_rxfcb = rxfcb;
    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_tx_cv_packs
 功能描述  : oam    VS层发包总计数
 输入参数  : oamvskey
 输出参数  : txpkt
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月11日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_tx_cv_packs(OAM_VS_KEY oamvskey, NBB_BYTE *txpkt)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_unm_alarm"));

    /*判断输入参数是否正确*/
    if (txpkt == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unm_alarm null error!"));
        goto EXIT_LABEL;
    }

    *txpkt = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *txpkt = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                           CV_CNT_TX_OAM + g_stoam_local[usoamindex].module_index));
    }
    else
    {
        *txpkt = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                           CV_CNT_TX_OAM + g_stoam_local[usoamindex].module_index));
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_packdelay_sec
 功能描述  : oam    VS层时延s
 输入参数  : oamvskey
 输出参数  : packdelay
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月14日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_packdelay_sec(OAM_VS_KEY oamvskey, uint64 *packdelay)
{
    NBB_USHORT uslogportid             = 0;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;
    if (packdelay == NULL)
    {
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    if ((oamvskey.SubBoardId > MAX_SUB_CARD_NUM))
    {
        goto EXIT_LABEL;
    }

    if (oamvskey.SlotId > MAX_SLOT_NUM)
    {
        goto EXIT_LABEL;
    }

    if (oamvskey.PhyPortId > 0x1f)
    {
        goto EXIT_LABEL;
    }

    /*根据输入key中物理端口及子卡号计算逻辑端口号*/
    uslogportid  = ((oamvskey.SubBoardId & 0x7) << 5) | (oamvskey.PhyPortId & 0x1f);

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VS_OAM)
        {
            continue;


        }

        /*如果查找到portinfo，则将portinfo中内容与传入参数比较，如果一致，
        说明找到了想要的条目，根据条目号去查寄存器信息*/
        if ((g_stoam_local[usoamactindex].vs_Portid == uslogportid)
            && (g_stoam_local[usoamactindex].vs_Slotid == oamvskey.SlotId))
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            delaycfg = g_stoam_local[usoamactindex].delay;
            oam_write_fpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspoffset
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延s，高32位是s，低32位是ns,一起组成当前时戳
            将读取到数值全部转换为ns计算*/
            
            /*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
                       - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000);*/
            delaycfg = g_stoam_local[usoamactindex].delay;

            if (0 == delaycfg)
            {
                *packdelay = rxstampb - txtimestampf;
                *packdelay = (*packdelay) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980); //取值范围为正负0.2以内
                *packdelay = (*packdelay) / 1000000000;
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vs_rx_cv_packs
 功能描述  : oam    VS层收包总计数
 输入参数  : oamvskey
 输出参数  : rxpkt
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月11日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vs_rx_cv_packs(OAM_VS_KEY oamvskey, NBB_BYTE *rxpkt)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_USHORT usoamindex                = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vs_unm_alarm"));

    /*判断输入参数是否正确*/
    if (rxpkt == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unm_alarm null error!"));
        goto EXIT_LABEL;
    }

    *rxpkt = 0;
    usoamindex = oam_vs_find_active_oamid(oamvskey);

    /*如果oamid为0xffff，表示查找失败*/
    if (usoamindex == 0xffff)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *rxpkt = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                           CV_CNT_RX_OAM + g_stoam_local[usoamindex].module_index));
    }
    else
    {
        *rxpkt = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                           CV_CNT_RX_OAM + g_stoam_local[usoamindex].module_index));
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}


/*****************************************************************************
 函 数 名  : oam_rsvp_find_active_oamindex_tx
 功能描述  : VP层根据key查找主动oam id,发方向
 输入参数  : oamvpkey
 输出参数  :
 返 回 值     : 条目号 ;
                          0xffff表示未查找到与输入相同端口配置的条目
                          其他值表示查到了对应条目
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT oam_rsvp_find_active_oamid_tx(OAM_RSVP_LSP_KEY *oamvpkey, NBB_ULONG *oamid)
{
    SPM_OAM_RSVP_CB_TX *pstrsvp_tx = NULL;
    OAM_RSVP_LSP_KEY strsvp_key;
    NBB_INT iret = ERROR;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_find_active_oamindex_tx"));
    if (NULL == oamvpkey)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the rsvp rx key!"));
        goto EXIT_LABEL;
    }

    if (NULL == oamid)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the oamid!"));
        goto EXIT_LABEL;
    }

    OS_MEMSET(&strsvp_key, 0, sizeof(OAM_RSVP_LSP_KEY));
    strsvp_key.Egress = oamvpkey->Egress;
    strsvp_key.Ingress = oamvpkey->Ingress;
    strsvp_key.LspId = oamvpkey->LspId;
    strsvp_key.TunnelId = oamvpkey->TunnelId;
    pstrsvp_tx = (SPM_OAM_RSVP_CB_TX *)AVLL_FIND(v_spm_shared->g_oamavltreebyrsvptx, &strsvp_key);

    if (NULL == pstrsvp_tx)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the rsvp rx key!"));
        goto EXIT_LABEL;
    }

    *oamid = pstrsvp_tx->active_oam_id;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : oam_rsvp_find_active_oamindex_rx
 功能描述  : VP层根据key查找主动oam id，收方向
 输入参数  : oamvpkey
 输出参数  :
 返 回 值     : 条目号 ;
                          0xffff表示未查找到与输入相同端口配置的条目
                          其他值表示查到了对应条目
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT oam_rsvp_find_active_oamid_rx(OAM_RSVP_LSP_KEY *oamvpkey,
                                      NBB_ULONG *oamid)
{
    SPM_OAM_RSVP_CB_RX *pstrsvp_rx = NULL;
    OAM_RSVP_LSP_KEY strsvp_key;
    NBB_INT iret = ERROR;

    if (NULL == oamvpkey)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the rsvp rx key!"));
        goto EXIT_LABEL;
    }

    if (NULL == oamid)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the oamid!"));
        goto EXIT_LABEL;
    }

    OS_MEMSET(&strsvp_key, 0, sizeof(OAM_RSVP_LSP_KEY));
    strsvp_key.Egress = oamvpkey->Egress;
    strsvp_key.Ingress = oamvpkey->Ingress;
    strsvp_key.LspId = oamvpkey->LspId;
    strsvp_key.TunnelId = oamvpkey->TunnelId;
    pstrsvp_rx = (SPM_OAM_RSVP_CB_RX *)AVLL_FIND(v_spm_shared->g_oamavltreebyrsvprx, &strsvp_key);

    if (NULL == pstrsvp_rx)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the rsvp rx key!"));
        goto EXIT_LABEL;
    }

    *oamid = pstrsvp_rx->active_oam_id;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}


/*****************************************************************************
 函 数 名  : oam_rsvp_find_active_oamindex_rx
 功能描述  : VP层根据key查找主动oam id，收方向
 输入参数  : oamvpkey
 输出参数  :
 返 回 值     : 条目号 ;
                          0xffff表示未查找到与输入相同端口配置的条目
                          其他值表示查到了对应条目
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT oam_rsvp_find_active_oamid_rx_for_pm(OAM_RSVP_LSP_KEY *oamvpkey,
        NBB_USHORT *oamindex)
{
    SPM_OAM_RSVP_CB_RX *pstrsvp_rx = NULL;
    OAM_RSVP_LSP_KEY strsvp_key;
    NBB_INT iret = ERROR;

    if (NULL == oamvpkey)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the rsvp rx key!"));
        goto EXIT_LABEL;
    }

    if ((NULL == oamindex))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the oamid!"));
        goto EXIT_LABEL;
    }

    OS_MEMSET(&strsvp_key, 0, sizeof(OAM_RSVP_LSP_KEY));
    strsvp_key.Egress = oamvpkey->Egress;
    strsvp_key.Ingress = oamvpkey->Ingress;
    strsvp_key.LspId = oamvpkey->LspId;
    strsvp_key.TunnelId = oamvpkey->TunnelId;
    pstrsvp_rx = (SPM_OAM_RSVP_CB_RX *)AVLL_FIND(v_spm_shared->g_oamavltreebyrsvprx, &strsvp_key);

    if (NULL == pstrsvp_rx)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the rsvp rx key!"));
        goto EXIT_LABEL;
    }

    *oamindex = pstrsvp_rx->oamindex;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_sd_alarm
 功能描述  : oam    VP层上报LOC告警
 输入参数  : oamvpkey
 输出参数  : sd_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_sd_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *sd_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_USHORT alarmtemp = 0;

    if (1 == g_dbg_ces_print_flag)
    {
        printf("oam_rsvp_loc_alarm  Ingress %d Egress %d TunnelId %d LspId %d\n", 
            oamvpkey.Ingress, oamvpkey.Egress, oamvpkey.TunnelId, oamvpkey.LspId);
    }

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_loc_alarm"));

    /*判断输入参数是否正确*/
    if (sd_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "sd_alarm null error!"));
        goto EXIT_LABEL;
    }

    *sd_alarm = 0;
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);

    /*如果oamid为0xffff，表示查找失败*/
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if ((g_stoam_local[usoamindex].vp_attrib == 1) && (g_stoam_local[usoamindex].cv_enable != 0))
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            alarmtemp = spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                       SD_OVERRIDE_OAM + g_stoam_local[usoamindex].module_index / 16);
            *sd_alarm = (alarmtemp >> g_stoam_local[usoamindex].module_index % 16)
                        & 0x1;
            *sd_alarm = (*sd_alarm == 0) ? 0 : 1;
        }
        else
        {
            alarmtemp = spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                       SD_OVERRIDE_OAM + g_stoam_local[usoamindex].module_index / 16);
            *sd_alarm = (alarmtemp >> g_stoam_local[usoamindex].module_index % 16)
                        & 0x1;
            *sd_alarm = (*sd_alarm == 0) ? 0 : 1;
        }

        ulrtn = SUCCESS;
    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_loc_alarm
 功能描述  : oam    VP层上报LOC告警
 输入参数  : oamvpkey
 输出参数  : loc_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_loc_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *loc_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE ais_alarm = 0; 
    NBB_BYTE lck_alarm = 0; 
    NBB_BYTE rdi_alarm = 0;
    NBB_BYTE tempalarm = 0;
    NBB_BYTE tempalarm2 = 0;
    NBB_BYTE uclocalslot = 0;

    if (1 == g_dbg_ces_print_flag)
    {
        printf("oam_rsvp_loc_alarm  Ingress %d Egress %d TunnelId %d LspId %d\n", 
            oamvpkey.Ingress, oamvpkey.Egress, oamvpkey.TunnelId, oamvpkey.LspId);
    }

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_loc_alarm"));

    /*判断输入参数是否正确*/
    if (loc_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "loc_alarm null error!"));
        goto EXIT_LABEL;
    }

    *loc_alarm = 0;
    oam_rsvp_lck_alarm(oamvpkey, &lck_alarm);
    oam_rsvp_ais_alarm(oamvpkey, &ais_alarm);
    oam_rsvp_rdi_alarm(oamvpkey, &rdi_alarm);
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);

    /*如果oamid为0xffff，表示查找失败*/
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if ((g_stoam_local[usoamindex].vp_attrib == 1) && (g_stoam_local[usoamindex].cv_enable != 0))
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            tempalarm = spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                       OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index);
            tempalarm2 = tempalarm & 0x10;

            *loc_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x10);
        }
        else
        {
            *loc_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x10);
        }

        *loc_alarm = (*loc_alarm == 0) ? 0 : 1;

        ulrtn = SUCCESS;
    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_lck_alarm
 功能描述  : oam    VP层上报LCK告警
 输入参数  : oamvpkey
 输出参数  : lck_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_lck_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *lck_alarm)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_ULONG uloamid = 0;
    NBB_BYTE     iret                        = ERROR;
    NBB_BYTE ucodloop = 0;
    NBB_BYTE lck_alar = 0;
    NBB_BYTE odloop = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT temp = 0;

    // NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_lck_alarm"));

    /*判断输入参数是否正确*/
    if (lck_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *lck_alarm = 0;
    iret = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usloop);

    if (SUCCESS != iret)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "vp key cannot be find."));
        goto EXIT_LABEL;
    }

    spm_get_moudle_base_addr(usloop, &base_addr);
    iret = spm_odindex_by_activeindex(usloop, &odloop);

    if (SUCCESS != iret)
    {
        goto EXIT_LABEL;
    }

    ucodloop = g_stoam_odlocal[odloop].ucOdLoop - 1;
    temp = spm_oam_16read(base_addr, OAM_LCK_ALARM_OFFSET);
    *lck_alarm = (temp >> ucodloop) & 0x1;
    iret = SUCCESS;
#if 0

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == uloamid)
        {
            usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

            switch (g_stoam_local[usoamactindex].bModuleSwitch)
            {
                case 0 :
                    /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
                    指的就是按需条目号*/
                    *lck_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                                           LCK_ALARM_OFFSET) & (0x1 << usloop));

                    if (1 == g_alarm_debug)
                    {
                        printf("VP LCK module 1 lck_alarm = %d usloop = %d\n", *lck_alarm, usloop);
                    }

                    break;

                case 1 :
                    *lck_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                                           LCK_ALARM_OFFSET) & (0x1 << usloop));

                    if (1 == g_alarm_debug)
                    {
                        printf("VP LCK module 2 lck_alarm = %d usloop = %d\n", *lck_alarm, usloop);
                    }

                    break;

                default:
                    break;
            }

            *lck_alarm = (*lck_alarm == 0) ? 0 : 1;
            iret = SUCCESS;
            goto EXIT_LABEL;
        }
    }

    if (OAM_MAX_NUM_OD == usloop)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find related active oam."));
        goto EXIT_LABEL;
    }

#endif
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_mmg_alarm
 功能描述  : oam    VP层上报CVMEG不匹配告警
 输入参数  : oamvpkey
 输出参数  : mmg_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_mmg_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *mmg_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG uloamid                = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_USHORT usloop = 0 ;
    NBB_BYTE ais_alarm = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_mmg_alarm"));

    /*判断输入参数是否正确*/
    if (mmg_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "mmg_alarm null error!"));
        goto EXIT_LABEL;
    }

    *mmg_alarm = 0;
    oam_rsvp_ais_alarm(oamvpkey, &ais_alarm);
    oam_rsvp_lck_alarm(oamvpkey, &lck_alarm);
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usloop);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "vp key cannot be find."));
        goto EXIT_LABEL;
    }

#if 0
    pstoamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &uloamid);

    if (NULL == pstoamcfg)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "vp key not find in the avll tree by oamid."));
        goto EXIT_LABEL;
    }

    usloop = pstoamcfg->oamindex;
#endif

    if (FALSE == g_stoam_local[usloop].bModuleSwitch)
    {
        *mmg_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usloop].module_index) & 0x4);
    }
    else
    {
        *mmg_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usloop].module_index) & 0x4);
    }

    if (1 == (ais_alarm || lck_alarm))
    {
        *mmg_alarm = 0;
    }
    else
    {
        *mmg_alarm = (*mmg_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_unm_alarm
 功能描述  : oam    VP层上报CVMEP不匹配告警
 输入参数  : oamvpkey
 输出参数  : unm_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_unm_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *unm_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE mmg_alarm = 0; 
    NBB_BYTE ais_alarm = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_unm_alarm"));

    /*判断输入参数是否正确*/
    if (unm_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unm_alarm null error!"));
        goto EXIT_LABEL;
    }

    *unm_alarm = 0;
    oam_rsvp_mmg_alarm(oamvpkey, &mmg_alarm);
    oam_rsvp_ais_alarm(oamvpkey, &ais_alarm);
    oam_rsvp_lck_alarm(oamvpkey, &lck_alarm);
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *unm_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x2);
    }
    else
    {
        *unm_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x2);
    }

    if (1 == (mmg_alarm || ais_alarm || lck_alarm))
    {
        *unm_alarm = 0;
    }
    else
    {
        *unm_alarm = (*unm_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_unp_alarm
 功能描述  : oam    VP层上报CV周期不匹配告警
 输入参数  : oamvpkey
 输出参数  : unp_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_unp_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *unp_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE unm_alarm = 0; 
    NBB_BYTE mmg_alarm = 0;
    NBB_BYTE ais_alarm = 0; 
    NBB_BYTE lck_alarm = 0;

    /*判断输入参数是否正确*/
    if (unp_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unp_alarm null error!"));
        goto EXIT_LABEL;
    }

    *unp_alarm = 0;

    oam_rsvp_unm_alarm(oamvpkey, &unm_alarm);
    oam_rsvp_mmg_alarm(oamvpkey, &mmg_alarm);
    oam_rsvp_ais_alarm(oamvpkey, &ais_alarm);
    oam_rsvp_lck_alarm(oamvpkey, &lck_alarm);
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *unp_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x8);
    }
    else
    {
        *unp_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x8);
    }

    if (1 == (unm_alarm || mmg_alarm || ais_alarm || lck_alarm))
    {
        *unp_alarm = 0;
    }
    else
    {
        *unp_alarm = (*unp_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_rdi_alarm
 功能描述  : oam    VP层上报RDI告警
 输入参数  : oamvpkey
 输出参数  : rdi_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_rdi_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *rdi_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE ais_alarm = 0;
    NBB_BYTE lck_alarm = 0;


    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_rdi_alarm"));

    /*判断输入参数是否正确*/
    if (rdi_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "rdi_alarm null error!"));
        goto EXIT_LABEL;
    }

    *rdi_alarm = 0;
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);
    oam_rsvp_ais_alarm(oamvpkey, &ais_alarm);
    oam_rsvp_lck_alarm(oamvpkey, &lck_alarm);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (1 == g_stoam_local[usoamindex].rdi_alarm_flag)
    {
        *rdi_alarm = 0;
        goto EXIT_LABEL;
    }
    else
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            *rdi_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                          OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x1);
        }
        else
        {
            *rdi_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                          OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x1);
        }

        if (1 == (ais_alarm || lck_alarm))
        {
            *rdi_alarm = 0;
        }
        else
        {
            *rdi_alarm = (*rdi_alarm == 0) ? 0 : 1;
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_ais_alarm
 功能描述  : oam    VP层上报ais告警
 输入参数  : oamvpkey
 输出参数  : ais_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_ais_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *ais_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_ais_alarm"));

    /*判断输入参数是否正确*/
    if (ais_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "ais_alarm null error!"));
        goto EXIT_LABEL;
    }

    *ais_alarm = 0;
    oam_rsvp_lck_alarm(oamvpkey, &lck_alarm);
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (1 == g_stoam_local[usoamindex].ais_alarm_flag)
    {
        *ais_alarm = 0;
        goto EXIT_LABEL;
    }
    else
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            *ais_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                    OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x20);
        }
        else
        {
            *ais_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                    OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x20);
        }

        if (1 == lck_alarm)
        {
            *ais_alarm = 0;
        }
        else
        {
            *ais_alarm = (*ais_alarm == 0) ? 0 : 1;
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_lblos_alarm
 功能描述  : oam    VP层上报lblos告警
 输入参数  : oamvpkey
 输出参数  : lblos_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_lblos_alarm(OAM_RSVP_LSP_KEY oamvpkey, NBB_BYTE *lblos_alarm)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odloop = 0;
    NBB_BYTE ucodloop = 0;
    NBB_BYTE temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vp_lblos_alarm"));

    /*判断输入参数是否正确*/
    if (lblos_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lblos_alarm null error!"));
        goto EXIT_LABEL;
    }

    *lblos_alarm = 0;
    
    /*属于按需帧告警，直接查按需数组*/
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usloop);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
        goto EXIT_LABEL;
    }

//    spm_get_oamindex_by_oamid(oamid, &usloop);
    spm_get_moudle_base_addr(usloop, &base_addr);
    ulrtn = spm_odindex_by_activeindex(usloop, &odloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ucodloop = g_stoam_odlocal[odloop].ucOdLoop - 1;

    if (NULL != g_stoam_odlocal[odloop].pLoopFrame)
    {
        if (1 == g_stoam_odlocal[odloop].pLoopFrame->byte1.send_enable)
        {
            temp = spm_oam_16read(base_addr, OAM_LBLOS_ALARM_OFFSET);
            *lblos_alarm = (temp >> ucodloop) & 0x1;
        }
    }

    ulrtn = SUCCESS;

#if 0

    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            switch (g_stoam_local[usoamactindex].bModuleSwitch)
            {
                case 0 :

                    /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
                    指的就是按需条目号*/
                    if (NULL == g_stoam_odlocal[usloop].pLoopFrame)
                    {
                        goto EXIT_LABEL;
                    }

                    *lblos_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                              LBLOS_ALARM_OFFSET) & (0x1 << usloop));
                    break;

                case 1 :
                    if (NULL == g_stoam_odlocal[usloop].pLoopFrame)
                    {
                        goto EXIT_LABEL;
                    }

                    *lblos_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                              LBLOS_ALARM_OFFSET) & (0x1 << usloop));
                    break;

                default:
                    break;
            }

            *lblos_alarm = (*lblos_alarm == 0) ? 0 : 1;
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }
    }

    if (usloop == OAM_MAX_NUM_OD)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "do not find lb alarm."));
    }

#endif
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_tx_cv_packs
 功能描述  : oam    VP层发送cv帧计数
 输入参数  : OamRsvpKey
 输出参数  : tx_stat
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_tx_cv_packs(OAM_RSVP_LSP_KEY oamvpkey, NBB_USHORT *tx_stat)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usstat                    = 0;
    NBB_USHORT usoamindex                = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_tx_cv_packs"));

    /*判断输入参数是否正确*/
    if (tx_stat == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "tx_stat null error!"));
        goto EXIT_LABEL;
    }

    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usoamindex);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *tx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                           CV_CNT_TX_OAM + g_stoam_local[usoamindex].module_index));
    }
    else
    {
        *tx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                           CV_CNT_TX_OAM + g_stoam_local[usoamindex].module_index));
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_rx_cv_packs
 功能描述  : oam    VP层接收cv帧计数
 输入参数  : OamRsvpKey
 输出参数  : rx_stat
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_rx_cv_packs(OAM_RSVP_LSP_KEY oamvpkey, NBB_USHORT *rx_stat)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_ULONG usoamindex                = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_rx_cv_packs"));

    /*判断输入参数是否正确*/
    if (rx_stat == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "rx_stat null error!"));
        goto EXIT_LABEL;
    }

    ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &usoamindex);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *rx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                           CV_CNT_RX_OAM + g_stoam_local[usoamindex].module_index));
    }
    else
    {
        *rx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                           CV_CNT_RX_OAM + g_stoam_local[usoamindex].module_index));
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_packdelay
 功能描述  : oam    VP层时延s
 输入参数  : oamvpkey
 输出参数  : packdelay
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_packdelay(OAM_RSVP_LSP_KEY oamvpkey, uint64 * packdelay)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_packdelay"));

    /*判断输入参数是否正确*/
    if (packdelay == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelay null error!"));
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*packdelay = (rxstampb - txtimestampf) ;*/
            //*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //           - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000) ;
            delaycfg = g_stoam_local[usoamactindex].delay;

            if (0 == delaycfg)
            {
                *packdelay = rxstampb - txtimestampf;
                *packdelay = (*packdelay) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980);
                *packdelay = (*packdelay) / 1000000000;
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_rsvp_packdelay_ns
功能描述  : oam    VP层时延ns
输入参数  : oamvpkey
输出参数  : packdelay
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                     SUCCESS:根据key值查找到了对应的loc_alarm信息
调用函数  :
被调函数  :

修改历史      :
1.日    期   : 2013年10月14日
  作    者   : 胡晓露
  审 核 人   : #
  修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_packdelay_ns(OAM_RSVP_LSP_KEY oamvpkey, uint64 * packdelay)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress = 0;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_packdelay"));

    /*判断输入参数是否正确*/
    if (packdelay == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelay null error!"));
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));
            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*packdelay = (rxstampb - txtimestampf) ;*/
            //*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //          - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000) ;
            delaycfg = g_stoam_local[usoamactindex].delay;

            if ((NULL != g_stoam_odlocal[usloop].pDmFrame) && (0 == delaycfg))
            {
                *packdelay = rxstampb - txtimestampf - (g_stoam_odlocal[usloop].pDmFrame->frame_length + 30) * 32;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980);
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
        return SUCCESS;

}

/*****************************************************************************
函 数 名  : oam_rsvp_packdelayvar
功能描述  : oam    VP层时延变化量s
输入参数  : oamvpkey
输出参数  : packdelayvar
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                      SUCCESS:根据key值查找到了对应的loc_alarm信息
调用函数  :
被调函数  :

修改历史      :
 1.日    期   : 2013年10月15日
   作    者   : 胡晓露
   审 核 人   : #
   修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_packdelayvar(OAM_RSVP_LSP_KEY oamvpkey, uint64 * packdelayvar)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    static uint64    oldTimevar               = 0;
    NBB_ULONG ulregbaseaddress = 0;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT vardelaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_packdelayvar"));

    /*判断输入参数是否正确*/
    if (packdelayvar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelayvar null error!"));
        goto EXIT_LABEL;
    }

    *packdelayvar = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   RX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + (uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                    RX_TIMESTPF_OFFSET + 3 + lspoffset);

            txstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                               TX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*/
            //*packdelayvar = ((rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000))
            //        - ((txstampb&TIME_STAMP_MASK + ((txstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (rxtimestampf&TIME_STAMP_MASK + ((rxtimestampf>>32)&TIME_STAMP_MASK)*1000000000) );
            //*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf);
            vardelaycfg = g_stoam_local[usoamactindex].vardelay;

            if (0 == vardelaycfg)
            {
                if ((rxstampb - txtimestampf) > oldTimevar)
                {
                    *packdelayvar = (rxstampb - txtimestampf) - oldTimevar;
                }
                else
                {
                    *packdelayvar =  oldTimevar - (rxstampb - txtimestampf);
                }

                oldTimevar = rxstampb - txtimestampf;
                *packdelayvar = (*packdelayvar) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelayvar = rand() % (40 * vardelaycfg + 1) + (vardelaycfg * 980);
                *packdelayvar = (*packdelayvar) / 1000000000;
            }

            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_rsvp_packdelayvar_ns
功能描述  : oam    VP层时延变化量ns
输入参数  : oamvpkey
输出参数  : packdelayvar
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                     SUCCESS:根据key值查找到了对应的loc_alarm信息
调用函数  :
被调函数  :

修改历史      :
1.日    期   : 2013年10月15日
  作    者   : 胡晓露
  审 核 人   : #
  修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_packdelayvar_ns(OAM_RSVP_LSP_KEY oamvpkey, uint64 * packdelayvar)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    static uint64    oldTimevar               = 0;
    NBB_ULONG ulregbaseaddress = 0;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT vardelaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_packdelayvar_ns"));

    /*判断输入参数是否正确*/
    if (packdelayvar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "packdelayvar null error!"));
        goto EXIT_LABEL;
    }

    *packdelayvar = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;
            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   RX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + (uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                    RX_TIMESTPF_OFFSET + 3 + lspoffset);

            txstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                               TX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*/
            // *packdelayvar = ((rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //         - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000))
            //         - ((txstampb&TIME_STAMP_MASK + ((txstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //         - (rxtimestampf&TIME_STAMP_MASK + ((rxtimestampf>>32)&TIME_STAMP_MASK)*1000000000) );
            //*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf);
            vardelaycfg = g_stoam_local[usoamactindex].vardelay;

            if (0 == vardelaycfg)
            {
                if ((rxstampb - txtimestampf) > oldTimevar)
                {
                    *packdelayvar = (rxstampb - txtimestampf) - oldTimevar;
                }
                else
                {
                    *packdelayvar =  oldTimevar - (rxstampb - txtimestampf);
                }

                oldTimevar = rxstampb - txtimestampf;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelayvar = rand() % (40 * vardelaycfg + 1) + (vardelaycfg * 980);
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_pktlos_near
 功能描述  : oam    VP层丢包测量近端丢包率
 输入参数  : oamvpkey
 输出参数  : PktLosNear
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_pktlos_near(OAM_RSVP_LSP_KEY oamvpkey,  float * PktLosNear)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG txpcf = 0;
    NBB_ULONG rxpcf = 0;
    NBB_ULONG txpcb = 0;
    NBB_ULONG rxpcl = 0;
    static NBB_ULONG s_old_txpcf = 0;
    static NBB_ULONG s_old_rxpcf = 0;
    static NBB_ULONG s_old_txpcb = 0;
    static NBB_ULONG s_old_rxpcl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosNear == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosNear null error!"));
        goto EXIT_LABEL;
    }

    *PktLosNear = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            usodoamid   = usloop;
            spm_od_lm_task(usloop);
            
            /*LMM 发送*/
            txpcf = g_stoam_odlmdata[usloop].lmtxpcf;
            
            /*LMM 接收*/
            rxpcf = g_stoam_odlmdata[usloop].lmrxpcf;
            
            /*LMR 发送*/
            txpcb = g_stoam_odlmdata[usloop].lmtxpcb;
            
            /*LMR 接收*/
            rxpcl = g_stoam_odlmdata[usloop].lmrxpcl;

            //*PktLosNear = (float)(SUB_ABS(txpcb, s_old_txpcb) - 
            //SUB_ABS(rxpcl, s_old_rxpcl))/SUB_ABS(txpcb, s_old_txpcb) * 100;
            if (1 == g_alarm_debug)
            {
                printf("RSVP ONDEMAN NEAR:\n");
                printf("txpcb : %d\n", txpcb);
                printf("s_old_txpcb %d\n", s_old_txpcb);
                printf("rxpcl %d\n", rxpcl);
                printf("s_old_rxpcl %d\n", s_old_rxpcl);
            }

            if (txpcb > s_old_txpcb)
            {
                temp1 = txpcb - s_old_txpcb;
            }
            else
            {
                temp1 = s_old_txpcb - txpcb;
            }

            if (rxpcl > s_old_rxpcl)
            {
                temp2 = rxpcl - s_old_rxpcl;
            }
            else
            {
                temp2 = s_old_rxpcl - rxpcl;
            }

            *PktLosNear = (float)(temp1 - temp2) / temp1 * 100;

            if (1 == g_alarm_debug)
            {
                printf("PktLosNear : %f\n", *PktLosNear);
            }

            s_old_txpcb = txpcb;
            s_old_rxpcl = rxpcl;

            /*计算近端丢包率*/
            //* PktLosNear = SUB_ABS(g_stoam_activelmdata[usodoamid].lmtxpcb_new,
            //g_stoam_activelmdata[usodoamid].lmtxpcb_old)
            //                      -SUB_ABS(g_stoam_activelmdata[usodoamid].lmrxpcl_new,
            //g_stoam_activelmdata[usodoamid].lmrxpcl_old);
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_pktlos_far
 功能描述  : oam    VP层丢包测量远端丢包率
 输入参数  : oamvpkey
 输出参数  : PktLosFar
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_pktlos_far(OAM_RSVP_LSP_KEY oamvpkey,  float * PktLosFar)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG txpcf = 0;
    NBB_ULONG rxpcf = 0;
    NBB_ULONG txpcb = 0;
    NBB_ULONG rxpcl = 0;
    static NBB_ULONG s_old_txpcf = 0;
    static NBB_ULONG s_old_rxpcf = 0;
    static NBB_ULONG s_old_txpcb = 0;
    static NBB_ULONG s_old_rxpcl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosFar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosFar null error!"));
        goto EXIT_LABEL;
    }

    *PktLosFar = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VP_OAM)
        {
            continue;


        }

        ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            usodoamid   = usloop;
            spm_od_lm_task(usloop);
            
            /*LMM 发送*/
            txpcf = g_stoam_odlmdata[usloop].lmtxpcf;
            
            /*LMM 接收*/
            rxpcf = g_stoam_odlmdata[usloop].lmrxpcf;
            
            /*LMR 发送*/
            txpcb = g_stoam_odlmdata[usloop].lmtxpcb;
            
            /*LMR 接收*/
            rxpcl = g_stoam_odlmdata[usloop].lmrxpcl;

            if (1 == g_alarm_debug)
            {
                printf("RSVP ONDEMAN FAR:\n");
                printf("txpcf : %d\n", txpcf);
                printf("s_old_txpcf : %d\n", s_old_txpcf);
                printf("rxpcf : %d \n", rxpcf);
                printf("s_old_rxpcf : %d\n", s_old_rxpcf);
            }

            if (txpcf > s_old_txpcf)
            {
                temp1 = txpcf - s_old_txpcf;
            }
            else
            {
                temp1 = s_old_txpcf - txpcf;
            }

            if (rxpcf > s_old_rxpcf)
            {
                temp2 = rxpcf - s_old_rxpcf;
            }
            else
            {
                temp2 = s_old_rxpcf - rxpcf;
            }

            *PktLosFar = (float)(temp1 - temp2) / temp1 * 100;

            if (1 == g_alarm_debug)
            {
                printf("PktLosFar : %f", *PktLosFar);
            }

            //*PktLosFar =(float)(SUB_ABS(txpcf, s_old_txpcf) - 
            //SUB_ABS(rxpcf, s_old_rxpcf))/SUB_ABS(txpcf, s_old_txpcf) * 100;
            s_old_txpcf = txpcf;
            s_old_rxpcf = rxpcf;
            
            /*计算远端丢包率*/
            //* PktLosFar = SUB_ABS(g_stoam_activelmdata[usodoamid].lmtxpcf_new,
            //g_stoam_activelmdata[usodoamid].lmtxpcf_old)
            //                    -SUB_ABS(g_stoam_activelmdata[usodoamid].lmrxpcf_new, 
            //g_stoam_activelmdata[usodoamid].lmrxpcf_old);
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
}


/*****************************************************************************
 函 数 名  : oam_rsvp_active_pktlos_near
 功能描述  : oam    VP层丢包测量近端丢包率
 输入参数  : oamvpkey
 输出参数  : PktLosNear
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_active_pktlos_near(OAM_RSVP_LSP_KEY oamvpkey,  float * PktLosNear)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG txfcb = 0;
    NBB_ULONG rxfcb = 0;
    static NBB_ULONG s_old_txfcf = 0;
    static NBB_ULONG s_old_rxfci = 0;
    static NBB_ULONG s_old_txfcb = 0;
    static NBB_ULONG s_old_rxfcb = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    float temppktlos1 = 0;
    float temppktlos2 = 0;
    NBB_USHORT nearpktloscfg = 0;
    float temp3 = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosNear == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosNear null error!"));
        goto EXIT_LABEL;
    }

    *PktLosNear = 0;
    ulrtn = oam_rsvp_find_active_oamid_rx_for_pm(&oamvpkey, &usloop);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
        goto EXIT_LABEL;
    }

#if 0
    pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pstoamcfg)
    {
        goto EXIT_LABEL;
    }

    usloop = pstoamcfg->oamindex;
#endif

    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    /*计算近端丢包率*/
    /*LMM 发送*/
    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    
    /*LMM 接收*/
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;
    
    /*LMR 发送*/
    txfcb = g_stoam_activelmdata[usloop].lmtxpcb;
    
    /*LMR 接收*/
    rxfcb = g_stoam_activelmdata[usloop].lmrxpcl;
    nearpktloscfg = g_stoam_local[usloop].nearpklos;

    if (1 == g_alarm_debug)
    {
        printf("RSVP NEAR :\n");
        printf("txfcf %d\n", txfcf);
        printf("s_old_txfcf %d\n", s_old_txfcf);
        printf("rxfci %d\n", rxfci);
        printf("s_old_rxfci %d\n", s_old_rxfci);
    }

    //*PktLosNear = (float)(SUB_ABS(txpcb, s_old_txpcb) - 
    //SUB_ABS(rxpcl, s_old_rxpcl))/SUB_ABS(txpcb, s_old_txpcb) * 100;
    if (txfcf > s_old_txfcf)
    {
        temp1 = txfcf - s_old_txfcf;
    }
    else
    {
        temp1 = s_old_txfcf - txfcf;
    }

    if (rxfci > s_old_rxfci)
    {
        temp2 = rxfci - s_old_rxfci;
    }
    else
    {
        temp2 = s_old_rxfci - rxfci;
    }

#if 0

    if (0 == temp2)
    {
        *PktLosNear = 0;
    }
    else
    {
        *PktLosNear = (float)(temp1 - temp2) / temp1 * 100;
    }

#endif

    if (0 == temp1)
    {
        temppktlos1 = 0;
    }
    else
    {
        temppktlos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    temppktlos2 = rand() % (40 * nearpktloscfg + 1) + (nearpktloscfg * 980);
    temppktlos2 = temppktlos2 / 1000;

    if (0 != nearpktloscfg)
    {
        *PktLosNear = temppktlos2;
        g_vpnear_pklos = temppktlos2;
    }
    else
    {
        *PktLosNear =  temppktlos1;
    }

    if (1 == g_alarm_debug)
    {
        printf("near PktLosNear %f\n", *PktLosNear);
    }

    s_old_txfcf = txfcf;
    s_old_rxfci = rxfci;
    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_rsvp_active_pktlos_far
 功能描述  : oam    VP层丢包测量远端丢包率
 输入参数  : oamvpkey
 输出参数  : PktLosFar
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_rsvp_active_pktlos_far(OAM_RSVP_LSP_KEY oamvpkey,  float * PktLosFar)
{
    NBB_USHORT usloop                     = 0;
    NBB_ULONG ulloop = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG txfcb = 0;
    NBB_ULONG rxfcb = 0;
    static NBB_ULONG s_old_txfcf = 0;
    static NBB_ULONG s_old_rxfci = 0;
    static NBB_ULONG s_old_txfcb = 0;
    static NBB_ULONG s_old_rxfcb = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    float temppktlos1 = 0;
    float temppktlos2 = 0;
    NBB_USHORT farpktloscfg = 0;
    float temp3 = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_rsvp_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosFar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosFar null error!"));
        goto EXIT_LABEL;
    }

    *PktLosFar = 0;
    ulrtn = oam_rsvp_find_active_oamid_rx(&oamvpkey, &ulloop);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
        goto EXIT_LABEL;
    }
    usloop = (NBB_USHORT)ulloop;
#if 0
    pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pstoamcfg)
    {
        goto EXIT_LABEL;
    }

    usloop = pstoamcfg->oamindex;
#endif

    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    /*LMM 发送*/
    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    
    /*LMM 接收*/
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;
    
    /*LMR 发送*/
    txfcb = g_stoam_activelmdata[usloop].lmtxpcb;
    
    /*LMR 接收*/
    rxfcb = g_stoam_activelmdata[usloop].lmrxpcl;
    farpktloscfg = g_stoam_local[usloop].farpklos;

    if (1 == g_alarm_debug)
    {
        printf("RSVP FAR :\n");
        printf("txfcb %d\n", txfcb);
        printf("s_old_txfcb %d\n", s_old_txfcb);
        printf("rxfcb %d\n", rxfcb);
        printf("s_old_rxfcb %d\n", s_old_rxfcb);
    }

    //*PktLosFar =(float)(SUB_ABS(txpcf, s_old_txpcf) - SUB_ABS(rxpcf, s_old_rxpcf))/SUB_ABS(txpcf, s_old_txpcf) * 100;
    if (txfcb > s_old_txfcb)
    {
        temp1 = txfcb - s_old_txfcb;
    }
    else
    {
        temp1 = s_old_txfcb - txfcb;
    }

    if (rxfcb > s_old_rxfcb)
    {
        temp2 = rxfcb - s_old_rxfcb;
    }
    else
    {
        temp2 = s_old_rxfcb - rxfcb;
    }

#if 0

    if (0 == temp2)
    {
        *PktLosFar = 0;
    }
    else
    {
        *PktLosFar = (float)(temp1 - temp2) / temp1 * 100;
    }

#endif

    if (0 == temp1)
    {
        temppktlos1 = 0;
    }
    else
    {
        temppktlos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    temppktlos2 = rand() % (40 * farpktloscfg + 1) + (farpktloscfg * 980);
    temppktlos2 = temppktlos2 / 1000;

    if (0 != farpktloscfg)
    {
        *PktLosFar = temppktlos2;
        g_vpfar_pklos = temppktlos2;
    }
    else
    {
        *PktLosFar =  temppktlos1;
    }

    if (1 == g_alarm_debug)
    {
        printf(" PktLosFar %f\n", *PktLosFar);
    }

    s_old_txfcb = txfcb;
    s_old_rxfcb = rxfcb;
    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_find_active_oamid
 功能描述  : VC层根据key查找主动oam id
 输入参数  : oamvckey
 输出参数  :
 返 回 值     : 条目号 ;
                          0xffff表示未查找到与输入相同端口配置的条目
                          其他值表示查到了对应条目
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT oam_vc_find_active_oamid(OAM_VC_KEY oamvckey, NBB_ULONG *oamid)
{
    NBB_ULONG usoamid                  = 0XFFFF;
    NBB_USHORT usloop                     = 0;
    SPM_OAM_PW_CB *pstpwcfg = NULL;
    SPM_PW_KEY stpwkey;
    NBB_INT iret = ERROR;

    if (NULL == oamid)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find the oamid!"));
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stpwkey, 0, sizeof(SPM_PW_KEY));
    stpwkey.peer_ip = oamvckey.PeerIp;
    stpwkey.vc_id = oamvckey.VcId;
    stpwkey.vc_type = oamvckey.VcType;
    pstpwcfg = (SPM_OAM_PW_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebypw, &stpwkey);

    if (NULL == pstpwcfg)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not find PW KEY!"));
        goto EXIT_LABEL;
    }

    *oamid = pstpwcfg->active_oam_id;
    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}

/*****************************************************************************
 函 数 名  : oam_vc_sd_alarm
 功能描述  : oam    Vc层上报sd告警
 输入参数  : oamvckey
 输出参数  : sd_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_sd_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *sd_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_USHORT alarmtemp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_loc_alarm"));

    /*判断输入参数是否正确*/
    if (sd_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "sd_alarm null error!"));
        goto EXIT_LABEL;
    }

    *sd_alarm = 0;
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if ((g_stoam_local[usoamindex].vp_attrib == 1) && (g_stoam_local[usoamindex].cv_enable != 0))
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            alarmtemp = spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                       SD_OVERRIDE_OAM + g_stoam_local[usoamindex].module_index / 16);
            *sd_alarm = (alarmtemp >> g_stoam_local[usoamindex].module_index % 16)
                        & 0x1;
            *sd_alarm = (*sd_alarm == 0) ? 0 : 1;
        }
        else
        {
            alarmtemp = spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                       SD_OVERRIDE_OAM + g_stoam_local[usoamindex].module_index / 16);
            *sd_alarm = (alarmtemp >> g_stoam_local[usoamindex].module_index % 16)
                        & 0x1;
            *sd_alarm = (*sd_alarm == 0) ? 0 : 1;
        }

        ulrtn = SUCCESS;
    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_loc_alarm
 功能描述  : oam    Vc层上报LOC告警
 输入参数  : oamvckey
 输出参数  : loc_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_loc_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *loc_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE ais_alarm = 0;
    NBB_BYTE lck_alarm = 0;
    NBB_BYTE rdi_alarm = 0;
    NBB_BYTE uclocalslot = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_loc_alarm"));

    /*判断输入参数是否正确*/
    if (loc_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "loc_alarm null error!"));
        goto EXIT_LABEL;
    }

    *loc_alarm = 0;
    oam_vc_ais_alarm(oamvckey, &ais_alarm);
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    oam_vc_rdi_alarm(oamvckey, &rdi_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if ((g_stoam_local[usoamindex].vp_attrib == 1) && (g_stoam_local[usoamindex].cv_enable != 0))
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            *loc_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                         OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x10);
        }
        else
        {
            *loc_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                         OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x10);
        }

        *loc_alarm = (*loc_alarm == 0) ? 0 : 1;
        ulrtn = SUCCESS;
    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_lck_alarm
 功能描述  : oam    VC层上报LCK告警
 输入参数  : oamvckey
 输出参数  : lck_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_lck_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *lck_alarm)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_USHORT tempalarm = 0;
    NBB_BYTE temp = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odloop = 0;
    NBB_BYTE ucodloop = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_lck_alarm"));

    /*判断输入参数是否正确*/
    if (lck_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *lck_alarm = 0;
    ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
        goto EXIT_LABEL;
    }

//    spm_get_oamindex_by_oamid(oamid, &usloop);
    spm_get_moudle_base_addr(usloop, &base_addr);
    ulrtn = spm_odindex_by_activeindex(usloop, &odloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ucodloop = g_stoam_odlocal[odloop].ucOdLoop - 1;
    temp = spm_oam_16read(base_addr, OAM_LCK_ALARM_OFFSET);
    *lck_alarm = (temp >> ucodloop) & 0x1;
    ulrtn = SUCCESS;
#if 0

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;


        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
                指的就是按需条目号*/
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                tempalarm = spm_oam_16read(OAM_REG_BASE1_ADDRESS, LCK_ALARM_OFFSET);
                tempflag = (g_stoam_odlocal[usloop].ucOdLoop - 1);
                *lck_alarm = (tempalarm << tempflag) & 0x1;
                *lck_alarm = (*lck_alarm == 0) ? 0 : 1;

                if (1 == g_alarm_debug)
                {
                    printf("VC LCK module 1 lck_alarm = %d usloop = %d\n", *lck_alarm, usloop);
                }
            }
            else
            {
                tempalarm = spm_oam_16read(OAM_REG_BASE1_ADDRESS, LCK_ALARM_OFFSET);
                tempflag = (g_stoam_odlocal[usloop].ucOdLoop - 1);
                *lck_alarm = (tempalarm << tempflag) & 0x1;
                *lck_alarm = (*lck_alarm == 0) ? 0 : 1;

                if (1 == g_alarm_debug)
                {
                    printf("VC LCK module 2 lck_alarm = %d usloop = %d\n", *lck_alarm, usloop);
                }
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

#endif
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_mmg_alarm
 功能描述  : oam    VC层上报CVMEG不匹配告警
 输入参数  : oamvckey
 输出参数  : mmg_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_mmg_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *mmg_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE ais_alarm = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_mmg_alarm"));

    /*判断输入参数是否正确*/
    if (mmg_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "mmg_alarm null error!"));
        goto EXIT_LABEL;
    }

    *mmg_alarm = 0;
    oam_vc_ais_alarm(oamvckey, &ais_alarm);
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *mmg_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x4);
    }
    else
    {
        *mmg_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x4);
    }

    if (1 == (ais_alarm || lck_alarm))
    {
        *mmg_alarm = 0;
    }
    else
    {
        *mmg_alarm = (*mmg_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_unm_alarm
 功能描述  : oam    VC层上报CVMEP不匹配告警
 输入参数  : oamvckey
 输出参数  : unm_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_unm_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *unm_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE mmg_alarm = 0; 
    NBB_BYTE ais_alarm = 0; 
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_unm_alarm"));

    /*判断输入参数是否正确*/
    if (unm_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unm_alarm null error!"));
        goto EXIT_LABEL;
    }

    *unm_alarm = 0;
    oam_vc_mmg_alarm(oamvckey, &mmg_alarm);
    oam_vc_ais_alarm(oamvckey, &ais_alarm);
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *unm_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x2);
    }
    else
    {
        *unm_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x2);
    }

    if (1 == (mmg_alarm || ais_alarm || lck_alarm))
    {
        *unm_alarm = 0;
    }
    else
    {
        *unm_alarm = (*unm_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_unp_alarm
 功能描述  : oam    VC层上报CV周期不匹配告警
 输入参数  : oamvckey
 输出参数  : unp_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_unp_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *unp_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE unm_alarm = 0;
    NBB_BYTE mmg_alarm = 0; 
    NBB_BYTE ais_alarm = 0; 
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_unp_alarm"));

    /*判断输入参数是否正确*/
    if (unp_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "unp_alarm null error!"));
        goto EXIT_LABEL;
    }

    *unp_alarm = 0;
    oam_vc_unm_alarm(oamvckey, &unm_alarm);
    oam_vc_mmg_alarm(oamvckey, &mmg_alarm);
    oam_vc_ais_alarm(oamvckey, &ais_alarm);
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *unp_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x8);
    }
    else
    {
        *unp_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x8);
    }

    if (1 == (unm_alarm || mmg_alarm || ais_alarm || lck_alarm))
    {
        *unp_alarm = 0;
    }
    else
    {
        *unp_alarm = (*unp_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_rdi_alarm
 功能描述  : oam    VC层上报RDI告警
 输入参数  : oamvckey
 输出参数  : rdi_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_rdi_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *rdi_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE loc_alarm = 0; 
    NBB_BYTE ais_alarm = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_rdi_alarm"));

    /*判断输入参数是否正确*/
    if (rdi_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "rdi_alarm null error!"));
        goto EXIT_LABEL;
    }

    *rdi_alarm = 0;
    oam_vc_ais_alarm(oamvckey, &ais_alarm);
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *rdi_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x1);
    }
    else
    {
        *rdi_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x1);
    }

    if (1 == (ais_alarm || lck_alarm))
    {
        *rdi_alarm = 0;
    }
    else
    {
        *rdi_alarm = (*rdi_alarm == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_csf_alarm
 功能描述  : oam    VC层上报RDI告警
 输入参数  : oamvckey
 输出参数  : rdi_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_csf_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *csf_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_USHORT temp = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_csf_alarm"));

    /*判断输入参数是否正确*/
    if (csf_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "csf_alarm null error!"));
        goto EXIT_LABEL;
    }

    *csf_alarm = 0;
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        temp = (spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x800);
    }
    else
    {
        temp = (spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                               OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x800);
    }

    if (1 == lck_alarm)
    {
        *csf_alarm = 0;
    }
    else
    {
        *csf_alarm = (temp == 0) ? 0 : 1;
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_ais_alarm
 功能描述  : oam    Vc层上报ais告警
 输入参数  : oamvckey
 输出参数  : ais_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_ais_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *ais_alarm)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;
    NBB_BYTE lck_alarm = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_ais_alarm"));

    /*判断输入参数是否正确*/
    if (ais_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "ais_alarm null error!"));
        goto EXIT_LABEL;
    }

    *ais_alarm = 0;
    oam_vc_lck_alarm(oamvckey, &lck_alarm);
    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (1 == g_stoam_local[usoamindex].ais_alarm_flag)
    {
        *ais_alarm = 0;
        goto EXIT_LABEL;
    }
    else
    {
        if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
        {
            *ais_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                         OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x20);
        }
        else
        {
            *ais_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                         OAM_ALARM_OFFSET_READ + g_stoam_local[usoamindex].module_index) & 0x20);
        }

        if (1 == lck_alarm)
        {
            *ais_alarm = 0;
        }
        else
        {
            *ais_alarm = (*ais_alarm == 0) ? 0 : 1;
        }
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_lblos_alarm
 功能描述  : oam    VC层上报lblos告警
 输入参数  : oamvckey
 输出参数  : lblos_alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_lblos_alarm(OAM_VC_KEY oamvckey, NBB_BYTE *lblos_alarm)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odloop = 0;
    NBB_BYTE ucodloop = 0;
    NBB_USHORT temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_lblos_alarm"));

    /*判断输入参数是否正确*/
    if (lblos_alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lblos_alarm null error!"));
        goto EXIT_LABEL;
    }

    *lblos_alarm = 0;

    ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
        goto EXIT_LABEL;
    }

//    spm_get_oamindex_by_oamid(oamid, &usloop);
    spm_get_moudle_base_addr(usloop, &base_addr);
    ulrtn = spm_odindex_by_activeindex(usloop, &odloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ucodloop = g_stoam_odlocal[odloop].ucOdLoop - 1;

    if (NULL != g_stoam_odlocal[odloop].pLoopFrame)
    {
        if (1 == g_stoam_odlocal[odloop].pLoopFrame->byte1.send_enable)
        {
            temp = spm_oam_16read(base_addr, OAM_LBLOS_ALARM_OFFSET);
            *lblos_alarm = (temp >> ucodloop) & 0x1;
        }
    }

    ulrtn = SUCCESS;
#if 0

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;


        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
                指的就是按需条目号*/
            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                if (NULL == g_stoam_odlocal[usloop].pLoopFrame)
                {
                    goto EXIT_LABEL;
                }

                *lblos_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                          LBLOS_ALARM_OFFSET) & (0x1 << (g_stoam_odlocal[usloop].ucOdLoop - 1)));
            }
            else
            {
                if (NULL == g_stoam_odlocal[usloop].pLoopFrame)
                {
                    goto EXIT_LABEL;
                }

                *lblos_alarm = (NBB_BYTE)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                          LBLOS_ALARM_OFFSET) & (0x1 << (g_stoam_odlocal[usloop].ucOdLoop - 1)));
            }

            ulrtn = SUCCESS;
            *lblos_alarm = (*lblos_alarm == 0) ? 0 : 1;
            goto EXIT_LABEL;
        }

    }

#endif
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_tx_cv_packs
 功能描述  : oam    VC层发送cv帧计数
 输入参数  : oamvckey
 输出参数  : tx_stat
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_tx_cv_packs(OAM_VC_KEY oamvckey, NBB_USHORT *tx_stat)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_tx_cv_packs"));

    /*判断输入参数是否正确*/
    if (tx_stat == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "tx_stat null error!"));
        goto EXIT_LABEL;
    }

    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *tx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                           CV_CNT_TX_OAM + g_stoam_local[usoamindex].module_index));
    }
    else
    {
        *tx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                           CV_CNT_TX_OAM + g_stoam_local[usoamindex].module_index));
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_rx_cv_packs
 功能描述  : oam    VP层接收cv帧计数
 输入参数  : OamRsvpKey
 输出参数  : rx_stat
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_rx_cv_packs(OAM_VC_KEY oamvckey, NBB_USHORT *rx_stat)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_ULONG usoamid                = 0;
    NBB_USHORT usoamindex                = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_rx_cv_packs"));

    /*判断输入参数是否正确*/
    if (rx_stat == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "rx_stat null error!"));
        goto EXIT_LABEL;
    }

    ulrtn = oam_vc_find_active_oamid(oamvckey, &usoamid);

//    spm_get_oamindex_by_oamid(usoamid,&usoamindex);
    if (SUCCESS != ulrtn)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "find oamIndex error!"));
        goto EXIT_LABEL;
    }

    if (FALSE == g_stoam_local[usoamindex].bModuleSwitch)
    {
        *rx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE1_ADDRESS,
                                           CV_CNT_RX_OAM + g_stoam_local[usoamindex].module_index));
    }
    else
    {
        *rx_stat = (uint64)(spm_oam_16read(OAM_REG_BASE2_ADDRESS,
                                           CV_CNT_RX_OAM + g_stoam_local[usoamindex].module_index));
    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_packdelay
 功能描述  : oam    VC层时延s
 输入参数  : oamvckey
 输出参数  : packdelay
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_packdelay(OAM_VC_KEY oamvckey, uint64 * packdelay)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;
    NBB_BYTE uclocalslot = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_packdelay"));

    /*判断输入参数是否正确*/
    if (packdelay == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;

        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;

            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));
            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*/
            //*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //           - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000);
            delaycfg = g_stoam_local[usoamactindex].delay;

            if (0 == delaycfg)
            {
                *packdelay = rxstampb - txtimestampf;
                *packdelay = (*packdelay) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980);
                *packdelay = (*packdelay) / 1000000000;
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
        return SUCCESS;

}

/*****************************************************************************
函 数 名  : oam_vc_packdelay_ns
功能描述  : oam    VC层时延ns
输入参数  : oamvckey
输出参数  : packdelay
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                     SUCCESS:根据key值查找到了对应的loc_alarm信息
调用函数  :
被调函数  :

修改历史      :
1.日    期   : 2013年10月14日
  作    者   : 胡晓露
  审 核 人   : #
  修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_packdelay_ns(OAM_VC_KEY oamvckey, uint64 * packdelay)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT delaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_packdelay"));

    /*判断输入参数是否正确*/
    if (packdelay == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *packdelay = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;

        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;

            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));
            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*/
            //从FPGA读取到的时戳的单位就是ns
            //*packdelay = (rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //           - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000) ;
            delaycfg = g_stoam_local[usoamactindex].delay;

            if ((NULL != g_stoam_odlocal[usloop].pDmFrame) && (0 == delaycfg))
            {
                *packdelay = rxstampb - txtimestampf - (g_stoam_odlocal[usloop].pDmFrame->frame_length + 30) * 32;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelay = rand() % (40 * delaycfg + 1) + (delaycfg * 980);
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_vc_packdelayvar
功能描述  : oam    VC层时延变化量s
输入参数  : oamvckey
输出参数  : packdelayvar
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                      SUCCESS:根据key值查找到了对应的loc_alarm信息
调用函数  :
被调函数  :

修改历史      :
 1.日    期   : 2013年10月15日
   作    者   : 胡晓露
   审 核 人   : #
   修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_packdelayvar(OAM_VC_KEY oamvckey, uint64 * packdelayvar)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    static uint64    oldTimevar               = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT vardelaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_packdelayvar"));

    /*判断输入参数是否正确*/
    if (packdelayvar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *packdelayvar = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;

        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;

            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   RX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + (uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                    RX_TIMESTPF_OFFSET + 3 + lspoffset);

            txstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                               TX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*/
            //*packdelayvar = ((rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000))
            //        - ((txstampb&TIME_STAMP_MASK + ((txstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (rxtimestampf&TIME_STAMP_MASK + ((rxtimestampf>>32)&TIME_STAMP_MASK)*1000000000) );
            //*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf);
            vardelaycfg = g_stoam_local[usoamactindex].vardelay;

            if (0 == vardelaycfg)
            {
                if ((rxstampb - txtimestampf) > oldTimevar)
                {
                    *packdelayvar = (rxstampb - txtimestampf) - oldTimevar;
                }
                else
                {
                    *packdelayvar =  oldTimevar - (rxstampb - txtimestampf);
                }

                oldTimevar = rxstampb - txtimestampf;
                *packdelayvar = (*packdelayvar) / 1000000000;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelayvar = rand() % (40 * vardelaycfg + 1) + (vardelaycfg * 980);
                *packdelayvar = (*packdelayvar) / 1000000000;
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
函 数 名  : oam_vc_packdelayvar_ns
功能描述  : oam    VC层时延变化量ns
输入参数  : oamvckey
输出参数  : packdelayvar
返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                     SUCCESS:根据key值查找到了对应的loc_alarm信息
调用函数  :
被调函数  :

修改历史      :
1.日    期   : 2013年10月15日
  作    者   : 胡晓露
  审 核 人   : #
  修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_packdelayvar_ns(OAM_VC_KEY oamvckey, uint64 * packdelayvar)
{
#if 0
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_BYTE     lspoffset                     = 0;
    uint64           txtimestampf           = 0;
    uint64           rxtimestampf           = 0;
    uint64           txstampb                 = 0;
    uint64           rxstampb                 = 0;
    static uint64    oldTimevar               = 0;
    NBB_ULONG ulregbaseaddress;
    NBB_ULONG oamid = 0;
    NBB_USHORT timeflag = 0;
    NBB_USHORT vardelaycfg = 0;
    uint64 temp = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_packdelayvar_s"));

    /*判断输入参数是否正确*/
    if (packdelayvar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "lck_alarm null error!"));
        goto EXIT_LABEL;
    }

    *packdelayvar = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;

        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            /*芯片手册中描述为16组数据，根据lspid区分。芯片手册中lspid
            指的就是按需条目号*/
            lspoffset = (g_stoam_odlocal[usloop].ucOdLoop - 1) * 16;

            if (FALSE == g_stoam_local[usoamactindex].bModuleSwitch)
            {
                ulregbaseaddress = OAM_REG_BASE1_ADDRESS;
            }
            else
            {
                ulregbaseaddress = OAM_REG_BASE2_ADDRESS;
            }

            hwwritefpga(ulregbaseaddress + DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET, 0x1);

            while (1)
            {
                timeflag = spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET + DEMAND_READ_TIME_OFFSET);

                if (0 == (timeflag & 0x1))
                {
                    break;
                }
            }

            txtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   TX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   TX_TIMESTPF_OFFSET + 3 + lspoffset));

            rxtimestampf = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                   RX_TIMESTPF_OFFSET + lspoffset) << 48)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 1 + lspoffset) << 32)
                           + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                   RX_TIMESTPF_OFFSET + 2 + lspoffset) << 16)
                           + (uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                    RX_TIMESTPF_OFFSET + 3 + lspoffset);

            txstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                               TX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET +
                                                 TX_STAMPB_OFFSET + 3 + lspoffset));

            rxstampb = ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                               RX_STAMPB_OFFSET + lspoffset) << 48)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 1 + lspoffset) << 32)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 2 + lspoffset) << 16)
                       + ((uint64)spm_oam_16read(ulregbaseaddress, DEMAND_MODULE_OFFSET  +
                                                 RX_STAMPB_OFFSET + 3 + lspoffset));

            /* 计算帧时延*/
            //*packdelayvar = ((rxstampb&TIME_STAMP_MASK + ((rxstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (txtimestampf&TIME_STAMP_MASK + ((txtimestampf>>32)&TIME_STAMP_MASK)*1000000000))
            //        - ((txstampb&TIME_STAMP_MASK + ((txstampb>>32)&TIME_STAMP_MASK)*1000000000)
            //        - (rxtimestampf&TIME_STAMP_MASK + ((rxtimestampf>>32)&TIME_STAMP_MASK)*1000000000) );
            //*packdelayvar = (rxstampb - txtimestampf) - (txstampb - rxtimestampf);
            vardelaycfg = g_stoam_local[usoamactindex].vardelay;

            if (0 == vardelaycfg)
            {
                if ((rxstampb - txtimestampf) > oldTimevar)
                {
                    *packdelayvar = (rxstampb - txtimestampf) - oldTimevar;
                }
                else
                {
                    *packdelayvar =  oldTimevar - (rxstampb - txtimestampf);
                }

                oldTimevar = rxstampb - txtimestampf;
            }
            else
            {
                srand((unsigned)time(NULL));
                *packdelayvar = rand() % (40 * vardelaycfg + 1) + (vardelaycfg * 980);
            }

            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
#endif
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : oam_vc_pktlos_near
 功能描述  : oam    VC层丢包测量近端丢包率
 输入参数  : oamvckey
 输出参数  : PktLosNear
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_pktlos_near(OAM_VC_KEY oamvckey,  float * PktLosNear)
{
    NBB_ULONG usodoamid              = 0XFFFF;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG txpcf = 0;
    NBB_ULONG rxpcf = 0;
    NBB_ULONG txpcb = 0;
    NBB_ULONG rxpcl = 0;
    static NBB_ULONG s_old_txpcf = 0;
    static NBB_ULONG s_old_rxpcf = 0;
    static NBB_ULONG s_old_txpcb = 0;
    static NBB_ULONG s_old_rxpcl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosNear == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosNear null error!"));
        goto EXIT_LABEL;
    }

    *PktLosNear = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;

        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            usodoamid   = usloop;
            spm_od_lm_task(usloop);
            
            /*LMM 发送*/
            txpcf = g_stoam_odlmdata[usloop].lmtxpcf;
            
            /*LMM 接收*/
            rxpcf = g_stoam_odlmdata[usloop].lmrxpcf;
            
            /*LMR 发送*/
            txpcb = g_stoam_odlmdata[usloop].lmtxpcb;
            
            /*LMR 接收*/
            rxpcl = g_stoam_odlmdata[usloop].lmrxpcl;

            //*PktLosNear = (float)(SUB_ABS(txpcb, s_old_txpcb) - 
            //SUB_ABS(rxpcl, s_old_rxpcl))/SUB_ABS(txpcb, s_old_txpcb) * 100;
            //if(1 ==g_alarm_debug)
            {
                printf("VC ONDEMAN NEAR:\n");
                printf("txpcb : %d\n", txpcb);
                printf("s_old_txpcb %d\n", s_old_txpcb);
                printf("rxpcl %d\n", rxpcl);
                printf("s_old_rxpcl %d\n", s_old_rxpcl);
            }

            if (txpcb > s_old_txpcb)
            {
                temp1 = txpcb - s_old_txpcb;
            }
            else
            {
                temp1 = s_old_txpcb - txpcb;
            }

            if (rxpcl > s_old_rxpcl)
            {
                temp2 = rxpcl - s_old_rxpcl;
            }
            else
            {
                temp2 = s_old_rxpcl - rxpcl;
            }

            *PktLosNear = (float)(temp1 - temp2) / temp1 * 100;
            
            //if(1 == g_alarm_debug)
            {
                printf("PktLosNear : %f\n", *PktLosNear);
            }
            s_old_txpcb = txpcb;
            s_old_rxpcl = rxpcl;
            
            /*计算近端丢包率*/
            //* PktLosNear = SUB_ABS(g_stoam_activelmdata[usodoamid].lmtxpcb_new,
            //g_stoam_activelmdata[usodoamid].lmtxpcb_old)
            //                      -SUB_ABS(g_stoam_activelmdata[usodoamid].lmrxpcl_new,
            //g_stoam_activelmdata[usodoamid].lmrxpcl_old);
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:

    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_pktlos_far
 功能描述  : oam    VC层丢包测量远端丢包率
 输入参数  : oamvckey
 输出参数  : PktLosFar
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_pktlos_far(OAM_VC_KEY oamvckey,  float * PktLosFar)
{
    NBB_USHORT usodoamid              = 0XFFFF;
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    NBB_ULONG txpcf = 0;
    NBB_ULONG rxpcf = 0;
    NBB_ULONG txpcb = 0;
    NBB_ULONG rxpcl = 0;
    static NBB_ULONG s_old_txpcf = 0;
    static NBB_ULONG s_old_rxpcf = 0;
    static NBB_ULONG s_old_txpcb = 0;
    static NBB_ULONG s_old_rxpcl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;

//   NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_pktlos_far"));

    /*判断输入参数是否正确*/
    if (PktLosFar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosFar null error!"));
        goto EXIT_LABEL;
    }

    *PktLosFar = 0;

    /*属于按需帧告警，直接查按需数组*/
    for (usloop = 0; usloop < OAM_MAX_NUM_OD; usloop++)
    {
        if (g_stoam_odlocal[usloop].ulOamActiveKey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_odlocal!"));
            goto EXIT_LABEL;
        }

        usoamactindex = g_stoam_odlocal[usloop].usOamLoop;

        /*检查对应的主动配置是否正确*/
        if (g_stoam_local[usoamactindex].ulActiveOamkey == USELESS)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found in g_stoam_local!"));
            goto EXIT_LABEL;
        }

        if (g_stoam_local[usoamactindex].oam_type != VC_OAM)
        {
            continue;

        }

        ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

        if (SUCCESS != ulrtn)
        {
            //NBB_TRC_FLOW((NBB_FORMAT "not found oam id."));
            goto EXIT_LABEL;
        }

        /*根据pstRsvplspCfg中记录的信息进行判断*/
        if (oamid == g_stoam_odlocal[usloop].ulOamActiveKey)
        {
            usodoamid   = usloop;
            spm_od_lm_task(usloop);

            /*LMM 发送*/
            txpcf = g_stoam_odlmdata[usloop].lmtxpcf;
            
            /*LMM 接收*/
            rxpcf = g_stoam_odlmdata[usloop].lmrxpcf;
            
            /*LMR 发送*/
            txpcb = g_stoam_odlmdata[usloop].lmtxpcb;
            
            /*LMR 接收*/
            rxpcl = g_stoam_odlmdata[usloop].lmrxpcl;

            //if(1 == g_alarm_debug)
            {
                printf("VC ONDEMAN FAR:\n");
                printf("txpcf : %d\n", txpcf);
                printf("s_old_txpcf : %d\n", s_old_txpcf);
                printf("rxpcf : %d \n", rxpcf);
                printf("s_old_rxpcf : %d\n", s_old_rxpcf);
            }

            if (txpcf > s_old_txpcf)
            {
                temp1 = txpcf - s_old_txpcf;
            }
            else
            {
                temp1 = s_old_txpcf - txpcf;
            }

            if (rxpcf > s_old_rxpcf)
            {
                temp2 = rxpcf - s_old_rxpcf;
            }
            else
            {
                temp2 = s_old_rxpcf - rxpcf;
            }

            *PktLosFar = (float)(temp1 - temp2) / temp1 * 100;

            //if(1 == g_alarm_debug)
            {
                printf("PktLosFar : %f", *PktLosFar);
            }

            //*PktLosFar =(float)(SUB_ABS(txpcf, s_old_txpcf) -
            //SUB_ABS(rxpcf, s_old_rxpcf))/SUB_ABS(txpcf, s_old_txpcf) * 100;
            /*计算远端丢包率*/
            //* PktLosFar = SUB_ABS(g_stoam_activelmdata[usodoamid].lmtxpcf_new,
            //g_stoam_activelmdata[usodoamid].lmtxpcf_old)
            //                    -SUB_ABS(g_stoam_activelmdata[usodoamid].lmrxpcf_new,
            //g_stoam_activelmdata[usodoamid].lmrxpcf_old);
            s_old_txpcf = txpcf;
            s_old_rxpcf = rxpcf;
            ulrtn = SUCCESS;
            goto EXIT_LABEL;
        }

    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_active_pktlos_near
 功能描述  : oam    VC层丢包测量近端丢包率
 输入参数  : oamvckey
 输出参数  : PktLosNear
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_active_pktlos_near(OAM_VC_KEY oamvckey,  float * PktLosNear)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG txfcb = 0;
    NBB_ULONG rxfcb = 0;
    static NBB_ULONG s_old_txfcf = 0;
    static NBB_ULONG s_old_rxfci = 0;
    static NBB_ULONG s_old_txfcb = 0;
    static NBB_ULONG s_old_rxfcb = 0;
    NBB_ULONG temp1; 
    NBB_ULONG temp2;
    float temppktlos1 = 0;
    float temppktlos2 = 0;
    NBB_USHORT nearpktloscfg = 0;
    float temp3 = 0;
    NBB_BYTE uclocalslot = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_pktlos_near"));

    /*判断输入参数是否正确*/
    if (PktLosNear == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosNear null error!"));
        goto EXIT_LABEL;
    }

    *PktLosNear = 0;
    ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ulrtn = spm_hardw_getslot(&uclocalslot);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pstoamcfg)
    {
        goto EXIT_LABEL;
    }

    usloop = pstoamcfg->oamindex;

    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    /*计算近端丢包率*/
    /*LMM 发送*/
    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    
    /*LMM 接收*/
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;
    
    /*LMR 发送*/
    txfcb = g_stoam_activelmdata[usloop].lmtxpcb;
    
    /*LMR 接收*/
    rxfcb = g_stoam_activelmdata[usloop].lmrxpcl;
    nearpktloscfg = g_stoam_local[usloop].nearpklos;

    if (1 == g_alarm_debug)
    {
        printf("VC NEAR :\n");
        printf("txfcf %d\n", txfcf);
        printf("s_old_txfcf %d\n", s_old_txfcf);
        printf("rxfci %d\n", rxfci);
        printf("s_old_rxfci %d\n", s_old_rxfci);
    }

    if (txfcf > s_old_txfcf)
    {
        temp1 = txfcf - s_old_txfcf;
    }
    else
    {
        temp1 = s_old_txfcf - txfcf;
    }

    if (rxfci > s_old_rxfci)
    {
        temp2 = rxfci - s_old_rxfci;
    }
    else
    {
        temp2 = s_old_rxfci - rxfci;
    }

#if 0

    if (0 == temp2)
    {
        *PktLosNear = 0;
    }
    else
    {
        *PktLosNear = (float)(temp1 - temp2) / temp1 * 100;
    }

#endif

    if (0 == temp1)
    {
        temppktlos1 = 0;
    }
    else
    {
        temppktlos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    temppktlos2 = rand() % (40 * nearpktloscfg + 1) + (nearpktloscfg * 980);
    temppktlos2 = temppktlos2 / 1000;

    if (0 != nearpktloscfg)
    {
        *PktLosNear = temppktlos2;
        g_vcnear_pklos = temppktlos2;
    }
    else
    {
        *PktLosNear =  temppktlos1;
    }

    if (1 == g_alarm_debug)
    {
        printf("near PktLosNear %f\n", *PktLosNear);
    }

    s_old_txfcf = txfcf;
    s_old_rxfci = rxfci;
    ulrtn = SUCCESS;
EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_vc_active_pktlos_far
 功能描述  : oam    VC层丢包测量远端丢包率
 输入参数  : oamvckey
 输出参数  : PktLosFar
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
                       SUCCESS:根据key值查找到了对应的loc_alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_active_pktlos_far(OAM_VC_KEY oamvckey,  float * PktLosFar)
{
    NBB_USHORT usloop                     = 0;
    NBB_USHORT usoamactindex        = 0;
    NBB_BYTE     ulrtn                        = ERROR;
    NBB_ULONG oamid = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_ULONG txfcf = 0;
    NBB_ULONG rxfci = 0;
    NBB_ULONG txfcb = 0;
    NBB_ULONG rxfcb = 0;
    static NBB_ULONG s_old_txfcf = 0;
    static NBB_ULONG s_old_rxfci = 0;
    static NBB_ULONG s_old_txfcb = 0;
    static NBB_ULONG s_old_rxfcb = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    float temppktlos1 = 0;
    float temppktlos2 = 0;
    NBB_USHORT farpktloscfg = 0;
    float temp3 = 0;
    NBB_BYTE uclocalslot = 0;

//   NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_pktlos_far"));

    /*判断输入参数是否正确*/
    if (PktLosFar == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "PktLosFar null error!"));
        goto EXIT_LABEL;
    }

    *PktLosFar = 0;
    ulrtn = oam_vc_find_active_oamid(oamvckey, &oamid);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    ulrtn = spm_hardw_getslot(&uclocalslot);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pstoamcfg)
    {
        goto EXIT_LABEL;
    }

    usloop = pstoamcfg->oamindex;

    ulrtn = spm_packet_loss_ratio_by_active_lm(usloop);

    if (SUCCESS != ulrtn)
    {
        goto EXIT_LABEL;
    }

    txfcf = g_stoam_activelmdata[usloop].lmtxpcf;
    
    /*LMM 接收*/
    rxfci = g_stoam_activelmdata[usloop].lmrxpcf;
    
    /*LMR 发送*/
    txfcb = g_stoam_activelmdata[usloop].lmtxpcb;
    
    /*LMR 接收*/
    rxfcb = g_stoam_activelmdata[usloop].lmrxpcl;
    farpktloscfg = g_stoam_local[usloop].farpklos;

    if (1 == g_alarm_debug)
    {
        printf("VC FAR :\n");
        printf("txfcb %d\n", txfcb);
        printf("s_old_txfcb %d\n", s_old_txfcb);
        printf("rxfcb %d\n", rxfcb);
        printf("s_old_rxfcb %d\n", s_old_rxfcb);
    }

    //*PktLosFar =(float)(SUB_ABS(txpcf, s_old_txpcf) - SUB_ABS(rxpcf, s_old_rxpcf))/SUB_ABS(txpcf, s_old_txpcf) * 100;
    if (txfcb > s_old_txfcb)
    {
        temp1 = txfcb - s_old_txfcb;
    }
    else
    {
        temp1 = s_old_txfcb - txfcb;
    }

    if (rxfcb > s_old_rxfcb)
    {
        temp2 = rxfcb - s_old_rxfcb;
    }
    else
    {
        temp2 = s_old_rxfcb - rxfcb;
    }

#if 0

    if (0 == temp2)
    {
        *PktLosFar = 0;
    }
    else
    {
        *PktLosFar = (float)(temp1 - temp2) / temp1 * 100;
    }

#endif

    if (0 == temp1)
    {
        temppktlos1 = 0;
    }
    else
    {
        temppktlos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    temppktlos2 = rand() % (40 * farpktloscfg + 1) + (farpktloscfg * 980);
    temppktlos2 = temppktlos2 / 1000;

    if (0 != farpktloscfg)
    {
        *PktLosFar = temppktlos2;
        g_vcfar_pklos = temppktlos2;
    }
    else
    {
        *PktLosFar =  temppktlos1;
    }

    if (1 == g_alarm_debug)
    {
        printf(" PktLosFar %f\n", *PktLosFar);
    }

    s_old_txfcb = txfcb;
    s_old_rxfcb = rxfcb;
    ulrtn = SUCCESS;
EXIT_LABEL:
    return ulrtn;
}


#ifdef PTN690_CES
#if 0
/*****************************************************************************
 函 数 名  : get_oam_vc_rdi_loc_alarm
 功能描述  : 从NP芯片获取CES板卡Vc层上报LOC和RDI告警
 输入参数  : OAMID
 输出参数  : alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
             SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月19日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE get_oam_vc_all_alarm()
{
    NBB_BYTE     ulrtn = ERROR;
    static NBB_BYTE s_flag = 0;//用来判断上升沿和下降沿的标志
    NBB_BYTE ucgroupid = 0;
    NBB_BYTE uctype = 0;
    NBB_BYTE uccsfena = 0;
    static NBB_ULONG count = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_loc_alarm"));

    g_stoam_walarmdog = BmuWdCreate();

    if (NULL == g_stoam_walarmdog)
    {
        printf("creat g_stoam_walarmdog fail!!!!\n");
    }

    if (1 == star_test_cpu_flag)
    {
        if (0 != BmuWdStart(g_stoam_walarmdog, BmuWd5MSTickBase(3), get_oam_vc_all_alarm, 0))
        {
            printf("start g_stoam_walarmdog fail!!!!!\n");
        }
    }

    if (0 == star_test_cpu_flag)
    {
        if (0 == BmuWdCancel(g_stoam_walarmdog))
        {
            printf("calcel g_stoam_walarmdog success!!!!!\n");
        }
        else
        {
            printf("cancel g_stoam_walarmdog fail!!!!!\n");
        }
    }

    //调用李艳群的接口从NP芯片读取CV帧的告警
    if (1 == g_cv_ena)
    {
        ulrtn = ApiGetFastWarning(0, 1, CES_OAM_MAX_NUM - 1, g_oam_alarm);
        count++;

        if (count == 10)
        {
            if (1 == g_alarm_debug)
            {
                printf("g_oam_alarm[0]= %d\n", g_oam_alarm[0]);
                printf("g_oam_alarm[1]= %d\n", g_oam_alarm[1]);
            }

            count = 0;
        }

        if (SUCCESS != ulrtn)
        {
            NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
                           "TPOAM cannot get oam alarm from ApiGetFastWarning()"
                           "get oam alarm faild",
                           ATG_DCI_RC_UNSUCCESSFUL,
                           "", "", "", "", 0, 0, 0, 0));
            OS_SPRINTF(g_uc_message, "%s\n get oam from NP alarm faild\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            //goto EXIT_LABEL;

        }
    }

    if (1 == g_csf_ena)
    {
        uccsfena = g_csf_ena;
        //调用刘凯的接口读取8个SDH口的告警
        ulrtn = spm_hardw_getRealAlmInfo(&g_stoam_alm);

        if (SUCCESS != ulrtn)
        {
            NBB_EXCEPTION((PCT_SPM | OAM_PD, 1,  "s d s s s s d d d d",
                           "TPOAM cannot get oam alarm from spm_hardw_getRealAlmInfo()"
                           "get oam alarm faild",
                           ATG_DCI_RC_UNSUCCESSFUL,
                           "", "", "", "", 0, 0, 0, 0));
            OS_SPRINTF(g_uc_message, "%s\n get oam alarm faild\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;

        }

        //根据读到的告警来判断需要发送什么类型的帧
        if ((g_group_id < 1) || (g_group_id > 8))
        {
            goto EXIT_LABEL;
        }

        if ((1 == g_stoam_alm[g_group_id - 1].ucSpiLos) || (1 == g_stoam_alm[g_group_id - 1].ucMsExc)
            || (1 == g_stoam_alm[g_group_id - 1].ucMsExc) || (1 == g_stoam_alm[g_group_id - 1].ucMsSd))
        {
            ucgroupid = g_group_id;
            uctype = CES_LOS_ALM;
            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 1;
        }
        else if (1 == g_stoam_alm[g_group_id - 1].ucMsAis)
        {
            ucgroupid = g_group_id;
            uctype = CES_AIS_ALM;
            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 1;
        }
        else if (1 == g_stoam_alm[g_group_id - 1].ucMsRdi)
        {
            ucgroupid = g_group_id;
            uctype = CES_RDI_ALM;
            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 1;
        }
        else if (((0 == g_stoam_alm[g_group_id - 1].ucSpiLos) || (0 == g_stoam_alm[g_group_id - 1].ucMsExc)
                  || (0 == g_stoam_alm[g_group_id - 1].ucMsExc) || (0 == g_stoam_alm[g_group_id - 1].ucMsSd)
                  || (0 == g_stoam_alm[g_group_id - 1].ucMsAis) || (0 == g_stoam_alm[g_group_id - 1].ucMsRdi)) && (1 == s_flag))
        {
            ucgroupid = g_group_id;
            uccsfena = 0;
            uctype = CES_DCI_ALM;
            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 0;
        }

    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    //NBB_TRC_EXIT();
    return ulrtn;

}
#endif
/*****************************************************************************
 函 数 名  : spm_oam_get_csftype_alarm
 功能描述  : 从NP芯片获取CES板卡Vc层上报LOC告警
 输入参数  : OAMID
 输出参数  : alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
             SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月19日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_oam_get_csftype_alarm()
{
    NBB_BYTE     ulrtn = ERROR;
    static NBB_BYTE s_flag = 0;//用来判断上升沿和下降沿的标志
    NBB_BYTE ucgroupid = 0;
    NBB_BYTE uctype = 0;
    NBB_BYTE uccsfena = 0;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_loc_alarm"));

    g_stoam_walarmdog = BmuWdCreate();

    if (NULL == g_stoam_walarmdog)
    {
        printf("creat g_stoam_walarmdog fail!!!!\n");
    }

    if (0 != BmuWdStart(g_stoam_walarmdog, BmuWd5MSTickBase(2), (int (*)(int))spm_oam_get_csftype_alarm, 0))
    {
        printf("start g_stoam_walarmdog fail!!!!!\n");
    }

    if (1 == g_test_ces_timer)
    {
        printf("the timer is start csf enable is : %d g_group_id is %d\n", g_csf_ena, g_group_id);
    }

    if (1 == g_csf_ena)
    {
        uccsfena = g_csf_ena;

        //调用刘凯的接口读取8个SDH口的告警
        ulrtn = spm_hardw_getRealAlmInfo(&g_stoam_alm);

        if (1 == g_test_ces_timer)
        {
            printf("spm_hardw_getRealAlmInfo return is %d\n", ulrtn);
        }

        if (SUCCESS != ulrtn)
        {
//            NBB_EXCEPTION((PCT_SPM| OAM_PD, 1,  "s d s s s s d d d d",
//              "TPOAM cannot get oam alarm from spm_hardw_getRealAlmInfo()"
//                "get oam alarm faild",
//              ATG_DCI_RC_UNSUCCESSFUL,
//              "","","","",0,0,0,0));
            OS_SPRINTF(g_uc_message, "%s\n get oam alarm faild\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;

        }

        //根据读到的告警来判断需要发送什么类型的帧
        if ((g_group_id < 1) || (g_group_id > 8))
        {
            goto EXIT_LABEL;
        }

        if ((1 == g_stoam_alm[g_group_id - 1].ucSpiLos) || (1 == g_stoam_alm[g_group_id - 1].ucMsExc)
            || (1 == g_stoam_alm[g_group_id - 1].ucMsExc) || (1 == g_stoam_alm[g_group_id - 1].ucMsSd))
        {
            ucgroupid = g_group_id;
            uctype = CES_LOS_ALM;

            if (1 == g_test_ces_timer)
            {
                printf("AAA ucgroupid %d uctype %d uccsfena %d\n", ucgroupid, uctype, uccsfena);
            }

            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 1;
        }
        else if (1 == g_stoam_alm[g_group_id - 1].ucMsAis)
        {
            ucgroupid = g_group_id;
            uctype = CES_AIS_ALM;

            if (1 == g_test_ces_timer)
            {
                printf("BBB ucgroupid %d uctype %d uccsfena %d\n", ucgroupid, uctype, uccsfena);
            }

            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 1;
        }
        else if (1 == g_stoam_alm[g_group_id - 1].ucMsRdi)
        {
            ucgroupid = g_group_id;
            uctype = CES_RDI_ALM;

            if (1 == g_test_ces_timer)
            {
                printf("CCC ucgroupid %d uctype %d uccsfena %d\n", ucgroupid, uctype, uccsfena);
            }

            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 1;
        }
        else if (((0 == g_stoam_alm[g_group_id - 1].ucSpiLos) || (0 == g_stoam_alm[g_group_id - 1].ucMsExc)
                  || (0 == g_stoam_alm[g_group_id - 1].ucMsExc) || (0 == g_stoam_alm[g_group_id - 1].ucMsSd)
                  || (0 == g_stoam_alm[g_group_id - 1].ucMsAis) || (0 == g_stoam_alm[g_group_id - 1].ucMsRdi)) 
                  && (1 == s_flag))
        {
            ucgroupid = g_group_id;
            uccsfena = 0;
            uctype = CES_DCI_ALM;

            if (1 == g_test_ces_timer)
            {
                printf("DDD ucgroupid %d uctype %d uccsfena %d\n", ucgroupid, uctype, uccsfena);
            }

            ApiSetCsfGroup(0, ucgroupid, uctype, uccsfena);
            s_flag = 0;
        }

    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : spm_get_oam_alarm
 功能描述  : 每隔5ms轮询读取告警信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月19日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_oam_alarm()
{

    printf("#####enter spm_get_oam_alarm#####\n");
    spm_oam_get_csftype_alarm();

}

/*****************************************************************************
 函 数 名  : oam_vc_loc_rdi_alarm
 功能描述  : oam    CES板卡Vc层上报LOC和RDI告警
 输入参数  : OAMID
             alarmtype
                       0 获取loc告警
                       2 获取csf告警
 输出参数  : alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
             SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月19日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_vc_loc_rdi_alarm(NBB_ULONG uloamid, NBB_BYTE *alarm, NBB_BYTE alarmtype)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_BYTE     galarm[CES_OAM_MAX_NUM];
    NBB_BYTE     localarm = 0;
    NBB_BYTE     csfalarm = 0;
    NBB_USHORT   usoamindex = 0;
    struct timeval tv;
    struct timezone tz;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_loc_alarm"));

    /*判断输入参数是否正确*/
    if (alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "loc_alarm null error!"));
        goto EXIT_LABEL;
    }

    spm_oam_get_local_oamindex(uloamid, &usoamindex);

    if (usoamindex > (CES_OAM_MAX_NUM - 1))
    {
        //NBB_TRC_FLOW((NBB_FORMAT "usoamindex is error!"));
        goto EXIT_LABEL;
    }

    if (1 == g_ces_loc_debug)
    {
        printf("------LOC and CSF--------\n");
        gettimeofday(&tv, &tz);
        printf("befor tv_sec:%d\n", tv.tv_sec);
        printf("befor tv_usec: %d\n", tv.tv_usec);
    }

    ApiGetFastLocWarning(0, usoamindex, 1, galarm);

    if (1 == g_ces_loc_debug)
    {
        gettimeofday(&tv, &tz);
        printf("after tv_sec:%d\n", tv.tv_sec);
        printf("after tv_usec: %d\n", tv.tv_usec);
        printf("------end LOC and CSF--------\n");
    }

    if (1 == g_alarm_debug)
    {
        printf("usoamindex %d alarm %d\n", usoamindex, galarm[usoamindex]);
    }

    switch (alarmtype)
    {
        case LOC_ALARM:
            *alarm = galarm[usoamindex] & 0x1;


            break;

        case CSF_ALARM:
            *alarm = (galarm[usoamindex] >> 1) & 0x1;
            break;

        default:
            goto EXIT_LABEL;

    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_ces_vc_rx_cv_packs
 功能描述  : 获取CES盘VC层的CV帧计数
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月21日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_ces_vc_rx_cv_packs(OAM_VC_KEY oamvckey, NBB_ULONG *rx_stat)
{
    NBB_ULONG uloamid = 0;
    NBB_USHORT usoamindex = 0;
    NBB_ULONG rx_pack_temp = 0;
    NBB_INT iret = ERROR;
    struct timeval tv;
    struct timezone tz;

    iret = oam_vc_find_active_oamid(oamvckey, &uloamid);

    if (SUCCESS != iret)
    {
        return;
    }

    spm_oam_get_local_oamindex(uloamid, &usoamindex);

    if (usoamindex > CES_OAM_MAX_NUM - 1)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "usoamindex is error!"));
        return;
    }

    rx_pack_temp = g_st_cvcount[usoamindex].dataH;

    if (1 == g_alarm_debug)
    {
        printf("usoamindex %d  rx_pack_temp = %d\n", usoamindex, rx_pack_temp);
    }

    *rx_stat = (rx_pack_temp >> 3) & 0x1FFFFFFF;

    if (1 == g_alarm_debug)
    {
        printf("rx_stat = %d\n", *rx_stat);
    }

}

/*****************************************************************************
 函 数 名  : oam_ces_vc_tx_cv_packs
 功能描述  : 获取CES盘VC层的CV帧计数
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年4月21日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_ces_vc_tx_cv_packs(OAM_VC_KEY oamvckey, NBB_ULONG *tx_stat)
{
    NBB_ULONG uloamid = 0;
    NBB_USHORT usoamindex = 0;
    NBB_ULONG tx_pack_temp = 0;
    NBB_ULONG tx_pack_temp1 = 0;
    NBB_INT iret = ERROR;
    struct timeval tv;
    struct timezone tz;

    iret = oam_vc_find_active_oamid(oamvckey, &uloamid);

    if (SUCCESS != iret)
    {
        return;
    }

    spm_oam_get_local_oamindex(uloamid, &usoamindex);

    if (usoamindex > CES_OAM_MAX_NUM - 1)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "usoamindex is error!"));
        return;
    }

    tx_pack_temp = g_st_cvcount[usoamindex].dataH;

    if (1 == g_alarm_debug)
    {
        printf("usoamindex %d  tx_pack_temp = %d\n", usoamindex, tx_pack_temp);
    }

    *tx_stat = (tx_pack_temp >> 3) & 0x1FFFFFFF;

    if (1 == g_alarm_debug)
    {
        printf("tx_pack_temp = %d\n", *tx_stat);
    }

}

/*****************************************************************************
 函 数 名  : spm_oam_get_ces_allalm
 功能描述  : a
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_oam_get_ces_allalm()
{
    NBB_BYTE     ulrtn                      = ERROR;
    static int s_flag = 0;
    struct timeval tv;
    struct timezone tz;
    OS_MEMSET(g_oam_alarm, 0, 1024);
    s_flag = s_flag + 1;

    if (s_flag == 100)
    {
        if (1 == g_ces_alarm_debug)
        {
            printf("--------ALL ALARM--------\n");
            gettimeofday(&tv, &tz);
            printf("befor tv_sec:%d\n", tv.tv_sec);
            printf("befor tv_usec: %d\n", tv.tv_usec);
        }

        ulrtn = ApiGetFastWarning(0, 1, g_usoam_num, g_oam_alarm);

        if (1 == g_ces_alarm_debug)
        {
            gettimeofday(&tv, &tz);
            printf("after tv_sec:%d\n", tv.tv_sec);
            printf("after tv_usec: %d\n", tv.tv_usec);
            printf("--------END ALL ALARM--------\n");
        }

        s_flag = 0;

        if (SUCCESS != ulrtn)
        {
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : spm_oam_get_ces_allpm
 功能描述  : a
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_oam_get_ces_allpm()
{
    NBB_BYTE     ulrtn                      = ERROR;
    static int s_flag = 0;
    struct timeval tv;
    struct timezone tz;

    OS_MEMSET(g_oam_alarm, 0, 1024);
    s_flag = s_flag + 1;

    if (s_flag == 50)
    {
        if (1 == g_ces_alarm_debug)
        {
            printf("--------ALL PM--------\n");
            gettimeofday(&tv, &tz);
            printf("befor tv_sec:%d\n", tv.tv_sec);
            printf("befor tv_usec: %d\n", tv.tv_usec);
        }

        ulrtn = ApiGetCcmFastRxCnt(0, 1, g_usoam_num, g_st_cvcount);

        if (1 == g_ces_alarm_debug)
        {
            gettimeofday(&tv, &tz);
            printf("after tv_sec:%d\n", tv.tv_sec);
            printf("after tv_usec: %d\n", tv.tv_usec);
            printf("--------END ALL PM--------\n");
        }

        s_flag = 0;

        if (SUCCESS != ulrtn)
        {
            goto EXIT_LABEL;
        }
    }

EXIT_LABEL:
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : spm_oam_get_ces_alarm
 功能描述  : oam    CES板卡Vc层上报LOC和RDI告警
 输入参数  : OAMID
             alarmtype
                warn - bit0:loc告警 0:无告警 1: 有告警
                       bit1:rdi告警 0:无告警 1: 有告警
                       bit2:csf 0:无告警 1: 有告警
                       bit3:aps告警 0:无告警 1:有告警
                       bit4:meg 0:无告警 1: 有告警
                       bit5:mep告警 0:无告警 1: 有告警
                       bit6:period 0:无告警 1: 有告警
 输出参数  : alarm
 返 回 值  : ERROR:key值错误或未查到与key值中一致的条目
             SUCCESS:根据key值查找到了对应的alarm信息
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月19日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_oam_get_ces_alarm(NBB_ULONG uloamid, NBB_BYTE *alarm, NBB_BYTE alarmtype)
{
    NBB_BYTE     ulrtn                      = ERROR;
    NBB_BYTE     galarm[CES_OAM_MAX_NUM];
    NBB_USHORT   usoamindex;
    struct timeval tv;
    struct timezone tz;

    //NBB_TRC_ENTRY((NBB_FORMAT "oam_vc_loc_alarm"));

    /*判断输入参数是否正确*/
    if (alarm == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "loc_alarm null error!"));
        goto EXIT_LABEL;
    }

#if 0
    OS_MEMSET(g_oam_alarm, 0, 1024);

    if (1 == g_ces_alarm_debug)
    {
        printf("--------ALL ALARM--------\n");
        gettimeofday(&tv, &tz);
        printf("befor tv_sec:%d\n", tv.tv_sec);
        printf("befor tv_usec: %d\n", tv.tv_usec);
    }

    //ulrtn = ApiGetFastWarning(0,1,CES_OAM_MAX_NUM-1,g_oam_alarm);
    ulrtn = ApiGetFastWarning(0, 1, g_usoam_num, g_oam_alarm);

    if (1 == g_ces_alarm_debug)
    {
        gettimeofday(&tv, &tz);
        printf("after tv_sec:%d\n", tv.tv_sec);
        printf("after tv_usec: %d\n", tv.tv_usec);
        printf("--------END ALL ALARM--------\n");
    }

    if (SUCCESS != ulrtn)
    {
//        NBB_EXCEPTION((PCT_SPM| OAM_PD, 1,  "s d s s s s d d d d",
//          "TPOAM cannot get oam alarm from ApiGetFastWarning()"
//            "get oam alarm faild",
//          ATG_DCI_RC_UNSUCCESSFUL,
//          "","","","",0,0,0,0));
        OS_SPRINTF(g_uc_message, "%s\n get oam from NP alarm faild\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    }

#endif
    spm_oam_get_local_oamindex(uloamid, &usoamindex);

    if (usoamindex >= CES_OAM_MAX_NUM)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "usoamindex is error!"));
        goto EXIT_LABEL;
    }

    if (1 == g_alarm_debug)
    {
        printf("alarm %d  usoamindex %d\n", g_oam_alarm[usoamindex], usoamindex);
    }

    switch (alarmtype)
    {
        case LOC_ALARM:
            *alarm = g_oam_alarm[usoamindex] & 0x1;
            break;

        case RDI_ALARM:
            *alarm = (g_oam_alarm[usoamindex] >> 1) & 0x1;
            break;

        case CSF_ALARM:
            *alarm = (g_oam_alarm[usoamindex] >> 2) & 0x1;
            break;

        case APS_ALARM:
            *alarm = (g_oam_alarm[usoamindex] >> 3) & 0x1;
            break;

        case MEG_ALARM:
            *alarm = (g_oam_alarm[usoamindex] >> 4) & 0x1;
            break;

        case MEP_ALARM:
            *alarm = (g_oam_alarm[usoamindex] >> 5) & 0x1;
            break;

        case UNP_ALARM:
            *alarm = (g_oam_alarm[usoamindex] >> 6) & 0x1;
            break;

        default:
            goto EXIT_LABEL;
            break;

    }

    ulrtn = SUCCESS;

EXIT_LABEL:
    return ulrtn;
}

#endif

/*****************************************************************************
 函 数 名  : oam_alarm_vs_test001
 功能描述  : 测试VS层各接口功能
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_alarm_vs_test001(NBB_USHORT  portid, NBB_BYTE SlotId, NBB_BYTE SubBoardId)
{
    NBB_USHORT ulrtn = 0;
    NBB_BYTE ulalarmval = 0;
    OAM_VS_KEY testvskey;
    testvskey.PhyPortId = portid;
    testvskey.SlotId      = SlotId;
    testvskey.SubBoardId = SubBoardId;
    ulrtn = oam_vs_find_active_oamid(testvskey);
    NBB_PRINTF(" ulrtn = %#x\n", ulrtn);

    ulrtn = oam_vs_loc_alarm(testvskey, &ulalarmval);
    NBB_PRINTF(" ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vs_lck_alarm(testvskey, &ulalarmval);
    NBB_PRINTF("oam_vs_lck_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vs_mmg_alarm(testvskey, &ulalarmval);
    NBB_PRINTF("oam_vs_mmg_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vs_unm_alarm(testvskey, &ulalarmval);
    NBB_PRINTF("oam_vs_unm_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vs_unp_alarm(testvskey, &ulalarmval);
    NBB_PRINTF("oam_vs_unp_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vs_rdi_alarm(testvskey, &ulalarmval);
    NBB_PRINTF("oam_vs_rdi_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_alarm_vp_test001
 功能描述  : 测试VP层各接口功能
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_alarm_vp_test001
(NBB_ULONG  ingress, NBB_ULONG  egress, NBB_ULONG  tunId, NBB_ULONG  LspId)
{
    NBB_BYTE ulrtn = 0;
    NBB_ULONG ulRtn1 = 0;
    NBB_BYTE ulalarmval = 0x41;
    NBB_USHORT ulCvStat  = 0;

    uint64    packdelay  = 0;
    float      packlos       = 0;

    OAM_RSVP_LSP_KEY testvpkey;
    testvpkey.Ingress   = ingress;
    testvpkey.Egress    = egress;
    testvpkey.TunnelId = tunId;
    testvpkey.LspId     = LspId;

    ulrtn = oam_rsvp_find_active_oamid_rx(&testvpkey, &ulRtn1);
    NBB_PRINTF(" ulrtn = %#x\n", ulRtn1);

    ulrtn = oam_rsvp_tx_cv_packs(testvpkey, &ulCvStat);
    NBB_PRINTF("oam_rsvp_tx_cv_packs ulrtn = %#x    ulCvStat = %#x\n", ulrtn, ulCvStat);

    ulrtn = oam_rsvp_rx_cv_packs(testvpkey, &ulCvStat);
    NBB_PRINTF("oam_rsvp_rx_cv_packs ulrtn = %#x    ulCvStat = %#x\n", ulrtn, ulCvStat);

    ulrtn = oam_rsvp_mmg_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_mmg_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_loc_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_loc_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_unm_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_unm_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_unp_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_unp_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_rdi_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_rdi_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_ais_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_ais_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    /*按需*/
    ulrtn = oam_rsvp_lblos_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_lblos_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_lck_alarm(testvpkey, &ulalarmval);
    NBB_PRINTF("oam_rsvp_lck_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_rsvp_packdelay(testvpkey, &packdelay);
    NBB_PRINTF("oam_rsvp_packdelay ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_rsvp_packdelay_ns(testvpkey, &packdelay);
    NBB_PRINTF("oam_rsvp_packdelay_ns ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_rsvp_packdelayvar(testvpkey, &packdelay);
    NBB_PRINTF("oam_rsvp_packdelayvar ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_rsvp_packdelayvar_ns(testvpkey, &packdelay);
    NBB_PRINTF("oam_rsvp_packdelayvar_ns ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_rsvp_pktlos_near(testvpkey, &packlos);
    NBB_PRINTF("oam_rsvp_pktlos_near ulrtn = %#x    packlos = %d\n", ulrtn, packlos);

    ulrtn = oam_rsvp_pktlos_far(testvpkey, &packlos);
    NBB_PRINTF("oam_rsvp_pktlos_far ulrtn = %#x    packlos = %d\n", ulrtn, packlos);


    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_alarm_vc_test001
 功能描述  : 测试VC层各接口功能
 输入参数  :
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2013年10月15日
    作    者   : 胡晓露
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE oam_alarm_vc_test001
(NBB_ULONG  vcid, NBB_ULONG  vctype, NBB_ULONG  peerip)
{
    NBB_BYTE ulrtn = 0;
    NBB_ULONG ulRtn1 = 0;
    NBB_BYTE ulalarmval = 0x41;
    NBB_USHORT ulCvStat  = 0;

    uint64    packdelay  = 0;
    float      packlos       = 0;

    OAM_VC_KEY testvckey;
    testvckey.VcId = vcid;
    testvckey.VcType = vctype;
    testvckey.PeerIp = peerip;

    ulrtn = oam_vc_find_active_oamid(testvckey, &ulRtn1);
    NBB_PRINTF(" ulrtn = %#x\n", ulRtn1);

    ulrtn = oam_vc_tx_cv_packs(testvckey, &ulCvStat);
    NBB_PRINTF("oam_vc_tx_cv_packs ulrtn = %#x    ulCvStat = %#x\n", ulrtn, ulCvStat);

    ulrtn = oam_vc_rx_cv_packs(testvckey, &ulCvStat);
    NBB_PRINTF("oam_vc_rx_cv_packs ulrtn = %#x    ulCvStat = %#x\n", ulrtn, ulCvStat);

    ulrtn = oam_vc_mmg_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_mmg_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_loc_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_loc_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_unm_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_unm_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_unp_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_unp_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_rdi_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_rdi_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_ais_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_ais_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_csf_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_csf_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    /*按需*/
    ulrtn = oam_vc_lblos_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_lblos_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_lck_alarm(testvckey, &ulalarmval);
    NBB_PRINTF("oam_vc_lck_alarm ulrtn = %#x    ulalarmval = %d\n", ulrtn, ulalarmval);

    ulrtn = oam_vc_packdelay(testvckey, &packdelay);
    NBB_PRINTF("oam_vc_packdelay ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_vc_packdelay_ns(testvckey, &packdelay);
    NBB_PRINTF("oam_vc_packdelay_ns ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_vc_packdelayvar(testvckey, &packdelay);
    NBB_PRINTF("oam_vc_packdelayvar ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_vc_packdelayvar_ns(testvckey, &packdelay);
    NBB_PRINTF("oam_vc_packdelayvar_ns ulrtn = %#x    packdelay = %d\n", ulrtn, packdelay);

    ulrtn = oam_vc_pktlos_near(testvckey, &packlos);
    NBB_PRINTF("oam_vc_pktlos_near ulrtn = %#x    packlos = %d\n", ulrtn, packlos);

    ulrtn = oam_vc_pktlos_far(testvckey, &packlos);
    NBB_PRINTF("oam_vc_pktlos_far ulrtn = %#x    packlos = %d\n", ulrtn, packlos);


    return ulrtn;
}

#ifdef SPU

/*****************************************************************************
 函 数 名  : oam_alarm_value
 功能描述  : 获取告警值，给定位key值和告警代码，返回对应的告警状态
 输入参数  : unsigned char *uckey: 存放告警对应key值的指针
             unsigned char uckeylen: key值的长度
             unsigned char ucalmid:告警的ID
 输出参数  :unsigned char *ucalmvalue: 返回的告警值存放的指针:0-无告警，1-有告警
 返 回 值  : 0-获取成功，<0获取失败
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年2月12日
    作    者   : huxiaolu
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
int oam_alarm_value(IN unsigned char *uckey, IN unsigned char uckeylen,
                    IN unsigned char ucalmid, OUT unsigned char *ucalmvalue)
{
    NBB_BYTE     ulrtn   = ERROR;
    NBB_BYTE alarm = 0;
    OAM_VC_KEY oamvckey;
    OAM_RSVP_LSP_KEY oamvpkey;
    OAM_RSVP_LSP_KEY oamvpbindvckey;
    OAM_VS_KEY oamvskey;
    SPM_OAM_VS_CB *vs_cfg;
    SPM_PORT_INFO_CB portfo;
    static NBB_BYTE s_vslckalarm = 0;
    static NBB_BYTE s_vplckalarm = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_ULONG oamid = 0;
    NBB_BYTE uclocalslot = 0;
    NBB_INT fd = 0;
    NBB_ULONG ulportindex = 0;
    NBB_BYTE rdi_alarm = 0; 
    NBB_BYTE mmg_alarm = 0;
    NBB_BYTE lck_alarm = 0;
    SPM_PORT_INFO_CB stportinfo;
    OS_MEMSET(&oamvckey, 0, sizeof(OAM_VC_KEY));
    OS_MEMSET(&oamvpkey, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&oamvpbindvckey, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&oamvskey, 0, sizeof(OAM_VS_KEY));

    if (NULL == uckey)
    {
        printf("the KEY is NULL");
        return ulrtn;
    }

    //VS层告警
    if (VS_KEYLEN == uckeylen)
    {
        oamvskey.SlotId = *uckey;
        oamvskey.SubBoardId = *(uckey + 1);
        oamvskey.PhyPortId = *(unsigned short *)(uckey + 2);
        s_vplckalarm = 0;

        switch (ucalmid)
        {
            case VS_LOC:
                oam_vs_loc_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_RDI:
                oam_vs_rdi_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_LCK:
                oam_vs_lck_alarm(oamvskey, &alarm);
                *ucalmvalue = 0;

                if (alarm == 1)
                {
                    s_vslckalarm = 1;
                }

                break;

            case VS_MMG:
                oam_vs_mmg_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_UNM:
                oam_vs_unm_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_UNP:
                oam_vs_unp_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VSLB_LOS:
                oam_vs_lblos_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case SD_ALARM:
                oam_vs_sd_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;

        }
    }

    //VP层告警
    if (VP_KEYLEN == uckeylen)
    {
        oamvpkey.Ingress = *(unsigned long *)uckey;

        oamvpkey.Egress =  *(unsigned long *)(uckey + 4);

        oamvpkey.TunnelId = *(unsigned long *)(uckey + 8);

        oamvpkey.LspId = *(unsigned long *)(uckey + 12);

        switch (ucalmid)
        {
            case VP_AIS:
                oam_rsvp_ais_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            //VP_LCK 放在VC层上报,VS层KEY值放在VP层上报。
            case VP_LCK:
                oam_rsvp_lck_alarm(oamvpkey, &alarm);
                *ucalmvalue = (s_vslckalarm || alarm);

                if (alarm == 1)
                {
                    s_vplckalarm = 1;
                }

                break;

            case VP_LOC:
                oam_rsvp_loc_alarm(oamvpkey, &alarm);

                if (1 == (s_vslckalarm || s_vplckalarm))
                {
                    *ucalmvalue = 0;
                }
                else
                {
                    *ucalmvalue = alarm;
                }

                break;

            case VP_MMG:
                oam_rsvp_mmg_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_UNM:
                oam_rsvp_unm_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_RDI:
                oam_rsvp_rdi_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_UNP:
                oam_rsvp_unp_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VPLB_LOS:
                oam_rsvp_lblos_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                s_vslckalarm = 0;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;

        }
    }

    //VC层的告警
    if (VC_KEYLEN == uckeylen)
    {
        oamvckey.VcId = *(unsigned long *)uckey;
        oamvckey.VcType = *(unsigned short *)(uckey + 4);
        oamvckey.PeerIp = *(unsigned long *)(uckey + 6);

        switch (ucalmid)
        {
            case VC_AIS:
                oam_vc_ais_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_CSF:
                oam_vc_csf_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_LCK:
                oam_vc_lck_alarm(oamvckey, &alarm);
                *ucalmvalue = ((alarm) || (s_vplckalarm));

                if (1 == g_oam_debug)
                {
                    printf("ucalmvalue %d\n", *ucalmvalue);
                }

                break;

            case VC_LOC:
                oam_vc_loc_alarm(oamvckey, &alarm);
                oam_vc_lck_alarm(oamvckey, &lck_alarm);

                if (1 == (s_vplckalarm || lck_alarm))
                {
                    *ucalmvalue = 0;
                }
                else
                {
                    *ucalmvalue = alarm;
                }

                break;

            case VC_MMG:
                oam_vc_mmg_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_UNM:
                oam_vc_unm_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_RDI:
                oam_vc_rdi_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_UNP:
                oam_vc_unp_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VCLB_LOS:
                oam_vc_lblos_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }

    }

    ulrtn = SUCCESS;
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_pve_alarm_value
 功能描述  : a
 输入参数  : IN unsigned char *uckey        
             IN unsigned char uckeylen      
             IN unsigned char ucalmid       
             OUT unsigned char *ucalmvalue  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
int oam_pve_alarm_value(IN unsigned char *uckey, IN unsigned char uckeylen,
                        IN unsigned char ucalmid, OUT unsigned char *ucalmvalue)
{
    NBB_BYTE     ulrtn   = ERROR;
    NBB_BYTE alarm = 0;
    OAM_VC_KEY oamvckey;
    OAM_RSVP_LSP_KEY oamvpkey;
    OAM_RSVP_LSP_KEY oamvpbindvckey;
    OAM_VS_KEY oamvskey;
    SPM_OAM_VS_CB *vs_cfg;
    SPM_PORT_INFO_CB portfo;
    static NBB_BYTE s_vslckalarm = 0;
    static NBB_BYTE s_vplckalarm = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_ULONG oamid = 0;
    NBB_BYTE uclocalslot = 0;
    NBB_INT fd = 0;
    NBB_ULONG ulportindex = 0;
    NBB_BYTE rdi_alarm = 0;
    NBB_BYTE mmg_alarm = 0;
    NBB_BYTE lck_alarm = 0;
    SPM_PORT_INFO_CB stportinfo;
    OS_MEMSET(&oamvckey, 0, sizeof(OAM_VC_KEY));
    OS_MEMSET(&oamvpkey, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&oamvpbindvckey, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&oamvskey, 0, sizeof(OAM_VS_KEY));

    if (NULL == uckey)
    {
        printf("the KEY is NULL");
        return ulrtn;
    }

    if (VS_KEYLEN == uckeylen)
    {
        ulportindex = *(unsigned long *)uckey;

        if (1 == g_pve_debug)
        {
            printf("the ulportindex is %d\n", ulportindex);
        }

        spm_get_portid_from_physical_port_index(ulportindex, &stportinfo);
        spm_hardw_getslot(&uclocalslot);
        oamvskey.SlotId = uclocalslot;
        oamvskey.SubBoardId = 0;
        oamvskey.PhyPortId = stportinfo.port_id;
        s_vplckalarm = 0;

        switch (ucalmid)
        {
            case VS_LOC:
                oam_vs_loc_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_RDI:
                oam_vs_rdi_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_LCK:
                oam_vs_lck_alarm(oamvskey, &alarm);
                *ucalmvalue = 0;

                if (alarm == 1)
                {
                    s_vslckalarm = 1;
                }

                break;

            case VS_MMG:
                oam_vs_mmg_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_UNM:
                oam_vs_unm_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VS_UNP:
                oam_vs_unp_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VSLB_LOS:
                oam_vs_lblos_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            case SD_ALARM:
                oam_vs_sd_alarm(oamvskey, &alarm);
                *ucalmvalue = alarm;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    if (VP_KEYLEN == uckeylen)
    {
        oamvpkey.Ingress = *(unsigned long *)uckey;
        oamvpkey.Egress =  *(unsigned long *)(uckey + 4);
        oamvpkey.TunnelId = *(unsigned long *)(uckey + 8);
        oamvpkey.LspId = *(unsigned long *)(uckey + 12);

        switch (ucalmid)
        {
            case VP_AIS:
                oam_rsvp_ais_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_LCK:
                oam_rsvp_lck_alarm(oamvpkey, &alarm);
                *ucalmvalue = (s_vslckalarm || alarm);

                if (alarm == 1)
                {
                    s_vplckalarm = 1;
                }

                break;

            case VP_LOC:
                oam_rsvp_loc_alarm(oamvpkey, &alarm);

                if (1 == (s_vslckalarm || s_vplckalarm))
                {
                    *ucalmvalue = 0;
                }
                else
                {
                    *ucalmvalue = alarm;
                }

                break;

            case VP_MMG:
                oam_rsvp_mmg_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_UNM:
                oam_rsvp_unm_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_RDI:
                oam_rsvp_rdi_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VP_UNP:
                oam_rsvp_unp_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VPLB_LOS:
                oam_rsvp_lblos_alarm(oamvpkey, &alarm);
                *ucalmvalue = alarm;
                s_vslckalarm = 0;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    if (VC_KEYLEN == uckeylen)
    {
        oamvckey.VcId = *(unsigned long *)uckey;
        oamvckey.VcType = *(unsigned short *)(uckey + 4);
        oamvckey.PeerIp = *(unsigned long *)(uckey + 6);

        switch (ucalmid)
        {
            case VC_AIS:
                oam_vc_ais_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_CSF:
                oam_vc_csf_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_LCK:
                oam_vc_lck_alarm(oamvckey, &alarm);
                *ucalmvalue = ((alarm) || (s_vplckalarm));

                if (1 == g_oam_debug)
                {
                    printf("ucalmvalue %d\n", *ucalmvalue);
                }

                break;

            case VC_LOC:
                oam_vc_loc_alarm(oamvckey, &alarm);
                oam_vc_lck_alarm(oamvckey, &lck_alarm);

                if (1 == (s_vplckalarm || lck_alarm))
                {
                    *ucalmvalue = 0;
                }
                else
                {
                    *ucalmvalue = alarm;
                }

                break;

            case VC_MMG:
                oam_vc_mmg_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_UNM:
                oam_vc_unm_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_RDI:
                oam_vc_rdi_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VC_UNP:
                oam_vc_unp_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            case VCLB_LOS:
                oam_vc_lblos_alarm(oamvckey, &alarm);
                *ucalmvalue = alarm;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }

    }

    ulrtn = SUCCESS;
    return ulrtn;
}


/******************************************************************************
函 数 名  : oam_pm_value
函数功能 : 获取性能值，给定位key值和性能代码，返回对应的性能值
输入参数 : unsigned char *uckey: 存放性能对应key值的指针
           unsigned char uckeylen: key值的长度
           unsigned char ucpmid: 所要获取的性能ID
输出参数 : 性能值返回8字节长度，按高低4字节的方式返回，若是32bit的性能值则按低
                 四字节的值为准
            unsigned long *ulpmvaluehigh: 高4字节的性能值
            unsigned long *ulpmvaluelow: 低4字节的性能值
返 回 值 : 0-获取成功，<0获取失败
调用函数  :
被调函数  :
修改历史      :
 1.日    期   : 2014年2月12日
   作    者   : huxiaolu
   审 核 人   : #
   修改内容   : 新生成函数
******************************************************************************/
int oam_pm_value(IN unsigned char *uckey, IN unsigned char uckeylen, IN unsigned char ucpmid,
                 OUT unsigned long *ulpmvaluehigh, OUT unsigned long *ulpmvaluelow)
{
    NBB_BYTE     ulrtn   = ERROR;

#if 0
    uint64 packdelay = 0;
    float  pktlos = 0;
    NBB_USHORT pack = 0;
    OAM_VC_KEY oamvckey;
    OAM_VC_KEY *pstvckey;
    OAM_RSVP_LSP_KEY oamvpkey;
    OAM_RSVP_LSP_KEY *pstVpKey;
    OAM_VS_KEY oamvskey;
    OAM_VS_KEY *pstVsKey;
    NBB_ULONG oamid = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_USHORT usloop = 0;
    NBB_BYTE uclocalslot = 0;
    SPM_PORT_INFO_CB stportinfo;
    NBB_ULONG ulportindex = 0;

    if (NULL == uckey)
    {
        printf("the KEY is NULL\n");
        return ulrtn;
    }

    //VC层
    if (VC_KEYLEN == uckeylen)
    {
        oamvckey.VcId = *(unsigned long *)uckey;
        oamvckey.VcType = *(unsigned short *)(uckey + 4);
        oamvckey.PeerIp = *(unsigned long *)(uckey + 6);

        switch (ucpmid)
        {
            case PACKDELAY_S:
                oam_vc_packdelay(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAY_NS:
                oam_vc_packdelay_ns(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_S:
                oam_vc_packdelayvar(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_NS:
                oam_vc_packdelayvar_ns(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case VC_RX_CV:
                oam_vc_rx_cv_packs(oamvckey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                
                //g_rx_vc_total_pack += pack;
                //*ulpmvaluehigh = (unsigned long)(g_rx_vc_total_pack >> 32 & 0xffffffff);
                //*ulpmvaluelow = (unsigned long)(g_rx_vc_total_pack & 0xffffffff);
                break;

            case VC_TX_CV:
                oam_vc_tx_cv_packs(oamvckey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                
                //g_tx_vc_total_pack += pack;
                //*ulpmvaluehigh = (unsigned long)(g_tx_vc_total_pack >> 32 & 0xffffffff);
                //*ulpmvaluelow = (unsigned long)(g_tx_vc_total_pack & 0xffffffff);
                break;

            case LM_PACKLOSR_NEAR:
                
                /*oam_vc_find_active_oamid(oamvckey,&oamid);
                ulrtn = spm_hardw_getslot(&uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                ulrtn = spm_compare_local_slot_with_process(oamid, uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }
                pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);
                if(NULL == pstoamcfg)
                {
                    return;
                }
                usloop = pstoamcfg->oamindex;
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_vc_active_pktlos_near(oamvckey,&pktlos);
                }
                else
                {
                    oam_vc_pktlos_near(oamvckey,&pktlos);
                }*/
                
                oam_vc_active_pktlos_near(oamvckey, &pktlos);
                *ulpmvaluehigh = 0;
                
                /*if(1 == g_alarm_debug)
                {
                    printf("VC near pktlos %f\n",pktlos);
                }*/
                
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VC near ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            case LM_PACKLOSR_FAR:
                
                /*oam_vc_find_active_oamid(oamvckey,&oamid);
                ulrtn = spm_hardw_getslot(&uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                ulrtn = spm_compare_local_slot_with_process(oamid, uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }
                pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);
                if(NULL == pstoamcfg)
                {
                    return;
                }
                usloop = pstoamcfg->oamindex;
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_vc_active_pktlos_far(oamvckey,&pktlos);
                }
                else
                {
                     oam_vc_pktlos_far(oamvckey,&pktlos);
                }*/
                
                oam_vc_active_pktlos_far(oamvckey, &pktlos);
                *ulpmvaluehigh = 0;
                
                /*if(1 == g_alarm_debug)
                {
                    printf("VC far pktlos %f\n",pktlos);
                }*/
                
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VC far ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            default:
                printf("VC Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    //VS层
    if (VS_KEYLEN == uckeylen)
    {
        oamvskey.SlotId = *uckey;
        oamvskey.SubBoardId = *(uckey + 1);
        oamvskey.PhyPortId = *(unsigned short *)(uckey + 2);

        switch (ucpmid)
        {
            case PACKDELAY_S:
                oam_vs_packdelay(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAY_NS:
                oam_vs_packdelay_ns(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_S:
                oam_vs_packdelayvar(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_NS:
                oam_vs_packdelayvar_ns(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case VS_RX_CV:
                oam_vs_rx_cv_packs(oamvskey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                
                //g_rx_vs_total_pack += pack;
                //*ulpmvaluehigh = (unsigned long)(g_rx_vs_total_pack >> 32 & 0xffffffff);
                //*ulpmvaluelow = (unsigned long)(g_rx_vs_total_pack & 0xffffffff);
                break;

            case VS_TX_CV:
                oam_vs_tx_cv_packs(oamvskey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                
                //g_tx_vs_total_pack += pack;
                //*ulpmvaluehigh = (unsigned long)(g_tx_vs_total_pack >> 32 & 0xffffffff);
                //*ulpmvaluelow = (unsigned long)(g_tx_vs_total_pack & 0xffffffff);
                break;

            case LM_PACKLOSR_NEAR:
                
                /*usloop = oam_vs_find_active_oamid(oamvskey);
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_vs_active_pktlos_near(oamvskey,&pktlos);
                }
                else
                {
                    oam_vs_pktlos_near(oamvskey,&pktlos);
                }*/
                oam_vs_active_pktlos_near(oamvskey, &pktlos);
                *ulpmvaluehigh = 0;
                
                /*if(1 == g_alarm_debug)
                {
                    printf("VS near pktlos %f\n",pktlos);
                }*/
                
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VS near ulpmvaluelow %d pktlos: %f\n", *ulpmvaluelow, pktlos);
                }

                break;

            case LM_PACKLOSR_FAR:
                
                /*usloop = oam_vs_find_active_oamid(oamvskey);
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_vs_active_pktlos_far(oamvskey,&pktlos);
                }
                else
                {
                    oam_vs_pktlos_far(oamvskey,&pktlos);
                }*/
                oam_vs_active_pktlos_far(oamvskey, &pktlos);
                *ulpmvaluehigh = 0;
                
                /*if(1 == g_alarm_debug)
                {
                    printf("far pktlos %f\n",pktlos);
                }*/
                
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("far ulpmvaluelow %d  pktlos: %f\n", *ulpmvaluelow, pktlos);
                }

                break;

            default:
                printf("VS Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    //VP层
    if (VP_KEYLEN == uckeylen)
    {
        oamvpkey.Ingress = *(unsigned long *)uckey;

        oamvpkey.Egress =  *(unsigned long *)(uckey + 4);

        oamvpkey.TunnelId = *(unsigned long *)(uckey + 8);

        oamvpkey.LspId = *(unsigned long *)(uckey + 12);



        switch (ucpmid)
        {
            case PACKDELAY_S:
                oam_rsvp_packdelay(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAY_NS:
                oam_rsvp_packdelay_ns(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_S:
                oam_rsvp_packdelayvar(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_NS:
                oam_rsvp_packdelayvar_ns(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case VP_RX_CV:
                oam_rsvp_rx_cv_packs(oamvpkey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                
                //g_rx_vp_total_pack += pack;
                //*ulpmvaluehigh = (unsigned long)(g_rx_vp_total_pack >> 32 & 0xffffffff);
                //*ulpmvaluelow = (unsigned long)(g_rx_vp_total_pack & 0xffffffff);
                break;

            case VP_TX_CV:
                oam_rsvp_tx_cv_packs(oamvpkey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                
                //g_tx_vp_total_pack += pack;
                //*ulpmvaluehigh = (unsigned long)(g_tx_vp_total_pack >> 32 & 0xffffffff);
                //*ulpmvaluelow = (unsigned long)(g_tx_vp_total_pack & 0xffffffff);
                break;

            case LM_PACKLOSR_NEAR:
                
                /*oam_rsvp_find_active_oamid_rx(&oamvpkey,&oamid);
                ulrtn = spm_hardw_getslot(&uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                ulrtn = spm_compare_local_slot_with_process(oamid, uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);
                if(NULL == pstoamcfg)
                {
                    return;
                }
                usloop = pstoamcfg->oamindex;
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_rsvp_active_pktlos_near(oamvpkey,&pktlos);
                }
                else
                {
                    oam_rsvp_pktlos_near(oamvpkey,&pktlos);
                }*/
                oam_rsvp_active_pktlos_near(oamvpkey, &pktlos);

                if (1 == g_alarm_debug)
                {
                    printf("VP near pktlos %f\n", pktlos);
                }

                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VP near ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            case LM_PACKLOSR_FAR:

                oam_rsvp_active_pktlos_far(oamvpkey, &pktlos);
                *ulpmvaluehigh = 0;

                if (1 == g_alarm_debug)
                {
                    printf("VP far pktlos %f\n", pktlos);
                }

                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VP far ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            default:
                printf("VP Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }
#endif
    ulrtn = SUCCESS;
    return ulrtn;
}

/*****************************************************************************
 函 数 名  : oam_pve_pm_value
 功能描述  : a
 输入参数  : IN unsigned char *uckey           
             IN unsigned char uckeylen         
             IN unsigned char ucpmid           
             OUT unsigned long *ulpmvaluehigh  
             OUT unsigned long *ulpmvaluelow   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
int oam_pve_pm_value(IN unsigned char *uckey, IN unsigned char uckeylen, IN unsigned char ucpmid,
                     OUT unsigned long *ulpmvaluehigh, OUT unsigned long *ulpmvaluelow)
{
    NBB_BYTE     ulrtn   = ERROR;
#if 0
    uint64 packdelay = 0;
    float  pktlos = 0;
    NBB_USHORT pack = 0;
    OAM_VC_KEY oamvckey;
    OAM_VC_KEY *pstvckey;
    OAM_RSVP_LSP_KEY oamvpkey;
    OAM_RSVP_LSP_KEY *pstVpKey;
    OAM_VS_KEY oamvskey;
    OAM_VS_KEY *pstVsKey;
    NBB_ULONG oamid = 0;
    SPM_OAM_CB *pstoamcfg = NULL;
    NBB_USHORT usloop = 0;
    NBB_BYTE uclocalslot = 0;
    SPM_PORT_INFO_CB stportinfo;
    NBB_ULONG ulportindex = 0;

    if (NULL == uckey)
    {
        printf("the KEY is NULL\n");
        return ulrtn;
    }

    if (VC_KEYLEN == uckeylen)
    {
        oamvckey.VcId = *(unsigned long *)uckey;
        oamvckey.VcType = *(unsigned short *)(uckey + 4);
        oamvckey.PeerIp = *(unsigned long *)(uckey + 6);

        switch (ucpmid)
        {
            case PACKDELAY_S:
                oam_vc_packdelay(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAY_NS:
                oam_vc_packdelay_ns(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_S:
                oam_vc_packdelayvar(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_NS:
                oam_vc_packdelayvar_ns(oamvckey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case VC_RX_CV:
                oam_vc_rx_cv_packs(oamvckey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case VC_TX_CV:
                oam_vc_tx_cv_packs(oamvckey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case LM_PACKLOSR_NEAR:
                oam_vc_active_pktlos_near(oamvckey, &pktlos);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VC near ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            case LM_PACKLOSR_FAR:
                oam_vc_active_pktlos_far(oamvckey, &pktlos);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VC far ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            default:
                printf("VC Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    if (VS_KEYLEN == uckeylen)
    {
        ulportindex = *(unsigned long *)uckey;

        if (1 == g_pve_debug)
        {
            printf("the ulportindex is %d\n", ulportindex);
        }

        spm_get_portid_from_physical_port_index(ulportindex, &stportinfo);
        spm_hardw_getslot(&uclocalslot);
        oamvskey.SlotId = uclocalslot;
        oamvskey.SubBoardId = 0;
        oamvskey.PhyPortId = stportinfo.port_id;

        switch (ucpmid)
        {
            case PACKDELAY_S:
                oam_vs_packdelay(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAY_NS:
                oam_vs_packdelay_ns(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_S:
                oam_vs_packdelayvar(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_NS:
                oam_vs_packdelayvar_ns(oamvskey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case VS_RX_CV:
                oam_vs_rx_cv_packs(oamvskey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case VS_TX_CV:
                oam_vs_tx_cv_packs(oamvskey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case LM_PACKLOSR_NEAR:
                oam_vs_active_pktlos_near(oamvskey, &pktlos);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VS near ulpmvaluelow %d pktlos: %f\n", *ulpmvaluelow, pktlos);
                }

                break;

            case LM_PACKLOSR_FAR:
                oam_vs_active_pktlos_far(oamvskey, &pktlos);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("far ulpmvaluelow %d  pktlos: %f\n", *ulpmvaluelow, pktlos);
                }

                break;

            default:
                printf("VS Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    if (VP_KEYLEN == uckeylen)
    {
        oamvpkey.Ingress = *(unsigned long *)uckey;
        oamvpkey.Egress =  *(unsigned long *)(uckey + 4);
        oamvpkey.TunnelId = *(unsigned long *)(uckey + 8);
        oamvpkey.LspId = *(unsigned long *)(uckey + 12);

        switch (ucpmid)
        {
            case PACKDELAY_S:
                oam_rsvp_packdelay(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAY_NS:
                oam_rsvp_packdelay_ns(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_S:
                oam_rsvp_packdelayvar(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case PACKDELAYVAR_NS:
                oam_rsvp_packdelayvar_ns(oamvpkey, &packdelay);
                *ulpmvaluehigh = (unsigned long)(packdelay >> 32 & 0xffffffff);
                *ulpmvaluelow = (unsigned long)(packdelay & 0xffffffff);
                break;

            case VP_RX_CV:
                oam_rsvp_rx_cv_packs(oamvpkey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case VP_TX_CV:
                oam_rsvp_tx_cv_packs(oamvpkey, &pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case LM_PACKLOSR_NEAR:
                
                /*oam_rsvp_find_active_oamid_rx(&oamvpkey,&oamid);
                ulrtn = spm_hardw_getslot(&uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                ulrtn = spm_compare_local_slot_with_process(oamid, uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);
                if(NULL == pstoamcfg)
                {
                    return;
                }
                usloop = pstoamcfg->oamindex;
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_rsvp_active_pktlos_near(oamvpkey,&pktlos);
                }
                else
                {
                    oam_rsvp_pktlos_near(oamvpkey,&pktlos);
                }*/
                oam_rsvp_active_pktlos_near(oamvpkey, &pktlos);

                if (1 == g_alarm_debug)
                {
                    printf("VP near pktlos %f\n", pktlos);
                }

                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VP near ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            case LM_PACKLOSR_FAR:
                
                /*oam_rsvp_find_active_oamid_rx(&oamvpkey,&oamid);
                ulrtn = spm_hardw_getslot(&uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                ulrtn = spm_compare_local_slot_with_process_forpm(oamid, uclocalslot);
                if(ulrtn == ERROR)
                {
                    return;
                }

                pstoamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);
                if(NULL == pstoamcfg)
                {
                    return;
                }
                usloop = pstoamcfg->oamindex;
                if(NULL == g_stoam_odlocal[usloop].pLmFrame)
                {
                    oam_rsvp_active_pktlos_far(oamvpkey,&pktlos);
                }
                else
                {
                     oam_rsvp_pktlos_far(oamvpkey,&pktlos);
                }*/
                oam_rsvp_active_pktlos_far(oamvpkey, &pktlos);
                *ulpmvaluehigh = 0;

                if (1 == g_alarm_debug)
                {
                    printf("VP far pktlos %f\n", pktlos);
                }

                *ulpmvaluelow = pktlos * 1000;

                if (1 == g_alarm_debug)
                {
                    printf("VP far ulpmvaluelow %d\n", *ulpmvaluelow);
                }

                break;

            default:
                printf("VP Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }
#endif
    ulrtn = SUCCESS;
    return ulrtn;
}

/******************************************************************************
函 数 名  : spm_oam_regoamalmpmcode
函数功能 : 注册端口性能告警代码
输入参数 : 无
输出参数 : 无
返 回 值 : 无
调用函数  :
被调函数  :
修改历史      :
 1.日    期   : 2014年2月12日
   作    者   : huxiaolu
   审 核 人   : #
   修改内容   : 新生成函数
******************************************************************************/
NBB_VOID spm_oam_regoamalmpmcode(NBB_VOID)
{
    NBB_BYTE num = 0;
    NBB_BYTE loop = 0;
    NBB_BYTE ucalmnum = 0;
    NBB_BYTE ucpmnum = 0;

    NBB_BYTE ucvsalarm[] =
    {
        VS_LOC,
        VS_RDI,
        VS_LCK,
        VS_MMG,
        VS_UNM,
        VS_UNP,
        VSLB_LOS,
        SD_ALARM
    };
    NBB_BYTE ucvpalarm[] =
    {
        VP_AIS,
        VP_LCK,
        VP_LOC,
        VP_MMG,
        VP_UNM,
        VP_RDI,
        VP_UNP,
        VPLB_LOS
    };

    NBB_BYTE ucvcalarm[] =
    {
        VC_AIS,
        VC_CSF,
        VC_LCK,
        VC_LOC,
        VC_MMG,
        VC_UNM,
        VC_RDI,
        VC_UNP,
        VCLB_LOS
    };

    NBB_BYTE ucvspm[] =
    {
        PACKDELAY_S,
        PACKDELAY_NS,
        PACKDELAYVAR_S,
        PACKDELAYVAR_NS,
        VS_RX_CV,
        VS_TX_CV,
        LM_PACKLOSR_NEAR,
        LM_PACKLOSR_FAR
    };

    NBB_BYTE ucvppm[] =
    {
        PACKDELAY_S,
        PACKDELAY_NS,
        PACKDELAYVAR_S,
        PACKDELAYVAR_NS,
        VP_RX_CV,
        VP_TX_CV,
        LM_PACKLOSR_NEAR,
        LM_PACKLOSR_FAR
    };

    NBB_BYTE ucvcpm[] =
    {
        PACKDELAY_S,
        PACKDELAY_NS,
        PACKDELAYVAR_S,
        PACKDELAYVAR_NS,
        VC_RX_CV,
        VC_TX_CV,
        LM_PACKLOSR_NEAR,
        LM_PACKLOSR_FAR
    };
    
    // VS层告警和性能
    ucalmnum = sizeof(ucvsalarm);
    ucpmnum = sizeof(ucvspm);
    
    //almpm_regalm_vsoam(ucvsalarm, ucalmnum);
    //almpm_regpm_vsoam(ucvspm, ucpmnum);

    // VP层告警和性能
    ucalmnum = sizeof(ucvpalarm);
    ucpmnum = sizeof(ucvppm);
    
    //almpm_regalm_rsvprxlspoam(ucvpalarm, ucalmnum);
    //almpm_regpm_rsvprxlspoam(ucvppm, ucalmnum);

    // VC层告警和性能
    ucalmnum = sizeof(ucvcalarm);
    ucpmnum = sizeof(ucvcpm);
    
    //almpm_regalm_vcoam(ucvcalarm, ucalmnum);
    //almpm_regpm_vcoam(ucvcpm, ucalmnum);
    ucalmnum = sizeof(ucvsalarm);
    ucpmnum = sizeof(ucvspm);
    
    //almpm_regalm_pve(ucvsalarm, ucalmnum);
    //almpm_regpm_pve(ucvspm, ucpmnum);
    return;
}

#endif
#ifdef PTN690_CES

/*****************************************************************************
 函 数 名  : oam_alarm_value
 功能描述  : 获取告警值，给定位key值和告警代码，返回对应的告警状态
 输入参数  : unsigned char *uckey: 存放告警对应key值的指针
             unsigned char uckeylen: key值的长度
             unsigned char ucalmid:告警的ID
 输出参数  :unsigned char *ucalmvalue: 返回的告警值存放的指针:0-无告警，1-有告警
 返 回 值  : 0-获取成功，<0获取失败
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2014年2月12日
    作    者   : huxiaolu
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
int oam_alarm_value(IN unsigned char *uckey, IN unsigned char uckeylen,
                    IN unsigned char ucalmid, OUT unsigned char *ucalmvalue)
{
    NBB_BYTE     ulrtn   = ERROR;
    NBB_BYTE alarm = 0;
    OAM_VC_KEY oamvckey;
    NBB_ULONG uloamid = 0;

    if (NULL == uckey)
    {
        printf("the KEY is NULL");
        return ulrtn;
    }

    if (1 == g_alarm_debug)
    {
        printf("GET CES ALARM\n");
    }

    //VC层的告警
    if (VC_KEYLEN == uckeylen)
    {
        oamvckey.VcId = *(unsigned long *)uckey;
        oamvckey.VcType = *(unsigned short *)(uckey + 4);
        oamvckey.PeerIp = *(unsigned long *)(uckey + 6);
        ulrtn = oam_vc_find_active_oamid(oamvckey, &uloamid);

        if (SUCCESS != ulrtn)
        {
            return ulrtn;
        }

        switch (ucalmid)
        {
            case VC_CSF:
                spm_oam_get_ces_alarm(uloamid, &alarm, CSF_ALARM);
                *ucalmvalue = alarm;
                break;

            case VC_LOC:
                spm_oam_get_ces_alarm(uloamid, &alarm, LOC_ALARM);
                *ucalmvalue = alarm;
                break;

            case VC_MMG:
                spm_oam_get_ces_alarm(uloamid, &alarm, MEG_ALARM);
                *ucalmvalue = alarm;
                break;

            case VC_UNM:
                spm_oam_get_ces_alarm(uloamid, &alarm, MEP_ALARM);
                *ucalmvalue = alarm;
                break;

            case VC_RDI:
                spm_oam_get_ces_alarm(uloamid, &alarm, RDI_ALARM);
                *ucalmvalue = alarm;
                break;

            case VC_UNP:
                spm_oam_get_ces_alarm(uloamid, &alarm, UNP_ALARM);
                *ucalmvalue = alarm;
                break;

            default:
                printf("Unknown alarm ID %d\n", ucalmid);
                ulrtn = ERROR;
                return ulrtn;
                break;

        }
    }

    ulrtn = SUCCESS;
    return ulrtn;
}


/******************************************************************************
函 数 名  : GeOamtPmValue
函数功能 : 获取性能值，给定位key值和性能代码，返回对应的性能值
输入参数 : unsigned char *uckey: 存放性能对应key值的指针
           unsigned char uckeylen: key值的长度
           unsigned char ucpmid: 所要获取的性能ID
输出参数 : 性能值返回8字节长度，按高低4字节的方式返回，若是32bit的性能值则按低
                 四字节的值为准
            unsigned long *ulpmvaluehigh: 高4字节的性能值
            unsigned long *ulpmvaluelow: 低4字节的性能值
返 回 值 : 0-获取成功，<0获取失败
调用函数  :
被调函数  :
修改历史      :
 1.日    期   : 2014年2月12日
   作    者   : huxiaolu
   审 核 人   : #
   修改内容   : 新生成函数
******************************************************************************/
int oam_pm_value(IN unsigned char *uckey, IN unsigned char uckeylen, IN unsigned char ucpmid,
                 OUT unsigned long *ulpmvaluehigh, OUT unsigned long *ulpmvaluelow)
{
    NBB_BYTE     ulrtn   = ERROR;
    uint64 packdelay = 0;
    float  pktlos = 0;
    NBB_USHORT pack = 0;
    OAM_VC_KEY oamvckey;
    OAM_VC_KEY *pstvckey;

    if (NULL == uckey)
    {
        printf("the KEY is NULL\n");
        return ulrtn;
    }

    //VC层
    if (VC_KEYLEN == uckeylen)
    {
        oamvckey.VcId = *(unsigned long *)uckey;
        oamvckey.VcType = *(unsigned short *)(uckey + 4);
        oamvckey.PeerIp = *(unsigned long *)(uckey + 6);

        switch (ucpmid)
        {


            case VC_RX_CV:
                oam_ces_vc_rx_cv_packs(oamvckey, (NBB_ULONG *)&pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            case VC_TX_CV:
                oam_ces_vc_tx_cv_packs(oamvckey, (NBB_ULONG *)&pack);
                *ulpmvaluehigh = 0;
                *ulpmvaluelow = pack;
                break;

            default:
                printf("VC Unknown pm ID %d\n", ucpmid);
                ulrtn = ERROR;
                return ulrtn;
                break;
        }
    }

    ulrtn = SUCCESS;
    return ulrtn;
}

/******************************************************************************
函 数 名  : spm_oam_regoamalmpmcode
函数功能 : 注册端口性能告警代码
输入参数 : 无
输出参数 : 无
返 回 值 : 无
调用函数  :
被调函数  :
修改历史      :
 1.日    期   : 2014年2月12日
   作    者   : huxiaolu
   审 核 人   : #
   修改内容   : 新生成函数
******************************************************************************/
NBB_VOID spm_oam_regoamalmpmcode(NBB_VOID)
{
    NBB_BYTE num = 0;
    NBB_BYTE loop = 0;
    NBB_BYTE ucalmnum = 0;
    NBB_BYTE ucpmnum = 0;

    NBB_BYTE ucvcalarm[] =
    {
        VC_CSF,
        VC_LOC,
        VC_MMG,
        VC_UNM,
        VC_RDI,
        VC_UNP
    };

    NBB_BYTE ucvcpm[] =
    {
        VC_RX_CV,
        VC_TX_CV,
    };
    
    // VC层告警和性能
    ucalmnum = sizeof(ucvcalarm);
    ucpmnum = sizeof(ucvcpm);
    almpm_regalm_vcoam(ucvcalarm, ucalmnum);
    almpm_regpm_vcoam(ucvcpm, ucpmnum);
    return;
}

#endif

