// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "SnowfallActor.generated.h"

USTRUCT(BlueprintType)
struct FParticle
{
	GENERATED_BODY()
	
	FVector Position;
	FVector Velocity;
	float Diameter;
	float Speed;

};

UCLASS()
class MYPROJECT_API ASnowfallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASnowfallActor();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	// Class variables for simulation
	UPROPERTY(EditAnywhere, Category="Snowfall")
	int32 NumParticles;
	
	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float Alpha; // Shape parameter for gamma distribution

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float Beta; // Scale parameter for gamma distribution

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float minD; //cm, minimal particle size

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float maxD; //cm, maximal particle size

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float ra1; 

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float d;
	
	UPROPERTY(EditAnywhere, Category = "Snowfall")
	int32 FlakeRate;
	
	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float SpawnHeight; // Height of the spawn area

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float ScreenWidth; // Width of the spawn area

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	FVector WindSpeed; // X: horizontal wind speed, Y: vertical wind offset (if needed)

	TArray<FParticle> Particles; // Array of particles

	UPROPERTY(EditAnywhere, Category = "Niagara")
	UNiagaraSystem* SnowNiagaraSystem; // Reference to Niagara system

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent; // Niagara component for spawning particles

	// Functions for particle initialization and update
	void InitializeParticles();
	void SpawnParticle();
	void UpdateParticle(float DeltaTime);
	float GenerateParticleDiameter();
	float CalculateTerminalSpeed(float Diameter);
	//void UpdateNiagaraParameters();
};
