#include "Controller/PNPlayerController.h"
#include "UI/PNChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Project9.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PNGameModeBase.h"
#include "Player/PNPlayerState.h"
#include "Net/UnrealNetwork.h"

APNPlayerController::APNPlayerController()
{
	bReplicates = true;
}

void APNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UPNChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void APNPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void APNPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		APNPlayerState* PNPS = GetPlayerState<APNPlayerState>();
		if (IsValid(PNPS) == true)
		{
			ServerRPCPrintChatMessageString(InChatMessageString);
		}
	}
}

void APNPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//µð¹ö±ë ·Î±×
	//FString NetModeString = Project9FunctionLibrary::GetNetModeString(this);
	//FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	//Project9FunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);

	Project9FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);

}

void APNPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void APNPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		APNGameModeBase* PNGM = Cast<APNGameModeBase>(GM);
		if (IsValid(PNGM) == true)
		{
			APNPlayerState* PNPS = GetPlayerState<APNPlayerState>();
			if (IsValid(PNPS) == true)
			{

				int Index = InChatMessageString.Len() - 3;
				FString GuessNumberString = InChatMessageString.RightChop(Index); 

				if (PNGM->IsGuessNumberString(GuessNumberString) == true)
				{
					PNGM->IncreaseGuessCount(this);
				}

				FString CombinedMessageString = PNPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

				PNGM->PrintChatMessageString(this, CombinedMessageString);

			}
		}
	}
}

void APNPlayerController::ClientRPC_ShowNotificationText_Draw_Implementation()
{
	NotificationText = FText::FromString("Draw...");

	FTimerHandle NotificationClearHandle;

	GetWorld()->GetTimerManager().SetTimer(
		NotificationClearHandle,
		this,
		&APNPlayerController::ClearNotification,
		5.f,
		false);
}

void APNPlayerController::ClientRPC_ShowNotificationText_Win_Implementation()
{
	APNPlayerState* PNPS = GetPlayerState<APNPlayerState>();
	if (IsValid(PNPS) == true)
	{
		FString CombinedMessageString = PNPS->PlayerNameString + TEXT(" Winner!");
		NotificationText = FText::FromString(CombinedMessageString);

		FTimerHandle NotificationClearHandle;

		GetWorld()->GetTimerManager().SetTimer(
			NotificationClearHandle,
			this,
			&APNPlayerController::ClearNotification,
			5.f,
			false);
	}
}

void APNPlayerController::ClientRPC_ShowNotificationText_Lose_Implementation()
{
	APNPlayerState* PNPS = GetPlayerState<APNPlayerState>();
	if (IsValid(PNPS) == true)
	{
		FString CombinedMessageString = PNPS->PlayerNameString + TEXT(" Looser!");
		NotificationText = FText::FromString(CombinedMessageString);

		FTimerHandle NotificationClearHandle;

		GetWorld()->GetTimerManager().SetTimer(
			NotificationClearHandle,
			this,
			&APNPlayerController::ClearNotification,
			5.f,
			false);
	}
}

void APNPlayerController::ClearNotification()
{
	NotificationText = FText::GetEmpty();
}