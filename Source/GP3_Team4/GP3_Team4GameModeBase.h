#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GP3_Team4GameModeBase.generated.h"

UCLASS()
class GP3_TEAM4_API AGP3_Team4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGP3_Team4GameModeBase();
	virtual void BeginPlay() override;
};
