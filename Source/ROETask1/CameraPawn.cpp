// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(Root);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(Root);
    CameraBoom->TargetArmLength = 1500.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(CameraBoom);

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraPawn::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
    bIsDragging = true;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        PC->GetInputTouchState(FingerIndex, LastTouchPosition.X, LastTouchPosition.Y, bIsDragging);
        CurrentTouchPosition = LastTouchPosition;
    }
}

void ACameraPawn::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
    bIsDragging = false;
}

void ACameraPawn::OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        bool bPressed;
        PC->GetInputTouchState(FingerIndex, CurrentTouchPosition.X, CurrentTouchPosition.Y, bPressed);
    }
}

void ACameraPawn::MoveCamera(const FVector2D& ScreenDelta, float DeltaTime)
{
    FVector Right = Camera->GetRightVector();
    FVector Forward = Camera->GetForwardVector();

    Forward.Z = 0.f;
    Forward.Normalize();

    FVector MoveDir =
        (-Right * ScreenDelta.X* 0.02f) +
        (-Forward * ScreenDelta.Y* 0.02f);

    FVector NewLocation = GetActorLocation();
    NewLocation += MoveDir * CameraMoveSpeed * DeltaTime;

    SetActorLocation(NewLocation);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (bIsDragging)
    {
        FVector2D Delta = CurrentTouchPosition - LastTouchPosition;
        Delta.X = FMath::Clamp(Delta.X, -20.f, 20.f);
        Delta.Y = FMath::Clamp(Delta.Y, -20.f, 20.f);

        MoveCamera(Delta, DeltaTime);
        LastTouchPosition = CurrentTouchPosition;
    }
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindTouch(IE_Pressed, this, &ACameraPawn::OnTouchPressed);
    PlayerInputComponent->BindTouch(IE_Released, this, &ACameraPawn::OnTouchReleased);
    PlayerInputComponent->BindTouch(IE_Repeat, this, &ACameraPawn::OnTouchMoved);
}

