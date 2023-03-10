// Fill out your copyright notice in the Description page of Project Settings.


#include "ABWeapon.h"

// Sets default values
AABWeapon::AABWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPONMESH"));
	RootComponent = mWeaponMesh;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (WeaponMesh.Succeeded() == true)
	{
		mWeaponMesh->SetSkeletalMesh(WeaponMesh.Object);
	}
}

// Called when the game starts or when spawned
void AABWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

