// Fill out your copyright notice in the Description page of Project Settings.

#include "NemoPlayerController.h"
#include "Runtime/ActorSequence/Private/ActorSequencePrivatePCH.h"

void ANemoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ActiveContainerIndex = 4;

	// Get camera and set as viewport cam
	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ACameraActor *Camera = *ActorItr;
		if (Camera->Tags.Contains(FName("MainCamera")))
		{
			SetViewTargetWithBlend(Camera);
		}
	}

	// Get containers 
	for (TActorIterator<AContainerCtrl> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AContainerCtrl* Container = *ActorItr;
		if (Container->Tags.Contains(FName("3MM")))
		{
			SimulationContainers[0] = Container;
		}
		else if (Container->Tags.Contains(FName("3MMT")))
		{
			SimulationContainers[1] = Container;
		}
		else if (Container->Tags.Contains(FName("5MM")))
		{
			SimulationContainers[2] = Container;
		}
		else if (Container->Tags.Contains(FName("5MMT")))
		{
			SimulationContainers[3] = Container;
		}
	}
}

void ANemoPlayerController::ResetContainer() {}

void ANemoPlayerController::ToggleEmitter()
{
	if (ActiveContainerIndex == 4)
	{
		return;
	}

	if (SimulationContainers[ActiveContainerIndex] == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SimulationContainer %i not set"), ActiveContainerIndex)
		return;
	}

	SimulationContainers[ActiveContainerIndex]->ToggleEmitter();
}

void ANemoPlayerController::NextContainer()
{
	ActiveContainerIndex++;

	if (ActiveContainerIndex == 5)
	{
		ActiveContainerIndex = 0;
	}

	SwitchCamera();
}


void ANemoPlayerController::PreviousContainer()
{
	ActiveContainerIndex--;

	if (ActiveContainerIndex == -1)
	{
		ActiveContainerIndex = 4;
	}

	SwitchCamera();
}


void ANemoPlayerController::SwitchMaterial()
{
	
}

void ANemoPlayerController::SwitchCamera()
{
	if (ActiveContainerIndex == 4)
	{
		// Get camera and set as viewport cam
		for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			ACameraActor *Camera = *ActorItr;
			if (Camera->Tags.Contains(FName("MainCamera")))
			{
				SetViewTargetWithBlend(Camera);
			}
		}
	}
	else
	{
		SetViewTargetWithBlend(SimulationContainers[ActiveContainerIndex]);
	}
}

void ANemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Workaround for a bug that happened if you open the player controller blueprint  https://issues.unrealengine.com/issue/UE-36929
	if (GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		return;
	}

	InputComponent->BindAction("Reset", IE_Released, this, &ANemoPlayerController::ResetContainer);
	InputComponent->BindAction("Toggle", IE_Released, this, &ANemoPlayerController::ToggleEmitter);
	InputComponent->BindAction("Left", IE_Released, this, &ANemoPlayerController::PreviousContainer);
	InputComponent->BindAction("Right", IE_Released, this, &ANemoPlayerController::NextContainer);
	InputComponent->BindAction("Material", IE_Released, this, &ANemoPlayerController::SwitchMaterial);
}
