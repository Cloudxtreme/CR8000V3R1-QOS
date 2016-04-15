/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_clk_proc.c
   版 本 号   : 初稿
   作    者   : xwl
   生成日期   : 2013年3月11日
   最近修改   :
   功能描述   : Clk_globe  配置处理
   函数列表   :
   修改历史   :
   1.日    期   : 2013年3月11日
    作    者   : xwl
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include "spm.h"

/************************************************/
NBB_VOID spm_rcv_clk_set_glob(ATG_DCI_SET_GLOB_CLK *pstSetclkglob)
{
#ifdef SRC
    NBB_ULONG i = 0;
    NBB_ULONG j = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_BYTE ucSubBoardNo = 0;

    SPM_CLK_CB pstclkglob;

    /* 获取的子配置 */

    ATG_DCI_GLOBAL_CLK_BASIC *pstbasicdata = NULL;
    ATG_DCI_EXT_CLK *pstextclkdata = NULL;
    ATG_DCI_TIMER_SRCCLK *psttimersrcclkdata[ATG_DCI_G_CLK_TIMER_SRCCLK_NUM];
    ATG_DCI_EXTCLK_EXPORT *pstextclkexportdata[ATG_DCI_G_CLK_EXTCLK_EXPORT_NUM];
    ATG_DCI_OUTSRC_QL *pstoutsrcqldata[ATG_DCI_G_CLK_OUTSRC_QL_NUM];

    /* IPS消息偏移的首地址 */

    NBB_BYTE *pucbasicDataStart = NULL;
    NBB_BYTE *pucextclkDataStart = NULL;
    NBB_BYTE *puctimersrcclkDataStart = NULL;
    NBB_BYTE *pucextclkexportDataStart = NULL;
    NBB_BYTE *pucoutsrcqlDataStart = NULL;

    /* 子配置的操作模式 */

    NBB_ULONG ulOperbasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperextclk = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOpertimerscrclk = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperextclkexport = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperoutsrcql = ATG_DCI_OPER_NULL;

    // SPM_PORT_STRUCT stPort;

    NBB_ULONG ulClkglobKey = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_rcv_clk_set_glob");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetclkglob != NULL);

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetclkglob->return_code = ATG_DCI_RC_OK;

    ulClkglobKey = pstSetclkglob->key;
    NBB_TRC_DETAIL((NBB_FORMAT "ulClkglobKey = %ld", ulClkglobKey));

    //初始化配置
    for (i = 0; i < ATG_DCI_G_CLK_TIMER_SRCCLK_NUM; i++)
    {
        psttimersrcclkdata[i] = NULL;
    }

    for (i = 0; i < ATG_DCI_G_CLK_EXTCLK_EXPORT_NUM; i++)
    {
        pstextclkexportdata[i] = NULL;
    }

    for (i = 0; i < ATG_DCI_G_CLK_OUTSRC_QL_NUM; i++)
    {
        pstoutsrcqldata[i] = NULL;
    }

    // pstclkglob = AVLL_FIND(SHARED.clk_glob_cb, &ulClkglobKey);

    /* 如果条目不存在，不删除 */

    //  if (pstclkglob == NULL)
    // {
    //     ucIfExist = ATG_DCI_UNEXIST;
    // }

    /* 获取子配置操作 */
    ulOperbasic = pstSetclkglob->oper_basic;
    ulOperextclk = pstSetclkglob->oper_ext_clk;
    ulOpertimerscrclk = pstSetclkglob->oper_timer_scrclk;
    ulOperextclkexport = pstSetclkglob->oper_extclk_export;
    ulOperoutsrcql = pstSetclkglob->oper_outsrc_ql;

    /* 删除整个条目 */
    if (pstSetclkglob->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "删除所有的时钟配置"));
#if 0
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:要删除的时钟配置并不存在"));
            pstSetclkglob->return_code = ATG_DCI_RC_OK;

            goto EXIT_LABEL;
        }

        /* 存在删除 */
        else
        {
            /* todo list */

            // NBB_TRC_FLOW((NBB_FORMAT "查询到此配置，从tree中删除"));
            // spm_dbg_print_physical_port_head(ulPortIndexKey, SPM_OPER_DEL);

            // AVLL_DELETE(SHARED.clk_glob_cb, pstPhysicalPort->spm_physical_port_node);

            //释放接口物理配置节点的内存空间
            //spm_free_physical_port_cb(pstPhysicalPort);
        }
#endif
    }

    /* 增加或更新条目 */
    else
    {
        /* 如果条目不存在，树中要增加条目 */
        //if (ucIfExist == ATG_DCI_UNEXIST)
        //{
        //    NBB_TRC_DETAIL((NBB_FORMAT "增加时钟配置"));

            //spm_dbg_print_physical_port_head(ulPortIndexKey, SPM_OPER_ADD);

            //pstPhysicalPort = spm_alloc_physical_port_cb();
        //}
        //else
        //{
            NBB_TRC_DETAIL((NBB_FORMAT "更新时钟配置"));

            // spm_dbg_print_physical_port_head(ulPortIndexKey, SPM_OPER_UPD);
        //}

        /***************************************************************************/
        /* 先获取配置                                                              */
        /***************************************************************************/
        /******************************** 基本配置 *********************************/
        if (ulOperbasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucbasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetclkglob,
                &pstSetclkglob->basic_data);

            /* 首地址为NULL，异常 */
            if (pucbasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucbasicDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstbasicdata = (ATG_DCI_GLOBAL_CLK_BASIC *)pucbasicDataStart;

                /* 保存数据 */

                //         if (pstclkglob->basic_clk_cb == NULL)
                //         {
                //              pstclkglob->basic_clk_cb = (ATG_DCI_GLOBAL_CLK_BASIC
                // *)NBB_MM_ALLOC(sizeof(ATG_DCI_GLOBAL_CLK_BASIC),
                //                                                     NBB_NORETRY_ACT,
                //                                                      MEM_SPM_PHY_PORT_BASIC_CB);
                //           }
                NBB_TRC_DETAIL((NBB_FORMAT "pstbasicdata->work_mode = %ld", pstbasicdata->work_mode));
                memcpy(&(SHARED.clk_glob_cb.basic_clk_cb), pstbasicdata, sizeof(ATG_DCI_GLOBAL_CLK_BASIC));

                //保存子卡号
                //ucSubBoardNo = pstBasicData->sub_board;
   		 spm_clk_spec_basic(pstbasicdata);

            }
        }
        else if (ulOperbasic == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperbasic == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 外时钟配置 ********************************/
        if (ulOperextclk == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucextclkDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetclkglob,
                &pstSetclkglob->ext_clk);

            /* 首地址为NULL，异常 */
            if (pucextclkDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucextclkDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstextclkdata = (ATG_DCI_EXT_CLK *)pucextclkDataStart;

                /* 保存数据 */

                /*
                   if (pstclkglob->ext_clk_cb == NULL)
                   {
                    pstclkglob->ext_clk_cb = (ATG_DCI_EXT_CLK *)NBB_MM_ALLOC(sizeof(ATG_DCI_EXT_CLK),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_BASIC_CB);
                   }
                 */
                memcpy(&(SHARED.clk_glob_cb.ext_clk_cb), pstextclkdata, sizeof(ATG_DCI_EXT_CLK));

                //保存子卡号
                //ucSubBoardNo = pstBasicData->sub_board;
		spm_clk_spec_ext(pstextclkdata);
            }
        }
        else if (ulOperextclk == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperextclk == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 定时源输入配置 ********************************/
        if (ulOpertimerscrclk == ATG_DCI_OPER_UPDATE)
        {

            /* 计算第一个entry的地址。*/
            puctimersrcclkDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetclkglob,
                &pstSetclkglob->timer_scrclk);

            /*先判断num值，为0则为空配置*/
            if (pstSetclkglob->timer_scrclk_num == 0)
            {
                SHARED.clk_glob_cb.clk_pri_num = 0;
            }
            else
            {

                /* 首地址为NULL，异常 */
                if (puctimersrcclkDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "puctimersrcclkDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    SHARED.clk_glob_cb.clk_pri_num = pstSetclkglob->timer_scrclk_num;
                    for (i = 0; i < pstSetclkglob->timer_scrclk_num; i++)
                    {
                        psttimersrcclkdata[i] = (ATG_DCI_TIMER_SRCCLK *)
                            (puctimersrcclkDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_TIMER_SRCCLK))) * i);

                        memcpy(&(SHARED.clk_glob_cb.clk_pri_cb[i]), psttimersrcclkdata[i], sizeof(ATG_DCI_TIMER_SRCCLK));
                    }

                    /* 保存数据 */

                    /*
                       if (pstclkglob->clk_pri_cb == NULL)
                       {
                        pstclkglob->clk_pri_cb = (ATG_DCI_TIMER_SRCCLK *)NBB_MM_ALLOC(sizeof(ATG_DCI_TIMER_SRCCLK),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_PHY_PORT_STM1_CB);
                       }
                     */

                    //  memcpy(&(SHARED.clk_glob_cb.clk_pri_cb), psttimersrcclkdata, sizeof(ATG_DCI_TIMER_SRCCLK));

                }
            }
        }
        else if (ulOpertimerscrclk == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOpertimerscrclk == ATG_DCI_OPER_ADD)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 外时钟导出配置 ********************************/
        if (ulOperextclkexport == ATG_DCI_OPER_UPDATE)
        {

            /* 计算第一个entry的地址。*/
            pucextclkexportDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetclkglob,
                &pstSetclkglob->extclk_export);

            /*先判断num值，为0则为空配置*/
            if (pstSetclkglob->extclk_export_num == 0)
            {
                SHARED.clk_glob_cb.t4_clk_num = 0;
            }
            else
            {
                /* 首地址为NULL，异常 */
                if (pucextclkexportDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "pucextclkexportDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    //pstextclkexportdata = (ATG_DCI_EXTCLK_EXPORT *)pucextclkexportDataStart;

                    /* 配置处理，todo */

                    /* 保存数据 */

                    /*
                       if (pstclkglob->t4_clk_cb == NULL)
                       {
                        pstclkglob->t4_clk_cb = (ATG_DCI_TIMER_SRCCLK *)NBB_MM_ALLOC(sizeof(ATG_DCI_EXTCLK_EXPORT),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_PHY_PORT_CES_CB);
                       }
                     */
                    SHARED.clk_glob_cb.t4_clk_num = pstSetclkglob->extclk_export_num;
                    for (i = 0; i < pstSetclkglob->extclk_export_num; i++)
                    {
                        pstextclkexportdata[i] = (ATG_DCI_EXTCLK_EXPORT *)
                            (pucextclkexportDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_EXTCLK_EXPORT))) * i);

                        memcpy(&(SHARED.clk_glob_cb.t4_clk_cb[i]), pstextclkexportdata[i],
                            sizeof(ATG_DCI_EXTCLK_EXPORT));
                    }
                }
            }
        }
        else if (ulOperextclkexport == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperextclkexport == ATG_DCI_OPER_ADD)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 输出源QL配置 ********************************/
        if (ulOperoutsrcql == ATG_DCI_OPER_UPDATE)
        {

            /* 计算第一个entry的地址。*/
            pucoutsrcqlDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetclkglob,
                &pstSetclkglob->outsrc_ql);

            /*先判断num值，为0则为空配置*/
            if (pstSetclkglob->outsrc_ql_num == 0)
            {
                SHARED.clk_glob_cb.ql_out_num = 0;
            }
            else
            {
                /* 首地址为NULL，异常 */
                if (pucoutsrcqlDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "pucoutsrcqlDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    //pstoutsrcqldata = (ATG_DCI_OUTSRC_QL *)pucoutsrcqlDataStart;

                    /* 配置处理，todo */

                    /* 保存数据 */

                    /*
                       if (pstclkglob->ql_out_cb == NULL)
                       {
                        pstclkglob->ql_out_cb = (ATG_DCI_OUTSRC_QL *)NBB_MM_ALLOC(sizeof(ATG_DCI_OUTSRC_QL),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_PHY_PORT_CEP_CB);
                       }
                     */
                    SHARED.clk_glob_cb.ql_out_num = pstSetclkglob->outsrc_ql_num;
                    for (i = 0; i < pstSetclkglob->outsrc_ql_num; i++)
                    {
                        pstoutsrcqldata[i] = (ATG_DCI_OUTSRC_QL *)
                            (pucoutsrcqlDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_OUTSRC_QL))) * i);

                        memcpy(&(SHARED.clk_glob_cb.ql_out_cb[i]), pstoutsrcqldata[i], sizeof(ATG_DCI_OUTSRC_QL));
                    }

                }
            }
        }
        else if (ulOperoutsrcql == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperoutsrcql == ATG_DCI_OPER_ADD)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        //spm_dbg_print_physical_port_cfg(ulPortIndexKey);

    }

    //spm_snd_dci_ips((NBB_VOID*)pstSetPhysicalPort, &(pstSetPhysicalPort->ips_hdr) NBB_CCXT);

    //spm_send_ppcfg(pstSetPhysicalPort, ucSubBoardNo);
    spm_clk_spec(&(SHARED.clk_glob_cb));
EXIT_LABEL: NBB_TRC_EXIT();
#endif
    return;
}

