// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHumanCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


AMainHumanCharacter::AMainHumanCharacter()
{
	PrimaryActorTick.bCanEverTick = false;//�imdilik Tick fonksiyonuna ihtiyac�m�z yok.
	
	Mihenk = CreateDefaultSubobject<UStaticMeshComponent>("Mihenk");//karakterin �zerine ba�l� bir mihenk ta�� olu�turduk.
	Mihenk->SetupAttachment(GetCapsuleComponent());//Mihenk ta��n� karakterin meshinin �st�ne ba�lad�k.
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");//karakterin uzerine bagl� bir spring arm olusturduk.
	SpringArm->SetupAttachment(Mihenk);//Spring arm 'i Mihenk ta��na ba�lad�k.

	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");//Karakterin �zerine ba�l� bir kamera olu�turuduk.
	Camera->SetupAttachment(SpringArm);//Kameray� spring arm �n �zerine yerle�tirdik.

	InitialCamearPicthRotation = Camera->GetRelativeRotation().Yaw;//De�eri ba�latt�k.

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
	PlayerInputComponent->BindAxis("Forward", this, &AMainHumanCharacter::ForwardMove);//Forward Axix eventini Forwardmove fonksiyonuna a ba�lad�k.
	PlayerInputComponent->BindAxis("Right", this, &AMainHumanCharacter::RightMove);//Right axix eventini rightmove fonksiyonuna a ba�lad�k.

	PlayerInputComponent->BindAxis("MouseX", this, &AMainHumanCharacter::CameraX);//MouseX i CameraX e ba�lad�k.
	PlayerInputComponent->BindAxis("MouseY", this, &AMainHumanCharacter::CameraY);//MouseY yi CameraY ye ba�lad�k.

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainHumanCharacter::Jumping);//Jump action eventini Jumping fonksiyonuna ba�lad�k.
}

void AMainHumanCharacter::ForwardMove(float Value)
{
	if (Controller != NULL && Value != 0.0f)//E�er Controller var ve value 0 de�ilse
	{
		FRotator MihenkYaw = Mihenk->GetRelativeRotation();
		MihenkYaw.Yaw -= 90.0f;
		//Karakterin bakt��� yeri mihenk ile e�itmelemiz gerekiyordu. Ancak e�itleme durumunda karakter 90 derece fazla sa�a kay�yordu.  Bu y�zden <str 58> 
		GetMesh()->SetRelativeRotation(MihenkYaw);

		AddMovementInput(Mihenk->GetRelativeRotation().Vector(), Value * GetWorld()->DeltaTimeSeconds * CharacterForwardMovementSpeed);
	}
}

void AMainHumanCharacter::RightMove(float Value)
{
	if (Controller != NULL && Value != 0.0f)
	{
		FRotator MihenkYaw = Mihenk->GetRelativeRotation();//Mihenk ta��n�n rotasyonunu ald�k.
		MihenkYaw.Yaw -= 90.0f;//Eger Meshin rotasyonunu mihenkle e�itlersek mesh 90 derece fazla sa�a yat�k kal�yor bu y�zden <str 58>

		GetMesh()->SetRelativeRotation(MihenkYaw);//Mesh ile mihenki e�itledik
		AddMovementInput(MihenkYaw.Vector(), Value * GetWorld()->DeltaTimeSeconds * CharacterRightMovementSpeed);//Mesh fazladan saga yat�k oldu�undan  right vector yerine de ge�ebilir.
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