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

typedef struct JSONObject {
private:
	std::string mainKey;

	std::vector<JSON> simpleJSON;
	std::vector<std::pair<std::string, std::vector<JSON>>> complexJSON;
	std::vector<std::string> completeJSON;

	std::string jsonObjString;
public:
	JSONObject() { this->mainKey = ""; }
	JSONObject(std::string mainKey) { this->mainKey = mainKey; }

	void AddJSON(const JSON& json) {
		simpleJSON.push_back(json);
	}

	void AddJSON(const std::pair<std::string, std::vector<JSON>>& jsonArray) {
		complexJSON.push_back(jsonArray);
	}

	void FormatObject() {
		std::string tab = "\t";
		std::string newLine = "\n";
		std::string quote = "\"";
		std::string colon = " : ";
		std::string comma = ",";
		std::string openBracket = "{", closeBracket = "}";

		completeJSON.push_back(tab + quote + mainKey + quote + colon + openBracket + newLine);

		// Key value pairs
		// Simple parsing
		for (int index = 0; index < simpleJSON.size(); index++) {
			if (index != simpleJSON.size() - 1)
				completeJSON.push_back(tab + tab + simpleJSON[index].GetJSONString() + comma + newLine);
			else {
				if(complexJSON.size() > 0)
					completeJSON.push_back(tab + tab + simpleJSON[index].GetJSONString() + comma + newLine);
				else
					completeJSON.push_back(tab + tab + simpleJSON[index].GetJSONString() + newLine);
			}
		}

		// Complex parsing
		for (int index = 0; index < complexJSON.size(); index++) { // For all pairs in the vector
			completeJSON.push_back(tab + tab + quote + complexJSON.at(index).first + quote + colon + openBracket + newLine); // Parse the main key
			if (index != complexJSON.size() - 1) { // If not the last object in the vector
				for (int pair = 0; pair < complexJSON.at(index).second.size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
					if(pair != complexJSON.at(index).second.size() - 1) 
						completeJSON.push_back(tab + tab + tab + quote + complexJSON.at(index).second.at(pair).key + quote + colon + quote + complexJSON.at(index).second.at(pair).value + quote + comma + newLine);
					else
						completeJSON.push_back(tab + tab + tab + quote + complexJSON.at(index).second.at(pair).key + quote + colon + quote + complexJSON.at(index).second.at(pair).value + quote + newLine);
				}
			}  else {
				for (int pair = 0; pair < complexJSON.at(index).second.size(); pair++) { // If the pairs are less then the sizeof the vector of jsons
					if (pair != complexJSON.at(index).second.size() - 1)
						completeJSON.push_back(tab + tab + tab + quote + complexJSON.at(complexJSON.size() - 1).second.at(pair).key + quote + colon + quote + complexJSON.at(complexJSON.size() - 1).second.at(pair).value + quote + comma + newLine);
					else
						completeJSON.push_back(tab + tab + tab + quote + complexJSON.at(complexJSON.size() - 1).second.at(pair).key + quote + colon + quote + complexJSON.at(complexJSON.size() - 1).second.at(pair).value + quote + newLine);
				}
			}

			if (index != complexJSON.size() - 1) {
				completeJSON.push_back(tab + tab + closeBracket + comma + newLine + newLine);
			} else {
				completeJSON.push_back(tab + tab + closeBracket + newLine);
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

int main() {

	std::ofstream jsonFile;

	JSONObject jsonObject("player");
	JSON one("name", "kyla");
	JSON two("level", "1");

	std::pair<std::string, std::vector<JSON>> complexJSON;

	std::vector<JSON> jsonObjects;
	jsonObjects.push_back(JSON("hat", "warrior's hat"));
	jsonObjects.push_back(JSON("robe", "robe of destruction"));
	jsonObjects.push_back(JSON("shoes", "shoes of life"));

	complexJSON = std::pair<std::string, std::vector<JSON>>(std::pair<std::string, std::vector<JSON>>("gear", jsonObjects));



	std::pair<std::string, std::vector<JSON>> complexJSON1;

	std::vector<JSON> jsonObjects1;
	jsonObjects1.push_back(JSON("world", "North Pole"));
	jsonObjects1.push_back(JSON("destination", "Santa's Workshop"));
	jsonObjects1.push_back(JSON("world-x-position", "3943.21f"));
	jsonObjects1.push_back(JSON("world-y-position", "4923.12f")); 

	complexJSON1 = std::pair<std::string, std::vector<JSON>>(std::pair<std::string, std::vector<JSON>>("location", jsonObjects1));

	jsonObject.AddJSON(one);
	jsonObject.AddJSON(two);
	jsonObject.AddJSON(complexJSON);
	jsonObject.AddJSON(complexJSON1);


	jsonObject.FormatObject();

	std::cout << jsonObject.GetJSONObjectString() << std::endl;

//	WriteJSONObjectToFile(jsonFile, "JsonFile.txt");

	return 0;
}
//
//void WriteJSONObjectToFile(std::ofstream& file, const char* path) {
//	file.open(path, std::ios::in | std::ios::out | std::ios::trunc);
//
//	if (!file.is_open()) {
//		std::cout << "Failed to open file for writing JSONObject: " << path << std::endl;
//		return;
//	}
//
//	std::string mainKey = "Player";
//
//	std::vector<std::pair<std::string, std::string>> gearObject;
//	gearObject.push_back(std::pair<std::string, std::string>("hat", "warrior's hat"));
//	gearObject.push_back(std::pair<std::string, std::string>("robe", "warrior's robe"));
//	gearObject.push_back(std::pair<std::string, std::string>("shoes", "warrior's shoes"));
//
//	std::string gearObjectToStr;
//	gearObjectToStr.append("\n");
//	for (int index = 0; index < gearObject.size(); index++) {
//		if (index != gearObject.size() - 1)
//			gearObjectToStr.append("\t\t\t\"" + gearObject.at(index).first + "\" : " + "\"" + gearObject.at(index).second + "\",\n");
//		else
//			gearObjectToStr.append("\t\t\t\"" + gearObject.at(index).first + "\" : " + "\"" + gearObject.at(index).second + "\"\n");
//
//	}
//	gearObjectToStr.append("\t\t}\n");
//
//	std::pair<std::string, std::string> keyValuePair1("name", "kyla");
//	std::pair<std::string, std::string> keyValuePair2("level", "1");
//	std::pair<std::string, std::string> keyValuePair3("gear", gearObjectToStr);
//	//std::pair <std::string, std::string>> complexKeyValuePair("gear", gearObject);
//	
//	std::string fileContentVector;
//	std::vector<std::string> jsonObjects;
//
//	// Type, key name -> for location
//	// if type is main, append under main section
//	// if type is sub, ask for subkey name
//
//	jsonObjects.push_back("\n\t\"" + mainKey + "\": {" + "\n");
//
//	// Store string objects in array
//	std::string jsonObjString1 = "";
//	std::string jsonObjString2 = "";
//	std::string jsonObjString3 = "";
//
//	jsonObjString1.append("\t\t\"" + keyValuePair1.first + "\" : \"" + keyValuePair1.second + "\"");
//	jsonObjString2.append("\t\t\"" + keyValuePair2.first + "\" : \"" + keyValuePair2.second + "\"");
//	jsonObjString3.append("\t\t\"" + keyValuePair3.first + "\" : {" + keyValuePair3.second);
//
//	jsonObjects.push_back(jsonObjString1);
//	jsonObjects.push_back(jsonObjString2);
//	jsonObjects.push_back(jsonObjString3);
//
//
//	fileContentVector.append(jsonObjects[0]);
//	for (int index = 1; index < jsonObjects.size(); index++) {
//		if(index != jsonObjects.size() - 1)
//			fileContentVector.append(jsonObjects.at(index) + ",\n");
//		else 
//			fileContentVector.append(jsonObjects.at(index) + "\n\t}\n");
//	} 
//
//	fileContentVector.insert(0, "{\n");
//	fileContentVector.append("\n}");
//
//	file << fileContentVector.c_str();
//
//	file.flush();
//	file.close();
//}