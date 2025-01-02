
#include "SnowfallActor.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include <random>

// Sets default values
ASnowfallActor::ASnowfallActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	// Create the Niagara Component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	// Initialize Ground Mesh
	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	GroundMesh->SetupAttachment(RootComponent);

	// Optionally set a default static mesh (e.g., a cube or plane)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		GroundMesh->SetStaticMesh(CubeMesh.Object);
	}

	// Set the size of the ground mesh to 100x100
	GroundMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
	GroundMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.5f));

	// Default parameters
	Alpha = 2.0f;        // Example shape parameter for gamma distribution
	Beta = 1.0f;         // Example scale parameter for gamma distribution
	NumParticles = 100;  // Number of snowflakes
	SpawnHeight = 1000.0f; // Height of spawn area
	ScreenWidth = 1000.0f; // Width of spawn area

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
		NiagaraComponent->SetFloatParameter(TEXT("CustomSpawnRate"), static_cast<float>(NumParticles));
	}

	// Dynamically calculate spawn height above the ground
	FVector GroundPosition = GroundMesh->GetComponentLocation();
	SpawnHeight = GroundPosition.Z + 1000.0f; // 1000 units above the ground

	InitializeParticles();
}

// Tick: Update particles every frame
void ASnowfallActor::Tick(float DeltaTime)
{
	// Clamp DeltaTime to avoid extreme variations
	DeltaTime = FMath::Clamp(DeltaTime, 0.016f, 0.033f); // ~30–60 FPS range
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);
	
	UpdateNiagaraParameters(); // Update Niagara parameters dynamically

	// Update particles
	for (FParticle& Particle : Particles)
	{
		UpdateParticle(Particle, DeltaTime);
	}
}

// Initialize the particles
void ASnowfallActor::InitializeParticles()
{
	Particles.Empty(); // Clear any existing particles in the array

	for (int32 i = 0; i < NumParticles; ++i)
	{
		FParticle NewParticle;

		// Generate particle properties
		NewParticle.Position.X = FMath::RandRange(-ScreenWidth / 2.0f, ScreenWidth / 2.0f);
		NewParticle.Position.Y = FMath::RandRange(-ScreenWidth / 2.0f, ScreenWidth / 2.0f);
		NewParticle.Position.Z = SpawnHeight;

		NewParticle.Diameter = GenerateParticleDiameter();
		NewParticle.Speed = CalculateTerminalSpeed(NewParticle.Diameter);

		// Add the particle to the array
		Particles.Add(NewParticle);
	}
}

// Generate a random particle diameter using gamma distribution
float ASnowfallActor::GenerateParticleDiameter()
{
	std::random_device Rd;
	std::mt19937 Gen(Rd());
	std::gamma_distribution<double> GammaDist(Alpha, Beta);

	return static_cast<float>(GammaDist(Gen)); // Convert double to float
}

// Calculate terminal velocity using the power-law equation
float ASnowfallActor::CalculateTerminalSpeed(float Diameter)
{
	float Speed = 0.84f * FMath::Pow(Diameter, 0.36f);
	UE_LOG(LogTemp, Warning, TEXT("Diameter: %f, Speed: %f"), Speed, Diameter);
	return Speed;
}

// Spawn a new particle
void ASnowfallActor::SpawnParticle(FParticle& Particle)
{
	FVector ActorLocation = GetActorLocation(); // Actor's world location
	
	// Calculate spawn position relative to the actor
	Particle.Position.X = ActorLocation.X + FMath::RandRange(-ScreenWidth / 2.0f, ScreenWidth / 2.0f);
	Particle.Position.Y = ActorLocation.Y + FMath::RandRange(-ScreenWidth / 2.0f, ScreenWidth / 2.0f);
	Particle.Position.Z = SpawnHeight;

	Particle.Diameter = GenerateParticleDiameter();
	Particle.Speed = CalculateTerminalSpeed(Particle.Diameter);

	// Optionally spawn the particle in the Niagara system
	if (SnowNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SnowNiagaraSystem, Particle.Position);
	}
}

// Update particle position and reset if it falls below ground level
void ASnowfallActor::UpdateParticle(FParticle& Particle, float DeltaTime)
{
	// Update particle position based on velocity
	Particle.Position.X += WindSpeed.X * DeltaTime;
	Particle.Position.Y += WindSpeed.Y * DeltaTime;
	Particle.Position.Z -= Particle.Speed * DeltaTime;
	
	// Check if the particle has fallen below the ground
	FVector GroundPosition = GroundMesh->GetComponentLocation();
	if (Particle.Position.Z <= GroundPosition.Z)
	{
		// Respawn particle at the top
		Particle.Position.X = FMath::RandRange(-ScreenWidth / 2.0f, ScreenWidth / 2.0f);
		Particle.Position.Z = GroundPosition.Z + 1000.0f;
		Particle.Diameter = GenerateParticleDiameter();
		Particle.Speed = CalculateTerminalSpeed(Particle.Diameter);
	}
}

void ASnowfallActor::UpdateNiagaraParameters()
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
}
