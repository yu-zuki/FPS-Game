// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGua.h"
#include <Perception/PawnSensingComponent.h>
#include <DrawDebugHelpers.h>
#include <Engine/EngineTypes.h>
#include <GameFramework/Actor.h>
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGua::AFPSAIGua()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGua::OnPawnSee);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGua::OnNoiseHeard);

	OriginlRotation = GetActorRotation();
}

// Called when the game starts or when spawned
void AFPSAIGua::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSAIGua::OnPawnSee(APawn* SeePawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("DEBUG Can See Pawn"));
	if (SeePawn == nullptr)
	{
		return;
	}

	//プレイヤーを見つかったら
	AFPSCharacter* APlayer = Cast<AFPSCharacter>(SeePawn);
	if (APlayer)
	{
		FVector Location = APlayer->GetActorLocation();
		FVector Direction = Location - GetActorLocation();

		Direction.Normalize();

		FVector NewVector = GetActorLocation() + (Direction * 50);
		SetActorLocation(NewVector);

		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.0f;
		NewLookAt.Roll = 0.0f;

		SetActorRotation(NewLookAt);

		UE_LOG(LogTemp,Warning, TEXT("See Player On"));
	}

	//ゲームモード取得して、ゲームオーバーの　終了処理をします。
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		//BPにゲームオーバーを送る
		GM->CompletMission(SeePawn, false);
	}


	DrawDebugSphere(GetWorld(), SeePawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);
	//UE_LOG(LogTemp, Warning, TEXT("DEBUG Can Draw Pawn"));
}

void AFPSAIGua::OnNoiseHeard(APawn* _Instigator, const FVector& Location, float Volume)
{

	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);
	
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	//GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	//GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGua::ResetOrientation, 3.0f);
}

void AFPSAIGua::ResetOrientation()
{
	SetActorRotation(OriginlRotation);
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

}
