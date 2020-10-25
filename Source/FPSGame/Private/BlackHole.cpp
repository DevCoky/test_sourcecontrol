// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Black Hole Mesh"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerArea = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Area"));
	InnerArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerArea->SetSphereRadius(200.0f);
	InnerArea->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerArea);
	InnerArea->SetupAttachment(MeshComp);

	OuterArea = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Area"));
	OuterArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterArea->SetSphereRadius(2000.0f);
	OuterArea->SetupAttachment(MeshComp);

}

void ABlackHole::OverlapInnerArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> ObjectsInAttractionArea;
	OuterArea->GetOverlappingComponents(ObjectsInAttractionArea);

	for (auto& Object : ObjectsInAttractionArea)
	{
		Object->AddRadialForce(GetActorLocation(), OuterArea->GetScaledSphereRadius(), -2000, ERadialImpulseFalloff::RIF_Constant, true);
	}

}

