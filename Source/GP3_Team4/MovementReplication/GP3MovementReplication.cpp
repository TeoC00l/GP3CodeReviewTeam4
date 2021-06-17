#include "GP3MovementReplication.h"

void UGP3MovementReplication::CreateConnection(AActor* Connection)
{
	ConnectedActor = Connection;
}

void UGP3MovementReplication::SetPartnerAsParent()
{
	if(!ConnectedActor)
	{
		return;
	}
	DetachFromParent();
	GetOwner()->AttachToActor(ConnectedActor, FAttachmentTransformRules::KeepWorldTransform);
}

void UGP3MovementReplication::SetPartnerAsChild()
{
	if(!ConnectedActor)
	{
		return;
	}
	DetachFromParent();
	UGP3MovementReplication* Replication = Cast<UGP3MovementReplication>(ConnectedActor->GetComponentByClass(UGP3MovementReplication::StaticClass()));
	if(Replication)
	{
		Replication->SetPartnerAsParent();
	}}

void UGP3MovementReplication::DetachFromParent()
{
	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UGP3MovementReplication::SetPartnerOffset()
{
	if(!ConnectedActor)
	{
		return;
	}

	OffsetVector = GetOwner()->GetActorLocation() - ConnectedActor->GetActorLocation();
}

void UGP3MovementReplication::ApplyPositionOffset()
{
	if(!ConnectedActor && OffsetVector != FVector::ZeroVector)
	{
		return;
	}
	GetOwner()->SetActorLocation(ConnectedActor->GetActorLocation() + OffsetVector);
}

void UGP3MovementReplication::ApplyOffsetToPartner()
{
	if(!ConnectedActor)
	{
		return;
	}
	UGP3MovementReplication* Replication = Cast<UGP3MovementReplication>(ConnectedActor->GetComponentByClass(UGP3MovementReplication::StaticClass()));
	if(Replication)
	{
		Replication->ApplyPositionOffset();
	}
		
	
}

void UGP3MovementReplication::BreakPartnerConnection()
{
	ConnectedActor = nullptr;
}
