# Flight Management System

A Flight Management System for managing flights, cities, and reservations. Features include displaying flight data, finding the shortest path between cities using Dijkstra’s algorithm, finding reachable cities with BFS, adding and deleting reservations, and visualizing flight routes between cities.

## Features:
- **Display All Flights**: Lists all flights with details like flight number, cities, and timings.
- **Display All Cities**: Shows all cities with at least one flight.
- **Display Departures for a City**: Displays all flights departing from a specified city.
- **Display Arrivals for a City**: Lists all flights arriving at a specified city.
- **Find Shortest Path**: Computes the shortest path between two cities based on flight travel time using Dijkstra’s Algorithm.
- **Find Reachable Cities**: Uses BFS to find all cities reachable from a specified city.
- **Add Reservation**: Adds a new reservation and updates flight passenger counts.
- **Print Reservations**: Displays all passenger reservations.
- **Print Passengers of a Flight**: Lists all passengers on a specific flight.
- **Delete Reservation**: Removes a passenger’s reservation and updates the flight's passenger count.
- **Show Graph Connections**: Displays the flight routes between cities, visualizing direct connections.

## Installation:
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/flight-management-system.git

2. Compile the code using a C++ compiler:
g++ -o FlightManagementSystem main.cpp

3. Run the program:
./FlightManagementSystem

Usage:
Run the program after compilation.
Use the system’s menu to interact with different features like viewing flights, cities, making reservations, or finding the shortest paths.
Select options to perform desired operations such as adding reservations or viewing available flights.
Contributing:
Fork the repository.
Create a new branch (git checkout -b feature-name).
Commit your changes (git commit -am 'Add new feature').
Push to your branch (git push origin feature-name).



