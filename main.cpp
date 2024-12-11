//HOTELLI HARJOITUS TYÖ 
//Oskar Engman
//Arvosanatavoite 4-5

#include<iostream>
#include<string>
#include<cstdlib>
#include<ctype.h>
#include<ctime>
#include<cmath>
#include<iomanip>
#include<sstream>
#include<vector>
#include<fstream>

using namespace std;


const int generatedDiscounts = 90;
const int roomAmount = 300;
const int singleRoomPrice = 100;
const int doubleRoomPrice = 150;

const string fileRooms = "Rooms.txt";
const string fileDiscounts = "Discounts.txt";

string TimestampToString(tm timestamp);

// Structit hotellia varten
struct Reservation {
	
	int id = 0;
	int price = 0; 
	int roomNumber = 0;

	tm reservationStartDate;
	tm reservationEndDate;

	string nameOnReservation = "Error";

	string toString() const {
		ostringstream oss;
		oss << to_string(id) << "," << TimestampToString(reservationStartDate) << "," << TimestampToString(reservationEndDate) << "," << nameOnReservation << "," << to_string(price) << "_";
		return oss.str();
	}
};

struct Room {
	int number = 0;
	int roomType = 0; // Single = 1, Double = 2

	vector<Reservation> reservationsForRoom;

	string toString() const {
		return to_string(number) + "," + to_string(roomType);
	}
};

struct DiscountDay {
	int discountAmount;
	string discountDate;

	string toString() const {
		return discountDate + "," + to_string(discountAmount);
	}
};

struct Hotel {

	vector<DiscountDay> discountList;
	vector<Room> roomList;
	//vector<Reservation> reservationList;
};

//
void SaveToFile(int option, const string& filename, Hotel hotel); void CreateFiles(int option, const string& fileRooms); bool fileExists(const string& fileName);
void HasLaunchedBefore(); tm ReturnTimestamp(int days, string startTime); void Menu(); string GetUserInput(int option); void CreateReservation(int roomType, int amountOfNights, string fullname, tm startDate);
vector<Reservation> GetReservations(string search, int option); vector<string> splitString(string line, char separator); tm ReturnTimestamp(int days, string startTime);
bool CheckIfInRange(int number, int start, int end); vector<Reservation> LoadReservations(int forRoom);
//


void SaveToFile(int option, const string& filename, Hotel hotel) {

	ofstream output(filename);
	if (output.is_open()) {
		switch (option) {
		case 1:
			cout << "Amount of rooms seems to be: " << hotel.roomList.size() << "\n";
			for (int i = 0; i < roomAmount; i++) {
				if (i < hotel.roomList.size()) {
					Room room = hotel.roomList[i];

					output << room.toString() << "_";
					if (!room.reservationsForRoom.empty()) {

						for (int y = 0; y < room.reservationsForRoom.size(); y++) {
							output << room.reservationsForRoom[y].toString();
						}
						output << endl;
					}
					else {
						output << "Available" << endl;
					}
				}
				else {
					cout << "\nError: Room list did not include room number: " << i + 1 << "\n";
				}
			}
			cout << "Rooms succesfully saved!\n";
			break;
		case 2:
			cout << "Amount of discounts seems to be: " << hotel.discountList.size() << "\n";
			for (int i = 0; i < generatedDiscounts; i++) {
				if (i < hotel.discountList.size()) {
					DiscountDay discount = hotel.discountList[i];
					output << discount.toString() << endl;
				}
			}

			cout << "Discounts succesfully saved!\n";
			break;
		}
		output.close();
	}
	else {
		cout << "Unable to create file: " << filename;
	}
}

void CreateFiles(int option, const string& file) {
	
	Hotel hotel;

	if (option == 1) {
		for (int i = 1; i <= roomAmount; i++) {
			Room room;
			room.number = i;
			if (i <= 150) {
				room.roomType = 1;
			}
			else {
				room.roomType = 2;
			}

			hotel.roomList.push_back(room);
		}
		cout << "Hotel System Created Succesfully!\n";

		SaveToFile(1, file, hotel);
	}
	if (option == 2) {
		
		srand(static_cast<unsigned>(time(0)));

		for (int i = 0; i <= generatedDiscounts; i++) {
			DiscountDay discount;

			discount.discountDate = TimestampToString(ReturnTimestamp(i, "0"));
			int discountAmount = (rand() % 3);
			
			if (discountAmount > 0) {
				discount.discountAmount = discountAmount;
				hotel.discountList.push_back(discount);
			}
		}
		cout << "Randomised discounts succesfully!\n";

		SaveToFile(2, file, hotel);
	}
}

bool fileExists(const string& fileName) {
	ifstream file(fileName);
	return file.good();	
}

void HasLaunchedBefore() {

	if (!fileExists(fileRooms)) {
	
		cout << "Creating file: " << fileRooms << '\n';
		CreateFiles(1, fileRooms);

	}
	else {
		cout << fileRooms << " Found!\n";
	}

	if (!fileExists(fileDiscounts)) {
		cout << "Creating file: " << fileDiscounts << '\n';
		CreateFiles(2, fileDiscounts);
	}
	else {
		cout << fileDiscounts << " Found!\n";
	}

}

tm ReturnTimestamp(int days, string startTime) {

	time_t t = time(nullptr);
	tm now;
	localtime_s(&now, &t);

	if (!(stoi(startTime) == 0)) {
		vector<string> dateInfo = splitString(startTime, '.');
		now.tm_year = stoi(dateInfo[2]) - 1900; // Year since 1900 
		now.tm_mon = stoi(dateInfo[1]) - 1; // Month (0-11, where 0 = January) 
		now.tm_mday = stoi(dateInfo[0]); 
	}
	mktime(&now);

	if (days < 1) {
		return now;
	}
	else {

		for (int i = 0; i < days; i++) {

			now.tm_mday += 1;
			mktime(&now);

		}
		return now;
	}
}

string TimestampToString(tm timestamp) {
	ostringstream oss;

	oss << put_time(&timestamp, "%d.%m.%Y");

	return oss.str();
}

tm StringToTimestamp(string date) {
	
	tm timestamp = ReturnTimestamp(0,"0");


	vector<string> dateInfo = splitString(date, '.');

	//cout << "Currently changing string: " << date << " to tm." << endl;
	//cout << endl <<"Splitting date: " << date << endl;
	timestamp.tm_year = (stoi(dateInfo[2]) - 1900); //cout << "Year has been set as: " << dateInfo[2] << endl;
	timestamp.tm_mon = (stoi(dateInfo[1]) - 1); //cout << "month has been set as: " << dateInfo[1] << endl;
	timestamp.tm_mday = stoi(dateInfo[0]); //cout << "day has been set as: " << dateInfo[0] << endl;
	mktime(&timestamp);

	return timestamp;
}

vector<string> splitString(string line, char separator) {
	
	vector<string> stringDivider;

	string item;
	stringstream ss(line);

	while (getline(ss, item, separator)) {

		stringDivider.push_back(item);

	}

	return stringDivider;
}

vector<Room> LoadRooms() {

	//cout << "Preparing to load rooms..." << endl;

	Room room;
	vector<Room> listOfFoundRooms;

	int lineNumber = 0;
	string line;
	
	ifstream input(fileRooms);
	if (input.is_open()) {

		// Ladataan ja palautetaan huoneet vectorina
		
		vector<string> roomInfo;

		while (getline(input, line)) {
						
			roomInfo = splitString(splitString(line, '_')[0], ',');
			room.number = stoi(roomInfo[0]);
			room.roomType = stoi(roomInfo[1]);
			
			room.reservationsForRoom = LoadReservations(lineNumber);

			listOfFoundRooms.push_back(room);

			lineNumber++;
		}
		//cout << "Amount of rooms seems to be: " << listOfFoundRooms.size() << "\n";
		//cout << "Rooms succesfully loaded!\n";

		input.close();
	}
	else {
		cout << "Unable to read file: " << fileRooms;
	}

	return listOfFoundRooms;
}

vector<Reservation> LoadReservations(int forRoom) {

	Reservation reservation;
	vector<Reservation> listOfFoundReservations;

	string line;
	

	ifstream input(fileRooms);
	if (input.is_open()) {

		// Ladataan ja palautetaan varaukset vectorina
		
		vector<string> ReservationInfo;

		int lineNumber = 0;
		while (getline(input, line)) {
			
			if (lineNumber == forRoom) {


				//*Reservation ID*, *Start Date*, *End Date*, *Name on reservation*, *Price*
				// OR
				//Available

				// Jaetaan kahden eri tiedon väliltä tiedoston riviltä *Room* _ *Reservation*
				// savedDataOnline[0] = Room, [1] = Reservation

				//cout << "Line under inspection: " << line << endl;
				vector<string> savedDataOnLine = splitString(line, '_');
				//cout << "savedDataOnLine - List size: " << savedDataOnLine.size() << endl;


				// Jos savedDataOnline[1] lukee "Available" tarkoittaa ettei huoneelle ole yhtäkään varausta.
				if (savedDataOnLine[1].find("Available") != string::npos) {
					//cout << "Room available!" << endl;
				}
				else {
					// Pyöritetään for loopilla mahdollisesti useamman varauksen läpi samalle huoneelle.
					for (int i = 1; i < savedDataOnLine.size(); i++) {

						// Jaetaan rivin loppupuoli vielä osiin saadakseen tiedot löytyneelle varaukselle.
						ReservationInfo = splitString(savedDataOnLine[i], ',');


						//cout << "ReservationInfo - list size: " << ReservationInfo.size() << " step for for() loop: " << i << endl;



						// Talletetaan vectoriin tallennetut tiedot structiin reservation
						reservation.id = stoi(ReservationInfo[0]);

						reservation.reservationStartDate = StringToTimestamp(ReservationInfo[1]);
						reservation.reservationEndDate = StringToTimestamp(ReservationInfo[2]);

						reservation.nameOnReservation = ReservationInfo[3];
						reservation.price = stoi(ReservationInfo[4]);

						reservation.roomNumber = lineNumber;

						// Viimeiseksi vielä lisätään varaus huoneen varaus listaan.
						listOfFoundReservations.push_back(reservation);
					}
				}
				// lineNumberia lisäämällä varmistetaan, että käsitellään oikean huoneen vector listaa.
			}
			lineNumber++;
		}

		input.close();
	}
	else {
		cout << "Unable to read file: " << fileRooms;
	}

	return listOfFoundReservations;

}

vector<DiscountDay> LoadDiscounts() {

	vector<DiscountDay> listOfFoundDiscounts;

	string line;
	int lineNumber = 0;

	ifstream input(fileDiscounts);
	if (input.is_open()) {

		vector<string> discountInfo;

		while (getline(input, line)) {

			DiscountDay discount;
			discountInfo = splitString(line, ',');

			
			discount.discountDate = (discountInfo[0]);
			discount.discountAmount = stoi(discountInfo[1]);

			listOfFoundDiscounts.push_back(discount);
		}

		//cout << "Amount of discounts seems to be: " << listOfFoundDiscounts.size() << "\n";
		//cout << "Discounts succesfully loaded!\n";
			

		input.close();
	}
	else {
		cout << "Unable to read file: " << fileDiscounts;
	}

	return listOfFoundDiscounts;

}

void ReserveRoom() {

	bool dataSet = false;
	int manualDate;
	tm startingDate;
	int roomType;
	int amountOfNights;
	string fullname;




	while (!dataSet) {
		cout << "Enter 1 for single, 2 for double\n";
		roomType = stoi(GetUserInput(1));
		if (roomType == 1 || roomType == 2) {
			dataSet = true;
			//cout << "Room type has been set to: " << roomType << "\n";
		}
		else {
			cout << "\nError: Wrong room type, try again!\n";
		}

	}
	dataSet = false;

		
	startingDate = ReturnTimestamp(0, "0");
			

	while (!dataSet) {
		cout << "The amount of nights: \n";
		amountOfNights = stoi(GetUserInput(1));
		if (amountOfNights > 0) {
			dataSet = true;
		}
		else {
			cout << "\nError: Nights not properly set, try again!\n";
		}

	}
	dataSet = false;

	cout << "Name for the reservation: \n";
	fullname = (GetUserInput(2));

	CreateReservation(roomType, amountOfNights, fullname, startingDate);

}

bool CheckIfRoomAvailable(vector<Reservation> reservationList) {

	if (!reservationList.empty()) {
		return false;
	}
	else {
		//cout << "Room is available" << endl;
		return true;
	}
}

bool CheckIfSameDay(tm day1, tm day2) {
	if (day1.tm_year == day2.tm_year && day1.tm_mon == day2.tm_mon && day1.tm_mday == day2.tm_mday) {
		return true;
	}
	else {
		return false;
	}
}

double CalculatePriceForNight(vector<DiscountDay> discountList, tm day, int roomType) {
	
	int roomCost = 0;

	if (roomType == 1) {
		roomCost = singleRoomPrice;
	}
	else if (roomType == 2) {
		roomCost = doubleRoomPrice;
	}


	for (int i = 0; i < discountList.size(); i++) {
		DiscountDay discount = discountList[i];
		if (CheckIfSameDay(StringToTimestamp(discount.discountDate), day)) {
			if (discount.discountAmount == 1) {
				return 0.90*roomCost;
			}
			if (discount.discountAmount == 2) {
				return 0.80 * roomCost;
			}
		}
	}
	return roomCost;
}

void CreateReservation(int roomType, int amountOfNights, string fullname, tm startDate) {

	bool roomFound = false;

	int roomRangeStart;
	int roomRangeEnd;

	double discount;

	Hotel hotel;
	
	string startDateS = TimestampToString(startDate);

	hotel.roomList = LoadRooms();
	hotel.discountList = LoadDiscounts();
	switch (roomType) {
		case 1:
			roomRangeStart = 0;
			roomRangeEnd = (hotel.roomList.size()/2)-1;
			break;
		case 2:
			roomRangeStart = (hotel.roomList.size() / 2)-1;
			roomRangeEnd = (hotel.roomList.size() - 1);
			break;
	}
	//cout << "Room Search Start Range: " << roomRangeStart << "\n";
	//cout << "Room Search End Range: " << roomRangeEnd << "\n";

	//cout << "Rooms in hotel: " << hotel.roomList.size() << endl;
	//cout << "Single rooms: " << (hotel.roomList.size() / 2) << endl;
	for (int i = 0; i < hotel.roomList.size(); i++) {
		//cout << "In step number: " << i << " System found: " << hotel.roomList[i].reservationsForRoom.size() << " Reservations for Room number: " << hotel.roomList[i].number << endl;


		//cout << "Checking if room: " << hotel.roomList[i].number << " is available, " << endl;
		if (CheckIfRoomAvailable(hotel.roomList[i].reservationsForRoom) 
			&& roomRangeStart <= i && i <= roomRangeEnd) {
			//cout << "\nRoom is available!" << endl;
			if (!roomFound) {

				Reservation reservation;

				cout << "Creating a reservation...\n";

				srand(time(nullptr));
				while(true) {

					int randID = rand() % 90000 + 10000;
					if (GetReservations(to_string(randID), 1).size() == 0) {
						reservation.id = randID;
						break;
					}

				}
				
				cout << "Reservation ID set as: " << reservation.id << "\n";

				reservation.reservationStartDate = startDate;
				reservation.reservationEndDate = ReturnTimestamp(amountOfNights, startDateS);
				cout << "Reservation start date set as: " << startDateS << "\n";
				cout << "Reservation end date set as: " << TimestampToString(reservation.reservationEndDate) << "\n";

				reservation.nameOnReservation = fullname;
				cout << "Reservation name has been set as: " << reservation.nameOnReservation << "\n";

				reservation.roomNumber = hotel.roomList[i].number;
				cout << "Reservation room number has been set as: " << reservation.roomNumber << "\n";

				reservation.price = 0;
				for (int y = 0; y < amountOfNights; y++) {
					tm day;
					if (y == 0) {
						day = startDate;
					}
					else {
						day = ReturnTimestamp(y, startDateS);
					}

					double PriceForNight = CalculatePriceForNight(hotel.discountList, day, roomType);
					reservation.price += PriceForNight;
					cout << "Day: " << y + 1 << " (" << TimestampToString(day) << ") " << " Price:" << PriceForNight << "\n";

				}
				cout << "Reservation total price: " << reservation.price << "\n";

				hotel.roomList[i].reservationsForRoom.push_back(reservation);
				SaveToFile(1, fileRooms, hotel);
				roomFound = true;
			}
		}
		else {
			//cout << "\n------------------\n";
			//cout << "Room is not available" << endl;
			//cout << "------------------\n\n";
		}

	}
}

vector<Reservation> GetReservations(string search, int option) {
	
	vector<Room> loadedRooms = LoadRooms();
	//cout << "Rooms loaded in GetReservations()\n";
	vector<Reservation> allReservations;

	//cout << "Loaded roomList size:" << loadedRooms.size() << "\n";

	for (int i = 0; i < loadedRooms.size(); i++) {
		
		if (!loadedRooms[i].reservationsForRoom.empty()) {

			//cout << "Room: " << i+1 << " reservation count: " << loadedRooms[i].reservationsForRoom.size() << "\n";
			for (int y = 0; y < loadedRooms[i].reservationsForRoom.size(); y++) {
				//cout << "Checking reservation number: " << y+1 << " reservations in room: " << i+1 << "\n";
				allReservations.push_back(loadedRooms[i].reservationsForRoom[y]);
			}
		}
	}

	//cout << "Loaded all reservations! (" << allReservations.size() << ")\n";
	vector<Reservation> foundReservations;
	
	if (!search.empty()) {
		switch (option) {
		case 1:
			for (int i = 0; i < allReservations.size(); i++) {
				if (allReservations[i].id == stoi(search)) {
					foundReservations.push_back(allReservations[i]);
				}
			}
			break;
		case 2:
			for (int i = 0; i < allReservations.size(); i++) {
				if (allReservations[i].nameOnReservation.find(search) != string::npos) {
					foundReservations.push_back(allReservations[i]);
				}
			}
			break;
		case 3:
			for (int i = 0; i < allReservations.size(); i++) {
				if (allReservations[i].roomNumber == (stoi(search))) {
					foundReservations.push_back(allReservations[i]);
				}
			}
			break;
		}

		return foundReservations;
	}
	else {
		return allReservations;
	}
}

void PrintReservations(vector<Reservation> loadedReservations) {

	vector<Reservation> allReservations;




	if (loadedReservations.size() > 0) {
		cout << "\n" <<
			"Reservations found: \n"
			"----------------------------------------------------------------------------------- \n"
			" ID | Room Number | start - end | Name | Price \n";



		for (int i = 0; i < loadedReservations.size(); i++) {
			Reservation reservationToPrint = loadedReservations[i];

			cout
				<< reservationToPrint.id << " | "
				<< reservationToPrint.roomNumber + 1 << " | "
				<< TimestampToString(reservationToPrint.reservationStartDate) << " - " << TimestampToString(reservationToPrint.reservationEndDate)
				<< " | " << reservationToPrint.nameOnReservation << " | "
				<< reservationToPrint.price << "\n";

		}
		cout <<
			"----------------------------------------------------------------------------------- \n";
	}
	else {
		cout <<
			"\n-----------------------------------------------------------------------------------\n"
			"\nNo reservations found!\n"
			"\n-----------------------------------------------------------------------------------\n";
	}
}

void Reservations() {
	bool inMenu = true;

	while (inMenu) {

		cout << "\n" <<
			TimestampToString(ReturnTimestamp(0, "0")) << endl <<
			"Check reservations: \n"
			"-------------------------------- \n"
			" Menu options: \n"
			" 1. Get all Reservations\n"
			" 2. Get Reservations by id\n"
			" 3. Get Reservations by name\n"
			" \n5. Exit\n"
			"-------------------------------- \n";

		int input = stoi(GetUserInput(1));

		switch (input) {
		case 1:
			PrintReservations(GetReservations("",1));
			break;
		case 2:
			cout << "Search with id of reservation: " << endl;
			PrintReservations(GetReservations(GetUserInput(1), 1));
			break;
		case 3:
			cout << "Search with name on reservation: " << endl;
			PrintReservations(GetReservations(GetUserInput(2), 2));
			break;
		case 5:
			inMenu = false;
			break;
		default:
			cout << "\n Error: Menu not found \n";
			break;
		}
	}
}

void PrintDiscounts() {
	vector<DiscountDay> discounts = LoadDiscounts();

	if (discounts.size() > 0) {
		cout << "\n" <<
			"Discounts found: \n"
			"----------------------------------------------------------------------------------- \n"
			" Amount | Date \n";

		for (int i = 0; i < discounts.size(); i++) {
			DiscountDay discountToPrint = discounts[i];

			int discountPercentage;
			switch(discountToPrint.discountAmount) {
				case 1:
					discountPercentage = 10;
					break;
				case 2:
					discountPercentage = 20;
					break;
			}


			cout
				<< discountPercentage << "% | "
				<< discountToPrint.discountDate << "\n";

		}
		cout <<
			"----------------------------------------------------------------------------------- \n";
	}
	else {
		cout <<
			"\n-----------------------------------------------------------------------------------\n"
			"\nNo discounts found!\n"
			"\n-----------------------------------------------------------------------------------\n";
	}

}

void Menu() {
	bool inMenu = true;
	int input;

	while (true) {
		input = 0;
		cout << "\n" <<
			TimestampToString(ReturnTimestamp(0,"0")) << endl <<
			"Main: \n"
			"-------------------------------- \n"
			" Menu options: \n"
			" 1. Reserve room\n"
			" 2. Reservations\n"
			" 3. Print Active Discounts\n"
			" \n5. Exit\n"
			"-------------------------------- \n";

		input = stoi(GetUserInput(1));
		//cout << "Input: " << input << endl;

		if (input > 0) {
			switch (input) {
			case 1:
				ReserveRoom();
				break;
			case 2:
				Reservations();
				break;
			case 3:
				PrintDiscounts();
				break;
			case 5:
				exit(0);
				break;
			default:
				cout << "\n Error: Menu not found \n";
				break;
			}
		}
		else {
			cout << "\nError: input value returned 0 - Try again!";
		}
	}
}

string GetUserInput(int option) {
	string userInput;
	string verifiedInput;

	while (userInput.empty()) {

		cout << '\n' << "$ ";
		getline(cin, userInput);
		cin.clear();
		cout << "\n";

		switch (option) {

			// Kysytään käyttäjältä numeroa syötteeksi.
			case 1:
				for (size_t i = 0; i < userInput.size(); i++) {

					char inputChar = userInput[i];

					if (isdigit(inputChar)) {
						//cout << inputChar << " is an allowed character" << endl;
						verifiedInput.push_back(inputChar);
						//cout << "VerifiedInput is now: " << verifiedInput << endl;
					}
					else {
						cout << "\nError: " << inputChar << " is not acceptable character" << " (removed)" << endl;
					}
				} 
				break;

			// Kysytään käyttäjältä stringiä syötteeksi. (Nimi)
			case 2:
				for (size_t i = 0; i < userInput.size(); i++) {

					char inputChar = userInput[i];

					verifiedInput.push_back(inputChar);

					/*
					if (isdigit(inputChar)) {
						cout << "\nError: " << inputChar << " is not acceptable character" << " (removed)" << endl;
					}
					else if (inputChar <= 'z' && inputChar >= 'a' || inputChar <= 'Z' && inputChar >= 'A' || isspace(inputChar)) {
						//cout << inputChar << " is an allowed character" << endl;

						//cout << "VerifiedInput is now: " << verifiedInput << endl;
					}
					else {
						cout << "\nError: " << inputChar << " is not acceptable character" << " (removed)" << endl;
					}
					*/
				}
				break;
			default:

				break;
		}
	}

	if (!verifiedInput.empty()) {
		return verifiedInput;
	}
	else {
		return "0";
	}
}

bool CheckIfInRange(int number, int start, int end) {
	if (start <= number <= end) {
		return true;
	}
	else {
		return false;
	}
}

int main() {

	

	//Ensimmäinen käynnistys tarkistus
	// - Luodaan uudet tiedot jos on ensimmäinen
	HasLaunchedBefore();


	// Jos ei ole ensimmäinen käynnistys tai tiedostot on saatu luotua jatketaan ohjelmaan:
	while (true) {
		
		Menu();

	}
	return 1;
}