#ifndef POKERLIB_EVALUATOR_H
#define POKERLIB_EVALUATOR_H

#include <vector>
#include <string>
#include <cstdint>
#include "pokerlib/Card.h"

namespace pokerlib {
	/**
	 * @brief Enumeration of possible win statuses
	 */
	enum class WinStatus {
		Ahead,
		Tied,
		Behind
	};

	/**
	 * @brief Enumeration of possible game states
	 */
	enum class GameState {
		PreFlop,
		Flop,
		Turn,
		River
	};

	/**
	 * @brief Structure containing detailed poker evaluation results for a player
	 */
	struct PlayerResults {
		WinStatus winStatus;           ///< Current win status of the player

		std::vector<Card> hand;        ///< The player's hole cards
		uint16_t eval;				   ///< Evaluation of the player's current hand

		uint64_t numWins;              ///< Number of win outcomes in simulations
		uint64_t numTies;              ///< Number of tie outcomes in simulations

		double winPercentage;          ///< Win percentage from simulations
		double tiePercentage;          ///< Tie percentage from simulations
		double equityPercentage;       ///< Total equity percentage from simulations

		std::vector<Card> immediateOutsToWin;     ///< 1 Card runouts that improve this player to win
		std::vector<Card> immediateOutsToTie;     ///< 1 Card runouts that tie this player
	};

	/**
	 * @brief Gets the evaluation of a hand
	 * @param hand The hand to evaluate
	 * @return Evaluation value of the hand
	 */
	uint16_t GetHandEvaluation(omp::Hand hand);

	/**
	 * @brief Checks if a player's hand is winning against opponents
	 * @param opponentEvals Vector of opponent hand evaluations
	 * @param playerEval The player's hand evaluation
	 * @return True if the player is winning
	 */
	bool IsWinning(std::vector<uint16_t> opponentEvals, uint16_t playerEval);

	/**
	 * @brief Checks if a player's hand is tied with the best opponent hand
	 * @param opponentEvals Vector of opponent hand evaluations
	 * @param playerEval The player's hand evaluation
	 * @return True if the player is tied for the best hand
	 */
	bool IsTie(std::vector<uint16_t> opponentEvals, uint16_t playerEval);

	/**
	 * @brief Gets the five best cards from a set of cards
	 * @param cards Vector of cards to evaluate
	 * @return The five best cards
	 */
	std::vector<Card> GetFiveBestCards(std::vector<Card> cards);

	/**
	 * @brief Evaluates poker hands and calculates equity
	 * @param hands Vector of hand strings (e.g., "AhKs")
	 * @param boardStr String representing the board cards
	 * @param deadStr String representing dead cards to exclude from runouts
	 * @return Vector of PlayerResults for each player
	 */
	std::vector<PlayerResults> Evaluate(std::vector<std::string> hands, std::string boardStr = "", std::string deadStr = "");

} // namespace pokerlib

#endif // POKERLIB_EVALUATOR_H 