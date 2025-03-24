#ifndef POKERLIB_CARD_H
#define POKERLIB_CARD_H

#include <string>
#include <vector>
#include <algorithm>
#include "omp/Hand.h"

namespace pokerlib {
	/**
	 * @brief Class representing a single playing card.
	 *
	 * This class provides a convenient interface for working with cards,
	 * converting between string representations and the underlying Hand type.
	 */
	class Card {
		Card() = default;

	public:
		/**
		 * @brief Converts a vector of cards to a Hand object for evaluation
		 * @param cards Vector of Card objects to convert
		 * @return Hand object for evaluation
		 */
		static omp::Hand CreateHand(const std::vector<Card>& cards);

		/**
		 * @brief Combines a vector of cards into a single vector
		 * @param cards Vector of Card objects to combine
		 * @return Combined vector of Card objects
		 */
		static std::vector<Card> CombineCards(std::vector<Card> cards);

		/**
		 * @brief Combines multiple vectors of cards into a single vector
		 * @param firstVector First vector of Card objects
		 * @param restVectors Rest of the vectors of Card objects
		 * @return Combined vector of Card objects
		 * 
		 * This method combines an arbitrary number of Card vectors into a single vector.
		 */
		template<typename... Vectors>
		static std::vector<Card> CombineCards(const std::vector<Card>& firstVector, const Vectors&... restVectors) {
			// Start with an empty vector
			std::vector<Card> result;
			
			// Reserve space for all vectors to avoid reallocations
			result.reserve(firstVector.size() + (restVectors.size() + ...));
			
			// Add cards from the first vector
			result.insert(result.end(), firstVector.begin(), firstVector.end());
			
			// Recursive call to add the rest of the vectors
			if constexpr (sizeof...(restVectors) > 0) {
				AddToVector(result, restVectors...);
			}
			
			return result;
		}

	private:
		/**
		 * @brief Helper method to recursively add cards to a vector
		 * @param result Vector to add cards to
		 * @param vector Vector of Card objects to add
		 * @param restVectors Rest of the vectors of Card objects
		 */
		template<typename... Vectors>
		static void AddToVector(std::vector<Card>& result, const std::vector<Card>& vector, const Vectors&... restVectors) {
			// Add cards from the current vector
			result.insert(result.end(), vector.begin(), vector.end());
			
			// Recursive call to add the rest of the vectors
			if constexpr (sizeof...(restVectors) > 0) {
				AddToVector(result, restVectors...);
			}
		}

	public:
		static int RankSuitToCardIndex(int rank, int suit);

		/**
		 * @brief Construct a card from a string (e.g., "Ah" for Ace of hearts)
		 * @param str A two-character string with rank and suit
		 */
		static Card FromString(std::string str);

		/**
		 * @brief Construct a card from separate rank and suit characters
		 * @param cRank Rank character (2-9, t, j, q, k, a)
		 * @param cSuit Suit character (s, h, c, d)
		 */
		static Card FromCharRankSuit(char cRank, char cSuit);

		/**
		 * @brief Construct a card from numerical rank and suit
		 * @param rank Rank from 0 (2) to 12 (Ace)
		 * @param suit Suit from 0 (spades) to 3 (diamonds)
		 */
		static Card FromRankSuit(int rank, int suit);

		/**
		 * @brief Get cards from a string representation
		 * @param str String containing card representations (e.g., "AhKs" for Ace of hearts, King of spades)
		 * @return Vector of Card objects
		 */
		static std::vector<Card> GetCards(std::string str);

		/**
		 * @brief Convert a character to a rank value
		 * @param c Character representation of a rank
		 * @return Numerical rank value
		 */
		static unsigned charToRank(char c);

		/**
		 * @brief Convert a character to a suit value
		 * @param c Character representation of a suit
		 * @return Numerical suit value
		 */
		static unsigned charToSuit(char c);

		/**
		 * @brief Get the numerical representation of a card
		 * @param cRank Rank character
		 * @param cSuit Suit character
		 * @return Numerical card value
		 */
		static unsigned int GetCard(char cRank, char cSuit);

		/**
		 * @brief Check equality between two cards
		 * @param card2 Card to compare with
		 * @return True if cards are the same
		 */
		bool operator==(const Card& card2) const;

		std::string to_string() const;

		unsigned int rank;  ///< Rank of the card (0-12)
		unsigned int suit;  ///< Suit of the card (0-3)
	};
} // namespace pokerlib

#endif // POKERLIB_CARD_H 