// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DynastyCountDownAction.h"

void FDynastyCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

void FDynastyCountDownAction::UpdateOperation(FLatentResponse& Response)
{
    if (bNeedToCancel)
    {
        CountDownOutput = EDynastyCountDownActionOutput::Cancelled;

        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

        return;
    }

    if (ElapsedTimeSinceStart >= TotalCountDownTime)
    {
        CountDownOutput = EDynastyCountDownActionOutput::Completed;

        Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

        return;
    }

    if (ElapsedInterval < UpdateInterval)
    {
        ElapsedInterval += Response.ElapsedTime(); // Delta time과 같은 역할
    }
    else
    {
        ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

        OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

        CountDownOutput = EDynastyCountDownActionOutput::Updated;

        Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

        ElapsedInterval = 0.f;
    }

}
