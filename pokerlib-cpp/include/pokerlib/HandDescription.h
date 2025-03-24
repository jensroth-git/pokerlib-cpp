#ifndef POKERLIB_HAND_DESCRIPTION_H
#define POKERLIB_HAND_DESCRIPTION_H

#include <string>
#include <vector>
#include <cstdint>

namespace pokerlib {

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

	enum class CardValue {
		Two = 0,
		Three = 1,
		Four = 2,
		Five = 3,
		Six = 4,
		Seven = 5,
		Eight = 6,
		Nine = 7,
		Ten = 8,
		Jack = 9,
		Queen = 10,
		King = 11,
		Ace = 12
	};

	/**
	 * @brief Represents a description of a poker hand
	 *
	 * This class provides a convenient way to represent and describe
	 * poker hands like "pair of aces", "straight", etc.
	 */
	struct HandDescription {
		union {
			//the evaluation of the hand
			uint16_t eval;

			struct {
				//evaluation of the hand within the category
				uint16_t categoryEval : 12;

				//the category high card = 1, ... straight flush = 9
				uint16_t category : 4;
			};
		};

		uint8_t highCard : 4; ///< High card of the hand (e.g., in a pair, the rank of the pair)
		uint8_t lowCard : 4;  ///< Low card of the hand (e.g., in a full house, the rank of the pair)

		HandCategory GetCategory() const {
			return static_cast<HandCategory>(category);
		}

		CardValue GetHighCard() const {
			return static_cast<CardValue>(highCard);
		}

		CardValue GetLowCard() const {
			return static_cast<CardValue>(lowCard);
		}

		/**
		 * @brief Converts the hand description to a human-readable string
		 * @return String representation of the hand (e.g., "pair of aces")
		 */
		std::string to_string();

		/**
		 * @brief Creates an invalid hand description
		 * @return An invalid hand description
		 */
		static HandDescription Invalid();
	};

	/**
	 * @brief Provides functions to get descriptions for poker hands
	 */
	class HandDescriptionProvider {
	public:
		/**
		 * @brief Get a description for a hand evaluation
		 * @param eval The hand evaluation value
		 * @return HandDescription object with details about the hand
		 */
		static HandDescription GetDescription(uint16_t eval);
	};

	//std::string GetDescriptionValidate(uint16_t evaluation);
} // namespace pokerlib

#endif // POKERLIB_HAND_DESCRIPTION_H 