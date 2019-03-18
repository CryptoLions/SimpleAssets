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

#include <SimpleAssets.hpp>
#include <string>

ACTION SimpleAssets::regauthor( name author, string data, string stemplate) {

	require_auth( author );
	require_recipient( author );

	check( data.size() > 3, "Data field is too short. Please tell us about yourselves." );	
	
	authors author_(_self, _self.value);
	auto itr = author_.find( author.value );

	if (itr == author_.end()){
		author_.emplace( author, [&]( auto& s ) {     
			s.author = author;
			s.data = data;	
			s.stemplate = stemplate;
		});
		
		return;
	}
	
	check (false, "Registration Error. You're probably already registered. Try the authupdate action.");
}


ACTION SimpleAssets::authorupdate( name author, string data, string stemplate) {
	require_auth( author );
	require_recipient( author );

	authors author_(_self, _self.value);
	auto itr = author_.find( author.value );

	check ( itr != author_.end(), "author not registered" );
	
	if (data == "" && stemplate == "") {
		itr = author_.erase(itr);
	} else {
		author_.modify( itr, author, [&]( auto& s ) {
			s.data = data;	
			s.stemplate = stemplate;		
		});
	}
}

// Non-Fungible Token Logic

ACTION SimpleAssets::create( name author, name category, name owner, string idata, string mdata, bool requireclaim) {

	require_auth( author );
	check( is_account( owner ), "owner account does not exist");

	uint64_t newID = getid();
	
	name assetOwner = owner;
	
	check (!(author.value == owner.value && requireclaim == 1), "Can't requireclaim if author == owner.");
	
	if (requireclaim){
		assetOwner = author;
		//add info to offers table
		offers offert(_self, _self.value);
		offert.emplace( author, [&]( auto& s ) {     
			s.assetid = newID;
			s.offeredTo = owner;
			s.owner = author;
			s.cdate = now();
		});
	}
	
	sassets assets(_self, assetOwner.value);
	assets.emplace( author, [&]( auto& s ) {     
		s.id = newID;
		s.owner = assetOwner;
		s.author = author;
		s.category = category;
		s.mdata = mdata; // mutable data
		s.idata = idata; // immutable data
	});
}


ACTION SimpleAssets::claim( name claimer, std::vector<uint64_t>& assetids) {
	require_auth( claimer );
	require_recipient( claimer );
	
	check( is_account( claimer ), "claimer account does not exist");

	offers offert(_self, _self.value);
	sassets assets_t(_self, claimer.value);
	
	for( size_t i = 0; i < assetids.size(); ++i ) {
		uint64_t assetid = assetids[i];
	
		auto itrc = offert.find( assetid );

		check(itrc != offert.end(), "Cannot find at least one of the assets you're attempting to claim.");
		check(claimer == itrc->offeredTo, "At least one of the assets has not been offerred to you.");

		sassets assets_f( _self, itrc->owner.value );
		auto itr = assets_f.find( assetid );
		check(itr != assets_f.end(), "Cannot find at least one of the assets you're attempting to claim.");

		check(itrc->owner.value == itr->owner.value, "Owner was changed for at least one of the items!?");   

		uint64_t 			id = itr->id;
		name				author = itr->author;
		name				category = itr->category;
		string				mdata = itr->mdata;
		string				idata = itr->idata;

		assets_t.emplace( claimer, [&]( auto& s ) {     
			s.id = id;
			s.owner = claimer;
			s.author = author;
			s.category = category;		
			s.mdata = mdata; 		// mutable data
			s.idata = idata; 		// immutable data
		});

		assets_f.erase(itr);
		offert.erase(itrc);
	
	}
}


ACTION SimpleAssets::transfer( name from, name to, std::vector<uint64_t>& assetids, string memo){
	
	check( from != to, "cannot transfer to yourself" );
	check( is_account( to ), "TO account does not exist");
	check( memo.size() <= 256, "memo has more than 256 bytes" );	
		
	require_recipient( from );
	require_recipient( to );
		
	sassets assets_f( _self, from.value );
	sassets assets_t(_self, to.value);
	
	delegates delegatet(_self, _self.value);
	offers offert(_self, _self.value);

	bool isDelegeting = false;
	uint64_t assetid;
	
	for( size_t i = 0; i < assetids.size(); i++ ) {
		assetid = assetids[i];
		auto itrd = delegatet.find( assetid );

		isDelegeting = false;
		if (itrd != delegatet.end()){
			auto dg = *itrd;
			if (itrd->owner == to || itrd->delegatedto == to){
				isDelegeting = true;		
				if (itrd->owner == to)
					delegatet.erase(itrd);
			} else {
				check ( false, "At least one of the assets cannot be transferred because it is delegated" );	
			}
		}
		
		if (isDelegeting){
			require_auth( has_auth( itrd->owner  ) ? itrd->owner  : from);
			
		} else {
			require_auth( from );
		}
		
		auto itr = assets_f.find( assetid );
		check(itr != assets_f.end(), "At least one of the assets cannot be found (check ids?)");

		check(from.value == itr->owner.value, "At least one of the assets is not yours to transfer.");   

		uint64_t	id = itr->id;
		name		author = itr->author;
		name		category = itr->category;
		string		mdata = itr->mdata;
		string		idata = itr->idata;

		auto itrc = offert.find( assetid );
		check ( itrc == offert.end(), "At least one of the assets has been offered for a claim and cannot be transferred. Cancel offer?" );
		
		assets_f.erase(itr);
		
		auto rampayer = has_auth( to ) ? to : from;
		
		assets_t.emplace( rampayer, [&]( auto& s ) {     
			s.id = id;
			s.owner = to;
			s.author = author;
			s.category = category;		
			s.mdata = mdata; 		// mutable data
			s.idata = idata; 		// immutable data
		});
	}
}


ACTION SimpleAssets::update( name author, name owner, uint64_t assetid, string mdata ) {
	
	require_auth( author );

	sassets assets_f( _self, owner.value );

	auto itr = assets_f.find( assetid );
	check(itr != assets_f.end(), "asset not found");

	check(itr->author == author, "Only author can update asset.");
	
	assets_f.modify( itr, author, [&]( auto& a ) {
        a.mdata = mdata;
    });
	
}


ACTION SimpleAssets::offer( name owner, name newowner, std::vector<uint64_t>& assetids){

	require_auth( owner );
	require_recipient( owner );
	
	check( is_account( newowner ), "newowner account does not exist");
	
	sassets assets_f( _self, owner.value );
	offers offert(_self, _self.value);
	delegates delegatet(_self, _self.value);	
		
	for( size_t i = 0; i < assetids.size(); ++i ) {
		uint64_t assetid = assetids[i];

		auto itr = assets_f.find( assetid );
		check(itr != assets_f.end(), "At least one of the assets was not found.");

		auto itrc = offert.find( assetid );

		check ( itrc == offert.end(), "At least one of the assets is already offered for claim." );

		auto itrd = delegatet.find( assetid );
		check ( itrd == delegatet.end(), "At least one of the assets is delegated and cannot be offered." );
		
		offert.emplace( owner, [&]( auto& s ) {     
			s.assetid = assetid;
			s.offeredTo = newowner;
			s.owner = owner;
			s.cdate = now();
		});
	}
	
}


ACTION SimpleAssets::canceloffer( name owner, std::vector<uint64_t>& assetids){

	require_auth( owner );
	require_recipient( owner );
	
	offers offert(_self, _self.value);

	for( size_t i = 0; i < assetids.size(); ++i ) {

		uint64_t assetid = assetids[i];
		auto itr = offert.find( assetid );

		check ( itr != offert.end(), "The offer for at least one of the assets was not found." );
		check (owner.value == itr->owner.value, "You're not the owner of at least one of the assets whose offers you're attempting to cancel.");

		offert.erase(itr);
	}
}


ACTION SimpleAssets::burn( name owner, std::vector<uint64_t>& assetids, string memo ) {

	require_auth( owner );	

	sassets assets_f( _self, owner.value );
	offers offert(_self, _self.value);
	delegates delegatet(_self, _self.value);
	
	for( size_t i = 0; i < assetids.size(); ++i ) {
		uint64_t assetid = assetids[i];
		
		auto itr = assets_f.find( assetid );
		check(itr != assets_f.end(), "At least one of the assets was not found.");

		check(owner.value == itr->owner.value, "At least one of the assets you're attempting to burn is not yours.");

		auto itrc = offert.find( assetid );
		check ( itrc == offert.end(), "At least one of the assets has an open offer and cannot be burned." );

		auto itrd = delegatet.find( assetid );
		check ( itrd == delegatet.end(), "At least one of assets is delegated and cannot be burned." );
		
		assets_f.erase(itr);
	}
}


ACTION SimpleAssets::delegate( name owner, name to, std::vector<uint64_t>& assetids, uint64_t untildate ){

	require_auth( owner );
	require_recipient( owner );
	
	check( is_account( to ), "TO account does not exist");

	if (untildate != 0)
		check(untildate > now(), "untildate should be timestamp in the future or 0");

	sassets assets_f( _self, owner.value );
	delegates delegatet(_self, _self.value);
	offers offert(_self, _self.value);

	string assetidsmemo = "";
	
	for( size_t i = 0; i < assetids.size(); ++i ) {
		uint64_t assetid = assetids[i];
		
		auto itr = assets_f.find( assetid );
		check(itr != assets_f.end(), "At least one of the assets cannot be found.");
		

		auto itrd = delegatet.find( assetid );
		check ( itrd == delegatet.end(), "At least one of the assets is already delegated." );

		auto itro = offert.find( assetid );
		check ( itro == offert.end(), "At least one of the assets has an open offer and cannot be delegated." );
		
		delegatet.emplace( owner, [&]( auto& s ) {     
			s.assetid = assetid;
			s.owner = owner;
			s.delegatedto = to;
			s.cdate = now();
			s.untildate = untildate;
		});
		
		if (i != 0) assetidsmemo += ", ";
		assetidsmemo += std::to_string(assetid);
	}
	
	SEND_INLINE_ACTION( *this, transfer, { {owner, "active"_n} },  { owner, to, assetids, "delegate assetids: "+ assetidsmemo}   );
}


ACTION SimpleAssets::undelegate( name owner, name from, std::vector<uint64_t>& assetids ){

	require_auth( owner );
	require_recipient( owner );
	
	check( is_account( from ), "to account does not exist");

	sassets assets_f( _self, from.value );
	delegates delegatet(_self, _self.value);

	string assetidsmemo = "";	
	for( size_t i = 0; i < assetids.size(); ++i ) {
		uint64_t assetid = assetids[i];
		
		auto itr = assets_f.find( assetid );
		check(itr != assets_f.end(), "At least one of the assets cannot be found.");

		auto itrc = delegatet.find( assetid );
		check ( itrc != delegatet.end(), "At least one of the assets is not delegated." );

		check(owner == itrc->owner, "You are not the owner of at least one of these assets.");
		check(from == itrc->delegatedto, "FROM does not match DELEGATEDTO for at least one of the assets.");   
		check(itr->owner == itrc->delegatedto, "FROM does not match DELEGATEDTO for at least one of the assets.");   		
		check( itrc->untildate < now(), "Too early to undelegate at least one of these assets. UNTILDATE not yet reached.");   		
			
		if (i != 0) assetidsmemo += ", ";
		assetidsmemo += std::to_string(assetid);
	}
	
	SEND_INLINE_ACTION( *this, transfer, { {owner, "active"_n} },  { from, owner, assetids, "undelegate assetid: "+assetidsmemo }   );
}

//------------------------
// Fungible Token Logic

ACTION SimpleAssets::createf( name author, asset maximum_supply, bool authorctrl){

	require_auth( author );

    auto sym = maximum_supply.symbol;

    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( maximum_supply.is_valid(), "invalid supply");
    eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, author.value ); 

    auto existing = statstable.find( sym.code().raw() );
    eosio_assert( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( author, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = author;
	   s.id			   = getid();
	   s.authorctrl    = authorctrl;
    });
}


ACTION SimpleAssets::issuef( name to, name author, asset quantity, string memo ){
	
	auto sym = quantity.symbol;
	eosio_assert( sym.is_valid(), "invalid symbol name" );
	eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

	stats statstable( _self, author.value );
	auto existing = statstable.find( sym.code().raw() );
	eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
	const auto& st = *existing;

	require_auth( st.issuer );
	eosio_assert( quantity.is_valid(), "invalid quantity" );
	eosio_assert( quantity.amount > 0, "must issue positive quantity" );

	eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
	eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

	statstable.modify( st, same_payer, [&]( auto& s ) {
		s.supply += quantity;
	});

	add_balancef( st.issuer, author, quantity, st.issuer );

	if( to != st.issuer ) {
		SEND_INLINE_ACTION( *this, transferf, { {st.issuer, "active"_n} },
			{ st.issuer, to, author, quantity, memo }
		);
	}	
}


ACTION SimpleAssets::transferf( name from, name to, name author, asset quantity, string memo ){
	
	eosio_assert( from != to, "cannot transfer to self" );

    eosio_assert( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable( _self, author.value );
    const auto& st = statstable.get( sym.raw() );

    require_recipient( from );
    require_recipient( to );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

	auto payer = has_auth( to ) ? to : from;
	auto checkAuth = from;
	
	if (st.authorctrl &&  has_auth( st.issuer )){
			checkAuth = st.issuer;
			payer = st.issuer;
	}
	
	require_auth( checkAuth );	

    sub_balancef( from, author, quantity );
    add_balancef( to, author, quantity, payer );
}


ACTION SimpleAssets::burnf( name from, name author, asset quantity, string memo ){

	auto sym = quantity.symbol;
	eosio_assert( sym.is_valid(), "invalid symbol name" );
	eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

	stats statstable( _self, author.value );
	auto existing = statstable.find( sym.code().raw() );
	eosio_assert( existing != statstable.end(), "token with symbol does not exist" );
	const auto& st = *existing;

	require_auth( st.authorctrl && has_auth( st.issuer ) ? st.issuer  : from);
		
	eosio_assert( quantity.is_valid(), "invalid quantity" );
	eosio_assert( quantity.amount > 0, "must retire positive quantity" );

	eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

	statstable.modify( st, same_payer, [&]( auto& s ) {
		s.supply -= quantity;
	});

	sub_balancef( from, author, quantity );
}


ACTION SimpleAssets::openf( name owner, name author, const symbol& symbol, name ram_payer ){
	
	require_auth( ram_payer );

	auto sym_code_raw = symbol.code().raw();

	stats statstable( _self, author.value );
	const auto& st = statstable.get( sym_code_raw, "symbol does not exist" );
	eosio_assert( st.supply.symbol == symbol, "symbol precision mismatch" );

	accounts acnts( _self, owner.value );

	uint64_t ftid = st.id; 

	auto it = acnts.find( ftid );
	if( it == acnts.end() ) {
		acnts.emplace( ram_payer, [&]( auto& a ){
			a.id = st.id;
			a.balance = asset{0, symbol};
		});
	}
}


ACTION SimpleAssets::closef( name owner, name author, const symbol& symbol ){
	require_auth( owner );
	accounts acnts( _self, owner.value );
	
	uint64_t ftid = getFTIndex(author, symbol); 
	
	auto it = acnts.find( ftid );
	eosio_assert( it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect." );
	eosio_assert( it->balance.amount == 0, "Cannot close because the balance is not zero." );
	acnts.erase( it );
}



//-------------------------------------------------------------------------------------
//------------- PRIVATE ---------------------------------------------------------------

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
/*
* getid private action
* Increment, save and return id for a new asset or new fungible token.
*/
uint64_t SimpleAssets::getid(){

	conf config(_self, _self.value);
	_cstate = config.exists() ? config.get() : global{};

	_cstate.lastid++;
	config.set(_cstate, _self);
	
	return _cstate.lastid;
}

uint64_t SimpleAssets::getFTIndex(name author, symbol symbol){

	stats statstable( _self, author.value );

	auto existing = statstable.find( symbol.code().raw() );
	eosio_assert( existing != statstable.end(), "token with symbol does not exist." );
	const auto& st = *existing;

	uint64_t res =  st.id;

	return res;
}


void SimpleAssets::sub_balancef( name owner, name author, asset value ) {

	accounts from_acnts( _self, owner.value );
	uint64_t ftid = getFTIndex(author, value.symbol);
	
	const auto& from = from_acnts.get( ftid, "no balance object found" );
	eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

	check( value.symbol.code().raw() == from.balance.symbol.code().raw(), "Wrong symbol");
	auto payer = has_auth( author ) ? author : owner;
	
	from_acnts.modify( from, payer, [&]( auto& a ) {
		a.balance -= value;
	});
}


void SimpleAssets::add_balancef( name owner, name author, asset value, name ram_payer ) {
	
	accounts to_acnts( _self, owner.value );
	
	uint64_t ftid = getFTIndex(author, value.symbol);
	auto to = to_acnts.find( ftid );
	
	if( to == to_acnts.end() ) {
		to_acnts.emplace( ram_payer, [&]( auto& a ){
			a.id = ftid;
			a.balance = value;
		});
	} else {
		to_acnts.modify( to, same_payer, [&]( auto& a ) {
			a.balance += value;
		});
	}
}


//------------------------------------------------------------------------------------------------------------   

EOSIO_DISPATCH( SimpleAssets, 	(create)(transfer)(burn)(update)
								(offer)(canceloffer)(claim)
								(regauthor)(authorupdate)
								(delegate)(undelegate)
								(createf)(issuef)(transferf)(burnf)
								(openf)(closef) )

//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================
