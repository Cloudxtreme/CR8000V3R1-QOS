/******************************************************************************

                  ��Ȩ���� (C), ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************

  �� �� ��   : spm_logical_port_cfg_proc.c
  �� �� ��   : ����
  ��    ��   : lish
  ��������   : 2015��8��10��
  ����޸�   :
  ��������   : �߼��˿����ô����ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    �� : 2015��8��10��
    ��    �� : lish
    �޸����� : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef LOGICAL_PORT_CFG

extern unsigned char g_logical_port_cfg_print_setting;
extern char *port_route_type_string[];
extern char *cfg_oper_type_string[];

/* vpws max_num�����豸���ע��ʹ�� (hdjia) */
NBB_ULONG g_l2_vpws_maxnum = 0;

/* vpls max_num�����豸���ע��ʹ�� (hdjia) */
NBB_ULONG g_l2_vpls_maxnum = 0;

/*****************************************************************************
 �� �� ��  : spm_logical_port_vpbindvpws_proc
 ��������  : vp�߼��ں�vpws�İ󶨹�ϵ������Ϊ��Ӱ󶨣���ɾ�����
 �������  : NBB_ULONG ul_port_key  
             SPM_VPWS_CB *pst_vpws  
             NBB_BYTE    proc_type  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��5��
    ��    ��   : hdjia
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_vpbindvpws_proc(
    NBB_ULONG ul_port_key,
    SPM_VPWS_CB *pst_vpws,
    NBB_BYTE    proc_type   /* �����ģʽ����Ϊ��Ӻ�ɾ�� (hdjia) */
    )
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
   

    NBB_TRC_ENTRY("spm_logical_port_vpbindvpws_proc");

    if (NULL == pst_vpws)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_vpws NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_vpws NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }    
    
    /* ɾ�����ͣ����*/
    if(ATG_DCI_OPER_DEL == proc_type)
    {
        if(ul_port_key == pst_vpws->port_index)
        {                    
            pst_vpws->port_index = 0;

            /* ����portΪ0�������uni_no��uni_num������һ (hdjia) */
            if (0 == pst_vpws->port_index_bak)
            {
                pst_vpws->uni_no = 0;                    
                pst_vpws->uni_num--;
            }
        }
        else if(ul_port_key == pst_vpws->port_index_bak)
        {                    
            pst_vpws->port_index_bak = 0;

            /* ����portΪ0�������uni_no��uni_num������һ (hdjia) */
            if (0 == pst_vpws->port_index)
            {
                pst_vpws->uni_no = 0;                    
                pst_vpws->uni_num--;
            }
        }
        else if(ul_port_key == pst_vpws->port_index_2)
        {                    
            pst_vpws->port_index_2 = 0;

            /* ����portΪ0�������uni_no��uni_num������һ (hdjia) */
            if (0 == pst_vpws->port_index_bak_2)
            {
                pst_vpws->uni_no_2 = 0;                    
                pst_vpws->uni_num--;
            }
        }
        else if(ul_port_key == pst_vpws->port_index_bak_2)
        {                    
            pst_vpws->port_index_bak_2 = 0;

            /* ����portΪ0�������uni_no��uni_num������һ (hdjia) */
            if (0 == pst_vpws->port_index_2)
            {
                pst_vpws->uni_no_2 = 0;                    
                pst_vpws->uni_num--;
            }
        }                
        
    }
    else if (ATG_DCI_OPER_ADD == proc_type)     /* ��ӣ��󶨹�ϵ (hdjia) */
    {

    }
           


    
   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}



/*****************************************************************************
 �� �� ��  : spm_logical_port_del_l2vpn_cfg
 ��������  : �߼�����ɾ��l2vpn����ʱ��1��������vpΪ��Ч��2�����vp��vpn��
             ��ϵ��3���ͷ�l2vpn�������ڴ棬��������������������ؽ�ҵ��
 �������  : SPM_LOGICAL_PORT_CB *pst_logical_port  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��5��
    ��    ��   : hdjia
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_del_l2vpn_cfg(
    SPM_LOGICAL_PORT_CB *pst_logical_port
    )
{
    NBB_INT         ret = SUCCESS;
    
    NBB_CHAR        uc_message[SPM_MSG_INFO_LEN];

    SPM_VPWS_CB     *pst_vpws = NULL;    
    
    SPM_VPLS_CB     *pst_vpls = NULL;    

    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;

    NBB_USHORT      vpn_id = 0;

    NBB_TRC_ENTRY("spm_logical_port_del_l2vpn_cfg");

    if ((NULL == pst_logical_port) || (NULL == pst_logical_port->l2vpn_cfg_cb))
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port or l2vpn NULL!\n",
            __FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port or l2vpn NULL!\n",
            __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    /* ������vpΪ��Ч���� (hdjia) */
    ret = spm_logical_port_drv_set_vp_invalid(pst_logical_port);

    /* ********************************* (hdjia) */
    /* ���vp��vpws��ϵ (hdjia) */
    /* ********************************** (hdjia) */
    vpn_id = pst_logical_port->l2vpn_cfg_cb->vpn_id;

    if (L2VPN_VPWS == pst_logical_port->l2vpn_cfg_cb->vpn_type)  /* vpws���� (hdjia) */    
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &vpn_id);
        
        if (NULL == pst_vpws)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPWS_ID=%ld does not exist !\n",
                __FUNCTION__, __LINE__, vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPWS_ID=%ld does not exist !\n",
                __FUNCTION__, __LINE__, vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else    /* �ҵ�vpws ��� (hdjia) */
        {
            ret = spm_logical_port_vpbindvpws_proc(
                pst_logical_port->port_index_key, pst_vpws, ATG_DCI_OPER_DEL);            
        }

    }
    else if (L2VPN_VPLS == pst_logical_port->l2vpn_cfg_cb->vpn_type)  /* vpls���� (hdjia) */    
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &vpn_id);
        
        if (NULL == pst_vpls)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPLS_ID=%ld does not exist \n",
                __FUNCTION__, __LINE__, vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPLS_ID=%ld does not exist \n",
                __FUNCTION__, __LINE__, vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else    /* �ҵ�vpls (hdjia) */
        {  
            pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree, 
                &pst_logical_port->l2vpn_cfg_cb->uni_no);

            if (NULL != pst_vpls_uni_cb)
            {
                AVLL_DELETE(pst_vpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);
                
                //�ͷ��ڴ�ռ�
                spm_free_vpls_uni_cb(pst_vpls_uni_cb NBB_CCXT);             
                pst_vpls->uni_num--;    

            } 
            else
            {
                OS_PRINTF(" ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> not find uni_cb!\n",
                    __FUNCTION__, __LINE__,pst_logical_port->port_index_key, vpn_id);
                OS_SPRINTF(uc_message, 
                    " ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> not find uni_cb!\n",
                    __FUNCTION__, __LINE__,pst_logical_port->port_index_key, vpn_id);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_NOT_FOUND;
                goto EXIT_LABEL;

            }

        }

    }

    
    /* ********************************* (hdjia) */
    /* �ͷ�l2vpn������ (hdjia) */
    /* ********************************** (hdjia) */
    NBB_MM_FREE(pst_logical_port->l2vpn_cfg_cb, MEM_SPM_LOG_PORT_L2VPN_CB);
    pst_logical_port->l2vpn_cfg_cb = NULL;

    
    EXIT_LABEL:
        NBB_TRC_EXIT();
        return ret;
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_terminal_if_cb
   ��������  : �˿��߼������ս��ӽӿڽṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_TERMINAL_IF_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��9��5��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_TERMINAL_IF_CB *spm_logical_port_alloc_terminal_if_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_terminal_if_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_terminal_if_cb = (SPM_TERMINAL_IF_CB *)NBB_MM_ALLOC(sizeof(SPM_TERMINAL_IF_CB), NBB_NORETRY_ACT, 
        MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    
    if (pst_terminal_if_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_terminal_if_cb, 0, sizeof(SPM_TERMINAL_IF_CB));

    /* �������ڸ�������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_terminal_if_cb->spm_terminal_if_handle = NBB_CREATE_HANDLE(pst_terminal_if_cb, HDL_SPM_TERMINAL_IF_CB);

    /* �ɹ�����һ���µ�������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_TERMINAL_IF_CB allocated at %p with handle %#lx",
            pst_terminal_if_cb, pst_terminal_if_cb->spm_terminal_if_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_terminal_if_cb->spm_terminal_if_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_terminal_if_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_terminal_if_cb
   ��������  : �ͷŶ˿��߼������ս��ӽӿڽڵ���ڴ�ռ�
   �������  : (SPM_TERMINAL_IF_CB *pstTerminalIf
             NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��9��5��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_terminal_if_cb(SPM_TERMINAL_IF_CB *pst_terminal_if_cb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_logical_port_free_terminal_if_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_terminal_if_cb, sizeof(SPM_TERMINAL_IF_CB), MEM_SPM_LOG_PORT_TERMINAL_IF_CB);

    if (pst_terminal_if_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    
    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_terminal_if_cb->terminal_if_cfg, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
	
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_terminal_if_cb->spm_terminal_if_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_terminal_if_cb, MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    pst_terminal_if_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_terminal_if_key_compare
 ��������  : �ս��ӽӿ�Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��5��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_terminal_if_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    ATG_DCI_LOG_PORT_VLAN *terminal_if_key1;
    ATG_DCI_LOG_PORT_VLAN *terminal_if_key2;

    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    terminal_if_key1 = (ATG_DCI_LOG_PORT_VLAN *) key1;
    terminal_if_key2 = (ATG_DCI_LOG_PORT_VLAN *) key2;
     
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                   */
    /***********************************************************************/
    rc = compare_ushort(&terminal_if_key1->svlan_id, &terminal_if_key2->svlan_id NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ushort(&terminal_if_key1->cvlan_id, &terminal_if_key2->cvlan_id NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}
    
    
	EXIT_LABEL:

    return(rc);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_diff_serv_cb
   ��������  : �˿��߼�����diff_serv�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��1��22��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_DIFF_SERV_CB *spm_logical_port_alloc_diff_serv_cb(NBB_CXT_T NBB_CXT)
{

    SPM_DIFF_SERV_CB *pst_diff_serv_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_diff_serv_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_diff_serv_cb = (SPM_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    
    if (pst_diff_serv_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_diff_serv_cb, 0, sizeof(SPM_DIFF_SERV_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_diff_serv_cb->spm_diff_serv_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_diff_serv_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_diff_serv_cb
  ��������  : �ͷŶ˿��߼�����diff_serv�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��1��22��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_diff_serv_cb(SPM_DIFF_SERV_CB *pst_diff_serv_cb NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_logical_port_free_diff_serv_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_diff_serv_cb, sizeof(SPM_DIFF_SERV_CB), MEM_SPM_LOG_PORT_DIFF_SERV_CB);

    if (pst_diff_serv_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_diff_serv_cb->diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));    

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_diff_serv_cb, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    pst_diff_serv_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_logical_port_diff_serv_key_compare
 ��������  : diff_serv Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��1��27��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_diff_serv_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_key1;
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    diff_serv_key1 = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *) key1;
    diff_serv_key2 = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *) key2;
    
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                   */
    /***********************************************************************/
    rc = compare_ushort(&diff_serv_key1->svlan, &diff_serv_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&diff_serv_key1->cvlan, &diff_serv_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    return(rc);
    
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_flow_diff_serv_cb
   ��������  : �˿��߼�����flow_diff_serv�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_FLOW_DIFF_SERV_CB *spm_logical_port_alloc_flow_diff_serv_cb(NBB_CXT_T NBB_CXT)
{

    SPM_FLOW_DIFF_SERV_CB *pst_flow_diff_serv_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_flow_diff_serv_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    
    if (pst_flow_diff_serv_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_flow_diff_serv_cb, 0, sizeof(SPM_FLOW_DIFF_SERV_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_flow_diff_serv_cb->spm_flow_diff_serv_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_flow_diff_serv_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_flow_diff_serv_cb
  ��������  : �ͷŶ˿��߼�����flow_diff_serv�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_flow_diff_serv_cb(SPM_FLOW_DIFF_SERV_CB *pst_flow_diff_serv_cb NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_logical_port_free_flow_diff_serv_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_flow_diff_serv_cb, sizeof(SPM_FLOW_DIFF_SERV_CB), MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);

    if (pst_flow_diff_serv_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    
    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_flow_diff_serv_cb->flow_diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
    
    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_flow_diff_serv_cb, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    pst_flow_diff_serv_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_logical_port_flow_diff_serv_key_compare
 ��������  : diff_serv Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��3��6��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_flow_diff_serv_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_key1;
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    flow_diff_serv_key1 = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *) key1;
    flow_diff_serv_key2 = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *) key2;
    
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                                                       */
    /***********************************************************************/
    rc = compare_ushort(&flow_diff_serv_key1->svlan, &flow_diff_serv_key2->svlan NBB_CCXT);
      
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&flow_diff_serv_key1->cvlan, &flow_diff_serv_key2->cvlan NBB_CCXT);   
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }


    EXIT_LABEL:
    
    return(rc);
    
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_inclassify_qos_cb
   ��������  : �˿��߼�����inclassify_qos�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pst_logical_port->inclassify_qos_tree,

*****************************************************************************/
SPM_INCLASSIFY_QOS_CB *spm_logical_port_alloc_inclassify_qos_cb(NBB_CXT_T NBB_CXT)
{

    SPM_INCLASSIFY_QOS_CB *pst_tlv_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_inclassify_qos_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_tlv_cfg_cb = (SPM_INCLASSIFY_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_INCLASSIFY_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    
    if (pst_tlv_cfg_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_tlv_cfg_cb, 0, sizeof(SPM_INCLASSIFY_QOS_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tlv_cfg_cb->spm_inclassify_qos_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tlv_cfg_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_inclassify_qos_cb
  ��������  : �ͷŶ˿��߼�����inclassify_qos�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_inclassify_qos_cb(SPM_INCLASSIFY_QOS_CB *pst_tlv_cfg_cb)
{

    NBB_TRC_ENTRY("spm_logical_port_free_inclassify_qos_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tlv_cfg_cb, sizeof(SPM_INCLASSIFY_QOS_CB), MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);

    if (pst_tlv_cfg_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    
    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_tlv_cfg_cb->inclassify_qos_cfg, 0, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
    
    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_tlv_cfg_cb, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    pst_tlv_cfg_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_logical_port_inclassify_qos_key_compare
 ��������  : diff_serv Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��3��6��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_inclassify_qos_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg_key1;
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    pst_tlv_cfg_key1 = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *) key1;
    pst_tlv_cfg_key2 = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *) key2;
    
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                                                       */
    /***********************************************************************/
    rc = compare_ushort(&pst_tlv_cfg_key1->svlan, &pst_tlv_cfg_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&pst_tlv_cfg_key1->cvlan, &pst_tlv_cfg_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
   
    return(rc);
    
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_up_user_qos_cb
   ��������  : �˿��߼�����inclassify_qos�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pst_logical_port->inclassify_qos_tree,

*****************************************************************************/
SPM_UP_USER_QOS_CB *spm_logical_port_alloc_up_user_qos_cb(NBB_CXT_T NBB_CXT)
{

    SPM_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_up_user_qos_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_tlv_cfg_cb = (SPM_UP_USER_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_UP_USER_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
    
    if (NULL == pst_tlv_cfg_cb)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_tlv_cfg_cb, 0, sizeof(SPM_UP_USER_QOS_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tlv_cfg_cb->spm_up_user_qos_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tlv_cfg_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_up_user_qos_cb
  ��������  : �ͷŶ˿��߼�����inclassify_qos�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_up_user_qos_cb(SPM_UP_USER_QOS_CB *pst_tlv_cfg_cb)
{

    NBB_TRC_ENTRY("spm_logical_port_free_up_user_qos_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tlv_cfg_cb, sizeof(SPM_UP_USER_QOS_CB), MEM_SPM_LOG_PORT_UP_USER_QOS_CB);

    if (NULL == pst_tlv_cfg_cb)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    
    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_tlv_cfg_cb->up_user_qos_cfg, 0, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
    
    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_tlv_cfg_cb, MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
    pst_tlv_cfg_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_logical_port_up_user_qos_key_compare
 ��������  : diff_serv Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��3��6��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_up_user_qos_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg_key1;
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    pst_tlv_cfg_key1 = (ATG_DCI_LOG_UP_USER_QOS_POLICY *) key1;
    pst_tlv_cfg_key2 = (ATG_DCI_LOG_UP_USER_QOS_POLICY *) key2;
    
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                                                       */
    /***********************************************************************/
    rc = compare_ushort(&pst_tlv_cfg_key1->svlan, &pst_tlv_cfg_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&pst_tlv_cfg_key1->cvlan, &pst_tlv_cfg_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
   
    return(rc);
    
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_flow_up_user_qos_cb
   ��������  : �˿��߼����ÿ��� 19)����ϻ��û�QOS��������L2���ô���
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pst_logical_port->inclassify_qos_tree,

*****************************************************************************/
SPM_FLOW_UP_USER_QOS_CB *spm_logical_port_alloc_flow_up_user_qos_cb()
{

    SPM_FLOW_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_flow_up_user_qos_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_tlv_cfg_cb = (SPM_FLOW_UP_USER_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_UP_USER_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);
    
    if (NULL == pst_tlv_cfg_cb)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_tlv_cfg_cb, 0, sizeof(SPM_FLOW_UP_USER_QOS_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tlv_cfg_cb->spm_flow_up_user_qos_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tlv_cfg_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_flow_up_user_qos_cb
  ��������  : �ͷŶ˿��߼�����inclassify_qos�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_flow_up_user_qos_cb(SPM_FLOW_UP_USER_QOS_CB *pst_tlv_cfg_cb)
{

    NBB_TRC_ENTRY("spm_logical_port_free_flow_up_user_qos_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tlv_cfg_cb, sizeof(SPM_FLOW_UP_USER_QOS_CB), MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);

    if (NULL == pst_tlv_cfg_cb)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    
    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_tlv_cfg_cb->flow_up_user_qos_cfg, 0, sizeof(SPM_FLOW_UP_USER_QOS_CB));
    
    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_tlv_cfg_cb, MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);
    pst_tlv_cfg_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_logical_port_flow_up_user_qos_key_compare
 ��������  : diff_serv Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��3��6��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_flow_up_user_qos_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg_key1;
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg_key2;
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    pst_tlv_cfg_key1 = (ATG_DCI_LOG_FLOW_UP_USER_QOS *) key1;
    pst_tlv_cfg_key2 = (ATG_DCI_LOG_FLOW_UP_USER_QOS *) key2;
    
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                                                       */
    /***********************************************************************/
    rc = compare_ushort(&pst_tlv_cfg_key1->svlan, &pst_tlv_cfg_key2->svlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_ushort(&pst_tlv_cfg_key1->cvlan, &pst_tlv_cfg_key2->cvlan NBB_CCXT);
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
   
    return(rc);
    
}


/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_traffic_filter_cb
   ��������  : �˿��߼�����traffic_filter�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :(SPM_TRAFFIC_FILTER_CB*) AVLL_NEXT(pst_logical_port->traffic_filter_tree,

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_TRAFFIC_FILTER_CB *spm_logical_port_alloc_traffic_filter_cb(NBB_CXT_T NBB_CXT)
{

    SPM_TRAFFIC_FILTER_CB *pst_traffic_filter_cb = NULL;

    NBB_TRC_ENTRY("spm_logical_port_alloc_traffic_filter_cb");

    /* ����һ���µ�������Ŀ��*/
    pst_traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB *)NBB_MM_ALLOC(sizeof(SPM_TRAFFIC_FILTER_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    
    if (pst_traffic_filter_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_traffic_filter_cb, 0, sizeof(SPM_TRAFFIC_FILTER_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_traffic_filter_cb->spm_traffic_filter_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_traffic_filter_cb);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_free_traffic_filter_cb
  ��������  : �ͷŶ˿��߼�����traffic_filter�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_free_traffic_filter_cb(SPM_TRAFFIC_FILTER_CB *pst_traffic_filter_cb NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_logical_port_free_traffic_filter_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_traffic_filter_cb, sizeof(SPM_TRAFFIC_FILTER_CB), MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

    if (pst_traffic_filter_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_TRAFFIC_FILTER_CB at %p", pst_traffic_filter_cb));
    

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pst_traffic_filter_cb->traffic_filter_cfg, 0, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_traffic_filter_cb, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    pst_traffic_filter_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_logical_port_traffic_filter_key_compare
 ��������  : diff_serv Keyֵ�ȽϺ���
 �������  : NBB_VOID *key1  
             NBB_VOID *key2  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��3��6��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_traffic_filter_key_compare(NBB_VOID *key1, NBB_VOID *key2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *traffic_filter_key1;
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *traffic_filter_key2;   

    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    traffic_filter_key1 = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *) key1;
    traffic_filter_key2 = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *) key2;
        
    rc = compare_ushort(&traffic_filter_key1->svlan, &traffic_filter_key2->svlan NBB_CCXT);
       
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }
    
    rc = compare_ushort(&traffic_filter_key1->cvlan, &traffic_filter_key2->cvlan NBB_CCXT);
    
    
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    return(rc);
    
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_if_intf_cfg_acceptd
   ��������  : ��������ǻ���̣����Ƕ˿ڶ�Ӧ����������Ͳ�Ϊpve��intf��־��Ч
   �������  : NBB_ULONG logical_index
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��8��15��
    ��    ��   : lish
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_if_intf_cfg_acceptd(NBB_ULONG logical_index)
{
    NBB_ULONG physical_port_index = 0;
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;

    pst_logical_port = AVLL_FIND(v_spm_shared->logical_port_tree,&logical_index);

    if((NULL != pst_logical_port)
        && (NULL != pst_logical_port->phy_cfg_cb))
    {
        physical_port_index = pst_logical_port->phy_cfg_cb->phy_port_index;

        pst_physical_port = AVLL_FIND(v_spm_shared->physical_port_tree,&physical_port_index);

        if((NULL != pst_physical_port)
            && (NULL != pst_physical_port->basic_cfg_cb))
        {
           /* if(MIX1LP4 == g_stHardwBoardInf.enBoardName 
            || MIX10LP2 == g_stHardwBoardInf.enBoardName
            || MIX1OLP2 == g_stHardwBoardInf.enBoardName
            || MSA1LN4 == g_stHardwBoardInf.enBoardName)
            {
                if(ATG_DCI_PVE != pst_physical_port->basic_cfg_cb->port_type)
                {
                    return ERROR;
                }
            } */
        }
    }

    return SUCCESS;
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_ipv4_key_compare
   ��������  : ipv4��Ϊkeyֵ�ıȽϺ���
   �������  : NBB_VOID *key1                     
               NBB_VOID *key2 NBB_CCXT_T NBB_CXT  
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��8��15��
    ��    ��   : lish
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_ipv4_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_key1 = NULL;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_key2 = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv4_key_compare");
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    ipv4_key1 = (ATG_DCI_LOG_PORT_IPV4_DATA *) key1;
    ipv4_key2 = (ATG_DCI_LOG_PORT_IPV4_DATA *) key2;
     
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                   */
    /***********************************************************************/
    rc = compare_ulong(&ipv4_key1->ip_addr, &ipv4_key2->ip_addr NBB_CCXT);
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }

    rc = compare_byte(&ipv4_key1->mask_len, &ipv4_key2->mask_len NBB_CCXT);
    if (rc != 0)
    {
        goto EXIT_LABEL;
    }
        
    EXIT_LABEL:
    return(rc);
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_ipv4_key_compare
   ��������  : ipv6��Ϊkeyֵ�ıȽϺ���
   �������  : NBB_VOID *key1                     
               NBB_VOID *key2 NBB_CCXT_T NBB_CXT  
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��8��15��
    ��    ��   : lish
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_ipv6_key_compare(NBB_VOID *key1, NBB_VOID *key2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT rc = 0;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_key1 = NULL;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_key2 = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv6_key_compare");
    
    NBB_ASSERT(key1 != NULL);
    NBB_ASSERT(key2 != NULL);

    /***************************************************************************/
    /* Cast the supplied keys to the correct form.  Note that we cannot assert */
    /* the type of memory because the key structure may be a local variable.   */
    /***************************************************************************/
    ipv6_key1 = (ATG_DCI_LOG_PORT_IPV6_DATA *) key1;
    ipv6_key2 = (ATG_DCI_LOG_PORT_IPV6_DATA *) key2;
     
    /***********************************************************************/
    /* �Ƚ�keyֵ��С                   */
    /***********************************************************************/
    rc = compare_ulong(&ipv6_key1->ip_addr[0], &ipv6_key2->ip_addr[0] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ulong(&ipv6_key1->ip_addr[1], &ipv6_key2->ip_addr[1] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ulong(&ipv6_key1->ip_addr[2], &ipv6_key2->ip_addr[2] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_ulong(&ipv6_key1->ip_addr[3], &ipv6_key2->ip_addr[3] NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}

    rc = compare_byte(&ipv6_key1->mask_len, &ipv6_key2->mask_len NBB_CCXT);
	if (rc != 0)
	{
		goto EXIT_LABEL;
	}
	
	EXIT_LABEL:
    return(rc);
}

/*****************************************************************************
   �� �� ��   : spm_logical_port_check_if_ipv4_exist
   ��������   : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������   : 
   �������   : ��
   �� �� ֵ   : 0 �����ظ�
                n : ���ظ���n=����λ��+1
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_check_if_ipv4_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_IPV4_DATA *pst_ipv4)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_ipv4_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_ipv4)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_IPV4_NUM; loop++)
    {
        if (NULL == pst_logical_port->ipv4_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //���IPV4��ȣ����ش洢λ��
            if (0 == (NBB_MEMCMP(pst_ipv4, pst_logical_port->ipv4_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��   : spm_logical_port_check_if_ipv6_exist
   ��������   : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������   : 
   �������   : ��
   �� �� ֵ   : 0 �����ظ�
                n : ���ظ���n=����λ��+1
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_check_if_ipv6_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_IPV6_DATA *pst_ipv6)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_ipv6_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_ipv6)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_IPV6_NUM; loop++)
    {
        if (NULL == pst_logical_port->ipv6_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //���IPV6��ȣ����ش洢λ��
            if (0 == (NBB_MEMCMP(pst_ipv6, pst_logical_port->ipv6_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��   : spm_logical_port_check_if_mc_ipv4_exist
   ��������   : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������   : 
   �������   : ��
   �� �� ֵ   : 0 �����ظ�
                n : ���ظ���n=����λ��+1
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_check_if_mc_ipv4_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *pst_mc_ipv4)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_mc_ipv4_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_mc_ipv4)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_MC_IPV4_NUM; loop++)
    {
        if (NULL == pst_logical_port->mc_ipv4_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //���IPV4��ȣ����ش洢λ��
            if (0 == (NBB_MEMCMP(pst_mc_ipv4, pst_logical_port->mc_ipv4_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��   : spm_logical_port_check_if_mc_ipv6_exist
   ��������   : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������   : 
   �������   : ��
   �� �� ֵ   : 0 �����ظ�
                n : ���ظ���n=����λ��+1
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_check_if_mc_ipv6_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *pst_mc_ipv6)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_mc_ipv6_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_mc_ipv6)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_MC_IPV6_NUM; loop++)
    {
        if (NULL == pst_logical_port->mc_ipv6_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //���IPV4��ȣ����ش洢λ��
            if (0 == (NBB_MEMCMP(pst_mc_ipv6, pst_logical_port->mc_ipv6_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��   : spm_logical_port_check_if_vip_vmac_exist
   ��������   : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������   : 
   �������   : ��
   �� �� ֵ   : 0 �����ظ�
                n : ���ظ���n=����λ��+1
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_check_if_vip_vmac_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pst_vip_vmac)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_vip_vmac_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_vip_vmac)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; loop++)
    {
        if (NULL == pst_logical_port->vip_vmac_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //���IPV4��ȣ����ش洢λ��
            if (0 == (NBB_MEMCMP(pst_vip_vmac, pst_logical_port->vip_vmac_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��   : spm_logical_port_check_if_vipv6_vmac_exist
   ��������   : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������   : 
   �������   : ��
   �� �� ֵ   : 0 �����ظ�
                n : ���ظ���n=����λ��+1
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_check_if_vipv6_vmac_exist(SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac)
{
    NBB_INT loop = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_check_if_vipv6_vmac_exist");

    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_vipv6_vmac)
    {        
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    for (loop = 0; loop < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; loop++)
    {
        if (NULL == pst_logical_port->vipv6_vmac_cfg_cb[loop])
        {
            continue;
        }
        else
        {
            //���IPV4��ȣ����ش洢λ��
            if (0 == (NBB_MEMCMP(pst_vipv6_vmac, pst_logical_port->vipv6_vmac_cfg_cb[loop],
                sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))))
            {
                ret = loop + 1;
                goto EXIT_LABEL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_add_intf
   ��������  : ɾ���˿��߼����õ�intf
   �������  : SPM_LOGICAL_PORT_CB* pst_logical_port
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2016��1��22��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_add_intf(SPM_INTF_CB *st_intf_cb)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit = 0; 
    
    NBB_TRC_ENTRY("spm_logical_port_add_intf");
       
    if (NULL == st_intf_cb)
    { 
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,st_intf_cb->svlan_id,st_intf_cb->cvlan_id);       
        goto EXIT_LABEL;
    }
   
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;
        
        //����ETHҵ��ҵ������һƬC3�ϣ��Ͱ�posid������������ҵ����
        //�ڶ�ƬC3�ϣ���intf2_pos_id��Ч
        //ve��LAG����ƬC3�϶���
        if((ATG_DCI_LAG == st_intf_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == st_intf_cb->port_sub_type)
            || ((ATG_DCI_LAG != st_intf_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != st_intf_cb->port_sub_type) 
            && (c3_unit == st_intf_cb->unit_id)))
        {
            if(ATG_DCI_L2 == st_intf_cb->port_route_type)
            {
                func_ret = spm_logical_port_drv_add_l2_intf(c3_unit,st_intf_cb);
            } 
            else if(ATG_DCI_L3 == st_intf_cb->port_route_type)
            {
                func_ret = spm_logical_port_drv_add_l3_intf(c3_unit,st_intf_cb);
            }   
            
            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_two_value(ret,c3_unit,func_ret); 
                goto EXIT_LABEL;
            }        
        }
    }
       
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_logical_port_del_cfg
   ��������  : ɾ���˿��߼�����
   �������  : SPM_LOGICAL_PORT_CB* pst_logical_port
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��8��15��
    ��    ��   : lish
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_del_cfg(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_INT loop = 0;
    NBB_ULONG posid = 0; 
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_del_cfg");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);  
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);  
        goto EXIT_LABEL;
    }

    //ɾ��intf
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
        spm_logical_port_apply_del_vlan(pst_logical_port,pst_terminal_if_cb);
    }
        
    //���������ӿڣ�ɾ��intf
    if (pst_logical_port->basic_cfg_cb->port_route_type == ATG_DCI_L3)
    {
        for (loop = 0; loop < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; loop++)
        {
            if (pst_logical_port->vip_vmac_cfg_cb[loop])
            {
                posid = pst_logical_port->logic_port_info_cb.vip_vmac_posid[loop][0];
                func_ret = spm_logical_port_drv_del_acl_vrrp(posid);
                
                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
                    spm_l2_record_err_code(ret);  
                }
            }
        }    
    }
    else if (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
    {
        //ɾ��vp
        if (NULL != pst_logical_port->l2vpn_cfg_cb)
        {
        	if(L2VPN_VPWS == pst_logical_port->l2vpn_cfg_cb->vpn_type)
        	{
        		func_ret = spm_logical_port_apply_del_l2vpn(
                	pst_logical_port,
                	pst_logical_port->l2vpn_cfg_cb);
        	}

			else if(L2VPN_VPLS == pst_logical_port->l2vpn_cfg_cb->vpn_type)
			{
				func_ret = spm_logical_Port_apply_del_vpls_uni(
					pst_logical_port,
                	pst_logical_port->l2vpn_cfg_cb);
			}
            
            
            if (SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
                spm_l2_record_err_code(ret); 
            }
        }
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_set_intf_vrrp
   ��������  : ɾ���˿��߼�����
   �������  : SPM_LOGICAL_PORT_CB* pst_logical_port
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��8��15��
    ��    ��   : lish
    �޸�����   : �����ɺ���
   �޸���ʷ      :
   1.��    ��   : 2016��1��25��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���
*****************************************************************************/
NBB_INT spm_logical_port_set_intf_vrrp(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    NBB_BYTE c3_unit =0; 
    
    NBB_TRC_ENTRY("spm_logical_port_set_intf_vrrp");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        goto EXIT_LABEL;
    }
    
    //����intf VRRP
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {        
            if(0 != pst_terminal_if_cb->intf_pos_id[c3_unit])
            {
                func_ret = SUCCESS;
                func_ret = spm_logical_port_drv_set_intf_vrrp(c3_unit,pst_terminal_if_cb->intf_pos_id[c3_unit]);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,
                        pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                        pst_terminal_if_cb->terminal_if_cfg.cvlan_id,
                        func_ret);
                    break;
                }         
            }
        }    
    }   
    
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_get_vpws_vc_key_from_uni_index;
   ��������  : ���߼��˿�index�ж϶�Ӧ��VPWS��vc key
   �������  : NBB_ULONG ulPortIndex
   �������  : ��
   �� �� ֵ  :  0/-1 = ��������/������ȡ
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��6��9��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_get_vpws_vc_key_from_uni_index(NBB_ULONG index,ATG_DCI_VC_KEY *vc_key)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    SPM_LOGICAL_PORT_CB *pstLogicalport = NULL;
    SPM_VPWS_CB *pstVpws = NULL;
    NBB_USHORT vpws_key = 0;

    if(NULL == vc_key)
    {
        OS_PRINTF("	***ERROR***:(%s:%d)pointer is NULL\n", __FILE__, __LINE__);

        OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)pointer is NULL\n", __FILE__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }

    pstLogicalport = AVLL_FIND(v_spm_shared->logical_port_tree,&index);
    
    if((NULL == pstLogicalport)
    || (NULL == pstLogicalport->basic_cfg_cb))
    {
        OS_PRINTF("	***ERROR***:(%s:%d)INDEX=%ld���߼��˿�����Ϊ��\n", 
            __FILE__, __LINE__, index);

        OS_SPRINTF(ucMessage,"	***ERROR***:(%s:%d)INDEX=%ld���߼��˿�����Ϊ��\n",
            __FILE__,__LINE__,index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }

    if((L2VPN_VPWS != pstLogicalport->logic_port_info_cb.vpn_type)
    || (0 == pstLogicalport->logic_port_info_cb.vpn_id))
    {
        OS_PRINTF("	***ERROR***:(%s:%d)INDEX=%ld���߼��ڲ���VPWS_UNI��\n", 
            __FILE__, __LINE__, index);

        OS_SPRINTF(ucMessage,"	***ERROR***:(%s:%d)INDEX=%ld���߼��ڲ���VPWS_UNI��\n",
            __FILE__,__LINE__,index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }

    vpws_key = pstLogicalport->logic_port_info_cb.vpn_id;

    pstVpws = AVLL_FIND(v_spm_shared->vpws_tree,&vpws_key);

    if(NULL == pstVpws)
    {
        OS_PRINTF("	***ERROR***:(%s:%d)VPWS_ID=%d��VPWS��NNI����Ϊ��\n", 
            __FILE__, __LINE__, vpws_key);

        OS_SPRINTF(ucMessage,"	***ERROR***:(%s:%d)VPWS_ID=%d��VPWS��NNI����Ϊ��\n",
            __FILE__,__LINE__,vpws_key);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        return ERROR;
    }
    else if(0 != pstVpws->vckey_main.vc_id)
    {
        vc_key->vc_id = pstVpws->vckey_main.vc_id;
        vc_key->vc_type = pstVpws->vckey_main.vc_type;
        vc_key->peer_ip = pstVpws->vckey_main.peer_ip;       
    }
    else if(0 != pstVpws->vckey_back.vc_id)
    {
        vc_key->vc_id = pstVpws->vckey_back.vc_id;
        vc_key->vc_type = pstVpws->vckey_back.vc_type;
        vc_key->peer_ip = pstVpws->vckey_back.peer_ip;       
    }

    return SUCCESS;

    
}


/*****************************************************************************
   �� �� ��   : spm_logical_port_get_portid
   ��������   : ���߼��˿����õ�index��ȡ��Ӧ������оƬ��
   �������   : NBB_ULONG ulPortIndex, SPM_PORT_INFO_CB *pstPortInfo
   �������   : ��
   �� �� ֵ   :
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_get_portid(NBB_ULONG ul_port_index, SPM_PORT_INFO_CB *pst_port_info)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    SPM_PORT_INFO_CB st_logic_port_info;
    NBB_BYTE uc_lag_slot_id = 0;
    NBB_USHORT us_lag_port_id = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_logical_port_get_portid");

    if (NULL == pst_port_info)
    {
        OS_PRINTF(" ***ERROR***:(%s:%d)pst_port_info is NULL\n",__FUNCTION__, __LINE__);

        OS_SPRINTF(uc_message, " ***ERROR***:(%s:%d)pst_port_info is NULL\n",__FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&st_logic_port_info, 0, sizeof(SPM_PORT_INFO_CB));

    pst_logical_port = AVLL_FIND(v_spm_shared->logical_port_tree, &ul_port_index);

    //�߼��˿ڲ�����
    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld logical port does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);

        OS_SPRINTF(uc_message, " ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld logical port does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);

        ret = SPM_LOGICAL_E_NOT_FOUND;
        goto EXIT_LABEL;
    }

    //�˿��߼����ô��ڣ��������ã���������/�������ò����ڵĻ���Ҳ�޷���ȡPortId
    if ((NULL == pst_logical_port->basic_cfg_cb) || (NULL == pst_logical_port->phy_cfg_cb))
    {
        OS_PRINTF(" ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:basic config or physical config does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);

        OS_SPRINTF(uc_message, 
            " ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:basic config or physical config does not exist!\n",
            __FUNCTION__, __LINE__,ul_port_index);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);

        ret = SPM_LOGICAL_E_BASIC_CFG_IS_FAIL;
        goto EXIT_LABEL;
    }

    //�ӿڡ��ӽӿڡ�pve��
    if (((pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_SUB_INTFACE)
            || (pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
            || (pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_PVE_PORT))
        && (pst_logical_port->phy_cfg_cb->phy_port_index != 0))
    {
        func_ret = spm_get_portid_from_physical_port_index(pst_logical_port->phy_cfg_cb->phy_port_index,
            &st_logic_port_info NBB_CCXT);
        if (func_ret == SUCCESS)
        {
            pst_port_info->port_id = st_logic_port_info.port_id;
            pst_port_info->slot_id = st_logic_port_info.slot_id;
            pst_port_info->card_id = st_logic_port_info.card_id;
            pst_port_info->unit_id = st_logic_port_info.unit_id;
            pst_port_info->phy_port_type = st_logic_port_info.phy_port_type;
            pst_port_info->port_type = pst_logical_port->basic_cfg_cb->port_sub_type;
            pst_port_info->pve_backup1_slot_id = st_logic_port_info.pve_backup1_slot_id;
            pst_port_info->pve_backup1_port_id = st_logic_port_info.pve_backup1_port_id;
            pst_port_info->pve_backup2_slot_id = st_logic_port_info.pve_backup2_slot_id;
            pst_port_info->pve_backup2_port_id = st_logic_port_info.pve_backup2_port_id;
            pst_port_info->pve_backup3_slot_id = st_logic_port_info.pve_backup3_slot_id;
            pst_port_info->pve_backup3_port_id = st_logic_port_info.pve_backup3_port_id;

        }
        else
        {
            OS_PRINTF(" ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:"
                "call spm_get_portid_from_physical_port_index failed,ret=%d!\n",
                __FUNCTION__, __LINE__,ul_port_index,func_ret);

            OS_SPRINTF(uc_message, 
                " ***ERROR***:(%s:%d)LOGICAL_PORT_INDEX = %ld:"
                "call spm_get_portid_from_physical_port_index failed,ret=%d!\n", 
                __FUNCTION__, __LINE__,ul_port_index,func_ret);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);

            ret = SPM_LOGICAL_E_CALL_PHYSICAL;
            goto EXIT_LABEL;
        }
    }

    //LAG��
    else if ((pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
        && (pst_logical_port->phy_cfg_cb->lag_id != 0))
    {
        //func_ret = spm_link_aggr_get_lag_min_slot_port(pst_logical_port->phy_cfg_cb->lag_id, &uc_lag_slot_id, 
            //&us_lag_port_id NBB_CCXT);

        pst_port_info->port_id = pst_logical_port->phy_cfg_cb->lag_id + ATG_DCI_LAG_OFFSET;
        pst_port_info->slot_id = 0;
        pst_port_info->unit_id = UNIT_0;  
        pst_port_info->port_type = ATG_DCI_LAG;
        pst_port_info->lag_slot_id = uc_lag_slot_id;
        pst_port_info->lag_port_id = us_lag_port_id;
    }

    //L2 L3�Žӿ�
    else if ((pst_logical_port->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)
        && (pst_logical_port->phy_cfg_cb->ve_group_id != 0))
    {
            pst_port_info->port_id = pst_logical_port->phy_cfg_cb->ve_group_id;
            pst_port_info->slot_id = 0;
            pst_port_info->unit_id = UNIT_0;  
            pst_port_info->port_type = ATG_DCI_VE_PORT;

            //����L2ve��L3ve
            if (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                pst_port_info->ve_type = L2_VE;
            }
            else if (ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                pst_port_info->ve_type = L3_VE;
            }
    }

    if (pst_logical_port->basic_cfg_cb != NULL)
    {
        pst_port_info->svlan = pst_logical_port->basic_cfg_cb->control_vid;

    }

    pst_port_info->vp_pos_id = pst_logical_port->logic_port_info_cb.vp_idx;

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   �� �� ��   : spm_vpn_del_vp_uni
   ��������   : ɾ��vp���ͷ�nhi�����Ӧ�ò�洢������Ϣ
   �������   : 
   �������   : ��
   �� �� ֵ   :
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpn_del_vp_uni(SPM_LOGICAL_PORT_CB *pst_logical_port) 
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_UINT vp_idx = 0;
    NBB_BYTE uc_vpn_type;
    NBB_USHORT us_vpn_id;   

    /* �߼��ӿڵļ�ֵ */
    NBB_ULONG ul_port_key = 0;

    NBB_TRC_ENTRY("spm_vpn_del_vp_uni");

    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pointer is NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pointer is NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    vp_idx = pst_logical_port->logic_port_info_cb.vp_idx;
    uc_vpn_type = pst_logical_port->logic_port_info_cb.vpn_type;
    us_vpn_id = pst_logical_port->logic_port_info_cb.vpn_id;
    ul_port_key = pst_logical_port->port_index_key;

#if 0       
    if (0 == pst_l2vpn_data->uni_no)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld from VPN_TYPE=%d VPN_ID=%d error:uni_no ==0!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        OS_SPRINTF(uc_message, 
            " ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld from VPN_TYPE=%d VPN_ID=%d error:uni_no ==0!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_NOT_FOUND;
        goto EXIT_LABEL;

    }
#endif

    if (0 != vp_idx)
    {
        func_ret = spm_logical_port_drv_del_port_to_vpn(uc_vpn_type,us_vpn_id,vp_idx);
        if (SUCCESS != func_ret)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:"
                "call spm_logical_port_del_port_to_vpn failed,ret=%d!\n",
                __FUNCTION__, __LINE__,ul_port_key,func_ret);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:"
                "call spm_logical_port_del_port_to_vpn failed,ret=%d!\n", 
                __FUNCTION__, __LINE__,ul_port_key,func_ret);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_CALL_DRV;
        }

    }
    else
    {
        OS_PRINTF(" ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld  from VPN_TYPE=%d VPN_ID=%d repeatedly!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        OS_SPRINTF(uc_message, 
            " ***ERROR***(%s:%d)del uni LOGICAL_PORT_INDEX=%ld  from VPN_TYPE=%d VPN_ID=%d repeatedly!\n",
            __FUNCTION__, __LINE__,ul_port_key,uc_vpn_type,us_vpn_id);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_NOT_FOUND;
        goto EXIT_LABEL;
    }

    
    if (SUCCESS == ret)
    {
    	//����next_hop_id��Դ
		if (pst_logical_port->logic_port_info_cb.next_hop_id != 0)
		{
	    	func_ret =  spm_vc_free_nhi_id(pst_logical_port->logic_port_info_cb.next_hop_id NBB_CCXT);
             if (SUCCESS != func_ret)
             {
                OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,call spm_vc_free_nhi_id error,ret=%d!\n", 
                    __FUNCTION__, __LINE__,ul_port_key,func_ret);
                OS_SPRINTF(uc_message, 
                    " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,call spm_vc_free_nhi_id error,ret=%d!\n", 
                    __FUNCTION__, __LINE__,ul_port_key,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_VC;
             }
	    }

        //����߼��ӿ������е���������ֵ
        OS_MEMSET(&pst_logical_port->logic_port_info_cb, 0, sizeof(SPM_LOGICAL_PORT_INFO_CB));

    }
    else
    {
        OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:call spm_logical_port_del_port_to_vpn failed!\n",
            __FUNCTION__, __LINE__,ul_port_key);
        OS_SPRINTF(uc_message, 
            " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld:call spm_logical_port_del_port_to_vpn failed!\n", 
            __FUNCTION__, __LINE__,ul_port_key);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_DRV;
        goto EXIT_LABEL;
    }
        

   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_del_uni_from_vpn
   ��������  : Ϊvpnɾ��һ��VP(UNI),���vp��vpn�󶨹�ϵ����l2vpnΪ�գ���˵��
                ֮ǰ��������ϵ�������ֻɾ���������ͷ�nhi��Դ����
   �������  : NBB_BYTE uc_vpn_type
               NBB_USHORT us_vpn_id
               NBB_VOID *pst_uni_data
               SPM_VPN_PORT_INFO_CB *pst_vpn_port_info
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��8��15��
    ��    ��   : lish
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_del_uni_from_vpn(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;

    /* �߼��ӿڵļ�ֵ */
    NBB_ULONG ul_port_key = 0;


    NBB_TRC_ENTRY("spm_logical_port_del_uni_from_vpn");

    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    /* ��l2vpn����Ϊ�գ���˵��vp vpn�󶨹�ϵ�ѽ����ֻ��ɾ����vp�ͷ�nhi��Դ (hdjia) */
    if (NULL == pst_logical_port->l2vpn_cfg_cb)
    {
        ret = spm_vpn_del_vp_uni(pst_logical_port);
        if (SUCCESS != ret)
        {    
            OS_PRINTF(" ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,spm_vpn_del_vp_uni, ret=%d!\n",
                __FUNCTION__, __LINE__,pst_logical_port->port_index_key,ret);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)LOGICAL_PORT_INDEX=%ld,spm_vpn_del_vp_uni, ret=%d!\n",
                __FUNCTION__, __LINE__,pst_logical_port->port_index_key,ret);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_CONFIG;
        }

        /* ������ɺ��˳����� (hdjia) */
        goto EXIT_LABEL;
    }

    /*  *************************************************************** */
    /* ���²�������l2vpn���Դ��ڣ�ɾ���߼���ʱ��ɾ��vp�Ĵ��� (hdjia) */
    /*  *************************************************************** */
    ul_port_key = pst_logical_port->port_index_key;
    pst_l2vpn_data = pst_logical_port->l2vpn_cfg_cb;
    
    /* vpn����ΪVPWS*/
    if(L2VPN_VPWS == pst_l2vpn_data->vpn_type)
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpws)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPWS_ID=%ld does not exist,can not del uni!\n",
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPWS_ID=%ld does not exist,can not del uni!\n", 
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else
        {
            func_ret = spm_vpn_del_vp_uni(pst_logical_port);
            
            //����ɹ�������ac vp��vpn�󶨹�ϵ
            if (SUCCESS == func_ret)
            {
                ret = spm_logical_port_vpbindvpws_proc(ul_port_key, pst_vpws, ATG_DCI_OPER_DEL);
            }
            else //�������UNI��ʧ�ܼ�¼����״̬
            {
                OS_PRINTF("***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPWS_ID=%d>"
                    " spm_vpn_del_vp_uni() VPWS error,ret=%d! \n",
                    __FUNCTION__, __LINE__, ul_port_key, pst_vpws->vpws_id_key,func_ret);
                OS_SPRINTF(uc_message,
                    "***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPWS_ID=%d> spm_vpn_del_vp_uni() VPWS error,ret=%d��\n",
                    __FUNCTION__, __LINE__, ul_port_key, pst_vpws->vpws_id_key,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_LOGICAL;
            }

        }

    }

    /* vpn����ΪVPLS*/
    else if(L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpls)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)VPLS_ID=%ld does not exist,can not del uni!\n",
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)VPLS_ID=%ld does not exist,can not del uni!\n",
                __FUNCTION__, __LINE__,pst_l2vpn_data->vpn_id);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_NOT_FOUND;
            goto EXIT_LABEL;
        }
        else
        {
            func_ret = spm_vpn_del_vp_uni(pst_logical_port);
            
            //����ɹ���ɾ������
            if (SUCCESS == func_ret)
            {
                pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree, &pst_l2vpn_data->uni_no);

                if (NULL != pst_vpls_uni_cb)
                {
    	            AVLL_DELETE(pst_vpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);
                    
    	            //�ͷ��ڴ�ռ�
    	            spm_free_vpls_uni_cb(pst_vpls_uni_cb NBB_CCXT);	            
    				pst_vpls->uni_num--;	

                } 
                else
                {
                    OS_PRINTF(" ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> del uni data error!\n",
                        __FUNCTION__, __LINE__,ul_port_key,pst_l2vpn_data->vpn_id);
                    OS_SPRINTF(uc_message, 
                        " ***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> del uni data error!\n",
                        __FUNCTION__, __LINE__,ul_port_key,pst_l2vpn_data->vpn_id);
                    BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                    ret = SPM_LOGICAL_E_NOT_FOUND;
                    goto EXIT_LABEL;

                }
            }
            else //�������UNI��ʧ�ܼ�¼����״̬
            {
                OS_PRINTF("***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d>"
                    " spm_vpn_del_vp_uni() VPLS error,ret=%d! \n",
                    __FUNCTION__, __LINE__,ul_port_key,pst_vpls->vpls_id_key,func_ret);
                OS_SPRINTF(uc_message,
                    "***ERROR***(%s:%d)<LOGICAL_PORT_INDEX=%ld><VPLS_ID=%d> spm_vpn_del_vp_uni() VPLS error,ret=%d��\n",
                    __FUNCTION__, __LINE__,ul_port_key,pst_vpls->vpls_id_key,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_LOGICAL;
            }


        }

    }
    
   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}


/*****************************************************************************
   �� �� ��   : spm_vpls_add_mc_uni_vp
   ��������   : vpls�鲥���uni vp
   �������   : ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port
   �������   : ��
   �� �� ֵ   :
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_add_mc_uni_vp(SPM_LOGICAL_PORT_CB *pst_logical_port,
    SPM_VPN_PORT_INFO_CB *pst_vpls_port_info NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT vp_id = 0;
    
    NBB_TRC_ENTRY("spm_vpls_add_mc_uni_vp");
    
    if (NULL == pst_logical_port)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port NULL!\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port NULL!\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vpls_port_info)
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_vpls_port_info NULL\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_vpls_port_info NULL\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    //�����߼��˿������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600


   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}


/*****************************************************************************
 �� �� ��  : spm_l2_set_vpn_maxnum_value
 ��������  : ����vpws vpls��֧�ֵ���������
 �������  : NBB_ULONG vpws_maxnum  
             NBB_ULONG vpls_maxnum  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��12��21��
    ��    ��   : hdjia
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_l2_set_vpn_maxnum_value(
    NBB_ULONG vpws_maxnum,  /* �豸��֧�ֵ����vpws��� (hdjia) */
    NBB_ULONG vpls_maxnum   /* �豸��֧�ֵ����vpls��� (hdjia) */
    )
{
    g_l2_vpws_maxnum = vpws_maxnum;
    g_l2_vpls_maxnum = vpls_maxnum;

    return 0;
}


/*****************************************************************************
 �� �� ��  : spm_l2_create_vpn_maxnum
 ��������  : ��ʼ�����������������������vpws vpls
 �������  : NBB_ULONG vpws_maxnum  
             NBB_ULONG vpls_maxnum  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��12��21��
    ��    ��   : hdjia
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_l2_create_vpn_maxnum(
    NBB_ULONG vpws_maxnum,  /* �豸��֧�ֵ����vpws��� (hdjia) */
    NBB_ULONG vpls_maxnum   /* �豸��֧�ֵ����vpls��� (hdjia) */
    )
{
    NBB_INT         ret = SUCCESS;
    NBB_ULONG       vpn_id = 0; 

    NBB_TRC_ENTRY("spm_l2_create_vpn_maxnum_init");

    /* vpws create  (hdjia) */
    for (vpn_id = 1; vpn_id <= g_l2_vpws_maxnum; vpn_id++)
    {
        ret = spm_drv_create_mpls_vpn(L2VPN_VPWS, vpn_id);
    }

    /* VPLS create  (hdjia) */
    for (vpn_id = 1; vpn_id <= g_l2_vpws_maxnum; vpn_id++)
    {
        ret = spm_drv_create_mpls_vpn(L2VPN_VPLS, vpn_id);
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}
    

/*****************************************************************************
 �� �� ��  : spm_l2_vpn_maxnum_init
 ��������  : 1�������豸��֧�ֵ�vpws��vpls�����Ŀ��2������vpn
 �������  : NBB_ULONG vpws_maxnum  
             NBB_ULONG vpls_maxnum  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��12��21��
    ��    ��   : hdjia
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_l2_vpn_maxnum_init(
    NBB_ULONG vpws_maxnum,  /* �豸��֧�ֵ����vpws��� (hdjia) */
    NBB_ULONG vpls_maxnum   /* �豸��֧�ֵ����vpls��� (hdjia) */
    )
{
    spm_l2_set_vpn_maxnum_value(vpws_maxnum, vpls_maxnum);

    spm_l2_create_vpn_maxnum(vpws_maxnum, vpls_maxnum);
}

/*****************************************************************************
   �� �� ��    : spm_logical_port_basic_data_proc
   ��������    : �˿��߼����ÿ�������ô��� 1)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      �������ݿ�ָ�� 
               : NBB_ULONG                  ul_oper_basic          ������
                                   
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���
   2.��    �� : 2016��1��19��
     ��    �� : fansongbo
     �޸����� : 8000����   
*****************************************************************************/
NBB_INT spm_logical_port_basic_data_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_BASIC_DATA *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;    
    NBB_INT ret = SUCCESS; 
        
    NBB_TRC_ENTRY("spm_logical_port_basic_data_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_BASIC_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_BASIC_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->basic_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = LOG_PORT_BASIC_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_BASIC_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  1��Basic Config\n");
                    spm_dbg_print_logical_port_basic_cfg(pst_tlv_cfg);
                }
                              
                OS_SPRINTF(uc_message,"  1��Basic Config\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_basic_cfg(pst_tlv_cfg);
                }
                
                spm_logical_port_apply_set_basic_data(pst_logical_port,pst_tlv_cfg);
                
                /* �������� */
                if (NULL == pst_logical_port->basic_cfg_cb)
                {
                    pst_logical_port->basic_cfg_cb =
                        (ATG_DCI_LOG_PORT_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_BASIC_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_BASIC_CB);

                    if(NULL == pst_logical_port->basic_cfg_cb)
                    {
                        ret = LOG_PORT_BASIC_PROC_ALLOC_CB_FAIL;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }              
                OS_MEMCPY(pst_logical_port->basic_cfg_cb, pst_tlv_cfg, 
                    sizeof(ATG_DCI_LOG_PORT_BASIC_DATA));             
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = LOG_PORT_BASIC_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = LOG_PORT_BASIC_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_BASIC_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��    : spm_logical_port_phy_data_proc
   ��������    : �˿��߼����ÿ��������ô��� 2)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
               : NBB_ULONG              ul_oper_phy            ������
               : SPM_LOGICAL_PORT_CB    *pst_logical_port      �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_phy_data_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_PHY_DATA *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 

    NBB_TRC_ENTRY("spm_logical_port_phy_data_proc");
      
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_PHY_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_PHY_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->physical_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = LOG_PORT_PHY_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret); 
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_PHY_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  2��Physical Config\n");
                    spm_dbg_print_logical_port_phy_cfg(pst_tlv_cfg);
                }

                OS_SPRINTF(uc_message,"  2��Physical Config\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                               
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_phy_cfg(pst_tlv_cfg);
                }
                
                spm_logical_port_apply_set_phy_data(pst_logical_port,pst_tlv_cfg);
                
                /* �������� */
                if (NULL == pst_logical_port->phy_cfg_cb)
                {
                    pst_logical_port->phy_cfg_cb =
                        (ATG_DCI_LOG_PORT_PHY_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_PHY_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_PHY_CB);

                    if(NULL == pst_logical_port->phy_cfg_cb)
                    {
                        ret = LOG_PORT_PHY_PROC_ALLOC_CB_FAIL;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }              
                OS_MEMCPY(pst_logical_port->phy_cfg_cb, pst_tlv_cfg, 
                    sizeof(ATG_DCI_LOG_PORT_PHY_DATA));         
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = LOG_PORT_PHY_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = LOG_PORT_PHY_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_PHY_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    EXIT_LABEL :NBB_TRC_EXIT();
    
    if (NULL != pst_logical_port)
    {
        pst_logical_port->phy_cfg_flag = ret;
    }
    
    return ret;
}

/*****************************************************************************
   �� �� ��    : spm_logical_port_logic_l3_proc
   ��������    : �˿��߼����ÿ��߼�����L3���ô��� 3)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
               : NBB_ULONG                   uloperbasic           ������
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��11��3��
     ��    ��  : lish
     �޸�����  : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_logic_l3_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper_logic_l3)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *pst_logic_l3_data = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_logic_l3_data_start = NULL;
    NBB_BYTE c3_unit = 0; 
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    SPM_PORT_INFO_CB st_port_info;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    NBB_ULONG intf_pos_id;
    
    NBB_TRC_ENTRY("spm_logical_port_logic_l3_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_LOGIC_L3_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_LOGIC_L3_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    switch(ul_oper_logic_l3)
    { 
        case ATG_DCI_OPER_ADD:                          /*����*/
            
            /* �����entry�ĵ�ַ��*/
            puc_logic_l3_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->logical_l3_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_logic_l3_data_start)
            {
                ret = LOG_PORT_LOGIC_L3_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);
            }
            else
            {
                pst_logic_l3_data = (ATG_DCI_LOG_PORT_LOGIC_L3_DATA *)puc_logic_l3_data_start;

                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  3��Logical Config L3\n");
                    spm_dbg_print_logical_port_logic_l3_cfg(pst_logic_l3_data);
                }

                OS_SPRINTF(uc_message, "  3��Logical Config L3\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_logic_l3_cfg(pst_logic_l3_data);
                }                
                
                spm_logical_port_drv_set_logic_l3(pst_logical_port,pst_logic_l3_data);
                
                /* �������� */
                if (NULL == pst_logical_port->logic_l3_cfg_cb)
                {
                    pst_logical_port->logic_l3_cfg_cb =
                        (ATG_DCI_LOG_PORT_LOGIC_L3_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_L3_CB);

                    if (NULL == pst_logical_port->logic_l3_cfg_cb)
                    {
                        ret = LOG_PORT_LOGIC_L3_PROC_ALLOC_CB_FAIL;    
                        spm_l2_record_err_code(ret);       
                        goto EXIT_LABEL;           			
                    }
                }

                OS_MEMCPY(pst_logical_port->logic_l3_cfg_cb, pst_logic_l3_data,
                    sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA));
            }
                        
            break; 
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = LOG_PORT_LOGIC_L3_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = LOG_PORT_LOGIC_L3_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = LOG_PORT_LOGIC_L3_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret);
            break;
    }

    EXIT_LABEL :NBB_TRC_EXIT();
    if (NULL != pst_logical_port)
    {
        pst_logical_port->logical_l3_cfg_flag = ret;
    }
    return ret;
}


/*****************************************************************************
   �� �� ��    : spm_logical_port_phy_proc
   ��������    : �˿��߼����ÿ��߼�����L2���ô��� 4)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ                   
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      �������ݿ�ָ��
               : NBB_ULONG                  uloperbasic            ������
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���
   2.��    �� : 2016��1��14��
     ��    �� : fansongbo
     �޸����� : 8000����    
*****************************************************************************/
NBB_INT spm_logical_port_logic_l2_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper_logic_l2)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_logic_l2_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_logic_l2_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_LOGIC_L2_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_LOGIC_L2_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    switch(ul_oper_logic_l2)
    {
        case ATG_DCI_OPER_ADD:                     /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_logic_l2_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->logical_l2_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_logic_l2_data_start)
            {
                ret = LOG_PORT_LOGIC_L2_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret);              
            }
            else    /*  ���������·�*/
            {
                pst_logic_l2_data = (ATG_DCI_LOG_PORT_LOGIC_L2_DATA *)puc_logic_l2_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  4��Logical Config L2\n");
                    spm_dbg_print_logical_port_logic_l2_cfg(pst_logic_l2_data);
                }

                OS_SPRINTF(uc_message, "  4��Logical Config L2\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_logic_l2_cfg(pst_logic_l2_data);
                }                   
                
                spm_logical_port_apply_set_logic_l2(pst_logical_port,pst_logic_l2_data);
                
                /* �������� */
                if (NULL == pst_logical_port->logic_l2_cfg_cb)
                {
                    pst_logical_port->logic_l2_cfg_cb =
                        (ATG_DCI_LOG_PORT_LOGIC_L2_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_L2_CB);

                    if (NULL == pst_logical_port->logic_l2_cfg_cb)
                    {
                        ret = LOG_PORT_LOGIC_L2_PROC_ALLOC_CB_FAIL;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->logic_l2_cfg_cb, pst_logic_l2_data,
                    sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));               
            }
            break;
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = LOG_PORT_LOGIC_L2_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = LOG_PORT_LOGIC_L2_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_LOGIC_L2_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   �� �� ��    : spm_logical_port_l2vpn_proc
   ��������    : �˿��߼����ÿ�L2VPN���ô��� 5)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ                   
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      �������ݿ�ָ��
               : NBB_ULONG                   ul_oper_l2vpn            ������
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_l2vpn_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper_l2vpn)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_l2vpn_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_BYTE ul_oper_mode = 0;  
	SPM_VPLS_CB *pst_vpls = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_l2vpn_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_L2VPN_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_L2VPN_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
	
    switch(ul_oper_l2vpn)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL: 
            
           /* �����entry�ĵ�ַ��*/
            puc_l2vpn_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->l2vpn_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_l2vpn_data_start)
            {
                ret = LOG_PORT_L2VPN_PROC_GET_DATA_START_FAIL;
                spm_l2_record_err_code(ret); 
            }
            else
            {				
                pst_l2vpn_data = (ATG_DCI_LOG_PORT_L2VPN_DATA *)puc_l2vpn_data_start;
                
                ul_oper_mode = ul_oper_l2vpn - 1;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  5��L2VPN Config %s\n",*(cfg_oper_type_string + ul_oper_mode));                 
                    spm_dbg_print_logical_port_l2vpn_cfg(pst_l2vpn_data);
                }

                OS_SPRINTF(uc_message, "  5��L2VPN Config %s\n",*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                {
                    spm_dbg_record_logical_port_l2vpn_cfg(pst_l2vpn_data);
                }
                
                if(ATG_DCI_OPER_ADD == ul_oper_l2vpn)
                {
                    spm_logical_port_apply_add_l2vpn(pst_logical_port,pst_l2vpn_data);
					
                    
                    /* �������� */
                    if (NULL == pst_logical_port->l2vpn_cfg_cb)
                    {
                        pst_logical_port->l2vpn_cfg_cb =
                            (ATG_DCI_LOG_PORT_L2VPN_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_L2VPN_DATA),
                            NBB_NORETRY_ACT,
                            MEM_SPM_LOG_PORT_L2VPN_CB);

                        if (NULL == pst_logical_port->l2vpn_cfg_cb)
                        {
                            ret = LOG_PORT_L2VPN_PROC_ALLOC_CB_FAIL;
                            spm_l2_record_err_code(ret);
                            goto EXIT_LABEL;
                        }
                    }
                    
                    OS_MEMCPY(pst_logical_port->l2vpn_cfg_cb, pst_l2vpn_data,
                        sizeof(ATG_DCI_LOG_PORT_L2VPN_DATA));    
                }               
                else
                {
                    spm_logical_port_apply_del_l2vpn(pst_logical_port,pst_l2vpn_data);
                    NBB_MM_FREE(pst_logical_port->l2vpn_cfg_cb, MEM_SPM_LOG_PORT_L2VPN_CB);
                    pst_logical_port->l2vpn_cfg_cb = NULL;
                }                
            }            
            break;   
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = LOG_PORT_L2VPN_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_L2VPN_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    
    if (NULL != pst_logical_port)
    {
        pst_logical_port->l2vpn_cfg_flag = ret;
    }
    
    return ret;
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_ces_data_proc
   ��������    : �˿��߼����ÿ��߼�����	�������ô��� 6)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_ces_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ces NBB_CCXT_T NBB_CXT)
{
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_ipv4_data_proc
   ��������    : �˿��߼����ÿ�Ipv4��ַ����L3���ô��� 7)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_ipv4_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ipv4)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_IPV4_DATA *pst_ipv4_data = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_ipv4_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv4_data_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    switch(ul_oper_ipv4)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_ipv4_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv4_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_ipv4_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  7) Ipv4 Address Config L3 (ipv4_num = %d) ADD\n", 
                        pst_set_logical_port->ipv4_num);
                }

                OS_SPRINTF(uc_message, "  7) Ipv4 Address Config L3 (ipv4_num = %d) ADD\n", 
                    pst_set_logical_port->ipv4_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv4_num; loop++)
                {
                    pst_ipv4_data = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                        (puc_ipv4_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv4_cfg(pst_ipv4_data);
                    }
                    
                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv4_cfg(pst_ipv4_data);
                    }

                    //���ipv4�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_ipv4_exist(pst_logical_port, pst_ipv4_data);

                    //����
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV4_NUM; j++)
                        {
                            //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
                            if (NULL == pst_logical_port->ipv4_cfg_cb[j])
                            {
                                pst_logical_port->ipv4_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_IPV4_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_IPV4_CB);
                                
                                if (NULL == pst_logical_port->ipv4_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->ipv4_cfg_cb[j], pst_ipv4_data,
                                    sizeof(ATG_DCI_LOG_PORT_IPV4_DATA));
                                pst_logical_port->ipv4_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //���ظ�����������
                    {
                        OS_MEMCPY(pst_logical_port->ipv4_cfg_cb[icfg_pos - 1], pst_ipv4_data,
                            sizeof(ATG_DCI_LOG_PORT_IPV4_DATA));
                    }
                }               
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_ipv4_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv4_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_ipv4_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  7) Ipv4 Address Config L3 (ipv4_num = %d) DEL\n", pst_set_logical_port->ipv4_num);
                }

                OS_SPRINTF(uc_message, "  7) Ipv4 Address Config L3 (ipv4_num = %d) DEL\n", 
                    pst_set_logical_port->ipv4_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv4_num; loop++)
                {
                    pst_ipv4_data = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                        (puc_ipv4_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv4_cfg(pst_ipv4_data);
                    }

                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv4_cfg(pst_ipv4_data);
                    }
                    
                    //���ipv4�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_ipv4_exist(pst_logical_port, pst_ipv4_data);

                    if(0 < icfg_pos)    //�����ã�����ɾ��
                    {
                        NBB_MM_FREE(pst_logical_port->ipv4_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_IPV4_CB);
                        pst_logical_port->ipv4_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->ipv4_num--;                        
                    }                     
                }               
            } 
            break;
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_ipv6_data_proc
   ��������    : �˿��߼����ÿ�Ipv6��ַ����L3���ô��� 8)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_ipv6_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ipv6)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_IPV6_DATA *pst_ipv6_data = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_ipv6_data_start = NULL;

    /* �߼��ӿڵļ�ֵ */
    NBB_INT ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_ipv6_data_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper_ipv6)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_ipv6_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv6_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_ipv6_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  8) Ipv6 Address Config L3 (ipv6_num = %d) ADD\n", 
                        pst_set_logical_port->ipv6_num);
                }

                OS_SPRINTF(uc_message, "  8) Ipv6 Address Config L3 (ipv6_num = %d) ADD\n", 
                    pst_set_logical_port->ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv6_num; loop++)
                {
                    pst_ipv6_data = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                        (puc_ipv6_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv6_cfg(pst_ipv6_data);
                    }
                    
                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv6_cfg(pst_ipv6_data);
                    }

                    //���ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_ipv6_exist(pst_logical_port, pst_ipv6_data);

                    //����
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV6_NUM; j++)
                        {
                            //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
                            if (NULL == pst_logical_port->ipv6_cfg_cb[j])
                            {
                                pst_logical_port->ipv6_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_IPV6_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_IPV6_CB);
                                
                                if (NULL == pst_logical_port->ipv6_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->ipv6_cfg_cb[j], pst_ipv6_data,
                                    sizeof(ATG_DCI_LOG_PORT_IPV6_DATA));
                                pst_logical_port->ipv6_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //���ظ�����������
                    {
                        OS_MEMCPY(pst_logical_port->ipv6_cfg_cb[icfg_pos - 1], pst_ipv6_data,
                            sizeof(ATG_DCI_LOG_PORT_IPV6_DATA));
                    }
                }               
            }   
            
            break;       
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_ipv6_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ipv6_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_ipv6_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  8) Ipv6 Address Config L3 (ipv6_num = %d) DEL\n", pst_set_logical_port->ipv6_num);
                }

                OS_SPRINTF(uc_message, "  8) Ipv6 Address Config L3 (ipv6_num = %d) DEL\n", 
                    pst_set_logical_port->ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->ipv6_num; loop++)
                {
                    pst_ipv6_data = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                        (puc_ipv6_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_ipv6_cfg(pst_ipv6_data);
                    }

                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_ipv6_cfg(pst_ipv6_data);
                    }
                    
                    //���ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_ipv6_exist(pst_logical_port, pst_ipv6_data);

                    if(0 < icfg_pos)    //�����ã�����ɾ��
                    {
                        NBB_MM_FREE(pst_logical_port->ipv6_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_IPV6_CB);
                        pst_logical_port->ipv6_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->ipv6_num--;                        
                    }                     
                }               
            }   
            break;
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_mc_ipv4_proc
   ��������    : �˿��߼����ÿ�IPV4�鲥���ַL3���ô��� 9)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_mc_ipv4_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_mc_ipv4)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *pst_mc_ipv4 = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_mc_ipv4_start = NULL;

    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_mc_ipv4_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    switch(ul_oper_mc_ipv4)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_mc_ipv4_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv4_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_mc_ipv4_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) ADD\n", 
                        pst_set_logical_port->mc_ipv4_num);
                }

                //OS_SPRINTF(uc_message, "  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) ADD\n", 
                    //pst_set_logical_port->mc_ipv4_num);
                //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv4_num; loop++)
                {
                    pst_mc_ipv4 = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                        (puc_mc_ipv4_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv4_cfg(pst_mc_ipv4);
                    }

                    spm_logical_port_apply_set_mc_ipv4(pst_logical_port,pst_mc_ipv4);
                    
                    //���mc ipv4�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_mc_ipv4_exist(pst_logical_port, pst_mc_ipv4);

                    //����
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV4_NUM; j++)
                        {
                            //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
                            if (NULL == pst_logical_port->mc_ipv4_cfg_cb[j])
                            {
                                pst_logical_port->mc_ipv4_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_MC_IPV4_CB);
                                
                                if (NULL == pst_logical_port->mc_ipv4_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->mc_ipv4_cfg_cb[j], pst_mc_ipv4,
                                    sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA));
                                pst_logical_port->mc_ipv4_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //���ظ�����������
                    {
                        OS_MEMCPY(pst_logical_port->mc_ipv4_cfg_cb[icfg_pos - 1], pst_mc_ipv4,
                            sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA));
                    }
                }               
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_mc_ipv4_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv4_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_mc_ipv4_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) DEL\n", 
                        pst_set_logical_port->mc_ipv4_num);
                }

                //OS_SPRINTF(uc_message, "  9) Mc Ipv4 Address Config L3 (mc ipv4_num = %d) DEL\n", 
                    //pst_set_logical_port->mc_ipv4_num);
                //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv4_num; loop++)
                {
                    pst_mc_ipv4 = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                        (puc_mc_ipv4_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv4_cfg(pst_mc_ipv4);
                    }
                    
                    //���mc ipv4�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_mc_ipv4_exist(pst_logical_port, pst_mc_ipv4);

                    if(0 < icfg_pos)    //�����ã�����ɾ��
                    {
                        NBB_MM_FREE(pst_logical_port->mc_ipv4_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_MC_IPV4_CB);
                        pst_logical_port->mc_ipv4_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->mc_ipv4_num--;                        
                    }                     
                }               
            } 

            break;
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_mc_ipv6_proc
   ��������    : �˿��߼����ÿ�IPV4�鲥���ַL3���ô��� 22)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_mc_ipv6_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_mc_ipv6)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *pst_mc_ipv6 = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_mc_ipv6_start = NULL;

    /* �߼��ӿڵļ�ֵ */
    NBB_USHORT mc_ve_portid = 0;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_mc_ipv6_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    switch(ul_oper_mc_ipv6)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_mc_ipv6_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv6_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_mc_ipv6_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                        pst_set_logical_port->mc_ipv6_num);
                }

                OS_SPRINTF(uc_message, "  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                    pst_set_logical_port->mc_ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv6_num; loop++)
                {
                    pst_mc_ipv6 = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                        (puc_mc_ipv6_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv6_cfg(pst_mc_ipv6);
                    }

                    //����dk�ӿ�
                    spm_logical_port_apply_set_mc_ipv4(pst_logical_port,pst_mc_ipv6);
                        
                    //���mc ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_mc_ipv6_exist(pst_logical_port, pst_mc_ipv6);

                    //����
                    if (0 == icfg_pos)
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV6_NUM; j++)
                        {
                            //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
                            if (NULL == pst_logical_port->mc_ipv6_cfg_cb[j])
                            {
                                pst_logical_port->mc_ipv6_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_MC_IPV6_CB);
                                
                                if (NULL == pst_logical_port->mc_ipv6_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->mc_ipv6_cfg_cb[j], pst_mc_ipv6,
                                    sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA));
                                pst_logical_port->mc_ipv6_num++;
                                break;
                            }
                        }
                    }
                    else if(0 < icfg_pos)    //���ظ�����������
                    {
                        OS_MEMCPY(pst_logical_port->mc_ipv6_cfg_cb[icfg_pos - 1], pst_mc_ipv6,
                            sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA));
                    }
                }               
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_mc_ipv6_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->mc_ipv6_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_mc_ipv6_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                        pst_set_logical_port->mc_ipv6_num);
                }

                OS_SPRINTF(uc_message, "  22) Mc Ipv6 Address Config L3 (mc ipv6_num = %d) ADD\n", 
                    pst_set_logical_port->mc_ipv6_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->mc_ipv6_num; loop++)
                {
                    pst_mc_ipv6 = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                        (puc_mc_ipv6_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_mc_ipv6_cfg(pst_mc_ipv6);
                    }
                    
                    //���mc ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_mc_ipv6_exist(pst_logical_port, pst_mc_ipv6);

                    if(0 < icfg_pos)    //�����ã�����ɾ��
                    {
                        NBB_MM_FREE(pst_logical_port->mc_ipv6_cfg_cb[icfg_pos - 1], MEM_SPM_LOG_PORT_MC_IPV6_CB);
                        pst_logical_port->mc_ipv6_cfg_cb[icfg_pos - 1] = NULL;
                        pst_logical_port->mc_ipv6_num--;                        
                    }                     
                }               
            }
            
            break;
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_vip_vmac_proc
   ��������    : �˿��߼����ÿ�VRRP��MAC��IP��ַ����L3���ô��� 10)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_vip_vmac_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_vip_vmac)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pst_vip_vmac = NULL;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_vip_vmac_start = NULL;
    SPM_VRRP_CB st_vrrp_cb;
  
    /* �߼��ӿڵļ�ֵ */
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    NBB_ULONG terminal_intf_num = 0;  
    NBB_ULONG long_mac[2] = {0};  
    
    NBB_TRC_ENTRY("spm_logical_port_vip_vmac_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    switch(ul_oper_vip_vmac)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_vip_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vip_vmac_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_vip_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vip_vmac_num);
                }

                OS_SPRINTF(uc_message, "  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vip_vmac_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                //�ҵ�vlan���Ե�һ������
                pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);

                //û��vlan���ԣ��޷��󶨣���ӡ����ֱ���˳�
                if(NULL == pst_terminal_if_cb)
                {
                    //�˴��·�������VRRP���þ�����ʧ��
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                }
                else
                {
                    spm_logical_port_set_intf_vrrp(pst_logical_port);                
                }
                
                //��ʼ��Vrrp�Ĺ�����Ա
                st_vrrp_cb.port_id = pst_logical_port->port_id;/*lint !e613 */
                st_vrrp_cb.slot_id = pst_logical_port->slot_id;/*lint !e613 */

                //VE��ֻ����VE��־����VE����Ҫ��vlan��Ϣ
                if(ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)/*lint !e613 */
                {
                    st_vrrp_cb.ve_flag = 1;
                }
                
                for (loop = 0; loop < pst_set_logical_port->vip_vmac_num; loop++)
                {
                    pst_vip_vmac = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                        (puc_vip_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * loop);

                    /* ���ô���todo */
                    if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_logical_port_vip_vmac_cfg(pst_vip_vmac);
                    }

                    st_vrrp_cb.dmac[0] = pst_vip_vmac->mac[0];
                    st_vrrp_cb.dmac[1] = pst_vip_vmac->mac[1];
                    st_vrrp_cb.dmac[2] = pst_vip_vmac->mac[2];
                    st_vrrp_cb.dmac[3] = pst_vip_vmac->mac[3];
                    st_vrrp_cb.dmac[4] = pst_vip_vmac->mac[4];
                    st_vrrp_cb.dmac[5] = pst_vip_vmac->mac[5];   
                    spm_l2_convert_mac_to_ulong(&(pst_vip_vmac->mac[0]),long_mac);                  
                    spm_logical_port_drv_set_acl_vrrp(pst_logical_port,&st_vrrp_cb,&terminal_intf_num); 
                    
                    if(pst_logical_port->terminal_if_num != terminal_intf_num)
                    {
                        pst_set_logical_port->vip_vmac_return_code[loop] = ATG_DCI_RC_UNSUCCESSFUL;                      
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_three_value(ret,long_mac[0],long_mac[1],func_ret);
                        break;
                    }    
                    
                    //���mc ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_vip_vmac_exist(pst_logical_port, pst_vip_vmac);                        

                    //����
                    if ((0 == icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                        {
                            //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
                            if (NULL == pst_logical_port->vip_vmac_cfg_cb[j])
                            {
                                pst_logical_port->vip_vmac_cfg_cb[j] =
                                    (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_VIP_VMAC_CB);
                                
                                if (NULL == pst_logical_port->vip_vmac_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->vip_vmac_cfg_cb[j], pst_vip_vmac,
                                    sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA));
                                pst_logical_port->logic_port_info_cb.vip_vmac_posid[j][0] = st_vrrp_cb.posid[0];
                                pst_logical_port->logic_port_info_cb.vip_vmac_posid[j][1] = st_vrrp_cb.posid[1];
                                pst_logical_port->vip_vmac_num++;
                                break;
                            }
                        }
                    }
                    else if ((0 < icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))    //���ظ�����������
                    {
                        OS_MEMCPY(pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1], pst_vip_vmac,
                            sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA));
                    }
                }                                   
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_vip_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vip_vmac_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_vip_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) DEL\n", 
                        pst_set_logical_port->vip_vmac_num);
                }

                OS_SPRINTF(uc_message, "  10) Vrrp mac Ipv4 address Config L3 (vip_vmac_num = %d) DEL\n", 
                        pst_set_logical_port->vip_vmac_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->vip_vmac_num; loop++)
                {
                    pst_vip_vmac = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                        (puc_vip_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * loop);

                    /* ���ô���todo */
                    if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_logical_port_vip_vmac_cfg(pst_vip_vmac);
                    }
                                            
                    //���mc ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_vip_vmac_exist(pst_logical_port, pst_vip_vmac);                        

                    //���ڣ���Ҫɾ������
                    if (0 < icfg_pos)
                    {
                        func_ret = SUCCESS;
                        func_ret = spm_logical_port_drv_del_acl_vrrp(pst_logical_port,icfg_pos - 1);

                        if(SUCCESS != func_ret)
                        {
                            pst_set_logical_port->vip_vmac_return_code[loop] = ATG_DCI_RC_UNSUCCESSFUL;                      
                        } 
                        
                        if (NULL != pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1])
                        {
                            NBB_MM_FREE(pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1],
                                MEM_SPM_LOG_PORT_VIP_VMAC_CB);
                            pst_logical_port->vip_vmac_cfg_cb[icfg_pos - 1] = NULL;                               
                        }
                        
                        pst_logical_port->vip_vmac_num--;       
                    }
                    else
                    {
                        pst_set_logical_port->vip_vmac_return_code[loop] = ATG_DCI_RC_UNSUCCESSFUL; 
                        ret = SPM_LOGICAL_E_PARAM_NULL;    
                        spm_l2_record_err_code(ret);  
                    }
                }                                   
            }            
            break;    
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_vipv6_vmac_proc
   ��������    : �˿��߼����ÿ�VRRP��MAC��IP��ַ����L3���ô���25)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_vipv6_vmac_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_vipv6_vmac)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac = NULL;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL; 
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_vipv6_vmac_start = NULL;
    SPM_VRRP_CB st_vrrp_cb;
  
    /* �߼��ӿڵļ�ֵ */
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_ULONG j = 0;
    NBB_INT icfg_pos = 0;
    NBB_ULONG terminal_intf_num = 0;  
    NBB_ULONG long_mac[2] = {0};  
    
    NBB_TRC_ENTRY("spm_logical_port_vipv6_vmac_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    

    OS_MEMSET(&st_vrrp_cb, 0, sizeof(SPM_VRRP_CB));   

    switch(ul_oper_vipv6_vmac)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_vipv6_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vrrp_ipv6_mac_l3_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_vipv6_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                }

                OS_SPRINTF(uc_message, "  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                //�ҵ�vlan���Ե�һ������
                pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);

                //û��vlan���ԣ��޷��󶨣���ӡ����ֱ���˳�
                if(NULL == pst_terminal_if_cb)
                {
                    //�˴��·�������VRRP���þ�����ʧ��
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                }
                else
                {
                    spm_logical_port_set_intf_vrrp(pst_logical_port);                
                }
                
                //��ʼ��Vrrp�Ĺ�����Ա
                st_vrrp_cb.port_id = pst_logical_port->port_id;/*lint !e613 */
                st_vrrp_cb.slot_id = pst_logical_port->slot_id;/*lint !e613 */

                //VE��ֻ����VE��־����VE����Ҫ��vlan��Ϣ
                if(ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)/*lint !e613 */
                {
                    st_vrrp_cb.ve_flag = 1;
                }
                
                for (loop = 0; loop < pst_set_logical_port->vrrp_ipv6_mac_l3_num; loop++)
                {
                    pst_vipv6_vmac = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                        (puc_vipv6_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * loop);

                    /* ���ô���todo */
                    if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac);
                    }

                    st_vrrp_cb.dmac[0] = pst_vipv6_vmac->mac_address[0];
                    st_vrrp_cb.dmac[1] = pst_vipv6_vmac->mac_address[1];
                    st_vrrp_cb.dmac[2] = pst_vipv6_vmac->mac_address[2];
                    st_vrrp_cb.dmac[3] = pst_vipv6_vmac->mac_address[3];
                    st_vrrp_cb.dmac[4] = pst_vipv6_vmac->mac_address[4];
                    st_vrrp_cb.dmac[5] = pst_vipv6_vmac->mac_address[5];   
                    spm_l2_convert_mac_to_ulong(&(pst_vipv6_vmac->mac_address[0]),long_mac);                  
                    spm_logical_port_drv_set_acl_vrrp(pst_logical_port,&st_vrrp_cb,&terminal_intf_num); 
                    
                    if(pst_logical_port->terminal_if_num != terminal_intf_num)
                    {                     
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_three_value(ret,long_mac[0],long_mac[1],func_ret);
                        break;
                    }    
                    
                    //���mc ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_vipv6_vmac_exist(pst_logical_port, pst_vipv6_vmac);                        

                    //����
                    if ((0 == icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))
                    {
                        for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                        {
                            //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
                            if (NULL == pst_logical_port->vipv6_vmac_cfg_cb[j])
                            {
                                pst_logical_port->vipv6_vmac_cfg_cb[j] =
                                    (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
                                
                                if (NULL == pst_logical_port->vipv6_vmac_cfg_cb[j])
                                {
                                    ret = SPM_LOGICAL_E_PARAM_NULL;
                                    spm_l2_record_err_code(ret);   
                                    goto EXIT_LABEL;                                   
                                }
                                
                                OS_MEMCPY(pst_logical_port->vipv6_vmac_cfg_cb[j], pst_vipv6_vmac,
                                    sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3));
                                pst_logical_port->logic_port_info_cb.vipv6_vmac_posid[j][0] = st_vrrp_cb.posid[0];
                                pst_logical_port->logic_port_info_cb.vipv6_vmac_posid[j][1] = st_vrrp_cb.posid[1];
                                pst_logical_port->vipv6_vmac_num++;
                                break;
                            }
                        }
                    }
                    else if ((0 < icfg_pos) && (pst_logical_port->terminal_if_num == terminal_intf_num))    //���ظ�����������
                    {
                        OS_MEMCPY(pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1], pst_vipv6_vmac,
                            sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3));
                    }
                }                                   
            }            
            break;       
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_vipv6_vmac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->vrrp_ipv6_mac_l3_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_vipv6_vmac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                }

                OS_SPRINTF(uc_message, "  25) Vrrp mac Ipv6 address Config L3 (vipv6_vmac_num = %d) ADD\n", 
                        pst_set_logical_port->vrrp_ipv6_mac_l3_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                for (loop = 0; loop < pst_set_logical_port->vrrp_ipv6_mac_l3_num; loop++)
                {
                    pst_vipv6_vmac = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                        (puc_vipv6_vmac_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * loop);

                    /* ���ô���todo */
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        //spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac);
                    }
                                            
                    //���mc ipv6�Ƿ����ù�
                    icfg_pos = spm_logical_port_check_if_vipv6_vmac_exist(pst_logical_port, pst_vipv6_vmac);                        

                    //���ڣ���Ҫɾ������
                    if (0 < icfg_pos)
                    {
                        func_ret = SUCCESS;
                        func_ret = spm_logical_port_drv_del_acl_vrrp(pst_logical_port,icfg_pos - 1);

                        if(SUCCESS != func_ret)
                        {
                            pst_set_logical_port->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;                      
                        } 
                        
                        if (NULL != pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1])
                        {
                            NBB_MM_FREE(pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1],
                                MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
                            pst_logical_port->vipv6_vmac_cfg_cb[icfg_pos - 1] = NULL;                               
                        }
                        
                        pst_logical_port->vipv6_vmac_num--;       
                    }
                    else
                    {
                        pst_set_logical_port->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL; 
                        ret = SPM_LOGICAL_E_PARAM_NULL;    
                        spm_l2_record_err_code(ret);  
                    }
                }                                   
            }            
            break;    
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_ve_mac_proc
   ��������    : �˿��߼����ÿ�VE MAC��ַ�������ô���
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_ve_mac_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_ve_mac)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_VE_MAC_DATA *pst_ve_mac = NULL;
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_ve_mac_start = NULL;
  
    /* �߼��ӿڵļ�ֵ */
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_ve_mac_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    
    
    if (NULL == pst_logical_port->phy_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    switch(ul_oper_ve_mac)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_ve_mac_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ve_mac_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_ve_mac_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {               
                pst_ve_mac = (ATG_DCI_LOG_PORT_VE_MAC_DATA *)puc_ve_mac_start;

                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  11��Ve Mac Config\n");
                    spm_dbg_print_logical_port_ve_mac_cfg(pst_ve_mac);
                }

                OS_SPRINTF(uc_message, "  11��Ve Mac Config\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                spm_logical_port_drv_set_ve_port_mac(pst_logical_port,pst_ve_mac);
                
                /* �������� */
                if (NULL == pst_logical_port->ve_mac_cfg_cb)
                {
                    pst_logical_port->ve_mac_cfg_cb =
                        (ATG_DCI_LOG_PORT_VE_MAC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_VE_MAC_CB);
                    
                    if (NULL == pst_logical_port->ve_mac_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_PARAM_NULL;
                        spm_l2_record_err_code(ret);   
                        goto EXIT_LABEL;                                   
                    }
                }

                OS_MEMCPY(pst_logical_port->ve_mac_cfg_cb, pst_ve_mac, sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA));                           
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;  
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;            
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_diff_serv_proc
   ��������    : �˿��߼����ÿ�,12��Diff-Serv���� L3ר��
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_diff_serv_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_DIFF_SERV_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;         
    
    NBB_TRC_ENTRY("spm_logical_port_diff_serv_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL:                           /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->diff_serv_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {     
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  12��Diff-serv Config L3 (num = %d) %s\n",
                        pst_set_logical_port->diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  12��Diff-serv Config L3 (num = %d) %s\n",
                    pst_set_logical_port->diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->diff_serv_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_diff_serv_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //���vlan���ò����ڣ��˳�
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->diff_serv_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_diff_serv_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                            AVLL_INSERT(pst_logical_port->diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_diff_serv_node);
                            pst_logical_port->diff_serv_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                        }                        
                    }
                    else
                    {
                        //������ fansongbo
                        pst_tlv_cfg->l3_in_diff_serv_id = 0;
                        pst_tlv_cfg->l3_out_diff_serv_id = 0;
                        func_ret = spm_logical_port_drv_add_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->diff_serv_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_diff_serv_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_diff_serv_cb(pst_tlv_cfg_cb);
                            pst_logical_port->diff_serv_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_flow_diff_serv_proc
   ��������    : �˿��߼����ÿ������Diff-Serv����L2���ô��� 13)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_flow_diff_serv_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_FLOW_DIFF_SERV_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;     
    
    NBB_TRC_ENTRY("spm_logical_port_flow_diff_serv_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL:                           /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->flow_diff_serv_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {      
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  13��Flow Diff-serv Config L2 (num = %d) %s\n",
                        pst_set_logical_port->flow_diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  13��Flow Diff-serv Config L2 (num = %d) %s\n",
                        pst_set_logical_port->flow_diff_serv_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->flow_diff_serv_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_flow_diff_serv_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //���vlan���ò����ڣ��˳�
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_flow_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_diff_serv_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_flow_diff_serv_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                            AVLL_INSERT(pst_logical_port->flow_diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_flow_diff_serv_node);
                            pst_logical_port->flow_diff_serv_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_diff_serv_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->l2_in_diff_serv_id = 0;
                        func_ret = spm_logical_port_drv_add_flow_diff_serv(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_diff_serv_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->flow_diff_serv_tree, 
                                pst_tlv_cfg_cb->spm_flow_diff_serv_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_flow_diff_serv_cb(pst_tlv_cfg_cb);
                            pst_logical_port->flow_diff_serv_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_inclassify_qos_proc
   ��������    : �˿��߼����ÿ�14����TLV(L2 L3)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_inclassify_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_INCLASSIFY_QOS_CB *pst_tlv_cfg_cb = NULL;   
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE port_route_type = 0;
    NBB_BYTE ul_oper_mode = 0;     
    
    NBB_TRC_ENTRY("spm_logical_port_inclassify_qos_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL:                           /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->inclassify_qos_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  14��Inclassify Qos Config %s (num = %d) %s\n",
                        *(port_route_type_string + port_route_type),
                        pst_set_logical_port->inclassify_qos_num,
                        *(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  14��Inclassify Qos Config %s (num = %d) %s\n",
                    *(port_route_type_string + port_route_type),
                    pst_set_logical_port->inclassify_qos_num,
                    *(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->inclassify_qos_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_inclassify_qos_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //���vlan���ò����ڣ��˳�
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_classify_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->inclassify_qos_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_inclassify_qos_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->inclassify_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                            AVLL_INSERT(pst_logical_port->inclassify_qos_tree, 
                                pst_tlv_cfg_cb->spm_inclassify_qos_node);
                            pst_logical_port->inclassify_qos_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->inclassify_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->qos_policy_index = 0;
                        func_ret = spm_logical_port_drv_add_classify_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->inclassify_qos_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->inclassify_qos_tree, 
                                pst_tlv_cfg_cb->spm_inclassify_qos_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_inclassify_qos_cb(pst_tlv_cfg_cb);
                            pst_logical_port->inclassify_qos_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_up_user_qos_policy_proc
   ��������    : �˿��߼����ÿ�15���ϻ��û�QOS�������� L3ר��
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_up_user_qos_policy_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;      
    
    NBB_TRC_ENTRY("spm_logical_port_up_user_qos_policy_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL:                           /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->up_user_qos_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {   
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  15��UP User Qos Policy Config L3 (num = %d) %s\n",
                        pst_set_logical_port->up_user_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  15��UP User Qos Policy Config L3 (num = %d) %s\n",
                    pst_set_logical_port->up_user_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->up_user_qos_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_UP_USER_QOS_POLICY *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_up_user_qos_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //���vlan���ò����ڣ��˳�
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->up_user_qos_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_up_user_qos_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
                            AVLL_INSERT(pst_logical_port->up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_up_user_qos_node);
                            pst_logical_port->up_user_qos_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->qos_policy_index = 0;
                        func_ret = spm_logical_port_drv_add_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->up_user_qos_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_up_user_qos_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_up_user_qos_cb(pst_tlv_cfg_cb);
                            pst_logical_port->up_user_qos_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_up_user_group_qos_proc
   ��������    : �˿��߼����ÿ��ϻ��û���QOS�����������ô��� 16)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_up_user_group_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper_user_group_qos NBB_CCXT_T NBB_CXT)
{

}

/*****************************************************************************
   �� �� ��     : spm_logical_port_down_user_queue_qos_proc
   ��������    : �˿��߼����ÿ� 17)�»��û�����QOS�������� L2 L3 ���� ��18)��������
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      �������ݿ�ָ�� 
               : NBB_ULONG                  ul_oper_basic          ������
                                   
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���
   2.��    �� : 2016��1��19��
     ��    �� : fansongbo
     �޸����� : 8000����   
*****************************************************************************/
NBB_INT spm_logical_port_down_user_queue_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_BYTE port_route_type = 0;   
    
    NBB_TRC_ENTRY("spm_logical_port_down_user_queue_qos_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->down_user_queue_qos_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)puc_tlv_cfg_data_start;
                
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  17��Down User Qos Policy Config %s ADD\n",
                        *(port_route_type_string + port_route_type));
                    spm_dbg_print_logical_port_down_user_queue_qos_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  17��Down User Qos Policy Config %s ADD\n",
                    *(port_route_type_string + port_route_type));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                func_ret = spm_logical_port_drv_add_down_user_queue_qos(pst_logical_port,pst_tlv_cfg);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_E_CALL_QOS;
                    spm_l2_record_err_code(ret);   
                    goto EXIT_LABEL;
                }
                
                /* �������� */
                if (NULL == pst_logical_port->down_user_qos_cfg_cb)
                {
                    pst_logical_port->down_user_qos_cfg_cb = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);

                    if(NULL == pst_logical_port->down_user_qos_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->down_user_qos_cfg_cb, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));          
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_down_user_group_qos_proc
   ��������    : �˿��߼����ÿ� 18)�»��û���QOS�������ô���  L2 l3����
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
               : SPM_LOGICAL_PORT_CB        *pst_logical_port      �������ݿ�ָ�� 
               : NBB_ULONG                  ul_oper_basic          ������
                                   
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���
   2.��    �� : 2016��1��19��
     ��    �� : fansongbo
     �޸����� : 8000����   
*****************************************************************************/
NBB_INT spm_logical_port_down_user_group_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_down_user_group_qos_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->down_user_queue_qos_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  18��Down User Group Qos Config ADD\n");
                    spm_dbg_print_logical_port_down_user_group_qos_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  18��Down User Group Qos Config ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                func_ret = spm_logical_port_drv_add_down_user_group_qos(pst_logical_port,pst_tlv_cfg);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_E_CALL_QOS;
                    spm_l2_record_err_code(ret);   
                    goto EXIT_LABEL;
                }
                
                /* �������� */
                if (NULL == pst_logical_port->down_user_group_qos_cfg_cb)
                {
                    pst_logical_port->down_user_group_qos_cfg_cb = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);

                    if(NULL == pst_logical_port->down_user_group_qos_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->down_user_group_qos_cfg_cb, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS));
                
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��     : spm_logical_port_flow_up_user_qos_proc
   ��������    : �˿��߼����ÿ��� 19)����ϻ��û�QOS��������L2���ô���
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_flow_up_user_qos_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_FLOW_UP_USER_QOS_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE ul_oper_mode = 0;  
    
    NBB_TRC_ENTRY("spm_logical_port_flow_up_user_qos_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL:                           /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->flow_upuser_qos_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {    
                ul_oper_mode = ul_oper - 1;
            
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  19) Flow Up User Qos Config L2 (num = %d) %s\n",
                        pst_set_logical_port->flow_upuser_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  19) Flow Up User Qos Config L2 (num = %d) %s\n",
                    pst_set_logical_port->flow_upuser_qos_num,*(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (loop = 0; loop < pst_set_logical_port->flow_upuser_qos_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_FLOW_UP_USER_QOS *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_flow_upuser_qos(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //���vlan���ò����ڣ��˳�
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_flow_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_up_user_qos_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_flow_up_user_qos_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS));
                            AVLL_INSERT(pst_logical_port->flow_up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_flow_up_user_qos_node);
                            pst_logical_port->flow_up_user_qos_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->flow_up_user_qos_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->qos_policy_index = 0;
                        func_ret = spm_logical_port_drv_add_flow_up_user_qos(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->flow_up_user_qos_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->flow_up_user_qos_tree, 
                                pst_tlv_cfg_cb->spm_flow_up_user_qos_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_flow_up_user_qos_cb(pst_tlv_cfg_cb);
                            pst_logical_port->flow_up_user_qos_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_traffic_filter_proc
   ��������    : �˿��߼����ÿ��������traffic_filter���ô��� 20) L2 L3����
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_traffic_filter_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
     SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pst_tlv_cfg = NULL;
    ATG_DCI_LOG_PORT_VLAN terminal_if_key;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_TRAFFIC_FILTER_CB *pst_tlv_cfg_cb = NULL; 
    
    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    NBB_ULONG loop = 0;
    NBB_BYTE port_route_type = 0;
    NBB_BYTE ul_oper_mode = 0;    
    
    NBB_TRC_ENTRY("spm_logical_port_traffic_filter_proc");  
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    OS_MEMSET(&terminal_if_key,0,sizeof(ATG_DCI_LOG_PORT_VLAN)); 
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                          /*����*/
        case ATG_DCI_OPER_DEL:                           /*ɾ��*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->traffic_filter_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);   
            }
            else
            {  
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                ul_oper_mode = ul_oper - 1;
                
                if (g_logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  20) Traffic Filter Qos Config %s (num = %d) %s\n",
                        *(port_route_type_string + port_route_type),
                        pst_set_logical_port->traffic_filter_num,
                        *(cfg_oper_type_string + ul_oper_mode));
                }

                OS_SPRINTF(uc_message, "  20) Traffic Filter Qos Config %s (num = %d) %s\n",
                    *(port_route_type_string + port_route_type),
                    pst_set_logical_port->traffic_filter_num,
                    *(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);       
                
                for (loop = 0; loop < pst_set_logical_port->traffic_filter_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)(puc_tlv_cfg_data_start
                        + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER))) * loop);   
                    
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_traffic_filter_cfg(pst_tlv_cfg);
                    }
                    
                    terminal_if_key.svlan_id = pst_tlv_cfg->svlan;
                    terminal_if_key.cvlan_id = pst_tlv_cfg->cvlan;
                    pst_terminal_if_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, &terminal_if_key);

                    //���vlan���ò����ڣ��˳�
                    if (NULL == pst_terminal_if_cb)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                        spm_l2_record_err_code_and_two_value(ret,terminal_if_key.svlan_id,
                            terminal_if_key.cvlan_id);
                        goto EXIT_LABEL;
                    }
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        func_ret = spm_logical_port_drv_add_traffic_filter(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->traffic_filter_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_traffic_filter_cb();

                            if (NULL == pst_tlv_cfg_cb)/*lint !e613 */
                            {
                                ret = SPM_LOGICAL_E_CALL_QOS;
                                spm_l2_record_err_code(ret);   
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->traffic_filter_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                            AVLL_INSERT(pst_logical_port->traffic_filter_tree, 
                                pst_tlv_cfg_cb->spm_traffic_filter_node);
                            pst_logical_port->traffic_filter_num++;
                        }
                        else	
                        {
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->traffic_filter_cfg),
                                pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                        }                        
                    }
                    else
                    {
                        pst_tlv_cfg->acl_id = 0;
                        func_ret = spm_logical_port_drv_add_traffic_filter(pst_logical_port,pst_tlv_cfg,
                            pst_terminal_if_cb);

                        if(SUCCESS != func_ret)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        
                        pst_tlv_cfg_cb = 
                            AVLL_FIND(pst_logical_port->traffic_filter_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = SPM_LOGICAL_E_CALL_QOS;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            AVLL_DELETE(pst_logical_port->traffic_filter_tree, 
                                pst_tlv_cfg_cb->spm_traffic_filter_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_traffic_filter_cb(pst_tlv_cfg_cb);
                            pst_logical_port->traffic_filter_num--;	
                        }                        
                    }                   
                }         
            }            
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��    : spm_logical_port_vlan_proc
   ��������    : �˿��߼����ÿ�VLAN����L2���ô���
   �������    : ATG_DCI_SET_LOGICAL_PORT  *pst_set_logical_port  IPS��Ϣ
               : SPM_LOGICAL_PORT_CB       *pst_logical_port      �������ݿ�ָ��         
   �������    : ��
   �� �� ֵ    :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��  : 2015��8��6��
     ��    ��  : lish
     �޸�����  : �����ɺ���

*****************************************************************************/
NBB_INT spm_logical_port_vlan_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_VLAN *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    SPM_TERMINAL_IF_CB *pst_tlv_cfg_cb = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_ULONG loop = {0};
    NBB_BOOL avll_ret_code;
    NBB_BYTE port_route_type = 0;
    NBB_BYTE ul_oper_mode = 0;    
     
    NBB_TRC_ENTRY("spm_logical_port_vlan_proc"); 
    
    if (NULL == pst_set_logical_port)
    {
        ret = LOG_PORT_VLAN_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_VLAN_PROC_LOCAL_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_VLAN_PROC_LOCAL_BASIC_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*����*/
        case ATG_DCI_OPER_DEL:                      /*ɾ��*/
            
            /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(
                pst_set_logical_port,
                &pst_set_logical_port->vlan_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = LOG_PORT_VLAN_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
            }
            else
            {       
                if (3 > pst_logical_port->basic_cfg_cb->port_route_type)
                {
                    port_route_type = pst_logical_port->basic_cfg_cb->port_route_type;
                }
                else
                {
                    port_route_type = 3;
                }
                
                ul_oper_mode = ul_oper - 1;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  21��VLAN %s (num = %d) %s\n", 
                        port_route_type_string [port_route_type],
                        pst_set_logical_port->vlan_num,
                        *(cfg_oper_type_string + ul_oper_mode));
                    
                }
                
                OS_SPRINTF(uc_message, "  21��VLAN %s (num = %d) %s\n", 
                    *(port_route_type_string + port_route_type),
                    pst_set_logical_port->vlan_num,
                    *(cfg_oper_type_string + ul_oper_mode));
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (loop = 0; loop < pst_set_logical_port->vlan_num; loop++)
                {
                    pst_tlv_cfg = (ATG_DCI_LOG_PORT_VLAN*)
                        (puc_tlv_cfg_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * loop);
                        
                    if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_print_logical_port_terminal_if_cfg(pst_tlv_cfg);
                    }
                    
                    if (SPM_PRINT_DETAIL == g_logical_port_cfg_print_setting)
                    {
                        spm_dbg_record_logical_port_terminal_if_cfg(pst_tlv_cfg);
                    }  
                    
                    if(ATG_DCI_OPER_ADD == ul_oper)
                    {
                        spm_logical_port_apply_add_vlan(pst_logical_port,pst_tlv_cfg);
                        
                        pst_tlv_cfg_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            pst_tlv_cfg_cb = spm_logical_port_alloc_terminal_if_cb(NBB_CXT);

                            if(NULL == pst_tlv_cfg_cb)
                            {
                                ret = LOG_PORT_VLAN_PROC_ALLOC_CB_FAIL;
                                spm_l2_record_err_code_and_two_value(ret,
                                    pst_tlv_cfg->svlan_id,pst_tlv_cfg->cvlan_id);
                                goto EXIT_LABEL;
                            }

                            //�������ݲ���������
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->terminal_if_cfg), pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_VLAN));
                            NBB_MEMCPY(&(pst_tlv_cfg_cb->intf_pos_id), &(pst_logical_port->temp_intf_pos_id), 2);
                            avll_ret_code = AVLL_INSERT(pst_logical_port->terminal_if_tree, 
                                pst_tlv_cfg_cb->spm_terminal_if_node);
                            NBB_ASSERT(avll_ret_code);
                            pst_logical_port->terminal_if_num++;
                        }
                        
                        NBB_MEMCPY(&(pst_tlv_cfg_cb->terminal_if_cfg), pst_tlv_cfg, sizeof(ATG_DCI_LOG_PORT_VLAN));
                        NBB_MEMCPY(&(pst_tlv_cfg_cb->intf_pos_id), &(pst_logical_port->temp_intf_pos_id), 8);
                    }
                    else
                    {
                        pst_tlv_cfg_cb = AVLL_FIND(pst_logical_port->terminal_if_tree, pst_tlv_cfg);

                        //��������ڣ��������ڵ㣬�������У�������1
                        if (NULL == pst_tlv_cfg_cb)
                        {
                            ret = LOG_PORT_VLAN_PROC_FIND_DEL_CB_NULL;
                            spm_l2_record_err_code(ret);   
                            goto EXIT_LABEL;
                        }
                        else	
                        {
                            spm_logical_port_apply_del_vlan(pst_logical_port,pst_tlv_cfg_cb);
                            
                            AVLL_DELETE(pst_logical_port->terminal_if_tree, pst_tlv_cfg_cb->spm_terminal_if_node);

                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                            spm_logical_port_free_terminal_if_cb(pst_tlv_cfg_cb);

                            pst_logical_port->terminal_if_num--;	
                        }   
                    }
                }            
            }
            break;
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = LOG_PORT_VLAN_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = LOG_PORT_VLAN_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;           
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_ds_l2_data_proc
   ��������    : �˿��߼����ÿ�Diff-Serv����L2��VP�»�UNI�ࣩ���ô��� 23)
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_ds_l2_data_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_DS_L2_DATA *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_ds_l2_data_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->ds_l2_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_DS_L2_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  23��Diff-serv Config L2 ADD\n");
                    spm_dbg_print_logical_port_ds_l2_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  23��Diff-serv Config L2 ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                spm_logical_port_drv_add_ds_l2(pst_logical_port,pst_tlv_cfg);

                /* �������� */
                if (NULL == pst_logical_port->ds_l2_cfg_cb)
                {
                    pst_logical_port->ds_l2_cfg_cb = (ATG_DCI_LOG_PORT_DS_L2_DATA *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_DS_L2_CB);

                    if(NULL == pst_logical_port->ds_l2_cfg_cb)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->ds_l2_cfg_cb, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA));
                
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��     : spm_logical_port_car_data_proc
   ��������    : �˿��߼����ÿ�car�������ô��� 24) L2
   �������    : ATG_DCI_SET_LOGICAL_PORT   *pst_set_logical_port  IPS��Ϣ
                   : NBB_ULONG                              uloperbasic             ������
                   : SPM_LOGICAL_PORT_CB            *pst_logical_port        �������ݿ�ָ��                                    
   �������    : ��
   �� �� ֵ     :
   ���ú���    :
   ��������    :
   �޸���ʷ    :
   1.��    ��   : 2015��8��6��
      ��    ��   : fansongbo
      �޸����� : �����ɺ���

*****************************************************************************/
NBB_VOID spm_logical_port_car_data_proc(
    ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port,
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
 
    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_LOG_PORT_CAR_DATA *pst_tlv_cfg = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;  
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_car_data_proc");
    
    if (NULL == pst_set_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:                      /*����*/
            
           /* �����entry�ĵ�ַ��*/
            puc_tlv_cfg_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_logical_port,
                &pst_set_logical_port->car_data);   

            /* �׵�ַΪNULL���쳣 */
            if (NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;
                spm_l2_record_err_code(ret);  
            }
            else
            {
                pst_tlv_cfg = (ATG_DCI_LOG_PORT_CAR_DATA *)puc_tlv_cfg_data_start;
                
                if (SPM_PRINT_CFG == g_logical_port_cfg_print_setting)
                {
                    printf("  24��car Config L2 ADD\n");
                    spm_dbg_print_logical_port_car_cfg(pst_tlv_cfg);
                }
                
                OS_SPRINTF(uc_message,"  24��car Config L2 ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                spm_logical_port_drv_add_car(pst_logical_port,pst_tlv_cfg);
    
                /* �������� */
                if (NULL == pst_logical_port->car_cfg)
                {
                    pst_logical_port->car_cfg = (ATG_DCI_LOG_PORT_CAR_DATA *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_LOG_PORT_CAR_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_CAR_CB);

                    if(NULL == pst_logical_port->car_cfg)
                    {
                        ret = SPM_LOGICAL_E_MEMORY;
                        spm_l2_record_err_code(ret);
                        goto EXIT_LABEL;
                    }
                }
                
                OS_MEMCPY(pst_logical_port->car_cfg, pst_tlv_cfg,
                    sizeof(ATG_DCI_LOG_PORT_CAR_DATA));               
            }            
            break;     
        case ATG_DCI_OPER_DEL:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;       
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL :NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_init_vp_tree
   ��������  : ��ʼ���洢VPWS���õ���
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

 �޸���ʷ      :
  1.��    ��   : 2015��8��31��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_vp_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vp_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_VP_CB, vp_idx);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vp_tree, compare_ushort,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_VP_CB, spm_vp_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   �� �� ��  : spm_free_vp_cb
   ��������  : �ͷ�VPWS�ṹ����ڴ�
   �������  : SPM_VPWS_CB *pstVpws NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

 �޸���ʷ      :
  1.��    ��   : 2015��8��31��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_vp_cb(SPM_VPWS_CB *pst_vp)
{
    NBB_TRC_ENTRY("spm_free_vp_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_vp, sizeof(SPM_VP_CB), MEM_SPM_VP_CB);

    if (NULL == pst_vp)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vp, MEM_SPM_VP_CB);
    pst_vp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_alloc_vp_cb
   ��������  : VPWS�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_VPWS_CB
   ���ú���  :
   ��������  :

 �޸���ʷ      :
  1.��    ��   : 2015��8��31��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VP_CB *spm_alloc_vp_cb()
{
    SPM_VP_CB *pst_vp = NULL;

    NBB_TRC_ENTRY("spm_alloc_vp_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pst_vp = (SPM_VP_CB *)NBB_MM_ALLOC(sizeof(SPM_VP_CB), NBB_NORETRY_ACT, MEM_SPM_VP_CB);
    
    if (NULL == pst_vp)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pst_vp, 0, sizeof(SPM_VP_CB));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vp->spm_vp_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_vp);
}


/*****************************************************************************
   �� �� ��  : spm_logical_port_alloc_logical_port_cb
   ��������  : �˿��߼����ýṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_LOGICAL_PORT_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_LOGICAL_PORT_CB *spm_alloc_logical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    NBB_BUF_SIZE avll_key_offset;
    NBB_INT i = 0;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_alloc_logical_port_cb");

    /* ����һ���µ��߼��ӿ�������Ŀ��*/
    pst_logical_port = (SPM_LOGICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_LOGICAL_PORT_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_LOGICAL_PORT_CB);
    
    if (NULL == pst_logical_port)
    {    
        goto EXIT_LABEL;
    }

    /* ��ʼ�� */
    OS_MEMSET(pst_logical_port, 0, sizeof(SPM_LOGICAL_PORT_CB));

    /* ��ʼ��ָ�� */
    pst_logical_port->basic_cfg_cb = NULL;
    pst_logical_port->phy_cfg_cb = NULL;
    pst_logical_port->logic_l3_cfg_cb = NULL;
    pst_logical_port->logic_l2_cfg_cb = NULL;
    pst_logical_port->l2vpn_cfg_cb = NULL;
    pst_logical_port->ces_cfg_cb = NULL;
    pst_logical_port->ve_mac_cfg_cb = NULL;
    pst_logical_port->up_user_group_qos_cfg_cb = NULL;
    pst_logical_port->down_user_qos_cfg_cb = NULL;
    pst_logical_port->down_user_group_qos_cfg_cb = NULL;
    pst_logical_port->ds_l2_cfg_cb = NULL;
    pst_logical_port->car_cfg = NULL;

    /***************************************************************************/
    /* ��ʼ��IPV4�鲥���ַL3����                                              */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        pst_logical_port->ipv4_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ��IPv6��ַ����L3����                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        pst_logical_port->ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ��IPV4�鲥����                                                      */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        pst_logical_port->mc_ipv4_cfg_cb[i] = NULL;
    }

	/***************************************************************************/
    /* ��ʼ��IPV6�鲥����                                                      */
    /***************************************************************************/
	for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        pst_logical_port->mc_ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ��VRRP��MAC��IP��ַ����L3����                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pst_logical_port->vip_vmac_cfg_cb[i] = NULL;
    }
    
    /***************************************************************************/
    /* ��ʼ��VRRP��MAC��IPV6��ַ����L3����                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pst_logical_port->vipv6_vmac_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_TERMINAL_IF_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->terminal_if_tree, spm_logical_port_terminal_if_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, terminal_if_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, spm_terminal_if_node));


    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_DIFF_SERV_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->diff_serv_tree, spm_logical_port_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, diff_serv_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, spm_diff_serv_node));

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_FLOW_DIFF_SERV_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->flow_diff_serv_tree, spm_logical_port_flow_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, flow_diff_serv_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, spm_flow_diff_serv_node));

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_INCLASSIFY_QOS_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->inclassify_qos_tree, spm_logical_port_inclassify_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, inclassify_qos_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, spm_inclassify_qos_node));
    
    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_UP_USER_QOS_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->up_user_qos_tree, spm_logical_port_up_user_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_UP_USER_QOS_CB, up_user_qos_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_UP_USER_QOS_CB, spm_up_user_qos_node));

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_UP_USER_QOS_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->flow_up_user_qos_tree, spm_logical_port_flow_up_user_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_UP_USER_QOS_CB, flow_up_user_qos_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_UP_USER_QOS_CB, spm_flow_up_user_qos_node));

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_TRAFFIC_FILTER_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_logical_port->traffic_filter_tree, spm_logical_port_traffic_filter_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, traffic_filter_cfg),
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, spm_traffic_filter_node));  

    
    /* �������ڸ��߼��ӿ�������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_logical_port->spm_logical_port_handle = NBB_CREATE_HANDLE(pst_logical_port,
        HDL_SPM_LOGICAL_PORT_CB);

    /* �ɹ�����һ���µ��߼��ӿ�������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_LOGICAL_PORT_CB allocated at %p with handle %#lx",
            pst_logical_port, pst_logical_port->spm_logical_port_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_logical_port->spm_logical_port_node);
    
	EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_logical_port);
}


/*****************************************************************************
   �� �� ��  : spm_logical_port_free_logical_port_cb
   ��������  : �ͷŶ˿��߼����ýڵ���ڴ�ռ�
   �������  : SPM_LOGICAL_PORT_CB *pst_logical_port
               NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��8��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_logical_port_cb(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    SPM_DIFF_SERV_CB *pst_diff_serv_cb = NULL;   
    SPM_FLOW_DIFF_SERV_CB *pst_flow_diff_serv_cb = NULL; 
    SPM_INCLASSIFY_QOS_CB *pst_inclassify_qos_cb = NULL; 
    SPM_UP_USER_QOS_CB *pst_up_user_qos_cb = NULL;  
    SPM_FLOW_UP_USER_QOS_CB *pst_flow_up_user_qos_cb = NULL; 
    SPM_TRAFFIC_FILTER_CB *pst_traffic_filter_cb = NULL;     
    AVLL_NODE node = {0};

    NBB_TRC_ENTRY("spm_free_logical_port_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_logical_port, sizeof(SPM_LOGICAL_PORT_CB), MEM_SPM_LOGICAL_PORT_CB);

    if (pst_logical_port == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    if (pst_logical_port->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->basic_cfg_cb, MEM_SPM_LOG_PORT_BASIC_CB);
        pst_logical_port->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ������������ݿ顣                                                    */
    /***************************************************************************/
    if (pst_logical_port->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->phy_cfg_cb, MEM_SPM_LOG_PORT_PHY_CB);
        pst_logical_port->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��߼�����L3���ݿ顣                                                  */
    /***************************************************************************/
    if (pst_logical_port->logic_l3_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->logic_l3_cfg_cb, MEM_SPM_LOG_PORT_L3_CB);
        pst_logical_port->logic_l3_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��߼�����L2���ݿ顣                                                  */
    /***************************************************************************/
    if (pst_logical_port->logic_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->logic_l2_cfg_cb, MEM_SPM_LOG_PORT_L2_CB);
        pst_logical_port->logic_l2_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�L2VPN���ݿ顣                                                  */
    /***************************************************************************/
    if (pst_logical_port->l2vpn_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->l2vpn_cfg_cb, MEM_SPM_LOG_PORT_L2VPN_CB);
        pst_logical_port->l2vpn_cfg_cb = NULL;
    }
	
    /***************************************************************************/
    /* �ͷ��߼�����CES/CEP���ݿ顣                                             */
    /***************************************************************************/
    if (pst_logical_port->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->ces_cfg_cb, MEM_SPM_LOG_PORT_CES_CB);
        pst_logical_port->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�VE MAC��ַ���ݿ顣                                             */
    /***************************************************************************/
    if (pst_logical_port->ve_mac_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->ve_mac_cfg_cb, MEM_SPM_LOG_PORT_VE_MAC_CB);
        pst_logical_port->ve_mac_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* �ͷ��ϻ��û���QOS�����������ô����������ݿ顣                               */
    /***************************************************************************/
    if (pst_logical_port->up_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->up_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB);
        pst_logical_port->up_user_group_qos_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* �ͷ��»��û�����QOS��������L3�������ݿ顣                               */
    /***************************************************************************/
    if (pst_logical_port->down_user_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->down_user_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
        pst_logical_port->down_user_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��»��û���QOS����L3�������ݿ顣                                     */
    /***************************************************************************/
    if (pst_logical_port->down_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->down_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
        pst_logical_port->down_user_group_qos_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* �ͷ�Diff-Serv����L2��VP�»�UNI�ࣩ�������ݿ顣                          */
    /***************************************************************************/
    if (pst_logical_port->ds_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_logical_port->ds_l2_cfg_cb, MEM_SPM_LOG_PORT_DS_L2_CB);
        pst_logical_port->ds_l2_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�CAR�������ݿ顣                                         */
    /***************************************************************************/
    if (pst_logical_port->car_cfg != NULL)
    {
        NBB_MM_FREE(pst_logical_port->car_cfg, MEM_SPM_LOG_PORT_CAR_CB);
        pst_logical_port->car_cfg = NULL;
    }
    
    /***************************************************************************/
    /* �ͷ�Ipv4��ַ����L3���ݿ顣                                              */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        if (pst_logical_port->ipv4_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->ipv4_cfg_cb[i], MEM_SPM_LOG_PORT_IPV4_CB);
            pst_logical_port->ipv4_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* �ͷ�Ipv6��ַ����L3���ݿ顣                                              */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        if (pst_logical_port->ipv6_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->ipv6_cfg_cb[i], MEM_SPM_LOG_PORT_IPV6_CB);
            pst_logical_port->ipv6_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* �ͷ��鲥���ַL3���ݿ顣                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        if (pst_logical_port->mc_ipv4_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->mc_ipv4_cfg_cb[i], MEM_SPM_LOG_PORT_MC_IPV4_CB);
            pst_logical_port->mc_ipv4_cfg_cb[i] = NULL;
        }
    }
    
    /***************************************************************************/
    /* �ͷ�IPV6�鲥���ַL3���ݿ顣                                            */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        if (pst_logical_port->mc_ipv6_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->mc_ipv6_cfg_cb[i], MEM_SPM_LOG_PORT_MC_IPV6_CB);
            pst_logical_port->mc_ipv6_cfg_cb[i] = NULL;
        }
    }
    
    /***************************************************************************/
    /* �ͷ�����MAC��IP��ַ����L3���ݿ顣                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pst_logical_port->vip_vmac_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->vip_vmac_cfg_cb[i], MEM_SPM_LOG_PORT_VIP_VMAC_CB);
            pst_logical_port->vip_vmac_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* �ͷ�VRRP��MAC��IPV6��ַ����L3����                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pst_logical_port->vipv6_vmac_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pst_logical_port->vipv6_vmac_cfg_cb[i], MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
            pst_logical_port->vipv6_vmac_cfg_cb[i] = NULL;
        }        
    }

    /***************************************************************************/
    /* �ͷ��ս��ӽӿ��������ݿ顣                                              */
    /***************************************************************************/
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree))
    {
        AVLL_DELETE(pst_logical_port->terminal_if_tree, pst_terminal_if_cb->spm_terminal_if_node);
        spm_logical_port_free_terminal_if_cb(pst_terminal_if_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* �ͷ�diff_serv�������ݿ顣                                               */
    /***************************************************************************/
    for (pst_diff_serv_cb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->diff_serv_tree);
        pst_diff_serv_cb != NULL;
        pst_diff_serv_cb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->diff_serv_tree))
    {
        AVLL_DELETE(pst_logical_port->diff_serv_tree, pst_diff_serv_cb->spm_diff_serv_node);
        spm_logical_port_free_diff_serv_cb(pst_diff_serv_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* �ͷ������Diff-Serv����L2���ݿ顣                                       */
    /***************************************************************************/
    for (pst_flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->flow_diff_serv_tree);
        pst_flow_diff_serv_cb != NULL;
        pst_flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(pst_logical_port->flow_diff_serv_tree))
    {
        AVLL_DELETE(pst_logical_port->flow_diff_serv_tree, pst_flow_diff_serv_cb->spm_flow_diff_serv_node);
        spm_logical_port_free_flow_diff_serv_cb(pst_flow_diff_serv_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* �ͷ��ϻ�����������QOS��������L3�������ݿ顣                             */
    /***************************************************************************/
    for (pst_inclassify_qos_cb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(pst_logical_port->inclassify_qos_tree);
         pst_inclassify_qos_cb != NULL;
         pst_inclassify_qos_cb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(pst_logical_port->inclassify_qos_tree))
    {
    	AVLL_DELETE(pst_logical_port->inclassify_qos_tree, pst_inclassify_qos_cb->spm_inclassify_qos_node);
    	spm_logical_port_free_inclassify_qos_cb(pst_inclassify_qos_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* �ͷ��ϻ��û�QOS���������������ݿ顣                              */
    /***************************************************************************/
    for (pst_up_user_qos_cb = (SPM_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->up_user_qos_tree);
         pst_up_user_qos_cb != NULL;
         pst_up_user_qos_cb = (SPM_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->up_user_qos_tree))
    {
    	AVLL_DELETE(pst_logical_port->up_user_qos_tree, pst_up_user_qos_cb->spm_up_user_qos_node);
    	spm_logical_port_free_up_user_qos_cb(pst_up_user_qos_cb NBB_CCXT);
    }
         
    /***************************************************************************/
    /* �ͷ��� 19)����ϻ��û�QOS���������������ݿ顣                             */
    /***************************************************************************/
    for (pst_flow_up_user_qos_cb = (SPM_FLOW_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->flow_up_user_qos_tree);
         pst_flow_up_user_qos_cb != NULL;
         pst_flow_up_user_qos_cb = (SPM_FLOW_UP_USER_QOS_CB*) AVLL_FIRST(pst_logical_port->flow_up_user_qos_tree))
    {
    	AVLL_DELETE(pst_logical_port->flow_up_user_qos_tree, pst_flow_up_user_qos_cb->spm_flow_up_user_qos_node);
    	spm_logical_port_free_flow_up_user_qos_cb(pst_flow_up_user_qos_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* �ͷ�����������traffic_filterL3�������ݿ顣                              */
    /***************************************************************************/
    for (pst_traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(pst_logical_port->traffic_filter_tree);
         pst_traffic_filter_cb != NULL;
         pst_traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(pst_logical_port->traffic_filter_tree))
    {
    	AVLL_DELETE(pst_logical_port->traffic_filter_tree, pst_traffic_filter_cb->spm_traffic_filter_node);
    	spm_logical_port_free_traffic_filter_cb(pst_traffic_filter_cb NBB_CCXT);
    }
         
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_logical_port->spm_logical_port_handle, HDL_SPM_LOGICAL_PORT_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pst_logical_port, MEM_SPM_LOGICAL_PORT_CB);
    pst_logical_port = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_logical_port_init_logical_port_tree
   ��������  : ��ʼ���洢�˿��߼����õ���
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��24��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_logical_port_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_logical_port_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_LOGICAL_PORT_CB, port_index_key);
    AVLL_INIT_TREE(SHARED.logical_port_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_LOGICAL_PORT_CB, spm_logical_port_node));

    NBB_TRC_EXIT();
}


/*****************************************************************************
   �� �� ��  : spm_logical_port_free_all_logical_port
   ��������  : �ͷŶ˿��߼��������е��ڴ�ռ�
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��11��6��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_all_logical_port(NBB_CXT_T NBB_CXT)
{
	SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
	
    for (pst_logical_port = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
         pst_logical_port != NULL;
         pst_logical_port = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree))
    {
    	AVLL_DELETE(SHARED.logical_port_tree, pst_logical_port->spm_logical_port_node);
    	spm_free_logical_port_cb(pst_logical_port NBB_CCXT);
    }
}

/*****************************************************************************
   �� �� ��   : spm_logical_port_process
   ��������   : �˿��߼����ô���
   �������   : ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port
   �������   : ��
   �� �� ֵ   :
   ���ú���   :
   ��������   :

   �޸���ʷ   :
   1.��    �� : 2015��8��29��
     ��    �� : lish
     �޸����� : �����ɺ���
   2.��    �� : 2016��1��14��
     ��    �� : fansongbo
     �޸����� : 8000����    

*****************************************************************************/
NBB_VOID spm_logical_port_proc(ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port)
{
    //NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    /*��¼�����ô�ӡʱ��*/
    //struct timeval logic_tv_begin = {0};
    //struct timeval logic_tv_end = {0};
    //gettimeofday(&logic_tv_begin, NULL);

    /*��¼��ʼ��������ʱ��*/
    //OS_SPRINTF(uc_message, "time_test logic_port_proc begin = %ld\n", logic_tv_begin.tv_usec);
    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);  

    /* �����õĲ���ģʽ */
    NBB_ULONG ul_oper_basic = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_phy = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_logic_l2 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_logic_l3 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_l2vpn = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_vlan = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_ipv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_ipv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_mc_ipv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_mc_ipv6 = ATG_DCI_OPER_NULL;    
    NBB_ULONG ul_oper_vip_vmac = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_vipv6_vmac = ATG_DCI_OPER_NULL;      
    NBB_ULONG ul_oper_ve_mac = ATG_DCI_OPER_NULL;       
    NBB_ULONG ul_oper_diff_serv = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_flow_diff_serv = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_inclassify_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_up_user_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_up_user_group_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_down_user_queue_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_down_user_group_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_flow_up_user_qos = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_traffic_filter = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_ds_l2 = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_car = ATG_DCI_OPER_NULL;
     
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    NBB_ULONG ul_port_key = 0;
    NBB_BYTE uc_if_exist = ATG_DCI_EXIST;
    NBB_BOOL avll_ret_code;
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_logical_port_process");

    /* �������ָ�������Ч */
    NBB_ASSERT(pst_set_logical_port != NULL);

    if (NULL == pst_set_logical_port)
    {      
        ret = LOG_PORT_PROC_IPS_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    ul_port_key = pst_set_logical_port->key;
    pst_logical_port = AVLL_FIND(SHARED.logical_port_tree, &ul_port_key);

    if (NULL == pst_logical_port) /* �����Ŀ������*/
    {
        uc_if_exist = ATG_DCI_UNEXIST;
    }

    /* ��ȡ�����ò��� */
    ul_oper_basic = pst_set_logical_port->oper_basic;
    ul_oper_phy = pst_set_logical_port->oper_physical;
    ul_oper_logic_l2 = pst_set_logical_port->oper_logical_l2;
    ul_oper_logic_l3 = pst_set_logical_port->oper_logical_l3;
    ul_oper_l2vpn = pst_set_logical_port->oper_l2vpn;
    ul_oper_vlan = pst_set_logical_port->oper_vlan;
    ul_oper_ipv4 = pst_set_logical_port->oper_ipv4;
    ul_oper_ipv6 = pst_set_logical_port->oper_ipv6;
    ul_oper_mc_ipv4 = pst_set_logical_port->oper_mc_ipv4;
    ul_oper_mc_ipv6 = pst_set_logical_port->oper_mc_ipv6;    
    ul_oper_vip_vmac = pst_set_logical_port->oper_vip_vmac;
    ul_oper_vipv6_vmac = pst_set_logical_port->oper_vrrp_ipv6_mac_l3;     
    ul_oper_ve_mac = pst_set_logical_port->oper_ve_mac;     
    ul_oper_diff_serv = pst_set_logical_port->oper_diff_serv;
    ul_oper_flow_diff_serv = pst_set_logical_port->oper_flow_diff_serv;
    ul_oper_inclassify_qos = pst_set_logical_port->oper_inclassify_qos;
    ul_oper_up_user_qos = pst_set_logical_port->oper_up_user_qos;
    ul_oper_up_user_group_qos = pst_set_logical_port->oper_upuser_group_qos;
    ul_oper_down_user_queue_qos = pst_set_logical_port->oper_down_user_queue_qos;
    ul_oper_down_user_group_qos = pst_set_logical_port->oper_down_user_group_qos;
    ul_oper_flow_up_user_qos = pst_set_logical_port->oper_flow_upuser_qos;
    ul_oper_traffic_filter = pst_set_logical_port->oper_traffic_filter;
    ul_oper_ds_l2 = pst_set_logical_port->oper_ds_l2;
    ul_oper_car = pst_set_logical_port->oper_car;
    
    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pst_set_logical_port->return_code = ATG_DCI_RC_OK;

    /* ɾ��������Ŀ */
    if (TRUE == pst_set_logical_port->delete_struct)
    {
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ATG_DCI_UNEXIST == uc_if_exist)
        {        
            ret = LOG_PORT_PROC_DEL_NULL_LOCAL_CFG;
            spm_l2_record_err_code(ret);         
            goto EXIT_LABEL;
        }                
        else/* ����ɾ�� */
        {
            spm_dbg_print_logical_port_head(ul_port_key, SPM_OPER_DEL);        
            spm_dbg_record_logical_port_head(ul_port_key, SPM_OPER_DEL);

            //ɾ���˿��߼�������������
            spm_logical_port_del_cfg(pst_logical_port);

            if (NULL != pst_logical_port)
            {
                AVLL_DELETE(SHARED.logical_port_tree, pst_logical_port->spm_logical_port_node);

                //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                spm_free_logical_port_cb(pst_logical_port NBB_CCXT);
            }
        }

        /*��¼ɾ�����ý���ʱ��*/
        //gettimeofday(&logic_tv_end, NULL);
        /*��¼��ʼ��������ʱ��*/
        //OS_SPRINTF(uc_message, "time_test logic_port_proc delete end = %ld\n", logic_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message); 
        //OS_SPRINTF(uc_message, "time_test logic_interval = %ld\n", logic_tv_end.tv_usec - logic_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
    }

    /* ���ӻ������Ŀ */
    else
    {
        /* �����Ŀ�����ڣ�����Ҫ������Ŀ */
        if (uc_if_exist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_logical_port_head(ul_port_key, SPM_OPER_ADD);
            spm_dbg_record_logical_port_head(ul_port_key, SPM_OPER_ADD);
            pst_logical_port = spm_alloc_logical_port_cb(NBB_CXT);

            if(NULL == pst_logical_port)
            {
                ret = LOG_PORT_PROC_ALLOC_CB_FAIL;
                spm_l2_record_err_code(ret);   
                goto EXIT_LABEL;
            }

            if (pst_logical_port != NULL)
            {
                pst_logical_port->port_index_key = ul_port_key;
                avll_ret_code = AVLL_INSERT(SHARED.logical_port_tree, pst_logical_port->spm_logical_port_node);
                NBB_ASSERT(avll_ret_code);
            }
        }
        else
        {
            spm_dbg_print_logical_port_head(ul_port_key, SPM_OPER_UPD);
            spm_dbg_record_logical_port_head(ul_port_key, SPM_OPER_UPD);
        }

        /************************************ 1) �������� **************************************/
        if(ATG_DCI_OPER_NULL != ul_oper_basic)
        {
             spm_logical_port_basic_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_basic); 
        }

        /************************************2) �������� ***************************************/
        if(ATG_DCI_OPER_NULL != ul_oper_phy)
        {
             spm_logical_port_phy_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_phy); 
        }

        /************************************************************************************/
        /* L2�˿�                                                                                                     */
        /************************************************************************************/
        if ((NULL != pst_logical_port) && (NULL != pst_logical_port->basic_cfg_cb) 
            && (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type))
        {
            /************************************4) �߼�����L2 **********************************/
            if(ATG_DCI_OPER_NULL != ul_oper_logic_l2)
            {
                 spm_logical_port_logic_l2_proc(pst_set_logical_port, pst_logical_port,ul_oper_logic_l2); 
            }

            /************************************5) L2VPN *************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_l2vpn)
            {
                 spm_logical_port_l2vpn_proc(pst_set_logical_port, pst_logical_port,ul_oper_l2vpn); 
            }

            /************************************21) VLAN����***********************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vlan)
            {
                 spm_logical_port_vlan_proc(pst_set_logical_port, pst_logical_port,ul_oper_vlan);  
            }

            /************************************ 13�������Diff-Serv����L2************************/
            if(ATG_DCI_OPER_NULL != ul_oper_flow_diff_serv)
            {
                spm_logical_port_flow_diff_serv_proc(pst_set_logical_port, pst_logical_port,ul_oper_flow_diff_serv);
            }
            
            /************************************ 19��������ϻ��û�QOS��������L2 ***********************/
            if(ATG_DCI_OPER_NULL != ul_oper_flow_up_user_qos)
            {
                spm_logical_port_flow_up_user_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_flow_up_user_qos);
            } 
            
            /************************************ 14���ϻ�����������QOS��������L2***************************/
            if(ATG_DCI_OPER_NULL != ul_oper_inclassify_qos)
            {
                spm_logical_port_inclassify_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_inclassify_qos);
            } 
            
            /************************************ 18���»��û���QOS����L2*************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_group_qos)
            {
                spm_logical_port_down_user_group_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_group_qos);
            }
            
            /************************************ 17���»��û�����QOS��������L2******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_queue_qos)
            {
                spm_logical_port_down_user_queue_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_queue_qos);
            }
            
            /************************************ 20����������traffic_filterL2******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_traffic_filter)
            {
                spm_logical_port_traffic_filter_proc(pst_set_logical_port, pst_logical_port,ul_oper_traffic_filter);
            }  
            
            /************************************ 23) diff-Serv����L2��VP�»�UNI�ࣩ******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ds_l2)
            {
                spm_logical_port_ds_l2_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_ds_l2);
            }
            
            /************************************ 24��car����******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_car)
            {
                spm_logical_port_car_data_proc(pst_set_logical_port, pst_logical_port,ul_oper_car);
            }       
        }

        /**************************************************************************************************************/
        /* L3�˿�                                                                                                     */
        /**************************************************************************************************************/
        else if((NULL != pst_logical_port) && (NULL != pst_logical_port->basic_cfg_cb) 
            && (ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type))
        {
        
            /************************************3) �߼�����L3 **********************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_logic_l3)
            {
                 spm_logical_port_logic_l3_proc(pst_set_logical_port, pst_logical_port, ul_oper_logic_l3);

            }
            
            /************************************7) IPv4��ַ����L3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ipv4)
            {
                 spm_logical_port_ipv4_data_proc(pst_set_logical_port, pst_logical_port, ul_oper_ipv4);
            }

            /************************************8) IPv6��ַ����L3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ipv6)
            {
                 spm_logical_port_ipv6_data_proc(pst_set_logical_port, pst_logical_port, ul_oper_ipv6);
            }
            
            /************************************9) �鲥���ַL3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_mc_ipv4)
            {
                 spm_logical_port_mc_ipv4_proc(pst_set_logical_port, pst_logical_port, ul_oper_mc_ipv4);
            }
            
            /************************************22) IPv6�鲥���ַL3*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_mc_ipv6)
            {
                 spm_logical_port_mc_ipv6_proc(pst_set_logical_port, pst_logical_port, ul_oper_mc_ipv6);
            } 
            
            /************************************10)VRRP����MAC��IP��ַ����L3 *******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vip_vmac)
            {
                 spm_logical_port_vip_vmac_proc(pst_set_logical_port, pst_logical_port, ul_oper_vip_vmac);
            } 
            
            /************************************25) VRRP����MAC��IPv6��ַ����L3 *******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vipv6_vmac)
            {
                 spm_logical_port_vipv6_vmac_proc(pst_set_logical_port, pst_logical_port, ul_oper_vipv6_vmac);
            } 
            
            /************************************11��VE MAC��ַ����*******************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_ve_mac)
            {
                 spm_logical_port_ve_mac_proc(pst_set_logical_port, pst_logical_port, ul_oper_ve_mac);
            } 
            
            /************************************21) VLAN����*************************************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_vlan)
            {
                 spm_logical_port_vlan_proc(pst_set_logical_port, pst_logical_port,ul_oper_vlan);  
            }

            /************************************ 12��Diff-Serv����****************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_flow_diff_serv)
            {
                spm_logical_port_diff_serv_proc(pst_set_logical_port, pst_logical_port,ul_oper_flow_diff_serv);
            }

            /************************************ 14���ϻ�����������QOS��������L3***************************/
            if(ATG_DCI_OPER_NULL != ul_oper_inclassify_qos)
            {
                spm_logical_port_inclassify_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_inclassify_qos);
            }
            
            /************************************ 15���ϻ��û�QOS�������� *********************************/
            if(ATG_DCI_OPER_NULL != ul_oper_up_user_qos)
            {
                spm_logical_port_up_user_qos_policy_proc(pst_set_logical_port, pst_logical_port,ul_oper_up_user_qos);
            }
        
            /************************************ 18���»��û���QOS����L3*************************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_group_qos)
            {
                spm_logical_port_down_user_group_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_group_qos);
            }
            
            /************************************ 17���»��û�����QOS��������L3******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_down_user_queue_qos)
            {
                spm_logical_port_down_user_queue_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_down_user_queue_qos);
            }
            
            /************************************ 20����������traffic_filterL3******************************/
            if(ATG_DCI_OPER_NULL != ul_oper_traffic_filter)
            {
                spm_logical_port_traffic_filter_proc(pst_set_logical_port, pst_logical_port,ul_oper_traffic_filter);
            }        
        }
        else //�ȷ�L2Ҳ��L3
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_two_value(ret,ul_port_key,pst_logical_port->logic_cvt_id);
            goto EXIT_LABEL;
        }
        
        /************************************ 16���ϻ��û���QOS��������(��Ч)**************************/
        if(ATG_DCI_OPER_NULL != ul_oper_up_user_group_qos)
        {
            spm_logical_port_up_user_group_qos_proc(pst_set_logical_port, pst_logical_port,ul_oper_up_user_group_qos);
        }

        /*��¼ɾ�����ý���ʱ��*/
        //gettimeofday(&logic_tv_end, NULL);
        /*��¼��ʼ��������ʱ��*/
        //OS_SPRINTF(uc_message, "time_test logic_port_proc add end = %ld\n", logic_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        //OS_SPRINTF(uc_message, "time_test logic_interval = %ld\n", logic_tv_end.tv_usec - logic_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();
}


#endif

