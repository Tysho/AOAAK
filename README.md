Epic Console Battle
developped by Michael Saillard "Tysho" for Eugen Systems application

A battle simulation, turn-by-turn, opposing two heroes from 3 classes : Orc, Knight and Elf.
More hero-classes can be added for more diversity.
Each class possesses a unique skill, but more skills can be added to existing classes.

Each turn, heroes use available skills then attack, and a resume of the battle turn is generated and displayed to the user. As long as the fight is not over and closed, you can go back to review the battle screen by screen : a history is generated.

The first heroes to reach 0 hp die and the battle is over, you can then start a new one, or quit.

You can use preset hero or create a new one from scratch, but not skills : skills are determined by the hero class you'll choose.

For every interactive phase of the game, the possible actions are displayed on the bottom of the screen, guiding the user.

To edit or update the game more easily, settings files (.csv) are accessible in the /Settings folder: 
	- Language.csv contains the translation of all the different UI elements. Adding a new language is easy.
	- Heroes.csv contains preset heroes. You can add more heroes, but please use existing classes

The base to add more Hero classes, skills, effects and weapons are laid, by inheritance or simply create new instances with custom values.

I decided to not use any external library to make the code as low-level and self-sustainable as possible, apart from std and some of Window corporation classes.

All the header and cpp files are kept in the same folder : it's not mandatory as the project is not heavy at all.

I developed the code to be clean and understandable. Class Members are described in header files, methodes description are placed in .cpp.

This application is developed to be run on a window environment. No other specific requirement.
To run the project, just launch and run it with visual studio.

You can find the GitHub of this project on 
https://github.com/Tysho/Epic-Console-Battle
(It's a public project)

Have fun !
Tysho