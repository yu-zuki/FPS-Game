// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompletMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn) 
	{
		InstigatorPawn->DisableInput(nullptr);

		//Actor���@BP�̒��Ł@�ݒ肷��
		if (SpectatingViewpointClass)
		{
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			//���_���ݒ肳��Ă�̂݁@���_��؂�ւ���
			if (ReturnedActors.Num() > 0)
			{
				AActor* NewViewTraget = ReturnedActors[0];

				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTraget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass is nullptr.Please update GameMode class with valid subclass. Cannot change spectating view"));
		}
	}

	//BP�̂Ȃ��ŌĂяo��
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);

}
