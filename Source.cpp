#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// void WriteJSONObjectToFile(std::ofstream& file, const char* path);

typedef struct JSON {
public:
	std::string key, value;

	JSON(std::string key, std::string value) {
		this->key = key, this->value = value;
	}

	const std::pair<std::string, std::string> GetJSON() const { return std::pair<std::string, std::string>(key, value); }
	const std::string GetJSONString() const { return std::string("\"" + key + "\" : " + "\"" + value + "\""); }
	const std::string FindValueByKey(const std::string key) { return value; }
} Json, JSON;

typedef struct ComplexJSON {
private:
	std::string mainKey;
	std::pair<std::string, std::vector<JSON>> complexJSON;
public:
	ComplexJSON() { this->mainKey = ""; };
	ComplexJSON(std::string mainKey) { this->mainKey = mainKey; };

	void AddJSON(const JSON& json) { complexJSON.second.push_back(json); }

	const std::string GetKey() { return mainKey; }
	const std::vector<JSON> GetJSONValues() { return complexJSON.second; }
	const std::pair<std::string, std::vector<JSON>> GetJSON() const { return complexJSON; }

} ComplexJSON, ComplexJson;

typedef struct JSONObject {
private:
	std::string mainKey;

	std::vector<JSON> simpleJSON;
	std::vector<ComplexJSON> complexJSON;
	//std::vector<std::pair<std::string, std::vector<JSON>>> complexJSON;
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
			completeJSON.push_back(tab + tab + tab + quote + complexJSON.at(index).GetKey() + quote + colon + openBracket + newLine); // Parse the main key
			if (index != complexJSON.size() - 1) { // If not the last object in the vector
				for (int pair = 0; pair < complexJSON.at(index).GetJSONValues().size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
					if (pair != complexJSON.at(index).GetJSONValues().size() - 1)
						completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + comma + newLine);
					else
						completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(index).GetJSONValues().at(pair).value + quote + newLine);
				}
			} else {
				for (int pair = 0; pair < complexJSON.at(index).GetJSONValues().size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
					if (pair != complexJSON.at(index).GetJSONValues().size() - 1)
						completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(complexJSON.size() - 1).GetJSONValues().at(pair).value + quote + comma + newLine);
					else
						completeJSON.push_back(tab + tab + tab + tab + quote + complexJSON.at(index).GetJSONValues().at(pair).key + quote + colon + quote + complexJSON.at(complexJSON.size() - 1).GetJSONValues().at(pair).value + quote + newLine);
				}
			}

			if (index != complexJSON.size() - 1) {
				completeJSON.push_back(tab + tab + tab + closeBracket + comma + newLine + newLine);
			}
			else {
				completeJSON.push_back(tab + tab + tab + closeBracket + newLine + newLine);
			}
		}

		completeJSON.push_back(tab + closeBracket);

		for (int index = 0; index < completeJSON.size(); index++)
			jsonObjString.append(completeJSON[index]);
	}

	const std::string GetJSONObjectString() const {
		return jsonObjString;
	}

} JsonObject, JSONObject;

typedef struct JSONFile {
private:
	std::fstream file;
	std::string filePath;
	std::vector<JSONObject> jsonObjects;
public:
	JSONFile(const std::string& filePath) { this->filePath = filePath; }
	void AddJSONObject(const JSONObject& jsonObject) { jsonObjects.push_back(jsonObject); }
	void Format() {
		file.open(filePath, std::ios::in | std::ios::out | std::ios::trunc);

		if (!file.is_open()) {
			std::cout << "Failed to open file for writing jsonObject(s): " << filePath << std::endl;
			return;
		}

		std::string comma = ",";
		std::string openBracket = "{", closeBracket = "}";
		std::string tab = "\t";
		std::string newLine = "\n";

		std::string fileContent;
		fileContent.append(openBracket + newLine);

		for (int index = 0; index < jsonObjects.size(); index++) {
			if (index != jsonObjects.size() - 1)
				fileContent.append(tab + jsonObjects.at(index).GetJSONObjectString() + comma + newLine + newLine);
			else
				fileContent.append(tab + jsonObjects.at(index).GetJSONObjectString() + newLine + newLine);
		}

		fileContent.append(closeBracket);

		file << fileContent;
		file.flush();
		file.close();

	}

} JSONFile, JsonFile;

int main() {
	JSONObject jsonObject("player");
	JSON one("name", "kyla");
	JSON two("level", "1");

	ComplexJSON cJSON1("gear");
	cJSON1.AddJSON(JSON("hat", "warrior's hat"));
	cJSON1.AddJSON(JSON("robe", "robe of destruction"));
	cJSON1.AddJSON(JSON("boots", "steel boots"));

	ComplexJSON cJSON2("location");
	cJSON2.AddJSON(JSON("world", "north pole"));
	cJSON2.AddJSON(JSON("destination", "santa's workshop"));
	cJSON2.AddJSON(JSON("world-x-position", "48543.2f"));
	cJSON2.AddJSON(JSON("world-y-position", "2849.9f"));

	jsonObject.AddJSON(one);
	jsonObject.AddJSON(two);
	jsonObject.AddJSON(cJSON1);
	jsonObject.AddJSON(cJSON2);

	jsonObject.FormatObject();

	JSONFile jsonFile("JsonFile.json");
	jsonFile.AddJSONObject(jsonObject);
	jsonFile.Format();


	return 0;
}
