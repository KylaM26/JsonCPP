#ifndef JSONCPP_H
#define JSONCPP_H

#include <iostream>
#include <vector>
#include <fstream>
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

	typedef struct ComplexJSON {
	private:
		std::string mainKey;
		std::pair<std::string, std::vector<JSON>> complexJSON;
	public:
		ComplexJSON() { this->mainKey = ""; };
		ComplexJSON(std::string mainKey) { this->mainKey = mainKey; };

		void AddJSON(const JSON& json) { complexJSON.second.push_back(json); }

		const std::string GetKey() const { return mainKey; }
		const std::vector<JSON> GetJSONValues() { return complexJSON.second; }
		const std::pair<std::string, std::vector<JSON>> GetJSON() const { return complexJSON; }
		const std::string FindValueByKey(const std::string& mainKey, const std::string& subKey) const {
			if (this->mainKey == mainKey) {
				int counter = 0;
				while (counter < complexJSON.second.size()) {
					if (complexJSON.second.at(counter).key == subKey) {
						return complexJSON.second.at(counter).FindValueByKey(subKey);
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

	} ComplexJSON, ComplexJson;

	typedef struct JSONObject {
	private:
		std::string mainKey;

		std::vector<JSON> simpleJSON;
		std::vector<ComplexJSON> complexJSON;
		std::vector<std::string> completeJSON;

		std::string jsonObjString;
	public:
		JSONObject() { this->mainKey = ""; }
		JSONObject(std::string mainKey) { this->mainKey = mainKey; }

		void AddJSON(const JSON& json) {
			simpleJSON.push_back(json);
		}

		void AddJSON(const ComplexJSON& json) {
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

			// Complex parsing
			for (int index = 0; index < complexJSON.size(); index++) { // For all pairs in the vector
				completeJSON.push_back(newLine + tab + tab + tab + quote + complexJSON.at(index).GetKey() + quote + colon + openBracket + newLine); // Parse the main key
				if (index != complexJSON.size() - 1) { // If not the last object in the vector
					for (int pair = 0; pair < complexJSON.at(index).GetJSONValues().size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
						if (pair != complexJSON.at(index).GetJSONValues().size() - 1)
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + comma + newLine);
						else
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + newLine);
					}
				}
				else {
					for (int pair = 0; pair < complexJSON.at(index).GetJSONValues().size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
						if (pair != complexJSON.at(index).GetJSONValues().size() - 1)
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + comma + newLine);
						else
							completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + newLine);
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
		const std::vector<ComplexJSON> GetComplexJSON() const { return complexJSON; }

	} JsonObject, JSONObject;

	typedef struct JSONFile {
	private:
		std::fstream file;

		std::string filePath;
		std::string fileContent;

		std::vector<JSONObject> jsonObjects;
		std::vector<std::pair<std::string, std::string>> simpleJSONContent;
		std::vector<std::pair<std::string, std::vector<JSON>>> complexJSONContent;

	public:
		JSONFile() {
			this->filePath = "jsonfile.json";
		}
		JSONFile(const std::string& filePath) { this->filePath = filePath; }
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

		const std::string FindValueByKeys(const std::string& mainKey, const std::string& subKey) {
			if (fileContent.find(mainKey) != std::string::npos) {
				for (int i = 0; i < jsonObjects.size(); i++) {
					if (jsonObjects.at(i).GetMainKey() == mainKey) {
						if (jsonObjects.at(i).GetJSONObjectString().find(subKey) != std::string::npos) {
							for (int j = 0; j < jsonObjects.at(i).GetJSON().size(); j++) {
								return jsonObjects.at(i).GetJSON().at(j).FindValueByKey(subKey);
							}			
						} else {
							std::cout << "Could not find subkey \"" << subKey << "\" of main key \"" << mainKey << std::endl;
							return "";
						}
					}
				}
			} else {
				std::cout << "Could not find main key: " << mainKey << std::endl;
				return "";
			}
		}
	
		const std::string FindValueByKeys(const std::string& mainKey, const std::string& subKey, const std::string& finalKey) {
			if (fileContent.find(mainKey) != std::string::npos) {
				for (int i = 0; i < jsonObjects.size(); i++) {
					if (jsonObjects.at(i).GetMainKey() == mainKey) {
						if (jsonObjects.at(i).GetJSONObjectString().find(subKey) != std::string::npos) {
							for (int j = 0; j < jsonObjects.at(i).GetComplexJSON().size(); j++) {
								if (jsonObjects.at(i).GetComplexJSON().at(j).GetKey() == subKey) {
									return jsonObjects.at(i).GetComplexJSON().at(j).FindValueByKey(subKey, finalKey);
								}
							}
						}
						else {
							std::cout << "Could not find subkey \"" << subKey << "\" of main key \"" << mainKey << std::endl;
							return "";
						}
					}
				}
			}
			else {
				std::cout << "Could not find main key: " << mainKey << ", file: " <<  std::endl;
				return "";
			}
		}

	} JSONFile, JsonFile;

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
