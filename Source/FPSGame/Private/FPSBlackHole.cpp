// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/PrimitiveComponent.h>
#include "FPSProjectile.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh作成　当たり判定OFF
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	//ブラックホールの中の部分を作成
	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComponent->SetSphereRadius(100);
	InnerSphereComponent->SetupAttachment(MeshComp);

	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AFPSBlackHole::OverlapInnerSphere);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapInnerSphere"));
	OuterSphereComponent->SetSphereRadius(3000);
	//OuterSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OuterSphereComponent->SetupAttachment(MeshComp);

}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterSphereComponent->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		// PrimCompがあれば　＆＆　物理効果がONになってるなら実行する
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			PrimComp->AddRadialForce(GetActorLocation(),SphereRadius,ForceStrength,ERadialImpulseFalloff::RIF_Constant, true);	//最后一个参数 加速度变化
		}
	}
}

void AFPSBlackHole::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AFPSProjectile* MyProjectile = Cast<AFPSProjectile>(OtherActor);
	if (MyProjectile)
	{
		Destroy();
	}
}

