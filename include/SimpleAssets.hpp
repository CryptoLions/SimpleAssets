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
 */

 
/**
*  MIT License
* 
* Copyright (c) 2019 Cryptolions.io
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>

using namespace eosio;
using std::string;

CONTRACT SimpleAssets : public contract {
   //=============================================================================================================================
   //=============================================================================================================================
   public:
		using contract::contract;

		
		// ===============================================================================================
		// ============= Non-Fungible Token Actions ======================================================
		// ===============================================================================================
		
		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* New Author registration. Action is not mandatory.  Markets *may* choose to use information here
		* to display info about the author, and to follow specifications expressed here for displaying
		* asset fields.
		*
		* author      - authors account who will create assets;
		* data        - stringified json. Recommendations to include: game, company, logo, url, desc;
		* stemplate   - stringified json with key:state values, where key is key from mdata or idata and 
		*               state indicates recommended way of displaying field: 
		*                         url, img, webgl, mp3, video, hide (ie. don't display), etc.
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
		* author      - authors account who will create assets; 
		* data        - stringified json. Recommendations to include: game, company, logo, url, desc;
		* stemplate   - stringified json with key:state values, where key is key from mdata or idata and 
		*               state indicates recommended way of displaying field: 
		*                         url, img, webgl, mp3, video, hide (ie. don't display), etc.
		*/	
		ACTION authorupdate( name author, string data, string stemplate);
		using authorupdate_action = action_wrapper<"authorupdate"_n, &SimpleAssets::authorupdate>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Create a new asset.
		* 
		* author         - asset's author, who will able to updated asset's mdata;
		* category       - assets category;
		* owner          - assets owner;
		* idata          - stringified json or just sha256 string with immutable assets data;
		* mdata          - stringified json or just sha256 string with mutable assets data, can be changed only by author
		* requireclaim   - true or false. If disabled, upon creation, the asset will be transfered to owner (but 
		*		           but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
		*		           author will remain the owner, but an offer will be created for the account specified in 
		*		           the owner field to claim the asset using the account's RAM.
		*/
		ACTION create( name author, name category, name owner, string idata, string mdata, bool requireclaim);
		using create_action = action_wrapper<"create"_n, &SimpleAssets::create>;

		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Claim the specified asset (assuming it was offered to claimer by the asset owner).
		* 
		* claimer  - account claiming the asset
		* assetids - array of assetid's to claim
		*/
		ACTION claim( name claimer, std::vector<uint64_t>& assetids);
		using claim_action = action_wrapper<"claim"_n, &SimpleAssets::claim>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* This actions transfers an asset. On transfer owner asset's and scope asset's changes to {{to}}'s.
		* Senders RAM will be charged to transfer asset.
		* Transfer will fail if asset is offered for claim or is delegated.
		* 
		* from     - account who sends the asset;
		* to       - account of receiver;
		* assetids - array of assetid's to transfer;
		* memo     - transfers comment;
		*/		
		ACTION transfer( name from, name to, std::vector<uint64_t>& assetids, string memo);
		using transfer_action = action_wrapper<"transfer"_n, &SimpleAssets::transfer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Update assets mutable data (mdata) field. Action is available only for authors.
		* 
		* author  - authors account;
		* owner   - current assets owner;
		* assetid - assetid to update;
		* mdata   - stringified json with mutable assets data. All mdata will be replaced;
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
		* owner    - current asset owner account;
		* newowner - new asset owner, who will able to claim;
		* assetids - array of assetid's to offer
		*/		
		ACTION offer( name owner, name newowner, std::vector<uint64_t>& assetids);
		using offer_action = action_wrapper<"offer"_n, &SimpleAssets::offer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Cancel and remove offer. Available for the asset owner.
		* 
		* owner    - current asset owner account;
		* assetids - array of assetid's to cancel from offer
		*/		
		ACTION canceloffer( name owner, std::vector<uint64_t>& assetids);
		using canceloffer_action = action_wrapper<"canceloffer"_n, &SimpleAssets::canceloffer>;

		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Burns asset {{assetid}}. This action is only available for the asset owner. After executing, the 
		* asset will disappear forever, and RAM used for asset will be released.
		* 
		* owner    - current asset owner account;
		* assetids - array of assetid's to burn;
		* memo     - memo for burn action;
		*/		
		ACTION burn( name owner, std::vector<uint64_t>& assetids, string memo );
		using burn_action = action_wrapper<"burn"_n, &SimpleAssets::burn>;

	  
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Delegates asset to {{to}}. This action changes the asset owner by calling the transfer action.
		* It also adds a record in the delegates table to record the asset as borrowed.  This blocks
		* the asset from all owner actions (transfers, offers, burning by borrower).
		* 
		* owner     - current asset owner account;
		* to        - borrower account name;
		* assetids  - array of assetid's to delegate;
		* untildate - untildate - The delegating account will not be able to undelegate before this date. 
		*             However, the receiver of the lent asset can transfer back any time
		*             Should be timestamp in future or 0;
		*/		
		ACTION delegate( name owner, name to, std::vector<uint64_t>& assetids, uint64_t untildate );
		using delegate_action = action_wrapper<"delegate"_n, &SimpleAssets::delegate>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Undelegates an asset from {{from}} account. Executing action by real owner will return asset immediately,
		* and the entry in the delegates table recording the borrowing will be erased.
		* 
		* owner    - real asset owner account;
		* from     - current account owner (borrower);
		* assetids - array of assetid's to undelegate;
		*/		
		ACTION undelegate( name owner, name from, std::vector<uint64_t>& assetids );
		using undelegate_action = action_wrapper<"undelegate"_n, &SimpleAssets::undelegate>;

		

		// ===============================================================================================
		// ============= Fungible Token Actions ==========================================================
		// ===============================================================================================
		//	
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Creates fungible token with specified maximum supply; You can not change anything after creation.
		* 
		* author          - fungible token author;
		* maximum_supply  - maximum token supply, example "10000000.0000 GOLD", "10000000 SEED", "100000000.00 WOOD" ;
		*                   Precision is also important here.
		* authorctrl      - IMPORTANT! If true(1) allows token author (and not just owner) to burnf and transferf. 
		*					Cannot be changed after creation!
		*/		
		ACTION createf( name author, asset maximum_supply, bool authorctrl);
		using createf_action = action_wrapper<"createf"_n, &SimpleAssets::createf>;
	
	
		/*
		* This action issues a fungible token.
		* 
		* to        - account receiver;
		* author    - fungible token author;
		* quantity  - amount to issue, example "1000.00 WOOD";
		* memo      - issue comment;
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
		* memo     - transfers comment;
		*/
		ACTION transferf( name from, name to, name author, asset quantity, string memo );
		using transferf_action = action_wrapper<"transferf"_n, &SimpleAssets::transferf>;

		
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
		* Action works only if balance is 0;
		* 
		* owner     - account who woud like to close table with fungible token;
		* author    - account of fungible token author;	
		* symbol    - token symbol, example "WOOD", "ROCK", "GOLD";
		*/		
		ACTION closef( name owner, name author, const symbol& symbol );
		using closef_action = action_wrapper<"closef"_n, &SimpleAssets::closef>;

		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Returns Current token supply 
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
		* owner	                  - token holder account
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

		uint64_t getid();
		uint64_t getFTIndex(name author, symbol symbol);

		void sub_balancef( name owner, name author, asset value );
		void add_balancef( name owner, name author, asset value, name ram_payer );

		
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
			
			EOSLIB_SERIALIZE( sauthor, (author)(data)(stemplate))
		};

		typedef eosio::multi_index< "authors"_n, sauthor > authors;

		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Assets table which stores information about simple assets. 
		* Scope: asset owner
		*/			
		TABLE sasset {
			uint64_t			id;
			name				owner;
			name				author;
			name				category;
			string				idata; // immutable data
			string				mdata; // mutable data
							
			auto primary_key() const {
				return id;
			}
			uint64_t by_author() const {
				return author.value;
			}
			
			EOSLIB_SERIALIZE( sasset, (id)(owner)(author)(category)(idata)(mdata))
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
			name			offeredTo;
			uint64_t		cdate;
			
			auto primary_key() const {
				return assetid;
			}
			uint64_t by_owner() const {
				return owner.value;
			}
			uint64_t by_offeredTo() const {
				return offeredTo.value;
			}

			EOSLIB_SERIALIZE( soffer, (assetid)(owner)(offeredTo)(cdate))
		};

		typedef eosio::multi_index< "offers"_n, soffer,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun<soffer, uint64_t, &soffer::by_owner> >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun<soffer, uint64_t, &soffer::by_offeredTo> >
			> offers;

			
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
			uint64_t		untildate;
			
			auto primary_key() const {
				return assetid;
			}
			uint64_t by_owner() const {
				return owner.value;
			}
			uint64_t by_delegatedto() const {
				return delegatedto.value;
			}

			EOSLIB_SERIALIZE( sdelegate, (assetid)(owner)(delegatedto)(cdate)(untildate))
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
			uint64_t lastid = 100000000000000;
			uint64_t spare1 = 0;
			uint64_t spare2 = 0;			
			uint64_t spare3 = 0;			
			EOSLIB_SERIALIZE( global, (lastid)(spare1)(spare2)(spare3) )

		};

		typedef eosio::singleton< "global"_n, global> conf;
		global _cstate;
		
	  
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Fungible token accounts table which stores information about balances. 
		* Scope: token owner
		*/			
		TABLE account {
			uint64_t	id;
			asset		balance;

			uint64_t primary_key()const { 
				return id;//buildFungibleIndex(author, balance.symbol);// author.value+balance.symbol.code().raw(); 
			}
		};

		typedef eosio::multi_index< "accounts"_n, account > accounts;

				
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
			
			uint64_t primary_key()const { 
				return supply.symbol.code().raw(); 
			}
		};

		typedef eosio::multi_index< "stat"_n, currency_stats > stats;	
  
};

//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================
