#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PNChatInput.generated.h"

class UEditableTextBox;

UCLASS()
class PROJECT9_API UPNChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

};
