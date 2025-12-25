// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class ROETASK1_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    // === Components ===
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* Camera;

    // === Touch State ===
    bool bIsDragging = false;

    FVector2D LastTouchPosition;
    FVector2D CurrentTouchPosition;

    // === Movement Settings ===
    UPROPERTY(EditAnywhere, Category = "RTS Camera")
    float CameraMoveSpeed = 1000.f;

    UPROPERTY(EditAnywhere, Category = "RTS Camera")
    float MaxDeltaClamp = 20.f;

    // === Input Handlers ===
    void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
    void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);
    void OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location);

    void MoveCamera(const FVector2D& ScreenDelta, float DeltaTime);
};
