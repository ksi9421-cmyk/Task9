#include "Player/PNPawn.h"
#include "Project9.h"


void APNPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = Project9FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("PNPawn::BeginPlay() %s [%s]"), *Project9FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Project9FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void APNPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = Project9FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("PNPawn::PossessedBy() %s [%s]"), *Project9FunctionLibrary::GetNetModeString(this), *NetRoleString);
	Project9FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

