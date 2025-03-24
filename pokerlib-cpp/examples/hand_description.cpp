#include "pokerlib/PokerLib.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Helper function to print a card vector
void printCards(const std::vector<pokerlib::Card>& cards) {
    for (const auto& card : cards) {
        std::cout << card.to_string() << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::string hand_str;
    std::string board_str;
    
    std::cout << "Hand Description Example" << std::endl;
    std::cout << "-----------------------" << std::endl;
    
    std::cout << "Enter your hole cards (e.g., AhKs): ";
    std::getline(std::cin, hand_str);
    
    if (hand_str.length() < 4) {
        std::cout << "Invalid hand format. Exiting." << std::endl;
        return 1;
    }
    
    std::cout << "Enter the board cards (e.g., AsTdQc5h2s): ";
    std::getline(std::cin, board_str);
    
    // Parse the cards
    auto hand_cards = pokerlib::Card::GetCards(hand_str);
    auto board_cards = pokerlib::Card::GetCards(board_str);
    
    // Get the combined hand
    auto combined_cards = pokerlib::Card::CombineCards(hand_cards, board_cards);
    auto combined_hand = pokerlib::Card::CreateHand(combined_cards);
    
    // Evaluate the hand
    uint16_t eval = pokerlib::GetHandEvaluation(combined_hand);
    
    // Get the hand description
    auto desc = pokerlib::HandDescriptionProvider::GetDescription(eval);
    
    std::cout << std::endl << "Hand Analysis:" << std::endl;
    std::cout << "-------------" << std::endl;
    
    std::cout << "Hole cards: ";
    printCards(hand_cards);
    
    std::cout << "Board: ";
    printCards(board_cards);
    
    std::cout << "Hand evaluation: " << eval << std::endl;
    std::cout << "Hand category: " << (int)desc.category << std::endl;
    std::cout << "Hand description: " << desc.to_string() << std::endl;
    
    // Find the best 5 cards if we have more than 5
    if (combined_cards.size() > 5) {
        auto best_five = pokerlib::GetFiveBestCards(combined_cards);
        
        std::cout << "Best five cards: ";
        printCards(best_five);
    }
    
    // Compare with some standard hands
    struct HandExample {
        std::string name;
        std::string cards;
    };
    
    std::vector<HandExample> examples = {
        {"Royal Flush", "AhKhQhJhTh"},
        {"Straight Flush", "9s8s7s6s5s"},
        {"Four of a Kind", "AhAcAdAsKs"},
        {"Full House", "KhKcKdQsQc"},
        {"Flush", "AhJh9h5h2h"},
        {"Straight", "9s8h7d6c5h"},
        {"Three of a Kind", "QhQcQd8s2c"},
        {"Two Pair", "JhJc8d8s2h"},
        {"One Pair", "ThTc9s5d2c"},
        {"High Card", "AhQs9c5d2h"}
    };
    
    std::cout << std::endl << "Hand Rankings:" << std::endl;
    std::cout << "-------------" << std::endl;
    
    for (const auto& example : examples) {
        auto ex_cards = pokerlib::Card::GetCards(example.cards);
        auto ex_hand = pokerlib::Card::CreateHand(ex_cards);
        auto ex_eval = pokerlib::GetHandEvaluation(ex_hand);
        auto ex_desc = pokerlib::HandDescriptionProvider::GetDescription(ex_eval);
        
        std::cout << std::left << std::setw(20) << example.name << " - " 
                  << ex_desc.to_string() << std::endl;
        
        // Compare with the user's hand
        if (ex_eval < eval) {
            std::cout << "  (Your hand is stronger than this)" << std::endl;
        } else if (ex_eval == eval) {
            std::cout << "  (Your hand is equal to this)" << std::endl;
        } else {
            std::cout << "  (Your hand is weaker than this)" << std::endl;
        }
    }
    
    return 0;
} 