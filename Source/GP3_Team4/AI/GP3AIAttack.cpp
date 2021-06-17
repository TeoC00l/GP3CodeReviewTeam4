// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AIAttack.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP3_Team4/PlayerCharacter/GP3PlayerCharacter.h"
#include "GP3_Team4/PlayerCharacter/Health/GP3HealthComponent.h"

UGP3AIAttack::UGP3AIAttack()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UGP3AIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AICon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());

	float AttackRangeSq = 400*400;
	
	if(AICon)
	{
		UBlackboardComponent* BlackboardComponent = AICon->GetBlackBoardComp();

		AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject("Player"));

		APawn* Self = AICon->GetPawn();


float DistSq = FVector::DistSquared(Self->GetActorLocation(), Target->GetActorLocation());
		
		if(DistSq < AttackRangeSq)
		{
			UGP3HealthComponent* TargetHealth = Target->FindComponentByClass<UGP3HealthComponent>();
		
			TargetHealth->Damage(1);

			UE_LOG(LogTemp, Warning, TEXT("Bam"));

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
