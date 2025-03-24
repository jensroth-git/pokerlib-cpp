#include "pokerlib/Deck.h"

namespace pokerlib {
	std::pair<int, int> CardIndexToRankSuit(int card) {
		int rank = card / 4;
		int suit = card % 4;
		return { rank, suit };
	}

	void Deck::ResetDeck() {
		cards.clear();

		for (int i = 0; i < 52; i++) {
			cards.push_back(i);
		}
	}

	void Deck::Shuffle() {
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(cards.begin(), cards.end(), g);
	}

	Card Deck::Deal() {
		if (cards.empty()) {
			// Handle error: attempting to deal from an empty deck
			throw std::runtime_error("Cannot deal from an empty deck");
		}

		int card = cards[0];
		cards.erase(cards.begin());

		auto rankSuit = CardIndexToRankSuit(card);
		int rank = rankSuit.first;
		int suit = rankSuit.second;

		return Card::FromRankSuit(rank, suit);
	}

	void Deck::RemoveCards(const std::vector<Card>& remove) {
		//remove cards from the deck
		for (const auto& card : remove) {
			auto cardIndex = Card::RankSuitToCardIndex(card.rank, card.suit);
			auto it = std::find(cards.begin(), cards.end(), cardIndex);
			if (it != cards.end()) {
				cards.erase(it);
			}
		}
	}

	std::vector<Card> Deck::GetRemainingCards() {
		std::vector<Card> cards;

		for (int cardIndex : this->cards) {
			auto rankSuit = CardIndexToRankSuit(cardIndex);
			int rank = rankSuit.first;
			int suit = rankSuit.second;
			cards.push_back(Card::FromRankSuit(rank, suit));
		}

		return cards;
	}
} // namespace pokerlib