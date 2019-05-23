# SimpleAssets  

[中文翻译](#中文翻译)

A simple standard for digital assets (both fungible and non-fungible tokens - NFTs) for EOSIO blockchains   
by [CryptoLions](https://CryptoLions.io)  
  
web: http://simpleassets.io  
Git: https://github.com/CryptoLions/SimpleAssets  
Telegram: https://t.me/simpleassets

EOS Mainnet Account: **simpleassets**

Intro & Demos:  https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4

Events Receiver Example for authors: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample  

**WARNING!!! CDT currently has a bug that doesn't allow compilation on v1.6.1.
  1.5.0 also has a bug "Segmentation fault (core dumped)", but only with abi generation.
  Recommendation: Use 1.5.0 for contract compilation and use our abi**
    
---------------------------  

There are two ways to use Simple Assets:  
  
1) As an external [ownership authority](https://medium.com/@cryptolions/digital-assets-we-need-to-think-about-ownership-authority-a2b0465c17f6).  When deployed on an EOSIO chain, Simple Assets will be a separate contract which other Dapps can call to manage their digital assets.  This serves as an additional guarantee to users of the Dapp that the ownership of assets is managed by a reputable outside authority, and that once created, the Dapp can only manage the asset's mdata.  All the ownership-related functionality exists outside the game.  
  
   EOS Mainnet Account: **simpleassets** 

   We are in the process of creating a DAC which will curate updates to Simple Assets after deployment to the EOSIO mainnet. 
  
2) Dapps can Deploy their own copy of Simple Assets and make modifications to have greater control of functionality.  We consider this an example of a dapp being its own "ownership authority."  Before deploying, Simple Assets should be modified to prevent anyone from making assets.

---------------------------
## RAM usage

The RAM usage for NFTs depends on how much data is stored in the idata and mdata fields.  If they both empty, each NFT takes up `276 bytes`.

Each symbol in imdata and mdata is +1 byte.

---------------------------
## Scope:
1. [Contract actions](#contract-actions)
2. [Data Structures](#data-structures)
3. [EXAMPLES: how to use Simple Assets in smart contracts](#examples-how-to-use-simple-assets-in-smart-contracts)
4. [ChangeLog](#change-log-v101)
---------------------------  

# Contract actions  
A description of each parameter can be found here:  
https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp  

```
 # -- For Non-Fungible Tokens ---
 
 create			(author, category, owner, idata, mdata, requireсlaim)  
 update			(author, owner, assetid, mdata)  
 transfer		(from, to , [assetid1,..,assetidn], memo)  
 burn			(owner, [assetid1,..,assetidn], memo)  
 
 offer			(owner, newowner, [assetid1,..,assetidn], memo)  
 canceloffer		(owner, [assetid1,..,assetidn])  
 claim			(claimer, [assetid1,..,assetidn])  
  
 regauthor		(name author, data, stemplate)  
 authorupdate		(author, data, stemplate)  
 
 delegate		(owner, to, [assetid1,..,assetidn], period, memo)  
 undelegate		(owner, from, [assetid1,..,assetidn])  
 
 attach			(owner, assetidc, [assetid1,..,assetidn])
 detach			(owner, assetidc, [assetid1,..,assetidn])
 
 attachf		(owner, author, quantity, assetidc)
 detachf		(owner, author, quantity, assetidc)
 
 # -- For Fungible Tokens ---
 
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
 
```

# Data Structures  
## Assets  
```
sasset {  
	uint64_t	id; 		// asset id used for transfer and search;  
	name		owner;  	// asset owner (mutable - by owner!!!);  
	name		author;		// asset author (game contract, immutable);  
	name		category;	// asset category, chosen by author, immutable;  
	string		idata;		// immutable assets data. Can be stringified JSON or just sha256 string;  
	string		mdata;		// mutable assets data, added on creation or asset update by author. Can be  
					// stringified JSON or just sha256 string;  
	sasset[]	container;	// other NFTs attached to this asset
	account[]	containerf;	// FTs attached to this asset
}  
```
// Please include in idata or mdata info about asset name img desc which will be used by Markets  

## Offers  
```
offers {  
	uint64_t	assetid;	// asset id offered for claim ; 
	name		owner;		// asset owner;  
	name		offeredto;	// who can claim this asset ; 
	uint64_t	cdate;		// offer create date;  
}  
```

## Authors  
```
authors {  
	name	author;			// assets author, who will be able to create and update assets;  
	string	data;			// author’s data (json) will be used by markets for better display;
					// recommendations: logo, info, url;  
	string	stemplate;		// data (json) schema for markets. key: state values, where key is key from;  
					// recommendations for non-text fields: hide, url, img, webgl, mp3, video;  
}  
```

## Delegates  
```
delegates{  
	uint64_t	assetid;	// asset id offered for claim;  
	name		owner;		// asset owner;  
	name		delegatedto;	// who can claim this asset;  
	uint64_t	cdate;		// offer create date;  
	uint64_t	period;		// Time in seconds that the asset will be lent. Lender cannot undelegate until 
					// the period expires, however the receiver can transfer back at any time.
}  
```

## Currency Stats (Fungible Token)
```
stat {  
	asset		supply;		// Tokens supply 
	asset		max_supply;	// Max token supply
	name		issuer;		// Fungible token author
	uint64_t	id;		// Unique ID for this token
	bool		authorctrl;	// if true(1) allow token author (and not just owner) to burn and transfer.
	string		data;		// strigified json. recommended keys to include: `img`, `name`
}
```

## Account (Fungible Token)  
```
accounts {  
	uint64_t	id;		// token id, from stat table
	name		author;		// token author
	asset		balance;	// token balance
}  
```

```
sofferf {
	uint64_t	id;		// id of the offer for claim (increments automatically) 
	name		author;		// ft author
	name		owner;		// ft owner
	asset		quantity;	// quantity
	name		offeredto;	// account who can claim the offer
	uint64_t	cdate;		// offer creation date
}
```    
  
# EXAMPLES: how to use Simple Assets in smart contracts

## Creating Asset and transfer to owner account ownerowner22:
```
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
```
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
	**WARNING!!! CDT currently has a bug that doesn't allow compilation (v1.6.1).  
	1.5.0 also has a bug "Segmentation fault (core dumped)", but only with abi generation (including array of self objects : std::vector<sasset> container;)**
```
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
```
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
```
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
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

std::vector<uint64_t> assetids;
assetids.push_back(assetid);

string memo = "Transfer one asset";

action saUpdate = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(from, to, assetids, memo)
);
saUpdate.send();
```

## Transfer two Asset to same receiver with same memo  
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

std::vector<uint64_t> assetids;
assetids.push_back(assetid1);
assetids.push_back(assetid2);

string memo = "Transfer two asset"

action saUpdate = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(from, to, assetids, memo)
);
saUpdate.send();
```

## issuef (fungible) issue created token
```
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
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 20;
wood.symbol = symbol("WOOD", 0);

name from = "lioninjungle"_n;
name to = get_self();
name author = get_self();

std::string memo = "best WOOD";
action saRes1 = action(
	permission_level{author, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transferf"_n,
	std::make_tuple(from, to, author, wood, memo)
);
saRes1.send();
```

## burnf (fungible) by author if authorctrl is enabled
```
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
**WARNING!!! CDT currently has a bug that doesn't allow compilation (v1.6.1).  
  1.5.0 also has a bug "Segmentation fault (core dumped)", but only with abi generation.
  Recommendation: Use 1.5.0 for contract compilation and use our abi**
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



========
# 中文翻译
﻿﻿﻿﻿﻿﻿﻿﻿﻿简单资产（Simple Assets）
=========================

一款EOSIO区块链上的数字资产（可替代和不可替代代币——NFTs）标准，由CryptoLions开发创建。  

web: [http://simpleassets.io](http://simpleassets.io/  
Git: <https://github.com/CryptoLions/SimpleAssets>  
Telegram: <https://t.me/simpleassets>

EOS 主网帐户：**simpleassets**

简介和演示：https://medium.com/\@cryptolions/introducing-simple-assets-b4e17caafaa4

作者的事件接收器示例：https://github.com/CryptoLions/SimpleAssets-EventReceiverExample

**警告!!! CDT目前有一个不被允许在v1.6.1上编译的漏洞。
1.5.0也有一个漏洞“Segmentation fault（core dumped）”，但只有abi生效。**
**建议：使用1.5.0进行合约编译，并使用我们的abi。**

------

使用简单资产（Simple Assets）有两种方法：

1.  作为外部*所有权机构*。当部署在EOSIO链上时，简单资产（Simple
    Assets）将成为其他Dapps可以调用以管理其数字资产的独立合约。这为Dapp用户提供额外保证，即资产所有权由信誉良好的外部机构管理，并且一旦创建，Dapp只能管理资产的mdata。余下与所有权相关的功能都存在于游戏之外。  

EOS Mainnet帐户：**simpleassets**  


我们正在创建一个DAC，它将被部署到EOSIO主网，兼顾对Simple Assets的更新。  


1.  Dapps可以部署自己的简单资产（Simple
    Assets）副本并进行修改，用以更好地控制功能。我们认为这是一个例子——dapp作为自己的“所有权”。在部署之前，简单资产（Simple
    Assets）应当被修改以防止任何人制作资产。
------
## RAM使用情况

NFT的RAM使用量取决于idata和mdata字段中存储的数据量。
如果它们都为空，则每个NFT占用276个字节。

imdata和mdata中的每个符号都是+1字节。

----
## 目录

1.  [合约操作](https://github.com/CryptoLions/SimpleAssets#contract-actions)

2.  [数据结构](https://github.com/CryptoLions/SimpleAssets#data-structures)

3.  [示例：如何在智能合约中使用简单资产](https://github.com/CryptoLions/SimpleAssets#examples-how-to-use-simple-assets-in-smart-contracts)

4.  [更新日志](https://github.com/CryptoLions/SimpleAssets#change-log-v101)
---

合约操作
========

可在此处找到每个参数的说明：

[https](https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp)*：*[//github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp](https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp)

```
\# -- For Non-Fungible Tokens ---

create (author, category, owner, idata, mdata, requireсlaim)

update (author, owner, assetid, mdata)

transfer (from, to , [assetid1,..,assetidn], memo)

burn (owner, [assetid1,..,assetidn], memo)

offer (owner, newowner, [assetid1,..,assetidn], memo)

canceloffer (owner, [assetid1,..,assetidn])

claim (claimer, [assetid1,..,assetidn])

regauthor (name author, data, stemplate)

authorupdate (author, data, stemplate)

delegate (owner, to, [assetid1,..,assetidn], period, memo)

undelegate (owner, from, [assetid1,..,assetidn])

attach (owner, assetidc, [assetid1,..,assetidn])

detach (owner, assetidc, [assetid1,..,assetidn])

attachf (owner, author, quantity, assetidc)

detachf (owner, author, quantity, assetidc)

\# -- For Fungible Tokens ---

createf (author, maximum_supply, authorctrl, data)

updatef (author, sym, data)

issuef (to, author, quantity, memo)

transferf (from, to, author, quantity, memo)

burnf (from, author, quantity, memo)

offerf (owner, newowner, author, quantity, memo)

cancelofferf (owner, [ftofferid1,...,ftofferidn])

claimf (claimer, [ftofferid1,...,ftofferidn])

openf (owner, author, symbol, ram_payer)

closef (owner, author, symbol)
```
---
数据结构
========

## 资产

```
sasset {

uint64_t id; // asset id used for transfer and search;

name owner; // asset owner (mutable - by owner!!!);

name author; // asset author (game contract, immutable);

name category; // asset category, chosen by author, immutable;

string idata; // immutable assets data. Can be stringified JSON or just sha256
string;

string mdata; // mutable assets data, added on creation or asset update by
author. Can be

// stringified JSON or just sha256 string;

sasset[] container; // other NFTs attached to this asset

account[] containerf; // FTs attached to this asset

}

//请包含有关资产名称img desc的idata或mdata信息，这些信息将由Markets使用。
```
----
## 报价

```
offers {

uint64_t assetid; // asset id offered for claim ;

name owner; // asset owner;

name offeredto; // who can claim this asset ;

uint64_t cdate; // offer create date;

}
```
---
## 创建者
```
authors {

name author; // assets author, who will be able to create and update assets;

string data; // author’s data (json) will be used by markets for better display;

// recommendations: logo, info, url;

string stemplate; // data (json) schema for markets. key: state values, where
key is key from;

// recommendations for non-text fields: hide, url, img, webgl, mp3, video;

}
```
----
## 委托
```
delegates{

uint64_t assetid; // asset id offered for claim;

name owner; // asset owner;

name delegatedto; // who can claim this asset;

uint64_t cdate; // offer create date;

uint64_t period; // Time in seconds that the asset will be lent. Lender cannot
undelegate until

// the period expires, however the receiver can transfer back at any time.

}
```
---
## 货币统计（可替代代币）
```
stat {

asset supply; // Tokens supply

asset max_supply; // Max token supply

name issuer; // Fungible token author

uint64_t id; // Unique ID for this token

bool authorctrl; // if true(1) allow token author (and not just owner) to burn
and transfer.

string data; // strigified json. recommended keys to include: \`img\`, \`name\`

}
```
---
## 账户（可替代代币）
```
accounts {

uint64_t id; // token id, from stat table

name author; // token author

asset balance; // token balance

}

sofferf {

uint64_t id; // id of the offer for claim (increments automatically)

name author; // ft author

name owner; // ft owner

asset quantity; // quantity

name offeredto; // account who can claim the offer

uint64_t cdate; // offer creation date

}
```
---
示例：如何在智能合约中使用简单资产
==================================

## 创建资产并转移到所有者帐户ownerowner22：
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();

name category = "weapon"_n;

name owner = "ownerowner22"_n;

string idata = "{\\"power\\": 10, \\"speed\\": 2.2, \\"name\\": \\"Magic
Sword\\" }";

string mdata = "{\\"color\\": \\"bluegold\\", \\"level\\": 3, \\"stamina\\": 5,
\\"img\\": \\"https://bit.ly/2MYh8EA\\" }";

action createAsset = action(

permission_level{author, "active"_n},

SIMPLEASSETSCONTRACT,

"create"_n,

std::make_tuple( author, category, owner, idata, mdata, 0 )

);

createAsset.send();
```
---
## 使用ownerowner22的requireclaim选项创建资产：
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();

name category = "balls"_n;

name owner = "ownerowner22"_n;

string idata = "{\\"radius\\": 2, \\"weigh\\": 5, \\"material\\": \\"rubber\\",
\\"name\\": \\"Baseball\\" }";

string mdata = "{\\"color\\": \\"white\\", \\"decay\\": 99, \\"img\\":
\\"https://i.imgur.com/QoTcosp.png\\" }";

action createAsset = action(

permission_level{author, "active"_n},

SIMPLEASSETSCONTRACT,

"create"_n,

std::make_tuple( author, category, owner, idata, mdata, 1 )

);

createAsset.send();
```
---
## 搜索资产并获取资产信息  

1.  请添加有关资产结构的hpp文件信息。

**警告! CDT目前有一个不允许编译的漏洞（v1.6.1)。1.5.0也有一个漏洞“Segmentation fault（core
dumped）”，但只有abi可生成（包括self对象数组：std :: vector container;）**

```
TABLE account {

uint64_t id;

name author;

asset balance;

uint64_t primary_key()const {

return id;

}

};

typedef eosio::multi_index\< "accounts"_n, account \> accounts;

TABLE sasset {

uint64_t id;

name owner;

name author;

name category;

string idata;

string mdata;

std::vector\<sasset\> container;

std::vector\<account\> containerf;

auto primary_key() const {

return id;

}

uint64_t by_author() const {

return author.value;

}

};

typedef eosio::multi_index\< "sassets"_n, sasset,

eosio::indexed_by\< "author"_n, eosio::const_mem_fun\<sasset, uint64_t,
&sasset::by_author\> \>

\> sassets;
```

2.  搜索和使用信息
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();

name owner = "lioninjungle"_n;

uint64_t assetid = 100000000000187

sassets assets(SIMPLEASSETSCONTRACT, owner.value);

auto idx = assets.find(assetid);

check(idx != assets.end(), "Asset not found or not yours");

check (idx-\>author == author, "Asset is not from this author");

auto idata = json::parse(idx-\>idata); // for parsing json here is used nlohmann
lib

auto mdata = json::parse(idx-\>mdata); // https://github.com/nlohmann/json

check(mdata["cd"] \< now(), "Not ready yet for usage");
```
---
## 更新资产
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

auto mdata = json::parse(idxp-\>mdata);

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
---
## 转移一个资产
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();

name from = "lioninjungle"_n;

name to = "ohtigertiger"_n;

std::vector\<uint64_t\> assetids;

assetids.push_back(assetid);

string memo = "Transfer one asset";

action saUpdate = action(

permission_level{author, "active"_n},

SIMPLEASSETSCONTRACT,

"transfer"_n,

std::make_tuple(from, to, assetids, memo)

);

saUpdate.send();
```
---
## 将两个资产转移到具有相同备忘录的同一接收器
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();

name from = "lioninjungle"_n;

name to = "ohtigertiger"_n;

std::vector\<uint64_t\> assetids;

assetids.push_back(assetid1);

assetids.push_back(assetid2);

string memo = "Transfer two asset"

action saUpdate = action(

permission_level{author, "active"_n},

SIMPLEASSETSCONTRACT,

"transfer"_n,

std::make_tuple(from, to, assetids, memo)

);

saUpdate.send();
```
---
## issuef创建代币问题（可替代代币）
```
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
---

## 如果启用了authorctrl，则由创建者转让代币（可替代）  

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;

wood.amount = 20;

wood.symbol = symbol("WOOD", 0);

name from = "lioninjungle"_n;

name to = get_self();

name author = get_self();

std::string memo = "best WOOD";

action saRes1 = action(

permission_level{author, "active"_n},

SIMPLEASSETSCONTRACT,

"transferf"_n,

std::make_tuple(from, to, author, wood, memo)

);

saRes1.send();
```
----
## 如果启用了authorctrl，则由创建者烧录代币（可替代）
```
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
------

## 更改日志v1.0.1

-   `createlog`  操作中的新参数 `requireclaim`，用于 `create` 操作历史记录日志。

---
## 更改日志v1.0.0

-   阻止所有者向自己提供资产
---
## 更改日志v0.4.2

-   `saeclaim` 事项的格式已更改：由map \<assetid，from\>替换asseti数组
---
## 更改日志v0.4.1

-   添加了require_recipient（所有者）来执行`create`操作
---
## 更改日志v0.4.0

**轻松找到可替代代币的信息（可替换代币有创建者范围）：**

-   FT的 `account` 表中的新字段 `author`。 （更容易找到可替代代币信息）

**更多可替代代币信息**

-   新领域 `data` 中 `currency_stats` 的表-字符串化JSON其中可能包括键 `img`，`name`（建议最好通过市场显示）

-   新参数`data`在 `createf` 操作中

-   updatef改变FT的新举措data

**提供/声明可替代的代币**

-   `sofferf` 用于`offer` / `calimFT`的新表

-   新的操作`offer`，`cancelofferf` 和 `claimf` 

-   对 `closef` 检查如果没有公开招股（内部）

**集装资产**

**警告!!! CDT目前有一个不被允许在v1.6.1上编译的漏洞。
1.5.0也有一个漏洞“Segmentation fault（core dumped）”，但只有abi生效。**

**建议：使用1.5.0进行合约编译，并使用我们的abi。**

-   nft资产结构中用于附加和分离其他NFT或FT的新字段“container”和“containerf”

-   新操作 `attach`，`detach`

-   新操作 `attachf`，`detach`

**杂项**

-   字段重命名 `lasted`- \>` lnftid`，`spare`- \> `defid`（内部用法）在表 `global` 中

-   字段 `providedTo` 在 `soffer` 表中重命名为 `offersto`
---
## 更改日志v0.3.2

-   为操作 `offer` 添加了 `memo` 参数;

-   为操作 `delegate` 添加了 `memo` 参数;
---
## 更改日志v0.3.1

-   增加了NFT的内部操作 `createlog` 。由创建操作用于记录资产ID，以便第三方资源管理器可以轻松获取新的资产ID和其他信息。

-   增加了新的单例表 `tokenconfigs`。它有助于外部合约正确解析操作和表格（对于分散交换，市场和使用多个代币的其他合约有用）。市场，交易所和其他依赖合约将能够使用以下代码查看此信息。
```
Configs configs("simpleassets"_n, "simpleassets"_n.value);

configs.get("simpleassets"_n);
```
-   增加了操作 `updatever`。它为第三方钱包，市场等更新了SimpleAstes部署的版本;

-   事件通知的新示例： <https://github.com/CryptoLions/SimpleAssets-EventReceiverExample>
---
## 更改日志v0.3.0

-   使用延迟事务添加了事件通知。资产作者将收到有关资产创建，转移，索赔或烧录的通知。要收到它，请为您的创建者合约添加以下操作：
```
ACTION saecreate ( name owner, uint64_t assetid );

ACTION saetransfer ( name from, name to, std::vector\<uint64_t\>& assetids,
std::string memo );

ACTION saeclaim ( name account, std::vector\<uint64_t\>& assetids );

ACTION saeburn ( name account, std::vector\<uint64_t\>& assetids, std::string
memo );
```
-   `untildate` 参数更改为 `period`（以秒为单位）的操作 `delegate` 和表 `sdelegates` 
---
## 更改日志v0.2.0

### 使用eosio.token合约添加了可替代代币（Fungible Token）表和逻辑，但有一些更改

-   新的操作和逻辑：`createf`，`issuef`，`transfer`，`burnf`，`openf`，`closef`

-   添加了新表 `stat(supply, max_supply, issuer, id)` 和 `accounts (id, balance)` 。

-   统计表的范围（关于可替代代币的信息）已更改为创建者

-   `accountstable` 的主索引是在 `create` f操作上创建的 `uniq id` 并存储在 `stats` 表中。

-   添加 `createf` 与 `parameter` 可替代代币操作 `authorctrl` 至 `stats` 表。如果为true（1）允许代币创建者（而不仅仅是所有者）使用burnf和transferf。创建后无法更改！

-   李嘉图合约已更新

-   以下有更多用法示例
---
## 更改日志v0.1.1

**杂项**

-  sdelagate 表结构重命名为 sdelegate（typo）

- 创建操作参数重命名：requireClaim - \> requireclaim

- assetID操作参数在所有要声明的操作中重命名

**借入资产**

- sdelegate表 - 添加了新字段：untildate

- 委托操作添加参数untildate。如果参数输入正确（零或将来），操作会进行简单检查。

- undelegate在不公开之前不会工作（这保证了资产贷款的最低期限）。

- 如果被委托，允许转移资产（返还）早于截至时间（借款人可以提前返还）

**批量处理**

- 声明操作（claim action）：assetid参数已更改为assetsids数组。添加了多个声明逻辑。

- 报价操作（offer action）：assetid参数已更改为assetsids数组。添加了多个提供逻辑。

- 取消报价操作（canceloffer action）：assetid参数已更改为assetsids数组。添加了多个取消逻辑。

- 传输操作（transfer action）：assetid参数已更改为assetsids数组。添加了多个资产转移逻辑。

- 烧录操作（burn action）：assetid参数已更改为assetsids数组。添加了多个刻录逻辑。

- 委托/非委托操作（delegate / undelegated action）：assetid参数已更改为assetsids数组。添加了多个委托/取消授权逻辑。









