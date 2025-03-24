# PokerLib - C++ Library

A powerful, high-performance poker hand evaluation and equity calculation library for C++. This is the C++ implementation of the PokerLib project, which is also available as a Node.js library through native bindings.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Features

- **Blazing Fast Performance**: Optimized C++ algorithms for poker hand evaluation
- **Comprehensive Hand Evaluation**: Evaluates poker hands quickly and accurately
- **Equity Calculation**: Calculate win/tie probabilities for multiple hands
- **Winning Outs Analysis**: Identify cards that improve a player's hand
- **Hand Description**: Human-readable descriptions of poker hands
- **Clean API**: Intuitive interface with modern C++ design

## Installation

### Using node-gyp

```bash
# Clone the repository
git clone https://github.com/jensroth-git/pokerlib-cpp.git
cd pokerlib-cpp

# Install dependencies
npm install

# Build the native module
npm run build
```

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2019+)
- Node.js 12+ and npm
- node-gyp (will be installed with npm dependencies)

## Quick Start

```cpp
#include "pokerlib/PokerLib.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    // Parse cards from string notation
    auto holecards = pokerlib::Card::GetCards("AhKh");
    auto board = pokerlib::Card::GetCards("8s9cTd");
    
    // Combine cards for evaluation
    auto all_cards = pokerlib::Card::CombineCards(holecards, board);
    
    // Evaluate the hand
    uint16_t eval = pokerlib::GetHandEvaluation(all_cards);
    
    // Get a human-readable description
    auto desc = pokerlib::HandDescriptionProvider::GetDescription(eval);
    std::cout << "Hand: " << desc.to_string() << std::endl;
    
    // Calculate equity between multiple hands
    std::vector<std::string> hands = {"AhKh", "QsQc"};
    auto results = pokerlib::Evaluate(hands, "8s9cTd");
    
    // Display equity for each hand
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << hands[i] << ": " 
                  << (results[i].equityPercentage * 100.0) << "% equity" 
                  << std::endl;
    }
    
    return 0;
}
```

## Card Notation

Cards are represented as strings with a rank (2-9, T, J, Q, K, A) followed by a suit (h, d, c, s):

- `Ah` - Ace of hearts
- `Kd` - King of diamonds 
- `Qc` - Queen of clubs
- `Js` - Jack of spades
- `Th` - Ten of hearts

Multiple cards can be combined in a single string:
- `AhKs` - Ace of hearts and King of spades
- `QcJdTh` - Queen of clubs, Jack of diamonds, and Ten of hearts

## API Reference

### Card Handling

#### `Card::GetCards(std::string str)`

Parses a string of cards into a vector of Card objects.

```cpp
auto cards = pokerlib::Card::GetCards("AhKs");
// Returns vector of Card objects
```

#### `Card::CombineCards(std::vector<Card> cards)`

Combines multiple cards into a hand object for evaluation.

```cpp
auto hand = pokerlib::Card::CombineCards(cards);
```

#### `Card::CombineCards(const std::vector<Card>& firstVector, const Vectors&... restVectors)`

Variadic template to combine multiple card vectors efficiently.

```cpp
auto hand = pokerlib::Card::CombineCards(holecards, board);
```

### Hand Evaluation

#### `GetHandEvaluation(omp::Hand hand)`

Evaluates a poker hand and returns its numerical value.

```cpp
uint16_t eval = pokerlib::GetHandEvaluation(hand);
```

#### `HandDescriptionProvider::GetDescription(uint16_t eval)`

Gets a human-readable description of a hand evaluation.

```cpp
auto desc = pokerlib::HandDescriptionProvider::GetDescription(eval);
std::cout << desc.to_string() << std::endl;
```

#### `GetFiveBestCards(std::vector<Card> cards)`

Finds the best 5-card hand from a set of cards.

```cpp
auto bestFive = pokerlib::GetFiveBestCards(allCards);
```

### Equity Calculation

#### `Evaluate(std::vector<std::string> hands, std::string boardStr = "", std::string deadStr = "")`

Calculates equity for multiple hands with optional board and dead cards.

```cpp
auto results = pokerlib::Evaluate({"AhKh", "QsQc"}, "8s9cTd");
```

### Deck Management

#### `Deck()`

Creates a new deck of cards.

```cpp
pokerlib::Deck deck;
```

#### `Deck::Shuffle()`

Shuffles the deck.

```cpp
deck.Shuffle();
```

#### `Deck::Deal()`

Deals a card from the deck.

```cpp
pokerlib::Card card = deck.Deal();
```

#### `Deck::RemoveCards(const std::vector<Card>& remove)`

Removes specified cards from the deck.

```cpp
deck.RemoveCards(deadCards);
```

## Examples

### Complete Hand Evaluator Example

```cpp
#include "pokerlib/PokerLib.h"
#include <iostream>
#include <string>

int main() {
    std::string hand_str = "AhKh";
    std::string board_str = "QhJhTh";
    
    // Parse the cards
    auto hand_cards = pokerlib::Card::GetCards(hand_str);
    auto board_cards = pokerlib::Card::GetCards(board_str);
    
    // Combine all cards using the variadic template method
    auto combined_hand = pokerlib::Card::CombineCards(hand_cards, board_cards);
    
    // Evaluate the hand
    uint16_t eval = pokerlib::GetHandEvaluation(combined_hand);
    
    // Get the hand description
    auto desc = pokerlib::HandDescriptionProvider::GetDescription(eval);
    
    std::cout << "Hand: " << hand_str << std::endl;
    std::cout << "Board: " << board_str << std::endl;
    std::cout << "Evaluation: " << eval << std::endl;
    std::cout << "Category: " << static_cast<int>(desc.GetCategory()) << std::endl;
    std::cout << "Description: " << desc.to_string() << std::endl;
    
    // Find the best 5 cards
    std::vector<pokerlib::Card> all_cards;
    all_cards.insert(all_cards.end(), hand_cards.begin(), hand_cards.end());
    all_cards.insert(all_cards.end(), board_cards.begin(), board_cards.end());
    
    auto best_five = pokerlib::GetFiveBestCards(all_cards);
    
    std::cout << "Best five cards: ";
    for (const auto& card : best_five) {
        std::cout << card.to_string() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### Equity Calculator Example

```cpp
#include "pokerlib/PokerLib.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

int main() {
    // Calculate equity between multiple hands
    std::vector<std::string> hands = {"AhKh", "QsQc"};
    std::string board = "8s9cTd";
    
    auto results = pokerlib::Evaluate(hands, board);
    
    // Display results
    std::cout << "Equity Calculation Results:" << std::endl;
    std::cout << "-------------------------" << std::endl;
    
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << "Player " << (i + 1) << " (" << hands[i] << "):" << std::endl;
        
        // Show current hand evaluation
        auto desc = pokerlib::HandDescriptionProvider::GetDescription(results[i].eval);
        std::cout << "  Current hand: " << desc.to_string() << std::endl;
        
        // Show win status
        if (results[i].winStatus == pokerlib::WinStatus::Ahead) {
            std::cout << "  Currently AHEAD" << std::endl;
        }
        else if (results[i].winStatus == pokerlib::WinStatus::Tied) {
            std::cout << "  Currently TIED for the lead" << std::endl;
        }
        else if (results[i].winStatus == pokerlib::WinStatus::Behind) {
            std::cout << "  Currently BEHIND" << std::endl;
        }
        
        // Show equity percentages
        std::cout << "  Win: " << std::fixed << std::setprecision(2)
                  << (results[i].winPercentage * 100.0) << "%" << std::endl;
        std::cout << "  Tie: " << std::fixed << std::setprecision(2)
                  << (results[i].tiePercentage * 100.0) << "%" << std::endl;
        std::cout << "  Total equity: " << std::fixed << std::setprecision(2)
                  << (results[i].equityPercentage * 100.0) << "%" << std::endl;
        
        // Show winning outs
        if (!results[i].immediateOutsToWin.empty()) {
            std::cout << "  Winning outs (" << results[i].immediateOutsToWin.size() << "): ";
            for (const auto& card : results[i].immediateOutsToWin) {
                std::cout << card.to_string() << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    return 0;
}
```

## Types

### Hand Categories

```cpp
enum class HandCategory {
    HighCard = 1,
    Pair = 2,
    TwoPair = 3,
    ThreeOfAKind = 4,
    Straight = 5,
    Flush = 6,
    FullHouse = 7,
    FourOfAKind = 8,
    StraightFlush = 9
};
```

### Win Status

```cpp
enum class WinStatus {
    Ahead,   // Currently winning
    Tied,    // Currently tied for the best hand
    Behind   // Currently not the best hand
};
```

### PlayerResults

```cpp
struct PlayerResults {
    WinStatus winStatus;           // Current win status of the player
    std::vector<Card> hand;        // The player's hole cards
    uint16_t eval;                 // Evaluation of the player's current hand
    
    uint64_t numWins;              // Number of win outcomes in simulations
    uint64_t numTies;              // Number of tie outcomes in simulations
    
    double winPercentage;          // Win percentage from simulations
    double tiePercentage;          // Tie percentage from simulations
    double equityPercentage;       // Total equity percentage from simulations
    
    std::vector<Card> immediateOutsToWin;  // Cards that would make this hand win
    std::vector<Card> immediateOutsToTie;  // Cards that would make this hand tie
};
```

## Integration Tips

- When evaluating multiple hands or calculating equity, use `Evaluate()` rather than individual evaluations.
- Use the variadic template `CombineCards` method to combine multiple card vectors efficiently.
- For performance-critical applications, pre-parse card strings to `Card` objects when possible.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Based on the OMPEval poker evaluation library.