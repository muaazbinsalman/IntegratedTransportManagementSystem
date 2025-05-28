#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <iomanip>
#include <string>
using namespace std;

//-------------------------//
//   Utility Functions     //
//-------------------------//

// URL-decode: converts '+' to space and decodes "%HH" hex codes.
string urlDecode(const string &value) {
    string result;
    for (size_t i = 0; i < value.length(); ++i) {
        if (value[i] == '%') {
            if (i + 2 < value.length()) {
                string hex = value.substr(i + 1, 2);
                result += static_cast<char>(strtol(hex.c_str(), nullptr, 16));
                i += 2;
            }
        } else if (value[i] == '+') {
            result += ' ';
        } else {
            result += value[i];
        }
    }
    return result;
}

// Extract a parameter's value from the URL query string.
string getParam(const string &query, const string &key) {
    size_t pos = query.find(key + "=");
    if (pos == string::npos)
        return "";
    size_t start = pos + key.length() + 1;
    size_t end = query.find("&", start);
    if (end == string::npos)
        end = query.length();
    return urlDecode(query.substr(start, end - start));
}

// Parse a time string in HH:MM format into a float value (hour + minute/100).
// This function validates that hours are 0–23 and minutes 0–59.
float parseTime(const string &timeStr) {
    size_t pos = timeStr.find(":");
    if (pos == string::npos)
        throw invalid_argument("Invalid time format. Please use HH:MM.");
    int hour = stoi(timeStr.substr(0, pos));
    int minute = stoi(timeStr.substr(pos + 1));
    if (hour < 0 || hour > 23)
        throw invalid_argument("Hour must be between 0 and 23.");
    if (minute < 0 || minute > 59)
        throw invalid_argument("Minute must be between 0 and 59.");
    // For discount calculation using float comparison.
    return hour + minute / 100.0f;
}

//-------------------------//
//         Classes         //
//-------------------------//

// Class representing a User (Passenger)
class User {
private:
    static long nextBookingNumber;
    string firstName;
    string lastName;
    long bookingNumber;
public:
    User(const string &fName, const string &lName) : firstName(fName), lastName(lName) {
        bookingNumber = nextBookingNumber++;
    }
    string getFullName() const {
        return firstName + " " + lastName;
    }
    long getBookingNumber() const {
        return bookingNumber;
    }
    static void resetBookingNumber() {
        nextBookingNumber = 34556;
    }
};

long User::nextBookingNumber = 34556;

// Class representing a Train
class Train {
private:
    string name;
    vector<float> prices;
    vector<string> stations;
    float discountStartTime;
    float discountEndTime;
public:
    Train() : name(""), discountStartTime(0.0), discountEndTime(0.0) {}
    Train(const string &trainName, const vector<string> &stationNames, const vector<float> &stationPrices, float discountStart, float discountEnd)
        : name(trainName), stations(stationNames), prices(stationPrices), discountStartTime(discountStart), discountEndTime(discountEnd) {}
    
    float getPrice(int stationIndex) const {
        if (stationIndex < 0 || stationIndex >= prices.size())
            return 0;
        return prices[stationIndex];
    }
    
    bool isDiscountTime(float time) const {
        return (time >= discountStartTime && time <= discountEndTime);
    }
    
    string getName() const {
        return name;
    }
    
    size_t getNumStations() const {
        return stations.size();
    }
    
    string getStationName(int index) const {
        if (index >= 0 && index < stations.size())
            return stations[index];
        return "";
    }
    
    // Calculates total price using the prices of the start and end stations.
    // Applies a 40% discount if within discount time.
    float calculateTotalPrice(float journeyTime, int startStation, int endStation) const {
        if (startStation < 1 || startStation > stations.size() ||
            endStation < 1 || endStation > stations.size() || startStation == endStation) {
            throw invalid_argument("Invalid start or end station.");
        }
        float totalPrice = getPrice(startStation - 1) + getPrice(endStation - 1);
        if (isDiscountTime(journeyTime)) {
            totalPrice *= 0.6; // 40% discount
        }
        return totalPrice;
    }
    
    // Outputs discount status in HTML.
    void printDiscountStatus(float journeyTime) const {
        cout << "Discount Status: " 
             << (isDiscountTime(journeyTime) ? "Discount Applied" : "No Discount") 
             << "<br>";
    }
};

//-------------------------//
//       Decorative        //
//-------------------------//

// decor() outputs an HTML horizontal rule.
void decor() {
    cout << "<hr>";
}

//-------------------------//
//          Main           //
//-------------------------//

int main() {
    // Output HTTP header and start of HTML with inline CSS for a unified look.
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>Booking Confirmation</title>";
    cout << "<style>"
         << "body { background: #0D0D0D; color: #fff; font-family: 'Poppins', sans-serif; margin: 0; padding: 20px; }"
         << ".container { width: 90%; max-width: 500px; margin: 20px auto; background: #1e1e1e; padding: 20px; border-radius: 10px; box-shadow: 0 8px 20px rgba(0,0,0,0.2); }"
         << "hr { border: 1px solid #333; }"
         << "</style></head><body><div class=\"container\">";
    
    // Retrieve query string from the environment variable.
    string query = getenv("QUERY_STRING") ? getenv("QUERY_STRING") : "";
    
    int passengerCount = 0;
    string pc = getParam(query, "passengerCount");
    if (!pc.empty()) {
        passengerCount = stoi(pc);
    } else {
        cout << "<p>Error: Missing passenger count.</p></div></body></html>";
        return 1;
    }
    
    // Parse and validate journey time (expects HH:MM format).
    float journeyTime;
    try {
        string jt = getParam(query, "journeyTime");
        if (jt.empty()) throw invalid_argument("Missing journey time.");
        journeyTime = parseTime(jt);
    } catch (const exception &ex) {
        cout << "<p>Error: " << ex.what() << "</p></div></body></html>";
        return 1;
    }
    
    int startStation = stoi(getParam(query, "startStation"));
    int endStation = stoi(getParam(query, "endStation"));
    int trainChoice = stoi(getParam(query, "trainChoice"));
    
    // Define station names and prices (two train options).
    string stationNamesArray[] = {"Lahore Junction", "Gujranwala", "Wazirabad Junction", "Gujrat"};
    vector<string> stationNames(stationNamesArray, stationNamesArray + (sizeof(stationNamesArray) / sizeof(stationNamesArray[0])));
    float stationPrices1Array[] = {1270, 1550, 1750, 1920};
    vector<float> stationPrices1(stationPrices1Array, stationPrices1Array + (sizeof(stationPrices1Array) / sizeof(stationPrices1Array[0])));
    float stationPrices2Array[] = {2300, 2650, 2780, 3200};
    vector<float> stationPrices2(stationPrices2Array, stationPrices2Array + (sizeof(stationPrices2Array) / sizeof(stationPrices2Array[0])));
    
    // Create train objects.
    Train train1("Express Train", stationNames, stationPrices1, 0.0, 10.0);
    Train train2("Local Train", stationNames, stationPrices2, 1.0, 11.0);
    Train chosenTrain;
    
    try {
        if (trainChoice == 1)
            chosenTrain = train1;
        else if (trainChoice == 2)
            chosenTrain = train2;
        else
            throw invalid_argument("Invalid train choice.");
    } catch (const invalid_argument &e) {
        decor();
        cout << "<p>Error: " << e.what() << "</p></div></body></html>";
        return 1;
    }
    
    // Build passenger list using dynamic parameters (firstName1, lastName1, firstName2, etc.).
    vector<User> passengers;
    for (int i = 1; i <= passengerCount; i++) {
        string fname = getParam(query, "firstName" + to_string(i));
        string lname = getParam(query, "lastName" + to_string(i));
        if (fname.empty() || lname.empty()) {
            cout << "<p>Error: Missing name for passenger " << i << ".</p></div></body></html>";
            return 1;
        }
        passengers.push_back(User(fname, lname));
    }
    
    // Output booking confirmation.
    cout << "<h1>Booking Confirmation</h1>";
    decor();
    for (size_t i = 0; i < passengers.size(); i++) {
        float totalPrice;
        try {
            totalPrice = chosenTrain.calculateTotalPrice(journeyTime, startStation, endStation);
        } catch (const invalid_argument &e) {
            cout << "<p>Error: " << e.what() << "</p></div></body></html>";
            return 1;
        }
        cout << "<p><strong>Name:</strong> " << passengers[i].getFullName() << "<br>"
             << "<strong>Booking Number:</strong> " << passengers[i].getBookingNumber() << "<br>"
             << "<strong>Train:</strong> " << chosenTrain.getName() << "<br>"
             << "<strong>Total Price:</strong> PKR " << fixed << setprecision(2) << totalPrice << "<br>";
        chosenTrain.printDiscountStatus(journeyTime);
        cout << "</p>";
        decor();
    }
    
    cout << "</div></body></html>";
    User::resetBookingNumber();
    return 0;
}