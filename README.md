# Advanced-Coin-Bank-System
### by Alexander Ramos-Rodriguez & Emma Valente

This is an advanced coin bank system in which you will select the coin amount you are adding. Then after pressing a button input, the user will be able to add a coin which will be registered by a break-beam sensor which will update the display to show the new total coin amount. This can be done for each coin denomination and will keep counting until a valid code input through a keypad is inputted. If a wrong code is inputted the display will display an according message and a red LED will signify that the code is wrong. Otherwise, if a correct password is inputted a motor will open up the gate and allow for the coins to be retrieved. Once the user closes the gate through the keypad, the coin amount will restart and the coin bank system will once again be ready to keep track of coins inserted.

## Advanced Coin Bank System
| Behavior  | Test Result | Comment |
| ------------- | ------------- |--------------|
| Whe button is pressed and coin is inserted the display changes to reflect the new coin total | PASS | This happens for each button with their appropiate coin denomation|
| The correct coin amount is added to the displayed total  | PASS  |  |
| When a incorrect code is inputted, the display shows a message and the board Red LED turns on | PASS  | Press "#" twice to start a new attempt |
| When correct password inputted the gate opens, the display shows a message, and the LED turns on| PASS | |
|When the "#" key is pressed three times the gate closses and the display shows the total as zero again|PASS|||
