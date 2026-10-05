#include "MaintenanceRecord.h"
#include <iostream>

// Constructor
MaintenanceRecord::MaintenanceRecord(
	std::string description,
	double cost,
	std::string date)
{
	this->description = description;
	this->cost = cost;
	this->date = date;
}

// Display maintenance information
void MaintenanceRecord::display()
{
	std::cout
		<< date
		<< " | "
		<< description
		<< " - $"
		<< cost
		<< std::endl;
}

std::string MaintenanceRecord::getDescription() const
{
	return description;
}

double MaintenanceRecord::getCost() const
{
	return cost;
}

std::string MaintenanceRecord::getDate() const
{
	return date;
}