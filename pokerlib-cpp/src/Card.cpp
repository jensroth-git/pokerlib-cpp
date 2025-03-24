#include "pokerlib/Card.h"
#include <cctype>
#include <stdexcept>

namespace pokerlib {

	unsigned Card::charToRank(char c) {
		switch (c) {
		case 'a': return 12;
		case 'k': return 11;
		case 'q': return 10;
		case 'j': return 9;
		case 't': return 8;
		case '9': return 7;
		case '8': return 6;
		case '7': return 5;
		case '6': return 4;
		case '5': return 3;
		case '4': return 2;
		case '3': return 1;
		case '2': return 0;
		default: return ~0u;
		}
	}

	unsigned Card::charToSuit(char c) {
		switch (c) {
		case 's': return 0;
		case 'h': return 1;
		case 'c': return 2;
		case 'd': return 3;
		default: return ~0u;
		}
	}

	omp::Hand Card::CreateHand(const std::vector<Card>& cards) {
		omp::Hand hand = omp::Hand::empty();

		for (const auto& card : cards) {
			hand += omp::Hand(RankSuitToCardIndex(card.rank, card.suit));
		}

		return hand;
	}

	std::vector<Card> Card::CombineCards(std::vector<Card> cards) {
		return cards;
	}

	int Card::RankSuitToCardIndex(int rank, int suit) {
		return rank * 4 + suit;
	}

	unsigned int Card::GetCard(char cRank, char cSuit) {
		auto rank = charToRank(tolower(cRank));
		auto suit = charToSuit(tolower(cSuit));

		if (rank == ~0u || suit == ~0u) {
			// In a real library, we would throw an exception here
			throw std::invalid_argument("Invalid card specification");
		}

		return (4 * rank) + suit;
	}

	std::vector<Card> Card::GetCards(std::string str) {
		std::vector<Card> cards;

		for (size_t i = 0; i < str.length(); i += 2) {
			if (i + 1 < str.length()) {
				cards.push_back(FromCharRankSuit(str[i], str[i + 1]));
			}
		}

		return cards;
	}

	Card Card::FromString(std::string str)
	{
		assert(str.length() == 2);

		char cRank = str[0];
		char cSuit = str[1];

		return FromCharRankSuit(cRank, cSuit);
	}

	Card Card::FromCharRankSuit(char cRank, char cSuit)
	{
		int rank = charToRank(tolower(cRank));
		int suit = charToSuit(tolower(cSuit));

		if (rank == ~0u || suit == ~0u) {
			throw std::invalid_argument("Invalid card specification");
		}

		return FromRankSuit(rank, suit);
	}

	Card Card::FromRankSuit(int rank, int suit)
	{
		Card card;

		if (rank < 0 || rank > 12 || suit < 0 || suit > 3) {
			throw std::invalid_argument("Invalid rank or suit");
		}

		card.rank = rank;
		card.suit = suit;

		return card;
	}

	std::string Card::to_string() const 
	{
		static const char* ranks = "23456789tjqka";
		static const char* suits = "shcd";

		return std::string("") + ranks[rank] + suits[suit];
	}

	bool Card::operator==(const Card& card2) const {
		return rank == card2.rank && suit == card2.suit;
	}

} // namespace pokerlib 