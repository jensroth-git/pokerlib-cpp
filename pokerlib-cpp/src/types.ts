/**
 * Represents a playing card with rank and suit information
 */
export interface Card {
  /** The rank of the card (2-14, where 14 is Ace) */
  rank: number;
  
  /** The suit of the card (0-3, where 0=spades, 1=hearts, 2=diamonds, 3=clubs) */
  suit: number;
  
  /** String representation of the card (e.g., "Ah", "Ks") */
  string: string;
}

/**
 * Enum representing different poker hand categories
 */
export enum HandCategory {
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

/**
 * Describes a poker hand evaluation result
 */
export interface HandDescription {
  /** The hand category (high card, pair, two pair, etc.) */
  category: HandCategory;
  
  /** Human-readable name of the hand category */
  categoryName: string;
  
  /** Full description of the hand (e.g., "pair of aces") */
  description: string;
}

/**
 * Hand evaluation result including numerical value and description
 */
export interface HandEvaluation {
  /** Numerical evaluation value (lower is better) */
  value: number;
  
  /** Description of the hand */
  description: HandDescription;
}

/**
 * Player position relative to other players
 */
export enum WinStatus {
  /** Player is ahead of all other players */
  Ahead = 'Ahead',
  
  /** Player is tied with at least one other player */
  Tied = 'Tied',
  
  /** Player is behind at least one other player */
  Behind = 'Behind'
}

/**
 * Results from an equity calculation for a player
 */
export interface PlayerResults {
  /** Percentage of time the player wins (0-1) */
  winPercentage: number;
  
  /** Percentage of time the player ties (0-1) */
  tiePercentage: number;
  
  /** Total equity (winPercentage + tiePercentage/2) */
  equityPercentage: number;
  
  /** Current status compared to other players */
  winStatus: WinStatus;
  
  /** Cards that would immediately improve the hand to a winning hand */
  winningOuts: Card[];
  
  /** Cards that would immediately improve the hand to a tied hand */
  tyingOuts: Card[];
}

/**
 * A card or collection of cards in string notation
 * Examples: "Ah" (Ace of hearts), "KsQh" (King of spades, Queen of hearts)
 */
export type CardNotation = string;

/**
 * A collection of 2+ cards that form a poker hand
 */
export type Hand = CardNotation | Card[];

/**
 * Board cards in string notation
 */
export type Board = CardNotation;

/**
 * Dead cards (cards that are no longer in play) in string notation
 */
export type DeadCards = CardNotation; 