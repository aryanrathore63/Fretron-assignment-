#include <iostream>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

using Point = std::pair<int, int>;
using FlightPath = std::vector<Point>;

std::vector<FlightPath> generate_flight_paths(const std::vector<FlightPath>& flights) {
    std::vector<FlightPath> paths;
    for (const auto& flight : flights) {
        FlightPath path;
        path.push_back({1, 1});  // All flights start at (1,1)
        
        if (flight[1].first > 1) {
            path.push_back({2, 1});  // Move right first
        } else if (flight[1].second > 1) {
            path.push_back({1, 2});  // Move up first
        }
        
        path.insert(path.end(), flight.begin() + 1, flight.end());
        paths.push_back(path);
    }
    return paths;
}

void draw_flight_paths(const std::vector<FlightPath>& paths) {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Flight Paths");
    
    // Scale factor to fit the paths in the window
    const float scale = 100.0f;
    const float offset = 100.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw grid
        for (int i = 0; i <= 8; ++i) {
            sf::Vertex lineH[] = {
                sf::Vertex(sf::Vector2f(0, i * scale + offset), sf::Color::Cyan),
                sf::Vertex(sf::Vector2f(800, i * scale + offset), sf::Color::Cyan)
            };
            sf::Vertex lineV[] = {
                sf::Vertex(sf::Vector2f(i * scale + offset, 0), sf::Color::Cyan),
                sf::Vertex(sf::Vector2f(i * scale + offset, 800), sf::Color::Cyan)
            };
            window.draw(lineH, 2, sf::Lines);
            window.draw(lineV, 2, sf::Lines);
        }

        // Colors for each flight path
        const sf::Color colors[] = {sf::Color::Blue, sf::Color::Red, sf::Color::Yellow};

        for (size_t i = 0; i < paths.size(); ++i) {
            sf::VertexArray lines(sf::LinesStrip, paths[i].size());
            for (size_t j = 0; j < paths[i].size(); ++j) {
                lines[j].position = sf::Vector2f(paths[i][j].first * scale + offset, 
                                                 (6 - paths[i][j].second) * scale + offset);
                lines[j].color = colors[i];
            }
            window.draw(lines);

            // Draw points
            for (const auto& point : paths[i]) {
                sf::CircleShape circle(5);
                circle.setFillColor(colors[i]);
                circle.setPosition(point.first * scale + offset - 5, 
                                   (6 - point.second) * scale + offset - 5);
                window.draw(circle);
            }
        }

        window.display();
    }
}

int main() {
    std::vector<FlightPath> flights = {
        {{1, 1}, {2, 2}, {3, 3}},
        {{1, 1}, {2, 4}, {3, 2}},
        {{1, 1}, {4, 2}, {3, 4}}
    };

    auto paths = generate_flight_paths(flights);
    draw_flight_paths(paths);

    return 0;
}