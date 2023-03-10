// Fill out your copyright notice in the Description page of Project Settings.


#include "ABBox.h"
#include "ABCharacter.h"
#include "ABWeapon.h"

// Sets default values
AABBox::AABBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Weapon 관련
	cWeaponClass = AABWeapon::StaticClass();
	
	// 박스 - 콜리전 관련
	cBox = CreateDefaultSubobject<UBoxComponent>(TEXT("COLBOX"));
	cBox->SetCollisionProfileName(TEXT("ABBox"));
	cBox->SetBoxExtent({ 45.f,42.f,25.f });
	cBox->SetRelativeLocation({ 0,0,20.f });
	RootComponent = cBox;

	//박스 - 메시 관련
	cBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMesh(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	check(BoxMesh.Succeeded() == true);
	cBoxMesh->SetStaticMesh(BoxMesh.Object);
	cBoxMesh->BoundsScale = 1.6f;
	cBoxMesh->SetCollisionProfileName(TEXT("NoCollision"));
	cBoxMesh->SetRelativeLocation({ 0,0,-20.f });
	cBoxMesh->SetupAttachment(cBox);



	//상자 여는 파티클 관련
	cChestOpenParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE"));
	cChestOpenParticle->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> NewChestParticle(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	check(NewChestParticle.Succeeded());
	cChestOpenParticle->SetTemplate(NewChestParticle.Object);
	cChestOpenParticle->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AABBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	cBox->OnComponentBeginOverlap.AddDynamic(this, &AABBox::BeginOverlap);
		// UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult);
}

void AABBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Charactor = Cast<AABCharacter>(OtherActor);
	check(IsValid(Charactor));

	if (Charactor->CanPickUpWeapon() == false)
	{
		return;
	}

	cBoxMesh->SetHiddenInGame(true, true);
	cChestOpenParticle->Activate();
	cChestOpenParticle->OnSystemFinished.AddDynamic(this, &AABBox::FinishOpenEffect);
	auto Weapon = GetWorld()->SpawnActor<AABWeapon>(cWeaponClass);
	Charactor->SetWeapon(Weapon);
}

// Called every frame

void AABBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABBox::FinishOpenEffect(UParticleSystemComponent* _PSystem)
{
	Destroy();
}

