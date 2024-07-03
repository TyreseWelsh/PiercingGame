// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttack.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "../Public/PlayerCharacter.h"

#include "../Public/Damageable.h"

// Sets default values
ABasicAttack::ABasicAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PivotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	RootComponent = PivotPoint;

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->InitBoxExtent(FVector(AttackLength, 10.f, 32.f));
	AttackCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AttackCollider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	AttackCollider->SetupAttachment(RootComponent);
	AttackCollider->SetRelativeLocation(FVector(AttackLength, 0.f, 0.f));
	AttackCollider->bHiddenInGame = false;
	AttackCollider->SetupAttachment(RootComponent);

	AttackSprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("AttackSprite"));
	AttackSprite->AlwaysLoadOnClient = true;
	AttackSprite->SetRelativeScale3D(FVector(2.f, 1.f, 2.f));
	AttackSprite->AlwaysLoadOnServer = true;
	AttackSprite->bOwnerNoSee = false;
	AttackSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackSprite->SetLooping(false);
	AttackSprite->SetupAttachment(AttackCollider);
}

// Called when the game starts or when spawned
void ABasicAttack::BeginPlay()
{
	Super::BeginPlay();

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ABasicAttack::BeginOverlap);

	SetLifeSpan(AttackSprite->GetFlipbookLength());
}

void ABasicAttack::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "begin overlap");

	if (IDamageable* DamageableInterface = Cast<IDamageable>(OtherActor))
	{

		if (!bJumpHasReset && OwningPlayer != nullptr)
		{
			FTransform AttackHitEffectTransform;
			AttackHitEffectTransform.SetLocation(FVector(OtherActor->GetActorLocation().X, OtherActor->GetActorLocation().Y + 5, OtherActor->GetActorLocation().Z));
			AttackHitEffectTransform.SetRotation(FVector(0, 0, 0).ToOrientationQuat());
			AttackHitEffectTransform.SetScale3D(FVector::One());

			AttackHitActor = GetWorld()->SpawnActor<AActor>(AttackHit, AttackHitEffectTransform);
			//AttackHitActor->SetActorLocation(FVector(OtherActor->GetActorLocation().X, OtherActor->GetActorLocation().Y + 2, OtherActor->GetActorLocation().Z));
			AttackHitActor->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			bJumpHasReset = true;
			OwningPlayer->JumpCurrentCount = 0;
			GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "COLLISION");

		}
		
	}
}

// Called every frame
void ABasicAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicAttack::SetOwningPlayer(APlayerCharacter* NewOwner)
{
	OwningPlayer = NewOwner;
	GEngine->AddOnScreenDebugMessage(int32(-1), 20.f, FColor::Green, "OWNING PLAYer set");

}
