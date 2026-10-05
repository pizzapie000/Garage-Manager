#ifndef MAINTENANCE_RECORD_H
#define MAINTENANCE_RECORD_H

#include <string>

class MaintenanceRecord
{
private:
	std::string description;
	double cost;
	std::string date;

public:
	MaintenanceRecord(
		std::string description,
		double cost,
		std::string date
	);

	void display();

	// Getters used for save/load
	std::string getDescription() const;
	double getCost() const;
	std::string getDate() const;
};

#endif