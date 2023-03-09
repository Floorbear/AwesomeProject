// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "ABWeapon.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->TargetArmLength = 400.f;
	Camera->SetupAttachment(SpringArm);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0, 0));
									
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.f), FRotator(0, -90.f, 0));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CARDBOARD_MESH(TEXT("/Game/MyModel/Merc.Merc"));
	if (CARDBOARD_MESH.Succeeded() == true)
	{
		GetMesh()->SetSkeletalMesh(CARDBOARD_MESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/MyModel/MercBlueprint.MercBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded() == true)
	{
		GetMesh()->SetAnimClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(EControlMode::GTA);
	GetCharacterMovement()->JumpZVelocity = 800.f;


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case AABCharacter::EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.f,0.f,0.f));
		ArmLengthTo = 800.f;
		ArmRotationTo = FRotator(-45.f, 0.f, 0.f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

		break;
	default:
		break;
	}

}

float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (FinalDamage > 0.f)
	{
		ABAnim->SetDead();
		SetActorEnableCollision(false);
	}
	UE_LOG(AwesomeProject, Warning, TEXT("Take Damage"));
	return FinalDamage;
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//AnimInstance 관련
	//생성자에 이거 하면 AnimInstance = nullptr 됨.
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		ABAnim = Cast<UABAnimInstance>(AnimInstance);
	}

	ABAnim->NotifyAttackDelegate.AddLambda([&]()
		->
		void
		{
			if (IsInputOn == true)
			{
				ABAnim->JumpAttackSection(CurrentCombo);
				IsInputOn = false;
			}
		});
	ABAnim->NotifyHitCheckDelegate.AddUObject(this,&AABCharacter::HitCheck);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnd);

	//WeaponMesh 관련
	auto WeaponActor = GetWorld()->SpawnActor<AABWeapon>();
	check(WeaponActor != nullptr);
	FName WeaponSocket_Name = TEXT("WeaponHand");
	WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket_Name);
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);


	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::DIABLO:
	{
		FRotator RelativeRotation = FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationspeed);
		SpringArm->SetRelativeRotation(RelativeRotation);
		if (DirectionMove.SizeSquared() > 0.f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromY(DirectionMove).Rotator());

			AddMovementInput(DirectionMove);
		}
		break;
	}

	default:
		break;
	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &AABCharacter::TurnRight);
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);

}

void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

		break;
	case AABCharacter::EControlMode::DIABLO:
		DirectionMove.X = NewAxisValue;

		break;
	default:
		break;
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

		break;
	case AABCharacter::EControlMode::DIABLO:
		DirectionMove.Y = NewAxisValue;

		break;
	default:
		break;
	}
}

void AABCharacter::TurnRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);

		break;
	case AABCharacter::EControlMode::DIABLO:
		break;
	default:
		break;
	}
}

void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);

		break;
	case AABCharacter::EControlMode::DIABLO:
		break;
	default:
		break;
	}
}

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case AABCharacter::EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	default:
		break;
	}
}

void AABCharacter::Attack()
{
	if (IsAttacking == true)
	{
		if (IsInputOn == false)
		{
			IsInputOn = true;
			CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, 4);
		}
		return;
	}


	ABAnim->PlayAttackMontage();
	IsAttacking = true;
}

void AABCharacter::HitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Parms(NAME_None, false, this);
	bool Result = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(50.f), Parms);


#if ENABLE_DRAW_DEBUG
	DrawDebugCapsule(GetWorld(), GetActorLocation() + GetActorForwardVector() * AttackRange * 0.5f,
		AttackRange * 0.5f + AttackRadius, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), Result ? FColor::Green : FColor::Red, false, 3.0f);
#endif // ENABLE_DRAW_DEBUG


	if (Result == true)
	{
		if (IsValid(HitResult.GetActor()) == true)
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(50.f, DamageEvent, GetController(), this);
			//UE_LOG(AwesomeProject, Warning, TEXT("Actor Name : %s"), *HitResult.GetActor()->GetName());
		}
	}

}

void AABCharacter::OnAttackMontageEnd(UAnimMontage* _Mon, bool _Inter)
{
	IsAttacking = false;
	IsInputOn = false;
	CurrentCombo = 1;
}

