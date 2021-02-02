// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHumanCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


AMainHumanCharacter::AMainHumanCharacter()
{
	PrimaryActorTick.bCanEverTick = false;//Þimdilik Tick fonksiyonuna ihtiyacýmýz yok.
	
	Mihenk = CreateDefaultSubobject<UStaticMeshComponent>("Mihenk");//karakterin üzerine baðlý bir mihenk taþý oluþturduk.
	Mihenk->SetupAttachment(GetCapsuleComponent());//Mihenk taþýný karakterin meshinin üstüne baðladýk.
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");//karakterin uzerine baglý bir spring arm olusturduk.
	SpringArm->SetupAttachment(Mihenk);//Spring arm 'i Mihenk taþýna baðladýk.

	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");//Karakterin üzerine baðlý bir kamera oluþturuduk.
	Camera->SetupAttachment(SpringArm);//Kamerayý spring arm ýn üzerine yerleþtirdik.

	InitialCamearPicthRotation = Camera->GetRelativeRotation().Yaw;//Deðeri baþlattýk.

	GetCharacterMovement()->MaxWalkSpeed = CharacterForwardMovementSpeed;
	GetCharacterMovement()->JumpZVelocity = CharacterJumpVelocity;
}

void AMainHumanCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &AMainHumanCharacter::ForwardMove);//Forward Axix eventini Forwardmove fonksiyonuna a baðladýk.
	PlayerInputComponent->BindAxis("Right", this, &AMainHumanCharacter::RightMove);//Right axix eventini rightmove fonksiyonuna a baðladýk.

	PlayerInputComponent->BindAxis("MouseX", this, &AMainHumanCharacter::CameraX);//MouseX i CameraX e baðladýk.
	PlayerInputComponent->BindAxis("MouseY", this, &AMainHumanCharacter::CameraY);//MouseY yi CameraY ye baðladýk.

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainHumanCharacter::Jumping);//Jump action eventini Jumping fonksiyonuna baðladýk.
}

void AMainHumanCharacter::ForwardMove(float Value)
{
	if (Controller != NULL && Value != 0.0f)//Eðer Controller var ve value 0 deðilse
	{
		FRotator MihenkYaw = Mihenk->GetRelativeRotation();
		MihenkYaw.Yaw -= 90.0f;
		//Karakterin baktýðý yeri mihenk ile eþitmelemiz gerekiyordu. Ancak eþitleme durumunda karakter 90 derece fazla saða kayýyordu.  Bu yüzden <str 58> 
		GetMesh()->SetRelativeRotation(MihenkYaw);

		AddMovementInput(Mihenk->GetRelativeRotation().Vector(), Value * GetWorld()->DeltaTimeSeconds * CharacterForwardMovementSpeed);
	}
}

void AMainHumanCharacter::RightMove(float Value)
{
	if (Controller != NULL && Value != 0.0f)
	{
		FRotator MihenkYaw = Mihenk->GetRelativeRotation();//Mihenk taþýnýn rotasyonunu aldýk.
		MihenkYaw.Yaw -= 90.0f;//Eger Meshin rotasyonunu mihenkle eþitlersek mesh 90 derece fazla saða yatýk kalýyor bu yüzden <str 58>

		GetMesh()->SetRelativeRotation(MihenkYaw);//Mesh ile mihenki eþitledik
		AddMovementInput(MihenkYaw.Vector(), Value * GetWorld()->DeltaTimeSeconds * CharacterRightMovementSpeed);//Mesh fazladan saga yatýk olduðundan  right vector yerine de geçebilir.
	}
}

void AMainHumanCharacter::Jumping()
{
	if (!ensure(GetCharacterMovement() != nullptr)) return;
	if (GetCharacterMovement()->IsFalling()) return;
	Jump();
}

void AMainHumanCharacter::CameraX(float Value)
{
	Mihenk->AddRelativeRotation(FRotator(0, Value * GetWorld()->DeltaTimeSeconds * CameraMouseXSpeed, 0));
}

void AMainHumanCharacter::CameraY(float Value)
{
	if (Value == 0) { return; }
	if (SpringArm->GetRelativeRotation().Pitch <= (InitialCamearPicthRotation - MaxCameraPitch) && Value < 0.f) { return; }
	if (SpringArm->GetRelativeRotation().Pitch >= (InitialCamearPicthRotation + MinCameraPitch) && Value > 0.f) { return; }
	SpringArm->AddRelativeRotation(FRotator(Value * GetWorld()->DeltaTimeSeconds * CameraMouseYSpeed, 0, 0));
}