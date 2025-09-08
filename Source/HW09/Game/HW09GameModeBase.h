// HW09GameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HW09GameModeBase.generated.h"

class AHW09PlayerController;

/**
 * 
 */
UCLASS()
class HW09_API AHW09GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	
	void PrintChatMessageString(AHW09PlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(AHW09PlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(AHW09PlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AHW09PlayerController>> AllPlayerControllers;
};
