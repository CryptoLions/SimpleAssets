# SimpleAssets  

A simple standard for digital assets (ie. Non-Fungible Tokens) for EOSIO blockchains   
by [CryptoLions](https://CryptoLions.io)  
  
web: http://simpleassets.io  
Git: https://github.com/CryptoLions/SimpleAssets  

Presentation:  https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4  


# Contract actions  
```
 create          (author, category, owner, idata, mdata, requireClaim)  
 update          (author, owner, assetID, mdata)  
 transfer        (from, to , assetID, memo)  
 burn            (owner, assetID, memo)  
 
 offer           (owner, newowner, assetID)  
 canceloffer     (owner, assetID)  
 claim           (claimer, assetID)  
  
 regauthor       (name author, data, stemplate)  
 authorupdate    (author, data, stemplate)  
 
 delegate        (owner, to, assetID)  
 undelegate      (owner, from, assetID)  
```

# Data Structures  
## Assets  
```
assets {  
	uint64_t id, 		// asset id used for transfer and search  
	name owner,  		// asset owner (mutable - by owner!!!)  
	name author,		// asset author (game contract, immutable)  
	name category,		// asset category, chosen by author, immutable  
	string idata,		// immutable assets data  
	string mdata		// mutable assets data, added on creation or asset update by author  
}  
```
// Please include in idata or mdata info about asset name img desc which will be used by Markets  

## Offers  
```
offers {  
	uint64_t assetID, 	// asset id offered for claim  
	name owner,  		// asset owner  
	name offeredTo,		// who can claim this asset  
	uint64_t cdate		// offer create date  
}  
```

## Authors  
```
authors {  
	name author,    		// assets author, who will be able to create and update assets  
	string data,			// author’s data (json) will be used by markets for better display  
	string stemplate		// data (json) schema for markets. key: state values, where key is key from  
        				// recommendations for non-text fields: hide, url, img, webgl, mp3, video  
}  
```

## Delegates  
```
delegates{  
	uint64_t assetID, 	// asset id offered for claim  
	name owner,  		// asset owner  
	name delegatedto,	// who can claim this asset  
	uint64_t cdate		// offer create date  
}  
```

# EXAMPLES: how to use Simple Assets in smart contracts

## Creating Asset and transfer to owner account ownerowner22:
```
// Creating asset
action createAsset = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"create"_n,
	std::make_tuple(
		get_self(),
		"weapon"_n,
		"ownerowner22"_n,
		std::string("{\"power\": 10, \"speed\": 2.2, \"name\": \"Magic Sword\" }"),
		std::string("{\"color\": \"bluegold\", \"level\": 3, \"stamina\": 5, \"img\": \"https://bit.ly/2MYh8EA\" }"),
		0
	)
);
createAsset.send();	
```

## Creating Asset with requireClaim option for ownerowner22:
```
// Creating asset
action createAsset = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"create"_n,
	std::make_tuple(
		get_self(),
		"balls"_n,
		"ownerowner22_",
		std::string("{\"radius\": 2, \"weigh\": 5, \"material\": \"rubber\", \"name\": \"Baseball\" }"),
		std::string("{\"color\": \"white\", \"decay\": 99, \"img\": \"https://i.imgur.com/QoTcosp.png\" }"),
		1
	)
);
createAsset.send();	
```

## Update Asset
```
action saUpdate = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"update"_n,
	std::make_tuple(_self, owner, assetID, datam.dump())
);
saUpdate.send();
```

## Transfer Asset
```
action saUpdate = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"transfer"_n,
	std::make_tuple(from, to, assetID, memo)
);
saUpdate.send();
```
