// Fill out your copyright notice in the Description page of Project Settings.


#include "Human/AI/Perception/CAIPerceptionComponent.h"

void UCAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
	if(OnExpiredStimulus.IsBound())
	{
		OnExpiredStimulus.Broadcast();
	}
	
}
