// Mars Rover Simulator
// The rover drives around a 10x10 area following mission strings.

#include <iostream>
#include <string>
using namespace std;

const int MAX_BATTERY = 40;
const int GRID_SIZE = 10;

// 0 = North, 1 = East, 2 = South, 3 = West
string headingName(int heading) {
    if (heading == 0) return "North";
    if (heading == 1) return "East";
    if (heading == 2) return "South";
    return "West";
}

// Each command function returns "" if it worked, or the reason it failed.

string moveForward(int &x, int &y, int heading, int &battery) {
    int newX = x;
    int newY = y;
    if (heading == 0) newY++;
    else if (heading == 1) newX++;
    else if (heading == 2) newY--;
    else newX--;

    // check bounds before battery
    if (newX < 0 || newX >= GRID_SIZE || newY < 0 || newY >= GRID_SIZE) return "out of bounds";
    if (battery < 2) return "insufficient battery";

    x = newX;
    y = newY;
    battery -= 2;
    return "";
}

string turn(char direction, int &heading, int &battery) {
    if (battery < 1) return "insufficient battery";

    if (direction == 'L') heading = (heading + 3) % 4; // +3 is the same as -1
    else heading = (heading + 1) % 4;
    battery--;
    return "";
}

string scan(int &battery) {
    if (battery < 3) return "insufficient battery";
    battery -= 3;
    return "";
}

string charge(int x, int y, int &battery) {
    if (x != 0 || y != 0) return "not at charging station";
    battery = MAX_BATTERY;
    return "";
}

string runCommand(char c, int &x, int &y, int &heading, int &battery) {
    if (c == 'F') return moveForward(x, y, heading, battery);
    if (c == 'L' || c == 'R') return turn(c, heading, battery);
    if (c == 'S') return scan(battery);
    if (c == 'C') return charge(x, y, battery);
    return "unknown command";
}

void printSummary(int x, int y, int heading, int battery, int successCount, bool goalReached) {
    cout << "Mission Complete!" << endl;
    cout << "Rover ended at coordinates (" << x << "," << y << ") with heading "
         << headingName(heading) << " and battery " << battery << "." << endl;
    cout << "Number of Successful Commands: " << successCount << endl;

    bool isReset = (x == 0 && y == 0 && battery == MAX_BATTERY);
    if (goalReached && isReset) cout << "Goal reached and rover reset." << endl;
    else if (goalReached) cout << "Goal reached but rover not reset." << endl;
    else if (isReset) cout << "Goal not reached but rover reset." << endl;
    else cout << "Goal not reached and rover not reset." << endl;
}

int main() {
    int x = 0;
    int y = 0;
    int heading = 1; // starts facing East
    int battery = MAX_BATTERY;
    string mission;

    while (true) {
        cout << "Input mission: ";
        if (!getline(cin, mission) || mission == "quit") break;

        int goalX = -1, goalY = -1;
        cout << "Input goal x coordinate: ";
        cin >> goalX;
        cout << "Input goal y coordinate: ";
        cin >> goalY;
        cin.ignore(1000, '\n'); // get rid of the newline so getline works next time

        int successCount = 0;
        bool goalReached = (x == goalX && y == goalY);

        for (int i = 0; i < mission.length(); i++) {
            char c = mission[i];
            string error = runCommand(c, x, y, heading, battery);

            if (error != "") {
                cout << "command " << c << " rejected: " << error << endl;
            } else {
                successCount++;
                cout << "Command " << c << " succeeded. Current coordinates: ("
                     << x << "," << y << ") Heading: " << headingName(heading)
                     << " Battery: " << battery << endl;
                if (x == goalX && y == goalY) goalReached = true;
            }
        }

        printSummary(x, y, heading, battery, successCount, goalReached);
    }

    return 0;
}
