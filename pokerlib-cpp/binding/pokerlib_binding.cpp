#include <napi.h>
#include "pokerlib/PokerLib.h"
#include "pokerlib/Card.h"
#include "pokerlib/Deck.h"
#include "pokerlib/Evaluator.h"
#include "pokerlib/HandDescription.h"

// Utility function to convert JS array to vector of Cards
std::vector<pokerlib::Card> JsArrayToCards(const Napi::Array& jsArray) {
    std::vector<pokerlib::Card> cards;
    for (uint32_t i = 0; i < jsArray.Length(); i++) {
        Napi::Value val = jsArray[i];
        if (val.IsString()) {
            std::string cardStr = val.As<Napi::String>().Utf8Value();
            if (cardStr.length() >= 2) {
                // Use FromCharRankSuit to create a Card from rank and suit chars
                auto card = pokerlib::Card::FromCharRankSuit(cardStr[0], cardStr[1]);
                cards.push_back(card);
            }
        }
    }
    return cards;
}

// Utility function to convert string of cards to vector of Cards
std::vector<pokerlib::Card> StringToCards(const std::string& cardsStr) {
    return pokerlib::Card::GetCards(cardsStr);
}

// Convert a Card to a JavaScript object
Napi::Object CardToJsObject(Napi::Env env, const pokerlib::Card& card) {
    auto obj = Napi::Object::New(env);
    obj.Set("rank", card.rank);
    obj.Set("suit", card.suit);
    obj.Set("string", Napi::String::New(env, card.to_string()));
    return obj;
}

// Convert vector of Cards to JavaScript array
Napi::Array CardsToJsArray(Napi::Env env, const std::vector<pokerlib::Card>& cards) {
    auto result = Napi::Array::New(env, cards.size());
    for (size_t i = 0; i < cards.size(); i++) {
        result[i] = CardToJsObject(env, cards[i]);
    }
    return result;
}

// HandDescription to JavaScript object
Napi::Object HandDescriptionToJsObject(Napi::Env env, const pokerlib::HandDescription& desc) {
    // Create a non-const copy that we can call to_string on
    pokerlib::HandDescription descCopy = desc;
    
    auto obj = Napi::Object::New(env);
    obj.Set("category", static_cast<int>(desc.GetCategory()));
    
    // Convert HandCategory enum to string
    std::string categoryName;
    switch (desc.GetCategory()) {
        case pokerlib::HandCategory::HighCard: categoryName = "High Card"; break;
        case pokerlib::HandCategory::Pair: categoryName = "Pair"; break;
        case pokerlib::HandCategory::TwoPair: categoryName = "Two Pair"; break;
        case pokerlib::HandCategory::ThreeOfAKind: categoryName = "Three of a Kind"; break;
        case pokerlib::HandCategory::Straight: categoryName = "Straight"; break;
        case pokerlib::HandCategory::Flush: categoryName = "Flush"; break;
        case pokerlib::HandCategory::FullHouse: categoryName = "Full House"; break;
        case pokerlib::HandCategory::FourOfAKind: categoryName = "Four of a Kind"; break;
        case pokerlib::HandCategory::StraightFlush: categoryName = "Straight Flush"; break;
        default: categoryName = "Unknown"; break;
    }
    
    obj.Set("categoryName", Napi::String::New(env, categoryName));
    obj.Set("description", Napi::String::New(env, descCopy.to_string()));
    return obj;
}

// PlayerResults to JavaScript object
Napi::Object PlayerResultsToJsObject(Napi::Env env, const pokerlib::PlayerResults& results) {
    auto obj = Napi::Object::New(env);
    obj.Set("winPercentage", results.winPercentage);
    obj.Set("tiePercentage", results.tiePercentage);
    obj.Set("equityPercentage", results.equityPercentage);
    
    // Convert WinStatus enum to string
    std::string statusStr = "Unknown";
    if (results.winStatus == pokerlib::WinStatus::Ahead) {
        statusStr = "Ahead";
    } else if (results.winStatus == pokerlib::WinStatus::Tied) {
        statusStr = "Tied";
    } else if (results.winStatus == pokerlib::WinStatus::Behind) {
        statusStr = "Behind";
    }
    obj.Set("winStatus", Napi::String::New(env, statusStr));
    
    // Convert winning outs
    auto winningOuts = Napi::Array::New(env, results.immediateOutsToWin.size());
    for (size_t i = 0; i < results.immediateOutsToWin.size(); i++) {
        winningOuts[i] = CardToJsObject(env, results.immediateOutsToWin[i]);
    }
    obj.Set("winningOuts", winningOuts);
    
    // Convert tying outs
    auto tyingOuts = Napi::Array::New(env, results.immediateOutsToTie.size());
    for (size_t i = 0; i < results.immediateOutsToTie.size(); i++) {
        tyingOuts[i] = CardToJsObject(env, results.immediateOutsToTie[i]);
    }
    obj.Set("tyingOuts", tyingOuts);
    
    return obj;
}

// Method: getCards(cardsString)
Napi::Value GetCards(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string cardsStr = info[0].As<Napi::String>().Utf8Value();
    auto cards = StringToCards(cardsStr);
    
    return CardsToJsArray(env, cards);
}

// Method: evaluateHand(cards)
Napi::Value EvaluateHand(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || (!info[0].IsString() && !info[0].IsArray())) {
        Napi::TypeError::New(env, "String or Array expected").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::vector<pokerlib::Card> cards;
    
    if (info[0].IsString()) {
        std::string cardsStr = info[0].As<Napi::String>().Utf8Value();
        cards = StringToCards(cardsStr);
    } else if (info[0].IsArray()) {
        Napi::Array jsArray = info[0].As<Napi::Array>();
        cards = JsArrayToCards(jsArray);
    }
    
    auto hand = pokerlib::Card::CreateHand(cards);
    uint16_t eval = pokerlib::GetHandEvaluation(hand);
    
    auto result = Napi::Object::New(env);
    result.Set("value", eval);
    
    auto desc = pokerlib::HandDescriptionProvider::GetDescription(eval);
    result.Set("description", HandDescriptionToJsObject(env, desc));
    
    return result;
}

// Method: evaluate(hands, board, deadCards)
Napi::Value Evaluate(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "At least hands and board arguments expected").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    if (!info[0].IsArray()) {
        Napi::TypeError::New(env, "First argument must be an array of hands").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::vector<std::string> hands;
    Napi::Array handsArray = info[0].As<Napi::Array>();
    for (uint32_t i = 0; i < handsArray.Length(); i++) {
        Napi::Value val = handsArray[i];
        if (val.IsString()) {
            hands.push_back(val.As<Napi::String>().Utf8Value());
        }
    }
    
    std::string board = "";
    if (info[1].IsString()) {
        board = info[1].As<Napi::String>().Utf8Value();
    }
    
    std::string deadCards = "";
    if (info.Length() > 2 && info[2].IsString()) {
        deadCards = info[2].As<Napi::String>().Utf8Value();
    }
    
    auto results = pokerlib::Evaluate(hands, board, deadCards);
    
    auto jsResults = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); i++) {
        jsResults[i] = PlayerResultsToJsObject(env, results[i]);
    }
    
    return jsResults;
}

// Method: getFiveBestCards(cards)
Napi::Value GetFiveBestCards(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || (!info[0].IsString() && !info[0].IsArray())) {
        Napi::TypeError::New(env, "String or Array expected").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::vector<pokerlib::Card> cards;
    
    if (info[0].IsString()) {
        std::string cardsStr = info[0].As<Napi::String>().Utf8Value();
        cards = StringToCards(cardsStr);
    } else if (info[0].IsArray()) {
        Napi::Array jsArray = info[0].As<Napi::Array>();
        cards = JsArrayToCards(jsArray);
    }
    
    // Call the C++ GetFiveBestCards function
    auto bestCards = pokerlib::GetFiveBestCards(cards);
    
    return CardsToJsArray(env, bestCards);
}

// Method: createDeck()
Napi::Value CreateDeck(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    pokerlib::Deck deck;
    auto remainingCards = deck.GetRemainingCards();
    
    return CardsToJsArray(env, remainingCards);
}

// Register the module
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getCards", Napi::Function::New(env, GetCards));
    exports.Set("evaluateHand", Napi::Function::New(env, EvaluateHand));
    exports.Set("evaluate", Napi::Function::New(env, Evaluate));
    exports.Set("getFiveBestCards", Napi::Function::New(env, GetFiveBestCards));
    exports.Set("createDeck", Napi::Function::New(env, CreateDeck));
    return exports;
}

NODE_API_MODULE(pokerlib, Init) 