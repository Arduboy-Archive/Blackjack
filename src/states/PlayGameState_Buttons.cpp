#include "PlayGameState.h"
#include "../images/Images.h"

uint8_t PlayGameState::isValidButton_BettingButtons(uint8_t highlightedButton) {

	if (highlightedButton >= 4 && currentBetInit > 0) return highlightedButton;
	if (highlightedButton == 3 && currentBetInit <= 175 && purse >= 25) return highlightedButton;
	if (highlightedButton == 2 && currentBetInit <= 190 && purse >= 10) return highlightedButton;
	if (highlightedButton == 1 && currentBetInit <= 195 && purse >= 5) return highlightedButton;
	if (highlightedButton == 0 && currentBetInit <= 199 && purse >= 1) return highlightedButton;

	uint8_t newIndex = decreaseHighlightButton_BettingButtons(highlightedButton);

	if (newIndex != highlightedButton) {

		return newIndex;

	}
	else {

		return 4; 

	}

	return NO_VALID_BUTTON;

}

uint8_t PlayGameState::decreaseHighlightButton_BettingButtons(uint8_t highlightedButton) {

	if (highlightedButton >= 5 && currentBetInit > 0) return 4;
	if (highlightedButton >= 4 && currentBetInit <= 175 && purse >= 25) return 3;
	if (highlightedButton >= 3 && currentBetInit <= 190 && purse >= 10) return 2;
	if (highlightedButton >= 2 && currentBetInit <= 195 && purse >= 5) return 1;
	if (highlightedButton >= 1 && currentBetInit <= 199 && purse >= 1) return 0;

	return highlightedButton;

}

uint8_t PlayGameState::increaseHighlightButton_BettingButtons(uint8_t highlightedButton) {

	if (highlightedButton <= 0 && currentBetInit <= 195 && purse >= 5) return 1;
	if (highlightedButton <= 1 && currentBetInit <= 190 && purse >= 10) return 2;
	if (highlightedButton <= 2 && currentBetInit <= 175 && purse >= 25) return 3;
	if (highlightedButton <= 3 && currentBetInit > 0) return 4;
	if (highlightedButton <= 4 && currentBetInit > 0) return 5;

	return highlightedButton;

}


uint8_t PlayGameState::isValidButton_InsuranceButtons(uint8_t highlightedButton) {

	uint8_t halfBet = (currentBetInit / 2);

	if (highlightedButton >= 4 && insurance > 0) return highlightedButton;
	if (highlightedButton == 3 && (insurance + 25) <= halfBet && purse >= 25) return highlightedButton;
	if (highlightedButton == 2 && (insurance + 10) <= halfBet && purse >= 10) return highlightedButton;
	if (highlightedButton == 1 && (insurance + 5) <= halfBet && purse >= 5) return highlightedButton;
	if (highlightedButton == 0 && (insurance + 1) <= halfBet && purse >= 1) return highlightedButton;

	uint8_t newIndex = decreaseHighlightButton_InsuranceButtons(highlightedButton);

	if (newIndex != highlightedButton) {

		return newIndex;

	}
	else {

		return 4; 

	}

	return NO_VALID_BUTTON;

}

uint8_t PlayGameState::decreaseHighlightButton_InsuranceButtons(uint8_t highlightedButton) {

	uint8_t halfBet = (currentBetInit / 2);

	if (highlightedButton >= 5 && insurance > 0) return 4;
	if (highlightedButton >= 4 && (insurance + 25) <= halfBet && purse >= 25) return 3;
	if (highlightedButton >= 3 && (insurance + 10) <= halfBet && purse >= 10) return 2;
	if (highlightedButton >= 2 && (insurance + 5) <= halfBet && purse >= 5) return 1;
	if (highlightedButton >= 1 && (insurance + 1) <= halfBet && purse >= 1) return 0;

	return highlightedButton;

}

uint8_t PlayGameState::increaseHighlightButton_InsuranceButtons(uint8_t highlightedButton) {

	uint8_t halfBet = (currentBetInit / 2);

	if (highlightedButton <= 0 && (insurance + 5) <= halfBet && purse >= 5) return 1;
	if (highlightedButton <= 1 && (insurance + 10) <= halfBet && purse >= 10) return 2;
	if (highlightedButton <= 2 && (insurance + 25) <= halfBet && purse >= 25) return 3;
	if (highlightedButton <= 3 && insurance > 0) return 4;
	if (highlightedButton <= 4) return 5;

	return highlightedButton;

}

uint8_t PlayGameState::isValidButton_GamePlayButtons(uint8_t highlightedButton) {

	if (highlightedButton == 3 && (currentBetInit <= purse && ((this->player.firstHand_CardCount == 2 && handInPlay == FIRST_HAND) || (this->player.secondHand_CardCount == 2 && handInPlay == SECOND_HAND)))) return highlightedButton;
	if (highlightedButton == 2 && (currentBetInit <= purse && player.split && handInPlay != DEALER_HAND)) return highlightedButton;
	if (highlightedButton == 1) return highlightedButton;
	if (highlightedButton == 0) return highlightedButton;

	return decreaseHighlightButton_GamePlayButtons(highlightedButton);

}

uint8_t PlayGameState::decreaseHighlightButton_GamePlayButtons(uint8_t highlightedButton) {

	if (highlightedButton == 3 && currentBetInit <= purse && ((this->player.firstHand_CardCount == 2 && handInPlay == FIRST_HAND) || (this->player.secondHand_CardCount == 2 && handInPlay == SECOND_HAND))) return 2;
	if (highlightedButton == 2) return 1;
	if (highlightedButton == 1) return 0;

	return highlightedButton;

}

uint8_t PlayGameState::increaseHighlightButton_GamePlayButtons(uint8_t highlightedButton) {

	if (highlightedButton <= 0) return 1;
	if (highlightedButton <= 1 && (currentBetInit <= purse && ((this->player.firstHand_CardCount == 2 && handInPlay == FIRST_HAND) || (this->player.secondHand_CardCount == 2 && handInPlay == SECOND_HAND)))) return 2;
	if (highlightedButton <= 2 && (currentBetInit <= purse && player.split && handInPlay != DEALER_HAND)) return 3;

	return highlightedButton;

}
