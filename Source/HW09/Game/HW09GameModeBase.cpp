// HW09GameModeBase.cpp

#include "Game/HW09GameModeBase.h"
#include "HW09GameStateBase.h"
#include "Player/HW09PlayerController.h"
#include "EngineUtils.h"
#include "Player/HW09PlayerState.h"

void AHW09GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHW09PlayerController* HW09PlayerController = Cast<AHW09PlayerController>(NewPlayer);
	if (IsValid(HW09PlayerController) == true)
	{
		HW09PlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		AllPlayerControllers.Add(HW09PlayerController);

		AHW09PlayerState* HW09PS = HW09PlayerController->GetPlayerState<AHW09PlayerState>();
		if (IsValid(HW09PS) == true)
		{
			HW09PS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AHW09GameStateBase* HW09GameStateBase =  GetGameState<AHW09GameStateBase>();
		if (IsValid(HW09GameStateBase) == true)
		{
			HW09GameStateBase->MulticastRPCBroadcastLoginMessage(HW09PS->PlayerNameString);
		}
	}
}

FString AHW09GameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AHW09GameModeBase::IsGuessNumberString(const FString& InNumberString)
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

FString AHW09GameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
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

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AHW09GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();

	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
}

void AHW09GameModeBase::PrintChatMessageString(AHW09PlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	{
		FString ChatMessageString = InChatMessageString;
		int Index = InChatMessageString.Len() - 3;
		FString GuessNumberString = InChatMessageString.RightChop(Index);
		if (IsGuessNumberString(GuessNumberString) == true)
		{
			FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

			IncreaseGuessCount(InChattingPlayerController);
			
			for (TActorIterator<AHW09PlayerController> It(GetWorld()); It; ++It)
			{
				AHW09PlayerController* HW09PlayerController = *It;
				if (IsValid(HW09PlayerController) == true)
				{
					FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
					HW09PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

					int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
					JudgeGame(InChattingPlayerController, StrikeCount);
				}
			}
		}
		else
		{
			for (TActorIterator<AHW09PlayerController> It(GetWorld()); It; ++It)
			{
				AHW09PlayerController* HW09PlayerController = *It;
				if (IsValid(HW09PlayerController) == true)
				{
					HW09PlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
				}
			}
		}
	}
}

void AHW09GameModeBase::IncreaseGuessCount(AHW09PlayerController* InChattingPlayerController)
{
	AHW09PlayerState* HW09PS = InChattingPlayerController->GetPlayerState<AHW09PlayerState>();
	if (IsValid(HW09PS) == true)
	{
		HW09PS->CurrentGuessCount++;
	}
}

void AHW09GameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& HW09PlayerController : AllPlayerControllers)
	{
		AHW09PlayerState* HW09PS = HW09PlayerController->GetPlayerState<AHW09PlayerState>();
		if (IsValid(HW09PS) == true)
		{
			HW09PS->CurrentGuessCount = 0;
		}
	}
}

void AHW09GameModeBase::JudgeGame(AHW09PlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AHW09PlayerState* HW09PS = InChattingPlayerController->GetPlayerState<AHW09PlayerState>();
		for (const auto& HW09PlayerController : AllPlayerControllers)
		{
			if (IsValid(HW09PS) == true)
			{
				FString CombinedMessageString = HW09PS->PlayerNameString + TEXT(" has won the game.");
				HW09PlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& HW09PlayerController : AllPlayerControllers)
		{
			AHW09PlayerState* HW09PS = HW09PlayerController->GetPlayerState<AHW09PlayerState>();
			if (IsValid(HW09PS) == true)
			{
				if (HW09PS->CurrentGuessCount < HW09PS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& HW09PlayerController : AllPlayerControllers)
			{
				HW09PlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
