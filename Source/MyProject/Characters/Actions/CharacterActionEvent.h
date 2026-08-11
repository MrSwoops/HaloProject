#pragma once

#include "GameplayTagContainer.h"
#include "CharacterActionEvent.generated.h"

struct FGameplayTag;

UENUM(BlueprintType)
enum class ECharacterActionEventInteractionBehavior : uint8
{
	Default = 0,
	Buffer = 1,
	Override = 2,
	
};

UCLASS(Blueprintable, BlueprintType)
class UCharacterActionEvent : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Priority")
	FGameplayTag ActionTypeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Priority") // Action will always play, overriding any current action
	bool bMasterPriority = false; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "!bMasterPriority", EditConditionHides), Category = "Priority") // Determines if this action can override itself if called again
	bool bCanOverrideSelf = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "!bMasterPriority", EditConditionHides), Category = "Priority") // Determines which actions this action can override
	FGameplayTagContainer PriorityMap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FName TargetSlot = "DefaultSlot";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation") // Heart of the event. Should contain notifies to drive logic
	UAnimSequenceBase* EventAnimSequence = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffering") // Whether an action may be buffered instantly. If not, actions may only be buffered after being activated using anim notifies
	bool bCanInstantlyBuffer = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffering") // Whether the action can be buffered
	bool bBufferable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid ActionEventId = FGuid::NewGuid();
};
