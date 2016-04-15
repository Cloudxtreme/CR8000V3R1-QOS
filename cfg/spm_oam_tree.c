/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_oam_tree.c
   版 本 号   : 初稿
   作    者   : limingming
   生成日期   : 2012年9月24日
   最近修改   :
   功能描述   : FPGA二叉树处理模块，供BFD和OAM共同使用
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月24日
    作    者   : limingming
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
extern int  ReadFpgaMmap(unsigned int offset, unsigned short *data);
extern int  WriteFpgaMmap(unsigned int offset, unsigned short data);
extern NBB_INT WriteFpga32b(IN NBB_INT iOffset, IN NBB_UINT uiData);

/* 清配置标记 */
extern NBB_BYTE g_ucCleanCfgCheck;

/*****************************************************************************
   函 数 名  : spm_oamtree_init
   功能描述  : OAM/BFD二叉树初始化
   输入参数  : IN    NBB_USHORT  usTreeNodeMaxNum:二叉树的最大节点数
               IN SPM_OAMTREE_GET_DATA_PFN pfnGetDate:二叉树将本地存储数据转换成FPGA需要的数据的函数指针
             INOUT SPM_OAMTREE_SET *pstTreeANodeSet : 二叉树
   输出参数  : INOUT SPM_OAMTREE_SET *pstTreeANodeSet
   返 回 值  : SUCCESS/ERROR
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月20日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oamtree_init(
    IN NBB_USHORT usTreeNodeMaxNum,
    IN SPM_OAMTREE_GET_DATA_PFN pfnGetDate,
    INOUT SPM_OAMTREE_SET *pstTreeANodeSet)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ulTmpNO = 0; /* 循环变量 */
    NBB_INT iRet = ERROR;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_init"));

    /***************************************************************************/
    /* 入参判断                                                                */
    /***************************************************************************/
    if ((NULL == pstTreeANodeSet) || (NULL == pfnGetDate))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 入参判断                                                                */
    /***************************************************************************/
    if (0 == usTreeNodeMaxNum)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Tree node is zero"));
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 初始化                                                                  */
    /***************************************************************************/
    pstTreeANodeSet->usNodeNum = 0;
    pstTreeANodeSet->pfnGetDate = pfnGetDate;

    pstTreeANodeSet->pstNodeArr = (SPM_OAMTREE_NODE *)NBB_MM_ALLOC(sizeof(SPM_OAMTREE_NODE) * usTreeNodeMaxNum,
        NBB_NORETRY_ACT, MEM_SPM_TPOAM_TREE_CB);

    /***************************************************************************/
    /* 内存申请失败                                                            */
    /***************************************************************************/
    if (NULL == pstTreeANodeSet->pstNodeArr)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        goto EXIT_LABEL;
    }

    /* 循环节点 */
    for (ulTmpNO = 0; ulTmpNO < usTreeNodeMaxNum; ulTmpNO++)
    {
        pstTreeANodeSet->pstNodeArr[ulTmpNO].ulTunnelId = 0xffffffff;
        pstTreeANodeSet->pstNodeArr[ulTmpNO].ulPwId = 0xffffffff;
        pstTreeANodeSet->pstNodeArr[ulTmpNO].usLspNo = 0;
    }

    iRet = SUCCESS;

    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}


/*****************************************************************************
   函 数 名  : spm_oamtree_nodeadd
   功能描述  : 二叉树，加入节点
   输入参数  : IN NBB_USHORT usLspNumMax : 最大条目数
             IN SPM_OAMTREE_NEWSET *pstTreeNode :需要插入的节点
   输出参数  : INOUT SPM_OAMTREE_SET *pstTree :二叉树
   返 回 值  : SUCCES/ERROR
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月20日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oamtree_nodeadd(
    IN NBB_USHORT usLspNumMax, 
    IN SPM_OAMTREE_NODE *pstTreeNode, 
    INOUT SPM_OAMTREE_SET *pstTree)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT iRet = ERROR;  /* 函数返回值 */
    NBB_USHORT usNodeNum = 0;
    NBB_USHORT usSearchIndex = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_nodeadd"));

    /***************************************************************************/
    /* 入参判断                                                                */
    /***************************************************************************/
    if ((NULL == pstTree) || (NULL == pstTreeNode))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        iRet = -1;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 入参判断                                                                */
    /***************************************************************************/
    if ( (pstTreeNode->usLspNo >= usLspNumMax) 
        || (pstTreeNode->ulPwId > TMCLABELMAX))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Insert node is invalid，p1 = %ld, p2 = %d, p3 = %ld, p4 = %d",
            pstTreeNode->ulTunnelId, pstTreeNode->usLspNo, pstTreeNode->ulPwId, usLspNumMax));
        iRet = -2;
        goto EXIT_LABEL;
    }

    /* 判断条件 */
    if (pstTree->usNodeNum > usLspNumMax)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Tree is full,p1 = %d,p2 = %d", pstTree->usNodeNum, usLspNumMax));
        iRet = -3;
        goto EXIT_LABEL;
    }

    /* 新节点在数组中查找 */
    iRet = spm_oamtree_nodesearch(pstTreeNode->ulTunnelId, pstTreeNode->ulPwId, pstTree, &usSearchIndex);

    if (SUCCESS == iRet) /* 已经存在节点 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "This node has been add in the tree idx = %d, tunnel = %d , pw = %d",
            usSearchIndex, pstTreeNode->ulTunnelId, pstTreeNode->ulPwId));
        iRet = -4;
        goto EXIT_LABEL;
    }

    /* 将新节点保存在数组的最后 */
    usNodeNum = pstTree->usNodeNum;
    pstTree->pstNodeArr[usNodeNum].ulTunnelId = pstTreeNode->ulTunnelId;
    pstTree->pstNodeArr[usNodeNum].usLspNo = pstTreeNode->usLspNo;
    pstTree->pstNodeArr[usNodeNum].ulPwId = pstTreeNode->ulPwId;
    pstTree->usNodeNum++;

    /* 给数组排序(按从小到大的顺序) */
    qsort(pstTree->pstNodeArr, pstTree->usNodeNum, sizeof(SPM_OAMTREE_NODE), spm_oamtree_nodecmp);

    iRet = SUCCESS;

    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
   函 数 名  : spm_oamtree_nodedel
   功能描述  : 二叉树，加入节点
   输入参数  : IN NBB_USHORT usLspNumMax : 最大条目数
             IN SPM_OAMTREE_NODE *pstTreeNode :需要删除的节点
   输出参数  : INOUT SPM_OAMTREE_SET *pstTree :二叉树
   返 回 值  : SUCCES/FAILED
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月20日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

  2.日    期   : 2015年7月6日
    作    者   : gaos
    修改内容   : 修改返错类型
*****************************************************************************/
NBB_INT spm_oamtree_nodedel(
    IN NBB_USHORT usLspNumMax, 
    IN SPM_OAMTREE_NODE *pstTreeNode, 
    INOUT SPM_OAMTREE_SET *pstTree)
{
    NBB_INT iRet = ERROR;  /* 函数返回值 */
    NBB_USHORT usSearchIndex = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_nodedel"));

    /* 入参判断 */
    if ((NULL == pstTree) || (NULL == pstTreeNode))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        iRet = -1;
        goto EXIT_LABEL;
    }

    /* 入参判断 */
    if ((pstTreeNode->usLspNo >= usLspNumMax) 
        || (pstTreeNode->ulPwId > TMCLABELMAX))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Del node is invalid，p1 = %ld, p2 = %d, p3 = %ld, p4 = %d",
            pstTreeNode->ulTunnelId, pstTreeNode->usLspNo, pstTreeNode->ulPwId, usLspNumMax));
        iRet = -2;
        goto EXIT_LABEL;
    }

    /* 若没有节点 */
    if (0 == pstTree->usNodeNum)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Tree is empty"));
        iRet = -3;
        goto EXIT_LABEL;
    }

    /* 新节点在数组中查找 */
    iRet = spm_oamtree_nodesearch(pstTreeNode->ulTunnelId, pstTreeNode->ulPwId, pstTree, &usSearchIndex);
    if (SUCCESS != iRet) /* 判断执行结果 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "This node is not in the tree idx = %d, tunnel = %d , pw = %d",
            usSearchIndex, pstTreeNode->ulTunnelId, pstTreeNode->ulPwId));
        iRet = -4;
        goto EXIT_LABEL;
    }

    /* 将二叉树中的节点置位无效值 */
    pstTree->pstNodeArr[usSearchIndex].usLspNo = pstTree->pstNodeArr[pstTree->usNodeNum - 1].usLspNo;
    pstTree->pstNodeArr[usSearchIndex].ulTunnelId = pstTree->pstNodeArr[pstTree->usNodeNum - 1].ulTunnelId;
    pstTree->pstNodeArr[usSearchIndex].ulPwId = pstTree->pstNodeArr[pstTree->usNodeNum - 1].ulPwId;

    pstTree->pstNodeArr[pstTree->usNodeNum - 1].usLspNo = 0;
    pstTree->pstNodeArr[pstTree->usNodeNum - 1].ulTunnelId = 0xffffffff;
    pstTree->pstNodeArr[pstTree->usNodeNum - 1].ulPwId = 0xffffffff;
    pstTree->usNodeNum--;

    /* 给数组排序(按从小到大的顺序) */
    qsort(pstTree->pstNodeArr, pstTree->usNodeNum, sizeof(SPM_OAMTREE_NODE), spm_oamtree_nodecmp);

    iRet = SUCCESS;
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;

}

/*****************************************************************************
   函 数 名  : spm_oamtree_nodesearch
   功能描述  : 数组中查找节点，数组已经按照规则顺序排序，采用二分法查找
   输入参数  : IN NBB_ULONG ulTunnelId : 节点的第一个比较元素
               IN NBB_ULONG ulPwId     : 节点的第二个比较元素
               IN SPM_OAMTREE_SET *pstTree  : 存储节点的数组指针
   输出参数  :  OUT NBB_USHORT *usIndex  : 若存在，表示节点号
   返 回 值  :  SUCCESS 该节点存在
                ERROR  该节点不存在
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月23日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oamtree_nodesearch (
    IN NBB_ULONG ulTunnelId, 
    IN NBB_ULONG ulPwId, 
    IN SPM_OAMTREE_SET *pstTree, 
    OUT NBB_USHORT *usIndex)
{
    NBB_USHORT usBegin = 0;
    NBB_USHORT usEnd = 0;
    NBB_USHORT usSearchIndex = 0;
    NBB_INT iRet = ERROR;
        NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_nodesearch"));

    /* 入参检查 */
    if ((NULL == pstTree) || (NULL == usIndex))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        goto EXIT_LABEL;
    }

    usEnd = pstTree->usNodeNum;

    /* 二分法查找 */
    while (usBegin < usEnd)
    {
        usSearchIndex = (usBegin + usEnd) / 2;

        /* 若相等，则找到 */
        if ((pstTree->pstNodeArr[usSearchIndex].ulTunnelId == ulTunnelId) 
            && (pstTree->pstNodeArr[usSearchIndex].ulPwId == ulPwId))
        {
            iRet = SUCCESS;
            *usIndex = usSearchIndex;
            break;
        }
        else /* 若不等，则二分法查找 */
        {
            if (pstTree->pstNodeArr[usSearchIndex].ulTunnelId == ulTunnelId) /* tunnelid相等 */
            {
                if (pstTree->pstNodeArr[usSearchIndex].ulPwId < ulPwId) /* 判断pwid */
                {
                    if (usBegin >= usSearchIndex) /* 判断条件 */
                    {
                        break;
                    }
                    usBegin = usSearchIndex;
                }
                else if (pstTree->pstNodeArr[usSearchIndex].ulPwId > ulPwId) /* 判断pwid */
                {
                    if (usEnd <= usSearchIndex) /* 查询条件比较 */
                    {
                        break;
                    }
                    usEnd = usSearchIndex;
                }
            }
            else if (pstTree->pstNodeArr[usSearchIndex].ulTunnelId < ulTunnelId) /* 判断tunnelid */
            {
                if (usBegin >= usSearchIndex) /* 查询条件比较 */
                {
                    break;
                }
                usBegin = usSearchIndex;
            }
            else if (pstTree->pstNodeArr[usSearchIndex].ulTunnelId > ulTunnelId) /* 判断tunnelid */
            {
                if (usEnd <= usSearchIndex) /* 查询条件比较 */
                {
                    break;
                }
                usEnd = usSearchIndex;
            }
        }
    }

    if (ERROR == iRet) /* 若没有找到 */
    {
        /* 判断最中间的节点 */
        if ((usBegin == usEnd) && (pstTree->pstNodeArr[usBegin].ulTunnelId == ulTunnelId)
            && (pstTree->pstNodeArr[usSearchIndex].ulPwId == ulPwId))
        {
            iRet = SUCCESS;
            *usIndex = usSearchIndex;
        }
    }

    EXIT_LABEL:NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
   函 数 名  : spm_oamtree_nodecmp
   功能描述  : 排序比较函数
   输入参数  : const NBB_VOID *enta
               const NBB_VOID *entb
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月24日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oamtree_nodecmp (IN const NBB_VOID *enta, IN const NBB_VOID *entb)
{
    NBB_ULONG ulTunnela = 0;  
    NBB_ULONG ulTunnelb = 0;
    NBB_ULONG ulPwa = 0;
    NBB_ULONG ulPwb = 0;
    NBB_INT   iRet = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    /* 合法性检查 */
    if ((NULL == enta) || (NULL == entb))
    {
        goto EXIT_LABEL;
    }

    ulTunnela = ((SPM_OAMTREE_NODE *)enta)->ulTunnelId;
    ulTunnelb = ((SPM_OAMTREE_NODE *)entb)->ulTunnelId;
    ulPwa = ((SPM_OAMTREE_NODE *)enta)->ulPwId;
    ulPwb = ((SPM_OAMTREE_NODE *)entb)->ulPwId;

    /* 判断条件，若tunnelid值相等则比较pwid */
    if (ulTunnela == ulTunnelb) 
    {
        if (ulPwa == ulPwb) /* 若pwid值相等 */
        {
            goto EXIT_LABEL;
        }
        else /* 若pwid值不相等 */
        {
            iRet = ((ulPwa < ulPwb) ? -1 : 1);
            goto EXIT_LABEL;
        }
    }
    else /* 若tunnelid值不等 */
    {
        iRet = ((ulTunnela < ulTunnelb) ? -1 : 1);
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL://NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
   函 数 名  : spm_oamtree_getarridx
   功能描述  : 根据输入的二叉树的深度和该深度的结点号，获取对应在Tree数组成
               员的序号。
   输入参数  : IN NBB_USHORT usTreeLevel   :  二叉树的深度
               IN NBB_USHORT usLevelIndex  :  深度的节点号
               IN NBB_USHORT usTreeLevelMax : 二叉树的最大层数
   输出参数  : OUT NBB_USHORT *pusArrIndex :  数组成员的序号
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月27日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oamtree_getarridx(
    IN NBB_USHORT usTreeLevel, 
    IN NBB_USHORT usLevelIndex, 
    IN NBB_USHORT usTreeLevelMax, 
    OUT NBB_USHORT *pusArrIndex)
{
    NBB_USHORT usLength = 0;
    NBB_USHORT usArridx = 0;
    NBB_INT iRet = ERROR;
        NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    /* 合法性检查 */
    if (usTreeLevel >= usTreeLevelMax)
    {
        iRet = -1;
        goto EXIT_LABEL;
    }

    /* 参数检测 */
    if (NULL == pusArrIndex)
    {
        iRet = -2;
        goto EXIT_LABEL;
    }

    iRet = spm_oamtree_gettreelen(usTreeLevel, usTreeLevelMax, &usLength);
    if (SUCCESS != iRet) /* 判断执行结果 */
    {
        iRet = -3;
        goto EXIT_LABEL;
    }

    if (usLevelIndex >= usLength) /* 判断返回值的有效性 */
    {
        iRet = -4;
        goto EXIT_LABEL;
    }

    usArridx = (1 << (usTreeLevelMax - 1 - usTreeLevel))
        + (usLevelIndex / NODESTEP) * (1 << (usTreeLevelMax - usTreeLevel)) - 1;
    *pusArrIndex = usArridx;
        
    EXIT_LABEL://NBB_TRC_EXIT();

    return iRet;

}

/*****************************************************************************
   函 数 名  : spm_oamtree_gettreelen
   功能描述  : 获取二叉树指定的深度的长度
   输入参数  : IN NBB_USHORT usTreeLevel  :二叉树的指定深度
               IN NBB_USHORT usTreeLevelMax:二叉树的最大层数
   输出参数  : OUT NBB_USHORT *pusLenth  :指定深度的长度
   返 回 值  : SUCCESS/FAILED
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月27日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oamtree_gettreelen (
    IN NBB_USHORT usTreeLevel, 
    IN NBB_USHORT usTreeLevelMax, 
    OUT NBB_USHORT *pusLenth)
{
    NBB_USHORT usLength = 0;
    NBB_INT    iRet = ERROR;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();    
    
    /* 合法性检查 */
    if (usTreeLevel >= usTreeLevelMax)
    {
        goto EXIT_LABEL;
    }

    /* 合法性检查 */
    if (NULL == pusLenth)
    {
        goto EXIT_LABEL;
    }

    usLength = NODESTEP * (1 << usTreeLevel);
    *pusLenth = usLength;

    iRet = SUCCESS;
    
    EXIT_LABEL://NBB_TRC_EXIT();

    return iRet;

}

/*****************************************************************************
   函 数 名  : spm_oamtree_build
   功能描述  : 建立二叉树
   输入参数  : IN SPM_OAMTREE_SET *pstTreeNodeSet:二叉树
             IN NBB_USHORT usTreeLevelMax:二叉树最大层数
             IN NBB_ULONG *pulTreeBaseAdd:二叉树每层的起始地址
             IN NBB_ULONG ulBaseAddress:二叉树的基地址
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年8月27日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

  2.日    期   : 2015年5月28日
    作    者   : gaos
    修改内容   : 移位部分移至外部，同690

  3.日    期   : 2015年7月10日
    作    者   : gaos
    修改内容   : mantis 问题0080532，针对转发二叉树写fpga内容检查
*****************************************************************************/
NBB_INT spm_oamtree_build(
    IN NBB_USHORT usTreeLevelMax,
    IN SPM_OAMTREE_SET *pstTreeNodeSet,
    IN NBB_ULONG *pulTreeBaseAdd,
    IN NBB_ULONG ulBaseAddress)
{
    NBB_USHORT usTreeLevel = 0; /* 二叉树的层数 */
    NBB_USHORT usLevelLength = 0; /* 二叉树的节点号 */
    NBB_USHORT usIndex = 0; /* 对应的数组的序号 */
    NBB_ULONG  ulAddress = 0;
    NBB_USHORT usTempLoop = 0; /* 循环变量 */
    NBB_USHORT usTempLabel = 0;
    NBB_INT    iRet = ERROR;
    SPM_FPGATREE_DATA stFpagData;
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usFpgaData = 0;
    NBB_UINT uiCheckLoop = 0;
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_build"));

    /* 合法性检查 */
    if ((NULL == pstTreeNodeSet) || (NULL == pulTreeBaseAdd))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stFpagData, 0, sizeof(SPM_FPGATREE_DATA));

    /* 循环遍历 */
    for (usTreeLevel = 0; usTreeLevel < usTreeLevelMax; usTreeLevel++)
    {
        pulTreeBaseAdd[usTreeLevel] = (((1 << usTreeLevelMax) - 2) << (usTreeLevel + 2)) 
            & ((1 << usTreeLevelMax) * 4 - 1); 
        
        iRet = spm_oamtree_gettreelen(usTreeLevel, usTreeLevelMax, &usLevelLength);
        if (SUCCESS != iRet) /* 判断执行结果 */
        {
            NBB_TRC_FLOW((NBB_FORMAT "get tree len error, treelevel = %d, maxlevel = %d", usTreeLevel, usTreeLevelMax));
            goto EXIT_LABEL;
        }

        /* 循环 */
        for (usTempLoop = 0; usTempLoop < usLevelLength; usTempLoop = usTempLoop + 4)
        {
            iRet = spm_oamtree_getarridx(usTreeLevel, usTempLoop, usTreeLevelMax, &usIndex);
            if (SUCCESS != iRet) /* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT "Get tree Index error,p1 = %d,p2 = %d,p3 = %d", 
                    usTreeLevel, usTempLoop, usIndex));
                goto EXIT_LABEL;
            }
            ulAddress = pulTreeBaseAdd[usTreeLevel] + usTempLoop;


            if(NULL != pstTreeNodeSet->pfnGetDate)
            {
                /* 转换本地存储的值为FPGA需要使用的内容 */
                iRet = pstTreeNodeSet->pfnGetDate(&(pstTreeNodeSet->pstNodeArr[usIndex]), &stFpagData);
                if (SUCCESS != iRet)
                {
                    goto EXIT_LABEL;
                }

                /* 防止以后扩充数据结构SPM_FPGATREE_DATA总长度但是没有改宏FPGA_TREE_DATA_NUM */
                NBB_ASSERT(stFpagData.num <= FPGA_TREE_DATA_NUM);
                
                /* 往FPGA写数据 */
                for (usLoopi = 0; usLoopi < stFpagData.num; usLoopi++) /* 对变量usLoopi进行循环 */
                {
                    spm_oamtree_s16write(ulBaseAddress, ulAddress + usLoopi,
                        stFpagData.usData.fpgaData.fpgaData[usLoopi]);
                }
            }
        }
    }
    
    iRet = SUCCESS;
    EXIT_LABEL:NBB_TRC_EXIT();

    return iRet;
}


/*******************************************************调试打印函数******************************************************/

/*****************************************************************************
 函 数 名  : spm_oamtree_print
 功能描述  : 二叉树打印函数
 输入参数  : IN SPM_OAMTREE_SET *pstTree :二叉树指针
             IN NBB_USHORT usNumMax      :需要打印的节点数          
             NBB_CCXT_T NBB_CCXT          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oamtree_print(IN SPM_OAMTREE_SET *pstTree, IN NBB_USHORT usNumMax)
{
    NBB_USHORT usLoopi = 0; /* 循环变量 */
        NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_print"));
    
    /* 判断入参 */
    if (NULL == pstTree)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        goto EXIT_LABEL;
    }

    /* 判断入参，若要求输入的值大于节点数，则不返回，打印全部存在的节点数 */
    if (usNumMax > pstTree->usNodeNum)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Exceed tree node，input num is %d", usNumMax));
        usNumMax = pstTree->usNodeNum;
    }
    
    NBB_TRC_FLOW((NBB_FORMAT "Tree node = %d", pstTree->usNodeNum));

    /* 循环打印 */
    for (usLoopi = 0; usLoopi < usNumMax; usLoopi++)
    {
        NBB_TRC_FLOW((NBB_FORMAT "tunnel = %d, pw = %d, lspno = %d\n", pstTree->pstNodeArr[usLoopi].ulTunnelId,
            pstTree->pstNodeArr[usLoopi].ulPwId, pstTree->pstNodeArr[usLoopi].usLspNo));
    }
    
    EXIT_LABEL:NBB_TRC_EXIT();

    return;
}

/*******************************************FPGA相关操作**************************************************/
/*****************************************************************************
 函 数 名  : spm_oamtree_switch
 功能描述  : 等待二叉树处于非翻页状态
 输入参数  : IN NBB_ULONG ulBaseAddress  : 二叉树基准地址        
             IN NBB_ULONG ulTreeSwtichOffset  : 表项基准地址偏移
             NBB_CCXT_T NBB_CCXT              
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID  spm_oamtree_switch(IN NBB_ULONG ulBaseAddress, IN NBB_ULONG ulTreeSwtichOffset)
{
    NBB_USHORT usTempValue = DRV_USHORT_INVALID;
        NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY(("spm_oamtree_switch"));

    /* 等待二叉树处于空闲状态 */
    while (0 != usTempValue)
    {
        usTempValue = spm_oamtree_s16read(ulBaseAddress, ulTreeSwtichOffset);
        usTempValue = usTempValue & 0x1;
    }

    EXIT_LABEL:NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_oamtree_s16write
   功能描述  : 寄存器写操作
   输入参数  : NBB_ULONG ulBaseAddress    :基地址
               NBB_ULONG ulOffsetAddress  :偏移地址
               NBB_USHORT usData       :赋值
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年9月12日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oamtree_s16write (
    NBB_ULONG ulBaseAddress, 
    NBB_ULONG ulOffsetAddress, 
    NBB_USHORT usData)
{
    NBB_UINT uiOffSet = 0;
    
    uiOffSet = (NBB_INT)(ulBaseAddress + ulOffsetAddress);
    (NBB_VOID)WriteFpgaMmap(uiOffSet, usData);
    
    return;
}

/*****************************************************************************
   函 数 名  : spm_oamtree_s16read
   功能描述  : 寄存器读操作
   输入参数  : NBB_ULONG ulBaseAddress  :基地址
             NBB_ULONG ulOffsetAddress:偏移地址
   输出参数  : 无
   返 回 值  : 寄存器中的数值
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年9月12日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_USHORT spm_oamtree_s16read (NBB_ULONG ulBaseAddress, NBB_ULONG ulOffsetAddress)
{
    NBB_USHORT usData = 0;
    NBB_UINT uiOffSet = 0;
            
    uiOffSet = (NBB_INT)(ulBaseAddress + ulOffsetAddress);
    (NBB_VOID)ReadFpgaMmap(uiOffSet, &usData);

    return usData;
}

/*****************************************************************************
   函 数 名  : spm_oamtree_s32write
   功能描述  : 寄存器写操作
   输入参数  : NBB_ULONG ulBaseAddress    :基地址
               NBB_ULONG ulOffsetAddress  :偏移地址
               NBB_UINT uiData       :赋值
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年9月12日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oamtree_s32write (
    NBB_ULONG ulBaseAddress, 
    NBB_ULONG ulOffsetAddress, 
    NBB_UINT  uiData)
{
    NBB_UINT uiOffSet = 0;
    
    uiOffSet = (NBB_INT)(ulBaseAddress + ulOffsetAddress);
    (NBB_VOID)WriteFpga32b(uiOffSet, uiData);
    
    return;
}


