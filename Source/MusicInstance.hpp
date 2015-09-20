// Defines a single instance of the music visualizer

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Fractal.hpp"

class MusicInstance
{
private:
	Fractal m_fractal;
	sf::RenderWindow * m_window;
	sf::Music m_song;
	sf::SoundBuffer m_buffer;
	sf::Color m_variance;
	sf::Color m_maxHue;
	int m_bpm;
public:
	// Constructors
	MusicInstance(sf::RenderWindow * window, const std::string & songname, const std::string & title, const sf::Color & variance, const sf::Color & maxHue, int bpm, unsigned int width, unsigned int height) : m_window(window), m_fractal(width, height), m_variance(variance), m_maxHue(maxHue), m_bpm(bpm)
	{
		m_window->setTitle("Now playing " + title);
		if (!m_song.openFromFile(songname))
		{
			std::cout << "Failed to load \"" + songname + "\"" << std::endl;
			m_window->close();
		}
		if (!m_buffer.loadFromFile(songname))
		{
			std::cout << "Failed to load \"" + songname + "\"" << std::endl;
			m_window->close();
		}
	}
	// Utility Functions
	int runEventLoop()
	{
		m_song.play();
		sf::Int16 maxSample = m_buffer.getSamples()[0];
		sf::Int16 minSample = m_buffer.getSamples()[0];
		Concurrency::parallel_for(0, static_cast<int>(m_buffer.getSampleCount()), [&](int i)
		{
			if (m_buffer.getSamples()[i] > maxSample)
			{
				maxSample = m_buffer.getSamples()[i];
			}
			if (m_buffer.getSamples()[i] < minSample)
			{
				minSample = m_buffer.getSamples()[i];
			}
		});
		sf::Clock clock;
		int samples = m_window->getSize().x;
		sf::VertexArray soundWave(sf::PrimitiveType::LinesStrip, samples * 2);
		while (m_window->isOpen() && m_song.getStatus() != sf::Music::Stopped)
		{
			sf::Event event;
			while (m_window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_window->close();
			}
			// Handle keyboard input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) // This will exit the visualizer
			{
				return 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // This will go to the next song
			{
				return 1;
			}
			// Modify the fractal
			int sample = static_cast<int>(m_buffer.getSampleCount() * (m_song.getPlayingOffset().asSeconds() / m_song.getDuration().asSeconds()));
			sf::Uint8 red = m_maxHue.r + std::sinf(.5f * clock.getElapsedTime().asSeconds()) * static_cast<int>(m_variance.r) * (1 - 2 * static_cast<long double>(m_buffer.getSamples()[sample] - minSample) / static_cast<long double>(maxSample - minSample));
			sf::Uint8 green = m_maxHue.g + std::sinf(.5f * clock.getElapsedTime().asSeconds()) * static_cast<int>(m_variance.g) * (1 - 2 * static_cast<long double>(m_buffer.getSamples()[sample] - minSample) / static_cast<long double>(maxSample - minSample));
			sf::Uint8 blue = m_maxHue.b + std::sinf(.5f * clock.getElapsedTime().asSeconds()) * static_cast<int>(m_variance.b) * (1 - 2 * static_cast<long double>(m_buffer.getSamples()[sample] - minSample) / static_cast<long double>(maxSample - minSample));
			m_fractal.setMaxHue(sf::Color(red, green, blue));
			m_fractal.setMinHue(sf::Color(10, 10, 10));
			m_fractal.setOffset(Complex(std::sinf((3.1415f * static_cast<float>(m_bpm) / 120.f) * clock.getElapsedTime().asSeconds()), std::sinf(clock.getElapsedTime().asSeconds()))); // Move to different views of the fractal
			// Create the central visualizer
			sf::Vector2u size = m_window->getSize();
			long double two_15 = std::pow(2.0, 15.0);
			for (int i = 0; i < samples; ++i)
			{
				long double deltaT = .075f;
				int sampleForX = min<int>(m_buffer.getSampleCount(), max<int>(0, sample + m_buffer.getSampleRate() * (-deltaT + 2 * deltaT * (static_cast<long double>(i) / static_cast<long double>(samples)))));
				soundWave[i * 2] = sf::Vertex(sf::Vector2f(size.x / 2 - (samples / 2) + i, size.y / 2 + 125 * (m_buffer.getSamples()[sampleForX]) / two_15), sf::Color(red, green, blue, 0));
				soundWave[i * 2 + 1] = sf::Vertex(sf::Vector2f(size.x / 2 - (samples / 2) + i, size.y / 2), sf::Color(0, 0, 0, 127));
			}
			m_window->clear();
			m_window->draw(m_fractal);
			m_window->draw(soundWave);
			m_window->display();
		}
		return 1; // Successfully listened to the song
	}
};