#pragma once

#include "ActionRequest.generated.h"

class UCharacterActionEvent;

DECLARE_DELEGATE(FActionCallback);

USTRUCT(BlueprintType, Blueprintable)
struct FActionRequest
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCharacterActionEvent> Action = nullptr;

	FActionCallback OnTriggered;
	FActionCallback OnCanceled;
	FActionCallback OnEnded;

	bool IsValid() const
	{
		return Action != nullptr;
	}

	void Reset()
	{
		Action = nullptr;
		OnTriggered = nullptr;
		OnCanceled = nullptr;
		OnEnded = nullptr;
	}
};
