#ifndef POKERLIB_DECK_H
#define POKERLIB_DECK_H

#include <vector>
#include <random>
#include <algorithm>

#include "pokerlib/Card.h"

namespace pokerlib {
	/**
	 * @brief Represents a deck of cards
	 *
	 * This class provides functionality for working with a deck of cards,
	 * including shuffling and dealing cards.
	 */
	class Deck {
	public:
		/**
		 * @brief Constructs a new deck of cards
		 */
		Deck() { ResetDeck(); }

		/**
		 * @brief Resets the deck to its initial state (52 cards in order)
		 */
		void ResetDeck();

		/**
		 * @brief Shuffles the deck
		 */
		void Shuffle();

		/**
		 * @brief Deals a card from the deck
		 * @return The card that was dealt
		 */
		Card Deal();

		/**
		 * @brief Removes the specified cards from the deck
		 * @param remove Vector of cards to remove
		 */
		void RemoveCards(const std::vector<Card>& remove);

		/**
		* @brief Gets the remaining cards in the deck
		* does not modify the deck
		* @return Vector of remaining cards
		*/
		std::vector<Card> GetRemainingCards();

	private:
		std::vector<int> cards; ///< Cards in the deck
	};

} // namespace pokerlib

#endif // POKERLIB_DECK_H 