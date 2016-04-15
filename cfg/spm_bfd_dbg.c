/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_bfd_dbg.c
  版 本 号   : 初稿
  作    者   : limingming
  生成日期   : 2012年9月24日
  最近修改   :
  功能描述   : BFD配置处理
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月24日
    作    者   : limingming
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include <sys/time.h>
#include "bmuInclude.h"

/* 外部变量 */

/* 调试信息打印开关 */
extern BFD_DEBUGSHOW g_enBfdDebug;

/* fwdtable 翻页写备表的次数 */
extern NBB_UINT g_uiFwdTableCount;
extern NBB_VOID hwprintfpgareg32b(IN NBB_ULONG ulOffset, IN NBB_ULONG ulNum);
#if 1
/*****************************************************************************
 函 数 名  : spm_bfd_init_dbg
 功能描述  : 调试信息初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_init_dbg(NBB_CXT_T NBB_CXT)
{
    NBB_MEMSET(g_stFwdTxRec, 0, sizeof(SPM_BFD_FWDRECORD)*SYS_TOTAL_SLOTNUM);
    NBB_MEMSET(g_stFwdRxRec, 0, sizeof(SPM_BFD_FWDRECORD)*SYS_TOTAL_SLOTNUM);
    NBB_MEMSET(g_stintervalinfo, 0, sizeof(SPM_BFD_INTERVAL_REPORT)*BFD_MAX_SESNUM);
    NBB_MEMSET(g_BIsCloseStateMachine, FALSE, sizeof(NBB_BOOL)*BFD_MAX_SESNUM);
    g_ucCpuTick = 1;
    g_BIsRecordDownEvent = FALSE;
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_init_test
 功能描述  : 调试函数，用于手动创建会话
 输入参数  : IN NBB_USHORT usStartNum: 起始条目号              
             IN NBB_USHORT usEndNum  : 终结条目号                
             IN NBB_BYTE ucType      : 会话类型 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_init_test(IN NBB_USHORT usStartNum,
                           IN NBB_USHORT usEndNum,
                           IN NBB_BYTE ucType,
                           IN NBB_BYTE ucFwdtype,
                           IN NBB_BYTE ucFlag)
{
    NBB_USHORT usLoopi = 0;

    /* 检查参数 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        return;
    }
    
    /* 检查参数 */
    if ((ucType >= BFD_PCK_BUTT) || (ucFwdtype >= BFD_FWD_BUTT))
    {
        return;
    }
    
    NBB_PRINTF("process bfd init\n ");

    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)/* 初始化 */
    {
        g_stBfdlocal[usLoopi].ulSip[3] = 0x0c010101 + usLoopi;
        g_stBfdlocal[usLoopi].ulDip[3] = 0x0c010102 + usLoopi;
        g_stBfdlocal[usLoopi].ucTos = 2;
        g_stBfdlocal[usLoopi].usTotallength = 52;
        g_stBfdlocal[usLoopi].usSport = 5000;
        g_stBfdlocal[usLoopi].usUdplength = 32;
        g_stBfdlocal[usLoopi].ucSessionState = BFD_DOWN;
        g_stBfdlocal[usLoopi].ucLocalDiag = bfd_diag_no_diag;
        g_stBfdlocal[usLoopi].ucLength = 24;
        g_stBfdlocal[usLoopi].ucDetectMult = 3;
        g_stBfdlocal[usLoopi].ulLocalDiscr = 156 + usLoopi;
        g_stBfdlocal[usLoopi].ulDiscrimer = g_stBfdlocal[usLoopi].ulLocalDiscr;
        if (1 == ucFlag)
        {
            g_stBfdlocal[usLoopi].ulRemoteDiscr = 156 + usLoopi + ((usLoopi % 2) ? (-1) : 1);
        }
        else
        {
            g_stBfdlocal[usLoopi].ulRemoteDiscr = 156 + usLoopi;
        }
        g_stBfdlocal[usLoopi].ucDesiredMinTxInterval = BFD_1000MS;
        g_stBfdlocal[usLoopi].ucRequiredMinRxInterval = BFD_1000MS;
        g_stBfdlocal[usLoopi].ucDM = 30;
        g_stBfdlocal[usLoopi].ucDMTI = BFD_80MS;
        g_stBfdlocal[usLoopi].ucRMRI = BFD_80MS;
        g_stBfdlocal[usLoopi].ucDetectiontype = 0;
        g_stBfdlocal[usLoopi].ucBfdtype = ucType;
        g_stBfdlocal[usLoopi].ulLspLabel = 12345 + 5 * usLoopi;
        g_stBfdlocal[usLoopi].ulPwLabel  = 12345 + 5 * usLoopi;
        #ifdef R8000
        g_stBfdlocal[usLoopi].usLspID  = 1;
        #endif
        g_stBfdlocal[usLoopi].usTunnelID = 1;
        g_stBfdlocal[usLoopi].usPortID = 12;
        g_stBfdlocal[usLoopi].bIsLag = FALSE;
        g_stBfdlocal[usLoopi].usUnit = 0;
        g_stBfdlocal[usLoopi].usSVlanID = 2;

        g_stBfdlocal[usLoopi].ucBfdFwdType = ucFwdtype;

        g_stBfdlocal[usLoopi].ucSendflag = TRUE;

        g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;

//        g_stBfdlocal[usLoopi].usTimerPortDownToUp = 100;
//        g_stBfdlocal[usLoopi].usTimerPortUPToDown= 100;

        spm_bfd_add_test(usLoopi);
        g_bIsModifySession = TRUE;
        
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_initipv6_test
 功能描述  : 调试函数，用于手动创建会话
 输入参数  : IN NBB_USHORT usStartNum: 起始条目号              
             IN NBB_USHORT usEndNum  : 终结条目号                
             IN NBB_BYTE ucType      : 会话类型 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_initipv6_test(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum, IN NBB_BYTE ucType NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;

    /* 检查参数 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        return;
    }
    
    /* 检查参数 */
    if (ucType >= BFD_PCK_BUTT)
    {
        return;
    }
    
    NBB_PRINTF("process bfd init\n ");

    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)/* 初始化 */
    {
        g_stBfdlocal[usLoopi].ulSip[0] = 0xfe800000;
        g_stBfdlocal[usLoopi].ulSip[1] = 0;
        g_stBfdlocal[usLoopi].ulSip[2] = 0;
        g_stBfdlocal[usLoopi].ulSip[3] = 35 + usLoopi%2 + 2 * usLoopi;
        g_stBfdlocal[usLoopi].ulDip[0] = 0xfe800000;
        g_stBfdlocal[usLoopi].ulDip[1] = 0;
        g_stBfdlocal[usLoopi].ulDip[2] = 0;
        g_stBfdlocal[usLoopi].ulDip[3] = 35 + (usLoopi+1)%2 + 2*usLoopi;
        g_stBfdlocal[usLoopi].usTotallength = 52;
        g_stBfdlocal[usLoopi].usSport = 5000;
        g_stBfdlocal[usLoopi].usUdplength = 32;
        g_stBfdlocal[usLoopi].ucSessionState = BFD_DOWN;
        g_stBfdlocal[usLoopi].ucLocalDiag = bfd_diag_no_diag;
        g_stBfdlocal[usLoopi].ucLength = 24;
        g_stBfdlocal[usLoopi].ucDetectMult = 3;
        g_stBfdlocal[usLoopi].ulLocalDiscr = 156 + 17 * usLoopi;
        g_stBfdlocal[usLoopi].ulDiscrimer = g_stBfdlocal[usLoopi].ulLocalDiscr;
        g_stBfdlocal[usLoopi].ulRemoteDiscr = 156 + 17 * usLoopi;
        g_stBfdlocal[usLoopi].ucDesiredMinTxInterval = BFD_1000MS;
        g_stBfdlocal[usLoopi].ucRequiredMinRxInterval = BFD_1000MS;
        g_stBfdlocal[usLoopi].ucDM = 30;
        g_stBfdlocal[usLoopi].ucDMTI = BFD_10MS;
        g_stBfdlocal[usLoopi].ucRMRI = BFD_10MS;
        g_stBfdlocal[usLoopi].ucDetectiontype = 0;
        g_stBfdlocal[usLoopi].ucBfdtype = ucType;
        g_stBfdlocal[usLoopi].ulLspLabel = 12345 + 5 * usLoopi;
        g_stBfdlocal[usLoopi].ulPwLabel  = 12345 + 5 * usLoopi;
        g_stBfdlocal[usLoopi].usLspID  = 0;
        g_stBfdlocal[usLoopi].usPortID = 12;
        g_stBfdlocal[usLoopi].bIsLag = FALSE;
        g_stBfdlocal[usLoopi].usUnit = 1;
        g_stBfdlocal[usLoopi].usSVlanID = 2;
        g_stBfdlocal[usLoopi].ucIptype = 1;

        g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_IP;

        g_stBfdlocal[usLoopi].ucSendflag = TRUE;

        g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;

        spm_bfd_add_test(usLoopi);
        g_bIsModifySession = TRUE;
        
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_initfwd_test
 功能描述  : 调试用于手动建立转发表项
 输入参数  : IN NBB_USHORT usStartNum                
             IN NBB_USHORT usEndNum                  
             NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年12月11日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_initfwd_test(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum, IN NBB_USHORT usSlotid NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_ULONG  ulDiscrimer = 0;

    /* 检查参数 */
    if ((usEndNum > BFD_MAX_FWDNUM) || (usStartNum > BFD_MAX_FWDNUM))
    {
        return;
    }
    
    NBB_PRINTF("process bfdfwd init\n ");
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)/* 初始化 */
    {
        ulDiscrimer = 156 + usLoopi;
        spm_bfd_addfwdtable(ulDiscrimer, usSlotid);
    }
    return;
}
NBB_VOID spm_bfd_sendpacket_fpgareg_test(IN NBB_USHORT usLocalNum, IN NBB_BOOL bIsModify)
{
    NBB_USHORT   usData = 0;
    NBB_ULONG    ulLsplabel = 0;
    NBB_ULONG    ulPwlabel = 0;
    SPM_BFD_REG_PACKET1 stPacketInfo1;
    SPM_BFD_MPLSLABEL   stMplsLabel1;
    SPM_BFD_MPLSLABEL   stMplsLabel2;
        
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        return;
    }
    
    OS_MEMSET(&stPacketInfo1, 0, sizeof(SPM_BFD_REG_PACKET1));
    OS_MEMSET(&stMplsLabel1, 0, sizeof(SPM_BFD_MPLSLABEL));
    OS_MEMSET(&stMplsLabel2, 0, sizeof(SPM_BFD_MPLSLABEL));

    /* DMAC+SMAC共12字节，封装走转发平面，格式为reserved(1byte)+SVLAN(2bytes)+LSPID(2bytes)+ reserved(5bits)+FWDTYPE(3bits)
        + vrfid(2byte)+ipv4address(4bytes), 主控盘上的BFD需要先发到一块业务盘算，然后走转发平面，因此主控的格式
        为 ITMH(4bytes)+PKT_TYPE(3bits)+Dslotid(5bits)+Sslotid(5bits)+FWDTYPE(3bits)+vrfid(2byte)+
        ipv4address(4bytes) */
    
    spm_bfd_sendpacket_macreg(usLocalNum);

    spm_bfd_sendpacket_intervalreg(usLocalNum, bIsModify);

    /* 报文内容 */
    if (BFD_IPV4 == g_stBfdlocal[usLocalNum].ucIptype)
    {
        spm_bfd_sendpacket_ipv4fpga(usLocalNum);
    }
    else /* ipv6 */
    {
        spm_bfd_sendpacket_ipv6fpga(usLocalNum);
    }
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x8000 + usLocalNum, g_stBfdlocal[usLocalNum].usSport);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x9000 + usLocalNum, g_stBfdlocal[usLocalNum].usUdplength);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xa000 + usLocalNum, g_stBfdlocal[usLocalNum].ucLength);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xb000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulLocalDiscr >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xb000 + 2 * usLocalNum + 0x1, 
        (g_stBfdlocal[usLocalNum].ulLocalDiscr)&DRV_USHORT_INVALID);
    /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
    /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);

    if (BFD_LSP == g_stBfdlocal[usLocalNum].ucBfdtype)/* lsp */
    {
        stMplsLabel1.ulMplsLabel = g_stBfdlocal[usLocalNum].ulLspLabel;
        stMplsLabel1.ulStack = 1;
        stMplsLabel1.ulExp = BFD_DEFAULT_MPLS_EXP;
        stMplsLabel1.ulTtl = BFD_DEFAULT_MPLS_TTL;
        ulLsplabel = *(NBB_ULONG *)(&stMplsLabel1);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x0,
            (ulLsplabel >> 16)&DRV_USHORT_INVALID);
        
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x1,
            ulLsplabel&DRV_USHORT_INVALID);
    }

    /* pw */
    else if ((BFD_PW_type1 == g_stBfdlocal[usLocalNum].ucBfdtype)
        || (BFD_PW_type2 == g_stBfdlocal[usLocalNum].ucBfdtype))
    {
        stMplsLabel1.ulMplsLabel = g_stBfdlocal[usLocalNum].ulLspLabel;
        stMplsLabel1.ulStack = 0;
        stMplsLabel1.ulExp = BFD_DEFAULT_MPLS_EXP;
        stMplsLabel1.ulTtl = BFD_DEFAULT_MPLS_TTL;
        ulLsplabel = *(NBB_ULONG *)(&stMplsLabel1);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x0,
            (ulLsplabel >> 16)&DRV_USHORT_INVALID);
        
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x1,
            ulLsplabel&DRV_USHORT_INVALID);

        stMplsLabel2.ulMplsLabel = g_stBfdlocal[usLocalNum].ulPwLabel;
        stMplsLabel2.ulStack = 1;
        stMplsLabel2.ulExp = BFD_DEFAULT_MPLS_EXP;
        stMplsLabel2.ulTtl = BFD_DEFAULT_MPLS_TTL;
        ulPwlabel = *(NBB_ULONG *)(&stMplsLabel2);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x0,
            (ulPwlabel >> 16)&DRV_USHORT_INVALID);
        
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x1,
            ulPwlabel&DRV_USHORT_INVALID);
        
    }
    
    stPacketInfo1.usSendFlag = g_stBfdlocal[usLocalNum].ucSendflag;

    /* UDP端口号 */
    stPacketInfo1.usUDPtype = g_stBfdlocal[usLocalNum].ucDetectiontype;

    /* 判断封装类型 */
    stPacketInfo1.usSendType = g_stBfdlocal[usLocalNum].ucBfdtype;
    stPacketInfo1.usVlanAble = TRUE;
    if (BFD_IPV6 == g_stBfdlocal[usLocalNum].ucIptype)
    {
        stPacketInfo1.usIpType = TRUE;
    }
    usData = *(NBB_USHORT *)(&stPacketInfo1);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLocalNum, usData);
        
    return;
    
}

NBB_VOID spm_bfd_add_test(NBB_USHORT usLoopi)
{
    SPM_OAMTREE_NODE stBfdTreeNode;
    NBB_INT iRet = ERROR;

    iRet = spm_bfd_tree_set_data(usLoopi, g_stBfdlocal[usLoopi].ulLocalDiscr, &stBfdTreeNode);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //BFD_LOCAL_INIT(usLoopi);
        //NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }

    /* 加节点 */
    iRet = spm_oamtree_nodeadd(g_usBfdLspNoMax, &stBfdTreeNode, &g_stTreeBfdNodeSet);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //BFD_LOCAL_INIT(usLoopi);
        //NBB_TRC_FLOW((NBB_FORMAT"Add node failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }
    
    spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
    
    /* 建立会话二叉树 */
    iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, g_pulBfdTreebase, BFDFGPABASE);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //BFD_LOCAL_INIT(usLoopi);
        //NBB_TRC_FLOW((NBB_FORMAT"Build main Tree failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }

    /* 二叉树建表后要翻页 */
    spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET,1);

    spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
    
    /* 写备表 */
    iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
        g_pulBfdTreebase, BFDFGPABASE);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //BFD_LOCAL_INIT(usLoopi);
        //NBB_TRC_FLOW((NBB_FORMAT"Build back Tree failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }
    spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET,1);
    g_stBfdlocal[usLoopi].bIsSendtoMaster = FALSE;
    g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;
    spm_bfd_sendpacket_fpgareg_test(usLoopi, FALSE);

    EXIT_LABEL :
    return;
}


/*****************************************************************************
 函 数 名  : spm_bfd_addloop_test
 功能描述  : 调试函数，usStartNum~usEndNum条目的BFD写入到FPGA
 输入参数  : IN NBB_USHORT usStartNum, 
                IN NBB_USHORT usEndNum    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月18日
    作    者   : tianf
    修改内容   : 新生成函数
*****************************************************************************/
NBB_VOID spm_bfd_addloop_test(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    
    /* 检查参数 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        return;
    }

    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)/* 初始化 */
    {
        if (DRV_ULONG_INVALID != g_stBfdlocal[usLoopi].ulLocalDiscr)
        {
            spm_bfd_add_test(usLoopi);
        }
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_cpu_work
 功能描述  : ..
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 加函数头而已

*****************************************************************************/
NBB_VOID spm_bfd_delete_test(NBB_USHORT usLoopi)
{
    SPM_OAMTREE_NODE stBfdTreeNode;
    NBB_INT iRet = ERROR;
    
    iRet = spm_bfd_tree_set_data(usLoopi, g_stBfdlocal[usLoopi].ulLocalDiscr, &stBfdTreeNode);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }
    
    /* 删除节点 */
    iRet = spm_oamtree_nodedel(g_usBfdLspNoMax, &stBfdTreeNode, &g_stTreeBfdNodeSet);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }
    
    spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
    
    /* 重建会话二叉树 */
    iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
        g_pulBfdTreebase, BFDFGPABASE);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }
    
    /* 二叉树建表后要翻页 */
    spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET,1);

    spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
    
    /* 重建会话二叉树 */
    iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
        g_pulBfdTreebase, BFDFGPABASE);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        //NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", g_stBfdlocal[usLoopi].ulLocalDiscr));
        goto EXIT_LABEL;
    }
    g_stBfdlocal[usLoopi].ucbIsNeedDelete = TRUE;
    
    EXIT_LABEL : 
    return;
}

NBB_VOID spm_bfd_deleteloop_test(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    
    /* 检查参数 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        return;
    }

    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)/* 初始化 */
    {
        if (DRV_ULONG_INVALID != g_stBfdlocal[usLoopi].ulLocalDiscr)
        {
            spm_bfd_delete_test(usLoopi);
        }
    }
    return;
}

NBB_VOID spm_bfd_init_nni_test(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_ULONG  ulDiscrimer = 0;
    NBB_ULONG  ulPeerIp = 0;
    NBB_ULONG  ulPwLabel = 0;
    NBB_ULONG  ulNniId = 0;

    /* 检查参数 */
    if ((usEndNum > BFD_MAX_FWDNUM) || (usStartNum > BFD_MAX_FWDNUM))
    {
        return;
    }
    
    NBB_PRINTF("process bfd nni init\n ");
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)/* 初始化 */
    {
        ulDiscrimer = 156 + usLoopi;
        ulPeerIp    = 0x0c010101 + usLoopi;
        ulPwLabel   = 156 + usLoopi;
        ulNniId     = 256 + usLoopi;
        spm_bfd_add_nniId_table(ulDiscrimer, ulPeerIp, ulPwLabel, ulNniId NBB_CCXT);
    }
    return;
}


NBB_VOID spm_bfd_cpu_sleep(NBB_VOID)
{
    g_ucCpuTick = 1;
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_cpu_work
 功能描述  : ..
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 加函数头而已

*****************************************************************************/
NBB_VOID spm_bfd_cpu_work(NBB_VOID)
{
    g_ucCpuTick = 0;
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "cfg end!!!\r\n");
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_set_send_switch
 功能描述  : 调试函数，关闭或开启ulBfdId的发送使能开关
 输入参数  : IN NBB_ULONG ulBfdId
                IN NBB_BYTE ucEnable        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月18日
    作    者   : tianf
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT spm_bfd_set_send_switch(IN NBB_ULONG ulBfdId, IN NBB_BYTE ucEnable)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usData = 0;

    if((ucEnable != 0) && (ucEnable != 1))
    {
        NBB_PRINTF("InPut ucEnable must be 0 or 1!\n");
        return -1;
    }
    
    /* 循环输出内容 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if(g_stBfdlocal[usLoopi].ulDiscrimer == ulBfdId)
        {
            if(0 == ucEnable) /*发送去使能*/
            {
                usData = spm_oamtree_s16read(BFDPAKFGPABASE, 0x1000 + usLoopi);
                usData = (usData << 1) >> 1;
                spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLoopi, usData);
            }
            else /*发送使能*/
            {
                usData = spm_oamtree_s16read(BFDPAKFGPABASE, 0x1000 + usLoopi);
                usData = usData | 0x8000;
                spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLoopi, usData);
            }
        }
    }
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_bfd_set_send_master_switch
 功能描述  : 调试函数，关闭或开启总的发送使能开关 仅SPU 有效
 输入参数  : IN NBB_BYTE ucEnable  0/1--去使能/使能      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月18日
    作    者   : tianf
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT spm_bfd_set_send_master_switch(IN NBB_BYTE ucEnable)
{
    NBB_USHORT usData = 0;

    if((ucEnable != 0) && (ucEnable != 1))
    {
        NBB_PRINTF("InPut ucEnable must be 0 or 1!\n");
        return -1;
    }

    ucEnable = ucEnable ^ 0x1; /*寄存器0/1--使能/去使能*/
    if(0 == ucEnable) /*使能*/
    {
        
        usData = spm_oamtree_s16read(BFDPAKFGPABASE, 0x3);
        usData = (usData << 1) >> 1;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x3, usData);
    }
    else /*去使能*/
    {
        
        usData = spm_oamtree_s16read(BFDPAKFGPABASE, 0x3);
        usData = usData | 0x8000;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x3, usData);
    }
    return 0;
    
}

/*****************************************************************************
 函 数 名  : bfddbgaddfwdtable
 功能描述  : 添加转发表自测函数
 输入参数  : NBB_USHORT usNum     
             NBB_USHORT usSlotid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月9日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT bfddbgaddfwdtable(NBB_USHORT usNum, NBB_USHORT usSlotid)
{
    NBB_ULONG ulLoop = 0;
    NBB_INT iRv = 0;
    
    if((usNum > 2048) || (usSlotid > 12) || (0 == usSlotid))
    {
       return ERROR; 
    }
            
    for(ulLoop = 0; ulLoop < usNum; ulLoop++)
    {
       iRv += spm_bfd_addfwdtable(ulLoop + 1, usSlotid);
       
       //BmuTaskDelay(1); 
    }

    return iRv;
}

/*****************************************************************************
 函 数 名  : bfddbgdelfwdtable
 功能描述  : 删除转发表条目自测函数，该函数配合添加转发表函数使用
 输入参数  : NBB_USHORT usNum     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月9日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT bfddbgdelfwdtable(NBB_USHORT usNum)
{
    NBB_ULONG ulLoop = 0;
    NBB_INT iRv = 0;
    
    if(usNum > 2048)
    {
       return ERROR; 
    }
            
    for(ulLoop = 0; ulLoop < usNum; ulLoop++)
    {
       iRv += spm_bfd_delfwdtable(ulLoop + 1);
       
       //BmuTaskDelay(1); 
    }

    return iRv;
}

/*****************************************************************************
 函 数 名  : bfddbgfindfwd
 功能描述  : 根据描述符查找全局转发表
 输入参数  : NBB_ULONG ulDiscrimer  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年6月19日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfddbgfindfwd(NBB_ULONG ulDiscrimer)
{
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    
    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulDiscrimer);
    if(NULL != pstFwdInfo)
    {
        printf("Discrimer:%lu, main slotid:%d!\n",pstFwdInfo->ulDiscrimer, pstFwdInfo->usSlotid);
    }
    else
    {
        printf("Not exist!!\n");
        
    }  
    
}

/*****************************************************************************
 函 数 名  : bfddbgfindlocalfwd
 功能描述  : 打印输出本地转发表
 输入参数  : NBB_USHORT usStartNum  
             NBB_USHORT usEndNum    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月4日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfddbgfindlocalfwd(NBB_USHORT usStartNum, NBB_USHORT usEndNum)
{
    NBB_USHORT usLoopi = 0;
    
    /* 入参判断 */
    if ((usEndNum > BFD_MAX_FWDNUM) || (usStartNum > BFD_MAX_FWDNUM))
    {
        NBB_PRINTF("Please check input\n");
        return;
    } 
    NBB_PRINTF("Loop |  Slotid  |   MyDrim  |\r\n"); 
    
    /* 循环输出 */
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
    {
        NBB_PRINTF("%d  | %d  |  %lu |\n",usLoopi, g_stBfdfwd[usLoopi].usSlotid, g_stBfdfwd[usLoopi].ulDiscrimer);
    }
    return;
}

/*****************************************************************************
 函 数 名  : bfddbgfindip
 功能描述  : 根据ipv4key值查找全局会话二叉树
 输入参数  : NBB_ULONG ulsip     
             NBB_ULONG ulDip     
             NBB_USHORT usVrfid  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年6月19日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfddbgfindip(NBB_ULONG ulsip, NBB_ULONG ulDip, NBB_USHORT usVrfid)
{
    SPM_BFDGLB_IP_KEY stBfdGlbKey;
    SPM_BFD_IP_GLOBAL       *pstBfdIpCfg = NULL;
    
    memset(&stBfdGlbKey, 0, sizeof(stBfdGlbKey));
    
    /* 根据VRFID+DIP+SIP查询全局会话表项 */
    stBfdGlbKey.ulSip = ulsip;
    stBfdGlbKey.ulDip = ulDip;
    stBfdGlbKey.usVrfid = usVrfid;
    pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIp, &stBfdGlbKey);
    if(NULL != pstBfdIpCfg)
    {
        printf("myid: %lu,main slotid: %d \n", pstBfdIpCfg->ulDiscrimer, pstBfdIpCfg->usSlotid);
    }
    else
    {
        printf("Not exist!!\n");
        
    }
}

/*****************************************************************************
 函 数 名  : bfdshowFwdTableCount
 功能描述  : 查看翻页写转发的备表次数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月13日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfdshowFwdTableCount()
{
    printf("count :%d", g_uiFwdTableCount);
}

/*****************************************************************************
 函 数 名  : bfddebugshow
 功能描述  : 调试打印函数
 输入参数  : NBB_INT ucdebuglayer 
            0:close
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年6月18日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfddebugshow(NBB_INT ucdebuglayer)
{    
    switch (ucdebuglayer)
    {
        case 0:
            g_enBfdDebug = BFD_DEBUG_SHOWCLOSE;
            printf("bfddebugshow close!\n");
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfddebugshow close!\n");            
            break;        
        case 1:
            g_enBfdDebug = BFD_DEBUG_SHOWPACKETRECV;
            printf("bfddebugshow open! layer = %d!\n",ucdebuglayer);
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfddebugshow open! layer = %d!\n",ucdebuglayer);              
            break;
        default:
            printf("debug show:\n");
            printf("0/1/.. :\tclose/packet receive/..\n");
            return;

    }

//    NBB_BYTE pBuf[98]={
//0x00, 0x0A, 0xC2 ,0x00, 0x10 ,0x04, 0x00, 0x0A ,0xC2, 0x00 ,0x01, 0x01, 0x98, 0x76, 0x00, 0x00,
//0x00 ,0x21, 0xA0, 0x00 ,0x00, 0x00 ,0x03 ,0x00, 0x00 ,0x00, 0x00 ,0x00,
//0x34, 0x4B, 0x3D ,0xE7, 0x6C ,0x4D, 0x34 ,0x4B, 0x3D ,0xE8, 0xBA ,0x06, 0x81 ,0x00, 0x00 ,0x1F,
//0x08 ,0x00 ,0x45 ,0xF0 ,0x00 ,0x34, 0x00 ,0x00 ,0x00 ,0x00 ,0xFF ,0x11 ,0x66 ,0x39 ,0x50 ,0x06,
//0x01 ,0x01, 0x03 ,0x64 ,0x00 ,0x25, 0xC0 ,0x00 ,0x0E ,0xC8 ,0x00 ,0x20 ,0x00 ,0x00 ,0x20 ,0x40,
//0x03 ,0x18, 0x00 ,0x00 ,0x19 ,0x67 ,0x00, 0x00 ,0x00, 0x00 ,0x00, 0x0F ,0x42, 0x40 ,0x00, 0x0F,
//0x42 ,0x40, 0x00, 0x00 ,0x00, 0x00        
//        };
//    spm_bfd_packet_receive(pBuf, 28+70);

}


/*****************************************************************************
 函 数 名  : bfddebugshow
 功能描述  : 调试打印函数
 输入参数  : NBB_INT ucdebuglayer 
            0:close
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年6月18日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_process_test(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usData  = 0; 
    NBB_BYTE   ucSessionSta = 0;
    NBB_BOOL bIsTimeExpired = FALSE;
    NBB_USHORT  usRemoteDisc = 0;
    NBB_USHORT  usRemoteDisctmph = 0;
    NBB_USHORT  usRemoteDisctmpl = 0;
    
    /* 取接收会话状态寄存器中的内容 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        /* 学习对端描述符 */
        if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
        {
            if ((BFD_CFG_LSP == g_stBfdlocal[usLoopi].pstBasicBfdCB.bfd_frame_type) 
                || (BFD_CFG_Tunnel == g_stBfdlocal[usLoopi].pstBasicBfdCB.bfd_frame_type))
            {
                if (g_stBfdlocal[usLoopi].ulRemoteDiscr == 0)
                {
                    usRemoteDisctmpl = spm_oamtree_s16read(BFDSTUDYBASE, usLoopi * 2 + 0x0);
                    usRemoteDisctmph = spm_oamtree_s16read(BFDSTUDYBASE, usLoopi * 2 + 0x1);
                    usRemoteDisc = (usRemoteDisctmph << 16) + usRemoteDisctmpl;

                    if (0 != usRemoteDisc)/* 描述符有效 */
                    {
                        g_stBfdlocal[usLoopi].ulRemoteDiscr = usRemoteDisc;
                        
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                            (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                            (g_stBfdlocal[usLoopi].ulRemoteDiscr)&DRV_USHORT_INVALID);
                        g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                    }
                }
            }
        }

        /* 会话正常 */
        if (BFD_NORMAL == g_stBfdlocal[usLoopi].usBfdflag)
        {
            usData = spm_oamtree_s16read(BFDSESSIONBASE, 0x800 + usLoopi);
            
            if (BFD_ADMINDOWN == g_stBfdlocal[usLoopi].ucSessionState)
            {
                bIsTimeExpired = FALSE;
            }
            else
            {
                bIsTimeExpired = ((usData >> 15) & 0x1);
            }
            
            /* 判断是否超时 */
            if (TRUE == bIsTimeExpired)
            {
                g_stBfdlocal[usLoopi].ucLocalDiag = bfd_diag_ctrl_detect_exp;
                spm_bfd_state_fsm_down(usLoopi,BFD_STA_BUTT);
            }
            
            else/* 解析收到对端的报文的stat */
            {
                /* 判断是否是新收到的报文的stat */                
                if (g_aucCouterRecord[usLoopi] != (NBB_BYTE)(usData & 0x3))
                {
                    ucSessionSta = (NBB_BYTE)((usData >> 7) & 0x3);
                    spm_bfd_state_fsm(usLoopi, ucSessionSta);
                    g_aucCouterRecord[usLoopi] = (NBB_BYTE)(usData & 0x3);
                }
            }
        }
        
        if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* 若需要删除 */
        {
            /* 将会话信息至为无效值 */
            //BFD_LOCAL_INIT(usLoopi);
            g_bIsModifySession = TRUE;
            
            /* 清空FPGA对应的发送寄存器内容 */
            spm_bfd_clear_sendfpgareg(usLoopi);
        }
    }
    return;
}

/*****************************************************************************
 * 函 数 名  : bfdhelp
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月27日
 * 函数功能  : BFD 调试函数
 * 输入参数  : NBB_VOID  无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfdhelp(NBB_VOID)
{
    NBB_CHAR **ppcMsg;
    
    static NBB_CHAR *pHelpMsg[] = {
        "---------------------------------------------------------","",
        "show_bfd_mainboard(Discrim)",    "show session main slot id",
        "show_bfd_session_info(Discrim)",          "show session info",
        "show_bfd_state(startnum, endnum)",      "show session state",
        "show_bfd_counter(startnum, endnum)",   "show session counter",
        "show_bfd_event(startnum, endnum)",   "show session event",
        "show_bfd_recource",                   "show total num",
        "show_bfd_fpga_drop_counter()",             "show drop counter of fpga RxBfdPacket",
        "showbfdid",                         "src show id info",
        "showbfdnpboard",                    "src show first normal slotid",
        "showbfdpckrxtx",                    "show bfd fwd packetinfo",
        "showbfdpckrxtxclear",               "clear bfd fwd packetinfo",
        "clear_bfd_event",                     "clear bfd event info",
        "bfd_show_hdlc_bfd_recrd(StartMid, EndMid)",    "show hdlc record state with myid from StartMid to EndMid",
        "show_single_bfd_state(ulBfdId)",             "show session state of single bfd",
        "show_single_bfd_counter(ulBfdId)",           "show session counter of single bfd",
        "show_single_bfd_event(ulBfdId)",             "show session event of single bfd",
        "---------------------------------------------------------","",
        "bfd_dbg_help","debug info",
        NULL };
    
    printf ("\n");

    for (ppcMsg = pHelpMsg; *ppcMsg != NULL; ppcMsg += 2) /* 打印格式 */
    {
        if(strlen(*ppcMsg)>45)/* 判断打印长度 */
        {
            printf (" %s %s\n", *ppcMsg, *(ppcMsg + 1));
        }
        else /* 判断打印长度 */
        {
            printf (" %-40s %s\n", *ppcMsg, *(ppcMsg + 1)); 
        }
    }

    printf ("\n");
}

NBB_VOID bfdfpgahelp(NBB_VOID)
{
    NBB_CHAR **ppcMsg;
    
    static NBB_CHAR *pHelpMsg[] = {
        "---------------------------------------------------------","",
        "setspugeloop(ucType)",    "set spu ge(1/2/3) loop",
        "setsrcgeloop(ucType)",    "set src ge(1/2/3) loop",
        "bfdwritefpga(Addr, OffAddr, Data, Offdate, Num)", "write fpga in a row",
        "bfdwritefpga32b(Addr, OffAddr, Data, Offdate, Num)", "write fpga 32 bit in a row",
#if defined(SPU)
        "show_bfd_write_bus_info(Num)",                "show information of BUS 0x910000",
        "show_bfd_read_bus_info(usK,Num)",             "show information of BUS 0x920000",
#elif defined(SRC)
        "show_bfd_fpga_rxbus_info(SlotId,Num)", "show bfd RX_BUS(0x920000) information",
        "show_bfd_fpga_bus_map_table(SlotId,Num)", "show bfd BUS map table(0x930000) information",
#endif
        "---------------------------------------------------------","",
        NULL };
    
    printf ("\n");

    for (ppcMsg = pHelpMsg; *ppcMsg != NULL; ppcMsg += 2) /* 打印格式 */
    {
        if(strlen(*ppcMsg)>45)/* 判断打印长度 */
        {
            printf (" %s %s\n", *ppcMsg, *(ppcMsg + 1));
        }
        else /* 判断打印长度 */
        {
            printf (" %-40s %s\n", *ppcMsg, *(ppcMsg + 1)); 
        }
    }

    printf ("\n");
}

/*****************************************************************************
 * 函 数 名  : bfd_dbg_help
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月27日
 * 函数功能  : BFD 调试函数
 * 输入参数  : NBB_VOID  无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfd_dbg_help(NBB_VOID)
{
    NBB_CHAR **ppcMsg;
    
    static NBB_CHAR *pHelpMsg[] = {
        "---------------------------------------------------------","",
        "spm_bfd_init_test(StartNum, usEndNum, ucType, ucFwdtype)",    "create bfd from StartNum to usEndNum",
        "spm_bfd_deleteloop_test(StartNum, EndNum)",                   "delete bfd from StartNum to usEndNum",
        "setStateMachineSwitch(bSwitch, usstartnum, usendnum)",        "switch 0 is close, 1 is open",
        "showStateMachineSwitch(usloopi)",                             "show item machine state",
        "showStateMachineSwitchall",                                   "show all item machine state",
        "bfddebugshow(ucdebuglayer)",                                  "debugshow,ucdebuglayer=0/1/..",
        "bfddbgfindip(ulsip,ulDip,usVrfid)",                           "show ipkey in tree(only for dynamic)",
        "bfddbgfindfwd(ulDiscrimer)",                                  "show dicrimer in gloabal fwd tree",
        "bfddbgfindlocalfwd(StartNum, EndNum)",                        "show local fwd table",        
        "bfdshowFwdTableCount",                                        "show backup table count ",
        "---------------------------------------------------------",   "",
        "bfd_dbgtest_help",                                            " ",
        
        NULL };
    
    printf ("\n");

    for (ppcMsg = pHelpMsg; *ppcMsg != NULL; ppcMsg += 2) /* 打印格式 */
    {
        if(strlen(*ppcMsg)>60)/* 判断打印长度 */
        {
            printf (" %s %s\n", *ppcMsg, *(ppcMsg + 1));
        }
        else /* 判断打印长度 */
        {
            printf (" %-55s %s\n", *ppcMsg, *(ppcMsg + 1)); 
        }
    }

    printf ("\n");
}

/*****************************************************************************
 函 数 名  : bfd_dbgtest_help
 功能描述  : 自测调试函数
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月20日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfd_dbgtest_help(NBB_VOID)
{
    NBB_CHAR **ppcMsg;
    
    static NBB_CHAR *pHelpMsg[] = {
        "---------------------------------------------------------","",       
        "bfddbgaddfwdtable(usNum,usSlotid)",                           "addfwdtable test",
        "bfddbgdelfwdtable(usNum)",                                    "delfwdtable test",
        "bfdTxEnable(StartNum，EndNum，en)",                         "en:0/1=disable/enable",
        "---------------------------------------------------------","",
        
        NULL };
    
    printf ("\n");

    for (ppcMsg = pHelpMsg; *ppcMsg != NULL; ppcMsg += 2) /* 打印格式 */
    {
        if(strlen(*ppcMsg)>60)/* 判断打印长度 */
        {
            printf (" %s %s\n", *ppcMsg, *(ppcMsg + 1));
        }
        else /* 判断打印长度 */
        {
            printf (" %-55s %s\n", *ppcMsg, *(ppcMsg + 1)); 
        }
    }

    printf ("\n");
}


/*****************************************************************************
 函 数 名  : bfdTxEnable
 功能描述  : 调试函数，关闭或开启每条bfd的发送使能
 输入参数  : NBB_SHORT usStartNum  
             NBB_SHORT usEndNum    
             NBB_BYTE ucEn         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月6日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfdTxEnable(NBB_SHORT usStartNum, NBB_SHORT usEndNum, NBB_BYTE ucEn)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usData = 0;
    
    /* 入参判断 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        NBB_PRINTF("Please check input\n");
        return;
    }

    /* 关闭BFD发送使能 */
    if(0 == ucEn)
    {
        for(usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
        {
            usData = spm_oamtree_s16read(BFDPAKFGPABASE, 0x1000 + usLoopi);

            /* bit15置0，其余位保留 */
            usData &= 0x7fff;
            spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLoopi, usData);
        }
    }
    else
    {
        for(usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
        {
            usData = spm_oamtree_s16read(BFDPAKFGPABASE, 0x1000 + usLoopi);

            /* bit15置1，其余位保留.Bit15：每一条BFD报文发送使能。'1'发送报文 */
            usData |= 0x8000;
            spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLoopi, usData);
        }        
    }
}


#endif

#if 2
/*****************************************************************************
 函 数 名  : show_bfd_mainboard
 功能描述  : 通过全局信息，查看会话对应的主处理盘是哪个盘
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID show_bfd_mainboard(IN NBB_ULONG ulDrim  NBB_CCXT_T NBB_CXT)
{
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulDrim);
    if (NULL != pstBfdCfg)/* 说明会话存在 */
    {
        NBB_PRINTF("ID %ld , mainboard is %d, framtype is %d\n", pstBfdCfg->ulDiscrimer, pstBfdCfg->usSlotid, pstBfdCfg->ucFramtype);
        NBB_PRINTF("dip %lx,%lx,%lx,%lx\n",pstBfdCfg->ulDip[0],pstBfdCfg->ulDip[1],pstBfdCfg->ulDip[2],pstBfdCfg->ulDip[3]);
        NBB_PRINTF("sip %lx,%lx,%lx,%lx\n",pstBfdCfg->ulSip[0],pstBfdCfg->ulSip[1],pstBfdCfg->ulSip[2],pstBfdCfg->ulSip[3]);
        NBB_PRINTF("vrfid %ld\n",pstBfdCfg->usVrfid);
        NBB_PRINTF("ldp key %lx, %ld, %ld\n",pstBfdCfg->stLdpCfg.fec_prefix, pstBfdCfg->stLdpCfg.fec_prefix_len,
            pstBfdCfg->stLdpCfg.vrf_id);
        NBB_PRINTF("rsvp key %lx, %lx, %ld, %ld\n",pstBfdCfg->stRsvpCfg.egress, pstBfdCfg->stRsvpCfg.ingress,
            pstBfdCfg->stRsvpCfg.lsp_id, pstBfdCfg->stRsvpCfg.tunnel_id);
        NBB_PRINTF("vc key %lx, %ld, %ld\n",pstBfdCfg->stVcCfg.peer_ip, pstBfdCfg->stVcCfg.vc_id, pstBfdCfg->stVcCfg.vc_type);
        NBB_PRINTF("port key %ld\n", pstBfdCfg->ulPortIndex);
    }
    
    else/* 说明会话不存在 */
    {
        NBB_PRINTF("ID %ld is not exist\n", ulDrim);
    }
    return;
}


/*****************************************************************************
 函 数 名  : show_bfd_session_info
 功能描述  : 业务盘会话详细信息打印函数
 输入参数  : IN NBB_ULONG ulDrim  NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月23日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID show_bfd_session_info(IN NBB_ULONG ulDrim)
{
    NBB_USHORT usLoopi = 0;

    if ((ulDrim > BFD_MAX_ID) || (0 == ulDrim))/* 判断入参 */
    {
        NBB_PRINTF("Input Drim exceed scope 1 ~ 11*1023\n");
        return;
    }

    /* 匹配描述符 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (ulDrim == g_stBfdlocal[usLoopi].ulDiscrimer)/* 相等则输出 */
        {
            NBB_PRINTF("Drim %ld Info is : Item num %d\n", ulDrim, usLoopi);
            NBB_PRINTF("Type %d (IP/LSP/PW1/PW2)\n", g_stBfdlocal[usLoopi].ucBfdtype);
            NBB_PRINTF("Flag %d (Nocreat/Normal)\n", g_stBfdlocal[usLoopi].usBfdflag);
            NBB_PRINTF("BIsUpOnce %d\n", g_stBfdlocal[usLoopi].bIsUpOnce);
            NBB_PRINTF("Remotedrim %ld\n", g_stBfdlocal[usLoopi].ulRemoteDiscr);
            NBB_PRINTF("Iptype %d\n", g_stBfdlocal[usLoopi].ucIptype);
            if (BFD_IPV4 == g_stBfdlocal[usLoopi].ucIptype)
            {
                NBB_PRINTF("Dip %lx\n", g_stBfdlocal[usLoopi].ulDip[3]);
                NBB_PRINTF("Sip %lx\n", g_stBfdlocal[usLoopi].ulSip[3]);
            }
            else
            {
                NBB_PRINTF("Dip %lx.%lx.%lx.%lx\n", g_stBfdlocal[usLoopi].ulDip[0], g_stBfdlocal[usLoopi].ulDip[1], 
                    g_stBfdlocal[usLoopi].ulDip[2], g_stBfdlocal[usLoopi].ulDip[3]);
                NBB_PRINTF("Sip %lx.%lx.%lx.%lx\n", g_stBfdlocal[usLoopi].ulSip[0], g_stBfdlocal[usLoopi].ulSip[1], 
                    g_stBfdlocal[usLoopi].ulSip[2], g_stBfdlocal[usLoopi].ulSip[3]);
            }
            NBB_PRINTF("Sendflag %d\n", g_stBfdlocal[usLoopi].ucSendflag);
            NBB_PRINTF("State %d (admin/down/init/up)\n", g_stBfdlocal[usLoopi].ucSessionState);
            NBB_PRINTF("Diag %d\n", g_stBfdlocal[usLoopi].ucLocalDiag);
            NBB_PRINTF("ParaChangeFlag %d\n", g_stBfdlocal[usLoopi].ucParaChangeFlag);

            if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
            {
                NBB_PRINTF("Dectect method %d(single/mutiple)\n", g_stBfdlocal[usLoopi].pstBasicBfdCB.detect_method);
            }
            if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdTunnel)
            {
                NBB_PRINTF("----------------------------------------\n");
                NBB_PRINTF("For tunnel, ingress %x, egress %x, tunnelid %d, lspid %d\n",
                g_stBfdlocal[usLoopi].pstBasicBfdTunnel->ingress, g_stBfdlocal[usLoopi].pstBasicBfdTunnel->egress,
                g_stBfdlocal[usLoopi].pstBasicBfdTunnel->tunnel_id, g_stBfdlocal[usLoopi].pstBasicBfdTunnel->lsp_id);
            }
            if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdLsp)
            {
                NBB_PRINTF("----------------------------------------\n");
                NBB_PRINTF("For lsp, fec %lx, fec-len %d, vrfid %d\n",
                g_stBfdlocal[usLoopi].pstBasicBfdLsp->fec_prefix, g_stBfdlocal[usLoopi].pstBasicBfdLsp->fec_prefix_len,
                g_stBfdlocal[usLoopi].pstBasicBfdLsp->vrf_id);
            }
            if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdPw)
            {
                NBB_PRINTF("----------------------------------------\n");
                NBB_PRINTF("For pw, vcid %d, vctype %d, peerid %lx\n",
                g_stBfdlocal[usLoopi].pstBasicBfdPw->vc_id, g_stBfdlocal[usLoopi].pstBasicBfdPw->vc_type,
                g_stBfdlocal[usLoopi].pstBasicBfdPw->peer_id);
            }
            NBB_PRINTF("----------------------------------------\n");
            NBB_PRINTF("FWD info Fwdtype %d (IP/LSPID/TUNNELID/PORT/PW/PAD/PW_SRC)\n", g_stBfdlocal[usLoopi].ucBfdFwdType);
            NBB_PRINTF("VrfID %d\n", g_stBfdlocal[usLoopi].usVrfid);
            NBB_PRINTF("Lspid %d\n", g_stBfdlocal[usLoopi].usLspID);
            NBB_PRINTF("Tunnelid %d\n", g_stBfdlocal[usLoopi].usTunnelID);
            NBB_PRINTF("Nhid %d\n", g_stBfdlocal[usLoopi].usNHIID);
            NBB_PRINTF("BIslag %d, Portid %d, Svlan %d, Unit %d\n", 
                g_stBfdlocal[usLoopi].bIsLag, g_stBfdlocal[usLoopi].usPortID, 
                g_stBfdlocal[usLoopi].usSVlanID, g_stBfdlocal[usLoopi].usUnit);
            NBB_PRINTF("----------------------------------------\n");
            NBB_PRINTF("Syn Info BIschangetoup %d, BIschangetodown %d\n", 
                g_stBfdlocal[usLoopi].bIsChangeToUp, g_stBfdlocal[usLoopi].bIsChangeToDown);
            NBB_PRINTF("BIsadmin %d, StateRecord %d\n",
                g_stBfdlocal[usLoopi].bIsAdminFlag, g_stBfdlocal[usLoopi].ucStateRecord);
            NBB_PRINTF("TimeUP %d, TimeDown %d\n", g_stBfdlocal[usLoopi].usTimerPortDownToUp, g_stBfdlocal[usLoopi].usTimerPortUPToDown);
            NBB_PRINTF("BIsNeedDelete %d, TimerLiveUp %d, TimerLiveDown %d, bIsSendtoMaster %d\n", g_stBfdlocal[usLoopi].ucbIsNeedDelete, 
                g_stBfdlocal[usLoopi].usTimerLiveUp, g_stBfdlocal[usLoopi].usTimerLiveDown, g_stBfdlocal[usLoopi].bIsSendtoMaster);
            NBB_PRINTF("TimerStayDown %u\r\n",g_stBfdlocal[usLoopi].usTimerStayDown);
            break;
        }
    }
    
    if (BFD_MAX_SESNUM == usLoopi)/* 未查询到 */
    {
        NBB_PRINTF("Can not find Drim %ld\n", ulDrim);
    }
    return;
}

NBB_VOID show_bfd_state(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;
    NBB_USHORT usData3 = 0;
    NBB_USHORT usData4 = 0;
    NBB_USHORT usData5 = 0;
    NBB_USHORT usData6 = 0;
    SPM_BFD_REG_PACKET3 *pstPacketInfo3 = NULL;
    NBB_ULONG  ulDecttime = 0;

    NBB_USHORT usWorkingTx = 0;
    NBB_USHORT usFpgaCfgRmri = 0;
    NBB_USHORT usFpgaCfgDmti = 0;
    NBB_USHORT uslocalCfgTx = 0;
    NBB_USHORT uslocalCfgRx = 0;

    /* 收发包间隔的本地记录方式与实际值的映射表 */
    NBB_CHAR *lpszInterval[] = {"None",      /* 收发包技术的记录从1开始，此处定义忽略0号位 */
                                "3.3",
                                "10",
                                "20",
                                "30",
                                "40",
                                "50",
                                "60",
                                "70",
                                "80",
                                "90",
                                "100",
                                "200",
                                "300",
                                "400",
                                "500",
                                "600",
                                "700",
                                "800",
                                "900",
                                "1000"};
    NBB_CHAR *lpszSta[] = {"Admin",
                           "Down",
                           "Init",
                           "Up"};

    NBB_USHORT usIntervalNum = sizeof(lpszInterval) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */

    /* 入参判断 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        NBB_PRINTF("Please check input\n");
        return;
    }

    /* 输出表格头 */
    NBB_PRINTF("     | State info   |           |           |          Working info           |       fpga  cfg      |    local cfg   \r\n");
    NBB_PRINTF("Loop |--------------|   MyDrim  |  YourDrim |---------------------------------|----------------------|----------------\r\n");
    NBB_PRINTF("     | state | diag |           |           | alarm | rxinterval | txinterval | rmri | dmti | pfflag |  tx |  rx | dm \r\n");   
    NBB_PRINTF("-----|-------|------|-----------|-----------|-------|------------|------------|------|------|--------|-----|-----|----\r\n");
    /* 循环输出内容 */
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
    {
        usData1 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xd000 + 2 * usLoopi + 0x0);
        usData2 = spm_oamtree_s16read(BFDSESSIONBASE, 0x800 + usLoopi);
		usData2 = ((usData2 >> 15) & 0x1);
        usData3 = spm_oamtree_s16read(BFDSESSIONBASE, 0x1000 + usLoopi);
        usData4 = spm_oamtree_s16read(BFDSESSIONBASE, 0x1800 + usLoopi);
        ulDecttime = usData4;
        ulDecttime = (ulDecttime << 16) +  usData3;
        usData5 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xe000 + usLoopi);
        usData6 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xd000 + 2 * usLoopi + 0x1);
        pstPacketInfo3 = (SPM_BFD_REG_PACKET3 *)(&usData6);
        #if 0
        NBB_PRINTF("Loop %d state info: state %d, diag %d\n", usLoopi, (usData1 >> 9)&0x3, 
            g_stBfdlocal[usLoopi].ucLocalDiag);
        NBB_PRINTF("MyDrim = %ld, YouDrim = %ld\n", g_stBfdlocal[usLoopi].ulDiscrimer, 
            g_stBfdlocal[usLoopi].ulRemoteDiscr);
        NBB_PRINTF("alarm %d, working rxinterval %ld, txinterval %d, fpga rmri %d, dmti %d, pfflag %d\n", 
            usData2, ulDecttime, 
            usData5, pstPacketInfo3->usRxInterval, pstPacketInfo3->usTxInterval, pstPacketInfo3->usModify);
        NBB_PRINTF("cfg tx %d , rx %d, dm %d\n", g_stBfdlocal[usLoopi].ucDMTI, g_stBfdlocal[usLoopi].ucRMRI,
            g_stBfdlocal[usLoopi].ucDM);
        #else
        
        usWorkingTx = usData5;
        usFpgaCfgRmri = pstPacketInfo3->usRxInterval;
        usFpgaCfgDmti = pstPacketInfo3->usTxInterval;
        uslocalCfgTx = g_stBfdlocal[usLoopi].ucDMTI;
        uslocalCfgRx = g_stBfdlocal[usLoopi].ucRMRI;

        /* 越界检查 */        
        if(usWorkingTx >= usIntervalNum)
        {
            usWorkingTx = 0;    /* 强制指定非法值 */
            NBB_PRINTF("Working info txinterval error, value = %u", usWorkingTx);
        }
        if(usFpgaCfgRmri >= usIntervalNum)
        {
            usFpgaCfgRmri = 0;    /* 强制指定非法值 */
            NBB_PRINTF("FPGA config rmri error, value = %u", usFpgaCfgRmri);
        }
        if(usFpgaCfgDmti >= usIntervalNum)
        {
            usFpgaCfgDmti = 0;    /* 强制指定非法值 */
            NBB_PRINTF("FPGA config dmti error, value = %u", usFpgaCfgDmti);
        }
        if(uslocalCfgTx >= usIntervalNum)
        {
            uslocalCfgTx = 0;    /* 强制指定非法值 */
            NBB_PRINTF("Local config dmti error, value = %u", uslocalCfgTx);
        }
        if(uslocalCfgRx >= usIntervalNum)
        {
            uslocalCfgRx = 0;    /* 强制指定非法值 */
            NBB_PRINTF("Local config rmri error, value = %u", uslocalCfgRx);
        }
        
        NBB_PRINTF("%4u |%6s |%5d |%10lu |%10lu |%6u |%11lu |%11s |%5s |%5s |%7u |%4s |%4s |%3u \r\n",
            usLoopi, lpszSta[((SPM_BFD_REG_PACKET2 *)(&usData1))->usSta], g_stBfdlocal[usLoopi].ucLocalDiag,
            g_stBfdlocal[usLoopi].ulDiscrimer, g_stBfdlocal[usLoopi].ulRemoteDiscr,
            usData2, ulDecttime, lpszInterval[usWorkingTx],
            lpszInterval[usFpgaCfgRmri], lpszInterval[usFpgaCfgDmti], pstPacketInfo3->usModify,
            lpszInterval[uslocalCfgTx], lpszInterval[uslocalCfgRx], g_stBfdlocal[usLoopi].ucDM);
        #endif
    }
    return;
}

/*****************************************************************************
 函 数 名  : show_single_bfd_state
 功能描述  : 查看单个BFD的状态
 输入参数  : IN NBB_USHORT MyBfdId                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT show_single_bfd_state(IN NBB_ULONG ulBfdId)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;
    NBB_USHORT usData3 = 0;
    NBB_USHORT usData4 = 0;
    NBB_USHORT usData5 = 0;
    NBB_USHORT usData6 = 0;
    SPM_BFD_REG_PACKET3 *pstPacketInfo3 = NULL;
    NBB_ULONG  ulDecttime = 0;

    NBB_USHORT usWorkingTx = 0;
    NBB_USHORT usFpgaCfgRmri = 0;
    NBB_USHORT usFpgaCfgDmti = 0;
    NBB_USHORT uslocalCfgTx = 0;
    NBB_USHORT uslocalCfgRx = 0;

    NBB_USHORT existFlag = 0;

    /* 收发包间隔的本地记录方式与实际值的映射表 */
    NBB_CHAR *lpszInterval[] = {"None",      /* 收发包技术的记录从1开始，此处定义忽略0号位 */
                                "3.3",
                                "10",
                                "20",
                                "30",
                                "40",
                                "50",
                                "60",
                                "70",
                                "80",
                                "90",
                                "100",
                                "200",
                                "300",
                                "400",
                                "500",
                                "600",
                                "700",
                                "800",
                                "900",
                                "1000"};
    NBB_CHAR *lpszSta[] = {"Admin",
                           "Down",
                           "Init",
                           "Up"};

    NBB_USHORT usIntervalNum = sizeof(lpszInterval) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */


    /* 输出表格头 */
    NBB_PRINTF("     | State info   |           |           |          Working info           |       fpga  cfg      |"
        "    local cfg   \r\n");
    NBB_PRINTF("Loop |--------------|   MyDrim  |  YourDrim |---------------------------------|----------------------|"
        "----------------\r\n");
    NBB_PRINTF("     | state | diag |           |           | alarm | rxinterval | txinterval | rmri | dmti | pfflag |"
        "  tx |  rx | dm \r\n");   
    NBB_PRINTF("-----|-------|------|-----------|-----------|-------|------------|------------|------|------|--------|"
        "-----|-----|----\r\n");
    
    /* 循环输出内容 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if(g_stBfdlocal[usLoopi].ulDiscrimer == ulBfdId)
        {
            usData1 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xd000 + 2 * usLoopi + 0x0);
            usData2 = spm_oamtree_s16read(BFDSESSIONBASE, 0x800 + usLoopi);
    		usData2 = ((usData2 >> 15) & 0x1);
            usData3 = spm_oamtree_s16read(BFDSESSIONBASE, 0x1000 + usLoopi);
            usData4 = spm_oamtree_s16read(BFDSESSIONBASE, 0x1800 + usLoopi);
            ulDecttime = usData4;
            ulDecttime = (ulDecttime << 16) +  usData3;
            usData5 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xe000 + usLoopi);
            usData6 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xd000 + 2 * usLoopi + 0x1);
            pstPacketInfo3 = (SPM_BFD_REG_PACKET3 *)(&usData6);        
        
            usWorkingTx = usData5;
            usFpgaCfgRmri = pstPacketInfo3->usRxInterval;
            usFpgaCfgDmti = pstPacketInfo3->usTxInterval;
            uslocalCfgTx = g_stBfdlocal[usLoopi].ucDMTI;
            uslocalCfgRx = g_stBfdlocal[usLoopi].ucRMRI;

            /* 越界检查 */        
            if(usWorkingTx >= usIntervalNum)
            {
                usWorkingTx = 0;    /* 强制指定非法值 */
                NBB_PRINTF("Working info txinterval error, value = %u", usWorkingTx);
            }
            if(usFpgaCfgRmri >= usIntervalNum)
            {
                usFpgaCfgRmri = 0;    /* 强制指定非法值 */
                NBB_PRINTF("FPGA config rmri error, value = %u", usFpgaCfgRmri);
            }
            if(usFpgaCfgDmti >= usIntervalNum)
            {
                usFpgaCfgDmti = 0;    /* 强制指定非法值 */
                NBB_PRINTF("FPGA config dmti error, value = %u", usFpgaCfgDmti);
            }
            if(uslocalCfgTx >= usIntervalNum)
            {
                uslocalCfgTx = 0;    /* 强制指定非法值 */
                NBB_PRINTF("Local config dmti error, value = %u", uslocalCfgTx);
            }
            if(uslocalCfgRx >= usIntervalNum)
            {
                uslocalCfgRx = 0;    /* 强制指定非法值 */
                NBB_PRINTF("Local config rmri error, value = %u", uslocalCfgRx);
            }
        
            NBB_PRINTF("%4u |%6s |%5d |%10lu |%10lu |%6u |%11lu |%11s |%5s |%5s |%7u |%4s |%4s |%3u \r\n",
            usLoopi, lpszSta[((SPM_BFD_REG_PACKET2 *)(&usData1))->usSta], g_stBfdlocal[usLoopi].ucLocalDiag,
            g_stBfdlocal[usLoopi].ulDiscrimer, g_stBfdlocal[usLoopi].ulRemoteDiscr,
            usData2, ulDecttime, lpszInterval[usWorkingTx],
            lpszInterval[usFpgaCfgRmri], lpszInterval[usFpgaCfgDmti], pstPacketInfo3->usModify,
            lpszInterval[uslocalCfgTx], lpszInterval[uslocalCfgRx], g_stBfdlocal[usLoopi].ucDM);

            existFlag = 1;
            break;
        }
    }

    if(0 == existFlag)
    {
            NBB_PRINTF("BfdId = %ld is not existed!\n",ulBfdId);    
    }
    return 0;
}

/*****************************************************************************
 函 数 名  : show_bfd_counter
 功能描述  : 统计某条目BFD的收发包计数
 输入参数  : IN NBB_USHORT usStartNum                    
             IN NBB_USHORT usEndNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID show_bfd_counter(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT TxCounter = 0;
    NBB_USHORT RxCounter = 0;

    NBB_PRINTF("     |        |          |        Counter        |\r\n");
    NBB_PRINTF("Loop | MyDrim | YourDrim |-----------------------|\r\n");
    NBB_PRINTF("     |        |          | TxCounter | RxCounter |\r\n");   
    NBB_PRINTF("-----|--------|----------|-----------|-----------|\r\n");

    /* 入参判断 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    { 
        NBB_PRINTF("Please check input\n");
        return;
    }

    /* 循环输出 */
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
    {
        TxCounter = spm_oamtree_s16read(BFDPAKFGPABASE, 0xf000 + usLoopi); /* 发包计数 */
        RxCounter = spm_oamtree_s16read(BFDSESSIONBASE, 0x2800 + usLoopi); /* 收包计数 */
        
        NBB_PRINTF("%4d |%7ld |%9ld |", usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, 
            g_stBfdlocal[usLoopi].ulRemoteDiscr);
        NBB_PRINTF("%10d |%10d |\n", TxCounter, RxCounter);

    }
    return;
}


/*****************************************************************************
 函 数 名  : show_single_bfd_counter
 功能描述  : 统计单个BFD的收发包计数
 输入参数  : IN NBB_USHORT MyBfdId
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_single_bfd_counter(IN NBB_ULONG ulBfdId)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT TxCounter = 0;
    NBB_USHORT RxCounter = 0;
    NBB_USHORT existFlag = 0;


    NBB_PRINTF("     |        |          |        Counter        |\r\n");
    NBB_PRINTF("Loop | MyDrim | YourDrim |-----------------------|\r\n");
    NBB_PRINTF("     |        |          | TxCounter | RxCounter |\r\n");   
    NBB_PRINTF("-----|--------|----------|-----------|-----------|\r\n");
    
    /* 循环输出 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if(g_stBfdlocal[usLoopi].ulDiscrimer == ulBfdId)
        {
            TxCounter = spm_oamtree_s16read(BFDPAKFGPABASE, 0xf000 + usLoopi); /* 发包计数 */
            RxCounter = spm_oamtree_s16read(BFDSESSIONBASE, 0x2800 + usLoopi); /* 收包计数 */
            
            NBB_PRINTF("%4d |%7ld |%9ld |", usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, 
                g_stBfdlocal[usLoopi].ulRemoteDiscr);
            NBB_PRINTF("%10d |%10d |\n", TxCounter, RxCounter);
            
            existFlag = 1;
            break;
        }
    }

    if(0 == existFlag)
    {
        NBB_PRINTF("BfdId = %ld is not existed!\n",ulBfdId);    
    }
    
    return 0;
}

/*****************************************************************************
 函 数 名  : show_bfd_event
 功能描述  : 查看单个BFD的事件
 输入参数  : IN NBB_USHORT MyBfdId                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
NBB_VOID show_bfd_event(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum)
{
    NBB_USHORT usLoopi;
    NBB_CHAR *BfdState[] = {"Admin","Down","Init","Up"};
    NBB_BYTE ucIntervalNum = sizeof(BfdState) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */
	NBB_CHAR *RxBfdState[] = {"ADMINDOWN","DOWN","INIT","UP","OTHER"};
    NBB_BYTE ucNumber = sizeof(RxBfdState) / 4;

    /* 入参检查 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    { 
        NBB_PRINTF("Please check input\n");
        return;
    }

    NBB_PRINTF("Total record item is %d\n", g_usRecordNum);
    NBB_PRINTF("     |           State transfer process                 |"
    "           |                  time                  |\r\n");
    NBB_PRINTF("Loop |--------------------------------------------------|"
    "   MyDrim  |----------------------------------------|\r\n");
    NBB_PRINTF("     | LastState--(RxPacketState)-->PresentState | diag |"
    "           | year-month-date hour:minute:second:tick|\r\n");   
    NBB_PRINTF("-----|------------(_____________)-->-------------|------|"
    "-----------|----------------------------------------|\r\n");
    
    /* 循环输出 */
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
    {
        NBB_PRINTF("%4d |",g_stBfdEventRecord[usLoopi].usLoopi);
        if(g_stBfdEventRecord[usLoopi].ucLastState < ucIntervalNum)
        {
            NBB_PRINTF("%10s--(",BfdState[g_stBfdEventRecord[usLoopi].ucLastState]);    
        }
        if(g_stBfdEventRecord[usLoopi].ucRxPacketState < ucNumber)
        {
            NBB_PRINTF("%9s    )-->",RxBfdState[g_stBfdEventRecord[usLoopi].ucRxPacketState]);    
        }
        if(g_stBfdEventRecord[usLoopi].ucState < ucIntervalNum)
        {
            NBB_PRINTF("%-12s |",BfdState[g_stBfdEventRecord[usLoopi].ucState]);    
        }
        NBB_PRINTF("%5d |%10lu |",g_stBfdEventRecord[usLoopi].ucDiag,g_stBfdEventRecord[usLoopi].ulDisrim);
        NBB_PRINTF("         %d-%d-%d %d:%d:%d:%d\n", g_stBfdEventRecord[usLoopi].usYear, 
        g_stBfdEventRecord[usLoopi].ucMonth, g_stBfdEventRecord[usLoopi].ucDate,
        g_stBfdEventRecord[usLoopi].ucHour, g_stBfdEventRecord[usLoopi].ucMinute,
        g_stBfdEventRecord[usLoopi].ucSecond, g_stBfdEventRecord[usLoopi].us5Ms);
    }
  
    NBB_PRINTF("-----|------------(_____________)-->-------------|------|"
        "-----------|----------------------------------------|\r\n");
    NBB_PRINTF("     | LastState--(RxPacketState)-->PresentState | diag |"
        "           | year-month-date hour:minute:second:tick|\r\n"); 
    NBB_PRINTF("Loop |--------------------------------------------------|"
        "   MyDrim  |----------------------------------------|\r\n");
    NBB_PRINTF("     |           State transfer process                 |"
        "           |                  time                  |\r\n");
    return;
}

/*****************************************************************************
 函 数 名  : show_single_bfd_event
 功能描述  : 查看单个BFD的事件
 输入参数  : IN NBB_USHORT MyBfdId                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_single_bfd_event(IN NBB_ULONG ulBfdId)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT existFlag = 0;


    NBB_CHAR *BfdState[] = {"Admin","Down","Init","Up"};
    NBB_BYTE ucIntervalNum = sizeof(BfdState) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */
	NBB_CHAR *RxBfdState[] = {"ADMINDOWN","DOWN","INIT","UP","OTHER"};
    NBB_BYTE ucNumber = sizeof(RxBfdState) / 4;
    
    NBB_PRINTF("Total record item is %d\n", g_usRecordNum);
    NBB_PRINTF("     |           State transfer process                 |"
        "           |                  time                  |\r\n");
    NBB_PRINTF("Loop |--------------------------------------------------|"
        "   MyDrim  |----------------------------------------|\r\n");
    NBB_PRINTF("     | LastState--(RxPacketState)-->PresentState | diag |"
        "           | year-month-date hour:minute:second:tick|\r\n");   
    NBB_PRINTF("-----|------------(_____________)-->-------------|------|"
        "-----------|----------------------------------------|\r\n");

    /* 循环输出 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if(g_stBfdEventRecord[usLoopi].ulDisrim == ulBfdId)
        {
            NBB_PRINTF("%4d |",g_stBfdEventRecord[usLoopi].usLoopi);
            if(g_stBfdEventRecord[usLoopi].ucLastState < ucIntervalNum)
            {
                NBB_PRINTF("%10s--(",BfdState[g_stBfdEventRecord[usLoopi].ucLastState]);    
            }
            if(g_stBfdEventRecord[usLoopi].ucRxPacketState < ucNumber)
            {
                NBB_PRINTF("%9s    )-->",RxBfdState[g_stBfdEventRecord[usLoopi].ucRxPacketState]);    
            }
            if(g_stBfdEventRecord[usLoopi].ucState < ucIntervalNum)
            {
                NBB_PRINTF("%-12s |",BfdState[g_stBfdEventRecord[usLoopi].ucState]);    
            }
            NBB_PRINTF("%5d |%10lu |",g_stBfdEventRecord[usLoopi].ucDiag,g_stBfdEventRecord[usLoopi].ulDisrim);
            NBB_PRINTF("         %d-%d-%d %d:%d:%d:%d\n", g_stBfdEventRecord[usLoopi].usYear, 
            g_stBfdEventRecord[usLoopi].ucMonth, g_stBfdEventRecord[usLoopi].ucDate,
            g_stBfdEventRecord[usLoopi].ucHour, g_stBfdEventRecord[usLoopi].ucMinute,
            g_stBfdEventRecord[usLoopi].ucSecond, g_stBfdEventRecord[usLoopi].us5Ms);
            existFlag = 1;
            continue;
        }

    }

    if(0 == existFlag)
    {
        NBB_PRINTF("BfdId = %ld is not existed!\n",ulBfdId);
    }

    return 0;
}

/*****************************************************************************
 函 数 名  : show_bfd_fpga_drop_couter
 功能描述  : 查看FPGA RX丢包统计
 输入参数  : 无             
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月18日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_bfd_fpga_drop_counter()
{
    NBB_USHORT usDate = 0;

	NBB_PRINTF("FPGA Drop Counter:\n");
    usDate = spm_oamtree_s16read(BFDSESSIONSTATE,0x5);
    NBB_PRINTF("\tErrorHead:%d\n",usDate);
    usDate = spm_oamtree_s16read(BFDSESSIONSTATE,0x6);
    NBB_PRINTF("\tErrorType:%d\n",usDate);
    return 0;
}

#if defined(SRC)

/*****************************************************************************
 函 数 名  : show_bfd_fpga_rxbus_info
 功能描述  : 查看SRC上总线收到的BFD 信息 
 输入参数  : IN NBB_USHORT SlotId
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_bfd_fpga_rxbus_info(IN NBB_BYTE ucSlotId,IN NBB_USHORT usNum)
{
    NBB_ULONG  ulBaseAddress = 0;

    if ((ucSlotId >= SYS_MAX_SLOTNUM) || (0 == ucSlotId))/* 判断入参 */
    {
        NBB_PRINTF("Input SlotId exceed scope 1 ~ 12\n");
        return -1;
    }
    if(usNum > 0x400)
    {
        NBB_PRINTF("Input usNum exceed scope 0 ~ 1024\n");
        return -1;
    }

    ulBaseAddress = BFDSTATEBASE + 0x20000 + (ucSlotId - 1)*0x100;
    if(0 == usNum)
    {
        hwprintfpgareg32b(ulBaseAddress,100);
    }
    else
    {
        hwprintfpgareg32b(ulBaseAddress,usNum);
    }

    return 0;
}


/*****************************************************************************
 函 数 名  : show_bfd_fpga_bus_map_table
 功能描述  : 查看SRC上总线映射表
 输入参数  : IN NBB_USHORT SlotId
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_bfd_fpga_bus_map_table(IN NBB_BYTE ucSlotId,IN NBB_USHORT usNum)
{
    NBB_ULONG  ulBaseAddress = 0;

    if ((ucSlotId > SYS_MAX_SLOTNUM + 9) || (0 == ucSlotId))/* 判断入参 */
    {
        NBB_PRINTF("Input SlotId exceed scope 1 ~ 22\n");
        return -1;
    }
    if(usNum > 0x400)
    {
        NBB_PRINTF("Input usNum exceed scope 0 ~ 1024\n");
        return -1;
    }

    ulBaseAddress = BFDSTATEBASE + 0x30000 + (ucSlotId - 1)*0x400;
    if(0 == usNum)
    {
        hwprintfpgareg32b(ulBaseAddress,100);
    }
    else
    {
        hwprintfpgareg32b(ulBaseAddress,usNum);
    }

    return 0;
}

#elif defined(SPU)


/*****************************************************************************
 函 数 名  : show_bfd_write_bus_info
 功能描述  : 查看BFD写总线信息
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_bfd_write_bus_info(IN NBB_USHORT usNum)
{
 	NBB_PRINTF("4 bytes represent states of 16 BFD session, and every 2 bits have four different values,\n");
	NBB_PRINTF("which represent different states of BFD, as flowing:\n\n");
	NBB_PRINTF("BFD_RECORD_INVALID = 0\t");
	NBB_PRINTF("BFD_RECORD_UP = 1\n");
	NBB_PRINTF("BFD_RECORD_DOWN = 2\t\t");
	NBB_PRINTF("BFD_RECORD_BUTT = 3\n");
	NBB_PRINTF("\n");
	NBB_PRINTF("|       forth       |       third      |     second    |     first     | \n");
	NBB_PRINTF("| 16 | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | \n");
	NBB_PRINTF("  01   01   01   01   01   01   01   01  01  01  01  01  01  01  01  01  \n");

    if(usNum > 0x400)
    {
        NBB_PRINTF("Input usNum exceed scope 0 ~ 1024\n");
        return -1;
    }

    if(0 == usNum)
    {
        hwprintfpgareg32b(0x910000,100);
    }
    else
    {
        hwprintfpgareg32b(0x910000,usNum);
    }    
    return 0;    
}

/*****************************************************************************
 函 数 名  : show_bfd_read_bus_info
 功能描述  : 查看BFD同步的总线信息
 输入参数  : IN NBB_BYTE usK,    第几K条目
                IN NBB_USHORT usNum   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月16日
    作    者   : tianf
    审 核 人   : #
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT show_bfd_read_bus_info(IN NBB_BYTE ucK, IN NBB_USHORT usNum)
{
    NBB_ULONG  ulBaseAddress = 0;
    
 	NBB_PRINTF("4 bytes represent states of 16 BFD session, and every 2 bits have four different values,\n");
	NBB_PRINTF("which represent different states of BFD, as flowing:\n\n");
	NBB_PRINTF("BFD_RECORD_INVALID = 0\t");
	NBB_PRINTF("BFD_RECORD_UP = 1\n");
	NBB_PRINTF("BFD_RECORD_DOWN = 2\t\t");
	NBB_PRINTF("BFD_RECORD_BUTT = 3\n");
	NBB_PRINTF("\n");
	NBB_PRINTF("|       forth       |       third      |     second    |     first     | \n");
	NBB_PRINTF("| 16 | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | \n");
	NBB_PRINTF("  01   01   01   01   01   01   01   01  01  01  01  01  01  01  01  01  \n");

    if((0 == ucK) || (ucK > 21))
    {
        NBB_PRINTF("Input usK exceed scope 1 ~ 21\n");
        return -1;
    }

    if(usNum > 0x400)
    {
        NBB_PRINTF("Input usNum exceed scope 0 ~ 1024\n");
        return -1;
    }

    ulBaseAddress = 0x920000 + (ucK - 1) * 0x40;
    if(0 == usNum)
    {
        hwprintfpgareg32b(ulBaseAddress,0x40);
    }
    else
    {
        hwprintfpgareg32b(ulBaseAddress,usNum);
    } 
    return 0;    
}


#endif

/*****************************************************************************
 函 数 名  : show_bfd_recource
 功能描述  : 显示本盘处理的会话数目和转发的数目
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月18日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID show_bfd_recource(NBB_VOID)
{ 
    NBB_BYTE ucLocalSlot = 0;
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    NBB_PRINTF("BoardId %d , total session num %d, fwd num %d\n", ucLocalSlot, g_usBfdNum, g_usFwdNum);
    return;
}

NBB_VOID showbfdid(NBB_VOID)
{
    NBB_BYTE ucLocalSlot;
    NBB_USHORT usLoopi;
    NBB_USHORT usLoopj = 0;
    NBB_USHORT usSlotid;
    
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    if (0 == spm_hardw_getboardtype(ucLocalSlot))
    {
        for (usSlotid = 1; usSlotid < SYS_MAX_SLOTNUM; usSlotid++)
        {
            NBB_PRINTF("Slot %d, syn id :\n", usSlotid);
            for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
            {
                if ((DRV_ULONG_INVALID != g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer)
                    && (0 != g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer)
                    && (DRV_USHORT_INVALID != g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer))
                {
                    NBB_PRINTF("item %d,discrim %ld ", usLoopi, 
                        g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer);
                    usLoopj++;
                    if (8 == usLoopj)
                    {
                        NBB_PRINTF("\n");
                        usLoopj = 0;
                    }
                }
            }
            
            if (usLoopj < 8)
            {
                NBB_PRINTF("\n");
            }
            
            NBB_PRINTF("----------------------------------------------------\n");
        }
    }
    return;
}

NBB_VOID showbfdnpboard(NBB_VOID)
{
    NBB_PRINTF("At moment Slot %d is normal\n", g_ucFirstNpBoard);
    return;
}

NBB_VOID showbfdpckrxtx(NBB_VOID)
{
    NBB_USHORT usSlotid;
    NBB_USHORT usLoopj = 0;
    NBB_USHORT usLoopk = 0;
    
    for (usSlotid = 1; usSlotid < SYS_MAX_SLOTNUM; usSlotid++)
    {
        NBB_PRINTF("The board Tx Info : \n");
        NBB_PRINTF("Slot %d: total tx num %ld\n", usSlotid, g_stFwdTxRec[usSlotid - 1].ulNum);
        if (0 == g_stFwdTxRec[usSlotid - 1].ulNum)
        {
            NBB_PRINTF("---------------------------------\n");
        }
        else
        {
            for (usLoopj = 0; usLoopj < BFD_MAX_FWDRECORDNUM; usLoopj++)
            {   
                if (0 != g_stFwdTxRec[usSlotid - 1].aulDiscrimer[usLoopj])
                {
                    usLoopk++;
                    NBB_PRINTF("Current drim is %ld ", g_stFwdTxRec[usSlotid - 1].aulDiscrimer[usLoopj]);
                    if (8 == usLoopk)
                    {
                        NBB_PRINTF("\n");
                        usLoopk = 0;
                    }
                }
            }
            NBB_PRINTF("\n");
            NBB_PRINTF("---------------------------------\n");
        }
        
        NBB_PRINTF("total rx num %ld\n", g_stFwdRxRec[usSlotid - 1].ulNum);
        if (0 == g_stFwdRxRec[usSlotid - 1].ulNum)
        {
            NBB_PRINTF("---------------------------------\n");
        }
        else
        {
            for (usLoopj = 0; usLoopj < BFD_MAX_FWDRECORDNUM; usLoopj++)
            {   
                if (0 != g_stFwdRxRec[usSlotid - 1].aulDiscrimer[usLoopj])
                {
                    usLoopk++;
                    NBB_PRINTF("Current drim is %ld", g_stFwdRxRec[usSlotid - 1].aulDiscrimer[usLoopj]);
                    if (8 == usLoopk)
                    {
                        NBB_PRINTF("\n");
                        usLoopk = 0;
                    }
                }
            }
            NBB_PRINTF("\n");
            NBB_PRINTF("---------------------------------\n");
        }
    }
}

NBB_VOID showbfdpckrxtxclear(NBB_VOID)
{
    OS_MEMSET(g_stFwdTxRec, 0, sizeof(SPM_BFD_FWDRECORD)*SYS_TOTAL_SLOTNUM);
    OS_MEMSET(g_stFwdRxRec, 0, sizeof(SPM_BFD_FWDRECORD)*SYS_TOTAL_SLOTNUM);
    return;
}

/*****************************************************************************
 * 函 数 名  : show_bfd_anti_attack_info
 * 负 责 人  : tianf
 * 创建日期  : 2015年12月2日
 * 函数功能  : 查看BFD防攻击，寄存器配置信息
 * 输入参数  : IN NBB_ULONG ulBfdId
 * 输出参数  : 无
 * 返 回 值  : 0
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT show_bfd_anti_attack_info(IN NBB_ULONG ulBfdId)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;
    NBB_USHORT usData3 = 0;
    NBB_BYTE ucEnable = 0;
    NBB_ULONG  ulDiscrm = 0;
    NBB_USHORT existFlag = 0;
    NBB_CHAR *lpszSta[] = {"Admin","Down","Init","Up"};
    NBB_BYTE ucIntervalNum = sizeof(lpszSta) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */
    
    /* 输出表格头 */
    NBB_PRINTF("     |         |           |  State info  |       Anti-Attack       |\r\n");
    NBB_PRINTF("Loop | MyDiscr | YourDiscr |--------------|-------------------------|\r\n");
    NBB_PRINTF("     |         |           | state | diag |    Discr  | Anti_Enable |\r\n");   
    NBB_PRINTF("-----|---------|-----------|-------|------|-----------|-------------|\r\n");
        
    /* 循环输出内容 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if(g_stBfdlocal[usLoopi].ulDiscrimer == ulBfdId)
        {
            usData1 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xd000 + 2 * usLoopi + 0x0);

            /* BFD收模块寄存器偏移0x4000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
            usData2 = spm_oamtree_s16read(BFDSESSIONBASE, 0x4000 + 2 * usLoopi + 0x1);
            
            /* BFD收模块寄存器偏移0x4000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
            usData3 = spm_oamtree_s16read(BFDSESSIONBASE, 0x4000 + 2 * usLoopi + 0x0);

            ulDiscrm = (NBB_ULONG)usData2;
            ulDiscrm = (ulDiscrm << 16) + usData3;
             
            /*本端YID与收包MYID匹配置位使能*/
            ucEnable = spm_oamtree_s16read(BFDSESSIONBASE, 0x5000 + usLoopi) & 0x1;   
            NBB_PRINTF("%4d |%8lu |%10lu ",
            usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, g_stBfdlocal[usLoopi].ulRemoteDiscr);
            
            if(((SPM_BFD_REG_PACKET2 *)(&usData1))->usSta < ucIntervalNum)
            {
                NBB_PRINTF("|%6s |",lpszSta[((SPM_BFD_REG_PACKET2 *)(&usData1))->usSta]);
            }
            NBB_PRINTF("%5d |",g_stBfdlocal[usLoopi].ucLocalDiag);

            NBB_PRINTF("%10d |%12s |\r\n",ulDiscrm,(ucEnable == 1)?"ENABLE":"DISABLE");
            existFlag = 1;
            break;
        }
    }

    if(0 == existFlag)
    {
            NBB_PRINTF("BfdId = %ld is not existed!\n",ulBfdId);    
    }

    return 0;
}

/*****************************************************************************
 * 函 数 名  : show_bfd_ipv4_tree_cfg
 * 负 责 人  : tianf
 * 创建日期  : 2015年12月2日
 * 函数功能  : 查看BFD FOR IPV4 存储配置
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 0
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT show_bfd_ipv4_tree_cfg()
{
    SPM_BFD_IP_GLOBAL *pstBfdIpv4Cfg = NULL;
    NBB_ULONG ulCnt = 0;
    
    /* 输出表格头 */
    NBB_PRINTF("   SIP  |   DIP  | VrfId | Discrimer | SlotId |\r\n");  
    NBB_PRINTF("--------|--------|-------|-----------|--------|\r\n");
    
    for (pstBfdIpv4Cfg = (SPM_BFD_IP_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByIp);
         pstBfdIpv4Cfg != NULL;
         pstBfdIpv4Cfg = (SPM_BFD_IP_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByIp,
                                        pstBfdIpv4Cfg->spm_bfdip_node))
    {
        NBB_PRINTF("%8x|%8x|%7d|%11d|%8d|\r\n",pstBfdIpv4Cfg->stBfdGlbKey.ulSip,
            pstBfdIpv4Cfg->stBfdGlbKey.ulDip,pstBfdIpv4Cfg->stBfdGlbKey.usVrfid,pstBfdIpv4Cfg->ulDiscrimer,
            pstBfdIpv4Cfg->usSlotid);
        ulCnt++;

    }
    NBB_PRINTF("The counters of SPM_BFD_IP_GLOBAL is %ld!\r\n",ulCnt);
    return 0;
}

/*****************************************************************************
 * 函 数 名  : show_bfd_ipv6_tree_cfg
 * 负 责 人  : tianf
 * 创建日期  : 2015年12月2日
 * 函数功能  : 查看BFD FOR IPV6存储配置
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 0
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT show_bfd_ipv6_tree_cfg()
{
    SPM_BFD_IPV6_GLOBAL *pstBfdIpv6Cfg = NULL;
    NBB_BYTE ucLoopi = 0;
    NBB_ULONG ulCnt = 0;
    
    /* 输出表格头 */
    NBB_PRINTF("                SIP                 |                  DIP               |"
        " VrfId | Discrimer | SlotId |\r\n");  
    NBB_PRINTF("------------------------------------|------------------------------------|"
        "-------|-----------|--------|\r\n");
    
    for (pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByIpv6);
         pstBfdIpv6Cfg != NULL;
         pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByIpv6,
                                        pstBfdIpv6Cfg->spm_bfdipv6_node))
    {
        for(ucLoopi = 0; ucLoopi < 4; ucLoopi++)
        {
            NBB_PRINTF(" %8x",pstBfdIpv6Cfg->stBfdGlbKey.ulSip[ucLoopi]);
            if(3 == ucLoopi)
            {
                NBB_PRINTF("|");
            }
        }

        for(ucLoopi = 0; ucLoopi < 4; ucLoopi++)
        {
            NBB_PRINTF(" %8x",pstBfdIpv6Cfg->stBfdGlbKey.ulDip[ucLoopi]);
            if(3 == ucLoopi)
            {
                NBB_PRINTF("|");
            }
        }
        
        NBB_PRINTF("%7d|%11d|%8d|\r\n",pstBfdIpv6Cfg->stBfdGlbKey.usVrfid,
            pstBfdIpv6Cfg->ulDiscrimer,pstBfdIpv6Cfg->usSlotid);
        ulCnt++;

    }
    NBB_PRINTF("The counters of SPM_BFD_IPV6_GLOBAL is %ld!\r\n",ulCnt);
    return 0;
}


#endif
/*****************************************************************************
 函 数 名  : spm_bfd_update_test
 功能描述  : 批量动态改变时间参数函数。BFD UP之后，批量对BFD的RMRI,DMTI参数
             发起一次改变。条目之间连续8个时间参数（1S-3.3ms）不重复。
 输入参数  : IN NBB_USHORT usStartNum                    
             IN NBB_USHORT usEndNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_update_test(IN NBB_USHORT usStartNum, IN NBB_USHORT usEndNum)
{
    NBB_USHORT usLoopi = 0;

    /* 入参判断 */
    if ((usEndNum > BFD_MAX_SESNUM) || (usStartNum > BFD_MAX_SESNUM))
    {
        return;
    }

    /* 循环输出 */
    for (usLoopi = usStartNum; usLoopi < usEndNum; usLoopi++)
    {
        /* 会话状态UP，则修改参数 */
        if((usLoopi % BFD_500MS + BFD_3MS3) != g_stBfdlocal[usLoopi].ucDMTI)/* DMTI不相等 */
        {
            g_stBfdlocal[usLoopi].ucDMTI = usLoopi % BFD_500MS + BFD_3MS3;
            g_stBfdlocal[usLoopi].ucParaChangeFlag |= BFD_PARA_INTERVAL_CHANGE;
            
        }
        if ((usLoopi % BFD_500MS + BFD_3MS3) != g_stBfdlocal[usLoopi].ucRMRI)/* RMRI不相等 */
        {
            g_stBfdlocal[usLoopi].ucRMRI = usLoopi % BFD_500MS + BFD_3MS3;
            g_stBfdlocal[usLoopi].ucParaChangeFlag |= BFD_PARA_INTERVAL_CHANGE;
        }
    }
    return;
}

/*****************************************************************************
 函 数 名  : bfdwritefpga
 功能描述  : 按照要求提供连续写函数,按照康俊需求提供写函数，基地址部分不需要乘以2
 输入参数  : IN NBB_UINT uiAddr  :   偏移地址, 减去基地址ea或eb后的值 
             IN NBB_UINT uiOffAddr:  地址步长
             IN NBB_USHORT usData  : 起始值                   
             IN NBB_USHORT usOffdate :步长                 
             IN NBB_USHORT usNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfdwritefpga(
        IN NBB_UINT uiAddr, 
        IN NBB_UINT uiOffAddr,
        IN NBB_USHORT usData, 
        IN NBB_USHORT usOffdate, 
        IN NBB_USHORT usNum)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT uiOffsetTmp = 0;

    /* 循环写入 */
    for (usLoopi = 0; usLoopi < usNum; usLoopi++)
    {
        uiOffsetTmp = uiAddr + uiOffAddr * usLoopi;
        (NBB_VOID)WriteFpgaMmap(uiOffsetTmp, usData + usOffdate * usLoopi);
    }
    
    return;
}

/*****************************************************************************
 函 数 名  : bfdwritefpga32b
 功能描述  : 按照要求提供连续写函数,按照康俊需求提供写函数，基地址部分不需要乘以2
 输入参数  : IN NBB_UINT uiAddr  : 偏移地址, 减去基地址ea或eb后的值 
             IN NBB_UINT uiOffAddr:  地址步长
             IN NBB_UINT uiData  : 起始值                   
             IN NBB_USHORT usOffdate :步长                 
             IN NBB_USHORT usNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID bfdwritefpga32b(
        IN NBB_UINT uiAddr, 
        IN NBB_UINT uiOffAddr,
        IN NBB_UINT uiData, 
        IN NBB_USHORT usOffdate, 
        IN NBB_USHORT usNum)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT uiOffsetTmp = 0;

    /* 循环写入 */
    for (usLoopi = 0; usLoopi < usNum; usLoopi++)
    {
        uiOffsetTmp = uiAddr + uiOffAddr * usLoopi;
        (NBB_VOID)WriteFpga32b(uiOffsetTmp, uiData + usOffdate * usLoopi);
    }
    
    return;
}


/*****************************************************************************
 函 数 名  : setsrcgeloop
 功能描述  : 设置FPGA的GE口环回
 输入参数  : IN NBB_BYTE ucType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年12月4日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID setsrcgeloop(IN NBB_BYTE ucType)
{
    (NBB_VOID)WriteFpgaMmap(0x20001, 0x0);
    (NBB_VOID)WriteFpgaMmap(0x20002, 0x5140);
    if (1 == ucType)/* GE0 */
    {
        (NBB_VOID)WriteFpgaMmap(0x20003, 0x1080);
    }
    else if (2 == ucType)/* GE1 */
    {
        (NBB_VOID)WriteFpgaMmap(0x20003, 0x1180);
    }
    else if (3 == ucType)/* GE2 */
    {
        (NBB_VOID)WriteFpgaMmap(0x20003, 0x1280);
    }
    else
    {
        return;
    }
    (NBB_VOID)WriteFpgaMmap(0x20003, 0x0);
    return;
}

/*****************************************************************************
 函 数 名  : setspugeloop
 功能描述  : 设置FPGA的GE口环回
 输入参数  : IN NBB_BYTE ucType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年12月4日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID setspugeloop(IN NBB_BYTE ucType)
{
    (NBB_VOID)WriteFpgaMmap(0x1001, 0x0);
    (NBB_VOID)WriteFpgaMmap(0x1002, 0x5140);
    if (1 == ucType)/* GE0 */
    {
        (NBB_VOID)WriteFpgaMmap(0x1003, 0x1080);
    }
    else if (2 == ucType)/* GE1 */
    {
        (NBB_VOID)WriteFpgaMmap(0x1003, 0x1180);
    }
    else if (3 == ucType)/* GE2 */
    {
        (NBB_VOID)WriteFpgaMmap(0x1003, 0x1280);
    }
    else
    {
        return;
    }
    (NBB_VOID)WriteFpgaMmap(0x1003, 0x0);
    return;
}

NBB_VOID setdownrecordtrue(NBB_VOID)
{
    g_BIsRecordDownEvent = TRUE;
}

/*****************************************************************************
 * 函 数 名  : setdownrecordfalse
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2013年12月17日
 * 函数功能  : 设置开关
 * 输入参数  : 
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID setdownrecordfalse(NBB_VOID)
{
    g_BIsRecordDownEvent = FALSE;
}

/*****************************************************************************
 * 函 数 名  : setStateMachineSwitch
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2013年12月17日
 * 函数功能  : 设置状态机开关
 * 输入参数  : NBB_BOOL bSwitch  状态机开关，0=不跑状态机，1=跑状态机
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID setStateMachineSwitch(NBB_BOOL bSwitch, NBB_USHORT usstartnum, NBB_USHORT usendnum)
{
    NBB_USHORT usloopi = 0;
    if ((usstartnum > usendnum) || (usendnum > BFD_MAX_SESNUM) || (usstartnum > BFD_MAX_SESNUM))
    {
        NBB_PRINTF("input error. scope is 0~2047\r\n");
        return;
    }
    
    if(FALSE == bSwitch)    /* 不跑状态机 */
    {
        for (usloopi = usstartnum; usloopi <usendnum; usloopi++)
        {
            g_BIsCloseStateMachine[usloopi] = TRUE;
        }
    }
    else    /* 跑状态机 */
    {
        for (usloopi = usstartnum; usloopi <usendnum; usloopi++)
        {
            g_BIsCloseStateMachine[usloopi] = FALSE;
        }
    }
}

/*****************************************************************************
 * 函 数 名  : setdownrecordfalse
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2013年12月17日
 * 函数功能  : 设置开关
 * 输入参数  : 
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID showStateMachineSwitch(NBB_USHORT usloopi)
{
    NBB_PRINTF("bfd state machine is %s\r\n", ((FALSE == g_BIsCloseStateMachine[usloopi]) ? "running" : "closing"));
}

/*****************************************************************************
 * 函 数 名  : setdownrecordfalse
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2013年12月17日
 * 函数功能  : 设置开关
 * 输入参数  : 
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID showStateMachineSwitchall(NBB_VOID)
{
    NBB_USHORT usloopi = 0;
    NBB_USHORT usloopj = 0;

    NBB_PRINTF("state machine close is:");
    for (usloopi = 0; usloopi < BFD_MAX_SESNUM; usloopi++)
    {
        if (TRUE == g_BIsCloseStateMachine[usloopi])
        {
            NBB_PRINTF("%d.", usloopi);
            usloopj++;
        }

        if ((0 == (usloopj % 12)) && (0 != usloopj))
        {
            NBB_PRINTF("\r\n");
        }
    }
    NBB_PRINTF("\r\n");
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_event
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2013年12月17日
 * 函数功能  : 设置开关
 * 输入参数  : 
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID spm_bfd_event(SPM_BFD_EVENT_INFO *pstBfdEventDate, NBB_BOOL bIsDown)
{
    struct SYSTIME   systime;
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;
    NBB_USHORT usLoopi = 0;
    
    if(NULL == pstBfdEventDate)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "psBfdEventDate is NULL!\r\n");
        return;
    }
    
    GetSysClock(&systime);
    usLoopi = pstBfdEventDate->usLocalIndex;
    usData1 = spm_oamtree_s16read(BFDPAKFGPABASE, 0xf000 + usLoopi); /* 发包计数 */
    usData2 = spm_oamtree_s16read(BFDSESSIONBASE, 0x2800 + usLoopi); /* 收包计数 */

    g_stBfdEventRecord[g_usRecordNum].usTxCounter = usData1;
    g_stBfdEventRecord[g_usRecordNum].usRxCounter = usData2;
    g_stBfdEventRecord[g_usRecordNum].usYear = systime.usYear;
    g_stBfdEventRecord[g_usRecordNum].ucMonth = systime.ucMonth;
    g_stBfdEventRecord[g_usRecordNum].ucHour = systime.ucHour;
    g_stBfdEventRecord[g_usRecordNum].ucMinute = systime.ucMinute;
    g_stBfdEventRecord[g_usRecordNum].ucDate = systime.ucDate;
    g_stBfdEventRecord[g_usRecordNum].ucSecond = systime.ucSecond;
    g_stBfdEventRecord[g_usRecordNum].us5Ms = systime.us5Ms;
    g_stBfdEventRecord[g_usRecordNum].ulDisrim = g_stBfdlocal[usLoopi].ulDiscrimer;
    g_stBfdEventRecord[g_usRecordNum].ucLastState = pstBfdEventDate->ucLastSessionState;
    g_stBfdEventRecord[g_usRecordNum].ucState = g_stBfdlocal[usLoopi].ucSessionState;
    g_stBfdEventRecord[g_usRecordNum].ucDiag = g_stBfdlocal[usLoopi].ucLocalDiag;
    g_stBfdEventRecord[g_usRecordNum].ucRxPacketState = pstBfdEventDate->ucRxPacketState;
    g_stBfdEventRecord[g_usRecordNum].usLoopi = usLoopi;

    if (TRUE ==  bIsDown)
    {
        g_stBfdEventRecord[g_usRecordNum].ucState = BFD_DOWN;
    }

    g_usRecordNum++;
    if(g_usRecordNum == BFD_MAX_SESNUM)
    {
        g_usRecordNum = 0;
        g_usRecordNum++;
    }
    
    return;
}



/*****************************************************************************
 函 数 名  : clear_bfd_event
 功能描述  :  清楚bfd event
 输入参数  : void          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
NBB_VOID clear_bfd_event(NBB_VOID)
{
    OS_MEMSET(g_stBfdEventRecord ,0 , sizeof(BFD_EVENT_INFO) * BFD_MAX_SESNUM);
    g_usRecordNum = 0;
    return;
}

NBB_VOID spm_bfd_report_sessioninfo(IN NBB_ULONG ulDiscrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_BYTE  *pucMsgInfo = NULL;
    NBB_USHORT usMsgLenth = 0;
    NBB_USHORT usSessionNum = 0;
    BFD_REPORT_INFO stReportInfo[BFD_MAX_SESNUM];
    NBB_USHORT usData = 0;
    NBB_ULONG  ulDIdata = 0;
    NBB_USHORT *pstDatatmp = NULL;
    NBB_BYTE ucLocalSlot;
    NBB_BYTE ucMainSlot;
    
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
            return;
        }
    }
    
    OS_MEMSET(stReportInfo, 0, sizeof(BFD_REPORT_INFO)*BFD_MAX_SESNUM);    
    if (0 == ulDiscrim) /* 报告本盘所有会话当前状态 */
    {
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            if (DRV_ULONG_INVALID != g_stBfdlocal[usLoopi].ulDiscrimer)
            {
                stReportInfo[usSessionNum].ulLocalDisrim = g_stBfdlocal[usLoopi].ulDiscrimer;
                stReportInfo[usSessionNum].ulRemoteDisrim = g_stBfdlocal[usLoopi].ulRemoteDiscr;
                stReportInfo[usSessionNum].ucSessionState = g_stBfdlocal[usLoopi].ucSessionState;
                stReportInfo[usSessionNum].ucDiag = g_stBfdlocal[usLoopi].ucLocalDiag;
                if (BFD_DOWN == stReportInfo[usSessionNum].ucSessionState)
                {
                    stReportInfo[usSessionNum].ucDMTI = g_stBfdlocal[usLoopi].ucDMTI;
                    stReportInfo[usSessionNum].ucRMRI = 8;
                }
                else
                {
                    usData = spm_oamtree_s16read(BFDPAKFGPABASE, usLoopi + 0xe000);
                    stReportInfo[usSessionNum].ucDMTI = usData & 0xf;
                    
                    usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1800);
                    ulDIdata = usData;
                    usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1000);
                    ulDIdata = (ulDIdata << 16) + usData;
                    switch (ulDIdata)
                    {
                        case 4:
                            stReportInfo[usSessionNum].ucRMRI = BFD_3MS3;
                            break;
                        case 10:
                            stReportInfo[usSessionNum].ucRMRI = BFD_10MS;
                            break;
                        case 20:
                            stReportInfo[usSessionNum].ucRMRI = BFD_20MS;
                            break;
                        case 30:
                            stReportInfo[usSessionNum].ucRMRI = BFD_30MS;
                            break;
                        case 40:
                            stReportInfo[usSessionNum].ucRMRI = BFD_40MS;
                            break;
                        case 50:
                            stReportInfo[usSessionNum].ucRMRI = BFD_50MS;
                            break;
                        case 60:
                            stReportInfo[usSessionNum].ucRMRI = BFD_60MS;
                            break;
                        case 70:
                            stReportInfo[usSessionNum].ucRMRI = BFD_70MS;
                            break;
                        case 80:
                            stReportInfo[usSessionNum].ucRMRI = BFD_80MS;
                            break;
                        case 90:
                            stReportInfo[usSessionNum].ucRMRI = BFD_90MS;
                            break;
                        case 100:
                            stReportInfo[usSessionNum].ucRMRI = BFD_100MS;
                            break;
                        case 200:
                            stReportInfo[usSessionNum].ucRMRI = BFD_200MS;
                            break;
                        case 300:
                            stReportInfo[usSessionNum].ucRMRI = BFD_300MS;
                            break;
                        case 400:
                            stReportInfo[usSessionNum].ucRMRI = BFD_400MS;
                            break;
                        case 500:
                            stReportInfo[usSessionNum].ucRMRI = BFD_500MS;
                            break;
                        case 600:
                            stReportInfo[usSessionNum].ucRMRI = BFD_600MS;
                            break;
                        case 700:
                            stReportInfo[usSessionNum].ucRMRI = BFD_700MS;
                            break;
                        case 800:
                            stReportInfo[usSessionNum].ucRMRI = BFD_800MS;
                            break;
                        case 900:
                            stReportInfo[usSessionNum].ucRMRI = BFD_900MS;
                            break;
                        case  1000:
                            stReportInfo[usSessionNum].ucRMRI = BFD_1000MS;
                            break;
                        default :
                            stReportInfo[usSessionNum].ucRMRI = BFD_1000MS;
                            break;
                    }
                        
                }
                usSessionNum++;
            }
        }
    }
    else
    {
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            if (ulDiscrim == g_stBfdlocal[usLoopi].ulDiscrimer)
            {
                stReportInfo[usSessionNum].ulLocalDisrim = g_stBfdlocal[usLoopi].ulDiscrimer;
                stReportInfo[usSessionNum].ulRemoteDisrim = g_stBfdlocal[usLoopi].ulRemoteDiscr;
                stReportInfo[usSessionNum].ucSessionState = g_stBfdlocal[usLoopi].ucSessionState;
                stReportInfo[usSessionNum].ucDiag = g_stBfdlocal[usLoopi].ucLocalDiag;
                if (BFD_DOWN == stReportInfo[usSessionNum].ucSessionState)
                {
                    stReportInfo[usSessionNum].ucDMTI = g_stBfdlocal[usLoopi].ucDMTI;
                    stReportInfo[usSessionNum].ucRMRI = 8;
                }
                else
                {
                    usData = spm_oamtree_s16read(BFDPAKFGPABASE, usLoopi + 0xe000);
                    stReportInfo[usSessionNum].ucDMTI = usData & 0xf;
                    
                    usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1800);
                    ulDIdata = usData;
                    usData = spm_oamtree_s16read(BFDSESSIONBASE, usLoopi + 0x1000);
                    ulDIdata = (ulDIdata << 16) + usData;
                    switch (ulDIdata)
                    {
                        case 4:
                            stReportInfo[usSessionNum].ucRMRI = BFD_3MS3;
                            break;
                        case 10:
                            stReportInfo[usSessionNum].ucRMRI = BFD_10MS;
                            break;
                        case 20:
                            stReportInfo[usSessionNum].ucRMRI = BFD_20MS;
                            break;
                        case 30:
                            stReportInfo[usSessionNum].ucRMRI = BFD_30MS;
                            break;
                        case 40:
                            stReportInfo[usSessionNum].ucRMRI = BFD_40MS;
                            break;
                        case 50:
                            stReportInfo[usSessionNum].ucRMRI = BFD_50MS;
                            break;
                        case 60:
                            stReportInfo[usSessionNum].ucRMRI = BFD_60MS;
                            break;
                        case 70:
                            stReportInfo[usSessionNum].ucRMRI = BFD_70MS;
                            break;
                        case 80:
                            stReportInfo[usSessionNum].ucRMRI = BFD_80MS;
                            break;
                        case 90:
                            stReportInfo[usSessionNum].ucRMRI = BFD_90MS;
                            break;
                        case 100:
                            stReportInfo[usSessionNum].ucRMRI = BFD_100MS;
                            break;
                        case 200:
                            stReportInfo[usSessionNum].ucRMRI = BFD_200MS;
                            break;
                        case 300:
                            stReportInfo[usSessionNum].ucRMRI = BFD_300MS;
                            break;
                        case 400:
                            stReportInfo[usSessionNum].ucRMRI = BFD_400MS;
                            break;
                        case 500:
                            stReportInfo[usSessionNum].ucRMRI = BFD_500MS;
                            break;
                        case 600:
                            stReportInfo[usSessionNum].ucRMRI = BFD_600MS;
                            break;
                        case 700:
                            stReportInfo[usSessionNum].ucRMRI = BFD_700MS;
                            break;
                        case 800:
                            stReportInfo[usSessionNum].ucRMRI = BFD_800MS;
                            break;
                        case 900:
                            stReportInfo[usSessionNum].ucRMRI = BFD_900MS;
                            break;
                        case  1000:
                            stReportInfo[usSessionNum].ucRMRI = BFD_1000MS;
                            break;
                        default :
                            stReportInfo[usSessionNum].ucRMRI = BFD_1000MS;
                            break;
                    }
                        
                }
                usSessionNum++;
                break;
            }
        }
    }

    if (0 != usSessionNum)
    {
        usMsgLenth = 24 + 6 + BFD_ALARM_LENTH * usSessionNum;
        pucMsgInfo = (NBB_BYTE *)NBB_MM_ALLOC(usMsgLenth, NBB_NORETRY_ACT, 0);
        if (NULL == pucMsgInfo)
        {
            return;
        }
        pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 24);
        *pstDatatmp = 0x6F;
        pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 2 + 24);
        *pstDatatmp = 2 + BFD_ALARM_LENTH * usSessionNum;
        pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 4 + 24);
        *pstDatatmp = usSessionNum;
        NBB_MEMCPY(pucMsgInfo + 6 + 24, stReportInfo, BFD_ALARM_LENTH * usSessionNum);

        /* 反馈信息 */
        DipcSendData(ucMainSlot, 1, 0x123457, (usMsgLenth - 24), pucMsgInfo);
        NBB_MM_FREE(pucMsgInfo, 0);
    }
    return;
}

/*****************************************************************************
 * 函 数 名  : bfdlog
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2013年12月11日
 * 函数功能  : 将记录在内存中的数据立即写入到日志文件中
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfdlog()
{
    BMU_SLOG(BMU_CRIT, SPM_BFD_LOG_DIR, "\r\n");
    return;
}

#if defined(SPU)

/*****************************************************************************
 * 函 数 名  : bfd_show_hdlc_bfd_state
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月18日
 * 函数功能  : 显示HDLC的BFD告警信息
 * 输入参数  : IN NBB_USHORT usStartMid     起始的MID值
               IN NBB_USHORT usEndMid       结束的MID值
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfd_show_hdlc_bfd_recrd(IN NBB_ULONG ulStartMid, IN NBB_ULONG ulEndMid)
{
    en_BfdRcordSta_T eRcordSta = BFD_RECORD_INVALID;
    NBB_ULONG ulLoopi = 0;
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;
    NBB_USHORT usLoopj = 0;
    en_BfdRcordSta_T eLocalCfg = BFD_RECORD_INVALID;
    en_BfdRcordSta_T eFpgaSend = BFD_RECORD_INVALID;
    en_BfdRcordSta_T eFpgaRcv = BFD_RECORD_INVALID;
    NBB_UINT uiDataSend = 0;
    NBB_UINT uiDataRcv = 0;
    NBB_USHORT usArrOff = 0;
    NBB_USHORT usDateOff = 0;
    NBB_BYTE ucLocalSlot = 0;
    NBB_USHORT usSlotid = 0;
    
    /* BFD状态记录信息与实际值的映射表 */
    NBB_CHAR *lpszRecord[] = {"invalid",
                                "up",
                                "down",
                                "none"};

    NBB_USHORT usRecordNum = sizeof(lpszRecord) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */

    /* 入参判断 */
    if ((ulStartMid > BFD_MAX_ID) || (0 == ulStartMid) || (ulEndMid > BFD_MAX_ID) || (0 == ulEndMid))
    {
        NBB_PRINTF("Please check input, out of range ulStartMid:%lu, ulEndMid:%lu\r\n",
            ulStartMid, ulEndMid);
        return;
    }
    
    /* 获取本盘槽位 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 输出表格头 */
    NBB_PRINTF("bfd Mid | slot | local cfg | fpga send | fpga rev \r\n");
    NBB_PRINTF("--------|------|-----------|-----------|----------\r\n");
    
    /* 循环输出内容 */
    for (ulLoopi = ulStartMid; ulLoopi < ulEndMid; ulLoopi++)
    {
        pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulLoopi);
        if (NULL != pstBfdCfg) /* 说明会话存在 */
        {
            usSlotid = pstBfdCfg->usSlotid;
            if(ucLocalSlot == usSlotid)  /* 会话是本盘处理 */
            {
                for (usLoopj = 0; usLoopj < BFD_MAX_SESNUM; usLoopj++)
                {
                    if (ulLoopi == g_stBfdlocal[usLoopj].ulDiscrimer)/* 相等则输出 */
                    {
                        eLocalCfg = g_stBfdlocal[usLoopj].ucStateRecord;

                        usArrOff = usLoopj >> 4;            /* 16的除数 */
                        usDateOff = (usLoopj & 0xf) << 1;   /* 16的余数乘2 */
                        ReadFpga32b(BFDSTATEBASE + 0x10000 + usArrOff, &uiDataSend);
                        eFpgaSend = (uiDataSend & (3 << usDateOff)) >> usDateOff;
                        break;
                    }
                }
                if(BFD_MAX_SESNUM == usLoopj)
                {
                    eLocalCfg = BFD_RECORD_BUTT;
                    eFpgaSend = BFD_RECORD_BUTT;
                }
            }
            else
            {
                eLocalCfg = BFD_RECORD_BUTT;
                eFpgaSend = BFD_RECORD_BUTT;
            }
        }
        else
        {
            usSlotid = 0;
            eLocalCfg = BFD_RECORD_BUTT;
            eFpgaSend = BFD_RECORD_BUTT;
        }
        
        usArrOff = (ulLoopi - 1) >> 4;            /* 16的除数 */
        usDateOff = ((ulLoopi - 1) & 0xf) << 1;   /* 16的余数乘2 */
        ReadFpga32b(BFDSTATEBASE + 0x20000 + usArrOff, &uiDataRcv);
        eFpgaRcv = (uiDataRcv & (3 << usDateOff)) >> usDateOff;
        
        NBB_PRINTF("%8lu|%6u|%11s|%11s|%10s\r\n",
            ulLoopi, usSlotid, lpszRecord[eLocalCfg], lpszRecord[eFpgaSend], lpszRecord[eFpgaRcv]);
    }
    return;
}

#elif defined(SRC)

/*****************************************************************************
 * 函 数 名  : bfd_show_hdlc_bfd_state
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月18日
 * 函数功能  : 显示HDLC的BFD告警信息
 * 输入参数  : IN NBB_USHORT usStartMid     起始的MID值
               IN NBB_USHORT usEndMid       结束的MID值
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfd_show_hdlc_bfd_recrd(IN NBB_ULONG ulStartMid, IN NBB_ULONG ulEndMid)
{
    en_BfdRcordSta_T eRcordSta = BFD_RECORD_INVALID;
    NBB_ULONG ulLoopi = 0;
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;
    NBB_USHORT usLoopj = 0;
    en_BfdRcordSta_T eLocalCfg = BFD_RECORD_INVALID;
    en_BfdRcordSta_T eFpgaSend = BFD_RECORD_INVALID;
    en_BfdRcordSta_T eFpgaRcv = BFD_RECORD_INVALID;
    en_BfdRcordSta_T eFpgaMergeSend = BFD_RECORD_INVALID;
    NBB_UINT uiDataSend = 0;
    NBB_UINT uiDataRcv = 0;
    NBB_UINT uiDataMergeSend = 0;
    NBB_USHORT usArrOff = 0;
    NBB_USHORT usDateOff = 0;
    NBB_BYTE ucLocalSlot = 0;
    NBB_USHORT usSlotid = 0;
    NBB_USHORT usLoopk = 0;
    
    /* BFD状态记录信息与实际值的映射表 */
    NBB_CHAR *lpszRecord[] = {"invalid",
                                "up",
                                "down",
                                "none"};

    NBB_USHORT usRecordNum = sizeof(lpszRecord) / 4;    /* 指针数组，取出来的长度是数组个数*4字节 */

    /* 入参判断 */
    if ((ulStartMid > BFD_MAX_ID) || (0 == ulStartMid) || (ulEndMid > BFD_MAX_ID) || (0 == ulEndMid))
    {
        NBB_PRINTF("Please check input, out of range ulStartMid:%lu, ulEndMid:%lu\r\n",
            ulStartMid, ulEndMid);
        return;
    }
    
    /* 获取本盘槽位 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 输出表格头 */
    NBB_PRINTF("bfd Mid | slot | local cfg | fpga send | fpga rev | fpga merge send \r\n");
    NBB_PRINTF("--------|------|-----------|-----------|----------|-----------------\r\n");
    
    /* 循环输出内容 */
    for (ulLoopi = ulStartMid; ulLoopi < ulEndMid; ulLoopi++)
    {
        pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulLoopi);
        if (NULL != pstBfdCfg) /* 说明会话存在 */
        {
            usSlotid = pstBfdCfg->usSlotid;
            if(ucLocalSlot == usSlotid)  /* 会话是本盘处理 */
            {
                eFpgaRcv = BFD_RECORD_BUTT;
                
                for (usLoopj = 0; usLoopj < BFD_MAX_SESNUM; usLoopj++)
                {
                    if (ulLoopi == g_stBfdlocal[usLoopj].ulDiscrimer)/* 相等则输出 */
                    {
                        eLocalCfg = g_stBfdlocal[usLoopj].ucStateRecord;

                        /* 2bit读写 */
                        usArrOff = usLoopj;            
                        usDateOff = 0;
                        ReadFpga32b(BFDSTATEBASE + 0x60000 + usArrOff, &uiDataSend);
                        eFpgaSend = (uiDataSend & (3 << usDateOff)) >> usDateOff;
                        break;
                    }
                }
                if(BFD_MAX_SESNUM == usLoopj)
                {
                    eLocalCfg = BFD_RECORD_BUTT;
                    eFpgaSend = BFD_RECORD_BUTT;
                }
            }
            else
            {
                eLocalCfg = BFD_RECORD_BUTT;
                eFpgaSend = BFD_RECORD_BUTT;

                if ((usSlotid > 0) && (usSlotid < SYS_MAX_SLOTNUM))
                {
                    for (usLoopk = 0; usLoopk < BFD_MAX_SESNUM; usLoopk++)
                    {
                        if (ulLoopi == g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopk].ulDiscrimer)
                        {
                            /* 8位读写 */
                            usArrOff = (((usSlotid - 1) << 10) + usLoopk) >> 2;               /* 4的除数 */
                            usDateOff = ((((usSlotid - 1) << 10) + usLoopk) & 0x3) << 1;      /* 4的余数乘2 */
                            ReadFpga32b(BFDSTATEBASE + 0x20000 + usArrOff, &uiDataRcv);
                            eFpgaRcv = (uiDataRcv & (3 << usDateOff)) >> usDateOff;
                            break;
                        }
                    }
                    if(BFD_MAX_SESNUM == usLoopk)
                    {
                        eFpgaRcv = BFD_RECORD_BUTT;
                    }
                }
                else
                {
                    eFpgaRcv = BFD_RECORD_BUTT;
                }
            }
        }
        else
        {
            usSlotid = 0;
            eLocalCfg = BFD_RECORD_BUTT;
            eFpgaSend = BFD_RECORD_BUTT;
            eFpgaRcv = BFD_RECORD_BUTT;
        }
        
        usArrOff = (ulLoopi - 1) >> 4;            /* 16的除数 */
        usDateOff = ((ulLoopi - 1) & 0xf) << 1;   /* 16的余数乘2 */
        ReadFpga32b(BFDSTATEBASE + 0x50000 + usArrOff, &uiDataMergeSend);
        eFpgaMergeSend = (uiDataMergeSend & (3 << usDateOff)) >> usDateOff;
        
        NBB_PRINTF("%8lu|%6u|%11s|%11s|%10s|%17s\r\n",
            ulLoopi, usSlotid, lpszRecord[eLocalCfg],
            lpszRecord[eFpgaSend], lpszRecord[eFpgaRcv], lpszRecord[eFpgaMergeSend]);
    }
    return;
}

#else

/*****************************************************************************
 * 函 数 名  : bfd_show_hdlc_bfd_state
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月18日
 * 函数功能  : 显示HDLC的BFD告警信息
 * 输入参数  : IN NBB_USHORT usStartMid     起始的MID值
               IN NBB_USHORT usEndMid       结束的MID值
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfd_show_hdlc_bfd_recrd(IN NBB_ULONG ulStartMid, IN NBB_ULONG ulEndMid)
{
    return;
}

#endif

/*****************************************************************************
 * 函 数 名  : set_delay_to_make_bfd_down_before_port_shutdown
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2015年2月13日
 * 函数功能  : 配置发送BFD之后关闭端口的延时时间
 * 输入参数  : tick  延时时间,5ms为1tick。其中0为shutdown不发送BFD
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID set_delay_to_make_bfd_down_before_port_shutdown(tick)
{
    g_make_bfd_down_before_port_shutdown = tick;
}

/*****************************************************************************
 * 函 数 名  : bfd_shutdown_port
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2015年2月13日
 * 函数功能  : shutdown端口前先发BFD down包
 * 输入参数  : IN NBB_BYTE ucCardId  子卡号
               IN NBB_BYTE ucPortId  端口号
               IN NBB_BYTE ucEnable  使能开关
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID bfd_shutdown_port(IN NBB_BYTE card_id, IN NBB_BYTE port_id, IN NBB_BYTE enable)
{
    SPM_PORT_INFO_CB port_info;
    NBB_USHORT i = 0;
    NBB_INT ret;
    NBB_USHORT   data = 0;
    SPM_BFD_REG_PACKET2 packet_info2;
    NBB_USHORT local_num = 0;

    if (0 != enable)
    {
        goto EXIT_LABEL;
    }
    
    if(0 == g_make_bfd_down_before_port_shutdown)
    {
        goto EXIT_LABEL;
    }
    
    for (i = 0; i < BFD_MAX_SESNUM; i++) /* 对变量i进行循环 */
    {
        if(DRV_ULONG_INVALID != g_stBfdlocal[i].ulDiscrimer)
        {
            ret = spm_get_portid_from_logical_port_index(g_stBfdlocal[i].ulPortIndex, &port_info);
            if(SUCCESS != ret)
            {
                continue;
            }
            
            if(port_info.card_id == card_id && port_info.phy_port == port_id)
            {
                OS_MEMSET(&packet_info2, 0, sizeof(SPM_BFD_REG_PACKET2));
                packet_info2.usDiag = bfd_diag_ctrl_detect_exp;
                packet_info2.usSta = BFD_DOWN;
                packet_info2.usDectMult = g_stBfdlocal[i].ucDetectMult;
                data = *(NBB_USHORT *)(&packet_info2);
                
                /* BFD发送模块寄存器偏移0xd000+2*N+0x0：
                   Bit15-11：BFD报文诊断码（Diag），表明发送方最近一次会话down的原因。
                   Bit10-9：BFD当前会话状态（Sta），0-AdminDown，1-Down，2-Init，3-Up。
                   Bit7-0：检测时间倍数（Detect Mult）。默认值为3 */
                spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * i + 0x0, data);

                local_num = 0x8000 + i;
                
                /* BFD发送模块寄存器偏移0x0002：
                   Bit15: 修改BFD报文中5种动态参数（Diag/ Sta/ Detect Mult/DMTI/RMRI）标识。由软件在每条配置5种动态参数完成后写'1'。
                   Bit10-0：所配置BFD报文5种动态参数的条目号。由软件在配置参数前写入要配置参数的条目号。*/
                spm_oamtree_s16write(BFDPAKFGPABASE, 0x0002, local_num);

                BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, "make the %uth bfd myid = %u down\r\n",
                    i, g_stBfdlocal[i].ulDiscrimer);
            }
        }
    }


    BmuTaskDelay(g_make_bfd_down_before_port_shutdown);
    
EXIT_LABEL:
    return;
}

/*****************************************************************************
 * 函 数 名  : bfd_convert
 * 负 责 人  : tianf
 * 创建日期  : 2015年12月2日
 * 函数功能  : 进制转换输出
 * 输入参数  : NBB_INT Date
 * 输出参数  : 无
 * 返 回 值  : SUCCESS/ERROR
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT bfd_convert(NBB_INT Date)
{
	NBB_INT usLoopi = 0;
    NBB_INT iRet = SUCCESS;
	
	if(Date < 0)
	{
		NBB_PRINTF("Input number must > 0!\n");
        iRet = ERROR;
		goto EXIT_LABEL;
	}
    
	NBB_PRINTF("Hexadecimal: %x\n",Date);
	NBB_PRINTF("Decimal: %d\n",Date);

	//NBB_PRINTF("Octonary: 0%o\n",Date);
	
	/*二进制输出 start*/
    
	NBB_PRINTF("Binary:");
	for(usLoopi = 31; usLoopi > 0; usLoopi--)
	{
		if(0 == ((usLoopi + 1) % 4))
		{
			NBB_PRINTF(" ");
		}	
		NBB_PRINTF("%d", (Date >> usLoopi) & 0x1);	
	}
	NBB_PRINTF("%d\n", Date & 0x1);
    
	/*二进制输出 end*/
    
	EXIT_LABEL:
	return iRet;	
}



