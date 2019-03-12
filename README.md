# SimpleAssets

A simple standard for digital assets, ie. Non-Fungible Tokens (NFT) for EOSIO blockchains   
by [CryptoLions](https://CryptoLions.io)  
  
web: http://simpleassets.io  
Git: https://github.com/CryptoLions/SimpleAssets  

Presentation:  https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4

---------------------------  

# Change Log v0.1.1

Misc  
- sdelagate table structure renamed to sdelegate (typo)  
- create action parameters renamed: requireClaim -> requireclaim    
- assetID action parameter renamed in all actions to assetid   

Borrowing Assets  
- sdelegate table - added new field: untildate   
- delegate action added parameters untildate.  Action does a simple check if parameter was entered correctly (either zero or in the future).  
- undelegate will not work until untildate (this guarantees a minimum term of the asset loan).  
- allow transfer asset back (return) if its delegated, sooner than untiltime  (borrower has option ton return early)  
  
Batch Processing    
- claim action: assetid parameter changed to array of assetsids. Multiple claim logic added.    
- offer action: assetid parameter changed to array of assetsids. Multiple offer logic added.  
- canceloffer action: assetid parameter changed to array of assetsids. Multiple cancelation logic added.   
- transfer action: assetid parameter changed to array of assetsids. Multiple assets transfer logic added.   
- burn action: assetid parameter changed to array of assetsids. Multiple burning logic added.   
- delegate/undelegate action: assetid parameter changed to array of assetsids. Multiple delegation/undelegation logic added.  
  
  
------------------------  

# Contract actions  
```
 # -- For Non-Fungible Tokens ---
 create          (author, category, owner, idata, mdata, requireсlaim)  
 update          (author, owner, assetid, mdata)  
 transfer        (from, to , [assetid1,..,assetidn], memo)  
 burn            (owner, [assetid1,..,assetidn], memo)  
 
 offer           (owner, newowner, [assetid1,..,assetidn])  
 canceloffer     (owner, [assetid1,..,assetidn])  
 claim           (claimer, [assetid1,..,assetidn])  
  
 regauthor       (name author, data, stemplate)  
 authorupdate    (author, data, stemplate)  
 
 delegate        (owner, to, [assetid1,..,assetidn], untildate)  
 undelegate      (owner, from, [assetid1,..,assetidn])  
 
 # -- For Fungible Tokens ---
 
 createf         (author, maximum_supply)
 issuef          (to, author, quantity, memo)
 transferf       (from, to, author, quantity, memo)
 burn            (from, author, quantity, memo)

 openf           (owner, author, symbol, ram_payer)
 closef          (owner, author, symbol)
 
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
	uint64_t untildate; // The delegating account will not be able to undelegate before this date.
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

## Creating Asset with requireclaim option for ownerowner22:
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
auto mdata = json::parse(idxp->mdata);
mdata["cd"] = now() + 84600;

action saUpdate = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"update"_n,
	std::make_tuple(_self, owner, assetID, datam.dump())
);
saUpdate.send();
```

## Transfer one Asset
```
action saUpdate = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"transfer"_n,
	std::make_tuple(from, to, std::vector<uint64_t>(assetid), memo)
);
saUpdate.send();
```

## Transfer two Asset to same receiver with same memo  
```
action saUpdate = action(
	permission_level{get_self(),"active"_n},
	"simpl1assets"_n,
	"transfer"_n,
	std::make_tuple(from, to, std::vector<uint64_t>(assetid1, assetid2), memo)
);
saUpdate.send();
```