#include "GP3HealthComponent.h"

void UGP3HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(MaxHealth > 0)
	{
		CurrentHealth = MaxHealth;
	}
	else
	{
		MaxHealth = 3;
		CurrentHealth = MaxHealth;
	}
}

int UGP3HealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UGP3HealthComponent::Damage(int Damage)
{
	if(CurrentHealth == 0)
		return;

	if(Damage < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage is negative use heal to increase hp instead"));
		return;
	}
	
	if(CurrentHealth - Damage <= 0)
	{
		CurrentHealth = 0;
		OnHealthChange.Broadcast(CurrentHealth);
		OnDeathEvent.Broadcast();

		return;
	}

	CurrentHealth -= Damage;
	OnHealthChange.Broadcast(CurrentHealth);
}

void UGP3HealthComponent::Heal(int Heal)
{
	if(CurrentHealth == MaxHealth)
	{
		return;
	}

	if(Heal < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Heal is negative use damage to decrease hp instead"));
		return;
	}
	
	if(CurrentHealth + Heal >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
		OnHealthChange.Broadcast(CurrentHealth);
		
		return;
	}

	CurrentHealth += Heal;
	OnHealthChange.Broadcast(CurrentHealth);
}

int UGP3HealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}
