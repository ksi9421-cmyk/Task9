#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PNGameModeBase.generated.h"

class APNPlayerController;

UCLASS()
class PROJECT9_API APNGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	void PrintChatMessageString(APNPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void IncreaseGuessCount(APNPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(APNPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<APNPlayerController>> AllPlayerControllers;
};
