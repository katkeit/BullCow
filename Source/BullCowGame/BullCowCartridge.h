// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;	// Number of letters that match and are in the correct spot.
	int32 Cows = 0;		// Number of letters that match, but in the wrong spot.
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
	TArray<FString> WordList;
	TArray<FString> HiddenWords;
	bool bShowTitle = false;
	bool bGameStarted = false;
	bool bSetDifficulty = false;
	bool bSetHints = false;
	bool bShowHints = false;
	FString HiddenWord; // The isogram the player has to guess.
	// TArray<char> Hints;
	FString Hints;
	int32 Lives; // Number of lives a player gets.

	void OnLoad();
	void TitleScreen();
	void SetDifficulty(const FString& Difficulty);
	void ShowHints(const FString& Input);
	void ShowStats(const FString& PreviousGuess, FBullCowCount Counter);
	void SetupGame();
	void EndGame(bool HasWon);
	void PlayGame(const FString& PlayersGuess);
	bool IsIsogram(const FString& Word) const;
	TArray<FString> GetValidWords(const TArray<FString>& Words, int32 MinLength, int32 MaxLength) const;
	FBullCowCount GetBullCowCount(const FString& PlayersGuess) const;
	void GetHints(const FString& PlayersGuess);
};
