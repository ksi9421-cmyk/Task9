#include "Game/PNGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/PNPlayerController.h"

void APNGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			APNPlayerController* PNPC = Cast<APNPlayerController>(PC);
			if (IsValid(PNPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				PNPC->ClientRPCPrintChatMessageString(NotificationString);
			}
		}
	}
}
