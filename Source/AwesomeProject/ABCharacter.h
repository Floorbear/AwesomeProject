// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AwesomeProject.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class AWESOMEPROJECT_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	enum class EControlMode
	{
		GTA,
		DIABLO
	};
	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::DIABLO;
	FVector DirectionMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 3.f;
	float ArmRotationspeed = 10.f;

	 float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;


private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void TurnRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void ViewChange();
	void Attack();
	void HitCheck();

	UFUNCTION()
	void OnAttackMontageEnd(UAnimMontage* _Mon, bool _Inter);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsInputOn = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo = 1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange = 200.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius = 50.f;


	UPROPERTY()
		class UABAnimInstance* ABAnim;


};
