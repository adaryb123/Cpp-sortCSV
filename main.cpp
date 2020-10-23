#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> column_sequence;

class Row {
public:
	Row(std::vector<std::string> values) : m_row_values(values) {}
	std::vector<std::string> get_values()
	{
		return m_row_values;
	}

	std::string get_value_at(size_t position)
	{
		return m_row_values[position];
	}

	friend std::ostream& operator<<(std::ostream& os, Row row);
	friend bool operator<(Row& first, Row& second);
private:
	std::vector<std::string> m_row_values;
};

std::ostream& operator<<(std::ostream& os, Row row)
{
	for (size_t i = 0; i < row.get_values().size(); i++)
	{
		if (i != 0)
			os << ",";
		os << row.get_values()[i];
	}
	os << "\n";
	return os;
}

bool operator< (Row& first_row, Row& second_row)
{
	for (size_t i = 0; i < column_sequence.size(); i++)
	{
		std::string first, second;
		first = first_row.get_value_at(static_cast<size_t>(column_sequence[i]) - 1);
		second = second_row.get_value_at(static_cast<size_t>(column_sequence[i]) - 1);
		if (first.compare(second) < 0)
			return true;
		else if (first.compare(second) == 0)
			continue;
		else
			return false;
	}
	return false;
}

bool read_file_row(const std::string& fileline, std::vector<std::string>& row_values, int& col_num)
{
	int current_col_num = 0;

	std::string buffer = "";
	for (size_t i = 0; i < fileline.size(); i++)
	{
		if (fileline[i] == ',')
		{
			row_values.push_back(buffer);
			buffer.clear();
			current_col_num++;
		}
		else
		{
			buffer += fileline[i];
		}
	}
	row_values.push_back(buffer);
	buffer.clear();
	current_col_num++;

	if (col_num == -1)
		col_num = current_col_num;
	
	if (current_col_num != col_num)
		return false;
	else
		return true;
}

bool read_parameters(const int& argc, char* argv[], std::string& input_file_name, std::string& output_file_name, std::string& column_sequence_string)
{
	if (argc != 4)
		return false;

	input_file_name = argv[1];
	output_file_name = argv[2];
	column_sequence_string = argv[3];
	return true;
}

bool transform_sequence_to_vector(const std::string& sequence_string, std::vector<int>& sequence_vector)
{
	std::string buffer = "";
	for (size_t i = 0; i < sequence_string.size(); i++)
	{
		if (sequence_string[i] == ',')
		{
			if (buffer.empty())
				return false;
			else
			{
				int temp = std::stoi(buffer);
				if (!sequence_vector.empty() && std::find(sequence_vector.begin(), sequence_vector.end(), temp) != sequence_vector.end())
					return false;
				sequence_vector.push_back(temp);
				buffer.clear();
			}
		}
		else if (sequence_string[i] < '0' || sequence_string[i] > '9')
			return false;
		else
		{
			if (sequence_string[i] == '0' && buffer.empty())
				return false;
			else
				buffer += sequence_string[i];
		}
	}
	if (buffer.empty())
		return false;
	int temp = std::stoi(buffer);
	if (!sequence_vector.empty() && std::find(sequence_vector.begin(), sequence_vector.end(), temp) != sequence_vector.end())
		return false;
	sequence_vector.push_back(temp);
	return true;
}

int main(int argc, char* argv[])
{
	std::string input_file_name, output_file_name, column_sequence_string;
	if (!read_parameters(argc, argv, input_file_name, output_file_name, column_sequence_string))
	{
		std::cout << "Error: invalid parameters!\n";
		return 1;
	}

	std::ifstream input_file(input_file_name);
	std::ofstream output_file(output_file_name);
	if (!input_file.good() || !output_file.good())
	{
		std::cout << "Error: invalid input or output file!\n";
		return 1;
	}

	if (!transform_sequence_to_vector(column_sequence_string, column_sequence)) {
		std::cout << "Error: invalid sequence of columns (3rd parameter)!\n";
		return 1;
	}

	int column_count = -1;
	std::vector<Row> table;
	while (true)
	{
		std::string fileline;
		std::vector<std::string> row_values;
		if (!std::getline(input_file, fileline))
			break;

		if (!read_file_row(fileline, row_values, column_count))
		{
			std::cout << "Error: different number of columns in this row!\n";
			return 1;
		}
		table.push_back(Row{ row_values });
	}

	if (!input_file.eof())
	{
		std::cout << "Error: couldnt read whole input file!\n";
		return 1;
	}
	std::sort(table.begin(), table.end());

	for (size_t i = 0; i < table.size(); i++)
		output_file << table[i];

	return 0;
}