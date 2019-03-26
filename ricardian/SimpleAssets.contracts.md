<h1 class="contract"> regauthor </h1>
        ## ACTION NAME: regauthor

        ### INTENT
		New Author registration. Action is not mandatory.  Markets *may* choose to use information here 
		to display info about the author, and to follow specifications expressed here for displaying asset fields.

        ### Input parameters:
        `author`      - authors account who will create assets;
		`data`        - stringified json. Recommendations to include: game, company, logo, url, desc;
		`stemplate`   - stringified json with key:state values, where key is key from mdata or idata and 
		                state indicates recommended way of displaying field: 
		                         url, img, webgl, mp3, video, hide (ie. don't display), etc.

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> authorupdate </h1>
        ## ACTION NAME: authorupdate

        ### INTENT
		Authors info update. Used to updated author information, and asset display recommendations created with the regauthor action. This action replaces the fields data and stemplate.
		
		To remove author entry, call this action with null strings for data and stemplate.
		
        ### Input parameters:
        `author`      - authors account who will create assets; 
		`data`        - stringified json. Recommendations to include: game, company, logo, url, desc;
		`stemplate`   - stringified json with key:state values, where key is key from mdata or idata and 
		                state indicates recommended way of displaying field: 
		                          url, img, webgl, mp3, video, hide (ie. don't display), etc.
        
        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> create </h1>
        ## ACTION NAME: create

        ### INTENT
		Сreate a new asset.

        ### Input parameters:
        `author`         - asset's author, who will able to updated asset's mdata;
		`category`       - assets category;
		`owner`          - assets owner;
		`idata`          - stringified json with immutable assets data
		`mdata`          - stringified json with mutable assets data, can be changed only by author
		`requireclaim`   - true or false. If disabled, upon creation, the asset will be transfered to owner (but 
				           but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
				           author will remain the owner, but an offer will be created for the account specified in 
				           the owner field to claim the asset using the account's RAM.

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> claim </h1>
        ## ACTION NAME: claim

        ### INTENT
		Claim the specified asset (assuming it was offered to claimer by the asset owner).

        ### Input parameters:
		`claimer`  - account claiming the asset
		`assetids` - array of assetid's to claim

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> transfer </h1>
        ## ACTION NAME: transfer

        ### INTENT
		This actions transfers an asset. On transfer owner asset's and scope asset's changes to {{to}}'s.
		Senders RAM will be charged to transfer asset.
		Transfer will fail if asset is offered for claim or is delegated.

        ### Input parameters:
		`from`     - account who sends the asset;
		`to`       - account of receiver;
		`assetids` - array of assetid's to transfer;
		`memo`     - transfers comment;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> update </h1>
        ## ACTION NAME: update

        ### INTENT
		Update assets mutable data (mdata) field. Action is available only for authors.

        ### Input parameters:
		`author`  - authors account;
		`owner`   - current assets owner;
		`assetid` - assetid to update;
		`mdata`   - stringified json with mutable assets data. All mdata will be replaced;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> offer </h1>
        ## ACTION NAME: offer

        ### INTENT
		Offer asset for claim. This is an alternative to the transfer action. Offer can be used by an 
		asset owner to transfer the asset without using their RAM. After an offer is made, the account
		specified in {{newowner}} is able to make a claim, and take control of the asset using their RAM.
		Offer action is not available if an asste is delegated (borrowed).

        ### Input parameters:
		`owner`    - current asset owner account;
		`newowner` - new asset owner, who will able to claim;
		`assetids` - array of assetid's to offer;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> canceloffer </h1>
        ## ACTION NAME: canceloffer

        ### INTENT
		Cancel and remove offer. Available for the asset owner.

        ### Input parameters:
		`owner`    - current asset owner account;
		`assetids` - array of assetid's to cancel offer;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> burn </h1>
        ## ACTION NAME: burn

        ### INTENT
		Burns asset {{assetid}}. This action is only available for the asset owner. After executing, the 
		asset will disappear forever, and RAM used for asset will be released.

        ### Input parameters:
		`owner`    - current asset owner account;
		`assetids` - array of assetid's to burn;
		`memo`     - memo for burn action;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> delegate </h1>
        ## ACTION NAME: delegate

        ### INTENT
		Delegates asset to {{to}}. This action changes the asset owner by calling the transfer action.
		It also adds a record in the delegates table to record the asset as borrowed.  This blocks
		the asset from all owner actions (transfers, offers, burning by borrower).

        ### Input parameters:
		`owner`     - current asset owner account;
		`to`        - borrower account name;
		`assetids`  - array of assetid's to delegate;
		`period`    - time in seconds that the asset will be lent. Lender cannot undelegate until 
		              the period expires, however the receiver can transfer back at any time.

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]



<h1 class="contract"> undelegate </h1>
        ## ACTION NAME: undelegate

        ### INTENT
		Undelegates an asset from {{from}} account. Executing action by real owner will return asset immediately,
		and the entry in the delegates table recording the borrowing will be erased.

        ### Input parameters:
		`owner`    - real asset owner account;
		`from`     - current account owner (borrower);
		`assetids` - array of assetid's to undelegate;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]


<h1 class="contract"> createf </h1>
        ## ACTION NAME: createf

        ### INTENT
		Creates fungible token with specified maximum supply; You can not change anything after creation.

        ### Input parameters:
		`author`          - fungible token author;
		`maximum_supply`  - maximum token supply, example "10000000.0000 GOLD", "10000000 SEED", "100000000.00 WOOD". Precision is also important here;
		`authorctrl`      - if true(1) allow token author (and not just owner) to burnf and transferf. Cannot be changed after creation!
		
        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]
		
		
<h1 class="contract"> issuef </h1>
        ## ACTION NAME: issuef

        ### INTENT
		This action issues a fungible token.		

        ### Input parameters:
		`to`       - account receiver;
		`author`   - fungible token author;
		`quantity` - amount to issue, example "1000.00 WOOD";
		`memo`     - transfers memo;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]
		
		
<h1 class="contract"> transferf </h1>
        ## ACTION NAME: transferf
		This actions transfers an fungible token.
		
        ### INTENT
		This actions transfers a specified quantity of fungible tokens.

        ### Input parameters:
		`from`     - account who sends the token;
		`to`       - account of receiver;
		`author`   - account of fungible token author;
		`quantity` - amount to transfer, example "1.00 WOOD";
		`memo`     - transfers comment;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]

		
<h1 class="contract"> burnf </h1>
        ## ACTION NAME: burnf

        ### INTENT
		Burns a fungible token. This action is available for the token owner and author. After executing, 
		accounts balance and supply in stats table for this token will reduce by the specified quantity.

        ### Input parameters:
		`from`     - account who burns the token;
		`author`   - account of fungible token author;
		`quantity` - amount to burn, example "1.00 WOOD";
		`memo`     - memo for burnf action;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]

		
<h1 class="contract"> openf </h1>
        ## ACTION NAME: openf

        ### INTENT
		Opens accounts table for specified fungible token.

        ### Input parameters:
		`owner`     - account who woud like to close table with fungible token;
		`author`    - account of fungible token author;
		`symbol`    - token symbol, example "WOOD", "ROCK", "GOLD";
		`ram_payer` - account who will pay for ram used for table creation;

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]
		
<h1 class="contract"> closef </h1>
        ## ACTION NAME: closef

        ### INTENT
		Closes accounts table for provided fungible token and releases RAM.
		Action works only if balance is 0;

        ### Input parameters:
		`owner`  - account who woud like to close table with fungible token;
		`author` - account of fungible token author;
		`symbol` - token symbol, example "WOOD", "ROCK", "GOLD";

        ### TERM
        This Contract expires at the conclusion of code execution.

        by CryptoLions [ https://cryptolions.io ]
		
		

		
