// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATKMON(TEXT("/Game/MyModel/Merc_Skeleton_Montage.Merc_Skeleton_Montage"));
	if (ATKMON.Succeeded() == true)
	{
		AttackMontage = ATKMON.Object;
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsDead == true)
		return;
	if (::IsValid(Pawn) == true)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (::IsValid(Character) == true)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}


}

void UABAnimInstance::PlayAttackMontage()
{
	check(IsDead == false);
	Montage_Play(AttackMontage);
}

void UABAnimInstance::JumpAttackSection(int32 _Index)
{
	check(FMath::IsWithinInclusive<int32>(_Index, 1, 4));

	FString SectionName = FString::Printf(TEXT("Attack%d"),_Index);
	Montage_JumpToSection(FName(SectionName),AttackMontage);
}

void UABAnimInstance::AnimNotify_NextAttckCheck()
{
	NotifyAttackDelegate.Broadcast();
}

void UABAnimInstance::AnimNotify_HitCheck()
{
	NotifyHitCheckDelegate.Broadcast();
}
