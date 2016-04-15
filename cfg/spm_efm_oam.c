#include <nbase.h>
#include <spmincl.h>
#include <c3api.h>
#if 0
#ifndef SNU

#ifdef SRC
NBB_VOID spm_set_efm_oam(EFM_PHY_PORT *p_efm_phy_port, ATG_DCI_PHY_PORT_EFM_OAM_DATA *p_efm_oam)
{
    return;
}
NBB_VOID InitAllPortOamSelfCfg()
{
    return;
}
#endif

#ifdef SPU

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_efm
 ��������  : ��������ģ��
 �������  : NBB_BYTE *p
 �������  : ��
 �� �� ֵ  : NBB_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : huxi
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_set_efm_oam(EFM_PHY_PORT *p_efm_phy_port, ATG_DCI_PHY_PORT_EFM_OAM_DATA *p_efm_oam)
{
    NBB_INT iRet = ERROR;
    NBB_BYTE local_slot;
    NBB_BYTE port_temp;
    NBB_BYTE sub_card;
    /* ��ȡ���̲�λ */
    iRet = spm_hardw_getslot(&local_slot);

    if (SUCCESS != iRet)
    {
        NBB_PRINTF(("Get slot info failed."));
        return;
    }

    if (local_slot != p_efm_phy_port->slots)
    {
        return;
    }

    sub_card = p_efm_phy_port->cards;

    //������ӿ�
    if (0 == sub_card)
    {
        g_efm_oam_port_cfg[sub_card].sub_cards = sub_card;
        port_temp = p_efm_phy_port->ports - 33;
        g_efm_oam_port_cfg[sub_card].logic_port[port_temp] = p_efm_phy_port->ports;
    }
    else
    {
        if (1 == sub_card) //�ӿ�1
        {
            g_efm_oam_port_cfg[sub_card].sub_cards = sub_card;
            port_temp = p_efm_phy_port->ports - 33; //�ӿ�1���߼��˿ںŷ�ΧΪ32+port
            g_efm_oam_port_cfg[sub_card].logic_port[port_temp] = p_efm_phy_port->ports;
        }
        else if (2 == sub_card)
        {
            g_efm_oam_port_cfg[sub_card].sub_cards = sub_card;
            port_temp = p_efm_phy_port->ports - 65;//�ӿ�2���߼��˿ںŷ�ΧΪ64+port
            g_efm_oam_port_cfg[sub_card].logic_port[port_temp] = p_efm_phy_port->ports;
        }
    }

    /* ����ж� */
    if ((NULL == p_efm_oam) || (0 == p_efm_oam->byte1.oam_enable))
    {
        //ɾ�����ô洢
        g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp] = 0;
        g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg_last[port_temp] = 0;
        g_efm_oam_port_cfg[sub_card].port_oam_mode_cfg[port_temp] = 0;
        g_efm_oam_port_cfg[sub_card].port_oam_maxsize_cfg[port_temp] = oam_max_size;
        g_efm_oam_port_cfg[sub_card].port_oam_overtime_cfg[port_temp] = lb_overtime;
        g_efm_oam_port_cfg[sub_card].port_oam_sym_period_cfg[port_temp] = 1;
        g_efm_oam_port_cfg[sub_card].port_oam_frm_period_cfg[port_temp] = 1;
        g_efm_oam_port_cfg[sub_card].port_oam_frm_prd_period_cfg[port_temp] = 1;
        g_efm_oam_port_cfg[sub_card].port_oam_frm_snd_period_cfg[port_temp] = 60;
        OS_MEMSET(&(g_efm_oam_port_cfg[sub_card].port_oam_x[port_temp]), 0, sizeof(port_oam));
        NBB_PRINTF("spm_set_efm_oam cfg==null\n");
        return;
    }

    //�洢����
    g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp] = p_efm_oam->byte1.oam_enable;
    g_efm_oam_port_cfg[sub_card].port_oam_mode_cfg[port_temp] = p_efm_oam->byte1.oam_mode;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_event_cfg[port_temp] = p_efm_oam->byte1.support_error_notify;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_lb_cfg[port_temp] = p_efm_oam->byte1.support_loop;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_var_cfg[port_temp] = p_efm_oam->byte1.support_mib_search;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_uni_cfg[port_temp] = p_efm_oam->byte1.support_unidirectional_oam;
    g_efm_oam_port_cfg[sub_card].port_oam_overtime_cfg[port_temp] = p_efm_oam->reply_overtime_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_maxsize_cfg[port_temp] = p_efm_oam->max_frame_length;
    g_efm_oam_port_cfg[sub_card].port_oam_sym_period_cfg[port_temp] = p_efm_oam->error_symbol_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_sym_threshold_cfg[port_temp] = p_efm_oam->error_symbol_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_period_cfg[port_temp] = p_efm_oam->error_frame_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_threshold_cfg[port_temp] = p_efm_oam->error_frame_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_prd_period_cfg[port_temp] = p_efm_oam->error_frame_cycle_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_prd_threshold_cfg[port_temp] = p_efm_oam->error_frame_cycle_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_snd_period_cfg[port_temp] = p_efm_oam->error_frame_second_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_snd_threshold_cfg[port_temp] = p_efm_oam->error_frame_second_limit;
    CfgPortOamEnb(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp]); //���ö˿�OAMʹ��
    CfgPortOamSupportLb(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_sup_lb_cfg[port_temp]); //�Ƿ�֧�ֻ���
    CfgPortOamSupportUnidirection(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_sup_uni_cfg[port_temp]); //�Ƿ�֧�ֵ���
    CfgPortOamSupportVar(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp]); //�Ƿ�֧�ֱ�����ѯ
    CfgPortOamSupportEvent(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_sup_var_cfg[port_temp]); //�Ƿ�֧���¼�֡�շ�
    CfgPortOamMode(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_mode_cfg[port_temp]); //����OAMģʽ
    AllPortOamCfgEnb();/* ������Щ�˿ڿ���OAM�������Ƿ�ʹ������OAM�շ�����*/
}
/*************************** ����˵�� 20080904  ������
**************************************************
1.�û������нӿں����򵥣��Ҷ��ڱ��ļ��������ļ������ע��
2.�û�Ӧ���屾�ļ�������ע�ͣ��û�Ҳֻ���ע���ļ���
3.��OAM������Ӧ�ڽ��������ú�������
4.Ϊ�˼�࣬�������ݲ����ǵ����ж������оƬ�����⣡�Ժ�Ҫ��չ���ס�
5.������Ŀǰ�����BROADCOM����оƬ�������Ժ��ZARLINKоƬ������  #define
ZARLINK
6.�û�Ӧע������˿�������˿ڣ����߼��˿ڣ�Ӧ���Ƕ˿�ӳ��任��

*****************************************************************************/


/*****************************************************************************
���ܣ� ������LAN�˿�OAM��ʼ��Ϊ��ʹ��OAM������ȫ�ֱ�������ֵ��Ӧ���ϵ�
SELFCONFIG �г�ʼ��ʱ����
���ߣ� hmliu
ʱ�䣺 20080904
���룺 ��
����� ��
˵���� �û�Ӧ�Լ�������LAN�˿ڳ�ʼ������PVID�����Ա���������OAM���ŵ�,
�˲���Ӧ�����ý�����ʱ���
�޸ģ�
******************************************************************************/
NBB_VOID InitAllPortOamSelfCfg()
{
    unsigned char i, j;
    dst_mac_overall[0] = 0x01;
    dst_mac_overall[1] = 0x80;
    dst_mac_overall[2] = 0xc2;
    dst_mac_overall[3] = 0x00;
    dst_mac_overall[4] = 0x00;
    dst_mac_overall[5] = 0x02;
    len_type_overall[0] = 0x88;
    len_type_overall[1] = 0x09;
    subtype_overall = 0x03;
    wri_fh_oui[0] = 0x00;
    wri_fh_oui[1] = 0x0a;
    wri_fh_oui[2] = 0xc2;
    board_name[0] = 'C';
    board_name[1] = 'R';
    board_name[2] = '8';
    board_name[3] = 'K';
    minFrameSize = 60;
    oam_max_size = 1496;
    lb_overtime = 10;
    lb_overtime = 5; //110513
    Period_100ms = 10;

    for (j = 0 ; j < SUB_NUM; j++)
    {
        for (i = 0; i < MAX_PORT_NUM; i++)
        {
            g_efm_oam_port_cfg[j].port_oam_enb_cfg[i] = 0;
            g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i] = 0;
            g_efm_oam_port_cfg[j].port_oam_mode_cfg[i] = 0;
            g_efm_oam_port_cfg[j].port_oam_maxsize_cfg[i] = oam_max_size;
            g_efm_oam_port_cfg[j].port_oam_overtime_cfg[i] = lb_overtime;
            g_efm_oam_port_cfg[j].port_oam_sym_period_cfg[i] = 1;
            g_efm_oam_port_cfg[j].port_oam_frm_period_cfg[i] = 1;
            g_efm_oam_port_cfg[j].port_oam_frm_prd_period_cfg[i] = 1;
            g_efm_oam_port_cfg[j].port_oam_frm_snd_period_cfg[i] = 60;
            OS_MEMSET(&(g_efm_oam_port_cfg[j].port_oam_x[i]), 0, sizeof(port_oam));
        }
    }
}

/**************************************************
** �������ƣ�RxOamPortResolveHdr
** ��������������EFM OAM PDU��ͬ�ı���ͷ
** ��    �룺port:��EFM OAM PDU�˿�
             p:���յı�������ָ��
** ��    ������
** ��    �ߣ�yy09
** ��    �ڣ�2012.06.12
***************************************************/
NBB_BYTE RxOamPortResolveHdr(NBB_BYTE sub_cards, NBB_BYTE port, oam_header *p)
{
    port_oam_pdu *p_rx; /*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (FALSE != NBB_MEMCMP(p->dst_mac, dst_mac_overall, 6))    /*Ŀ�ĵ�ַ��� EFM OAM����Ŀ��MACΪ 01-80-c2-00-00-02*/
    {
        NBB_PRINTF("dst_mac_overall FAIL\n");
        goto RxErrorExit;
    }

    if (FALSE != NBB_MEMCMP(p->len_type, len_type_overall, 2))    /*Э�����ͼ�� ��Э�鹲ͬЭ������ 0x8809*/
    {
        NBB_PRINTF("len_typeFAIL\n");
        goto RxErrorExit;
    }

    if ((p->subtype) != subtype_overall)      /*SUBTYPE��� EFM OAM��Э������Ϊ 0X03*/
    {
        NBB_PRINTF("subtype_overall FAIL\n");
        goto RxErrorExit;
    }

    NBB_MEMCPY(p_ctrl->src_mac_remote, p->src_mac, 6);   /*SA*/
    NBB_MEMCPY(p_rx->oam_pdu_x.oam_header_x.src_mac, p->src_mac, 6);   /*SA*/
    NBB_MEMCPY(&(p_rx->oam_pdu_x.oam_header_x.flags_reserved), &(p->flags_reserved), 2);       /*flag*/
    /*���Զ�LINK״̬��ȡ��������Ϣֱ�Ӵ��ڽṹ��*/
    p_ctrl->remote_link_status_last = p_rx->oam_pdu_x.oam_header_x.link_fault \
                                      | p_rx->oam_pdu_x.oam_header_x.dying_gasp
                                      | p_rx->oam_pdu_x.oam_header_x.critical_event;
    /*�������Զ˷����ı�����Ϣ���뷢����Զ����Ϣ*/
    p_tx->oam_pdu_x.oam_header_x.remote_stable = p_rx->oam_pdu_x.oam_header_x.local_stable;
    p_tx->oam_pdu_x.oam_header_x.remote_discovering = p_rx->oam_pdu_x.oam_header_x.local_discovering;

    if (p_ctrl->src_mac_remote_copied == 0)  /*���¶Զ˵�ԴMAC*/
    {
        NBB_MEMCPY(p_ctrl->src_mac_remote, p_rx->oam_pdu_x.oam_header_x.src_mac, 6);
        p_ctrl->src_mac_remote_copied = 1;

        if (p_ctrl->discovery_status == DISCOVER_ANY)  /* ���Э������ɣ���������Ϣ������*/
        {
            return(0);
        }
    }

    /*  ������������Զ˵ĶԶ���Ϣ��������Ϣ���ݲ��ã�������  */
    return(0);   /*ͷ����ͨ���İ�������ִ�кð��ۼ�*/
RxErrorExit:
    rx_tx_oam.rx_cnt_err++; /* ����ı��Ƿ�����Լ����״̬�����Դ˲���*/
    p_ctrl->oam_pkt_rx_err++;
    return(1);
}

/**************************************************
** �������ƣ�RxOamPortResolveInf
** ���������������յ���Information ֡����
** ��    �룺port: �ձ��ĵĶ˿�
             p:�հ�����ָ��
             size:�հ����ݳ���
** ��    ������
** ��    �ߣ�yy09
** ��    �ڣ�2012.06.12
***************************************************/
NBB_VOID RxOamPortResolveInf(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p, NBB_INT size)
{
    oam_control_status *p_ctrl; /* Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu *p_tx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (*p == INF_TLV_TYPE_END)  /* �յ���TLV�İ���*/
    {
        p_ctrl->rx_pkt_type = RX_INF_NO_TLV;
        goto RxValidExit;
    }

    /*����Information TLV*/
    if (*p == INF_TLV_TYPE_LOCAL)  /*Local Information TLV*/
    {
        if (*(p + 1) == INF_TLV_LEN)    /*TLV length*/
        {
            if (*(p + INF_TLV_LEN) == INF_TLV_TYPE_END)    /*End of TLV marker*/
            {
                p_ctrl->rx_pkt_type = RX_INF_LOCAL;
                goto RxValidExit;
            }

            if (*(p + INF_TLV_LEN) == INF_TLV_TYPE_WITH_REMOTE)    /*Remote Information TLV*/
            {
                if (*(p + INF_TLV_LEN + 1) == INF_TLV_LEN)    /*�ݹ涨�ڷ��ʹ��Զ���Ϣʱ�������ORG_INF*/
                {
                    if (*(p + INF_TLV_LEN + INF_TLV_LEN) == INF_TLV_TYPE_ORG)
                    {
                        if (*(p + INF_TLV_LEN + INF_TLV_LEN + 1) == INF_ORG_TLV_LEN)
                        {
                            p_ctrl->rx_pkt_type = RX_INF_LOCAL_REMOTE; /*֡��ʽ��ȫ�̶�����EVENT�䳤��ͬ*/
                            goto RxValidExit;
                        }
                    }
                }

                if (*(p + INF_TLV_LEN + 1) == INF_TLV_TYPE_END)    /*���ݱ�׼���Զ˲���ORG��Ϣ*/
                {
                    goto RxValidExit;
                }
            }
        }
    }

    p_ctrl->rx_pkt_type = RX_INF_LOCAL_REMOTE;
    //goto RxValidExit; /*100609*/
    goto RxErrorExit;
RxValidExit:
    p_ctrl->oam_pkt_rx++; /*��ִ��unfinish����*/
    p_ctrl->rx_pkt_cnt_in_5s++;
    rx_tx_oam.rx_cnt_total++;
    p_tx->org_pdu_tlv_x.rx_inf++;
    AnalyzeRxInf(sub_cards, port, p_ctrl->rx_pkt_type, p);  /*�յ��ð����ٴ���*/
    return;
RxErrorExit:
    NBB_PRINTF("RxOamPortResolveInf:RxErrorExit....\n");
    p_ctrl->rx_pkt_type = NON_PDU_RX; /*�յ���Ч������Ч��û�յ���*/
    p_ctrl->oam_pkt_rx_err++;
    rx_tx_oam.rx_cnt_err++;
    return;
}

/**************************************************
** �������ƣ�RxOamPortResolveEvent
** ���������������յ���Event֡����
** ��    �룺port: �ձ��ĵĶ˿�
             p1:�հ�����ָ��
             size:�հ����ݳ���
** ��    ������
** ��    �ߣ�yy09
** ��    �ڣ�2012.06.12
***************************************************/
NBB_VOID RxOamPortResolveEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)
{
    oam_control_status *p_ctrl; /* Ϊ�ɶ��Ժ���ʱ��*/
    NBB_BYTE *p;
    NBB_BYTE valid_event;
    port_oam_pdu *p_tx;
    unsigned short event_sq_tmp;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p = p1;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    event_sq_tmp = (*p << 8) + *(p + 1);

    /* ����SQ��1��ʼ,�����Ǽ�����ת SQΪ0��Ҳ�Ϸ�*/
    if (event_sq_tmp == p_ctrl->event_rx_sq)
    {
        /* �ظ��¼�ͨ�棬��ֻ���������账�� */
        goto RxValidExit;
    }

    p += 2;

    if (*p == EVENT_TLV_TYPE_END)  /* ʵ����û���κ��¼�����,��Ҳ��ϸ������*/
    {
        goto RxValidExit;
    }

    valid_event = 0;

    while (*p)  /* �Ƿ�Ϊ0����ʶ���ݽ���*/
    {
        switch (*p)
        {
            case EVENT_TLV_TYPE_SYM_PERIOD:
                if (*(p + 1) == EVENT_SYM_TLV_LEN)    /* �¼����ȺϷ�*/
                {
                    p_ctrl->rx_sym_err = 1;
                    p_ctrl->rx_sym_err_cnt = 1;
                    AnalyzeRxSymErrEvent(sub_cards, port, p);  /* �����յ����¼�*/
                    p = p + EVENT_SYM_TLV_LEN; /* ��ָ��ָ����һ���¼�����*/
                    valid_event++;
                }
                else /* �¼����Ȳ��Ϸ�*/
                {
                    if (valid_event > 0) /* �����¼����Ȳ��Ϸ������账��,�������кϸ��¼������Լ���*/
                    {
                        goto RxValidExit;
                    }
                    else /* û��һ���ϸ��¼����Ǵ��*/
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_FRM:
                if (*(p + 1) == EVENT_FRM_TLV_LEN)
                {
                    p_ctrl->rx_frm_err = 1;
                    AnalyzeRxFrmErrEvent(sub_cards, port, p);
                    p = p + EVENT_FRM_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_FRM_PERIOD:
                if (*(p + 1) == EVENT_FRM_PRD_TLV_LEN)
                {
                    p_ctrl->rx_frm_prd_err = 1;
                    AnalyzeRxFrmPeriodEvent(sub_cards, port, p);
                    p = p + EVENT_FRM_PRD_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_SECOND_SUM:
                if (*(p + 1) == EVENT_FRM_SECOND_TLV_LEN)
                {
                    p_ctrl->rx_frm_snd_err = 1;
                    AnalyzeRxFrmSecondEvent(sub_cards, port, p);
                    p = p + EVENT_FRM_SECOND_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_ORG: /* ORG�¼����ж�ʱ������ΪûORG�¼������ϸ�����ORG_INFO��ͬ*/
                if (*(p + 1) == EVENT_ORG_TLV_LEN)
                {
                    AnalyzeRxOrgEvent(sub_cards, port, p);
                    p = p + EVENT_ORG_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            default: /* �¼����ʹ�*/
                if (valid_event > 0)
                {
                    goto RxValidExit;
                }
                else
                {
                    goto RxErrorExit;
                }
        }
    }

RxValidExit:
    p_ctrl->rx_pkt_type = RX_EVENT; /* ��ִ�� unfinish ����*/
    p_ctrl->oam_pkt_rx++; /* �˿�����*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5���ڼ������ж��Ƿ�ʱû�հ�*/
    rx_tx_oam.rx_cnt_total++; /* ���ж˿ڵ�����*/
    p_tx->org_pdu_tlv_x.rx_event++;
    return;
RxErrorExit:
    p_ctrl->rx_pkt_type = NON_PDU_RX;         /* �յ���Ч������Ч��û�յ���*/
    p_ctrl->oam_pkt_rx_err++;
    rx_tx_oam.rx_cnt_err++;
    return;
}

NBB_VOID RxOamPortResolveVarReq(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size) /*
                                                                                SIZE�������ã�ͨ���ж�BRANCH=0��ʶ��������*/
{
    oam_control_status *p_ctrl; /* Ϊ�ɶ��Ժ���ʱ��     */
    NBB_BYTE valid_var_req;
    port_oam_pdu *p_tx;
    port_oam_pdu *p_rx;
    NBB_BYTE i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    valid_var_req = 0;
    NBB_MEMCPY(&(p_rx->var_req_x[0]), p1, 5 * sizeof(var_req));

    for (i = 0; i < 5; i++)
    {
        if (p_rx->var_req_x[i].branch > 0)
        {
            NBB_MEMCPY(&(p_tx->var_res_x[i]), &(p_rx->var_req_x[i]), sizeof(var_req));         /*Ϊ����Ӧ����׼��*/
            valid_var_req++;
        }
        else
        {
            break;
        }
    }

    p_ctrl->unfinished = UNF_VAR_RES;
    p_rx->var_num = valid_var_req;
    p_tx->var_num = valid_var_req;
    ReactRxVarReq(sub_cards, port);  /* ���Ӧ�����ݣ�Ϊ����׼��*/
    p_ctrl->rx_pkt_type = RX_REQ;
    p_ctrl->oam_pkt_rx++; /* �˿�����*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5���ڼ������ж��Ƿ�ʱû�հ�*/
    rx_tx_oam.rx_cnt_total++; /* ���ж˿ڵ�����*/
    p_tx->org_pdu_tlv_x.rx_var_req++; /* �����˼��������ڷ��͵��Զ˵�*/
}

NBB_VOID RxOamPortResolveVarRes(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)  /* ͬ��ѯ����*/
{
    oam_control_status *p_ctrl; /* Ϊ�ɶ��Ժ���ʱ��     */
    NBB_BYTE valid_var_req;
    port_oam_pdu *p_tx;
    port_oam_pdu *p_rx;
    NBB_BYTE i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    valid_var_req = 0;
    NBB_MEMCPY(&(p_rx->var_res_x[0]), p1, 5 * sizeof(var_res));

    for (i = 0; i < 5; i++)
    {
        if (p_rx->var_res_x[i].branch == VALID_BRANCH)
        {
            switch (p_rx->var_res_x[i].leaf)  /* 2/5/6/8/0xe*/
            {
                case 2:
                    p_ctrl->remote_var_cnt[TX_FRM] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                case 5:
                    p_ctrl->remote_var_cnt[RX_FRM] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                case 6:
                    p_ctrl->remote_var_cnt[RX_CRC] = p_rx->var_res_x[i].value;
                    valid_var_req++;

                case 8:
                    p_ctrl->remote_var_cnt[TX_BYTE] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                case 0xe:
                    p_ctrl->remote_var_cnt[RX_BYTE] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                default:
                    break;
            }
        }
    }

    p_rx->var_num = valid_var_req; /* ��δ�����޵ȴ�Ӧ������*/
    p_ctrl->rx_pkt_type = RX_RES;
    p_ctrl->oam_pkt_rx++; /* �˿�����*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5���ڼ������ж��Ƿ�ʱû�հ�*/
    rx_tx_oam.rx_cnt_total++; /* ���ж˿ڵ�����*/
    p_tx->org_pdu_tlv_x.rx_var_res++;
}

NBB_VOID RxOamPortResolveLb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)  /*����*/
{
    port_oam_pdu *p_rx; /* Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    NBB_MEMCPY(&(p_rx->lb_cmd_x), p1, sizeof(lb_cmd));
    p_ctrl->oam_pkt_rx++; /* �˿�����*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5���ڼ������ж��Ƿ�ʱû�հ�*/
    rx_tx_oam.rx_cnt_total++; /* ���ж˿ڵ�����*/

    if (p_tx->inf_tlv_local_x.loopback_spt == 0)
    {
        return;
    }

    if (p_rx->lb_cmd_x.cmd == LB_DISENB)
    {
        p_ctrl->rx_pkt_type = RX_LB_DISENB;       /* �����ͱ������     */
        p_tx->org_pdu_tlv_x.rx_dislb_cmd++; /* �յ��������ͱ����������Ӧ�ۼ�*/
        p_ctrl->unfinished = UNF_DIS_LB_EXE;
    }

    if (p_rx->lb_cmd_x.cmd == LB_ENB)
    {
        p_ctrl->rx_pkt_type = RX_LB_ENB;
        p_tx->org_pdu_tlv_x.rx_lb_cmd++;

        if (p_ctrl->finished_state == FINISH_NORMAL)  /* ���˴�������̬���ſ���ӦԶ�˷����Ļ�������*/
        {
            p_ctrl->unfinished = UNF_LB_EXE; /* ֻ���ڷ�����Ӧ��֡�����MAC�������Żظ���ֵ*/
        }
        else
        {
            if (p_ctrl->unfinished == UNF_LB_CMD || p_ctrl->finished_state == FINISH_LB_LOCAL_CMD)
            {
                if (NBB_MEMCMP(p_ctrl->src_mac_remote, src_mac_overall, 6) < 0)
                {
                    p_ctrl->unfinished = UNF_LB_EXE;
                }
            }
        }
    }
}

NBB_VOID RxOamPortResolveOrg(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)  /* ����*/
{
    port_oam_pdu *p_rx; /* Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    NBB_MEMCPY(&(p_rx->org_pdu_tlv_x), p1, sizeof(org_pdu_tlv));
    p_ctrl->rx_pkt_type = RX_ORG_PDU; /* ���漰  unfinished �Ĳ���              */
    p_tx->org_pdu_tlv_x.rx_org_pdu++;
    p_ctrl->oam_pkt_rx++; /* �˿�����*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5���ڼ������ж��Ƿ�ʱû�հ�*/
    rx_tx_oam.rx_cnt_total++; /* ���ж˿ڵ�����*/
}

NBB_VOID RxOamPortResolveCodeErr(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu *p_rx; /* Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_pkt_type = NON_PDU_RX;         /* �յ���Ч������Ч��û�յ���,�����κδ���*/
    p_ctrl->oam_pkt_rx_err++;
    rx_tx_oam.rx_cnt_err++;
}

NBB_BYTE RxOamPortResolve(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *pData, NBB_INT size)  /*���˿��յ��Լ���OAM�����������*/
{
    oam_header *p_oam_header; /* Ϊ����������ݶ����õ���ʱָ�����*/
    NBB_BYTE *p_oam_pdu_data;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->oppsite_active = 1; /*�Զ˼���*/
    p_oam_header = (oam_header *)pData;

    if (TRUE == RxOamPortResolveHdr(sub_cards, port, p_oam_header)) /*�����ͷ�д�������������˳�*/
    {
        return 1;
    }

    p_oam_pdu_data = pData + OAM_HDR_SIZE; /*p_oam_pdu_dataָ�����ͬCFM PDU Header��ĵ�һ�ֽ�*/

    switch (*(&(p_oam_header->code)))
    {
        case CODE_INF:
            RxOamPortResolveInf(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_EVENT:
            RxOamPortResolveEvent(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_VAR_REQ:
            RxOamPortResolveVarReq(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_VAR_RES:
            RxOamPortResolveVarRes(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_LOOPBACK:
            RxOamPortResolveLb(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_ORG:
            RxOamPortResolveOrg(sub_cards, port, p_oam_pdu_data, size);
            break;

        default:
            RxOamPortResolveCodeErr(sub_cards, port);
            break;
    }

    return 0;
}

/**************************************************
** �������ƣ�RxLinkOam
** ������������EFM OAM PDU
** ��    �룺uint: �ձ���оƬ��Ԫ��Ŀǰδ0
             pkt:�հ���Ϣָ�룬����:���ݣ���������˿ڵ���Ϣ
** ��    ������
** ��    �ߣ�yy09
** ��    �ڣ�2012.06.12
***************************************************/
c3_rx_ret_t RxLinkOam(NBB_INT unit, c3_rxtx_pkt_t *pkt)
{
    NBB_BYTE *pData;
    NBB_BYTE rx_port, rx_card, i;
    NBB_USHORT data_size;
    minm_header_t *p_head;
    rx_pkts++;
    pData = pkt->payload; /*���ݴ��⿪ʼ��*/
    data_size = pkt->len;

    if (data_size > (oam_max_size + 0))    /*�������*/
    {
        goto RxErrorExit;
    }

    pData = pkt->payload; /*��������ָ��*/
    p_head = pkt->head; //MACINMACͷ

    /*�����հ��˿�*/
    if (ETH_PORT_TYPE != p_head->port_type)
    {
        goto RxErrorExit;//�˿����Ͳ���ȷ
    }

    rx_card = p_head->port_instance.eth_port_bits.cards;

    if (2 == rx_card) /*�˿ڼ��*/
    {
        rx_port = p_head->port_instance.eth_port_bits.ports - 65;

        if (rx_port > (MAX_PORT_NUM - 1))
        {
            goto RxErrorExit;
        }
    }
    else
    {
        rx_port = p_head->port_instance.eth_port_bits.ports - 33;

        if (rx_port > (MAX_PORT_NUM - 1))
        {
            goto RxErrorExit;
        }
    }

    for (i = 0 ; i < SUB_NUM; i++)
    {
        if (g_efm_oam_port_cfg[i].port_oam_x[rx_port].enb == 0
            || g_efm_oam_port_cfg[i].port_oam_enb_cfg_last[rx_port] == 0)  /*�յ��Ķ˿�δʹ��OAM*/
        {
            goto RxErrorExit;
        }

        if (RxOamPortResolve(i, rx_port, pData, data_size))   /*���Ľ�������*/
        {
            goto RxErrorExit;
        }
    }

    return C3_RX_HANDLED;
RxErrorExit:
    rx_tx_oam.rx_cnt_err++; /*���ܳ��ȷǷ���Ŀ�Ķ˿�δ����OAM����*/
    return C3_RX_NOT_HANDLED;
}

NBB_VOID AnalyzeRxInf(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE rx_pkt_type, NBB_BYTE *p)
{
    port_oam_pdu *p_rx; /* ֻΪ��ʽ�Ͽɶ��Ժ���ʱ��*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    inf_tlv inf_tlv_tmp; /* ֻ��Ϊ�˷����д�����õ���ʱ����*/
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    NBB_MEMCPY((char *)(&inf_tlv_tmp), p, INF_TLV_LEN);

    if (rx_pkt_type == RX_INF_NO_TLV)  /* ֻ��ʾ�Ƿ����¼������ڰ�ͷ�Ѵ�����*/
    {
        return;
    }

    NBB_MEMCPY(&(p_rx->inf_tlv_local_x), p, INF_TLV_LEN);     /* ���ڽ������ı���*/
    /*���ڷ������ĶԶˣ�Ϊ���ŵķ���������׼��*/
    NBB_MEMCPY(&(p_tx->inf_tlv_remote_x), p, INF_TLV_LEN);
    /*ֻ���������Զ���Ϣ������͸�һ��*/
    p_tx->inf_tlv_remote_x.type = INF_TLV_TYPE_WITH_REMOTE;

    /*ֻ���շ�ͬ����ORG��Ϣ������remote_inf���账��*/
    if (rx_pkt_type == RX_INF_LOCAL_REMOTE)
    {
        NBB_MEMCPY(&(p_rx->org_inf_tlv_x), p + INF_TLV_LEN + INF_TLV_LEN,
                   INF_ORG_TLV_LEN);
        p_ctrl->remote_sq = p_rx->org_inf_tlv_x.local_sq;
    }
}

NBB_VOID AnalyzeRxSymErrEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)  /* �Զ˸�Ĵ�����ż�¼*/
{
    port_oam_pdu *p_rx;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /*�����ݴ�������� */
    NBB_MEMCPY(&(p_rx->err_sym_period_tlv_x), p, EVENT_SYM_TLV_LEN);
    p_ctrl->rx_sym_err_cnt = p_rx->err_sym_period_tlv_x.err;
}

NBB_VOID AnalyzeRxFrmErrEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)  /* �Զ˸�Ĵ���֡��¼*/
{
    port_oam_pdu *p_rx;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /* �����ݴ�������� */
    NBB_MEMCPY(&(p_rx->err_frm_tlv_x), p, EVENT_FRM_TLV_LEN);
    p_ctrl->rx_frm_err_cnt = p_rx->err_frm_tlv_x.err;
}

NBB_VOID AnalyzeRxFrmPeriodEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)
/* �Զ˸��ָ�����ڴ���֡��¼*/
{
    port_oam_pdu *p_rx;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /* �����ݴ�������� */
    NBB_MEMCPY(&(p_rx->err_frm_period_tlv_x), p, EVENT_FRM_PRD_TLV_LEN);
    p_ctrl->rx_frm_prd_err_cnt = p_rx->err_frm_period_tlv_x.err;
}

/* �Զ˸�Ĵ������֡������¼*/
NBB_VOID AnalyzeRxFrmSecondEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)
{
    port_oam_pdu *p_rx; /* Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /* �����ݴ�������� */
    NBB_MEMCPY(&(p_rx->err_frm_second_tlv_x), p, EVENT_FRM_SECOND_TLV_LEN);
    p_ctrl->rx_frm_snd_err_cnt = p_rx->err_frm_second_tlv_x.err;
}

NBB_VOID AnalyzeRxOrgEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)
{
    port_oam_pdu *p_rx; /* Ϊ�ɶ��Ժ���ʱ��     */
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    /* �����ݴ�������� */
    NBB_MEMCPY(&(p_rx->org_event_tlv_x), p, EVENT_ORG_TLV_LEN);
    p_ctrl->remote_sq = p_rx->org_event_tlv_x.local_sq;
}

NBB_VOID ReactRxVarReq(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status *p_ctrl; /* Ϊ�ɶ��Ժ���ʱ��     */
    port_oam_pdu *p_tx;
    NBB_BYTE i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    for (i = 0; i < p_tx->var_num; i++)
    {
        if (p_tx->var_res_x[i].branch == VALID_BRANCH)
        {
            switch (p_tx->var_res_x[i].leaf)  /* 2/5/6/8/0xe*/
            {
                case 2:
                    /* �ݶ���һ�лظ��ı���ֵռ4�ֽ�*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[TX_FRM];
                    break;

                case 5:
                    /* �ݶ���һ�лظ��ı���ֵռ4�ֽ�*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[RX_FRM];
                    break;

                case 6:
                    /* �ݶ���һ�лظ��ı���ֵռ4�ֽ�*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[RX_CRC];
                    break;

                case 8:
                    /* �ݶ���һ�лظ��ı���ֵռ4�ֽ�*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[TX_BYTE];
                    break;

                case 0xe:
                    /* �ݶ���һ�лظ��ı���ֵռ4�ֽ�*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[RX_BYTE];
                    break;

                default:
                    p_tx->var_res_x[i].width = 0x81;
                    p_tx->var_res_x[i].value = 0;
                    break;
            }
        }
        else
        {
            /* ���˲�֧�ֻ���ԭ����Ӧ��Ҫע���ʱ value �����ݼ���ռ�ֽ�*/
            p_tx->var_res_x[i].width = 0x81;
            p_tx->var_res_x[i].value = 0;
        }
    }
}
/* ��ʼ��oam_cfg_tx oam_pdu_cfg_tx ��������������*/
NBB_VOID PortOamCtrlInitCfg(oam_control_status* p, NBB_BYTE port)
{
    p->max_size_recept = 1518; /* Э��ֵΪĬ�ϵ�1518*/
}

/* ���ö�ÿ��ʹ��OAM�Ķ˿ڷ������ݽṹ��ʼ��,�������������ʼ��*/
NBB_VOID PortOamPduInitCfg(NBB_BYTE sub_cards, port_oam_pdu* p, NBB_BYTE port)
{
    NBB_MEMCPY(p->oam_pdu_x.oam_header_x.dst_mac, dst_mac_overall, 6);   /* ��ͷ*/
    NBB_MEMCPY(p->oam_pdu_x.oam_header_x.src_mac, src_mac_overall, 6);
    NBB_MEMCPY(p->oam_pdu_x.oam_header_x.len_type, len_type_overall, 2);
    p->oam_pdu_x.oam_header_x.subtype = subtype_overall; /* һ���ֽڲ�����  OS_MEMSET */
    /* Ϊ�������ݣ���������ͷ������һ�οռ䣬*/
    p->oam_pdu_x.data = (NBB_BYTE*)NBB_MM_ALLOC(oam_max_size, NBB_NORETRY_ACT,
                        MEM_SPM_OAM_CFG_CB);
    p->inf_tlv_local_x.type = INF_TLV_TYPE_LOCAL; /* ������Ϣ��*/
    p->inf_tlv_local_x.length = INF_TLV_LEN;
    p->inf_tlv_local_x.oam_ver = 0x01; /* δ�ٸ�ֵ�Ķ���ȡ��ʼ����Ϊ0*/
    p->inf_tlv_local_x.revision = 0x00;
    p->inf_tlv_local_x.oam_mode = g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[port];
    p->inf_tlv_local_x.unidirection_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_uni_cfg[port];
    p->inf_tlv_local_x.loopback_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_lb_cfg[port];
    p->inf_tlv_local_x.link_event_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_event_cfg[port];
    p->inf_tlv_local_x.var_response_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_var_cfg[port];
    p->inf_tlv_local_x.max_size = g_efm_oam_port_cfg[sub_cards].port_oam_maxsize_cfg[port];
    NBB_MEMCPY(p->inf_tlv_local_x.oui, wri_fh_oui, 3);
    NBB_MEMCPY(p->inf_tlv_local_x.ven_spe_inf, board_name, 4);
    p->inf_tlv_remote_x.type = INF_TLV_TYPE_WITH_REMOTE; /* �Զ���Ϣ��*/
    p->inf_tlv_remote_x.length = INF_TLV_LEN;
    p->inf_tlv_remote_x.oam_ver = 0x01;
    p->org_inf_tlv_x.type = 0xfe; /* �Զ�������Ϣ���д��ݵ��շ�SQ*/
    p->org_inf_tlv_x.length = EVENT_ORG_TLV_LEN;
    NBB_MEMCPY(p->org_inf_tlv_x.oui, wri_fh_oui, 3);
    p->org_event_tlv_x.type = 0xfe; /* �Զ������¼����д��ݵ��շ�SQ*/
    p->org_event_tlv_x.length = EVENT_ORG_TLV_LEN;
    /* Ŀǰ���Ƕ����ORQ���¼�����Ϣ�����ݵ�������ʵ��ȫ��ͬ����*/
    NBB_MEMCPY(p->org_event_tlv_x.oui, wri_fh_oui, 3);
    NBB_MEMCPY(p->org_pdu_tlv_x.oui, wri_fh_oui, 3);   /* ֡��27*/
    p->err_sym_period_tlv_x.type = EVENT_TLV_TYPE_SYM_PERIOD;
    p->err_sym_period_tlv_x.length = EVENT_SYM_TLV_LEN;
    p->err_sym_period_tlv_x.upper_window = 0x0;
    /*�ݶ���1G��1���ڷ���SYMBOL����Ӧ����ֽڼ�4B/5Bת����Ӧһ��SYMBOL����
    ��Ӧ������շ�SYMBOL��������*/
    p->err_sym_period_tlv_x.window = 0x1DCD6500 * g_efm_oam_port_cfg[sub_cards].port_oam_sym_period_cfg[port];
    /* ��ȡPHY�㹤��״̬ */
    p->err_sym_period_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_sym_threshold_cfg[port];
    p->err_frm_tlv_x.type = EVENT_TLV_TYPE_FRM;
    p->err_frm_tlv_x.length = EVENT_FRM_TLV_LEN;
    p->err_frm_tlv_x.window = g_efm_oam_port_cfg[sub_cards].port_oam_frm_period_cfg[port]; /* 1����*/
    /* �ʹ������ʱһ����ֻ������ͳ�Ʒ�Χ����Ϊ60��*/
    p->err_frm_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_frm_threshold_cfg[port];
    p->err_frm_period_tlv_x.type = EVENT_TLV_TYPE_FRM_PERIOD;
    p->err_frm_period_tlv_x.length = EVENT_FRM_PRD_TLV_LEN;
    /* 1����ǧ�״�64�ֽڰ���*/
    p->err_frm_period_tlv_x.window = 1488000 * g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_period_cfg[port];
    /* ��ʾ���Ǳ������Ҫ��������Ϊ�ڸ�����ʱ���ڲŷ���*/
    p->err_frm_period_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_threshold_cfg[port];
    p->err_frm_second_tlv_x.type = EVENT_TLV_TYPE_SECOND_SUM;
    p->err_frm_second_tlv_x.length = EVENT_FRM_SECOND_TLV_LEN;
    p->err_frm_second_tlv_x.window = g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_period_cfg[port]; /* 1������*/
    /* ������1���д���֡  ������Խ���Ĵ��󣬵�ͳ�Ƹ�ȫ*/
    p->err_frm_second_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_threshold_cfg[port];
    /* var_req_x  var_res_x  lb_cmd_x����ʼ��ֵ0������˳�ʼ��      */
}

/* ������Щ�˿ڿ���OAM�������Ƿ�ʹ������OAM�շ�����*/
NBB_VOID AllPortOamCfgEnb(NBB_VOID)
{
    NBB_BYTE enb, i, j;
    enb = 0;

    /*port_oam_enb_cfg[26]=1;
    port_oam_enb_cfg[27]=1;
    lb_overtime=10;
    g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[26]=1;
    g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[27]=1;
    ApiSetEtherOamPacketToCpu(0, 26, dst_mac_overall, 0x8809, 1);
    ApiSetEtherOamPacketToCpu(0, 27, dst_mac_overall, 0x8809, 1);

    ApiSetPortPvid(0,28,5);
    ApiSetPortPvid(0,29,5);
    ApiSetPortPvid(0,30,5);
    ApiSetPortPvid(0,31,5);

    ApiSetPortPvid(0,26,55);
    ApiSetPortPvid(0,27,33);


    src_mac_overall[3]=0x1;
    src_mac_overall[4]=0x1;
    src_mac_overall[5]=0x1;*/
    for (j = 0 ; j < SUB_NUM; j++)
    {
        for (i = 0; i < MAX_PORT_NUM; i++)
        {
            if (TRUE == g_efm_oam_port_cfg[j].port_oam_enb_cfg[i])
            {
                enb = 1;
            }

            if (g_efm_oam_port_cfg[j].port_oam_enb_cfg[i]
                != g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i])
            {
                if (TRUE == g_efm_oam_port_cfg[j].port_oam_enb_cfg[i])
                {
                    g_efm_oam_port_cfg[j].port_oam_x[i].enb = 1;
                    g_efm_oam_port_cfg[j].port_oam_x[i].port_num = i;
                    g_efm_oam_port_cfg[j].port_oam_x[i].p_tx = (port_oam_pdu*)NBB_MM_ALLOC(sizeof(port_oam_pdu),
                            NBB_NORETRY_ACT, MEM_SPM_OAM_CFG_CB);
                    g_efm_oam_port_cfg[j].port_oam_x[i].p_rx = (port_oam_pdu*)NBB_MM_ALLOC(sizeof(port_oam_pdu),
                            NBB_NORETRY_ACT, MEM_SPM_OAM_CFG_CB);
                    g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl =
                        (oam_control_status*)NBB_MM_ALLOC(sizeof(oam_control_status),
                                                          NBB_NORETRY_ACT, MEM_SPM_OAM_CFG_CB);
                    OS_MEMSET(g_efm_oam_port_cfg[j].port_oam_x[i].p_tx, 0, sizeof(port_oam_pdu));     /* ȫ��������ֵ0*/
                    OS_MEMSET(g_efm_oam_port_cfg[j].port_oam_x[i].p_rx, 0, sizeof(port_oam_pdu));
                    OS_MEMSET(g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl, 0, sizeof(oam_control_status));
                    /*Ϊ�ö˿ڷ������ݽṹ��ʼ��������DATA�ռ䣬�����������㼴��*/
                    PortOamPduInitCfg(j, g_efm_oam_port_cfg[j].port_oam_x[i].p_tx, i);
                    /* ��ʼ��oam_cfg_tx oam_pdu_cfg_tx*/
                    PortOamCtrlInitCfg(g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl, i);
                }
                else
                {
                    // NBB_PRINTF("port %d delete\n",i);
                    g_efm_oam_port_cfg[j].port_oam_x[i].enb = 0;
                    g_efm_oam_port_cfg[j].port_oam_x[i].port_num = i;

                    /* ���ָ���ֱ�Ӹ�ֵ��  NBB_VOID*���������*/
                    if (NULL != g_efm_oam_port_cfg[j].port_oam_x[i].p_tx)
                    {
                        NBB_MM_FREE(g_efm_oam_port_cfg[j].port_oam_x[i].p_tx, MEM_SPM_OAM_CFG_CB);
                        g_efm_oam_port_cfg[j].port_oam_x[i].p_tx = NULL;
                    }

                    if (NULL != g_efm_oam_port_cfg[j].port_oam_x[i].p_rx)
                    {
                        NBB_MM_FREE(g_efm_oam_port_cfg[j].port_oam_x[i].p_rx, MEM_SPM_OAM_CFG_CB);
                        g_efm_oam_port_cfg[j].port_oam_x[i].p_rx = NULL;
                    }

                    if (NULL != g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl)
                    {
                        NBB_MM_FREE(g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl, MEM_SPM_OAM_CFG_CB);
                        g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl = NULL;
                    }
                }

                g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i] = g_efm_oam_port_cfg[j].port_oam_enb_cfg[i];
            }
        }

        PortOamEnableApi(0, enb);
    }
}

NBB_VOID RxTxLinkOamEnbInit(NBB_VOID)/* ���շ�OAM�ṹ����*/
{
    OS_MEMSET(&rx_tx_oam, 0, sizeof(rx_tx_oam));     /* ȫ��������ֵ0*/
}

/* ִ��ע���ע��OAM�շ�����*/
NBB_INT PortOamEnableApi(NBB_INT unit, NBB_BYTE enable)
{
    unsigned char funcname[FUNCNMLEN] = {0};
    NBB_INT  rx_error = 0, tx_error = 0;
    c3_rxreg_info_t rxcb;

    if ((enable == 1) && (oam_enb_cfg == 0))
    {
        /*1) ע���������*/
        rxcb.name = funcname;
        rxcb.priority = 0;
        rxcb.callback = RxLinkOam;
        rxcb.type = 18;//��ʱ��
        rx_error = ApiC3RxCbRegister(unit, &rxcb);
        /*2) ������������*/
        RxTxLinkOamEnbInit();
        oam_enb_cfg = 1;
    }
    else if ((enable == 0) && (oam_enb_cfg == 1))
    {
        oam_enb_cfg = 0;
        /*1) ע����������*/
        rxcb.priority = 0;
        rxcb.callback = RxLinkOam;
        rxcb.type = 18;//��ʱ��
        rx_error = ApiC3RxCbUnRegister(unit, &rxcb);
    }

    rx_tx_oam.rx_error = rx_error;
    rx_tx_oam.tx_error = tx_error;
    return (rx_error < 0) ? rx_error : tx_error;
}

/************************************  ��Ӧ
****************************************/
NBB_VOID PortOamEnbCfg(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE cfg)
{
    g_efm_oam_port_cfg[sub_cards].port_oam_x[port].enb = cfg;
}

/*****************************************************************************
���ܣ� ִ��״̬���Ӳ�ѯ�����Ӧ��������װ���û�����BCT
Э��ʱ������ԭЭ��������޸ĺ�������
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�
����� ָ��CHAR �����ָ��
˵���� һ���û������ע����ô˺��������û���������޸�BCT
Э�飬��Ӧ��ɾ�˺���ĳЩ�д���
�޸ģ�
******************************************************************************/
NBB_BYTE *QueryPortOamApi(NBB_BYTE sub_cards, NBB_BYTE port)
{
    NBB_BYTE i = 0;
    port_oam_pdu                    *p_tx;
    port_oam_pdu                    *p_rx;
    oam_control_status              *p_ctrl;
    OS_MEMSET(lookup, 0, LOOKUP_MAX_SIZE);

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg[port] == 0)
    {
        return(lookup);
    }

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg[port])
    {
        p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
        p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        lookup[i++] = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].enb;
        lookup[i++] = p_tx->inf_tlv_local_x.oam_mode;
        lookup[i++] = p_ctrl->oppsite_active;

        /*110409 ���յ��Զ�LINK FAULTʱ�߼���SEND̬*/
        if (p_rx->oam_pdu_x.oam_header_x.link_fault)
        {
            lookup[i++] = 0;
        }
        else
        {
            if (p_tx->oam_pdu_x.oam_header_x.local_discovering == DISCOVER_UNFINISHED)
            {
                lookup[i++] = 0;
            }
            else
            {
                lookup[i++] = p_tx->oam_pdu_x.oam_header_x.local_stable + 1;
            }
        }

        lookup[i++] = (p_ctrl->max_size_recept >> 8);
        lookup[i++] = (p_ctrl->max_size_recept);

        if (p_rx->oam_pdu_x.oam_header_x.link_fault)  //110409
        {
            lookup[i++] = DISCOVER_SEND;
        }
        else
        {
            lookup[i++] = p_ctrl->discovery_status;
        }

        lookup[i++] = p_ctrl->local_pdu;
        lookup[i++] = p_ctrl->finished_state;
        lookup[i++] = p_tx->inf_tlv_local_x.mux_action;

        /*lookup[i++]=p_tx->inf_tlv_local_x.par_action;100606*/
        if (p_tx->inf_tlv_local_x.par_action == 1)
        {
            lookup[i++] = 2;
        }
        else if (p_tx->inf_tlv_local_x.par_action == 2)
        {
            lookup[i++] = 1;
        }
        else
        {
            lookup[i++] = 0;
        }

        lookup[i++] = p_ctrl->var_lb_fail_ais;
        lookup[i++] = p_ctrl->local_link_status_last;
        lookup[i++] = p_ctrl->remote_link_status_last;
        lookup[i++] = p_ctrl->oppsite_active;
        lookup[i++] = p_rx->inf_tlv_local_x.oam_mode;

        if (p_ctrl->finished_state == FINISH_LB_REMOTE_EXE)
        {
            lookup[i++] = 1;
        }
        else if (p_rx->inf_tlv_local_x.par_action == PAR_LB)
        {
            lookup[i++] = 2;
        }
        else
        {
            lookup[i++] = 0;
        }

        lookup[i++] = p_rx->inf_tlv_local_x.mux_action;

        if (p_rx->inf_tlv_local_x.par_action == 1)
        {
            lookup[i++] = 2;
        }
        else if (p_rx->inf_tlv_local_x.par_action == 2)
        {
            lookup[i++] = 1;
        }
        else
        {
            lookup[i++] = 0;
        }

        /*lookup[i++]=p_rx->inf_tlv_local_x.par_action;100606  ԭ����LB��DISCARD
        ״̬�㷴��*/
        NBB_MEMCPY(&(lookup[i]), p_ctrl->src_mac_remote, 6);
        i = i + 6;
        NBB_MEMCPY(&(lookup[i]), p_rx->inf_tlv_local_x.oui, 3);
        i = i + 3;
        lookup[i++] = p_rx->var_res_x[0].branch;
        lookup[i++] = p_rx->var_res_x[0].leaf;

        if (p_ctrl->late_rx_var_res)
        {
            lookup[i++] = 1;
        }
        else
        {
            lookup[i++] = ((p_rx->var_res_x[0].width) >> 7) & 0x1;
        }

        lookup[i++] = ((p_rx->var_res_x[0].value) >> 8);
        lookup[i++] = ((p_rx->var_res_x[0].value));
        lookup[i++] = p_ctrl->rx_sym_err;
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_sym_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_sym_period_tlv_x.window / 0x1DCD6500);
        lookup[i++] = ((p_ctrl->rx_sym_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_sym_err_cnt));
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.event_ttl));
#if 0
        lookup[i++] = p_ctrl->rx_frm_err;
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.window);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt));
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->rx_frm_prd_err;
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.window / 1488000);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl));
#endif
        lookup[i++] = p_ctrl->rx_frm_prd_err;/*110616
����֡�����¼��ʹ���֡�¼��㷴����*/
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.window / 1488000);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->rx_frm_err;
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.window);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt));
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->rx_frm_snd_err;
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_second_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_second_tlv_x.window);
        lookup[i++] = ((p_ctrl->rx_frm_snd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_snd_err_cnt));
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.event_ttl));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_inf) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_inf));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_lb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_lb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_dislb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_dislb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_event) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_event));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_req) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_req));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_res) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_res));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_inf) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_inf));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_lb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_lb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_dislb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_dislb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_event) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_event));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_req) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_req));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_res) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_res));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_inf) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_inf));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_lb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_lb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_dislb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_dislb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_event) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_event));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_req) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_req));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_res) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_res));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_inf) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_inf));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_lb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_lb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_dislb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_dislb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_event) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_event));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_req) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_req));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_res) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_res));
        lookup[i++] = p_ctrl->tx_sym_err;
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_sym_period_tlv_x.threshold);
        /*1000 Base T ȡ  0x1DCD6500 ��Ӧ500M������100BaseTx  ȡ 0x7735940 ����Ӧ
        125M������10Base Tȡ 0x1312d00 ��20M  125*/
        lookup[i++] = (NBB_BYTE)(p_tx->err_sym_period_tlv_x.window / 0x1DCD6500);
        lookup[i++] = ((p_ctrl->tx_sym_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_sym_err_cnt));
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->tx_frm_err;
        lookup[i++] = ((p_tx->err_frm_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_frm_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_tlv_x.window);
        lookup[i++] = ((p_ctrl->tx_frm_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_frm_err_cnt));
        lookup[i++] = ((p_tx->err_frm_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_frm_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->tx_frm_prd_err;
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_period_tlv_x.window / 1488000);
        lookup[i++] = ((p_ctrl->tx_frm_prd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_frm_prd_err_cnt));
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->tx_frm_snd_err;
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_second_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_second_tlv_x.window);
        lookup[i++] = ((p_ctrl->tx_frm_snd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_frm_snd_err_cnt));
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.event_ttl));
    }

    p_ctrl->late_rx_var_res = 0;
    p_rx->var_res_x[0].branch = 0;
    p_rx->var_res_x[0].leaf = 0;
    p_rx->var_res_x[0].width = 0;
    p_rx->var_res_x[0].value = 0;
    p_ctrl->rx_sym_err = 0;
    p_ctrl->rx_sym_err_cnt = 0;
    p_ctrl->rx_frm_err = 0;
    p_ctrl->rx_frm_err_cnt = 0;
    p_ctrl->rx_frm_prd_err = 0;
    p_ctrl->rx_frm_prd_err_cnt = 0;
    p_ctrl->rx_frm_snd_err = 0;
    p_ctrl->rx_frm_snd_err_cnt = 0;
    p_ctrl->tx_sym_err = 0;
    p_ctrl->tx_sym_err_cnt = 0;
    p_ctrl->tx_frm_err = 0;
    p_ctrl->tx_frm_err_cnt = 0;
    p_ctrl->tx_frm_prd_err = 0;
    p_ctrl->tx_frm_prd_err_cnt = 0;
    p_ctrl->tx_frm_snd_err = 0;
    p_ctrl->tx_frm_snd_err_cnt = 0;
    /*for(i=0;i<170;i++)
    {
            if(lookup[i]<0x10)
                    NBB_PRINTF("0%x  ", lookup[i]);
                    else
                            NBB_PRINTF("%x  ", lookup[i]);

            if((i+1)%10==0)
                    NBB_PRINTF("....LINE %d \n",(i+1));

    }101229*/
    return(lookup);
}

#if 0
/*****************************************************************************
���ܣ� ��ȡ��оƬ��������MAC������OAM�ı���ԴMAC��Ӧ�� CONFIG��
�����ö˿�ǰ����
���ߣ� hmliu
ʱ�䣺 20080904
���룺 unit:��оƬ�ţ�һ��̶�����0�����ǵ����ж��оƬͬʱӦ�ã����ݲ�����
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID SetSrcMac(NBB_INT unit)
{
    if (ApiGetGlobalMacAddr(unit, src_mac_overall))
    {
        NBB_PRINTF("SetSrcMac FAIL\n");
    }
}
#endif

NBB_VOID ExeMacTxRx(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/* Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_ctrl->need_exe_mac == 0)
    {
        return;
    }

    switch (p_ctrl->finished_state)
    {
        case FINISH_NORMAL:
            ExeMacRxTxMod(sub_cards, port, MUX_FWD, PAR_FWD);
            break;

        case FINISH_LB_REMOTE_EXE:
            ExeMacRxTxMod(sub_cards, port, MUX_DISCARD, PAR_LB);
            break;

        case FINISH_LB_LOCAL_CMD:
            ExeMacRxTxMod(sub_cards, port, MUX_FWD, PAR_DISCARD);
            break;

        default:
            break;
    }

    p_ctrl->need_exe_mac = 0;
}

NBB_VOID SendInfNoTlv(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_INF; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_INF_NO_TLV;
    p_tx->oam_pdu_x.data_size = 0;
    p_tx->org_pdu_tlv_x.tx_inf++;
    //NBB_PRINTF("SendInfNoTlv  %d\n",port);
}

NBB_VOID SendInfLocalTlv(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_INF; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_INF_LOCAL;
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->inf_tlv_local_x), INF_TLV_LEN);
    p_tx->oam_pdu_x.data_size = INF_TLV_LEN;
    p_tx->org_pdu_tlv_x.tx_inf++;
    //      NBB_PRINTF("SendInfLocalTlv  %d\n",port);
}

NBB_VOID SendInfLocalRemoteTlv(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                        *p;
    //NBB_PRINTF("SendInfLocalRemoteTlv  %d\n",port);
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_INF; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_INF_LOCAL_REMOTE;
    NBB_MEMCPY(p, &(p_tx->inf_tlv_local_x), INF_TLV_LEN);
    p += INF_TLV_LEN;
    NBB_MEMCPY(p, &(p_tx->inf_tlv_remote_x), INF_TLV_LEN);
    p += INF_TLV_LEN;
    NBB_MEMCPY(p, &(p_tx->org_inf_tlv_x), INF_ORG_TLV_LEN);
    p_tx->oam_pdu_x.data_size = INF_TLV_LEN + INF_TLV_LEN + INF_ORG_TLV_LEN;
    p_tx->org_pdu_tlv_x.tx_inf++;
}

NBB_VOID SendVarRes(NBB_BYTE sub_cards, NBB_BYTE port)  /*Ӧ��Զ�*/
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                        *p;
    NBB_BYTE                        i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_VAR_RES; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_RES;
    p_tx->oam_pdu_x.data_size = 0;

    for (i = 0; i < p_tx->var_num; i++)
    {
        if (p_tx->var_res_x[i].width & 0x80)
        {
            /*��������Է��Ĳ�ѯ���󼴷��ز�������*/
            NBB_MEMCPY(p, &(p_tx->var_res_x[i]), VAR_RES_TLV_LEN);
            /*��ʹ��ѯʧ�ܣ��ݷ�������7�����ݼ�VALUE��ֵ0����VAR_RES_TLV_FAIL��
            ���Ժ���������ѯ�����˴�Ӧ��ReactRxVarReq���*/
            p += VAR_RES_TLV_LEN;
            p_tx->oam_pdu_x.data_size += VAR_RES_TLV_LEN;
        }
        else
        {
            NBB_MEMCPY(p, &(p_tx->var_res_x[i]), VAR_RES_TLV_LEN);
            p += VAR_RES_TLV_LEN;
            p_tx->oam_pdu_x.data_size += VAR_RES_TLV_LEN;
        }
    }

    p_tx->org_pdu_tlv_x.tx_var_res++;
}

/*�̶������ORG�¼������¼�ֵ�Զ����£���������ֻ�ܷ������¼���*/
NBB_VOID SendEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                        *p;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_EVENT; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_EVENT;
    p_tx->oam_pdu_x.data_size = 0;
    p_ctrl->event_tx_sq++;
    NBB_MEMCPY(p, &(p_ctrl->event_tx_sq), 2);     /*���·����¼�SQ*/
    p += 2;
    p_tx->oam_pdu_x.data_size += 2;

    if (p_ctrl->sym_period_req)
    {
        /*��SYM�¼���������*/
        p_tx->err_sym_period_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_sym_period_tlv_x), EVENT_SYM_TLV_LEN);     /*װ����*/
        p += EVENT_SYM_TLV_LEN; /*ָ�������ƶ�λ*/
        p_tx->oam_pdu_x.data_size += EVENT_SYM_TLV_LEN; /*���ݳߴ����*/
        p_tx->err_sym_period_tlv_x.err = 0; /*�����������ǰ������*/
        p_ctrl->sym_period_req = 0; /*�����ʶ�����Ϊ�Ѵ���*/
        NBB_PRINTF("port sym_period_req %d \n", port);
    }

    if (p_ctrl->frm_err_req)
    {
        p_tx->err_frm_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_frm_tlv_x), EVENT_FRM_TLV_LEN);
        p += EVENT_FRM_TLV_LEN;
        p_tx->oam_pdu_x.data_size += EVENT_FRM_TLV_LEN;
        p_tx->err_frm_tlv_x.err = 0;
        p_ctrl->frm_err_req = 0;
        NBB_PRINTF("port frm_err_req %d \n", port);
    }

    if (p_ctrl->frm_period_req)
    {
        p_tx->err_frm_period_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_frm_period_tlv_x), EVENT_FRM_PRD_TLV_LEN);
        p += EVENT_FRM_PRD_TLV_LEN;
        p_tx->oam_pdu_x.data_size += EVENT_FRM_PRD_TLV_LEN;
        p_tx->err_frm_period_tlv_x.err = 0;
        p_ctrl->frm_period_req = 0;
        NBB_PRINTF("port frm_period_req %d \n", port);
    }

    if (p_ctrl->second_period_req)
    {
        p_tx->err_frm_second_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_frm_second_tlv_x), EVENT_FRM_SECOND_TLV_LEN);
        p += EVENT_FRM_SECOND_TLV_LEN;
        p_tx->oam_pdu_x.data_size += EVENT_FRM_SECOND_TLV_LEN;
        p_tx->err_frm_second_tlv_x.err = 0;
        p_ctrl->second_period_req = 0;
        NBB_PRINTF("port second_period_req %d \n", port);
    }

    NBB_MEMCPY(p, &(p_tx->org_event_tlv_x), EVENT_ORG_TLV_LEN);     /*�̶���ORG�¼�*/
    p += EVENT_ORG_TLV_LEN;
    p_tx->oam_pdu_x.data_size += EVENT_ORG_TLV_LEN;
    p_tx->org_pdu_tlv_x.tx_event++;
}

NBB_VOID SendLbCmd(NBB_BYTE sub_cards, NBB_BYTE port)  /*��Ӧ����*/
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->tx_pkt_type = TX_LB_ENB;
    p_tx->lb_cmd_x.cmd = LB_ENB;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_LOOPBACK; /*����CODE*/
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->lb_cmd_x), sizeof(lb_cmd));
    p_tx->oam_pdu_x.data_size = sizeof(lb_cmd);
    p_tx->org_pdu_tlv_x.tx_lb_cmd++;
    p_ctrl->lb_exe_fail = 0; /*������һ���µ� �������������ִ��ʧ�ܵĸ澯*/
    p_ctrl->var_lb_fail_ais = 0;            /* ����������Զ�Ӧ�������Ӷ�ԭ�澯��ʧ     */
    NBB_PRINTF("SendLbCmd port %d\n", port);
}
NBB_VOID SendDisLbCmd(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_LOOPBACK; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_LB_DISENB;
    p_tx->lb_cmd_x.cmd = LB_DISENB;
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->lb_cmd_x), sizeof(lb_cmd));
    p_tx->oam_pdu_x.data_size = sizeof(lb_cmd);
    p_tx->org_pdu_tlv_x.tx_dislb_cmd++;
    p_ctrl->lb_exe_fail = 0; /*������һ���µ� �������������ִ��ʧ�ܵĸ澯*/
    p_ctrl->var_lb_fail_ais = 0;    /* ����������Զ�Ӧ�������Ӷ�ԭ�澯��ʧ     */
    NBB_PRINTF("SendDisLbCmd port %d\n", port);
}

NBB_VOID SetVarReqFrmMgr(NBB_BYTE port)   /*��Ӧ��������,���÷�����������*/
{
}

NBB_VOID SendVarReq(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                                                                        *p;
    NBB_BYTE                                                                        i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_VAR_REQ; /*����CODE*/
    SetVarReqFrmMgr(port);
    p_ctrl->tx_pkt_type = TX_REQ;
    p_tx->oam_pdu_x.data_size = 0;

    for (i = 0; i < p_tx->var_num; i++)
    {
        NBB_MEMCPY(p, &(p_tx->var_req_x[i]), VAR_REQ_TLV_LEN);
        p += VAR_REQ_TLV_LEN;
        p_tx->oam_pdu_x.data_size += VAR_REQ_TLV_LEN;
    }

    p_tx->org_pdu_tlv_x.tx_var_req++;
    p_ctrl->var_lb_fail_ais = 0;            /* ����������Զ�Ӧ�������Ӷ�ԭ�澯��ʧ             */
    NBB_PRINTF("SendVarReq port %d\n", port);
}
NBB_VOID SendOrgPdu(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_ORG; /*����CODE*/
    p_ctrl->tx_pkt_type = TX_ORG_PDU;
    p_tx->org_pdu_tlv_x.tx_org_pdu++;
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->org_pdu_tlv_x), PDU_ORG_TLV_LEN);
    p_tx->oam_pdu_x.data_size = PDU_ORG_TLV_LEN;
//      NBB_PRINTF("port SendOrgPdu %d \n",port);
}

NBB_VOID OamCtrlParMux(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE mux_action, NBB_BYTE par_action)
/*��λ������Ϣ*/
{
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->inf_tlv_local_x.par_action = par_action;
    p_tx->inf_tlv_local_x.mux_action = mux_action;
    p_ctrl->need_exe_mac = 1;
    p_tx->inf_tlv_local_x.revision++;
    /*ÿ��ִ��STATE�ĸ��£��򱾶˷�����revision Ҳ���£��������� revision ֻ�ǻ�ȡֵ*/
    NBB_PRINTF("OamCtrlParMux port %d : par %d  mux %d \n", port, par_action,
               mux_action);
//      ExeMacRxTxMod(port,par_action,mux_action);/*101229  ԭ��ûִ�и����*/
    return;
}

/*ʵ�ʲ�������MAC���շ�ģʽ*/
NBB_VOID ExeMacRxTxMod(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE mux_action, NBB_BYTE par_action)
{
    /*�ն��������أ�����ʹ��*/
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    switch (par_action)
    {
        case PAR_LB:
            ExeMacRxLoopBack(port);
            break;

        case PAR_DISCARD:
            ExeMacRxDiscard(port);
            break;

        case PAR_FWD:
            ExeMacRxFwd(port);
            break;

        default:
            break;
    }

    switch (mux_action)
    {
        case MUX_DISCARD:
            ExeMacTxDiscard(port);
            break;

        case MUX_FWD:
            ExeMacTxFwd(port);
            break;

        default:
            break;
    }
}

NBB_VOID ReturnToLinkFault(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;

    //return;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*δ����OAM*/
    {
        return;
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    UpdateFlag(sub_cards, port);

    if (p_tx->oam_pdu_x.oam_header_x.link_fault || p_ctrl->local_lost_link_time_down)
    {
        //if(p_tx->oam_pdu_x.oam_header_x.link_fault/*||p_ctrl->local_lost_link_time_down*/){/*110410 ����״̬����*/
        //  NBB_PRINTF("p_tx->oam_pdu_x.oam_header_x.link_fault=%d\n",p_tx->oam_pdu_x.oam_header_x.link_fault);
        //  NBB_PRINTF("p_ctrl->local_lost_link_time_down=%d\n",p_ctrl->local_lost_link_time_down);
        p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
        p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_UNFINISHED;
        p_ctrl->oppsite_active = 0;
        p_ctrl->discovery_status = DISCOVER_LINK_FAULT;
        p_ctrl->local_pdu = PDU_LF_INFO;
        //NBB_PRINTF("ReturnToLinkFault...\n");
    }
}

NBB_VOID ReturnToLinkFaultDbg(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu            *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu            *p_tx;
    oam_control_status      *p_ctrl;

    /*return;*/

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*δ����OAM*/
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl; //110409
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;

    if (p_tx->oam_pdu_x.oam_header_x.link_fault || p_ctrl->local_lost_link_time_down)
    {
        p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
        p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_UNFINISHED;
        p_ctrl->oppsite_active = 0;
        p_ctrl->discovery_status = DISCOVER_LINK_FAULT;
        p_ctrl->local_pdu = PDU_LF_INFO;
        //  NBB_PRINTF("ReturnToLinkFault...\n");
    }
}


/***   110303   ����LINK-OAM״̬����ȡ�������Թ�Ӧ�ò�
���� ����Ӧ����������
*******************************************/

NBB_BYTE   LinkOamStatus_Get(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_tx;
    oam_control_status          *p_ctrl;

    if ((port + 1) > MAX_PORT_NUM)
    {
        //NBB_PRINTF("ERROR:port number is invalid!\n");
        return(0);
    }

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*δ����OAM*/
    {
        return(0);    /*110505 �ڲ�����ʱ����Ϊ����̬��link oamʧ��*/
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if ((p_ctrl->discovery_status == DISCOVER_ANY)
        && (p_rx->oam_pdu_x.oam_header_x.link_fault == 0))
        //if(p_ctrl->discovery_status==DISCOVER_ANY) 110409
    {
        return(0);
    }   /*link oam���������*/
    else
    {
        return(1);
    }
}



/*discovery״̬*/
/*ÿ���1�Σ��ͷ�ͬ���ڣ�*/
NBB_VOID DiscoveryState(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*δ����OAM*/
    {
        return;
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    switch (p_ctrl->discovery_status)
    {
        case DISCOVER_LINK_FAULT:
            p_ctrl->local_pdu = PDU_LF_INFO;

            if (p_tx->oam_pdu_x.oam_header_x.link_fault == 0)
            {
                if (p_tx->inf_tlv_local_x.oam_mode == OAM_ACTIVE)
                {
                    p_ctrl->discovery_status = DISCOVER_ACTIVE;
                    p_ctrl->local_pdu = PDU_INFO;
                }

                if (p_tx->inf_tlv_local_x.oam_mode == OAM_PASSIVE)
                {
                    p_ctrl->discovery_status = DISCOVER_WAIT;
                    p_ctrl->local_pdu = PDU_RX_INFO;
                }

                p_tx->oam_pdu_x.oam_header_x.local_discovering =
                    DISCOVER_UNFINISHED; /*Э�����ڽ���*/
            }

            break;

        case DISCOVER_WAIT:

            /*ReturnToLinkFault(port);*/
            if (p_ctrl->rx_pkt_type != NON_PDU_RX)
            {
                p_ctrl->discovery_status = DISCOVER_SEND;
                p_ctrl->local_pdu = PDU_INFO; /*PDU_RX_INFO*/
            }

            break;

        case DISCOVER_ACTIVE:

            /*ReturnToLinkFault(port);*/
            if (p_ctrl->rx_pkt_type != NON_PDU_RX)
            {
                p_ctrl->discovery_status = DISCOVER_SEND;
                p_ctrl->local_pdu = PDU_INFO;
            }

            break;

        case DISCOVER_SEND:
            ReturnToLinkFault(sub_cards, port);

            //if(VerifySatisfied(port)){ 110409
            if ((1 == VerifySatisfied(sub_cards, port))
                && (p_rx->oam_pdu_x.oam_header_x.link_fault == 0)
                && (p_ctrl->local_lost_link_time_down == 0))
            {
                p_ctrl->discovery_status = DISCOVER_ANY;
                p_ctrl->local_pdu = PDU_ANY;
            }

            break;

        case DISCOVER_ANY: /*���ڸ�λ�������Ӧ���������Զ���λ�����ø�Ϊ
OAM DISABLE��������ʱע��״̬��*/

            //if(VerifySatisfied(port)==0){ 110409
            if ((VerifySatisfied(sub_cards, port) == 0) || (p_rx->oam_pdu_x.oam_header_x.link_fault))
            {
                p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_UNFINISHED;//110409
                p_ctrl->discovery_status = DISCOVER_SEND;
                p_ctrl->local_pdu = PDU_INFO;
            }

            ReturnToLinkFault(sub_cards, port);
            break;

        default:
            break;
    }
}

NBB_VOID ResetOamPort(NBB_BYTE sub_cards, NBB_BYTE port)  /*��λ�ö˿�OAM��ֻ��������*/
{
    port_oam_pdu                    *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        //  NBB_PRINTF("This Port Disable Oam\n");
        return;
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    OS_MEMSET(p_tx, 0, sizeof(port_oam_pdu));     /*ȫ��������ֵ0*/
    OS_MEMSET(p_rx, 0, sizeof(port_oam_pdu));
    OS_MEMSET(p_ctrl, 0, sizeof(oam_control_status));
    /*Ϊ�ö˿ڷ������ݽṹ��ʼ��������DATA�ռ䣬�����������㼴�� */
    PortOamPduInitCfg(sub_cards, p_tx, port);
    PortOamCtrlInitCfg(p_ctrl, port);   /*��ʼ��oam_cfg_tx oam_pdu_cfg_tx*/
    p_ctrl->need_exe_mac = 1;
    //  NBB_PRINTF("p_tx->err_frm_period_tlv_x.err %d\n",p_tx->err_frm_period_tlv_x.err);
    //  NBB_PRINTF(" Now ResetOamPort PORT %d...........\n",port);
    //  ExeMacRxTxMod(port,PAR_FWD,MUX_FWD);/*101229��λ��ִ������ת��������MAC���ʼ��Ϊ�����շ�  * /
    p_ctrl->unfinished_time = 0; /*110516��λ��һ�и�ԭ*/
    p_ctrl->unfinished = FINISHED; /* �����ʱ����Ӧ�������*/
    NBB_PRINTF("p_ctrl->unfinished,Then Local OamCtrlParMux %d 0,0\n", port);
    p_ctrl->finished_state = FINISH_NORMAL;/* �ȴ�Ӧ�������ֻ�л���/��ѯ�������ܵ����������ȴ��Ƿ�Ӧ��*/
    OamCtrlParMux(sub_cards, port, MUX_FWD, PAR_FWD);  /*110513 100609*/
    return;     /*20080912  22:00 lhm*/
}

#if 0   /*2011.11.27 qjwu*/
NBB_VOID RxOamLostTimer(NBB_BYTE port)/*5�뿴�Ź�����һ�Σ���1
�뿴�Ź���ʱ����Ӧ�������ж˿�*/
{
    port_oam_pdu                    *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    //return;//1fcdsf 110111
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_ctrl->rx_pkt_cnt_in_5s == 0)
    {
        p_ctrl->local_lost_link_time_down = 1;
    }
    else
    {
        p_ctrl->local_lost_link_time_down = 0;
        p_ctrl->rx_pkt_cnt_in_5s = 0;
    }
}
#endif

NBB_VOID RxOamLostTimer(NBB_BYTE sub_cards, NBB_BYTE port)/*1�뿴�Ź�����һ�Σ���1
�뿴�Ź���ʱ����Ӧ�������ж˿�*/
{
    port_oam_pdu                *p_rx;  /*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (rx_pkt_cnt_in_5s_last[sub_cards][port] != p_ctrl->rx_pkt_cnt_in_5s)
    {
        rx_pkt_cnt_in_5s_timer[sub_cards][port] = 0;
    }
    else
    {
        p_ctrl->rx_pkt_cnt_in_5s = 0;

        if (rx_pkt_cnt_in_5s_timer[sub_cards][port] < 5)
        {
            rx_pkt_cnt_in_5s_timer[sub_cards][port]++;
        }
    }

    rx_pkt_cnt_in_5s_last[sub_cards][port] = p_ctrl->rx_pkt_cnt_in_5s;

    if (rx_pkt_cnt_in_5s_timer[sub_cards][port] >= 5)
    {
        p_ctrl->local_lost_link_time_down = 1;
    }
    else
    {
        p_ctrl->local_lost_link_time_down = 0;
    }
}

NBB_VOID ExeMacRxLoopBack(NBB_BYTE port)   /*��MAC��ִ�з�������*/
{
}
NBB_VOID ExeMacRxDiscard(NBB_BYTE port)   /*��MAC��ִ���հ�����*/
{
}
NBB_VOID ExeMacRxFwd(NBB_BYTE port)   /*��MAC��ִ���հ�����ת��*/
{
}

NBB_VOID ExeMacTxDiscard(NBB_BYTE port)   /*��MAC��ִ�з�����������ʹ�ܷ�*/
{
}

NBB_VOID ExeMacTxFwd(NBB_BYTE port)   /*��MAC��ִ�з�������ת��*/
{
}

NBB_VOID GetPmMacPhyVar(NBB_BYTE sub_cards, NBB_BYTE port)/*��ȡ����ͳ�������¼���������
����ӿ�*//*������ѯ�е���*/
{
    NBB_ULONG cnt_hi;
    NBB_ULONG cnt_low;
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_rx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    /*return; DEBUG*/
    cnt_hi = 0;
    //return;/*ֱ�ӵ����������ݣ����Դ˺�������Ҫ*/
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              T_OkPkts, &cnt_low, &cnt_hi) < 0)
        /*�����Ǽ�����ת*/
    {
        NBB_PRINTF("Get T_Pkts FAIL\n");
    }

    p_ctrl->local_var_cnt[TX_FRM] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_OkPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_Pkts FAIL\n");
    }

    p_ctrl->local_var_cnt[RX_FRM] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_CrcPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_CrcPkts FAIL\n");
    }

    p_ctrl->local_var_cnt[RX_CRC] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              T_Octets, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get T_Octets FAIL\n");
    }

    p_ctrl->local_var_cnt[TX_BYTE] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_Octets, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_Octets FAIL\n");
    }

    p_ctrl->local_var_cnt[RX_BYTE] = cnt_low;
}

#if 0
NBB_VOID GetPmMacPhySymErrDbg(NBB_BYTE port)/*��ȡ����ͳ�������¼���������  ����ӿ�
ÿ���һ��*/
{
    NBB_INT cnt_hi;
    NBB_INT cnt_low;
    port_oam_pdu                    *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_rx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    cnt_hi = 0;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->sym_err_1s = 0;
    cnt_low = 0;
    cnt_hi = 0; /* R_SymbolError  ����ʱ��ȡ�������� */

    if (ApiC3GetHWStatCounter(0, port, RxSymbErrFram, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxSymbErrFram FAIL\n");
    }
    else
    {
        if (p_ctrl->sym_err_last < cnt_low)
        {
            p_ctrl->sym_err_1s += (cnt_low - p_ctrl->sym_err_last);
            /*�Ĵ����ۼ�ֵ�����ˣ����ȡ����*/
        }

        p_ctrl->sym_err_last = cnt_low;
        /*�������ۼ�ֵ����Ϊ�ϴ�ֵ���Ա��´�Ӧ�ã����ϴ�ֵ���ڵ�ǰֵ��������Ĵ�����ת�������������ݲ����¼����ɽ���*/
    }

    if (p_ctrl->need_exe_mac)
    {
        p_ctrl->sym_err_1s = 0;
    }

    p_tx->err_sym_period_tlv_x.err += p_ctrl->sym_err_1s;
}

/*��ȡ����ͳ�������¼���������  ����ӿ� ÿ���һ��*/
NBB_VOID GetPmMacPhySymErr(NBB_BYTE port)
{
    NBB_ULONG            cnt_hi;
    NBB_ULONG            cnt_low;
    port_oam_pdu            *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu            *p_rx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    cnt_hi = 0;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->sym_err_1s = 0;
    cnt_low = 0;
    cnt_hi = 0; /* R_SymbolError  ����ʱ��ȡ�������� */

    if (ApiC3GetHWStatCounter(0, port, RxSymbErrFram, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxSymbErrFram FAIL\n");
    }
    else
    {
        if (p_ctrl->sym_err_last < cnt_low)
        {
            p_ctrl->sym_err_1s += (cnt_low - p_ctrl->sym_err_last);
            /*�Ĵ����ۼ�ֵ�����ˣ����ȡ����*/
        }

        p_ctrl->sym_err_last = cnt_low;
        /*�������ۼ�ֵ����Ϊ�ϴ�ֵ���Ա��´�Ӧ�ã����ϴ�ֵ���ڵ�ǰֵ��
        ������Ĵ�����ת�������������ݲ����¼����ɽ���*/
    }

    if (p_ctrl->need_exe_mac)
    {
        p_ctrl->sym_err_1s = 0;
    }

    p_tx->err_sym_period_tlv_x.err += p_ctrl->sym_err_1s;
}
#endif
NBB_VOID GetPmMacPhyFrmErr(NBB_BYTE sub_cards, NBB_BYTE port)/*��ȡ����ͳ�������¼���������  ����ӿ�
ÿ���һ��*/
{
    NBB_ULONG            cnt_hi;
    NBB_ULONG            cnt_low;
    NBB_ULONG            cnt_low_tmp;
    port_oam_pdu            *p_tx;/*Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu            *p_rx;/*Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    cnt_hi = 0;
    cnt_low_tmp = 0;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->frm_err_1s = 0;
    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_UndersizePkts, &cnt_low, &cnt_hi) < 0)
        /*������תʱ�ôεķ���������Ч����ת��һ�м���*/
    {
        NBB_PRINTF("Get R_UndersizePkts FAIL\n");
    }
    else
    {
        if (p_ctrl->undersize_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->undersize_last);
        }

        p_ctrl->undersize_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_FragmentPkts, &cnt_low, &cnt_hi) < 0)
        /*������תʱ�ôεķ���������Ч����ת��һ�м���*/
    {
        NBB_PRINTF("Get R_FragmentPkts FAIL\n");
    }
    else
    {
        if (p_ctrl->fragment_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->fragment_last);
        }

        p_ctrl->fragment_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_OversizePkts, &cnt_low, &cnt_hi) < 0)
        /*������תʱ�ôεķ���������Ч����ת��һ�м���*/
    {
        NBB_PRINTF("Get R_OversizePkts FAIL\n");
    }
    else
    {
        if (p_ctrl->oversize_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->oversize_last);
        }

        p_ctrl->oversize_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_JabberPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxJabberFram FAIL\n");
    }
    else
    {
        if (p_ctrl->jabber_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->jabber_last);
        }

        p_ctrl->jabber_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_CrcPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_CrcPkts FAIL\n");
    }
    else
    {
        if (p_ctrl->crc_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->crc_last);
        }

        p_ctrl->crc_last = cnt_low;
    }

    cnt_low = 0;
#if 0

    if (ApiC3GetHWStatCounter(0, port, R_AlignmentPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxShortErrFram FAIL\n");
    }
    else
    {
        if (p_ctrl->alignment_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->alignment_last);
        }

        p_ctrl->alignment_last = cnt_low;
    }

#endif

    if (p_ctrl->need_exe_mac)
    {
        p_ctrl->frm_err_1s = 0;
    }

    p_tx->err_frm_tlv_x.err += p_ctrl->frm_err_1s;
    p_tx->err_frm_period_tlv_x.err += p_ctrl->frm_err_1s;

    if (p_ctrl->frm_err_1s)
    {
        NBB_PRINTF("port %d :frm_err_1s=%d\n", port, p_ctrl->frm_err_1s);
        p_tx->err_frm_second_tlv_x.err++;
    }
}


NBB_BYTE GetAisMacPhy(NBB_BYTE sub_card, NBB_BYTE port)/*/��ȡ�澯����FLAG�¼������°�ͷ�� flags
����ӿ�*/
{
    int status;
    status = 0x33;
    /*if(ApiGetPhyLink(0,g_efm_oam_port_cfg[sub_cards].logic_port[port],&status))
    {
        NBB_PRINTF("FAIL to GET  LINK STATUS port: %d,  rv = %d\n", port, rv);
        return 0;
    }
    else
    {
        if(status)
            return(0);
        else
            return(1);
    }*//*2011.10.18 qjwu ApiGetPhyLink()ʹ���˻����ź���*/

    if (phy_link_state[sub_card][port])
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
NBB_VOID UpdateFlag(NBB_BYTE sub_cards, NBB_BYTE port)/* ̽���Ƿ�����·�˿��¹�,1���1�Σ���
DiscoveryState ֮ǰ����*/
{
    port_oam_pdu                    *p_tx;/*/Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status              *p_ctrl;
    port_oam_pdu                    *p_rx;//110409
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;

    //if(p_ctrl->enb_link_fault_send||(p_rx->oam_pdu_x.oam_header_x.link_fault))/*110409 ʵ�ֵ����յ��Զ�LINK-FAULTʱ���ر�LINK-FAULT*/

    if (p_ctrl->enb_link_fault_send)
    {
        p_tx->oam_pdu_x.oam_header_x.link_fault = 1;
    }
    else
    {
        p_tx->oam_pdu_x.oam_header_x.link_fault = GetAisMacPhy(sub_cards, port);
    }

    if (p_ctrl->local_link_status_last == p_tx->oam_pdu_x.oam_header_x.link_fault)
    {
        return;
    }

    p_ctrl->local_link_status_last = p_tx->oam_pdu_x.oam_header_x.link_fault;
    /* ��Ȼ revision ��INFO������ݣ�����LINK����ʱ�������Ҳ�Ǻ����*/
    p_tx->inf_tlv_local_x.revision++;
}

NBB_VOID UpdateOamCfgPduCfg(NBB_BYTE sub_cards, NBB_BYTE port)  /*ÿ��һ�ε������þ�ִ��һ�Σ��Ը������ݣ������Ƿ��и���*/
{
    port_oam_pdu            *p_tx;/* Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;/* ��ֵ oam_cfg_tx  oam_pdu_cfg_tx������Ӧ����״̬��������*/
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->inf_tlv_local_x.revision++;
}

NBB_VOID DecideSymEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* ÿ���1�Σ������ܲ�������������Ӧ*/
{
    port_oam_pdu            *p_tx;
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_sym_period_tlv_x.err > p_tx->err_sym_period_tlv_x.threshold)
    {
        /* Ŀǰֻȡ��4�ֽ�*/
        p_ctrl->sym_period_req = 1;
        p_ctrl->tx_sym_err = 1;
        p_ctrl->tx_sym_err_cnt = p_tx->err_sym_period_tlv_x.err;
        p_tx->err_sym_period_tlv_x.err_ttl += p_tx->err_sym_period_tlv_x.err;
        /*ֻ��Խ��Ĵ���Ż��ۻ���������*/
        p_tx->err_sym_period_tlv_x.event_ttl++;

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_sym_period_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_sym_period_tlv_x.err = 0;
    }

    /*���ܱ��δ����Ƿ�Ҫ��¼����ֵ�У�Ӧ����Ϊ�´��о���׼��*/
}

NBB_VOID DecideFrmEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* ÿ���1�Σ������ܲ�������������Ӧ*/
{
    port_oam_pdu            *p_tx;
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_frm_tlv_x.err > p_tx->err_frm_tlv_x.threshold)  /* Ŀǰֻȡ��4�ֽ�*/
    {
        p_ctrl->frm_err_req = 1;
        p_ctrl->tx_frm_err = 1;
        p_ctrl->tx_frm_err_cnt = p_tx->err_frm_tlv_x.err;
        p_tx->err_frm_tlv_x.err_ttl += p_tx->err_frm_tlv_x.err;
        p_tx->err_frm_tlv_x.event_ttl++;    /* ֻ��ֱ�Ӹ��·�����*/

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_frm_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_frm_tlv_x.err = 0;
    }
}

NBB_VOID DecideFrmPeriodEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* ÿ���1�Σ������ܲ�������������Ӧ*/
{
    port_oam_pdu            *p_tx;/* Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_frm_period_tlv_x.err > p_tx->err_frm_period_tlv_x.threshold)
        /* Ŀǰֻȡ��4�ֽ�*/
    {
        p_ctrl->frm_period_req = 1;
        p_ctrl->tx_frm_prd_err = 1;
        p_ctrl->tx_frm_prd_err_cnt = p_tx->err_frm_period_tlv_x.err;
        p_tx->err_frm_period_tlv_x.err_ttl += p_tx->err_frm_period_tlv_x.err;
        p_tx->err_frm_period_tlv_x.event_ttl++;

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_frm_period_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_frm_period_tlv_x.err = 0;
    }
}

NBB_VOID DecideFrmSecondEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* ÿ60���1�Σ������ܲ�������������Ӧ*/
{
    port_oam_pdu            *p_tx;/* Ϊ�ɶ��Ժ���ʱ��*/
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_frm_second_tlv_x.err > p_tx->err_frm_second_tlv_x.threshold)
        /* Ŀǰֻȡ��4�ֽ�*/
    {
        p_ctrl->second_period_req = 1;
        p_ctrl->tx_frm_snd_err = 1;
        p_ctrl->tx_frm_snd_err_cnt = p_tx->err_frm_second_tlv_x.err;
        p_tx->err_frm_second_tlv_x.err_ttl += p_tx->err_frm_second_tlv_x.err;
        p_tx->err_frm_second_tlv_x.event_ttl++;

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_frm_second_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_frm_second_tlv_x.err = 0;
    }
}

NBB_BYTE VerifySatisfied(NBB_BYTE sub_cards, NBB_BYTE port) /*���ô˺��������յ��Զ˰�����˲�����Э�̲��������̬*/
{
    /*  �Ͼ� oam_cfg_tx/oam_cfg_rx  oam_pdu_cfg_tx/oam_pdu_cfg_rx �Ƿ�һ��
    �Ƿ�Ϊ PASSIVE*/
    /* ���� ������ oam_flag  ������״̬��*/
    port_oam_pdu                    *p_rx;  /* Ϊ�ɶ��Ժ���ʱ��*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    /*if(p_tx->inf_tlv_local_x.var_response_spt==0) goto LinkFail;
    if(p_tx->inf_tlv_local_x.link_event_spt==0) goto LinkFail;100609*/
    /*if(p_tx->inf_tlv_local_x.loopback_spt==0) goto LinkFail;
    *//*���ز��������룬��������Թػ���Ӧ��*/
    //if(p_tx->inf_tlv_local_x.unidirection_spt==0) goto LinkFail;
    /*if(p_rx->inf_tlv_local_x.var_response_spt==0) goto LinkFail;
    if(p_rx->inf_tlv_local_x.link_event_spt==0) goto LinkFail;100609*/
    /*if(p_rx->inf_tlv_local_x.loopback_spt==0) goto LinkFail;*/
    /*if(p_rx->inf_tlv_local_x.unidirection_spt==0) goto LinkFail;100609*/
    p_ctrl->max_size_recept = (p_tx->inf_tlv_local_x.max_size < p_rx->inf_tlv_local_x.max_size)
                              ?  p_tx->inf_tlv_local_x.max_size : p_rx->inf_tlv_local_x.max_size;
LinkSuccess:
    p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_FINISHED;
    p_tx->oam_pdu_x.oam_header_x.local_stable = STABLE_SATISFIED;
    return(1);
LinkFail:/* ֻ��Local Discovering/Local Stable ��BIT3/BIT4����Զ�˵��ڽ���ʱ�Ѵ���*/
    p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_FINISHED;
    p_tx->oam_pdu_x.oam_header_x.local_stable = STABLE_FAIL;
    return(0);
}

#if 0
/*****************************************************************************
���ܣ� ��ÿ���������ж˿�ǰ��ʼ��ĳЩ���ݣ�Ӧ�����ö˿�ǰ����
���ߣ� hmliu
ʱ�䣺 20080904
���룺 ��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID InitBeforeCfgPort(NBB_VOID)
{
    SetSrcMac(0);
}
#endif

/*****************************************************************************
���ܣ� ����ĳ�˿��Ƿ�ʹ��OAM
����ʹ���򿪱ٿռ䲢��ʼ����������ʹ����ע���ռ䣻Ӧ�� CONFIG�е��ã�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�enb��ʹ��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamEnb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    g_efm_oam_port_cfg[sub_cards].port_oam_x[port].enb = enb;

    if (enb)
    {
    }
    else
    {
    }
}

/*****************************************************************************
���ܣ� ����OAMģʽ��Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�enb���Ƿ�����
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamMode(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p->inf_tlv_local_x.oam_mode = enb;
        p_ctrl->discovery_status = DISCOVER_LINK_FAULT;
        p_ctrl->local_pdu = PDU_LF_INFO;
        p_ctrl->rx_pkt_type = NON_PDU_RX;
        DiscoveryState(sub_cards, port);
        //  printf("p_ctrl->discovery_status=%d\n",p_ctrl->discovery_status);
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[port] = enb;
}

/*****************************************************************************
���ܣ� ����OAM�Ƿ�֧�ֱ�����ѯ��Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�enb���Ƿ�֧��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamSupportVar(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.var_response_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_var_cfg[port] = enb;
}


/*****************************************************************************
���ܣ� ����OAM�Ƿ�֧���¼�֡�շ���Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�enb���Ƿ�֧��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamSupportEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.link_event_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_event_cfg[port] = enb;
}

/*****************************************************************************
���ܣ� ����OAM�Ƿ�֧�ֻ��أ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�enb���Ƿ�֧��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamSupportLb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.loopback_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_lb_cfg[port] = enb;
}

/*****************************************************************************
���ܣ� ����OAM�Ƿ�֧�ֵ���Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�enb���Ƿ�֧��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamSupportUnidirection(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.unidirection_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_uni_cfg[port] = enb;

    if (enb)
    {
        ApiC3SetPortAutoNego(0, port, 0);
        taskDelay(10);

        if (GetAisMacPhy(sub_cards, port))   /*���ײ�֧�ַ���Э�̼���֧�ֵ���*/
        {
            ApiC3SetPortAutoNego(0, port, 1);
        }
    }
    else
    {
        ApiC3SetPortAutoNego(0, port, 1);
    }
}

/*****************************************************************************
���ܣ� ����OAM���֡����Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�max:���֡��
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamMaxSize(NBB_BYTE sub_cards, NBB_BYTE port, unsigned short max)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.max_size = max;
        DiscoveryState(sub_cards, port);
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_maxsize_cfg[port] = max;
}

/*****************************************************************************
���ܣ� ����OAMӦ��ʱ���ޣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�max:��������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamOvertimeThreshold(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE max)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p_ctrl->overtime_threshold = max;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_overtime_cfg[port] = max;

    if (OvertimeThreshold < max)
    {
        OvertimeThreshold = max;
    }
}

/*****************************************************************************
���ܣ� ����OAM��������¼����ڣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�prd������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamSymPrd(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_sym_period_tlv_x.window = 0x1DCD6500 * prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sym_period_cfg[port] = prd;
}

/*****************************************************************************
���ܣ� ����OAM��������¼����ޣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�thr������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamSymThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_sym_period_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sym_threshold_cfg[port] = thr;
}


/*****************************************************************************
���ܣ� ����OAM����֡�¼����ڣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�prd������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamFrmPrd(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_tlv_x.window = prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_period_cfg[port] = prd;
}

/*****************************************************************************
���ܣ� ����OAM����֡�¼����ޣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�thr������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamFrmThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_threshold_cfg[port] = thr;
}

/*****************************************************************************
���ܣ� ����OAM����֡�����¼����ڣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�prd������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamFrmPrdPrd(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_period_tlv_x.window = 1488000 * prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_period_cfg[port] = prd;
}

/*****************************************************************************
���ܣ� ����OAM����֡�����¼����ޣ�Ӧ�� CONFIG�н����
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�thr������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamFrmPrdThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_period_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_threshold_cfg[port] = thr;
}

/****************************************************************************
���ܣ� ����OAM����֡���¼����ڣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�prd������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamFrmSndPrd(NBB_BYTE sub_cards, NBB_BYTE port, unsigned short prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_second_tlv_x.window = prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_period_cfg[port] = prd;
}

/*****************************************************************************
���ܣ� ����OAM����֡���¼����ޣ�Ӧ�� CONFIG�е���
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�thr������
����� ��
˵����
�޸ģ�
******************************************************************************/

NBB_VOID CfgPortOamFrmSndThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_second_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_threshold_cfg[port] = thr;
}

/*****************************************************************************
���ܣ� ִ���Ƿ��迪����ע��OAM�շ�������ÿ�������������ж˿ں���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 ��
����� ��
˵���� �û�Ӧע�������������ж˿ں�Ӧ���ô˺�����
�޸ģ�
******************************************************************************/
NBB_VOID BeginOamTxRxTask(NBB_VOID)
{
    AllPortOamCfgEnb();
}


/*****************************************************************************
���ܣ� ִ��״̬���Ӳ�ѯ�����û��ӿڣ���ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ָ��CHAR �����ָ��,������ NULLָ�룬������ֵ������Ϊ0
˵���� �û����ô�ָ��ֱ�ӿ����ݵ�Ӧ����������ķ��Ͷ��У�����������������Э�鶨
             �û����ò�ͬ��BCT
Э������ʱ����ĵײ���룬���������Ϊ�˼�࣬�������û����µ�BCTЭ�鲢������
�޸ģ�
******************************************************************************/
NBB_BYTE *QueryPortOam(NBB_BYTE sub_cards, NBB_BYTE port)
{
    return(QueryPortOamApi(sub_cards, port));
}

/*****************************************************************************
���ܣ� ��Ӧ������Ŵ��¼����ʹ�ñ��˷����¼�֡����ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/

NBB_VOID CmdSymErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_SYM;
}

/*****************************************************************************
���ܣ� ��Ӧ����֡���¼����ʹ�ñ��˷����¼�֡����ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/

NBB_VOID CmdFrmErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_FRM;
}

/*****************************************************************************
���ܣ� ��Ӧ����֡���ڴ��¼����ʹ�ñ��˷����¼�֡����ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/

NBB_VOID CmdFrmPrdErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_FRM_PERIOD;
}

/*****************************************************************************
���ܣ� ��Ӧ����֡����¼����ʹ�ñ��˷����¼�֡����ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/

NBB_VOID CmdFrmSndErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_FRM_SECOND;
}


/*****************************************************************************
���ܣ� ִ��OAM��λ����ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ��
˵���� ��λ���OAM�˿ڱ�������ܺ�״̬����ʼ��
�޸ģ�
******************************************************************************/
NBB_VOID CmdResetOam(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_RST_OAM;

    if (p_ctrl->local_pdu != PDU_ANY)
    {
        ResetOamPort(sub_cards, port);
        p_ctrl->rx_mgr_cmd = 0;
    }
}

/*****************************************************************************
���ܣ� ִ��OAM���أ���ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�cmd����/��
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/
NBB_VOID CmdSetOamLb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE cmd)
{
    oam_control_status      *p_ctrl;
    port_oam_pdu            *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;

    if (p->inf_tlv_local_x.loopback_spt == 0)
    {
        return;
    }

    if (p->inf_tlv_local_x.oam_mode == OAM_PASSIVE)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (cmd)
    {
        p_ctrl->rx_mgr_cmd = MGR_CMD_LB;
    }
    else
    {
        p_ctrl->rx_mgr_cmd = MGR_CMD_DIS_LB;
    }
}

/*****************************************************************************
���ܣ� ִ��MIB������ѯ����ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿ڣ�branch��֦ leaf��Ҷ
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/
NBB_VOID CmdQueryMibVar(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE branch, NBB_BYTE leaf)
{
    oam_control_status      *p_ctrl;
    port_oam_pdu            *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p->var_req_x[0].branch = branch;
    p->var_req_x[0].leaf = leaf;
    p->var_num = 1;
    p_ctrl->rx_mgr_cmd = MGR_CMD_REQ;
}

/*****************************************************************************
���ܣ� ִ�з����Զ����ѯ�˿�OAM�շ�ͳ�������ÿ���²�ѯ����ʱ���á�
���ߣ� hmliu
ʱ�䣺 20080904
���룺 port���˿�
����� ��
˵���� �˺����������ز���������Ч����״̬���Ӳ�ѯ��ſɿ���
�޸ģ�
******************************************************************************/
NBB_VOID CmdOrgSelfDefine(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_ORG;
}

NBB_VOID PrintfInfo(NBB_BYTE enb)
{
    print_info = enb;
}

NBB_VOID CmdSetLinkFault(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->enb_link_fault_send = enb;
}

NBB_VOID GetMacPmMibVar(NBB_BYTE sub_cards, NBB_BYTE port, int tx_frm, int rx_frm, int rx_crc, int tx_byte,
                        int rx_byte)
{
    oam_control_status      *p_ctrl;/* Ϊ�ɶ��Ժ���ʱ�� */
    port_oam_pdu            *p_tx;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->local_var_cnt[TX_FRM] = tx_frm;
    p_ctrl->local_var_cnt[RX_FRM] = rx_frm;
    p_ctrl->local_var_cnt[RX_CRC] = rx_crc;
    p_ctrl->local_var_cnt[TX_BYTE] = tx_byte;
    p_ctrl->local_var_cnt[RX_BYTE] = rx_byte;
}
#endif
#else
NBB_VOID spm_set_efm_oam(EFM_PHY_PORT *p_efm_phy_port, ATG_DCI_PHY_PORT_EFM_OAM_DATA *p_efm_oam)
{
    return;
}
#endif

#endif
