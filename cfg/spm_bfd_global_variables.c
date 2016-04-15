/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_bfd_global_variables.c
  版 本 号   : 初稿
  作    者   : jiangbo
  生成日期   : 2013年11月11日
  最近修改   :
  功能描述   : BFD模块使用的全局变量
  函数列表   :
  修改历史   :
  1.日    期   : 2013年11月11日
    作    者   : jiangbo
    修改内容   : 创建文件

******************************************************************************/


#include <nbase.h>
#include <spmincl.h>

/***************************************************************************/
/* BFD模块全局变量  Added by limingming, 2012/10/8                         */
/***************************************************************************/    
NBB_ULONG g_ulSequenceID;                 /* 业务盘BFD会话同步ID序列号 */
NBB_BOOL  g_bIsReadySendtoMaster;         /* 是否可以同步ID,因为存在发送等待时间 */
NBB_BOOL  g_bIsModifySession;             /* 会话ID是否发生了变化，若发生变化，则需要同步 */


NBB_USHORT g_usBfdTreeNodeMaxnum;         /* 会话二叉树最大节点数 */
NBB_USHORT g_usBfdTreeLevel;              /* 会话二叉树的层数，每个业务盘支持1k会话，但FPGA按照2k空间，因此层数为11 */
NBB_USHORT g_usBfdLspNoMax;               /* 会话最大条目号 */
BFD_PUL_TREE_BASE g_pulBfdTreebase;            /* 会话存储二叉树每一层的相对地址 */
NBB_USHORT g_usBfdNum;                    /* 会话数目 */
   
SPM_OAMTREE_SET  g_stTreeBfdNodeSet;           /* 存储BFD会话二叉树的结点设置的内容，1kBFD会话 */

SPM_BFD_CB g_stBfdlocal[BFD_MAX_SESNUM];     /* 本地会话存储 */
NBB_BYTE   g_aucCouterRecord[BFD_MAX_SESNUM]; /* 每个条目记录的接收到的会话的Couter，若有变化才说明收到的是新报文 */
AVLL_TREE  g_stBfdGlbTreeByDis;              /* 全局配置存储,按照描述符存储(索引) */
AVLL_TREE  g_stBfdGlbTreeByIp;               /* BFD FOR IP配置存储，按照vrfid+sip存储(索引) */
AVLL_TREE  g_stBfdGlbTreeByIpv6;             /* BFD FOR IPV6配置存储，按照vrfid+sipv6存储(索引) */
AVLL_TREE  g_stFwdGlbTreeByDis;              /* 业务盘转发会话存储，按照描述符存储(索引) */

NBB_USHORT g_usFwdTreeNodeMaxnum;             /* 转发二叉树最大节点数 */
NBB_USHORT g_usFwdTreeLevel;                  /* 转发二叉树的层数，每个业务盘支持2k转发，因此层数为11*/
NBB_USHORT g_usFwdLspNoMax;                   /* 转发最大条目号 */
BFD_PUL_FWD_TREE_BASE g_pulFwdTreebase;            /* 转发存储二叉树每一层的相对地址 */
NBB_USHORT g_usFwdNum;                        /* 转发数目 */
SPM_OAMTREE_SET  g_stTreeFwdNodeSet;     /* 存储第二颗二叉树的结点设置的内容，2k转发表项 */

SPM_BFD_FWDINFO  g_stBfdfwd[BFD_MAX_FWDNUM];  /* 转发表项存储 */

SPM_BFD_FWDRECORD g_stFwdTxRec[SYS_TOTAL_SLOTNUM]; /* 每个盘转发的报文计数，以目的槽位号减1为数组下标 */
SPM_BFD_FWDRECORD g_stFwdRxRec[SYS_TOTAL_SLOTNUM]; /* 每个盘接收到的的报文计数，以源槽位号减1为数组下标 */
SPM_BFD_ID_STORE  g_stBfdIdStore[SYS_MAX_SLOTNUM]; /* 主控盘存储的业务盘ID信息 */
MTL_NODE  g_stSrcNode;           /* IPC 源节点信息 */
MTL_NODE  g_stDesNode;           /* IPC 目的节点信息 */
NBB_INT linx_fd;                  /* LINX的fd换成全局变量 add by ljuan */
BFD_PUC_SYN_ID_INFO g_pucSynIdInfo;         /* 同步ID报文 */
NBB_ULONG g_ulSynIdInfoLth;       /* 同步ID报文长度 */

NBB_ULONG g_ulStateToDownNum;   /* 记录从up变为down的bfd个数 */
BFD_EVENT_INFO g_stBfdEventRecord[BFD_MAX_SESNUM];
NBB_USHORT g_usRecordNum;
BFD_PUC_ALARM_INFO g_pucAlarmInfo;         /* 状态上报协议栈 */
NBB_USHORT g_usAlarmInfoLenth;
NBB_BYTE   g_ucFirstNpBoard;     /* 当前第一块在位的NP盘，初始化为0 */
NBB_BYTE   g_ucCpuTick;          /* 心跳控制 */
NBB_BOOL   g_BIsRecordDownEvent;
NBB_BOOL   g_BIsCloseStateMachine[BFD_MAX_SESNUM];      /* 关闭状态机开关 */

/* dma 读操作 */
/* 存储从FPGA读取的接收会话状态
   比特[15]: 条目N的BFD告警状态
   比特[8：7]：条目N的接收STATE值。
   比特[6：2：条目N的DIAG 值。
   比特[1：0]：COUNT自动累加翻转值。*/
NBB_UINT  g_auiDmaData[BFD_MAX_DMA_NUM];
/*存储从FPGA读取的接收报文中的MYID，
  00H+N*2HMYID低16位比特。在删除会话时写全零
  01H+N*2HMYID高16位比特；在删除会话时写全零*/
NBB_UINT  g_auiDmaDrimData[BFD_MAX_DMA_DRIM_NUM];

unsigned long long g_aullDmaDataInfo[2][2];

int g_iDevFd;   /* BFD DMA操作需要交互的fd */

NBB_BYTE g_ucMainEmuId;

BFD_BOARD_INFO  g_astBoardState[10];      /* 8010 SPU盘在位信息 */

NBB_BYTE ucSynIdTimer = 0;  /* 定时同步BFD ID计数器 */

NBB_USHORT g_usNniTreeNodeMaxnum;             /* NNI表二叉树最大节点数 */
NBB_USHORT g_usNniTreeLevel;                  /* NNI表二叉树的层数，每个业务盘支持1k转发，因此层数为10*/
NBB_USHORT g_usNniNoMax;                     /* NNI表最大条目号 */
BFD_PUL_FWD_TREE_BASE g_pulNniTreebase;            /* NNI表存储二叉树每一层的相对地址 */
NBB_USHORT g_usNniNum;                        /* NNI表数目 */
SPM_OAMTREE_SET  g_stTreeNniNodeSet;     /* 存储第二颗NNI表二叉树的结点设置的内容，1k转发表项 */
SPM_BFD_NNIINFO g_stBfdNni[BFD_MAX_NNINUM];  /* NNI表 */
NBB_BYTE g_ucLocalSlot;       /* 本盘槽位号 */

AVLL_TREE g_stAlarmFailTree;    /* 上报告警失败的定时器 */

/* 作假功能，shutdown端口时先发down包出去,然后延时的时间,间隔为5ms，其中0表示不使用该功能 */
NBB_INT g_make_bfd_down_before_port_shutdown = 0;

SPM_BFD_INTERVAL_REPORT g_stintervalinfo[BFD_MAX_SESNUM];/* 状态机变量 */


/* 调试信息打印开关 */
BFD_DEBUGSHOW g_enBfdDebug;

/* 清配置标记 */
NBB_BYTE g_ucCleanCfgCheck = 0;

/* 操作fpga转发二叉树读写锁 */
BMU_RWLOCK_ID g_build_fpga_fwd_lock;

/* fwdtable 翻页写备表的次数 */
NBB_UINT g_uiFwdTableCount = 0;

