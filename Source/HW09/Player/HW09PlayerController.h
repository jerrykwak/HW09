// HW09PlyaerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW09PlayerController.generated.h"


class UHW09ChatInput;
class UUSerWidget;

/**
 * 
 */
UCLASS()
class HW09_API AHW09PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHW09PlayerController();
	
	virtual void BeginPlay() override;
	
	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW09ChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UHW09ChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
