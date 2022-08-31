// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGua.h"
#include <Perception/PawnSensingComponent.h>
#include <DrawDebugHelpers.h>
#include <Engine/EngineTypes.h>
#include <GameFramework/Actor.h>
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include <NavigationSystem.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGua::AFPSAIGua()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGua::OnPawnSee);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGua::OnNoiseHeard);

	OriginlRotation = GetActorRotation();

	GuardState = EAIState::Idle;

	CurrentPatorPoint = nullptr;
}

// Called when the game starts or when spawned
void AFPSAIGua::BeginPlay()
{
	Super::BeginPlay();
	
	if (bPatrol)
	{
		MoveToNextPatorPoint();
	}
}

void AFPSAIGua::OnPawnSee(APawn* SeePawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("DEBUG Can See Pawn"));
	if (SeePawn == nullptr)
	{
		return;
	}

	////�v���C���[������������
	//AFPSCharacter* APlayer = Cast<AFPSCharacter>(SeePawn);
	//if (APlayer)
	//{
	//	FVector Location = APlayer->GetActorLocation();
	//	FVector Direction = Location - GetActorLocation();

	//	Direction.Normalize();

	//	FVector NewVector = GetActorLocation() + (Direction * 50);
	//	SetActorLocation(NewVector);

	//	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	//	NewLookAt.Pitch = 0.0f;
	//	NewLookAt.Roll = 0.0f;

	//	SetActorRotation(NewLookAt);

	//	UE_LOG(LogTemp,Warning, TEXT("See Player On"));
	//}

	DrawDebugSphere(GetWorld(), SeePawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

	//�Q�[�����[�h�擾���āA�Q�[���I�[�o�[�́@�I�����������܂��B
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		//BP�ɃQ�[���I�[�o�[�𑗂�
		GM->CompletMission(SeePawn, false);
	}
	//�G�̏�Ԃ��@�x���@�Őݒ�
	SetGuardState(EAIState::Alerted);

	//�v���C���[������������A�����~
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}

	//UE_LOG(LogTemp, Warning, TEXT("DEBUG Can Draw Pawn"));
}

void AFPSAIGua::OnNoiseHeard(APawn* _Instigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);
	
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	//�G�̏�Ԃ��@�^�킵���@�Őݒ�
	SetGuardState(EAIState::Suspicious);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGua::ResetOrientation, 3.0f);

	//���𕷂�������A�����~
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGua::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginlRotation);
	//�G�̏�Ԃ��@�m�[�}���@�Őݒ�
	SetGuardState(EAIState::Idle);

	//
	if (bPatrol)
	{
		MoveToNextPatorPoint();
	}
}

void AFPSAIGua::SetGuardState(EAIState newState)
{
	if (GuardState == newState)
	{
		return;
	}
	
	GuardState = newState;

	OnStateChanged(GuardState);

}
// Called every frame
void AFPSAIGua::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatorPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatorPoint->GetActorLocation();
		float DistanceGoal = Delta.Size();

		if (DistanceGoal < 100)
		{
			MoveToNextPatorPoint();
		}
	}

}
//���̓_�Ɉړ�����
void AFPSAIGua::MoveToNextPatorPoint()
{
	if (CurrentPatorPoint == nullptr || CurrentPatorPoint == SecondPatorPoint)
	{
		CurrentPatorPoint = FirstPatorPoint;
	}
	else
	{
		CurrentPatorPoint = SecondPatorPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatorPoint);
}
