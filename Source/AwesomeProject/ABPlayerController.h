// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AwesomeProject.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AWESOMEPROJECT_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void OnPossess(APawn* aPawn) override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	
};
