#ifndef JSONCPP_H
#define JSONCPP_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

namespace JSONCpp {
	typedef struct JSON {
	public:
		std::string key, value;

		JSON(std::string key, std::string value) {
			this->key = key, this->value = value;
		}

		const std::pair<std::string, std::string> GetJSON() const { return std::pair<std::string, std::string>(key, value); }
		const std::string GetJSONString() const { return std::string("\"" + key + "\" : " + "\"" + value + "\""); }
		const std::string FindValueByKey(const std::string key) const { return value; }
	} Json, JSON;

	typedef struct NestedJSON {
	private:
		std::string mainKey;
		std::pair<std::string, std::vector<JSON>> nestedJSON;
		std::pair<std::string, std::vector<NestedJSON>> complexJSON;
	public:
		NestedJSON() { this->mainKey = ""; };
		NestedJSON(std::string mainKey) { this->mainKey = mainKey; };

		void AddJSON(const JSON& json) { nestedJSON.second.push_back(json); }
		void AddJSON(const NestedJSON& json) {
			complexJSON.second.push_back(json);
		}

		const std::string GetKey() const { return mainKey; }
		const std::vector<JSON> GetJSONValues() { return nestedJSON.second; }
		const std::vector<NestedJSON> GetComplexJSONValues() { return complexJSON.second; }
		const std::pair<std::string, std::vector<JSON>> GetJSON() const { return nestedJSON; }
		const std::string FindValueByKey(const std::string& mainKey, const std::string& subKey) const {
			if (this->mainKey == mainKey) {
				int counter = 0;
				while (counter < nestedJSON.second.size()) {
					if (nestedJSON.second.at(counter).key == subKey) {
						return nestedJSON.second.at(counter).FindValueByKey(subKey);
					}

					counter++;
				}

			std::cout << "Could not find subkey \"" << subKey << "\" of main key \"" << mainKey << std::endl;
			return "";

			} else {
				std::cout << "Could not find main key: " << mainKey << std::endl;
				return "";
			}


		}

	} NestedJSON, NestedJson;

	typedef struct JSONObject {
	private:
		std::string mainKey;

		std::vector<JSON> simpleJSON;
		std::vector<NestedJSON> complexJSON;
		std::vector<std::string> completeJSON;

		std::string jsonObjString;
	public:
		JSONObject() { this->mainKey = ""; }
		JSONObject(std::string mainKey) { this->mainKey = mainKey; }

		void AddJSON(const JSON& json) {
			simpleJSON.push_back(json);
		}

		void AddJSON(const NestedJSON& json) {
			complexJSON.push_back(json);
		}

		void FormatObject() {
			std::string tab = "\t";
			std::string newLine = "\n";
			std::string quote = "\"";
			std::string colon = " : ";
			std::string comma = ",";
			std::string openBracket = "{", closeBracket = "}";

			completeJSON.push_back(newLine + tab + quote + mainKey + quote + colon + openBracket + newLine);

			// Key value pairs
			// Simple parsing
			for (int index = 0; index < simpleJSON.size(); index++) {
				if (index != simpleJSON.size() - 1)
					completeJSON.push_back(tab + tab + tab + simpleJSON[index].GetJSONString() + comma + newLine);
				else {
					if (complexJSON.size() > 0)
						completeJSON.push_back(tab + tab + tab + simpleJSON[index].GetJSONString() + comma + newLine);
					else
						completeJSON.push_back(tab + tab + tab + simpleJSON[index].GetJSONString() + newLine);
				}
			}

			// Complex json formatting
			for (int index = 0; index < complexJSON.size(); index++) { // For all pairs in the vector
				completeJSON.push_back(newLine + tab + tab + tab + quote + complexJSON.at(index).GetKey() + quote + colon + openBracket + newLine); // Parse the main key
				if (index != complexJSON.size() - 1) { // If not the last object in the vector
					for (int pair = 0; pair < complexJSON.at(index).GetJSONValues().size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
						if (pair != complexJSON.at(index).GetJSONValues().size() - 1) {
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + comma + newLine);
						} else { // If last one
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + newLine);
						}
					}
				}
				else {
					for (int pair = 0; pair < complexJSON.at(index).GetJSONValues().size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
						if (pair != complexJSON.at(index).GetJSONValues().size() - 1) {
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + comma + newLine);
						} else {
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + newLine);
						}
					}
				}

				if (index != complexJSON.size() - 1) {
					completeJSON.push_back(tab + tab + tab + closeBracket + comma + newLine);
				}
				else {
					completeJSON.push_back(tab + tab + tab + closeBracket + newLine );
				}
			}

			completeJSON.push_back(tab + closeBracket);

			for (int index = 0; index < completeJSON.size(); index++)
				jsonObjString.append(completeJSON[index]);
		}

		const std::string GetJSONObjectString() const {
			return jsonObjString;
		}
		const std::string GetMainKey() const { return mainKey; }
	
		const std::vector<JSON> GetJSON() const { return simpleJSON; }
		const std::vector<NestedJSON> GetComplexJSON() const { return complexJSON; }

	} JsonObject, JSONObject;

	typedef struct JSONFileWriter {
	private:
		std::fstream file;

		std::string filePath;
		std::string fileContent;
		std::vector<JSONObject> jsonObjects;
	public:
		JSONFileWriter() {
			this->filePath = "JSONFileWriter.json";
		}
		JSONFileWriter(const std::string& filePath) { this->filePath = filePath; }
		void AddJSONObject(const JSONObject& jsonObject) { jsonObjects.push_back(jsonObject); }
		void Format() {
			file.open(filePath, std::ios::out | std::ios::trunc);

			if (!file.is_open()) {
				std::cout << "Failed to open file for writing jsonObject(s): " << filePath << std::endl;
				return;
			}

			std::string comma = ",";
			std::string openBracket = "{", closeBracket = "}";
			std::string tab = "\t";
			std::string newLine = "\n";

			fileContent.append(openBracket + newLine);

			for (int index = 0; index < jsonObjects.size(); index++) {
				if (index != jsonObjects.size() - 1)
					fileContent.append(tab + jsonObjects.at(index).GetJSONObjectString() + comma + newLine);
				else
					fileContent.append(tab + jsonObjects.at(index).GetJSONObjectString() + newLine + newLine);
			}

			fileContent.append(closeBracket);

			file << fileContent;
			file.flush();
			file.close();

		}

	} JSONFileWriter, JSONFileWriter;

	typedef struct JSONFileReader {
	private:
		std::ifstream file;
		std::string fileContent;
		const char* filePath;
		std::vector<std::string> keyPairs;

		void ReadFile() {
			file.open(filePath, std::ios::out);

			if (!file.is_open()) {
				std::cout << "Failed to open file for reading: " << filePath << std::endl;
				return;
			}

			std::string code = "";

			while (getline(file, code)) {
				fileContent.append(code + "\n");
			}

			file.close();
		}
		void ParseKeyValuePairs() {
			const char* jsonData = fileContent.c_str();

			bool isOpenQuote = false;
			int characterIndex = 0, infoIndex = -1;

			while (characterIndex < strlen(jsonData)) {
				switch (*++jsonData) {
				case '"': {
					if (isOpenQuote) { // If qoute is open but finds another
						isOpenQuote = false; // close the qoute

					}
					else {
						isOpenQuote = true; // If found open qoute
						keyPairs.push_back(std::string(""));
						infoIndex++;
					}

				} break;
				case '\n':
					break;

				case '\t':
					break;

				case '{':
					break;

				case '}':
					break;

				case ':':
					break;

				case ',':
					break;

				default: {
					if (isOpenQuote) {
						if (keyPairs.size() > 0) {
							keyPairs.at(infoIndex) += jsonData[characterIndex];
						}
					}
				} break;
					characterIndex++;
				}
			}
		}

		const int DidFindKey(const char* key) const {
			for (int index = 0; index < keyPairs.size(); index++) {
				if (strcmp(keyPairs.at(index).c_str(), key) == 0)
					return index;
				else
					continue;
			}
			return -1;
		}

	public:
		JSONFileReader(const char* filePath) {
			this->filePath = filePath;
			ReadFile();
			ParseKeyValuePairs();

		}

		const std::string GetValueForKey(const char* jsonObjectName, const char* key) const {
			int i = DidFindKey(jsonObjectName);
			int j = DidFindKey(key) + i;

			if (i > -1 && j > -1)
				return keyPairs.at(j + 1);

			std::cout << "Could not find '" << jsonObjectName << "' or '" << key << "'." << std::endl;
			return "";
		}

		// Test with anothe object in file.
		const std::string GetValueForKey(const char* jsonObjectName, const char* subObjectName, const char* key) const {
			int i = DidFindKey(jsonObjectName);
			int j = DidFindKey(subObjectName) + (i - 1);
						
			if (i > -1 && j > -1) {
				for (int k = j; k < keyPairs.size(); k++) {
					if (strcmp(keyPairs.at(k).c_str(), key) == 0)
						return keyPairs.at(k + 1);
					else
						continue;
				}
			}
			
			std::cout << "Could not find one or more keys of '" << jsonObjectName << "','"  << subObjectName << "','" << key << "'." << std::endl;
			return "";
		}
	} JSONFileReader, JSONFileReader;

	float StringToFloat(std::string str) {
		return atof(str.c_str());
	}

	int StringToInteger(std::string str) {
		return atoi(str.c_str());
	}

	double StringToDouble(std::string str) {
		return atof(str.c_str());
	}

}

#endif
