/**
 *  SimpleAssets (Digital Assets)
 *  (C) 2019 by CryptoLions [ https://CryptoLions.io ]
 *
 *  A simple standard for digital assets (ie. Non-Fungible Tokens) for EOSIO blockchains
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

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>

#include <string>

using namespace eosio;
using std::string;


CONTRACT SimpleAssets : public contract {
   //=============================================================================================================================
   //=============================================================================================================================
   public:
		using contract::contract;

		
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
		* idata          - stringified json with immutable assets data
		* mdata          - stringified json with mutable assets data, can be changed only by author
		* requireClaim   - true or false. If disabled, upon creation, the asset will be transfered to owner (but 
		*		           but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
		*		           author will remain the owner, but an offer will be created for the account specified in 
		*		           the owner field to claim the asset using the account's RAM.
		*/
		ACTION create( name author, name category, name owner, string idata, string mdata, bool requireClaim);
		using create_action = action_wrapper<"create"_n, &SimpleAssets::create>;

		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Claim the specified asset (assuming it was offered to claimer by the asset owner).
		* 
		* claimer - account claiming the asset
		* assetID - assetID to claim
		*/
		ACTION claim( name claimer, uint64_t assetID);
		using claim_action = action_wrapper<"claim"_n, &SimpleAssets::claim>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* This actions transfers an asset. On transfer owner asset's and scope asset's changes to {{to}}'s.
		* Senders RAM will be charged to transfer asset.
		* Transfer will fail if asset is offered for claim or is delegated.
		* 
		* from    - account who sends the asset;
		* to      - account of receiver;
		* assetID - assetID to transfer;
		* memo    - transfers comment;
		*/		
		ACTION transfer( name from, name to, uint64_t assetID, string memo);
		using transfer_action = action_wrapper<"transfer"_n, &SimpleAssets::transfer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Update assets mutable data (mdata) field. Action is available only for authors.
		* 
		* author  - authors account;
		* owner   - current assets owner;
		* assetID - assetID to update;
		* mdata   - stringified json with mutable assets data. All mdata will be replaced;
		*/		
		ACTION update( name author, name owner, uint64_t assetID, string mdata );
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
		* assetID  - assetID to offer;
		*/		
		ACTION offer( name owner, name newowner, uint64_t assetID);
		using offer_action = action_wrapper<"offer"_n, &SimpleAssets::offer>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Cancel and remove offer. Available for the asset owner.
		* 
		* owner    - current asset owner account;
		* assetID  - assetID to update;
		*/		
		ACTION canceloffer( name owner, uint64_t assetID);
		using canceloffer_action = action_wrapper<"canceloffer"_n, &SimpleAssets::canceloffer>;

		
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Burns asset {{assetID}}. This action is only available for the asset owner. After executing, the 
		* asset will disappear forever, and RAM used for asset will be released.
		* 
		* owner    - current asset owner account;
		* assetID  - assetID to update;
		* memo     - memo for burn action;
		*/		
		ACTION burn( name owner, uint64_t assetID, string memo );
		using burn_action = action_wrapper<"burn"_n, &SimpleAssets::burn>;

	  
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Delegates asset to {{to}}. This action changes the asset owner by calling the transfer action.
		* It also adds a record in the delegates table to record the asset as borrowed.  This blocks
		* the asset from all owner actions (transfers, offers, burning by borrower).
		* 
		* owner    - current asset owner account;
		* to       - borrower account name;
		* assetID  - assetID to delegate;
		*/		
		ACTION delegate( name owner, name to, uint64_t assetID );
		using delegate_action = action_wrapper<"delegate"_n, &SimpleAssets::delegate>;


		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* Undelegates an asset from {{from}} account. Executing action by real owner will return asset immediately,
		* and the entry in the delegates table recording the borrowing will be erased.
		* 
		* owner    - real asset owner account;
		* from     - current account owner (borrower);
		* assetID  - assetID to undelegate;
		*/		
		ACTION undelegate( name owner, name from, uint64_t assetID );
		using undelegate_action = action_wrapper<"undelegate"_n, &SimpleAssets::undelegate>;

		

	//=============================================================================================================================
	//=============================================================================================================================
	private:

		uint64_t getid();

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
			uint64_t		assetID;
			name			owner;
			name			offeredTo;
			uint64_t		cdate;
			
			auto primary_key() const {
				return assetID;
			}
			
			uint64_t by_owner() const {
				return owner.value;
			}

			uint64_t by_offeredTo() const {
				return offeredTo.value;
			}

			EOSLIB_SERIALIZE( soffer, (assetID)(owner)(offeredTo)(cdate))
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
		TABLE sdelagate {
			uint64_t		assetID;
			name			owner;
			name			delegatedto;
			uint64_t		cdate;
			
			auto primary_key() const {
				return assetID;
			}
			
			uint64_t by_owner() const {
				return owner.value;
			}

			uint64_t by_delegatedto() const {
				return delegatedto.value;
			}

			EOSLIB_SERIALIZE( sdelagate, (assetID)(owner)(delegatedto)(cdate))
		};

		typedef eosio::multi_index< "delegates"_n, sdelagate,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun<sdelagate, uint64_t, &sdelagate::by_owner> >,
			eosio::indexed_by< "delegatedto"_n, eosio::const_mem_fun<sdelagate, uint64_t, &sdelagate::by_delegatedto> >
		> delegates;

				
		//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		/*
		* global singelton table, used for assetID building
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
		
	  
};

//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================
