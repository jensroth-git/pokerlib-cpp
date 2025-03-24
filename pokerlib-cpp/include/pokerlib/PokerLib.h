#ifndef POKERLIB_H
#define POKERLIB_H

/**
 * @file PokerLib.h
 * @brief Main header file for the Poker Library
 *
 * This file includes all the necessary headers for using the Poker Library.
 */

#include "pokerlib/Card.h"
#include "pokerlib/Deck.h"
#include "pokerlib/Evaluator.h"
#include "pokerlib/HandDescription.h"

 /**
  * @namespace pokerlib
  * @brief Namespace containing all the Poker Library functionality
  */
namespace pokerlib {

	/**
	 * @brief Gets the version of the Poker Library
	 * @return Version string
	 */
	inline const char* version() {
		return "1.0.0";
	}

} // namespace pokerlib

#endif // POKERLIB_H 