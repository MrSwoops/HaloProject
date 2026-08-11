#include "ActionHandler.h"

#include "CharacterActionEvent.h"
#include "GameFramework/Character.h"
#include "MyProject/Characters/GameplayCharacter.h"

void UActionHandler::InitializeActionComponent(AGameplayCharacter* InCharacter)
{
	if (!InCharacter) { UE_LOG(LogTemp, Error, TEXT("Invalid character given in UActionHandler::InitializeComponent")); return; }
	CharacterOwner = InCharacter;

	OnEndActionCleanup.RemoveDynamic(InCharacter, &AGameplayCharacter::OnActionEndCleanup);
	OnEndActionCleanup.AddDynamic(InCharacter, &AGameplayCharacter::OnActionEndCleanup);
}

void UActionHandler::TriggerActionEvent(const FActionRequest& InEvent)
{
	if (!InEvent.IsValid()) { UE_LOG(LogTemp, Warning, TEXT("Tried to trigger an invalid character action event in ACustomCharacter::TriggerActionEvent")); return; }
	if (CurrentAction.IsValid())
	{
		if (InEvent.Action->bMasterPriority || CurrentAction.Action->ActionTypeTag.MatchesAny(InEvent.Action->PriorityMap) || (InEvent.Action->ActionEventId == CurrentAction.Action->ActionEventId && InEvent.Action->bCanOverrideSelf)) // Override Current Event
		{
			if (auto* AnimInst = GetAnimInstance())
			{
				if (AnimInst->Montage_IsPlaying(ActiveAnimMontage)) AnimInst->Montage_Stop(0.0f, ActiveAnimMontage);
				EndOfActionCleanup();
				InternalTriggerActionEvent(InEvent); // Trigger Action
			}
		}
		else // Buffer Action
		{
			BufferActionEvent(InEvent, InEvent.Action->bBufferable && bCanBuffer);
		}
	}
	else
	{
		InternalTriggerActionEvent(InEvent); // Trigger Action
	}
}
void UActionHandler::InternalTriggerActionEvent(const FActionRequest& InEvent)
{
	if (!InEvent.IsValid()) { UE_LOG(LogTemp, Warning, TEXT("Tried to trigger an invalid character action event in ACustomCharacter::InternalTriggerActionEvent")); return; }

	if (auto* AnimInst = GetAnimInstance())
	{
		CurrentAction = InEvent;
		bCanBuffer = InEvent.Action->bBufferable && InEvent.Action->bCanInstantlyBuffer;
		ActiveAnimMontage = AnimInst->PlaySlotAnimationAsDynamicMontage(InEvent.Action->EventAnimSequence, InEvent.Action->TargetSlot);
		if (ActiveAnimMontage)
		{
			CurrentAction.OnTriggered.ExecuteIfBound();
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UActionHandler::OnActionFinished);
			AnimInst->Montage_SetEndDelegate(EndDelegate, ActiveAnimMontage);
		}
	}
}
void UActionHandler::BufferActionEvent(const FActionRequest& InEvent, const bool& bBufferCheck)
{
	if (!InEvent.IsValid()) { UE_LOG(LogTemp, Warning, TEXT("Tried to buffer an invalid character action event in ACustomCharacter::BufferAction")); return; }
	if (!bBufferCheck) return;
	
	BufferedAction = InEvent;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(BufferCheckHandle)) { TimerManager.SetTimer(BufferCheckHandle, this, &UActionHandler::BufferCheck, ActionBufferCheckRate, true); }
}

void UActionHandler::ForceSetBufferedAction(const FActionRequest& InEvent)
{
	BufferedAction = InEvent;
}

void UActionHandler::OnActionFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != ActiveAnimMontage) return;
	if (bInterrupted)
	{
		CurrentAction.OnCanceled.ExecuteIfBound();
		EndOfActionCleanup();
		UE_LOG(LogTemp, Warning, TEXT("Action interrupted in ACustomCharacter::OnActionFinished"));
		return;
	}

	CurrentAction.OnEnded.ExecuteIfBound();
	EndOfActionCleanup();
	if (BufferedAction.IsValid())
	{
		InternalTriggerActionEvent(BufferedAction); // Trigger Buffered Action
		BufferedAction.Reset();
		GetWorld()->GetTimerManager().ClearTimer(BufferCheckHandle);
	}
}

void UActionHandler::BufferCheck()
{
	if (!bAtActionExitRegion) return;
	if (!BufferedAction.IsValid()) { GetWorld()->GetTimerManager().ClearTimer(BufferCheckHandle); return; }

	if (auto* AnimInst = GetAnimInstance())
	{
		if (AnimInst->Montage_IsPlaying(ActiveAnimMontage)) AnimInst->Montage_Stop(0.0f, ActiveAnimMontage);
		EndOfActionCleanup();
		InternalTriggerActionEvent(BufferedAction); // Trigger Buffered Action
		BufferedAction.Reset();
		GetWorld()->GetTimerManager().ClearTimer(BufferCheckHandle);
	}
}

void UActionHandler::EndOfActionCleanup()
{
	CurrentAction.Reset();
	ActiveAnimMontage = nullptr;
	bCanBuffer = false;
	bAtActionExitRegion = false;
	OnEndActionCleanup.Broadcast();
}

UAnimInstance* UActionHandler::GetAnimInstance()
{
	if (!CharacterOwner)
	{
		if (auto* Character = Cast<AGameplayCharacter>(GetOwner()))
		{
			CharacterOwner = Character;
			return Character->GetMesh()->GetAnimInstance();
		}
	}
	else
	{
		return CharacterOwner->GetMesh()->GetAnimInstance();
	}
	return nullptr;
}
