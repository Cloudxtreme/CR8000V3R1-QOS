/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_bfd_task.c
  版 本 号   : 初稿
  作    者   : limingming
  生成日期   : 2012年9月24日
  最近修改   :
  功能描述   : BFD轮询任务模块
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月24日
    作    者   : limingming
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include "r870_lib.h"
#include <mtl.h>

struct dma_read_unit g_stDmaReadData;

/* 清配置标记 */
extern NBB_BYTE g_ucCleanCfgCheck;

/* 操作fpga转发二叉树读写锁 */
extern BMU_RWLOCK_ID g_build_fpga_fwd_lock;

/*****************************************************************************
 函 数 名  : spm_bfd_fsm_task
 功能描述  : 每5ms轮询一次本盘所有会话对应寄存器信息，更改会话状态
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月13日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

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
    NBB_BYTE    ucLocalSlot   = 0;          /* 本盘槽位号 */

//    struct timeval tv_start;
//    struct timeval tv_end;

    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();

//    gettimeofday(&tv_start, NULL);

    /* 通过DMA方式获取bfd包 */
    spm_bfd_damread();

    /* 获取单盘所在槽位号 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    
    /* 取接收会话状态寄存器中的内容 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        /* 先删除需要删除的BFD */
        if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* 若需要删除 */
        {
            if (TRUE == g_stBfdlocal[usLoopi].bIsChangeToDown)
            {
                if (g_ulStateToDownNum > 0) /* 可能为0 */
                {
                    g_ulStateToDownNum--;
                }
            }
            
            /* 清空FPGA对应的发送寄存器内容 */
            spm_bfd_clear_sendfpgareg(usLoopi);

            /*清空FPGA BFD攻击寄存器内容*/
            spm_bfd_clear_recv_fpgareg(usLoopi);
            
            /* 将会话信息至为无效值 */
            OS_MEMSET(&g_stBfdlocal[usLoopi], 0, sizeof(SPM_BFD_CB));
            g_stBfdlocal[usLoopi].bIsSendtoMaster = FALSE;  /* 上面已经清0了，为什么还要清0，是为了防止FALSE定义不为0? */
            g_stBfdlocal[usLoopi].ucbIsNeedDelete = FALSE;  /* 上面已经清0了，为什么还要清0，是为了防止FALSE定义不为0? */
            g_stBfdlocal[usLoopi].ulDiscrimer = DRV_ULONG_INVALID;

            /* 会话状态发生变化，需要同步 */
            g_bIsModifySession = TRUE;
            
        }
        
        /* 学习对端描述符 */
        if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
        {
            /* FPGA根据目的IP对应好BFD报文了，这里不需要考虑哪个BFD是对应收到的包的 */
            if (g_stBfdlocal[usLoopi].ulRemoteDiscr == 0)
            {
                /* 获取收到的包里的my discriminator */
                usLoopj = usLoopi * 2;
                
                usRemoteDisctmpl = (NBB_USHORT)g_auiDmaDrimData[usLoopj];
                usRemoteDisctmph = (NBB_USHORT)g_auiDmaDrimData[usLoopj + 1];
                ulRemoteDisc = usRemoteDisctmph;    /* 从FPGA里读出YID的高低word, 拼成4字节的YID */
                ulRemoteDisc = (ulRemoteDisc << 16) + usRemoteDisctmpl;

                if (0 != ulRemoteDisc)/* 描述符有效 */
                {
                    /* log必须在这里记录，如果在if外面，会导致配置的BFD每5ms记录一次 */
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

                    /* 清空FPGA学习到的YID */
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);

                    g_stBfdlocal[usLoopi].ulRemoteDiscr = ulRemoteDisc;
                    
                    /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
                    /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr)&DRV_USHORT_INVALID);
                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                }
            }
            else /* 若描述符不为零，条件判断描述符是否老化需要重新学习 */
            {
                if ((BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState) || (BFD_INIT == g_stBfdlocal[usLoopi].ucSessionState))
                {
                    if (TRUE == g_stBfdlocal[usLoopi].pstBasicBfdCB.peer_sid_selflearn_enable)
                    {
                        /* 从FPGA获取FPGA学到的描述符 */
                        usLoopj = usLoopi * 2;

                        usRemoteDisctmpl = (NBB_USHORT)g_auiDmaDrimData[usLoopj];
                        usRemoteDisctmph = (NBB_USHORT)g_auiDmaDrimData[usLoopj + 1];
                        ulRemoteDisc = usRemoteDisctmph;
                        ulRemoteDisc = (ulRemoteDisc << 16) + usRemoteDisctmpl;

                        if (0 != ulRemoteDisc) /* 描述符有效 */
                        {
                            /* 本地记录的描述符与FPGA学习到的不一致，重新学习 */
                            if (g_stBfdlocal[usLoopi].ulRemoteDiscr != ulRemoteDisc)
                            {
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "the %u bfd study remote dis again,"
                                    "my_sid = %u, your_discriminator %ld\r\n",
                                    usLoopi, g_stBfdlocal[usLoopi].pstBasicBfdCB.my_sid, ulRemoteDisc);

                                /* 清空FPGA学习到的YID */
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

        if (FALSE == g_BIsCloseStateMachine[usLoopi])/* 调试用，正常跑状态机 */
        {
            /* 会话正常 */
            if ((BFD_NORMAL == g_stBfdlocal[usLoopi].usBfdflag) && (BFD_ADMINDOWN != g_stBfdlocal[usLoopi].ucSessionState))
            {
                usData = (NBB_USHORT)g_auiDmaData[usLoopi];

                bIsTimeExpired = ((usData >> 15) & 0x1);    /* 条目N的BFD告警状态 */
                
                /* 判断是否超时 */
                if (TRUE == bIsTimeExpired)
                {
                    g_stBfdlocal[usLoopi].ucLocalDiag = bfd_diag_ctrl_detect_exp;
                    
                    if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
                    {
                        /* ldp lsp 主动端超时down之后，1个发包周期老花掉学习到的YID，保证down发出且不会让对端学错YID */
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
                    
                    spm_bfd_state_fsm_down(usLoopi,BFD_STA_BUTT);    /* 超时直接设down，不使用spm_bfd_state_fsm */
                }
                else/* 解析收到对端的报文的stat */
                {                
                    /* 判断是否是新收到的报文的stat */                
                    if (g_aucCouterRecord[usLoopi] != (NBB_BYTE)(usData & 0x3)) /* 2bit的计数 */
                    {
                        ucSessionSta = (NBB_BYTE)((usData >> 7) & 0x3); /* 条目N的接收STATE值 */
                        spm_bfd_state_fsm(usLoopi, ucSessionSta);
                        g_aucCouterRecord[usLoopi] = (NBB_BYTE)(usData & 0x3);
                    }
                }

                /* add begin by limingming, 2013/4/11, Mantis号:HSDxxxxxx, 原因:ID同步 */
                /* 第一次up后更新计数器并择机上报up时间 */
                if ((0 != g_stBfdlocal[usLoopi].usTimerLiveUp)
                    && (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState))
                {
                    /* 更新第一次up后的计数器 */
                    g_stBfdlocal[usLoopi].usTimerLiveUp--;  /* 在状态脱离UP之后直接清空g_stBfdlocal，同时会清空该计数器 */

                    /* up稳定规定时间后上报up事件 */
                    if (0 == g_stBfdlocal[usLoopi].usTimerLiveUp)
                    {
                        if (TRUE == g_stintervalinfo[usLoopi].bflag)
                        {
                            g_stintervalinfo[usLoopi].bflag = FALSE; 
                        }
                        
                        /* 上报up事件 */
                        spm_bfd_alamup_task(usLoopi);

                    }
                }
                /* add end by limingming, 2013/4/11 */

                if((0 != g_stBfdlocal[usLoopi].usTimerLiveDown)
                    && (BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState))
                {
                    g_stBfdlocal[usLoopi].usTimerLiveDown--;
                    
                    if (0 == g_stBfdlocal[usLoopi].usTimerLiveDown) /* down延时到期 */
                    {
                        g_stBfdlocal[usLoopi].bIsChangeToDown = TRUE;
                        g_ulStateToDownNum++;
                        
                        //spm_bfd_event(usLoopi, TRUE);
                    }
                }
            }
        }
        else    /* 调试用，强制不跑状态机 */
        {
            /* 会话正常 */
            if ((BFD_NORMAL == g_stBfdlocal[usLoopi].usBfdflag) && (BFD_ADMINDOWN != g_stBfdlocal[usLoopi].ucSessionState))
            {
                usData = (NBB_USHORT)g_auiDmaData[usLoopi];

                bIsTimeExpired = ((usData >> 15) & 0x1);    /* 条目N的BFD告警状态 */
                
                /* 判断是否超时 */
                if (TRUE == bIsTimeExpired)
                {
                    /* 判断是否是新收到的报文的stat */                
                    if (g_aucCouterRecord[usLoopi] != (NBB_BYTE)(usData & 0x3)) /* 2bit的计数 */
                    {                    
                        NBB_PRINTF("MID=%u down,control detection time expired!\r\n", g_stBfdlocal[usLoopi].ulDiscrimer);
                
                        g_aucCouterRecord[usLoopi] = (NBB_BYTE)(usData & 0x3);
                    }
                }

                /* 不跑状态机 */
            }
        }

        /* 会话在up状态时，会话参数被修改（一般是修改配置导致） */
        if ((0 != g_stBfdlocal[usLoopi].ucParaChangeFlag) && (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState))
        {
            /* 此处如果是位图的方式记录，使用if else方式，只是目前使用的标记较少，这样没有问题 */
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
    
    /* 调试用变量，设置为TRUE后，会话即使是超时未收到报文，也不上报down，可以在不用修改参数的情况下定位BFD报文丢在了哪里
       ；并且确定到底是平台协议通知删除BFD会话，还是BFD会话本身没有收到报文超时down */
    if (FALSE == g_BIsRecordDownEvent)
    {
        /* 上报down事件 */
        spm_bfd_alamdown_task();
    }

    #if 1
    /* TODO:检查YID 为0的情况没有命令转发表和本地会话，包去哪里了 */
    /* FPGA接收到yourid非0的报文既没有命中转发表，有没有命中本地会话表 */

    for (usLoopi = 0; usLoopi < 128; usLoopi++) /* FIFO只有128个 */
    {
        /* 判断FIFO是否有数据 */
        /* 查找转发模块寄存器偏移0FH：远端标示符为0的报文中的参数的FIFO状态指示。比特[0]：FIFO 满。比特[1]：FIFO空。 */
        usData = spm_oamtree_s16read(OAMFWDSRCFPAGBASE, 0xf);   /* 数据量少，比使用DMA方式读取速度快 */
        if (0x2 == (usData&0x2)) /* FIFO为空 */     /* 比特[1]无值表示有数据，但不一定满 */
        {
            break;
        }

        spm_oamtree_s16write(OAMFWDSRCFPAGBASE, 0xf, 1);

        /* 查找转发模块寄存器偏移0FH：远端BFD报文中state=down, DIAG=0的YID的高16位 */
        usRemoteYidtmph = spm_oamtree_s16read(OAMFWDSRCFPAGBASE, 0xb);
        /* 查找转发模块寄存器偏移0FH：远端BFD报文中state=down, DIAG=0的YID的低16位 */
        usRemoteYidtmpl = spm_oamtree_s16read(OAMFWDSRCFPAGBASE, 0xc);
        ulRemoteYid = usRemoteYidtmph;
        ulRemoteYid = (ulRemoteYid << 16) + usRemoteYidtmpl;

        if (0 != ulRemoteYid)
        {
            /* 加锁，防止配置删除时出现问题 */
//            BmuWrLock(g_build_fpga_fwd_lock, BMU_WAIT_FOREVER);
            /* 如果正在清配置 */
            if(g_ucCleanCfgCheck)
            {
                continue;
            }
            
            /* 先判断一下是否在转发软件表里，若存在，说明FPGA有问题 */
            pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulRemoteYid);
            if (NULL == pstFwdInfo)/* 为空 */
            {
                /* 必须有配置才会创建转发表 */
                pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulRemoteYid);
                if (NULL != pstBfdCfg)/* 不为空 */
                {
                    if (g_usFwdNum >= BFD_MAX_FWDNUM)/* 判断条件 */
                    {
                        continue;
                    }

                    /* 判断是否为本槽位，本槽位不需要转发表，可能是先发报文，后建立表项导致收到的YID为0
                       主控槽位也不需要建立转发表，创建业务的时候会建立 */
                    if ((ucLocalSlot == pstBfdCfg->usSlotid) || (0 == spm_hardw_getboardtype(pstBfdCfg->usSlotid)))
                    {
                        continue;
                    }
                    
                    /* 记录转发信息 */
                    /* modify begin by 蒋博,jiangbo, 20131113, Mantis号:无, 原因:因为本函数不由nbase分配线程，不能使用NB-
                       B_MM_ALLOC管理，同时不能被verify */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL), MEM_SPM_BFD_FWD_GLOBAL_CB);
                    /* modify end by 蒋博,jiangbo, 20131113 */
                    if (NULL != pstFwdInfo)/* 不为空 */
                    {
                        AVLL_INIT_NODE(pstFwdInfo->spm_fwd_node);
                        pstFwdInfo->ulDiscrimer = ulRemoteYid;
                        pstFwdInfo->usSlotid = pstBfdCfg->usSlotid;
                        /* g_stFwdGlbTreeByDis是全局转发表和g_stBfdfwd是本盘转发表，索引不同*/
                        if(!AVLL_INSERT(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node))
                        {
                            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));
                            pstFwdInfo = NULL;
                            continue;
                        }
                        
                        iRet = spm_bfd_addfwdtable(ulRemoteYid, pstBfdCfg->usSlotid);
                        if (SUCCESS != iRet)/* 执行失败 */
                        {
                            AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by 蒋博,jiangbo, 20131113,
                                                                               Mantis号:无, 原因:因为本函数不由nbase分配
                                                                               线程,不能使用NBB_MM_FREE管理，同时不能被
                                                                               verify */
                            pstFwdInfo = NULL;                                                  
                            continue;
                        }

                        /* 转发条目数+1 */
                        g_usFwdNum++;
                    }
                }
            }
            
            /* 解锁 */
//            BmuUnlock(g_build_fpga_fwd_lock);
            
            /* ##添加延时，让线程主动休眠，防止在建立转发条目多的时候，独占资源
            mantis:CR8000 V1R1-ER版本中试测试 0082660## */

            usleep(20);//延时20us
        }
    }        
    
    #endif


//    gettimeofday(&tv_end, NULL);
//    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd fsm use %ld microseconds\r\n",
//        (1000000 * (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec)));

    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_synid_task
 功能描述  : 业务盘每200ms检查是否需要同步本盘ID
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月13日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

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
 函 数 名  : spm_bfd_alarmfail_task
 功能描述  : 每1s上报本盘错误信息
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月5日
    作    者   : jiangbo
    审 核 人   : #
    修改内容   : 新生成函数

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
 函 数 名  : spm_bfd_synstate_task
 功能描述  : 各盘每10ms向背板总线同步会话状态
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月14日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_synstate_task(NBB_VOID)
{
    NBB_USHORT usLoopi;
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* 循环 */
    {
        /* 若已经同步给了主控盘，则取真实的状态写入寄存器，否则写入无效 */
        if ((TRUE == g_stBfdlocal[usLoopi].bIsSendtoMaster) && (TRUE == g_stBfdlocal[usLoopi].bIsUpOnce))
        {
            if (BFD_UP == g_stBfdlocal[usLoopi].ucSessionState)/* 状态机处于UP状态 */
            {
                g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_UP;/* ucStateRecord是上报给主控的状态，2bit，节约空间*/
            }
            else if (BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState)/* 状态机处于DOWN状态 */
            {
                if (TRUE != g_stBfdlocal[usLoopi].bIsAdminFlag) /* 不是因为收到admin down导致的 */
                {
                    g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_DOWN;
                }
                else    /* 是因为收到admin down导致的 */
                {
                    g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
                }
            }
            else/* 状态机处于INIT或admin down状态 */
            {
                g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
            }
            
        }
        else/* 未同步给主控盘或不是第一次UP */
        {
            g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
        }        
    }
    
    /* 写入背板总线 */
    spm_bfd_state_fpgareg();    /* 写入到背板总线后，该信息会被主控广播给所有单盘 */

    /* 增加cpu心跳写入 */
    if (0 == g_ucCpuTick)   /* 调试用*/
    {
        spm_oamtree_s16write(0x800088, 0, 0x1); /* 通报本盘CPU开始工作 */
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_syncheck_task
 功能描述  : 为了防止传递全局ID的报文丢失或过度时延回应，需开启定时器，
             等待传递报文发送后5S内，若无回应，则强行bIsReadySendtoMaster = false，
             序列号加一，从新再一次ID传递。
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月14日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_syncheck_task(NBB_INT iPara)
{
    /***************************************************************************/
    /* 定时器执行                                                              */
    /***************************************************************************/
    (NBB_VOID)iPara;
    if (TRUE == g_bIsReadySendtoMaster)
    {
        /* 强制置为false */
        g_bIsReadySendtoMaster = FALSE;
        g_bIsModifySession = TRUE;
        g_ulSequenceID++;    
    }

    return SUCCESS;
}


/*****************************************************************************
 函 数 名  : spm_bfd_dipc_open
 功能描述  : mtl相关初始化挪出来，提前做初始化
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年05月28日
    作    者   : 刘娟
    修改内容   : 新生成函数


  2.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 新修改MTL要求的方式
*****************************************************************************/
NBB_INT spm_bfd_dipc_open(NBB_VOID)
{
    NBB_BOOL ret = FALSE;
    unsigned char ucSlot = 0;
    
    /* 源 */
    spm_hardw_getslot(&ucSlot);    
    g_stSrcNode.module_id = TYPE_MTL_BFD;
    g_stSrcNode.frame = 1;
    g_stSrcNode.slot = (unsigned int)ucSlot;
    g_stSrcNode.cpu = 0;

    /* 目的 */
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
 函 数 名  : spm_bfd_synrec_task
 功能描述  : BFD盘间通信接收函数
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月22日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

  2.日    期   : 2013年05月28日
    作    者   : 刘娟
    修改内容   : 将MTL的初始化分离出来，提前调用
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
        ret = mtl_select(maxfd + 1, &mtl_set, NULL, NULL, NULL);     /* 阻塞式收linx包 */
        for (fd = 0; fd <= maxfd; fd++)
        {
            if(ret > 0 && MTL_FD_ISSET(fd, &mtl_set))     /* 检查linx_fd是否在这个集合rset里面, */
            {
                if (0 != mtl_prefetch_pkt_len(fd, &uiRecvLen))
                {
                    continue;   /* 进行下一次收包 */
                }

                pDataBuf = malloc(uiRecvLen);
                if (NULL == pDataBuf)
                {
                    continue;   /* 进行下一次收包 */
                }

                uiRecvLen = mtl_recv(fd, &stRemoteNode.module_id, 0, pDataBuf, uiRecvLen);
                path_id = mtl_fd_to_identity(fd);

                stRemoteNode.frame = 1;
                stRemoteNode.slot = (path_id >> 16) & 0XFF;
                stRemoteNode.cpu = path_id & 0XFF;
                pBufTmp = (NBB_BYTE *)pDataBuf; /* 第一个字节是包的类型 */

                switch(((SPM_BFD_SYN_REV_PKT_HEAD*)pDataBuf)->syn_head.ucType)
                {
                    case BFD_FWD_MSG_ID:    /* 同步ID的包 */
                    {
                        pBufTmp = pBufTmp + sizeof(SPM_BFD_SYN_REV_PKT_HEAD);          /* 偏移到净荷 */
                        uiRecvLen = uiRecvLen - sizeof(SPM_BFD_SYN_REV_PKT_HEAD);      /* 得到净荷长度 */
                        (NBB_VOID)spm_bfd_synrecv_sessionid((NBB_VOID *)pBufTmp,
                            uiRecvLen,
                            ((SPM_BFD_SYN_REV_PKT_HEAD*)pDataBuf)->syn_head.ulSrcSlot);
                        break;
                    }
                    case BFD_FWD_MSG_PCK:   /* 转发报文 */
                    {
                        pBufTmp = pBufTmp + sizeof(SPM_BFD_SYN_REV_PKT_HEAD);          /* 偏移到净荷 */
                        uiRecvLen = uiRecvLen - sizeof(SPM_BFD_SYN_REV_PKT_HEAD);      /* 得到净荷长度 */
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
 函 数 名  : spm_bfd_alamup_task
 功能描述  : BFD会话属性上报函数
 输入参数  : IN NBB_USHORT usLoopi : 对应的条目号  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月9日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_alamup_task(IN NBB_USHORT usLoopi)
{
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMainSlot = 0;/* 主用主控盘的槽位号 */
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_USHORT *pusData = NULL;
    NBB_ULONG ulDataLenth = 0;

    //spm_bfd_event(usLoopi, FALSE);

    ucMainSlot = spm_hardw_getemustate();
    if (0xff == ucMainSlot) /* 主控无盘 */
    {
        return;
    }
    
    /* 主用主控盘、业务盘上报 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 如果是主控盘，判断是否为主用 */
    if (0 == spm_hardw_getboardtype(ucLocalSlot))
    {
        if (ucMainSlot != ucLocalSlot)
        {
            g_stBfdlocal[usLoopi].bIsChangeToUp = FALSE;
            return;
        }
    }

    /* 报文组帧: 24+2(类型0001)+长度(从上报数目~尾部 2+n*12)+2(数目)+n*12 */
    pucInfoTmp = g_pucAlarmInfo + 24;
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = BFD_ALARM_TYPE;  /* 填写类型 */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    ulDataLenth = 2 + BFD_ALARM_LENTH;
    *pusData = (NBB_USHORT)ulDataLenth;  /* 长度 */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = 1;  /* 数目 */
    pucInfoTmp = pucInfoTmp + 2;
    
    spm_bfd_alarminfo_encap(pucInfoTmp, usLoopi, TRUE);

    DipcSendData(ucMainSlot, 1, 0x123457, (ulDataLenth + 4), g_pucAlarmInfo);
        
    /* 上报过后置位 */
    g_stBfdlocal[usLoopi].bIsChangeToUp = FALSE;
}

/*****************************************************************************
 函 数 名  : spm_bfd_alamdown_task
 功能描述  : BFD会话属性上报函数
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月9日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

  2.日    期   : 2015年6月4日
    作    者   : gaos
    修改内容   : 修改备用盘告警处理，添加告警标志清空
                mantis:CR8000 V1R1-ER版本中试测试 0078366
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
    
    /* 主用主控盘、业务盘上报 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 如果是主控盘，判断是否为主用,BFD报文是主备主控双份复制的 只有主用主控盘才上报*/
    if (0 == spm_hardw_getboardtype(ucLocalSlot))//非主用
    {
        if (ucMainSlot != ucLocalSlot)
        {
            /* 清空需要上报down的总告警计数 */
            g_ulStateToDownNum = 0;
            
            /* 清空每条BFD产生的需要上报告警的标记 */
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
    *pusData = BFD_ALARM_TYPE;  /* 填写类型 */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    ulDataLenth = 2 + BFD_ALARM_LENTH * g_ulStateToDownNum; 
    *pusData = (NBB_USHORT)ulDataLenth;  /* 长度 */
    pucInfoTmp = pucInfoTmp + 2;
    
    pusData = (NBB_USHORT *)pucInfoTmp;
    *pusData = g_ulStateToDownNum;  /* 数目 */
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
 函 数 名  : spm_bfd_alarminfo_encap
 功能描述  : 封装上报告警的报文
 输入参数  : IN NBB_BYTE *pucBuf : 报文指针   
             IN NBB_USHORT usLoopi:需要上报的条目号  
             IN NBB_BOOL BIsUpAlarm : 上报up or down 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_alarminfo_encap(IN NBB_BYTE *pucBuf, IN NBB_USHORT usLoopi, IN NBB_BOOL BIsUpAlarm)
{
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_ULONG *pulData = NULL;
    NBB_BYTE *pucData1 = NULL;
    NBB_USHORT usData = 0;
    NBB_ULONG  ulDIdata = 0;
    NBB_ULONG *pulData2 = NULL;

    /* 12字节内容: 4(local id)+1(state)+ 1(diag)+1(dmti，实际发送频率)+1(rmri，实际发送频率)+4(remote id)*/
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
        *pucData1 = BFD_1000MS;    /* DOWN报文固定为1秒 */
        pucInfoTmp = pucInfoTmp + 1;
    }
    
    else
    {
        pucData1 = (NBB_BYTE *)pucInfoTmp;
        /*BFD报文配置寄存器偏移0xe000+N:Bit3-0：表明本端每一条BFD报文实际发送时间间隔。默认值为1s */
        usData = spm_oamtree_s16read(BFDPAKFGPABASE, usLoopi + 0xe000);
        *pucData1 = (NBB_BYTE)(usData & 0xf);
        pucInfoTmp = pucInfoTmp + 1;

        pucData1 = (NBB_BYTE *)pucInfoTmp;
        /*接收会话状态寄存器偏移1800H+NH:条目N的实际工作周期值的比特[31：16]。（以MS 为单位） */
        usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1800);
        ulDIdata = usData;
        /*接收会话状态寄存器偏移1000H+NH:条目N的实际工作周期值的比特[15：0]。（以MS 为单位） */
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
 函 数 名  : spm_bfd_alamfail_task
 功能描述  : BFD会话属性上报函数(上报配置失败)
 输入参数  : IN NBB_ULONG ulDiscrimer : BFD key  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月1日
    作    者   : 蒋博,jiangbo
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_alamfail_task(IN NBB_ULONG ulDiscrimer)
{
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMainSlot = 0;/* 主用主控盘的槽位号 */
    NBB_BYTE *pucInfoTmp = NULL;
    NBB_ULONG ulDataLenth = 0;
    NBB_BYTE arrucDate[42];

    ucMainSlot = spm_hardw_getemustate();
    if (0xff == ucMainSlot) /* 主控无盘 */
    {
        return;
    }
    
    /* 主用主控盘、业务盘上报 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 如果是主控盘，判断是否为主用 */
    if (0 == spm_hardw_getboardtype(ucLocalSlot))
    {
        /* 备用主用主控不上报 */
        if (ucMainSlot != ucLocalSlot)
        {
            return;
        }
    }

    NBB_OS_MEMSET(arrucDate, 0, sizeof(arrucDate));

    pucInfoTmp = arrucDate;
    
    /* 报文组帧: 24+2(类型0001)+长度(从上报数目~尾部 2+n*12)+2(数目)+n*12 */
    NBB_PUT_SHORT(pucInfoTmp + 24, BFD_ALARM_TYPE); /* 填写类型 */

    ulDataLenth = 2 + BFD_ALARM_LENTH;
    NBB_PUT_SHORT(pucInfoTmp + 26, ulDataLenth);  /* 长度 */

    NBB_PUT_SHORT(pucInfoTmp + 28, 1);    /* 数目 */
    
    NBB_PUT_LONG(pucInfoTmp + 30, ulDiscrimer);   /* BFD会话ID */

    *(pucInfoTmp + 34) =  5;    /* 会话状态，5表示BFD配置失败 */

    DipcSendData(ucMainSlot, 1, 0x123457, (ulDataLenth + 4), arrucDate);
        
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd id %lu cfg fail, has reported!\r\n", ulDiscrimer);
    
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_alamfail_task
 功能描述  : 记录配置失败的BFD
 输入参数  : IN NBB_ULONG ulDiscrimer : BFD key  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年12月1日
    作    者   : 蒋博,jiangbo
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_need_alarm_fail(IN NBB_ULONG ulDiscrimer)
{
    BFD_ALARM_FAIL_INFO *current_fail_info = NULL;
    NBB_INT iRet = SUCCESS;

    current_fail_info = (BFD_ALARM_FAIL_INFO *)NBB_MM_ALLOC(sizeof(BFD_ALARM_FAIL_INFO), 
        NBB_NORETRY_ACT, MEM_SPM_BFD_ALARM_FAIL_TREE_CB);
    if (NULL == current_fail_info)/* 若为空 */
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
 函 数 名  : spm_bfd_boardcheck_task
 功能描述  : 主控判断第一块在位的NP盘是否发生了变化
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

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
    NBB_BYTE  ucSlotid = 0;        /* 当前第一块在位且cpu工作的SPU盘 */
    SPM_BFD_FWD_PACKET2 stBfdFwd2;
    SPM_BFD_FWD_PACKET3 stBfdFwd3;

    OS_MEMSET(&stBfdFwd2, 0, sizeof(SPM_BFD_FWD_PACKET2));
    OS_MEMSET(&stBfdFwd3, 0, sizeof(SPM_BFD_FWD_PACKET3));
    

    /* 主控控制部分寄存器，从高到低依次为槽位16至1SPU盘在位信息。高在位，低不在位 */
    usData1 = spm_oamtree_s16read(0x80000c, 0);
    
    /* 主控控制部分寄存器，从高到低依次为槽位16至1SPU盘在位信息。高BMU工作正常，低BMU工作不正常 */
    usData2 = spm_oamtree_s16read(0x800010, 0);
    
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 处理SPU盘拔出id问题 */
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
            if ((0 == aucBoardState[ucLoopi]) && (1 == g_astBoardState[ucLoopi].ucBoardState))/* 说明盘被拔出 */
            {
                g_astBoardState[ucLoopi].usTimeCounter = 1;
            }

            if ((0 == aucBoardBmuState[ucLoopi]) && (1 == g_astBoardState[ucLoopi].ucBoardBmuState))/* 说明盘reboot */
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
            /* 修改SRC盘所有会话发送的NP盘 */
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
                    /* BFD发送模块寄存器偏移0x12000+4*N+0x1，Bit15-0：源MAC（SMAC）地址bit48-33 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x1, usData6);
                }
            }
        }
    }
    return;
    
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_daminit
 * 负 责 人  : 李明明,limingming
 * 创建日期  : 2013年11月12日
 * 函数功能  : DMA访问方式的初始化，bfd使用的是PCIE(函数名应为spm_bfd_dmainit，原始代码就这样，不改了O(∩_∩)O~)
 * 输入参数  : NBB_VOID
 * 输出参数  : 无  
 * 返 回 值  :   0=成功，其他=失败
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_daminit(NBB_VOID)
{
    NBB_INT iRet = ERROR;
    g_stDmaReadData.channel = 0;
    g_aullDmaDataInfo[0][0] = 0xc80000000LL + (BFDSESSIONBASE + 0x800) * 4;/* 状态 */
    g_aullDmaDataInfo[0][1] = BFD_MAX_DMA_NUM * 4 / 4;
    g_aullDmaDataInfo[1][0] = 0xc80000000LL + BFDSTUDYBASE*4; /* 描述符学习 */
    g_aullDmaDataInfo[1][1] = BFD_MAX_DMA_DRIM_NUM * 4 / 4;

    g_iDevFd = open("/dev/pciedev", O_RDWR);    /* why:为什么读写方式打开，不使用只读方式"O_RDONLY"打开 */
    if (ERROR == g_iDevFd)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "OPEN %d-------------\r\n", g_iDevFd);
        return iRet;
    }
    
    iRet = Read_Dma(g_iDevFd, g_aullDmaDataInfo, 2, 5000, 0);    /* 从指定的外设读取信息 */
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "READ %d-------------\r\n", iRet);
    return iRet;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_damread
 * 负 责 人  : 李明明,limingming
 * 创建日期  : 2013年11月12日
 * 函数功能  : 通过DMA方式读取外设数据,读取后的数据存放在全局变量g_auiDmaData和g_auiDmaDrimData中
 * 输入参数  : NBB_VOID  无
 * 输出参数  : 无
 * 返 回 值  :   无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID spm_bfd_damread(NBB_VOID)
{
    NBB_INT iRet = SUCCESS;
    iRet = read(g_iDevFd,&g_stDmaReadData,BFD_MAX_DMA_NUM + BFD_MAX_DMA_DRIM_NUM);
    

    if (0 == g_stDmaReadData.error)
    {
        OS_MEMCPY(g_auiDmaData, &g_stDmaReadData.data[0], BFD_MAX_DMA_NUM * 4);         /*接收会话状态*/
        OS_MEMCPY(g_auiDmaDrimData, &g_stDmaReadData.data[BFD_MAX_DMA_NUM], BFD_MAX_DMA_DRIM_NUM * 4);/*接收报文中的MYID*/
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_checkemustate
 功能描述  : BFD主备倒换
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_checkemustate(NBB_VOID)
{
    /* 交换CPU主备倒换 */
    g_bIsModifySession = TRUE;
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "mustate is changed\r\n");
    return;
}


/*****************************************************************************
 函 数 名  : spm_bfd_intervalreport
 功能描述  : 会话上报真实工作频率
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年4月16日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

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
        /* 每一分钟查询一次是否需要上报收发频率 */
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
