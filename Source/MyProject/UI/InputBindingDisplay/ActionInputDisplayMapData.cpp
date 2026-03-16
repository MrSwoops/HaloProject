#include "ActionInputDisplayMapData.h"

const FInputPromptData* UActionInputDisplayMapData::GetInputPromptData(const FKey& InKey)
{
	if (KeyMappings.Contains(InKey))
		return &KeyMappings[InKey];
	return nullptr;
}
