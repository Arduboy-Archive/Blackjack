#include "PlayGameState.h"
#include "../images/Images.h"

void PlayGameState::render4DigitNumber(uint16_t val) {
	
	uint8_t digits[4] = {};
	extractDigits(digits, static_cast<uint16_t>(absT(val)));
	
	for (uint8_t j = 4; j > 0; --j) {
		font3x5.print(digits[j - 1]);
	}

}

void PlayGameState::render3DigitNumber(uint16_t val) {
	
	uint8_t digits[3] = {};
	extractDigits(digits, static_cast<uint16_t>(absT(val)));
	
	for (uint8_t j = 3; j > 0; --j) {
		font3x5.print(digits[j - 1]);
	}

}

void PlayGameState::drawDealerHand(StateMachine & machine, bool hideDealersFirstCard) {

  for (int x = 0; x < dealer_CardCount; x++) {
			
		if (x < dealer_CardCount - 1) {

			drawDealerCard(machine, CARD_LARGE_RIGHT_DEALER - (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_DEALER, dealer[x], false, hideDealersFirstCard && x == 0);   

		}
		else {

			drawDealerCard(machine, CARD_LARGE_RIGHT_DEALER - (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_DEALER, dealer[x], true, hideDealersFirstCard && x == 0);   

		}
    
  }
    
}


void PlayGameState::drawFirstHand(StateMachine & machine) {

  for (int x = 0; x < this->player.firstHand_CardCount; x++) {
   
    if (x == this->player.firstHand_CardCount - 1 && this->player.firstHand_Double) {
  
      drawCard(machine, CARD_LARGE_LEFT_FIRST_HAND + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, this->player.firstHand[x], true, true);   
  
    }
    else {
			
			if (x < this->player.firstHand_CardCount - 1) {

	      drawCard(machine, CARD_LARGE_LEFT_FIRST_HAND + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, this->player.firstHand[x], false, false);   

			}
			else {

	      drawCard(machine, CARD_LARGE_LEFT_FIRST_HAND + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, this->player.firstHand[x], false, true);   

			}
  
    }
    
  }
    
}

void PlayGameState::drawSecondHand(StateMachine & machine) {

  for (uint8_t x = 0; x < this->player.secondHand_CardCount; x++) {
   
    if (x == this->player.firstHand_CardCount - 1 && this->player.firstHand_Double) {
  
      drawCard(machine, CARD_LARGE_LEFT_SECOND_HAND + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, this->player.secondHand[x], true, true);   
  
    }
    else {
			
			if (x < this->player.secondHand_CardCount - 1) {
  
	      drawCard(machine, CARD_LARGE_LEFT_SECOND_HAND + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, this->player.secondHand[x], false, false);   

			}
			else {
  
	      drawCard(machine, CARD_LARGE_LEFT_SECOND_HAND + (x * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, this->player.secondHand[x], false, true);   

			}
  
    }
    
  }
      
}


void PlayGameState::drawCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool rotated, bool fullSizeCard) {

	auto & ardBitmap = machine.getContext().ardBitmap;

	uint8_t cardNumber = card % 13;

	if (fullSizeCard) {

		ardBitmap.drawCompressed(xPos, yPos, Images::Card_Outline_Full, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 8, yPos + 10, Images::Suits[card / 13], WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 2, yPos + 3, Images::Pips[cardNumber], WHITE, ALIGN_NONE, MIRROR_NONE);

	}
	else if (rotated) {

		ardBitmap.drawCompressed(xPos, yPos + 6, Images::Card_Outline_Rotated_Mask, BLACK, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos, yPos + 6, Images::Card_Outline_Rotated, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 13, yPos + 16, Images::Suits_Rot[card / 13], WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 23, yPos + 10, Images::Pips_Rot[cardNumber], WHITE, ALIGN_NONE, MIRROR_NONE);

	}
	else {

		ardBitmap.drawCompressed(xPos, yPos, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 2, yPos + 10, Images::Suits[card / 13], WHITE, ALIGN_NONE, MIRROR_NONE);
		ardBitmap.drawCompressed(xPos + 2, yPos + 3, Images::Pips[cardNumber], WHITE, ALIGN_NONE, MIRROR_NONE);

	}

}


void PlayGameState::drawDealerCard(StateMachine & machine, uint8_t xPos, uint8_t yPos, uint8_t card, bool fullSizeCard, bool renderBackground) {

	auto & ardBitmap = machine.getContext().ardBitmap;

	uint8_t cardNumber = card % 13;

	if (fullSizeCard) {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING_FULL, yPos - 5, Images::Card_Outline_Full, WHITE, ALIGN_NONE, MIRROR_HOR_VER);

		if (!renderBackground) {
			ardBitmap.drawCompressed(xPos - 14, yPos + 1, Images::Suits[card / 13], WHITE, ALIGN_NONE, MIRROR_HOR_VER);
			ardBitmap.drawCompressed(xPos - 8, yPos + 8, Images::Pips[cardNumber], WHITE, ALIGN_NONE, MIRROR_HOR_VER);
		}
		else {
			ardBitmap.drawCompressed(xPos - 16, yPos, Images::Card_Background_Full, WHITE, ALIGN_NONE, MIRROR_NONE);
		}

	}
	else {

		ardBitmap.drawCompressed(xPos - CARD_LARGE_SPACING, yPos - 5, Images::Card_Outline_Half, WHITE, ALIGN_NONE, MIRROR_HOR_VER);

		if (!renderBackground) {
			ardBitmap.drawCompressed(xPos - 8, yPos + 1, Images::Suits[card / 13], WHITE, ALIGN_NONE, MIRROR_HOR_VER);
			ardBitmap.drawCompressed(xPos - 8, yPos + 8, Images::Pips[cardNumber], WHITE, ALIGN_NONE, MIRROR_HOR_VER);
		}
		else {
			ardBitmap.drawCompressed(xPos - 9, yPos, Images::Card_Background_Half, WHITE, ALIGN_NONE, MIRROR_NONE);
		}

	}

}

void PlayGameState::drawButtons(StateMachine & machine) {

	auto & ardBitmap = machine.getContext().ardBitmap;
	auto & arduboy = machine.getContext().arduboy;

	bool flash = arduboy.getFrameCountHalf(FLASH_DELAY);

	switch (this->buttonMode) {

		case SHOW_GAME_PLAY_BUTTONS:

			if (handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(0, 53, Images::Button_HitMe, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(30, 53, Images::Button_Stand, WHITE, ALIGN_NONE, MIRROR_NONE); }
      if (currentBetInit <= purse && ((this->player.firstHand_CardCount == 2 && handInPlay == FIRST_HAND) || (this->player.secondHand_CardCount == 2 && handInPlay == SECOND_HAND))) {
			  ardBitmap.drawCompressed(58, 53, Images::Button_Double, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }
      if (currentBetInit <= purse && player.split && handInPlay != DEALER_HAND) {
			  ardBitmap.drawCompressed(90, 53, Images::Button_Split, WHITE, ALIGN_NONE, MIRROR_NONE); 
      }

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(0, 53, Images::Button_30_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(30, 53, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 2:
						ardBitmap.drawCompressed(58, 53, Images::Button_32_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 3:
						ardBitmap.drawCompressed(90, 53, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;
        
        }

      }

			break;

		case SHOW_GAME_PLAY_BUTTONS_DISABLED:
    
			// lcd.setTextColour(GREY5);
			// lcd.drawRoundRect(BUTTONS_HITME_LEFT, BUTTONS_HITME_TOP, BUTTONS_HITME_LEFT + BUTTONS_HITME_WIDTH, BUTTONS_HITME_TOP + BUTTONS_HITME_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.drawRoundRect(BUTTONS_STAND_LEFT, BUTTONS_STAND_TOP, BUTTONS_STAND_LEFT + BUTTONS_STAND_WIDTH, BUTTONS_STAND_TOP + BUTTONS_STAND_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.drawRoundRect(BUTTONS_DOUBLE_LEFT, BUTTONS_DOUBLE_TOP, BUTTONS_DOUBLE_LEFT + BUTTONS_DOUBLE_WIDTH, BUTTONS_DOUBLE_TOP + BUTTONS_DOUBLE_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.drawRoundRect(BUTTONS_SPLIT_LEFT, BUTTONS_SPLIT_TOP, BUTTONS_SPLIT_LEFT + BUTTONS_SPLIT_WIDTH, BUTTONS_SPLIT_TOP + BUTTONS_SPLIT_HEIGHT, radius5, GREY5, hollowFill);
			// lcd.string(BUTTONS_HITME_LEFT + 12, BUTTONS_HITME_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Hit Me", 0);
			// lcd.string(BUTTONS_STAND_LEFT + 12, BUTTONS_STAND_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Stand", 0);
			// lcd.string(BUTTONS_DOUBLE_LEFT + 11, BUTTONS_DOUBLE_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Double", 0);
			// lcd.string(BUTTONS_SPLIT_LEFT + 18, BUTTONS_SPLIT_TOP + 9, MAX_X_PORTRAIT, MAX_Y_PORTRAIT, "Split", 0);
			break;

		case SHOW_BET_BUTTONS:

			if (currentBetInit <= 199 && purse >= 1 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(0, 53, Images::Bet_01, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (currentBetInit <= 195 && purse >= 5 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(18, 53, Images::Bet_05, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (currentBetInit <= 190 && purse >= 10 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(36, 53, Images::Bet_10, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (currentBetInit <= 175 && purse >= 25 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(54, 53, Images::Bet_25, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (currentBetInit > 0 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(76, 53, Images::Button_Play, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (currentBetInit > 0 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(100, 53, Images::Button_Clear, WHITE, ALIGN_NONE, MIRROR_NONE); }

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(0, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(18, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 2:
						ardBitmap.drawCompressed(36, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 3:
						ardBitmap.drawCompressed(54, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 4:
						ardBitmap.drawCompressed(76, 53, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 5:
						ardBitmap.drawCompressed(100, 53, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;
						
				}

			}
  
			break;

		case SHOW_INSURANCE_BUTTONS:
  
			if ((this->insurance + 1) <= (this->currentBetInit / 2) && purse >= 1 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(0, 53, Images::Bet_01, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if ((this->insurance + 5) <= (this->currentBetInit / 2) && purse >= 5 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(18, 53, Images::Bet_05, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if ((this->insurance + 10) <= (this->currentBetInit / 2) && purse >= 10 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(36, 53, Images::Bet_10, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if ((this->insurance + 25) <= (this->currentBetInit / 2) && purse >= 25 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(54, 53, Images::Bet_25, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (insurance > 0 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(76, 53, Images::Button_Play, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (insurance > 0 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(100, 53, Images::Button_Clear, WHITE, ALIGN_NONE, MIRROR_NONE); }
			if (insurance == 0 && handInPlay != DEALER_HAND) { ardBitmap.drawCompressed(104, 53, Images::Button_Skip, WHITE, ALIGN_NONE, MIRROR_NONE); }

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(0, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(18, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 2:
						ardBitmap.drawCompressed(36, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 3:
						ardBitmap.drawCompressed(54, 53, Images::Bet_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 4:
						ardBitmap.drawCompressed(76, 53, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 5:
            if (insurance != 0) {
              ardBitmap.drawCompressed(100, 53, Images::Button_28_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
            }
            else {
              ardBitmap.drawCompressed(104, 53, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
            }
						break;
						
				}

			}
    
			break;

		case SHOW_END_OF_GAME_BUTTONS:

			ardBitmap.drawCompressed(65, 53, Images::Button_Continue, WHITE, ALIGN_NONE, MIRROR_NONE); 
			ardBitmap.drawCompressed(105, 53, Images::Button_Quit, WHITE, ALIGN_NONE, MIRROR_NONE); 

			if (flash) {

				switch (this->highlightedButton) {

					case 0:
						ardBitmap.drawCompressed(65, 53, Images::Button_40_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

					case 1:
						ardBitmap.drawCompressed(105, 53, Images::Button_24_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
						break;

				}
	
			}

			break;

		case SHOW_OK_ONLY_BUTTONS:

			ardBitmap.drawCompressed(109, 53, Images::Button_OK, WHITE, ALIGN_NONE, MIRROR_NONE); 

			if (flash) {

				ardBitmap.drawCompressed(109, 53, Images::Button_20_Highlight, BLACK, ALIGN_NONE, MIRROR_NONE);
	
			}

			break;

  }
  
}
