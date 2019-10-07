#include <SimpleAssets.hpp>

ACTION SimpleAssets::updatever( string version ) {

	require_auth( get_self() );
	Configs configs( _self, _self.value );
	configs.set( tokenconfigs{ "simpleassets"_n, version }, _self );
}

ACTION SimpleAssets::regauthor( name author, string data, string stemplate, string imgpriority ) {

	require_auth( author );
	require_recipient( author );
	check( data.size() > 3, "Data field is too short. Please tell us about yourselves." );
	authors author_( _self, _self.value );

	if ( author_.find( author.value ) == author_.end() ) {
		author_.emplace( author, [&]( auto& s ) {
			s.author      = author;
			s.data        = data;
			s.stemplate   = stemplate;
			s.imgpriority = imgpriority;
		});
	}
	else {
		check( false, "Registration Error. You're probably already registered. Try the authupdate action." );
	}
}

ACTION SimpleAssets::authorupdate( name author, string data, string stemplate, string imgpriority ) {

	require_auth( author );
	require_recipient( author );
	authors author_( _self, _self.value );
	auto itr = author_.find( author.value );
	check( itr != author_.end(), "author not registered" );

	if ( data.empty() && stemplate.empty() ) {
		itr = author_.erase( itr );
	}
	else {
		author_.modify( itr, author, [&]( auto& s ) {
			s.data        = data;
			s.stemplate   = stemplate;
			s.imgpriority = imgpriority;
		});
	}
}

ACTION SimpleAssets::create( name author, name category, name owner, string idata, string mdata, bool requireclaim ) {

	require_auth( author );
	check( is_account( owner ), "owner account does not exist" );
	require_recipient( owner );
	const auto newID = getid();
	name assetOwner = owner;
	check( !( author.value == owner.value && requireclaim == 1 ), "Can't requireclaim if author == owner." );

	if ( requireclaim ) {
		assetOwner = author;
		//add info to offers table
		offers offert( _self, _self.value );
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
	sendEvent( author, author, "saecreate"_n, std::make_tuple( owner, newID ) );
	SEND_INLINE_ACTION( *this, createlog, { {_self, "active"_n} }, { author, category, owner, idata, mdata, newID, requireclaim } );
}

ACTION SimpleAssets::createlog( name author, name category, name owner, string idata, string mdata, uint64_t assetid, bool requireclaim ) {

	require_auth(get_self());
}

ACTION SimpleAssets::claim( name claimer, std::vector<uint64_t>& assetids ) {

	require_auth( claimer );
	require_recipient( claimer );
	offers offert( _self, _self.value );
	sassets assets_t( _self, claimer.value );

	std::map< name, std::map< uint64_t, name > > uniqauthor;
	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itrc = offert.find( assetids[i] );
		check( itrc != offert.end(), "Cannot find at least one of the assets you're attempting to claim." );
		check( claimer == itrc->offeredto, "At least one of the assets has not been offerred to you." );

		sassets assets_f( _self, itrc->owner.value );
		auto itr = assets_f.find( assetids[i] );
		check( itr != assets_f.end(), "Cannot find at least one of the assets you're attempting to claim." );
		check( itrc->owner.value == itr->owner.value, "Owner was changed for at least one of the items!?" );

		assets_t.emplace( claimer, [&]( auto& s ) {
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

	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = std::move( uniqauthorIt->first );
		sendEvent( keyauthor, claimer, "saeclaim"_n, std::make_tuple( claimer, uniqauthor[keyauthor] ) );
	}
}

ACTION SimpleAssets::transfer( name from, name to, std::vector<uint64_t>& assetids, string memo ) {

	check( from != to, "cannot transfer to yourself" );
	check( is_account( to ), "TO account does not exist" );
	check( memo.size() <= 256, "memo has more than 256 bytes" );

	require_recipient( from );
	require_recipient( to );

	sassets assets_f( _self, from.value );
	sassets assets_t( _self, to.value );

	delegates delegatet( _self, _self.value );
	offers offert( _self, _self.value );

	const auto rampayer = has_auth( to ) ? to : from;

	bool isDelegeting = false;

	std::map< name, std::vector<uint64_t> > uniqauthor;

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
				check( false, "At least one of the assets cannot be transferred because it is delegated" );
			}
		}

		if ( isDelegeting ) {
			require_auth( has_auth( itrd->owner ) ? itrd->owner : from );
		}
		else {
			require_auth( from );
		}

		auto itr = assets_f.find( assetids[i] );
		check( itr != assets_f.end(), "At least one of the assets cannot be found (check ids?)" );
		check( from.value == itr->owner.value, "At least one of the assets is not yours to transfer." );
		check( offert.find( assetids[i] ) == offert.end(), "At least one of the assets has been offered for a claim and cannot be transferred. Cancel offer?" );

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
		assets_f.erase(itr);
	}

	//Send Event as deferred
	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = std::move( uniqauthorIt->first );
		sendEvent( keyauthor, rampayer, "saetransfer"_n, std::make_tuple( from, to, uniqauthor[keyauthor], memo ) );
	}
}

ACTION SimpleAssets::update( name author, name owner, uint64_t assetid, string mdata ) {

	require_auth( author );
	sassets assets_f( _self, owner.value );
	const auto itr = assets_f.find( assetid );
	check( itr != assets_f.end(), "asset not found" );
	check( itr->author == author, "Only author can update asset." );

	assets_f.modify( itr, author, [&]( auto& a ) {
		a.mdata = mdata;
	});
}

ACTION SimpleAssets::offer( name owner, name newowner, std::vector<uint64_t>& assetids, string memo ) {

	check( owner != newowner, "cannot offer to yourself" );
	require_auth( owner );
	require_recipient( owner );
	require_recipient( newowner );
	check( is_account( newowner ), "newowner account does not exist" );

	sassets assets_f( _self, owner.value );
	offers offert( _self, _self.value );
	delegates delegatet( _self, _self.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		check( assets_f.find( assetids[i] ) != assets_f.end(), "At least one of the assets was not found." );
		check( offert.find( assetids[i] ) == offert.end(), "At least one of the assets is already offered for claim." );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "At least one of the assets is delegated and cannot be offered." );

		offert.emplace( owner, [&]( auto& s ) {
			s.assetid   = assetids[i];
			s.offeredto = newowner;
			s.owner     = owner;
			s.cdate     = current_time_point().sec_since_epoch();
		});
	}
}

ACTION SimpleAssets::canceloffer( name owner, std::vector<uint64_t>& assetids ) {

	require_auth( owner );
	require_recipient( owner );
	offers offert( _self, _self.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itr = offert.find( assetids[i] );
		check( itr != offert.end(), "The offer for at least one of the assets was not found." );
		check( owner.value == itr->owner.value, "You're not the owner of at least one of the assets whose offers you're attempting to cancel." );
		offert.erase( itr );
	}
}

ACTION SimpleAssets::burn( name owner, std::vector<uint64_t>& assetids, string memo ) {

	require_auth( owner );
	sassets assets_f( _self, owner.value );
	offers offert( _self, _self.value );
	delegates delegatet( _self, _self.value );

	std::map< name, std::vector<uint64_t> > uniqauthor;

	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itr = assets_f.find( assetids[i] );
		check( itr != assets_f.end(), "At least one of the assets was not found." );
		check( owner.value == itr->owner.value, "At least one of the assets you're attempting to burn is not yours." );
		check( offert.find( assetids[i] ) == offert.end(), "At least one of the assets has an open offer and cannot be burned." );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "At least one of assets is delegated and cannot be burned." );

		//Events
		uniqauthor[itr->author].push_back( assetids[i] );
		assets_f.erase(itr);
	}

	//Send Event as deferred
	for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
		name keyauthor = std::move( uniqauthorIt->first );
		sendEvent( keyauthor, owner, "saeburn"_n, std::make_tuple( owner, uniqauthor[keyauthor], memo ) );
	}
}

ACTION SimpleAssets::delegate( name owner, name to, std::vector<uint64_t>& assetids, uint64_t period, string memo ) {

	check(memo.size() <= 64, "Error. Size of memo cannot be bigger 64");
	check( owner != to, "cannot delegate to yourself" );
	require_auth( owner );
	require_recipient( owner );
	check( is_account( to ), "TO account does not exist" );

	sassets assets_f( _self, owner.value );
	delegates delegatet( _self, _self.value );
	offers offert( _self, _self.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		check( assets_f.find( assetids[i] ) != assets_f.end(), "At least one of the assets cannot be found." );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "At least one of the assets is already delegated." );
		check( offert.find( assetids[i] ) == offert.end(), "At least one of the assets has an open offer and cannot be delegated." );

		delegatet.emplace( owner, [&]( auto& s ) {
			s.assetid     = assetids[i];
			s.owner       = owner;
			s.delegatedto = to;
			s.cdate       = current_time_point().sec_since_epoch();
			s.period      = period;
			s.memo        = memo;
		});
	}

	transfer( owner, to, assetids, "Delegate memo: " + memo );
}

ACTION SimpleAssets::delegatemore( name owner, uint64_t assetidc, uint64_t period ) {

	require_auth( owner );
	require_recipient( owner );

	delegates delegatet( _self, _self.value );
	const auto itrc = delegatet.find( assetidc );
	check( itrc != delegatet.end(), "Assets assetidc is not delegated." );
	check( owner == itrc->owner, "You are not the owner of this asset." );

	delegatet.modify( itrc, owner, [&]( auto& s ) {
		s.period = itrc->period + period;
	});
}

ACTION SimpleAssets::undelegate( name owner, name from, std::vector<uint64_t>& assetids ) {

	require_auth( owner );
	require_recipient( owner );
	check( is_account( from ), "to account does not exist" );

	sassets assets_f( _self, from.value );
	delegates delegatet( _self, _self.value );

	string assetidsmemo;
	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itr = assets_f.find( assetids[i] );
		check( itr != assets_f.end(), "At least one of the assets cannot be found." );
		auto itrc = delegatet.find( assetids[i] );
		check( itrc != delegatet.end(), "At least one of the assets is not delegated." );
		check( owner == itrc->owner, "You are not the owner of at least one of these assets." );
		check( from == itrc->delegatedto, "FROM does not match DELEGATEDTO for at least one of the assets." );
		check( itr->owner == itrc->delegatedto, "FROM does not match DELEGATEDTO for at least one of the assets." );
		check( ( itrc->cdate + itrc->period ) < current_time_point().sec_since_epoch(), "Cannot undelegate until the PERIOD expires." );

		if ( i != 0 ) {
			assetidsmemo += ", ";
		}

		assetidsmemo += std::to_string( assetids[i] );
	}

	transfer( from, owner, assetids, "undelegate assetid: " + assetidsmemo );
}


ACTION SimpleAssets::attach( name owner, uint64_t assetidc, std::vector<uint64_t>& assetids ) {

	sassets assets_f( _self, owner.value );
	delegates delegatet( _self, _self.value );
	offers offert( _self, _self.value );
	require_recipient( owner );
	const auto ac_ = assets_f.find( assetidc );
	check( ac_ != assets_f.end(), "Asset cannot be found." );
	require_auth( ac_->author );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itr = assets_f.find( assetids[i] );
		check( itr != assets_f.end(), "At least one of the assets cannot be found." );
		check( assetidc != assetids[i], "Cannot attcach to self." );
		check( itr->author == ac_->author, "Different authors." );
		check( delegatet.find( assetids[i] ) == delegatet.end(), "At least one of the assets is delegated." );
		check( offert.find( assetids[i] ) == offert.end(), "At least one of the assets has an open offer and cannot be delegated." );

		assets_f.modify( ac_, ac_->author, [&]( auto& a ) {
			a.container.push_back( *itr );
		});
		assets_f.erase( itr );
	}
}

ACTION SimpleAssets::detach( name owner, uint64_t assetidc, std::vector<uint64_t>& assetids ) {

	require_auth( owner );
	require_recipient( owner );
	sassets assets_f( _self, owner.value );

	const auto ac_ = assets_f.find( assetidc );
	check( ac_ != assets_f.end(), "Asset cannot be found." );

	delegates delegatet( _self, _self.value );
	check( delegatet.find( assetidc ) == delegatet.end(), "Cannot detach from delegated. assetidc is delegated." );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		std::vector<sasset> newcontainer;

		for ( auto j = 0; j < ac_->container.size(); ++j ) {
			auto acc = ac_->container[j];
			if ( assetids[i] == acc.id ) {
				assets_f.emplace( owner, [&]( auto& s ) {
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

		assets_f.modify( ac_, owner, [&]( auto& a ) {
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
		s.id            = getid();
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
	check( memo.size() <= 256, "memo has more than 256 bytes" );

	stats statstable( _self, author.value );
	const auto existing = statstable.find( sym.code().raw() );
	check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );

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
	check( memo.size() <= 256, "memo has more than 256 bytes" );

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
	check( is_account( newowner ), "newowner account does not exist" );
	check( owner != newowner, "cannot offer to yourself" );
	const auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check( memo.size() <= 256, "memo has more than 256 bytes" );

	stats statstable( _self, author.value );
	const auto existing = statstable.find( sym.code().raw() );
	check( existing != statstable.end(), "token with symbol does not exist" );
	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must retire positive quantity" );
	check( quantity.symbol == existing->supply.symbol, "symbol precision mismatch" );

	offerfs offert( _self, _self.value );
	auto owner_index = offert.template get_index< "owner"_n >();

	for ( auto itro = owner_index.find( owner.value ); itro != owner_index.end(); itro++ ) {
		if ( itro->author == author && itro->offeredto == newowner && itro->quantity.symbol == quantity.symbol ) {
			auto itr = offert.find( itro->id );
			if ( itr != offert.end() ) {
				  offert.modify( itr, owner, [&](auto& s) {
					s.quantity.amount += quantity.amount;
					s.cdate = current_time_point().sec_since_epoch();
				});
				sub_balancef( owner, author, quantity );
				return ;
			}
		}
	}

	offert.emplace( owner, [&]( auto& s ) {
		s.id        = getid( true );
		s.author    = author;
		s.quantity  = quantity;
		s.offeredto = newowner;
		s.owner     = owner;
		s.cdate     = current_time_point().sec_since_epoch();
	});
	sub_balancef( owner, author, quantity );
}

ACTION SimpleAssets::cancelofferf( name owner, std::vector<uint64_t>& ftofferids ) {

	require_auth( owner );
	require_recipient( owner );
	offerfs offert( _self, _self.value );

	for ( auto i = 0; i < ftofferids.size(); ++i ) {
		auto itr = offert.find( ftofferids[i] );
		check( itr != offert.end(), "The offer for at least one of the FT was not found." );
		check( owner.value == itr->owner.value, "You're not the owner of at least one of those FTs." );
		add_balancef( owner, itr->author, itr->quantity, owner );
		offert.erase( itr );
	}
}

ACTION SimpleAssets::claimf( name claimer, std::vector<uint64_t>& ftofferids ) {

	require_auth( claimer );
	require_recipient( claimer );
	offerfs offert( _self, _self.value );
	std::map< name, std::vector< uint64_t > > uniqauthor;

	for ( auto i = 0; i < ftofferids.size(); ++i ) {
		auto itrc = offert.find( ftofferids[i] );
		check( itrc != offert.end(), "Cannot find at least one of the FT you're attempting to claim." );
		check( claimer == itrc->offeredto, "At least one of the FTs has not been offerred to you." );
		add_balancef( claimer, itrc->author, itrc->quantity, claimer );
		offert.erase( itrc );
	}
}

ACTION SimpleAssets::burnf( name from, name author, asset quantity, string memo ) {

	auto sym = quantity.symbol;
	check( sym.is_valid(), "invalid symbol name" );
	check( memo.size() <= 256, "memo has more than 256 bytes" );
	stats statstable( _self, author.value );

	const auto existing = statstable.find( sym.code().raw() );
	check( existing != statstable.end(), "token with symbol does not exist" );
	require_auth( existing->authorctrl && has_auth( existing->issuer ) ? existing->issuer : from );

	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must retire positive quantity" );
	check( quantity.symbol == existing->supply.symbol, "symbol precision mismatch" );

	statstable.modify( *existing, same_payer, [&]( auto& s ) {
		s.supply -= quantity;
	});

	sub_balancef( from, author, quantity );
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
	auto it = acnts.find( getFTIndex( author, symbol ) );
	check( it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect." );
	check( it->balance.amount == 0, "Cannot close because the balance is not zero." );

	offerfs offert( _self, _self.value );
	const auto owner_index = offert.template get_index< "owner"_n >();
	for ( auto itro = owner_index.find( owner.value ); itro != owner_index.end(); itro++ ) {
		check( !( itro->author == author && itro->quantity.symbol == symbol ), "You have open offers for this FT.." );
	}

	acnts.erase( it );
}

uint64_t SimpleAssets::getid( bool defer ) {

	// getid private action Increment, save and return id for a new asset or new fungible token.
	conf config( _self, _self.value );
	_cstate = config.exists() ? config.get() : global{};

	uint64_t resid;
	if ( defer ) {
		_cstate.defid++;
		resid = _cstate.defid;
	}
	else {
		_cstate.lnftid++;
		resid = _cstate.lnftid;
	}

	config.set( _cstate, _self );
	return resid;
}

uint64_t SimpleAssets::getFTIndex( name author, symbol symbol ) {

	stats statstable( _self, author.value );
	const auto existing = statstable.find( symbol.code().raw() );
	check( existing != statstable.end(), "token with symbol does not exist." );
	return existing->id;
}

void SimpleAssets::attachdeatch( name owner, name author, asset quantity, uint64_t assetidc, bool attach ) {

	sassets assets_f( _self, owner.value );
	delegates delegatet( _self, _self.value );
	offers offert( _self, _self.value );
	stats statstable( _self, author.value );
	const auto& st = statstable.get( quantity.symbol.code().raw() );

	require_recipient( owner );

	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must transfer positive quantity" );
	check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
	check( st.issuer == author, "Different authors" );

	if ( attach ) {
		require_auth( author );	 //attach
	}
	else {
		require_auth( owner );  //deatach
	}

	const auto itr = assets_f.find( assetidc );
	check( itr != assets_f.end(), "assetid cannot be found." );
	check( itr->author == author, "Different authors." );
	check( delegatet.find(assetidc) == delegatet.end(), "Asset is delegated." );
	check( offert.find(assetidc) == offert.end(), "Assets has an open offer and cannot be delegated." );

	std::vector<account> newcontainerf;
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
		add_balancef( owner, author, quantity, owner );
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
	check( is_account( owner ), "owner account does not exist" );
	require_recipient( owner );
	const auto newID = getid();
	name assetOwner = owner;
	check( !( author.value == owner.value && requireclaim == 1 ), "Can't requireclaim if author == owner." );

	if ( requireclaim ) {
		assetOwner = author;
		//add info to offers table
		nttoffers nttoffer( _self, _self.value );
		nttoffer.emplace( author, [&]( auto& s ) {
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
	const auto itr = assets_f.find(assetid);
	check(itr != assets_f.end(), "asset not found");
	check(itr->author == author, "Only author can update asset.");

	assets_f.modify(itr, author, [&](auto& a) {
		a.mdata = mdata;
	});
}

ACTION SimpleAssets::claimntt( name claimer, std::vector<uint64_t>& assetids ) {

	require_auth( claimer );
	require_recipient( claimer );
	nttoffers nttoffert( _self, _self.value );
	snttassets assets_claimer( _self, claimer.value );

	std::map< name, std::map< uint64_t, name > > uniqauthor;
	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itrc = nttoffert.find( assetids[i] );
		check( !( itrc == nttoffert.end() ), "Cannot find at least one of the offers you're attempting to claim." );
		check( claimer == itrc->offeredto, "At least one of the assets has not been offerred to you." );

		snttassets assets_owner( _self, itrc->owner.value );
		auto itr = assets_owner.find( assetids[i] );
		check( !(itr == assets_owner.end() ), "Cannot find at least one of the assets you're attempting to claim." );
		check( itrc->owner.value == itr->owner.value, "Owner was changed for at least one of the items!?" );

		assets_claimer.emplace( claimer, [&](auto& s) {
			s.id = itr->id;
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
	//	name keyauthor = std::move( uniqauthorIt->first );
	//	sendEvent( keyauthor, claimer, "saeclaim"_n, std::make_tuple( claimer, uniqauthor[keyauthor] ) );
	//}
}

ACTION SimpleAssets::burnntt( name owner, std::vector<uint64_t>& assetids, string memo ) {

	require_auth( owner );
	require_recipient( owner );

	snttassets assets_ntt( _self, owner.value );
	nttoffers nttoffert( _self, _self.value );

	for ( auto i = 0; i < assetids.size(); ++i ) {
		auto itr_asset = assets_ntt.find( assetids[i] );
		check( itr_asset != assets_ntt.end(), "At least one of the assets was not found." );

		auto itroffer = nttoffert.find( assetids[i] );

		if ( itroffer != nttoffert.end() ) {
			check( owner.value == itroffer->owner.value, "You're not the owner of at least one of the assets whose offers you're attempting to cancel." );
			nttoffert.erase( itroffer );
		}

		assets_ntt.erase( itr_asset );
	}

	//Send Event as deferred
	//for ( auto uniqauthorIt = uniqauthor.begin(); uniqauthorIt != uniqauthor.end(); ++uniqauthorIt ) {
	//	name keyauthor = std::move( uniqauthorIt->first );
	//	sendEvent( keyauthor, owner, "saeburn"_n, std::make_tuple( owner, uniqauthor[keyauthor], memo ) );
	//}
}

template<typename... Args>
void SimpleAssets::sendEvent( name author, name rampayer, name seaction, const std::tuple<Args...> &adata ) {

	transaction sevent{};
	sevent.actions.emplace_back( permission_level{ _self, "active"_n }, author, seaction, adata );
	sevent.delay_sec = 0;
	sevent.send( getid(true), rampayer );
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

EOSIO_DISPATCH( SimpleAssets, ( create )( createlog )( transfer )( burn )( update )
( offer )( canceloffer )( claim )
( regauthor )( authorupdate )
( delegate )( undelegate )( delegatemore )( attach )( detach )
( createf )( updatef )( issuef )( transferf )( burnf )
( offerf )( cancelofferf )( claimf )
( attachf )( detachf )( openf )( closef )
( updatever )  ( createntt ) ( burnntt ) ( createnttlog ) ( claimntt ) ( updatentt ) )


//============================================================================================================
//=======================================- SimpleAssets.io -==================================================
//======================================- by CryptoLions.io -=================================================
//============================================================================================================