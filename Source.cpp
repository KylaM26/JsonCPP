#include "JSONCpp.h"

using namespace JSONCpp;

int main() {
	JSONObject jsonObject("player");
	JSONObject jsonObject1("textures");

	JSON one("name", "kyla");
	JSON two("level", "1");

	JSON three("name", "brick.bmp");
	JSON four("width", "1024");
	JSON five("height", "680");
	
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
	
	jsonObject1.AddJSON(three);
	jsonObject1.AddJSON(four);
	jsonObject1.AddJSON(five);

	jsonObject.FormatObject();
	jsonObject1.FormatObject();

	JSONFile jsonFile("JsonFile1.json");
	jsonFile.AddJSONObject(jsonObject);
	jsonFile.AddJSONObject(jsonObject1);
	jsonFile.Format();

	const float xPos = StringToFloat(jsonFile.FindValueByKeys("player", "location", "world-x-position"));
	const float yPos = StringToFloat(jsonFile.FindValueByKeys("player", "location", "world-y-position"));


	std::cout << xPos << "," << yPos << std::endl;



	return 0;
}
