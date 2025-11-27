#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PNPawn.generated.h"

UCLASS()
class PROJECT9_API APNPawn : public APawn
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};
