#include "pokerlib/HandDescription.h"
#include "omp/Constants.h"
#include "omp/HandEvaluator.h"
#include <vector>
#include <string>
#include <array>
#include <cmath>

#include "pokerlib/Card.h"

namespace pokerlib {

	// Implementation of HandDescription methods
	std::string HandDescription::to_string() {
		if (eval == 0) {
			return "";
		}

		static std::vector<std::string> rankToString = { "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king", "ace" };
		static std::vector<std::string> rankToStringPlural = { "twos", "threes", "fours", "fives", "sixes", "sevens", "eights", "nines", "tens", "jacks", "queens", "kings", "aces" };

		if (category == 1) {
			return "high card, " + rankToString[highCard];
		}
		else if (category == 2) {
			return "pair of " + rankToStringPlural[highCard];
		}
		else if (category == 3) {
			return "two pair, " + rankToStringPlural[highCard] + " and " + rankToStringPlural[lowCard];
		}
		else if (category == 4) {
			return "three of a kind, " + rankToStringPlural[highCard];
		}
		else if (category == 5) {
			return rankToString[highCard] + "-high straight";
		}
		else if (category == 6) {
			return rankToString[highCard] + "-high flush";
		}
		else if (category == 7) {
			return "full house, " + rankToStringPlural[highCard] + " full of " + rankToStringPlural[lowCard];
		}
		else if (category == 8) {
			return "four of a kind, " + rankToStringPlural[highCard];
		}
		else if (category == 9) {
			if (highCard == 12) {
				return "royal flush";
			}

			return rankToString[highCard] + "-high straight flush";
		}

		return "";
	}

	HandDescription HandDescription::Invalid() {
		return { 0, 0, 0 };
	}

	// Implementation of HandDescriptionProvider
	HandDescription HandDescriptionProvider::GetDescription(uint16_t eval) {
		// determine category
		uint8_t category = eval / omp::HAND_CATEGORY_OFFSET;

		// high card
		if (category == 1) {
			// not easily fit-able to a function
			// counts number of possible kicker combinations excluding straights
			std::array<uint16_t, omp::RANK_COUNT> evaluations = { 4098, 4099, 4101, 4105, 4113, 4128, 4158, 4214, 4312, 4474, 4729, 5114, 5675 };
			uint8_t highCard = 0;

			for (uint8_t i = 0; i < evaluations.size(); i++) {
				if (evaluations[i] > eval) {
					break;
				}
				highCard = i;
			}

			return { eval, highCard, 0 };
		}
		// pair
		else if (category == 2) {
			// not easily fit-able to a function
			// counts number of possible kicker combinations excluding straights
			std::array<uint16_t, omp::RANK_COUNT> evaluations = { 8193, 8492, 8791, 9090, 9389, 9688, 9987, 10286, 10585, 10884, 11183, 11482, 11781 };
			uint8_t highCard = 0;

			for (uint8_t i = 0; i < evaluations.size(); i++) {
				if (evaluations[i] > eval) {
					break;
				}
				highCard = i;
			}

			return { eval, highCard, 0 };
		}
		// two pair
		else if (category == 3) {
			// smallest 2 pair starts at 33 22, hence only 12 two pair sub categories
			int categoryIndex = (eval % omp::HAND_CATEGORY_OFFSET - 1);
			int index = categoryIndex / (omp::RANK_COUNT - 1);

			uint8_t highCard = (sqrt((8 * index) + 1) - 1) / 2;

			int highCardStartingIndex = (0.5 * highCard) + (0.5 * (highCard * highCard));

			uint8_t lowCard = index - highCardStartingIndex;
			int kicker = categoryIndex % (omp::RANK_COUNT - 1);

			highCard += 1;

			return { eval, highCard, lowCard };
		}
		// three of a kind
		else if (category == 4) {
			int categoryIndex = (eval % omp::HAND_CATEGORY_OFFSET - 1);

			// excludes full houses & four of a kind
			const int threeOfAKindSubCombinations = 1 + (omp::RANK_COUNT - 1) + (((omp::RANK_COUNT - 1) * (omp::RANK_COUNT - 2)) / 2);

			uint8_t highCard = categoryIndex / threeOfAKindSubCombinations;

			return { eval, highCard, 0 };
		}
		// straight
		else if (category == 5) {
			int categoryIndex = (eval % omp::HAND_CATEGORY_OFFSET - 1);
			uint8_t highCard = categoryIndex + 3;

			return { eval, highCard, 0 };
		}
		// flush
		else if (category == 6) {
			// not easily fit-able to a function since there are more flushes at higher ranks
			std::array<uint16_t, 8> evaluations = { 24623, 24653, 24709, 24807, 24969, 25224, 25609, 26177 };
			uint8_t highCard = 0;

			for (uint8_t i = 0; i < evaluations.size(); i++) {
				if (evaluations[i] > eval) {
					break;
				}
				highCard = i;
			}

			highCard += 5;

			return { eval, highCard, 0 };
		}
		// full house
		else if (category == 7) {
			// full house skips high card = low card combination since it would be five of a kind
			const int fullHouseSubCombinations = omp::RANK_COUNT - 1;

			int categoryIndex = (eval % omp::HAND_CATEGORY_OFFSET - 1);

			uint8_t highCard = categoryIndex / fullHouseSubCombinations;
			uint8_t lowCard = categoryIndex % fullHouseSubCombinations;

			// skip high card index
			if (lowCard >= highCard) {
				lowCard++;
			}

			return { eval, highCard, lowCard };
		}
		// four of a kind
		else if (category == 8) {
			int categoryIndex = (eval % omp::HAND_CATEGORY_OFFSET - 1);
			uint8_t highCard = categoryIndex / omp::RANK_COUNT;

			return { eval, highCard, 0 };
		}
		// straight flush
		else if (category == 9) {
			int categoryIndex = (eval % omp::HAND_CATEGORY_OFFSET - 1);
			uint8_t highCard = categoryIndex + 3;

			return { eval, highCard, 0 };
		}

		if (category == 0 || category > 9) {
			return HandDescription::Invalid();
		}
	}

	// validation implementation to ensure the math crazyness above is correct
	// dont use this in production
	std::string GetDescriptionValidate(uint16_t evaluation)
	{
		std::vector<std::string> rankToString = { "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king", "ace" };
		std::vector<std::string> rankToStringPlural = { "twos", "threes", "fours", "fives", "sixes", "sevens", "eights", "nines", "tens", "jacks", "queens", "kings", "aces" };

		omp::HandEvaluator ev;

		int category = evaluation / omp::HAND_CATEGORY_OFFSET;

		switch (category)
		{
		case 1: //HighCard 
		{
			//suit doesn't matter
			uint8_t rank = 0;
			for (uint8_t rankCheck = 0; rankCheck < omp::RANK_COUNT; rankCheck++)
			{
				uint16_t testEval = ev.evaluate(
					omp::Hand::empty() +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 0))
				);

				if (testEval > evaluation)
				{
					break;
				}

				rank = rankCheck;
			}

			return "high card, " + rankToString[rank];
		}
		case 2: //Pair
		{
			//suit doesn't matter
			uint8_t rank = 0;
			for (uint8_t rankCheck = 0; rankCheck < omp::RANK_COUNT; rankCheck++)
			{
				uint16_t testEval = ev.evaluate(
					omp::Hand::empty() +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 1))
					);

				if (testEval > evaluation)
				{
					break;
				}

				rank = rankCheck;
			}

			return "pair of " + rankToStringPlural[rank];
		}
		case 3: //Two Pair
		{
			//suit doesn't matter
			uint8_t rankMainPair = 0;
			uint8_t rankKickerPair = 0;

			//find main pair then kicker pair

			//start one rank offset ... (checking against twos already)
			for (uint8_t rankMainPairCheck = 1; rankMainPairCheck < omp::RANK_COUNT; rankMainPairCheck++)
			{
				uint16_t testEval = ev.evaluate(
					omp::Hand::empty() +
					omp::Hand(Card::RankSuitToCardIndex(rankMainPairCheck, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(rankMainPairCheck, 1)) +
					omp::Hand(Card::RankSuitToCardIndex(0, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(0, 1))
				);

				if (testEval > evaluation)
				{
					break;
				}

				rankMainPair = rankMainPairCheck;
			}

			for (uint8_t rankKickerPairCheck = 0; rankKickerPairCheck < omp::RANK_COUNT; rankKickerPairCheck++)
			{
				if (rankKickerPairCheck != rankMainPair)
				{
					uint16_t testEval = ev.evaluate(
						omp::Hand::empty() +
						omp::Hand(Card::RankSuitToCardIndex(rankMainPair, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(rankMainPair, 1)) +
						omp::Hand(Card::RankSuitToCardIndex(rankKickerPairCheck, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(rankKickerPairCheck, 1))
					);

					if (testEval > evaluation)
					{
						break;
					}

					rankKickerPair = rankKickerPairCheck;
				}
			}

			return "two pair, " + rankToStringPlural[rankMainPair] + " and " + rankToStringPlural[rankKickerPair];
		}
		case 4: //Three of a kind
		{
			//suit doesn't matter
			uint8_t rank = 0;
			for (uint8_t rankCheck = 0; rankCheck < omp::RANK_COUNT; rankCheck++)
			{
				uint16_t testEval = ev.evaluate(
					omp::Hand::empty() +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 1)) +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 2))
				);

				if (testEval > evaluation)
				{
					break;
				}

				rank = rankCheck;
			}

			return "three of a kind, " + rankToStringPlural[rank];
		}
		case 5: //Straight
		{
			//suit doesn't matter
			uint8_t straight = 0;
			for (uint8_t straightCheck = 0; straightCheck < 10; straightCheck++)
			{
				//build straight for rank
				omp::Hand straightHand = omp::Hand::empty();

				if (straightCheck == 0)
				{
					straightHand += omp::Hand(Card::RankSuitToCardIndex(12, 1));

					for (int i = 0; i < 4; i++)
					{
						straightHand += omp::Hand(Card::RankSuitToCardIndex(straightCheck + i, i % 4));
					}
				}
				else
				{
					for (int i = 0; i < 5; i++)
					{
						straightHand += omp::Hand(Card::RankSuitToCardIndex((straightCheck - 1) + i, i % 4));
					}
				}

				uint16_t testEval = ev.evaluate(straightHand);

				if (testEval > evaluation)
				{
					break;
				}

				straight = straightCheck;
			}

			return rankToString[straight + 3] + "-high straight";
		}
		case 6: //Flush 
		{
			//there is no way to have a flush lower than 7?
			//since it would be a forced straight flush (23456)

			//suit doesn't matter
			uint8_t rank = 0;
			for (uint8_t rankCheck = 5; rankCheck < omp::RANK_COUNT; rankCheck++)
			{
				omp::Hand hand = omp::Hand::empty();

				if (rankCheck < 12)
				{
					hand += omp::Hand(Card::RankSuitToCardIndex(rankCheck, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(0, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(1, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(2, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(3, 0));
				}
				else
				{
					//avoid straight flush
					hand += omp::Hand(Card::RankSuitToCardIndex(rankCheck, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(0, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(1, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(2, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(4, 0));
				}

				uint16_t testEval = ev.evaluate(hand);

				if (testEval > evaluation)
				{
					break;
				}

				rank = rankCheck;
			}

			return rankToString[rank] + "-high flush";
		}
		case 7: //Full House
		{
			//suit doesn't matter
			uint8_t rankMainPair = 0;
			uint8_t rankKickerPair = 0;

			//find main pair then kicker pair

			//start one rank offset ... (checking against twos already)
			for (uint8_t rankMainPairCheck = 1; rankMainPairCheck < omp::RANK_COUNT; rankMainPairCheck++)
			{
				uint16_t testEval = ev.evaluate(
					omp::Hand::empty() +
					omp::Hand(Card::RankSuitToCardIndex(rankMainPairCheck, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(rankMainPairCheck, 1)) +
					omp::Hand(Card::RankSuitToCardIndex(rankMainPairCheck, 2)) +
					omp::Hand(Card::RankSuitToCardIndex(0, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(0, 1))
				);

				if (testEval > evaluation)
				{
					break;
				}

				rankMainPair = rankMainPairCheck;
			}

			for (uint8_t rankKickerPairCheck = 0; rankKickerPairCheck < omp::RANK_COUNT; rankKickerPairCheck++)
			{
				if (rankKickerPairCheck != rankMainPair)
				{
					uint16_t testEval = ev.evaluate(
						omp::Hand::empty() +
						omp::Hand(Card::RankSuitToCardIndex(rankMainPair, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(rankMainPair, 1)) +
						omp::Hand(Card::RankSuitToCardIndex(rankMainPair, 2)) +
						omp::Hand(Card::RankSuitToCardIndex(rankKickerPairCheck, 0)) +
						omp::Hand(Card::RankSuitToCardIndex(rankKickerPairCheck, 1))
					);

					if (testEval > evaluation)
					{
						break;
					}

					rankKickerPair = rankKickerPairCheck;
				}
			}

			return "full house, " + rankToStringPlural[rankMainPair] +
				" full of " + rankToStringPlural[rankKickerPair];
		}
		case 8: //Four of a kind
		{
			//suit doesn't matter
			uint8_t rank = 0;
			for (uint8_t rankCheck = 0; rankCheck < omp::RANK_COUNT; rankCheck++)
			{
				uint16_t testEval = ev.evaluate(
					omp::Hand::empty() +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 0)) +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 1)) +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 2)) +
					omp::Hand(Card::RankSuitToCardIndex(rankCheck, 3))
				);

				if (testEval > evaluation)
				{
					break;
				}

				rank = rankCheck;
			}

			return "four of a kind, " + rankToStringPlural[rank];
		}
		case 9: //Straight Flush
		{
			//suit doesn't matter
			uint8_t straight = 0;
			for (uint8_t straightCheck = 0; straightCheck < 10; straightCheck++)
			{
				//build straight for rank
				omp::Hand straightHand = omp::Hand::empty();

				if (straightCheck == 0)
				{
					straightHand += omp::Hand(Card::RankSuitToCardIndex(12, 0));

					for (int i = 0; i < 4; i++)
					{
						straightHand += omp::Hand(Card::RankSuitToCardIndex(straightCheck + i, 0));
					}
				}
				else
				{
					for (int i = 0; i < 5; i++)
					{
						straightHand += omp::Hand(Card::RankSuitToCardIndex((straightCheck - 1) + i, 0));
					}
				}

				uint16_t testEval = ev.evaluate(straightHand);

				if (testEval > evaluation)
				{
					break;
				}

				straight = straightCheck;
			}

			if (straight == 9)
			{
				return "royal flush";
			}

			return rankToString[straight + 3] + "-high straight flush";
		}
		default:
		{
			break;
		}
		}

		return "";
	}
} // namespace pokerlib 