#pragma once

#include "ActionRequest.h"
#include "ActionHandler.generated.h"

class AGameplayCharacter;
class UCharacterActionEvent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndActionCleanup);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UActionHandler : public UActorComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void TriggerActionEvent(const FActionRequest& InEvent);
	void InitializeActionComponent(AGameplayCharacter* InCharacter);
	void BufferActionEvent(const FActionRequest& InEvent, const bool& bBufferCheck);

	void ForceSetBufferedAction(const FActionRequest& InEvent);

	// Flags
	UFUNCTION(BlueprintCallable) // Sets whether the action is in a state where a buffered action can start
	void SetBufferExitRegion(const bool& bActive) { bAtActionExitRegion = bActive; }
	UFUNCTION(BlueprintCallable) // Sets whether an action can be buffered
	void SetCanBuffer(const bool& bActive) { bCanBuffer = bActive; }

	UFUNCTION(BlueprintCallable)
	bool HasActiveAction() { return ActiveAnimMontage != nullptr;}
	UFUNCTION(BlueprintCallable)
	const FActionRequest& GetCurrentAction() { return CurrentAction; }

	UPROPERTY(BlueprintAssignable)
	FOnEndActionCleanup OnEndActionCleanup;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions|Settings")
	float ActionBufferCheckRate = 0.1f;

private:
	UFUNCTION(BlueprintCallable)
	void EndOfActionCleanup();
	UFUNCTION(BlueprintCallable)
	void BufferCheck();

	UPROPERTY()
	AGameplayCharacter* CharacterOwner = nullptr;
	FActionRequest CurrentAction;
	FActionRequest BufferedAction;
	UPROPERTY()
	UAnimMontage* ActiveAnimMontage;
	FTimerHandle BufferCheckHandle;
	bool bCanBuffer = false;
	bool bAtActionExitRegion = false;
	void InternalTriggerActionEvent(const FActionRequest& InEvent);
	
	UFUNCTION()
	void OnActionFinished(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	UAnimInstance* GetAnimInstance();
};
