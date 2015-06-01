struct Button {
  uint8_t index;
  uint8_t menuIndex;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint8_t action;
};

struct Menu {
  uint8_t index;
  uint8_t buttons;
  uint8_t cols;
  uint8_t align;
};



//Function Prototypes - WIP
//Gets the Menu Specification out of /lang/xx/menu.txt
Menu getMenu(uint8_t index);
//Gets the Button Information out of /lang/xx/button.txt
Button getButton(Menu *, uint8_t index);
//Paints the Buttons Specified in *menuButtons
void drawButtons(Button *);
//Analyzes Display press and returns pressed Button index
Button *getPress(Button*);
//
uint8_t getAction(uint8_t menuIndex, uint8_t pressedIndex);
void call(uint8_t functionIndex);

//Initialize Menu
Menu currentMenu = {0,0,0,0};

/*Menu Construction
  Paints a dynamic Menu Screen depending on the Specifications in /lang/xx/menu.txt, button.txt and dialog.txt
*/
uint8_t buildMenu(uint8_t menuIndex){
	uint8_t action = 0;
        Button *menuButtons = NULL;
        Button *pressedButton = NULL;
	do{
	if(currentMenu.index != menuIndex){
		currentMenu = getMenu(menuIndex);
		 menuButtons = (Button*) realloc(menuButtons, currentMenu.buttons * sizeof(Button));
		for(int buttonIndex = 0; buttonIndex < currentMenu.buttons; buttonIndex++){
			menuButtons[buttonIndex] = getButton(&currentMenu, buttonIndex); 
	        }
		drawButtons(menuButtons); 
	}	
	while(!(pressedButton = getPress(menuButtons)));
	if(pressedButton->action&0x7F) call(pressedButton->action);
	}while(pressedButton->action < 0x80);
	return action;
}


