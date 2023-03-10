// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AwesomeProject.h"
#include "GameFramework/Actor.h"
#include "ABBox.generated.h"

UCLASS()
class AWESOMEPROJECT_API AABBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(VisibleAnywhere, Category = Mesh)
		UStaticMeshComponent* cBoxMesh;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		UBoxComponent* cBox;

	UPROPERTY(EditInstanceOnly, Category = Weapon)
		TSubclassOf<class AABWeapon> cWeaponClass;

	void PostInitializeComponents() override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	



//----------------   파티클 관련   ------------------------------------------------------------
private:
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* cChestOpenParticle;
	UFUNCTION()
		void FinishOpenEffect(UParticleSystemComponent* _PSystem);


};
