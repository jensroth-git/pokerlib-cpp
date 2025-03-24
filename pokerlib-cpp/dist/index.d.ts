import { Card, Hand, HandEvaluation, PlayerResults, Board, DeadCards, CardNotation, HandCategory, WinStatus } from './types';
/**
 * PokerLib - A powerful poker hand evaluation and equity calculation library
 */
export declare class PokerLib {
    /**
     * Converts a card string notation to an array of Card objects
     *
     * @example
     * ```ts
     * const cards = PokerLib.parseCards('AhKs');
     * // Returns: [{ rank: 14, suit: 1, string: 'Ah' }, { rank: 13, suit: 0, string: 'Ks' }]
     * ```
     *
     * @param notation - String notation of cards (e.g., "AhKs")
     * @returns Array of Card objects
     */
    static parseCards(notation: CardNotation): Card[];
    /**
     * Evaluates a poker hand
     *
     * @example
     * ```ts
     * const result = PokerLib.evaluateHand('AhKhQhJhTh');
     * console.log(result.description.description); // "royal flush"
     * ```
     *
     * @param hand - Hand to evaluate (string notation or array of Card objects)
     * @returns Hand evaluation result
     */
    static evaluateHand(hand: Hand): HandEvaluation;
    /**
     * Calculates equity for multiple hands given a board
     *
     * @example
     * ```ts
     * const results = PokerLib.calculateEquity(['AhKh', 'QsQc'], '8s9cTd');
     * console.log(`Player 1 equity: ${results[0].equityPercentage * 100}%`);
     * ```
     *
     * @param hands - Array of hands (string notation)
     * @param board - Board cards (string notation)
     * @param deadCards - Dead cards not in play (string notation)
     * @returns Array of player results with equity data
     */
    static calculateEquity(hands: CardNotation[], board?: Board, deadCards?: DeadCards): PlayerResults[];
    /**
     * Creates a new deck of cards
     *
     * @example
     * ```ts
     * const deck = PokerLib.createDeck();
     * console.log(`Deck has ${deck.length} cards`);
     * ```
     *
     * @returns Array of all cards in a standard deck
     */
    static createDeck(): Card[];
    /**
     * Determines if a hand is currently winning against other hands
     *
     * @example
     * ```ts
     * const isWinning = PokerLib.isWinning('AhKh', ['QsQc', '7d7s'], '8s9cTd');
     * console.log(isWinning); // false
     * ```
     *
     * @param hand - Hand to check
     * @param otherHands - Other hands to compare against
     * @param board - Board cards
     * @returns True if the hand is currently ahead
     */
    static isWinning(hand: CardNotation, otherHands: CardNotation[], board?: Board): boolean;
    /**
     * Gets possible winning outs for a hand
     *
     * @example
     * ```ts
     * const outs = PokerLib.getWinningOuts('AhKh', ['QsQc'], '8s9cTd');
     * console.log(`You have ${outs.length} winning outs`);
     * ```
     *
     * @param hand - Hand to find outs for
     * @param otherHands - Other hands to compare against
     * @param board - Board cards
     * @returns Array of cards that would improve the hand to a winner
     */
    static getWinningOuts(hand: CardNotation, otherHands: CardNotation[], board?: Board): Card[];
    /**
     * Gets the best five cards from a set of cards
     *
     * @example
     * ```ts
     * const bestHand = PokerLib.getBestHand('AhKh7d8c', 'QhJhTh');
     * // Returns the best 5 cards (a royal flush in this case)
     * ```
     *
     * @param hand - Hand cards
     * @param board - Board cards
     * @returns Array of the 5 best cards
     */
    static getBestHand(hand: CardNotation, board?: Board): Card[];
    /**
     * Formats hand description in a user-friendly way
     *
     * @example
     * ```ts
     * const desc = PokerLib.formatHandDescription('AhKhQhJhTh');
     * console.log(desc); // "Royal Flush"
     * ```
     *
     * @param hand - Hand to format
     * @param board - Optional board cards
     * @returns Formatted hand description
     */
    static formatHandDescription(hand: Hand, board?: Board): string;
}
export { Card, HandCategory, HandEvaluation, PlayerResults, WinStatus };
export default PokerLib;
