import { PokerLib, HandCategory, WinStatus } from '../src/index';

describe('PokerLib', () => {
  describe('parseCards', () => {
    it('should parse card notation correctly', () => {
      const cards = PokerLib.parseCards('AhKs');
      expect(cards).toHaveLength(2);
      expect(cards[0].rank).toBe(12); // Ace
      expect(cards[0].suit).toBe(1);  // Hearts
      expect(cards[0].string).toBe('ah');
      expect(cards[1].rank).toBe(11); // King
      expect(cards[1].suit).toBe(0);  // Spades
      expect(cards[1].string).toBe('ks');
    });
  });

  describe('evaluateHand', () => {
    it('should correctly evaluate a royal flush', () => {
      const result = PokerLib.evaluateHand('AhKhQhJhTh');
      expect(result.description.category).toBe(HandCategory.StraightFlush);
      expect(result.description.description).toContain('royal flush');
    });

    it('should correctly evaluate a pair', () => {
      const result = PokerLib.evaluateHand('AhAd2s3c4h');
      expect(result.description.category).toBe(HandCategory.Pair);
      expect(result.description.description).toContain('pair');
    });
  });

  describe('calculateEquity', () => {
    it('should calculate equity for multiple hands', () => {
      const results = PokerLib.calculateEquity(['AhKh', 'QsQc'], '8s9cTd');
      expect(results).toHaveLength(2);
      expect(results[0]).toHaveProperty('winPercentage');
      expect(results[0]).toHaveProperty('tiePercentage');
      expect(results[0]).toHaveProperty('equityPercentage');
      expect(results[0]).toHaveProperty('winStatus');
    });
  });

  describe('createDeck', () => {
    it('should create a complete deck', () => {
      const deck = PokerLib.createDeck();
      expect(deck).toHaveLength(52);
    });
  });

  describe('isWinning', () => {
    it('should determine if a hand is winning', () => {
      // Pocket aces vs pocket kings with a board of 8s9cTd
      const isWinning = PokerLib.isWinning('AsAh', ['KsKh'], '8s9cTd');
      expect(typeof isWinning).toBe('boolean');
    });
  });

  describe('getWinningOuts', () => {
    it('should get winning outs for a hand', () => {
      const outs = PokerLib.getWinningOuts('AhKh', ['QsQc'], '8s9c2d');
      // There should be winning outs for AK vs QQ on this board
      expect(Array.isArray(outs)).toBe(true);
    });
  });

  describe('getBestHand', () => {
    it('should return the best 5-card hand from a royal flush', () => {
      const bestHand = PokerLib.getBestHand('AhKh', 'QhJhTh');
      expect(bestHand).toHaveLength(5);
      
      // Check that it's a royal flush by evaluating the hand
      const handString = bestHand.map(card => card.string).join('');
      const evaluation = PokerLib.evaluateHand(handString);
      
      expect(evaluation.description.category).toBe(HandCategory.StraightFlush);
      expect(evaluation.description.description).toContain('royal flush');
    });

    it('should return the best 5-card hand from a 7-card set', () => {
      const bestHand = PokerLib.getBestHand('8c8s', '8d8hAcKdQh');
      expect(bestHand).toHaveLength(5);
      
      // Check that it's four of a kind by evaluating the hand
      const handString = bestHand.map(card => card.string).join('');
      const evaluation = PokerLib.evaluateHand(handString);
      
      expect(evaluation.description.category).toBe(HandCategory.FourOfAKind);
      expect(evaluation.description.description).toContain('four of a kind');
    });

    it('should handle more than 7 cards by taking the best 5', () => {
      const bestHand = PokerLib.getBestHand('AsKs', '2h4d6cQsJs9s');
      expect(bestHand).toHaveLength(5);
      
      // Should find the flush in spades
      const handString = bestHand.map(card => card.string).join('');
      const evaluation = PokerLib.evaluateHand(handString);
      
      expect(evaluation.description.category).toBe(HandCategory.Flush);
    });
  });

  describe('formatHandDescription', () => {
    it('should format hand description nicely', () => {
      const desc = PokerLib.formatHandDescription('AhKhQhJhTh');
      expect(desc).toBe('Royal Flush');
    });
  });
}); 