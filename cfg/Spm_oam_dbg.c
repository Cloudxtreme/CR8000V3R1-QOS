/******************************************************************************

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
#include <sys/time.h>
#include "bmuOsHdrs.h"

/* 用于监视loc告警的定时器，调试用 */
BMU_WDOG_ID g_stOAMWdog;

/* lmm帧发包计数 */
NBB_USHORT dataTemp_lmtx = 0;

/* lmm帧收包计数 */
NBB_USHORT dataTemp_lmrx = 0;

/* lmr帧发包计数 */
NBB_USHORT dataTemp_lmrtx = 0;

/* lmr帧收包计数 */
NBB_USHORT dataTemp_lmrrx = 0;

/* lck帧收包计数 */
NBB_USHORT rx_lck = 0;

/* lck帧发包计数 */
NBB_USHORT tx_lck = 0;

/* dmm帧收包计数 */
NBB_USHORT rx_temp_dmm = 0;

/* dmm帧发包计数 */
NBB_USHORT tx_temp_dmm = 0;

/* dmr帧收包计数 */
NBB_USHORT rx_temp_dmr = 0;

/* dmr帧发包计数 */
NBB_USHORT tx_temp_dmr = 0;

/* lbm帧收包计数 */
NBB_USHORT rx_temp_lbm = 0;

/* lbm帧发包计数 */
NBB_USHORT tx_temp_lbm = 0;

/* lbr帧发包计数 */
NBB_USHORT tx_temp_lbr = 0;

/* lbr帧收包计数 */
NBB_USHORT rx_temp_lbr = 0;

/* test帧收包计数 */
NBB_USHORT rx_tst = 0;

/* test帧发包计数 */
NBB_USHORT tx_tst = 0;

/*****************************************************************************
 函 数 名  : oam_help
 功能描述  : oam帮助函数
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
NBB_VOID oam_help(void)
{
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_all", "oam_show_txlsp_cb");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_oamid", "oam_show_rxlsp_cb");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_oam_array(oamid)", "oam_show_vs_cb");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_oam_cb(oamid)", "oam_show_pw_cb");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_oam_num", "oam_show_wrapping_cb");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_cv_cnt(oamid)", "oam_monitor_loc(oamid)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_alarm(oamid)", "oam_close_monitor_loc");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_rev_lsp_no(module)", "oam_clear_sd");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_rev_frame_label(module)", "oam_sd_threshold(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_enable(oamid)", "oam_show_sd_list");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_aps_port(oamid)", "oam_show_wrapping_list");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_csf_port(oamid)", "oam_show_mclag_state(oamindex)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_ais_port(oamid)", "oam_show_mclag_list");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_lb_cnt(oamid)", "oam_show_ces_alarm(usOamId)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_dm_cnt(oamid)", "oam_show_alarm_pm(usOamId)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_lck_cnt(oamid)", "oam_show_pack(usOamId)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_test_cnt(oamid)", "test_time");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_lb_error(oamid)", "test_cpu(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_test_error(oamid)", "oam_show_od_lm(oamid)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_lck_alarm(oamid)", "oam_print_lck(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_lb_alarm(oamid)", "oam_print_vc_get_vp_lck(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_lm_cnt(oamid)", "oam_show_adjust(oamid)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_dm_cnt(oamid)", "test_ces_alarm(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_od_lm_cv_cnt", "test_ces_loc(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_active_lm_cv_cnt(oamid)", "test_ces_pm(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_change_tree_of_fpga(module)", "pve_almpm_print(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_print_for_debug(flag)", "test_ces_timer(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_open_sd_print", "state_printf(flag)");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_global_cb", "oam_show_rev_od_frame(module)");

    printf("%-s%s\n", "---------------------------------",
           "FPGA---------------------------------");

    printf("%-*s%s\n", DBG_STRING_LEN, "oam_catch_frame_arg(module,num,random)\n",
           "catch multiterm frame;catch continuous/random");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_show_all_cnt(module,type)",
           "0/lbm+tst+lck;1/lbm;2/lbr;3/tst;4/lck;");
    printf("%-*s%s\n", DBG_STRING_LEN, "", "5/dmr;6/dmm;7/lmr;8/lmm;9/fdi;");
    printf("%-*s%s\n", DBG_STRING_LEN, "", "10/csf;11/aps;12/cv;13/port");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_set_catch_frame_filter(module,filter)",
           "0/Dmac;1/Smac;2/lsp label;3/pw label;4/layer");
    printf("%-*s%s\n\n", DBG_STRING_LEN, "", "5/ftype;6/mel;7/lsp no;8/cycle");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_catch_rx_pack(module,frame_type)",
           "0/all;1/cv;2/aps;3/csf;4/fdi;5/rx_pdu");
    printf("%-*s%s\n", DBG_STRING_LEN, "", "6/cv_pdu;7/asp_pdu;8/csf_pdu;9/fdi_pdu;10/lmm");
    printf("%-*s%s\n\n", DBG_STRING_LEN, "",
           "11/lmr;12/dmm;13/dmr;14/lb+test+lck;15/od lsp match");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_catch_tx_pack(module,frame_type)",
           "0/cv+aps+csf;1/fdi;2/lmm;3/lmr");
    printf("%-*s%s\n\n", DBG_STRING_LEN, "", "4/dmm;5/dmr;6/lbm+tst+lck;7/lbr");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_disable_en(oamid,framtype)",
           "-0:DM, 1:LB, 2:APS,3:CSF,4:cv,5:LM,6:LCK,7:TST");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_disable_all_en(framtype,begin,end)",
           "begin/end: array index 0-4095");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_enable_en(oamid,framtype)",
           "-0:DM, 1:LB, 2:APS,3:CSF,4:cv,5:LM,6:LCK,7:TST");
    printf("%-*s%s\n", DBG_STRING_LEN, "oam_enable_all_en(framtype,begin,end)",
           "begin/end: array index 0-4095");
    printf("%-*s%s\n", DBG_STRING_LEN, "spm_oam_print_oam_id_conf",
           "spm_oam_print_oam_id_conf_db");
}

/*****************************************************************************
 函 数 名  : oam_show_all_cnt
 功能描述  : oam调试函数
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
NBB_VOID oam_show_all_cnt(NBB_BYTE module, NBB_BYTE type)
{
    static NBB_USHORT cnt_rx[14] = {0};
    static NBB_USHORT cnt_tx[14] = {0};
    NBB_BYTE iloop = 0;
    NBB_ULONG base_addr = 0;

    if (type > 13)
    {
        return;
    }

    if (0x0 == module)
    {
        base_addr = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        base_addr = OAM_REG_BASE2_ADDRESS;
    }

    cnt_tx[type] += spm_oam_16read(base_addr, LBR_TX_CNT + type);
    cnt_rx[type] += spm_oam_16read(base_addr, LBR_RX_CNT + type);

    switch (type)
    {
        case 0 :
            printf("----------lbm/tst/lck--------\n");
            break;

        case 1 :
            printf("----------lbm--------\n");
            break;

        case 2 :
            printf("----------lbr--------\n");
            break;

        case 3 :
            printf("----------tst--------\n");
            break;

        case 4 :
            printf("----------lck--------\n");
            break;

        case 5 :
            printf("--------dmr--------\n");
            break;

        case 6 :
            printf("--------dmm--------\n");
            break;

        case 7 :
            printf("--------lmr--------\n");
            break;

        case 8 :
            printf("--------lmm--------\n");
            break;

        case 9 :
            printf("--------fdi--------\n");
            break;

        case 10 :
            printf("--------csf--------\n");
            break;

        case 11 :
            printf("--------aps--------\n");
            break;

        case 12:
            printf("--------cv--------\n");
            break;

        case 13:
            printf("--------port 0--------\n");
            break;

        default:
            break;
    }

    printf("\t tx=%d\n", cnt_tx[type]);
    printf("\t rx=%d\n", cnt_rx[type]);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_all
 功能描述  : oam调试函数
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
NBB_VOID oam_show_all(NBB_CCXT_T NBB_CCXT)
{
    NBB_USHORT i = 0;
    NBB_BYTE odindex = 0;
    NBB_INT iret = ERROR;

    for (i = 0 ; i < OAM_MAX_NUM_ACTIVE_SPU; i++)
    {
        if (g_stoam_local[i].ulActiveOamkey == USELESS)
        {
            continue;
        }

        printf("\n\n");
        printf("---------------------oam index = %d-----------------------\n", i);
        printf("%*s\n", 25, "Active:");
        printf("%-*s%d\n", STRING_LEN, "oamkey : ", g_stoam_local[i].ulActiveOamkey);

        if (TRUE == g_stoam_local[i].aps_enable)
        {
            printf("%-*s%s\n", STRING_LEN, "frame type : ", "APS");
        }

        if (TRUE == g_stoam_local[i].csf_enable)
        {
            printf("%-*s%s\n", STRING_LEN, "frame type :", "CSF");
        }

        if (TRUE == g_stoam_local[i].cv_enable)
        {
            printf("%-*s%s\n", STRING_LEN, "frame type :", "CV");
        }

        if (TRUE == g_stoam_local[i].fdi_enable)
        {
            printf("%-*s%s\n", STRING_LEN, "frame type :", "FDI");
        }

        printf("%-*s%d\n", STRING_LEN, "oam type :", g_stoam_local[i].oam_type);

        if ((VS_OAM == g_stoam_local[i].oam_type)
            || (WRAPPING_OAM == g_stoam_local[i].oam_type))
        {
            printf("%-*s%#x\n", STRING_LEN, "ip : ", g_stoam_local[i].ip);
            printf("%-*s%d\n", STRING_LEN, "portid : ", g_stoam_local[i].vs_Portid);
        }

        if (VC_OAM == g_stoam_local[i].oam_type)
        {
            printf("%-*s%d\n", STRING_LEN, "vc_no : ", g_stoam_local[i].vc_no_section1);

//            printf("%-*s%d\n", STRING_LEN, "vc_no2 : ", g_stoam_local[i].vc_no_section2);
            printf("%-*s%d\n", STRING_LEN, "vc_portid : ", g_stoam_local[i].vc_PortId_section1);

//            printf("%-*s%d\n", STRING_LEN, "vc_portid2 : ", g_stoam_local[i].vc_PortId_section2);
            printf("%-*s%d\n", STRING_LEN, "uni_slot : ", g_stoam_local[i].uni_slot);
            printf("%-*s%d\n", STRING_LEN, "uni_port_type : ", g_stoam_local[i].uni_port_type);
            printf("%-*s%d\n", STRING_LEN, "nni_slot : ", g_stoam_local[i].nni_slot);
            printf("%-*s%d\n", STRING_LEN, "vpn_type : ", g_stoam_local[i].vpn_type);
            printf("%-*s%d\n", STRING_LEN, "vp_id : ", g_stoam_local[i].vp_id);
        }

        if (VP_OAM == g_stoam_local[i].oam_type)
        {
            if (0x0 == g_stoam_local[i].vp_attrib)
            {
                printf("%-*s%d\n", STRING_LEN, "inport1 :", g_stoam_local[i].inport1);
                printf("%-*s%#x\n", STRING_LEN, "lspid1 : ", g_stoam_local[i].lspid1);
                printf("%-*s%d\n", STRING_LEN, "inport2 :", g_stoam_local[i].inport2);
                printf("%-*s%#x\n", STRING_LEN, "lspid2 : ", g_stoam_local[i].lspid2);
                printf("%-*s%d\n", STRING_LEN, "nni_mip_slot : ", g_stoam_local[i].nni_mip_slot1);

                //            printf("%-*s%d\n", STRING_LEN, "nni_mip_slot2 : ", g_stoam_local[i].nni_mip_slot2);
            }
            else
            {
                printf("%-*s%#x\n", STRING_LEN, "lspid : ", g_stoam_local[i].vp_Lspid);
                printf("%-*s%#x\n", STRING_LEN, "portid : ", g_stoam_local[i].vp_PortId);
            }
        }

        printf("%-*s%d\n", STRING_LEN, "VP attribute : ", g_stoam_local[i].vp_attrib);


        printf("%-*s%#x%x%x%x%x%x\n",
               STRING_LEN, "MEG ICC : ",
               g_stoam_local[i].meg_icc[0],
               g_stoam_local[i].meg_icc[1],
               g_stoam_local[i].meg_icc[2],
               g_stoam_local[i].meg_icc[3],
               g_stoam_local[i].meg_icc[4],
               g_stoam_local[i].meg_icc[5]);
        printf("%-*s%#x%x%x%x%x%x\n",
               STRING_LEN, "MEG UMC : ",
               g_stoam_local[i].meg_umc[0],
               g_stoam_local[i].meg_umc[1],
               g_stoam_local[i].meg_umc[2],
               g_stoam_local[i].meg_umc[3],
               g_stoam_local[i].meg_umc[4],
               g_stoam_local[i].meg_umc[5]);
        printf("%-*s%d\n", STRING_LEN, "Source MEP ID : ", g_stoam_local[i].src_mep_id);
        printf("%-*s%d\n", STRING_LEN, "Destination MEP ID : ", g_stoam_local[i].dst_mep_id);
        printf("%-*s%d\n", STRING_LEN, "Mel : ", g_stoam_local[i].mel);
        printf("%-*s%d\n", STRING_LEN, "LSP_EXP : ", g_stoam_local[i].lsp_exp);
        printf("%-*s%d\n", STRING_LEN, "PW_EXP : ", g_stoam_local[i].pw_exp);
        printf("%-*s%d\n", STRING_LEN, "gal_EXP : ", g_stoam_local[i].gal_exp);
        printf("%-*s%d\n", STRING_LEN, "lsp_ttl : ", g_stoam_local[i].lsp_ttl);
        printf("%-*s%d\n", STRING_LEN, "PW_ttl : ", g_stoam_local[i].pw_ttl);
        printf("%-*s%d\n", STRING_LEN, "gal_ttl :", g_stoam_local[i].gal_ttl);

//        printf("%-*s%d\n", STRING_LEN, "lsp lable tx : ", g_stoam_local[i].ulTunnelLabel_tx);
//        printf("%-*s%d\n", STRING_LEN, "pw lable tx  : ", g_stoam_local[i].ulPwLable_tx);
        printf("%-*s%d\n", STRING_LEN, "lsp lable rx   : ", g_stoam_local[i].ulTunnelLabel_rx);

//        printf("%-*s%d\n", STRING_LEN, "lsp lable2 rx   : ", g_stoam_local[i].mip_second_tunnel_rx);
        printf("%-*s%d\n", STRING_LEN, "pw lable rx  : ", g_stoam_local[i].ulPwLable_rx);
        printf("%-*s%d\n", STRING_LEN, "module : ", g_stoam_local[i].bModuleSwitch);
        printf("%-*s%d\n", STRING_LEN, "index_in_module : ", g_stoam_local[i].module_index);
        printf("%-*s%d\n", STRING_LEN, "index_in_array : ", g_stoam_local[i].global_index);
        printf("%-*s%d\n", STRING_LEN, "isRelatedProct : ", g_stoam_local[i].bIsRelateProt);
        printf("%-*s%d\n", STRING_LEN, "near_pklos : ", g_stoam_local[i].nearpklos);
        printf("%-*s%d\n", STRING_LEN, "far_pklos : ", g_stoam_local[i].farpklos);
        printf("%-*s%d\n", STRING_LEN, "delay : ", g_stoam_local[i].delay);
        printf("%-*s%d\n", STRING_LEN, "vardelay : ", g_stoam_local[i].vardelay);
        printf("%-*s%d\n", STRING_LEN, "ais_alarm_flag : ", g_stoam_local[i].ais_alarm_flag);
        printf("%-*s%d\n", STRING_LEN, "fdi_alarm_flag : ", g_stoam_local[i].rdi_alarm_flag);

        iret = spm_odindex_by_activeindex(i, &odindex);

        if (SUCCESS == iret)
        {
            printf("\n\n");
            printf("%*s\n", 25, "Ondemand:");
            printf("%-*s%d\n", STRING_LEN, "OAM ID : ", g_stoam_odlocal[odindex].ulOamActiveKey);
            printf("%-*s%d\n", STRING_LEN, "od_index_in_array : ", odindex);

            if (NULL == g_stoam_odlocal[odindex].pBasicData)
            {
                printf("*****ERROR*****: basic cfg is empty\n");
                return;
            }

            printf("%-*s%d\n", STRING_LEN, "Module : ", g_stoam_odlocal[odindex].bModuleSwitch);
            printf("%-*s%d\n", STRING_LEN, "Mel : ", g_stoam_odlocal[odindex].pBasicData->mel);
            printf("%-*s%d\n", STRING_LEN, "LSP_EXP : ", g_stoam_odlocal[odindex].pBasicData->lsp_exp);
            printf("%-*s%d\n", STRING_LEN, "PW_EXP : ", g_stoam_odlocal[odindex].pBasicData->pw_exp);
            printf("%-*s%d\n", STRING_LEN, "gal_EXP : ", g_stoam_odlocal[odindex].pBasicData->gal_exp);
            printf("%-*s%d\n", STRING_LEN, "lsp_ttl : ", g_stoam_odlocal[odindex].pBasicData->lsp_ttl);
            printf("%-*s%d\n", STRING_LEN, "PW_ttl : ", g_stoam_odlocal[odindex].pBasicData->pw_ttl);
            printf("%-*s%d\n", STRING_LEN, "gal_ttl : ", g_stoam_odlocal[odindex].pBasicData->gal_ttl);

            if (NULL != g_stoam_odlocal[odindex].pDmFrame)
            {
                if (TRUE == g_stoam_odlocal[odindex].pDmFrame->byte1.send_enable)
                {
                    printf("%-*s%s\n", STRING_LEN, "frame type : ", "DM");
                }
            }

            if (NULL != g_stoam_odlocal[odindex].pLmFrame)
            {
                if (TRUE == g_stoam_odlocal[odindex].pLmFrame->byte1.send_enable)
                {
                    printf("%-*s%s\n", STRING_LEN, "frame type : ", "LM");
                }
            }

            if (NULL != g_stoam_odlocal[odindex].pLckFrame)
            {
                if (TRUE == g_stoam_odlocal[odindex].pLckFrame->lck_frame_send_enable)
                {
                    printf("%-*s%s\n", STRING_LEN, "frame type : ", "LCK");
                }
            }

            if (NULL != g_stoam_odlocal[odindex].pLoopFrame)
            {
                if (TRUE == g_stoam_odlocal[odindex].pLoopFrame->byte1.send_enable)
                {
                    printf("%-*s%s\n", STRING_LEN, "frame type : ", "Loop");
                    printf("%-*s%d\n", STRING_LEN, "lb_send_cycle : ",
                           g_stoam_odlocal[odindex].pLoopFrame->byte1.send_cycle);
                    printf("%-*s%d\n", STRING_LEN, "LB测试方式 : ",
                           g_stoam_odlocal[odindex].pLoopFrame->byte1.test_method);
                    printf("%-*s%d\n", STRING_LEN, "离线方式测试tlv类型 : ",
                           g_stoam_odlocal[odindex].pLoopFrame->byte1.test_tlv_type);
                    printf("%-*s%d\n", STRING_LEN, "lb_mode : ",
                           g_stoam_odlocal[odindex].pLoopFrame->byte1.lb_mode);
                    printf("%-*s%d\n", STRING_LEN, "数据tlv测试内容 : ",
                           g_stoam_odlocal[odindex].pLoopFrame->tlv_test_content);
                    printf("%-*s%d\n", STRING_LEN, "TLV长度 : ",
                           g_stoam_odlocal[odindex].pLoopFrame->tlv_length);
                    printf("%-*s%s\n", STRING_LEN, "Target TLV模式 : ",
                           g_stoam_odlocal[odindex].pLoopFrame->tlv_mode ? "MIP TLV" : "MEP TLV");
                    printf("%-*s%d\n", STRING_LEN, "Requesting Mep ID TLV: ",
                           g_stoam_odlocal[odindex].pLoopFrame->mep_id_enable);
                    printf("%-*s%d\n", STRING_LEN, "目的MEP ID: ",
                           g_stoam_odlocal[odindex].pLoopFrame->dest_mep_id);
                }
            }

            if (NULL != g_stoam_odlocal[odindex].pTestFrame)
            {
                if (TRUE == g_stoam_odlocal[odindex].pTestFrame->byte1.send_enable)
                {
                    printf("%-*s%s\n", STRING_LEN, "frame type : ", "TST");
                }
            }

            if (NULL != g_stoam_odlocal[odindex].pMip)
            {
                printf("%-*s%d\n", STRING_LEN, "expect_mep_id:", g_stoam_odlocal[odindex].pMip->expect_mep_id);
                printf("%-*s%d\n", STRING_LEN, "cc:",
                       g_stoam_odlocal[odindex].pMip->cc);
                printf("%-*s%d\n", STRING_LEN, "if_num:", g_stoam_odlocal[odindex].pMip->if_num);
                printf("%-*s%d\n", STRING_LEN, "mip_icc:", g_stoam_odlocal[odindex].pMip->mip_icc);
                printf("%-*s%d\n", STRING_LEN, "mip_id:", g_stoam_odlocal[odindex].pMip->mip_id);
                printf("%-*s%d\n", STRING_LEN, "node_id:", g_stoam_odlocal[odindex].pMip->node_id);
                printf("%-*s%d\n", STRING_LEN, "probe_jump:", g_stoam_odlocal[odindex].pMip->probe_jump);
                printf("%-*s%d\n", STRING_LEN, "tlv_select:", g_stoam_odlocal[odindex].pMip->tlv_select);
            }

            printf("\t%-*s%d\n", STRING_LEN, "OamLoop:", g_stoam_odlocal[odindex].usOamLoop);
            printf("\t%-*s%d\n", STRING_LEN, "odLoop:", g_stoam_odlocal[odindex].ucOdLoop);
        }
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_oamid
 功能描述  : oam调试函数
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
NBB_VOID oam_show_oamid()
{
    NBB_USHORT i;

    for (i = 0; i < OAM_MAX_NUM_ACTIVE_SPU; i++)
    {
        if (g_stoam_local[i].ulActiveOamkey == USELESS)
        {
            break;
        }

        printf("\n############MPLS-TP OAM(ID = %d) ##############\n", g_stoam_local[i].ulActiveOamkey);
    }
}

/*****************************************************************************
 函 数 名  : oam_catch_frame_arg
 功能描述  : oam调试函数
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
NBB_VOID oam_catch_frame_arg(NBB_BYTE module, NBB_BYTE frameNum, NBB_BOOL random)
{
    NBB_USHORT temp = 0;
    NBB_ULONG base_addr = 0;

    if (0x0 == module)
    {
        base_addr = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        base_addr = OAM_REG_BASE2_ADDRESS;
    }

    temp = random;
    temp = (temp << 8) + frameNum;
    spm_oam_16write(base_addr, 0xb, temp);
    return;
}


/*****************************************************************************
 函 数 名  : oam_set_catch_frame_filter
 功能描述  : oam调试函数
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
NBB_VOID oam_set_catch_frame_filter(NBB_BYTE module, NBB_BYTE filter)
{
    NBB_USHORT ultype = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT temp = 0;
    NBB_ULONG input[10] = {0};
    NBB_ULONG idata = 0;
    NBB_BYTE ulI = 0;
    NBB_BYTE idata2 = 0;

    if (0x0 == module)
    {
        base_addr = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        base_addr = OAM_REG_BASE2_ADDRESS;
    }

    switch (filter)
    {
        case 0 :
            ultype = 0x20;
            printf("enter Dmac(ep:0x12345678 0x090a):\n");

            #if 0
            scanf("%x %x",
                  &input[0], &input[1]);
            #endif
            
            temp = (input[0] >> 16) & 0xffff;
            spm_oam_16write(base_addr, 0x2b, temp);
            temp = input[0] & 0xffff;
            spm_oam_16write(base_addr, 0x2c, temp);
            temp = input[1] & 0xffff;
            spm_oam_16write(base_addr, 0x2d, temp);

            break;

        case 1 :
            ultype = 0x21;
            printf("enter Smac(ep:0x12345678 0x090a):\n");

            if (EOF == (scanf("%x%x", &input[0], &input[1])))
            {
                return;
            }

            temp = (input[0] >> 16) & 0xffff;
            spm_oam_16write(base_addr, 0x28, temp);
            temp = input[0] & 0xffff;
            spm_oam_16write(base_addr, 0x29, temp);
            temp = input[1] & 0xffff;
            spm_oam_16write(base_addr, 0x2a, temp);
            break;

        case 2 :
            ultype = 0x22;
            printf("enter lsp label:\n");

            #if 0
            scanf("%ld", &idata);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x26);
            temp &= 0xfff0;
            temp += ((idata >> 16) & 0xf);
            spm_oam_16write(base_addr, 0x26, temp);
            temp = idata & 0xffff;
            spm_oam_16write(base_addr, 0x27, temp);
            break;

        case 3 :
            ultype = 0x23;
            printf("enter pw label:\n");

            #if 0
            scanf("%ld", &idata);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x24);
            temp &= 0xfff0;
            temp += ((idata >> 16) & 0xf);
            spm_oam_16write(base_addr, 0x24, temp);
            temp = idata & 0xffff;
            spm_oam_16write(base_addr, 0x25, temp);
            break;

        case 4 :
            ultype = 0x27;
            printf("enter layer:\n");

            #if 0
            scanf("%d", &idata2);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x26);
            temp = temp & 0x0fff;
            temp += ((idata2 << 12) & 0xf000);
            spm_oam_16write(base_addr, 0x26, temp);
            break;

        case 5 :
            ultype = 0x28;
            printf("enter ftype:\n");
            
            #if 0
            scanf("%d", idata2);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x26);
            temp = temp & 0xf00f;
            temp += ((idata2 << 4) & 0x0ff0);
            spm_oam_16write(base_addr, 0x26, temp);
            break;

        case 6 :
            ultype = 0x29;
            printf("enter mel:\n");

            #if 0
            scanf("%d", &idata2);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x20);
            temp = temp & 0x8fff;
            temp += (idata2 & 0x0007);
            spm_oam_16write(base_addr, 0x20, temp);
            break;

        case 7 :
            ultype = 0x2A;
            printf("enter lsp no:\n");

            #if 0
            scanf("%d", &idata2);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x20);
            temp = temp & 0xff00;
            temp += (idata2 & 0x00ff);
            spm_oam_16write(base_addr, 0x20, temp);
            break;

        case 8 :
            ultype = 0x2B;
            printf("enter cycle:\n");

            #if 0
            scanf("%d", &idata2);
            #endif
            
            temp = spm_oam_16read(base_addr, 0x24);
            temp = temp & 0x0fff;
            temp += (idata2 & 0x000f);
            spm_oam_16write(base_addr, 0x24, temp);
            break;

        default:
            break;
    }

    /* 剔除scanf后的所有字符输入包括换行符 */
    while (getchar() != '\n')
    {
        continue;
    }

    spm_oam_16write(base_addr, CATCH_FRA_ARG, ultype);
    return;
}




/*****************************************************************************
 函 数 名  : oam_catch_rx_pack
 功能描述  : oam调试函数
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
NBB_VOID oam_catch_rx_pack(NBB_BYTE module, NBB_ULONG ultype)
{
    NBB_ULONG base_addr = 0;
    NBB_INT length = 0;

    if (0x0 == module)
    {
        base_addr = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        base_addr = OAM_REG_BASE2_ADDRESS;
    }

    length = spm_oam_16read(base_addr, 0xb);
    length &= 0xff;
    length = 0x40 * length;

    switch (ultype)
    {
        case 0:
            printf("rx_frame:\n");
            ultype = 0x5;
            break;

        case 1:
            printf("cv_rx: \n");
            ultype = 0x18;
            break;

        case 2:
            printf("aps_rx: \n");
            ultype = 0x19;
            break;

        case 3:
            printf("csf_rx: \n");
            ultype = 0x1a;
            break;

        case 4:
            printf("fdi_rx: \n");
            ultype = 0x1b;
            break;

        case 5:
            printf("rx_PDU: \n");
            ultype = 0x4;
            break;

        case 6:
            printf("cv_pdu_rx: \n");
            ultype = 0x10;
            break;

        case 7:
            printf("aps_pdu_rx: \n");
            ultype = 0x11;
            break;

        case 8:
            printf("csf_pdu_rx: \n");
            ultype = 0x12;
            break;

        case 9:
            printf("fdi_pdu_rx: \n");
            ultype = 0x13;
            break;

        case 10:
            printf("lmm_rx: \n");
            ultype = 0x14;
            break;

        case 11:
            printf("lmr_rx: \n");
            ultype = 0x15;
            break;

        case 12:
            printf("dmm_rx: \n");
            ultype = 0x16;
            break;

        case 13:
            printf("dmr_rx: \n");
            ultype = 0x17;
            break;

        case 14:
            printf("lbm/lbr/test/lck_rx: \n");
            ultype = 0x36;
            break;

        case 15:
            printf("od_label_match: \n");
            ultype = 0x37;
            break;

        default:
            break;
    }

    spm_oam_16write(base_addr, CATCH_FRA_ARG, ultype);
    oam_print_fpga(base_addr + CATCH_FRAME, length);
    return;
}


/*****************************************************************************
 函 数 名  : oam_catch_tx_pack
 功能描述  : oam调试函数
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
NBB_VOID oam_catch_tx_pack(NBB_BYTE module, NBB_ULONG ultype)
{
    NBB_ULONG base_addr = 0;
    NBB_INT length = 0;

    if (0x0 == module)
    {
        base_addr = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        base_addr = OAM_REG_BASE2_ADDRESS;
    }

    length = spm_oam_16read(base_addr, 0xb);
    length &= 0xff;
    length = 0x40 * length;

    switch (ultype)
    {
        case 0:
            printf("cv/aps/csf_tx: \n");
            ultype = 0xa;
            break;

        case 1:
            printf("fdi_tx: \n");
            ultype = 0xb;
            break;

        case 2 :
            printf("lmm_tx: \n");
            ultype = 0x30;
            break;

        case 3 :
            printf("lmr_tx: \n");
            ultype = 0x31;
            break;

        case 4 :
            printf("dmm_tx: \n");
            ultype = 0x32;
            break;

        case 5 :
            printf("dmr_tx: \n");
            ultype = 0x33;
            break;

        case 6 :
            printf("lbm/tst/lck_tx: \n");
            ultype = 0x34;
            break;

        case 7 :
            printf("lbr_tx: \n");
            ultype = 0x35;
            break;

        default:
            break;
    }

    spm_oam_16write(base_addr, CATCH_FRA_ARG, ultype);
    oam_print_fpga(base_addr + CATCH_FRAME, length);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_vs_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_vs_cb()
{
    SPM_OAM_VS_CB *p_vscfg = NULL;

    for (p_vscfg = (SPM_OAM_VS_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebyindex);
         p_vscfg != NULL;
         p_vscfg = (SPM_OAM_VS_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebyindex,
                   p_vscfg->spm_vs_node))
    {
        printf("------------OAM ID=%d----------------\n", p_vscfg->oamid);
        printf("\tvs_port_index : %d\n", p_vscfg->vs_index);
        printf("\tSlot : %d\n", p_vscfg->vs_slotid);
        printf("\tip : %#x\n", p_vscfg->ip);
        printf("\tout_portid : %#x\n", p_vscfg->vs_portid);
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_wrapping_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_wrapping_cb()
{
    SPM_OAM_WRAPPING_CB *p_wrapping = NULL;

    for (p_wrapping = (SPM_OAM_WRAPPING_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebywrapping);
         p_wrapping != NULL;
         p_wrapping = (SPM_OAM_WRAPPING_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebywrapping,
                      p_wrapping->spm_wrapping_node))
    {
        printf("------------OAM ID=%d----------------\n", p_wrapping->oamid);
        printf("\tring_id : %#x\n", p_wrapping->ring_id);
        printf("\tlogical_PortId : %d\n", p_wrapping->logical_port_index);
        printf("\tSlot : %d\n", p_wrapping->slot_id);
        printf("\tip : %#x\n", p_wrapping->ip);
        printf("\tout_portid : %d\n", p_wrapping->out_portid);
        printf("\tdirection_select : %d\n", p_wrapping->direction_select);
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_rxlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_rxlsp_cb()
{
    SPM_OAM_RSVP_CB_RX *p_Oamrsvp = NULL;

    for (p_Oamrsvp = (SPM_OAM_RSVP_CB_RX *)AVLL_FIRST(v_spm_shared->g_oamavltreebyrsvprx);
         p_Oamrsvp != NULL;
         p_Oamrsvp = (SPM_OAM_RSVP_CB_RX *)AVLL_NEXT(v_spm_shared->g_oamavltreebyrsvprx,
                     p_Oamrsvp->spm_oam_rsvp_node))
    {
        printf("------------OAM ID=%d----------------\n", p_Oamrsvp->active_oam_id);
        printf("\tingress=%#x\n", p_Oamrsvp->stRsvplspCfg.Ingress);
        printf("\tegress : %#x\n", p_Oamrsvp->stRsvplspCfg.Egress);
        printf("\ttunnelid : %#x\n", p_Oamrsvp->stRsvplspCfg.TunnelId);
        printf("\tlspid : %#x\n", p_Oamrsvp->stRsvplspCfg.LspId);
        printf("\toamid : %d\n", p_Oamrsvp->active_oam_id);
        printf("\tvp_PortId : %d\n", p_Oamrsvp->vp_PortId);
        printf("\tvp_Slot : %d\n", p_Oamrsvp->vp_Slot);
        printf("\tvp_Lspid : %x\n", p_Oamrsvp->vp_Lspid);
        printf("\tmip_inport_index : %x\n", p_Oamrsvp->inport_index);
    }

    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_txlsp_cb()
{
    SPM_OAM_RSVP_CB_TX *p_Oamrsvp = NULL;

    for (p_Oamrsvp = (SPM_OAM_RSVP_CB_TX *)AVLL_FIRST(v_spm_shared->g_oamavltreebyrsvptx);
         p_Oamrsvp != NULL;
         p_Oamrsvp = (SPM_OAM_RSVP_CB_TX *)AVLL_NEXT(v_spm_shared->g_oamavltreebyrsvptx,
                     p_Oamrsvp->spm_oam_rsvp_node))
    {
        printf("------------OAM ID=%d----------------\n", p_Oamrsvp->active_oam_id);
        printf("\tingress=%#x\n", p_Oamrsvp->stRsvplspCfg.Ingress);
        printf("\tegress : %#x\n", p_Oamrsvp->stRsvplspCfg.Egress);
        printf("\ttunnelid : %#x\n", p_Oamrsvp->stRsvplspCfg.TunnelId);
        printf("\tlspid : %#x\n", p_Oamrsvp->stRsvplspCfg.LspId);
        printf("\toamid : %d\n", p_Oamrsvp->active_oam_id);
        printf("\tvp_PortId : %d\n", p_Oamrsvp->vp_PortId);
        printf("\tvp_Slot : %d\n", p_Oamrsvp->vp_Slot);
        printf("\tvp_Lspid : %#x\n", p_Oamrsvp->vp_Lspid);
        printf("\tmip_inport_index : %#x\n", p_Oamrsvp->inport_index);
    }

    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_pw_cb()
{
    SPM_OAM_PW_CB *p_pwcfg = NULL;

    for (p_pwcfg = (SPM_OAM_PW_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebypw);
         p_pwcfg != NULL;
         p_pwcfg = (SPM_OAM_PW_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebypw,
                   p_pwcfg->spm_oam_pw_node))
    {
        printf("------------OAM ID=%d----------------\n", p_pwcfg->active_oam_id);
        printf("\tvc_id : %#x\n", p_pwcfg->stpwcfg.vc_id);
        printf("\tvc_type : %#x\n", p_pwcfg->stpwcfg.vc_type);
        printf("\tpeer_ip : %#x\n", p_pwcfg->stpwcfg.peer_ip);
        printf("\toamid : %d\n", p_pwcfg->active_oam_id);
        printf("\tvpn_type : %d\n", p_pwcfg->vpn_type);
        printf("\tvp_id : %d\n", p_pwcfg->vp_id);
        printf("\tvc_no_section1 : %d\n", p_pwcfg->vc_no_section1);
        printf("\tvc_no_section2 : %d\n", p_pwcfg->vc_no_section2);
        printf("\tvc_PortId_section1 : %d\n", p_pwcfg->vc_PortId_section1);
        printf("\tvc_PortId_section2 : %d\n", p_pwcfg->vc_PortId_section2);
        printf("\tuni_slot : %d\n", p_pwcfg->uni_slot);
        printf("\tuni_port_type : %d\n", p_pwcfg->uni_port_type);
        printf("\tnni_slot : %d\n", p_pwcfg->nni_slot);
        printf("\tmip_nni_slot1 : %d\n", p_pwcfg->mip_nni_slot1);
        printf("\tmip_nni_slot2 : %d\n", p_pwcfg->mip_nni_slot2);
        printf("\tvc_logical_port_index1 : %d\n", p_pwcfg->vc_logical_port_index1);
        printf("\tvc_logical_port_index2 : %d\n", p_pwcfg->vc_logical_port_index2);
        printf("\tswtich_pw_label_rx1 : %d\n", p_pwcfg->swtich_pw_label_rx1);
        printf("\tswtich_pw_label_rx2 : %d\n", p_pwcfg->swtich_pw_label_rx2);
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_disable_en(NBB_ULONG oamid, NBB_BYTE framtype)
{
    NBB_USHORT ustemp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG baseaddr = 0;
    NBB_USHORT moudleloop = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucLoop = 0;
    NBB_BYTE odLoop = 0;
    iRet = spm_oam_get_local_oamindex(oamid, &usLoop);

    if (iRet != SUCCESS)
    {
        printf("未找到此条oamid\n");
        return;
    }

    spm_get_moudle_base_addr(usLoop, &baseaddr);
    moudleloop = g_stoam_local[usLoop].module_index;

    switch (framtype)
    {
        case 0 ://dm
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                    + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
            ustemp = ustemp & 0xfff0;
            spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                            + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);

            break;

        case 1 ://lb
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                    + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
            ustemp = ustemp & 0xfff0;
            spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                            + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);

            break;

        case 2://aps
            ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);
            ustemp = ustemp & 0xfffd;
            spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

            break;

        case 3://csf
            ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);
            ustemp = ustemp & 0xfffb;
            spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

            break;

        case 4://cv
            ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);
            ustemp = ustemp & 0xfffe;
            spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

            break;

        case 5://lm
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                    + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
            ustemp = ustemp & 0xfff0;
            spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                            + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);
            break;

        case 6 ://lck
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                    + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
            ustemp = ustemp & 0xfff0;
            spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                            + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);

            break;

        case 7 ://tst
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                    + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
            ustemp = ustemp & 0xfff0;
            spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                            + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);

            break;

        default:
            break;
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_disable_all_en(NBB_BYTE framtype, NBB_USHORT begin, NBB_USHORT end)
{
    NBB_USHORT iLoop = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucLoop = 0;
    NBB_BYTE odLoop = 0;
    NBB_USHORT ustemp = 0;
    NBB_ULONG baseaddr = 0;
    NBB_USHORT moudleloop = 0;

    if ((begin >= OAM_MAX_NUM_ACTIVE_SPU) || (end >= OAM_MAX_NUM_ACTIVE_SPU))
    {
        return;
    }

    for (iLoop = begin ; iLoop <= end ; iLoop++)
    {
        spm_get_moudle_base_addr(iLoop, &baseaddr);

        if (0x0 == g_stoam_local[iLoop].ulActiveOamkey)
        {
            continue;
        }

        moudleloop = g_stoam_local[iLoop].module_index;

        switch (framtype)
        {
            case 0 ://dm
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                        + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
                ustemp = ustemp & 0xfff0;
                spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                                + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);

                break;

            case 1 ://lb
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                        + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
                ustemp = ustemp & 0xfff0;
                spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);

                break;

            case 2://aps
                ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);
                ustemp = ustemp & 0xfffd;
                spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);
                break;

            case 3://csf
                ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);
                ustemp = ustemp & 0xfffb;
                spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

                break;

            case 4://cv
                ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);
                ustemp = ustemp & 0xfffe;
                spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

                break;

            case 5://lm
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                        + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
                ustemp = ustemp & 0xfff0;
                spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                                + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);
                break;

            case 6 ://lck
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                        + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
                ustemp = ustemp & 0xfff0;
                spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);

                break;

            case 7 ://tst
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                        + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
                ustemp = ustemp & 0xfff0;
                spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);

                break;

            default:
                break;
        }
    }
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_enable_en(NBB_ULONG oamid, NBB_BYTE framtype)
{
    NBB_USHORT ustemp = 0;
    NBB_USHORT usLoop = 0;
    NBB_USHORT moudleloop = 0;
    NBB_ULONG baseaddr = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucLoop = 0;
    NBB_BYTE odLoop = 0;
    iRet = spm_oam_get_local_oamindex(oamid, &usLoop);

    if (iRet != SUCCESS)
    {
        printf("未找到此条oamid\n");
        return;
    }

    spm_get_moudle_base_addr(usLoop, &baseaddr);
    moudleloop = g_stoam_local[usLoop].module_index;

    switch (framtype)
    {
        case 0 ://dm
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                    + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
            ustemp = ustemp | 0x0001;
            spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                            + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);
            break;

        case 1 ://lb
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                    + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
            ustemp = ustemp | 0x0001;
            spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                            + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);
            break;

        case 2:
            ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);

            if (TRUE == g_stoam_local[usLoop].aps_enable)
            {
                ustemp = ustemp | 0x0002;
            }

            spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

            break;

        case 3://csf
            ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);

            if (TRUE == g_stoam_local[usLoop].csf_enable)
            {
                ustemp = ustemp | 0x0004;
            }

            spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

            break;

        case 4://cv
            ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);

            if (TRUE == g_stoam_local[usLoop].cv_enable)
            {
                ustemp = ustemp | 0x0001;
            }

            spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

            break;

        case 5://lm
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                    + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
            ustemp = ustemp | 0x0001;
            spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                            + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);
            break;

        case 6 ://lck
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                    + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
            ustemp = ustemp | 0x0001;
            spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                            + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);
            break;

        case 7 ://tst
            iRet = spm_oam_get_local_od_oamindex(oamid, &ucLoop);

            if (iRet != SUCCESS)
            {
                printf("未找到此条odoamid\n");
                return;
            }

            odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
            ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                    + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
            ustemp = ustemp | 0x0001;
            spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                            + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);
            break;

        default:
            break;
    }
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_enable_all_en(NBB_BYTE framtype, NBB_USHORT begin, NBB_USHORT end)
{
    NBB_USHORT iLoop = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucLoop = 0;
    NBB_BYTE odLoop = 0;
    NBB_USHORT ustemp = 0;
    NBB_ULONG baseaddr = 0;
    NBB_USHORT moudleloop = 0;

    if ((begin >= OAM_MAX_NUM_ACTIVE_SPU) || (end >= OAM_MAX_NUM_ACTIVE_SPU))
    {
        return;
    }

    for (iLoop = begin ; iLoop <= end ; iLoop++)
    {
        spm_get_moudle_base_addr(iLoop, &baseaddr);

        if (0x0 == g_stoam_local[iLoop].ulActiveOamkey)
        {
            continue;
        }

        moudleloop = g_stoam_local[iLoop].module_index;

        switch (framtype)
        {
            case 0 ://dm
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                        + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
                ustemp = ustemp | 0x0001;
                spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                                + DM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);
                break;

            case 1 ://lb
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                        + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
                ustemp = ustemp | 0x0001;
                spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);
                break;

            case 2://aps
                ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);

                if (TRUE == g_stoam_local[iLoop].aps_enable)
                {
                    ustemp = ustemp | 0x0002;
                }

                spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

                break;

            case 3://csf
                ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);

                if (TRUE == g_stoam_local[iLoop].csf_enable)
                {
                    ustemp = ustemp | 0x0004;
                }

                spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

                break;

            case 4://cv
                ustemp = spm_oam_16read(baseaddr, CV_TXENABLE_OFFSET + moudleloop);

                if (TRUE == g_stoam_local[iLoop].cv_enable)
                {
                    ustemp = ustemp | 0x0001;
                }

                spm_oam_16write(baseaddr, CV_TXENABLE_OFFSET + moudleloop, ustemp);

                break;

            case 5://lm
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_MODULE_OFFSET
                                        + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0);
                ustemp = ustemp | 0x0001;
                spm_oam_16write(baseaddr, DEMAND_MODULE_OFFSET
                                + LM_FRAME_HEAD_OFFSET + odLoop * 10 + 0x0, ustemp);
                break;

            case 6 ://lck
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                        + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
                ustemp = ustemp | 0x0001;
                spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);
                break;

            case 7 ://tst
                iRet = spm_odindex_by_activeindex(iLoop, &ucLoop);

                if (iRet != SUCCESS)
                {
                    printf("未找到此条odoamid\n");
                    return;
                }

                odLoop = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
                ustemp = spm_oam_16read(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                        + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0);
                ustemp = ustemp | 0x0001;
                spm_oam_16write(baseaddr, DEMAND_OAM_MODULE_OFFSET
                                + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0, ustemp);
                break;

            default:
                break;
        }
    }
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_oam_array(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_INT iRet = ERROR;
    NBB_USHORT iloop = 0;
    NBB_BYTE jloop = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    iloop = pOamCfg->oamindex;

    if (oamid != g_stoam_local[iloop].ulActiveOamkey)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    printf("---------------------oam index = %d-----------------------\n", iloop);
    printf("%*s\n", 25, "Active:");

    if (TRUE == g_stoam_local[iloop].aps_enable)
    {
        printf("%-*s%s\n", STRING_LEN, "frame type : ", "APS");
    }

    if (TRUE == g_stoam_local[iloop].csf_enable)
    {
        printf("%-*s%s\n", STRING_LEN, "frame type :", "CSF");
    }

    if (TRUE == g_stoam_local[iloop].cv_enable)
    {
        printf("%-*s%s\n", STRING_LEN, "frame type :", "CV");
    }

    if (TRUE == g_stoam_local[iloop].fdi_enable)
    {
        printf("%-*s%s\n", STRING_LEN, "frame type :", "FDI");
    }

    printf("%-*s%d\n", STRING_LEN, "oam type :", g_stoam_local[iloop].oam_type);
    printf("%-*s%d\n", STRING_LEN, "VP attribute : ", g_stoam_local[iloop].vp_attrib);

    switch (g_stoam_local[iloop].oam_type)
    {
        case WRAPPING_OAM :
        case VS_OAM :
            printf("%-*s%#x\n", STRING_LEN, "ip : ", g_stoam_local[iloop].ip);
            printf("%-*s%d\n", STRING_LEN, "portid : ", g_stoam_local[iloop].vs_Portid);
            printf("%-*s%d\n", STRING_LEN, "slotid : ", g_stoam_local[iloop].vs_Slotid);
            printf("%-*s%d\n", STRING_LEN, "wrapping port index : ",
                   g_stoam_local[iloop].wrapping_port_logical_index);
            break;

        case VP_OAM :
            if (0x0 == g_stoam_local[iloop].vp_attrib)
            {
                printf("%-*s%d\n", STRING_LEN, "inport :", g_stoam_local[iloop].inport1);
                printf("%-*s%#x\n", STRING_LEN, "lspid : ", g_stoam_local[iloop].lspid1);

//                printf("%-*s%d\n", STRING_LEN, "inport2 :", g_stoam_local[iloop].inport2);
//                printf("%-*s%#x\n", STRING_LEN, "lspid2 : ", g_stoam_local[iloop].lspid2);
            }
            else
            {
                printf("%-*s%#x\n", STRING_LEN, "lspid : ", g_stoam_local[iloop].vp_Lspid);
                printf("%-*s%#x\n", STRING_LEN, "portid : ", g_stoam_local[iloop].vp_PortId);
                printf("%-*s%#x\n", STRING_LEN, "slotid : ", g_stoam_local[iloop].vp_Slot);
            }

            break;

        case VC_OAM :
            printf("%-*s%d\n", STRING_LEN, "vc_no : ", g_stoam_local[iloop].vc_no_section1);
            printf("%-*s%d\n", STRING_LEN, "vc_ces_oamIdx : ", g_stoam_local[iloop].vc_ces_oamIdx);

//            printf("%-*s%d\n", STRING_LEN, "vc_no2 : ", g_stoam_local[iloop].vc_no_section2);
            printf("%-*s%d\n", STRING_LEN, "vc_portid : ", g_stoam_local[iloop].vc_PortId_section1);

//            printf("%-*s%d\n", STRING_LEN, "vc_portid2 : ", g_stoam_local[iloop].vc_PortId_section2);
            printf("%-*s%d\n", STRING_LEN, "uni_slot : ", g_stoam_local[iloop].uni_slot);
            printf("%-*s%d\n", STRING_LEN, "uni_port_type : ", g_stoam_local[iloop].uni_port_type);
            printf("%-*s%d\n", STRING_LEN, "nni_slot : ", g_stoam_local[iloop].nni_slot);
            printf("%-*s%d\n", STRING_LEN, "vpn_type : ", g_stoam_local[iloop].vpn_type);
            printf("%-*s%d\n", STRING_LEN, "vp_id : ", g_stoam_local[iloop].vp_id);
            break;

        default:
            printf("oam type is wrong!\n");
            return;
    }


    printf("%-*s%#x%x%x%x%x%x\n",
           STRING_LEN, "MEG ICC : ",
           g_stoam_local[iloop].meg_icc[0],
           g_stoam_local[iloop].meg_icc[1],
           g_stoam_local[iloop].meg_icc[2],
           g_stoam_local[iloop].meg_icc[3],
           g_stoam_local[iloop].meg_icc[4],
           g_stoam_local[iloop].meg_icc[5]);
    printf("%-*s%#x%x%x%x%x%x\n",
           STRING_LEN, "MEG UMC : ",
           g_stoam_local[iloop].meg_umc[0],
           g_stoam_local[iloop].meg_umc[1],
           g_stoam_local[iloop].meg_umc[2],
           g_stoam_local[iloop].meg_umc[3],
           g_stoam_local[iloop].meg_umc[4],
           g_stoam_local[iloop].meg_umc[5]);
    printf("%-*s%d\n", STRING_LEN, "Source MEP ID : ", g_stoam_local[iloop].src_mep_id);
    printf("%-*s%d\n", STRING_LEN, "Destination MEP ID : ", g_stoam_local[iloop].dst_mep_id);
    printf("%-*s%d\n", STRING_LEN, "Mel : ", g_stoam_local[iloop].mel);
    printf("%-*s%d\n", STRING_LEN, "LSP_EXP : ", g_stoam_local[iloop].lsp_exp);
    printf("%-*s%d\n", STRING_LEN, "PW_EXP : ", g_stoam_local[iloop].pw_exp);
    printf("%-*s%d\n", STRING_LEN, "gal_EXP : ", g_stoam_local[iloop].gal_exp);
    printf("%-*s%d\n", STRING_LEN, "lsp_ttl : ", g_stoam_local[iloop].lsp_ttl);
    printf("%-*s%d\n", STRING_LEN, "PW_ttl : ", g_stoam_local[iloop].pw_ttl);
    printf("%-*s%d\n", STRING_LEN, "gal_ttl :", g_stoam_local[iloop].gal_ttl);

//    printf("%-*s%d\n", STRING_LEN, "lsp lable tx : ", g_stoam_local[iloop].ulTunnelLabel_tx);
//    printf("%-*s%d\n", STRING_LEN, "pw lable tx  : ", g_stoam_local[iloop].ulPwLable_tx);
    printf("%-*s%d\n", STRING_LEN, "lsp lable rx   : ", g_stoam_local[iloop].ulTunnelLabel_rx);
    printf("%-*s%d\n", STRING_LEN, "pw lable rx  : ", g_stoam_local[iloop].ulPwLable_rx);
    printf("%-*s%d\n", STRING_LEN, "module : ", g_stoam_local[iloop].bModuleSwitch);
    printf("%-*s%d\n", STRING_LEN, "index_in_module : ", g_stoam_local[iloop].module_index);
    printf("%-*s%d\n", STRING_LEN, "index_in_array : ", g_stoam_local[iloop].global_index);
    printf("%-*s%d\n", STRING_LEN, "oamkey : ", g_stoam_local[iloop].ulActiveOamkey);
    printf("%-*s%d\n", STRING_LEN, "isRelatedProct : ", g_stoam_local[iloop].bIsRelateProt);
    iRet = spm_odindex_by_activeindex(iloop, &jloop);

    if (SUCCESS != iRet)
    {
        return;
    }

    printf("****************OD CFG*******************\n");
    printf("%-*s%d\n", STRING_LEN, "od_index_in_od_array :", jloop);
    printf("%-*s\n", STRING_LEN, "(1)od basic cfg:");

    if (NULL == g_stoam_odlocal[jloop].pBasicData)
    {
        return;
    }

    printf("\t%-*s%d\n", STRING_LEN, "Mel :", g_stoam_odlocal[jloop].pBasicData->mel);
    printf("\t%-*s%d\n", STRING_LEN, "LSP_EXP :", g_stoam_odlocal[jloop].pBasicData->lsp_exp);
    printf("\t%-*s%d\n", STRING_LEN, "PW_EXP = ", g_stoam_odlocal[jloop].pBasicData->pw_exp);
    printf("\t%-*s%d\n", STRING_LEN, "gal_EXP :", g_stoam_odlocal[jloop].pBasicData->gal_exp);
    printf("\t%-*s%d\n", STRING_LEN, "lsp_ttl :", g_stoam_odlocal[jloop].pBasicData->lsp_ttl);
    printf("\t%-*s%d\n", STRING_LEN, "PW_ttl :", g_stoam_odlocal[jloop].pBasicData->pw_ttl);
    printf("\t%-*s%d\n", STRING_LEN, "gal_ttl :", g_stoam_odlocal[jloop].pBasicData->gal_ttl);
    printf("%-*s\n", STRING_LEN, "(2)od lb cfg:");

    if (NULL != g_stoam_odlocal[jloop].pLoopFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "lb oam_enable_en :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "lb_send_cycle :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "lb_test_method :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.test_method);
        printf("\t%-*s%d\n", STRING_LEN, "离线方式测试tlv类型 :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.test_tlv_type);
        printf("\t%-*s%d\n", STRING_LEN, "LB模式 :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.lb_mode);
        printf("\t%-*s%d\n", STRING_LEN, "数据tlv测试内容 : ",
               g_stoam_odlocal[jloop].pLoopFrame->tlv_test_content);
        printf("\t%-*s%d\n", STRING_LEN, "TLV长度 : ",
               g_stoam_odlocal[jloop].pLoopFrame->tlv_length);
        printf("\t%-*s%s\n", STRING_LEN, "Target TLV模式 : ",
               g_stoam_odlocal[jloop].pLoopFrame->tlv_mode ? "MIP TLV" : "MEP TLV");
        printf("\t%-*s%d\n", STRING_LEN, "Requesting Mep ID TLV: ",
               g_stoam_odlocal[jloop].pLoopFrame->mep_id_enable);
        printf("\t%-*s%d\n", STRING_LEN, "目的MEP ID: ",
               g_stoam_odlocal[jloop].pLoopFrame->dest_mep_id);
    }

    printf("%-*s\n", STRING_LEN, "(3)od tst cfg:");

    if (NULL != g_stoam_odlocal[jloop].pTestFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "tst oam_enable_en :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "TST发送周期 :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "TST发送方式 :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.send_method);
        printf("\t%-*s%s\n", STRING_LEN, "测试TLV类型 :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.test_tlv_type ? "离线" : "在线");
        printf("\t%-*s%d\n", STRING_LEN, "tlv_length :",
               g_stoam_odlocal[jloop].pTestFrame->tlv_length);
    }

    printf("%-*s\n", STRING_LEN, "(4)od lck cfg:");

    if (NULL != g_stoam_odlocal[jloop].pLckFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "lck oam_enable_en :", 
            g_stoam_odlocal[jloop].pLckFrame->lck_frame_send_enable);
    }

    printf("%-*s\n", STRING_LEN, "(5)od lm cfg:");

    if (NULL != g_stoam_odlocal[jloop].pLmFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "lm_enable:", g_stoam_odlocal[jloop].pLmFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "lm_cycle:", g_stoam_odlocal[jloop].pLmFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "count_mode:", g_stoam_odlocal[jloop].pLmFrame->byte1.count_mode);
        printf("\t%-*s%d\n", STRING_LEN, "lmid:", g_stoam_odlocal[jloop].pLmFrame->byte1.lmid);
    }

    printf("%-*s\n", STRING_LEN, "(6)od lm cfg:");

    if (NULL != g_stoam_odlocal[jloop].pDmFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "dm_enable:", g_stoam_odlocal[jloop].pDmFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "dm_cycle:", g_stoam_odlocal[jloop].pDmFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "tlv_type(data/test):", g_stoam_odlocal[jloop].pDmFrame->byte1.test_tlv_type);
        printf("\t%-*s%d\n", STRING_LEN, "tlv_code_type(0/random):",
               g_stoam_odlocal[jloop].pDmFrame->byte1.tlv_code_pattern);
        printf("\t%-*s%d\n", STRING_LEN, "tlv_content:", g_stoam_odlocal[jloop].pDmFrame->content);
        printf("\t%-*s%d\n", STRING_LEN, "dm_frame_length:", g_stoam_odlocal[jloop].pDmFrame->frame_length);
    }

    printf("%-*s\n", STRING_LEN, "(7)od mip cfg:");

    if (NULL != g_stoam_odlocal[jloop].pMip)
    {
        return;
    }

    printf("\t%-*s%d\n", STRING_LEN, "oamid:", g_stoam_odlocal[jloop].ulOamActiveKey);
    printf("\t%-*s%d\n", STRING_LEN, "OamLoop:", g_stoam_odlocal[jloop].usOamLoop);
    printf("\t%-*s%d\n", STRING_LEN, "lbLoop:", g_stoam_odlocal[jloop].ucOdLoop);
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_oam_cb(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT iloop = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE jloop = 0;

    //SPM_REFRESH_INIT_RELATED

    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    iloop = pOamCfg->oamindex;
    printf("oamkey : %15d\n", oamid);
    printf("**********************ACTIVE CFG********************\n");

    /* @refresh */
    //SPM_REFRESH_SHOW_RELATED(pOamCfg)

//BASIC_CFG :
    printf("(1)Basic Cfg:\n");

    if (NULL == pOamCfg->pBasiCfg)
    {
        goto PW_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "VP attribute :", pOamCfg->pBasiCfg->vp_attrib);
    printf("\t%-*s%d\n", STRING_LEN, "oam type :", pOamCfg->oamtype);

    if (0x0 == g_stoam_local[iloop].vp_attrib)
    {
        printf("\t%-*s%d\n", STRING_LEN, "inport1 : ", g_stoam_local[iloop].inport1);
        printf("\t%-*s%#x\n", STRING_LEN, "lspid1 : ", g_stoam_local[iloop].lspid1);
        printf("\t%-*s%d\n", STRING_LEN, "inport2 : ", g_stoam_local[iloop].inport2);
        printf("\t%-*s%#x\n", STRING_LEN, "lspid2 : ", g_stoam_local[iloop].lspid2);
    }

    printf("\t%-*s%#x%x%x%x%x%x\n",
           STRING_LEN, "MEG ICC : ",
           pOamCfg->pBasiCfg->meg_icc[0],
           pOamCfg->pBasiCfg->meg_icc[1],
           pOamCfg->pBasiCfg->meg_icc[2],
           pOamCfg->pBasiCfg->meg_icc[3],
           pOamCfg->pBasiCfg->meg_icc[4],
           pOamCfg->pBasiCfg->meg_icc[5]);
    printf("\t%-*s%#x%x%x%x%x%x\n",
           STRING_LEN, "MEG UMC : ",
           pOamCfg->pBasiCfg->meg_umc[0],
           pOamCfg->pBasiCfg->meg_umc[1],
           pOamCfg->pBasiCfg->meg_umc[2],
           pOamCfg->pBasiCfg->meg_umc[3],
           pOamCfg->pBasiCfg->meg_umc[4],
           pOamCfg->pBasiCfg->meg_umc[5]);
    printf("\t%-*s%d\n", STRING_LEN, "Source MEP ID : ", pOamCfg->pBasiCfg->src_mep_id);
    printf("\t%-*s%d\n", STRING_LEN, "Destination MEP ID : ", pOamCfg->pBasiCfg->dst_mep_id);
    printf("\t%-*s%d\n", STRING_LEN, "Mel : ", pOamCfg->pBasiCfg->mel);
    printf("\t%-*s%d\n", STRING_LEN, "LSP_EXP : ", pOamCfg->pBasiCfg->lsp_exp);
    printf("\t%-*s%d\n", STRING_LEN, "PW_EXP : ", pOamCfg->pBasiCfg->pw_exp);
    printf("\t%-*s%d\n", STRING_LEN, "gal_EXP : ", pOamCfg->pBasiCfg->gal_exp);
    printf("\t%-*s%d\n", STRING_LEN, "lsp_ttl : ", pOamCfg->pBasiCfg->lsp_ttl);
    printf("\t%-*s%d\n", STRING_LEN, "PW_ttl : ", pOamCfg->pBasiCfg->pw_ttl);
    printf("\t%-*s%d\n", STRING_LEN, "gal_ttl : ", pOamCfg->pBasiCfg->gal_ttl);

//    printf("\t%-*s%d\n", STRING_LEN, "lsp lable tx : ", pOamCfg->ulTunnelLabel_tx);
//    printf("\t%-*s%d\n", STRING_LEN, "pw lable tx : ", pOamCfg->ulPwLable_tx);
    printf("\t%-*s%d\n", STRING_LEN, "lsp lable rx : ", pOamCfg->ulTunnelLabel_rx);
    printf("\t%-*s%d\n", STRING_LEN, "pw lable rx : ", pOamCfg->ulPwLable_rx);
    printf("\t%-*s%d\n", STRING_LEN, "module : ", pOamCfg->module);
    printf("\t%-*s%d\n", STRING_LEN, "index_in_module : ", pOamCfg->module_index);
    printf("\t%-*s%d\n", STRING_LEN, "index_in_global : ", pOamCfg->oamindex);
    printf("\t%-*s%s\n", STRING_LEN, "transaction type",
           pOamCfg->mcFlag ? "CES" : "Ethernet");
    printf("\t%-*s%d\n", STRING_LEN, pOamCfg->mcFlag ? "mcid:" : "slot:", pOamCfg->slotMcid);
    printf("\t%-*s%d\n", STRING_LEN, "IsAddSuccess", pOamCfg->IsaddSuccess);
PW_CFG:
    printf("%-*s\n", STRING_LEN, "(4)pw cfg:");

    if (NULL == pOamCfg->pPwCfg)
    {
        goto WRAPPING_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "vc id :", pOamCfg->pPwCfg->vc_id);
    printf("\t%-*s%d\n", STRING_LEN, "vc type :", pOamCfg->pPwCfg->vc_type);
    printf("\t%-*s%#x\n", STRING_LEN, "peer ip :", pOamCfg->pPwCfg->peer_ip);
    printf("\t%-*s%d\n", STRING_LEN, "lag id :", pOamCfg->lag_id);
WRAPPING_CFG:
    printf("%-*s\n", STRING_LEN, "(5)wrapping cfg:");

    if (NULL == pOamCfg->pWrapping)
    {
        goto VS_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "ring id :", pOamCfg->pWrapping->ring_id);
    printf("\t%-*s%d\n", STRING_LEN, "direction :", pOamCfg->pWrapping->direction_select);
VS_CFG:
    printf("%-*s\n", STRING_LEN, "(6)vs cfg:");

    if (NULL == pOamCfg->pVsCfg)
    {
        goto APS_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "port index :", pOamCfg->pVsCfg->index);
    printf("\t%-*s%#x\n", STRING_LEN, "next hop ip :", pOamCfg->pVsCfg->nxhop_ip);
APS_CFG:
    printf("%-*s\n", STRING_LEN, "(7)aps cfg:");

    if (NULL == pOamCfg->pApscfg)
    {
        goto CV_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "aps oam_enable_en :", pOamCfg->pApscfg->aps_enable);
CV_CFG:
    printf("%-*s\n", STRING_LEN, "(8)cv cfg:");

    if (NULL == pOamCfg->pCvcfg)
    {
        goto CSF_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "cv_enable : ", pOamCfg->pCvcfg->cv_enable);
    printf("\t%-*s%d\n", STRING_LEN, "cv_cycle : ", pOamCfg->pCvcfg->cv_cycle);
CSF_CFG:
    printf("%-*s\n", STRING_LEN, "(9)csf cfg:");

    if (NULL == pOamCfg->pCsfcfg)
    {
        goto FDI_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "csf_enable : ", pOamCfg->pCsfcfg->csf_enable);
FDI_CFG:
    printf("%-*s\n", STRING_LEN, "(10)fdi cfg:");

    if (NULL == pOamCfg->pFdicfg)
    {
        goto RSVP_CFG;
    }

    printf("\t%-*s%d\n", STRING_LEN, "fdi_enable : ", pOamCfg->pFdicfg->fdi_enable);
RSVP_CFG:
    printf("%-*s\n", STRING_LEN, "(11)rsvp cfg:");

    if (NULL == pOamCfg->pRsvplspCfg)
    {
        goto OD_CFG;
    }

    printf("\t正向路径:\n");
    printf("\t%-*s%#x\n", STRING_LEN, "ingress :", pOamCfg->pRsvplspCfg->tx_ingress);
    printf("\t%-*s%#x\n", STRING_LEN, "egress :", pOamCfg->pRsvplspCfg->tx_egress);
    printf("\t%-*s%#x\n", STRING_LEN, "tunnel id :", pOamCfg->pRsvplspCfg->tx_tunnel_id);
    printf("\t%-*s%#x\n", STRING_LEN, "lsp id :", pOamCfg->pRsvplspCfg->tx_lsp_id);
    printf("\t反向路径:\n");
    printf("\t%-*s%#x\n", STRING_LEN, "ingress :", pOamCfg->pRsvplspCfg->rx_ingress);
    printf("\t%-*s%#x\n", STRING_LEN, "egress :", pOamCfg->pRsvplspCfg->rx_egress);
    printf("\t%-*s%#x\n", STRING_LEN, "tunnel id :", pOamCfg->pRsvplspCfg->rx_tunnel_id);
    printf("\t%-*s%#x\n", STRING_LEN, "lsp id :", pOamCfg->pRsvplspCfg->rx_lsp_id);
OD_CFG:
    iRet = spm_odindex_by_activeindex(iloop, &jloop);

    if (SUCCESS != iRet)
    {
        return;
    }

    printf("****************OD CFG*******************\n");
    printf("%-*s%d\n", STRING_LEN, "od_index_in_od_array :", jloop);
    printf("%-*s\n", STRING_LEN, "(1)od basic cfg:");

    if (NULL == g_stoam_odlocal[jloop].pBasicData)
    {
        return;
    }

    printf("\t%-*s%d\n", STRING_LEN, "Mel :", g_stoam_odlocal[jloop].pBasicData->mel);
    printf("\t%-*s%d\n", STRING_LEN, "LSP_EXP :", g_stoam_odlocal[jloop].pBasicData->lsp_exp);
    printf("\t%-*s%d\n", STRING_LEN, "PW_EXP : ", g_stoam_odlocal[jloop].pBasicData->pw_exp);
    printf("\t%-*s%d\n", STRING_LEN, "gal_EXP :", g_stoam_odlocal[jloop].pBasicData->gal_exp);
    printf("\t%-*s%d\n", STRING_LEN, "lsp_ttl :", g_stoam_odlocal[jloop].pBasicData->lsp_ttl);
    printf("\t%-*s%d\n", STRING_LEN, "PW_ttl :", g_stoam_odlocal[jloop].pBasicData->pw_ttl);
    printf("\t%-*s%d\n", STRING_LEN, "gal_ttl :", g_stoam_odlocal[jloop].pBasicData->gal_ttl);
    printf("%-*s\n", STRING_LEN, "(2)od lb cfg:");

    if (NULL != g_stoam_odlocal[jloop].pLoopFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "lb oam_enable_en :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "lb_send_cycle :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "lb_test_method :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.test_method);
        printf("\t%-*s%d\n", STRING_LEN, "离线方式测试tlv类型 :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.test_tlv_type);
        printf("\t%-*s%d\n", STRING_LEN, "LB模式 :",
               g_stoam_odlocal[jloop].pLoopFrame->byte1.lb_mode);
        printf("\t%-*s%d\n", STRING_LEN, "数据tlv测试内容 : ",
               g_stoam_odlocal[jloop].pLoopFrame->tlv_test_content);
        printf("\t%-*s%d\n", STRING_LEN, "TLV长度 : ",
               g_stoam_odlocal[jloop].pLoopFrame->tlv_length);
        printf("\t%-*s%s\n", STRING_LEN, "Target TLV模式 : ",
               g_stoam_odlocal[jloop].pLoopFrame->tlv_mode ? "MIP TLV" : "MEP TLV");
        printf("\t%-*s%d\n", STRING_LEN, "Requesting Mep ID TLV: ",
               g_stoam_odlocal[jloop].pLoopFrame->mep_id_enable);
        printf("\t%-*s%d\n", STRING_LEN, "目的MEP ID: ",
               g_stoam_odlocal[jloop].pLoopFrame->dest_mep_id);
    }

    printf("%-*s\n", STRING_LEN, "(3)od tst cfg:");

    if (NULL != g_stoam_odlocal[jloop].pTestFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "tst oam_enable_en :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "TST发送周期 :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "TST发送方式 :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.send_method);
        printf("\t%-*s%s\n", STRING_LEN, "测试TLV类型 :",
               g_stoam_odlocal[jloop].pTestFrame->byte1.test_tlv_type ? "离线" : "在线");
        printf("\t%-*s%d\n", STRING_LEN, "tlv_length :",
               g_stoam_odlocal[jloop].pTestFrame->tlv_length);
    }

    printf("%-*s\n", STRING_LEN, "(4)od lck cfg:");

    if (NULL != g_stoam_odlocal[jloop].pLckFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "lck oam_enable_en :", 
            g_stoam_odlocal[jloop].pLckFrame->lck_frame_send_enable);
    }

    printf("%-*s\n", STRING_LEN, "(5)od lm cfg:");

    if (NULL != g_stoam_odlocal[jloop].pLmFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "lm_enable:",
               g_stoam_odlocal[jloop].pLmFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "lm_cycle:",
               g_stoam_odlocal[jloop].pLmFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "count_mode:",
               g_stoam_odlocal[jloop].pLmFrame->byte1.count_mode);
        printf("\t%-*s%d\n", STRING_LEN, "lmid:",
               g_stoam_odlocal[jloop].pLmFrame->byte1.lmid);
    }

    printf("%-*s\n", STRING_LEN, "(6)od lm cfg:");

    if (NULL != g_stoam_odlocal[jloop].pDmFrame)
    {
        printf("\t%-*s%d\n", STRING_LEN, "dm_enable:",
               g_stoam_odlocal[jloop].pDmFrame->byte1.send_enable);
        printf("\t%-*s%d\n", STRING_LEN, "dm_cycle:",
               g_stoam_odlocal[jloop].pDmFrame->byte1.send_cycle);
        printf("\t%-*s%d\n", STRING_LEN, "tlv_type(data/test):",
               g_stoam_odlocal[jloop].pDmFrame->byte1.test_tlv_type);
        printf("\t%-*s%d\n", STRING_LEN, "tlv_code_type(0/random):",
               g_stoam_odlocal[jloop].pDmFrame->byte1.tlv_code_pattern);
        printf("\t%-*s%d\n", STRING_LEN, "tlv_content:",
               g_stoam_odlocal[jloop].pDmFrame->content);
        printf("\t%-*s%d\n", STRING_LEN, "dm_frame_length:",
               g_stoam_odlocal[jloop].pDmFrame->frame_length);
    }

    printf("%-*s\n", STRING_LEN, "(7)od mip cfg:");

    if (NULL != g_stoam_odlocal[jloop].pMip)
    {
        printf("%-*s%d\n", STRING_LEN, "expect_mep_id:", g_stoam_odlocal[jloop].pMip->expect_mep_id);
        printf("%-*s%d\n", STRING_LEN, "cc:",
               g_stoam_odlocal[jloop].pMip->cc);
        printf("%-*s%d\n", STRING_LEN, "if_num:", g_stoam_odlocal[jloop].pMip->if_num);
        printf("%-*s%d\n", STRING_LEN, "mip_icc:", g_stoam_odlocal[jloop].pMip->mip_icc);
        printf("%-*s%d\n", STRING_LEN, "mip_id:", g_stoam_odlocal[jloop].pMip->mip_id);
        printf("%-*s%d\n", STRING_LEN, "node_id:", g_stoam_odlocal[jloop].pMip->node_id);
        printf("%-*s%d\n", STRING_LEN, "probe_jump:", g_stoam_odlocal[jloop].pMip->probe_jump);
        printf("%-*s%d\n", STRING_LEN, "tlv_select:", g_stoam_odlocal[jloop].pMip->tlv_select);
    }

    printf("%-*s%d\n", STRING_LEN, "oamid:", g_stoam_odlocal[jloop].ulOamActiveKey);
    printf("%-*s%d\n", STRING_LEN, "OamLoop:", g_stoam_odlocal[jloop].usOamLoop);
    printf("%-*s%d\n", STRING_LEN, "odLoop:", g_stoam_odlocal[jloop].ucOdLoop);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_rev_frame_label(NBB_BYTE module)
{
    NBB_USHORT ustemp = 0;
    NBB_ULONG baseaddr = 0;
    NBB_ULONG LspLabel = 0;
    NBB_ULONG PwLabel = 0;

    if (0x0 == module)
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }
    else if (0x1 == module)
    {
        baseaddr = OAM_REG_BASE2_ADDRESS;
    }
    else
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }

    ustemp = spm_oam_16read(baseaddr, REV_LABEL);
    LspLabel = ustemp;
    ustemp = spm_oam_16read(baseaddr, REV_LABEL + 1);
    LspLabel = ((LspLabel << 12) & 0x000FF000) + ((ustemp >> 4) & 0xfff);
    printf("rev lsp label=%#lx\n", LspLabel);
    PwLabel = ustemp & 0xf;
    ustemp = spm_oam_16read(baseaddr, REV_LABEL + 2);
    PwLabel = ((PwLabel << 16) & 0xf0000) + ustemp;
    printf("rev pw label=%#lx\n", PwLabel);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_rev_lsp_no(NBB_BYTE module)
{
    NBB_USHORT ustemp = 0;
    NBB_ULONG baseaddr = 0;
    NBB_USHORT btemp = 0;

    if (0x0 == module)
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }
    else if (0x1 == module)
    {
        baseaddr = OAM_REG_BASE2_ADDRESS;
    }

    ustemp = spm_oam_16read(baseaddr, TREE_USED_OFFSET);
    btemp = ustemp & 0xff00;
    printf("rev lsp =%#x\n", btemp);
    ustemp = spm_oam_16read(baseaddr, TREE_USED_OFFSET - 1);
    btemp = (ustemp >> 4) & 0x1;
    printf("search tree no =%#x\n", btemp);
    return;
}



/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_change_tree_of_fpga(NBB_BYTE module)
{
    NBB_ULONG baseaddr = 0;
    NBB_USHORT temp = 0;

    if (0x0 == module)
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }
    else if (0x1 == module)
    {
        baseaddr = OAM_REG_BASE2_ADDRESS;
    }
    else
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }

    temp = spm_oam_16read(baseaddr, TREE_USED_OFFSET);
    temp |= 0x8;
    spm_oam_16write(baseaddr , TREE_USED_OFFSET , temp);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_rev_od_frame(NBB_BYTE module)
{
#if 0
    NBB_USHORT ustemp = 0;
    NBB_ULONG baseaddr = 0;
    NBB_USHORT btemp = 0;

    if (0x0 == module)
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }
    else if (0x1 == module)
    {
        baseaddr = OAM_REG_BASE2_ADDRESS;
    }
    else
    {
        baseaddr = OAM_REG_BASE1_ADDRESS;
    }

    ustemp = spm_oam_16read(baseaddr, REV_OD_LSPNO);
    btemp = ustemp & 0x0f;
    printf("rev od index=%d\n", btemp);

    btemp = (ustemp >> 8) & 0x3;

    switch (btemp)
    {
        case 0 :
            printf("rev od level=%s\n", "TMP");
            break;

        case 1 :
            printf("rev od level=%s\n", "TMS");
            break;

        case 2 :
            printf("rev od level=%s\n", "TMC");
            break;

        case 3 :
            printf("rev od level=%s\n", "1731");
            break;

        default:
            break;
    }

    btemp = (ustemp >> 4) & 0xf;

    switch (btemp)
    {
        case 0 :
            printf("lbm\n");
            break;

        case 1 :
            printf("lbr\n");
            break;

        case 2 :
            printf("test\n");
            break;

        case 3 :
            printf("lck\n");
            break;

        case 4 :
            printf("dmm\n");
            break;

        case 5 :
            printf("dmr\n");
            break;

        default:
            break;
    }
#endif
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_log()
{
    BMU_SLOG(BMU_CRIT, SPM_OAM_LOG_DIR, "");
}


/*****************************************************************************
 函 数 名  : oam_show_cv_cnt
 功能描述  : show cv cnt
 输入参数  : NBB_ULONG oamid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_show_cv_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT rx_temp = 0;
    NBB_USHORT tx_temp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    rx_temp = spm_oam_16read(base_addr, CV_CNT_RX_OAM + modoule_index);
    g_rx_cnt[usLoop] = rx_temp;
    tx_temp = spm_oam_16read(base_addr, CV_CNT_TX_OAM + modoule_index);
    g_tx_cnt[usLoop] = tx_temp;
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\tcv_tx:\t%ld\n", g_tx_cnt[usLoop]);
    printf("\tcv_rx:\t%ld\n", g_rx_cnt[usLoop]);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_alarm(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT alarm_temp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_USHORT expect_mep = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    printf("---------OAM ID=%d--------------\n", oamid);
    alarm_temp = spm_oam_16read(base_addr, ALARM_ADDR + modoule_index);

    if (0x1 == ((alarm_temp >> 3) & 0x1))
    {
        printf("\talarm:\trdi\n");
    }

    if (0x1 == ((alarm_temp >> 1) & 0x1))
    {
        printf("\talarm:\tUNM,mep_id not match\n");
        expect_mep = spm_oam_16read(base_addr, CV_EXPECT_MEP_OFFSET + modoule_index);
        printf("\texpect mep id:\t%d\n", expect_mep);
    }

    if (0x1 == ((alarm_temp >> 0) & 0x1))
    {
        printf("\talarm:\tMMG,meg_id not match\n");
    }

    if (0x1 == ((alarm_temp >> 2) & 0x1))
    {
        printf("\talarm:\tUNP,cv cycle not match\n");
    }

    if (0x1 == ((alarm_temp >> 4) & 0x1))
    {
        printf("\talarm:\tLOC\n");
    }

    if (0x1 == ((alarm_temp >> 6) & 0x1))
    {
        printf("\talarm:\tFDI\n");
    }

    if (0x1 == ((alarm_temp >> 5) & 0x1))
    {
        printf("\talarm:\tAPS\n");
    }

    if (0x1 == ((alarm_temp >> 7) & 0x1))
    {
        printf("\talarm:\tCSF\n");
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_alarm_for_auto(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT alarm_temp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_USHORT expect_mep = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    printf("---------OAM ID=%d--------------\n", oamid);
    alarm_temp = spm_oam_16read(base_addr, ALARM_ADDR + modoule_index);

    if (0x1 == ((alarm_temp >> 3) & 0x1))
    {
        printf("\talarm:\trdi\n");
    }
    else
    {
        printf("\talarm:\tno_rdi\n");
    }

    if (0x1 == ((alarm_temp >> 1) & 0x1))
    {
        printf("\talarm:\tUNM,mep_id not match\n");
        expect_mep = spm_oam_16read(base_addr, CV_EXPECT_MEP_OFFSET + modoule_index);
        printf("\texpect mep id:\t%d\n", expect_mep);
    }
    else
    {
        printf("\talarm:\tno_UNM\n");
    }

    if (0x1 == ((alarm_temp >> 0) & 0x1))
    {
        printf("\talarm:\tMMG,meg_id not match\n");
    }
    else
    {
        printf("\talarm:\tno_MMG\n");
    }

    if (0x1 == ((alarm_temp >> 2) & 0x1))
    {
        printf("\talarm:\tUNP,cv cycle not match\n");
    }
    else
    {
        printf("\talarm:\tno_UNP\n");
    }

    if (0x1 == ((alarm_temp >> 4) & 0x1))
    {
        printf("\talarm:\tLOC\n");
    }
    else
    {
        printf("\talarm:\tno_LOC\n");
    }

    if (0x1 == ((alarm_temp >> 6) & 0x1))
    {
        printf("\talarm:\tFDI\n");
    }
    else
    {
        printf("\talarm:\tno_FDI\n");
    }

    if (0x1 == ((alarm_temp >> 5) & 0x1))
    {
        printf("\talarm:\tAPS\n");
    }
    else
    {
        printf("\talarm:\tno_APS\n");
    }

    if (0x1 == ((alarm_temp >> 7) & 0x1))
    {
        printf("\talarm:\tCSF\n");
    }
    else
    {
        printf("\talarm:\tno_CSF\n");
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_enable(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_USHORT enable_temp = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE odLoop = 0;
    NBB_BYTE odModuleIndex = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    enable_temp = spm_oam_16read(base_addr, CV_TXENABLE_OFFSET + modoule_index);
    printf("---------OAM ID=%d--------------\n", oamid);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tCV\n");
    }

    if (0x1 == ((enable_temp >> 1) & 0x1))
    {
        printf("\tenable:\tAPS\n");
    }

    if (0x1 == ((enable_temp >> 2) & 0x1))
    {
        printf("\tenable:\tCSF\n");
    }

    enable_temp = spm_oam_16read(base_addr, FDI_TX_CFG_OFFSET + modoule_index);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tFDI\n");
    }

    iRet = spm_oam_get_local_od_oamindex(oamid, &odLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    odModuleIndex = g_stoam_odlocal[odLoop].ucOdLoop - 1;
    enable_temp = spm_oam_16read(base_addr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET
                                 + odModuleIndex * 0X8);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tLB\n");
    }

    enable_temp = spm_oam_16read(base_addr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET
                                 + odModuleIndex * 0X8);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tTEST\n");
    }

    enable_temp = spm_oam_16read(base_addr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET
                                 + odModuleIndex * 0X8);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tLCK\n");
    }

    enable_temp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                                 + DM_FRAME_HEAD_OFFSET
                                 + odModuleIndex * 0X10);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tDM\n");
    }

    enable_temp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                                 + LM_FRAME_HEAD_OFFSET
                                 + odModuleIndex * 0X10);

    if (0x1 == (enable_temp & 0x1))
    {
        printf("\tenable:\tLM\n");
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_aps_port(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_BYTE aps_port = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_BYTE bitset = 0;
    NBB_USHORT temp = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    temp = spm_oam_16read(base_addr, RX_APS_PORT_OFFSET + modoule_index / 2);
    bitset = modoule_index & 0X1;

    if (0x1 == bitset)
    {
        aps_port = temp & 0xff;
    }
    else
    {
        aps_port = (temp >> 8) & 0xff;
    }

    printf("\tAps related port:%d\t\n", aps_port);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_csf_port(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_USHORT csf_port = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_USHORT temp  = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    temp = spm_oam_16read(base_addr, CV_TXENABLE_OFFSET + modoule_index);
    csf_port = (temp >> 8) & 0x3f;
    printf("\tCSF related port:%d\t\n", csf_port);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_ais_port(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_USHORT fdi_port = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_USHORT temp  = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    temp = spm_oam_16read(base_addr, FDI_TX_CFG_OFFSET + modoule_index);
    fdi_port = (temp >> 4) & 0x3f;
    printf("\tFdi related port:%d\t\n", fdi_port);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_lb_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT utemp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    if (0x0 == g_stoam_local[usLoop].vp_attrib)
    {
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    utemp = spm_oam_16read(base_addr, LBM_TX + ucOdindex);
    tx_temp_lbm += utemp;
    utemp = spm_oam_16read(base_addr, LBR_TX + ucOdindex);
    tx_temp_lbr += utemp;
    utemp = spm_oam_16read(base_addr, LBM_RX + ucOdindex);
    rx_temp_lbm += utemp;
    utemp = spm_oam_16read(base_addr, LBR_RX + ucOdindex);
    rx_temp_lbr += utemp;
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\tlbm_tx:\t%d\n", tx_temp_lbm);
    printf("\tlbm_rx:\t%d\n", rx_temp_lbm);
    printf("\tlbr_tx:\t%d\n", tx_temp_lbr);
    printf("\tlbr_rx:\t%d\n", rx_temp_lbr);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_dm_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_USHORT utemp = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    utemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + 0x100 + ucOdindex);
    tx_temp_dmm += utemp;
    utemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + 0x120 + ucOdindex);
    tx_temp_dmr += utemp;
    utemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + 0x110 + ucOdindex);
    rx_temp_dmm += utemp;
    utemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + 0x130 + ucOdindex);
    rx_temp_dmr += utemp;
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\tdmm_tx:\t%d\n", tx_temp_dmm);
    printf("\tdmm_rx:\t%d\n", rx_temp_dmm);
    printf("\tdmr_tx:\t%d\n", tx_temp_dmr);
    printf("\tdmr_rx:\t%d\n", rx_temp_dmr);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_lck_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT utemp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    utemp = spm_oam_16read(base_addr, LCK_TX + ucOdindex);
    tx_lck += utemp;
    utemp = spm_oam_16read(base_addr, LCK_RX  + ucOdindex);
    rx_lck += utemp;
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\tlck_tx:\t%d\n", tx_lck);
    printf("\tlck_rx:\t%d\n", rx_lck);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_test_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_USHORT utemp = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    utemp = spm_oam_16read(base_addr, TST_TX  + ucOdindex);
    tx_tst += utemp;
    utemp = spm_oam_16read(base_addr, TST_RX  + ucOdindex);
    rx_tst += utemp;
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\ttst_tx:\t%d\n", tx_tst);
    printf("\ttst_rx:\t%d\n", rx_tst);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_lb_error(NBB_ULONG oamid)
{
    NBB_USHORT tlv_err = 0;
    NBB_USHORT sequenceid_err = 0;
    NBB_USHORT all1_err = 0;
    NBB_USHORT all0_err = 0;
    NBB_USHORT randon_err = 0;
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    tlv_err = spm_oam_16read(base_addr, LBR_DATA_TLV_ERROR + ucOdindex);
    sequenceid_err = spm_oam_16read(base_addr, LBR_ID_ERROR + ucOdindex);
    all1_err = spm_oam_16read(base_addr, LBR_ONE_ERROR + ucOdindex);
    all0_err = spm_oam_16read(base_addr, LBR_ZERO_ERROR + ucOdindex);
    randon_err = spm_oam_16read(base_addr, LBR_RANDOM_ERROR + ucOdindex);
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\ttlv_err:\t%d\n", tlv_err);
    printf("\tsequenceid_err:\t%d\n", sequenceid_err);
    printf("\tall_1_err:\t%d\n", all1_err);
    printf("\tall_0_err:\t%d\n", all0_err);
    printf("\trandon_err:\t%d\n", randon_err);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_test_error(NBB_ULONG oamid)
{
    NBB_USHORT all1_err = 0;
    NBB_USHORT all0_err = 0;
    NBB_USHORT randon_err = 0;
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    if (SUCCESS != iRet)
    {
        return;
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    all1_err = spm_oam_16read(base_addr, TST_ONE_ERROR + ucOdindex);
    all0_err = spm_oam_16read(base_addr, TST_ZERO_ERROR + ucOdindex);
    randon_err = spm_oam_16read(base_addr, TST_RANDOM_ERROR + ucOdindex);
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\tall_1_err:\t%d\n", all1_err);
    printf("\tall_0_err:\t%d\n", all0_err);
    printf("\trandon_err:\t%d\n", randon_err);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_lm_cnt(NBB_ULONG oamid)
{
    NBB_INT iRet = ERROR;
    NBB_BYTE odLoop = 0;
    NBB_BYTE lmLoop = 0;
    NBB_USHORT dTemp = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT usLoop = 0;
    SPM_OAM_CB *pOamCfg = NULL;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    iRet = spm_oam_get_local_od_oamindex(oamid, &odLoop);

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }

    lmLoop = g_stoam_odlocal[odLoop].ucOdLoop - 1;
    dTemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + LMM_TX_ADDR + lmLoop);
    dataTemp_lmtx += dTemp;
    dTemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + LMM_RX_ADDR + lmLoop);
    dataTemp_lmrx += dTemp;
    dTemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + LMR_TX_ADDR + lmLoop);
    dataTemp_lmrtx += dTemp;
    dTemp = spm_oam_16read(base_addr, DEMAND_MODULE_OFFSET
                           + LMR_RX_ADDR + lmLoop);
    dataTemp_lmrrx += dTemp;
    printf("---------OAM ID=%d--------------\n", oamid);
    printf("\tlmm_tx:\t%d\n", dataTemp_lmtx);
    printf("\tlmm_rx:\t%d\n", dataTemp_lmrx);
    printf("\tlmr_tx:\t%d\n", dataTemp_lmrtx);
    printf("\tlmr_rx:\t%d\n", dataTemp_lmrrx);
EXIT_LABEL:
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_lck_alarm(NBB_ULONG oamid)
{
    NBB_USHORT temp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odLoop = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucOdLoop = 0;
    NBB_BYTE lck_alar = 0;
    SPM_OAM_CB *pOamCfg = NULL;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    iRet = spm_odindex_by_activeindex(usLoop, &odLoop);

    if (SUCCESS != iRet)
    {
        printf("find od index failed\n");
        return;
    }

    ucOdLoop = g_stoam_odlocal[odLoop].ucOdLoop - 1;
    temp = spm_oam_16read(base_addr, 0x0001b);

    lck_alar = (temp >> ucOdLoop) & 0x1;

    if (0x1 == lck_alar)
    {
        printf("\tlsp %d:LCK\t\n", ucOdLoop); //条目号从0开始
    }


    return;
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_lb_alarm(NBB_ULONG oamid)
{
    NBB_USHORT temp = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_BYTE odLoop = 0;
    NBB_BYTE ucOdLoop = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE lb_alar = 0;
    SPM_OAM_CB *pOamCfg = NULL;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    iRet = spm_odindex_by_activeindex(usLoop, &odLoop);

    if (SUCCESS != iRet)
    {
        printf("find od index failed\n");
        return;
    }

    ucOdLoop = g_stoam_odlocal[odLoop].ucOdLoop - 1;
    temp = spm_oam_16read(base_addr, 0x0001a);

    lb_alar = (temp >> ucOdLoop) & 0x1;

    if (0x1 == lb_alar)
    {
        printf("\tlsp %d:LB\t\n", ucOdLoop); //条目号从0开始
    }


    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_alarm_pm(NBB_ULONG oamid)
{
    SPM_OAM_CB *p_Oamcfg = NULL;
    NBB_BYTE Vcalarm[9] = {0};
    NBB_BYTE Vpalarm[8] = {0};
    NBB_BYTE Vsalarm[7] = {0};
    NBB_USHORT iLoop = 0;
    NBB_USHORT rxpack = 0;
    NBB_USHORT txpack = 0;
    NBB_BYTE i = 0;
    uint64 packdely = 0;

    p_Oamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == p_Oamcfg)
    {
        printf("OAMID is error\n");
        return;
    }

    iLoop = p_Oamcfg->oamindex;
    OAM_VC_KEY OamVcKey;
    OAM_VS_KEY OamVsKey;
    OAM_RSVP_LSP_KEY OamVpKey;

    switch (p_Oamcfg->oamtype)
    {
        case 1:
            OamVsKey.PhyPortId = g_stoam_local[iLoop].vs_Portid;
            OamVsKey.SlotId = g_stoam_local[iLoop].vs_Slotid;
            OamVsKey.SubBoardId = 0;
            printf("the VS KEY : PhyPortId %d SlotId %d \n", OamVsKey.PhyPortId, OamVsKey.SlotId);
            oam_vs_loc_alarm(OamVsKey, &Vsalarm[0]);
            oam_vs_lck_alarm(OamVsKey, &Vsalarm[1]);
            oam_vs_mmg_alarm(OamVsKey, &Vsalarm[2]);
            oam_vs_unm_alarm(OamVsKey, &Vsalarm[3]);
            oam_vs_unp_alarm(OamVsKey, &Vsalarm[4]);
            oam_vs_rdi_alarm(OamVsKey, &Vsalarm[5]);
            oam_vs_lblos_alarm(OamVsKey, &Vsalarm[6]);
            oam_vs_rx_cv_packs(OamVsKey, &rxpack);
            oam_vs_tx_cv_packs(OamVsKey, &txpack);
            oam_vs_packdelay_sec(OamVsKey, &packdely);

            printf("VS ALARM----------------------------\n");

            printf("loc lck mmg unm unp rdi lb\n");

            for (i = 0; i < 7; i++)
            {
                printf(" %d ", Vsalarm[i]);
            }

            printf("\n");

            printf("rx cvpack : %d\n", rxpack);
            printf("tx cvpack : %d\n", txpack);
            printf("pack dely : %lld\n", packdely);
            break;

        case 2:
            if (NULL == p_Oamcfg->pRsvplspCfg)
            {
                return;
            }

            OamVpKey.Egress = p_Oamcfg->pRsvplspCfg->rx_egress;
            OamVpKey.Ingress = p_Oamcfg->pRsvplspCfg->rx_ingress;
            OamVpKey.LspId = p_Oamcfg->pRsvplspCfg->rx_lsp_id;
            OamVpKey.TunnelId = p_Oamcfg->pRsvplspCfg->rx_tunnel_id;
            printf("MY VPKEY : Ingress %d  Egress %d  TunnelId %d LspId %d\n", OamVpKey.Ingress,
                   OamVpKey.Egress, OamVpKey.TunnelId, OamVpKey.LspId);
            oam_rsvp_loc_alarm(OamVpKey, &Vpalarm[0]);
            oam_rsvp_lck_alarm(OamVpKey, &Vpalarm[1]);
            oam_rsvp_mmg_alarm(OamVpKey, &Vpalarm[2]);
            oam_rsvp_unm_alarm(OamVpKey, &Vpalarm[3]);
            oam_rsvp_unp_alarm(OamVpKey, &Vpalarm[4]);
            oam_rsvp_rdi_alarm(OamVpKey, &Vpalarm[5]);
            oam_rsvp_ais_alarm(OamVpKey, &Vpalarm[6]);
            oam_rsvp_lblos_alarm(OamVpKey, &Vpalarm[7]);
            oam_rsvp_rx_cv_packs(OamVpKey, &rxpack);
            oam_rsvp_tx_cv_packs(OamVpKey, &txpack);
            printf("VP ALARM----------------------------\n");

            printf("loc lck mmg unm unp rdi ais lb\n");

            for (i = 0; i < 8; i++)
            {
                printf("%d  ", Vpalarm[i]);
            }

            printf("\n");

            printf("rx cvpack : %d\n", rxpack);
            printf("tx cvpack : %d\n", txpack);
            printf("pack dely : %lld\n", packdely);
            break;

        case 3:
            if (NULL == p_Oamcfg->pPwCfg)
            {
                return;
            }

            OamVcKey.PeerIp = p_Oamcfg->pPwCfg->peer_ip;
            OamVcKey.VcId = p_Oamcfg->pPwCfg->vc_id;
            OamVcKey.VcType = p_Oamcfg->pPwCfg->vc_type;
            oam_vc_loc_alarm(OamVcKey, &Vcalarm[0]);
            oam_vc_lck_alarm(OamVcKey, &Vcalarm[1]);
            oam_vc_mmg_alarm(OamVcKey, &Vcalarm[2]);
            oam_vc_unm_alarm(OamVcKey, &Vcalarm[3]);
            oam_vc_unp_alarm(OamVcKey, &Vcalarm[4]);
            oam_vc_rdi_alarm(OamVcKey, &Vcalarm[5]);
            oam_vc_csf_alarm(OamVcKey, &Vcalarm[6]);
            oam_vc_ais_alarm(OamVcKey, &Vcalarm[7]);
            oam_vc_lblos_alarm(OamVcKey, &Vcalarm[8]);
            oam_vc_rx_cv_packs(OamVcKey, &rxpack);
            oam_vc_tx_cv_packs(OamVcKey, &txpack);

            printf("VC ALARM----------------------------\n");

            printf("loc lck mmg unm unp rdi csf ais lb\n");

            for (i = 0; i < 9; i++)
            {
                printf("  %d ", Vcalarm[i]);
            }

            printf("\n");

            printf("rx cvpack : %d\n", rxpack);
            printf("tx cvpack : %d\n", txpack);
            printf("pack dely : %lld\n", packdely);
            break;

        default:
            printf("error\n");
            break;
            return;
    }

    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_monitor_loc(NBB_ULONG oamid)
{
    g_dbg_alarmoamid = oamid;
    g_stOAMWdog = BmuWdCreate();
    (NBB_VOID)BmuWdStart(g_stOAMWdog, BmuWd5MSTickBase(1), (int(*)(int))spm_monitor_loc_task, 0);
}


/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_INT spm_monitor_loc_task(NBB_INT arg)
{
    NBB_USHORT loc_temp = 0;
    NBB_USHORT modoule_index = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT alarm_temp = 0;
    SPM_OAM_CB *pOamCfg = NULL;
    static int locLast = 0;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &g_dbg_alarmoamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return 0;
    }

    if (BmuWdStart(g_stOAMWdog,
                   BmuWd5MSTickBase(1), spm_monitor_loc_task, 0) == ERROR)
    {
        printf("spm_monitor_loc_task BmuWdStart FAIL  !\n");
    }

    usLoop = pOamCfg->oamindex;
    modoule_index = g_stoam_local[usLoop].module_index;
    spm_get_moudle_base_addr(usLoop, &base_addr);
    alarm_temp = spm_oam_16read(base_addr, ALARM_ADDR + modoule_index);

    if (alarm_temp != locLast)
    {
        printf("\tlast_alarm:%d\n\tnow_alarm:%d\n", locLast, alarm_temp);
        locLast = alarm_temp;
    }

    return 1;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_close_monitor_loc()
{
    BmuWdCancel(g_stOAMWdog);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_mclag_state(NBB_USHORT usLoop)
{
    printf("\tlast_state:%d\n", g_lag_state_last[usLoop]);
    printf("\tnow_state:%d\n", g_lag_state_now[usLoop]);
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_mclag_list()
{
    LIST_NODE *pSearch = NULL;
    pSearch = g_oamid_encsflist;

    while (pSearch != NULL)
    {
        printf("--------------oam_id:%d------------\n", pSearch->oamid);
        printf("\tlag_id:%d\n", pSearch->lag_id);
        printf("\tmodule_index:%d\n", pSearch->module_index);
        printf("\tport_id:%d\n", pSearch->port_id);
        pSearch = pSearch->next;
    }

    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_sd_list()
{
    SD_LIST_NODE *pSearch = NULL;
    pSearch = g_oam_sdlist;
    NBB_USHORT iloop = 0;

    while (pSearch != NULL)
    {
        printf("--------------%d:------------\n", iloop);
        iloop++;
        printf("\toamLoop:%d\n", pSearch->oamLoop);
        pSearch = pSearch->next;
    }

    return;
}

/*****************************************************************************
 函 数 名  : oam_show_txlsp_cb
 功能描述  : oam调试函数
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
NBB_VOID oam_show_wrapping_list()
{
    SD_LIST_NODE *pSearch = NULL;
    pSearch = g_oam_wrappinglist;
    NBB_USHORT iloop = 0;

    while (pSearch != NULL)
    {
        printf("--------------%d:------------\n", iloop);
        iloop++;
        printf("\toamLoop:%d\n", pSearch->oamLoop);
        pSearch = pSearch->next;
    }

    return;
}

#ifdef PTN690_CES
NBB_VOID oam_show_ces_alarm(NBB_ULONG ulOamId)
{
    NBB_BYTE alarm = 0;
    NBB_USHORT   usOamIndex = 0;
    spm_oam_get_local_oamindex(ulOamId, &usOamIndex);

    if (usOamIndex > (CES_OAM_MAX_NUM - 1))
    {
        printf("Get OamIndex is error\n");
        return;
    }

    printf("--------------------------------------\n");
    printf("usOamIndex : %d  g_oam_alarm : %d\n", usOamIndex, g_oam_alarm[usOamIndex]);
    printf("--------------------------------------\n");

    if (1 == (g_oam_alarm[usOamIndex] & 0x1))
    {
        printf("LOC \n");
    }

    if (1 == ((g_oam_alarm[usOamIndex] >> 1) & 0x1))
    {
        printf("RDI \n");
    }

    if (1 == ((g_oam_alarm[usOamIndex] >> 2) & 0x1))
    {
        printf("CSF \n");
    }

    if (1 == ((g_oam_alarm[usOamIndex] >> 3) & 0x1))
    {
        printf("APS \n");
    }

    if (1 == ((g_oam_alarm[usOamIndex] >> 4) & 0x1))
    {
        printf("MEG \n");
    }

    if (1 == ((g_oam_alarm[usOamIndex] >> 5) & 0x1))
    {
        printf("MEP \n");
    }

    if (1 == ((g_oam_alarm[usOamIndex] >> 6) & 0x1))
    {
        printf("UNP \n");
    }

    return;

}

/*****************************************************************************
 函 数 名  : oam_show_pack
 功能描述  : show pack
 输入参数  : NBB_ULONG OamId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_show_pack(NBB_ULONG OamId)
{
    SPM_OAM_CB *p_Oamcfg = NULL;
    NBB_USHORT iLoop;
    OAM_VC_KEY OamVcKey;
    NBB_ULONG rx_packs = 0;
    NBB_ULONG tx_packs = 0;

    p_Oamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &OamId);

    if (NULL == p_Oamcfg)
    {
        return;
    }

    iLoop = p_Oamcfg->oamindex;

    OamVcKey.PeerIp = p_Oamcfg->pPwCfg->peer_ip;
    OamVcKey.VcId = p_Oamcfg->pPwCfg->vc_id;
    OamVcKey.VcType = p_Oamcfg->pPwCfg->vc_type;
    oam_ces_vc_rx_cv_packs(OamVcKey, &rx_packs);
    oam_ces_vc_tx_cv_packs(OamVcKey, &tx_packs);
    printf("rx_packs %d tx_packs %d \n", rx_packs, tx_packs);

}

/*****************************************************************************
 函 数 名  : test_time
 功能描述  : testtime
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID test_time()
{
    struct timeval tv;
    struct timezone tz;
    NBB_ULONG count = 0;
    NBB_ULONG usloop = 0;
    gettimeofday(&tv, &tz);
    printf("first tv_sec:%d\n", tv.tv_sec);
    printf("first tv_usec: %d\n", tv.tv_usec);

    for (usloop = 0; usloop < 1000; usloop++)
    {
        ApiGetFastWarning(0, 1, CES_OAM_MAX_NUM - 1, g_oam_alarm);
        count++;
    }

    printf("count = %d\n", count);
    gettimeofday(&tv, &tz);
    printf("last tv_sec:%d\n", tv.tv_sec);
    printf("last tv_usec: %d\n", tv.tv_usec);
    return;
}


#endif
#if 0
NBB_VOID DbgTestAlmPm(NBB_ULONG OamId)
{
    SPM_OAM_CB *p_Oamcfg = NULL;
    NBB_USHORT iLoop = 0;

    p_Oamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &OamId);
    iLoop = p_Oamcfg->oamindex;
    OAM_VC_KEY OamVcKey;
    OAM_VS_KEY OamVsKey;
    OAM_RSVP_LSP_KEY OamVpKey;

    OamVcKey.PeerIp = p_Oamcfg->pPwCfg->peer_ip;
    OamVcKey.VcId = p_Oamcfg->pPwCfg->vc_id;
    OamVcKey.VcType = p_Oamcfg->pPwCfg->vc_type;
    printf("OamVcKey.PeerIp %d\n", OamVcKey.PeerIp);
    printf("OamVcKey.VcId %d\n", OamVcKey.VcId);
    printf("OamVcKey.VcType %d\n", OamVcKey.VcType);


    OamVpKey.Egress = p_Oamcfg->pRsvplspCfg->tx_egress;
    OamVpKey.Ingress = p_Oamcfg->pRsvplspCfg->tx_ingress;
    OamVpKey.LspId = p_Oamcfg->pRsvplspCfg->tx_lsp_id;
    OamVpKey.TunnelId = p_Oamcfg->pRsvplspCfg->tx_tunnel_id;

    switch (p_Oamcfg->oamtype)
    {
        case VP_OAM:
            g_stoam_almpm_line.usLineType = VP_LINE_TYPE;
            g_stoam_almpm_line.usLineLayer = VP_LINE_LAYER;
            g_stoam_almpm_line.ucKeyLen = VP_KEY_LEN;
            OS_MEMCPY(g_stoam_almpm_line.ucKeyValue, &OamVpKey, VP_KEY_LEN);
            almpm_cfg(g_stoam_almpm_line, 1, 10, 1);
            break;

        case VC_OAM:
            g_stoam_almpm_line.usLineType = VC_LINE_TYPE;
            g_stoam_almpm_line.usLineLayer = VC_LINE_LAYER;
            g_stoam_almpm_line.ucKeyLen = VC_KEY_LEN;
            OS_MEMCPY(g_stoam_almpm_line.ucKeyValue, &OamVcKey, VC_KEY_LEN);
            almpm_cfg(g_stoam_almpm_line, 1, 10, 1);
            break;

        default:
            printf("no this type\n");
            break;
    }

}
#endif


/*****************************************************************************
 函 数 名  : test_cpu
 功能描述  : testcpu
 输入参数  : NBB_BYTE flag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID test_cpu(NBB_BYTE flag)
{
    g_dbg_ces_print_flag = flag;
    printf("g_dbg_ces_print_flag = %d\n", g_dbg_ces_print_flag);
    return;
}

/*****************************************************************************
 函 数 名  : state_printf
 功能描述  : stataflag
 输入参数  : NBB_BYTE flag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID state_printf(NBB_BYTE flag)
{
    g_state_printf_flag = flag;
    printf("g_state_printf_flag = %d\n", g_state_printf_flag);
    return;
}

/*****************************************************************************
 函 数 名  : oam_test_get_csf_mclag
 功能描述  : testcsf
 输入参数  : NBB_BYTE now   
             NBB_BYTE last  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_test_get_csf_mclag(NBB_BYTE now, NBB_BYTE last)
{
    NBB_USHORT usLoop = 0;

    for (usLoop = 0 ; usLoop < OAM_MAX_NUM_ACTIVE_SPU; usLoop++)
    {
        g_lag_state_now[usLoop] = now;
        g_lag_state_last[usLoop] = last;
    }

    return;
}

/*****************************************************************************
 函 数 名  : oam_show_oam_num
 功能描述  : oamshownum
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_show_oam_num()
{
    printf("%-*s%d\n", STRING_LEN, "oam_num_total =", g_usoam_num);
    printf("%-*s%d\n", STRING_LEN, "od_oam_num_total = ", g_ucod_oamnum);
    printf("%-*s%d\n", STRING_LEN, "od_num_module0 = ", g_odoam_cnt1);
    printf("%-*s%d\n", STRING_LEN, "od_num_module1 = ", g_odoam_cnt2);
}


/*****************************************************************************
 函 数 名  : oam_print_for_debug
 功能描述  : alarmdebug
 输入参数  : NBB_BYTE flag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID oam_print_for_debug(NBB_BYTE flag)
{
    g_alarm_debug = flag;
    printf("g_alarm_debug = %d\n", g_alarm_debug);
    return;
}

/*****************************************************************************
 函 数 名  : oam_open_sd_print
 功能描述  : sd flag print
 输入参数  : NBB_BYTE flag  
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
NBB_VOID oam_open_sd_print(NBB_BYTE flag)
{
    g_sd_debug = flag;
    printf("g_sd_debug = %d\n", g_sd_debug);
    return;
}

/*****************************************************************************
 函 数 名  : test_ces_alarm
 功能描述  : test csm alarm
 输入参数  : NBB_BYTE flag  
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
NBB_VOID test_ces_alarm(NBB_BYTE flag)
{
    g_ces_alarm_debug = flag;
    printf("g_ces_alarm_debug = %d\n", g_ces_alarm_debug);
    return;
}

/*****************************************************************************
 函 数 名  : test_ces_loc
 功能描述  : test csm loc
 输入参数  : NBB_BYTE flag  
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
NBB_VOID test_ces_loc(NBB_BYTE flag)
{
    g_ces_loc_debug = flag;
    printf("g_ces_loc_debug = %d\n", g_ces_loc_debug);
    return;
}

/*****************************************************************************
 函 数 名  : pve_almpm_print
 功能描述  : pve almpm print
 输入参数  : NBB_BYTE flag  
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
NBB_VOID pve_almpm_print(NBB_BYTE flag)
{
    g_pve_debug = flag;
    printf("g_pve_debug = %d\n", g_pve_debug);
    return;
}

/*****************************************************************************
 函 数 名  : test_ces_pm
 功能描述  : test ces pm
 输入参数  : NBB_BYTE flag  
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
NBB_VOID test_ces_pm(NBB_BYTE flag)
{
    g_ces_pm_debug = flag;
    printf("g_ces_pm_debug = %d\n", g_ces_pm_debug);
    return;
}

/*****************************************************************************
 函 数 名  : oam_print_lck
 功能描述  : oam print lck
 输入参数  : NBB_BYTE flag  
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
NBB_VOID oam_print_lck(NBB_BYTE flag)
{
    g_oam_debug = flag;
    printf("g_oam_debug = %d\n", g_oam_debug);
    return;
}

/*****************************************************************************
 函 数 名  : test_ces_timer
 功能描述  : test ces timer
 输入参数  : NBB_BYTE flag  
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
NBB_VOID test_ces_timer(NBB_BYTE flag)
{
    g_test_ces_timer = flag;
    printf("g_test_ces_timer = %d\n", g_test_ces_timer);
    return;
}

/*****************************************************************************
 函 数 名  : oam_print_vc_get_vp_lck
 功能描述  : vp lck alarm
 输入参数  : NBB_BYTE flag  
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
NBB_VOID oam_print_vc_get_vp_lck(NBB_BYTE flag)
{
    g_vp_lck_alarm_flag = flag;
    printf("g_vp_lck_alarm_flag = %d\n", g_vp_lck_alarm_flag);
    return;
}


/*****************************************************************************
 函 数 名  : oam_show_od_lm
 功能描述  : oam show od lm
 输入参数  : NBB_ULONG oamid  
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
NBB_VOID oam_show_od_lm(NBB_ULONG oamid)
{
    SPM_OAM_CB *p_Oamcfg = NULL;
    NBB_USHORT iLoop;
    NBB_USHORT rxpack;
    NBB_USHORT txpack;
    uint64 packdely;
    float PktLosNear;
    float PktLosFar;

    p_Oamcfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == p_Oamcfg)
    {
        printf("OAMID is error\n");
        return;
    }

    iLoop = p_Oamcfg->oamindex;
    OAM_VC_KEY OamVcKey;
    OAM_VS_KEY OamVsKey;
    OAM_RSVP_LSP_KEY OamVpKey;

    switch (p_Oamcfg->oamtype)
    {
        case 1:
            OamVsKey.PhyPortId = g_stoam_local[iLoop].vs_Portid;
            OamVsKey.SlotId = g_stoam_local[iLoop].vs_Slotid;
            OamVsKey.SubBoardId = 0;
            oam_vs_pktlos_near(OamVsKey,  &PktLosNear);
            oam_vs_pktlos_far(OamVsKey,  &PktLosFar);
            printf("VS PktLosNear %f PktLosFar %f\n", PktLosNear, PktLosFar);
            break;

        case 2:
            if (NULL == p_Oamcfg->pRsvplspCfg)
            {
                return;
            }

            OamVpKey.Egress = p_Oamcfg->pRsvplspCfg->rx_egress;
            OamVpKey.Ingress = p_Oamcfg->pRsvplspCfg->rx_ingress;
            OamVpKey.LspId = p_Oamcfg->pRsvplspCfg->rx_lsp_id;
            OamVpKey.TunnelId = p_Oamcfg->pRsvplspCfg->rx_tunnel_id;
            oam_rsvp_pktlos_near(OamVpKey,  &PktLosNear);
            oam_rsvp_pktlos_far(OamVpKey,  &PktLosFar);
            printf("VP PktLosNear %f PktLosFar %f\n", PktLosNear, PktLosFar);
            break;

        case 3:
            if (NULL == p_Oamcfg->pPwCfg)
            {
                return;
            }

            OamVcKey.PeerIp = p_Oamcfg->pPwCfg->peer_ip;
            OamVcKey.VcId = p_Oamcfg->pPwCfg->vc_id;
            OamVcKey.VcType = p_Oamcfg->pPwCfg->vc_type;
            oam_vc_pktlos_near(OamVcKey,  &PktLosNear);
            oam_vc_pktlos_far(OamVcKey,  &PktLosFar);
            printf("VC PktLosNear %f PktLosFar %f\n", PktLosNear, PktLosFar);
            break;

        default:
            break;
    }

    return;
}


#ifdef SPU

/*****************************************************************************
 函 数 名  : oam_show_active_lm_cv_cnt
 功能描述  : oam show lm cv
 输入参数  : NBB_ULONG oamid  
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
NBB_VOID oam_show_active_lm_cv_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    iRet = spm_packet_loss_ratio_by_active_lm(usLoop);

    if (SUCCESS != iRet)
    {
        printf("****ERROR*****:spm_packet_loss_ratio_by_active_lm() failed\n");
    }

#if 0
    printf("\tbefore:\n");
    printf("\t\ntxfcf:%d", g_stoam_activelmdata[usLoop].lmtxpcf_old);
    printf("\t\nrxfcf:%d", g_stoam_activelmdata[usLoop].lmrxpcf_old);
    printf("\t\ntxfcb:%d", g_stoam_activelmdata[usLoop].lmtxpcb_old);
    printf("\t\nrxfcb:%d", g_stoam_activelmdata[usLoop].lmrxpcl_old);
    printf("\tlater:\n");
    printf("\t\ntxfcf:%d", g_stoam_activelmdata[usLoop].lmtxpcf_new);
    printf("\t\nrxfcf:%d", g_stoam_activelmdata[usLoop].lmrxpcf_new);
    printf("\t\ntxfcb:%d", g_stoam_activelmdata[usLoop].lmtxpcb_new);
    printf("\t\nrxfcb:%d", g_stoam_activelmdata[usLoop].lmrxpcl_new);
#endif
    printf("lmrxpcl %d\n", g_stoam_activelmdata[usLoop].lmrxpcl);
    printf("lmtxpcf %d\n", g_stoam_activelmdata[usLoop].lmtxpcf);
    printf("lmrxpcf %d\n", g_stoam_activelmdata[usLoop].lmrxpcf);
    printf("lmtxpcb %d\n", g_stoam_activelmdata[usLoop].lmtxpcb);
    return;
}

/*****************************************************************************
 函 数 名  : oam_show_od_lm_cv_cnt
 功能描述  : oam show od lm cn cnt
 输入参数  : NBB_ULONG oamid  
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
NBB_VOID oam_show_od_lm_cv_cnt(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    NBB_USHORT usLoop = 0;
    NBB_INT iRet = ERROR;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    usLoop = pOamCfg->oamindex;
    spm_od_lm_task(usLoop);

    printf("\tbefore:\n");
    printf("\t\ntxfcf:%d", g_stoam_odlmdata[usLoop].lmtxpcf);
    printf("\t\nrxfcf:%d", g_stoam_odlmdata[usLoop].lmrxpcf);
    printf("\t\ntxfcb:%d", g_stoam_odlmdata[usLoop].lmtxpcb);
    printf("\t\nrxfcb:%d", g_stoam_odlmdata[usLoop].lmrxpcl);
    return;
}

/*****************************************************************************
 函 数 名  : oam_print_fpga
 功能描述  : oam print fpga
 输入参数  : IN NBB_ULONG ulOffset  
             IN NBB_ULONG ulNum     
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
NBB_VOID oam_print_fpga(IN NBB_ULONG ulOffset, IN NBB_ULONG ulNum)
{
    hwprintfpgareg(ulOffset, ulNum);
    return;
}

/*****************************************************************************
 函 数 名  : oam_write_fpga
 功能描述  : oam write fpga
 输入参数  : NBB_INT iOffset   
             NBB_INT iRegData  
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
NBB_VOID oam_write_fpga(NBB_INT iOffset, NBB_INT iRegData)
{
    hwwritefpga(iOffset, iRegData);
    return;
}

#else

/*****************************************************************************
 函 数 名  : oam_print_fpga
 功能描述  : oam print fpga
 输入参数  : IN NBB_ULONG ulOffset  
             IN NBB_ULONG ulNum     
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
NBB_VOID oam_print_fpga(IN NBB_ULONG ulOffset, IN NBB_ULONG ulNum)
{
    return;
}

/*****************************************************************************
 函 数 名  : oam_write_fpga
 功能描述  : oam write fpga
 输入参数  : NBB_INT iOffset   
             NBB_INT iRegData  
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
NBB_VOID oam_write_fpga(NBB_INT iOffset, NBB_INT iRegData)
{
    return;
}

#endif

/*****************************************************************************
 函 数 名  : oam_show_global_cb
 功能描述  : global print
 输入参数  : 无
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
NBB_VOID oam_show_global_cb()
{
    printf("sdthreshold=%d\n", g_sd_threshold);
    printf("sdwindow=%d\n", g_sd_windows);
    printf("lossratio=%d\n", g_sd_lossratio);
}


/*****************************************************************************
 函 数 名  : oam_clear_sd
 功能描述  : clear sd
 输入参数  : 无
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
NBB_VOID oam_clear_sd()
{
    NBB_USHORT iLoop = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG base_addr = 0;
    NBB_USHORT modoule_index = 0;
    NBB_USHORT sdNumIndex = 0;
    NBB_USHORT sdBitIndex = 0;
    NBB_USHORT sdDataTemp = 0;
    NBB_USHORT mask = 0;
#if 0

    for (iLoop = 0; iLoop < OAM_MAX_NUM_ACTIVE_SPU; iLoop++)
    {
        if (USELESS == g_sd_oamloop[iLoop])
        {
            break;
        }

        usLoop = g_sd_oamloop[iLoop];
        spm_get_moudle_base_addr(usLoop, &base_addr);
        modoule_index = g_stoam_local[usLoop].module_index;
        sdNumIndex = modoule_index / 16;
        sdBitIndex = modoule_index % 16;
        sdDataTemp = spm_oam_16read(base_addr, SD_OVERRIDE_OAM + sdNumIndex);
        mask = ~(0x1 << sdBitIndex);
        sdDataTemp = sdDataTemp & mask;
        spm_oam_16write(base_addr, SD_OVERRIDE_OAM + sdNumIndex, sdDataTemp);

        if (1 == g_sd_debug)
        {
            printf("sdAddr=%#x,sdData=%d\n", base_addr + SD_OVERRIDE_OAM + sdNumIndex, sdDataTemp);
        }
    }

#endif

    for (usLoop = 0 ; usLoop < 8 ; usLoop++)
    {
        sdDataTemp = spm_oam_16read(0x240007, usLoop);
        sdDataTemp = sdDataTemp & 0x3333;
        spm_oam_16write(0x240007, usLoop, sdDataTemp);
    }

    return;
}


/*****************************************************************************
 函 数 名  : oam_sd_threshold
 功能描述  : sd thread set
 输入参数  : NBB_USHORT flag  
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
NBB_VOID oam_sd_threshold(NBB_USHORT flag)
{
    g_sd_threshold = flag;
}

/*****************************************************************************
 函 数 名  : oam_show_adjust
 功能描述  : oam show adjust
 输入参数  : NBB_ULONG oamid  
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
NBB_VOID oam_show_adjust(NBB_ULONG oamid)
{
    SPM_OAM_CB *pOamCfg = NULL;
    pOamCfg = AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &oamid);

    if (NULL == pOamCfg)
    {
        printf("****ERROR*****:can not find this oamid\n");
        return;
    }

    if (1 == pOamCfg->oamtype)
    {
        printf("-------------Vs------------\n");
        printf("nearPkLos:%f\n", g_vsnear_pklos);
        printf("farPkLos:%f\n", g_vsfar_pklos);
    }

    if (2 == pOamCfg->oamtype)
    {
        printf("-------------Vp------------\n");
        printf("nearPkLos:%f\n", g_vpnear_pklos);
        printf("farPkLos:%f\n", g_vpfar_pklos);
    }

    if (3 == pOamCfg->oamtype)
    {
        printf("-------------Vc------------\n");
        printf("nearPkLos:%f\n", g_vcnear_pklos);
        printf("farPkLos:%f\n", g_vcfar_pklos);
    }

    return;
}
