/*
    Projet Minecraft Clone
    Auteur : Mehdi Mazouz
    Date : 2024

    Ce projet a pour but de créer un moteur de jeu en C++ ainsi qu'un jeu.

    L'objectif de ce projet est de développer un moteur de jeu qui soit le plus optimisé et extensible possible.

    Le projet sera en anglais pour m'exercer.

    Ma règle est que si une classe parent exemple : Window a pour attribut une classe enfant exemple : Input, alors la classse parent peut modifier la classe enfant mais la classe enfant
    ne modifira aucun attribut de la classe parent. Il ne prendra la classe parent en const ou une copie. One way relationship.

    * Prédiction *
    Amusement : 10/10 Vraiment
    Difficulté : 10/10 C'est long et difficile
    Utilité : 10/10 énormément appris
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