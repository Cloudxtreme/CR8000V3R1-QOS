/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_proc.c
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS����
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : zenglu
    �޸�����   : �����ļ�

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/* ȫ�ִ�ӡ����qos_log_cfg_print */
extern NBB_BYTE qos_log_cfg_print;

/* ȫ�ִ�ӡ����qos_vc_cfg_print */
extern NBB_BYTE qos_vc_cfg_print;

/* ȫ�ִ�ӡ����qos_vrf_cfg_print */
extern NBB_BYTE qos_vrf_cfg_print;

/* ȫ�ִ�ӡ����qos_txlsp_cfg_print */
extern NBB_BYTE qos_txlsp_cfg_print;






#if 1

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_phb2pri_vlan_map(NBB_USHORT dsptr, 
 NBB_BYTE map[ATG_DCI_MAX_PHB_NUM][ATG_DCI_MAX_COLOR_NUM] NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT k = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_phb2pri_vlan_map");

    /* ָ��Ϊ�� */
    if (NULL == map)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR phb2pri_vlan map is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage); 
        printf("**QOS ERROR**%s,%d,ָ��Ϊ��\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"QOS ָ��Ϊ��",ATG_DCI_RC_UNSUCCESSFUL,
					"","","","",
					0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN, k = 0; j < MAX_COLOR_NUM; j++, k++)
        {
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToPri(unit, dsptr, i, j, map[i][k]);               
                if (ATG_DCI_RC_OK != ret)
                {
            	    OS_SPRINTF(ucMessage,"%s,%d,ret=%ld : ApiC3SetPhbToPri ERROR.\n\n",__FUNCTION__,__LINE__,ret);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                    goto EXIT_LABEL;
                }  
#endif   
            } 
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_phb2pri_exp_map(NBB_USHORT dsptr,
    NBB_BYTE map[ATG_DCI_MAX_PHB_NUM][ATG_DCI_MAX_COLOR_NUM] NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT k = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_phb2pri_exp_map");

    /*ָ��Ϊ��*/
    if (NULL == map)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR phb2pri_exp map is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage); 
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN, k = 0; j < MAX_COLOR_NUM; j++, k++)
        {
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToExp(unit, dsptr, i, j, map[i][k]);          
                if (ATG_DCI_RC_OK != ret)
                {
            	    OS_SPRINTF(ucMessage,"%s,%d,ret=%ld : ApiC3SetPhbToExp ERROR.\n\n",__FUNCTION__,__LINE__,ret);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                    goto EXIT_LABEL;
                }
#endif  
            } 
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_phb2pri_dscp_map(NBB_USHORT dsptr,
    NBB_BYTE map[ATG_DCI_MAX_PHB_NUM][ATG_DCI_MAX_COLOR_NUM] NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT k = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_phb2pri_dscp_map");

    /*ָ��Ϊ��*/
    if (NULL == map)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR phb2pri_dscp map is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage); 
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN, k = 0; j < MAX_COLOR_NUM; j++, k++)
        {
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, map[i][k]);          
                if (ATG_DCI_RC_OK != ret)
                {
            	    OS_SPRINTF(ucMessage,"%s,%d,ret=%ld : ApiC3SetPhbToDscp ERROR.\n\n",__FUNCTION__,__LINE__,ret);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                    goto EXIT_LABEL;
                }
#endif 
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_pri2phb_vlan_map(NBB_USHORT dsptr, 
    ATG_DCI_DS_DOMAIN_PHB_COLOR *map NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_pri2phb_vlan_map");

    if (NULL == map)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pri2phb_vlan map is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage); 
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for (i = 0; i < MAX_VLAN_PRI_NUM; i++, map++)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPriToPhb(unit, dsptr, i, map->phb, map->color);       
            if (ATG_DCI_RC_OK != ret)
            {
        	    OS_SPRINTF(ucMessage,"%s,%d,ret=%ld : ApiC3SetPriToPhb ERROR.\n\n",__FUNCTION__,__LINE__,ret);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_pri2phb_exp_map(NBB_USHORT dsptr,
    ATG_DCI_DS_DOMAIN_PHB_COLOR *map NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_pri2phb_exp_map");

    if (NULL == map)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pri2phb_exp map is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage); 
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < MAX_EXP_PRI_NUM; i++, map++)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret += ApiC3SetExpToPhb(unit, dsptr, i, map->phb, map->color);        
            if (ATG_DCI_RC_OK != ret)
            {
        	    OS_SPRINTF(ucMessage,"%s,%d,ret=%ld : ApiC3SetExpToPhb ERROR.\n\n",__FUNCTION__,__LINE__,ret);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif 
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_pri2phb_dscp_map(NBB_USHORT dsptr, 
    ATG_DCI_DS_DOMAIN_PHB_COLOR *map NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_pri2phb_dscp_map");

    if (NULL == map)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pri2phb_dscp map is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage); 
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < ATG_DCI_DSCP_MAP_NUM; i++, map++)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret += ApiC3SetDscpToPhb(unit, dsptr, i, map->phb, map->color);      
            if (ATG_DCI_RC_OK != ret)
            {
        	    OS_SPRINTF(ucMessage,"%s,%d,ret=%ld : ApiC3SetDscpToPhb ERROR.\n\n",__FUNCTION__,__LINE__,ret);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif 
        }
        
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_exp2phb_map(NBB_USHORT dsptr NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
        /* Ĭ�ϵ�vlanpri��PHBӳ�� */
        for (i = 0; i < MAX_EXP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetExpToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_dscp2phb_map(NBB_USHORT dsptr NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE ucC3Num = 0;
#if defined (SPU) || defined (PTN690_CES)
    SPM_QOS_PRI2PHB_MAP ds_pri2phb_map[MAX_DSCP_PRI_NUM] = {
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS6, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS7, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN}
    };
#endif

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
        for (i = 0; i < MAX_DSCP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetDscpToPhb(unit,dsptr,i,ds_pri2phb_map[i].phb,ds_pri2phb_map[i].color);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }
    } 

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_vlan2phb_map(NBB_USHORT dsptr NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
        /* Ĭ�ϵ�vlanpri��PHBӳ�� */
        for (i = 0; i < MAX_VLAN_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPriToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }    
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_pri2phb_map(NBB_USHORT dsptr NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE ucC3Num = 0;

#if defined (SPU) || defined (PTN690_CES)
    SPM_QOS_PRI2PHB_MAP ds_pri2phb_map[MAX_DSCP_PRI_NUM] = {
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS6, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS7, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN}
    };
#endif

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
        /* Ĭ�ϵ�vlanpri��PHBӳ�� */
        for (i = 0; i < MAX_VLAN_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPriToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }

        /* Ĭ�ϵ�exp��PHBӳ�� */
        for (i = 0; i < MAX_EXP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetExpToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }

        /* Ĭ�ϵ�dscp��PHBӳ�� */
        for (i = 0; i < MAX_DSCP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetDscpToPhb(unit,dsptr,i,ds_pri2phb_map[i].phb,ds_pri2phb_map[i].color);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }       
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_phb2exp_map(NBB_USHORT dsptr NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    
    //NBB_USHORT offset = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    
    for(unit = 0;unit < ucC3Num;unit++)
    {
        /* phb��expӳ�� */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_GREEN, i);
            if (ATG_DCI_RC_OK != ret)
            {
              goto EXIT_LABEL;
            }
#endif
        }

        /* phb��expӳ�� */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        } 
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_phb2dscp_map(NBB_USHORT dsptr NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT offset = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_BE, QOS_COLOR_GREEN, 0);
        if (ATG_DCI_RC_OK != ret)
        {
           goto EXIT_LABEL;
        }
#endif
        for (i = PHB_AF1, offset = 10; i < PHB_EF; i++, offset += 2)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++, offset += 2)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, offset);
                if (ATG_DCI_RC_OK != ret)
                {
                   goto EXIT_LABEL;
                }
#endif
            }
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_EF, QOS_COLOR_GREEN, 46);
        if (ATG_DCI_RC_OK != ret)
        {
           goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS6, QOS_COLOR_GREEN, 48);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS7, QOS_COLOR_GREEN, 56);
        if (ATG_DCI_RC_OK != ret)
        {
           goto EXIT_LABEL;
        }  
#endif   
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_phb2vlan_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    
    //NBB_USHORT offset = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
        /* phb��vlanӳ�� */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_GREEN, i);
            if (ATG_DCI_RC_OK != ret)
            {
               goto EXIT_LABEL;
            }
#endif
        }

        /* phb��vlanӳ�� */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
              goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
               goto EXIT_LABEL;
            }
#endif
        }
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_default_phb2pri_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT offset = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    for(unit = 0;unit < ucC3Num;unit++)
    {
        /* phb��vlanӳ�� */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_GREEN, i);
            if (ATG_DCI_RC_OK != ret)
            {
               goto EXIT_LABEL;
            }
#endif
        }

        /* phb��vlanӳ�� */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
              goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
               goto EXIT_LABEL;
            }
#endif
        }

        /* phb��expӳ��:Ĭ��һһӳ��,���ڽ�����ܻ�mantis0072497 */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for(j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToExp(unit, dsptr, i, j, i);
                if (ATG_DCI_RC_OK != ret)
                {
                  goto EXIT_LABEL;
                }
#endif
            }
        }

#if 0
        /* phb��expӳ�� */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }
#endif

        /* phb be��DSCPӳ�� */
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_BE, QOS_COLOR_GREEN, 0);
        if (ATG_DCI_RC_OK != ret)
        {
           goto EXIT_LABEL;
        }
#endif
        for (i = PHB_AF1, offset = 10; i < PHB_EF; i++, offset += 2)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++, offset += 2)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, offset);
                if (ATG_DCI_RC_OK != ret)
                {
                   goto EXIT_LABEL;
                }
#endif
            }
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_EF, QOS_COLOR_GREEN, 46);
        if (ATG_DCI_RC_OK != ret)
        {
           goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS6, QOS_COLOR_GREEN, 48);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS7, QOS_COLOR_GREEN, 56);
        if (ATG_DCI_RC_OK != ret)
        {
           goto EXIT_LABEL;
        }  
#endif
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_ds_pri2phb_map_init(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT dsptr = 0;

    for (dsptr = 0; dsptr < 16; dsptr++)
    {
        ret = spm_set_default_pri2phb_map(dsptr);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_ds_phb2pri_map_init(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT dsptr = 0;

    for (dsptr = 0; dsptr < PHB2PRI_ASSIGN_OFFSET; dsptr++)
    {
        ret = spm_set_default_phb2pri_map(dsptr); 
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL:
    return ret;
}

#endif


#if 2

/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_logic_intf_cb
   ��������  : �����߼��˿���INTF DS�ڵ�
   �������  : �߼��˿ڵ�key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_LOGIC_INTF_CB *spm_alloc_qos_ds_logic_intf_cb(
    SPM_QOS_LOGIC_INTF_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_INTF_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_logic_intf_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR intf ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_LOGIC_INTF_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_LOGIC_INTF_CB),
              NBB_NORETRY_ACT, MEM_SPM_QOS_DS_LOGIC_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_LOGIC_INTF_CB));
    OS_MEMCPY(&pstTbl->logic_key, pkey, sizeof(SPM_QOS_LOGIC_INTF_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_logic_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_logic_univp_cb
   ��������  : ����vp�»�uni��ds�ڵ�
   �������  : �߼��˿�����key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_LOGIC_UNIVP_CB *spm_alloc_qos_ds_logic_univp_cb(
    NBB_ULONG LogicIndex, SPM_QOS_LOGIC_UNIVP* UniVp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_UNIVP_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_logic_univp_cb");

    if (0 == LogicIndex)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR UNIVP ds LogicIndex is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ�vp�»�uni��ds������Ŀ��*/
    pstTbl = (SPM_QOS_DS_LOGIC_UNIVP_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_LOGIC_UNIVP_CB),
              NBB_NORETRY_ACT, MEM_SPM_QOS_DS_LOGIC_UNIVP_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_LOGIC_UNIVP_CB));
    OS_MEMCPY(&(pstTbl->vpuni), UniVp, sizeof(SPM_QOS_LOGIC_UNIVP));
    AVLL_INIT_NODE(pstTbl->spm_qos_logic_univp_node);
    pstTbl->logic_index = LogicIndex;

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_logic_flow_cb
   ��������  : �����߼��˿���INTF DS�ڵ�
   �������  : �߼��˿�����key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_LOGIC_FLOW_CB *spm_alloc_qos_ds_logic_flow_cb(
    SPM_QOS_LOGIC_FLOW_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_FLOW_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_logic_flow_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR flow ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_LOGIC_FLOW_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_LOGIC_FLOW_CB),
              NBB_NORETRY_ACT, MEM_SPM_QOS_DS_LOGIC_FLOW_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_LOGIC_FLOW_CB));
    OS_MEMCPY(&pstTbl->flow_key, pkey, sizeof(SPM_QOS_LOGIC_FLOW_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_logic_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_vc_cb
   ��������  : ����VC DS�ڵ�
   �������  : vc key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_VC_CB *spm_alloc_qos_ds_vc_cb(ATG_DCI_VC_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_VC_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_vc_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vc ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_VC_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_VC_CB),NBB_NORETRY_ACT, MEM_SPM_QOS_DS_VC_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_VC_CB));
    OS_MEMCPY(&pstTbl->vc_key, pkey, sizeof(ATG_DCI_VC_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_vc_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_vrf_cb
   ��������  : ����VRFDS�ڵ�
   �������  : vrf key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_VRF_CB *spm_alloc_qos_ds_vrf_cb(SPM_QOS_VRF_INSTANSE_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_VRF_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_vrf_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_VRF_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_VRF_CB),NBB_NORETRY_ACT, MEM_SPM_QOS_DS_VRF_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_VRF_CB));
    OS_MEMCPY(&pstTbl->vrf_key, pkey, sizeof(SPM_QOS_VRF_INSTANSE_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_vrf_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_ilm_cb
   ��������  : ����ILM DS�ڵ�
   �������  : ILM��key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_ILM_CB *spm_alloc_qos_ds_ilm_cb(SPM_QOS_ILM_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_ILM_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_ilm_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ilm ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_ILM_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_ILM_CB),NBB_NORETRY_ACT, MEM_SPM_QOS_DS_ILM_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_ILM_CB));
    OS_MEMCPY(&pstTbl->ilm_key, pkey, sizeof(SPM_QOS_ILM_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_ilm_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_rx_lsp_cb
   ��������  : ����RX LSP DS�ڵ�
   �������  : RX LSP��key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_RX_LSP_CB *spm_alloc_qos_ds_rx_lsp_cb(
    SPM_QOS_DS_RXLSP_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_RX_LSP_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_rx_lsp_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR rxlsp ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_RX_LSP_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_RX_LSP_CB),     \
                NBB_NORETRY_ACT, MEM_SPM_QOS_DS_RX_LSP_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_RX_LSP_CB));
    OS_MEMCPY(&pstTbl->rx_lsp_key, pkey, sizeof(SPM_QOS_DS_RXLSP_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_rx_lsp_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_tx_lsp_cb
   ��������  : ����TX LSP DS�ڵ�
   �������  : TX LSP��key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_TX_LSP_CB *spm_alloc_qos_ds_tx_lsp_cb(CRTXLSP_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_TX_LSP_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_tx_lsp_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR txlsp ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_TX_LSP_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_TX_LSP_CB),     \
                NBB_NORETRY_ACT, MEM_SPM_QOS_DS_TX_LSP_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_TX_LSP_CB));
    OS_MEMCPY(&pstTbl->tx_lsp_key, pkey, sizeof(CRTXLSP_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_tx_lsp_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_qos_ds_tunnel_cb
   ��������  : ����TX LSP DS�ڵ�
   �������  : TX LSP��key
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_QOS_DS_TUNNEL_CB *spm_alloc_qos_ds_tunnel_cb(SPM_QOS_TUNNEL_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_TUNNEL_CB *pstTbl = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_alloc_qos_ds_tunnel_cb");

    if (NULL == pkey)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR tunnel ds pkey is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_DS_TUNNEL_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DS_TUNNEL_CB),     \
                NBB_NORETRY_ACT, MEM_SPM_QOS_DS_TUNNEL_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DS_TUNNEL_CB));
    OS_MEMCPY(&pstTbl->tunnel_key, pkey, sizeof(SPM_QOS_TUNNEL_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_qos_tunnel_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_logic_intf_cb
   ��������  : �ͷ��߼��˿���INTF DS�ڵ�
   �������  : �߼��˿�ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_logic_intf_cb(SPM_QOS_DS_LOGIC_INTF_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_logic_intf_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR intf ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_LOGIC_INTF_CB), MEM_SPM_QOS_DS_LOGIC_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_LOGIC_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_logic_flow_cb
   ��������  : �ͷ��߼��˿���FLOW DS�ڵ�
   �������  : �߼��˿�����ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_logic_flow_cb(SPM_QOS_DS_LOGIC_FLOW_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_logic_flow_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR flow ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_LOGIC_FLOW_CB), MEM_SPM_QOS_DS_LOGIC_FLOW_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_LOGIC_FLOW_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_logic_univp_cb
   ��������  : �ͷ�univp DS�ڵ�
   �������  : �߼��˿�����ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_logic_univp_cb(SPM_QOS_DS_LOGIC_UNIVP_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_logic_univp_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR flow ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_LOGIC_UNIVP_CB), MEM_SPM_QOS_DS_LOGIC_UNIVP_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_LOGIC_UNIVP_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_vc_cb
   ��������  : �ͷ�vc DS�ڵ�
   �������  : vcָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_vc_cb(SPM_QOS_DS_VC_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_vc_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vc ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_VC_CB), MEM_SPM_QOS_DS_VC_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_VC_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_vrf_cb
   ��������  : �ͷ�vrf DS�ڵ�
   �������  : vrfָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_vrf_cb(SPM_QOS_DS_VRF_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_vrf_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_VRF_CB), MEM_SPM_QOS_DS_VRF_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_VRF_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_ilm_cb
   ��������  : �ͷ�ILM DS�ڵ�
   �������  : ILM ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_ilm_cb(SPM_QOS_DS_ILM_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_ilm_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ilm ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_ILM_CB), MEM_SPM_QOS_DS_ILM_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_ILM_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_rx_lsp_cb
   ��������  : �ͷ�RX LSP DS�ڵ�
   �������  : RX LSP ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_rx_lsp_cb(SPM_QOS_DS_RX_LSP_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_rx_lsp_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR rxlsp ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_RX_LSP_CB), MEM_SPM_QOS_DS_RX_LSP_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_RX_LSP_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_tx_lsp_cb
   ��������  : �ͷ�TX LSP DS�ڵ�
   �������  : TX LSP ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_tx_lsp_cb(SPM_QOS_DS_TX_LSP_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_tx_lsp_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR txlsp ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_TX_LSP_CB), MEM_SPM_QOS_DS_TX_LSP_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_TX_LSP_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_tunnel_cb
   ��������  : �ͷ�TX LSP DS�ڵ�
   �������  : TX LSP ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_tunnel_cb(SPM_QOS_DS_TUNNEL_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_tunnel_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR tunnel ds pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DS_TUNNEL_CB), MEM_SPM_QOS_DS_TUNNEL_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_DS_TUNNEL_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 3

/*****************************************************************************
   �� �� ��  : spm_find_ds_cb
   ��������  : ����dsģ���������
   �������  : dsģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_DS_CB *spm_find_ds_cb(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_find_ds_cb");

    if (0 == index) /*�Ƿ�*/
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds index is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(SHARED.ds_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d,ds_index=%ld: QOS ERROR can't find ds cfg.\n\n",
                   __FUNCTION__,__LINE__,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
}


/*****************************************************************************
   �� �� ��  : spm_find_ds_domain_cb
   ��������  : ����ds���������
   �������  : ds���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_DS_DOMAIN_CB *spm_find_ds_domain_cb(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_find_ds_domain_cb");

    if (0 == index) /*�Ƿ�*/
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds_domain index is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(SHARED.ds_domain_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d,ds_domain_index=%ld: QOS ERROR can't find ds_domain cfg.\n\n",
                   __FUNCTION__,__LINE__,index);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
}


/*****************************************************************************
   �� �� ��  : spm_connect_ds_domain_cnt
   ��������  : ds�����ü�����1
   �������  : ds���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_connect_ds_domain_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_connect_ds_domain_cnt");

    if (0 == index) /* DS��Ϊ0���������ü��� */
    {
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.ds_domain_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds domain cfg.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    (pcb->cnt)++;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_phb2pri_ds_domain_intf_cnt
   ��������  : ds�����ü�����1
   �������  : ds���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_disconnect_ds_domain_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_disconnect_ds_domain_cnt");

    /* Ĭ������û�����ü��� */
    if (0 == index)
    {
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.ds_domain_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds domain cfg.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (0 == pcb->cnt)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds domain cnt is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pcb->cnt--;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_pri2phb_ds_cnt
   ��������  : ds���ü�����1
   �������  : ds��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_disconnect_pri2phb_ds_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_disconnect_pri2phb_ds_cnt");

    if (0 == index) /*����Ҫ���ü���*/
    {
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.ds_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds cfg.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->pri2phb_cfg_cb)
    {
        /* ӳ�� */
        if (PRI2PHB_ASSIGN != pcb->pri2phb_cfg_cb->pri_select)
        {
            ret = spm_disconnect_ds_domain_cnt(pcb->pri2phb_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    if (0 == pcb->cnt)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds cnt is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pcb->cnt--;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_phb2pri_ds_cnt
   ��������  : ds���ü�����1
   �������  : ds��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_disconnect_phb2pri_ds_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_disconnect_phb2pri_ds_cnt");

    /*����Ҫ���ü���*/
    if (0 == index)
    {
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(SHARED.ds_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds cfg.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->phb2pri_cfg_cb)
    {
        /* ����ӳ�� */
        if (PHB2PRI_MAP == pcb->phb2pri_cfg_cb->pri_select)
        {
            ret = spm_disconnect_ds_domain_cnt(pcb->phb2pri_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    if (0 == pcb->cnt)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds cnt is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* dsģ������ü��� */
    pcb->cnt--;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_ds_cnt
   ��������  : ds���ü�����1
   �������  : ds��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_disconnect_ds_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_disconnect_ds_cnt");

    if (0 == index) /*����Ҫ���ü���*/
    {
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.ds_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds cfg.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->pri2phb_cfg_cb)
    {
        /* ӳ�� */
        if (PRI2PHB_ASSIGN != pcb->pri2phb_cfg_cb->pri_select)
        {
            ret = spm_disconnect_ds_domain_cnt(pcb->pri2phb_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    if (NULL != pcb->phb2pri_cfg_cb)
    {
        /* ӳ�� */
        if (PHB2PRI_MAP == pcb->phb2pri_cfg_cb->pri_select)
        {
            ret = spm_disconnect_ds_domain_cnt(pcb->phb2pri_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    if (0 == pcb->cnt)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds cnt is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pcb->cnt--;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_intf_ds_cnt
   ��������  : ds���ü�����1
   �������  : ds��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_disconnect_intf_ds_cnt(SUB_PORT *sub_port,NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pcb = NULL;
    NBB_BYTE fapid = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_disconnect_intf_ds_cnt");

    if(NULL == sub_port)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR sub_port is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (0 == index) /*����Ҫ���ü���*/
    {
        goto EXIT_LABEL;
    }
    spm_hardw_getslot(&fapid);
    pcb = AVLL_FIND(SHARED.ds_tree, &index);
    if (NULL == pcb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds cfg.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if ((NULL != pcb->pri2phb_cfg_cb) && (fapid == sub_port->slot))
    {
        /*ӳ��*/
        if (PRI2PHB_ASSIGN != pcb->pri2phb_cfg_cb->pri_select)
        {
            ret = spm_disconnect_ds_domain_cnt(pcb->pri2phb_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    if (NULL != pcb->phb2pri_cfg_cb)
    {
        /*ӳ��*/
        if (PHB2PRI_MAP == pcb->phb2pri_cfg_cb->pri_select)
        {
            ret = spm_disconnect_ds_domain_cnt(pcb->phb2pri_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    if (0 == pcb->cnt)
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds cnt is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pcb->cnt--;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_pri2phb_l3_ds_domain_intf
   ��������  : ds�����L3 intf ���ȼ���PHB����ӳ��
   �������  : ӳ��ģʽmod��ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_pri2phb_l3_ds_domain_intf(NBB_LONG unit,NBB_BYTE mod, 
    NBB_ULONG index, NBB_ULONG posid,SPM_QOS_DS_LOGIC_INTF_CB *join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_USHORT dsptr = index;

    NBB_TRC_ENTRY("spm_add_pri2phb_l3_ds_domain_intf");

    if ((0 == posid) || (NULL == join_node))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR join_node is NULL OR posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����Ĭ������*/
    if (0 == index)
    {
#if defined (SPU) || defined (PTN690_CES)
        if(PRI2PHB_MAP_OVLAN == mod)
        {
            ret = ApiC3SetIntfPhb(unit, posid, PRI_TO_PHB, 0);
            join_node->in_mode = 0;
        }
        else if(PRI2PHB_MAP_DSCP == mod)
        {
            ret = ApiC3SetIntfPhb(unit, posid, DSCP_TO_PHB, 0);
            join_node->in_mode = DSCP_TO_PHB;
        }
        else
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR INTF DS no cfg.\n"
                       "unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,unit,mod,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (ATG_DCI_RC_OK != ret)
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "ret=%ld,unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,mod,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->in_dsptr = 0;
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if ((NULL != pcb->vlan2p_cfg_cb) && (PRI2PHB_MAP_OVLAN == mod)) /*VLAN���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetIntfPhb(unit, posid, PRI_TO_PHB, dsptr);
        if (ATG_DCI_RC_OK != ret)
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "ret=%ld,unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,mod,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->in_mode = PRI_TO_PHB;
        join_node->in_dsptr = dsptr;
    }
    else if ((NULL != pcb->d2p_cfg_cb) && (PRI2PHB_MAP_DSCP == mod)) /*DSCP���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetIntfPhb(unit, posid, DSCP_TO_PHB, dsptr);
        if (ATG_DCI_RC_OK != ret)
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "ret=%ld,unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,mod,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->in_mode = DSCP_TO_PHB;
        join_node->in_dsptr = dsptr;
    }
    else
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR no cfg.\n"
                   "unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                   __FUNCTION__,__LINE__,unit,mod,posid,dsptr);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_phb2pri_l3_ds_domain_intf
   ��������  : ds�����L3 intf ��PHB�����ȼ�����ӳ��
   �������  : ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_phb2pri_l3_ds_domain_intf(NBB_LONG unit,NBB_ULONG index,
    TX_PORT_T *node,SPM_QOS_DS_LOGIC_INTF_CB *join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_USHORT dsptr = index;

    NBB_TRC_ENTRY("spm_add_phb2pri_l3_ds_domain_intf");

    if ((NULL == node) || (NULL == join_node))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR join_node OR node is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����Ĭ������*/
    if (0 == index)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3UniPri(unit, 0, node, PHB_TO_DSCP_PRI, 0);
        if (ATG_DCI_RC_OK != ret)
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetL3UniPri.\n"
                       "ret=%ld,slot=%d,port=%d,vlan=%d,mode=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,node->slot,node->port,
                       node->vid,PHB_TO_DSCP_PRI);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->out_mode = PHB_TO_DSCP_PRI;
        join_node->out_dsptr = 0;
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->p2vlan_cfg_cb) /*VLAN���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3UniPri(unit, 0, node, PHB_TO_PRI, dsptr);
        if (ATG_DCI_RC_OK != ret) /*����ӳ���ʧ��*/
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetL3UniPri.\n"
                       "ret=%ld,slot=%d,port=%d,vlan=%d,mode=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,node->slot,node->port,
                       node->vid,PHB_TO_PRI);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->out_mode = PHB_TO_PRI;
        join_node->out_dsptr = dsptr;
    }
    if (NULL != pcb->p2d_cfg_cb) /*DSCP���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3UniPri(unit, 0, node, PHB_TO_DSCP_PRI, dsptr);
        if (ATG_DCI_RC_OK != ret) /*����ӳ���ʧ��*/
        {
    	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetL3UniPri.\n"
                       "ret=%ld,slot=%d,port=%d,vlan=%d,mode=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,node->slot,node->port,
                       node->vid,PHB_TO_DSCP_PRI);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->out_mode = PHB_TO_DSCP_PRI;
        join_node->out_dsptr = dsptr;
    }
    if ((NULL == pcb->p2d_cfg_cb) && (NULL == pcb->p2vlan_cfg_cb))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR cfg is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_phb2pri_l2_ds_domain_univp
   ��������  : ds�����L2 univp��PHB��vlan���ȼ�����ӳ��
   �������  : ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_phb2pri_l2_ds_domain_univp(NBB_INT unit, 
    NBB_ULONG index, NBB_LONG vpid, SPM_QOS_DS_LOGIC_UNIVP_CB* join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_USHORT dsptr = index;

    NBB_TRC_ENTRY("spm_add_phb2pri_l2_ds_domain_univp");

    if (0 == vpid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vpid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ����Ĭ������ */
    if (0 == index)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL2UniNhiPri(unit, vpid, PHB_TO_PRI, 0);
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set ApiC3SetL2UniNhiPri fail.\n"
                       "ret=%ld,phb2pri_ds_mode=%d,unit=%d,vp_posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,PHB_TO_PRI,unit,vpid,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->out_mode = PHB_TO_PRI;
        join_node->out_dsptr = 0;
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->p2vlan_cfg_cb) /* VLAN���ȼ��������� */
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL2UniNhiPri(unit, vpid, PHB_TO_PRI, dsptr);
        if (ATG_DCI_RC_OK != ret) /* ����ӳ���ʧ�� */
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set ApiC3SetL2UniNhiPri fail.\n"
                       "ret=%ld,phb2pri_ds_mode=%d,unit=%d,vp_posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,PHB_TO_PRI,unit,vpid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->out_mode = PHB_TO_PRI;
        join_node->out_dsptr = dsptr;
    }
    else if (NULL != pcb->p2d_cfg_cb) /* DSCP���ȼ��������� */
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL2UniNhiPri(unit, vpid, PHB_TO_DSCP_PRI, dsptr);
        if (ATG_DCI_RC_OK != ret) /* ����ӳ���ʧ�� */
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set ApiC3SetL2UniNhiPri fail.\n"
                       "ret=%ld,phb2pri_ds_mode=%d,unit=%d,vp_posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,PHB_TO_DSCP_PRI,unit,vpid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        join_node->out_mode = PHB_TO_DSCP_PRI;
        join_node->out_dsptr = dsptr;
    }
    else
    {
	    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR univp ds support only phb-vlanpri.\n\n",
                   __FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    ret = spm_connect_ds_domain_cnt(index NBB_CCXT);
	if (ATG_DCI_RC_OK != ret)
	{
	 OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_connect_ds_domain_cnt.\n\n",
	            	    __FUNCTION__,__LINE__);
	 BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
	 ret = ATG_DCI_RC_UNSUCCESSFUL;
	 goto EXIT_LABEL;
	}

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_ds_l3_logic_intf
   ��������  : ����dsģ���L3 intf ���ȼ���PHB��������
   �������  : dsģ���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_ds_l3_logic_intf(SUB_PORT *sub_port,NBB_ULONG index, NBB_ULONG posid,
    TX_PORT_T *node,SPM_QOS_DS_LOGIC_INTF_CB *join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG pri2phb_ret = ATG_DCI_RC_OK;
    NBB_LONG phb2pri_ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pB = NULL;
#if defined (SPU) || defined (PTN690_CES)
    NBB_USHORT dsptr = 0;
#endif
    NBB_LONG unit = 0;
    NBB_BYTE fapid = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_add_ds_l3_logic_intf");

    /*DSģ������Ϊ0*/
    if ((0 == index) || (NULL == join_node))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds index can't be 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pB = spm_find_ds_cb(index NBB_CCXT);
    if (NULL == pB)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if ((NULL == pB->phb2pri_cfg_cb) && (NULL == pB->pri2phb_cfg_cb))
    {
	    OS_SPRINTF(ucMessage,"%s,%d,ds_index=%ld : QOS ERROR QOS ds cfg is NULL.\n\n",
                   __FUNCTION__,__LINE__,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    spm_hardw_getslot(&fapid);
    unit = sub_port->unit;
#if 1 

    /* phb2pri����Ϊ�ղ���Ĭ������ */
    if ((NULL == pB->phb2pri_cfg_cb) || (NULL == node))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds phb2pri_cfg_cb is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        phb2pri_ret = ATG_DCI_RC_UNSUCCESSFUL;
    }
    else  /* ���÷ǿ� */
    {
#if defined (SPU) || defined (PTN690_CES)
        if (PHB2PRI_DSCP == pB->phb2pri_cfg_cb->pri_select) /*ָ�����ȼ�*/
        {
            dsptr = pB->phb2pri_cfg_cb->dscp + PHB2PRI_ASSIGN_OFFSET;
            
         /**********************************************************************************
          * L3�»���DS,��ʱDS���ǰ���intf,���ǰ���VP��;һ���߼��ӿڿ��԰������intf, *
          * ��ȴֻ�ܶ�Ӧһ��VP,���VP��Ψһ��,L3ҵ���»����ڷ����DS��Ҳ��Ψһ�ġ�       *
          **********************************************************************************/
            phb2pri_ret = ApiC3SetL3UniPri(unit, 0, node, PHB_TO_DSCP_PRI, dsptr);
            join_node->out_mode = PHB_TO_DSCP_PRI;
            join_node->out_dsptr = dsptr;
        }
        else if (PHB2PRI_OVLAN == pB->phb2pri_cfg_cb->pri_select) /* ָ�����ȼ� */
        {
            dsptr = (pB->phb2pri_cfg_cb->ovlan_pri) + PHB2PRI_ASSIGN_OFFSET;
            phb2pri_ret = ApiC3SetL3UniPri(unit, 0, node, PHB_TO_PRI, dsptr);

            join_node->out_mode = PHB_TO_PRI;
            join_node->out_dsptr = dsptr;
        }
        else if (PHB2PRI_DSCP_AND_VLAN == pB->phb2pri_cfg_cb->pri_select) /* ָ�����ȼ� */
        {
            dsptr = (pB->phb2pri_cfg_cb->dscp) + PHB2PRI_ASSIGN_OFFSET;
            phb2pri_ret += ApiC3SetL3UniPri(unit, 0, node, PHB_TO_DSCP_PRI, dsptr);

            dsptr = pB->phb2pri_cfg_cb->ovlan_pri + PHB2PRI_ASSIGN_OFFSET;
            phb2pri_ret = ApiC3SetL3UniPri(unit, 0, node,PHB_TO_PRI, dsptr);

            join_node->out_mode = PHB_TO_DSCP_PRI;
            join_node->out_dsptr = dsptr;
        }
        else if (PHB2PRI_NOT == pB->phb2pri_cfg_cb->pri_select) /* ������������ */
        {
            phb2pri_ret = ApiC3SetL3UniPri(unit, 0, node,PRI_DSCP_NO_ACTION, dsptr);
            join_node->out_mode = PRI_DSCP_NO_ACTION;
            
        }
        else if (PHB2PRI_MAP == pB->phb2pri_cfg_cb->pri_select) /* ӳ�� */
        {
            phb2pri_ret = spm_add_phb2pri_l3_ds_domain_intf(unit,pB->phb2pri_cfg_cb->index,
                          node,join_node NBB_CCXT);
            if (ATG_DCI_RC_OK == phb2pri_ret)
            {
                ret = spm_connect_ds_domain_cnt(pB->phb2pri_cfg_cb->index NBB_CCXT);
                if (ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }
            } 
        }
        else  /* ���ò�ƥ�� */
        {
            phb2pri_ret = ATG_DCI_RC_UNSUCCESSFUL;
            OS_SPRINTF(ucMessage,"%s,%d,ds_index=%ld : QOS ERROR phb2pri ds mode miss match.\n\n",
                       __FUNCTION__,__LINE__,index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        }
        if (ATG_DCI_RC_OK != phb2pri_ret) /*����ʧ��*/
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set phb2pri faile.\n"
                       "phb2pri_ret=%ld,phb2pri_ds_mode=%d,slot=%d,port=%d,vlan=%d.\n\n",
                       __FUNCTION__,__LINE__,phb2pri_ret,pB->phb2pri_cfg_cb->pri_select,
                       sub_port->slot,sub_port->port,sub_port->ovlan);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = phb2pri_ret;
            goto EXIT_LABEL;
        }
#endif
    }
#endif
    if(sub_port->slot != fapid)/*�Ǳ���λ��ִ������*/
    {
        
    }
    else if ((NULL == pB->pri2phb_cfg_cb) || (0 == posid))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds pri2phb_cfg_cb is NULL.\n\n",
                   __FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        pri2phb_ret = ATG_DCI_RC_UNSUCCESSFUL;
        
    }
    else /*���÷ǿ�*/
    {
#if defined (SPU) || defined (PTN690_CES)
        if (PRI2PHB_ASSIGN == pB->pri2phb_cfg_cb->pri_select) /*ָ�����ȼ�*/
        {
            dsptr = (pB->pri2phb_cfg_cb->phb) * MAX_COLOR_NUM + pB->pri2phb_cfg_cb->color + PRI2PHB_ASSIGN_OFFSET;
            pri2phb_ret = ApiC3SetIntfPhb(unit, posid, DSCP_TO_PHB, dsptr);      
            join_node->in_mode = DSCP_TO_PHB;
            join_node->in_dsptr = dsptr;
        }
        else
        {
            pri2phb_ret = spm_add_pri2phb_l3_ds_domain_intf(unit,pB->pri2phb_cfg_cb->pri_select,
                          pB->pri2phb_cfg_cb->index, posid, join_node NBB_CCXT);
            if (ATG_DCI_RC_OK == pri2phb_ret)
            {
                ret = spm_connect_ds_domain_cnt(pB->pri2phb_cfg_cb->index NBB_CCXT);
                if (ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }
            }
        }

        /*���ô���*/
        if (ATG_DCI_RC_OK != pri2phb_ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set pri2phb faile.\n"
                       "pri2phb_ret=%ld,pri2phb_ds_mode=%d,posid=%ld,port=%d,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,pri2phb_ret,pB->pri2phb_cfg_cb->pri_select,
                       posid,sub_port->port,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = pri2phb_ret;
            goto EXIT_LABEL;
        }
#endif
    }

    /*DSû���κ�����*/
    
    //coverity[dead_error_condition]
    if ((ATG_DCI_RC_UNSUCCESSFUL == phb2pri_ret) && ((ATG_DCI_RC_UNSUCCESSFUL == pri2phb_ret)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(ucMessage,"%s,%d,ds_index=%ld,posid=%ld: QOS ERROR cfg mismatch.\n\n",
                   __FUNCTION__,__LINE__,index,sub_port->posid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    pB->cnt++;
    OS_MEMCPY(&(join_node->sub_port),sub_port,sizeof(SUB_PORT));
    join_node->posid = posid;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_ds_l3_logic_intf
   ��������  : ����ΪĬ�����ò�ɾ�����Ľڵ�
   �������  : �ڵ�ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_del_ds_l3_logic_intf(NBB_LONG unit,NBB_ULONG posid,
        TX_PORT_T *node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_LONG pri2phb_ret = ATG_DCI_RC_OK;
    NBB_LONG phb2pri_ret = ATG_DCI_RC_OK;
#endif
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_del_ds_l3_logic_intf");

    /*ָ��Ϊ��*/
    if ((NULL == node) && (0 == posid))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR node is NULL AND posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����ΪĬ��ֵ*/
    if (0 != posid)
    {
#if defined (SPU) || defined (PTN690_CES)
        pri2phb_ret = ApiC3SetIntfPhb(unit, posid, UNI_SET_PHB, 0);
        if (ATG_DCI_RC_OK != pri2phb_ret) /* ���ô��� */
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "pri2phb_ret=%ld,unit=%ld,mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,pri2phb_ret,unit,DSCP_TO_PHB,posid,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = pri2phb_ret;
            goto EXIT_LABEL;
        }
#endif
    }

#if 1
    if (NULL != node)
    {
#if defined (SPU) || defined (PTN690_CES)
        phb2pri_ret = ApiC3SetL3UniPri(unit, 0, node, PRI_DSCP_NO_ACTION, 0);
        if (ATG_DCI_RC_OK != phb2pri_ret) /* ���ô��� */
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetL3UniPri.\n"
                       "phb2pri_ret=%ld,slot=%d,port=%d,vlan=%d,mode=%d.\n\n",
                       __FUNCTION__,__LINE__,phb2pri_ret,node->slot,node->port,
                       node->vid,PHB_TO_DSCP_PRI);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = phb2pri_ret;
        }
#endif
    }
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_pri2phb_l2_ds_domain_flow
   ��������  : ds�����L2 flow ���ȼ���PHB����ӳ��
   �������  : ӳ��ģʽmod��ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_pri2phb_l2_ds_domain_flow(NBB_LONG unit,NBB_BYTE mod, 
    NBB_ULONG index, NBB_ULONG posid,SPM_QOS_DS_LOGIC_FLOW_CB * join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
#if defined (SPU) || defined (PTN690_CES)
    NBB_USHORT dsptr = index;
#endif
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_add_pri2phb_l2_ds_domain_flow");

    /* �������ָ�������Ч */
    NBB_ASSERT(0 != posid);

    /* ָ��Ϊ�� */
    if (0 == posid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����Ĭ������*/
    if (0 == index)
    {
#if defined (SPU) || defined (PTN690_CES)
        if(PRI2PHB_MAP_OVLAN == mod)
        {
            ret = ApiC3SetIntfPhb(unit, posid, PRI_TO_PHB, 0);
            join_node->in_mode = PRI_TO_PHB;
            join_node->in_dsptr = 0;
        }
        else if(PRI2PHB_MAP_DSCP == mod)
        {
            ret = ApiC3SetIntfPhb(unit, posid, DSCP_TO_PHB, 0);     
            join_node->in_mode = DSCP_TO_PHB;
            join_node->in_dsptr = 0;
        }
        else
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR INTF FLOW cfg miss match.\n"
                       "unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,unit,mod,posid,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "ret=%ld,unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,mod,posid,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        }
#endif 
        goto EXIT_LABEL;
    }

    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }
    if ((NULL != pcb->vlan2p_cfg_cb) && (PRI2PHB_MAP_OVLAN == mod)) /*VLAN���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetIntfPhb(unit, posid, PRI_TO_PHB, dsptr);
        join_node->in_mode = PRI_TO_PHB;
        join_node->in_dsptr = dsptr;
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "ret=%ld,unit=%ld,mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,0,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
    }
    else if ((NULL != pcb->d2p_cfg_cb) && (PRI2PHB_MAP_DSCP == mod)) /*DSCP���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetIntfPhb(unit, posid, DSCP_TO_PHB, dsptr);
        join_node->in_mode = DSCP_TO_PHB;
        join_node->in_dsptr = dsptr;
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                       "ret=%ld,unit=%ld,mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,1,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
    }
    else
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR cfg miss match.\n"
                   "unit=%ld,pri2phb_ds_mode=%d,posid=%ld,dsptr=%d.\n\n",
                   __FUNCTION__,__LINE__,unit,mod,posid,0);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_ds_l2_logic_flow
   ��������  : ����dsģ���L2 flow ���ȼ���PHB��������
   �������  : dsģ���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_ds_l2_logic_univp(NBB_INT  unit, NBB_ULONG index,
    SPM_QOS_DS_LOGIC_UNIVP_CB* join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pB = NULL;
    NBB_USHORT dsptr = 0;
    NBB_ULONG  vpid  = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_add_ds_l2_logic_univp");

    if ((NULL == join_node) || (0 == index))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR index is 0 OR join_node is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    vpid = join_node->vpuni.vp_posid;
    pB = spm_find_ds_cb(index NBB_CCXT);
    if (NULL == pB)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* pri2phb����Ϊ�ղ���Ĭ������ */
    if (NULL == pB->phb2pri_cfg_cb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds phb2pri_cfg_cb is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    else  /* ���÷ǿ� */
    {
        if (PHB2PRI_OVLAN == pB->phb2pri_cfg_cb->pri_select) /* ָ��VLAN���ȼ� */
        {
            dsptr = pB->phb2pri_cfg_cb->ovlan_pri + PHB2PRI_ASSIGN_OFFSET;
#if defined (SPU) || defined (PTN690_CES)

         /**********************************************************************************
          * L2�»���DS,��ʱDS���ǰ���intf,���ǰ���VP��;һ���߼��ӿڿ��԰������intf, *
          * ��ȴֻ�ܶ�Ӧһ��VP,���VP��Ψһ��,L2ҵ���»����ڷ����DS��Ҳ��Ψһ�ġ�       *
          **********************************************************************************/
            ret = ApiC3SetL2UniNhiPri(unit,vpid,PHB_TO_PRI,dsptr);
            if (ATG_DCI_RC_OK != ret) /*����ʧ��*/
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set ApiC3SetL2UniNhiPri fail.\n"
                           "ret=%ld,phb2pri_ds_mode=%d,unit=%d,vp_posid=%ld,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,PHB2PRI_OVLAN,unit,vpid,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
            join_node->out_mode  = PHB_TO_PRI;
            join_node->out_dsptr = dsptr;
        }
        else if (PHB2PRI_DSCP == pB->phb2pri_cfg_cb->pri_select) /* ָ��dscp���ȼ� */
        {
            dsptr = pB->phb2pri_cfg_cb->dscp + PHB2PRI_ASSIGN_OFFSET;
#if defined (SPU) || defined (PTN690_CES)

         /**********************************************************************************
          * L2�»���DS,��ʱDS���ǰ���intf,���ǰ���VP��;һ���߼��ӿڿ��԰������intf, *
          * ��ȴֻ�ܶ�Ӧһ��VP,���VP��Ψһ��,L2ҵ���»����ڷ����DS��Ҳ��Ψһ�ġ�       *
          **********************************************************************************/
            ret = ApiC3SetL2UniNhiPri(unit,vpid,PHB_TO_DSCP_PRI,dsptr);
            if (ATG_DCI_RC_OK != ret) /*����ʧ��*/
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set ApiC3SetL2UniNhiPri fail.\n"
                           "ret=%ld,phb2pri_ds_mode=%d,unit=%d,vp_posid=%ld,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,PHB_TO_DSCP_PRI,unit,vpid,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
            join_node->out_mode  = PHB_TO_DSCP_PRI;
            join_node->out_dsptr = dsptr;
        }
        else if (PHB2PRI_MAP == pB->phb2pri_cfg_cb->pri_select) /* ����ӳ�� */
        {
            ret = spm_add_phb2pri_l2_ds_domain_univp(unit,pB->phb2pri_cfg_cb->index,vpid,join_node NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        else if (PHB2PRI_NOT == pB->phb2pri_cfg_cb->pri_select) /* ������������ */
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL2UniNhiPri(unit,vpid,PRI_DSCP_NO_ACTION,0);
            if (ATG_DCI_RC_OK != ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set ApiC3SetL2UniNhiPri fail.\n"
                           "ret=%ld,phb2pri_ds_mode=%d,unit=%d,vp_posid=%ld,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,PHB2PRI_NOT,unit,vpid,0);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
            join_node->out_mode  = PRI_DSCP_NO_ACTION;
            join_node->out_dsptr = 0;
        }
        else  /* ���ò�ƥ�� */
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR univp only support phb-vlanpri.\n\n",
                       __FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        }
    }
    pB->cnt++;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_ds_l2_logic_flow
   ��������  : ����dsģ���L2 flow ���ȼ���PHB��������
   �������  : dsģ���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_ds_l2_logic_flow(SUB_PORT *sub_port,NBB_ULONG index,
    NBB_ULONG posid,SPM_QOS_DS_LOGIC_FLOW_CB * join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pB = NULL;
    NBB_USHORT dsptr = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_add_ds_l2_logic_flow");

    if ((0 == posid) || (0 == index))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid OR index is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pB = spm_find_ds_cb(index NBB_CCXT);
    if (NULL == pB)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* pri2phb����Ϊ�ղ���Ĭ������ */
    if (NULL == pB->pri2phb_cfg_cb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds pri2phb_cfg_cb is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    else  /* ���÷ǿ� */
    {
        if (PRI2PHB_ASSIGN == pB->pri2phb_cfg_cb->pri_select) /* ָ�����ȼ� */
        {
            dsptr = (pB->pri2phb_cfg_cb->phb) * MAX_COLOR_NUM + pB->pri2phb_cfg_cb->color + PRI2PHB_ASSIGN_OFFSET;
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetIntfPhb(sub_port->unit, posid, PRI_TO_PHB, dsptr);
            join_node->in_mode = PRI_TO_PHB;
            join_node->in_dsptr = dsptr;
            if (ATG_DCI_RC_OK != ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                           "ret=%ld,unit=%d,mode=%d,posid=%ld,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,sub_port->unit,0,posid,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
        else /*����ģʽ*/
        {
            ret = spm_add_pri2phb_l2_ds_domain_flow(sub_port->unit,pB->pri2phb_cfg_cb->pri_select,
                  pB->pri2phb_cfg_cb->index, posid,join_node NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            ret = spm_connect_ds_domain_cnt(pB->pri2phb_cfg_cb->index NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
    }
    pB->cnt++;
    OS_MEMCPY(&(join_node->sub_port),sub_port,sizeof(SUB_PORT));
    join_node->posid = posid;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_ds_l2_logic_flow
   ��������  : ����ΪĬ�����ò�ɾ�����Ľڵ�
   �������  : �ڵ�ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_del_ds_l2_logic_flow(NBB_LONG unit,NBB_ULONG posid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_del_ds_l2_logic_flow");

    if (0 == posid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����ΪĬ��ֵ*/
#if defined (SPU) || defined (PTN690_CES)
    ret = ApiC3SetIntfPhb(unit, posid, UNI_SET_PHB, 0);
    if (ATG_DCI_RC_OK != ret) /*���ô���*/
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetIntfPhb.\n"
                   "ret=%ld,unit=%ld,mode=%d,posid=%ld,dsptr=%d.\n\n",
                   __FUNCTION__,__LINE__,ret,unit,0,posid,0);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_ds_l2_logic_univp
   ��������  : ����ΪĬ�����ò�ɾ�����Ľڵ�
   �������  : �ڵ�ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_del_ds_l2_logic_univp(NBB_LONG unit,NBB_ULONG vpid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_del_ds_l2_logic_univp");

    if (0 == vpid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vpid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ɾ�����ò�����ΪĬ��ֵ */
#if defined (SPU) || defined (PTN690_CES)
    ret = ApiC3SetL2UniNhiPri(unit, vpid, PRI_DSCP_NO_ACTION, 0);
    if (ATG_DCI_RC_OK != ret) /*���ô���*/
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetL2UniNhiPri.\n"
                   "ret=%ld,unit=%ld,mode=%d,vp_posid=%ld,dsptr=%d.\n\n",
                   __FUNCTION__,__LINE__,ret,unit,PHB_TO_PRI,vpid,0);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
#endif
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_pri2phb_ds_domain_vc
   ��������  : ds�����vc ���ȼ���PHB����ӳ��
   �������  : ӳ��ģʽmod��ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_pri2phb_ds_domain_vc(NBB_BYTE mod, NBB_ULONG index, NBB_ULONG posid,
    NBB_USHORT flag,SPM_QOS_DS_VC_CB *join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
#if defined (SPU) || defined (PTN690_CES)    
    NBB_LONG unit = 0;
#endif
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_USHORT dsptr = index;

    NBB_TRC_ENTRY("spm_add_pri2phb_ds_domain_vc");
 
    if (0 == posid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (0 == index)
    {   
#if defined (SPU) || defined (PTN690_CES)
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            if (VC_MAIN == flag)
            {
                ret += ApiC3SetVpIngPhb(unit, posid, UNIFORM, dsptr);
            }
            else if (VC_BACK == flag)
            {
                ret += ApiC3SetVpIngBackupPhb(unit, posid, UNIFORM, dsptr);
            }
        }
        join_node->in_mode = UNIFORM;
        join_node->in_dsptr = dsptr;
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetVpIngPhb.\n"
                       "ret=%ld,flag=%d,mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,flag,0,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        }
#endif
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if ((NULL != pcb->exp2p_cfg_cb) && (PRI2PHB_MAP_EXP == mod)) /*EXP���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            if (VC_MAIN == flag)
            {
                ret += ApiC3SetVpIngPhb(unit, posid, UNIFORM, dsptr);
            }
            else if (VC_BACK == flag)
            {
                ret += ApiC3SetVpIngBackupPhb(unit, posid, UNIFORM, dsptr);
            }
        }
        join_node->in_mode = UNIFORM;
        join_node->in_dsptr = dsptr;
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetVpIngPhb.\n"
                       "ret=%ld,flag=%d,mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,flag,0,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
    }
    else if ((NULL != pcb->d2p_cfg_cb) && (PRI2PHB_MAP_DSCP == mod)) /*DSCP���ȼ���������*/
    {
#if defined (SPU) || defined (PTN690_CES)
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            if (VC_MAIN == flag)
            {
                ret += ApiC3SetVpIngPhb(unit, posid, SHORT_PIPE, dsptr);
            }
            else if (VC_BACK == flag)
            {
                ret += ApiC3SetVpIngBackupPhb(unit, posid, SHORT_PIPE, dsptr);
            }
        }
        join_node->in_mode = SHORT_PIPE;
        join_node->in_dsptr = dsptr;
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetVpIngPhb.\n"
                       "ret=%ld,flag=%d,mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,flag,1,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
    }
    else
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR no cfg.\n"
                   "flag=%d,pri2phb_mode=%d,posid=%ld,dsptr=%d.\n\n",
                   __FUNCTION__,__LINE__,flag,mod,posid,dsptr);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_phb2pri_ds_domain_vc
   ��������  : ds�����vc ��PHB�����ȼ�����ӳ��
   �������  : ӳ��ģʽmod,ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_phb2pri_ds_domain_vc(NBB_BYTE mod, NBB_ULONG index,
    NBB_ULONG posid, NBB_USHORT flag,SPM_QOS_DS_VC_CB *join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
#if defined (SPU) || defined (PTN690_CES) 
    NBB_LONG unit = 0;
#endif
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_USHORT dsptr = index;

    NBB_TRC_ENTRY("spm_add_phb2pri_ds_domain_vc");

    if (0 == posid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if ((0 != index) && (NULL == pcb))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if ((0 == index) || (NULL != pcb->p2exp_cfg_cb))
    {
#if defined (SPU) || defined (PTN690_CES)
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            if (VC_MAIN == flag)
            {
                //coverity[dead_error_condition]
                ret += ApiC3SetVpEgrExp(unit, posid, dsptr,PHB_TO_EXP);
            }
            else if (VC_BACK == flag)
            {
                //coverity[dead_error_condition]
                ret += ApiC3SetVpEgrBackupExp(unit, posid, dsptr,PHB_TO_EXP);
            }
        }
        join_node->out_mode = PHB_TO_EXP;
        join_node->out_dsptr = dsptr;
        if (ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetVpEgrExp.\n"
                       "ret=%ld,flag=%d,phb2pri_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,flag,mod,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
    }
    else
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR no cfg.\n"
                   "flag=%d,phb2pri_mode=%d,posid=%ld,dsptr=%d.\n\n",
                   __FUNCTION__,__LINE__,flag,mod,posid,dsptr);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_ds_vc
   ��������  : ����dsģ���L3 intf ���ȼ���PHB��������
   �������  : dsģ���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_ds_vc(NBB_ULONG index, NBB_ULONG posid, NBB_USHORT flag,
    SPM_QOS_DS_VC_CB *join_node NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG pri2phb_ret = ATG_DCI_RC_OK;
    NBB_LONG phb2pri_ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pB = NULL;
#if defined (SPU) || defined (PTN690_CES) 
    NBB_USHORT dsptr = 0;
    NBB_LONG unit = 0;
#endif
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_add_ds_vc");

    if ((0 == posid) || (0 == index))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid OR index is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pB = spm_find_ds_cb(index NBB_CCXT);
    if (NULL == pB)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* pri2phb����Ϊ�ղ���Ĭ������ */
    if (NULL == pB->pri2phb_cfg_cb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS WARNING ds pri2phb_cfg_cb is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        pri2phb_ret = ATG_DCI_RC_UNSUCCESSFUL;
    }
    else /*���÷ǿ�*/
    {
#if defined (SPU) || defined (PTN690_CES)
        if (PRI2PHB_ASSIGN == pB->pri2phb_cfg_cb->pri_select) /*ָ��PHB*/
        {
            dsptr = (pB->pri2phb_cfg_cb->phb) * MAX_COLOR_NUM + pB->pri2phb_cfg_cb->color + PRI2PHB_ASSIGN_OFFSET;
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if (VC_MAIN == flag)
                {
                    pri2phb_ret += ApiC3SetVpIngPhb(unit, posid, UNIFORM, dsptr);
                }
                else if (VC_BACK == flag)
                {
                    pri2phb_ret += ApiC3SetVpIngBackupPhb(unit, posid, UNIFORM, dsptr);
                }
            }
            join_node->in_mode = UNIFORM;
            join_node->in_dsptr = dsptr;
        }
        else /*����ģʽ*/
        {
            pri2phb_ret = spm_add_pri2phb_ds_domain_vc(pB->pri2phb_cfg_cb->pri_select,
                          pB->pri2phb_cfg_cb->index, posid,flag,join_node NBB_CCXT);
            if(ATG_DCI_RC_OK == pri2phb_ret)
            {
                ret = spm_connect_ds_domain_cnt(pB->pri2phb_cfg_cb->index NBB_CCXT);
                if (ATG_DCI_RC_UNSUCCESSFUL == ret)
                {
                    goto EXIT_LABEL;
                }
            }
        }
        if (ATG_DCI_RC_OK != pri2phb_ret)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR VC DS set pri2phb faile.\n"
                       "pri2phb_ret=%ld,flag=%d,pri2phb_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,pri2phb_ret,flag,pB->pri2phb_cfg_cb->pri_select,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#endif
    }

    /* phb2pri����Ϊ�ղ���Ĭ������ */
    if (NULL == pB->phb2pri_cfg_cb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds phb2pri_cfg_cb is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        phb2pri_ret = ATG_DCI_RC_UNSUCCESSFUL;
    }

    /* ���÷ǿ� */
    else
    {
#if defined (SPU) || defined (PTN690_CES)

        /* ָ�����ȼ� */
        if (PHB2PRI_EXP == pB->phb2pri_cfg_cb->pri_select)
        {
            dsptr = pB->phb2pri_cfg_cb->exp + PHB2PRI_ASSIGN_OFFSET;
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if (VC_MAIN == flag)
                {
                    phb2pri_ret += ApiC3SetVpEgrExp(unit, posid, dsptr,PHB_TO_EXP);
                }
                else if (VC_BACK == flag)
                {
                    phb2pri_ret += ApiC3SetVpEgrBackupExp(unit, posid, dsptr,PHB_TO_EXP);
                }
            }
            join_node->out_mode = PHB_TO_EXP;
            join_node->out_dsptr = dsptr;
            
        }

        /* ӳ��ģʽ */
        else if (PHB2PRI_MAP == pB->phb2pri_cfg_cb->pri_select) 
        {
            phb2pri_ret = spm_add_phb2pri_ds_domain_vc(pB->phb2pri_cfg_cb->pri_select,
                          pB->phb2pri_cfg_cb->index,posid,flag,join_node NBB_CCXT);
            if (ATG_DCI_RC_OK == phb2pri_ret)
            {
                ret = spm_connect_ds_domain_cnt(pB->phb2pri_cfg_cb->index NBB_CCXT);
                if (ATG_DCI_RC_UNSUCCESSFUL == ret)
                {
                    goto EXIT_LABEL;
                }
            }
        }

        /* ������ */
        else if (PHB2PRI_NOT == pB->phb2pri_cfg_cb->pri_select)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR phb2pri mod is PHB2PRI_NOT.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if (VC_MAIN == flag)
                {
                    phb2pri_ret += ApiC3SetVpEgrExp(unit, posid, dsptr,NNI_NO_ACTION);
                }
                else if (VC_BACK == flag)
                {
                    phb2pri_ret += ApiC3SetVpEgrBackupExp(unit, posid, dsptr,NNI_NO_ACTION);
                }
            }
            join_node->out_mode = NNI_NO_ACTION;
            join_node->out_dsptr = dsptr;
        }

        /* ����������� */
        else
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR phb2pri mod missmatch.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            phb2pri_ret = ATG_DCI_RC_UNSUCCESSFUL;
        }
        if (ATG_DCI_RC_OK != phb2pri_ret)
        {
            /* ɾ��֮ǰ������ */
            if (ATG_DCI_RC_OK == pri2phb_ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR set pri2phb ok but phb2pri failed.\n\n",__FUNCTION__,__LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            }
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR VC DS set phb2pri faile.\n"
                       "phb2pri_ret=%ld,flag=%d,phb2pri_mode=%d,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,phb2pri_ret,flag,pB->phb2pri_cfg_cb->pri_select,posid,dsptr);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = phb2pri_ret;
            goto EXIT_LABEL;
        }
#endif

    }

    /* DSû���κ����� */
    if ((ATG_DCI_RC_UNSUCCESSFUL == phb2pri_ret) && ((ATG_DCI_RC_UNSUCCESSFUL == pri2phb_ret)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR cfg mismatch.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }
    pB->cnt++;
    join_node->flag = flag;
    join_node->posid = posid;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_ds_vc
   ��������  : ����ΪĬ�����ò�ɾ�����Ľڵ�
   �������  : �ڵ�ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_del_ds_vc(NBB_ULONG posid, NBB_USHORT flag NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES) 
    NBB_LONG pri2phb_ret = ATG_DCI_RC_OK;
    NBB_LONG phb2pri_ret = ATG_DCI_RC_OK;
#endif
    NBB_LONG unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_del_ds_vc");

    if (0 == posid)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR posid is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for(unit = 0; unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        if (VC_MAIN == flag)
        {
            pri2phb_ret = ApiC3SetVpIngPhb(unit, posid, UNIFORM, 0);
        }
        else if (VC_BACK == flag)
        {
            pri2phb_ret = ApiC3SetVpIngBackupPhb(unit, posid, UNIFORM, 0);
        }
        if (ATG_DCI_RC_OK != pri2phb_ret) /*���ô���*/
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetVpIngPhb.\n"
                       "pri2phb_ret=%ld,flag=%d,unit=%ld,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,pri2phb_ret,flag,unit,posid,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = pri2phb_ret;
            goto EXIT_LABEL;
        }
#endif
    }
    
    for(unit = 0; unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES) 
         if (VC_MAIN == flag)
        {
            phb2pri_ret = ApiC3SetVpEgrExp(unit, posid, 0,NNI_NO_ACTION);
        }
        else if (VC_BACK == flag)
        {
            phb2pri_ret = ApiC3SetVpEgrBackupExp(unit, posid, 0,NNI_NO_ACTION);
        }
        if (ATG_DCI_RC_OK != phb2pri_ret) /*���ô���*/
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetVpEgrExp.\n"
                       "phb2pri_ret=%ld,flag=%d,unit=%ld,posid=%ld,dsptr=%d.\n\n",
                       __FUNCTION__,__LINE__,phb2pri_ret,flag,unit,posid,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = phb2pri_ret;
            goto EXIT_LABEL;
        }
#endif
    }
    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_phb2pri_ds_domain_vrf
   ��������  : ds�����vrf  ��PHB�����ȼ�����ӳ��
   �������  : ӳ��ģʽmod,ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_phb2pri_ds_domain_vrf(NBB_ULONG index, NBB_ULONG nhiId NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_LONG unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_USHORT dsptr = index;

    NBB_TRC_ENTRY("spm_add_phb2pri_ds_domain_vrf");

    if (0 == nhiId)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR nhiId is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if ((0 != index) && (NULL == pcb))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't find ds domain cfg.\n"
                   "ret=%ld,ds_domain_index=%ld.\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

 /********************************************************************************* 
  * 1.ds����phb-exp��ӳ������p2exp_cfg_cbΪ����ds���������index��Ϊ��ʱ,����;    *
  * 2.ds����phb-exp��ӳ������p2exp_cfg_cb��Ϊ��,��ʹ���û�����ds��,������Ϊindex; *
  * 3.ds����phb-exp��ӳ������p2exp_cfg_cbΪ��,��ͬʱds�������indexΪ��,��ʱʹ��  *
  *   Ĭ�ϵ�phb-expӳ����,��ds�������indexΪ0.                                   *
  *********************************************************************************/
    if ((0 == index) || (NULL != pcb->p2exp_cfg_cb))
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetTxpwExp(unit, nhiId, dsptr, PHB_TO_EXP);
            if (ATG_DCI_RC_OK != ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetTxpwExp.\n"
                           "ret=%ld,unit=%ld,nhiId=%ld,dsptr=%d,ds_index=%ld.\n\n",
                           __FUNCTION__,__LINE__,ret,unit,nhiId,dsptr,index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
        
    }
    else
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR VRF DS no cfg.\n"
                   "unit=%ld,nhiId=%ld,dsptr=%d,ds_index=%ld.\n\n",
                   __FUNCTION__,__LINE__,unit,nhiId,dsptr,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_ds_vrf
   ��������  : ����dsģ���vrf ���ȼ���PHB��������
   �������  : dsģ���index  ,�����ṹ��,flag:���ü�����ʶ
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_ds_vrf(NBB_ULONG index, NBB_ULONG nhiId,
    SPM_QOS_DS_VRF_CB *join_node, NBB_ULONG flag NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_CB *pB = NULL;
    NBB_LONG unit = 0;
    NBB_USHORT dsptr = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((0 == nhiId) || ((0 == index)))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR nhiId OR index is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pB = spm_find_ds_cb(index NBB_CCXT);
    if (NULL == pB)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* phb2pri_cfg_cbΪ�����˳� */
    if (NULL == pB->phb2pri_cfg_cb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds phb2pri_cfg_cb is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    else
    {
        /* �û�ָ�����ȼ�PHB-EXP */
        if (PHB2PRI_EXP == pB->phb2pri_cfg_cb->pri_select)
        {
            /* ָ�����ȼ�ʱ,dsptr��ָ���expֵ�й� */
            dsptr = pB->phb2pri_cfg_cb->exp + PHB2PRI_ASSIGN_OFFSET;
            for(unit = 0; unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetTxpwExp(unit, nhiId, dsptr,PHB_TO_EXP);
                if (ATG_DCI_RC_OK != ret)
                {
                    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetTxpwExp.\n"
                               "ret=%ld,phb2pri_mode=%d,nhiId=%ld,dsptr=%d,ds_index=%ld.\n\n",
                               __FUNCTION__,__LINE__,ret,pB->phb2pri_cfg_cb->pri_select,
                               nhiId,dsptr,index);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                    goto EXIT_LABEL;
                }
#endif
            }
            join_node->dsptr = dsptr;
            join_node->out_mode = PHB_TO_EXP;
            
        }

        /* ������ */
        else if (PHB2PRI_NOT == pB->phb2pri_cfg_cb->pri_select)
        {
            for(unit = 0; unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetTxpwExp(unit, nhiId,0,NNI_NO_ACTION); /*�Ž�ҵ��̳�VP��exp*/
                if (ATG_DCI_RC_OK != ret)
                {
                    OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetTxpwExp.\n"
                               "ret=%ld,phb2pri_mode=%d,nhiId=%ld,dsptr=%d,ds_index=%ld.\n\n",
                               __FUNCTION__,__LINE__,ret,pB->phb2pri_cfg_cb->pri_select,
                               nhiId,0,index);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                    goto EXIT_LABEL;
                }
#endif
            }
            join_node->dsptr = 0;
            join_node->out_mode = NNI_NO_ACTION;
            
        }

        /* ����ӳ�� */
        else if (PHB2PRI_MAP == pB->phb2pri_cfg_cb->pri_select)
        {
            ret = spm_add_phb2pri_ds_domain_vrf(pB->phb2pri_cfg_cb->index,nhiId NBB_CCXT);

            /* DSģ����ӳ���DS�������ֵ */
            join_node->dsptr = pB->phb2pri_cfg_cb->index;
            join_node->out_mode = PHB_TO_EXP;
            if (ATG_DCI_RC_OK != ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR spm_add_phb2pri_ds_domain_vrf.\n\n",__FUNCTION__,__LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }

         /************************************************************** 
          * ֻ�е�dsģ���ǻ���ӳ���,��ӳ���ds������index��Ϊ��ʱ,��  *   
          * ���ø�dsģ��ʱ�Ż��dsģ�����ӳ���ds��index�������ü���. *
          **************************************************************/
            if(ATG_DCI_RC_OK != flag)
            {
                ret = spm_connect_ds_domain_cnt(pB->phb2pri_cfg_cb->index NBB_CCXT);
                if (ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }
            }
        }
        else
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR VRF DS config missmatch.\n"
                       "phb2pri_mode=%d,nhiId=%ld,dsptr=%d,ds_index=%ld.\n\n",
                       __FUNCTION__,__LINE__,pB->phb2pri_cfg_cb->pri_select,
                       nhiId,dsptr,index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }

    /* ��DSģ��������ü��� */
    if(ATG_DCI_RC_OK != flag)
    {
        pB->cnt++;
    }
    join_node->nhi = nhiId;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_ds_vrf
   ��������  : ����ΪĬ�����ò�ɾ�����Ľڵ�
   �������  : �ڵ�ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_del_ds_vrf(NBB_ULONG nhi NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_del_ds_vrf");

    if (0 == nhi)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR nhiId is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ɾ��vrf��txpw�ϰ󶨵�dsģ�� */
    for(unit = 0; unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetTxpwExp(unit, nhi, 0,NNI_NO_ACTION);
        if ((ATG_DCI_RC_OK != ret) && (API_C3_E_NOT_FOUND != ret))
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ApiC3SetTxpwExp.\n"
                       "ret=%ld,unit=%ld,nhiId=%ld,dsptr=%d,ds_index=%d.\n\n",
                       __FUNCTION__,__LINE__,ret,unit,nhi,0,0);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif
        ret = ATG_DCI_RC_OK;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_clear_ds_vrf_mem(SPM_QOS_VRF_INSTANSE_KEY *key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG flag = ATG_DCI_RC_OK; 
    SPM_QOS_DS_VRF_CB *cfg_cb = NULL;
    SPM_QOS_DS_VRF_CB *next_cfg_cb = NULL;
    
    //AVLL_NODE node = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_clear_ds_vrf_mem");

    if (NULL == key)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR key is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ����DS���vrf�� */
    cfg_cb = (SPM_QOS_DS_VRF_CB*) AVLL_FIRST(v_spm_shared->qos_ds_vrf_tree);
    while (cfg_cb != NULL)
    {
        next_cfg_cb = (SPM_QOS_DS_VRF_CB*)AVLL_NEXT(v_spm_shared->qos_ds_vrf_tree,cfg_cb->spm_qos_vrf_node);
        if((cfg_cb->vrf_key.vrf_id == key->vrf_id) && (cfg_cb->vrf_key.peer_ip == key->peer_ip))
        {
            flag = ATG_DCI_RC_UNSUCCESSFUL;
            ret = spm_disconnect_phb2pri_ds_cnt(cfg_cb->ds_index NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            AVLL_DELETE(v_spm_shared->qos_ds_vrf_tree, cfg_cb->spm_qos_vrf_node);
            spm_free_ds_vrf_cb(cfg_cb NBB_CCXT); 
        }
        cfg_cb = next_cfg_cb;
    }
    if(ATG_DCI_RC_OK == flag)/*���û���ҵ���������������*/
    {
        ret = ATG_DCI_RC_OK;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_add_phb2pri_vrf_ds_domain_intf
   ��������  : ds�����L3 intf ��PHB�����ȼ�����ӳ��
   �������  : ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_add_phb2pri_vrf_ds_domain_intf(NBB_ULONG index,
    NBB_ULONG nhiId NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_LONG unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
#if defined (SPU) || defined (PTN690_CES)
    NBB_USHORT dsptr = index;
#endif

    NBB_TRC_ENTRY("spm_add_phb2pri_vrf_ds_domain_intf");

    if (0 == nhiId)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR nhiId is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����Ĭ������*/
    if (0 == index)
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, 0);
            if (ATG_DCI_RC_OK != ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS ApiC3SetL3UniPri.\n"
                           "ret=%ld,nhiId=%ld,mode=%d,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,nhiId,2,0);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            }
#endif
        }
        
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->p2vlan_cfg_cb) /*VLAN���ȼ���������*/
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_PRI, dsptr);
            if (ATG_DCI_RC_OK != ret) /*����ӳ���ʧ��*/
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS ApiC3SetL3UniPri.\n"
                           "ret=%ld,nhiId=%ld,mode=%d,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,nhiId,PHB_TO_PRI,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
        
    }
    if (NULL != pcb->p2d_cfg_cb) /*DSCP���ȼ���������*/
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, dsptr);
            if (ATG_DCI_RC_OK != ret) /*����ӳ���ʧ��*/
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS ApiC3SetL3UniPri.\n"
                           "ret=%ld,nhiId=%ld,mode=%d,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,nhiId,PHB_TO_DSCP_PRI,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
        
    }
    if ((NULL == pcb->p2d_cfg_cb) && (NULL == pcb->p2vlan_cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(ucMessage,"%s,%d,nhiId=%ld : QOS ERROR vrf nhi DS cfg is null.\n\n",
                   __FUNCTION__,__LINE__,nhiId);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_phb2pri_vrf_ds_domain_intf
   ��������  : ds�����L3 intf ��PHB�����ȼ�����ӳ��
   �������  : ds���index  ,�����ṹ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_del_phb2pri_vrf_ds_domain_intf(NBB_ULONG index,
    NBB_ULONG nhiId NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_LONG unit = 0;
#if defined (SPU) || defined (PTN690_CES)
    NBB_USHORT dsptr = index;
#endif
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_del_phb2pri_vrf_ds_domain_intf");

    if (0 == nhiId)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR nhiId is 0.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ����Ĭ������ */
    if (0 == index)
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, 0);
            if (ATG_DCI_RC_OK != ret)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS ApiC3SetL3UniPri.\n"
                           "ret=%ld,nhiId=%ld,mode=%d,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,nhiId,PHB_TO_DSCP_PRI,0);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            }
#endif
        }
        
        goto EXIT_LABEL;
    }
    pcb = spm_find_ds_domain_cb(index NBB_CCXT);
    if (NULL == pcb)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS spm_find_ds_domain_cb.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (NULL != pcb->p2vlan_cfg_cb) /*VLAN���ȼ���������*/
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_PRI, dsptr);
            if (ATG_DCI_RC_OK != ret) /*����ӳ���ʧ��*/
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS ApiC3SetL3UniPri.\n"
                           "ret=%ld,nhiId=%ld,mode=%d,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,nhiId,PHB_TO_PRI,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
        
    }
    if (NULL != pcb->p2d_cfg_cb) /*DSCP���ȼ���������*/
    {
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, dsptr);
            if (ATG_DCI_RC_OK != ret) /*����ӳ���ʧ��*/
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR vrf nhi DS ApiC3SetL3UniPri.\n"
                           "ret=%ld,nhiId=%ld,mode=%d,dsptr=%d.\n\n",
                           __FUNCTION__,__LINE__,ret,nhiId,PHB_TO_DSCP_PRI,dsptr);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                goto EXIT_LABEL;
            }
#endif
        }
        
    }
    if ((NULL == pcb->p2d_cfg_cb) && (NULL == pcb->p2vlan_cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR QOS vrf nhi DS cfg is null.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 5

/*****************************************************************************
   �� �� ��  : spm_alloc_ds_cb
   ��������  : ����DSģ���ڴ�
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_DS_CB *spm_alloc_ds_cb(NBB_ULONG ulkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_CB *pstTbl = NULL;

    NBB_TRC_ENTRY("spm_alloc_ds_cb");

    /* ����һ���µ�DS��������Ŀ��*/
    pstTbl = (SPM_DS_CB *)NBB_MM_ALLOC(sizeof(SPM_DS_CB),
              NBB_NORETRY_ACT, MEM_SPM_DS_CB);

    /*�����ڴ�ʧ��*/
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstTbl, 0, sizeof(SPM_DS_CB));
    pstTbl->ds_key = ulkey;
    AVLL_INIT_NODE(pstTbl->spm_ds_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_free_ds_cb
   ��������  : ���ͷ�DSģ���ڴ�
   �������  : DSģ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_cb(SPM_DS_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS free DS",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_DS_CB), MEM_SPM_DS_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/


    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (NULL != pstTbl->pri2phb_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->pri2phb_cfg_cb, MEM_SPM_DS_PRI2PHB_CB);
        pstTbl->pri2phb_cfg_cb = NULL;
    }
    if (NULL != pstTbl->phb2pri_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->phb2pri_cfg_cb, MEM_SPM_DS_PHB2PRI_CB);
        pstTbl->phb2pri_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_DS_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_alloc_ds_domain_cb
   ��������  : ����DS���ڴ�
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_DS_DOMAIN_CB *spm_alloc_ds_domain_cb(NBB_ULONG ulkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_DOMAIN_CB *pstTbl = NULL;

    NBB_TRC_ENTRY("spm_alloc_ds_domain_cb");

    /* ����һ���µ�DS_DOMAIN��������Ŀ��*/
    pstTbl = (SPM_DS_DOMAIN_CB *)NBB_MM_ALLOC(sizeof(SPM_DS_DOMAIN_CB),
              NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_CB);

    /*�����ڴ�ʧ��*/
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }
    OS_MEMSET(pstTbl, 0, sizeof(SPM_DS_DOMAIN_CB));
    pstTbl->ds_domain_key = ulkey;
    AVLL_INIT_NODE(pstTbl->spm_ds_domain_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_alloc_ds_domain_cb
   ��������  : �ͷ�DS���ڴ�
   �������  : DS��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_free_ds_domain_cb(SPM_DS_DOMAIN_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_free_ds_domain_cb");

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pstTbl is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS free DS domain",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_DS_DOMAIN_CB), MEM_SPM_DS_DOMAIN_CB);

    /***************************************************************************/
    /* �ͷ�dscp2phb�������ݿ顣                                                */
    /***************************************************************************/
    if (NULL != pstTbl->d2p_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->d2p_cfg_cb, MEM_SPM_DS_DOMAIN_D2P_CB);
        pstTbl->d2p_cfg_cb = NULL;
    }
    if (NULL != pstTbl->vlan2p_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->vlan2p_cfg_cb, MEM_SPM_DS_DOMAIN_VLAN2P_CB);
        pstTbl->vlan2p_cfg_cb = NULL;
    }
    if (NULL != pstTbl->exp2p_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->exp2p_cfg_cb, MEM_SPM_DS_DOMAIN_EXP2P_CB);
        pstTbl->exp2p_cfg_cb = NULL;
    }
    if (NULL != pstTbl->p2exp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->p2exp_cfg_cb, MEM_SPM_DS_DOMAIN_P2EXP_CB);
        pstTbl->p2exp_cfg_cb = NULL;
    }
    if (NULL != pstTbl->p2vlan_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->p2vlan_cfg_cb, MEM_SPM_DS_DOMAIN_P2VLAN_CB);
        pstTbl->p2vlan_cfg_cb = NULL;
    }
    if (NULL != pstTbl->p2d_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->p2d_cfg_cb, MEM_SPM_DS_DOMAIN_P2D_CB);
        pstTbl->p2d_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_DS_DOMAIN_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_domain_d2p_cfg
   ��������  : ds��DSCP��PHBӳ�����ý��պ���
   �������  : ��TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_domain_d2p_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,
    ATG_DCI_DS_DOMAIN_DSCP2PHBMAP **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_domain_d2p_cfg");

    /*ips��Ϣ�Ķ���*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,LINE=%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_set_default_dscp2phb_map(dsptr NBB_CCXT);
        NBB_MM_FREE(*pcb,MEM_SPM_DS_DOMAIN_D2P_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*���û����������*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*������*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *)buf;


            /***************************************************************************/
            /*�����������ú���                                            */
            /***************************************************************************/
            ret = spm_set_pri2phb_dscp_map(dsptr, ptemp->dscp_map NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DS_DOMAIN_DSCP2PHBMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_D2P_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DS_DOMAIN_DSCP2PHBMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_domain_vlan2p_cfg
   ��������  : ds��vlan��PHBӳ�����ý��պ���
   �������  : ��TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_domain_vlan2p_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,
    ATG_DCI_DS_DOMAIN_V8021P2PHBMAP **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_domain_vlan2p_cfg");

    /*ips��Ϣ�Ķ���*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,LINE=%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_set_default_vlan2phb_map(dsptr NBB_CCXT);
        NBB_MM_FREE(*pcb,MEM_SPM_DS_DOMAIN_VLAN2P_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*���û����������*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*������*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *)buf;


            /***************************************************************************/
            /*�����������ú���                                            */
            /***************************************************************************/
            ret = spm_set_pri2phb_vlan_map(dsptr, ptemp->v8021p_map NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DS_DOMAIN_V8021P2PHBMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_VLAN2P_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DS_DOMAIN_V8021P2PHBMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_domain_exp2p_cfg
   ��������  : ds��exp��PHBӳ�����ý��պ���
   �������  : ��TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_domain_exp2p_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,
    ATG_DCI_DS_DOMAIN_EXP2PHBMAP **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_EXP2PHBMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_domain_exp2p_cfg");

    /*ips��Ϣ�Ķ���*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,LINE=%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_set_default_exp2phb_map(dsptr NBB_CCXT);
        NBB_MM_FREE(*pcb,MEM_SPM_DS_DOMAIN_EXP2P_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*���û����������*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*������*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_EXP2PHBMAP *)buf;


            /***************************************************************************/
            /*�����������ú���                                            */
            /***************************************************************************/
            ret = spm_set_pri2phb_exp_map(dsptr, ptemp->exp_map NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DS_DOMAIN_EXP2PHBMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DS_DOMAIN_EXP2PHBMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_EXP2P_CB);
            }
            if(NULL != (*pcb))
            {
                OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DS_DOMAIN_EXP2PHBMAP));
            }
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_domain_p2d_cfg
   ��������  : ds��PHB��dscpӳ�����ý��պ���
   �������  : ds���ֵ����TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_domain_p2d_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,
    ATG_DCI_DS_DOMAIN_PHB2DSCPMAP **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_domain_p2d_cfg");

    /*ips��Ϣ�Ķ���*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,LINE=%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_set_default_phb2dscp_map(dsptr NBB_CCXT);
        NBB_MM_FREE(*pcb,MEM_SPM_DS_DOMAIN_P2D_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*���û����������*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*������*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *)buf;


            /***************************************************************************/
            /*�����������ú���                                            */
            /***************************************************************************/
            ret = spm_set_phb2pri_dscp_map(dsptr, ptemp->phb_map_dscp NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DS_DOMAIN_PHB2DSCPMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_P2D_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DS_DOMAIN_PHB2DSCPMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_domain_p2vlan_cfg
   ��������  : ds��PHB��vlanӳ�����ý��պ���
   �������  : ds���ֵ����TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_domain_p2vlan_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,
    ATG_DCI_DS_DOMAIN_PHB2V8021PMAP **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_domain_p2vlan_cfg");

    /*ips��Ϣ�Ķ���*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,LINE=%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_set_default_phb2dscp_map(dsptr NBB_CCXT);
        NBB_MM_FREE(*pcb,MEM_SPM_DS_DOMAIN_P2VLAN_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*���û����������*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*������*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *)buf;


            /***************************************************************************/
            /*�����������ú���                                            */
            /***************************************************************************/
            ret = spm_set_phb2pri_vlan_map(dsptr, ptemp->phb_map_8021p NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DS_DOMAIN_PHB2V8021PMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_P2VLAN_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DS_DOMAIN_PHB2V8021PMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_domain_p2exp_cfg
   ��������  : ds��PHB��EXPӳ�����ý��պ���
   �������  : ds���ֵ����TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_domain_p2exp_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,
    ATG_DCI_DS_DOMAIN_PHB2EXPMAP **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_PHB2EXPMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_domain_p2exp_cfg");

    /*ips��Ϣ�Ķ���*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,LINE=%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_set_default_phb2exp_map(dsptr NBB_CCXT);
        NBB_MM_FREE(*pcb,MEM_SPM_DS_DOMAIN_P2EXP_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*���û����������*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*������*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_PHB2EXPMAP *)buf;


            /***************************************************************************/
            /*�����������ú���                                            */
            /***************************************************************************/
            ret = spm_set_phb2pri_exp_map(dsptr, ptemp->phb_map_exp NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DS_DOMAIN_PHB2EXPMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DS_DOMAIN_PHB2EXPMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_DOMAIN_P2EXP_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DS_DOMAIN_PHB2EXPMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_rcv_dci_set_ds_domain
   ��������  : DS����պ���
   �������  : DS��IPS��Ϣָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_ds_domain(ATG_DCI_SET_DS_DOMAIN *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulDsDomainkey = 0;
    SPM_DS_DOMAIN_CB *pstDsDomainCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    /* IPS��Ϣ������ƫ�Ƶ�ַ */
    NBB_BYTE *pucD2pDataStart = NULL;
    NBB_BYTE *pucVlan2pDataStart = NULL;
    NBB_BYTE *pucExp2pDataStart = NULL;
    NBB_BYTE *pucP2dscpDataStart = NULL;
    NBB_BYTE *pucP2vlanDataStart = NULL;
    NBB_BYTE *pucP2expDataStart = NULL;

    /*IPS��Ϣ�����ò���*/
    NBB_ULONG ulOperDscp2Phb = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperVlan2Phb = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperExp2Phb = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPhb2Dscp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPhb2vlan = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPhb2exp = ATG_DCI_OPER_NULL;

    /*�߼���������*/
    NBB_TRC_ENTRY("spm_rcv_dci_set_ds_domain");

    if(NULL == pstSetIps)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pstSetIps is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }
    if ((0 == pstSetIps->key) || (pstSetIps->key > 15))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR param errorL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL*/
    pstSetIps->return_code = ATG_DCI_RC_OK;
    ulDsDomainkey = pstSetIps->key;
    if ((0 == pstSetIps->key) || (pstSetIps->key > 16)) /*���ܳ�����Χ*/
    {
        OS_SPRINTF(ucMessage,"%s,%d,ulDsDomainkey=%ld : QOS ERROR ulDsDomainkey out of range.\n\n",
                   __FUNCTION__,__LINE__,ulDsDomainkey);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pstDsDomainCb = AVLL_FIND(SHARED.ds_domain_tree, &ulDsDomainkey);
    if (NULL == pstDsDomainCb)
    {
        ucIfExist = QOS_UNEXIST;
    }

    /* ��ȡ�����ò��� */
    ulOperDscp2Phb = pstSetIps->oper_dscp2PhbMap;
    ulOperVlan2Phb = pstSetIps->oper_v8021p2PhbMap;
    ulOperExp2Phb = pstSetIps->oper_exp2PhbMap;
    ulOperPhb2Dscp = pstSetIps->oper_phb2DscpMap;
    ulOperPhb2vlan = pstSetIps->oper_phb2V8021pMap;
    ulOperPhb2exp = pstSetIps->oper_phb2ExpMap;

    /* ips��Ϣ����������ƫ��*/
    pucD2pDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &(pstSetIps->dscp2PhbMap_data));
    pucVlan2pDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &(pstSetIps->v8021p2PhbMap_data));
    pucExp2pDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &(pstSetIps->exp2PhbMap_data));
    pucP2dscpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &(pstSetIps->phb2DscpMap_data));
    pucP2vlanDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &(pstSetIps->phb2V8021pMap_data));
    pucP2expDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &(pstSetIps->phb2ExpMap_data));

    /*ɾ������*/
    if (TRUE == pstSetIps->delete_struct)
    {
        /*OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't del ds_domain.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
        goto EXIT_LABEL;*/

//#if 1

        /* �����Ŀ�����ڣ���ɾ�� */
        
        //coverity[unreachable]
        if (NULL == pstDsDomainCb)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ds_domain key %ld is not in use.\n\n",
                       __FUNCTION__,__LINE__,ulDsDomainkey);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }
        
        /* ����,ɾ�� */
        else
        {
            OS_SPRINTF(ucMessage,"%s,%d : delete ds_domain key is %ld.\n\n",
                       __FUNCTION__,__LINE__,ulDsDomainkey);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            if(NULL == pstDsDomainCb)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR del ds_domain ret=%ld.\n\n",
                           __FUNCTION__,__LINE__,ret);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL; 
            }
            if (0 != pstDsDomainCb->cnt)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't del ds_domain ret=%ld.\n\n",
                           __FUNCTION__,__LINE__,ret);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            AVLL_DELETE(SHARED.ds_domain_tree, pstDsDomainCb->spm_ds_domain_node);
            spm_free_ds_domain_cb(pstDsDomainCb NBB_CCXT);
        }
        
//#endif
    }

    /*����/���²���*/
    else
    {
        OS_SPRINTF(ucMessage,"%s,%d : add/update DS_DOMAIN key=%ld.\n\n",
                   __FUNCTION__,__LINE__,ulDsDomainkey);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);

        /* �����Ŀ������,�������ڴ�ռ䱣������ */
        if (QOS_UNEXIST == ucIfExist)
        {
            /* ����һ������Ŀ���ڴ�ռ� */
            pstDsDomainCb = spm_alloc_ds_domain_cb(ulDsDomainkey NBB_CCXT);
            if (NULL == pstDsDomainCb)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR create DS_DOMAIN key=%ld fail.\n\n",
                           __FUNCTION__,__LINE__,ulDsDomainkey);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

        /***************************************************************************/
        /*                         DSCP to Priӳ��                                 */
        /***************************************************************************/
        
        //coverity[dead_error_condition]
        /*if(NULL == pstDsDomainCb)
        {
            goto EXIT_LABEL;
        }*/
        
        /*�����ô�������*/
        ret = spm_set_ds_domain_d2p_cfg(ulDsDomainkey,ulOperDscp2Phb,
                &(pstDsDomainCb->d2p_cfg_cb),pucD2pDataStart NBB_CCXT);/*lint !e613 */

        /*������IPS��Ϣ����ֵ*/
        pstSetIps->dscp2PhbMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                         VLAN to PHBӳ��                        */
        /***************************************************************************/
        ret = spm_set_ds_domain_vlan2p_cfg(ulDsDomainkey,ulOperVlan2Phb,
                &(pstDsDomainCb->vlan2p_cfg_cb),pucVlan2pDataStart NBB_CCXT);/*lint !e613 */

        /*������IPS��Ϣ����ֵ*/
        pstSetIps->v8021p2PhbMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                         EXP to PHBӳ��                        */
        /***************************************************************************/
        ret = spm_set_ds_domain_exp2p_cfg(ulDsDomainkey,ulOperExp2Phb,
                &(pstDsDomainCb->exp2p_cfg_cb),pucExp2pDataStart NBB_CCXT);/*lint !e613 */

        /*������IPS��Ϣ����ֵ*/
        pstSetIps->exp2PhbMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                        phb��dscpӳ��                        */
        /***************************************************************************/
        ret = spm_set_ds_domain_p2d_cfg(ulDsDomainkey,ulOperPhb2Dscp,
                &(pstDsDomainCb->p2d_cfg_cb),pucP2dscpDataStart NBB_CCXT);/*lint !e613 */

        /*������IPS��Ϣ����ֵ*/
        pstSetIps->phb2DscpMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                        phb��vlanӳ��                        */
        /***************************************************************************/
        ret = spm_set_ds_domain_p2vlan_cfg(ulDsDomainkey,ulOperPhb2vlan,
                &(pstDsDomainCb->p2vlan_cfg_cb),pucP2vlanDataStart NBB_CCXT);/*lint !e613 */

        /*������IPS��Ϣ����ֵ*/
        pstSetIps->phb2V8021pMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                        phb��expӳ��                        */
        /***************************************************************************/
        ret = spm_set_ds_domain_p2exp_cfg(ulDsDomainkey,ulOperPhb2exp,
                &(pstDsDomainCb->p2exp_cfg_cb),pucP2expDataStart NBB_CCXT);/*lint !e613 */

        /*������IPS��Ϣ����ֵ*/
        pstSetIps->phb2ExpMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }
    }
    pstSetIps->return_code = ips_ret;

    /* �������������Ŀ,���뵽���� */
    if (QOS_UNEXIST == ucIfExist)
    {
        rv = AVLL_INSERT(SHARED.ds_domain_tree, pstDsDomainCb->spm_ds_domain_node);
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_domain(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_DOMAIN_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_DS_DOMAIN_CB*) AVLL_FIRST(v_spm_shared->ds_domain_tree); cfg_cb != NULL;
        cfg_cb = (SPM_DS_DOMAIN_CB*) AVLL_FIRST(v_spm_shared->ds_domain_tree))
    {
        AVLL_DELETE(v_spm_shared->ds_domain_tree, cfg_cb->spm_ds_domain_node);
        spm_free_ds_domain_cb(cfg_cb NBB_CCXT);   
    }
    
    return;   
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_pri2phb_cfg
   ��������  : dsģ�����ȼ���phb���ý��պ���
   �������  : ��TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_pri2phb_cfg(NBB_ULONG oper, 
    ATG_DCI_DIFF_SERV_PRI2PHBMAP **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DIFF_SERV_PRI2PHBMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_pri2phb_cfg");

    /*��Ϣ�Ĳ�������*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        NBB_MM_FREE(*pcb,MEM_SPM_DS_PRI2PHB_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*��Ϣ����Ϊ��*/
        if (NULL == buf)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pucPri2PhbDataStart is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*��Ϣ���ݲ�Ϊ��*/
        else
        {
            ptemp = (ATG_DCI_DIFF_SERV_PRI2PHBMAP *)buf;

            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DIFF_SERV_PRI2PHBMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DIFF_SERV_PRI2PHBMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_PRI2PHB_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DIFF_SERV_PRI2PHBMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_ds_phb2pri_cfg
   ��������  : dsģ��phb�����ȼ����ý��պ���
   �������  : ��TLV�Ĳ����룬������ָ���ָ��,ips��Ϣ�Ļ���
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_set_ds_phb2pri_cfg(NBB_ULONG oper,
    ATG_DCI_DIFF_SERV_PHB2PRIMAP **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DIFF_SERV_PHB2PRIMAP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ds_phb2pri_cfg");

    /*��Ϣ�Ĳ�������*/
    switch (oper)
    {
        /*����*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*ɾ��*/
        case  ATG_DCI_OPER_DEL:
        if (NULL == (*pcb))
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR BUF is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        NBB_MM_FREE(*pcb,MEM_SPM_DS_PHB2PRI_CB);
        *pcb = NULL;
        break;

        /*����*/
        case  ATG_DCI_OPER_ADD:

        /*��Ϣ����Ϊ��*/
        if (NULL == buf)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pucPhb2PriDataStart is NULL.\n\n",__FUNCTION__,__LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*��Ϣ���ݲ�Ϊ��*/
        else
        {
            ptemp = (ATG_DCI_DIFF_SERV_PHB2PRIMAP *)buf;


            /***************************************************************************/
            /*      ������                                           */
            /***************************************************************************/
            if (NULL == (*pcb))
            {
                (*pcb) = (ATG_DCI_DIFF_SERV_PHB2PRIMAP *)NBB_MM_ALLOC(sizeof(ATG_DCI_DIFF_SERV_PHB2PRIMAP),
                          NBB_NORETRY_ACT, MEM_SPM_DS_PHB2PRI_CB);
            }
            OS_MEMCPY((*pcb), ptemp, sizeof(ATG_DCI_DIFF_SERV_PHB2PRIMAP));
        }
        break;

        default:
        break;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_rcv_dci_set_ds
   ��������  : DS���պ���
   �������  : DS IPS��Ϣָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_ds(ATG_DCI_SET_DIFF_SERV *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulDskey = 0;
    SPM_DS_CB *pstDsCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;

    NBB_BYTE *pucPri2PhbDataStart = NULL;
    NBB_BYTE *pucPhb2PriDataStart = NULL;
    NBB_ULONG ulOperPri2Phb = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPhb2Pri = ATG_DCI_OPER_NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    /*�߼���������*/
    NBB_TRC_ENTRY("spm_rcv_dci_set_ds");

    if(NULL == pstSetIps)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR pstSetIps is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }
    if (0 == pstSetIps->key)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR param error.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;
    ulDskey = pstSetIps->key;
    if (pstSetIps->key > 4096) /*���ܳ�����Χ*/
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR ulDskey=%ld is out of range.\n\n",
                   __FUNCTION__,__LINE__,ulDskey);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    pstDsCb = AVLL_FIND(SHARED.ds_tree, &ulDskey);
    if (NULL == pstDsCb)
    {
        ucIfExist = QOS_UNEXIST;
    }

    /* ��ȡ�����ò��� */
    ulOperPri2Phb = pstSetIps->oper_pri2PhbMap;
    ulOperPhb2Pri = pstSetIps->oper_phb2PriMap;

    /*��ȡ������ƫ�Ƶ�ַ*/
    pucPri2PhbDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->pri2PhbMap_data);
    pucPhb2PriDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->phb2PriMap_data);

    /*��Ϣ����Ϊɾ��*/
    if (TRUE == pstSetIps->delete_struct)
    {
        /* �����Ŀ�����ڣ���ɾ�� */
        if (QOS_UNEXIST == ucIfExist)
        {
            OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR delete ds key %ld is not in use.\n\n",
                       __FUNCTION__,__LINE__,ulDskey);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        /* ���ڣ�ɾ�� */
        else
        {
            OS_SPRINTF(ucMessage,"%s,%d : delete ds key is %ld.\n\n",
                       __FUNCTION__,__LINE__,ulDskey);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            if(NULL == pstDsCb)
            {
                OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR can't del ds key.\n\n",
                           __FUNCTION__,__LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            if (0 != pstDsCb->cnt) /*�����ò�֧��ɾ��*/
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            AVLL_DELETE(SHARED.ds_tree, pstDsCb->spm_ds_node);
            ret = spm_free_ds_cb(pstDsCb NBB_CCXT);
            if (ATG_DCI_RC_UNSUCCESSFUL == ret)
            {
                goto EXIT_LABEL;
            }
        }
    }

    /*��Ϣ����Ϊ���ӻ��߸���*/
    else
    {

        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (QOS_UNEXIST == ucIfExist)
        {
            pstDsCb = spm_alloc_ds_cb(ulDskey NBB_CCXT);
            if (NULL == pstDsCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
        
        //coverity[dead_error_condition]
        if(NULL == pstDsCb)
        {
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }
        if (0 != pstDsCb->cnt)
        {
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                             �û� ���ȼ���PHB��ӳ��                          */
        /***************************************************************************/
        ret = spm_set_ds_pri2phb_cfg(ulOperPri2Phb,&(pstDsCb->pri2phb_cfg_cb),pucPri2PhbDataStart NBB_CCXT);
        pstSetIps->pri2PhbMap_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                             phb���û����ȼ�                             */
        /***************************************************************************/
        ret = spm_set_ds_phb2pri_cfg(ulOperPhb2Pri,&(pstDsCb->phb2pri_cfg_cb),pucPhb2PriDataStart NBB_CCXT);
        pstSetIps->phb2PriMap_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }
    }
    pstSetIps->return_code = ips_ret;

    /* �������������Ŀ�����뵽���� */
    if (ucIfExist == QOS_UNEXIST)
    {
        rv = AVLL_INSERT(SHARED.ds_tree, pstDsCb->spm_ds_node);
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_DS_CB*) AVLL_FIRST(v_spm_shared->ds_tree); cfg_cb != NULL;
        cfg_cb = (SPM_DS_CB*) AVLL_FIRST(v_spm_shared->ds_tree))
    {
        AVLL_DELETE(v_spm_shared->ds_tree, cfg_cb->spm_ds_node);
        spm_free_ds_cb(cfg_cb NBB_CCXT);   
    }
    return;   
}

#endif


#if 6

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_ds_set_l3uni(NBB_ULONG logic_key,NBB_ULONG posid,
            ATG_DCI_LOG_PORT_DIFF_SERV_DATA *data NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
#if defined (SPU) || defined (PTN690_CES)
    NBB_LONG unit = 0;
#endif
    SPM_DS_CB *pB = NULL;
    SPM_PORT_INFO_CB stPortInfo = {0};
    NBB_ULONG dsptr = 0;
    EGR_UNI_MODE_E mode;
    SPM_DS_DOMAIN_CB *pcb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == data) || (0 == posid) || (0 == logic_key))
    {
        printf("%s %s,LINE=%d : data==null,logic_key==0 or posid==0.\n",HQOS_USR_ERROR,__FUNCTION__,__LINE__);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d : data==null,logic_key==0 or posid==0.\n", 
                   HQOS_USR_ERROR,__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    ret = spm_get_portid_from_logical_port_index(logic_key, &stPortInfo NBB_CCXT);
    if(0 != ret)
    {
        printf("%s %s,LINE=%d spm_get_portid_from_logical_port_index logic_key=%ld,ret=%d\n",
            HQOS_USR_ERROR,__FUNCTION__,__LINE__,logic_key,ret);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d spm_get_portid_from_logical_port_index logic_key=%ld,ret=%d\n",
            HQOS_USR_ERROR,__FUNCTION__,__LINE__,logic_key,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    pB = spm_find_ds_cb(data->diff_serv_id NBB_CCXT);
    if (NULL == pB)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(NULL == pB->phb2pri_cfg_cb)
    {
	    OS_SPRINTF(ucMessage,"%s,%d,ds_index=%ld : QOS ERROR QOS ds cfg is NULL.\n\n",
                   __FUNCTION__,__LINE__,pB->ds_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    else
    {
        if (PHB2PRI_DSCP == pB->phb2pri_cfg_cb->pri_select) /*ָ�����ȼ�*/
        {
            dsptr = pB->phb2pri_cfg_cb->dscp + PHB2PRI_ASSIGN_OFFSET;
            mode = PHB_TO_DSCP_PRI;
        }
        else if (PHB2PRI_OVLAN == pB->phb2pri_cfg_cb->pri_select) /* ָ�����ȼ� */
        {
            dsptr = (pB->phb2pri_cfg_cb->ovlan_pri) + PHB2PRI_ASSIGN_OFFSET;
            mode = PHB_TO_PRI;
        }
        else if (PHB2PRI_DSCP_AND_VLAN == pB->phb2pri_cfg_cb->pri_select) /* ָ�����ȼ� */
        {
            /*������dsptr�����vlan��ӳ���ϵ*/
            dsptr = (pB->phb2pri_cfg_cb->dscp) + PHB2PRI_ASSIGN_OFFSET;
            mode = PHB_TO_DSCP_PRI;

        }
        else if (PHB2PRI_NOT == pB->phb2pri_cfg_cb->pri_select) /* ������������ */
        {
            dsptr = 0;
            mode = PRI_DSCP_NO_ACTION;
            
        }
        else if (PHB2PRI_MAP == pB->phb2pri_cfg_cb->pri_select) /* ӳ�� */
        {
            dsptr = pB->phb2pri_cfg_cb->index;
            if(0 == pB->phb2pri_cfg_cb->index)/*Ĭ��ӳ���*/
            {
                mode = PHB_TO_DSCP_PRI;
            }
            else
            {
                pcb = spm_find_ds_domain_cb(pB->phb2pri_cfg_cb->index);
                if(NULL == pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                else 
                {
                    
                    if(NULL != pcb->p2d_cfg_cb)
                    {
                        mode = PHB_TO_DSCP_PRI;
                    }
                    else if(NULL != pcb->p2vlan_cfg_cb)
                    {
                        mode = PHB_TO_PRI;
                    }
                    else
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                        
                }
            }  
        }
        else  /* ���ò�ƥ�� */
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            OS_SPRINTF(ucMessage,"%s,%d,ds_index=%ld : QOS ERROR phb2pri ds mode miss match.\n\n",
                       __FUNCTION__,__LINE__,pB->ds_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
    }

    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,LINE=%d logic_key=%ld,posid=%ld,\n"
            "diff_serv_id=%ld\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,
            logic_key,posid,data->diff_serv_id);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d logic_key=%ld,posid=%ld,\n"
            "diff_serv_id=%ld\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,
            logic_key,posid,data->diff_serv_id);
       BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

#if defined (SPU) || defined (PTN690_CES)
    for(unit = 0; unit < SHARED.c3_num;unit++)
    {

        /*�����»�l3uni��hqos:��hqosʹ�ܿ��غ�flowid���ݸ�c3����*/
        ret = ApiC3SetL3UniPri(unit,posid,NULL, mode, dsptr);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,LINE=%d posid=%ld,dsptr=%ld,ret=%d\n"
                "ApiC3SetL3UniPri\n",
                HQOS_USR_ERROR,__FUNCTION__,__LINE__,posid,dsptr,ret);
            OS_SPRINTF(ucMessage,"%s %s,LINE=%d posid=%ld,dsptr=%ld,ret=%d\n"
                "ApiC3SetL3UniPri\n",
                HQOS_USR_ERROR,__FUNCTION__,__LINE__,posid,dsptr,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
#endif 

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_logic_intf_node(
    SUB_PORT               *sub_port,
    SPM_QOS_LOGIC_INTF_KEY *key,
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve NBB_CCXT_T NBB_CXT) //INTF ���ÿ���Ŀ��ֵ
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_INTF_CB *join_node = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    TX_PORT_T node = {0};
    TX_PORT_T *pL3Uni = NULL;
    NBB_ULONG ds_index = 0;
    NBB_ULONG posid = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_ds_add_logic_intf_node");

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != key);
    NBB_ASSERT(NULL != sub_port);
    NBB_ASSERT(NULL != diff_serve);

    /*ָ��Ϊ��*/
    if ((NULL == key) || (NULL == sub_port) || (NULL == diff_serve))
    {
        printf("%s %s %d key or sub_port== NULL\n",QOS_ERROR_STRING,
            __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS key or sub_port== NULL",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    ds_index = diff_serve->diff_serv_id;
    posid = sub_port->posid;
    node.slot = sub_port->slot;
    node.port = sub_port->port;
    node.vid = sub_port->ovlan;
    pL3Uni = &node;

    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,%d log key=%ld,port=%d,posid=%ld\n"
                  "svlan=%d,cvlan=%d,diff_serv_id=%ld\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  key->index,sub_port->port,sub_port->posid,
                  diff_serve->svlan,diff_serve->cvlan,diff_serve->diff_serv_id);
        OS_SPRINTF(ucMessage,"%s %s,%d log key=%ld,port=%d,posid=%ld\n"
                   "svlan=%d,cvlan=%d,diff_serv_id=%ld\n\n",QOS_CFG_STRING,
                   __FUNCTION__,__LINE__,key->index,sub_port->port,sub_port->posid,
                   diff_serve->svlan,diff_serve->cvlan,diff_serve->diff_serv_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    join_node = AVLL_FIND(SHARED.qos_ds_logic_intf_tree, key);

    /***************************************************************************/
    /*                      ���Ӳ���                                           */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != ds_index))
    {
        join_node = spm_alloc_qos_ds_logic_intf_cb(key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_ds_l3_logic_intf(sub_port,ds_index, posid, pL3Uni,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_ds_logic_intf_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        
        //coverity[no_effect_test]
        rv = AVLL_INSERT(SHARED.qos_ds_logic_intf_tree, join_node->spm_qos_logic_node);
        goto EXIT_LABEL;
    }

    /*����*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index != ds_index))
    {
        ret = spm_add_ds_l3_logic_intf(sub_port,ds_index, posid, pL3Uni,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_intf_ds_cnt(sub_port,join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        goto EXIT_LABEL;
    }
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index == ds_index))
    {
        printf("%s %d the same cfg",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS INTF DS the same cfg",ATG_DCI_RC_OK,
					   "sub_port.slot","sub_port.port","sub_port.ovlan","logic key",
					   sub_port->slot,sub_port->port,sub_port->ovlan,key->index));
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 == ds_index)) /*Ĭ������*/
    {
        ret = spm_del_ds_l3_logic_intf(sub_port->unit,posid, pL3Uni NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_intf_ds_cnt(sub_port,join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(SHARED.qos_ds_logic_intf_tree, join_node->spm_qos_logic_node);
        ret = spm_free_ds_logic_intf_cb(join_node NBB_CCXT);
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            goto EXIT_LABEL;
        }
        goto EXIT_LABEL;
    }
    else /*���������*/
    {
        printf("**ERROR**%s %d ds index config missmatch",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS INTF DS config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"sub_port.slot","sub_port.port","ds_index","logic key",
						sub_port->slot,sub_port->port,ds_index,key->index));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_qos_clear_ds_logic_intf(NBB_LONG logic_key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DS_LOGIC_INTF_CB *cfg_cb = NULL;
    SPM_QOS_DS_LOGIC_INTF_CB *next_cfg_cb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    cfg_cb = (SPM_QOS_DS_LOGIC_INTF_CB*) AVLL_FIRST(SHARED.qos_ds_logic_intf_tree);
    while(cfg_cb != NULL)
    {
        next_cfg_cb = (SPM_QOS_DS_LOGIC_INTF_CB*) AVLL_NEXT(SHARED.qos_ds_logic_intf_tree,cfg_cb->spm_qos_logic_node);
        if(cfg_cb->logic_key.index != logic_key)
        {
            cfg_cb = next_cfg_cb;
            continue;
        }
        
        /***************����������������ֱ��ɾ����������**************************/
        spm_disconnect_intf_ds_cnt(&(cfg_cb->sub_port),cfg_cb->ds_index NBB_CCXT);
        AVLL_DELETE(SHARED.qos_ds_logic_intf_tree, cfg_cb->spm_qos_logic_node);
        spm_free_ds_logic_intf_cb(cfg_cb NBB_CCXT); 
        cfg_cb = next_cfg_cb;
    }
    
    NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_logic_intf(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_INTF_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_DS_LOGIC_INTF_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_intf_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_LOGIC_INTF_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_intf_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_ds_logic_intf_tree, cfg_cb->spm_qos_logic_node);
        spm_free_ds_logic_intf_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_flow_vpuni_node
   ��������  : �߼��˿�flow���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿�flow��key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_logic_univp_node(NBB_ULONG logic_index, 
     SPM_QOS_LOGIC_UNIVP *univp, ATG_DCI_LOG_PORT_DS_L2_DATA *diff_serv  NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_UNIVP_CB *join_node = NULL;
    NBB_INT  unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_PORT_INFO_CB pstPortInfo = {0};
    NBB_ULONG posid = 0;
    NBB_ULONG ds_index = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_ds_add_logic_univp_node");

    /* ָ��Ϊ�� */
    if ((NULL == diff_serv) || (NULL == univp) || (0 == logic_index))
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS ERROR diff_serv or univp is NULL.\n\n",__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS univp or key == NULL",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,%d LogicPort_Index=%ld,VpType=%ld,VpnId=%ld,\n"
                  "Vp_Posid=%ld,diff_serv_id=%ld.\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  logic_index,univp->vp_type,univp->vpn_id,
                  univp->vp_posid,diff_serv->ds_id);
        OS_SPRINTF(ucMessage,"%s %s,%d LogicPortIndex=%ld,VpType=%ld,\n"
                  "VpnId=%ld,VpPosid=%ld,DsIndex=%ld.\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  logic_index,univp->vp_type,univp->vpn_id,
                  univp->vp_posid,diff_serv->ds_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    ret = spm_get_portid_from_logical_port_index(logic_index, &pstPortInfo NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_get_portid_from_logical_port_index_error_log(logic_index,
                __FUNCTION__,__LINE__,ret NBB_CCXT);
        goto EXIT_LABEL;
    }

    unit  = pstPortInfo.unit_id;
    posid = univp->vp_posid;
    ds_index = diff_serv->ds_id;
    join_node = AVLL_FIND(SHARED.qos_ds_logic_univp_tree, &logic_index);

    /***************************************************************************/
    /*                                ���Ӳ���                                 */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != ds_index))
    {
        join_node = spm_alloc_qos_ds_logic_univp_cb(logic_index, univp NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_ds_l2_logic_univp(unit, ds_index, join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_ds_logic_univp_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        rv = AVLL_INSERT(SHARED.qos_ds_logic_univp_tree, join_node->spm_qos_logic_univp_node);
        goto EXIT_LABEL;
    }

    /* ���� */
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index != ds_index))
    {
        ret = spm_add_ds_l2_logic_univp(unit, ds_index, join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_phb2pri_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index; 
        goto EXIT_LABEL;
    }

    /* ɾ�����ò��ָ�Ĭ������ */
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 == ds_index))
    {
        ret = spm_del_ds_l2_logic_univp(unit,posid NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_phb2pri_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(SHARED.qos_ds_logic_univp_tree, join_node->spm_qos_logic_univp_node);
        spm_free_ds_logic_univp_cb(join_node NBB_CCXT);
        goto EXIT_LABEL;
    }

    /* ��ͬ�����ò����ش��� */
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index == ds_index))
    {
        OS_SPRINTF(ucMessage,"%s %s,%d : LogicPortIndex=%ld,VpType=%ld,\n"
                  "VpnId=%ld,VpPosid=%ld,DsIndex=%ld,unit=%d.\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  logic_index,univp->vp_type,univp->vpn_id,
                  univp->vp_posid,diff_serv->ds_id,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS INTF FLOW the same cfg",ATG_DCI_RC_OK,
						"vp_type","vpn_id","vp_posid","logicPort_Index",
						univp->vp_type,univp->vpn_id,posid,logic_index));
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else /* ��������� */
    {
        OS_SPRINTF(ucMessage,"%s %s,%d : mis-match parm.LogicPortIndex=%ld,VpType=%ld,\n"
                  "VpnId=%ld,VpPosid=%ld,DsIndex=%ld,unit=%d.\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  logic_index,univp->vp_type,univp->vpn_id,
                  univp->vp_posid,diff_serv->ds_id,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS INTF FLOW config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"vp_type","vpn_id","vp_posid","logicPort_Index",
						univp->vp_type,univp->vpn_id,posid,logic_index));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_qos_clear_logic_univp(NBB_LONG logic_key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DS_LOGIC_UNIVP_CB *cfg_cb = NULL;
    SPM_QOS_DS_LOGIC_UNIVP_CB *next_cfg_cb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    cfg_cb = (SPM_QOS_DS_LOGIC_UNIVP_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_univp_tree);
    while(cfg_cb != NULL)
     {
        next_cfg_cb = (SPM_QOS_DS_LOGIC_UNIVP_CB*) AVLL_NEXT(v_spm_shared->qos_ds_logic_univp_tree, \
                        cfg_cb->spm_qos_logic_univp_node);
        if(cfg_cb->logic_index != logic_key)
        {
            cfg_cb = next_cfg_cb;
            continue;
        }
        
        /***************����������������ֱ��ɾ����������**************************/
        ret = spm_disconnect_phb2pri_ds_cnt(cfg_cb->ds_index NBB_CCXT);
        AVLL_DELETE(v_spm_shared->qos_ds_logic_univp_tree, cfg_cb->spm_qos_logic_univp_node);
        spm_free_ds_logic_univp_cb(cfg_cb NBB_CCXT); 
        cfg_cb = next_cfg_cb;
     }

    NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_univp_cb(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_LOGIC_UNIVP_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_QOS_DS_LOGIC_UNIVP_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_univp_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_LOGIC_UNIVP_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_univp_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_ds_logic_univp_tree, cfg_cb->spm_qos_logic_univp_node);
        spm_free_ds_logic_univp_cb(cfg_cb NBB_CCXT);
    }


    return;
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_flow_node
   ��������  : �߼��˿�flow���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿�flow��key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_logic_flow_node(SUB_PORT *sub_port, 
            SPM_QOS_LOGIC_FLOW_KEY *key,ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff_serv
            NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_FLOW_CB *join_node = NULL;
    NBB_BYTE fapid = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_ULONG posid = 0;
    NBB_ULONG ds_index = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_ds_add_logic_flow_node");

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != key);

    /*ָ��Ϊ��*/
    if ((NULL == diff_serv) || (NULL == key) || (NULL == sub_port))
    {
        printf("%s %s %d posid==0 or key == NULL\n",QOS_ERROR_STRING,
            __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS posid==0 or key == NULL",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,%d log key=%ld,port=%d,posid=%ld\n"
                  "svlan=%d,cvlan=%d,diff_serv_id=%ld\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  key->index,sub_port->port,sub_port->posid,
                  diff_serv->svlan,diff_serv->cvlan,diff_serv->diff_serv_id);
        OS_SPRINTF(ucMessage,"%s %s,%d log key=%ld,port=%d,posid=%ld\n"
                   "svlan=%d,cvlan=%d,diff_serv_id=%ld\n\n",QOS_CFG_STRING,
                   __FUNCTION__,__LINE__,key->index,sub_port->port,sub_port->posid,
                   diff_serv->svlan,diff_serv->cvlan,diff_serv->diff_serv_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    spm_hardw_getslot(&fapid);
    if(sub_port->slot != fapid)
    {
        goto EXIT_LABEL;
    }
    posid = sub_port->posid;
    ds_index = diff_serv->diff_serv_id;
    join_node = AVLL_FIND(SHARED.qos_ds_logic_flow_tree, key);

    /***************************************************************************/
    /* ���Ӳ���                              */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != ds_index))
    {
        join_node = spm_alloc_qos_ds_logic_flow_cb(key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_ds_l2_logic_flow(sub_port,ds_index, posid,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_ds_logic_flow_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        rv = AVLL_INSERT(SHARED.qos_ds_logic_flow_tree, join_node->spm_qos_logic_node);
        goto EXIT_LABEL;
    }

    /*����*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index != ds_index))
    {
        ret = spm_add_ds_l2_logic_flow(sub_port,ds_index, posid,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_pri2phb_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        goto EXIT_LABEL;
    }

    /* ɾ������ */
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 == ds_index))
    {
        ret = spm_del_ds_l2_logic_flow(sub_port->unit,posid NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_pri2phb_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(SHARED.qos_ds_logic_flow_tree, join_node->spm_qos_logic_node);
        spm_free_ds_logic_flow_cb(join_node NBB_CCXT);

        goto EXIT_LABEL;
    }

    /*��ͬ�����ò����ش���*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index == ds_index))
    {
        printf("%s %d the same cfg",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS INTF FLOW the same cfg",ATG_DCI_RC_OK,
						"sub_port.slot","sub_port.port","logic key","flow id",
						sub_port->slot,sub_port->port,key->index,key->svlan));
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else /*���������*/
    {
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS INTF FLOW config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"ds_index","sub_port.port","logic key","flow id",
						ds_index,sub_port->port,key->index,key->svlan));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_qos_clear_logic_ds_flow(NBB_LONG logic_key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DS_LOGIC_FLOW_CB *cfg_cb = NULL;
    SPM_QOS_DS_LOGIC_FLOW_CB *next_cfg_cb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    cfg_cb = (SPM_QOS_DS_LOGIC_FLOW_CB*) AVLL_FIRST(SHARED.qos_ds_logic_flow_tree);
    while(cfg_cb != NULL)
    {
        next_cfg_cb = (SPM_QOS_DS_LOGIC_FLOW_CB*) AVLL_NEXT(SHARED.qos_ds_logic_flow_tree,cfg_cb->spm_qos_logic_node);
        if(cfg_cb->flow_key.index != logic_key)
        {
            cfg_cb = next_cfg_cb;
            continue;
        }

        /***************����������������ֱ��ɾ����������**************************/
        spm_disconnect_pri2phb_ds_cnt(cfg_cb->ds_index NBB_CCXT);
        AVLL_DELETE(SHARED.qos_ds_logic_flow_tree, cfg_cb->spm_qos_logic_node);
        spm_free_ds_logic_flow_cb(cfg_cb NBB_CCXT); 
        cfg_cb = next_cfg_cb;
     }

    NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_logic_flow(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_LOGIC_FLOW_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_QOS_DS_LOGIC_FLOW_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_flow_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_LOGIC_FLOW_CB*) AVLL_FIRST(v_spm_shared->qos_ds_logic_flow_tree))
     {
        AVLL_DELETE(v_spm_shared->qos_ds_logic_flow_tree, cfg_cb->spm_qos_logic_node);
        spm_free_ds_logic_flow_cb(cfg_cb NBB_CCXT);   
     }


    return;   
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_vc_node
   ��������  : vc���ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,vc key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_vc_node(
    NBB_ULONG ds_index, //VC�� DSģ��ID
    ATG_DCI_VC_KEY *key, //VC�� ���ÿ���Ŀ��ֵ
    NBB_ULONG posid, //��������ֵ
    NBB_USHORT flag NBB_CCXT_T NBB_CXT)    //VC������ʶ
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_VC_CB *join_node = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_ds_add_vc_node");

    /*ָ��Ϊ��*/
    if ((0 == posid) || (NULL == key))
    {
        /*
            printf("%s %s %d posid or key  == NULL\n",QOS_ERROR_STRING,
            __FUNCTION__,__LINE__);
        */
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS posid or key  == NULL",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_vc_cfg_print)
    {
        OS_PRINTF("%s %s,%d ds_index=%ld,vc_id=%ld,peer_ip=0x%lx\n"
                  "vc_type=%d,posid=%ld,flag=%d\n\n",QOS_CFG_STRING,__FUNCTION__,
                  __LINE__,ds_index,key->vc_id,key->peer_ip,key->vc_type,posid,flag);
        OS_SPRINTF(ucMessage,"%s %s,%d ds_index=%ld,vc_id=%ld,peer_ip=0x%lx\n"
                   "vc_type=%d,posid=%ld,flag=%d\n\n",QOS_CFG_STRING,__FUNCTION__,
                   __LINE__,ds_index,key->vc_id,key->peer_ip,key->vc_type,posid,flag);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    join_node = AVLL_FIND(SHARED.qos_ds_vc_tree, key);

    /***************************************************************************/
    /* ���Ӳ���                              */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != ds_index))
    {
        join_node = spm_alloc_qos_ds_vc_cb(key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_ds_vc(ds_index, posid, flag,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_ds_vc_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        rv = AVLL_INSERT(SHARED.qos_ds_vc_tree, join_node->spm_qos_vc_node);
        goto EXIT_LABEL;
    }

    /*����*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index != ds_index))
    {
        ret = spm_add_ds_vc(ds_index, posid, flag,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        goto EXIT_LABEL;
    }
	
	/* ɾ������ */
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 == ds_index))
    {
        ret = spm_del_ds_vc(posid, flag NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(SHARED.qos_ds_vc_tree, join_node->spm_qos_vc_node);
        ret = spm_free_ds_vc_cb(join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        goto EXIT_LABEL;
    }

    /*��ͬ�����ò����ش���*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index == ds_index))
    {
       /* printf("%s %d the same cfg\n",__FUNCTION__,__LINE__); 
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS VC DS the same cfg",ATG_DCI_RC_OK,
						"key->peer_ip","key->vc_id","key->vc_type","ds_index",
						key->peer_ip,key->vc_id,key->vc_type,ds_index));*/
        goto EXIT_LABEL;
    }

    /*ɾ���˵�������ɾ��������*/
    else if ((NULL == join_node) && (0 == ds_index))
    {
        OS_SPRINTF(ucMessage,"QOS VC DS DUPLICATE ELIMINATION key->peer_ip=0x%lx,"
                   "key->vc_id=%ld,key->vc_type=%d,ds_index=%ld\n\n",
			       key->peer_ip,key->vc_id,key->vc_type,ds_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }
    else /*���������*/
    {
        /* printf( "**ERROR**%s %d ds index config missmatch\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS VC DS config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"key->peer_ip","key->vc_id","key->vc_type","ds_index",
						key->peer_ip,key->vc_id,key->vc_type,ds_index)); */
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_vc(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_VC_CB *cfg_cb = NULL;



    for (cfg_cb = (SPM_QOS_DS_VC_CB*) AVLL_FIRST(v_spm_shared->qos_ds_vc_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_VC_CB*) AVLL_FIRST(v_spm_shared->qos_ds_vc_tree))
     {
        AVLL_DELETE(v_spm_shared->qos_ds_vc_tree, cfg_cb->spm_qos_vc_node);
        spm_free_ds_vc_cb(cfg_cb NBB_CCXT);   
     }


    return;   
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_vrf_nhi
   ��������  : L3TXUNI ����NHI��DS����
   �������  : dsģ���index  ,nhiId
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_vrf_nhi(NBB_ULONG ds_index,NBB_ULONG nhiId NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_CB *pB = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_ULONG dsptr = 0;
    NBB_LONG phb2pri_flag = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_LONG unit = 0;
#endif

    NBB_TRC_ENTRY("spm_ds_add_vrf_nhi");

    /*DSģ������Ϊ0*/
    if (0 == ds_index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pB = spm_find_ds_cb(ds_index NBB_CCXT);

    /*ds���ò�����*/
    if (NULL == pB)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
#if 1

    /*phb2pri����Ϊ�ղ���Ĭ������*/
    if (NULL == pB->phb2pri_cfg_cb)
    {
        goto EXIT_LABEL;
    }
    else /*���÷ǿ�*/
    {
#if defined (SPU) || defined (PTN690_CES)
        if (PHB2PRI_DSCP == pB->phb2pri_cfg_cb->pri_select) /*ָ�����ȼ�*/
        {
            dsptr = pB->phb2pri_cfg_cb->dscp + PHB2PRI_ASSIGN_OFFSET;
            for(unit = 0; unit < SHARED.c3_num;unit++)
            {
                ret += ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, dsptr);
            }
        }
        else if (PHB2PRI_OVLAN == pB->phb2pri_cfg_cb->pri_select) /*ָ�����ȼ�*/
        {
            dsptr = pB->phb2pri_cfg_cb->ovlan_pri + PHB2PRI_ASSIGN_OFFSET;

            for(unit = 0; unit < SHARED.c3_num;unit++)
            {
                ret += ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_PRI, dsptr);
            }
        }
        else if (PHB2PRI_DSCP_AND_VLAN == pB->phb2pri_cfg_cb->pri_select) /*ָ�����ȼ�*/
        {
            dsptr = pB->phb2pri_cfg_cb->ovlan_pri + PHB2PRI_ASSIGN_OFFSET;

            for(unit = 0; unit < SHARED.c3_num;unit++)
            {
                ret += ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, dsptr);
            }
        }
        else if (PHB2PRI_MAP == pB->phb2pri_cfg_cb->pri_select) /*ӳ��*/
        {
            ret = spm_add_phb2pri_vrf_ds_domain_intf(pB->phb2pri_cfg_cb->index, 
                nhiId NBB_CCXT);
            phb2pri_flag = ret;
        }
        else
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (ATG_DCI_RC_OK != ret)
        {
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS vrf nhi DS",ret,
						"nhiId","phb2pri mode","dsptr","",
						nhiId,pB->phb2pri_cfg_cb->pri_select,dsptr,0));
            goto EXIT_LABEL;
        } 
#endif
    }
#endif

#if  0 
    if (ATG_DCI_RC_OK == phb2pri_flag)
    {
        ret = spm_connect_ds_domain_cnt(pB->phb2pri_cfg_cb->index);
        if (ATG_DCI_RC_OK != ret)
        {
            NBB_TRC_FLOW((NBB_FORMAT "**ERROR** spm_connect_ds_domain_cnt"));
            goto EXIT_LABEL;
        }
    }
    (pB->cnt)++;
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_ds_del_vrf_nhi
   ��������  : L3TXUNI ����NHI��DS����
   �������  : dsģ���index  ,nhiId
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_del_vrf_nhi(NBB_ULONG nhiId NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_ULONG dsptr = 0;
#endif
    NBB_LONG unit = 0;

    for(unit = 0; unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret += ApiC3SetL3UniPri(unit, nhiId, NULL, PHB_TO_DSCP_PRI, dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            return ret;
        }
#endif
    }
    
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_vrf_node
   ��������  : vrf���ȼ�ӳ��Ľӿں���ֻ����VRF�ϻ�NNI��(VRFʵ��ֻ����NNI)
   �������  : dsģ���index  ,ftn key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_vrf_node(NBB_ULONG ds_index, SPM_QOS_VRF_INSTANSE_KEY *key,
    NBB_ULONG nhiId NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_VRF_CB *join_node = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_ds_add_vrf_node");

    if(NULL == key)
    {
        printf("%s %s %d node or key == NULL\n",QOS_ERROR_STRING,
            __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS node or key == NULL",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_vrf_cfg_print)
    {
        OS_PRINTF("%s %s,%d ds_index=%ld,vrf_id=%d,peer_ip=0x%lx,label=%ld,nhiId=%ld\n\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,
            ds_index,key->vrf_id,key->peer_ip,key->label,nhiId);
        OS_SPRINTF(ucMessage,"%s %s,%d ds_index=%ld,vrf_id=%d,peer_ip=0x%lx,label=%ld,nhiId=%ld\n\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,
            ds_index,key->vrf_id,key->peer_ip,key->label,nhiId);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    /* ҵ�����QOSû������ֶ��������� */
    if (0 == nhiId)
    {
        ret = spm_clear_ds_vrf_mem(key NBB_CCXT); 
        goto EXIT_LABEL;
    }
    if ((0 == nhiId) || (NULL == key))
    {
        printf("%s %s %d node or key == NULL\n",QOS_ERROR_STRING,
                  __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS node or key == NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ����ds��vrf�� */
    join_node = AVLL_FIND(SHARED.qos_ds_vrf_tree,key);

    /* ���Ӳ��� */
    if ((NULL == join_node) && (0 != ds_index))
    {
        join_node = spm_alloc_qos_ds_vrf_cb(key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_ds_vrf(ds_index,nhiId,join_node,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_ds_vrf_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        rv = AVLL_INSERT(SHARED.qos_ds_vrf_tree, join_node->spm_qos_vrf_node);
    }

    /* ���²��� */
    else if((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index != ds_index))
    {
        ret = spm_add_ds_vrf(ds_index, nhiId,join_node,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_phb2pri_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        join_node->ds_index = ds_index;
        goto EXIT_LABEL;
    }

    /* ɾ��vrf�ϰ󶨵�dsģ�� */
    else if((NULL != join_node) && (0 != join_node->ds_index) && (0 == ds_index))
    {
        ret = spm_del_ds_vrf(nhiId NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        join_node->dsptr = 0;
        join_node->out_mode = NNI_NO_ACTION;
        ret = spm_disconnect_phb2pri_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(SHARED.qos_ds_vrf_tree, join_node->spm_qos_vrf_node);
        spm_free_ds_vrf_cb(join_node NBB_CCXT);
        goto EXIT_LABEL;
    }

    /*��ͬ�����ò����ش���*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index == ds_index))
    {
        OS_SPRINTF(ucMessage,"##### VRF:SAME DS CONFIGURATION (vrf_id=%d,peer_ip=0x%lx,"
	        		"label=%ld,ds_index=%ld,nhiId=%ld) #####\n",
	                key->vrf_id,key->peer_ip,key->label,ds_index,nhiId);
	    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = spm_add_ds_vrf(ds_index,nhiId,join_node,ATG_DCI_RC_OK NBB_CCXT);
        goto EXIT_LABEL;
    }

    /*���������*/
    else
    {
        /*printf("**ERROR**%s %d ds index config missmatch",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS VRF DS config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"peer_ip","label","vrf_id","ds_index",
						key->peer_ip,key->label,key->vrf_id,ds_index));*/
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_vrf(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_VRF_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_DS_VRF_CB*) AVLL_FIRST(v_spm_shared->qos_ds_vrf_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_VRF_CB*) AVLL_FIRST(v_spm_shared->qos_ds_vrf_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_ds_vrf_tree, cfg_cb->spm_qos_vrf_node);
        spm_free_ds_vrf_cb(cfg_cb NBB_CCXT);   
    }


    return;   
}



/*****************************************************************************
   �� �� ��  : spm_ds_add_ilm_node
   ��������  : ILM���ȼ�ӳ��Ľӿں���(���������ڲ��ǩ)
   �������  : dsģ���index  ,ilm key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_ilm_node(
            ILM_DIFSER *ilm_ds, 
            SPM_QOS_ILM_KEY *key, //ilm KEY
            NBB_ULONG posid, //����ILM��Ŀʱ�����ǩ��Ϣ
            NBB_BYTE node_action, //�ڵ㶯����Ϣ
            NBB_ULONG ds_index2, //��װ�ڶ���Tunnel��ǩ��DSģ��ID
            SPM_QOS_TUNNEL_KEY *key_tun2 NBB_CCXT_T NBB_CXT) //��װ�ڶ���Tunnel��ǩ����Ŀ��ֵ
{
    return 0;
}


#if 8

/*****************************************************************************
   �� �� ��  : spm_ds_add_ilm_node
   ��������  : ILM���ȼ�ӳ��Ľӿں���(���������ڲ��ǩ)
   �������  : dsģ���index  ,ilm key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static NBB_LONG spm_ds_set_out_ilm(NBB_ULONG posid, NBB_ULONG eposid,NBB_BYTE type,
                EGR_LSP_MODE_E out_mod,NBB_USHORT out_dsptr,NBB_BYTE exp) 
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*������*/
    if(1 == type)/*RxLspEgrInExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrInExp(unit, posid, out_mod,exp, out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(2 == type)/*RxLspEgrBackupInExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrBackupInExp(unit, posid, out_mod,exp, out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(3 == type)/*RxLspEgrOutExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrOutExp(unit, posid, out_mod,exp/*, out_dsptr*/);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(4 == type)/*RxLspEgrBackupOutExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrBackupOutExp(unit, posid, out_mod,exp/*, out_dsptr*/);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(5 == type)/*RxLspEcmpInExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrEcmpInExp(unit, posid,eposid, out_mod,exp, out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(6 == type)/*RxLspEcmpOutExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrEcmpOutExp(unit, posid,eposid, out_mod,exp/*, out_dsptr*/);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else
    {
        //spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_ilm_node
   ��������  : ILM���ȼ�ӳ��Ľӿں���(���������ڲ��ǩ)
   �������  : dsģ���index  ,ilm key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static NBB_LONG spm_ds_set_in_ilm(NBB_ULONG posid,NBB_BYTE type,
                                    ING_NNI_MODE_E in_mod,NBB_USHORT in_dsptr) 
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*�뷽��*/
    if(0 == type)
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxpwPhb(unit, posid, in_mod, in_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspIngPhb(unit, posid, in_mod, in_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }   
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_ilm_node
   ��������  : ILM���ȼ�ӳ��Ľӿں���(���������ڲ��ǩ)
   �������  : dsģ���index  ,ilm key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static NBB_LONG spm_ds_set_ilm(NBB_ULONG posid, NBB_ULONG eposid,NBB_BYTE type,
                ING_NNI_MODE_E in_mod,NBB_USHORT in_dsptr,EGR_LSP_MODE_E out_mod,
                NBB_USHORT out_dsptr,NBB_BYTE exp) 
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    ret = spm_ds_set_in_ilm(posid, type,in_mod, in_dsptr);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    
    ret = spm_ds_set_out_ilm(posid,eposid,type,out_mod,out_dsptr, exp);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_print_ilm_diff_serv_data(NBB_ULONG label,ILM_DIFSER *diff,
                            NBB_ULONG posid,NBB_ULONG eposid,NBB_BYTE type)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != qos_vrf_cfg_print)
        {
            printf("label =%ld,nextport=%ld,nextip=%ld\n"
                      "dserid=%ld,posid=%ld,eposid=%ld,type=%d\n\n",
                      label,diff->nextport,diff->nextip,diff->dserid,
                      posid,eposid,type);
        } 
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_log_ilm_diff_serv_data(NBB_ULONG label,ILM_DIFSER *diff,NBB_ULONG posid,
                                        NBB_ULONG eposid,NBB_BYTE type)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    
    qos_print_ilm_diff_serv_data(label,diff, posid,eposid, type);
    
    if(NULL != diff)
    {
        if(ATG_DCI_RC_OK != qos_vrf_cfg_print)
        {
            
            OS_SPRINTF(ucMessage,"(%s)label =%ld,nextport=%ld,nextip=%ld\n"
                      "dserid=%ld,posid=%ld,eposid=%ld,type=%d\n\n",
                      QOS_CFG_STRING,
                      label,diff->nextport,diff->nextip,diff->dserid,
                      posid,eposid,type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        }
    }
   
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_ilm_node
   ��������  : ILM���ȼ�ӳ��Ľӿں���(���������ڲ��ǩ)
   �������  : eposid:ECMP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/

//type:
//0:RXPW;1: RxLspEgrInExp;2:RxLspEgrBackupInExp;
//3:RxLspEgrOutExp;4:RxLspEgrBackupOutExp
//5:RxLspEcmpInExp;6:RxLspEcmpOutExp
NBB_LONG spm_add_ilm_ds_node(NBB_ULONG label,ILM_DIFSER *diff,NBB_ULONG posid,
                                NBB_ULONG eposid,NBB_BYTE type) 
{

    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ING_NNI_MODE_E in_mod = UNIFORM;
    EGR_LSP_MODE_E out_mod = LSP_NO_ACTION;
    NBB_BYTE unit = 0;
    NBB_USHORT in_dsptr = 0;
    NBB_USHORT out_dsptr = 0;
    SPM_DS_CB *pB = NULL;
    NBB_BYTE exp = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == diff)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    qos_log_ilm_diff_serv_data(label,diff, posid,eposid,type);


    /***************************************************************************/
    /*      �ָ�Ĭ��ֵ                                                    */
    /***************************************************************************/
    if(0 == diff->dserid)
    {    
        ret = spm_ds_set_ilm(posid,eposid,type,in_mod, in_dsptr,
                             out_mod,out_dsptr, exp);
        goto EXIT_LABEL;
    }

  

    /***************************************************************************/
    /* �������߸�������                                                      */
    /***************************************************************************/
    pB = spm_find_ds_cb(diff->dserid NBB_CCXT);
    if (NULL == pB)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    if (NULL != pB->pri2phb_cfg_cb)
    {
        if(0 == pB->pri2phb_cfg_cb->pri_select)/*ָ��*/
        {
            in_mod = UNIFORM;
            in_dsptr = (pB->pri2phb_cfg_cb->phb) * MAX_COLOR_NUM + pB->pri2phb_cfg_cb->color + PRI2PHB_ASSIGN_OFFSET;
        }
        else if(1 == pB->pri2phb_cfg_cb->pri_select)/*�̹ܵ�*/
        {
            in_mod = SHORT_PIPE;
            in_dsptr =  pB->pri2phb_cfg_cb->index;
        }
        else if(8 == pB->pri2phb_cfg_cb->pri_select)/*����EXP*/
        {
            in_mod = UNIFORM;
            in_dsptr =  pB->pri2phb_cfg_cb->index;
        }
        else
        {
            //spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ret = spm_ds_set_in_ilm(posid, type,in_mod, in_dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }


    if (NULL != pB->phb2pri_cfg_cb)
    {
        if(0 == pB->phb2pri_cfg_cb->pri_select)/*������*/
        {
            out_mod = LSP_NO_ACTION;
            out_dsptr = pB->phb2pri_cfg_cb->exp + PHB2PRI_ASSIGN_OFFSET;
        }
        else if(5 == pB->phb2pri_cfg_cb->pri_select)/*ָ��EXP*/
        {
            out_mod = MAP_EXP;
            exp = pB->phb2pri_cfg_cb->exp;
            out_dsptr =  pB->phb2pri_cfg_cb->index;
        }
        else if(16 == pB->phb2pri_cfg_cb->pri_select)/*����ӳ��*/
        {
            out_mod = MAP_EXP;
            out_dsptr =  pB->phb2pri_cfg_cb->index;
        }
        else
        {
            //spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }


        ret = spm_ds_set_out_ilm(posid,eposid,type,out_mod,out_dsptr, exp);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }
   
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}
#endif

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_ilm(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DS_ILM_CB *cfg_cb = NULL;


    for(cfg_cb = (SPM_QOS_DS_ILM_CB*) AVLL_FIRST(v_spm_shared->qos_ds_ilm_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_ILM_CB*) AVLL_FIRST(v_spm_shared->qos_ds_ilm_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_ds_ilm_tree, cfg_cb->spm_qos_ilm_node);
        spm_free_ds_ilm_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_rx_lsp_node
   ��������  : rx lsp���ȼ�ӳ��Ľӿں���(������ǩ��Ч���ڲ��ǩģʽ��Ч)
   �������  : dsģ���index  ,rx_lsp key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_LONG spm_ds_add_rx_lsp_node(
    NBB_ULONG ds_index, //ILM DSģ��ID
    CRRXLSP_KEY *key, //CRRXLSP_KEY key
    NBB_ULONG posid, //����ILM��Ŀʱ����������
    NBB_BYTE node_action, //�ڵ㶯����Ϣ
    NBB_USHORT flag, //����ILM��ʶ1/2:��/��
    NBB_ULONG ds_index2, //��װ�ڶ���Tunnel��ǩ��DSģ��ID
    SPM_QOS_TUNNEL_KEY *key_tun2 NBB_CCXT_T NBB_CXT) //��װ�ڶ���Tunnel��ǩ����Ŀ��ֵ
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
#if 0
    SPM_QOS_DS_RX_LSP_CB *join_node = NULL;
    SPM_QOS_DS_RXLSP_KEY rxlsp_key = {0};

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != key);
    NBB_ASSERT(0 != posid);

    /*ָ��Ϊ��*/
    if ((0 == posid) || (NULL == key))
    {
        NBB_TRC_FLOW((NBB_FORMAT "**FATAL ERROR** posid or key == NULL "));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        return ret;
    }
    OS_MEMCPY(&rxlsp_key.rxlspkey,key,sizeof(CRRXLSP_KEY));
    rxlsp_key.flag = flag;
    join_node = AVLL_FIND(SHARED.qos_ds_rx_lsp_tree, &rxlsp_key);

    /***************************************************************************/
    /* ���Ӳ���                              */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != ds_index))
    {
        join_node = spm_alloc_qos_ds_rx_lsp_cb(&rxlsp_key NBB_CCXT);
        if (NULL == join_node)
        {
            /*************************************************************************/
            /* �ڴ����ʧ�ܡ���¼�����˳���                                          */
            /*************************************************************************/
            NBB_TRC_FLOW((NBB_FORMAT "SPM_QOS_DS_RX_LSP_CB allocation failed"));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            return ret;
        }
        ret = spm_add_ds_rx_lsp(ds_index, posid, node_action, flag, ds_index2, key_tun2,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret) /*����ʧ��*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "**ERROR** add  ds rx_lsp faile"));
            spm_free_ds_rx_lsp_cb(join_node NBB_CCXT);
            return ret;
        }
        join_node->ds_index = ds_index;
        AVLL_INSERT(SHARED.qos_ds_rx_lsp_tree, join_node->spm_qos_rx_lsp_node);
        return ret;
    }

    /*����*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index != ds_index))
    {
        ret = spm_add_ds_rx_lsp(ds_index,posid,node_action,flag,ds_index2,key_tun2,join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            NBB_TRC_FLOW((NBB_FORMAT "**ERROR** add  ds rx_lsp  faile"));
            return ret;
        }
        ret = spm_disconnect_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            return ret;
        }
        join_node->ds_index = ds_index;
        return ret;
    }
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 == ds_index)) /*Ĭ������*/
    {
        ret = spm_del_ds_rx_lsp(posid, node_action, flag, ds_index2, key_tun2 NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            NBB_TRC_FLOW((NBB_FORMAT "**ERROR** del  ds rx_lsp  faile"));
            return ret;
        }
        ret = spm_disconnect_ds_cnt(join_node->ds_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            return ret;
        }
        AVLL_DELETE(SHARED.qos_ds_rx_lsp_tree, join_node->spm_qos_rx_lsp_node);
        ret = spm_free_ds_rx_lsp_cb(join_node NBB_CCXT);
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            return ret;
        }
        return ret;
    }

    /*��ͬ�����ò����ش���*/
    else if ((NULL != join_node) && (0 != join_node->ds_index) && (0 != ds_index) && (join_node->ds_index == ds_index))
    {
        NBB_TRC_FLOW((NBB_FORMAT "the same cfg"));
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS RXLSP DS the same cfg",ATG_DCI_RC_OK,
						"key->lspid","key->tunnelid","key->ingress","ds_index",
						key->lspid,key->tunnelid,key->ingress,ds_index));
        return ret;
    }
    else /*���������*/
    {
        NBB_TRC_FLOW((NBB_FORMAT "**ERROR** ds index config missmatch"));
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS RXLSP DS config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
					   "key->lspid","key->tunnelid","key->ingress","ds_index",
					   key->lspid,key->tunnelid,key->ingress,ds_index));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        return ret;
    }
#endif

    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_rxlsp(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_DS_RX_LSP_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_DS_RX_LSP_CB*) AVLL_FIRST(v_spm_shared->qos_ds_rx_lsp_tree);cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_RX_LSP_CB*) AVLL_FIRST(v_spm_shared->qos_ds_rx_lsp_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_ds_rx_lsp_tree, cfg_cb->spm_qos_rx_lsp_node);
        spm_free_ds_rx_lsp_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_ds_add_tx_lsp_node (NBB_ULONG ds_index, //FTN DSģ��ID
    SPM_QOS_TUNNEL_KEY *pkey, //FTN ���ÿ���Ŀ��ֵ
    NBB_ULONG posid, //����LSP_TX��Ŀʱ����������
    NBB_ULONG ds_index2, //��װ�ڶ���Tunnel��ǩ��DSģ��ID
    SPM_QOS_TUNNEL_KEY *key_tun2 NBB_CCXT_T NBB_CXT) //��װ�ڶ���Tunnel��ǩ����Ŀ��ֵ
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

#if 0
    NBB_LONG ret1 = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_LONG ret2 = ATG_DCI_RC_UNSUCCESSFUL;

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != pkey);
    NBB_ASSERT(0 != posid);

    /*��һ��LSP�������*/
    if ((0 == posid) || (NULL == pkey))
    {
        printf("%s %s %d node or key == NULL\n",QOS_ERROR_STRING,
            __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS node or key == NULL",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        return ret;
    }
    ret1 = spm_ds_add_tx_inlsp_node(ds_index, pkey, posid NBB_CCXT);
    if (ATG_DCI_RC_OK != ret1)
    {
        NBB_TRC_FLOW((NBB_FORMAT "**ERROR** ret1 = %ld", ret1));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
    }
    if (NULL != key_tun2)
    {
        ret2 = spm_ds_add_tx_outlsp_node(ds_index2, key_tun2, posid NBB_CCXT);
        if (ATG_DCI_RC_OK != ret2)
        {
            NBB_TRC_FLOW((NBB_FORMAT "**ERROR** ret2 = %ld", ret2));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
        }
    }
#endif

    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_ds_txlsp(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_DS_TUNNEL_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_DS_TUNNEL_CB*) AVLL_FIRST(v_spm_shared->qos_ds_tunnel_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_TUNNEL_CB*) AVLL_FIRST(v_spm_shared->qos_ds_tunnel_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_ds_tunnel_tree, cfg_cb->spm_qos_tunnel_node);
        spm_free_ds_tunnel_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}

#endif









