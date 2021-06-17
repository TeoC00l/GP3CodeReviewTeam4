#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP3HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, int, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_TEAM4_API UGP3HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintPure)
	int GetCurrentHealth() const;
	
	UFUNCTION(BlueprintCallable)
	void Damage(int Damage);

	UFUNCTION(BlueprintCallable)
	void Heal(int Heal);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathEvent OnDeathEvent;

	UFUNCTION(BlueprintPure)
	int GetMaxHealth() const;

private:
	UPROPERTY(EditDefaultsOnly, Category=Health)
	int MaxHealth = 3;

	UPROPERTY(Transient)
	int CurrentHealth;
};
