#include "cstdio"
#include "iostream"
#include "filesystem"
#include "fstream"
#include "string"
#include "sstream"

class Student {
private:

	std::string table = "./students.csv";

	bool fileExists(const std::string& filename) {

		if (!std::filesystem::exists(filename)) {

			std::ofstream file(filename);

			if (file.is_open()) {

				file << "id,full_name,average_score,speciality\n";
				file.close();
				std::cout << "File has been created: " << filename << "\n";

			}
			else {

				std::cerr << "Error to create file: " << filename << "\n";
				return false;

			}
		}
		else {

			readMaxIdFromFile(filename);

		}

		return true;
	}



	void readMaxIdFromFile(const std::string& filename) {

		std::ifstream file(filename);
		std::string line;
		if (file.is_open()) {

			std::getline(file, line);

			while (std::getline(file, line))
			{
				std::istringstream ss(line);
				std::string id_str;
				std::getline(ss, id_str, ',');
				int current_id = std::stoi(id_str);

				if (current_id >= next_id) {

					next_id = current_id + 1;

				}
			}

			file.close();
		}
		else {

			std::cerr << "Error to open file: " << filename << "\n";

		}
	}



public:

	static int next_id;
	int id;
	std::string full_name;
	float average_score;
	std::string speciality;



	void setNewStudent() {

		system("CLS");

		std::string av_score_str;
		bool corr_av_score = false;
		id = next_id++;

		std::cout << "Enter full name of student: ";
		std::cin.ignore();
		std::getline(std::cin, full_name);

		do
		{

			std::cout << "Enter an average score of student: ";
			std::cin >> av_score_str;

			try
			{

				average_score = std::stof(av_score_str);

				if (average_score >= 0.0f && average_score <= 10.0f)
				{

					corr_av_score = true;

				}
				else {

					std::cout << "Average score must be between 0 and 10. Please try again.\n";

				}

			}
			catch (const std::invalid_argument& e){

				std::cout << "Incorrect data. Please input a valid number.\n";

			}
			catch (const std::out_of_range& e){

				std::cout << "The number is out of range. Please try again.\n";

			}
		
		} while (!corr_av_score);
		

		std::cin.ignore();
		std::cout << "Enter a speciality of student: ";
		std::getline(std::cin, speciality);


		if (fileExists(table)) {

			std::ofstream file(table, std::ios::app);

			if (file.is_open()) {

				file << id << "," << full_name << "," << average_score << "," << speciality << "\n";
				file.close();
				std::cout << "The entry has been added to the file. \n";

			} 
			else {

				std::cout << "Error opening file. \n";

			}
		}

	}



	void getStudentsList() {

		if (!fileExists(table)) {

			std::cerr << "The file with students' data is missing. \n";
			return;

		}

		std::ifstream file(table);
		std::string line;

		if (file.is_open()) {

			system("CLS");

			std::getline(file, line);

			if (file.peek() == std::ifstream::traits_type::eof()) {

				std::cout << "There are no records in the database. \n";
				return;

			}

			std::cout << "List of students: \n";
			while (std::getline(file, line))
			{

				std::istringstream ss(line);
				std::string id, full_name, average_score, speciality;

				std::getline(ss, id, ',');
				std::getline(ss, full_name, ',');
				std::getline(ss, average_score, ',');
				std::getline(ss, speciality, ',');

				std::cout << "ID: " << id << ", Full name: " << full_name << ", Average score: " << average_score << ", Speciality: " << speciality << "\n";

			}

			file.close();

		}
		else {
			std::cerr << "Error opening file.\n";
		}
	}



	void deleteStudent(int student_id) {

		system("CLS");

		if (!fileExists(table)) {

			std::cerr << "The file with students' data is missing. \n";
			return;

		}

		std::ifstream file(table);
		std::ofstream tempFile("./temp.csv");
		std::string line;
		bool recordFound = false;

		if (!file.is_open()) {

			std::cerr << "Error opening file. \n";
			return;

		}

		if (!tempFile.is_open()) {

			std::cerr << "Error opening temporary file. \n";
			return;

		}

		std::getline(file, line);
		tempFile << line << "\n";

		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			std::string id_str;
			std::getline(ss, id_str, ',');

			if (std::stoi(id_str) != student_id) {

				tempFile << line << "\n";

			}
			else {
				recordFound = true;
			}
		}

		file.close();
		tempFile.close();

		std::string oldFile = std::filesystem::absolute(table).string();
		std::string newFile = std::filesystem::absolute("./temp.csv").string();

		if (recordFound) {

			std::remove(oldFile.c_str());
			std::rename(newFile.c_str(), oldFile.c_str());
			std::cout << "Record with id " << student_id << " successfully deleted.\n";

		}
		else {

			std::remove(newFile.c_str());
			std::cout << "Record with id " << student_id << " not found.\n";

		}
	}



	void findStudentByField() {

		system("CLS");

		if (!fileExists(table)) {

			std::cerr << "The file with students' data is missing. \n";
			return;

		}



		std::ifstream file(table);

		if (!file.is_open()) {

			std::cerr << "Error opening file. \n";
			return;

		}

		std::cout << "Enter search criteria: \n";
		std::cout << "1. ID \n";
		std::cout << "2. Full name \n";
		std::cout << "3. Average score \n";
		std::cout << "4. Speciality \n";
		int choice;
		std::cout << "Enter the criterion number (1-4): ";

		std::string field;
		
		do
		{
			std::cin >> choice;

			switch (choice)
			{

			case 1: field = "id"; break;
			case 2: field = "full_name"; break;
			case 3: field = "average_score"; break;
			case 4: field = "speciality"; break;

			default:
				std::cerr << "Incorrect choice. Please select a number from 1 to 4.\n";
				break;

			}

		} while (field.empty());
		


		std::string search_value;
		std::cout << "Enter value to search: ";
		std::cin.ignore();
		std::getline(std::cin, search_value);

		std::string line;
		std:getline(file, line);

		bool recordFound = false;

		while (std::getline(file, line))
		{

			std::istringstream ss(line);
			std::string id_str, name, score_str, speciality;
			std::getline(ss, id_str, ',');
			std::getline(ss, name, ',');
			std::getline(ss, score_str, ',');
			std::getline(ss, speciality, ',');

			if (field == "id" && id_str == search_value) {

				recordFound = true;
			}
			else if (field == "full_name" && name == search_value) {

				recordFound = true;
			}
			else if (field == "average_score" && score_str == search_value) {

				recordFound = true;
			}
			else if (field == "speciality" && speciality == search_value) {

				recordFound = true;
			}

			if (recordFound) {
				std::cout << "Record found: \n";
				std::cout << "ID: " << id_str << "\n";
				std::cout << "Full name: " << name << "\n";
				std::cout << "Average score: " << score_str << "\n";
				std::cout << "Speciality: " << speciality << "\n";
				break;
			}

		}

		if (!recordFound) {
			std::cout << "Record with the specified value not found. \n";

		}

		file.close();

	}
};



int Student::next_id = 1;

void showMenu();



int main() {

	Student student;
	int choice;
	bool running = true;

	while (running)
	{

		showMenu();
		std::cin >> choice;

		switch (choice)
		{
		case 1:

			std::cout << "Adding a new student:\n";
			student.setNewStudent();
			break;

		case 2:

			std::cout << "List of all students:\n";
			student.getStudentsList();
			break;

		case 3:

			std::cout << "Search for a student by criteria:\n";
			student.findStudentByField();
			break;

		case 4:

			int stud_id;
			std::cout << "Enter the student ID to delete:\n";
			std::cin >> stud_id;
			student.deleteStudent(stud_id);
			break;

		case 5:

			std::cout << "Exiting the program:\n";
			running = false;
			break;

		default:

			std::cout << "Wrong choice, try again:\n";
			break;

		}

	}

	return 0;

}




void showMenu() {
	std::cout << "\n=== ITMO Student Database ===\n";
	std::cout << "1. Add new student\n";
	std::cout << "2. Show all students\n";
	std::cout << "3. Find a student by criteria\n";
	std::cout << "4. Delete student by ID\n";
	std::cout << "5. Exit\n";
	std::cout << "Select an action: ";
}