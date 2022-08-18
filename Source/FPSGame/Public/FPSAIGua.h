// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGua.generated.h"

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AFPSAIGua : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGua();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;


	UFUNCTION()
	void OnPawnSee(APawn* SeePawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* _Instigator, const FVector& Location, float Volume);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
