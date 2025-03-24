"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.WinStatus = exports.HandCategory = void 0;
/**
 * Enum representing different poker hand categories
 */
var HandCategory;
(function (HandCategory) {
    HandCategory[HandCategory["HighCard"] = 1] = "HighCard";
    HandCategory[HandCategory["Pair"] = 2] = "Pair";
    HandCategory[HandCategory["TwoPair"] = 3] = "TwoPair";
    HandCategory[HandCategory["ThreeOfAKind"] = 4] = "ThreeOfAKind";
    HandCategory[HandCategory["Straight"] = 5] = "Straight";
    HandCategory[HandCategory["Flush"] = 6] = "Flush";
    HandCategory[HandCategory["FullHouse"] = 7] = "FullHouse";
    HandCategory[HandCategory["FourOfAKind"] = 8] = "FourOfAKind";
    HandCategory[HandCategory["StraightFlush"] = 9] = "StraightFlush";
})(HandCategory || (exports.HandCategory = HandCategory = {}));
/**
 * Player position relative to other players
 */
var WinStatus;
(function (WinStatus) {
    /** Player is ahead of all other players */
    WinStatus["Ahead"] = "Ahead";
    /** Player is tied with at least one other player */
    WinStatus["Tied"] = "Tied";
    /** Player is behind at least one other player */
    WinStatus["Behind"] = "Behind";
})(WinStatus || (exports.WinStatus = WinStatus = {}));
