#include <SimpleAssets.hpp>

ACTION SimpleAssets::updatever( string version ) {

	require_auth( get_self() );
	Configs configs( _self, _self.value );
	configs.set( tokenconfigs{ "simpleassets"_n, version }, _self );
}

ACTION SimpleAssets::changeauthor( name author, name newauthor, name owner, vector<uint64_t>& assetids, string memo ) {

	require_auth( author );
	require_recipient( author );
	check_empty_vector( assetids );

	sassets assets_f( _self, owner.value );

	map< name, map< uint64_t, name > > uniqauthor;

	for ( auto i = 0; i < assetids.size(); ++i ) {

		const auto itr = assets_f.require_find( assetids[i], string("asset id: " + to_string(assetids[i]) +" was not found").c_str() );

		check( offert.find( assetids[i] )    == offert.end(),    "At least one of the assets is already offered for claim. Asset id:" + to_string( assetids[i] ) );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "At least one of the assets is delegated and cannot be offered. Asset id: " + to_string( assetids[i] ) );
		check( itr->author == author, "Only author can update asset. Asset id: " + to_string( assetids[i] ) );
		check( !( itr->container.size()  != 0 ), "Asset has items in non-fungible container. Asset id: " + to_string( assetids[i] ) );
		check( !( itr->containerf.size() != 0 ), "Asset has items in funible container. Asset id: " + to_string( assetids[i] ) );

		auto author_index = arampayert.template get_index< "author"_n >();

		for (auto itr_ram = author_index.find(newauthor.value); itr_ram != author_index.end(); itr_ram++) {

			check(!(itr_ram->author == newauthor && itr_ram->category == itr->category && itr_ram->usearam),
				"Author: " + newauthor.to_string() + " for category: " + itr->category.to_string() 
				+ " has usearam = true. Set usearam = false before changing author. Asset id: " + to_string(assetids[i]));
		}


		assets_f.modify( itr, author, [&]( auto& a ) {
			a.author = newauthor;
		});
		uniqauthor[itr->author][assetids[i]] = itr->owner;
	}

	// Send Event as deferred
	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = move( uniqauthorIt->first );
		sendEvent( _self, author, "saechauthor"_n, make_tuple( author, newauthor, owner, uniqauthor[keyauthor], memo ) );
	}
}

ACTION SimpleAssets::saechauthor( name author, name newauthor, name owner, map< uint64_t, name >& assetids, string memo ) {
	require_recipient( author );
}

ACTION SimpleAssets::authorreg( name author, string dappinfo, string fieldtypes, string priorityimg ) {

	require_auth( author );
	require_recipient( author );

	check( dappinfo.size() > 3, "Data field is too short. Please tell us about yourselves." );

	if ( authort.find( author.value ) == authort.end() ) {
		authort.emplace( author, [&]( auto& s ) {
			s.author      = author;
			s.dappinfo    = dappinfo;
			s.fieldtypes  = fieldtypes;
			s.priorityimg = priorityimg;
		});
	}
	else {
		check( false, "Registration Error. You're probably already registered. Try the authupdate action." );
	}
}

ACTION SimpleAssets::authorupdate( name author, string dappinfo, string fieldtypes, string priorityimg ) {

	require_auth( author );
	require_recipient( author );

	auto itr = authort.require_find( author.value, string("author " + author.to_string() + " not registered").c_str() );

	if ( dappinfo.empty() && fieldtypes.empty() ) {
		itr = authort.erase( itr );
	}
	else {
		authort.modify( itr, author, [&]( auto& s ) {
			s.dappinfo    = dappinfo;
			s.fieldtypes  = fieldtypes;
			s.priorityimg = priorityimg;
		});
	}
}

ACTION SimpleAssets::create( name author, name category, name owner, string idata, string mdata, bool requireclaim ) {

	require_auth( author );
	check( is_account( owner ), "owner account does not exist" );
	require_recipient( owner );
	const auto newID = getid( asset_id );
	name assetOwner = owner;
	check( !( author.value == owner.value && requireclaim == 1 ), "Can't requireclaim if author == owner." );

	if ( requireclaim ) {
		assetOwner = author;
		//add info to offers table
		offert.emplace( author, [&]( auto& s ) {
			s.assetid   = newID;
			s.offeredto = owner;
			s.owner     = author;
			s.cdate     = current_time_point().sec_since_epoch();
		});
	}

	sassets assets( _self, assetOwner.value );
	assets.emplace( author, [&]( auto& s ) {
		s.id       = newID;
		s.owner    = assetOwner;
		s.author   = author;
		s.category = category;
		s.mdata    = mdata; // mutable data
		s.idata    = idata; // immutable data
	});

	//Events
	sendEvent( _self, author, "saecreate"_n, make_tuple( author, category, owner, idata, mdata, newID, requireclaim ) );
	SEND_INLINE_ACTION( *this, createlog, { {_self, "active"_n} }, { author, category, owner, idata, mdata, newID, requireclaim } );
}

ACTION SimpleAssets::saecreate( name author, name category, name owner, string idata, string mdata, uint64_t assetid, bool requireclaim ) {
	require_recipient( author );
}

ACTION SimpleAssets::createlog( name author, name category, name owner, string idata, string mdata, uint64_t assetid, bool requireclaim ) {

	require_auth( get_self() );
}

ACTION SimpleAssets::claim( name claimer, vector<uint64_t>& assetids ) {

	require_auth( claimer );
	require_recipient( claimer );

	check_empty_vector( assetids );
	
	sassets assets_t( _self, claimer.value );

	const auto empty_scope = assets_t.begin() == assets_t.end();

	if (empty_scope) {
		assets_t.emplace(claimer, [&](auto& s) {
			s.id = IMPOSSIBLE_ID;
		});
	}

	map< name, map< uint64_t, name > > uniqauthor;
	for ( auto i = 0; i < assetids.size(); ++i ) {

		auto itrc = offert.require_find( assetids[i], string("Cannot find offer for asset id:  " + to_string(assetids[i]) + " that you're attempting to claim.").c_str() );

		check( claimer == itrc->offeredto, "Asset id: " + to_string(assetids[i]) +  " has not been offerred to you. It offered to " + itrc->offeredto.to_string() );

		sassets assets_f( _self, itrc->owner.value );

		auto itr = assets_f.require_find(assetids[i], string("Cannot find asset id: " + to_string(assetids[i]) + " that you're attempting to claim at scope: " + itrc->owner.to_string()).c_str());

		check( itrc->owner.value == itr->owner.value, "Owner was changed for asset id:" + to_string(assetids[i]) + " .Owner at offers:" + itrc->owner.to_string() + " . Owner at assets: " + itr->owner.to_string() );

		const auto author_payer = get_payer( itr->author, itr->category, itr->id );
		const auto rampayer = author_payer == ""_n ? claimer : author_payer;

		assets_t.emplace( rampayer, [&]( auto& s ) {
			s.id         = itr->id;
			s.owner      = claimer;
			s.author     = itr->author;
			s.category   = itr->category;
			s.mdata      = itr->mdata; 		// mutable data
			s.idata      = itr->idata; 		// immutable data
			s.container  = itr->container;
			s.containerf = itr->containerf;
		});

		//Events
		uniqauthor[itr->author][assetids[i]] = itrc->owner;

		assets_f.erase( itr );
		offert.erase( itrc );
	}

	if (empty_scope) {
		assets_t.erase(assets_t.find(IMPOSSIBLE_ID));
	}

	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = move( uniqauthorIt->first );
		sendEvent( _self, claimer, "saeclaim"_n, make_tuple( keyauthor, claimer, uniqauthor[keyauthor] ) );
	}
}

ACTION SimpleAssets::saeclaim(name author, name claimer, map< uint64_t, name >& assetids) {
	require_recipient(author);
}

void SimpleAssets::check_empty_vector( vector<uint64_t>& vector_ids, string vector_name ) {

	check( !(vector_ids.size() == 0), "Please add values to parameter: " + move(vector_name) );
}

void SimpleAssets::check_memo_size( const string & memo ) {

	check( memo.size() <= MAX_MEMO_SIZE, "Size of memo cannot be bigger then " + to_string( MAX_MEMO_SIZE ) + " .You entered memo size = " + to_string( memo.size() ) );
}

ACTION SimpleAssets::transfer( name from, name to, vector<uint64_t>& assetids, string memo ) {

	check( from != to, "cannot transfer to yourself" );
	check( is_account( to ), "TO account does not exist" );
	check_memo_size( memo );
	check_empty_vector( assetids );
	
	require_recipient( from );
	require_recipient( to );

	sassets assets_f( _self, from.value );
	sassets assets_t( _self, to.value );

	const auto empty_scope = assets_t.begin() == assets_t.end();

	auto authorized_account = has_auth(to) ? to : from;

	if ( empty_scope ) {
		assets_t.emplace(authorized_account, [&](auto& s) {
			s.id = IMPOSSIBLE_ID;
		});
	}

	bool isDelegeting = false;

	map< name, vector<uint64_t> > uniqauthor;

	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itrd = delegatet.find( assetids[i] );
		isDelegeting = false;
		if ( itrd != delegatet.end() ) {
			if ( itrd->owner == to || itrd->delegatedto == to ) {
				isDelegeting = true;
				if ( itrd->owner == to ) {
					delegatet.erase( itrd );
				}
			}
			else {
				check( false, "Asset id: " + to_string(assetids[i]) + " cannot be transferred because it is delegated to " + itrd->delegatedto.to_string() );
			}
		}

		if ( isDelegeting ) {
			require_auth( has_auth( itrd->owner ) ? itrd->owner : from );
		}
		else {
			require_auth( from );
		}

		const auto itr = assets_f.require_find( assetids[i], string("Asset id: " + to_string( assetids[i] ) + " cannot be found (check ids?)").c_str() );

		check( from.value == itr->owner.value, "Asset id: " + to_string(assetids[i]) + " is not yours to transfer. Owner: " + itr->owner.to_string() );
		check( offert.find( assetids[i] ) == offert.end(), "Asset id: " + to_string(assetids[i]) + " offered for a claim and cannot be transferred. Cancel offer?" );

		const auto author_payer = get_payer( itr->author, itr->category, itr->id );
		const auto rampayer = author_payer == ""_n ? authorized_account : author_payer;

		assets_t.emplace( rampayer, [&]( auto& s ) {
			s.id         = itr->id;
			s.owner      = to;
			s.author     = itr->author;
			s.category   = itr->category;
			s.idata      = itr->idata; 		// immutable data
			s.mdata      = itr->mdata; 		// mutable data
			s.container  = itr->container;
			s.containerf = itr->containerf;
		});

		//Events
		uniqauthor[itr->author].push_back( assetids[i] );
		assets_f.erase( itr );
	}

	if ( empty_scope ) {
		assets_t.erase( assets_t.find( IMPOSSIBLE_ID ) );
	}

	//Send Event as deferred
	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = move( uniqauthorIt->first );
		sendEvent( _self, authorized_account, "saetransfer"_n, make_tuple( keyauthor, from, to, uniqauthor[keyauthor], memo ) );
	}
}

name SimpleAssets::get_payer( name author, name category, uint64_t id )
{
	name result = ""_n;
	auto author_index = arampayert.template get_index< "author"_n >();

	for ( auto itr_ram = author_index.find( author.value ); itr_ram != author_index.end(); itr_ram++ ) {
		if (itr_ram->author == author && itr_ram->category == category
			&& itr_ram->usearam && id > itr_ram->from_id) {
			result = author;
			break;
		}
	}

	return result;
}

ACTION SimpleAssets::saetransfer(name author, name from, name to, vector<uint64_t>& assetids, string memo) {
	require_recipient(author);
}

ACTION SimpleAssets::update( name author, name owner, uint64_t assetid, string mdata ) {

	require_auth( author );
	sassets assets_f( _self, owner.value );
	const auto itr = assets_f.require_find( assetid, string("asset id: " + to_string(assetid) + " not found").c_str() );

	check( itr->author == author, "Only for author allowed to update asset. Asset id: " + to_string(assetid) + " has author: " + itr->author.to_string() + " ,you entered author: " + author.to_string() );

	assets_f.modify( itr, author, [&]( auto& a ) {
		a.mdata = mdata;
	});
}

ACTION SimpleAssets::offer( name owner, name newowner, vector<uint64_t>& assetids, string memo ) {

	check( owner != newowner, "cannot offer to yourself" );
	check_empty_vector( assetids );

	require_auth( owner );
	require_recipient( owner );
	require_recipient( newowner );
	check( is_account( newowner ), "newowner account: " + newowner.to_string() + " does not exist" );

	sassets assets_f( _self, owner.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		const auto itr = assets_f.find ( assetids[i] );
		check( itr != assets_f.end(),  "Asset id: " + to_string( assetids[i] ) + " was not found." );
		check( offert.find   ( assetids[i] ) == offert.end(),    "Asset id: " + to_string(assetids[i]) + " is already offered for claim." );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "Asset id: " + to_string(assetids[i]) + " is delegated and cannot be offered." );

		offert.emplace( owner, [&]( auto& s ) {
			s.assetid   = assetids[i];
			s.offeredto = newowner;
			s.owner     = owner;
			s.cdate     = current_time_point().sec_since_epoch();
		});
	}
}

ACTION SimpleAssets::canceloffer( name owner, vector<uint64_t>& assetids ) {

	check_empty_vector( assetids );
	require_auth( owner );
	require_recipient( owner );

	for ( auto i = 0; i < assetids.size(); ++i ) {

		auto itr = offert.require_find( assetids[i], string("The offer for asset id: " + to_string(assetids[i]) + " was not found.").c_str() );

		check( owner.value == itr->owner.value, "You're not the owner of asset id: " + to_string(assetids[i]) + "  whose offers you're attempting to cancel. Owner is " + itr->owner.to_string());
		offert.erase( itr );
	}
}

ACTION SimpleAssets::burn( name owner, vector<uint64_t>& assetids, string memo ) {
	
	check_empty_vector( assetids );
	require_auth( owner );
	sassets assets_f( _self, owner.value );
	map< name, vector<uint64_t> > uniqauthor;

	for ( auto i = 0; i < assetids.size(); ++i ) {

		auto itr = assets_f.require_find( assetids[i], string("Asset id: " + to_string(assetids[i]) + " was not found.").c_str() );
		check( !(itr->container.size() != 0),  "Asset id: " + to_string(assetids[i]) + " has " + to_string(itr->container.size())  + " attached NFT assets. Detach them before burning." );
		check( !(itr->containerf.size() != 0), "Asset id: " + to_string(assetids[i]) + " has " + to_string(itr->containerf.size()) + " attached FT assets. Detach them before burning." );
		check( owner.value == itr->owner.value, "Asset id: " + to_string(assetids[i]) + " you're attempting to burn is not yours. Owner is " + itr->owner.to_string() + ", you entered owner " + owner.to_string() );
		check( offert.find( assetids[i] ) == offert.end(), "Asset id: " + to_string(assetids[i]) + " has an open offer and cannot be burned." );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "Asset id: " + to_string(assetids[i]) + " is delegated and cannot be burned." );

		//Events
		uniqauthor[itr->author].push_back( assetids[i] );
		assets_f.erase(itr);
	}

	//Send Event as deferred
	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = move( uniqauthorIt->first );
		sendEvent(_self, owner, "saeburn"_n, make_tuple( keyauthor, owner, uniqauthor[keyauthor], memo ) );
	}

	SEND_INLINE_ACTION(*this, burnlog, { {_self, "active"_n} }, { owner, assetids, memo });
}

ACTION SimpleAssets::saeburn(name author, name owner, vector<uint64_t>& assetids, string memo) {
	require_recipient(author);
}

ACTION SimpleAssets::burnflog( name from, name author, asset quantity, string memo ) {
	require_auth(get_self());
}

ACTION SimpleAssets::burnnttlog(name owner, vector<uint64_t>& assetids, string memo) {
	require_auth(get_self());
}

ACTION SimpleAssets::burnlog( name owner, vector<uint64_t>& assetids, string memo ) {
	require_auth(get_self());
}

ACTION SimpleAssets::delegate( name owner, name to, vector<uint64_t>& assetids, uint64_t period, bool redelegate, string memo ) {

	check( owner != to, "cannot delegate to yourself" );
	check_empty_vector( assetids );
	check_memo_size( memo );
	require_auth( owner );
	require_recipient( owner );
	check( is_account( to ), "TO account does not exist" );

	sassets assets_f( _self, owner.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		check( assets_f.find( assetids[i] ) != assets_f.end(),  "Asset id: " + to_string( assetids[i] ) + " cannot be found at scope " + owner.to_string() );
		check( offert.find( assetids[i] ) == offert.end(), "Asset id: " + to_string( assetids[i] ) + " has an open offer and cannot be delegated." );

		if ( auto itr_delegated = delegatet.find( assetids[i] ); itr_delegated != delegatet.end() )
		{
			check( !( itr_delegated->redelegate == false ), "For asset id:" + to_string( assetids[i] ) + " .Terms of delegation forbid re-delegation." );
			check( !( itr_delegated->owner == to ), "For asset id:" + to_string( assetids[i] ) + " .Not allowed re-delegate to original owner of asset" );

			delegatet.modify(itr_delegated, owner, [&](auto& s) {
				s.delegatedto = to;
				s.redelegate  = redelegate;
			});
		}
		else {
			delegatet.emplace(owner, [&](auto& s) {
				s.assetid		= assetids[i];
				s.owner			= owner;
				s.delegatedto	= to;
				s.cdate			= current_time_point().sec_since_epoch();
				s.period		= period;
				s.redelegate	= redelegate;
				s.memo			= memo;
			});
		}
	}

	transfer( owner, to, assetids, "Delegate memo: " + memo );
}

ACTION SimpleAssets::delegatemore( name owner, uint64_t assetidc, uint64_t period ) {

	require_auth( owner );
	require_recipient( owner );

	const auto itrc = delegatet.require_find( assetidc, string("Assets id: " + to_string( assetidc ) + " is not delegated.").c_str() );
	check( owner == itrc->owner, "You are not the owner of asset id: " + to_string(assetidc) + ". Owner is: " + itrc->owner.to_string() + " , you entered: " + owner.to_string() );

	delegatet.modify( itrc, owner, [&]( auto& s ) {
		s.period = itrc->period + period;
	});
}

ACTION SimpleAssets::undelegate( name owner, vector<uint64_t>& assetids ) {
	
	require_auth( owner );
	require_recipient( owner );
	check_empty_vector( assetids );

	const auto itrc = delegatet.require_find( assetids[0], string( "Asset id: " + to_string( assetids[0] ) + " is not delegated" ).c_str() );
	name from = itrc->delegatedto;

	sassets assets_f( _self, from.value );

	string assetidsmemo;
	for ( auto i = 0; i < assetids.size(); ++i ) {

		const auto itrc = delegatet.require_find( assetids[i], string( "Asset id: " + to_string( assetids[i] ) + " is not delegated").c_str() );
		check( owner == itrc->owner, "You are not the owner of asset id: " + to_string( assetids[i]) + ". Owner is: " + itrc->owner.to_string() + " , you entered: " + owner.to_string() );
		check( !(from != itrc->delegatedto), "All delegated assets in assetids must be delegated to one account. For asset id: " + to_string( assetids[i] ) + " delegatedto = " + itrc->delegatedto.to_string() + " but it must be same with first asset which has delegatedto = " + from.to_string());

		const auto itr = assets_f.require_find(assetids[i], string("Asset id: " + to_string(assetids[i]) + " cannot be found in scope " + from.to_string()).c_str());
		check( itr->owner == itrc->delegatedto, "Owner does not match DELEGATEDTO for asset id: " + to_string( assetids[i] ) + " .Owner = " + itr->owner.to_string() + " but it must be " + itrc->delegatedto.to_string() + " for this asset");

		check((itrc->cdate + itrc->period) < current_time_point().sec_since_epoch(), 
			"Cannot undelegate until the PERIOD expires. " + timeToWait(abs((int)((uint64_t)(itrc->cdate + itrc->period) - (uint64_t)current_time_point().sec_since_epoch()))));

		if ( i != 0 ) {
			assetidsmemo += ", ";
		}

		assetidsmemo += to_string( assetids[i] );
	}

	transfer( from, owner, assetids, "undelegate assetid: " + assetidsmemo );
}

ACTION SimpleAssets::attach( name owner, uint64_t assetidc, vector<uint64_t>& assetids ) {

	check_empty_vector( assetids );
	sassets assets_f( _self, owner.value );
	require_recipient( owner );

	check( delegatet.find( assetidc ) == delegatet.end(), "Asset id: " + to_string( assetidc ) + " from parameter assetidc is delegated." );

	const auto ac_ = assets_f.require_find( assetidc, string("assetidc : " + to_string( assetidc ) + " cannot be found").c_str() );

	require_auth( ac_->author );

	for ( auto i = 0; i < assetids.size(); ++i ) {

		auto itr = assets_f.require_find( assetids[i], string("assetids asset id: " + to_string( assetidc ) + " cannot be found").c_str() );

		check( assetidc != assetids[i], "Cannot attach to self" );
		check( itr->author == ac_->author, 
			"Different authors. For asset id: " + to_string( assetids[i] ) + " author is " + itr->author.to_string() + " but for assetidc : " + to_string( assetidc )  + " author is " + ac_->author.to_string() );

		check( delegatet.find( assetids[i] ) == delegatet.end(), "Cannot attach asset id: " + to_string(assetids[i]) + " is delegated." );
		check( offert.find( assetids[i] ) == offert.end(), "Cannot attach asset id " + to_string(assetids[i]) + " has an open offer." );

		assets_f.modify( ac_, ac_->author, [&]( auto& a ) {
			a.container.push_back( *itr );
		});
		assets_f.erase( itr );
	}
}

ACTION SimpleAssets::detach( name owner, uint64_t assetidc, vector<uint64_t>& assetids ) {

	check_empty_vector( assetids );

	require_recipient( owner );
	sassets assets_f( _self, owner.value );

	const auto ac_ = assets_f.require_find( assetidc, string("assetidc: "  + to_string(assetidc) +  " cannot be found").c_str() );

	const auto author = ac_->author;

	require_auth( author );

	check( delegatet.find( assetidc ) == delegatet.end(), 
		"Cannot detach asset id: " + to_string( assetidc ) + " is delegated." );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		vector<sasset> newcontainer;

		for ( auto j = 0; j < ac_->container.size(); ++j ) {
			auto acc = ac_->container[j];
			if ( assetids[i] == acc.id ) {
				assets_f.emplace( author, [&]( auto& s ) {
					s.id         = acc.id;
					s.owner      = owner;
					s.author     = acc.author;
					s.category   = acc.category;
					s.idata      = acc.idata; 		// immutable data
					s.mdata      = acc.mdata; 		// mutable data
					s.container  = acc.container;
					s.containerf = acc.containerf;
				});
			}
			else {
				newcontainer.push_back( acc );
			}
		}

		assets_f.modify( ac_, author, [&]( auto& a ) {
			a.container = newcontainer;
		});
	}
}

ACTION SimpleAssets::attachf( name owner, name author, asset quantity, uint64_t assetidc ) {

	attachdeatch( owner, author, quantity, assetidc, true );
}


ACTION SimpleAssets::detachf( name owner, name author, asset quantity, uint64_t assetidc ) {

	attachdeatch( owner, author, quantity, assetidc, false );
}

ACTION SimpleAssets::createf( name author, asset maximum_supply, bool authorctrl, string data ) {

	require_auth( author );
	const auto sym = maximum_supply.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check( maximum_supply.is_valid(), "invalid supply" );
	check( maximum_supply.amount > 0, "max-supply must be positive" );

	stats statstable( _self, author.value );
	check( statstable.find( sym.code().raw() ) == statstable.end(), "token with symbol already exists" );

	statstable.emplace( author, [&]( auto& s ) {
		s.supply.symbol = maximum_supply.symbol;
		s.max_supply    = maximum_supply;
		s.issuer        = author;
		s.id            = getid( asset_id );
		s.authorctrl    = authorctrl;
		s.data          = data;
	});
}

ACTION SimpleAssets::updatef( name author, symbol sym, string data ) {

	require_auth( author );
	check( sym.is_valid(), "invalid symbol name" );
	stats statstable( _self, author.value );
	const auto existing = statstable.find( sym.code().raw() );
	check( existing != statstable.end(), "Symbol not exists" );

	statstable.modify( existing, author, [&]( auto& a ) {
		a.data = data;
	});
}

ACTION SimpleAssets::issuef( name to, name author, asset quantity, string memo ) {

	const auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check_memo_size( memo );

	stats statstable( _self, author.value );
	const auto existing = statstable.require_find( sym.code().raw(), "token with symbol does not exist, create token before issue" );

	require_auth( existing->issuer );
	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must issue positive quantity" );

	check( quantity.symbol == existing->supply.symbol, "symbol precision mismatch" );
	check( quantity.amount <= existing->max_supply.amount - existing->supply.amount, "quantity exceeds available supply" );

	statstable.modify( *existing, same_payer, [&]( auto& s ) {
		s.supply += quantity;
	});

	add_balancef( existing->issuer, author, quantity, existing->issuer );

	if ( to != existing->issuer ) {
		transferf( existing->issuer, to, author, quantity, memo );
	}
}

ACTION SimpleAssets::transferf( name from, name to, name author, asset quantity, string memo ) {

	check( from != to, "cannot transfer to self" );
	check( is_account( to ), "to account does not exist" );
	const auto sym = quantity.symbol.code();
	stats statstable( _self, author.value );
	const auto& st = statstable.get( sym.raw() );

	require_recipient( from );
	require_recipient( to );

	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must transfer positive quantity" );
	check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
	check_memo_size( memo );

	auto payer = has_auth( to ) ? to : from;
	auto checkAuth = from;

	if ( st.authorctrl && has_auth( st.issuer ) ) {
		checkAuth = st.issuer;
		payer = st.issuer;
	}

	require_auth( checkAuth );
	sub_balancef( from, author, quantity );
	add_balancef( to, author, quantity, payer );
}

ACTION SimpleAssets::offerf( name owner, name newowner, name author, asset quantity, string memo ) {

	require_auth( owner );
	require_recipient( owner );
	require_recipient( newowner );
	check( is_account( newowner ), "newowner account " + newowner.to_string() +" does not exist" );
	check( owner != newowner, "cannot offer to yourself" );
	const auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check_memo_size( memo );

	stats statstable( _self, author.value );
	const auto existing = statstable.require_find( sym.code().raw(), "token with symbol does not exist" );
	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must retire positive quantity" );
	check( quantity.symbol == existing->supply.symbol, "symbol precision mismatch" );

	auto owner_index = offerft.template get_index< "owner"_n >();

	for ( auto itro = owner_index.find( owner.value ); itro != owner_index.end(); itro++ ) {
		if ( itro->author == author && itro->offeredto == newowner && itro->quantity.symbol == quantity.symbol ) {
			auto itr = offerft.find( itro->id );
			if ( itr != offerft.end() ) {
				offerft.modify( itr, owner, [&](auto& s) {
					s.quantity.amount += quantity.amount;
					s.cdate = current_time_point().sec_since_epoch();
				});
				sub_balancef( owner, author, quantity );
				return ;
			}
		}
	}

	offerft.emplace( owner, [&]( auto& s ) {
		s.id        = getid( offer_id );
		s.author    = author;
		s.quantity  = quantity;
		s.offeredto = newowner;
		s.owner     = owner;
		s.cdate     = current_time_point().sec_since_epoch();
	});
	sub_balancef( owner, author, quantity );
}

ACTION SimpleAssets::cancelofferf( name owner, vector<uint64_t>& ftofferids ) {

	check_empty_vector( ftofferids, "ftofferids" );
	require_auth( owner );
	require_recipient( owner );

	for ( auto i = 0; i < ftofferids.size(); ++i ) {
		const auto itr = offerft.require_find( ftofferids[i], string("The offer id " + to_string( ftofferids[i] ) + " was not found").c_str() );

		check( owner.value == itr->owner.value, "Owner was changed for asset id:" + to_string( ftofferids[i]) + " .Owner is" + itr->owner.to_string() + " , you entered  " + owner.to_string() );

		add_balancef( owner, itr->author, itr->quantity, owner );
		offerft.erase( itr );
	}
}

ACTION SimpleAssets::claimf( name claimer, vector<uint64_t>& ftofferids ) {

	check_empty_vector( ftofferids, "ftofferids" );
	require_auth( claimer );
	require_recipient( claimer );
	map< name, vector< uint64_t > > uniqauthor;

	for ( auto i = 0; i < ftofferids.size(); ++i ) {
		auto itrc = offerft.require_find( ftofferids[i], string("Cannot find offer for asset id: " + to_string(ftofferids[i]) + " attempting to claim.").c_str() );
		check( claimer == itrc->offeredto, "Asset id: " + to_string(ftofferids[i])  + " has not been offerred to you. It offered to " + itrc->offeredto.to_string() );
		add_balancef( claimer, itrc->author, itrc->quantity, claimer );
		offerft.erase( itrc );
	}
}

ACTION SimpleAssets::burnf( name from, name author, asset quantity, string memo ) {

	auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check_memo_size( memo );

	stats statstable( _self, author.value );

	const auto existing = statstable.require_find( sym.code().raw(), "token with symbol does not exist" );

	require_auth( existing->authorctrl && has_auth( existing->issuer ) ? existing->issuer : from );

	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must retire positive quantity" );
	check( quantity.symbol == existing->supply.symbol, "symbol precision mismatch" );

	statstable.modify( *existing, same_payer, [&]( auto& s ) {
		s.supply -= quantity;
	});

	sub_balancef( from, author, quantity );
	SEND_INLINE_ACTION(*this, burnflog, { {_self, "active"_n} }, { from, author, quantity, memo });
}

ACTION SimpleAssets::openf( name owner, name author, const symbol& symbol, name ram_payer ) {

	require_auth( ram_payer );
	stats statstable( _self, author.value );
	const auto& st = statstable.get( symbol.code().raw(), "symbol does not exist" );
	check( st.supply.symbol == symbol, "symbol precision mismatch" );
	accounts acnts( _self, owner.value );

	if ( acnts.find( st.id ) == acnts.end() ) {
		acnts.emplace( ram_payer, [&]( auto& a ) {
			a.id      = st.id;
			a.author  = author;
			a.balance = asset{ 0, symbol };
		});
	}
}

ACTION SimpleAssets::closef( name owner, name author, const symbol& symbol ) {

	require_auth( owner );
	accounts acnts( _self, owner.value );
	auto it = acnts.require_find( getFTIndex( author, symbol ), "Balance row already deleted or never existed. Action won't have any effect" );
	check( it->balance.amount == 0, "Cannot close because the balance is not zero." );

	const auto owner_index = offerft.template get_index< "owner"_n >();
	for ( auto itro = owner_index.find( owner.value ); itro != owner_index.end(); itro++ ) {
		check( !( itro->author == author && itro->quantity.symbol == symbol ), "You have open offers for this FT.." );
	}

	acnts.erase( it );
}

uint64_t SimpleAssets::getid( id_type type ) {

	conf config(get_self(), get_self().value);

	_cstate = config.exists() ? config.get() : global{};

	uint64_t result = 0;
	
	switch ( type ) {
	case asset_id:
		result = ++_cstate.lnftid;
		break;
	case offer_id:
	case deferred_id:
		result = ++_cstate.defid;
		break;
	case md_id:
		if ( _cstate.mdid < 100000000 )
			_cstate.mdid = 100000000;
		result = ++_cstate.mdid;
		break;
	default:
		checkid( type );
	}

	config.set( _cstate, _self );
	return result;
}

uint64_t SimpleAssets::getFTIndex( name author, symbol symbol ) {

	stats statstable( _self, author.value );
	return statstable.require_find(symbol.code().raw(), "token with symbol does not exist")->id;
}

void SimpleAssets::attachdeatch( name owner, name author, asset quantity, uint64_t assetidc, bool attach ) {

	sassets assets_f( _self, owner.value );
	stats statstable( _self, author.value );
	const auto& st = statstable.get( quantity.symbol.code().raw() );

	require_auth( author );
	require_recipient( owner );

	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must transfer positive quantity" );
	check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
	check( st.issuer == author, "Different authors. For asset " + quantity.to_string() + " issuer is " + st.issuer.to_string() + " you entered author = " + author.to_string() );

	const auto itr = assets_f.require_find( assetidc, string("assetid " + to_string( assetidc ) + " cannot be found").c_str() );

	check( itr->author == author, "Different authors. For asset id: " + to_string( assetidc ) + " author is " + itr->author.to_string() + " you entered author = " + author.to_string() );

	const auto itr_delegeted = delegatet.find( assetidc );
	check( itr_delegeted == delegatet.end(), "Asset id: " + to_string( assetidc ) + " is delegated to " + itr_delegeted->delegatedto.to_string() );

	const auto itr_offered = offert.find( assetidc );
	check( itr_offered == offert.end(), "Asset id: " + to_string( assetidc ) + " has an open offer to " + itr_offered->offeredto.to_string() + "and cannot be delegated." );

	vector<account> newcontainerf;
	bool found = false;

	for ( auto j = 0; j < itr->containerf.size(); j++ ) {
		auto accf = itr->containerf[j];
		if ( st.id == accf.id ) {
			if ( attach ) {
				accf.balance.amount += quantity.amount;
			}
			else {
				check( accf.balance.amount >= quantity.amount, "overdrawn balance" );
				accf.balance.amount -= quantity.amount;
			}
			found = true;
		}
		if ( accf.balance.amount > 0 )
			newcontainerf.push_back( accf );
	}

	if ( !found && attach ) {
		account addft;
		addft.id      = st.id;
		addft.author  = author;
		addft.balance = quantity;

		newcontainerf.push_back( addft );
	}

	if ( !attach )
		check( found, "not attached" );

	assets_f.modify( itr, author, [&]( auto& a ) {
		a.containerf = newcontainerf;
	});

	if ( attach ) {
		sub_balancef( owner, author, quantity );
	}
	else {
		add_balancef( owner, author, quantity, author );
	}
}

void SimpleAssets::sub_balancef( name owner, name author, asset value ) {

	accounts from_acnts( _self, owner.value );
	const auto& from = from_acnts.get( getFTIndex( author, value.symbol ), "no balance object found" );
	check( from.balance.amount >= value.amount, "overdrawn balance" );
	check( value.symbol.code().raw() == from.balance.symbol.code().raw(), "Wrong symbol" );

	from_acnts.modify( from, has_auth( author ) ? author : owner, [&]( auto& a ) {
		a.balance -= value;
	});
}

void SimpleAssets::add_balancef( name owner, name author, asset value, name ram_payer ) {

	accounts to_acnts( _self, owner.value );
	auto ftid = getFTIndex( author, value.symbol );
	auto to = to_acnts.find( ftid );

	if ( to == to_acnts.end() ) {
		to_acnts.emplace( ram_payer, [&]( auto& a ) {
			a.id      = ftid;
			a.balance = value;
			a.author  = author;
		});
	}
	else {
		to_acnts.modify( to, same_payer, [&]( auto& a ) {
			a.balance += value;
		});
	}
}

ACTION SimpleAssets::createntt( name author, name category, name owner, string idata, string mdata, bool requireclaim ) {

	require_auth( author );
	check( is_account( owner ), "owner account " + owner.to_string() + " does not exist" );
	require_recipient( owner );
	const auto newID = getid( asset_id );
	name assetOwner = owner;
	check( !( author.value == owner.value && requireclaim == 1 ), "Can't requireclaim if author == owner." );

	if ( requireclaim ) {
		assetOwner = author;
		//add info to offers table

		nttoffert.emplace( author, [&]( auto& s ) {
			s.assetid   = newID;
			s.offeredto = owner;
			s.owner     = author;
			s.cdate     = current_time_point().sec_since_epoch();
		});
	}

	snttassets ntt(_self, assetOwner.value);
	ntt.emplace( author, [&](auto& s) {
		s.id       = newID;
		s.owner    = assetOwner;
		s.author   = author;
		s.category = category;
		s.mdata    = mdata; // mutable data
		s.idata    = idata; // immutable data
	});

	SEND_INLINE_ACTION( *this, createnttlog, { {_self, "active"_n} }, { author, category, owner, idata, mdata, newID, requireclaim } );
}

ACTION SimpleAssets::createnttlog( name author, name category, name owner, string idata, string mdata, uint64_t assetid, bool requireclaim ) {

	require_auth(get_self());
}

ACTION SimpleAssets::updatentt( name author, name owner, uint64_t assetid, string mdata ) {

	require_auth(author);
	snttassets assets_f(_self, owner.value);

	const auto itr = assets_f.require_find( assetid, string("asset id: " + to_string( assetid ) + " not found").c_str() );

	check( itr->author == author, "Only for author allowed to update asset. Asset id: " + to_string( assetid ) + " has author: " + itr->author.to_string() + " ,you entered author: " + author.to_string() );

	assets_f.modify(itr, author, [&](auto& a) {
		a.mdata = mdata;
	});
}

ACTION SimpleAssets::claimntt( name claimer, vector<uint64_t>& assetids ) {

	check_empty_vector( assetids );
	require_auth( claimer );
	require_recipient( claimer );
	snttassets assets_claimer( _self, claimer.value );
	map< name, map< uint64_t, name > > uniqauthor;

	for ( auto i = 0; i < assetids.size(); ++i ) {

		auto itrc = nttoffert.require_find( assetids[i], string("Cannot find offer for asset id:  " + to_string( assetids[i] ) + " that you're attempting to claim.").c_str());

		check( claimer == itrc->offeredto, "Asset id: " + to_string(assetids[i]) + " has not been offerred to you. It offered to " + itrc->offeredto.to_string() );

		snttassets assets_owner( _self, itrc->owner.value );

		auto itr = assets_owner.require_find( assetids[i], string("Cannot find asset id: " + to_string( assetids[i] ) + " that you're attempting to claim at scope: " + itrc->owner.to_string()).c_str() );

		check( itrc->owner.value == itr->owner.value, "Owner was changed for asset id:" + to_string( assetids[i] ) + " .Owner at offers:" + itrc->owner.to_string() + " . Owner at assets: " + itr->owner.to_string() );

		assets_claimer.emplace( claimer, [&](auto& s) {
			s.id       = itr->id;
			s.owner    = claimer;
			s.author   = itr->author;
			s.category = itr->category;
			s.mdata    = itr->mdata; 		// mutable data
			s.idata    = itr->idata; 		// immutable data
		});

		//Events
		uniqauthor[itr->author][assetids[i]] = itrc->owner;

		assets_owner.erase( itr );
		nttoffert.erase( itrc );
	}

	//for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
	//	name keyauthor = move( uniqauthorIt->first );
	//	sendEvent( keyauthor, claimer, "saeclaim"_n, make_tuple( claimer, uniqauthor[keyauthor] ) );
	//}
}

ACTION SimpleAssets::burnntt( name owner, vector<uint64_t>& assetids, string memo ) {

	require_auth( owner );
	require_recipient( owner );

	snttassets assets_ntt( _self, owner.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {

		auto itr_asset = assets_ntt.require_find( assetids[i], string("Asset id: " + to_string( assetids[i] ) + " was not found." ).c_str());

		auto itroffer = nttoffert.find( assetids[i] );

		if ( itroffer != nttoffert.end() ) {
			check(owner.value == itroffer->owner.value, "You're not the owner of Asset id: " + to_string( assetids[i] ) + " you're attempting to burn. Owner at offer is " + itroffer->owner.to_string() + ", you entered owner " + owner.to_string());
			nttoffert.erase( itroffer );
		}

		assets_ntt.erase( itr_asset );
	}

	//Send Event as deferred
	//for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
	//	name keyauthor = move( uniqauthorIt->first );
	//	sendEvent( keyauthor, owner, "saeburn"_n, make_tuple( owner, uniqauthor[keyauthor], memo ) );
	//}
	SEND_INLINE_ACTION(*this, burnnttlog, { {_self, "active"_n} }, { owner, assetids, memo });
}

ACTION SimpleAssets::mdadd( name author, string data ) {
	require_auth( author );
	require_recipient( author );

	const auto newID = getid( md_id );

	moredatat.emplace( author, [&](auto& s) {
		s.id     = newID;
		s.author = author;
		s.data   = data;
	});

	SEND_INLINE_ACTION(*this, mdaddlog, { {_self, "active"_n} }, { newID, author, data });
}

ACTION SimpleAssets::mdupdate( uint64_t id, name author, string data ) {
	require_auth( author );
	require_recipient( author );

	auto itr = moredatat.require_find( id, string( "More data item with id: " + to_string( id ) + " was not found" ).c_str() );

	check( itr->author == author, "Only author " + itr->author.to_string() + " can update more data. You entered author " + author.to_string() );

	moredatat.modify( itr, author, [&](auto& s) {
		s.author = author;
		s.data   = data;
	});
}

ACTION SimpleAssets::mdremove( uint64_t id ) {

	auto itr = moredatat.require_find( id, string( "More data item with id: " + to_string( id ) + " was not found." ).c_str() );

	require_auth( itr->author );
	require_recipient( itr->author );

	moredatat.erase( itr );
}

ACTION SimpleAssets::mdaddlog( uint64_t id, name author, string data ) { 
	require_auth(get_self());
}

ACTION SimpleAssets::delarampayer( uint64_t id )
{
	require_auth( get_self() );

	auto itr = arampayert.require_find( id,
		string( "Ram payer id " + to_string( id ) + " does not exist").c_str() );

	itr = arampayert.erase( itr );
}

ACTION SimpleAssets::setarampayer( name author, name category, bool usearam )
{
	require_auth( author );
	require_recipient( author );

	auto author_index = arampayert.template get_index< "author"_n >();

	uint64_t ram_payer_id = 0;
	for ( auto itr = author_index.find(author.value); itr != author_index.end(); itr++ ) {
		if ( itr->author == author && itr->category == category ) {
			ram_payer_id = itr->id;
			break;
		}
	}

	if ( ram_payer_id != 0 )
	{
		auto itrrp = arampayert.require_find( ram_payer_id, 
			string( "Ram payer id " + to_string( ram_payer_id ) + " does not exist" ).c_str() );
		arampayert.modify( itrrp, author, [&]( auto& a ) {
			a.usearam = usearam;
			a.from_id = sa_getnextid(_self, asset_id) - 1;
		});
	}
	else
	{
		arampayert.emplace( author, [&](auto& s) {
			s.id		= getid( md_id );
			s.author	= author;
			s.category	= category;
			s.usearam	= usearam;
			s.from_id	= sa_getnextid( _self, asset_id ) - 1;
		});
	}
}

template<typename... Args>
void SimpleAssets::sendEvent( name author, name rampayer, name seaction, const tuple<Args...> &adata ) 
{
	transaction sevent{};
	sevent.actions.emplace_back( permission_level{ _self, "active"_n }, author, seaction, adata );
	sevent.delay_sec = 0;
	sevent.send( getid( deferred_id ), rampayer );
}

asset SimpleAssets::get_supply( name token_contract_account, name author, symbol_code sym_code ) {

	stats statstable( token_contract_account, author.value );
	return statstable.get( sym_code.raw() ).supply;
}

asset SimpleAssets::get_balance( name token_contract_account, name owner, name author, symbol_code sym_code ) {

	stats statstable( token_contract_account, author.value );
	accounts accountstable( token_contract_account, owner.value );
	return accountstable.get( statstable.get( sym_code.raw() ).id ).balance;
}

std::string SimpleAssets::timeToWait( uint64_t time_in_seconds ){

	uint64_t s, h, m = 0;
	m = time_in_seconds / 60;
	h = m / 60;
	return "Time to wait " + std::to_string(int(h)) + " hours " + std::to_string(int(m % 60)) + " minutes " + std::to_string(int(time_in_seconds % 60)) + " seconds";
}

EOSIO_DISPATCH( SimpleAssets, ( create )( createlog )( transfer )( burn )( update )
( offer )( canceloffer )( claim )
( authorreg )( authorupdate )
( delegate )( undelegate )( delegatemore )( attach )( detach )
( createf )( updatef )( issuef )( transferf )( burnf )
( offerf )( cancelofferf )( claimf )
( attachf )( detachf )( openf )( closef )
( updatever )( createntt )( burnntt )( createnttlog )( claimntt )( updatentt )( changeauthor ) 
( mdadd )( mdupdate )( mdremove )( mdaddlog ) ( burnlog ) ( burnnttlog ) ( burnflog ) 
( saetransfer ) ( saeburn ) ( saechauthor ) ( saecreate ) ( saeclaim ) ( setarampayer ) ( delarampayer ) )


//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================