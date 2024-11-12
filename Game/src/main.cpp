/*
    Projet Minecraft Clone
    Auteur : Mehdi Mazouz
    Date : 1/10/2024

    Ce projet a pour but de créer un moteur de jeu en C++ ainsi qu'un jeu.

    * Prédiction *
    Amusement : 10/10 Vraiment
    Difficulté : 10/10 C'est long et difficile
    Utilité : 10/10 énormément appris
*/

/*

    TODO: ( 1 \n = 1 commit )

    - [ ] Ajout de plus de forme de texture

	- [ ] Refonte du Network (Client/Server)
        |
		|= [ ] Ajout de serialisation et deserialisation
		|= [ ] L'envoie de toute classe ou packet
		|= [ ] La modularité du code

    - [ ] Ajout de la classe Entité

    - [ ] Creation de la classe Timer

	- [ ] Ajout de la classe Buffer

*/

#include <iostream>
#include <limits>
#include <Gem/Networking/network_server.h>
#include "game.h"

#pragma warning(disable:4996)

int main() {
    int choice;
    std::cout << "Enter 0 to run as server, or 1 to run as client: ";

    // Read user input
    if (!(std::cin >> choice)) {
        // Handle invalid input (e.g., non-integer input)
        std::cerr << "Invalid input. Please enter 0 or 1." << std::endl;
        return 1;
    }

    if (choice == 0) {
        // Run as server
        Network::Server server(1234); // Replace 1234 with your desired port
        server.Start();

        std::cout << "Server running. Press Enter to stop." << std::endl;

        // Clear the input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get(); // Wait for the user to press Enter

        server.Stop();
    }
    else if (choice == 1) {
        // Run as client
        Game game;
        game.run();
    }
    else {
        // Handle invalid choice
        std::cerr << "Invalid choice. Please enter 0 or 1." << std::endl;
        return 1;
    }

    return 0;
}