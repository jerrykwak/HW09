// HW09PlayerController.cpp


#include "Player/HW09PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HW09/HW09.h"
#include "UI/HW09ChatInput.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HW09GameModeBase.h"
#include "HW09PlayerState.h"
#include "Net/UnrealNetwork.h"


AHW09PlayerController::AHW09PlayerController()
{
	bReplicates = true;
}

void AHW09PlayerController::BeginPlay()
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
		ChatInputWidgetInstance = CreateWidget<UHW09ChatInput>(this, ChatInputWidgetClass);
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

void AHW09PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController() == true)
	{
		AHW09PlayerState* HW09PS = GetPlayerState<AHW09PlayerState>();
		if (IsValid(HW09PS) == true)
		{
			FString CombinedMessageString = HW09PS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AHW09PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, InChatMessageString, true, true, FLinearColor::Green, 5.0f);
}

void AHW09PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void AHW09PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AHW09PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AHW09GameModeBase* HW09GM = Cast<AHW09GameModeBase>(GM);
		if (IsValid(HW09GM) == true)
		{
			HW09GM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}