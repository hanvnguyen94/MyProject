
#include "SnowfallActor.h"
#include "NiagaraFunctionLibrary.h"
#include <random>
#include <boost/math/distributions/gamma.hpp>
#include <boost/random/sobol.hpp>

// Sets default values
ASnowfallActor::ASnowfallActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//default parameters
	Alpha = 2.0f;
	Beta = 1.0f;
	minD = .01; //cm
	maxD = .8; //cm
	NumParticles = 100; //number of snowflakes in the buffer

	FlakeRate = 45; //particles/m/s (per linear horizontal length of the sim screen)
	
	SpawnHeight = 1000.0f; // Height of spawn area
	ScreenWidth = 1000.0f; // Width of spawn area
	
	// Create the Niagara Component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);


}

// BeginPlay: Initialize particles
void ASnowfallActor::BeginPlay()
{
	Super::BeginPlay();


	//activate niagara system
	if (SnowNiagaraSystem)
	{
		NiagaraComponent->SetAsset(SnowNiagaraSystem);
		NiagaraComponent->Activate();

		// Set the initial value of the exposed parameter
		NiagaraComponent->SetFloatParameter(TEXT("CustomSpawnRate"), static_cast<float>(FlakeRate));
		NiagaraComponent->SetVectorParameter(TEXT("CustomWind"), WindSpeed);
		NiagaraComponent->SetFloatParameter(TEXT("UniformSpriteSizeMin"), minD);
		NiagaraComponent->SetFloatParameter(TEXT("UniformSpriteSizeMax"), maxD);
	}

	InitializeParticles();
	SpawnParticle();
}

// Tick: Update particles every frame
void ASnowfallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateParticle(DeltaTime);
}

// Initialize the particles
void ASnowfallActor::InitializeParticles()
{
	Particles.Empty();// Clear any existing particles in the array
	Particles.Reserve(NumParticles); 

	for (int32 i = 0; i < FlakeRate; ++i)
	{
		FParticle NewParticle;

		// Generate particle properties
		NewParticle.Position = FVector(
			FMath::RandRange(-ScreenWidth, ScreenWidth),
			FMath::RandRange(-ScreenWidth, ScreenWidth),
			SpawnHeight
			);

		NewParticle.Diameter = GenerateParticleDiameter();
		UE_LOG(LogTemp, Warning, TEXT("Particle diameter: %f"), NewParticle.Diameter);
		
		NewParticle.Speed = CalculateTerminalSpeed(NewParticle.Diameter);
		UE_LOG(LogTemp, Warning, TEXT("Particle speed: %f"), NewParticle.Speed);

		// Add the particle to the array
		Particles.Add(NewParticle);
	}
}

// Spawn a new particle
void ASnowfallActor::SpawnParticle()
{
	if (!SnowNiagaraSystem) return;

	for (const FParticle& Particle : Particles)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SnowNiagaraSystem,
			Particle.Position);
	if (NiagaraComp)
	{
		// NiagaraComp->SetFloatParameter(TEXT("SpriteSize"), SpriteSize);
	}
	}
}

// Generate a random particle diameter using gamma distribution
float ASnowfallActor::GenerateParticleDiameter()
{
	try
	{
		// Static Sobol generator for 1 dimension
		static boost::random::sobol sobolGen(1); // 1-dimensional Sobol generator

		// Generate the next Sobol value
		double SobolValue = sobolGen();

		// Validate SobolValue is in [0, 1] (this should always be the case for Sobol)
		SobolValue = FMath::Clamp(SobolValue, 0.0, 1.0);

		// Apply inverse cumulative Gamma distribution
		boost::math::gamma_distribution<float> GammaDist(Alpha, Beta);
		float Diameter = boost::math::quantile(GammaDist, static_cast<float>(SobolValue));

		// Debug log for verification
		UE_LOG(LogTemp, Warning, TEXT("Generated Diameter: %f"), Diameter);

		return Diameter;
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Error in GenerateParticleDiameter: %s"), *FString(e.what()));
		return 1.0f; // Fallback diameter
	}
}



// Calculate terminal velocity using the power-law equation
float ASnowfallActor::CalculateTerminalSpeed(float Diameter)
{
	float Speed = 0.84f * FMath::Pow(Diameter, 0.36f);
	UE_LOG(LogTemp, Warning, TEXT("Particle Speed: %f"), Speed);
	return Speed;
}


// Update particle position and reset if it falls below ground level
void ASnowfallActor::UpdateParticle(float DeltaTime)
{
	//update particles
	for (FParticle& Particle : Particles)
	{
		Particle.Position.Z -= Particle.Speed * DeltaTime;

		if (Particle.Position.Z <= 0.0f)
		{
			Particle.Position.Z = SpawnHeight;
		}
	}
}

/*void ASnowfallActor::UpdateNiagaraParameters()
{
	if (NiagaraComponent && NiagaraComponent->IsActive())
	{
		// Example: Set spawn rate dynamically
		NiagaraComponent->SetFloatParameter(TEXT("SpawnRate"), static_cast<float>(NumParticles));

		// Convert WindSpeed (FVector2D) to FVector for Niagara
		FVector WindDirection(WindSpeed.X, WindSpeed.Y, 0.0f);
		NiagaraComponent->SetVectorParameter(TEXT("CustomWindDirection"), WindDirection);

		// You can set other parameters as exposed in the Niagara system
	}
}*/
