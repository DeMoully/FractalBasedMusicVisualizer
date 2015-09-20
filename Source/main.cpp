// Written for HackISU by Zach DeMoully
// on September 18th, 2015

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Complex.hpp"
#include "Fractal.hpp"
#include "MusicInstance.hpp"
#include "SML.hpp"

int main()
{
	std::string filename;
	std::cout << "Enter the name of the playlist: ";
	std::cin >> filename;
	ash::SML inputfile("Playlists\\" + filename);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Window", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	for (auto variable : inputfile.getValueNames())
	{
		MusicInstance instance(&window, inputfile.getValue(variable, "FilePath"), inputfile.getValue(variable, "SongName"), inputfile.interpretAsColor(variable, "ColorVariance"), inputfile.interpretAsColor(variable, "BaseColor"), inputfile.interpretAsNumber<int>(variable, "BPM"), window.getSize().x, window.getSize().y);
		int result = instance.runEventLoop();
		switch (result)
		{
		case 0:
			window.close();
			return 0;
		case 1:
			continue;
		}
	}
	return 0;
}