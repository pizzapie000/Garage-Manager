#include "Vehicle.h"
#include <iostream>

// Constructor
// Creates a vehicle object and stores the vehicle information.
Vehicle::Vehicle(
	std::string make,
	std::string model,
	int year)
{
	this->make = make;
	this->model = model;
	this->year = year;
}

// Displays the vehicle information.
void Vehicle::displayInfo()
{
	std::cout
		<< year << " "
		<< make << " "
		<< model
		<< std::endl;
}

// Adds a maintenance record to this vehicle.
void Vehicle::addRecord(
	MaintenanceRecord record)
{
	records.push_back(record);
}

// Displays all maintenance records for this vehicle.
void Vehicle::displayRecords()
{
	if (records.empty())
	{
		std::cout << "No maintenance records found.\n";
		return;
	}

	std::cout << "\nMaintenance Records:\n";

	for (MaintenanceRecord& record : records)
	{
		record.display();
	}
}

// Returns the full vehicle name as a string.
// This helps when displaying multiple vehicles because they are numbered.
std::string Vehicle::getVehicleName() const
{
	return std::to_string(year)
		+ " "
		+ make
		+ " "
		+ model;
}

std::string Vehicle::getMake() const
{
	return make;
}

std::string Vehicle::getModel() const
{
	return model;
}

int Vehicle::getYear() const
{
	return year;
}

const std::vector<MaintenanceRecord>& Vehicle::getRecords() const
{
	return records;
}