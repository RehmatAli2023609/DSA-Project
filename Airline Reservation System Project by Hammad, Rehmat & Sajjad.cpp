//Rehmat Ali (2023609) //Sajjad Akbar Khan (2023634) // M.Hammad (2023420)
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

#define MAXCITY 30
#define MAXFLIGHT 100
#define ROUNDTRIP 0
#define ONEWAY 1
#define MINLAYOVER 30

struct flightNode {
    int flightNo;
    string departCity;
    int timeDepart;
    string arrivalCity;
    int timeArrival;
    int noOfPassengers;
    flightNode* nextDeparture;
    flightNode* nextArrival;
};

struct cityListNode {
    string cityName;
    flightNode* nextDeparture;
    flightNode* nextArrival;
};

struct flightListNode {
    int flightNo;
    flightNode* flight;
};

struct routeNode {
    int Day;
    int nHops;
    int FlightNo[10];
};

struct reservationNode {
    string firstName;
    string lastName;
    int tripType;
    routeNode routeForward;
    routeNode routeBack;
    reservationNode* nextReserve;
};

cityListNode cityList[MAXCITY];
flightListNode flightList[MAXFLIGHT];
reservationNode* reserveHead = nullptr, *reserveTail = nullptr;
int totalFlights = 0;

int Hash(const string& cityName) {
    int hash = 0;
    for (char ch : cityName)
        hash = (hash + ch) % MAXCITY;
    return hash;
}

void SortFlightList(flightNode*& head, bool isDeparture) {
    vector<flightNode*> flights;
    flightNode* current = head;
    while (current) {
        flights.push_back(current);
        current = isDeparture ? current->nextDeparture : current->nextArrival;
    }
    sort(flights.begin(), flights.end(), [isDeparture](flightNode* a, flightNode* b) {
        return (isDeparture ? a->timeDepart : b->timeArrival) < (isDeparture ? b->timeDepart : b->timeArrival);
    });

    head = nullptr;
    for (auto it = flights.rbegin(); it != flights.rend(); ++it) {
        (*it)->nextDeparture = head;
        head = *it;
    }
}

flightNode* MakeFlightNode(int flightNo, const string& departCity, int timeDepart, const string& arrivalCity, int timeArrival) {
    flightNode* newNode = new flightNode;
    newNode->flightNo = flightNo;
    newNode->departCity = departCity;
    newNode->timeDepart = timeDepart;
    newNode->arrivalCity = arrivalCity;
    newNode->timeArrival = timeArrival;
    newNode->noOfPassengers = 0;
    newNode->nextDeparture = nullptr;
    newNode->nextArrival = nullptr;
    return newNode;
}

void AddFlightToCityList(flightNode* flight) {
    int departHash = Hash(flight->departCity);
    if (cityList[departHash].cityName.empty()) {
        cityList[departHash].cityName = flight->departCity;
    }
    flight->nextDeparture = cityList[departHash].nextDeparture;
    cityList[departHash].nextDeparture = flight;
    SortFlightList(cityList[departHash].nextDeparture, true);

    int arrivalHash = Hash(flight->arrivalCity);
    if (cityList[arrivalHash].cityName.empty()) {
        cityList[arrivalHash].cityName = flight->arrivalCity;
    }
    flight->nextArrival = cityList[arrivalHash].nextArrival;
    cityList[arrivalHash].nextArrival = flight;
    SortFlightList(cityList[arrivalHash].nextArrival, false);
}

void ReadFlightData() {
    flightList[0].flight = MakeFlightNode(201, "Karachi", 900, "Lahore", 1030);
    flightList[1].flight = MakeFlightNode(203, "Karachi", 2100, "Kuwait", 2230);
    flightList[2].flight = MakeFlightNode(204, "Karachi", 1130, "Dubai", 1300);
    flightList[3].flight = MakeFlightNode(206, "Karachi", 1400, "Colombo", 1530);
    flightList[4].flight = MakeFlightNode(102, "Lahore", 1400, "Karachi", 1500);

    for (int i = 0; i < 5; ++i) {
        AddFlightToCityList(flightList[i].flight);
        ++totalFlights;
    }
}

void DisplayFlightInfo(flightNode* flight) {
    cout << "|  " << flight->flightNo << "  | " << flight->departCity << "  | " << flight->timeDepart
         << "  | " << flight->arrivalCity << "  | " << flight->timeArrival << "  |\n";
    cout << "-------------------------------------------------------------\n";
}

void DisplayAllFlightsData() {
    cout << "-------------------------------------------------------------\n";
    cout << "| FlightNo | Departure City | Time | Arrival City | Time |\n";
    cout << "-------------------------------------------------------------\n";
    for (int i = 0; i < totalFlights; ++i) {
        DisplayFlightInfo(flightList[i].flight);
    }
}

void DisplayAllCities() {
    cout << "---------------------------------\n";
    cout << "| City Name       |\n";
    cout << "---------------------------------\n";
    for (int i = 0; i < MAXCITY; ++i) {
        if (!cityList[i].cityName.empty()) {
            cout << "| " << cityList[i].cityName << string(30 - cityList[i].cityName.length(), ' ') << " |\n";
        }
    }
    cout << "---------------------------------\n";
}

void DisplayDeparturesForCity(const string& cityName) {
    system("cls");
    int hash = Hash(cityName);
    if (cityList[hash].cityName != cityName) {
        cout << "City not found.\n";
        cout << endl;
        return;
    }

    cout << "-------------------------------------------------------------\n";
    cout << "| FlightNo | Departure Time | Arrival City |\n";
    cout << "-------------------------------------------------------------\n";
    for (flightNode* current = cityList[hash].nextDeparture; current; current = current->nextDeparture) {
        cout << "|  " << current->flightNo << "   | " << current->timeDepart << "   | " << current->arrivalCity << "   |\n";
    }
    cout << "-------------------------------------------------------------\n";
}

void DisplayArrivalsForCity(const string& cityName) {
    system("cls");
    int hash = Hash(cityName);
    if (cityList[hash].cityName != cityName) {
        cout << "City not found.\n";
        return;
    }

    cout << "-------------------------------------------------\n";
    cout << "| FlightNo | Arrival Time | Departure City |\n";
    cout << "-------------------------------------------------\n";
    for (flightNode* current = cityList[hash].nextArrival; current; current = current->nextArrival) {
        cout << "| " << current->flightNo << " | " << current->timeArrival << " | " << current->departCity << " |\n";
    }
    cout << "--------------------------------------------------\n";
}

void BFSReachableCities(const string& cityName) {
    system("cls");
    vector<bool> visited(MAXCITY, false);
    queue<int> q;

    int startHash = Hash(cityName);
    if (cityList[startHash].cityName != cityName) {
        cout << "City not found.\n";
        cout << endl;
        return;
    }

    visited[startHash] = true;
    q.push(startHash);

    cout << "Cities reachable from " << cityName << ":\n---------------------------------\n";
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << cityList[current].cityName << "\n";

        for (flightNode* flight = cityList[current].nextDeparture; flight; flight = flight->nextDeparture) {
            int nextHash = Hash(flight->arrivalCity);
            if (!visited[nextHash]) {
                visited[nextHash] = true;
                q.push(nextHash);
            }
        }
    }
}

void DijkstraShortestPath(const string& startCity, const string& endCity) {
    system("cls");
    vector<int> dist(MAXCITY, INT_MAX);
    vector<int> prev(MAXCITY, -1);
    vector<bool> visited(MAXCITY, false);

    int startHash = Hash(startCity);
    int endHash = Hash(endCity);

    if (cityList[startHash].cityName != startCity || cityList[endHash].cityName != endCity) {
        cout << "City not found.\n";
        cout << endl;
        return;
    }

    dist[startHash] = 0;
    for (int i = 0; i < MAXCITY; ++i) {
        int u = -1;
        for (int j = 0; j < MAXCITY; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (dist[u] == INT_MAX)
            break;
        visited[u] = true;

        for (flightNode* flight = cityList[u].nextDeparture; flight; flight = flight->nextDeparture) {
            int v = Hash(flight->arrivalCity);
            int weight = flight->timeArrival - flight->timeDepart;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    if (dist[endHash] == INT_MAX) {
        cout << "No path found from " << startCity << " to " << endCity << ".\n";
        cout << endl;
        return;
    }

    vector<string> path;
    for (int at = endHash; at != -1; at = prev[at]) {
        path.push_back(cityList[at].cityName);
    }
    reverse(path.begin(), path.end());

    cout << "Shortest path from " << startCity << " to " << endCity << ":\n---------------------------------\n";
    for (const string& city : path) {
        cout << city << " -> ";
    }
    cout << "\nTotal travel time: " << dist[endHash] << " minutes.\n";
    cout << endl;
}

void AddReservation(const string& firstName, const string& lastName, int tripType, const routeNode& routeForward, const routeNode& routeBack) {
    reservationNode* newReserve = new reservationNode;
    newReserve->firstName = firstName;
    newReserve->lastName = lastName;
    newReserve->tripType = tripType;
    newReserve->routeForward = routeForward;
    newReserve->routeBack = routeBack;
    newReserve->nextReserve = nullptr;

    if (!reserveHead) {
        reserveHead = reserveTail = newReserve;
    } else {
        reservationNode* current = reserveHead, *prev = nullptr;
        while (current && current->lastName <= lastName) {
            prev = current;
            current = current->nextReserve;
        }
        if (prev) {
            prev->nextReserve = newReserve;
        } else {
            reserveHead = newReserve;
        }
        newReserve->nextReserve = current;
    }
}

void DeleteReservation(const string& firstName, const string& lastName) {
    reservationNode* current = reserveHead, *prev = nullptr;
    while (current) {
        if (current->firstName == firstName && current->lastName == lastName) {
            for (int i = 0; i < current->routeForward.nHops; ++i) {
                int flightNo = current->routeForward.FlightNo[i];
                for (int j = 0; j < totalFlights; ++j) {
                    if (flightList[j].flight->flightNo == flightNo) {
                        --flightList[j].flight->noOfPassengers;
                        break;
                    }
                }
            }
            if (current->tripType == ROUNDTRIP) {
                for (int i = 0; i < current->routeBack.nHops; ++i) {
                    int flightNo = current->routeBack.FlightNo[i];
                    for (int j = 0; j < totalFlights; ++j) {
                        if (flightList[j].flight->flightNo == flightNo) {
                            --flightList[j].flight->noOfPassengers;
                            break;
                        }
                    }
                }
            }

            if (prev) {
                prev->nextReserve = current->nextReserve;
            } else {
                reserveHead = current->nextReserve;
            }
            if (current == reserveTail) {
                reserveTail = prev;
            }
            delete current;
            cout << "Reservation deleted successfully.\n";
            return;
        }
        prev = current;
        current = current->nextReserve;
    }
    cout << "Reservation not found.\n";
    cout << endl;
}

void PrintReservations() {
    reservationNode* current = reserveHead;
    while (current) {
        cout << current->firstName << " " << current->lastName << "\n";
        current = current->nextReserve;
    }
}

void PrintPassengersOfFlight(int flightNo) {
    cout << "Passengers of flight " << flightNo << ":\n";
    reservationNode* current = reserveHead;
    while (current) {
        for (int i = 0; i < current->routeForward.nHops; ++i) {
            if (current->routeForward.FlightNo[i] == flightNo) {
                cout << current->firstName << " " << current->lastName << "\n";
                break;
            }
        }
        current = current->nextReserve;
    }
}

void DisplayGraphConnections() {
    cout << "Graph Representation of Flights:\n";
    cout << "---------------------------------\n";
    for (int i = 0; i < MAXCITY; ++i) {
        if (!cityList[i].cityName.empty()) {
            cout << cityList[i].cityName << " -> ";
            flightNode* current = cityList[i].nextDeparture;
            while (current) {
                cout << current->arrivalCity << " ";
                current = current->nextDeparture;
            }
            cout << "\n";
        }
    }
    cout << "---------------------------------\n";
}

int main() {
    for (int i = 0; i < MAXCITY; ++i) {
        cityList[i].cityName.clear();
        cityList[i].nextDeparture = nullptr;
        cityList[i].nextArrival = nullptr;
    }
    ReadFlightData();
    cout << "<#-Welcome to Airline Reservation System Portal-#>\n"; 
    cout << endl;

    int choice;
    do {   
        cout << "1. Display All Flights\n"
             << "2. Display All Cities\n"
             << "3. Display Departures for a City\n"
             << "4. Display Arrivals for a City\n"
             << "5. Find Shortest Path\n"
             << "6. Find Reachable Cities\n"
             << "7. Add Reservation\n"
             << "8. Print Reservations\n"
             << "9. Print Passengers of a Flight\n"
             << "10. Delete a Reservation\n"
             << "11. show graph connections\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> choice;
        cout << endl;

        switch (choice) {
        case 1:
            DisplayAllFlightsData();
            break;
        case 2:
            DisplayAllCities();
            break;
        case 3: {
            string cityName;
            cout << "Enter city name: ";
            cin >> cityName;
            DisplayDeparturesForCity(cityName);
            break;
        }
        case 4: {
            string cityName;
            cout << "Enter city name: ";
            cin >> cityName;
            DisplayArrivalsForCity(cityName);
            break;
        }
        case 5: {
            string startCity, endCity;
            cout << "Enter start city: ";
            cin >> startCity;
            cout << "Enter end city: ";
            cin >> endCity;
            DijkstraShortestPath(startCity, endCity);
            break;
        }
        case 6: {
            string cityName;
            cout << "Enter city name: ";
            cin >> cityName;
            BFSReachableCities(cityName);
            break;
        }
        case 7: {
            string firstName, lastName;
            cout << "Enter first name: ";
            cin >> firstName;
            cout << "Enter last name: ";
            cin >> lastName;
            cout << endl;
            routeNode routeForward = {0, 1, {201}};  
            routeNode routeBack = {0, 0, {}};        
            AddReservation(firstName, lastName, ONEWAY, routeForward, routeBack);
            break;
        }
        case 8:
            PrintReservations();
            break;
        case 9: {
            int flightNo;
            cout << "Enter flight number: ";
            cin >> flightNo;
            PrintPassengersOfFlight(flightNo);
            break;
        }
        case 10: {  
            string firstName, lastName;
            cout << "Enter first name: ";
            cin >> firstName;
            cout << "Enter last name: ";
            cin >> lastName;
            DeleteReservation(firstName, lastName);
            break;
        }
        case 11:  
    DisplayGraphConnections();
    break;
        case 0:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
