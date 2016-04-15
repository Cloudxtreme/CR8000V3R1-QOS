/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_cmd_switch_proc.c
   �� �� ��   : ����
   ��    ��   : zhangsen	
   ��������   : 2014��4��3��
   ����޸�   :
   ��������   : �����������ô���
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2014��4��3��
    ��    ��   : zhangsen
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef CMD_SWITCH_CFG

/*cmd_switch���ÿ��ӡ����*/
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
   �� �� ��  : spm_rcv_dci_set_cmd_swtich
   ��������  : �����������ô���
   �������  : ATG_DCI_CMD_SWITCH *pstSetCmdSwitch
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��4��3��
    ��    ��   : zhangsen
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_cmd_swtich(
    ATG_DCI_CMD_SWITCH *pstSetCmdSwitch NBB_CCXT_T NBB_CXT)
{
    /*��Ŵ�����Ϣ���ַ�����*/
    NBB_CHAR ucmessage[SPM_MSG_INFO_LEN];

    NBB_INT iret = SUCCESS;                             /*�������÷���ֵ*/
    ATG_DCI_SWITCH_OVER_DNIPW *pstdnipwdata = NULL;     /*��ȡ��������*/
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

    NBB_ULONG uloperdnipw = ATG_DCI_OPER_NULL;          /*�����õĲ���ģʽ*/
    NBB_ULONG ulopermsp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopermsrp = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperpw1to1 = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperbypass = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulcmdswitchidkey = 0;
    SPM_VPWS_CB *pstVpws = NULL;
    NBB_USHORT vpwsid = 0;
    
    NBB_TRC_ENTRY("spm_rcv_dci_set_cmd_swtich");

    /*�������ָ�������Ч*/
    NBB_ASSERT(pstSetCmdSwitch != NULL);

    /*����������ָ��Ϊ�գ���¼������Ϣ��ֱ���˳�*/
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

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
//    pstSetCmdSwitch->return_code = ATG_DCI_RC_OK;

    /*��IPS�л�ȡID*/
    ulcmdswitchidkey = pstSetCmdSwitch->key;

    /*��ȡ�����õĲ�������*/
    uloperdnipw = pstSetCmdSwitch->oper_dnipw_switch;
    ulopermsp = pstSetCmdSwitch->oper_msp_switch;
    ulopermsrp = pstSetCmdSwitch->oper_msrp_switch;
    uloperpw1to1 = pstSetCmdSwitch->oper_pw1to1_switch;
    uloperbypass = pstSetCmdSwitch->oper_bypass_switch;


    /**********************************************************************/
    /*                              ��ȡ����                              */
    /**********************************************************************/
    if (SPM_PRINT_CFG == cmd_switch_cfg_print_setting)
    {
        printf("\n###################��������(%s) ###################\n", "ADD");
    }

    //��¼�������
    OS_SPRINTF(ucmessage,"################## �������� (%s) ##################\n",
        "ADD");
    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage); 

    /************************* DNI-PW������������ *************************/
    if (ATG_DCI_OPER_ADD == uloperdnipw)
    {
        /* �����һ��entry�ĵ�ַ��*/
        puccmdswitchdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->dnipw_switch_data));    

        /* ���ָ��ΪNULL�������� */
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
                printf("  11��DniPw��������\n");
                spm_dbg_print_cmd_switch_dni_pw_cfg(pstdnipwdata);
            }

            OS_SPRINTF(ucmessage,"  11��DniPw��������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_dni_pw_cfg(pstdnipwdata);

            if ((0 == pstdnipwdata->switch_action) || 
                (pstdnipwdata->switch_action > 7))
            {
                OS_PRINTF("***ERROR***:DNI-PW��������ĵ�������%d"
                    "����������Χ��!\n", pstdnipwdata->switch_action);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                    "***ERROR***:DNI-PW��������ĵ�������%d"
                    "����������Χ��!\n", pstdnipwdata->switch_action);
            }
            else
            {
                //����aps�ӿڣ�todo
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

    /************************* MSP������������ *************************/
    if (ATG_DCI_OPER_ADD == ulopermsp)
    {
        /* �����һ��entry�ĵ�ַ��*/
        pucmspdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->msp_switch_data));    

        /* ���ָ��ΪNULL�������� */
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
                printf("  8��MSP��������\n");
                spm_dbg_print_cmd_switch_msp_cfg(pstmspdata);
            }

            OS_SPRINTF(ucmessage,"  8��MSP��������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_msp_cfg(pstmspdata);

            if ((pstmspdata->msp_type <= 0) || 
                (pstmspdata->msp_type > 7))
            {
                OS_PRINTF("***ERROR***:MSP��������ĵ�������%d"
                    "����������Χ��!\n", pstmspdata->msp_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                    "***ERROR***:MSP��������ĵ�������%d"
                    "����������Χ��!\n", pstmspdata->msp_type);
            }
            else
            {
                //����aps�ӿڣ�todo
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

    /************************* MSRP������������ *************************/
    
    if (ATG_DCI_OPER_ADD == ulopermsrp)
        {
            /* �����һ��entry�ĵ�ַ��*/
            pucmsrpdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
                pstSetCmdSwitch, &(pstSetCmdSwitch->msrp_switch_data));    
    
            /* ���ָ��ΪNULL�������� */
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
                    printf("  8��MSRP��������\n");
                    spm_dbg_print_cmd_switch_msrp_cfg(pstmsrpdata);
                }
    
                OS_SPRINTF(ucmessage,"  8��MSRP��������\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
                spm_dbg_record_cmd_switch_msrp_cfg(pstmsrpdata);
    
                if ((pstmsrpdata->msrp_type <= 0) || 
                    (pstmsrpdata->msrp_type > 5))
                {
                    OS_PRINTF("***ERROR***:MSRP��������ĵ�������%d"
                        "����������Χ��!\n", pstmsrpdata->msrp_type);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                        "***ERROR***:MSRP��������ĵ�������%d"
                        "����������Χ��!\n", pstmsrpdata->msrp_type);
                }
                else
                {
                    //����aps�ӿ�
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

    /************************* PW���൹���������� *************************/
    if (ATG_DCI_OPER_ADD == uloperpw1to1)
    {
        /* �����һ��entry�ĵ�ַ��*/
        pucpw1to1datastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->pw1to1_switch_data));    

        /* ���ָ��ΪNULL�������� */
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
                printf("  13��Pw������������\n");
                spm_dbg_print_cmd_switch_pw_cfg(pstpw1to1data);
            }

            OS_SPRINTF(ucmessage,"  13��Pw������������\n");
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
                    OS_PRINTF("***ERROR***:PW���൹������ĵ�������%d"
                        "����������Χ��!\n", pstpw1to1data->switch_action);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                        "***ERROR***:PW���൹������ĵ�������%d"
                        "����������Χ��!\n", pstpw1to1data->switch_action);
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

    /************************* BYPASS������������ *************************/
    if (ATG_DCI_OPER_ADD == uloperbypass)
    {
        /* �����һ��entry�ĵ�ַ��*/
        pucbypassdatastart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
            pstSetCmdSwitch, &(pstSetCmdSwitch->bypass_switch_data));    

        /* ���ָ��ΪNULL�������� */
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
                printf("  10��Bypass˫�鵹������\n");
                spm_dbg_print_cmd_switch_bypass_cfg(pstbypassdata);
            }

            OS_SPRINTF(ucmessage,"  10��Bypass˫�鵹������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
            spm_dbg_record_cmd_switch_bypass_cfg(pstbypassdata);

            if ((0 == pstbypassdata->bypass_type) || 
                (pstbypassdata->bypass_type > 7))
            {
                OS_PRINTF("***ERROR***:BYPASS˫�鵹������ĵ�������%d"
                    "����������Χ��!\n", pstbypassdata->bypass_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, 
                    "***ERROR***:BYPASS˫�鵹������ĵ�������%d"
                    "����������Χ��!\n", pstbypassdata->bypass_type);
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
 �� �� ��  : spm_init_cmd_switch_cb
 ��������  : ��ʼ�������������ýṹ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��3��
    ��    ��   : zhangsen
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_cmd_switch_cb(NBB_CXT_T NBB_CXT)
{    
    NBB_TRC_ENTRY("spm_init_cmd_switch_cb");

    OS_MEMSET(&(SHARED.cmd_switch_cb), 0, sizeof(SPM_CMD_SWITCH_CB));
    
    NBB_TRC_EXIT();    
}


#endif
