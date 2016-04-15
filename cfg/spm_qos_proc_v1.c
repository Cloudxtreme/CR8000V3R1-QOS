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


#include <nbase.h>
#include <spmincl.h>
#include <TmApi.h>


/* ȫ�ִ�ӡ����qos_phy_cfg_print */
NBB_BYTE g_qos_phy_cfg_print = ATG_DCI_RC_OK;

/*****************************************************************************
   �� �� ��  : spm_qos_logic_flow_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_logic_flow_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_FLOW_KEY *key1 = (SPM_QOS_LOGIC_FLOW_KEY *)keyarg1;
    SPM_QOS_LOGIC_FLOW_KEY *key2 = (SPM_QOS_LOGIC_FLOW_KEY *)keyarg2;
    NBB_INT ret = 0;

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&key1->index, &key2->index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ushort(&key1->svlan, &key2->svlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ushort(&key1->cvlan, &key2->cvlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_ilm_comp
   ��������  : ilm���ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_vrf_hqos_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 )
{
    NBB_INT ret = 0;
    ATG_DCI_VRF_INSTANCE_KEY *key1;
    ATG_DCI_VRF_INSTANCE_KEY *key2;

    key1 = (ATG_DCI_VRF_INSTANCE_KEY *)(keyarg1);
    key2 = (ATG_DCI_VRF_INSTANCE_KEY *)(keyarg2);


    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ushort(&key1->vrf_id, &key2->vrf_id );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&key1->peer_ip, &key2->peer_ip );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_ilm_comp
   ��������  : ilm���ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_vrf_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 )
{
    NBB_INT ret = 0;
    SPM_QOS_VRF_INSTANSE_KEY *key1;
    SPM_QOS_VRF_INSTANSE_KEY *key2;

    key1 = (SPM_QOS_VRF_INSTANSE_KEY *)(keyarg1);
    key2 = (SPM_QOS_VRF_INSTANSE_KEY *)(keyarg2);


    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ushort(&key1->vrf_id, &key2->vrf_id );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&key1->peer_ip, &key2->peer_ip );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&key1->label, &key2->label);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_qos_logic_flow_key_compare
   ��������  : �߼��˿������ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_port_wred_key_compare(NBB_VOID *keyarg1, 
    NBB_VOID *keyarg2 )
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_KEY *key1 = (SPM_PORT_WRED_KEY *)keyarg1;
    SPM_PORT_WRED_KEY *key2 = (SPM_PORT_WRED_KEY *)keyarg2;
    NBB_INT ret = 0;

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&key1->index, &key2->index );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_byte(&key1->cos, &key2->cos);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_ftn_comp
   ��������  : ftn���ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_ftn_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    FTN_KEY *key1 = (FTN_KEY *)keyarg1;
    FTN_KEY *key2 = (FTN_KEY *)keyarg2;
    NBB_INT rv = 0;

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    rv = compare_ushort(&key1->vrfid, &key2->vrfid);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->fec, &key2->fec);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_byte(&key1->mask, &key2->mask);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return rv;
}




/*****************************************************************************
   �� �� ��  : spm_qos_tx_lsp_comp
   ��������  : tx_lsp���ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_tx_lsp_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 )
{
    CRTXLSP_KEY *lsp_key1;
    CRTXLSP_KEY *lsp_key2;
    NBB_INT ret = 0;

    lsp_key1 = (CRTXLSP_KEY *)(keyarg1);
    lsp_key2 = (CRTXLSP_KEY *)(keyarg2);

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&(lsp_key1->ingress), &(lsp_key2->ingress));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&(lsp_key1->egress), &(lsp_key2->egress) );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(lsp_key1->lspid), &(lsp_key2->lspid) );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&(lsp_key1->tunnelid), &(lsp_key2->tunnelid) );
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_tunnel_comp
   ��������  : tunnel���ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_tunnel_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2  )
{
    SPM_QOS_TUNNEL_KEY *key1 = (SPM_QOS_TUNNEL_KEY *)keyarg1;
    SPM_QOS_TUNNEL_KEY *key2 = (SPM_QOS_TUNNEL_KEY *)keyarg2;
    NBB_INT rv = 0;

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    rv = compare_ushort(&key1->type, &key2->type );
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    rv = compare_ushort(&key1->flag, &key2->flag );
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    if(1 == key1->type)
    {
        rv = spm_qos_tx_lsp_comp(&key1->tx_lsp, &key2->tx_lsp );
        if (rv != 0)
        {
            goto EXIT_LABEL;
        }

    }
    else
    {
        rv = spm_qos_ftn_comp(&key1->ftn, &key2->ftn );
        if (rv != 0)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL:
    return rv;
}








/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_logic_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_INTF_KEY *key1 = (SPM_QOS_LOGIC_INTF_KEY *)keyarg1;
    SPM_QOS_LOGIC_INTF_KEY *key2 = (SPM_QOS_LOGIC_INTF_KEY *)keyarg2;
    NBB_INT ret = 0;


    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ulong(&key1->index, &key2->index);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ushort(&key1->svlan, &key2->svlan);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************/
    /***************************************************************/
    /***************************************************************/
    ret = compare_ushort(&key1->cvlan, &key2->cvlan);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}

#if 1




/*****************************************************************************
   �� �� ��  : spm_qos_init
   ��������  : qos������ĳ�ʼ��
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���

*****************************************************************************/
void spm_qos_init()
{

    NBB_TRC_ENTRY(__FUNCTION__);
    
    spm_qos_policy_init();
    spm_qos_acl_init();
    spm_qos_behavior_init();
    spm_qos_classify_init();

    spm_init_twamp();
    
    spm_qos_defend_init();
    spm_qos_hqos_init();



    //EXIT_LABEL: 
    NBB_TRC_EXIT();
    return;
}




/*****************************************************************************
   �� �� ��  : spm_qos_voq_init
   ��������  : arad����Ĭ��ȫ���ӹ�ϵ�ĳ�ʼ������
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���

*****************************************************************************/
NBB_LONG spm_qos_voq_init()
{
    NBB_LONG slot = 0;
    NBB_LONG portnum = 0;
    NBB_BYTE fapid = 0; /* ����λ */
    NBB_LONG base_queue = 0;
    NBB_LONG base_vcid = 0;
    NBB_INT rv = 0;
    
    //NBB_USHORT i = 0;
    //NBB_ULONG buf_size = 0;

    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;

    /* ��ȡ�������ڵĲ�λ�� */
    spm_hardw_getslot(&fapid);
    fapid--;

    /********************************************************************************/ 
    /* �ڱ��̽������˿ڵ�ȫ�����ڲ�λ(��������λ)��VOQ���ӹ�ϵ,��������ڵ�ITM����; */
    /* ��ʱ���̽�����Ϊҵ��������,ҵ����ܴӱ��̵�����˿��н���,����������(����  */
    /* ����)���п�����Ϊҵ��ĳ�����.                                               */
    /********************************************************************************/
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
        base_queue = offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        base_vcid = fapid * NUM_COS + offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;

        for (slot = 0; slot < MAX_SLOT_NUM; slot++) /* R8000�е�arad���֧��32����λ */
        {
            if (fapid == slot)  /* ����λ�����ڶ˿ڲ�����VOQ */
            {
#ifdef PTN690 
                if(PTN_690_PORT_OFFSET == portnum)
                {
#if defined (SPU) || defined (SRC)
                    ApiAradCheckPortValid(0, 0, &rv);
                    if (0 == rv)
                    {
                        base_queue += NUM_COS;
                        continue;
                    }
#endif
                }
                else
                {
#if defined (SPU) || defined (SRC)
                    ApiAradCheckPortValid(0, portnum, &rv);
                    if (0 == rv)
                    {
                        base_queue += NUM_COS;
                        continue;
                    }
#endif
                }
#else 
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, portnum, &rv);
                if (0 == rv)
                {
                    base_queue += NUM_COS;
                    continue;
                }
#endif
#endif
            }
#ifdef PTN690
            if(PTN_690_PORT_OFFSET == portnum) /* ARAD cpu�˿ں� */
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupItm(0, base_queue, NUM_COS, slot + 1, 0, base_vcid);
                if (rv != 0)
                {
                    printf("Error! ApiSetupItm err: %d (base_queue=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,base_queue, slot + 1, portnum, base_vcid);
                    goto EXIT_LABEL;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupItm(0, base_queue, NUM_COS, slot + 1, portnum, base_vcid);
                if (rv != 0)
                {
                    printf("Error! ApiSetupItm err: %d (base_queue=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,base_queue, slot + 1, portnum, base_vcid);
                    goto EXIT_LABEL;
                }
#endif
            }
#else
#if defined (SPU) || defined (SRC)
            rv = ApiSetupItm(0, base_queue, NUM_COS, slot + 1, portnum, base_vcid);
            if (rv != 0)
            {
                printf("Error! ApiSetupItm err: %d (base_queue=%ld,slot=%ld,port=%ld,"
                       "vc=%ld)\n", rv,base_queue, slot + 1, portnum, base_vcid);
                goto EXIT_LABEL;
            }
#endif
#endif  

#if 0       /*���������޸�voq��Ĭ�϶��л���ֵ*/
            for(i = 0; i < 5;i++)
            {   

                buf_size = 1*1024*1024;
 
#ifdef PTN690
                if(PTN_690_PORT_OFFSET == portnum) /* ARAD cpu�˿ں� */
                {
#if defined (SPU) || defined (SRC)
                    rv = ApiAradSetQueueTailDrop(0, base_queue, i, -1, buf_size);
#endif
                }
                else
                {
#if defined (SPU) || defined (SRC)
                    rv = ApiAradSetQueueTailDrop(0, base_queue, i, -1, buf_size);
#endif
                }
#else
#if defined (SPU) || defined (SRC)
                rv = ApiAradSetQueueTailDrop(0, base_queue, i, -1, buf_size);
#endif
#endif
                if (rv != 0)
                {
                    printf("Error! ApiAradSetQueueTailDrop err: %d (base_queue=%ld,cos=%ld)\n", rv, base_queue, i);
                    goto EXIT_LABEL;
                }
            }
#endif
            base_queue += NUM_COS;
        }
    }

    /* ��ʼ���˿�:Ϊ������˿ڽ���Enhanced HR��FQ(ȫSP) */
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
#ifdef PTN690 
        if(PTN_690_PORT_OFFSET == portnum)
        {
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, 0, &rv);
            if (0 == rv)
            {
                continue;
            }
#endif
        }
        else
        {
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, portnum, &rv);
            if (0 == rv)
            {
                continue;
            }
#endif
        }
#else 
#if defined (SPU) || defined (SRC)
        ApiAradCheckPortValid(0, portnum, &rv);
        if (0 == rv)
        {
            continue;
        }
#endif
#endif
#ifdef PTN690
        if(PTN_690_PORT_OFFSET == portnum)
        {
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseSeEtm(0, 0);
            if (rv != 0)
            {
                printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
                goto EXIT_LABEL;
            }
#endif
        }
        else
        {
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseSeEtm(0, portnum);
            if (rv != 0)
            {
                printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
                goto EXIT_LABEL;
            }
#endif
        }
#else
#if defined (SPU) || defined (SRC)
        rv = ApiSetupBaseSeEtm(0, portnum);
        if (rv != 0)
        {
            printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
            goto EXIT_LABEL;
        }
#endif
#endif
    }

    /************************************************************************/ 
    /* ����ȫ������̵����̸��˿ڵ���VC�����̸����˿��������VOQ֮��Ĺ���; */
    /* ��ʱ���̽�����Ϊҵ��ĳ����̣�������(��������)��������Ϊҵ��������.*/
    /************************************************************************/
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
        base_queue = fapid * NUM_COS + offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        base_vcid = offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        for (slot = 0; slot < MAX_SLOT_NUM; slot++)
        {
#ifdef PTN690 
            if(PTN_690_PORT_OFFSET == portnum)
            {
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, 0, &rv);
                if (0 == rv)
                {
                    base_vcid += NUM_COS;
                    continue;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, portnum, &rv);
                if (0 == rv)
                {
                    base_vcid += NUM_COS;
                    continue;
                }
#endif
            }
#else 
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, portnum, &rv);
            if (0 == rv)
            {
                base_vcid += NUM_COS;
                continue;
            }
#endif
#endif
#ifdef PTN690
            if(PTN_690_PORT_OFFSET == portnum) /* arad CPU�˿ں� */
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupBaseEtm(0, base_queue, NUM_COS, slot + 1, 0, base_vcid);
                if (rv != 0)    
                {
                    printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,base_queue, slot + 1, portnum, base_vcid);
                    goto EXIT_LABEL;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupBaseEtm(0, base_queue, NUM_COS, slot + 1, portnum, base_vcid);
                if (rv != 0)    
                {
                    printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,base_queue, slot + 1, portnum, base_vcid);
                    goto EXIT_LABEL;
                }
#endif
            }
#else
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseEtm(0, base_queue, NUM_COS, slot + 1, portnum, base_vcid);
            if (rv != 0)    
            {
                printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                       "vc=%ld)\n", rv,base_queue, slot + 1, portnum, base_vcid);
                goto EXIT_LABEL;
            }
#endif
#endif
            base_vcid += NUM_COS;
        }
    }

#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return rv;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_init_hqos_lsp()
{
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_INT  rv  = ATG_DCI_RC_OK;
#endif
    NBB_ULONG i;

    for(i = MIN_PHYSIC_PORT; i < MAX_PHYSIC_PORT; i++)
    {
#if defined (SPU) || defined (SRC)
        ApiAradCheckPortValid(0, i, &rv);
        if (0 == rv)
        {
            continue;
        }
#endif
#ifdef PTN690
        if(PTN_690_PORT_OFFSET < i)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosLspCreate(UNIT_0, i, i - MIN_PHYSIC_PORT);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**%s,line=%d,port=%ld,lsp id=%ld\n",
                          __FUNCTION__,__LINE__,i,i - MIN_PHYSIC_PORT);
                return ret;
            }
#endif
        }
#else
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspCreate(UNIT_0, i, i - MIN_PHYSIC_PORT);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,line=%d,port=%ld,lsp id=%ld\n",
                      __FUNCTION__,__LINE__,i,i - MIN_PHYSIC_PORT);
            return ret; 
        }
#endif
#endif
    }
    
    return ret;
}


/*SPU�̵���*/
/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_init_rcu_cir_pir()
{
    NBB_LONG ret = 0;
#if defined (SPU) || defined (SRC)
    NBB_LONG cir[NUM_COS] = {390,390,2730,390,1560,3719,4950,1900};
    NBB_LONG pir[NUM_COS] = {10000,390,10000,8000,16000,16000,16000,16000};
#endif
    NBB_LONG i = 0;

#ifdef PTN690
#if defined (SPU) || defined (SRC)
    ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT, 4 * 1024);
    if(0 != ret)
    {
        printf("set rcu port bandwidth = 4M EROOR\n");
        goto EXIT_LABEL;
    }
#endif
#else
#if defined (SPU) || defined (SRC)
    ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT, 16 * 1024);
    if(0 != ret)
    {
        printf("set rcu port bandwidth = 16M EROOR\n");
        goto EXIT_LABEL;
    }
#endif
#endif
    for(i = 0; i < NUM_COS;i++)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,QOS_RCU_PORT,i,cir[i],pir[i] - cir[i],1024,1024);
        if(0 != ret)
        {
            printf("set rcu cos = %ld cir eir EROOR\n",i);
            goto EXIT_LABEL;
        }
#endif
    }

    ret = spm_init_hqos_lsp();

#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return ret;
}

/*SRC�̵���*/
/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_init_arad_cpu_cir_pir()
{
    NBB_LONG ret = 0;
    NBB_LONG i = 0;
 
    for(i = 0; i < NUM_COS;i++)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,0,i,0,6 * 1024,1024,1024);
        if(0 != ret)
        {
            printf("set cpu cos =%ld eir 6M EROOR=%ld\n",i,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    //ret = spm_init_hqos_lsp();
#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return ret;
}

#endif




#if 5

/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_ipv4_sampler(SUB_PORT *sub_port, ATG_DCI_PHY_PORT_IPNF_SAMPLER *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG cnt_id = 0;
    NBB_USHORT port_id = 0;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ipv4_sampler");

    if((NULL == cfg) || (NULL == sub_port))
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS ipv4_sampler",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                  "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                  "eg_mode=%s,eg_sample_ratio=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  (cfg->in_enable_flag)?"enable":"disable",
                  (cfg->in_mode)?"fix":"random",cfg->in_sample_ratio,
                  (cfg->eg_enable_flag)?"enable":"disable",
                  (cfg->eg_mode)?"fix":"random",cfg->eg_sample_ratio);
        OS_SPRINTF(uc_message,"%s %s,%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                   "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                   "eg_mode=%s,eg_sample_ratio=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   (cfg->in_enable_flag)?"enable":"disable",
                   (cfg->in_mode)?"fix":"random",cfg->in_sample_ratio,
                   (cfg->eg_enable_flag)?"enable":"disable",
                   (cfg->eg_mode)?"fix":"random",cfg->eg_sample_ratio);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }

    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    if ((sub_port->port&0x60) != 0)
    {
        port_id = sub_port->port&0x1f;
    }
    else
    {
        port_id = ((sub_port->port&0x60)*(sub_port->port&0x1f));
    }

    cnt_id = (sub_port->slot - 1) * 50 + port_id + 1024;

#if defined (SPU) || defined (PTN690_CES)
    rv = ApiC3SetFlowSample(sub_port->unit,sub_port->port, 0, cfg->in_enable_flag, cfg->in_sample_ratio,cnt_id);
#endif  
    if(ATG_DCI_RC_OK != rv)
    {
        printf("%s %d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",
                  __FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    				   "QOS ipv4_sampler",rv,"phyPort","unit","in_enable_flag","in_sample_ratio",
    				   sub_port->port,sub_port->unit,cfg->in_enable_flag,cfg->in_sample_ratio));
        OS_SPRINTF(uc_message,"%s %s,%d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        ret = rv;
    }
     
#if defined (SPU) || defined (PTN690_CES)
    rv = ApiC3SetFlowSample(sub_port->unit,sub_port->port, 1, cfg->eg_enable_flag, cfg->eg_sample_ratio,cnt_id);
#endif
    if(ATG_DCI_RC_OK != rv)
    {
        printf("EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",sub_port->unit,rv,sub_port->port);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    				   "QOS ipv4_sampler",rv,"phyPort","unit","eg_enable_flag","eg_sample_ratio",
    				   sub_port->port,sub_port->unit,cfg->in_enable_flag,cfg->in_sample_ratio));
        OS_SPRINTF(uc_message,"%s %s,%d EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        ret = rv;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_lag_sampler(SUB_PORT *sub_port, ATG_DCI_LAG_IPNF_SAMPLER_DATA *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG cnt_id = 0;
    NBB_USHORT port_id = 0;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_set_lag_sampler");

    if(NULL == cfg)
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS lag_sampler",ATG_DCI_RC_UNSUCCESSFUL,
					   "phyPort","cfg==null","","",sub_port->port,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_qos_phy_cfg_print)
    {
        printf("%s %s,LINE=%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                  "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                  "eg_mode=%s,eg_sample_ratio=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  (cfg->ingress_flag)?"enable":"disable",
                  (cfg->ingress_mode)?"fix":"random",cfg->ingress_sampling,
                  (cfg->engress_flag)?"enable":"disable",
                  (cfg->engress_mode)?"fix":"random",cfg->engress_sampling);
        OS_SPRINTF(uc_message,"%s %s,LINE=%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                   "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                   "eg_mode=%s,eg_sample_ratio=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   (cfg->ingress_flag)?"enable":"disable",
                   (cfg->ingress_mode)?"fix":"random",cfg->ingress_sampling,
                   (cfg->engress_flag)?"enable":"disable",
                   (cfg->engress_mode)?"fix":"random",cfg->engress_sampling);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }

    if((sub_port->slot != 0) || (sub_port->port < 0x80))
    {
        printf("%s %s,LINE=%d,spm_set_lag_sampler ERROR\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__);
        OS_SPRINTF(uc_message,"%s %s,LINE=%d,spm_set_lag_sampler ERROR\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if ((sub_port->port&0x60) != 0)
    {
        port_id = sub_port->port&0x1f;
    }
    else
    {
        port_id = ((sub_port->port&0x60) * (sub_port->port&0x1f));
    }
    
    cnt_id = (sub_port->slot - 1) * 50 + port_id + 1024;
    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        rv = ApiC3SetFlowSample(unit,sub_port->port, 0, cfg->ingress_flag, cfg->ingress_sampling,cnt_id);
#endif
        if(ATG_DCI_RC_OK != rv)
        {
            printf("ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",unit,rv,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				   "QOS ipv6_sampler",rv,"phyPort","unit","in_enable_flag","in_sample_ratio",
        				   sub_port->port,unit,cfg->ingress_flag,cfg->ingress_sampling));
            OS_SPRINTF(uc_message,"%s %s,%d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,unit,rv,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            ret = rv;
        }
       
#if defined (SPU) || defined (PTN690_CES)
        rv = ApiC3SetFlowSample(unit,sub_port->port, 1, cfg->engress_flag, cfg->engress_sampling,cnt_id);
#endif
        if(ATG_DCI_RC_OK != rv)
        {
            printf("EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",unit,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				   "QOS ipv6_sampler",rv,"phyPort","unit","eg_enable_flag","eg_sample_ratio",
        				   sub_port->port,unit,cfg->engress_flag,cfg->engress_sampling));
            OS_SPRINTF(uc_message,"%s %s,%d EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,unit,rv,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            ret = rv;
        } 
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_port_flood_control(SUB_PORT *sub_port,
    NBB_BYTE bc_swtich,
    NBB_BYTE mc_swtich,
    NBB_BYTE fd_swtich,
    NBB_ULONG bc_bandwith,
    NBB_ULONG mc_bandwith,
    NBB_ULONG fd_bandwith)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    METER_CFG_T meter = {0};
    
    /*�������ۺ�Ĭ����д��ֵ������mantis������*/
    NBB_ULONG default_cbs = 2048;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_cir_pir");

    if (NULL == sub_port)
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "%s,%s,%s\n"
                  "bc_bandwith=%ld,mc_bandwith=%ld,fd_bandwith=%ld\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  bc_swtich?"�㲥���ƿ�":"�㲥���ƹ�",
                  mc_swtich?"�鲥���ƿ�":"�鲥���ƹ�",
                  fd_swtich?"�鷺���ƿ�":"�鷺���ƹ�",
                  bc_bandwith,mc_bandwith,fd_bandwith);
        OS_SPRINTF(uc_message,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "%s,%s,%s\n"
                   "bc_bandwith=%ld,mc_bandwith=%ld,fd_bandwith=%ld\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   bc_swtich?"�㲥���ƿ�":"�㲥���ƹ�",
                   mc_swtich?"�鲥���ƿ�":"�鲥���ƹ�",
                   fd_swtich?"�鷺���ƿ�":"�鷺���ƹ�",
                   bc_bandwith,mc_bandwith,fd_bandwith);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }

    if (bc_swtich + mc_swtich + fd_swtich)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFCEnable(sub_port->unit, sub_port->port, 1);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3;
        meter.eMeterType = RFC_2697;
        meter.cir = bc_bandwith;
        meter.pireir = bc_bandwith;
        meter.cbs = default_cbs;
        meter.pbsebs = default_cbs;
        meter.dropRed = 1;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meterId","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, BC_CONTROL, bc_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3 + 1;
        meter.cir = mc_bandwith;
        meter.pireir = mc_bandwith;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meterId","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, MC_CONTROL, mc_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meter id","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3 + 2;
        meter.cir = fd_bandwith;
        meter.pireir = fd_bandwith;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, UC_CONTROL, fd_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meter id","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }
    }
    else
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFCEnable(sub_port->unit, sub_port->port, 0);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,
						   "port","unit","ApiC3SetPortFCEnable","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_port_cir_pir(SUB_PORT *sub_port, NBB_LONG cos,
    ATG_DCI_PHY_FLOW_SHAPE_QUEUE *shape)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_LONG eir;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_cir_pir");

    if ((NULL == sub_port) || (NULL == shape) || (cos > 7))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "cos=%ld,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                  cos,shape->cir,shape->pir,shape->cbs,shape->pbs);
        OS_SPRINTF(uc_message,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "cos=%ld,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                   cos,shape->cir,shape->pir,shape->cbs,shape->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }
    if (shape->pir < shape->cir)
    {
        printf("**QOS ERROR**%s,%d,pir<cir\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "pir","cir","port","cos",
					   shape->pir,shape->cir,sub_port->port,cos));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(25 * 100 * 1000 == shape->pir)
    {
        shape->pir += 10000;
    }

    eir = shape->pir - shape->cir;

#if defined (SPU) || defined (SRC)
    ret = ApiAradSetPortQueueCirEir(unit, sub_port->port + PTN_690_PORT_OFFSET,
                            cos, shape->cir, eir, shape->cbs, shape->pbs);
#endif
    if (ATG_DCI_RC_OK != ret)
    {
        printf("%s %s,%d ret=%ld port=%d cos=%ld ApiAradSetPortQueueCirEir\n",
                  QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                  sub_port->port + PTN_690_PORT_OFFSET,cos);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port cir pir",ret,"slot","port","cos","",
					   sub_port->slot,sub_port->port + PTN_690_PORT_OFFSET,cos,0));
        OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d cos=%ld ApiAradSetPortQueueCirEir\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                   sub_port->port + PTN_690_PORT_OFFSET,cos);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_port_sp_wfq(SUB_PORT *sub_port, NBB_BYTE cos, 
    ATG_DCI_PHY_PORT_SCHEDULE_QUEUE *port_schedule)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_BYTE fapid = 0;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_sp_wfq");

    if ((NULL == sub_port) || (NULL == port_schedule) || (cos > 7))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != g_qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "cos=%d,mode=%s,weight=%d\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                  cos,(port_schedule->mode)?"WFQ":"SP",port_schedule->weight);
        OS_SPRINTF(uc_message,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "cos=%d,mode=%s,weight=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                   cos,(port_schedule->mode)?"WFQ":"SP",port_schedule->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }

    if (0 == port_schedule->mode)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueSP(unit, sub_port->port + PTN_690_PORT_OFFSET, cos, 7 - cos);
#endif
    }
    else
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueWfqWeight(unit, sub_port->port + PTN_690_PORT_OFFSET, cos, port_schedule->weight);
#endif
    }
    if (ATG_DCI_RC_OK != ret)
    {
        printf("%s %s,%d ret=%ld port=%d cos=%d mode=%d set sp or wfq err\n",
                  QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                  sub_port->port + PTN_690_PORT_OFFSET,cos,port_schedule->mode);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port sp wfq",ret,"slot","port","cos","mode",
					   sub_port->slot,sub_port->port,cos,port_schedule->mode));
        OS_SPRINTF(uc_message,"%s %s,%d ret=%ld port=%d cos=%d mode=%d set sp or wfq err\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                   sub_port->port + PTN_690_PORT_OFFSET,cos,port_schedule->mode);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}





#endif

#if 6

/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
void spm_qos_clear_all_cfg()
{
    spm_qos_clear_defend_cfg();
    spm_qos_defend_init();

    spm_qos_clear_all_acl();
    spm_qos_clear_all_logic_acl();
    spm_qos_clear_all_action();
    spm_qos_clear_all_classify();
    spm_qos_clear_all_policy();
    spm_qos_clear_all_logic_classify();
    
    spm_qos_clear_all_twamp_ipv4_cb();
    spm_qos_clear_all_twamp_ipv6_cb();
}

#endif

