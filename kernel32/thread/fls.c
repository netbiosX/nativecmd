/* $Id: fls.c 45019 2010-01-09 21:57:00Z ekohl $
 *
 * COPYRIGHT:  See COPYING in the top level directory
 * PROJECT:    ReactOS system libraries
 * FILE:       lib/kernel32/thread/fls.c
 * PURPOSE:    Fiber local storage functions
 * PROGRAMMER: KJK::Hyperion <noog@libero.it>
 *
 * UPDATE HISTORY:
 *             28/05/2003 - created. Stubs only
 *
 */

#include <k32.h>

#include <debug.h>
#if 0
/*
 * @unimplemented
 */
DWORD
WINAPI
FlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback)
{
   (void)lpCallback;

   UNIMPLEMENTED;
   SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
   return FLS_OUT_OF_INDEXES;
}


/*
 * @unimplemented
 */
BOOL
WINAPI
FlsFree(DWORD dwFlsIndex)
{
    (void)dwFlsIndex;

    UNIMPLEMENTED;
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}

#endif
/*
 * @implemented
 */
PVOID
WINAPI
FlsGetValue(DWORD dwFlsIndex)
{
    PVOID *ppFlsSlots;
    PVOID pRetVal;

    if(dwFlsIndex >= 128) goto l_InvalidParam;

    ppFlsSlots = NtCurrentTeb()->FlsData;

    if(ppFlsSlots == NULL) goto l_InvalidParam;

    SetLastError(0);
    pRetVal = ppFlsSlots[dwFlsIndex + 2];

    return pRetVal;

l_InvalidParam:
    SetLastError(ERROR_INVALID_PARAMETER);
    return NULL;
}


/*
 * @implemented
 */
BOOL
WINAPI
FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData)
{
    PVOID *ppFlsSlots;
    TEB *pTeb = NtCurrentTeb();

    if(dwFlsIndex >= 128) goto l_InvalidParam;

    ppFlsSlots = pTeb->FlsData;

    if (ppFlsSlots == NULL)
    {
        PEB *pPeb = pTeb->ProcessEnvironmentBlock;

        ppFlsSlots = RtlAllocateHeap(pPeb->ProcessHeap,
                                     HEAP_ZERO_MEMORY,
                                     (128 + 2) * sizeof(PVOID));
        if(ppFlsSlots == NULL) goto l_OutOfMemory;

        pTeb->FlsData = ppFlsSlots;

        RtlAcquirePebLock();

        /* TODO: initialization */

        RtlReleasePebLock();
    }

    ppFlsSlots[dwFlsIndex + 2] = lpFlsData;

    return TRUE;

l_OutOfMemory:
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    goto l_Fail;

l_InvalidParam:
    SetLastError(ERROR_INVALID_PARAMETER);

l_Fail:
    return FALSE;
}

/* EOF */
