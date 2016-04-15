/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_bfd_task.c
  �� �� ��   : ����
  ��    ��   : limingming
  ��������   : 2012��9��24��
  ����޸�   :
  ��������   : BFD��ѯ����ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��24��
    ��    ��   : limingming
    �޸�����   : �����ļ�

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include "r870_lib.h"
#include <mtl.h>

struct dma_read_unit g_stDmaReadData;

/* �����ñ�� */
extern NBB_BYTE g_ucCleanCfgCheck;

/* ����fpgaת����������д�� */
extern BMU_RWLOCK_ID g_build_fpga_fwd_lock;

/*****************************************************************************
 �� �� ��  : spm_bfd_fsm_task
 ��������  : ÿ5ms��ѯһ�α������лỰ��Ӧ�Ĵ�����Ϣ�����ĻỰ״̬
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��13��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_fsm_task(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usLoopj = 0;
    NBB_USHORT usData  = 0; 
    NBB_BYTE   ucSessionSta = 0;
    NBB_BOOL bIsTimeExpired = FALSE;
    NBB_ULONG ulRemoteDisc = 0;
    NBB_USHORT  usRemoteDisctmph = 0;
    NBB_USHORT  usRemoteDisctmpl = 0;
    
    NBB_ULONG  ulRemoteYid = 0;
    NBB_USHORT  usRemoteYidtmph = 0;
    NBB_USHORT  usRemoteYidtmpl = 0;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    NBB_INT                  iRet = ERROR;
    NBB_BYTE    ucLocalSlot   = 0;          /* ���̲�λ�� */

//    struct timeval tv_start;
//    struct timeval tv_end;

    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();

//    gettimeofday(&tv_start, NULL);

    /* ͨ��DMA��ʽ��ȡbfd�� */
    spm_bfd_damread();

    /* ��ȡ�������ڲ�λ�� */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    
    /* ȡ���ջỰ״̬�Ĵ����е����� */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        /* ��ɾ����Ҫɾ����BFD */
        if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* ����Ҫɾ�� */
        {
            if (TRUE == g_stBfdlocal[usLoopi].bIsChangeToDown)
            {
                if (g_ulStateToDownNum > 0) /* ����Ϊ0 */
                {
                    g_ulStateToDownNum--;
                }
            }
            
            /* ���FPGA��Ӧ�ķ��ͼĴ������� */
            spm_bfd_clear_sendfpgareg(usLoopi);

            /*���FPGA BFD�����Ĵ�������*/
            spm_bfd_clear_recv_fpgareg(usLoopi);
            
            /* ���Ự��Ϣ��Ϊ��Чֵ */
            OS_MEMSET(&g_stBfdlocal[usLoopi], 0, sizeof(SPM_BFD_CB));
            g_stBfdlocal[usLoopi].bIsSendtoMaster = FALSE;  /* �����Ѿ���0�ˣ�Ϊʲô��Ҫ��0����Ϊ�˷�ֹFALSE���岻Ϊ0? */
            g_stBfdlocal[usLoopi].ucbIsNeedDelete = FALSE;  /* �����Ѿ���0�ˣ�Ϊʲô��Ҫ��0����Ϊ�˷�ֹFALSE���岻Ϊ0? */
            g_stBfdlocal[usLoopi].ulDiscrimer = DRV_ULONG_INVALID;

            /* �Ự״̬�����仯����Ҫͬ�� */
            g_bIsModifySession = TRUE;
            
        }
        
        /* ѧϰ�Զ������� */
        if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
        {
            /* FPGA����Ŀ��IP��Ӧ��BFD�����ˣ����ﲻ��Ҫ�����ĸ�BFD�Ƕ�Ӧ�յ��İ��� */
            if (g_stBfdlocal[usLoopi].ulRemoteDiscr == 0)
            {
                /* ��ȡ�յ��İ����my discriminator */
                usLoopj = usLoopi * 2;
                
                usRemoteDisctmpl = (NBB_USHORT)g_auiDmaDrimData[usLoopj];
                usRemoteDisctmph = (NBB_USHORT)g_auiDmaDrimData[usLoopj + 1];
                ulRemoteDisc = usRemoteDisctmph;    /* ��FPGA�����YID�ĸߵ�word, ƴ��4�ֽڵ�YID */
                ulRemoteDisc = (ulRemoteDisc << 16) + usRemoteDisctmpl;

                if (0 != ulRemoteDisc)/* ��������Ч */
                {
                    /* log�����������¼�������if���棬�ᵼ�����õ�BFDÿ5ms��¼һ�� */
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "the %u bfd study remote from fpga bfd, my_sid = %u, local ip = %lx.%lx.%lx.%lx, remote ip = %lx.%lx.%lx.%lx, "
                        "your_discriminator %ld\r\n",
                        usLoopi, g_stBfdlocal[usLoopi].pstBasicBfdCB.my_sid,
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.local_ip[0],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.local_ip[1],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.local_ip[2],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.local_ip[3],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.remote_ip[0],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.remote_ip[1],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.remote_ip[2],
                        g_stBfdlocal[usLoopi].pstBasicBfdCB.remote_ip[3],
                        ulRemoteDisc);

                    /* ���FPGAѧϰ����YID */
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);

                    g_stBfdlocal[usLoopi].ulRemoteDiscr = ulRemoteDisc;
                    
                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr)&DRV_USHORT_INVALID);
                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                }
            }
            else /* ����������Ϊ�㣬�����ж��������Ƿ��ϻ���Ҫ����ѧϰ */
            {
                if ((BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState) || (BFD_INIT == g_stBfdlocal[usLoopi].ucSessionState))
                {
                    if (TRUE == g_stBfdlocal[usLoopi].pstBasicBfdCB.peer_sid_selflearn_enable)
                    {
                        /* ��FPGA��ȡFPGAѧ���������� */
                        usLoopj = usLoopi * 2;

                        usRemoteDisctmpl = (NBB_USHORT)g_auiDmaDrimData[usLoopj];
                        usRemoteDisctmph = (NBB_USHORT)g_auiDmaDrimData[usLoopj + 1];
                        ulRemoteDisc = usRemoteDisctmph;
                        ulRemoteDisc = (ulRemoteDisc << 16) + usRemoteDisctmpl;

                        if (0 != ulRemoteDisc) /* ��������Ч */
                        {
                            /* ���ؼ�¼����������FPGAѧϰ���Ĳ�һ�£�����ѧϰ */
                            if (g_stBfdlocal[usLoopi].ulRemoteDiscr != ulRemoteDisc)
                            {
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "the %u bfd study remote dis again,"
                                    "my_sid = %u, your_discriminator %ld\r\n",
                                    usLoopi, g_stBfdlocal[usLoopi].pstBasicBfdCB.my_sid, ulRemoteDisc);

                                /* ���FPGAѧϰ����YID */
                                spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                                spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);

                                g_stBfdlocal[usLoopi].ulRemoteDiscr = ulRemoteDisc;

                                spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0,
                                    (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                                spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1,
                                    (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                            }
                        }
                    }
                }
            }
        }

        if (FALSE == g_BIsCloseStateMachine[usLoopi])/* �����ã�������״̬�� */
        {
            /* �Ự���� */
            if ((BFD_NORMAL == g_stBfdlocal[usLoopi].usBfdflag) && (BFD_ADMINDOWN != g_stBfdlocal[usLoopi].ucSessionState))
            {
                usData = (NBB_USHORT)g_auiDmaData[usLoopi];

                bIsTimeExpired = ((usData >> 15) & 0x1);    /* ��ĿN��BFD�澯״̬ */
                
                /* �ж��Ƿ�ʱ */
                if (TRUE == bIsTimeExpired)
                {
                    g_stBfdlocal[usLoopi].ucLocalDiag = bfd_diag_ctrl_detect_exp;
                    
                    if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
                    {
                        /* ldp lsp �����˳�ʱdown֮��1�����������ϻ���ѧϰ����YID����֤down�����Ҳ����öԶ�ѧ��YID */
                        if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdLsp)
                        {
                            if (TRUE == g_stBfdlocal[usLoopi].pstBasicBfdCB.peer_sid_selflearn_enable)
                            {
                                if (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState)
                                {
                                    g_stBfdlocal[usLoopi].usTimerStayDown = BFD_STAYDOWN_LDP_UP;
                                }
                                else if(BFD_INIT == g_stBfdlocal[usLoopi].ucSessionState)
                                {
                                    g_stBfdlocal[usLoopi].usTimerStayDown = BFD_STAYDOWN_LDP_INIT;
                                }
                            }
                        }
                    }
                    
                    spm_bfd_state_fsm_down(usLoopi,BFD_STA_BUTT);    /* ��ʱֱ����down����ʹ��spm_bfd_state_fsm */
                }
                else/* �����յ��Զ˵ı��ĵ�stat */
                {                
                    /* �ж��Ƿ������յ��ı��ĵ�stat */                
                    if (g_aucCouterRecord[usLoopi] != (NBB_BYTE)(usData & 0x3)) /* 2bit�ļ��� */
                    {
                        ucSessionSta = (NBB_BYTE)((usData >> 7) & 0x3); /* ��ĿN�Ľ���STATEֵ */
                        spm_bfd_state_fsm(usLoopi, ucSessionSta);
                        g_aucCouterRecord[usLoopi] = (NBB_BYTE)(usData & 0x3);
                    }
                }

                /* add begin by limingming, 2013/4/11, Mantis��:HSDxxxxxx, ԭ��:IDͬ�� */
                /* ��һ��up����¼�����������ϱ�upʱ�� */
                if ((0 != g_stBfdlocal[usLoopi].usTimerLiveUp)
                    && (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState))
                {
                    /* ���µ�һ��up��ļ����� */
                    g_stBfdlocal[usLoopi].usTimerLiveUp--;  /* ��״̬����UP֮��ֱ�����g_stBfdlocal��ͬʱ����ոü����� */

                    /* up�ȶ��涨ʱ����ϱ�up�¼� */
                    if (0 == g_stBfdlocal[usLoopi].usTimerLiveUp)
                    {
                        if (TRUE == g_stintervalinfo[usLoopi].bflag)
                        {
                            g_stintervalinfo[usLoopi].bflag = FALSE; 
                        }
                        
                        /* �ϱ�up�¼� */
                        spm_bfd_alamup_task(usLoopi);

                    }
                }
                /* add end by limingming, 2013/4/11 */

                if((0 != g_stBfdlocal[usLoopi].usTimerLiveDown)
                    && (BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState))
                {
                    g_stBfdlocal[usLoopi].usTimerLiveDown--;
                    
                    if (0 == g_stBfdlocal[usLoopi].usTimerLiveDown) /* down��ʱ���� */
                    {
                        g_stBfdlocal[usLoopi].bIsChangeToDown = TRUE;
                        g_ulStateToDownNum++;
                        
                        //spm_bfd_event(usLoopi, TRUE);
                    }
                }
            }
        }
        else    /* �����ã�ǿ�Ʋ���״̬�� */
        {
            /* �Ự���� */
            if ((BFD_NORMAL == g_stBfdlocal[usLoopi].usBfdflag) && (BFD_ADMINDOWN != g_stBfdlocal[usLoopi].ucSessionState))
            {
                usData = (NBB_USHORT)g_auiDmaData[usLoopi];

                bIsTimeExpired = ((usData >> 15) & 0x1);    /* ��ĿN��BFD�澯״̬ */
                
                /* �ж��Ƿ�ʱ */
                if (TRUE == bIsTimeExpired)
                {
                    /* �ж��Ƿ������յ��ı��ĵ�stat */                
                    if (g_aucCouterRecord[usLoopi] != (NBB_BYTE)(usData & 0x3)) /* 2bit�ļ��� */
                    {                    
                        NBB_PRINTF("MID=%u down,control detection time expired!\r\n", g_stBfdlocal[usLoopi].ulDiscrimer);
                
                        g_aucCouterRecord[usLoopi] = (NBB_BYTE)(usData & 0x3);
                    }
                }

                /* ����״̬�� */
            }
        }

        /* �Ự��up״̬ʱ���Ự�������޸ģ�һ�����޸����õ��£� */
        if ((0 != g_stBfdlocal[usLoopi].ucParaChangeFlag) && (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState))
        {
            /* �˴������λͼ�ķ�ʽ��¼��ʹ��if else��ʽ��ֻ��Ŀǰʹ�õı�ǽ��٣�����û������ */
            if (BFD_PARA_INTERVAL_CHANGE == (BFD_PARA_INTERVAL_CHANGE & g_stBfdlocal[usLoopi].ucParaChangeFlag))
            {
                g_stBfdlocal[usLoopi].ucRequiredMinRxInterval = g_stBfdlocal[usLoopi].ucRMRI;
                g_stBfdlocal[usLoopi].ucDesiredMinTxInterval = g_stBfdlocal[usLoopi].ucDMTI;
                g_stBfdlocal[usLoopi].ucDetectMult = g_stBfdlocal[usLoopi].ucDM;
                spm_bfd_sendpacket_intervalreg(usLoopi, TRUE);
            }
            else
            {
                g_stBfdlocal[usLoopi].ucDetectMult = g_stBfdlocal[usLoopi].ucDM;
                spm_bfd_sendpacket_intervalreg(usLoopi, FALSE);
            }
            g_stBfdlocal[usLoopi].ucParaChangeFlag = 0;
            
        }
    }
    
    /* �����ñ���������ΪTRUE�󣬻Ự��ʹ�ǳ�ʱδ�յ����ģ�Ҳ���ϱ�down�������ڲ����޸Ĳ���������¶�λBFD���Ķ���������
       ������ȷ��������ƽ̨Э��֪ͨɾ��BFD�Ự������BFD�Ự����û���յ����ĳ�ʱdown */
    if (FALSE == g_BIsRecordDownEvent)
    {
        /* �ϱ�down�¼� */
        spm_bfd_alamdown_task();
    }

    #if 1
    /* TODO:���YID Ϊ0�����û������ת����ͱ��ػỰ����ȥ������ */
    /* FPGA���յ�yourid��0�ı��ļ�û������ת������û�����б��ػỰ�� */

    for (usLoopi = 0; usLoopi < 128; usLoopi++) /* FIFOֻ��128�� */
    {
        /* �ж�FIFO�Ƿ������� */
        /* ����ת��ģ��Ĵ���ƫ��0FH��Զ�˱�ʾ��Ϊ0�ı����еĲ�����FIFO״ָ̬ʾ������[0]��FIFO ��������[1]��FIFO�ա� */
        usData = spm_oamtree_s16read(OAMFWDSRCFPAGBASE, 0xf);   /* �������٣���ʹ��DMA��ʽ��ȡ�ٶȿ� */
        if (0x2 == (usData&0x2)) /* FIFOΪ�� */     /* ����[1]��ֵ��ʾ�����ݣ�����һ���� */
        {
            break;
        }

        spm_oamtree_s16write(OAMFWDSRCFPAGBASE, 0xf, 1);

        /* ����ת��ģ��Ĵ���ƫ��0FH��Զ��BFD������state=down, DIAG=0��YID�ĸ�16λ */
        usRemoteYidtmph = spm_oamtree_s16read(OAMFWDSRCFPAGBASE, 0xb);
        /* ����ת��ģ��Ĵ���ƫ��0FH��Զ��BFD������state=down, DIAG=0��YID�ĵ�16λ */
        usRemoteYidtmpl = spm_oamtree_s16read(OAMFWDSRCFPAGBASE, 0xc);
        ulRemoteYid = usRemoteYidtmph;
        ulRemoteYid = (ulRemoteYid << 16) + usRemoteYidtmpl;

        if (0 != ulRemoteYid)
        {
            /* ��������ֹ����ɾ��ʱ�������� */
//            BmuWrLock(g_build_fpga_fwd_lock, BMU_WAIT_FOREVER);
            /* ������������� */
            if(g_ucCleanCfgCheck)
            {
                continue;
            }
            
            /* ���ж�һ���Ƿ���ת�������������ڣ�˵��FPGA������ */
            pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulRemoteYid);
            if (NULL == pstFwdInfo)/* Ϊ�� */
            {
                /* ���������òŻᴴ��ת���� */
                pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulRemoteYid);
                if (NULL != pstBfdCfg)/* ��Ϊ�� */
                {
                    if (g_usFwdNum >= BFD_MAX_FWDNUM)/* �ж����� */
                    {
                        continue;
                    }

                    /* �ж��Ƿ�Ϊ����λ������λ����Ҫת�����������ȷ����ģ�����������յ���YIDΪ0
                       ���ز�λҲ����Ҫ����ת��������ҵ���ʱ��Ὠ�� */
                    if ((ucLocalSlot == pstBfdCfg->usSlotid) || (0 == spm_hardw_getboardtype(pstBfdCfg->usSlotid)))
                    {
                        continue;
                    }
                    
                    /* ��¼ת����Ϣ */
                    /* modify begin by ����,jiangbo, 20131113, Mantis��:��, ԭ��:��Ϊ����������nbase�����̣߳�����ʹ��NB-
                       B_MM_ALLOC����ͬʱ���ܱ�verify */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL), MEM_SPM_BFD_FWD_GLOBAL_CB);
                    /* modify end by ����,jiangbo, 20131113 */
                    if (NULL != pstFwdInfo)/* ��Ϊ�� */
                    {
                        AVLL_INIT_NODE(pstFwdInfo->spm_fwd_node);
                        pstFwdInfo->ulDiscrimer = ulRemoteYid;
                        pstFwdInfo->usSlotid = pstBfdCfg->usSlotid;
                        /* g_stFwdGlbTreeByDis��ȫ��ת�����g_stBfdfwd�Ǳ���ת����������ͬ*/
                        if(!AVLL_INSERT(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node))
                        {
                            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));
                            pstFwdInfo = NULL;
                            continue;
                        }
                        
                        iRet = spm_bfd_addfwdtable(ulRemoteYid, pstBfdCfg->usSlotid);
                        if (SUCCESS != iRet)/* ִ��ʧ�� */
                        {
                            AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by ����,jiangbo, 20131113,
                                                                               Mantis��:��, ԭ��:��Ϊ����������nbase����
                                                                               �߳�,����ʹ��NBB_MM_FREE����ͬʱ���ܱ�
                                                                               verify */
                            pstFwdInfo = NULL;                                                  
                            continue;
                        }

                        /* ת����Ŀ��+1 */
                        g_usFwdNum++;
                    }
                }
            }
            
            /* ���� */
//            BmuUnlock(g_build_fpga_fwd_lock);
            
            /* ##�����ʱ�����߳��������ߣ���ֹ�ڽ���ת����Ŀ���ʱ�򣬶�ռ��Դ
            mantis:CR8000 V1R1-ER�汾���Բ��� 0082660## */

            usleep(20);//��ʱ20us
        }
    }        
    
    #endif


//    gettimeofday(&tv_end, NULL);
//    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd fsm use %ld microseconds\r\n",
//        (1000000 * (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec)));

    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_synid_task
 ��������  : ҵ����ÿ200ms����Ƿ���Ҫͬ������ID
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��13��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_synid_task(NBB_VOID)
{
    ucSynIdTimer++;

    if(ucSynIdTimer > SYN_ID_TIMER)
    {
        g_bIsModifySession = TRUE;
        ucSynIdTimer = 0;
    }
    
    (NBB_VOID)spm_bfd_syn_sessionid();
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_alarmfail_task
 ��������  : ÿ1s�ϱ����̴�����Ϣ
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��5��
    ��    ��   : jiangbo
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_alarmfail_task(NBB_VOID)
{
    BFD_ALARM_FAIL_INFO *alarm_fail_info = NULL;
    BFD_ALARM_FAIL_INFO *current_fail_info = NULL;

    #if 1
    alarm_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_FIRST(g_stAlarmFailTree);
    while ( NULL != alarm_fail_info)
    {
        current_fail_info = alarm_fail_info;
        alarm_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_NEXT(g_stAlarmFailTree, alarm_fail_info->alarm_fail_node);
        
        current_fail_info->ucTimer++;
        if (current_fail_info->ucTimer > ALARM_FAIL_TIMER)
        {
            spm_bfd_alamfail_task(current_fail_info->ulBfdId);
            AVLL_DELETE(g_stAlarmFailTree, current_fail_info->alarm_fail_node);
            NBB_MM_FREE(current_fail_info, MEM_SPM_BFD_GLOBAL_CB);
        }
    }
    #else
    alarm_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_FIRST(g_stAlarmFailTree);
    while ( NULL != alarm_fail_info)
    {
        for (current_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_FIRST(g_stAlarmFailTree);
        NULL != current_fail_info;
        current_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_NEXT(g_stAlarmFailTree, current_fail_info->alarm_fail_node))
        {
            current_fail_info->ucTimer++;
            if (current_fail_info->ucTimer > ALARM_FAIL_TIMER)
            {
                spm_bfd_alamfail_task(current_fail_info->ulBfdId);
                AVLL_DELETE(g_stAlarmFailTree, current_fail_info->alarm_fail_node);
                NBB_MM_FREE(current_fail_info, MEM_SPM_BFD_GLOBAL_CB);
                break;
            }
        }
        
        alarm_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_FIRST(g_stAlarmFailTree);
    }
    #endif
    
    return;
}


/*****************************************************************************
 �� �� ��  : spm_bfd_synstate_task
 ��������  : ����ÿ10ms�򱳰�����ͬ���Ự״̬
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��14��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_synstate_task(NBB_VOID)
{
    NBB_USHORT usLoopi;
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* ѭ�� */
    {
        /* ���Ѿ�ͬ�����������̣���ȡ��ʵ��״̬д��Ĵ���������д����Ч */
        if ((TRUE == g_stBfdlocal[usLoopi].bIsSendtoMaster) && (TRUE == g_stBfdlocal[usLoopi].bIsUpOnce))
        {
            if (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState)/* ״̬������UP״̬ */
            {
                g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_UP;/* ucStateRecord���ϱ������ص�״̬��2bit����Լ�ռ�*/
            }
            else if (BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState)/* ״̬������DOWN״̬ */
            {
                if (TRUE != g_stBfdlocal[usLoopi].bIsAdminFlag) /* ������Ϊ�յ�admin down���µ� */
                {
                    g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_DOWN;
                }
                else    /* ����Ϊ�յ�admin down���µ� */
                {
                    g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
                }
            }
            else/* ״̬������INIT��admin down״̬ */
            {
                g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
            }
            
        }
        else/* δͬ���������̻��ǵ�һ��UP */
        {
            g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
        }        
    }
    
    /* д�뱳������ */
    spm_bfd_state_fpgareg();    /* д�뵽�������ߺ󣬸���Ϣ�ᱻ���ع㲥�����е��� */

    /* ����cpu����д�� */
    if (0 == g_ucCpuTick)   /* ������*/
    {
        spm_oamtree_s16write(0x800088, 0, 0x1); /* ͨ������CPU��ʼ���� */
    }
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_syncheck_task
 ��������  : Ϊ�˷�ֹ����ȫ��ID�ı��Ķ�ʧ�����ʱ�ӻ�Ӧ���迪����ʱ����
             �ȴ����ݱ��ķ��ͺ�5S�ڣ����޻�Ӧ����ǿ��bIsReadySendtoMaster = false��
             ���кż�һ��������һ��ID���ݡ�
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��14��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_syncheck_task(NBB_INT iPara)
{
    /***************************************************************************/
    /* ��ʱ��ִ��                                                              */
    /***************************************************************************/
    (NBB_VOID)iPara;
    if (TRUE == g_bIsReadySendtoMaster)
    {
        /* ǿ����Ϊfalse */
        g_bIsReadySendtoMaster = FALSE;
        g_bIsModifySession = TRUE;
        g_ulSequenceID++;    
    }

    return SUCCESS;
}


/*****************************************************************************
 �� �� ��  : spm_bfd_dipc_open
 ��������  : mtl��س�ʼ��Ų��������ǰ����ʼ��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��05��28��
    ��    ��   : ����
    �޸�����   : �����ɺ���


  2.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : ���޸�MTLҪ��ķ�ʽ
*****************************************************************************/
NBB_INT spm_bfd_dipc_open(NBB_VOID)
{
    NBB_BOOL ret = FALSE;
    unsigned char ucSlot = 0;
    
    /* Դ */
    spm_hardw_getslot(&ucSlot);    
    g_stSrcNode.module_id = TYPE_MTL_BFD;
    g_stSrcNode.frame = 1;
    g_stSrcNode.slot = (unsigned int)ucSlot;
    g_stSrcNode.cpu = 0;

    /* Ŀ�� */
    g_stDesNode.module_id = TYPE_MTL_BFD;
    g_stDesNode.frame = 1;
    g_stDesNode.cpu = 0;
    mtl_open(g_stSrcNode.frame << 24 | g_stSrcNode.slot << 16 | g_stSrcNode.cpu, "BFD");
    
 #if 0
    ret = pal_mtl_open(g_stSrcNode, &linx_fd);
    if(ret < 0)
    {
        NBB_PRINTF("linx open error=%d", ret);
        return ERROR;
    }
    else
    {
        return OK;
    }
 #endif
    return OK;
    
}

/*****************************************************************************
 �� �� ��  : spm_bfd_synrec_task
 ��������  : BFD�̼�ͨ�Ž��պ���
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��22��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

  2.��    ��   : 2013��05��28��
    ��    ��   : ����
    �޸�����   : ��MTL�ĳ�ʼ�������������ǰ����
*****************************************************************************/
NBB_VOID spm_bfd_synrec_task(NBB_VOID)
{
    MTL_NODE  stRemoteNode;
    NBB_BOOL ret = FALSE;
    NBB_VOID *pDataBuf = NULL;
    NBB_INT   uiRecvLen = 0;
    NBB_BYTE *pBufTmp = NULL;
    
    //fd_set rset;
    
    MTL_FD_SET_T mtl_set;
    int maxfd;
    int fd;
    int path_id;
    
    while(1)
    {
        mtl_gen_fds("BFD", &maxfd, &mtl_set);
        ret = mtl_select(maxfd + 1, &mtl_set, NULL, NULL, NULL);     /* ����ʽ��linx�� */
        for (fd = 0; fd <= maxfd; fd++)
        {
            if(ret > 0 && MTL_FD_ISSET(fd, &mtl_set))     /* ���linx_fd�Ƿ����������rset����, */
            {
                if (0 != mtl_prefetch_pkt_len(fd, &uiRecvLen))
                {
                    continue;   /* ������һ���հ� */
                }

                pDataBuf = malloc(uiRecvLen);
                if (NULL == pDataBuf)
                {
                    continue;   /* ������һ���հ� */
                }

                uiRecvLen = mtl_recv(fd, &stRemoteNode.module_id, 0, pDataBuf, uiRecvLen);
                path_id = mtl_fd_to_identity(fd);

                stRemoteNode.frame = 1;
                stRemoteNode.slot = (path_id >> 16) & 0XFF;
                stRemoteNode.cpu = path_id & 0XFF;
                pBufTmp = (NBB_BYTE *)pDataBuf; /* ��һ���ֽ��ǰ������� */

                switch(((SPM_BFD_SYN_REV_PKT_HEAD*)pDataBuf)->syn_head.ucType)
                {
                    case BFD_FWD_MSG_ID:    /* ͬ��ID�İ� */
                    {
                        pBufTmp = pBufTmp + sizeof(SPM_BFD_SYN_REV_PKT_HEAD);          /* ƫ�Ƶ����� */
                        uiRecvLen = uiRecvLen - sizeof(SPM_BFD_SYN_REV_PKT_HEAD);      /* �õ����ɳ��� */
                        (NBB_VOID)spm_bfd_synrecv_sessionid((NBB_VOID *)pBufTmp,
                            uiRecvLen,
                            ((SPM_BFD_SYN_REV_PKT_HEAD*)pDataBuf)->syn_head.ulSrcSlot);
                        break;
                    }
                    case BFD_FWD_MSG_PCK:   /* ת������ */
                    {
                        pBufTmp = pBufTmp + sizeof(SPM_BFD_SYN_REV_PKT_HEAD);          /* ƫ�Ƶ����� */
                        uiRecvLen = uiRecvLen - sizeof(SPM_BFD_SYN_REV_PKT_HEAD);      /* �õ����ɳ��� */
                        (NBB_VOID)spm_bfd_fwdpacket_receive(pBufTmp, uiRecvLen,
                            ((SPM_BFD_SYN_REV_PKT_HEAD*)pDataBuf)->syn_head.ulSrcSlot);
                        break;
                    }
                    default:
                        break;
                }

                /* free the mtl allocated data buffer. */
                //pal_mtl_free(g_stSrcNode, &pDataBuf);
                
                free(pDataBuf);
                pDataBuf = NULL;
            }
        }
        BmuTaskTestPoint();
    }
}

/*****************************************************************************
 �� �� ��  : spm_bfd_alamup_task
 ��������  : BFD�Ự�����ϱ�����
 �������  : IN NBB_USHORT usLoopi : ��Ӧ����Ŀ��  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��9��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_alamup_task(IN NBB_USHORT usLoopi)
{
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMainSlot = 0;/* ���������̵Ĳ�λ�� */
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_USHORT *pusData = NULL;
    NBB_ULONG ulDataLenth = 0;

    //spm_bfd_event(usLoopi, FALSE);

    ucMainSlot = spm_hardw_getemustate();
    if (0xff == ucMainSlot) /* �������� */
    {
        return;
    }
    
    /* ���������̡�ҵ�����ϱ� */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* ����������̣��ж��Ƿ�Ϊ���� */
    if (0 == spm_hardw_getboardtype(ucLocalSlot))
    {
        if (ucMainSlot != ucLocalSlot)
        {
            g_stBfdlocal[usLoopi].bIsChangeToUp = FALSE;
            return;
        }
    }

    /* ������֡: 24+2(����0001)+����(���ϱ���Ŀ~β�� 2+n*12)+2(��Ŀ)+n*12 */
    pucInfoTmp = g_pucAlarmInfo + 24;
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = BFD_ALARM_TYPE;  /* ��д���� */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    ulDataLenth = 2 + BFD_ALARM_LENTH;
    *pusData = (NBB_USHORT)ulDataLenth;  /* ���� */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = 1;  /* ��Ŀ */
    pucInfoTmp = pucInfoTmp + 2;
    
    spm_bfd_alarminfo_encap(pucInfoTmp, usLoopi, TRUE);

    DipcSendData(ucMainSlot, 1, 0x123457, (ulDataLenth + 4), g_pucAlarmInfo);
        
    /* �ϱ�������λ */
    g_stBfdlocal[usLoopi].bIsChangeToUp = FALSE;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_alamdown_task
 ��������  : BFD�Ự�����ϱ�����
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��9��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��6��4��
    ��    ��   : gaos
    �޸�����   : �޸ı����̸澯������Ӹ澯��־���
                mantis:CR8000 V1R1-ER�汾���Բ��� 0078366
*****************************************************************************/
NBB_VOID spm_bfd_alamdown_task(NBB_VOID)
{
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMainSlot = 0;
    NBB_USHORT usLoopi = 0;
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_USHORT *pusData = NULL;
    NBB_ULONG ulDataLenth = 0;
    struct timeval tv_start;
    struct timeval tv_end;
    
    if (0 == g_ulStateToDownNum)
    {
        return;
    }
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "g_ulStateToDownNum = %ld\r\n", g_ulStateToDownNum);
    gettimeofday(&tv_start, NULL);
    
    ucMainSlot = spm_hardw_getemustate();
    if (0xff == ucMainSlot)
    {
        return;
    }
    
    /* ���������̡�ҵ�����ϱ� */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* ����������̣��ж��Ƿ�Ϊ����,BFD��������������˫�ݸ��Ƶ� ֻ�����������̲��ϱ�*/
    if (0 == spm_hardw_getboardtype(ucLocalSlot))//������
    {
        if (ucMainSlot != ucLocalSlot)
        {
            /* �����Ҫ�ϱ�down���ܸ澯���� */
            g_ulStateToDownNum = 0;
            
            /* ���ÿ��BFD��������Ҫ�ϱ��澯�ı�� */
            for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
            {
                if (TRUE == g_stBfdlocal[usLoopi].bIsChangeToDown)
                {
                    g_stBfdlocal[usLoopi].bIsChangeToDown = FALSE;
                }
            }            
            return;
        }
    }

    pucInfoTmp = g_pucAlarmInfo + 24;
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = BFD_ALARM_TYPE;  /* ��д���� */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    ulDataLenth = 2 + BFD_ALARM_LENTH * g_ulStateToDownNum; 
    *pusData = (NBB_USHORT)ulDataLenth;  /* ���� */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = g_ulStateToDownNum;  /* ��Ŀ */
    pucInfoTmp = pucInfoTmp + 2;
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (0 == g_ulStateToDownNum)
        {
            break;
        }
        if (TRUE == g_stBfdlocal[usLoopi].bIsChangeToDown)
        {
            spm_bfd_alarminfo_encap(pucInfoTmp, usLoopi, FALSE);
            g_stBfdlocal[usLoopi].bIsChangeToDown = FALSE;
            pucInfoTmp = pucInfoTmp + BFD_ALARM_LENTH;
            
            g_ulStateToDownNum--;

            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Down report, discrim is %ld,local diag: %d\r\n",
                g_stBfdlocal[usLoopi].ulLocalDiscr, g_stBfdlocal[usLoopi].ucLocalDiag);
        }
    }

    if (0 != g_ulStateToDownNum)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "BFD ERROR, downnum is %ld\r\n", g_ulStateToDownNum);
        g_ulStateToDownNum = 0;
        return;
    }
    
    DipcSendData(ucMainSlot, 1, 0x123457, (ulDataLenth + 4), g_pucAlarmInfo);
    gettimeofday(&tv_end, NULL);
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "alarm down use %ld microseconds\r\n",
        (1000000 * (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec)));
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_alarminfo_encap
 ��������  : ��װ�ϱ��澯�ı���
 �������  : IN NBB_BYTE *pucBuf : ����ָ��   
             IN NBB_USHORT usLoopi:��Ҫ�ϱ�����Ŀ��  
             IN NBB_BOOL BIsUpAlarm : �ϱ�up or down 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��26��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_alarminfo_encap(IN NBB_BYTE *pucBuf, IN NBB_USHORT usLoopi, IN NBB_BOOL BIsUpAlarm)
{
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_ULONG *pulData = NULL;
    NBB_BYTE *pucData1 = NULL;
    NBB_USHORT usData = 0;
    NBB_ULONG  ulDIdata = 0;
    NBB_ULONG *pulData2 = NULL;

    /* 12�ֽ�����: 4(local id)+1(state)+ 1(diag)+1(dmti��ʵ�ʷ���Ƶ��)+1(rmri��ʵ�ʷ���Ƶ��)+4(remote id)*/
    pucInfoTmp = pucBuf;
    pulData = (NBB_ULONG *)pucInfoTmp;
    *pulData = g_stBfdlocal[usLoopi].ulLocalDiscr;
    pucInfoTmp = pucInfoTmp + 4;
    
    pucData1 = (NBB_BYTE *)pucInfoTmp;
    *pucData1 = g_stBfdlocal[usLoopi].ucSessionState;
    pucInfoTmp = pucInfoTmp + 1;

    pucData1 = (NBB_BYTE *)pucInfoTmp;
    *pucData1 = g_stBfdlocal[usLoopi].ucLocalDiag;
    pucInfoTmp = pucInfoTmp + 1;

    if (FALSE == BIsUpAlarm)
    {
        pucData1 = (NBB_BYTE *)pucInfoTmp;
        *pucData1 = g_stBfdlocal[usLoopi].ucDMTI;
        pucInfoTmp = pucInfoTmp + 1;

        pucData1 = (NBB_BYTE *)pucInfoTmp;
        *pucData1 = BFD_1000MS;    /* DOWN���Ĺ̶�Ϊ1�� */
        pucInfoTmp = pucInfoTmp + 1;
    }
    
    else
    {
        pucData1 = (NBB_BYTE *)pucInfoTmp;
        /*BFD�������üĴ���ƫ��0xe000+N:Bit3-0����������ÿһ��BFD����ʵ�ʷ���ʱ������Ĭ��ֵΪ1s */
        usData = spm_oamtree_s16read(BFDPAKFGPABASE, usLoopi + 0xe000);
        *pucData1 = (NBB_BYTE)(usData & 0xf);
        pucInfoTmp = pucInfoTmp + 1;

        pucData1 = (NBB_BYTE *)pucInfoTmp;
        /*���ջỰ״̬�Ĵ���ƫ��1800H+NH:��ĿN��ʵ�ʹ�������ֵ�ı���[31��16]������MS Ϊ��λ�� */
        usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1800);
        ulDIdata = usData;
        /*���ջỰ״̬�Ĵ���ƫ��1000H+NH:��ĿN��ʵ�ʹ�������ֵ�ı���[15��0]������MS Ϊ��λ�� */
        usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1000);
        ulDIdata = (ulDIdata << 16) + usData;
        switch (ulDIdata)
        {
            case 4:
                *pucData1 = BFD_3MS3;
                break;
            case 10:
                *pucData1 = BFD_10MS;
                break;
            case 20:
                *pucData1 = BFD_20MS;
                break;
            case 30:
                *pucData1 = BFD_30MS;
                break;
            case 40:
                *pucData1 = BFD_40MS;
                break;
            case 50:
                *pucData1 = BFD_50MS;
                break;
            case 60:
                *pucData1 = BFD_60MS;
                break;
            case 70:
                *pucData1 = BFD_70MS;
                break;
            case 80:
                *pucData1 = BFD_80MS;
                break;
            case 90:
                *pucData1 = BFD_90MS;
                break;
            case 100:
                *pucData1 = BFD_100MS;
                break;
            case 200:
                *pucData1 = BFD_200MS;
                break;
            case 300:
                *pucData1 = BFD_300MS;
                break;
            case 400:
                *pucData1 = BFD_400MS;
                break;
            case 500:
                *pucData1 = BFD_500MS;
                break;
            case 600:
                *pucData1 = BFD_600MS;
                break;
            case 700:
                *pucData1 = BFD_700MS;
                break;
            case 800:
                *pucData1 = BFD_800MS;
                break;
            case 900:
                *pucData1 = BFD_900MS;
                break;
            case  1000:
                *pucData1 = BFD_1000MS;
                break;
            default :
                *pucData1 = BFD_1000MS;
                break;
                
        }
        
        pucInfoTmp = pucInfoTmp + 1;
    }

    pulData2 = (NBB_ULONG *)pucInfoTmp;
    *pulData2 = g_stBfdlocal[usLoopi].ulRemoteDiscr;

    return;
}


/*****************************************************************************
 �� �� ��  : spm_bfd_alamfail_task
 ��������  : BFD�Ự�����ϱ�����(�ϱ�����ʧ��)
 �������  : IN NBB_ULONG ulDiscrimer : BFD key  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��1��
    ��    ��   : ����,jiangbo
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_alamfail_task(IN NBB_ULONG ulDiscrimer)
{
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMainSlot = 0;/* ���������̵Ĳ�λ�� */
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_ULONG ulDataLenth = 0;
    NBB_BYTE arrucDate[42];

    ucMainSlot = spm_hardw_getemustate();
    if (0xff == ucMainSlot) /* �������� */
    {
        return;
    }
    
    /* ���������̡�ҵ�����ϱ� */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* ����������̣��ж��Ƿ�Ϊ���� */
    if (0 == spm_hardw_getboardtype(ucLocalSlot))
    {
        /* �����������ز��ϱ� */
        if (ucMainSlot != ucLocalSlot)
        {
            return;
        }
    }

    NBB_OS_MEMSET(arrucDate, 0, sizeof(arrucDate));

    pucInfoTmp = arrucDate;
    
    /* ������֡: 24+2(����0001)+����(���ϱ���Ŀ~β�� 2+n*12)+2(��Ŀ)+n*12 */
    NBB_PUT_SHORT(pucInfoTmp + 24, BFD_ALARM_TYPE); /* ��д���� */

    ulDataLenth = 2 + BFD_ALARM_LENTH;
    NBB_PUT_SHORT(pucInfoTmp + 26, ulDataLenth);  /* ���� */

    NBB_PUT_SHORT(pucInfoTmp + 28, 1);    /* ��Ŀ */
    
    NBB_PUT_LONG(pucInfoTmp + 30, ulDiscrimer);   /* BFD�ỰID */

    *(pucInfoTmp + 34) =  5;    /* �Ự״̬��5��ʾBFD����ʧ�� */

    DipcSendData(ucMainSlot, 1, 0x123457, (ulDataLenth + 4), arrucDate);
        
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd id %lu cfg fail, has reported!\r\n", ulDiscrimer);
    
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_alamfail_task
 ��������  : ��¼����ʧ�ܵ�BFD
 �������  : IN NBB_ULONG ulDiscrimer : BFD key  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��12��1��
    ��    ��   : ����,jiangbo
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_need_alarm_fail(IN NBB_ULONG ulDiscrimer)
{
    BFD_ALARM_FAIL_INFO *current_fail_info = NULL;
    NBB_INT iRet = SUCCESS;

    current_fail_info = (BFD_ALARM_FAIL_INFO *)NBB_MM_ALLOC(sizeof(BFD_ALARM_FAIL_INFO), 
        NBB_NORETRY_ACT, MEM_SPM_BFD_ALARM_FAIL_TREE_CB);
    if (NULL == current_fail_info)/* ��Ϊ�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Memory is not enough"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough!\r\n");
        iRet = -1;
        goto EXIT_LABEL;
    }
    current_fail_info->ulBfdId = ulDiscrimer;
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd id %lu cfg fail, need reconfig\r\n", ulDiscrimer);
    AVLL_INIT_NODE(current_fail_info->alarm_fail_node);
    if (!AVLL_INSERT(g_stAlarmFailTree, current_fail_info->alarm_fail_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT"insert alarm fail avll tree error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "insert alarm fail avll tree error!\r\n");
        NBB_MM_FREE(current_fail_info, MEM_SPM_BFD_ALARM_FAIL_TREE_CB);
        iRet = -2;
        goto EXIT_LABEL;
    }

EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_boardcheck_task
 ��������  : �����жϵ�һ����λ��NP���Ƿ����˱仯
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��26��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_boardcheck_task(NBB_VOID)
{
    NBB_BYTE        aucBoardState[10];
    NBB_BYTE        aucBoardBmuState[10];
    NBB_USHORT  usData1 = 0;
    NBB_USHORT  usData2 = 0;
    NBB_USHORT  usData5 = 0;
    NBB_USHORT  usData6 = 0;
    NBB_USHORT usLocalNum = 0;
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE  ucLoopi = 0;
    NBB_USHORT  usLoopj = 0;
    NBB_BYTE  ucSlotid = 0;        /* ��ǰ��һ����λ��cpu������SPU�� */
    SPM_BFD_FWD_PACKET2 stBfdFwd2;
    SPM_BFD_FWD_PACKET3 stBfdFwd3;

    OS_MEMSET(&stBfdFwd2, 0, sizeof(SPM_BFD_FWD_PACKET2));
    OS_MEMSET(&stBfdFwd3, 0, sizeof(SPM_BFD_FWD_PACKET3));
    

    /* ���ؿ��Ʋ��ּĴ������Ӹߵ�������Ϊ��λ16��1SPU����λ��Ϣ������λ���Ͳ���λ */
    usData1 = spm_oamtree_s16read(0x80000c, 0);
    
    /* ���ؿ��Ʋ��ּĴ������Ӹߵ�������Ϊ��λ16��1SPU����λ��Ϣ����BMU������������BMU���������� */
    usData2 = spm_oamtree_s16read(0x800010, 0);
    
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* ����SPU�̰γ�id���� */
    for (ucLoopi = 0; ucLoopi < 10; ucLoopi++)
    {
        if (500 <= g_astBoardState[ucLoopi].usTimeCounter)
        {
            for (usLoopj = 0; usLoopj < BFD_MAX_SESNUM; usLoopj++)
            {
                g_stBfdIdStore[ucLoopi].stBfdIdInfo[usLoopj].ulDiscrimer = DRV_ULONG_INVALID;
            }
            spm_bfd_id_fpgareg((ucLoopi + 1), TRUE);
            g_astBoardState[ucLoopi].usTimeCounter = 0;
            continue;
        }
        
        if (0 != g_astBoardState[ucLoopi].usTimeCounter)
        {
            g_astBoardState[ucLoopi].usTimeCounter++;
            continue;
        }

        if (0 == g_astBoardState[ucLoopi].usTimeCounter)
        {
            aucBoardState[ucLoopi] = (NBB_BYTE)((NBB_USHORT)(usData1 << (15 - ucLoopi)) >> 15);
            aucBoardBmuState[ucLoopi] = (NBB_BYTE)((NBB_USHORT)(usData2 << (15 - ucLoopi)) >> 15);
            if ((0 == aucBoardState[ucLoopi]) && (1 == g_astBoardState[ucLoopi].ucBoardState))/* ˵���̱��γ� */
            {
                g_astBoardState[ucLoopi].usTimeCounter = 1;
            }

            if ((0 == aucBoardBmuState[ucLoopi]) && (1 == g_astBoardState[ucLoopi].ucBoardBmuState))/* ˵����reboot */
            {
                g_astBoardState[ucLoopi].usTimeCounter = 1;
            }
            g_astBoardState[ucLoopi].ucBoardState = aucBoardState[ucLoopi];
            g_astBoardState[ucLoopi].ucBoardBmuState = aucBoardBmuState[ucLoopi];
        }
    }

    for (ucLoopi = 0; ucLoopi < 10; ucLoopi++)
    {
        aucBoardState[ucLoopi] = (NBB_BYTE)((NBB_USHORT)(usData1 << (15 - ucLoopi)) >> 15);
        if (1 == aucBoardState[ucLoopi])
        {
            aucBoardBmuState[ucLoopi] = (NBB_BYTE)((NBB_USHORT)(usData2 << (15 - ucLoopi)) >> 15);
            if (1 == aucBoardBmuState[ucLoopi])
            {
                ucSlotid = ucLoopi + 1;
                break;
            }
        }
    }

    if (10 == ucLoopi)
    {
        g_ucFirstNpBoard = 0;
        return;
    }
    else
    {
        if (g_ucFirstNpBoard != ucSlotid)
        {
            /* �޸�SRC�����лỰ���͵�NP�� */
            g_ucFirstNpBoard = ucSlotid;

            for (usLocalNum = 0; usLocalNum < BFD_MAX_SESNUM; usLocalNum++)
            {
                if (DRV_ULONG_INVALID != g_stBfdlocal[usLocalNum].ulDiscrimer)
                {
                    stBfdFwd2.usSysPortID = g_ucFirstNpBoard * 256 + BFD_ARAD_LOGICPORT; 
                    usData5 = *(NBB_USHORT *)(&stBfdFwd2);
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x2, usData5);
                    
                    stBfdFwd3.usDslotid = g_ucFirstNpBoard; 
                    stBfdFwd3.usSslotid = ucLocalSlot;
                    stBfdFwd3.usPktType = BFD_DUNE_TOC3;
                    stBfdFwd3.usFwdType = g_stBfdlocal[usLocalNum].ucBfdFwdType;
                    usData6 = *(NBB_USHORT *)(&stBfdFwd3);
                    /* BFD����ģ��Ĵ���ƫ��0x12000+4*N+0x1��Bit15-0��ԴMAC��SMAC����ַbit48-33 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x1, usData6);
                }
            }
        }
    }
    return;
    
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_daminit
 * �� �� ��  : ������,limingming
 * ��������  : 2013��11��12��
 * ��������  : DMA���ʷ�ʽ�ĳ�ʼ����bfdʹ�õ���PCIE(������ӦΪspm_bfd_dmainit��ԭʼ�����������������O(��_��)O~)
 * �������  : NBB_VOID
 * �������  : ��  
 * �� �� ֵ  :   0=�ɹ�������=ʧ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_INT spm_bfd_daminit(NBB_VOID)
{
    NBB_INT iRet = ERROR;
    g_stDmaReadData.channel = 0;
    g_aullDmaDataInfo[0][0] = 0xc80000000LL + (BFDSESSIONBASE + 0x800) * 4;/* ״̬ */
    g_aullDmaDataInfo[0][1] = BFD_MAX_DMA_NUM * 4 / 4;
    g_aullDmaDataInfo[1][0] = 0xc80000000LL + BFDSTUDYBASE*4; /* ������ѧϰ */
    g_aullDmaDataInfo[1][1] = BFD_MAX_DMA_DRIM_NUM * 4 / 4;

    g_iDevFd = open("/dev/pciedev", O_RDWR);    /* why:Ϊʲô��д��ʽ�򿪣���ʹ��ֻ����ʽ"O_RDONLY"�� */
    if (ERROR == g_iDevFd)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "OPEN %d-------------\r\n", g_iDevFd);
        return iRet;
    }
    
    iRet = Read_Dma(g_iDevFd, g_aullDmaDataInfo, 2, 5000, 0);    /* ��ָ���������ȡ��Ϣ */
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "READ %d-------------\r\n", iRet);
    return iRet;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_damread
 * �� �� ��  : ������,limingming
 * ��������  : 2013��11��12��
 * ��������  : ͨ��DMA��ʽ��ȡ��������,��ȡ������ݴ����ȫ�ֱ���g_auiDmaData��g_auiDmaDrimData��
 * �������  : NBB_VOID  ��
 * �������  : ��
 * �� �� ֵ  :   ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_VOID spm_bfd_damread(NBB_VOID)
{
    NBB_INT iRet = SUCCESS;
    iRet = read(g_iDevFd,&g_stDmaReadData,BFD_MAX_DMA_NUM + BFD_MAX_DMA_DRIM_NUM);
    

    if (0 == g_stDmaReadData.error)
    {
        OS_MEMCPY(g_auiDmaData, &g_stDmaReadData.data[0], BFD_MAX_DMA_NUM * 4);         /*���ջỰ״̬*/
        OS_MEMCPY(g_auiDmaDrimData, &g_stDmaReadData.data[BFD_MAX_DMA_NUM], BFD_MAX_DMA_DRIM_NUM * 4);/*���ձ����е�MYID*/
    }
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_checkemustate
 ��������  : BFD��������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��11��27��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_checkemustate(NBB_VOID)
{
    /* ����CPU�������� */
    g_bIsModifySession = TRUE;
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "mustate is changed\r\n");
    return;
}


/*****************************************************************************
 �� �� ��  : spm_bfd_intervalreport
 ��������  : �Ự�ϱ���ʵ����Ƶ��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��4��16��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_intervalreport(NBB_VOID)
{
    static NBB_USHORT s_uscounter = 0;
    NBB_USHORT usloopi = 0;
    NBB_ULONG ulworkingrx = 0;
    NBB_ULONG ulworkingtx = 0;
    NBB_USHORT usdate1 = 0;
    NBB_USHORT usdate2 = 0;
    
    if (30 == s_uscounter)
    {
        /* ÿһ���Ӳ�ѯһ���Ƿ���Ҫ�ϱ��շ�Ƶ�� */
        for ( ; usloopi < BFD_MAX_SESNUM; usloopi++)
        {
            if ((TRUE == g_stintervalinfo[usloopi].bflag) || 
                (BFD_UP != g_stBfdlocal[usloopi].ucSessionState))
            {
                continue;
            }
            else
            {
                usdate1 = spm_oamtree_s16read(BFDSESSIONBASE, 0x1000 + usloopi);
                usdate2 = spm_oamtree_s16read(BFDSESSIONBASE, 0x1800 + usloopi);
                ulworkingrx = usdate2;
                ulworkingrx = (ulworkingrx << 16) +  usdate1;
                ulworkingtx = spm_oamtree_s16read(BFDPAKFGPABASE, 0xe000 + usloopi);
                if ((ulworkingrx != g_stintervalinfo[usloopi].ulworkingrx) ||
                    (ulworkingtx != g_stintervalinfo[usloopi].ulworkingtx))
                {
                    g_stintervalinfo[usloopi].bflag = TRUE;
                    g_stBfdlocal[usloopi].usTimerLiveUp = g_stBfdlocal[usloopi].usTimerLiveUp + BFD_LIVEUP;
                    g_stintervalinfo[usloopi].ulworkingrx = ulworkingrx;
                    g_stintervalinfo[usloopi].ulworkingtx = ulworkingtx;
                }
            }
        }
        
        s_uscounter = 0;
    }
    s_uscounter++;
    return;
}
