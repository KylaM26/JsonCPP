#include "JSONCpp.h"

using namespace JSONCpp;

int main() {

	JSONObject player("player");

	player.AddJSON(JSON("name", "Kyla")); // You must add the JSON to the object 
	player.AddJSON(JSON("level", "1"));
	player.AddJSON(JSON("pvp-ranking", "394"));
	
	ComplexJSON gearJSON("gear"); // Complex objects also need a key.
	ComplexJSON textureJSON("texture"); // Complex objects also need a key.

	// Add the json you wish to nest to the gear object.
	gearJSON.AddJSON(JSON("hat", "warrior's hat"));
	gearJSON.AddJSON(JSON("robe", "robe of destruction"));
	gearJSON.AddJSON(JSON("boots", "steel boots"));

	textureJSON.AddJSON(JSON("hat-texture", "t48f348.png"));
	textureJSON.AddJSON(JSON("robe-texture", "jfv483n7.png"));
	textureJSON.AddJSON(JSON("boots-texture", "1dhjfa8.png"));

	player.AddJSON(gearJSON); // Add the complex json to the JSONObject.
	player.AddJSON(textureJSON);
	player.FormatObject();
	
	JSONFile jsonFile("playerJSON.json"); 
	jsonFile.AddJSONObject(player);
	jsonFile.Format();


	// std::cout << jsonFile.FindValueByKeys("player", "4fkfidfvnm,daf") << std::endl;

	return 0;
}
