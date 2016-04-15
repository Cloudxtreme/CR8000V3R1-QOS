/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_cmd_switch_proc.c
   版 本 号   : 初稿
   作    者   : zhangsen	
   生成日期   : 2014年4月3日
   最近修改   :
   功能描述   : 倒换命令配置处理
   函数列表   :
   修改历史   :
   1.日    期   : 2014年4月3日
    作    者   : zhangsen
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef CMD_SWITCH_CFG

/*cmd_switch配置块打印开关*/
extern unsigned char cmd_switch_cfg_print_setting;

#if 0

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                             *
 * Function    : spm_cmd_ip_frr_switch_handler                                 *
 *                                                                             *
 * Description : This function handles the command of IP FRR switch.           *
 *                                                                             *
 * Input       : p_cmd_switch - The parameter from IPS message.                *
 *                                                                             *
 * Output      :                                                               *
 *                                                                             *
 * Return      :  0 - OK.                                                      *
 *               -1 - Parameter "p_cmd_switch"        *
 *                       or "ipfrr_switch_data" is NULL.   *                                                   
 *               -2 - The "switch_type" in "p_cmd_switch" is out of range.     *
 *               -3 - Getting from "peer tree" failed.                         *
 *                                                                             *
 * Calls       : spm_l3_get_frrid_by_peer                                      *
 *               spm_lps_set_cmd                                               *
 *                                                                             *
 * Called By   : spm_rcv_dci_set_cmd_swtich                                    *
 *                                                                             *
 * Others      :                                                               *
 *                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NBB_INT spm_cmd_ip_frr_switch_handler(ATG_DCI_CMD_SWITCH *p_cmd_switch)
{
    NBB_INT rv = SUCCESS;
    NBB_ULONG atg_dci_oper;
    ATG_DCI_SWITCH_OVER_IPFRR *ip_frr_data = NULL;
    NBB_BYTE switch_type;
    NBB_USHORT frr_id = 0;
    
    if (NULL == p_cmd_switch)
    {
        //SPM_L3_PRINTF(SPM_L3_DEBUG, "input invalid : pointer is NULL.");
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s s", FUNC_IN_PARAM_IS_NULL, 
            "switch parameter error", "p_cmd_switch is NULL"));
        
        //return SPM_ERR_NULL_PTR;
        return ERROR;
    }

    atg_dci_oper = p_cmd_switch->oper_ipfrr_switch;
    if (ATG_DCI_OPER_ADD == atg_dci_oper)
    {
        ip_frr_data = (ATG_DCI_SWITCH_OVER_IPFRR *)NTL_OFFLEN_GET_POINTER(
            p_cmd_switch, &(p_cmd_switch->ipfrr_switch_data)); 
        if (NULL == ip_frr_data)
        {
            //SPM_L3_PRINTF(SPM_L3_DEBUG, 
                //"input invalid : getting pointer NULL.");
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            
            //return SPM_ERR_NULL_PTR;
            return ERROR;
        }

        switch_type = ip_frr_data->switch_type;
        if (switch_type < 1 || switch_type > 7)
        {
            //SPM_L3_PRINTF(SPM_L3_DEBUG, 
               // "input invalid : switch type(%d) out of range.", switch_type);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s d", CONFIG_NUM_IS_OUT_RANGE, 
                "switch type is out of range", switch_type));

            return ERROR;
        }

        rv = spm_l3_get_frrid(e_mode_nbb, ip_frr_data->vrf_id, 
            ip_frr_data->master_peer_ip, ip_frr_data->slave_peer_ip, &frr_id);
        if (SUCCESS != rv)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "ip frr : no such entry(%d 0x%08x 0x%08x).", 
                ip_frr_data->vrf_id, ip_frr_data->master_peer_ip, 
                ip_frr_data->slave_peer_ip);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s s", 0, "switch find entry failed", "no such entry"));
            return SPM_ERR_GET_FAILED;
        }
        
        spm_lps_set_cmd(switch_type, (int)frr_id);
    }
    else
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "switch oper(%d) invalid.", atg_dci_oper);
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s d", 0, "switch oper invalid", atg_dci_oper));
    }

    return SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                             *
 * Function    : spm_cmd_vpn_frr_switch_handler                                *
 *                                                                             *
 * Description : This function handles the command of VPN FRR switch.          *
 *                                                                             *
 * Input       : p_cmd_switch - The parameter from IPS message.                *
 *                                                                             *
 * Output      :                                                               *
 *                                                                             *
 * Return      :  0 - OK.                                                      *
 *               -1 - Parameter "p_cmd_switch" or  *
 *                      "vpnfrr_switch_data" is NULL.   *                                                   
 *               -2 - The "switch_type" in "p_cmd_switch" is out of range.     *
 *               -3 - Getting from "peer tree" failed.                         *
 *                                                                             *
 * Calls       : spm_l3_get_frrid_by_peer                                      *
 *               spm_lps_set_cmd                                               *
 *                                                                             *
 * Called By   : spm_rcv_dci_set_cmd_swtich                                    *
 *                                                                             *
 * Others      :                                                               *
 *                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NBB_INT spm_cmd_vpn_frr_switch_handler(ATG_DCI_CMD_SWITCH *p_cmd_switch)
{
    NBB_INT rv = SUCCESS;
    NBB_ULONG atg_dci_oper;
    ATG_DCI_SWITCH_OVER_VPNFRR *vpn_frr_data = NULL;
    NBB_BYTE switch_type;
    NBB_USHORT frr_id = 0;
    
    if (NULL == p_cmd_switch)
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "input invalid : pointer is NULL.");
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s s", FUNC_IN_PARAM_IS_NULL, 
            "switch parameter error", "p_cmd_switch is NULL"));
        return SPM_ERR_NULL_PTR;
    }

    atg_dci_oper = p_cmd_switch->oper_vpnfrr_switch;
    if (ATG_DCI_OPER_ADD == atg_dci_oper)
    {
        vpn_frr_data = (ATG_DCI_SWITCH_OVER_VPNFRR *)NTL_OFFLEN_GET_POINTER(
            p_cmd_switch, &(p_cmd_switch->vpnfrr_switch_data)); 
        if (NULL == vpn_frr_data)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "input invalid : getting pointer NULL.");
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            return SPM_ERR_NULL_PTR;
        }

        switch_type = vpn_frr_data->switch_type;
        if (switch_type < 1 || switch_type > 7)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "input invalid : switch type(%d) out of range.", switch_type);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s d", CONFIG_NUM_IS_OUT_RANGE, 
                "switch type is out of range", switch_type));
            return SPM_ERR_OUT_OF_RANGE;
        }

        rv = spm_l3_get_frrid(e_mode_nbb, vpn_frr_data->vrf_id, 
            vpn_frr_data->master_peer_ip, vpn_frr_data->slave_peer_ip, &frr_id);
        if (SUCCESS != rv)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "vpn frr : no such entry(%d 0x%08x 0x%08x).", 
                vpn_frr_data->vrf_id, vpn_frr_data->master_peer_ip, 
                vpn_frr_data->slave_peer_ip);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s s", 0, "switch find entry failed", "no such entry"));
            return SPM_ERR_GET_FAILED;
        }
        
        spm_lps_set_cmd(switch_type, (int)frr_id);
    }
    else
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "switch oper(%d) invalid.", atg_dci_oper);
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s d", 0, "switch oper invalid", atg_dci_oper));
    }

    return SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                             *
 * Function    : spm_cmd_lsp_1_to_1_switch_handler                             *
 *                                                                             *
 * Description : This function handles the command of LSP 1:1 switch.          *
 *                                                                             *
 * Input       : p_cmd_switch - The parameter from IPS message.                *
 *                                                                             *
 * Output      :                                                               *
 *                                                                             *
 * Return      :  0 - OK.                                                      *
 *               -1 - Parameter "p_cmd_switch" or "lsp1to1_switch_data" is     *
 *                    NULL.                                                    *
 *               -2 - The "switch_action" in "p_cmd_switch" is out of range.   *
 *               -3 - Getting from "lspprothash" table failed.                 *
 *                                                                             *
 * Calls       : spm_l3_lspprot_get_entry                                      *
 *               spm_lps_set_cmd                                               *
 *                                                                             *
 * Called By   : spm_rcv_dci_set_cmd_swtich                                    *
 *                                                                             *
 * Others      :                                                               *
 *                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NBB_INT spm_cmd_lsp_1_to_1_switch_handler(ATG_DCI_CMD_SWITCH *p_cmd_switch)
{
    NBB_ULONG atg_dci_oper;
    ATG_DCI_SWITCH_OVER_LSP *lsp_data = NULL;
    NBB_BYTE switch_type;
    LSPPROT  *lspprot_entry = NULL;
    
    if (NULL == p_cmd_switch)
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "input invalid : pointer is NULL.");
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s s", FUNC_IN_PARAM_IS_NULL, 
            "switch parameter error", "p_cmd_switch is NULL"));
        return SPM_ERR_NULL_PTR;
    }

    SPM_L3_PRINTF(SPM_L3_INFO, "receiving command of LSP switch.");

    atg_dci_oper = p_cmd_switch->oper_lsp_switch;
    if (ATG_DCI_OPER_ADD == atg_dci_oper)
    {
        lsp_data = (ATG_DCI_SWITCH_OVER_LSP *)NTL_OFFLEN_GET_POINTER(
            p_cmd_switch, &(p_cmd_switch->lsp_switch_data)); 
        if (NULL == lsp_data)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "input invalid : getting pointer NULL.");
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            return SPM_ERR_NULL_PTR;
        }

        switch_type = lsp_data->switch_type;
        if (switch_type < 1 || switch_type > 7)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "input invalid : switch type(%d) out of range.", switch_type);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s d", CONFIG_NUM_IS_OUT_RANGE, 
                "switch type is out of range", switch_type));
            return SPM_ERR_OUT_OF_RANGE;
        }

        SPM_L3_PRINTF(SPM_L3_INFO, "LSP(0x%08x 0x%08x %d) switch type(%d).", 
            lsp_data->ingress, lsp_data->egress, lsp_data->tunnel_id, 
            switch_type);

        lspprot_entry = spm_l3_lspprot_get_entry(e_mode_nbb, lsp_data->ingress, 
            lsp_data->egress, lsp_data->tunnel_id);
        if (NULL == lspprot_entry)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, "no such entry(0x%08x 0x%08x %d).", 
                lsp_data->ingress, lsp_data->egress, lsp_data->tunnel_id);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s s", 0, "switch find entry failed", "no such entry"));
            return SPM_ERR_GET_FAILED;
        }
        
        spm_lps_set_cmd(switch_type, lspprot_entry->frrid);
    }
    else
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "switch oper(%d) invalid.", atg_dci_oper);
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s d", 0, "switch oper invalid", atg_dci_oper));
    }

    return SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                             *
 * Function    : spm_cmd_sncp_switch_handler                                   *
 *                                                                             *
 * Description : This function handles the command of SNCP switch.             *
 *                                                                             *
 * Input       : p_cmd_switch - The parameter from IPS message.                *
 *                                                                             *
 * Output      :                                                               *
 *                                                                             *
 * Return      :  0 - OK.                                                      *
 *               -1 - Parameter "p_cmd_switch" or "sncp_switch_data" is NULL.  *
 *               -2 - The "switch_action" in "p_cmd_switch" is out of range.   *
 *               -3 - Getting sncp_info failed.                                *
 *                                                                             *
 * Calls       : spm_l3_sncp_get_entry                                         *
 *               spm_lps_set_cmd                                               *
 *                                                                             *
 * Called By   : spm_rcv_dci_set_cmd_swtich                                    *
 *                                                                             *
 * Others      :                                                               *
 *                                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
NBB_INT spm_cmd_sncp_switch_handler(ATG_DCI_CMD_SWITCH *p_cmd_switch)
{
    NBB_INT rv = SUCCESS;
    NBB_ULONG atg_dci_oper;
    ATG_DCI_SWITCH_OVER_SNCP *sncp_data = NULL;
    NBB_BYTE  switch_type;
    NBB_ULONG sncp_id;
    SPM_SNCP_CB *sncp_entry = NULL;
    
    if (NULL == p_cmd_switch)
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "input invalid : pointer is NULL.");
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s s", FUNC_IN_PARAM_IS_NULL, 
            "switch parameter error", "p_cmd_switch is NULL"));
        return SPM_ERR_NULL_PTR;
    }

    atg_dci_oper = p_cmd_switch->oper_sncp_switch;
    if (ATG_DCI_OPER_ADD == atg_dci_oper)
    {
        sncp_data = (ATG_DCI_SWITCH_OVER_SNCP *)NTL_OFFLEN_GET_POINTER(
            p_cmd_switch, &(p_cmd_switch->sncp_switch_data)); 
        if (NULL == sncp_data)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "input invalid : getting pointer NULL.");
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            return SPM_ERR_NULL_PTR;
        }
        sncp_id = sncp_data->sncp_id;
        switch_type = sncp_data->switch_action;
        if (switch_type < 1 || switch_type > 7)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, 
                "input invalid : switch type(%d) out of range.", switch_type);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s d", CONFIG_NUM_IS_OUT_RANGE, 
                "switch type is out of range", switch_type));
            return SPM_ERR_OUT_OF_RANGE;
        }

        sncp_entry = spm_l3_sncp_get_entry(sncp_id);
        if (NULL == sncp_entry)
        {
            SPM_L3_PRINTF(SPM_L3_DEBUG, "no such entry(%d).", sncp_id);
            NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
                "d s s", 0, "switch find entry failed", "no such entry"));
            return SPM_ERR_GET_FAILED;
        }

        spm_lps_set_cmd(switch_type, sncp_entry->sncp_info_cb.frr_group_id);
    }
    else
    {
        SPM_L3_PRINTF(SPM_L3_DEBUG, "switch oper(%d) invalid.", atg_dci_oper);
        NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            "d s d", 0, "switch oper invalid", atg_dci_oper));
    }

    return SUCCESS;
}

#endif

/*****************************************************************************
   函 数 名  : spm_rcv_dci_set_cmd_swtich
   功能描述  : 倒换命令配置处理
   输入参数  : ATG_DCI_CMD_SWITCH *pstSetCmdSwitch
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年4月3日
    作    者   : zhangsen
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_cmd_swtich(
    ATG_DCI_CMD_SWITCH *pstSetCmdSwitch NBB_CCXT_T NBB_CXT)
{
    /*存放错误信息的字符数组*/
    NBB_CHAR ucmessage[SPM_MSG_INFO_LEN];

    NBB_INT iret = SUCCESS;                             /*函数调用返回值*/
    ATG_DCI_SWITCH_OVER_DNIPW *pstdnipwdata = NULL;     /*获取的子配置*/
    ATG_DCI_SWITCH_OVER_MSP *pstmspdata = NULL;
    ATG_DCI_SWITCH_OVER_MSRP *pstmsrpdata = NULL;
    ATG_DCI_SWITCH_OVER_PW1TO1 *pstpw1to1data = NULL;
    ATG_DCI_SWITCH_OVER_BYPASS *pstbypassdata = NULL;
	NBB_USHORT frr_group_id = 0;

    NBB_BYTE *puccmdswitchdatastart = NULL;
    NBB_BYTE *pucmspdatastart = NULL;
    NBB_BYTE *pucmsrpdatastart = NULL;
    NBB_BYTE *pucpw1to1datastart = NULL;
    NBB_BYTE *pucbypassdatastart = NULL;

    NBB_ULONG uloperdnipw = ATG_DCI_OPER_NULL;          /*子配置的操作模式*/
    NBB_ULONG ulopermsp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopermsrp = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperpw1to1 = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperbypass = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulcmdswitchidkey = 0;
    SPM_VPWS_CB *pstVpws = NULL;
    NBB_USHORT vpwsid = 0;
    
    NBB_TRC_ENTRY("spm_rcv_dci_set_cmd_swtich");

    /*输入参数指针必须有效*/
    NBB_ASSERT(pstSetCmdSwitch != NULL);

    /*如果输入参数指针为空，记录错误信息并直接退出*/
    if (NULL == pstSetCmdSwitch)
    {
        OS_PRINTF("***ERROR***:pstSetCmdSwitch(pstSetCmdSwitch==NULL)\n");
        OS_SPRINTF(ucmessage, "***ERROR***:pstSetCmdSwitch"
            "(pstSetCmdSwitch==NULL)\n");
        
        //NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, 
            //"d s s", FUNC_IN_PARAM_IS_NULL, 
            //"FUNC_IN_PARAM_IS_NULL", ucmessage));
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);

        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
//    pstSetCmdSwitch->return_code = ATG_DCI_RC_OK;

    /*从IPS中获取ID*/
    ulcmdswitchidkey = pstSetCmdSwitch->key;

    /*获取子配置的操作类型*/
    uloperdnipw = pstSetCmdSwitch->oper_dnipw_switch;
    ulopermsp = pstSetCmdSwitch->oper_msp_switch;
    ulopermsrp = pstSetCmdSwitch->oper_msrp_switch;
    uloperpw1to1 = pstSetCmdSwitch->oper_pw1to1_switch;
    uloperbypass = pstSetCmdSwitch->oper_bypass_switch;


    /**********************************************************************/
    /*                              获取配置                              */
    /**********************************************************************/
    if (SPM_PRINT_CFG == cmd_switch_cfg_print_setting)
    {
        printf("\n###################倒换命令(%s) ###################\n", "ADD");
    }

    //记录处理过程
    OS_SPRINTF(ucmessage,"################## 倒换命令 (%s) ##################\n",
        "ADD");
    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage); 

    /************************* DNI-PW倒换命令配置 *************************/
    if (ATG_DCI_OPER_ADD == uloperdnipw)
    {
        /* 计算第一个entry的地址。*/
        puccmdswitchdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->dnipw_switch_data));    

        /* 如果指针为NULL，无数据 */
        if (puccmdswitchdatastart == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  CmdSwitch dni_pw_data is NULL."));   
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
        }
        else
        {
            pstdnipwdata = (ATG_DCI_SWITCH_OVER_DNIPW *)puccmdswitchdatastart;
            if (cmd_switch_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  11）DniPw倒换命令\n");
                spm_dbg_print_cmd_switch_dni_pw_cfg(pstdnipwdata);
            }

            OS_SPRINTF(ucmessage,"  11）DniPw倒换命令\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_dni_pw_cfg(pstdnipwdata);

            if ((0 == pstdnipwdata->switch_action) || 
                (pstdnipwdata->switch_action > 7))
            {
                OS_PRINTF("***ERROR***:DNI-PW倒换命令的倒换动作%d"
                    "不在正常范围内!\n", pstdnipwdata->switch_action);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                    "***ERROR***:DNI-PW倒换命令的倒换动作%d"
                    "不在正常范围内!\n", pstdnipwdata->switch_action);
            }
            else
            {
                //调用aps接口，todo
                //aps_dni_pw_set_cmd(pstDniPwData->dni_pw_id,
                    //pstDniPwData->vpws_id, pstDniPwData->switch_action);

                OS_MEMCPY(&(SHARED.cmd_switch_cb.dni_pw_switch_cfg_cb), 
                    pstdnipwdata, sizeof(ATG_DCI_SWITCH_OVER_DNIPW));
            }
        }
    }
    else if(ATG_DCI_OPER_UPDATE == uloperdnipw)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }
    else if(ATG_DCI_OPER_DEL == uloperdnipw)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }

    /************************* MSP倒换命令配置 *************************/
    if (ATG_DCI_OPER_ADD == ulopermsp)
    {
        /* 计算第一个entry的地址。*/
        pucmspdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->msp_switch_data));    

        /* 如果指针为NULL，无数据 */
        if (pucmspdatastart == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  CmdSwitch msp_data is NULL."));   
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
        }
        else
        {
            pstmspdata = (ATG_DCI_SWITCH_OVER_MSP *)pucmspdatastart;
            if (cmd_switch_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  8）MSP倒换命令\n");
                spm_dbg_print_cmd_switch_msp_cfg(pstmspdata);
            }

            OS_SPRINTF(ucmessage,"  8）MSP倒换命令\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_msp_cfg(pstmspdata);

            if ((pstmspdata->msp_type <= 0) || 
                (pstmspdata->msp_type > 7))
            {
                OS_PRINTF("***ERROR***:MSP倒换命令的倒换动作%d"
                    "不在正常范围内!\n", pstmspdata->msp_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                    "***ERROR***:MSP倒换命令的倒换动作%d"
                    "不在正常范围内!\n", pstmspdata->msp_type);
            }
            else
            {
                //调用aps接口，todo
                aps_set_msp_cmd(pstmspdata->msp_id,pstmspdata->msp_type);

                OS_MEMCPY(&(SHARED.cmd_switch_cb.msp_switch_cfg_cb), 
                    pstmspdata, sizeof(ATG_DCI_SWITCH_OVER_MSP));
            }
        }
    }
    else if(ATG_DCI_OPER_UPDATE == ulopermsp)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }
    else if(ATG_DCI_OPER_DEL == ulopermsp)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }

    /************************* MSRP倒换命令配置 *************************/
    
    if (ATG_DCI_OPER_ADD == ulopermsrp)
        {
            /* 计算第一个entry的地址。*/
            pucmsrpdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
                pstSetCmdSwitch, &(pstSetCmdSwitch->msrp_switch_data));    
    
            /* 如果指针为NULL，无数据 */
            if (pucmsrpdatastart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  CmdSwitch msrp_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstmsrpdata = (ATG_DCI_SWITCH_OVER_MSRP *)pucmsrpdatastart;
                if (cmd_switch_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  8）MSRP倒换命令\n");
                    spm_dbg_print_cmd_switch_msrp_cfg(pstmsrpdata);
                }
    
                OS_SPRINTF(ucmessage,"  8）MSRP倒换命令\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
                spm_dbg_record_cmd_switch_msrp_cfg(pstmsrpdata);
    
                if ((pstmsrpdata->msrp_type <= 0) || 
                    (pstmsrpdata->msrp_type > 5))
                {
                    OS_PRINTF("***ERROR***:MSRP倒换命令的倒换动作%d"
                        "不在正常范围内!\n", pstmsrpdata->msrp_type);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                        "***ERROR***:MSRP倒换命令的倒换动作%d"
                        "不在正常范围内!\n", pstmsrpdata->msrp_type);
                }
                else
                {
                    //调用aps接口
                    //spm_wrap_set_cmd(pstMsrpData->msrp_type,pstMsrpData->msrp_id,pstMsrpData->direction);
                    OS_MEMCPY(&(SHARED.cmd_switch_cb.msrp_switch_cfg_cb), 
                        pstmsrpdata, sizeof(ATG_DCI_SWITCH_OVER_MSRP));
                }
            }
        }
        else if(ATG_DCI_OPER_UPDATE == ulopermsrp)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if(ATG_DCI_OPER_DEL == ulopermsrp)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }

    /************************* PW冗余倒换命令配置 *************************/
    if (ATG_DCI_OPER_ADD == uloperpw1to1)
    {
        /* 计算第一个entry的地址。*/
        pucpw1to1datastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->pw1to1_switch_data));    

        /* 如果指针为NULL，无数据 */
        if (pucpw1to1datastart == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  CmdSwitch pw1To1_data is NULL."));   
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
        }
        else
        {
            pstpw1to1data = (ATG_DCI_SWITCH_OVER_PW1TO1 *)pucpw1to1datastart;

            if (cmd_switch_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  13）Pw保护倒换命令\n");
                spm_dbg_print_cmd_switch_pw_cfg(pstpw1to1data);
            }

            OS_SPRINTF(ucmessage,"  13）Pw保护倒换命令\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_pw_cfg(pstpw1to1data);
            
            for (pstVpws = (SPM_VPWS_CB *)AVLL_FIRST(v_spm_shared->vpws_tree);
                pstVpws != NULL;
                pstVpws = (SPM_VPWS_CB *)AVLL_NEXT(v_spm_shared->vpws_tree,
                    pstVpws->spm_vpws_node))                                         
            {
            	#if 0

                if((NULL != pstVpws->basic_cfg_cb) && (pstpw1to1data->vpws_id == pstVpws->basic_cfg_cb->l2vc_id))
                {
                    vpwsid = pstVpws->vpws_id_key;
    
                    break;
                } 
				#endif
            }  

            iret = spm_get_nni_info_from_vpws_id(vpwsid, 
                &frr_group_id NBB_CCXT);
            if (SUCCESS != iret)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_nni_info_from_vpws_id() error! ret=%d",
                     iret));

                //OS_PRINTF("***ERROR***: spm_get_nni_info_from_vpws_id()"
                    //" ret=%d\n", iRet);

                OS_SPRINTF(ucmessage, "***ERROR***: spm_get_nni_info_from_vpws_id() ret=%d\n", iret);
                    
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);

                //NBB_EXCEPTION((PCT_SPM | CMD_SWITCH_PD, 0, "d s s",
                    //CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));

            }
            else
            {
                if ((0 == pstpw1to1data->switch_action ) || 
                    (pstpw1to1data->switch_action > 7))
                {
                    OS_PRINTF("***ERROR***:PW冗余倒换命令的倒换动作%d"
                        "不在正常范围内!\n", pstpw1to1data->switch_action);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                        "***ERROR***:PW冗余倒换命令的倒换动作%d"
                        "不在正常范围内!\n", pstpw1to1data->switch_action);
                }
                else
                {
                    spm_lps_set_cmd(pstpw1to1data->switch_action, 
                        frr_group_id);

                    OS_MEMCPY(&(SHARED.cmd_switch_cb.pw1To1_switch_cfg_cb), 
                        pstpw1to1data, sizeof(ATG_DCI_SWITCH_OVER_PW1TO1));
                }
            }
        }
    }
    else if(ATG_DCI_OPER_UPDATE == uloperpw1to1)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }
    else if(ATG_DCI_OPER_DEL == uloperpw1to1)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }

    /************************* BYPASS倒换命令配置 *************************/
    if (ATG_DCI_OPER_ADD == uloperbypass)
    {
        /* 计算第一个entry的地址。*/
        pucbypassdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->bypass_switch_data));    

        /* 如果指针为NULL，无数据 */
        if (pucbypassdatastart == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  CmdSwitch bypass_data is NULL."));   
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
        }
        else
        {
            pstbypassdata = (ATG_DCI_SWITCH_OVER_BYPASS *)pucbypassdatastart;

            if (cmd_switch_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  10）Bypass双归倒换命令\n");
                spm_dbg_print_cmd_switch_bypass_cfg(pstbypassdata);
            }

            OS_SPRINTF(ucmessage,"  10）Bypass双归倒换命令\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_bypass_cfg(pstbypassdata);

            if ((0 == pstbypassdata->bypass_type) || 
                (pstbypassdata->bypass_type > 7))
            {
                OS_PRINTF("***ERROR***:BYPASS双归倒换命令的倒换动作%d"
                    "不在正常范围内!\n", pstbypassdata->bypass_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                    "***ERROR***:BYPASS双归倒换命令的倒换动作%d"
                    "不在正常范围内!\n", pstbypassdata->bypass_type);
            }
            else
            {
                //aps_dh_vpws_set_cmd(pstBypassData->dh_id, pstBypassData->vpws_id, 
                    //pstBypassData->bypass_type);

                OS_MEMCPY(&(SHARED.cmd_switch_cb.bypass_switch_cfg_cb), 
                    pstbypassdata, sizeof(ATG_DCI_SWITCH_OVER_BYPASS));
            }

        }
    }
    else if(ATG_DCI_OPER_UPDATE == uloperbypass)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }
    else if(ATG_DCI_OPER_DEL == uloperbypass)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
    }
    
    //spm_cmd_ip_frr_switch_handler(pstSetCmdSwitch);

    //spm_cmd_vpn_frr_switch_handler(pstSetCmdSwitch);
    
    //spm_cmd_lsp_1_to_1_switch_handler(pstSetCmdSwitch);

    //spm_cmd_sncp_switch_handler(pstSetCmdSwitch);

    EXIT_LABEL: 
    
    NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_init_cmd_switch_cb
 功能描述  : 初始化倒换命令配置结构体
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月3日
    作    者   : zhangsen
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_cmd_switch_cb(NBB_CXT_T NBB_CXT)
{    
    NBB_TRC_ENTRY("spm_init_cmd_switch_cb");

    OS_MEMSET(&(SHARED.cmd_switch_cb), 0, sizeof(SPM_CMD_SWITCH_CB));
    
    NBB_TRC_EXIT();    
}


#endif
