#include "PlayGameState.h"
#include "../images/Images.h"

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void PlayGameState::activate(StateMachine & machine) {

	this->counter = 0;	
  this->purse = 80; // 1000 SJH 
  this->handInPlay = FIRST_HAND;
  this->dealer_CardCount = 0;
	this->flashDetails = false;
  this->insurance = 0;

  this->currentBetInit = 0;
  this->currentBetTotal = 0;

  this->currentWin = 0;
	this->player.reset();

	this->viewState = ViewState::StartHand;


}


// ---------------------------------------------------------------------------------------------------------------------------
//  Handle state updates .. 
// ---------------------------------------------------------------------------------------------------------------------------
//
void PlayGameState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();

	switch (this->viewState) {

		case ViewState::StartHand:

			this->endOfHand = false;
			this->buttonMode = SHOW_BET_BUTTONS;
			this->flashDetails = false;
			
			this->handInPlay = FIRST_HAND;
			this->dealer_CardCount = 0;
			this->player.reset();
			this->insurance = 0;
			this->currentBetInit = 0;
			this->currentBetTotal = 0;
			this->currentWin = 0;

  		for (uint8_t x = 0; x < 52; x++) {
				
				cards[x] = 0;
				
			}
		
			for (uint8_t x = 0; x < 12; x++) {
				
				dealer[x] = 0;
				this->player.firstHand[x] = 0;
				this->player.secondHand[x] = 0;
				
			}

			this->counter = 0;
			this->viewState = ViewState::InitBet;
			this->highlightedButton = 0;

			arduboy.resetFrameCount();

		case ViewState::InitBet:

			if (justPressed & LEFT_BUTTON) 	{ this->highlightedButton = decreaseHighlightButton_BettingButtons(this->highlightedButton); }
			if (justPressed & RIGHT_BUTTON) { this->highlightedButton = increaseHighlightButton_BettingButtons(this->highlightedButton); }
			if (justPressed & A_BUTTON) 		{ 
				
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::InitBet_1: 
						this->currentBetInit = this->currentBetInit + 1;
						this->currentBetTotal = this->currentBetInit;
						player.firstHand_Bet = this->currentBetInit;
						this->purse = purse - 1;
						break;
					
					case Buttons::InitBet_5: 
						this->currentBetInit = this->currentBetInit + 5;
						this->currentBetTotal = this->currentBetInit;
						player.firstHand_Bet = this->currentBetInit;
						this->purse = purse - 5;
						break;
					
					case Buttons::InitBet_10: 
						this->currentBetInit = this->currentBetInit + 10;
						this->currentBetTotal = this->currentBetInit;
						player.firstHand_Bet = this->currentBetInit;
						this->purse = purse - 10;
						break;
					
					case Buttons::InitBet_25: 
						this->currentBetInit = this->currentBetInit + 25;
						this->currentBetTotal = this->currentBetInit;
						player.firstHand_Bet = this->currentBetInit;
						this->purse = purse - 25;
						break;
					
					case Buttons::InitBet_PlayGame: 
						this->buttonMode = SHOW_NO_BUTTONS;
						this->counter = 0;
						this->viewState = ViewState::InitDeal;
						break;

					case Buttons::InitBet_Clear:
						this->purse = purse + currentBetInit;
						this->currentBetInit = 0;
						this->currentBetTotal = 0;
						break;

					default: break;

				}

				this->highlightedButton = isValidButton_BettingButtons(this->highlightedButton);

			}

			break;

		case ViewState::InitDeal:

			if (arduboy.everyXFrames(15)) {

				switch (this->counter) {

					case 0:
					case 2:
						getCard(DEALER, FIRST_HAND);
//						dealer[0] = 9;
						dealer[1] = 0;
						break;

					case 1:
					case 3:
						getCard(PLAYER, FIRST_HAND);
						this->player.firstHand[0] = 0;
						this->player.firstHand[1] = 9;
						break;

				}

				this->counter++;

				if (this->counter == 4) {

					this->player.split = ((this->player.firstHand[0] % 13) == (this->player.firstHand[1] % 13));


					// Offer insurance ?

					if ((dealer[1] % 13) == 0 && purse >= 1) {

						viewState = ViewState::OfferInsurance;
						this->highlightedButton = 0;

					}

				}

			}

			break;

		case ViewState::OfferInsurance:

			buttonMode = SHOW_INSURANCE_BUTTONS;

			if (justPressed & LEFT_BUTTON) 	{ this->highlightedButton = decreaseHighlightButton_InsuranceButtons(this->highlightedButton); }
			if (justPressed & RIGHT_BUTTON) { this->highlightedButton = increaseHighlightButton_InsuranceButtons(this->highlightedButton); }
			if (justPressed & A_BUTTON) 		{ 
				
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::InsuranceBet_1: 
						this->insurance = this->insurance + 1;
						this->currentBetTotal = this->currentBetTotal + 1;
						this->purse = purse - 1;
						break;
					
					case Buttons::InsuranceBet_5: 
						this->insurance = this->insurance + 5;
						this->currentBetTotal = this->currentBetTotal + 5;
						this->purse = purse - 5;
						break;
					
					case Buttons::InsuranceBet_10: 
						this->insurance = this->insurance + 10;
						this->currentBetTotal = this->currentBetTotal + 10;
						this->purse = purse - 10;
						break;
					
					case Buttons::InsuranceBet_25: 
						this->insurance = this->insurance + 25;
						this->currentBetTotal = this->currentBetTotal + 25;
						this->purse = purse - 25;
						break;
					
					case Buttons::InsuranceBet_PlayGame: 
						this->buttonMode = SHOW_NO_BUTTONS;
						this->counter = 0;
						this->viewState = ViewState::PeekOnTen;
						break;

					case Buttons::InsuranceBet_Clear:
            if (this->insurance > 0) {
              this->purse = this->purse + this->insurance;
              this->insurance = 0;
              this->currentBetTotal = this->currentBetInit;
            }
            else {
              this->buttonMode = SHOW_NO_BUTTONS;
              this->counter = 0;
              this->viewState = ViewState::PeekOnTen;
            }
						break;

					default: break;

				}

				this->highlightedButton = isValidButton_InsuranceButtons(this->highlightedButton);

			}

			break;

		case ViewState::PeekOnTen:

			if (this->counter < 32) {

				switch (this->counter) {

					case 0:
						this->insuranceResult = InsuranceResult::Peeking;	
						this->highlightedButton = 0;
						break;

					case 1 ... 30:
						break;

					case 31:

            if (calculateHand(DEALER, true) == 21) {

              if (calculateHand(PLAYER, FIRST_HAND, true) == 21) {
              
                this->insuranceResult = InsuranceResult::BothHaveBlackjack;			
                this->purse = this->purse + this->currentBetTotal + (2 * this->insurance);
                this->numberOfGamesPush++;
                this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
								this->flashDetails = true;

								if (this->insurance == 0) {
	                this->winStatus = WinStatus::Push;
									this->winStatusAmount = this->currentBetInit;
								}
								else {
	                this->winStatus = WinStatus::Win;
									this->winStatusAmount = (2 * this->insurance);
								}

              }
              else {

								this->numberOfGamesLost++;
								this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
								this->flashDetails = true;
								this->winStatusAmount = this->currentBetInit - (this->insurance * 2);

								if (this->insurance == 0) {
									Serial.println("highlightLoss A");	
									highlightLoss(FIRST_HAND, -currentBetInit);
									this->insuranceResult = InsuranceResult::DealerHasBlackjack;
									// this->winStatus = WinStatus::Lose;

								}
								else {

									this->insuranceResult = InsuranceResult::DealerHasBlackjackWithInsurance;
	                this->purse = this->purse + this->winStatusAmount;

									if (this->currentBetInit - (this->insurance * 2) == 0) {
//										this->winStatus = WinStatus::Push;
										highlightPush(FIRST_HAND);
									}
									else {
									Serial.println("highlightLoss B");	
										highlightLoss(FIRST_HAND, -(this->currentBetInit - (this->insurance * 2)));
//										this->winStatus = WinStatus::Lose;
									}

								}
  
              }

            }
            else {

							if (this->insurance != 0) {
								this->insuranceResult = InsuranceResult::DealerNoBlackjack;
								// this->winStatus = WinStatus::Lose;
								// this->flashDetails = true;
								// this->winStatusAmount = this->insurance;
								Serial.println("highlightLoss C");	
								highlightLoss(FIRST_HAND, -this->insurance);
								this->currentBetTotal = this->currentBetInit;
							}

							this->buttonMode = SHOW_OK_ONLY_BUTTONS;

            }

						break;

				}

				this->counter++;

			}
			else {

				if (justPressed & LEFT_BUTTON && this->highlightedButton == 1) 	{ this->highlightedButton = 0; }
				if (justPressed & RIGHT_BUTTON && this->highlightedButton == 0) { this->highlightedButton = 1; }

				if (justPressed & A_BUTTON) {
					
					if (this->insuranceResult != InsuranceResult::DealerNoBlackjack) {

						switch (static_cast<Buttons>(this->highlightedButton)) {
						
							case Buttons::PeekOnTen_Continue:
								this->flashDetails = true;
								this->winStatus = WinStatus::None;
								this->viewState = ViewState::StartHand; 
								break;

							case Buttons::PeekOnTen_Quit:
								machine.changeState(GameStateType::TitleScreen); 
								break;

							default: break;

						}

					}
					else {

            this->buttonMode = SHOW_GAME_PLAY_BUTTONS;
						this->viewState = ViewState::PlayHand;
						this->flashDetails = false;

					}

				}

			}

			break;

		case ViewState::PlayHand:

      this->buttonMode = SHOW_GAME_PLAY_BUTTONS;

      if (justPressed & LEFT_BUTTON) 	{ this->highlightedButton = decreaseHighlightButton_GamePlayButtons(this->highlightedButton); }
			if (justPressed & RIGHT_BUTTON) { this->highlightedButton = increaseHighlightButton_GamePlayButtons(this->highlightedButton); }
			if (justPressed & A_BUTTON) 		{ 
				
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::PlayHand_Hit:   // Hit machine

            player.split = false;

            if (handInPlay == FIRST_HAND) {
                
//              drawCard(CARD_LARGE_LEFT_PLAYER_FIRSTHAND + (player_FirstHand_CardCount * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, getCard(PLAYER, handInPlay), false, true);
              getCard(PLAYER, handInPlay);
              // drawPlayerScores();

              if (calculateHand(PLAYER, FIRST_HAND, false) > 21) {
                
                bust(PLAYER, FIRST_HAND);
              
              }
              
            }
            else {

//              drawCard(CARD_LARGE_LEFT_PLAYER_SECONDHAND + (player_SecondHand_CardCount * CARD_LARGE_SPACING), CARD_LARGE_TOP_PLAYER, getCard(PLAYER, handInPlay), false, true);
              getCard(PLAYER, this->handInPlay);
              // drawPlayerScores();
              
              if (calculateHand(PLAYER, SECOND_HAND, false) > 21) {
                
                 bust(PLAYER, SECOND_HAND);
              
              }
            
            }
            break;

          case Buttons::PlayHand_Stand:
            playNextHand();
            break;

					default: break;

        }

      }

			break;

    case ViewState::PlayDealerHand:

			this->handInPlay = DEALER_HAND;


			if (arduboy.everyXFrames(15)) {

				if (calculateHand(DEALER, false) <= 16) {
					
					getCard(DEALER, FIRST_HAND);

				}
				else {

					this->viewState = ViewState::CheckForWins;
					this->counter = 0;

				}

			}

      break;

		case ViewState::CheckForWins:

			// Are there two player hands ?
			
			if (player.secondHand_CardCount > 0) {

				if (!player.firstHand_Bust && !player.secondHand_Bust) {
					
					switch (this->counter) {

						case 0 ... 30: break;

						case 31:
							
							if (calculateHand(PLAYER, FIRST_HAND, true) > calculateHand(DEALER, true) || calculateHand(DEALER, true) > 21) {

								if (isBlackjack(PLAYER, FIRST_HAND)) {
									Serial.println("highlightWin A");	
									highlightWin(FIRST_HAND, player.firstHand_Bet * 3 / 2, player.firstHand_Bet * 5 / 2);

								}
								else {        
								
									Serial.println("highlightWin B");	
									highlightWin(FIRST_HAND, player.firstHand_Bet, player.firstHand_Bet * 2);
									
								}
								
							}
							else {

								Serial.println("highlightLoss D");	
								highlightLoss(FIRST_HAND, -player.firstHand_Bet);
								
							}

							break;

						case 32 ... 62: break;

						case 63:
						
							if (calculateHand(PLAYER, SECOND_HAND, true) > calculateHand(DEALER, true) || calculateHand(DEALER, true) > 21) {

								if (isBlackjack(PLAYER, SECOND_HAND)) {

									Serial.println("highlightWin C");	
									highlightWin(SECOND_HAND, player.secondHand_Bet * 3 / 2, player.secondHand_Bet * 5 / 2);

								}
								else {        
								
									Serial.println("highlightWin D");	
									highlightWin(SECOND_HAND, player.secondHand_Bet, player.secondHand_Bet * 2);
									
								}
								
							}
							else {
								
								if (isBlackjack(PLAYER, SECOND_HAND)) {

									Serial.println("highlightWin E");	
									highlightWin(SECOND_HAND, player.secondHand_Bet * 3 / 2, player.secondHand_Bet * 5 / 2);

								}
								else {        
								
									Serial.println("highlightLoss E");	
									highlightLoss(SECOND_HAND, -player.secondHand_Bet);
									
								}
								
							}

							break;

						case 64 ... 94: break;

						case 95:
		
							this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
							this->viewState = ViewState::EndOfGame;
							break;


					}
					
				}
						
				if (!player.firstHand_Bust && player.secondHand_Bust) {

					switch (this->counter) {

						case 0 ... 30: break;

						case 31:

							if (calculateHand(PLAYER, FIRST_HAND, true) > calculateHand(DEALER, true) || calculateHand(DEALER, true) > 21) {

								if (isBlackjack(PLAYER, FIRST_HAND)) {
									Serial.println("highlightWin F");	
									highlightWin(FIRST_HAND, player.firstHand_Bet * 3 / 2, player.firstHand_Bet * 5 / 2);

								}
								else {        
									Serial.println("highlightWin G");									
									highlightWin(FIRST_HAND, player.firstHand_Bet, player.firstHand_Bet * 2);
									
								}        
								
							}

							break;

						case 32 ... 62: break;

						case 63:
		
							this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
							this->viewState = ViewState::EndOfGame;
							break;

					}

				}
						
				if (player.firstHand_Bust && !player.secondHand_Bust) {

					switch (this->counter) {

						case 0 ... 30: break;

						case 31:					

							if (calculateHand(PLAYER, SECOND_HAND, true) > calculateHand(DEALER, true) || calculateHand(DEALER, true) > 21) {
				
								if (isBlackjack(PLAYER, SECOND_HAND)) {
									Serial.println("highlightWin H");	
									highlightWin(SECOND_HAND, player.secondHand_Bet * 3 / 2, player.secondHand_Bet * 5 / 2);

								}
								else {        
									Serial.println("highlightWin I");									
									highlightWin(SECOND_HAND, player.secondHand_Bet, player.secondHand_Bet * 2);
									
								}
								
							}
							else {
								
								if (isBlackjack(PLAYER, SECOND_HAND)) {
									Serial.println("highlightWin J");	
									highlightWin(SECOND_HAND, player.secondHand_Bet * 3 / 2, player.secondHand_Bet * 5 / 2);

								}
								else {        
									Serial.println("highlightLoss F");	
									highlightLoss(SECOND_HAND, -player.secondHand_Bet);
									
								}
								
							}

							break;

						case 32 ... 62: break;

						case 63:
		
							this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
							this->viewState = ViewState::EndOfGame;
							break;

					}

				}
				
			}
			
			
			// Only one hand in play ..

			else {

				switch (this->counter) {

					case 0 ... 30: break;

					case 31:	
												
						if (calculateHand(PLAYER, FIRST_HAND, true) > calculateHand(DEALER, true) || calculateHand(DEALER, true) > 21) {
				
							if (isBlackjack(PLAYER, FIRST_HAND)) {
									Serial.println("highlightWin K");	
								highlightWin(FIRST_HAND, player.firstHand_Bet * 3 / 2, player.firstHand_Bet * 5 / 2);

							}
							else {        
									Serial.println("highlightWin L");									
								highlightWin(FIRST_HAND, player.firstHand_Bet, player.firstHand_Bet * 2);
									
							} 
								
						}
						else {
								
							if (isBlackjack(PLAYER, FIRST_HAND)) {
									Serial.println("highlightWin M");	
								highlightWin(FIRST_HAND, player.firstHand_Bet * 3 / 2, player.firstHand_Bet * 5 / 2);

							}
							else {        
								Serial.println("highlightLoss G");	
								highlightLoss(FIRST_HAND, -player.firstHand_Bet);
								
							}
								
						}

						break;

					case 32 ... 62: break;

					case 63:
	
						this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
						this->viewState = ViewState::EndOfGame;
						break;
					
				}

			}
			
			this->counter++;
			break;

    case ViewState::EndOfGame:

      if (justPressed & LEFT_BUTTON && this->highlightedButton == 1) 	{ this->highlightedButton = 0; }
			if (justPressed & RIGHT_BUTTON && this->highlightedButton == 0) { this->highlightedButton = 1; }
			if (justPressed & A_BUTTON) 		{ 
					
				switch (static_cast<Buttons>(this->highlightedButton)) {
					
					case Buttons::EndOfGame_Continue:
						this->viewState = ViewState::StartHand; 
						this->counter = 0;
						break;

					case Buttons::EndOfGame_Quit:
						machine.changeState(GameStateType::TitleScreen); 
						break;

					default: break;
				
				}

			}

      break;

    default: break;

	}

}

void PlayGameState::highlightWin(uint8_t hand, int16_t win, int16_t purseInc) {

	Serial.print("highlightWin: ");
	Serial.print(hand);
	Serial.print(", ");
	Serial.print(win);
	Serial.print(", ");
	Serial.println(purseInc);

}

void PlayGameState::highlightLoss(uint8_t hand, int16_t loss) {

	Serial.print("highlightLoss: ");
	Serial.print(hand);
	Serial.print(", ");
	Serial.println(loss);

}

void PlayGameState::highlightPush(uint8_t hand) {

	Serial.print("highlightPush: ");
	Serial.println(hand);

}

void PlayGameState::bust(uint8_t playerNo, uint8_t hand) {
  
  numberOfGamesLost++;  
  
  if (playerNo == PLAYER && hand == FIRST_HAND) {

    player.firstHand_Bust = true;
    this->winStatus = WinStatus::Lose;
    this->winStatusAmount = this->player.firstHand_Bet;
    // drawPlayerScores();
    
    // highlightLoss(-player.firstHand_Bet);
    playNextHand();
  
  }
  
  if (playerNo == PLAYER && hand == SECOND_HAND) {
 
    player.secondHand_Bust = true;
    this->winStatus = WinStatus::Lose;
    this->winStatusAmount = this->player.secondHand_Bet;
    // drawPlayerScores();
    
    // highlightLoss(-player.secondHand_Bet);
    playNextHand();
    
  }
  
}

void PlayGameState::playNextHand() {
Serial.print("PlayNextHand: ");
Serial.println(player.firstHand_Bust);

  player.split = false;
  
  if (this->handInPlay == FIRST_HAND) {

    player.firstHand_Stand = true;
    
    if (player.secondHand_CardCount > 0) {
      this->handInPlay = SECOND_HAND;
      // playSecondHand();
      
    }
    else {

      if (!player.firstHand_Bust) {

        this->viewState = ViewState::PlayDealerHand;

        // playDealerHand();
        
      }
      else {
       
        // showDealerCards();
        this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
        this->viewState = ViewState::EndOfGame;
        // drawButtons(); 
Serial.println(player.firstHand_Bust);        
      }
      
    }
    
  }
  else {

    if (!player.firstHand_Bust || !player.secondHand_Bust) {

      player.secondHand_Stand = true;
      this->viewState = ViewState::PlayDealerHand;
      // playDealerHand();
        
    }
    else {
       
      // showDealerCards();
      this->buttonMode = SHOW_END_OF_GAME_BUTTONS;
      this->viewState = ViewState::EndOfGame;
      // drawButtons(); 
        
    }
    
  }
  
}


// ---------------------------------------------------------------------------------------------------------------------------
//  Render the state .. 
// ---------------------------------------------------------------------------------------------------------------------------
//
void PlayGameState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  //ardBitmap.drawCompressed(0, 51, Images::Background, WHITE, ALIGN_NONE, MIRROR_NONE); 

	switch (this->viewState) {

		case ViewState::InitBet:

			font3x5.setCursor(0, 22);
			font3x5.print(F("Initial~bet? "));
			arduboy.fillRect(49, 22, 13, 7);
			font3x5.setTextColor(BLACK);
			render3DigitNumber(this->currentBetInit);
			font3x5.setTextColor(WHITE);
			break;

		case ViewState::OfferInsurance:
			drawFirstHand(machine);
			drawDealerHand(machine, true);

			font3x5.setCursor(0, 22);
			font3x5.print(F("Insurance? "));
			arduboy.fillRect(43, 22, 13, 7);
			font3x5.setTextColor(BLACK);
			render3DigitNumber(this->insurance);
			font3x5.setTextColor(WHITE);

			break;

		case ViewState::InitDeal:
			drawFirstHand(machine);
			drawDealerHand(machine, true);
			break;

		case ViewState::PeekOnTen:

			drawFirstHand(machine);
			font3x5.setCursor(0, 22);

			switch (this->insuranceResult) {
				
				case InsuranceResult::Peeking:
					font3x5.print(F("Dealer~is~peeking~"));
					for (uint8_t x = 0; x < (this->counter / 8); x++) {
						font3x5.print(F("."));
					}
					drawDealerHand(machine, true);
					break;
				
				case InsuranceResult::BothHaveBlackjack:
					font3x5.print(F("Two~blackjacks!"));
					drawDealerHand(machine, false);
					break;
				
				case InsuranceResult::DealerHasBlackjack:
					font3x5.print(F("Dealer~has~Blackjack!"));
					drawDealerHand(machine, false);
					break;

				case InsuranceResult::DealerNoBlackjack:
					font3x5.print(F("Dealer~has~nothing!"));
					drawDealerHand(machine, true);
					break;

				case InsuranceResult::DealerHasBlackjackWithInsurance:
					font3x5.print(F("Insured Blackjack!"));
					drawDealerHand(machine, false);
					break;
			}
			break;

      case ViewState::PlayHand:
        if (handInPlay != DEALER_HAND) {
          drawFirstHand(machine);
          drawDealerHand(machine, true);
        }
        break;

      case ViewState::PlayDealerHand:
				drawFirstHand(machine);
				drawDealerHand(machine, false);
        break;

      case ViewState::CheckForWins:
				drawFirstHand(machine);
				drawDealerHand(machine, false);
        break;

      case ViewState::EndOfGame:
				font3x5.setCursor(0, 22);
			  drawFirstHand(machine);
        drawDealerHand(machine, false);

				if (player.firstHand_Bust) {
					font3x5.print(F("Bust!"));
				}

        break;

      default: break;
	}


	drawButtons(machine);
  drawStats(machine, this->flashDetails, this->winStatus, this->winStatusAmount);

}

void PlayGameState::drawStats(StateMachine & machine, bool flashDetails, WinStatus winStatus, int16_t winAmount) {

	auto & arduboy = machine.getContext().arduboy;
	bool flash = arduboy.getFrameCountHalf(FLASH_DELAY);

	font3x5.setCursor(76, 0);
	font3x5.setHeight(7);

  font3x5.setTextColor(WHITE);
	font3x5.print(F("  ~Purse:~"));

  if (flash && flashDetails) {
    arduboy.fillRect(111, 0, 17, 7);
    font3x5.setTextColor(BLACK);
  }
  render4DigitNumber(this->purse);
  font3x5.setTextColor(WHITE);

	font3x5.print(F("\nInit~Bet:~ "));
	render3DigitNumber(this->currentBetInit);

	font3x5.print(F("\n Tot~Bet:~ "));
	render3DigitNumber(this->currentBetTotal);

  if (winStatus != WinStatus::None) {

    switch (winStatus) {

      case WinStatus::Win:
        font3x5.print(F("\n    ~Win:~ "));
        break;

      case WinStatus::Lose:
        font3x5.print(F("\n   ~Lose:~ "));
        break;

      case WinStatus::Push:
        font3x5.print(F("\n   ~Push:~ "));
        break;

			default: break;

    }

    if (flash && flashDetails) {
      arduboy.fillRect(115, 21, 13, 7, WHITE);
      font3x5.setTextColor(BLACK);
    }
    render3DigitNumber(winAmount);

  }

  font3x5.setTextColor(WHITE);

}