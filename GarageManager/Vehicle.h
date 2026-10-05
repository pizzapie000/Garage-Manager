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