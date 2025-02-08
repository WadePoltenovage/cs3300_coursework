// Team: Soo Yuan Kong, Alex Ringelheim
// Project Name: Uno
// Project Description: 2-4 Players can play a game of standard uno against each other. The hands with each player's cards are in text files contained
// in this code's project file (created after user(s) have selected the amount of players there will be). There is an instruction manual txt file, please read it. 
// Input: Y/N to start new game or quit, amount of players, which cards you would like to play, color change based on if you play a wild card
// Output: A complete game of Uno
// 
// THERE IS AN INSTRUCTION MANUAL, PLEASE READ BEFORE PLAYING (text file was turned in too)
// 
// Change log:
//   Date          Programmer                           Description
//  ----------------------------------------------------------------------------------------------------
// 25 Oct 23     Both                               Created project and synced
// 29 Oct 23     Soo Yuan Kong              Beginning of the game (display menu & game recursion) & exception handling (option, numOfPlayers)
// 29 Oct 23     Alex Ringelheim            Created the class for Uno as well as the deck and operator overloading.
//                                          Every card is randomized and put into the deck (vector)
// 2 Nov 23      Alex Ringelheim            Player hands are now able to be distributed with 7 cards each. Each player has a text file
//                                          with their cards on it
// 5 Nov 23      Both                       Worked on game rotation, REVERSE & SKIP card pseudocode
// 15 Nov 23     Both                       Worked on Wild/Wild+4 and +2 pseudocode
// 16 Nov 23     Alex                       Created function to play and draw cards. This edits the text files properly (hopefully) with an updated hand that 
//                                          draws from the deck or places a card in the pile (playCard function).
// 19 Nov 23     Both                       Added to the playCard function: Reverse, Skip, +2, Wild, WIld+4. Also added a rotate funciton. Debugging.
// 21 Nov 23     Both                       Finished up winnerDisplay Function and made several adjustments to code based on results of debugging. However, features still are not working
// 24 Nov 23     Both                       Fixed features that were not working and debugged. Including the: skip card, reverse, and any other small things. Also put finishing touches
//                                          on the code. 


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <iomanip>

// Class for all objects and functions needed to make the game

class Uno
{
private:
    int deckCount{};    // Total cards in the deck (default is 108 in standard Uno)
    int pileCount{};    // Total cards in the center pile
    int playerCount{};
    int playerIndex = 0;
    bool direction = 0;
    bool isSkip = 0;
    std::string pileNumber = "7";   // Making the first card in the center pile always starts with 7        // First card will always be Blue-7
    std::string pileColor = "Blue"; // Making the first card in the center pile always starts with Blue
    std::string cardChoice{};
    std::string color{};


public:

    Uno()
    { // Defualt Constructor
        deckCount = 108;
        pileCount = 0;
    }

    Uno(int dTotal, int pTotal)
    {   // Constructor needed to overload ++ and -- operators)
        dTotal = 108;
        pTotal = 0;
    }

    int playerCardCounts[4] = { 7, 7, 7, 7 };   // All player card counts

    std::string numberedSet[15];    // Total amount of possible numbers for cards and special cards included
    std::string colorSet[4];    // Total color types (ex. blue)
    std::vector<std::string> deck;  // Vector to hold the deck (is randomized)
    std::vector <std::string> playerNames;

    // Overloads the prefix -- operator to subtract from deck count and check if the deck is empty. 

    Uno operator --()
    {
        // Checks if deck is empty whenever it is subtracted from

        --deckCount;
        if (deckCount == 0)
        {
            std::cout << "The deck is empty, reshuffling a new dack!" << std::endl;
        }

        return Uno(deckCount, pileCount);
    }

    // Each function is explained above their definition. Each is used in tandem with each other to make the game work

    void makeDeck();
    void makeHands(int playerCount);
    void playCard(int playerIndex, int playerCount);
    void rotate(int playerCount, int& playerIndex, bool isSkip);
    void winnerDisplay(int playerCount, int playerCardCounts[]);
    void playGame(int playerCount, int playerIndex, int playerCardCounts[]);
    void chooseWildColor();
    void handleSpecialCards(const std::string& card, int playerIndex, int playerCount, int direction);
    void updateDirection(int& playerIndex);
    int skipPlayer(int& playerIndex, int playerCount, int& direction, bool& isSkip);
    void drawCards(int numCards, int playerIndex, int playerCount);
};

// Creates a completely randomized deck and stores said deck in a vector. 

void Uno::makeDeck()
{

    // Holds the combination potential for the cards

    std::string numberedSet[15] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "+2", "Reverse", "Skip", "Wild-Card", "Wild-+4" };
    std::string colorSet[4] = { "Red-", "Yellow-", "Blue-", "Green-" };

    srand(time(0)); // Creates a seed for the randomization (initialization)

    int numberAmount = 15;
    int colorAmount = 4;
    int numberChoice = 0;
    int colorChoice = 0;
    std::string newCard;

    // 108 cards are needed for an Uno Deck, so a random number (or special card) is chosen as well as a color)

    for (int i = 0; i < 108; i++)
    {
        numberChoice = rand() % numberAmount;
        colorChoice = rand() % colorAmount;

        // Wild cards do not have a color so this prevents for example, Blue Wild, which does not exist

        if (numberedSet[numberChoice] == "Wild-Card" || numberedSet[numberChoice] == "Wild-+4")
        {
            newCard = numberedSet[numberChoice];
        }

        // Otherwise, the color and number are concated into one string 

        else
        {
            newCard = colorSet[colorChoice] + numberedSet[numberChoice];
        }

        // Said one string is then put into the vector 

        deck.push_back(newCard);
    }
}

// Making hands for 2/3/4 players case and creating text files as their hand
// Each file for each player has 7 random cards

void Uno::makeHands(int numOfPlayers) 
{

    // Each case is the same, just changes the file name and which vectors are added to

    switch (numOfPlayers)
    {
    case 2:
    {
        std::ofstream hands;    // Opens file
        hands.open("Player1.txt");  // Names file
        std::vector<std::string> hand1; // Two vectors for both hands
        std::vector<std::string> hand2;

        // Adds seven cards to each hand

        for (int i = 0; i < 7; i++)
        {
            hand1.push_back(deck[i]);
            hands << hand1[i] << "\n";
        }
        hands.close();  // Closes file
    }
    {
        std::ofstream hands;
        hands.open("Player2.txt");
        std::vector<std::string> hand2;
        for (int i = 0; i < 7; i++)
        {
            hand2.push_back(deck[7 + i]);
            hands << hand2[i] << "\n";

        }
        hands.close();
    }
    std::cout << "Both players may begin their match!" << std::endl;
    break;
    case 3:
    {
        std::ofstream hands;
        hands.open("Player1.txt");
        std::vector<std::string> hand1;
        std::vector<std::string> hand2;
        for (int i = 0; i < 7; i++)
        {
            hand1.push_back(deck[i]);
            hands << hand1[i] << "\n";
        }
        hands.close();
    }
    {
        std::ofstream hands;
        hands.open("Player2.txt");
        std::vector<std::string> hand2;
        for (int i = 0; i < 7; i++)
        {
            hand2.push_back(deck[7 + i]);
            hands << hand2[i] << "\n";

        }
        hands.close();
    }
    {
        std::ofstream hands;
        hands.open("Player3.txt");
        std::vector<std::string> hand3;
        for (int i = 0; i < 7; i++)
        {
            hand3.push_back(deck[14 + i]);
            hands << hand3[i] << "\n";

        }
        hands.close();
    }
    std::cout << "All 3 players may begin their match!" << std::endl;
    break;
    case 4:
    {
        std::ofstream hands;
        hands.open("Player1.txt");
        std::vector<std::string> hand1;
        std::vector<std::string> hand2;
        for (int i = 0; i < 7; i++)
        {
            hand1.push_back(deck[i]);
            hands << hand1[i] << "\n";
        }
        hands.close();
    }
    {
        std::ofstream hands;
        hands.open("Player2.txt");
        std::vector<std::string> hand2;
        for (int i = 0; i < 7; i++)
        {
            hand2.push_back(deck[7 + i]);
            hands << hand2[i] << "\n";

        }
        hands.close();
    }
    {
        std::ofstream hands;
        hands.open("Player3.txt");
        std::vector<std::string> hand3;
        for (int i = 0; i < 7; i++)
        {
            hand3.push_back(deck[14 + i]);
            hands << hand3[i] << "\n";

        }
        hands.close();
    }
    {
        std::ofstream hands;
        hands.open("Player4.txt");
        std::vector<std::string> hand4;
        for (int i = 0; i < 7; i++)
        {
            hand4.push_back(deck[21 + i]);
            hands << hand4[i] << "\n";

        }
        hands.close();
    }
    std::cout << "All 4 players may begin their match!" << std::endl;
    break;
    default:

        // Defualt case if there was an incorrect amount of players

        std::cout << "It seems there is an incorrect amount of players... Please try again" << std::endl;
        break;
    }
}

void Uno::playCard(int playerIndex, int playerCount)
{

    std::string cardColor{};
    std::string cardNumber{};
    bool isCardValid = false;
    int playCount = 0;

    // Gets player card they would like to play and splits the variable into two parts to be checked

    std::cout << "Which card would you like to play from your hand, Player " << playerIndex + 1 << "?: ";
    std::getline(std::cin >> std::ws, cardColor, '-'); // Consume whitespace, including newline
    std::cin >> cardNumber;

    // Gets rid of extra whitespaces

    cardNumber.erase(std::remove(cardNumber.begin(), cardNumber.end(), ' '), cardNumber.end());
    pileNumber.erase(std::remove(pileNumber.begin(), pileNumber.end(), ' '), pileNumber.end());
    cardColor.erase(std::remove(cardColor.begin(), cardColor.end(), ' '), cardColor.end());
    pileColor.erase(std::remove(pileColor.begin(), pileColor.end(), ' '), pileColor.end());

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Gets rid of newline characters

    // Informing players the current pile card

	std::cout << "Here is the current pile card: " << pileColor + "-" + pileNumber << std::endl;

    // Each case is the same again, just with different player hands

    switch (playerIndex)
    {
    case 0:
    {
        // Draw a card to current player's hand (text file) if the player types "Draw-Card"
        if (cardColor + "-" + cardNumber == "Draw-Card")
        {
            std::ofstream hands("Player1.txt", std::ios::app);
            if (deckCount > 0)
            {
                hands << deck[--deckCount] << "\n"; // Puts card from top of deck into current player hand
                hands.close();
                break;
            }
            // Outputs error message if the deck is empty and not available to draw cards
            else
            {
                std::cout << "Cannot draw a card. The deck is empty." << std::endl;
                hands.close();
                break;
            }
        }
        std::ifstream hands("Player1.txt");
        std::ofstream newFile("temp.txt");  // Replaces old file with updated file

        // Makes sure files opened properly

        if (!hands.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }

        if (!newFile.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }

        // Search phrase
        std::string searchPhrase = cardColor + "-" + cardNumber;

        // Variable to store the found information
        std::string foundCard;

        // Reset the file position to the beginning
        hands.clear();  // Clear any flags that might indicate an error
        hands.seekg(0, std::ios::beg);

        // Read the file line by line
        std::string line;
        while (std::getline(hands, line))
        {
            // Check if the line contains the search phrase
            if (line.find(searchPhrase) != std::string::npos)
            { 
                if (playCount == 0)
                {
                    foundCard = line;
                    isCardValid = true;
                    std::cout << "Card Found" << std::endl;
                }

                continue;  // Stop searching after the first match
            }

            newFile << line << std::endl;   // Updates temp file
        }

        std::cout << "Card in play: " << cardColor + "-" + cardNumber << std::endl;

        // Check for Wild and Wild+4 special cards as it will change the pile color

        if (cardColor + "-" + cardNumber == "Wild-Card" || cardColor + "-" + cardNumber == "Wild-+4")
        {
            handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);  // handles all special cards, function below, this one is used to call wild and wild+4 code
            playerCardCounts[playerIndex]--;    // subtracts from current player's card count
            hands.close();  // Closes the files
            newFile.close();
            std::remove("Player1.txt"); // Replaces old file with updated file
            std::rename("temp.txt", "Player1.txt");

            break;
        }

        // Checks for regular cards and updates the pile with the new card information

        if (cardNumber == pileNumber || cardColor == pileColor)
        {
            if (isCardValid == true)
            {
                pileNumber = cardNumber;
                pileColor = cardColor;

                handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);

                playerCardCounts[playerIndex]--;
                std::cout << "Card Played!" << std::endl;
				std::cout << "Here is the current pile card: " << pileColor + "-" + pileNumber << std::endl;
            }
        }

        // Check if the player plays invalid card

        else
        {
            std::cout << "You don't seem to have this card or it does not match the pile, please try again: " << std::endl;
            playCard(playerIndex, playerCount);
        }

        // Close the file
        hands.close();
        newFile.close();

        // Makes sure card is valid and replaces old file with updated file

        if (isCardValid == true)
        {
            std::remove("Player1.txt");
            std::rename("temp.txt", "Player1.txt");
        }

        break;
    }
    case 1:
    {
        if (cardColor + "-" + cardNumber == "Draw-Card")
        {
            std::ofstream hands("Player2.txt", std::ios::app);
            if (deckCount > 0)
            {
                hands << deck[--deckCount] << "\n";
                hands.close();
                break;
            }
            else
            {
                std::cout << "Cannot draw a card. The deck is empty." << std::endl;
                hands.close();
                break;
            }
        }
        std::ifstream hands("Player2.txt");
        std::ofstream newFile("temp.txt");

        if (!hands.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }

        if (!newFile.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }


        // Search phrase
        std::string searchPhrase = cardColor + "-" + cardNumber;

        // Variable to store the found information
        std::string foundCard;

        // Reset the file position to the beginning
        hands.clear();  // Clear any flags that might indicate an error
        hands.seekg(0, std::ios::beg);

        // Read the file line by line
        std::string line;
        while (std::getline(hands, line))
        {
            // Check if the line contains the search phrase
            if (line.find(searchPhrase) != std::string::npos)
            {
                if (playCount == 0)
                {
                    foundCard = line;
                    isCardValid = true;
                    std::cout << "Card Found" << std::endl;
                }

                continue;  // Stop searching after the first match
            }

            newFile << line << std::endl;

        }

        std::cout << "Card in play: " << cardColor + "-" + cardNumber << std::endl;

        if (cardColor + "-" + cardNumber == "Wild-Card" || cardColor + "-" + cardNumber == "Wild-+4")
        {
            handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);
            playerCardCounts[playerIndex]--;
            hands.close();
            newFile.close();
            std::remove("Player2.txt");
            std::rename("temp.txt", "Player2.txt");

            break;
        }

        if (cardNumber == pileNumber || cardColor == pileColor)
        {
            if (isCardValid == true)
            {
                pileNumber = cardNumber;
                pileColor = cardColor;

                handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);

                playerCardCounts[playerIndex]--;
                std::cout << "Card Played!" << std::endl;
				std::cout << "Here is the current pile card: " << pileColor + "-" + pileNumber << std::endl;
            }
        }

        else
        {
            std::cout << "You don't seem to have this card or it does not match the pile, please try again: " << std::endl;
            playCard(playerIndex, playerCount);
        }

        // Close the file
        hands.close();
        newFile.close();
        if (isCardValid == true)
        {
            std::remove("Player2.txt");
            std::rename("temp.txt", "Player2.txt");
        }

        break;
    }

    case 2:
    {
        if (cardColor + "-" + cardNumber == "Draw-Card")
        {
            std::ofstream hands("Player3.txt", std::ios::app);
            if (deckCount > 0)
            {
                hands << deck[--deckCount] << "\n";
                hands.close();
                break;
            }
            else
            {
                std::cout << "Cannot draw a card. The deck is empty." << std::endl;
                hands.close();
                break;
            }
        }
        std::ifstream hands("Player3.txt");
        std::ofstream newFile("temp.txt");

        if (!hands.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }

        if (!newFile.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }


        // Search phrase
        std::string searchPhrase = cardColor + "-" + cardNumber;

        // Variable to store the found information
        std::string foundCard;

        // Reset the file position to the beginning
        hands.clear();  // Clear any flags that might indicate an error
        hands.seekg(0, std::ios::beg);

        // Read the file line by line
        std::string line;
        while (std::getline(hands, line))
        {
            // Check if the line contains the search phrase
            if (line.find(searchPhrase) != std::string::npos)
            {
                if (playCount == 0)
                {
                    foundCard = line;
                    isCardValid = true;
                    std::cout << "Card Found" << std::endl;
                }

                continue;  // Stop searching after the first match
            }

            newFile << line << std::endl;

        }

        std::cout << "Card in play: " << cardColor + "-" + cardNumber << std::endl;

        if (cardColor + "-" + cardNumber == "Wild-Card" || cardColor + "-" + cardNumber == "Wild-+4")
        {
            handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);
            playerCardCounts[playerIndex]--;
            hands.close();
            newFile.close();
            std::remove("Player3.txt");
            std::rename("temp.txt", "Player3.txt");

            break;
        }

        if (cardNumber == pileNumber || cardColor == pileColor)
        {
            if (isCardValid == true)
            {
                pileNumber = cardNumber;
                pileColor = cardColor;

                handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);

                playerCardCounts[playerIndex]--;
                std::cout << "Card Played!" << std::endl;
				std::cout << "Here is the current pile card: " << pileColor + "-" + pileNumber << std::endl;
            }
        }

        else
        {
            std::cout << "You don't seem to have this card or it does not match the pile, please try again: " << std::endl;
            playCard(playerIndex, playerCount);
        }

        // Close the file
        hands.close();
        newFile.close();
        if (isCardValid == true)
        {
            std::remove("Player3.txt");
            std::rename("temp.txt", "Player3.txt");
        }

        break;
    }

    case 3:
    {
        if (cardColor + "-" + cardNumber == "Draw-Card")
        {
            std::ofstream hands("Player4.txt", std::ios::app);
            if (deckCount > 0)
            {
                hands << deck[--deckCount] << "\n";
                hands.close();
                break;
            }
            else
            {
                std::cout << "Cannot draw a card. The deck is empty." << std::endl;
                hands.close();
                break;
            }
        }
        std::ifstream hands("Player4.txt");
        std::ofstream newFile("temp.txt");

        if (!hands.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }

        if (!newFile.is_open())
        {
            std::cout << "Error opening file!" << std::endl;
            return;
        }


        // Search phrase
        std::string searchPhrase = cardColor + "-" + cardNumber;

        // Variable to store the found information
        std::string foundCard;

        // Reset the file position to the beginning
        hands.clear();  // Clear any flags that might indicate an error
        hands.seekg(0, std::ios::beg);

        // Read the file line by line
        std::string line;
        while (std::getline(hands, line))
        {
            // Check if the line contains the search phrase
            if (line.find(searchPhrase) != std::string::npos)
            {
                if (playCount == 0)
                {
                    foundCard = line;
                    isCardValid = true;
                    std::cout << "Card Found" << std::endl;
                }

                continue;  // Stop searching after the first match
            }

            newFile << line << std::endl;

        }

        std::cout << "Card in play: " << cardColor + "-" + cardNumber << std::endl;

        if (cardColor + "-" + cardNumber == "Wild-Card" || cardColor + "-" + cardNumber == "Wild-+4")
        {
            handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);
            playerCardCounts[playerIndex]--;
            hands.close();
            newFile.close();
            std::remove("Player4.txt");
            std::rename("temp.txt", "Player4.txt");

            break;
        }

        if (cardNumber == pileNumber || cardColor == pileColor)
        {
            if (isCardValid == true)
            {
                pileNumber = cardNumber;
                pileColor = cardColor;

                handleSpecialCards(cardColor + "-" + cardNumber, playerIndex, playerCount, direction);

                playerCardCounts[playerIndex]--;
                std::cout << "Card Played!" << std::endl;
				std::cout << "Here is the current pile card: " << pileColor + "-" + pileNumber << std::endl;
            }
        }

        else
        {
            std::cout << "You don't seem to have this card or it does not match the pile, please try again: " << std::endl;
            playCard(playerIndex, playerCount);
        }

        // Close the file
        hands.close();
        newFile.close();
        if (isCardValid == true)
        {
            std::remove("Player4.txt");
            std::rename("temp.txt", "Player4.txt");
        }

        break;
    }
    }
}

// Func. for Wild and Wild+4 special cards to change the pile color

void Uno::chooseWildColor()
{
    std::cout << "Choose a color (Red/Blue/Green/Yellow): ";
    std::cin >> pileColor;
    std::cout << "Here is the new pile color: " << pileColor << std::endl;
}

// Func. for all special cards (Reverse/Skip/+2/Wild/Wild+4)

void Uno::handleSpecialCards(const std::string& card, int playerIndex, int playerCount, int direction)
{
    if (card == "Blue-Reverse" || card == "Yellow-Reverse" || card == "Red-Reverse" || card == "Green-Reverse")
    {
        updateDirection(playerIndex);
    }
    else if (card == "Blue-Skip" || card == "Yellow-Skip" || card == "Red-Skip" || card == "Green-Skip")
    {
        skipPlayer(playerIndex, playerCount, direction, isSkip);
    }
    else if (card == "Blue-+2" || card == "Yellow-+2" || card == "Red-+2" || card == "Green-+2")
    {
        drawCards(2, playerIndex, playerCount);
    }
    else if (card == "Wild-Card")
    {
        chooseWildColor();
    }

    else if (card == "Wild-+4")
    {
        drawCards(4, playerIndex, playerCount);
        chooseWildColor();
    }
}

// Func. for Reverse card to switch direction of the game

void Uno::updateDirection(int& playerIndex)
{
    if (direction == 0) // clockwise direction
    {
        direction = 1;
    }

    else if (direction == 1) // counterclockwise direction
    {
        direction = 0;
    }
}

// Func. for Skip card to skip players in clockwise/counterclockwise direction

int Uno::skipPlayer(int& playerIndex, int playerCount, int& direction, bool& isSkip)
{
    isSkip = 1;
    if (direction == 0)
    {
        playerIndex = (playerIndex + 2) % playerCount;
    }
    else if (direction == 1)
    {
        playerIndex = (playerIndex - 2 + playerCount) % playerCount;
    }

    return playerIndex;
}

// Draws cards of any amount

void Uno::drawCards(int numCards, int playerIndex, int playerCount)
{
    // Amount of cards to draw

    for (int i = 0; i < numCards; i++)
    {
        int nextPlayerIndex = (playerIndex + 1) % playerCount;  // The next player recieves the cards
        std::ofstream hands("Player" + std::to_string(nextPlayerIndex + 1) + ".txt", std::ios::app);

        // Subtracts from deck count

        if (deckCount > 0)
        {
            hands << deck[--deckCount] << "\n";
            hands.close();
        }
        else
        {
            std::cout << "Cannot draw a card. The deck is empty." << std::endl;
            hands.close();
            break;
        }
    }
}

// Func. that moves to next player based on direction (clockwise/counterclockwise). This func. also works with Skip card to properly change playerIndex

void Uno::rotate(int playerCount, int& playerIndex, bool isSkip)
{
    if (direction == 0)
    {
        if (isSkip == 1)
        {
            playerIndex = (playerIndex + 2) % playerCount;  // Gets correct player index based on how many players there are
            isSkip = 0;
        }

        else if (playerIndex < playerCount - 1)
        {
            playerIndex++;
        }
        else
        {
            playerIndex = 0;
        }
    }

    if (direction == 1)
    {
        if (isSkip == 1)
        {
            playerIndex = (playerIndex - 2) % playerCount;
            isSkip = 0;
        }

        else if (playerIndex == 0)
        {
            playerIndex = playerCount - 1;
        }

        else if (playerIndex < playerCount && playerIndex != 0)
        {
            playerIndex--;
        }
    }
}

// Displays the winner using bubble sort. It sorts the winner from lowest to highest card counts when one player finishes playing all the cards.

void Uno::winnerDisplay(int playerCount, int playerCardCounts[])
{
    for (int i = 0; i < playerCount; i++)
    {
        for (int j = i + 1; j < playerCount; j++)
        {
            //comparing votes row by row
            if (playerCardCounts[i] > playerCardCounts[j])
            {
                int temp = playerCardCounts[i]; //Create temp as a token for # of counts
                playerCardCounts[i] = playerCardCounts[j];
                playerCardCounts[j] = temp;
                std::string tempp = playerNames[i]; //Create tempp as a token for names
                playerNames[i] = playerNames[j];
                playerNames[j] = tempp;
            }
        }
		//output results
		std::cout << std::setw(10) << playerNames[i] << std::setw(12) << playerCardCounts[i] << std::endl;
    }
    // output winner
    std::cout << playerNames[0] << " wins!" << std::endl;
}

// Func. that puts everything together

void Uno::playGame(int playerCount, int playerIndex, int playerCardCounts[])
{
    std::string name{}; // Holds player names

    // Gets each player's desired name

    for (int i = 0; i < playerCount; i++)
    {
        std::cout << "Please enter name for Player " << i + 1 << ": ";
        std::cin >> name;
        playerNames.push_back(name);
    }

    bool winner = false;
    bool gameStart = false;

    // If there is no winner the game continues

    while (winner == false)
    {
        // If this is the first run, it creates a deck and hands

        if (gameStart == false)
        {
            makeDeck();
            makeHands(playerCount);
            gameStart = true;
        }

        if (deckCount == 0)
        {
            makeDeck();
        }

        // Otherwise the game loops until there is a winner

        else
        {
            playCard(playerIndex, playerCount);
            rotate(playerCount, playerIndex, isSkip);
        }

        // Once a player has no more cards, they win

        if (playerCardCounts[playerIndex] == 0)
        {
            winner = true;
            winnerDisplay(playerCount, playerCardCounts);
            break;
            
        }
    }
}

int main()
{
    Uno unoGame;    // Class object

    // Loop keeps looping until user enters 2 to exit

    while (true)
    {
        int option;
        int numOfPlayers;
        // Outputing game menu
        std::cout << "Welcome to UNO!" << std::endl;
        std::cout << "\t1. New Game" << std::endl;
        std::cout << "\t2. Quit" << std::endl;
        std::cout << "Please select an option:" << std::endl;
        std::cin >> option;

        // Error handling for user inputs during menu

        try
        {
            if (option != 1 && option != 2)
            {
                throw std::exception();
            }
        }

        catch (std::exception& e)
        {
            std::cout << "Invalid menu choice." << "\nPlease re-enter a valid menu option: ";
            std::cin >> option;
        }

        if (option == 1)
        {
            std::cout << "Please enter number of players (2-4):";
            std::cin >> numOfPlayers;
            
            try
            {
                if (numOfPlayers < 2 || numOfPlayers > 5) // as it is a 2-4 players game
                {
                    throw std::exception();
                }
            }

            catch (std::exception& e)
            {
                std::cout << "Invalid player number." << "\nPlease re-enter a valid player number: ";
                std::cin >> numOfPlayers;
            }

            unoGame.playGame(numOfPlayers, 0, unoGame.playerCardCounts);    // Plays the game
        }
        else if (option == 2)
        {
            exit(0);    // exits game if user does not want to play again
        }
    }
}