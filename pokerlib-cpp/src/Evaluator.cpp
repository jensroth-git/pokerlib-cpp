#include "pokerlib/Evaluator.h"
#include "omp/HandEvaluator.h"
#include "omp/EquityCalculator.h"
#include "omp/Constants.h"
#include <algorithm>
#include <iostream>

#include "pokerlib/Deck.h"

namespace pokerlib {
	uint16_t GetHandEvaluation(omp::Hand hand) {
		omp::HandEvaluator ev;
		return ev.evaluate(hand);
	}

	bool IsWinning(std::vector<uint16_t> opponentEvals, uint16_t playerEval) {
		return playerEval > *std::max_element(opponentEvals.begin(), opponentEvals.end());
	}

	bool IsTie(std::vector<uint16_t> opponentEvals, uint16_t playerEval) {
		return playerEval == *std::max_element(opponentEvals.begin(), opponentEvals.end());
	}

	std::vector<Card> GetFiveBestCards(std::vector<Card> cards) {
		if (cards.size() <= 5) {
			return cards;
		}

		// Keep removing the card that decreases the score the least until 5 cards are left
		omp::HandEvaluator ev;

		while (cards.size() > 5) {
			uint16_t maxScore = 0;
			int maxIndex = -1;

			for (int i = 0; i < cards.size(); i++) {
				std::vector<Card> hand;

				for (size_t j = 0; j < cards.size(); j++) {
					if (i != j) {
						hand.push_back(cards[j]);
					}
				}

				uint16_t eval = ev.evaluate(Card::CreateHand(hand));

				if (eval > maxScore) {
					maxScore = eval;
					maxIndex = i;
				}
			}

			// Remove the card that gives the highest score when removed
			if (maxIndex >= 0) {
				cards.erase(cards.begin() + maxIndex);
			}
		}

		return cards;
	}

	std::vector<std::vector<Card>> GetPlayerHands(const std::vector<PlayerResults> &players) {
		std::vector<std::vector<Card>> playerHands;
		for (auto&& player : players) {
			playerHands.push_back(player.hand);
		}
		return playerHands;
	}

	std::vector<uint16_t> EvaluatePlayers(const std::vector<std::vector<Card>> &playerHands, std::vector<Card> board) {
		std::vector<uint16_t> evals;
		omp::HandEvaluator ev;
		for (auto&& player : playerHands) {
			std::vector<Card> boardHand(player);
			boardHand.insert(boardHand.end(), board.begin(), board.end());

			auto eval = GetHandEvaluation(Card::CreateHand(boardHand));
			evals.push_back(eval);
		}
		return evals;
	}

	std::vector<PlayerResults> Evaluate(std::vector<std::string> hands, std::string boardStr, std::string deadStr) {
		assert(hands.size() > 1);
		assert(boardStr.length() <= 5 * 2);

		std::unordered_map<unsigned int, GameState> stateMap = {
			{ 0, GameState::PreFlop },
			{ 3, GameState::Flop },
			{ 4, GameState::Turn },
			{ 5, GameState::River }
		};

		GameState gameState = stateMap[boardStr.length() / 2];

		std::vector<PlayerResults> players(hands.size());

		// Setup hands 
		for (size_t i = 0; i < players.size(); i++) {
			players[i].hand = Card::GetCards(hands[i]);
		}

		// Calculate winning / tying percentages
		omp::EquityCalculator eq;

		// Construct ranges
		std::vector<omp::CardRange> ranges;
		for (auto&& player : players) {
			ranges.push_back(player.hand[0].to_string() + player.hand[1].to_string());
		}

		std::vector<Card> dead = Card::GetCards(deadStr);
		std::vector<Card> board = Card::GetCards(boardStr);

		auto boardMask = omp::CardRange::getCardMask(boardStr);
		auto deadMask = omp::CardRange::getCardMask(deadStr);

		if (eq.start(ranges, boardMask, deadMask, true)) {
			// Make a copy of dead cards for later use
			std::vector<Card> allDeadCards = dead;

			// Add board cards to dead cards
			allDeadCards.insert(allDeadCards.end(), board.begin(), board.end());

			// Add player hands to dead cards
			for (auto&& player : players) {
				allDeadCards.insert(allDeadCards.end(), player.hand.begin(), player.hand.end());
			}

			Deck deck;

			deck.RemoveCards(allDeadCards);
			auto deckCards = deck.GetRemainingCards();

			// Calculate evaluation
			const auto playerHands = GetPlayerHands(players);
			const auto evals = EvaluatePlayers(playerHands, board);

			for (auto&& player : players) {
				// OMFG this is so bad
				const unsigned int playerIndex = &player - &players[0];

				player.eval = evals[playerIndex];
			}

			//determine win status
			for (auto&& player : players) {
				std::vector<uint16_t> opponentEvals;
				for (auto&& otherPlayer : players) {
					if (&player != &otherPlayer) {
						opponentEvals.push_back(otherPlayer.eval);
					}
				}

				player.winStatus = IsWinning(opponentEvals, player.eval) ? WinStatus::Ahead : (IsTie(opponentEvals, player.eval) ? WinStatus::Tied : WinStatus::Behind);
			}

			//calculate immediate outs
			if (gameState == GameState::Flop || gameState == GameState::Turn) {
				for (auto&& player : players) {

					// Skip if player is already ahead
					if (player.winStatus == WinStatus::Ahead) {
						continue;
					}

					const unsigned int playerIndex = &player - &players[0];

					for (auto&& card : deckCards) {
						std::vector<Card> boardWithOut(board);
						boardWithOut.push_back(card);
						auto outEvals = EvaluatePlayers(playerHands, boardWithOut);

						const auto playerEval = outEvals[playerIndex];

						//remove player eval
						outEvals.erase(outEvals.begin() + playerIndex);

						//if we are tied right now we need to win to improve
						if (player.winStatus == WinStatus::Tied) {
							if (IsWinning(outEvals, playerEval)) {
								player.immediateOutsToWin.push_back(card);

								continue;
							}
						}

						// if we are behind right now we need to tie or win to improve
						if (player.winStatus == WinStatus::Behind) {
							if (IsWinning(outEvals, playerEval)) {
								player.immediateOutsToWin.push_back(card);
							}
							else if (IsTie(outEvals, playerEval)) {
								player.immediateOutsToTie.push_back(card);
							}
						}
					}
				}
			}

			eq.wait();

			auto res = eq.getResults();

			for (size_t i = 0; i < players.size(); i++) {
				players[i].numWins = res.wins[i];
				players[i].numTies = res.ties[i];

				players[i].winPercentage = float(players[i].numWins) / res.hands;
				players[i].tiePercentage = float(players[i].numTies) / res.hands;
				players[i].equityPercentage = res.equity[i];
			}
		}

		return players;
	}
} // namespace pokerlib 