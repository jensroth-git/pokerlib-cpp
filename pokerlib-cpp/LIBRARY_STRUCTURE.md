# PokerLib C++ Library Structure

This document outlines the structure and organization of the PokerLib C++ implementation.

## Overview

PokerLib is a high-performance poker hand evaluation and equity calculation library written in C++. It provides a comprehensive set of tools for poker game development, analysis, and strategy research.

## File Structure

```
pokerlib-cpp/
├── binding/                   # Node.js native addon binding code
│   └── pokerlib_binding.cpp   # C++ to JavaScript bridge
├── include/                   # Public header files
│   └── pokerlib/              # Main library headers
│       ├── Card.h             # Card representation and manipulation
│       ├── Deck.h             # Deck management
│       ├── Evaluator.h        # Hand evaluation and equity calculation
│       ├── HandDescription.h  # Hand description and categorization
│       └── PokerLib.h         # Main include file
├── src/                       # Implementation files
│   ├── Card.cpp               # Card implementation
│   ├── Deck.cpp               # Deck implementation 
│   ├── Evaluator.cpp          # Evaluator implementation
│   └── HandDescription.cpp    # HandDescription implementation
├── examples/                  # Example programs
│   ├── equity_calculator.cpp  # Equity calculation example
│   └── hand_description.cpp   # Hand description example
├── omp/                       # Original OMPEval library (dependency)
├── binding.gyp                # Node-gyp build configuration
├── README.md                  # Project documentation
└── LIBRARY_STRUCTURE.md       # This file
```

## Core Components

### Card Class

The `Card` class provides a convenient way to represent playing cards and convert between different representations. It includes:

- Support for string representation (e.g., "Ah" for Ace of hearts)
- Conversion between character-based and numerical representations
- Methods for combining cards into hands
- Easy creation of cards from various formats (strings, separate rank/suit)
- Equality operator for card comparison

Important notes on `CombineCards`:
- The `CombineCards(vector<Card>)` method takes a vector of Card objects and returns an `std::vector<Card>` object
- This method should be called only once with all cards to evaluate, not separately for different card sets
- Multiple vectors can be combined using the convenient variadic template version `CombineCards(vector<Card>, ...vectors)`
- Use `CreateHand` to convert a card vector to a Hand object for evaluation

### Deck Class

The `Deck` class manages a standard 52-card deck with operations like:

- Shuffling the deck
- Dealing cards from the deck
- Removing specific cards from the deck
- Getting the count of remaining cards
- Retrieving all remaining cards

### Evaluator Module

The evaluator module provides functions for poker hand evaluation and equity calculation:

- Fast hand evaluation
- Best five-card hand determination
- Equity calculation between multiple hands
- Determining winning probability
- Identifying 'outs' (cards that improve a hand)

### HandDescription Module

The HandDescription module provides human-readable descriptions of poker hands:

- Conversion from evaluation score to hand category (e.g., straight, flush, pair)
- Detailed string representation of hands (e.g., "Ace-high flush")
- Access to individual components of hand descriptions

## PlayerResults Structure

The `PlayerResults` structure contains the results of equity calculations for a player:

- Current win status (ahead, tied, behind)
- Player's hole cards and evaluation
- Win, tie, and total equity percentages
- Simulation statistics (number of wins and ties)
- Immediate outs to win or tie on the next card

## Building and Using

The library uses node-gyp for building:

```bash
npm install
npm run build
```

To use the library in your project, include the main header:

```cpp
#include "pokerlib/PokerLib.h"
```

For more detailed usage examples, see the README.md file and the examples directory. 