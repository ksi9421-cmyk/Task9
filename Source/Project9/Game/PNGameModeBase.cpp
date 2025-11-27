#include "Game/PNGameModeBase.h"
#include "Game/PNGameStateBase.h"
#include "Controller/PNPlayerController.h"
#include "EngineUtils.h"
#include "Player/PNPlayerState.h"

void APNGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APNPlayerController* PNPlayerController = Cast<APNPlayerController>(NewPlayer);
	if (IsValid(PNPlayerController) == true)
	{
		PNPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(PNPlayerController);

		APNPlayerState* PNPS = PNPlayerController->GetPlayerState<APNPlayerState>();
		if (IsValid(PNPS) == true)
		{
			PNPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		APNGameStateBase* PNGameStateBase = GetGameState<APNGameStateBase>();
		if (IsValid(PNGameStateBase) == true)
		{
			PNGameStateBase->MulticastRPCBroadcastLoginMessage(PNPS->PlayerNameString);
		}
	}
}

void APNGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void APNGameModeBase::PrintChatMessageString(APNPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);     //문자열의 왼쪽에서 Index만큼 잘라내고 남은 오른쪽 부분을 반환하는 함수
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));

		for (TActorIterator<APNPlayerController> It(GetWorld()); It; ++It)
		{
			APNPlayerController* PNPlayerController = *It;
			if (IsValid(PNPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				PNPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

			}
		}
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (TActorIterator<APNPlayerController> It(GetWorld()); It; ++It)
		{
			APNPlayerController* PNPlayerController = *It;
			if (IsValid(PNPlayerController) == true)
			{
				PNPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}


FString APNGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s][Authority=%d] SecretNumber: %s"), *GetName(), HasAuthority(), *Result);
	}
	return Result;
}

bool APNGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			if (UniqueDigits.Contains(C))
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString APNGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
}

void APNGameModeBase::IncreaseGuessCount(APNPlayerController* InChattingPlayerController)
{
	APNPlayerState* PNPS = InChattingPlayerController->GetPlayerState<APNPlayerState>();
	if (IsValid(PNPS) == true)
	{
		PNPS->CurrentGuessCount++;
	}
}

void APNGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& PNPlayerController : AllPlayerControllers)
	{
		APNPlayerState* PNPS = PNPlayerController->GetPlayerState<APNPlayerState>();
		if (IsValid(PNPS) == true)
		{
			PNPS->CurrentGuessCount = 0;
		}
	}
}

void APNGameModeBase::JudgeGame(APNPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		APNPlayerController* WinnerPC = InChattingPlayerController;


		for (const auto& PNPlayerController : AllPlayerControllers)
		{
			if (PNPlayerController == WinnerPC)
			{
				PNPlayerController->ClientRPC_ShowNotificationText_Win();
			}
			else
			{
				PNPlayerController->ClientRPC_ShowNotificationText_Lose();
			}
		}
		ResetGame();
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& PNPlayerController : AllPlayerControllers)
		{
			APNPlayerState* PNPS = PNPlayerController->GetPlayerState<APNPlayerState>();
			if (IsValid(PNPS) == true)
			{
				if (PNPS->CurrentGuessCount < PNPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& PNPlayerController : AllPlayerControllers)
			{
				PNPlayerController->ClientRPC_ShowNotificationText_Draw();
			}
			ResetGame();
		}
	}
}
