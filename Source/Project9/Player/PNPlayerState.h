#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PNPlayerState.generated.h"

UCLASS()
class PROJECT9_API APNPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	APNPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;     //GameMode PostLogin 함수에서 배정받음

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

	FString GetPlayerInfoString();
};
