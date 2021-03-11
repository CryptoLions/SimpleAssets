# SimpleAssets  
*document version 29 July 2020*

## Scope:
1. [Introduction](#introduction)   
	- [Resources](#resources)   
	- [Token Types](#token-types)   
2. [Contract actions](#contract-actions)   
3. [Data Structures](#data-structures)   
4. [EXAMPLES: how to use Simple Assets in smart contracts](#examples-how-to-use-simple-assets-in-smart-contracts)   
5. [AuthorReg](#authorreg)   
6. [ChangeLog](#change-logs)   

---------------------------  

# Introduction

A simple standard for digital assets on EOSIO blockchains: Non-Fungible Tokens (NFTs), Fungible Tokens (FTs), and Non-Transferable Tokens (NTTs).   
by [CryptoLions](https://CryptoLions.io)  

中文翻译: https://github.com/CryptoLions/SimpleAssets/blob/master/README_ZH.md  
한국어 번역: https://github.com/CryptoLions/SimpleAssets/blob/master/README_KR.md  
Español: https://github.com/CryptoLions/SimpleAssets/blob/master/README_ES.md  

**WARNING** The minimum dependency on eosio.cdt is now v1.6.3.  

---------------------------  

Use Simple Assets by making calls to the Simple Assets contract.  It's like a Dapp for Dapps.  

Jungle Testnet: **simpleassets**  

EOS: **simpleassets**  
WAX: **simpleassets**  
MEETONE: **smplassets.m**  
TELOS: **simpleassets**  
PROTON: **simpleassets**  
EUROPECHAIN: **simpleassets**  

  
Simple Assets is a separate contract which other Dapps can call to manage their digital assets.  This serves as an additional guarantee to users of the Dapp that the ownership of assets is managed by a reputable outside authority, and that once created, the Dapp can only manage the asset's mdata.  All the ownership-related functionality exists outside the game.    
  
Related: understanding [ownership authority](https://medium.com/@cryptolions/digital-assets-we-need-to-think-about-ownership-authority-a2b0465c17f6).  
  
To post information about your NFTs to third-party marketplaces, use the ```authorreg``` action.  
  
Alternatively, dapps can Deploy their own copy of Simple Assets and make modifications to have greater control of functionality, however this may compromise compatibility with wallets and other EOSIO infrastructure.  Before deploying, Simple Assets should be modified to prevent anyone from making assets.  

---------------------------  

## Resources

web: http://simpleassets.io  
Git: https://github.com/CryptoLions/SimpleAssets    
Telegram: https://t.me/simpleassets  
  
Intro & Demos:  https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4  

**(important for developers)** A detailed description of each action parameter can be found here:  
https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp  

Events Receiver Example for authors: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample    


---------------------------
## Token Types


### Non-Fungible Tokens (NFTs)

NFTs are the most common type of digital assets.  They are used to express unique tokens.  

#### NFT Structure

Simple Asset NFTs are divided into **mdata** (data which the author can update at any time, regardless of ownership), and **idata** (data which is set upon the NFT's creation and can never be updated).

Both are stringified JSONs.  For example: `{\"key1\":\"some-string\", \"key2\":5}`

**Category** is an optional field that lets you group your NFTs for convenience.  Category names must be less than or equal to 12 characters (a-z, 1-5).

**Offer/Claim** versus **Transfer** - If you transfer an NFT, the sender pays for RAM.  As an alternative, you can simply offer the NFT, and the user claiming will pay for their RAM.  *(Note: we are working toward a feature that allows NFT authors to reserve a lot of RAM which will spare users for paying for transfers.)*

#### RAM usage

RAM usage for NFTs depends on how much data is stored in the idata and mdata fields.  If they both empty, each NFT takes up `276 bytes`.

Each symbol in idata and mdata is +1 byte.


### Fungible Tokens (FTs)

Dapps which need Fungible tokens should decide between using the standard eosio.token contract, and the Simple Assets contract.  Here are the differences:

In Simple Assets,

* Scope is Author instead of Symbol
* Stat table includes also additional data about each FT (see [Currency Stats](#currency-stats-fungible-token) below)
* For transfers you need to use ```transferf``` action from SA contract.
* If author sets ```authorcontrol``` flag, the author can transfers/burn/etc user's FTs independent of user's consent.
* The table which tracks FTs includes the author's account name, allowing different dapps to have FTs with the 
	   same name.  (Example: https://bloks.io/contract?tab=Tables&table=accounts&account=simpleassets&scope=bohdanbohdan&limit=100)

*(Note: Fungible Tokens also have **offer/claim** functionality as an alternative to **transfers**.  For FTs, the only time the sender would pay for RAM would be if the receiver never before held those FTs.  It uses approximately 300 bytes to create the FT table.)*


### Non-Transferrable Tokens (NTTs)

The two most likely use cases for NTTs are 

* licenses which can be granted to an account, but not transfered.
* prizes and awards given to a particular account.

The reasons for using NTTs are:

* the NTTs appearing in third party asset explorers.
* some functionality is handled by Simple Assets.

More on NTTs: https://medium.com/@cryptolions/introducing-non-transferable-tokens-ntts-2f1a532bf170

---------------------------

# Contract actions  
A description of each parameter can be found here:  
https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp  

```bash
authorreg		( name author, string dappinfo, string fieldtypes, string priorityimg )
authorupdate		( name author, string dappinfo, string fieldtypes, string priorityimg )
setarampayer		( name author, name category, bool usearam ) 


 # -- For Non-Fungible Tokens (NFTs)---
 
 create			(author, category, owner, idata, mdata, requireсlaim)  
 update			(author, owner, assetid, mdata)  
 transfer		(from, to , [assetid1,..,assetidn], memo)  
 burn			(owner, [assetid1,..,assetidn], memo)  
 
 offer			(owner, newowner, [assetid1,..,assetidn], memo)  
 canceloffer		(owner, [assetid1,..,assetidn])  
 claim			(claimer, [assetid1,..,assetidn])  
  
 delegate		(owner, to, [assetid1,..,assetidn], period, redelegate, memo)  
 undelegate		(owner, [assetid1,..,assetidn])  
 delegatemore		(owner, assetid, period)  
 
 attach			(owner, assetidc, [assetid1,..,assetidn])
 detach			(owner, assetidc, [assetid1,..,assetidn])
 
 attachf		(owner, author, quantity, assetidc)
 detachf		(owner, author, quantity, assetidc)
 
 mdadd			(author, data)
 mdupdate		(id, author, data) 
 mdremove		(id)
 mdaddlog		(id, author, data)

 # -- For Fungible Tokens (FTs) ---
 
 createf		(author, maximum_supply, authorctrl, data)
 updatef		(author, sym, data)
 issuef			(to, author, quantity, memo)
 transferf		(from, to, author, quantity, memo)
 burnf			(from, author, quantity, memo)

 offerf			(owner, newowner, author, quantity, memo)
 cancelofferf		(owner, [ftofferid1,...,ftofferidn])
 claimf			(claimer, [ftofferid1,...,ftofferidn])

 openf			(owner, author, symbol, ram_payer)
 closef			(owner, author, symbol)
 
 # -- For Non-Transferable Tokens (NTTs) ---

 createntt		(author, category, owner, idata, mdata, requireсlaim)  
 updatentt		(author, owner, assetid, mdata)  
 burnntt		(owner, [assetid1,..,assetidn], memo)  
 claimntt		(claimer, [assetid1,..,assetidn])  

```

# Data Structures  
## Assets  
```c++
sasset {  
	uint64_t	id; 		// asset id used for transfer and search;  
	name		owner;  	// asset owner (mutable - by owner!!!);  
	name		author;		// asset author (game contract, immutable);  
	name		category;	// asset category, chosen by author, immutable;  
	string		idata;		// immutable assets data. Can be stringified JSON (recommended) 
					// or just sha256 string;  
	string		mdata;		// mutable assets data, added on creation or asset update by author. Can be  
					// stringified JSON (recommended) or just sha256 string;  
					// using a format other than stringified JSON will not interfere with 
					// simple asset functionality, but will harm compatibility with third party
					// explorers attempting to diplay the asset

	sasset[]	container;	// other NFTs attached to this asset
	account[]	containerf;	// FTs attached to this asset
}  
```
To help third party asset explorers, we recommend including the following fields in `idata` or `mdata`: 
`name` (text) 
`img` (url to image file) 


## Offers  
```c++
offers {  
	uint64_t	assetid;	// asset id offered for claim ; 
	name		owner;		// asset owner;  
	name		offeredto;	// who can claim this asset ; 
	uint64_t	cdate;		// offer create date;  
}  
```

## Authors  
```c++
authors {  
	name	author;			// assets author, who will be able to create and update assets;  

	string	data;			// author’s data (json) will be used by markets for better display;
					// recommendations: logo, info, url;  

	string	stemplate;		// data (json) schema to tell third-party markets how to display each NFT field.
					// key: state values, where key is the key from mdata or idata;
					// recommended values: 
					// txt    | default type
					// url    | show as clickable URL
					// img    | link to img file
					// webgl  | link to webgl file
					// mp3    | link to mp3 file
					// video  | link to video file
					// hide   | do not show
					// imgb   | image as string in binary format
					// webglb | webgl binary
					// mp3b   | mp3 binary
					// videob | video binary

	string	imgpriority;		// Specifies primary image field for categories of NFTs.
					//
					// This is used when you want your NFTs primary image to be something other
					// than a URL to an image field specified in the field img.  It also allows you to
					// create categories of NFTs with different primary image fields.
					// 
					// data is a strigified json.
					// key: NFT categories.
					// value: a field from idata or mdata to be used as the primary image for 
					// all NFTs of that category.

}  
```

## Delegates  
```c++
delegates{  
	uint64_t	assetid;		// asset id offered for claim;  
	name		owner;			// asset owner;  
	name		delegatedto;		// who can claim this asset;  
	uint64_t	cdate;			// offer create date;   
	uint64_t	period;			// Time in seconds that the asset will be lent. Lender cannot undelegate until 
						// the period expires, however the receiver can transfer back at any time.
	bool 		redelegate;		// redelegate is allow more redelegate for to account or not.
	string		memo;			// memo from action parameters. Max 64 length.

}  
```

## Currency Stats (Fungible Token)
```c++
stat {  
	asset		supply;		// Tokens supply 
	asset		max_supply;	// Max token supply
	name		issuer;		// Fungible token author
	uint64_t	id;		// Unique ID for this token
	bool		authorctrl;	// if true(1) allow token author (and not just owner) to burn and transfer.
	string		data;		// stringified json. recommended keys to include: `img`, `name`
}
```

## Account (Fungible Token)  
```c++
accounts {  
	uint64_t	id;		// token id, from stat table
	name		author;		// token author
	asset		balance;	// token balance
}  
```

```c++
offerfs {
	uint64_t	id;		// id of the offer for claim (increments automatically) 
	name		author;		// ft author
	name		owner;		// ft owner
	asset		quantity;	// quantity
	name		offeredto;	// account which can claim the offer
	uint64_t	cdate;		// offer creation date
}
```    
  

## NTT  
```c++
snttassets {  
	uint64_t	id; 		// NTT id used for claim or burn;  
	name		owner;  	// asset owner (mutable - by owner!!!);  
	name		author;		// asset author (game contract, immutable);  
	name		category;	// asset category, chosen by author, immutable;  
	string		idata;		// immutable assets data. Can be stringified JSON (recommended) 
					// or just sha256 string;  
	string		mdata;		// mutable assets data, added on creation or asset update by author. Can be  
					// stringified JSON (recommended) or just sha256 string;  
					// using a format other than stringified JSON will not interfere with 
					// simple asset functionality, but will harm compatibility with third party
					// explorers attempting to diplay the asset
}  
```
  
```c++
nttoffers {
	uint64_t	id;		// id of the offer for claim (increments automatically) 
	name		author;		// ft author
	name		owner;		// ft owner
	asset		quantity;	// quantity
	name		offeredto;	// account who can claim the offer
	uint64_t	cdate;		// offer creation date
}
```    


## More Data 
```c++
moredata{
	uint64_t		id;	// id of the more data 
	name			author;	// author of the more data 
	string			data;	// more data. recommended format: strigified JSON
}
```

## Author RAM Payer
```
sarampayer{
      uint64_t 			id;
      name    			author;
      name    			category;
      bool    			usearam;
      uint64_t  		from_id;

      auto primary_key() const {
        return id;
      }

      uint64_t by_author() const {
        return author.value;
      }
    };
```

# EXAMPLES: how to use Simple Assets in smart contracts

## Creating Asset and transfer to owner account ownerowner22:
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name category = "weapon"_n;
name owner = "ownerowner22"_n;
string idata = "{\"power\": 10, \"speed\": 2.2, \"name\": \"Magic Sword\" }";
string mdata = "{\"color\": \"bluegold\", \"level\": 3, \"stamina\": 5, \"img\": \"https://bit.ly/2MYh8EA\" }";

action createAsset = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"create"_n,
	std::make_tuple( author, category, owner, idata, mdata,	0 )
);
createAsset.send();	
```

## Creating Asset with requireclaim option for ownerowner22:
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name category = "balls"_n;
name owner = "ownerowner22"_n;
string idata = "{\"radius\": 2, \"weigh\": 5, \"material\": \"rubber\", \"name\": \"Baseball\" }";
string mdata = "{\"color\": \"white\", \"decay\": 99, \"img\": \"https://i.imgur.com/QoTcosp.png\" }";


action createAsset = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"create"_n,
	std::make_tuple( author, category, owner, idata, mdata, 1 )
);
createAsset.send();	
```

## Search asset and get assets info
1. Please add in your hpp file info about assets structure 

```c++
TABLE account {
	uint64_t	id;
	name		author;
	asset		balance;

	uint64_t primary_key()const { 
		return id;
	}
};
typedef eosio::multi_index< "accounts"_n, account > accounts;

TABLE sasset {
	uint64_t		id;
	name			owner;
	name			author;
	name			category;
	string			idata;
	string			mdata;
	std::vector<sasset>	container;
	std::vector<account>	containerf;

			
	auto primary_key() const {
		return id;
	}

	uint64_t by_author() const {
		return author.value;
	}
};

typedef eosio::multi_index< "sassets"_n, sasset, 		
		eosio::indexed_by< "author"_n, eosio::const_mem_fun<sasset, uint64_t, &sasset::by_author> >
> sassets;
```

2. Searching and using info
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
name author = get_self();
name owner = "lioninjungle"_n;

uint64_t assetid = 100000000000187

sassets assets(SIMPLEASSETSCONTRACT, owner.value);
auto idx = assets.find(assetid);

check(idx != assets.end(), "Asset not found or not yours");

check (idx->author == author, "Asset is not from this author");

auto idata = json::parse(idx->idata);  // for parsing json here is used nlohmann lib
auto mdata = json::parse(idx->mdata);  // https://github.com/nlohmann/json

check(mdata["cd"] < now(), "Not ready yet for usage");
```

## Update Asset
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

auto mdata = json::parse(idxp->mdata);
mdata["cd"] = now() + 84600;

name author = get_self();
name owner = "ownerowner22"_n;
uint64_t assetid = 100000000000187;

action saUpdate = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"update"_n,
	std::make_tuple(author, owner, assetid, mdata.dump())
);
saUpdate.send();
```

## Transfer one Asset
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

uint64_t assetid = 100000000000187;

std::vector<uint64_t> assetids;
assetids.push_back(assetid);

string memo = "Transfer one asset";

action saTransfer = action(
	permission_level{from, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(from, to, assetids, memo)
);
saTransfer.send();
```

## Transfer two Asset to same receiver with same memo  
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

uint64_t assetid1 = 100000000000187;
uint64_t assetid2 = 100000000000188;

std::vector<uint64_t> assetids;
assetids.push_back(assetid1);
assetids.push_back(assetid2);

string memo = "Transfer two asset"

action saTransfer = action(
	permission_level{from, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(from, to, assetids, memo)
);
saTransfer.send();
```

## Burn Assets
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name owner = "lioninjungle"_n;
uint64_t assetid1 = 100000000000187;
uint64_t assetid2 = 100000000000188;

std::vector<uint64_t> assetids;
assetids.push_back(assetid1);
assetids.push_back(assetid2);

string memo = "Transfer two asset"

action saBurn = action(
	permission_level{owner, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(owner, assetids, memo)
);
saBurn.send();
```

## issuef (fungible) issue created token
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 100;
wood.symbol = symbol("WOOD", 0);

name author = get_self();
name to = "lioninjungle"_n;

std::string memo = "WOOD faucet";
action saRes1 = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"issuef"_n,
	std::make_tuple(to, author, wood, memo)
);
saRes1.send();
```

## transferf (fungible) by author if authorctrl is enabled
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 20;
wood.symbol = symbol("WOOD", 0);

name from = "lioninjungle"_n;
name to = get_self();
name author = get_self();

std::string memo = "best WOOD";
action saRes1 = action(
	permission_level{from, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transferf"_n,
	std::make_tuple(from, to, author, wood, memo)
);
saRes1.send();
```

## burnf (fungible) by author if authorctrl is enabled
```c++
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 20;
wood.symbol = symbol("WOOD", 0);

name author = get_self();
name from = "lioninjungle"_n;

std::string memo = "WOOD for oven";
action saRes1 = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"burnf"_n,
	std::make_tuple(from, author, wood, memo)
);
saRes1.send();
```

-----------------
# AuthorReg

## authorreg action
Authors can register in the authorreg table to communicate with third party asset explorers, wallets, and marketplaces.

```c++
ACTION authorreg( name author, string dappinfo, string fieldtypes, string priorityimg );
```

@param **author**     is author's account who will create assets.

@param **dappinfo**   is stringified JSON. Recommendations to include: 
	name	 		- name of the application  
	company 		- name of the company   
	logo 			- url to image    
	url 			- url to the game's websites    
	info			- short description of application   
 	defaultfee 		- 100x the % fee you'd like to collect from marketplaces.  (for 2%, 200)    

@param **fieldtypes** is stringified JSON with key:state values, where key is key from mdata or idata and 
state indicates recommended way of displaying the field. For the latest recommended values, please see [https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp](https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp).  

@param **priorityimg** is JSON which assosiates an NFT category with the field name from idata or mdata
that specifies the main image field for that category of NFTs.  This is probably a rare use case and
can be left blank.  If you wanted a category of NFTs to have a main image field other than img, 
you'd use "CATEGORY":"otherfieldname".  Most likely use case is if you wanted webgls or some other format
to be the main image.


## Cleos examples of authorreg and authorupdate


### authorreg
```bash
./cleos.sh.jungle push action simpleassets authorreg '["ilovekolobok", "{\"name\": \"Kolobok Breeding Game\", \"company\": \"CryptoLions\", \"info\": \"Breed your Kolobok\", \"logo\": \"https://imgs.cryptolions.io/logo_256.png\", \"url\": \"https://kolobok.io\", \"defaultfee\":200}", "{\"bdate\":\"timestamp\"},{\"cd\":\"timestamp\"},{\"img\":\"img\"},{\"st\":\"hide\"},{\"url\":\"url\"}", "{\"kolobok\":\"img\"},{\"*\":\"img\"}" ]' -p ilovekolobok
```

### authorupdate
```bash
./cleos.sh.jungle push action simpleassets authorupdate '["ilovekolobok", "{\"name\": \"Kolobok Breeding Game\", \"company\": \"CryptoLions\", \"info\": \"Breed your Kolobok\", \"logo\": \"https://imgs.cryptolions.io/logo_256.png\", \"url\": \"https://kolobok.io\", \"defaultfee\":200}", "{\"bdate\":\"timestamp\"},{\"cd\":\"timestamp\"},{\"img\":\"img\"},{\"st\":\"hide\"},{\"url\":\"url\"}", "{\"kolobok\":\"img\"},{\"*\":\"img\"}" ]' -p ilovekolobok
```

-----------------
# Change Logs

## Change Log v1.6.0
- Added author ram payer option
- Added actions setarampayer, delarampayer
- detach and detachf for author only
- Memo increased to 512
- Code improvement

## Change Log v1.5.2
- Re-enabled event notifications for the following actions:  
  saeburn, saeclaim, saetransfer, saechauthor, saecreate.  
  Changed notification logic.  
  If an author's contract needs to be notified about changes in scope of an asset, please see this example:  
  https://github.com/CryptoLions/SimpleAssets-EventReceiverExample

## Change Log v1.5.1
- Added burnlog, burnflog, burnnttlog added
- Added restriction for burning asset with assets attached in container
- Added restriction to attach to delegated assets
- Code improvement
- SAE notification temporary disabled

## Change Log v1.5.0

- Added possibility to include SimpleAssets.hpp into other projects.  This helps developers to easily integrate Simple Assets into other contracts.
- Added developers function sa_getnextid to easily get id of newly created assets.
- Added more data functionality (actions mdremove, mdupdate, mdaddlog, mdadd). This offers a Simple Asset table which can store extra or repeating information for NFTs, and keep RAM usage to a minimum.

## Change Log v1.4.1

- Renamed fields and actions in Author Registration for better larity
        regauthor -> authorreg  
        data -> dappinfo  
        stemplate -> fieldtypes  
        imgpriority -> priorityimg  
- Added Author Registration documentation to readme

## Change Log v1.4.0
- re-delegate assets. (lender of assets can allow them to be re-lent)
- New parameter `bool redelegate` added in delegate action, which allows asset re-delegation.
- New field `bool redelegate` added in table `delegates` => require migration in case of self- deployed contract !!!
- In `undelegate` action parameter `from` was removed. (identity of borrower is available in the delegates table) 
- Fixed transfer of empty assets array
- Error messages improved for clarity
- Code refactoring


## Change Log v1.3.0
- Upgrade work with latest Contract Development Toolkit (CDT v1.6.3).  
  (Resolves this compilation [issue](https://github.com/EOSIO/eosio.cdt/issues/527))  
- minor code refactoring.


## Change Log v1.2.0
- NON TRANSFERRABLE TOKENS (NTTs) - new tables: snttassets and nttoffers
- new NTT actions: createntt, createnttlog, claimntt, updatentt, burnntt
- delegatemore action fix (thanks to cc32d9)
- ricardian contracts updated.
- external tests for NTT logic added.


## Change Log v1.1.3
- ricardian contracts updated.
- fungible token offer issue fix


## Change Log v1.1.2
- added `string imgpriority` field in `sauthor` table and to `regauthor` and `authorupdate` actions
- IMPORTANT:  Self-deployed instances of Simple Assets may need to migrate the regauthor table (if used).


## Change Log v1.1.1
- optimized claim/transfer/burn functionality
- Memo field added to delegates table.  (This allows lenders/games to create different classes of borrowed assets - eg. high risk / low risk.)  On delegete action, the memo from action parmeter is stored to this new field. max 64 chars length.
- Added three new unit tests for delegate memo.


## Change Log v1.1.0
- Code refactoring  
- Fixed detaching containerized NFTs for delegated and transferred NFTs.  
- new action delegatemore which allows extending delegate period for borrowed NFT.  
- Added external(bash) unit tests  
  
## Change Log v1.0.1
- new parameter `requireclaim` in `createlog` action which is used internaly to `create` actions history logs.


## Change Log v1.0.0
- Block owner from offering assets to themselves

## Change Log v0.4.2
- format for `saeclaim` event changed: array of assetids replaced by map <assetid, from>

## Change Log v0.4.1
- added require_recipient(owner) to `create` action


## Change Log v0.4.0

**Easily find fungible token information (fungible tokens have scope author):**
- new field `author` in `account` table for FT. (makes it easier to find fungible token information)

**More fungible token information:**
- new field `data` in `currency_stats` table - stringify json which might include keys `img`, `name` (recommended for better displaying by markets)
- new parameter `data` in `createf` action
- new action `updatef` to change FT `data`

**Offer/claim fungible tokens**
- new table `sofferf` to use for `offer`/`calim` FT
- new actions `offerf`, `cancelofferf` and `claimf`
- on `closef` check if no open offers (internal)

**Containerizing assets**  
- new fields `container` and `containerf` in nft asset structure for attaching and detaching other NFT or FT
- new actions `attach`, `detach`
- new actions `attachf`, `detachf`

**misc**
- fields renamed `lastid` -> `lnftid`, `spare`->`defid` (internal usage) in table `global` 
- field `offeredTo` renamed to `offeredto` in table `soffer`


## Change Log v0.3.2
- Added `memo` parameter to action `offer`;  
- Added `memo` parameter to action `delegate`;


## Change Log v0.3.1
- Internal action for NFT `createlog` added. Used by create action to log assetid so that third party explorers can easily get new asset ids and other information.
- New singelton table `tokenconfigs` added. It helps external contracts parse actions and tables correctly (Usefull for decentralized exchanges, marketplaces and other contracts that use multiple tokens).
  Marketplaces, exchanges and other reliant contracts will be able to view this info using the following code.
	```
	Configs configs("simpleassets"_n, "simpleassets"_n.value);
	configs.get("simpleassets"_n);
	```
- added action `updatever`. It updates version of this SimpleAstes deployment for 3rd party wallets, marketplaces, etc;
- New examples for Event notifications: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample  


## Change Log v0.3.0
- Added event notifications using deferred transaction. Assets author will receive notification on assets create, transfer, claim or burn. To receive it please add next action to your author contract:  
	```
        ACTION saecreate   ( name owner, uint64_t assetid );  
        ACTION saetransfer ( name from, name to, std::vector<uint64_t>& assetids, std::string memo );  
        ACTION saeclaim    ( name account, std::vector<uint64_t>& assetids );  
        ACTION saeburn     ( name account, std::vector<uint64_t>& assetids, std::string memo );  
	```
- `untildate` parameter changed to `period` (in seconds) for actions `delegate` and table `sdelegates`  


## Change Log v0.2.0
### Added Fungible Token tables and logic using eosio.token contract but with some changes
- New actions and logic: `createf`, `issuef`, `transferf`, `burnf`, `openf`, `closef`
- added new tables `stat(supply, max_supply, issuer, id)` and `accounts (id, balance)`. 
- scope for stats table (info about fungible tokens) changed to author
- primary index for `accounts` table is uniq id created on createf action and stored in stats table.
- added  `createf` action for fungible token with parametr `authorctrl` to `stats` table. If true(1) allows token author (and not just owner) to burnf and transferf. Cannot be changed after creation!
- Ricardian contracts updated
- more usage examples below


## Change Log v0.1.1
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











