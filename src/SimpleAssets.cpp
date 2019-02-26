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


ACTION SimpleAssets::create( name author, name category, name owner, string idata, string mdata, bool requireClaim) {

	require_auth( author );
	check( is_account( owner ), "owner account does not exist");


	uint64_t newID = getid();
	
	name assetOwner = owner;
	
	if (requireClaim){
		assetOwner = author;
		//add info to offers table
		offers offert(_self, _self.value);
		offert.emplace( author, [&]( auto& s ) {     
			s.assetID = newID;
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

ACTION SimpleAssets::claim( name claimer, uint64_t assetID) {
	require_auth( claimer );
	require_recipient( claimer );
	
	check( is_account( claimer ), "claimer account does not exist");
	
	offers offert(_self, _self.value);
	auto itrc = offert.find( assetID );
	
	check(itrc != offert.end(), "nothing to claim");
	check(claimer == itrc->offeredTo, "not for you");
	
	sassets assets_f( _self, itrc->owner.value );
	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "nothing to claim");

	check(itrc->owner.value == itr->owner.value, "owner was changed!?");   
	
	sassets assets_t(_self, claimer.value);

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


ACTION SimpleAssets::transfer( name from, name to, uint64_t assetID, string memo){
	
	check( from != to, "cannot transfer to yourself" );
	
	delegates delegatet(_self, _self.value);
	auto itrd = delegatet.find( assetID );
	bool isDelegeting = false;
	
	if (itrd != delegatet.end()){

		auto dg = *itrd;
		print(assetID, ": ",dg.owner, " - ", to );
		if (itrd->owner == to || itrd->delegatedto == to){
			isDelegeting = true;		
			if (itrd->owner == to)
				delegatet.erase(itrd);
		} else {
			check ( false, "You can not transfer a delegated asset." );	
		}
	}
	
	if (isDelegeting){
		require_auth( itrd->owner );	
	} else {
		require_auth( from );
	}

	check( is_account( to ), "TO account does not exist");

	require_recipient( from );
	require_recipient( to );

	check( memo.size() <= 256, "memo has more than 256 bytes" );	

	sassets assets_f( _self, from.value );
	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "asset not found");

	check(from.value == itr->owner.value, "this is not your asset!");   

	uint64_t	id = itr->id;
	name		author = itr->author;
	name		category = itr->category;
	string		mdata = itr->mdata;
	string		idata = itr->idata;

	offers offert(_self, _self.value);
	auto itrc = offert.find( assetID );
	check ( itrc == offert.end(), "Asset has been offered for a claim and can not be transferred. Cancel the offer first?" );
	
	sassets assets_t(_self, to.value);
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


ACTION SimpleAssets::update( name author, name owner, uint64_t assetID, string mdata ) {
	
	require_auth( author );

	sassets assets_f( _self, owner.value );

	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "asset not found");

	check(itr->author == author, "Only author can update asset.");
	
	assets_f.modify( itr, author, [&]( auto& a ) {
        a.mdata = mdata;
    });
	
}


ACTION SimpleAssets::offer( name owner, name newowner, uint64_t assetID){

	require_auth( owner );
	require_recipient( owner );
	
	check( is_account( newowner ), "newowner account does not exist");

	sassets assets_f( _self, owner.value );
	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "asset not found");

	offers offert(_self, _self.value);
	auto itrc = offert.find( assetID );

	check ( itrc == offert.end(), "Already offered for claim." );
	
	delegates delegatet(_self, _self.value);
	auto itrd = delegatet.find( assetID );
	check ( itrd == delegatet.end(), "You can't offer a delegated asset." );
	
	offert.emplace( owner, [&]( auto& s ) {     
		s.assetID = assetID;
		s.offeredTo = newowner;
		s.owner = owner;
		s.cdate = now();
	});
	
}


ACTION SimpleAssets::canceloffer( name owner, uint64_t assetID){

	require_auth( owner );
	require_recipient( owner );
	
	offers offert(_self, _self.value);
	auto itr = offert.find( assetID );

	check ( itr != offert.end(), "Nothing to cancel - offer not found." );
	
	check (owner.value == itr->owner.value, "You are not the asset's owner");

	offert.erase(itr);
}


ACTION SimpleAssets::burn( name owner, uint64_t assetID, string memo ) {

	require_auth( owner );	

	sassets assets_f( _self, owner.value );
	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "asset not found");

	check(owner.value == itr->owner.value, "this is not your asset!");
	
	
	offers offert(_self, _self.value);
	auto itrc = offert.find( assetID );
	check ( itrc == offert.end(), "Asset has an open offer and can't be burned." );

	delegates delegatet(_self, _self.value);
	auto itrd = delegatet.find( assetID );
	check ( itrd == delegatet.end(), "You can't burn a delegated asset." );
	
	assets_f.erase(itr);
}


ACTION SimpleAssets::delegate( name owner, name to, uint64_t assetID ){

	require_auth( owner );
	require_recipient( owner );
	
	check( is_account( to ), "TO account does not exist");

	sassets assets_f( _self, owner.value );
	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "asset not found");

	
	delegates delegatet(_self, _self.value);
	auto itrd = delegatet.find( assetID );
	check ( itrd == delegatet.end(), "Already delegated." );

	offers offert(_self, _self.value);
	auto itro = offert.find( assetID );
	check ( itro == offert.end(), "Asset has an open offer and can't be delegated." );
	
	
	delegatet.emplace( owner, [&]( auto& s ) {     
		s.assetID = assetID;
		s.owner = owner;
		s.delegatedto = to;
		s.cdate = now();
	});
	
	SEND_INLINE_ACTION( *this, transfer, { {owner, "active"_n} },  { owner, to, assetID, "delegate assetID: "+std::to_string(assetID) }   );
}


ACTION SimpleAssets::undelegate( name owner, name from, uint64_t assetID ){

	require_auth( owner );
	require_recipient( owner );
	
	check( is_account( from ), "to account does not exist");
	

	sassets assets_f( _self, from.value );
	auto itr = assets_f.find( assetID );
	check(itr != assets_f.end(), "asset not found");

	delegates delegatet(_self, _self.value);
	auto itrc = delegatet.find( assetID );
	check ( itrc != delegatet.end(), "Not delegated" );
	
	check(owner == itrc->owner, "you are not the owner");
	check(from == itrc->delegatedto, "Error. FROM does not match DELEGATEDTO!");   
	
	check(itr->owner == itrc->delegatedto, "Error. FROM does not match DELEGATEDTO!");   

	SEND_INLINE_ACTION( *this, transfer, { {owner, "active"_n} },  { from, owner, assetID, "undelegate assetID: "+std::to_string(assetID) }   );
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
/*
* getid private action
* Increment, save and return id for a new asset.
*/
uint64_t SimpleAssets::getid(){

        conf config(_self, _self.value);
        _cstate = config.exists() ? config.get() : global{};

        _cstate.lastid++;
        config.set(_cstate, _self);
        return _cstate.lastid;

}


EOSIO_DISPATCH( SimpleAssets, (create)(offer)(canceloffer)(claim)(transfer)(burn)(regauthor)(authorupdate)(update)(delegate)(undelegate) )

//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================
