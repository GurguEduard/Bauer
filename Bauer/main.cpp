#define _CRT_SECURE_NO_WARNINGS   // must be the FIRST line, before any #include, is here because MSVC refuses to compile fopen()
#include <iostream>

using std::cin;


int current_year = 1;


double constant_growth;
double variable_growth;
double sector1_share; // used for the transfer between sectors and years

// rate of surplus value s/v (Bauer: 100 %)
double surplus_rate;

double table[3][7];

double& constant1 = table[0][0];
double& variable1 = table[0][1];
double& constant2 = table[1][0];
double& variable2 = table[1][1];

double& total_constant = table[2][0];
double& total_variable = table[2][1];

double& surplus1 = table[0][2];
double& surplus2 = table[1][2];

double& total_surplus = table[2][2];

double& consumption1 = table[0][3];
double& consumption2 = table[1][3];

double& total_consumption = table[2][3];

double& acc_constant1 = table[0][4];
double& acc_variable1 = table[0][5];
double& acc_constant2 = table[1][4];
double& acc_variable2 = table[1][5];

double& total_accumulated_constant = table[2][4];
double& total_accumulated_variable = table[2][5];

double& anual_product1 = table[0][6];
double& anual_product2 = table[1][6];
double& total_anual_product = table[2][6];

void set_total_starting_capital();
void set_surplus();
void set_accumulated();
void set_consumption();
void set_anual_product();
double get_profit_percentage();
FILE* open_export(const char* path);

void input_table()
{
	bool input_received = false;
	int input_mode;

	while (!input_received)
	{
		printf("Input '1' for default values,'2' for custom values or '0' to exit: ");
		cin >> input_mode;

		switch (input_mode)
		{
		case 1:
			constant1 = 120;
			variable1 = 50;
			constant2 = 80;
			variable2 = 50;

			constant_growth = 0.10;
			variable_growth = 0.05;
			surplus_rate = 1;

			input_received = true;

			break;

		case 2:
			printf("Input value of constant capital for sector 1: ");
			cin >> constant1;
			printf("Input value of variable capital for sector 1: ");
			cin >> variable1;
			printf("Input value of constant capital for sector 2: ");
			cin >> constant2;
			printf("Input value of variable capital for sector 2: ");
			cin >> variable2;
			printf("Input value of constant capital growth rate: ");
			cin >> constant_growth;
			printf("Input value of variable capital growth rate: ");
			cin >> variable_growth;
			printf("Input value of surplus rate: ");
			cin >> surplus_rate;

			input_received = true;

			break;

		case 0:
			std::exit(EXIT_SUCCESS);

		default:
			printf("Invalid input\n");
			continue;
		}
	}

	printf("\n");
}

void input_table(double c1, double v1, double c2, double v2)
{
	constant1 = c1;
	variable1 = v1;
	constant2 = c2;
	variable2 = v2;
}

void output_table()
{
	printf("Year %-5d%-10c%-10c%-10c%-10c%-10s%-10s%-10s\n",current_year, 'c', 'v', 's', 'k', "ac", "av", "AV");
	
	for (int i = 0; i < 3; i++)
	{
		if (i < 2) printf("Sector %-3d", i + 1);
		else printf("Total     ");
		
		for (int j = 0; j < 7; j++)
		{
			printf("%-10.3f", table[i][j]);
		}
		printf("\n");
	}

	printf("Rate of Profit: %.3f%%\n", get_profit_percentage());
}

double get_profit_percentage()
{
	return ((total_consumption + total_accumulated_constant + total_accumulated_variable) / (total_constant + total_variable)) * 100;
}

void compute_table_values()
{
	set_total_starting_capital();
	set_surplus();
	set_accumulated();
	set_consumption();
	set_anual_product();

	sector1_share = (1 + constant_growth) * total_accumulated_constant / (total_accumulated_constant + (1 + surplus_rate) * total_accumulated_variable);
}

void set_total_starting_capital()
{
	total_constant = constant1 + constant2;
	total_variable = variable1 + variable2;
}

void set_surplus()
{
	surplus1 = surplus_rate * variable1;
	surplus2 = surplus_rate * variable2;

	total_surplus = surplus1 + surplus2;
}

void set_accumulated()
{
	total_accumulated_constant = constant_growth * total_constant;
	total_accumulated_variable = variable_growth * total_variable;

	acc_variable1 = variable_growth * variable1;
	acc_variable2 = variable_growth * variable2;

	acc_constant1 = total_accumulated_constant * surplus1 / total_surplus;
	acc_constant2 = total_accumulated_constant * surplus2 / total_surplus;
}

void set_consumption()
{
	total_consumption = total_surplus - total_accumulated_constant - total_accumulated_variable;

	consumption1 = total_consumption / total_surplus * surplus1;
	consumption2 = total_consumption / total_surplus * surplus2;
}

void set_anual_product()
{
	anual_product1 = constant1 + variable1 + surplus1;
	anual_product2 = constant2 + variable2 + surplus2;
	total_anual_product = anual_product1 + anual_product2;
}

void turn()
{
	constant1 += sector1_share * total_accumulated_constant;
	variable1 += sector1_share * total_accumulated_variable;
	constant2 += (1 - sector1_share) * total_accumulated_constant;
	variable2 += (1 - sector1_share) * total_accumulated_variable;
}


FILE* open_export(const char* path)
{
	FILE* file = fopen(path, "w");
	if (file == nullptr)
	{
		printf("Failed to create data export file\n");
		return nullptr;
	}
	
	fprintf(file,"year,c1,v1,s1,k1,ac1,av1,AV1,"
				 "c2,v2,s2,k2,ac2,av2,AV2,"
				 "c,v,s,k,ac,av,AV\n");
	return file;
}

void export_year(FILE* file, int current_year)
{
	fprintf(file, "%d", current_year);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 7; j++)
			fprintf(file, ",%.10g", table[i][j]);
	fprintf(file, "\n");
}


int main()
{
	input_table();

	FILE* file = open_export("bauer_run.csv");
	if (file == nullptr) return 1;

	do
	{
		compute_table_values();
		output_table();
		export_year(file, current_year);
		turn();
		printf("\n");

		current_year++;
	} while ((consumption1 >= 0 && consumption2 >= 0) && current_year <= 1000);

	fclose(file);
	
	if (system("py graphs.py") != 0)
	{
		printf("Error trying to launch python script. Python or one of its libraries (matplotlib and pandas) are probably not installed, otherwise Python has not been added to the system variables or the file 'graphs.py' is missing from the folder where this exe is located\n");
	}

	printf("Press ENTER to close");
	cin.ignore(10000, '\n');   // discard the newline left over from the menu input
	cin.get();
	return 0;

	// make it so that this project can check if matplotlib is installed and if it isn t it installs it
}