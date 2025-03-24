# PokerLib

A powerful, high-performance poker hand evaluation and equity calculation library for Node.js, built as a native addon for maximum speed.

[![npm version](https://img.shields.io/npm/v/pokerlib-cpp.svg)](https://www.npmjs.com/package/pokerlib-cpp)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![TypeScript](https://img.shields.io/badge/TypeScript-Ready-blue.svg)](https://www.typescriptlang.org/)

## Features

- **Blazing Fast Performance**: C++ native addon core with optimized algorithms
- **Full TypeScript Support**: 100% type-safe API with rich documentation
- **Comprehensive Hand Evaluation**: Evaluates poker hands quickly and accurately
- **Equity Calculation**: Calculate win/tie probabilities for multiple hands
- **Winning Outs Analysis**: Identify cards that improve a player's hand
- **User-Friendly API**: Clean, intuitive interface with sensible defaults

## Installation

```bash
npm install pokerlib-cpp
```

## Requirements

- Node.js 14.x or later
- Python 3.x with setuptools installed (`pip3 install setuptools`)
- C++ build tools (Visual Studio on Windows, XCode on macOS, GCC on Linux)

## Quick Start

```typescript
import { PokerLib } from 'pokerlib-cpp';

// Evaluate a hand
const result = PokerLib.evaluateHand('AcAsAdQcQd');
console.log(`Hand: ${result.description.categoryName}`); // Full House
console.log(`Human readable: ${result.description.description}`); // full house, aces full of queens

// Calculate equity between hands
const hands = ['AhKh', 'QsQc'];
const board = '8s9cTd';
const results = PokerLib.calculateEquity(hands, board);

// Show equity for each hand
hands.forEach((hand, i) => {
    console.log(`${hand} is ${results[i].winStatus}`);
    console.log(`${hand}: ${(results[i].equityPercentage * 100).toFixed(2)}% equity`);
    console.log(`Winning outs: ${results[i].winningOuts.map(card => card.string).join(', ')}`);
    console.log(`Tying outs: ${results[i].tyingOuts.map(card => card.string).join(', ')}`);
    console.log(` `);
});
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

### `PokerLib.parseCards(notation: string): Card[]`

Converts a card string notation to an array of Card objects.

```typescript
const cards = PokerLib.parseCards('AhKs');
// Returns: [{ rank: 14, suit: 1, string: 'Ah' }, { rank: 13, suit: 0, string: 'Ks' }]
```

### `PokerLib.evaluateHand(hand: Hand): HandEvaluation`

Evaluates a poker hand, returning its value and description.

```typescript
const result = PokerLib.evaluateHand('AhKhQhJhTh');
console.log(result.description.description); // "royal flush"
```

### `PokerLib.calculateEquity(hands: string[], board?: string, deadCards?: string): PlayerResults[]`

Calculates equity for multiple hands given a board.

```typescript
const results = PokerLib.calculateEquity(['AhKh', 'QsQc'], '8s9cTd');
console.log(`Player 1 equity: ${results[0].equityPercentage * 100}%`);
```

### `PokerLib.createDeck(): Card[]`

Creates a new deck of cards.

```typescript
const deck = PokerLib.createDeck();
console.log(`Deck has ${deck.length} cards`);
```

### `PokerLib.isWinning(hand: string, otherHands: string[], board?: string): boolean`

Determines if a hand is currently winning against other hands.

```typescript
const isWinning = PokerLib.isWinning('AhKh', ['QsQc', '7d7s'], '8s9cTd');
console.log(isWinning); // true/false
```

### `PokerLib.getWinningOuts(hand: string, otherHands: string[], board?: string): Card[]`

Gets possible winning outs for a hand.

```typescript
const outs = PokerLib.getWinningOuts('AhKh', ['QsQc'], '8s9cTd');
console.log(`You have ${outs.length} winning outs`);
```

It is recommended to call the calculateEquity function when needing multiple types of information to only perform the computation on the hands once.

### `PokerLib.getBestHand(hand: string, board?: string): Card[]`

Gets the best five cards from a set of cards (useful for determining the optimal 5-card hand in Texas Hold'em from 7 available cards).

```typescript
// Get best 5 cards from a combination of hole cards and board
const bestHand = PokerLib.getBestHand('AhKh', 'QhJhTh');
// Returns royal flush: [Ah, Kh, Qh, Jh, Th]

// Works with 7-card hands (typical in Texas Hold'em)
const texasHoldemBestHand = PokerLib.getBestHand('8c8s', '8d8hAcKdQh');
// Returns four of a kind: [8c, 8s, 8d, 8h, Ac]
```

### `PokerLib.formatHandDescription(hand: Hand, board?: string): string`

Formats hand description in a user-friendly way.

```typescript
const desc = PokerLib.formatHandDescription('AhKhQhJhTh');
console.log(desc); // "Royal Flush"
```

## Types

### `Card`

```typescript
interface Card {
  rank: number;   // 2-14 (14 = Ace)
  suit: number;   // 0-3 (0=spades, 1=hearts, 2=diamonds, 3=clubs)
  string: string; // e.g., "Ah", "Ks"
}
```

### `HandCategory`

```typescript
enum HandCategory {
  HighCard = 1,
  Pair = 2,
  TwoPair = 3,
  ThreeOfAKind = 4,
  Straight = 5,
  Flush = 6,
  FullHouse = 7,
  FourOfAKind = 8,
  StraightFlush = 9
}
```

### `PlayerResults`

```typescript
interface PlayerResults {
  winPercentage: number;    // 0-1
  tiePercentage: number;    // 0-1
  equityPercentage: number; // 0-1
  winStatus: WinStatus;     // 'Ahead', 'Tied', or 'Behind'
  winningOuts: Card[];      // Cards that would make this hand win
  tyingOuts: Card[];        // Cards that would make this hand tie
}
```

## Development

### Building from Source

```bash
git clone https://github.com/jensroth-git/pokerlib-cpp.git
cd pokerlib-cpp
npm install
npm run build
```

### Troubleshooting Build Issues

If you're having issues building the native module:

1. Make sure you have Python installed and in your PATH
2. On Windows, install Visual Studio with "Desktop development with C++"
3. On macOS, install XCode and XCode Command Line Tools
4. On Linux, install GCC and required development packages

For specific issues with node-gyp, refer to the [node-gyp documentation](https://github.com/nodejs/node-gyp#installation).

### Running Tests

```bash
npm test
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Based on the OMPEval poker evaluation library. 