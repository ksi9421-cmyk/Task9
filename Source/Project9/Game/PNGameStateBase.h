#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PNGameStateBase.generated.h"

UCLASS()
class PROJECT9_API APNGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXX")));
};
