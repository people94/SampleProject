// Fill out your copyright notice in the Description page of Project Settings.


#include "SPBullet.h"
#include "Kismet/GameplayStatics.h"
#include "SPWeapon.h"
#include "Components/SphereComponent.h"

ASPBullet::ASPBullet()
{
}

void ASPBullet::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	debugf(TEXT("Child OnBeginOverlap %s"), *OtherActor->GetName());
}

void ASPBullet::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	debugf(TEXT("Child OnBeginOverlap%s"), *OtherActor->GetName());
	if (OtherActor != nullptr)
	{
		ASPWeapon* OwnerWeapon = Cast<ASPWeapon>(Owner);
		if (OwnerWeapon)
		{
			float Damage = OwnerWeapon->GetDamage() * DamageAmp;
			UGameplayStatics::ApplyDamage(OtherActor, Damage, Owner->GetInstigatorController(), this, OwnerWeapon->GetDamageType());
		}
	}
}
