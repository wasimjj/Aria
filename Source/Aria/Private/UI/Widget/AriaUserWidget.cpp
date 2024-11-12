// Copyright Savoie Interactive Studios


#include "UI/Widget/AriaUserWidget.h"

void UAriaUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
