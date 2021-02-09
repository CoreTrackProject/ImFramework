#pragma once

#include <string>
#include <map>
#include <fstream>
#include <algorithm>

class ImProperty {

public:
	template<typename T>
	static void SetValue(std::string property_name, T value) {

		std::string entry = std::to_string(value);
		ImProperty::removeComma(entry);

		if (ImProperty::config_data.find(property_name) == ImProperty::config_data.end()) {
			// key not found
			ImProperty::config_data.emplace(property_name, std::to_string(value));
		} else {
			// key found
			ImProperty::config_data[property_name] = std::to_string(value);
		}
	}

	template<>
	static void SetValue(std::string property_name, std::string value) {
		ImProperty::removeComma(value);
		if (ImProperty::config_data.find(property_name) == ImProperty::config_data.end()) {
			// key not found
			ImProperty::config_data.emplace(property_name, value);
		} else {
			// key found
			ImProperty::config_data[property_name] = value;
		}
	}

	template<>
	static void SetValue(std::string property_name, bool value) {
		if (ImProperty::config_data.find(property_name) == ImProperty::config_data.end()) {
			// key not found
			ImProperty::config_data.emplace(property_name, value ? "true" : "false");
		}
		else {
			// key found
			ImProperty::config_data[property_name] = value ? "true" : "false";
		}
	}

	template<typename T>
	static T GetValue(std::string property_name) {
		if (ImProperty::config_data.find(property_name) == ImProperty::config_data.end()) {
			return T();
		} else {
			return ImProperty::StringToType<T>(ImProperty::config_data[property_name]);
		}
	}

	template<>
	static std::string GetValue(std::string property_name) {
		if (ImProperty::config_data.find(property_name) == ImProperty::config_data.end()) {
			return "";
		} else {
			return ImProperty::config_data[property_name];
		}
	}

	template<>
	static bool GetValue(std::string property_name) {
		if (ImProperty::config_data.find(property_name) == ImProperty::config_data.end()) {
			return "";
		}
		else {
			return ImProperty::config_data[property_name].compare("true") == 0;
		}
	}

	static void Save(std::string file = "") {

		std::string curr_file = "";
		if (file.compare("") == 0) {
			curr_file = ImProperty::config_file;
		}

		
		std::string output_str;

		for (std::pair<std::string, std::string> entry : ImProperty::config_data) {
			output_str.append(entry.first + "," + entry.second + "\n");
		}

		std::ofstream file_stream;
		file_stream.open(curr_file, std::ios::out | std::ios::trunc);
		file_stream.write(output_str.c_str(), output_str.size());
		file_stream.close();
	}

	static void Load(std::string file) {

		ImProperty::config_file = file;
		std::ifstream file_stream(file);
		std::map<std::string, std::string> tmp_config_data;

		if (!file_stream.fail()) {

			std::string key_str;

			while (std::getline(file_stream, key_str, ',')) {

				std::string value_str;
				std::getline(file_stream, value_str, '\n');

				tmp_config_data.emplace(key_str, value_str);
			}

		}
		ImProperty::config_data = std::move(tmp_config_data);

	}

	static void Clear() {
		ImProperty::config_data.clear();
	}

private:
	template<typename T>
	static T StringToType(std::string value) {
		if (std::is_same<T, int>::value) {
			return std::stoi(value);
		}
		if (std::is_same<T, unsigned int>::value) {
			return std::stoul(value);
		}
		if (std::is_same<T, float>::value) {
			return std::stof(value);
		}
		if (std::is_same<T, double>::value) {
			return std::stod(value);
		}
		if (std::is_same<T, bool>::value) {
			return std::stoi(value);
		}
		return T();
	}

	static void removeComma(std::string& str) {
		std::replace(str.begin(), str.end(), ',', '_');
	}
	

private:
	static std::map<std::string, std::string> config_data;
	static std::string config_file;
};