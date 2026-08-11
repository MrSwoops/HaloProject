#include "CommonStackManager.h"

void UCommonStackManager::PushPage(const int32& PageIndex)
{
	PageIndexStack.Push(PageIndex);
	SetActiveWidgetIndex(PageIndexStack.Top());
}

void UCommonStackManager::PopPage()
{
	if (PageIndexStack.Num() <= 0 || (bBasePageUnPoppable && PageIndexStack.Top() == RootPage)) return;
	PageIndexStack.Pop();
	if (PageIndexStack.Num() <= 0) return;
	SetActiveWidgetIndex(PageIndexStack.Top());
}