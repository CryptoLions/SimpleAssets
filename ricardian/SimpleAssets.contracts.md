<h1 class="contract"> authorreg </h1>

---
spec_version: 0.0.2
title: New Author registration
summary: New Author registration 
icon: https://cryptolions.io/assets/images/sa-icons-256/regauthor.png#c6a539be8e7dfd1a4c466ba9cabfd13571cd77d5c988c652d2e8f87096f3548e
---

Action is not mandatory.  Markets *may* choose to use information here to display info about the author, and to follow specifications expressed here for displaying asset fields.

Input parameters:
`author`      -	authors account who will create assets;
`dappinfo`    - stringified json. Recommendations to include: game, company, logo, url, desc;
`fieldtypes`  - stringified json with key:state values, where key is key from mdata or idata and 
				state indicates recommended way of displaying field: 
				url, img, webgl, mp3, video, hide (ie. don't display), etc.
`priorityimg` - json with assosiation category with type of image or video 
				txt	- text (default)
				url	- show as clickable URL
				img	- link to img file
				webgl	- link to webgl file
				mp3	- link to mp3 file
				video	- link to video file
				hide	- do not show
				imgb 	- image as string in binary format
				webglb	- webgl binary
				mp3b 	- mp3 binary
				videob 	- video binary


TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> authorupdate </h1>

---
spec_version: 0.0.2
title: Authors info update
summary: Authors info update 
icon: https://cryptolions.io/assets/images/sa-icons-256/authorupdate.png#0b11c9c4e41b6ba1b00bd907c671f7ddc9e2f9caf26580f0b2e7c73e02f36ff3
---

Used to updated author information, and asset display recommendations created with the authorreg action. This action replaces the fields dappinfo and fieldtypes. To remove author entry, call this action with null strings for dappinfo and fieldtypes.

Input parameters:
`author`      - authors account who will create assets; 
`dappinfo`    - stringified json. Recommendations to include: game, company, logo, url, desc;
`fieldtypes`  - stringified json with key:state values, where key is key from mdata or idata and 
				state indicates recommended way of displaying field: 
				url, img, webgl, mp3, video, hide (ie. don't display), etc.
`priorityimg` - json with assosiation category with type of image or video 
				txt	- text (default)
				url	- show as clickable URL
				img	- link to img file
				webgl	- link to webgl file
				mp3	- link to mp3 file
				video	- link to video file
				hide	- do not show
				imgb 	- image as string in binary format
				webglb	- webgl binary
				mp3b 	- mp3 binary
				videob 	- video binary

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> changeauthor </h1>

---
spec_version: 0.0.2
title: Change author of assets
summary: Change author of assets
icon: https://cryptolions.io/assets/images/sa-icons-256/regauthor.png#c6a539be8e7dfd1a4c466ba9cabfd13571cd77d5c988c652d2e8f87096f3548e
---	

Input parameters:
`author`         - asset's author, who will able to change author name asset's;
`newauthor`      - asset's new author name;
`owner`          - assets owner;
`assetids`       - array of assetid's

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> create </h1>

---
spec_version: 0.0.2
title: Create a new asset
summary: Create a new asset
icon: https://cryptolions.io/assets/images/sa-icons-256/create.png#db2bc53cef5dbbce5247543a91234373ec4bd1baf743301a0d92a35b053eeb70
---	

Input parameters:
`author`         - asset's author, who will able to updated asset's mdata;
`category`       - assets category;
`owner`          - assets owner;
`idata`          - stringified json with immutable assets data
`mdata`          - stringified json with mutable assets data, can be changed only by author
`requireclaim`   - true or false. If disabled, upon creation, the asset will be transfered to owner (but 
				   but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
				   author will remain the owner, but an offer will be created for the account specified in 
				   the owner field to claim the asset using the account's RAM.

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> claim </h1>

---
spec_version: 0.0.2
title: Claim the specified asset
summary: Claim the specified asset (assuming it was offered to claimer by the asset owner)
icon: https://cryptolions.io/assets/images/sa-icons-256/claim.png#8c3fdf140ea14a3cb9762a0f6ec0f0d73f8c2eeeec3ea2f109f394a9f35f472d
---

Input parameters:
`claimer`  - account claiming the asset
`assetids` - array of assetid's to claim

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> transfer </h1>

---
spec_version: 0.0.2
title: This actions transfers an asset
summary: This actions transfers an asset
icon: https://cryptolions.io/assets/images/sa-icons-256/transfer.png#23227a4901414db5cd0973d1cc5df13f9b82375b21c6315fd3402cb4acd50cbf
---

On transfer owner asset's and scope asset's changes to {{to}}'s. Senders RAM will be charged to transfer asset. Transfer will fail if asset is offered for claim or is delegated.

Input parameters:
`from`     - account who sends the asset;
`to`       - account of receiver;
`assetids` - array of assetid's to transfer;
`memo`     - transfers comment;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> update </h1>

---
spec_version: 0.0.2
title: Update assets mutable data
summary: Update assets mutable data (mdata) field. Action is available only for authors
icon: https://cryptolions.io/assets/images/sa-icons-256/update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e
---

Input parameters:
`author`  - authors account;
`owner`   - current assets owner;
`assetid` - assetid to update;
`mdata`   - stringified json with mutable assets data. All mdata will be replaced;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> offer </h1>

---
spec_version: 0.0.2
title: Offer asset for claim
summary: Offer asset for claim
icon: https://cryptolions.io/assets/images/sa-icons-256/offer.png#4aea6612c87df6cde081689f861b55ddba2c277cc553f4faeb65a4cb26656c66
---

This is an alternative to the transfer action. Offer can be used by an asset owner to transfer the asset without using their RAM. After an offer is made, the account specified in {{newowner}} is able to make a claim, and take control of the asset using their RAM. Offer action is not available if an asste is delegated (borrowed). 

Input parameters:
`owner`    - current asset owner account;
`newowner` - new asset owner, who will able to claim;
`assetids` - array of assetid's to offer;
`memo`     - memo for offer action

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> canceloffer </h1>

---
spec_version: 0.0.2
title: Cancel and remove offer
summary: Cancel and remove offer. Available for the asset owner
icon: https://cryptolions.io/assets/images/sa-icons-256/canceloffer.png#83328bae49f18f0aeca6d04c2e3003e49637a462b44a492d3439a9c44724a654
---

Input parameters:
`owner`    - current asset owner account;
`assetids` - array of assetid's to cancel offer;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> burn </h1>

---
spec_version: 0.0.2
title: Burn asset
summary: Burn asset
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

This action is only available for the asset owner. After executing, the asset will disappear forever, and RAM used for asset will be released.

Input parameters:
`owner`    - current asset owner account;
`assetids` - array of assetid's to burn;
`memo`     - memo for burn action;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> delegatemore </h1>

---
spec_version: 0.0.2
title: Extend period of delegating of asset
summary: Extend period of delegating of asset
icon: https://cryptolions.io/assets/images/sa-icons-256/delegate_more.png#6d1f7319569e3780e25cfb717d83f4ad58c91c0890ab234f106d1593956dfd10
---

Input parameters:
`owner`     - current asset owner account;
`assetidc`  - assetid to delegate;
`period`    - time in seconds that the asset will be lent. Lender cannot undelegate until 
			  the period expires, however the receiver can transfer back at any time.

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> delegate </h1>

---
spec_version: 0.0.2
title: Delegates asset
summary: Delegates asset
icon: https://cryptolions.io/assets/images/sa-icons-256/delegate.png#f8d91c1b9c0f376464e69aadf41142a17492af1e75d394601872b6a992b420bc
---

This action changes the asset owner by calling the transfer action. It also adds a record in the delegates table to record the asset as borrowed. This block the asset from all owner actions (transfers, offers, burning by borrower).

Input parameters:
`owner`     - current asset owner account;
`to`        - borrower account name;
`assetids`  - array of assetid's to delegate;
`period`    - time in seconds that the asset will be lent. Lender cannot undelegate until 
			  the period expires, however the receiver can transfer back at any time;
`memo`      - memo for delegate action;
`autoreturn`- automatic return for delegated action;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> undelegate </h1>

---
spec_version: 0.0.2
title: Undelegates an asset
summary: Undelegates an asset
icon: https://cryptolions.io/assets/images/sa-icons-256/updatef.png#f5d5af2ed6af508777f9f0b3c3a8e24b61eeca2ea623cfec6f6d0e9ea2c4d1a7
---

Executing action by real owner will return asset immediately, and the entry in the delegates table recording the borrowing will be erased.

Input parameters:
`owner`    - real asset owner account;
`from`     - current account owner (borrower);
`assetids` - array of assetid's to undelegate;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> createf </h1>

---
spec_version: 0.0.2
title: Creates fungible token
summary: Creates fungible token
icon: https://cryptolions.io/assets/images/sa-icons-256/claimf.png#80086207646470fa9cbaf4638ba0dedec96ea0e84d5059b567adc9e4b8543c30
---

Creates fungible token with specified maximum supply; You can not change anything after creation.

Input parameters:
`author`         - fungible token author;
`maximum_supply` - maximum token supply, example "10000000.0000 GOLD", "10000000 SEED", "100000000.00 WOOD". Precision is also important here;
`authorctrl`     - if true(1) allow token author (and not just owner) to burnf and transferf. Cannot be changed after creation!
`data`           - stringify json (recommend including keys `img` and `name` for better displaying by markets)

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> issuef </h1>

---
spec_version: 0.0.2
title: Issue a fungible token
summary: This action issues a fungible token
icon: https://cryptolions.io/assets/images/sa-icons-256/issuef.png#a40d77b2162d646cd3ce0488dfeab1cdfae8801fcc1d0c6ef1f16f2547b16551
---

Input parameters:
`to`       - account receiver;
`author`   - fungible token author;
`quantity` - amount to issue, example "1000.00 WOOD";
`memo`     - transfers memo;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> transferf </h1>

---
spec_version: 0.0.2
title: Transfer a fungible token
summary: This action transfer the specified quantity of fungible tokens
icon: https://cryptolions.io/assets/images/sa-icons-256/transfer.png#23227a4901414db5cd0973d1cc5df13f9b82375b21c6315fd3402cb4acd50cbf
---

Input parameters:
`from`     - account who sends the token;
`to`       - account of receiver;
`author`   - account of fungible token author;
`quantity` - amount to transfer, example "1.00 WOOD";
`memo`     - transfers comment;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> burnf </h1>

---
spec_version: 0.0.2
title: Burns a fungible token
summary: Burns a fungible token
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

This action is available for the token owner and author. After executing, accounts balance and supply in stats table for this token will reduce by the specified quantity

Input parameters:
`from`     - account who burns the token;
`author`   - account of fungible token author;
`quantity` - amount to burn, example "1.00 WOOD";
`memo`     - memo for burnf action;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> openf </h1>

---
spec_version: 0.0.2
title: Opens accounts table for fungible token
summary: Opens accounts table for specified fungible token
icon: https://cryptolions.io/assets/images/sa-icons-256/attach.png#0b039adbbe1011a494959963917df4765dbae378004e00911464f865e58ef452
---

Input parameters:
`owner`     - account who woud like to close table with fungible token;
`author`    - account of fungible token author;
`symbol`    - token symbol, example "WOOD", "ROCK", "GOLD";
`ram_payer` - account who will pay for ram used for table creation;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> closef </h1>

---
spec_version: 0.0.2
title: Closes accounts table
summary: Action works only if balance is 0.  for provided fungible token and releases RAM
icon: https://cryptolions.io/assets/images/sa-icons-256/attach.png#0b039adbbe1011a494959963917df4765dbae378004e00911464f865e58ef452
---

Input parameters:
`owner`  - account who woud like to close table with fungible token;
`author` - account of fungible token author;
`symbol` - token symbol, example "WOOD", "ROCK", "GOLD";

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> attach </h1>

---
spec_version: 0.0.2
title: Attach other NFTs to the specified NFT
summary: Attach other NFTs to the specified NFT
icon: https://cryptolions.io/assets/images/sa-icons-256/attach.png#0b039adbbe1011a494959963917df4765dbae378004e00911464f865e58ef452
---

Restrictions. Only the Asset Author can do this. All assets must have the same author. All assets much have the same owner. 

Input parameters:
`owner`	   - owner of NFTs
`assetidc` - id of container NFT
`assetids` - array of asset ids to attach	

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> detach </h1>

---
spec_version: 0.0.2
title: Detach NFTs from the specified NFT
summary: Detach NFTs from the specified NFT
icon: https://cryptolions.io/assets/images/sa-icons-256/detach.png#a075de69e2cf8eed78306834cc5688b64ce6ed660c0db762ae20c7c7a1d19a6b
---

Input parameters:
`owner`    - owner of NFTs
`assetidc` - the id of the NFT from which we are detaching
`assetids` - the ids of the NFTS to be detached

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> attachf </h1>

---
spec_version: 0.0.2
title: Attach FTs to the specified NFT
summary: Attach FTs to the specified NFT 
icon: https://cryptolions.io/assets/images/sa-icons-256/attachf.png#2d190203c6ae474e01cd70963c82a54cb542d95e29174e7572acb8c7653d8a8c
---

Restrictions. Only the Asset Author can do this. All assets must have the same author. All assets much have the same owner 

Input parameters:
`owner`	   - owner of assets
`author`   - author of the assets
`assetidc` - id of container NFT
`quantity` - quantity to attach and token name (for example: "10 WOOD", "42.00 GOLD")

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> detachf </h1>

---
spec_version: 0.0.2
title: Detach FTs from the specified NFT
summary: Action is not mandatory
icon: https://cryptolions.io/assets/images/sa-icons-256/detachf.png#2476bc6b7c02a4ec7e0e307b60f44a02486b4bebb4d91714817e5816063198e8
---

Input parameters:
`owner`    - owner of NFTs
`author`   - author of the assets
`assetidc` - id of the container NFT
`quantity` - quantity to detach and token name (for example: "10 WOOD", "42.00 GOLD")

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> updatef </h1>

---
spec_version: 0.0.2
title: Update the data field of a fungible token
summary: Update the data field of a fungible token
icon: https://cryptolions.io/assets/images/sa-icons-256/updatef.png#f5d5af2ed6af508777f9f0b3c3a8e24b61eeca2ea623cfec6f6d0e9ea2c4d1a7
---

Input parameters:
`author` - fungible token author;
`sym`    - fingible token symbol ("GOLD", "WOOD", etc.)
`data`   - stringify json (recommend including keys `img` and `name` for better displaying by markets)

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> offerf </h1>

---
spec_version: 0.0.2
title: Offer fungible tokens to claim
summary: Offer fungible tokens for another EOS user to claim
icon: https://cryptolions.io/assets/images/sa-icons-256/offerf.png#a34462918e536372610a523225e5221cd2685eff3c329499ded7298907f8524b
---

This is an alternative to the transfer action. Offer can be used by a FT owner to transfer the FTs without using their RAM. After an offer is made, the account specified in new owner is able to make a claim, and take control of the asset using their RAM. FTs will be removed from the owner's balance while the offer is open

Input parameters:
`owner`    - original owner of the FTs
`newowner` - account which will be able to claim the offer
`author`   - account of fungible token author;	
`quantity` - amount to transfer, example "1.00 WOOD";
`memo`     - offer's comment;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> cancelofferf </h1>

---
spec_version: 0.0.2
title: Cancels offer of FTs
summary: Cancels offer of FTs 
icon: https://cryptolions.io/assets/images/sa-icons-256/cancelofferf.png#9c35f488e2c53464f430e0e31e5a4b3bd2e914fe3b6db126722166a8a408fa83
---

Input parameters:
`owner`      - riginal owner of the FT
`ftofferids` - id of the FT offer

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> claimf </h1>

---
spec_version: 0.0.2
title: Claim FTs which have been offered
summary: Claim FTs which have been offered
icon: https://cryptolions.io/assets/images/sa-icons-256/claimf.png#80086207646470fa9cbaf4638ba0dedec96ea0e84d5059b567adc9e4b8543c30
---

Input parameters:
`claimer`    - Account claiming FTs which have been offered
`ftofferids` - array of FT offer ids

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]
	
<h1 class="contract"> updatever </h1>

---
spec_version: 0.0.2
title: Update version (internal)
summary: Update version (internal)
icon: https://cryptolions.io/assets/images/sa-icons-256/claimf.png#80086207646470fa9cbaf4638ba0dedec96ea0e84d5059b567adc9e4b8543c30
---

<h1 class="contract"> createlog </h1>		

---
spec_version: 0.0.2
title: createlog (internal)
summary: createlog (internal) 
icon: https://cryptolions.io/assets/images/sa-icons-256/claimf.png#80086207646470fa9cbaf4638ba0dedec96ea0e84d5059b567adc9e4b8543c30
---

<h1 class="contract"> burnlog </h1>

---
spec_version: 0.0.2
title: burnlog (internal)
summary: burnlog (internal) 
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

<h1 class="contract"> burnflog </h1>

---
spec_version: 0.0.2
title: burnflog (internal)
summary: burnflog (internal) 
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

<h1 class="contract"> burnnttlog </h1>

---
spec_version: 0.0.2
title: burnnttlog (internal)
summary: burnnttlog (internal) 
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

<h1 class="contract"> createntt </h1>

---
spec_version: 0.0.2
title: create Non transferable token (NTT)
summary: This action create a new Non transferable token (NTT).
icon: https://cryptolions.io/assets/images/sa-icons-256/create.png#db2bc53cef5dbbce5247543a91234373ec4bd1baf743301a0d92a35b053eeb70
---

Input parameters:
`owner`    - original owner of the FTs
`newowner` - account which will be able to claim the offer
`author`   - account of fungible token author;	
`quantity` - amount to transfer, example "1.00 WOOD";
`memo`     - offer's comment;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> claimntt </h1>

---
spec_version: 0.0.2
title: Claim the specified asset
summary: Claim the specified asset (assuming it was offered to claimer by the asset owner)
icon: https://cryptolions.io/assets/images/sa-icons-256/claim.png#8c3fdf140ea14a3cb9762a0f6ec0f0d73f8c2eeeec3ea2f109f394a9f35f472d
---

Input parameters:
`claimer`  - account claiming the asset
`assetids` - array of assetid's to claim

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]


<h1 class="contract"> createnttlog </h1>
		
---
spec_version: 0.0.2
title: createlog (internal)
summary: createlog (internal) 
icon: https://cryptolions.io/assets/images/sa-icons-256/claimf.png#80086207646470fa9cbaf4638ba0dedec96ea0e84d5059b567adc9e4b8543c30
---

<h1 class="contract"> burnntt </h1>

---
spec_version: 0.0.2
title: Burn non transferable asset
summary: Burn non transferable asset
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

This action is only available for the asset owner. After executing, the asset will disappear forever, and RAM used for asset will be released.

Input parameters:
`owner`    - current asset owner account;
`assetids` - array of assetid's to burn;
`memo`     - memo for burn action;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> updatentt </h1>

---
spec_version: 0.0.2
title: Update assets mutable data
summary: Update assets mutable data (mdata) field. Action is available only for authors
icon: https://cryptolions.io/assets/images/sa-icons-256/update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e
---

Input parameters:
`author`  - authors account;
`owner`   - current assets owner;
`assetid` - assetid to update;
`mdata`   - stringified json with mutable assets data. All mdata will be replaced;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> mdadd </h1>

---
spec_version: 0.0.2
title: Create new more date
summary: Create new more date
icon: https://cryptolions.io/assets/images/sa-icons-256/update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e
---

Input parameters:
`author`  - authors account;
`data`   - stringified json with mutable assets data;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> mdaddlog </h1>

---
spec_version: 0.0.2
title: Internal action used for creating log
summary: Internal action used for creating log
icon: https://cryptolions.io/assets/images/sa-icons-256/update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e
---

Input parameters:
`id` - is id of more data;
`author` - authors account;
`data` - stringified json with mutable assets data;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> mdupdate </h1>

---
spec_version: 0.0.2
title: Update existing more data
summary: Update existing more data
icon: https://cryptolions.io/assets/images/sa-icons-256/update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e
---

Input parameters:
`id` - is id of more data;
`author` - authors account;
`data` - stringified json with mutable assets data;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> mdremove </h1>

---
spec_version: 0.0.2
title: Update existing more data
summary: Update existing more data
icon: https://cryptolions.io/assets/images/sa-icons-256/update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e
---

Input parameters:
`id` - is id of more data;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> saetransfer </h1>

---
spec_version: 0.0.2
title: Internal action used for creating log
summary: Internal action used for creating log
icon: https://cryptolions.io/assets/images/sa-icons-256/transfer.png#23227a4901414db5cd0973d1cc5df13f9b82375b21c6315fd3402cb4acd50cbf
---

This action is called during transfer in deferred transaction 

Input parameters:
`author`   - asset's author;
`from`     - account who sends the asset;
`to`       - account of receiver;
`assetids` - array of assetid's to transfer;
`memo`     - transfers comment;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> saeburn </h1>

---
spec_version: 0.0.2
title: Internal action used for creating log
summary: Internal action used for creating log
icon: https://cryptolions.io/assets/images/sa-icons-256/burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84
---

This action is called during burn in deferred transaction 

Input parameters:
`author`   - asset's author;
`owner`    - current asset owner account;
`assetids` - array of assetid's to burn;
`memo`     - memo for burn action;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> saechauthor </h1>

---
spec_version: 0.0.2
title: Internal action used for creating log
summary: Internal action used for creating log
icon: https://cryptolions.io/assets/images/sa-icons-256/regauthor.png#c6a539be8e7dfd1a4c466ba9cabfd13571cd77d5c988c652d2e8f87096f3548e
---	

Input parameters:
`author`         - asset's author, who will able to change author name asset's;
`newauthor`      - asset's new author name;
`owner`          - assets owner;
`assetids`       - array of assetid's

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> saecreate </h1>

---
spec_version: 0.0.2
title: Internal action used for creating log
summary: Internal action used for creating log
icon: https://cryptolions.io/assets/images/sa-icons-256/create.png#db2bc53cef5dbbce5247543a91234373ec4bd1baf743301a0d92a35b053eeb70
---	

Input parameters:
`author`         - asset's author, who will able to updated asset's mdata;
`category`       - assets category;
`owner`          - assets owner;
`idata`          - stringified json with immutable assets data
`mdata`          - stringified json with mutable assets data, can be changed only by author
`requireclaim`   - true or false. If disabled, upon creation, the asset will be transfered to owner (but 
				   but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
				   author will remain the owner, but an offer will be created for the account specified in 
				   the owner field to claim the asset using the account's RAM.

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> saeclaim </h1>

---
spec_version: 0.0.2
title: Internal action used for creating log
summary: Internal action used for creating log
icon: https://cryptolions.io/assets/images/sa-icons-256/claim.png#8c3fdf140ea14a3cb9762a0f6ec0f0d73f8c2eeeec3ea2f109f394a9f35f472d
---

Input parameters:
`author`   - asset's author, who will able to updated asset's mdata;
`claimer`  - account claiming the asset;
`assetids` - array of assetid's to claim;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> setarampayer </h1>

---
spec_version: 0.0.2
title: Action for setting a ram payer for author and category.
summary: Action for setting a ram payer for author and category.
icon: https://cryptolions.io/assets/images/sa-icons-256/claim.png#8c3fdf140ea14a3cb9762a0f6ec0f0d73f8c2eeeec3ea2f109f394a9f35f472d
---

Input parameters:
`author`   - asset's author, who will able to updated asset's mdata;
`category` - assets category;
`usearam`  - flag for on or off author is a ram payer functionaity;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

<h1 class="contract"> delarampayer </h1>

---
spec_version: 0.0.2
title: Action for deleting a ram payer record;
summary: Action for deleting a ram payer record;
icon: https://cryptolions.io/assets/images/sa-icons-256/claim.png#8c3fdf140ea14a3cb9762a0f6ec0f0d73f8c2eeeec3ea2f109f394a9f35f472d
---

Input parameters:
`id` - is id of a ram payer record;

TERM
This Contract expires at the conclusion of code execution.
by CryptoLions [ https://cryptolions.io ]

