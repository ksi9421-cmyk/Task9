#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PNPlayerController.generated.h"

class UPNChatInput;

UCLASS()
class PROJECT9_API APNPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APNPlayerController();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowNotificationText_Win();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowNotificationText_Lose();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowNotificationText_Draw();

	void ClearNotification();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPNChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UPNChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
