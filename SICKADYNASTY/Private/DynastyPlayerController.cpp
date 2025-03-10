// Fill out your copyright notice in the Description page of Project Settings.


#include "DynastyPlayerController.h"

ADynastyPlayerController::ADynastyPlayerController()
{
    PlayerTeamId =  FGenericTeamId(0);
}

FGenericTeamId ADynastyPlayerController::GetGenericTeamId() const
{
    return PlayerTeamId;
}
