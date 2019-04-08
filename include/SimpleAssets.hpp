/**
 *  SimpleAssets (Digital Assets)
 *  (C) 2019 by CryptoLions [ https://CryptoLions.io ]
 *
 *  A simple standard for digital assets (ie. Fungible and Non-Fungible Tokens - NFTs) for EOSIO blockchains
 *
 *    WebSite:        https://simpleassets.io
 *    GitHub:         https://github.com/CryptoLions/SimpleAssets
 *    Presentation:   https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4
 *
 *    Event Receiver: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample
 *
 */


#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>

using namespace eosio;
using std::string;

CONTRACT SimpleAssets : public contract {
	//=============================================================================================================================
	//=============================================================================================================================
	public:
		using contract::contract;

		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Update version of this SimpleAstes deployment for 3rd party wallets, marketplaces, etc
		*/
		ACTION updatever( string version);
		using updatever_action = action_wrapper<"updatever"_n, &SimpleAssets::updatever>;


		// ===============================================================================================
		// ============= Non-Fungible Token Actions ======================================================
		// ===============================================================================================
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* New Author registration. Action is not mandatory.  Markets *may* choose to use information here
		* to display info about the author, and to follow specifications expressed here for displaying
		* asset fields.
		*
		* author	- authors account who will create assets;
		* data		- stringified json. Recommendations to include: game, company, logo, url, desc;
		* stemplate	- stringified json with key:state values, where key is key from mdata or idata and
		*		  state indicates recommended way of displaying field:
		*		  url, img, webgl, mp3, video, hide (ie. don't display), etc.
		*/
		ACTION regauthor( name author, string data, string stemplate);
		using regauthor_action = action_wrapper<"regauthor"_n, &SimpleAssets::regauthor>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Authors info update. Used to updated author information, and asset display recommendations created
		* with the regauthor action. This action replaces the fields data and stemplate.
		*
		* To remove author entry, call this action with null strings for data and stemplate.
		*
		* author	- authors account who will create assets;
		* data		- stringified json. Recommendations to include: game, company, logo, url, desc;
		* stemplate	- stringified json with key:state values, where key is key from mdata or idata and
		*		  state indicates recommended way of displaying field:
		*		  url, img, webgl, mp3, video, hide (ie. don't display), etc.
		*/
		ACTION authorupdate( name author, string data, string stemplate);
		using authorupdate_action = action_wrapper<"authorupdate"_n, &SimpleAssets::authorupdate>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Create a new asset.
		*
		* author	- asset's author, who will able to updated asset's mdata;
		* category	- assets category;
		* owner		- assets owner;
		* idata		- stringified json or just sha256 string with immutable assets data;
		* mdata		- stringified json or just sha256 string with mutable assets data, can be changed only by author;
		* requireclaim	- true or false. If disabled, upon creation, the asset will be transfered to owner (but
		*		  but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
		*		  author will remain the owner, but an offer will be created for the account specified in
		*		  the owner field to claim the asset using the account's RAM.
		*/
		ACTION create( name author, name category, name owner, string idata, string mdata, bool requireclaim);
		using create_action = action_wrapper<"create"_n, &SimpleAssets::create>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Empty action. Used by create action to log assetid so that third party explorers can
		* easily get new asset ids and other information.
		*/
		ACTION createlog( name author, name category, name owner, string idata, string mdata, uint64_t assetid);
		using createlog_action = action_wrapper<"createlog"_n, &SimpleAssets::createlog>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Claim the specified asset (assuming it was offered to claimer by the asset owner).
		*
		* claimer	- account claiming the asset;
		* assetids	- array of assetid's to claim;
		*/
		ACTION claim( name claimer, std::vector<uint64_t>& assetids);
		using claim_action = action_wrapper<"claim"_n, &SimpleAssets::claim>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* This actions transfers an asset. On transfer owner asset's and scope asset's changes to {{to}}'s.
		* Senders RAM will be charged to transfer asset.
		* Transfer will fail if asset is offered for claim or is delegated.
		*
		* from		- account who sends the asset;
		* to		- account of receiver;
		* assetids	- array of assetid's to transfer;
		* memo		- transfers comment;
		*/
		ACTION transfer( name from, name to, std::vector<uint64_t>& assetids, string memo);
		using transfer_action = action_wrapper<"transfer"_n, &SimpleAssets::transfer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Update assets mutable data (mdata) field. Action is available only for authors.
		*
		* author	- authors account;
		* owner		- current assets owner;
		* assetid	- assetid to update;
		* mdata		- stringified json with mutable assets data. All mdata will be replaced;
		*/
		ACTION update( name author, name owner, uint64_t assetid, string mdata );
		using update_action = action_wrapper<"update"_n, &SimpleAssets::update>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Offer asset for claim. This is an alternative to the transfer action. Offer can be used by an
		* asset owner to transfer the asset without using their RAM. After an offer is made, the account
		* specified in {{newowner}} is able to make a claim, and take control of the asset using their RAM.
		*
		* Offer action is not available if an asste is delegated (borrowed).
		*
		* owner		- current asset owner account;
		* newowner	- new asset owner, who will able to claim;
		* assetids	- array of assetid's to offer;
		* memo		- memo for offer action;
		*/
		ACTION offer( name owner, name newowner, std::vector<uint64_t>& assetids, string memo);
		using offer_action = action_wrapper<"offer"_n, &SimpleAssets::offer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Cancel and remove offer. Available for the asset owner.
		*
		* owner		- current asset owner account;
		* assetids	- array of assetid's to cancel from offer;
		*/
		ACTION canceloffer( name owner, std::vector<uint64_t>& assetids);
		using canceloffer_action = action_wrapper<"canceloffer"_n, &SimpleAssets::canceloffer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Burns asset {{assetid}}. This action is only available for the asset owner. After executing, the
		* asset will disappear forever, and RAM used for asset will be released.
		*
		* owner		- current asset owner account;
		* assetids	- array of assetid's to burn;
		* memo		- memo for burn action;
		*/
		ACTION burn( name owner, std::vector<uint64_t>& assetids, string memo );
		using burn_action = action_wrapper<"burn"_n, &SimpleAssets::burn>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Delegates asset to {{to}}. This action changes the asset owner by calling the transfer action.
		* It also adds a record in the delegates table to record the asset as borrowed.  This blocks
		* the asset from all owner actions (transfers, offers, burning by borrower).
		*
		* owner		- current asset owner account;
		* to		- borrower account name;
		* assetids	- array of assetid's to delegate;
		* period	- time in seconds that the asset will be lent. Lender cannot undelegate until
		*		  the period expires, however the receiver can transfer back at any time;
		* memo		- memo for delegate action;
		*/
		ACTION delegate( name owner, name to, std::vector<uint64_t>& assetids, uint64_t period, string memo );
		using delegate_action = action_wrapper<"delegate"_n, &SimpleAssets::delegate>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Undelegates an asset from {{from}} account. Executing action by real owner will return asset immediately,
		* and the entry in the delegates table recording the borrowing will be erased.
		*
		* owner		- real asset owner account;
		* from		- current account owner (borrower);
		* assetids	- array of assetid's to undelegate;
		*/
		ACTION undelegate( name owner, name from, std::vector<uint64_t>& assetids );
		using undelegate_action = action_wrapper<"undelegate"_n, &SimpleAssets::undelegate>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Attach other NFTs to the specified NFT. Restrictions:
		* 1. Only the Asset Author can do this
		* 2. All assets must have the same author
		* 3. All assets much have the same owner
		*
		* owner		- owner of NFTs;
		* assetidc	- id of container NFT;
		* assetids	- array of asset ids to attach;
		*/
		ACTION attach( name owner, uint64_t assetidc, std::vector<uint64_t>& assetids );
		using attach_action = action_wrapper<"attach"_n, &SimpleAssets::attach>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Detach NFTs from the specified NFT.
		*
		* owner		- owner of NFTs;
		* assetidc	- the id of the NFT from which we are detaching;
		* assetids	- the ids of the NFTS to be detached;
		*/
		ACTION detach( name owner, uint64_t assetidc, std::vector<uint64_t>& assetids );
		using detach_t_action = action_wrapper<"detach"_n, &SimpleAssets::detach>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Attach FTs to the specified NFT. Restrictions:
		* 1. Only the Asset Author can do this
		* 2. All assets must have the same author
		* 3. All assets much have the same owner
		*
		* owner		- owner of assets;
		* author	- author of the assets;
		* assetidc	- id of container NFT;
		* quantity	- quantity to attach and token name (for example: "10 WOOD", "42.00 GOLD");
		*/
		ACTION attachf( name owner, name author, asset quantity, uint64_t assetidc );
		using attachf_t_action = action_wrapper<"attachf"_n, &SimpleAssets::attachf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Detach FTs from the specified NFT.
		*
		* owner		- owner of NFTs;
		* author	- author of the assets;
		* assetidc	- id of the container NFT;
		* quantity	- quantity to detach and token name (for example: 10 WOOD, 42.00 GOLD);
		*/
		ACTION detachf( name owner, name author, asset quantity, uint64_t assetidc );
		using detachf_t_action = action_wrapper<"detachf"_n, &SimpleAssets::detachf>;



		// ===============================================================================================
		// ============= Fungible Token Actions ==========================================================
		// ===============================================================================================
		//
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Creates fungible token with specified maximum supply; You can not change anything after creation.
		*
		* author         - fungible token author;
		* maximum_supply - maximum token supply, example "10000000.0000 GOLD", "10000000 SEED", "100000000.00 WOOD".
		*                  Precision is also important here;
		* authorctrl     - IMPORTANT! If true(1) allows token author (and not just owner) to burnf and transferf.
		*		   Cannot be changed after creation!
		* data           - stringify json (recommend including keys `img` and `name` for better displaying by markets);
		*/
		ACTION createf( name author, asset maximum_supply, bool authorctrl, string data);
		using createf_action = action_wrapper<"createf"_n, &SimpleAssets::createf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Update the data field of a fungible token.
		*
		* author - fungible token author;
		* sym    - fingible token symbol ("GOLD", "WOOD", etc.);
		* data   - stringify json (recommend including keys `img` and `name` for better displaying by markets);
		*/
		ACTION updatef( name author, symbol sym, string data);
		using updatef_action = action_wrapper<"updatef"_n, &SimpleAssets::updatef>;


		/*
		* This action issues a fungible token.
		*
		* to       - account receiver;
		* author   - fungible token author;
		* quantity - amount to issue, example "1000.00 WOOD";
		* memo     - issue comment;
		*/
		ACTION issuef( name to, name author, asset quantity, string memo );
		using issuef_action = action_wrapper<"issuef"_n, &SimpleAssets::issuef>;


		/*
		* This actions transfers a specified quantity of fungible tokens.
		*
		* from     - account who sends the token;
		* to       - account of receiver;
		* author   - account of fungible token author;
		* quantity - amount to transfer, example "1.00 WOOD";
		* memo     - transfer's comment;
		*/
		ACTION transferf( name from, name to, name author, asset quantity, string memo );
		using transferf_action = action_wrapper<"transferf"_n, &SimpleAssets::transferf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Offer fungible tokens for another EOS user to claim.
		* This is an alternative to the transfer action. Offer can be used by a
		* FT owner to transfer the FTs without using their RAM. After an offer is made, the account
		* specified in {{newowner}} is able to make a claim, and take control of the asset using their RAM.
		*
		* The FTs will be removed from the owner's balance while the offer is open.
		*
		* owner    - original owner of the FT;
		* newowner - account which will be able to claim the offer;
		* author   - account of fungible token author;
		* quantity - amount to transfer, example "1.00 WOOD";
		* memo     - offer's comment;
		*/
		ACTION offerf( name owner, name newowner, name author, asset quantity, string memo);
		using offerf_action = action_wrapper<"offerf"_n, &SimpleAssets::offerf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Cancels offer of FTs
		*
		* owner      - riginal owner of the FT;
		* ftofferids - id of the FT offer;
		*/
		ACTION cancelofferf( name owner, std::vector<uint64_t>& ftofferids);
		using cancelofferf_action = action_wrapper<"cancelofferf"_n, &SimpleAssets::cancelofferf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Claim FTs which have been offered
		*
		* claimer    - Account claiming FTs which have been offered;
		* ftofferids - array of FT offer ids;
		*/
		ACTION claimf( name claimer, std::vector<uint64_t>& ftofferids);
		using claimf_action = action_wrapper<"claimf"_n, &SimpleAssets::claimf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Burns a fungible token. This action is available for the token owner and author. After executing,
		* accounts balance and supply in stats table for this token will reduce by the specified quantity.
		*
		* from     - account who burns the token;
		* author   - account of fungible token author;
		* quantity - amount to burn, example "1.00 WOOD";
		* memo     - memo for burnf action;
		*/
		ACTION burnf( name from, name author, asset quantity, string memo );
		using burnf_action = action_wrapper<"burnf"_n, &SimpleAssets::burnf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Opens accounts table for specified fungible token.
		*
		* owner     - account where create table with fungible token;
		* author    - account of fungible token author;
		* symbol    - token symbol, example "WOOD", "ROCK", "GOLD";
		* ram_payer - account who will pay for ram used for table creation;
		*/
		ACTION openf( name owner, name author, const symbol& symbol, name ram_payer );
		using openf_action = action_wrapper<"openf"_n, &SimpleAssets::openf>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Closes accounts table for provided fungible token and releases RAM.
		* Action works only if balance is 0.
		*
		* owner  - account who woud like to close table with fungible token;
		* author - account of fungible token author;
		* symbol - token symbol, example "WOOD", "ROCK", "GOLD";
		*/
		ACTION closef( name owner, name author, const symbol& symbol );
		using closef_action = action_wrapper<"closef"_n, &SimpleAssets::closef>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Returns Current token supply.
		*
		* token_contract_account  - contract to check;
		* author                  - fungible tokens author account;
		* sym_code    	          - token symbol, example "WOOD", "ROCK", "GOLD";
		*/
		static asset get_supply( name token_contract_account, name author, symbol_code sym_code ) {
			stats statstable( token_contract_account, author.value );
			const auto& st = statstable.get( sym_code.raw() );
			return st.supply;
		}


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Returns token balance for account
		*
		* token_contract_account  - contract to check;
		* owner	                  - token holder account;
		* author                  - fungible tokens author account;
		* sym_code    	          - token symbol, example "WOOD", "ROCK", "GOLD";
		*/
		static asset get_balance( name token_contract_account, name owner, name author, symbol_code sym_code ) {
			stats statstable( token_contract_account, author.value );
			const auto& st = statstable.get( sym_code.raw() );

			accounts accountstable( token_contract_account, owner.value );
			const auto& ac = accountstable.get( st.id );
			return ac.balance;
		}



	//=============================================================================================================================
	//=============================================================================================================================
	private:

		uint64_t getid(bool defer);
		uint64_t getFTIndex(name author, symbol symbol);

		void attachdeatch( name owner, name author, asset quantity, uint64_t assetidc, bool attach );

		void sub_balancef( name owner, name author, asset value );
		void add_balancef( name owner, name author, asset value, name ram_payer );

		template<typename... Args>
		void sendEvent(name author, name rampayer, name seaction, const std::tuple<Args...> &tup);


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Authors table. Can be used by asset markets, asset explorers, or wallets for correct asset
		* data presentation.
		* Scope: self
		*/
		TABLE sauthor {
			name			author;
			string			data;
			string			stemplate;

			auto primary_key() const {
				return author.value;
			}

		};

		typedef eosio::multi_index< "authors"_n, sauthor > authors;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Fungible token accounts stats info: Max Supply, Current Supply, issuer (author), token unique id, authorctrl.
		* authorctrl if true(1) allow token author (and not just owner) to burn and transfer.
		* Scope: token author
		*/
		TABLE currency_stats {
			asset		supply;
			asset		max_supply;
			name		issuer;
			uint64_t 	id;
			bool		authorctrl;
			string		data;

			uint64_t primary_key()const {
				return supply.symbol.code().raw();
			}
		};

		typedef eosio::multi_index< "stat"_n, currency_stats > stats;

		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Fungible token accounts table which stores information about balances.
		* Scope: token owner
		*/
		TABLE account {
			uint64_t	id;
			name		author;
			asset		balance;

			uint64_t primary_key()const {
				return id;
			}
		};

		typedef eosio::multi_index< "accounts"_n, account > accounts;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Assets table which stores information about simple assets.
		* Scope: asset owner
		*/
		TABLE sasset {

			uint64_t                id;
			name                    owner;
			name                    author;
			name                    category;
			string                  idata; // immutable data
			string                  mdata; // mutable data
			std::vector<sasset>     container;
			std::vector<account>    containerf;

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


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Offers table keeps records of open offers of assets (ie. assets waiting to be claimed by their
		* intendend recipients.
		*
		* Scope: self
		*/
		TABLE soffer {
			uint64_t		assetid;
			name			owner;
			name			offeredto;
			uint64_t		cdate;

			auto primary_key() const {
				return assetid;
			}
			uint64_t by_owner() const {
				return owner.value;
			}
			uint64_t by_offeredto() const {
				return offeredto.value;
			}
		};

		typedef eosio::multi_index< "offers"_n, soffer,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun<soffer, uint64_t, &soffer::by_owner> >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun<soffer, uint64_t, &soffer::by_offeredto> >
			> offers;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Offers table keeps records of open offers of FT (ie. waiting to be claimed by their
		* intendend recipients.
		*
		* Scope: self
		*/
		TABLE sofferf {
			uint64_t		id;
			name			author;
			name			owner;
			asset			quantity;
			name			offeredto;
			uint64_t		cdate;

			auto primary_key() const {
				return id;
			}
			uint64_t by_owner() const {
				return owner.value;
			}
			uint64_t by_offeredto() const {
				return offeredto.value;
			}
		};

		typedef eosio::multi_index< "offerfs"_n, sofferf,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun<sofferf, uint64_t, &sofferf::by_owner> >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun<sofferf, uint64_t, &sofferf::by_offeredto> >
			> offerfs;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Delegates table keeps records about borrowed assets.
		* Scope: self
		*/
		TABLE sdelegate {
			uint64_t		assetid;
			name			owner;
			name			delegatedto;
			uint64_t		cdate;
			uint64_t		period;

			auto primary_key() const {
				return assetid;
			}
			uint64_t by_owner() const {
				return owner.value;
			}
			uint64_t by_delegatedto() const {
				return delegatedto.value;
			}
		};

		typedef eosio::multi_index< "delegates"_n, sdelegate,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun<sdelegate, uint64_t, &sdelegate::by_owner> >,
			eosio::indexed_by< "delegatedto"_n, eosio::const_mem_fun<sdelegate, uint64_t, &sdelegate::by_delegatedto> >
		> delegates;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* global singelton table, used for assetid building
		* Scope: self
		*/
		TABLE global {
			global(){}
			uint64_t lnftid	= 100000000000000;
			uint64_t defid	= 1000000;
			uint64_t spare1	= 0;
			uint64_t spare2	= 0;

			EOSLIB_SERIALIZE( global, (lnftid)(defid)(spare1)(spare2) )
		};

		typedef eosio::singleton< "global"_n, global> conf;
		global _cstate;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Helps external contracts parse actions and tables correctly (Usefull for decentralized exchanges,
		* marketplaces and other contracts that use multiple tokens)
		*
		* Marketplaces, exchanges and other reliant contracts will be able to view this info using the following code.
		*   Configs configs("simpl1assets"_n, "simpl1assets"_n.value);
		*	configs.get("simpl1assets"_n);

		*/
		TABLE tokenconfigs {
			name			standard;
			std::string		version;
		};

		typedef singleton<"tokenconfigs"_n, tokenconfigs> Configs;

};

//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================
