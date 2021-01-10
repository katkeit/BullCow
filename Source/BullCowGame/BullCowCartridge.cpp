// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "FileHelper.h"
#include "Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    OnLoad();
    TitleScreen();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{  
    if(bShowTitle)
        TitleScreen();
    else
    {
        // Difficulty hasn't be set.
        if(!bSetDifficulty)
            SetDifficulty(Input);

        // Ask to show hints.
        else if(!bSetHints)
            ShowHints(Input);
             
        // Difficulty has be set, but game not started.
        else if(!bGameStarted)
            SetupGame();
        
        // Game is setup, so let's play.
        if(bGameStarted)
            PlayGame(Input.ToLower());  // Sets input to lowercase.
    }
}

// Loads the hidden words from WordList.txt and handles the difficulty setting.
void UBullCowCartridge::OnLoad()
{
    // Find the direct pathway to WordList.txt
    const FString& WordListPath = FPaths::ProjectContentDir() / TEXT("WordList.txt");
    // Load the words from WordList.txt into FString array.
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);
}

// Creates the title screen.
void UBullCowCartridge::TitleScreen()
{
    ClearScreen();

    PrintLine(TEXT("Bulls and Cows:"));
    PrintLine(TEXT("The guess an isogram game!\n"));
    PrintLine(TEXT("Press enter to play."));

    bShowTitle = false;
}

void UBullCowCartridge::SetDifficulty(const FString& Difficulty)
{
    // Difficulty settings.
    const int32 EasyMinLength = 3;
    const int32 EasyMaxLength = 5;
    const int32 NormalMinLength = 5;
    const int32 NormalMaxLength = 7;
    const int32 HardMinLength = 7;
    const int32 HardMaxLength = 9;

    ClearScreen();

    PrintLine(TEXT("Select your difficulty!\n"));
    PrintLine(TEXT("1 - EASY"));
    PrintLine(TEXT("2 - NORMAL"));
    PrintLine(TEXT("3 - HARD"));

    if(Difficulty == "1")
    {
        HiddenWords = GetValidWords(WordList, EasyMinLength, EasyMaxLength);
        // Clear the word list.
        WordList.Empty();
        // The difficulty has been set.
        bSetDifficulty = true;

        PrintLine(TEXT("\nPress enter to continue."));
    }  
    
    if(Difficulty == "2")
    {
        HiddenWords = GetValidWords(WordList, NormalMinLength, NormalMaxLength);
        // Clear the word list.
        WordList.Empty();
        // The difficulty has been set.
        bSetDifficulty = true;

        PrintLine(TEXT("\nPress enter to continue."));
    }
        
    if(Difficulty == "3")
    {
        HiddenWords = GetValidWords(WordList, HardMinLength, HardMaxLength);
        // Clear the word list.
        WordList.Empty();
        // The difficulty has been set.
        bSetDifficulty = true;

        PrintLine(TEXT("\nPress enter to continue."));
    }
}

// TODO: Create a function to handle the creation of the hint.
void UBullCowCartridge::ShowHints(const FString& Input)
{
    ClearScreen();

    PrintLine(TEXT("Do you wish to see the Bull letters? (Recommended for beginners)\n"));
    PrintLine(TEXT("DEFINITION:"));
    PrintLine(TEXT("Bull(s) are characters guessed in the correct position for the hidden word.\n"));

    PrintLine(TEXT("1 - YES"));
    PrintLine(TEXT("2 - NO"));

    if(Input == "1")
    {
        bShowHints = true;
        bSetHints = true;

        PrintLine(TEXT("Press enter to continue."));
    }
    
    if(Input == "2")
    {
        bSetHints = true;
        PrintLine(TEXT("\nPress enter to continue."));
    }
        
}

// Shows the player's stats (lives, bulls, cows, etc).
void UBullCowCartridge::ShowStats(const FString& PreviousGuess, FBullCowCount Counter)
{
    ClearScreen();

    PrintLine(TEXT("The isogram is %i characters long."), HiddenWord.Len());
    PrintLine(TEXT("Previous Guess: %s"), *PreviousGuess);

    if(bShowHints)
        PrintLine(TEXT("Word Hints: %s\n"), *Hints);
    else
        PrintLine(TEXT(""));

    PrintLine(TEXT("LIVES: %i"), Lives);
    PrintLine(TEXT("BULLS: %i"), Counter.Bulls);
    PrintLine(TEXT("COWS: %i"), Counter.Cows);
}

// Sets up the game - selects the hidden word, sets lives, and starts game.
void UBullCowCartridge::SetupGame()
{    
    ClearScreen();

    // Get random index between the HiddenWords' number range (min to max).
    int32 Index = FMath::RandRange(0, (HiddenWords.Num() - 1));
    
    HiddenWord = HiddenWords[Index].ToLower(); // HiddenWord is lowercase.
    Lives = HiddenWord.Len();
    bGameStarted = true;

    // Game introduction.
    PrintLine(TEXT("Welcome to Bulls and Cows!"));
    PrintLine(TEXT("The goal is to guess the isogram word."));
    PrintLine(TEXT("[Hint] An isogram is a word without repeating letters.\n"));
    PrintLine(TEXT("Let's play!"));

    PrintLine(TEXT("The isogram is %i characters long."), HiddenWord.Len());
    PrintLine(TEXT("LIVES: %i\n"), Lives);
}

// Resets the game - turns the game off, removes the hidden word, and etc.
void UBullCowCartridge::EndGame(bool HasWon)
{
    ClearScreen();

    if(HasWon)
        PrintLine(TEXT("Congratulations! You discovered the isogram!"));
    else
    {
        PrintLine(TEXT("Sorry, you could not discover the isogram! Try again."));
        PrintLine(TEXT("The isogram: %s"), *HiddenWord);
    }
    
    PrintLine(TEXT("\nPress enter to continue."));

    bShowTitle = true;
    bSetDifficulty = false; // Player might want to select new difficulty.
    bShowHints = false;     // Player might want to not see hints.
    bSetHints = false;
    bGameStarted = false;   // Player is no longer playing.
    HiddenWord = "";        // Clear the hidden word.
    Lives = 0;              // Set lives to default of zero (0).
}

// Handles how the game should play.
void UBullCowCartridge::PlayGame(const FString& PlayersGuess)
{
    // The player's guess is the same length as the hidden word and is an isogram.
    if(PlayersGuess.Len() == HiddenWord.Len() && IsIsogram(PlayersGuess))
    {
        // The player guessed the correct word.
        if(PlayersGuess == HiddenWord)
        {
            EndGame(true);
            return;
        }

        Lives -= 1;

        // Ran out of lives.
        if(Lives == 0)
            EndGame(false);
        else    // Didn't guess the correct word.
        {
            GetHints(PlayersGuess);
            ShowStats(PlayersGuess, GetBullCowCount(PlayersGuess));
        }
    }
}

/*
 Checks if the player's guess is an isogram.
 Note: This function doesn't handle capitalization check.
*/
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 i = 0; i < (Word.Len() - 1); i++)
    {
        for(int32 j = (i + 1); j < Word.Len(); j++)
        {
            if(Word[i] == Word[j])
                return false;
        }
    }

    return true;
}

// Checks an TArray to ensure the words being used meet the length and isogram requirements.
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList, int32 MinLength, int32 MaxLength) const
{
    TArray<FString> ValidWords;

    for(FString Word : WordList)
    {
        if(Word.Len() >= MinLength && Word.Len() <= MaxLength && IsIsogram(Word))
            ValidWords.Emplace(Word);
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCowCount(const FString& PlayersGuess) const
{
    FBullCowCount Counter;

    // Calculate Bulls.
    for(int32 i = 0; i < HiddenWord.Len(); i++)
    {
        if(PlayersGuess[i] == HiddenWord[i])
        {
            Counter.Bulls += 1;
            continue;
        }
            
        // Calculate Cows.
        for(int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if(PlayersGuess[i] == HiddenWord[j])
            {
                Counter.Cows += 1;
                break;
            } 
        }
    }

    return Counter;
}

void UBullCowCartridge::GetHints(const FString& PlayersGuess)
{
    Hints.Empty();

    for(int32 i = 0; i < PlayersGuess.Len(); i++)
    {
        if(PlayersGuess[i] == HiddenWord[i])
            Hints.AppendChar(PlayersGuess[i]);
        else
            Hints.AppendChar(TEXT('-'));
    }
}