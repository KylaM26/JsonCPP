# JsonCPP
My small personal library that writes and reads JSON data to a file.

## How to use JSONCpp

### Step 1:

Include JSONCpp

`#include "JSONCpp.h"`

`using namespace JSONCpp;`

### Step 2:

Create a JSONObject

`JSONObject player("player"); // Nested JSON will be stored nested in the key passed into the paramater.`


### Step 3:

Add JSON to your object

`player.AddJSON(JSON("name", "kyla"));`

`player.AddJSON(JSON("level", "1")); // Store number datatypes as strings, use StringTo...() function.`

To add a JSON that stores more JSON you must create a ComplexJSON object.

`ComplexJSON gear("gear");`

`gear.AddJSON(JSON("hat", "noob hat"));`

`gear.AddJSON(JSON("robe", "noob robe"));`

`gear.AddJSON(JSON("boots", "noob boots"));`

`player.AddJSON(gear); // Finally, add the complex json to its parent object.`

### Step 4:

After you have added all the JSON data you wished to add, you must call the format function.

`player.FormatObject();`

### Step 5:

Before you can begin to access the data you must store it in a JSONFile Object.

`JSONFile jsonFile("playerJSON.json"); // Any file extension will work.`

### Step 6:

Add your jsonObject(s) to your file.

`jsonFile.AddJSONObject(player);`

### Step 7:

Finally, you must call Format function.

`jsonFile.Format();`

### Step 8:

Accessing the data from the file.

`const std::string name = jsonFile.FindValueByKeys("player", "name");`

`const int level = StringToInteger(jsonFile.FindValueByKeys("player", "level"));`

## All done!

# By the way!

## As of now, JSON Cpp does not support:

1.) Reading json data from an imported file.

2.) Nesting, nesting! 


## Example:

### Do not support:

     	"player" : {
	        "name" : "kyla",
		      "level" : "1",
		      "gear" : {
		           "hat": "warriors hat",
			        "robe" : "robe of destruction",
			        "boots" : "steel boots",
			        "accessories" : {
			          "necklace" : "some necklace",
			  	      "bracelet" : "some bracelet"
		          }
	       },

		    "location" : {
		        "world" : "north pole",
			      "destination" : "santa workshop",
			      "world-x-position" : "48543.2f",
			      "world-y-position" : "2849.9f"
	     }
      
    }
  
   gear cannot nest accessories.
  
  ### Do support:
  
       	"player" : {
	        "name" : "kyla",
		      "level" : "1",
		      "gear" : {
		          "hat": "warriors hat",
			        "robe" : "robe of destruction",
			        "boots" : "steel boots",
	        },
        
        	"accessories" : {
			       "necklace" : "some necklace",
			  	   "bracelet" : "some bracelet"
		       }

		      "location" : {
		        "world" : "north pole",
			      "destination" : "santa workshop",
			      "world-x-position" : "48543.2f",
			      "world-y-position" : "2849.9f"
	        }
      
    }

Also, a JSONFile can have as many json objects as you wish.
