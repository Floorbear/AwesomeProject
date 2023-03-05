// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AwesomeProject.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(NADelegate)
DECLARE_MULTICAST_DELEGATE(NHCDelegate)

UCLASS()
class AWESOMEPROJECT_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	void JumpAttackSection(int32 _Index); // 1~4

	NADelegate NotifyAttackDelegate;
	NHCDelegate NotifyHitCheckDelegate;

	void SetDead() { IsDead = true; }

private:
		UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

		UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			bool IsInAir;
		UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
			bool IsDead = false;

		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
			UAnimMontage* AttackMontage;

protected:
	UFUNCTION()
		void AnimNotify_NextAttckCheck();
	UFUNCTION()
		void AnimNotify_HitCheck();
	
};
