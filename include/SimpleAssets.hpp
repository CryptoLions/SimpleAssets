/*
 * @file
 * @author  (C) 2019 by CryptoLions [ https://CryptoLions.io ]
 * @version 1.1.0
 *
 * @section LICENSE
 *
 * This program is under GNU LESSER GENERAL PUBLIC LICENSE.
 * Version 2.1, February 1999.
 * The licenses for most software are designed to take away your
 * freedom to share and change it.  By contrast, the GNU General Public
 * Licenses are intended to guarantee your freedom to share and change
 * free software--to make sure the software is free for all its users.
 * GNU LESSER GENERAL PUBLIC LICENSE for more details at
 * https://github.com/CryptoLions/SimpleAssets/blob/master/LICENSE
 *
 * @section DESCRIPTION
 * SimpleAssets (Digital Assets)
 *
 * A simple standard for digital assets (ie. Fungible and Non-Fungible Tokens - NFTs) for EOSIO blockchains
 *    WebSite:        https://simpleassets.io
 *    GitHub:         https://github.com/CryptoLions/SimpleAssets
 *    Presentation:   https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4
 *    Event Receiver: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample
 */

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>

using namespace eosio;
using std::string;

CONTRACT SimpleAssets : public contract{
	public:
		using contract::contract;

		/*
		* Update version.
		*
		* This action updates the version string of this SimpleAssets deployment for 3rd party wallets,
                * marketplaces, etc.
		*
		* @param version is version number of SimpleAssetst deployment.
		* @return no return value.
		*/
		ACTION updatever( string version );
		using updatever_action = action_wrapper< "updatever"_n, &SimpleAssets::updatever >;

		/*
		* New Author registration.
		*
		* This action registers a new new author account. It is not mandatory.  Markets *may* choose to use
                * information here to display information about the author, and to follow specifications expressed
                * here for displaying asset fields.
		*
		* @param author     is author's account who will create assets.
		* @param data       is stringified JSON. Recommendations to include: game, company, logo, url, desc.
		* @param stemplate  is stringified JSON with key:state values, where key is key from mdata or idata and
		*		    state indicates recommended way of displaying field:
		* txt		- default
		* url		- show as clickable URL
		* img		- link to img file
		* webgl		- link to webgl file
		* mp3		- link to mp3 file
		* video		- link to video file
		* hide		- do not show
		* imgb 		- image as string in binary format
		* webglb	- webgl binary
		* mp3b 		- mp3 binary
		* videob 	- video binary
		*
		* @param imgpriority is JSON which assosiates an NFT category with the field name from idata or mdata
		* that specifies the main image field for that category of NFTs.
		*
		* @return no return value
		*/
		ACTION regauthor( name author, string data, string stemplate, string imgpriority );
		using regauthor_action = action_wrapper< "regauthor"_n, &SimpleAssets::regauthor >;

		/*
		* Authors info update.
		*
		* This action updates author's information and the asset display recommendations. This action replaces
                * the fields data, stemplate, and imgpriority.
		* To remove author entry, call this action with null strings for data, stemplate, and imgpriority.
		*
		* (See regauthor action for parameter info.)
		*
		* @return no return value.
		*/
		ACTION authorupdate( name author, string data, string stemplate, string imgpriority );
		using authorupdate_action = action_wrapper< "authorupdate"_n, &SimpleAssets::authorupdate >;

		/*
		* Create a new asset.
		*
		* This action creates a new asset.
		*
		* @param author	is the asset's author. This account is allowed to update the asset's mdata.
		* @param category is asset category.
		* @param owner is asset owner.
		* @param idata is stringified JSON or sha256 string with immutable asset data.
		* @param mdata is stringified JSON or sha256 string with mutable asset data. It can be changed only by author.
		* @param requireclaim is true or false. If set to "false", the newly created asset will be transferred to the
		*                 owner (but author's RAM will be used until the asset is transferred again). If set to
                *                 "true", the author will remain to be the owner, but an offer will be created for the
                *                 account specified in the owner field to claim the asset using the owner's RAM.
		* @return no return value.
		*/
		ACTION create( name author, name category, name owner, string idata, string mdata, bool requireclaim );
		using create_action = action_wrapper< "create"_n, &SimpleAssets::create >;

		/*
		* Create a new log entry.
		*
		* This action is doeing nothing, and it can only be called by SimpleAsset contract. It creates an entry
		* in transaction trace, so that that third party explorers can retrieve new asset ID and other
		* information.
		*
		* @param author	is the asset's author. This account is allowed to update the asset's mdata.
		* @param category is asset category.
		* @param owner is asset owner.
		* @param idata is stringified JSON or sha256 string with immutable asset data.
		* @param mdata is stringified JSON or sha256 string with mutable asset data. It can be changed only by author.
		* @param requireclaim is true or false. If set to "false", the newly created asset will be transferred to the
		*                 owner (but author's RAM will be used until the asset is transferred again). If set to
                *                 "true", the author will remain to be the owner, but an offer will be created for the
                *                 account specified in the owner field to claim the asset using the owner's RAM.
		* @return no return value.
		*/
		ACTION createlog( name author, name category, name owner, string idata, string mdata, uint64_t assetid,
                                  bool requireclaim );
		using createlog_action = action_wrapper< "createlog"_n, &SimpleAssets::createlog >;

		/*
		* Claim asset.
		*
		* This action claims the specified assets (assuming they were offered to claimer by the asset owner).
		*
		* @param claimer is account claiming the asset.
		* @param assetids is array of asset id's to claim.
		* @return no return value.
		*/
		ACTION claim( name claimer, std::vector< uint64_t >& assetids );
		using claim_action = action_wrapper< "claim"_n, &SimpleAssets::claim >;

		/*
		* Transfers one or more assets.
		*
		* This action transfers one or more assets by changing scope.
		* Sender's RAM will be charged to transfer asset.
		* Transfer will fail if asset is offered for claim or is delegated.
		*
		* @param from is account who sends the asset.
		* @param to is account of receiver.
		* @param assetids is array of assetid's to transfer.
		* @param memo is transfers comment.
		* @return no return value.
		*/
		ACTION transfer( name from, name to, std::vector< uint64_t >& assetids, string memo );
		using transfer_action = action_wrapper< "transfer"_n, &SimpleAssets::transfer >;

		/*
		* Update assets data.
		*
		* This action updates asset's mutable data (mdata) field. It is only available for the author of an asset.
		*
		* @param author	is author account.
		* @param owner is current asset owner.
		* @param assetid is asset id to update.
		* @param mdata is stringified JSON with mutable assets data. All mdata will be replaced.
		* @return no return value.
		*/
		ACTION update( name author, name owner, uint64_t assetid, string mdata );
		using update_action = action_wrapper< "update"_n, &SimpleAssets::update >;

		/*
		* Offer asset for claim.
		*
		* This is an alternative to the transfer action. Offer can be used by an
		* asset owner to transfer the asset without using the their RAM. After an offer is made, the account
		* specified in {{newowner}} is able to make a claim, and take control of the asset using their RAM.
		* Offer action is not available if an asset is delegated (borrowed).
		*
		* @param owner is the accout of current owner.
		* @param newowner is the future owner of the asset.
		* @param assetids is array of asset id's to offer.
		* @param memo is memo for offer action.
		* @return no return value.
		*/
		ACTION offer( name owner, name newowner, std::vector< uint64_t >& assetids, string memo );
		using offer_action = action_wrapper< "offer"_n, &SimpleAssets::offer >;

		/*
		* Cancel offer.
		*
		* This action cancels an offer. Only the current owner of an asset is allowed to execute it.
		*
		* @param owner		- current asset owner account.
		* @param assetids	- array of asset id's to cancel from offer.
		* @return no return value.
		*/
		ACTION canceloffer( name owner, std::vector<uint64_t>& assetids );
		using canceloffer_action = action_wrapper< "canceloffer"_n, &SimpleAssets::canceloffer >;

		/*
		* Burn asset.
		*
		* This action wil ldestroy the assets specified in {{assetids}}. This action is only available for the asset
                * owner. After executing, the asset will disappear forever, and RAM used for asset will be released. All
                * attached fungible and non-fungible assets will be destroyed too.
		*
		* @param owner is current asset owner account.
		* @param assetids is array of asset id's to burn.
		* @param memo is memo for burn action.
		* @return no return value.
		*/
		ACTION burn( name owner, std::vector< uint64_t >& assetids, string memo );
		using burn_action = action_wrapper< "burn"_n, &SimpleAssets::burn >;

		/*
		* Delegate assets.
		*
		* This action delegates asset to {{to}} account. This action changes the asset owner by calling the transfer
                * action. It also adds a record in the delegates table to record the asset as borrowed.  This blocks
		* the asset from all owner actions (transfers, offers, burning by borrower).
		*
		* @param owner is current asset owner account.
		* @param to is borrower account name.
		* @param assetids is array of asset id's to delegate.
		* @param period	is time in seconds that the asset will be lent for. The lender cannot undelegate until
		*		 the period expires, however the receiver can transfer back at any time.
		* @param memo is memo for delegate action.
		* @return no return value.
		*/
		ACTION delegate( name owner, name to, std::vector< uint64_t >& assetids, uint64_t period, string memo );
		using delegate_action = action_wrapper< "delegate"_n, &SimpleAssets::delegate >;

		/*
		* Undelegate assets.
		*
		* This action undelegates assets from {{from}} account. Executing action by real owner will return asset
                * immediately, and the entry in the delegates table recording the borrowing will be erased.
		*
		* @param owner is the account of real owner of the assets.
		* @param from is current account owner (borrower).
		* @param assetids is array of asset id's to undelegate.
		* @return no return value.
		*/
		ACTION undelegate( name owner, name from, std::vector< uint64_t >& assetids );
		using undelegate_action = action_wrapper< "undelegate"_n, &SimpleAssets::undelegate >;

		/*
		* Attach non-fungible token.
		*
		* This action attaches other NFTs to the specified NFT. Restrictions:
		* 1. Only the Asset Author can do this
		* 2. All assets must have the same author
		* 3. All assets much have the same owner
		*
		* @param owner is owner of NFTs.
		* @param assetidc is id of container NFT.
		* @param assetids is array of asset id's to attach.
		* @return no return value.
		*/
		ACTION attach( name owner, uint64_t assetidc, std::vector< uint64_t >& assetids );
		using attach_action = action_wrapper< "attach"_n, &SimpleAssets::attach >;

		/*
		* Detach non-fungible token.
		*
		* This action detaches NFTs from the specified NFT. Only the owner of the container asset is
                * allowed to execute this action.
		*
		* @param owner is owner of NFTs.
		* @param assetidc is the id of the NFT from which we are detaching.
		* @param assetids is the array of id's of the NFTS to be detached.
		* @return no return value.
		*/
		ACTION detach( name owner, uint64_t assetidc, std::vector< uint64_t >& assetids );
		using detach_t_action = action_wrapper< "detach"_n, &SimpleAssets::detach >;

		/*
		* Extend the delegation.
		*
		* This action extends the period of a delegated asset.
		*
		* @param owner is owner of NFTs.
		* @param assetidc is the id of the NFT for which we are extending the period.
		* @param period is number of seconds added to existing amount of period.
		* @return no return value.
		*/
		ACTION delegatemore( name owner, uint64_t assetidc, uint64_t period );
		using delegatemore_action = action_wrapper< "delegatemore"_n, &SimpleAssets::delegatemore >;

		/*
		* Attach fungible token.
		*
		* This action attaches FTs to the specified NFT. Restrictions:
		* 1. Only the Asset Author can do this
		* 2. All assets must have the same author
		* 3. All assets much have the same owner
		*
		* @param owner is the owner of the asset.
		* @param author	is author of the asset.
		* @param assetidc is id of container NFT.
		* @param quantity is quantity to attach and token name (for example: "10 WOOD", "42.00 GOLD").
		* @return no return value.
		*/
		ACTION attachf( name owner, name author, asset quantity, uint64_t assetidc );
		using attachf_t_action = action_wrapper< "attachf"_n, &SimpleAssets::attachf >;

		/*
		* Detach fungible tokens.
		*
		* This action detaches FTs from the specified NFT.
		*
		* @param owner is the owner of NFTs.
		* @param author	is the author of the assets.
		* @param assetidc is id of the container NFT.
		* @param quantity is quantity to detach and token name (for example: 10 WOOD, 42.00 GOLD).
		* @return no return value.
		*/
		ACTION detachf( name owner, name author, asset quantity, uint64_t assetidc );
		using detachf_t_action = action_wrapper< "detachf"_n, &SimpleAssets::detachf >;

		/*
		* Creates fungible token.
		*
		* This action creates a fungible token with specified maximum supply. Maximum supply and author control
                * cannot be modified after the token is created.
		*
		* @param author is fungible token author;
		* @param maximum_supply is maximum token supply, example "10000000.0000 GOLD", "10000000 SEED",
                * 	"100000000.00 WOOD". The amount must specify the exact precision.
		* @param authorctrl is IMPORTANT! If true(1) allows token author (and not just owner) to burnf and transferf.
                * 	Cannot be changed after creation!
		* @param data is stringified JSON (recommend including keys `img` and `name` for better displaying by markets).
		* @return no return value.
		*/
		ACTION createf( name author, asset maximum_supply, bool authorctrl, string data );
		using createf_action = action_wrapper< "createf"_n, &SimpleAssets::createf >;

		/*
		* Update fungible token.
		*
		* Update the data field of a fungible token.
		*
		* @param author is fungible token author.
		* @param sym is fingible token symbol ("GOLD", "WOOD", etc.).
		* @param data is stringified JSON (recommend including keys `img` and `name` for better displaying by markets).
		* @return no return value.
		*/
		ACTION updatef( name author, symbol sym, string data );
		using updatef_action = action_wrapper< "updatef"_n, &SimpleAssets::updatef >;

		/*
		* Issue fungible token.
		*
		* This action issues a fungible token.
		*
		* @param to is account receiver.
		* @param author is fungible token author.
		* @param quantity is amount to issue, example "1000.00 WOOD".
		* @param memo is issue comment.
		* @return no return value.
		*/
		ACTION issuef( name to, name author, asset quantity, string memo );
		using issuef_action = action_wrapper< "issuef"_n, &SimpleAssets::issuef >;

		/*
		* Transfer fungible token.
		*
		* This action transfers a specified quantity of fungible tokens.
		*
		* @param from is account who sends the token.
		* @param to is account of receiver.
		* @param author is account of fungible token author.
		* @param quantity is amount to transfer, example "1.00 WOOD".
		* @param memo is transfer's comment.
		* @return no return value.
		*/
		ACTION transferf( name from, name to, name author, asset quantity, string memo );
		using transferf_action = action_wrapper< "transferf"_n, &SimpleAssets::transferf >;

		/*
		* Offer fungible tokens.
		*
		* This action offer fungible tokens for another EOS user to claim.
		* This is an alternative to the transfer action. Offer can be used by a
		* FT owner to transfer the FTs without using their RAM. After an offer is made, the account
		* specified in {{newowner}} is able to make a claim, and take control of the asset using their RAM.
		* The FTs will be removed from the owner's balance while the offer is open.
		*
		* @param owner is original owner of the FT.
		* @param newowner is account which will be able to claim the offer.
		* @param author is account of fungible token author.
		* @param quantity is amount to transfer, example "1.00 WOOD".
		* @param memo is offer's comment;
		* @return no return value.
		*/
		ACTION offerf( name owner, name newowner, name author, asset quantity, string memo );
		using offerf_action = action_wrapper< "offerf"_n, &SimpleAssets::offerf >;

		/*
		* Cancel an offer of fungible tokens.
		*
		* This action cancels an offer of FTs.
		*
		* @param owner is original owner of the FT.
		* @param ftofferids is ID of the FT offer.
		* @return no return value.
		*/
		ACTION cancelofferf( name owner, std::vector< uint64_t >& ftofferids );
		using cancelofferf_action = action_wrapper< "cancelofferf"_n, &SimpleAssets::cancelofferf >;

		/*
		* Claim fungible tokens.
		*
		* This action claims FTs which have been offered.
		*
		* @param claimer is account claiming FTs which have been offered.
		* @param ftofferids is array of FT offer id's.
		* @return no return value.
		*/
		ACTION claimf( name claimer, std::vector< uint64_t >& ftofferids );
		using claimf_action = action_wrapper< "claimf"_n, &SimpleAssets::claimf >;

		/*
		* Burn fungible tokens
		*
		* This action burns a fungible token. This action is available for the token owner and author.
                * After executing,accounts balance and supply in stats table for this token will reduce by the
                * specified quantity.
		*
		* @param from is account that burns the token.
		* @param author is account of fungible token author.
		* @param quantity is amount to burn, example "1.00 WOOD".
		* @param memo is memo for burnf action.
		* @return no return value.
		*/
		ACTION burnf( name from, name author, asset quantity, string memo );
		using burnf_action = action_wrapper< "burnf"_n, &SimpleAssets::burnf >;

		/*
		* Open accoutns table.
		*
		* This action opens accounts table for specified fungible token.
		*
		* @param owner is account where create table with fungible token.
		* @param author is account of fungible token author.
		* @param symbol is token symbol, example "WOOD", "ROCK", "GOLD".
		* @param ram_payer is account who will pay for ram used for table creation.
		* @return no return value.
		*/
		ACTION openf( name owner, name author, const symbol& symbol, name ram_payer );
		using openf_action = action_wrapper< "openf"_n, &SimpleAssets::openf >;

		/*
		* Close accounts table.
		*
		* This action closes accounts table for provided fungible token and releases RAM.
		* Action works only if balance is zero.
		*
		* @param owner is account who woud like to close table with fungible token.
		* @param author is account of fungible token author.
		* @param symbol is token symbol, example "WOOD", "ROCK", "GOLD".
		* @return no return value.
		*/
		ACTION closef( name owner, name author, const symbol& symbol );
		using closef_action = action_wrapper< "closef"_n, &SimpleAssets::closef >;

		/*
		* Return current token supply.
		*
		* This function returns the current token supply.
		*
		* @param token_contract_account is contract to check.
		* @param author is fungible tokens author account.
		* @param sym_code is token symbol, example "WOOD", "ROCK", "GOLD".
		* @return asset
		*/
		static asset get_supply( name token_contract_account, name author, symbol_code sym_code );

		/*
		* Returns token balance for account.
		*
		* This function returns token balance for account.
		*
		* @param token_contract_account is contract to check;
		* @param owner is token holder account;
		* @param author is fungible tokens author account;
		* @param sym_code is token symbol, example "WOOD", "ROCK", "GOLD";
		* @return asset
		*/
		static asset get_balance( name token_contract_account, name owner, name author, symbol_code sym_code );


		/*
		* This action create a new Non transferable token (NTT).
		*
		* @param author	is asset's author, who will able to updated asset's mdata.
		* @param category is assets category.
		* @param owner is assets owner.
		* @param idata is stringified json or just sha256 string with immutable assets data
		* @param mdata is stringified json or just sha256 string with mutable assets data, can be changed only by author;
		* @param requireclaim is true or false. If disabled, upon creation, the asset will be transfered to owner (but
		*		  but AUTHOR'S memory will be used).  If enabled,
		*		  author will remain the owner, but an offer will be created for the account specified in
		*		  the owner field to claim the asset using the account's RAM.
		* @return no return value.
		*/
		ACTION createntt ( name author, name category, name owner, string idata, string mdata, bool requireclaim );
		using createntt_action = action_wrapper< "createntt"_n, &SimpleAssets::createntt >;
		
		/*
		* Create a new NTT log.
		*
		* This is empty action. Used by createntt action to log assetid so that third party explorers can
		* easily get new asset ids and other information.
		*
		* @param author	is asset's author, who will able to updated asset's mdata.
		* @param category is assets category.
		* @param owner is assets owner.
		* @param idata is stringified json or just sha256 string with immutable assets data.
		* @param mdata is stringified json or just sha256 string with mutable assets data, can be changed only by author.
		* @param assetid is id of the asset
		* @param requireclaim is true or false. If disabled, upon creation, the asset will be transfered to owner (but
		*		 but AUTHOR'S memory will be used until the asset is transferred again).  If enabled,
		*		 author will remain the owner, but an offer will be created for the account specified in
		*		 the owner field to claim the asset using the account's RAM.
		* @return no return value.
		*/
		ACTION createnttlog( name author, name category, name owner, string idata, string mdata, uint64_t assetid, bool requireclaim );
		using createnttlog_action = action_wrapper< "createnttlog"_n, &SimpleAssets::createnttlog >;

		/*
		* Claim NTT.
		*
		* This action claim the specified NTT asset (assuming it was offered to claimer by the asset owner).
		*
		* @param claimer is account claiming the NTT asset.
		* @param assetids is array of NTT assetid's to claim.
		* @return no return value.
		*/
		ACTION claimntt( name claimer, std::vector< uint64_t >& assetids );
		using claimntt_action = action_wrapper< "claimntt"_n, &SimpleAssets::claimntt >;		
		
		/*
		* Update NTT assets mdata.
		*
		* This action update NTT assets mutable data (mdata) field. Action is available only for authors.
		*
		* @param author	is authors account.
		* @param owner is current assets owner.
		* @param assetid is assetid to update.
		* @param mdata is stringified json with mutable assets data. All mdata will be replaced.
		* @return no return value.
		*/
		ACTION updatentt( name author, name owner, uint64_t assetid, string mdata );
		using updatentt_action = action_wrapper< "updatentt"_n, &SimpleAssets::updatentt >;		
		
		/*
		* Burn NTT asset.
		*
		* This action burn NTT asset {{assetid}}. This action is only available for the asset owner. After executing, the
		* asset will disappear forever, and RAM used for asset will be released.
		*
		* @param owner is current asset owner account.
		* @param assetids is array of assetid's to burn.
		* @param memo is memo for burn action.
		* @return no return value.
		*/
		ACTION burnntt( name owner, std::vector< uint64_t >& assetids, string memo );
		using burnntt_action = action_wrapper< "burnntt"_n, &SimpleAssets::burnntt >;

	private:
		const std::vector<name> waxauthors{ "vgo.wax"_n, "irl.wax"_n };
		void checkwaxauthor( name author );

		/*
		* Get new asset id.
		*
		* This function return new asset id.
		*
		* @param defer is flag for type of transaction true for defered;
		* @return new asset id
		*/
		uint64_t getid( bool defer = false );

		/*
		* Get fungible token index.
		*
		* This function return fungible token index.
		*
		* @param author is author name ;
		* @param symbol is symbol;
		* @return new fungible token index
		*/
		uint64_t getFTIndex( name author, symbol symbol );
		void attachdeatch( name owner, name author, asset quantity, uint64_t assetidc, bool attach );
		void sub_balancef( name owner, name author, asset value );
		void add_balancef( name owner, name author, asset value, name ram_payer );

		template<typename... Args>
		void sendEvent( name author, name rampayer, name seaction, const std::tuple<Args...> &tup );

		/*
		* Authors table. Can be used by asset markets, asset explorers, or wallets for correct asset
		* data presentation.
		* Scope: self
		*/
		TABLE sauthor {
			name			author;
			string			data;
			string			stemplate;
			string			imgpriority;

			auto primary_key() const {
				return author.value;
			}

		};
		typedef eosio::multi_index< "authors"_n, sauthor > authors;

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

		/*
		* NTT table which stores information about Non transferable tokens.
		* Scope: asset owner
		*/
		TABLE snttasset {
			uint64_t                id;
			name                    owner;
			name                    author;
			name                    category;
			string                  idata; // immutable data
			string                  mdata; // mutable data

			auto primary_key() const {
				return id;
			}
			uint64_t by_author() const {
				return author.value;
			}

		};
		typedef eosio::multi_index< "snttassets"_n, snttasset,
			eosio::indexed_by< "author"_n, eosio::const_mem_fun<snttasset, uint64_t, &snttasset::by_author> >
			> snttassets;

		/*
		* Offers table keeps records of open offers of assets (ie. assets waiting to be claimed by their
		* intendend recipients. Scope: self
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
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun< soffer, uint64_t, &soffer::by_owner > >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun< soffer, uint64_t, &soffer::by_offeredto > >
			> offers;

		/*
		* Offers table keeps records of open offers of FT (ie. waiting to be claimed by their
		* intendend recipients. Scope: self
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
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun< sofferf, uint64_t, &sofferf::by_owner > >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun< sofferf, uint64_t, &sofferf::by_offeredto > >
			> offerfs;

		/*
		* NTT Offers table keeps records of open offers of NTT assets (ie. assets waiting to be claimed by their
		* intendend recipients. Scope: self
		*/
		TABLE snttoffer {
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
		typedef eosio::multi_index< "nttoffers"_n, snttoffer,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun< snttoffer, uint64_t, &snttoffer::by_owner > >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun< snttoffer, uint64_t, &snttoffer::by_offeredto > >
			> nttoffers;

		/*
		* Delegates table keeps records about borrowed assets.Scope: self
		*/
		TABLE sdelegate {
			uint64_t		assetid;
			name			owner;
			name			delegatedto;
			uint64_t		cdate;
			uint64_t		period;
			string			memo;

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
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun< sdelegate, uint64_t, &sdelegate::by_owner > >,
			eosio::indexed_by< "delegatedto"_n, eosio::const_mem_fun< sdelegate, uint64_t, &sdelegate::by_delegatedto > >
		> delegates;

		/*
		* global singelton table, used for assetid building. Scope: self
		*/
		TABLE global {
			global() {}
			uint64_t lnftid = 100000000000000;
			uint64_t defid = 1000000;
			uint64_t spare1 = 0;
			uint64_t spare2 = 0;

			EOSLIB_SERIALIZE( global, ( lnftid )( defid )( spare1 )( spare2 ) )
		};

		typedef eosio::singleton< "global"_n, global > conf; /// singleton
		global _cstate; /// global state

		/*
		* Helps external contracts parse actions and tables correctly (Usefull for decentralized exchanges,
		* marketplaces and other contracts that use multiple tokens)
		* Marketplaces, exchanges and other reliant contracts will be able to view this info using the following code.
		*   Configs configs("simpl1assets"_n, "simpl1assets"_n.value);
		*	configs.get("simpl1assets"_n);
		*/
		TABLE tokenconfigs {
			name			standard;
			std::string		version;
		};
		typedef singleton< "tokenconfigs"_n, tokenconfigs > Configs;
};
