// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include <Components/BoxComponent.h>
#include <Components/DecalComponent.h>
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.f));

	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.f,200.f,200.f);
	DecalComp->SetupAttachment(OverlapComp);

	//�g�˂鎞�̏���
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this,&AFPSExtractionZone::HandleOverlap);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//�g�˂�Pawn���v���C���[�ƃA�C�e�������̎��̂݁@���s
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr) 
	{
		return;
	}

	if (MyPawn->bIsCarryingObjective) 
	{
		//�Q�[�����[�h�擾���āA�N���A�ł̏I�����������܂��B
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->CompletMission(MyPawn, true);
		}
	}
	else 
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}

	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zonel!"));
}

