#include "GP3AIBTPerformSearch.h"
#include "GP3AIPawnStalker.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP3_Team4/PlayerCharacter/GP3PawnPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

UGP3AIBTPerformSearch::UGP3AIBTPerformSearch()
{
	NodeName = "Wait";
	SearchTimePerState = 2.0f;
	SearchPadding = 50.0f;
	SearchDistance = 300.f;
	CurrentSearchState = 0;
	RemainingSearchTime = 0.f;

	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UGP3AIBTPerformSearch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());
	if(!AIcon)
	{
		return EBTNodeResult::Failed;
	}

	PawnStalker = Cast<AGP3AIPawnStalker>(AIcon->GetPawn());

	if(!PawnStalker)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();


	if(!BlackboardComp)
	{
			return EBTNodeResult::Failed;
	}
	
	RemainingSearchTime = SearchTimePerState;
	CurrentSearchState = 0;
	LerpTimeElapsed= 0.f;
	bExtendingForward = true;

	/*UKismetSystemLibrary::DrawDebugCircle(GetWorld(), AIcon->GetPawn()->GetActorLocation() + FVector(0,0,
        AIcon->GetPawn()->GetDefaultHalfHeight() - SearchPadding), SearchDistance,40 , FColor::Red,
        SearchTimePerState, 5.f, FVector(1,0, 0), FVector(0,1,0), true );
        */

	

	PawnStalker->StartSearch(AIcon->GetPawn()->GetDefaultHalfHeight() - SearchPadding);

	return::EBTNodeResult::InProgress;
}

void UGP3AIBTPerformSearch::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());
	
	if(!AIcon)
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();

	if(!BlackboardComp)
	{
		return;
	}
	
	
	AActor* Player = Cast<AActor>(BlackboardComp->GetValueAsObject("Player"));
	APawn* Pawn = AIcon->GetPawn();

	
//PERFORMING SEARCH

	if(!Pawn)
	{
		return;
	}

	FVector Position = AIcon->GetPawn()->GetActorLocation();

	float ZValue;
	if(CurrentSearchState == 0)
	{
		ZValue = Pawn->GetActorLocation().Z + Pawn->GetDefaultHalfHeight() - SearchPadding;	
	}
	else if (CurrentSearchState ==1)
	{
		ZValue = AIcon->GetPawn()->GetActorLocation().Z;
	}
	else
	{
		ZValue = Pawn->GetActorLocation().Z -Pawn->GetDefaultHalfHeight() + SearchPadding;
	}

	FVector Start(Pawn->GetActorLocation().X, Pawn->GetActorLocation().Y, ZValue);

	FVector PlayerLoc = Player->GetActorLocation();
	FVector Direction(PlayerLoc.X-Start.X,PlayerLoc.Y-Start.Y, 0);
	FVector DirectionNormal = Direction.GetSafeNormal();

	LerpTimeElapsed += GetWorld()->DeltaTimeSeconds *2;

	if(LerpTimeElapsed >= SearchTimePerState && bExtendingForward == true)
	{
		LerpTimeElapsed= 0.f;
		bExtendingForward = false;
	}
	else if(LerpTimeElapsed >= SearchTimePerState && bExtendingForward == false)
	{
		LerpTimeElapsed= 0.f;
		bExtendingForward = true;
	}
	
	Direction = DirectionNormal * SearchDistance;
	
	FVector End = Start + Direction;

	FVector SearchPosition;
	
	if(bExtendingForward)
	{
		SearchPosition = FMath::Lerp(Start, End, (LerpTimeElapsed/SearchTimePerState));
	}else
	{
		SearchPosition = FMath::Lerp(End, Start, (LerpTimeElapsed/SearchTimePerState));
	}

	AGP3AIPawnStalker* AI = Cast<AGP3AIPawnStalker>(AIcon->GetPawn());
	AI->TentacleLength = FVector::Distance(Start, SearchPosition);
	
	//FVector SearchPosition = FMath::Lerp(Start, End, TimeElapsed/SearchTimePerState);
		
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	
	//bool bHit = UKismetSystemLibrary::LineTraceSingle( GetWorld(), Start, SearchPosition, ETraceTypeQuery::TraceTypeQuery2, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true, FLinearColor::Green);
	bool bHit = UKismetSystemLibrary::LineTraceSingle( GetWorld(), Start, SearchPosition, ETraceTypeQuery::TraceTypeQuery2, false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	if(bHit)
	{
		AGP3PawnPlayer* PlayerPawn = Cast<AGP3PawnPlayer>(Hit.Actor);

		if(PlayerPawn)
		{
			AActor* Act = Cast<AActor>(PlayerPawn);

			//BlackboardComp->SetValueAsObject("Target", Act);
			BlackboardComp->SetValueAsBool("bChasePlayer", true);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

		
	}

//DEBUG STUFF

	if(RemainingSearchTime <= 0.0f)
	{
		if(CurrentSearchState == 0)
		{
			
			/*UKismetSystemLibrary::DrawDebugCircle(GetWorld(), AIcon->GetPawn()->GetActorLocation(), SearchDistance,40 , FColor::Red,
                SearchTimePerState, 5.f, FVector(1,0, 0), FVector(0,1,0), true );
                */

			PawnStalker->StartSearch(0.f);

		}

		if(CurrentSearchState == 1)
		{
			/*UKismetSystemLibrary::DrawDebugCircle(GetWorld(), AIcon->GetPawn()->GetActorLocation() - FVector(0,0,
                AIcon->GetPawn()->GetDefaultHalfHeight()) + FVector(0,0,SearchPadding), SearchDistance,40 , FColor::Red,
                SearchTimePerState, 5.f, FVector(1,0, 0), FVector(0,1,0), true );
                */

			PawnStalker->StartSearch(-AIcon->GetPawn()->GetDefaultHalfHeight() + SearchPadding);

		}
	}
	
//SETTING TIMERS AND STATES

	
	if(CurrentSearchState == 2 && RemainingSearchTime <= 0.0f)
	{
		CurrentSearchState = 0;
		RemainingSearchTime = SearchTimePerState;
			
		if(AIcon)
		{
			if(BlackboardComp)
			{
				BlackboardComp->SetValueAsBool("bSearch", false);
				BlackboardComp->SetValueAsBool("bChasePlayer", false);
			}
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	if(RemainingSearchTime <= 0.0f)
	{
		CurrentSearchState++;
		RemainingSearchTime = SearchTimePerState;
		
	}else
	{
		RemainingSearchTime -= DeltaSeconds;
	}
}
