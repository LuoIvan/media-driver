/*
* Copyright (c) 2017, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file      cm_task_rt.h
//! \brief     Declaration of CmTaskRT.
//!

#ifndef MEDIADRIVER_AGNOSTIC_COMMON_CM_CMTASKRT_H_
#define MEDIADRIVER_AGNOSTIC_COMMON_CM_CMTASKRT_H_

#include "cm_task.h"

namespace CMRT_UMD
{
class CmKernelRT;
class CmDeviceRT;

class CmTaskRT: public CmTask
{
public:
    static int32_t Create(CmDeviceRT *pCmDevice,
                          uint32_t index,
                          uint32_t max_kernel_count,
                          CmTaskRT* &pKernelArray);

    static int32_t Destroy(CmTaskRT *&pKernelArray);

    CM_RT_API int32_t AddKernel(CmKernel *pKernel);

    CM_RT_API int32_t Reset();

    CM_RT_API int32_t AddSync();

    CM_RT_API int32_t SetPowerOption(PCM_POWER_OPTION pPowerOption);

    int32_t SetPreemptionMode(CM_PREEMPTION_MODE mode);

    CM_RT_API CM_PREEMPTION_MODE GetPreemptionMode();

    CM_RT_API int32_t AddConditionalEnd(SurfaceIndex *pConditionalSurface,
                                        uint32_t offset,
                                        CM_CONDITIONAL_END_PARAM *pCondParam);

    CM_RT_API int32_t SetProperty(const CM_TASK_CONFIG &taskConfig);

    uint32_t GetKernelCount();

    CmKernelRT *GetKernelPointer(uint32_t index);

    uint32_t GetIndexInTaskArray();

    bool IntegrityCheckKernelThreadspace();

    uint64_t GetSyncBitmap();

    uint64_t GetConditionalEndBitmap();

    CM_HAL_CONDITIONAL_BB_END_INFO *GetConditionalEndInfo();

    int32_t SetConditionalEndInfo(SurfaceIndex *pIndex,
                                  uint32_t offset,
                                  CM_CONDITIONAL_END_PARAM *pCondParam);

    PCM_POWER_OPTION GetPowerOption();

    PCM_TASK_CONFIG GetTaskConfig();

#if CM_LOG_ON
    std::string Log();
#endif

protected:
    CmTaskRT(CmDeviceRT *pCmDevice,
             uint32_t index,
             uint32_t max_kernel_count);

    ~CmTaskRT();

    int32_t Initialize();

#if USE_EXTENSION_CODE
    void AddKernelForGTPin(CmKernel *pKernel);
#endif

    CmKernelRT **m_pKernelArray;

    CmDeviceRT *m_pCmDev;

    uint32_t m_KernelCount;

    uint32_t m_MaxKernelCount;

    uint32_t m_IndexTaskArray;

    // Reserve a 64-bit variable to indicate if synchronization is insert for kernels.
    // 1 bit per kernel, 0 -- No sync, 1 -- Need sync
    // Up to 64 kernels supported
    uint64_t m_ui64SyncBitmap;

    // 64-bit variable to indicate if a conditional batch buffer end is inserted between kernels
    // 1 bit per kernel, 0 -- No conditional end, 1 -- Insert conditional end
    // Up to 64 kernels supported
    uint64_t m_ui64ConditionalEndBitmap;

    CM_HAL_CONDITIONAL_BB_END_INFO
    m_ConditionalEndInfo[CM_MAX_CONDITIONAL_END_CMDS];

    CM_POWER_OPTION m_PowerOption;

    CM_PREEMPTION_MODE m_PreemptionMode;

    CM_TASK_CONFIG m_TaskConfig;

private:
    CmTaskRT(const CmTaskRT &other);
    CmTaskRT &operator=(const CmTaskRT &other);
};
};  // namespace;

#endif  // #ifndef MEDIADRIVER_AGNOSTIC_COMMON_CM_CMTASKRT_H_
