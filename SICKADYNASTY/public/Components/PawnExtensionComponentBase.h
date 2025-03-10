// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SICKADYNASTY_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()
protected:
	template<class T>
	T* GetOwningPawn() const
	{
		// 컴파일 시 조건이 false면 오류를 발생시킴
		static_assert(TPointerIsConvertibleFromTo<T,APawn>::Value,"T Template params to GetPawn must be derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template <class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T,AController>::Value,"T Template params to GetController must be derived from AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
