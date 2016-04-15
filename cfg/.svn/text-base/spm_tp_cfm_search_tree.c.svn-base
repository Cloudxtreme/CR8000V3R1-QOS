/******************************************************************************

  Copyright (C), 1999-2014, fiberhome Co., Ltd.

 ******************************************************************************
  File Name     : spm_tp_cfm_search_tree.c
  Version       : Initial Draft
  Author        : huxi
  Created       : 2014/9/3
  Last Modified :
  Description   : spm_tp_cfm_search_tree
  Function List :
  History       :
  1.Date        : 2014/9/3
    Author      : huxi
    Modification: Created file

******************************************************************************/
#include <nbase.h>
#include <spmincl.h>

/*****************************************************************************
 函 数 名  : spm_oam_tree_delete
 功能描述  : 删除配置，搜索树的删除
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月28日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_tree_delete(IN SPM_OAMTREE_NODE *pstoamtreenode,
                            IN NBB_ULONG uloamtreebase,
                            IN NBB_BYTE module,
                            IN NBB_BYTE flag)
{
    NBB_INT iret = ERROR;

    if (0x0 == module) //OAM模块0
    {
        iret = spm_oamtree_nodedel(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset0);
    }
    else
    {
        iret = spm_oamtree_nodedel(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset1);
    }

    /* 判断结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "Del oam tree node failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x0 == module)
    {
        /* 重建二叉树 */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* 判断执行结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "Rebuild oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 搜索表切换开关 */
    spm_oam_16write(uloamtreebase, TREE_USED_OFFSET , 0X1);

    if (0x0 == module)
    {
        /* 写备表 */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        /* 写备表 */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* 判断执行结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "Rebuild oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

EXIT_LABEL:

    return iret;
}


/*****************************************************************************
   函 数 名  : spm_oam_searchtree_build
   功能描述  : 建立二叉树
   输入参数  : IN SPM_OAMTREE_NEWSET *psttreenodeset:二叉树
             IN NBB_USHORT ustreelevelmax:二叉树最大层数
             IN NBB_ULONG *pultreebaseadd:二叉树每层的起始地址
             IN NBB_ULONG ulbaseaddress:二叉树的基地址
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月27日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_searchtree_build(
    IN NBB_USHORT ustreelevelmax,
    IN SPM_OAMTREE_SET *psttreenodeset,
    IN NBB_ULONG *pultreebaseadd,
    IN NBB_ULONG ulbaseaddress,
    IN NBB_BYTE flag
    NBB_CCXT_T NBB_CCXT)
{
    NBB_USHORT ustreelevel = 0; /* 二叉树的层数 */
    NBB_USHORT uslevellength = 0; /* 二叉树的节点号 */
    NBB_USHORT usindex = 0; /* 对应的数组的序号 */
    NBB_ULONG  uladdress = 0;
    NBB_USHORT ustemploop = 0; /* 循环变量 */
    NBB_USHORT ustemplabel = 0;
    NBB_INT    iret = ERROR;
    NBB_USHORT mepi = 0;

    /* 合法性检查 */
    if ((NULL == psttreenodeset) || (NULL == pultreebaseadd))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n input pointer illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = -2;
        goto EXIT_LABEL;
    }

    /* 循环遍历 */
    for (ustreelevel = 0; ustreelevel < ustreelevelmax; ustreelevel++)
    {
        pultreebaseadd[ustreelevel] = (((1 << ustreelevelmax) - 2) << (ustreelevel + 2))
                                      & ((1 << ustreelevelmax) * 4 - 1);

        iret = spm_oamtree_gettreelen(ustreelevel, ustreelevelmax, &uslevellength);

        if (SUCCESS != iret) /* 判断执行结果 */
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n get tree len error ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            iret = -3;
            goto EXIT_LABEL;
        }

        /* 循环 */
        for (ustemploop = 0; ustemploop < uslevellength; ustemploop = ustemploop + 4)
        {
            iret = spm_oamtree_getarridx(ustreelevel, ustemploop, ustreelevelmax, &usindex);

            if (SUCCESS != iret) /* 判断执行结果 */
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n Get tree Index error ret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                iret = -4;
                goto EXIT_LABEL;
            }

            uladdress = pultreebaseadd[ustreelevel] + ustemploop;

            /*  索引号 */
            ustemplabel = psttreenodeset->pstNodeArr[usindex].usLspNo;
            spm_oam_16write(ulbaseaddress, uladdress, ustemplabel);
            mepi = ustemplabel;

            if (0 == flag)
            {
                /*  TUNNEL  LABEL    */
                ustemplabel = (NBB_USHORT)((psttreenodeset->pstNodeArr[usindex].ulTunnelId >> 8) & 0x1fff);
                spm_oam_16write(ulbaseaddress, uladdress + 1, ustemplabel);

                ustemplabel = (NBB_USHORT)(((psttreenodeset->pstNodeArr[usindex].ulTunnelId << 8) & 0xff00)
                                           + (NBB_USHORT)((psttreenodeset->pstNodeArr[usindex].ulPwId >> 16) & 0x1f));
                spm_oam_16write(ulbaseaddress, uladdress + 2, ustemplabel);

                /*  PW  LABEL     */
                ustemplabel = (NBB_USHORT)(psttreenodeset->pstNodeArr[usindex].ulPwId & 0xffff);
                spm_oam_16write(ulbaseaddress, uladdress + 3, ustemplabel);
            }
            else
            {
                /*  vid1    */
                ustemplabel = (NBB_USHORT)(psttreenodeset->pstNodeArr[usindex].ulTunnelId & 0x00000fff) + (1 << 12);
                spm_oam_16write(ulbaseaddress, uladdress + 1, ustemplabel);

                /*  vid2    */
                ustemplabel = (NBB_USHORT)(psttreenodeset->pstNodeArr[usindex].ulPwId & 0x00000fff);
                ustemplabel = (ustemplabel & 0x000f) + ((ustemplabel & 0xfff0) << 4);
                spm_oam_16write(ulbaseaddress, uladdress + 2, ustemplabel);

                /*  vid3 + mel */
                //ustemplabel = CFMMEP[mepi].mdlevel;
                //spm_oam_16write(ulbaseaddress, uladdress + 3, ustemplabel);
            }

        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 函 数 名  : spm_oam_tree_build
 功能描述  : 二叉搜索树添加节点和建树的过程
 输入参数  : 无
 输出参数  : 无
 返 回 值  : ERROR/SUCCESS
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月23日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_tree_build(IN SPM_OAMTREE_NODE *pstoamtreenode,
                           IN NBB_ULONG uloamtreebase,
                           IN NBB_BYTE module,
                           IN NBB_BYTE flag
                           NBB_CCXT_T NBB_CXT)
{
    NBB_INT iret = ERROR;
    
    if (0x0 ==  module) //OAM模块0
    {
        iret = spm_oamtree_nodeadd(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset0);
    }
    else //OAM模块1
    {
        iret = spm_oamtree_nodeadd(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset1);
    }

    /* 二叉树添加节点 */

    /* 判断执行结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n\n"
                   "Add node failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x0 == module)
    {
        /* 重建二叉树 */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* 判断执行结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n\n"
                   "Build oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    //spm_oam_change_search_tree(uloamtreebase);
    
    /* 搜索表切换开关 */
    spm_oam_16write(uloamtreebase , TREE_USED_OFFSET , 0X1);

    /* 写备表 */
    if (0x0 == module)
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* 判断执行结果 */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n\n"
                   "Build oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

EXIT_LABEL:

    return iret;
}


/*****************************************************************************
 函 数 名  : spm_oam_16write
 功能描述  : OAM模块FPGA写函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月3日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_16write(NBB_ULONG base_addr,
                         NBB_ULONG offset_addr,
                         NBB_USHORT data
                         NBB_CCXT_T NBB_CCXT)
{
#if SPU
#if 0
    if (MIX1LP4 == g_stHardwBoardInf.enBoardName
        || MIX10LP2 == g_stHardwBoardInf.enBoardName
        || MIX1OLP2 == g_stHardwBoardInf.enBoardName
        || MSA1LN4  == g_stHardwBoardInf.enBoardName)
    {
        (NBB_VOID)WriteFpga32b(base_addr + offset_addr, data);
    }
    else
    {
        spm_oamtree_s16write(base_addr, offset_addr, data);
    }
#endif
    spm_oamtree_s16write(base_addr, offset_addr, data);

#endif
#if SRC
    spm_ccu_32write(base_addr, offset_addr, data);
#endif
    return;
}



/*****************************************************************************
 函 数 名  : spm_oam_16read
 功能描述  : OAM模块FPGA读函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_USHORT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月3日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_USHORT spm_oam_16read(NBB_ULONG base_addr,
                          NBB_ULONG offset_addr
                          NBB_CCXT_T NBB_CCXT)
{
    NBB_USHORT data = 0;
#if SPU
#if 0
    if (MIX1LP4 == g_stHardwBoardInf.enBoardName
        || MIX10LP2 == g_stHardwBoardInf.enBoardName
        || MIX1OLP2 == g_stHardwBoardInf.enBoardName
        || MSA1LN4  == g_stHardwBoardInf.enBoardName)
    {
        data = spm_oam_32read(base_addr, offset_addr);
    }
    else
    {
        data = spm_oamtree_s16read(base_addr, offset_addr);
    }
#endif
    data = spm_oamtree_s16read(base_addr, offset_addr);
#endif
#if SRC
    data = spm_ccu_32read(base_addr, offset_addr);
#endif
    return data;
}



/*****************************************************************************
 函 数 名  : spm_oam_32read
 功能描述  : FPGA寄存器32位读操作
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_ULONG
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年1月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_oam_32read(NBB_ULONG base_addr,
                         NBB_ULONG offset_addr
                         NBB_CCXT_T NBB_CCXT)
{
    NBB_UINT uldata = 0;
    NBB_UINT uioffset = 0;

    uioffset = (NBB_INT)(base_addr + offset_addr);
    (NBB_VOID)ReadFpga32b(uioffset, &uldata);

    return uldata;
}

/*****************************************************************************
 Prototype    : spm_assign_oam_index
 Description  : 分配cfm和tp在FPGA的索引
 Input        : NBB_INT
 Output       : None
 Return Value : NBB_USHORT *oamindex
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/3
    Author       : huxi
    Modification : Created function
*****************************************************************************/
NBB_INT spm_assign_oam_index(OUT NBB_USHORT *oamindex,
                    OUT NBB_USHORT *moduleindex,
                    OUT NBB_BYTE *module)
{
    NBB_INT iret = ERROR;
    NBB_USHORT uli = 0;
    if ((NULL == oamindex) || (NULL == moduleindex) || (NULL == module))
    {
        goto EXIT_LABEL;
    }
    
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    for (uli = 0; uli < OAM_MAX_NUM_ACTIVE_SPU; uli++)
    {
        if (FALSE == g_tp_cfmindex[uli].isIndexEnable)
        {
            *oamindex = uli;
            g_tp_cfmindex[uli].isIndexEnable = TRUE;
            if (uli < 2047)
            {
                *moduleindex = uli;
                *module = 0;
            }
            else
            {
                *moduleindex = uli - 2048;
                *module = 1;
            }
            iret = SUCCESS;
            break;
        }
    }        
    if(uli == OAM_MAX_NUM_ACTIVE_SPU)
    {
        printf("fpga is full!\n");
        goto EXIT_LABEL;
    }

EXIT_LABEL:

    return iret;
}


/*****************************************************************************
 Prototype    : spm_recycle_oam_index
 Description  : 回收tp和cfm在FPGA的索引号
 Input        : NBB_USHORT oamindex
 Output       : None
 Return Value : NBB_INT
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/9/3
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_recycle_oam_index(NBB_USHORT oamindex)
{
    NBB_INT iret = ERROR;

    if (oamindex >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    if (g_tp_cfmindex[oamindex].isIndexEnable == TRUE)
    {
        g_tp_cfmindex[oamindex].isIndexEnable = FALSE;
        iret = SUCCESS;
    }

EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 Prototype    : spm_oam_change_search_tree
 Description  : FPGA搜索树切表函数
 Input        : NBB_ULONG base_addr
 Output       : None
 Return Value : NBB_INT
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/10/16
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_oam_change_search_tree(NBB_ULONG base_addr)
{
    NBB_USHORT temp = 0;
    NBB_BYTE tree_no_last = 0xff;
    NBB_BYTE tree_no_now = 0xff;
    temp = spm_oam_16read(base_addr, TREE_USED_OFFSET - 1);
    tree_no_last = (temp >> 4) & 0x1;
    do
    {
        /* 搜索表切换开关 */
        temp = spm_oam_16read(base_addr,TREE_USED_OFFSET);
        temp |= 0x8;
        spm_oam_16write(base_addr , TREE_USED_OFFSET , temp);
        
        temp = spm_oam_16read(base_addr, TREE_USED_OFFSET - 1);
        tree_no_now = (temp >> 4) & 0x1;
    }while(!(tree_no_last ^ tree_no_now));
    return SUCCESS;
}

