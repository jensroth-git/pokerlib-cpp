#include "pokerlib/PokerLib.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

std::string concatHands(const std::vector<pokerlib::Card>& hand)
{
	std::string result = "";
	for (const auto& card : hand) {
		result += card.to_string();
	}
	return result;
}

void printResults(const std::vector<pokerlib::PlayerResults>& results)
{
	// Display results
	std::cout << std::endl << "Results:" << std::endl;
	std::cout << "--------" << std::endl;

	for (size_t i = 0; i < results.size(); i++) {
		std::cout << "Player " << (i + 1) << " (" << concatHands(results[i].hand) << "):" << std::endl;

		// Show current hand evaluation
		auto desc = pokerlib::HandDescriptionProvider::GetDescription(results[i].eval);
		std::cout << "  Current hand: " << desc.to_string() << std::endl;

		if (results[i].winStatus == pokerlib::WinStatus::Ahead) {
			std::cout << "  Currently AHEAD" << std::endl;
		}
		else if (results[i].winStatus == pokerlib::WinStatus::Tied) {
			std::cout << "  Currently TIED for the lead" << std::endl;
		}
		else if (results[i].winStatus == pokerlib::WinStatus::Behind) {
			std::cout << "  Currently BEHIND" << std::endl;
		}

		// Show equity percentages
		std::cout << "  Win: " << std::fixed << std::setprecision(2)
			<< (results[i].winPercentage * 100.0) << "%" << std::endl;
		std::cout << "  Tie: " << std::fixed << std::setprecision(2)
			<< (results[i].tiePercentage * 100.0) << "%" << std::endl;
		std::cout << "  Total equity: " << std::fixed << std::setprecision(2)
			<< ((results[i].equityPercentage) * 100.0) << "%" << std::endl;

		// Show winning outs
		if (!results[i].immediateOutsToWin.empty()) {
			std::cout << "  Winning outs (" << results[i].immediateOutsToWin.size() << "): ";
			for (const auto& card : results[i].immediateOutsToWin) {
				std::cout << card.to_string() << " ";
			}
			std::cout << std::endl;
		}

		// Show tying outs if not ahead/tied and not on the river
		if (!results[i].immediateOutsToTie.empty()) {
			std::cout << "  Tying outs (" << results[i].immediateOutsToTie.size() << "): ";
			for (const auto& card : results[i].immediateOutsToTie) {
				std::cout << card.to_string() << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}

int main() {

	//2 pair vs 2 pair vs higher 2 pair vs higher 2 pair 
	auto test1 = pokerlib::Evaluate({ "2h3h", "2c3c", "AhKh", "AdKd" }, "4d5s7h");

	printResults(test1);
	std::getchar();

	//lower straight vs higher straight vs 3 of a kind
	auto test2 = pokerlib::Evaluate({ "6s3s", "adjc", "8d8s" }, "7ctc9s8h");

	printResults(test2);
	std::getchar();

	//full house vs open ended straight flush draw
	auto test3 = pokerlib::Evaluate({ "6s6c", "9h8h" }, "6h7h7d");

	printResults(test3);
	std::getchar();

	//test dead cards
	auto test4 = pokerlib::Evaluate({ "ah4h", "kh5s" }, "", "jc2c5c");

	printResults(test4);
	std::getchar();

	return 0;
}