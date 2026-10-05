#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>

#include "MaintenanceRecord.h"

class Vehicle
{
private:
	std::string make;
	std::string model;
	int year;

	std::vector<MaintenanceRecord> records;

public:
	Vehicle(
		std::string make,
		std::string model,
		int year);

	void displayInfo();

	void addRecord(
		MaintenanceRecord record);

	void displayRecords();

	std::string getVehicleName() const;

	// Getters used by save/load logic (const so they can be called on const Vehicle&)
	std::string getMake() const;
	std::string getModel() const;
	int getYear() const;

	// Access records for saving/loading
	const std::vector<MaintenanceRecord>& getRecords() const;
};

#endif
#include <iostream>
#include <vector>
#include "Vehicle.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string escape_commas(const std::string& s)
{
	std::string out;
	out.reserve(s.size());
	for (char c : s)
	{
		if (c == ',')
			out.push_back(';'); // simple escape: replace comma with semicolon
		else if (c == '\n' || c == '\r')
			continue;
		else
			out.push_back(c);
	}
	return out;
}

int main()
{
	std::vector<Vehicle> garage;

	bool running = true;

	while (running)
	{
		std::cout << "GARAGE MANAGER\n";
		std::cout << "1. Add Vehicle\n";
		std::cout << "2. View Vehicles\n";
		std::cout << "3. Add Maintenance Record\n";
		std::cout << "4. View Maintenance Records\n";
		std::cout << "5. Save Garage\n";
		std::cout << "6. Load Garage\n";
		std::cout << "7. Exit\n";

		int choice;
		if (!(std::cin >> choice))
		{
			std::cin.clear();
			std::cin.ignore(1000, '\n');

			std::cout << "Please enter a number.\n";

			continue;
		}

		switch (choice)
		{
		case 1:
		{
			std::string make;
			std::string model;
			int year;

			std::cout << "Make: ";
			std::cin >> make;

			std::cout << "Model: ";
			std::cin >> model;

			std::cout << "Year: ";
			if (!(std::cin >> year))
			{
				std::cin.clear();
				std::cin.ignore(1000, '\n');

				std::cout << "Please enter a number.\n";
				break;
			}

			if (year < 1886 || year > 2100)
			{
				std::cout << "Invalid year.\n";
				break;
			}

			garage.push_back(
				Vehicle(make, model, year));

			std::cout << "Vehicle Added Successfully!\n";

			break;
		}

		case 2:
		{
			for (Vehicle& vehicle : garage)
			{
				vehicle.displayInfo();
			}

			break;
		}

		case 3:
		{
			if (garage.empty())
			{
				std::cout << "No vehicles added yet.\n";
				break;
			}

			std::cout << "\nVehicles:\n";

			for (int i = 0; i < static_cast<int>(garage.size()); i++)
			{
				std::cout << i
					<< ". "
					<< garage[i].getVehicleName()
					<< std::endl;
			}

			int vehicleIndex;

			std::cout << "Vehicle Number: ";
			if (!(std::cin >> vehicleIndex))
			{
				std::cin.clear();
				std::cin.ignore(1000, '\n');

				std::cout << "Please enter a number.\n";
				break;
			}

			// Prevents any crashing from invalid inputs
			if (vehicleIndex < 0 ||
				vehicleIndex >= static_cast<int>(garage.size()))
			{
				std::cout << "Invalid vehicle number.\n";
				break;
			}

			std::string date;
			std::string description;
			double cost;

			std::cin.ignore(1000, '\n');

			std::cout << "Date (YYYY-MM-DD): ";
			std::getline(std::cin, date);

			std::cout << "Description: ";
			std::getline(std::cin, description);

			if (description.empty())
			{
				std::cout << "Description cannot be empty.\n";
				break;
			}

			std::cout << "Cost: ";
			if (!(std::cin >> cost))
			{
				std::cin.clear();
				std::cin.ignore(1000, '\n');

				std::cout << "Please enter a number.\n";
				break;
			}

			if (cost < 0)
			{
				std::cout << "Cost cannot be negative.\n";
				break;
			}

			MaintenanceRecord record(
				description,
				cost,
				date);

			garage[vehicleIndex].addRecord(record);

			std::cout << "Maintenance Record Added!\n";

			break;
		}

		case 4:
		{
			if (garage.empty())
			{
				std::cout << "No vehicles added yet.\n";
				break;
			}

			for (Vehicle& vehicle : garage)
			{
				std::cout << "\n"
					<< vehicle.getVehicleName()
					<< std::endl;

				vehicle.displayRecords();
			}

			break;
		}

		case 5: // Save Garage (vehicles + records)
		{
			std::ofstream file("garage.txt");

			if (!file)
			{
				std::cout << "Could not save file.\n";
				break;
			}

			// Format:
			// V,year,make,model,recordCount
			// R,date,description,cost
			for (const Vehicle& vehicle : garage)
			{
				int rc = static_cast<int>(vehicle.getRecords().size());
				file << "V," << vehicle.getYear() << ","
					<< escape_commas(vehicle.getMake()) << ","
					<< escape_commas(vehicle.getModel()) << ","
					<< rc << "\n";

				for (const MaintenanceRecord& rec : vehicle.getRecords())
				{
					file << "R,"
						<< escape_commas(rec.getDate()) << ","
						<< escape_commas(rec.getDescription()) << ","
						<< rec.getCost() << "\n";
				}
			}

			file.close();

			std::cout << "Garage saved.\n";

			break;
		}

		case 6: // Load Garage (vehicles + records)
		{
			std::ifstream file("garage.txt");

			if (!file)
			{
				std::cout << "Could not open garage.txt for reading.\n";
				break;
			}

			garage.clear();

			std::string line;
			while (std::getline(file, line))
			{
				if (line.empty())
					continue;

				std::stringstream ss(line);
				std::string token;
				if (!std::getline(ss, token, ','))
					continue;

				if (token == "V")
				{
					std::string yearStr, make, model, recordCountStr;
					if (!std::getline(ss, yearStr, ','))
						continue;
					if (!std::getline(ss, make, ','))
						continue;
					if (!std::getline(ss, model, ','))
						continue;
					if (!std::getline(ss, recordCountStr))
						continue;

					int year = 0;
					int recordCount = 0;
					try
					{
						year = std::stoi(yearStr);
						recordCount = std::stoi(recordCountStr);
					}
					catch (...)
					{
						continue;
					}

					garage.emplace_back(make, model, year);

					// read following recordCount lines
					for (int i = 0; i < recordCount; ++i)
					{
						if (!std::getline(file, line))
							break;

						std::stringstream rs(line);
						std::string rtoken;
						if (!std::getline(rs, rtoken, ','))
							continue;
						if (rtoken != "R")
							continue;

						std::string date, description, costStr;
						if (!std::getline(rs, date, ','))
							continue;
						if (!std::getline(rs, description, ','))
							continue;
						if (!std::getline(rs, costStr))
							continue;

						double cost = 0.0;
						try
						{
							cost = std::stod(costStr);
						}
						catch (...)
						{
							continue;
						}

						MaintenanceRecord rec(description, cost, date);
						garage.back().addRecord(rec);
					}
				}
				else
				{
					// Backwards compatible: legacy line may be year,make,model (no records)
					std::string rest = line;
					std::stringstream ls(rest);
					std::string yearStr, make, model;
					if (!std::getline(ls, yearStr, ','))
						continue;
					if (!std::getline(ls, make, ','))
						continue;
					if (!std::getline(ls, model))
						continue;

					try
					{
						int year = std::stoi(yearStr);
						garage.emplace_back(make, model, year);
					}
					catch (...)
					{
						continue;
					}
				}
			}

			file.close();

			std::cout << "Garage loaded.\n";

			break;
		}

		case 7:
		{
			running = false;
			break;
		}

		default:
		{
			std::cout << "Invalid choice.\n";
			break;
		}
		}
	}

	return 0;
}