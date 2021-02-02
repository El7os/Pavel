// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainHumanCharacter.generated.h"

UCLASS()
class PAVEL_API AMainHumanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AMainHumanCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	//karakterin ileri geri gitmesini saglar
	void ForwardMove(float Value);

	//Karakterin sag-sol gitmesini saglar.
	void RightMove(float Value);

	//Karakterin ziplamasini saglar.
	void Jumping();

	//Faredeki x deðerinin kamera üzerindeki kontrolünü saðlar.
	void CameraX(float Value);
	
	//Faredeki y deðerinin kamera üzerindeki kontrolünü saðlar.
	void CameraY(float Value);

	//Kameranýn baðlangýç pitch deðeri;
	float InitialCamearPicthRotation;

	UPROPERTY(EditAnywhere, Category = "Movement")//Karakterin ileri-geri giderkenki hizi
	float CharacterForwardMovementSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")//Karakterin sag-sol giderkenki hizi
	float CharacterRightMovementSpeed = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")//Karakterin ziplama yukseligi
	float CharacterJumpVelocity = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Control")//Karakterin kamerasinin x eksenindeki hizi
	float CameraMouseXSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Camera Control")//Karakterin kamerasinin y eksenindeki hizi
	float CameraMouseYSpeed = 100.0f;


	UPROPERTY(EditAnywhere, Category = "Camera Control")//Bu deger kameranin baslangic konumun uzerine eklenir.
	float MaxCameraPitch = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Control")//Bu deger kameranin baslangic degerinden cikarilir.
	float MinCameraPitch = 50.0f;


	UPROPERTY(VisibleAnywhere)//SpringArm Componenti tanimladik
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere)//Camera tanimladik.
	class UCameraComponent* Camera = nullptr;

	//Mihenk taþýný tanýmladýk.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mihenk = nullptr;

private:




};
