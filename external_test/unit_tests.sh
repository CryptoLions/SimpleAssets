#sudo apt-get install bc
#./unit_tests.sh simpleasset5 alexnag1tran PW5KUxSPhXY1YSH9risb7CVpTXzwH7DEVK84wQBugoy1uqBjo5Twm

echo "Test account where deployed SimpleAsset: " $1
echo "Account second to test transfer: " $2
echo "Key to wallet to unlock before test " $3

ACC=$1
ACC_TO=$2
SCOPE=$1
KEY_TO_WALLET=$3

CATEGORY=ctg
WAIT_TIME=2
#NAME_CURRENCY=$(cat /dev/urandom | tr -dc 'A-Z' | fold -w 5 | head -n 1)
NAME_CURRENCY=PWLQF
echo $NAME_CURRENCY

#---------------------------------------------------------------------------------------------------
# Unlock wallet workable
#---------------------------------------------------------------------------------------------------
function unlock_wallet
{
 	ERROR=$(./cleos.sh wallet unlock -n default --password $KEY_TO_WALLET 2>&1 >/dev/null)
	echo "ERROR: " ${ERROR}
}

#---------------------------------------------------------------------------------------------------
# clean up delegates table
#---------------------------------------------------------------------------------------------------
function CLEAN_DELEGATES_TABLE
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE delegates | jq -r '.rows | length')
	asset_ids=$(./cleos.sh get table $ACC $SCOPE delegates | jq -r '.rows[].assetid')
	owner_ids=$(./cleos.sh get table $ACC $SCOPE delegates | jq -r '.rows[].owner')
	delegatedto_ids=$(./cleos.sh get table $ACC $SCOPE delegates | jq -r '.rows[].delegatedto')

	if (( size > 0 ))
	  then
		echo "delegates table has $size data asset_ids: $asset_ids owner_ids: $owner_ids delegatedto_ids: $delegatedto_ids"
		for asset_id in ${asset_ids}; do
			for owner_id in ${owner_ids}; do
				for delegate_id in ${delegatedto_ids}; do
					echo "asset_id: " ${asset_id} "owner_id: " ${owner_id} "delegate_id: " ${delegate_id}
					./cleos.sh push action $ACC undelegate '{"owner":"'${owner_id}'", "from":"'${delegate_id}'", "assetids":['${asset_id}']}' -p $ACC
					echo "delegete [${asset_id}] is canceled"
				done
			done
		done
	  else
		echo "delegates table is empty"
	fi
}

#---------------------------------------------------------------------------------------------------
# clean up nttoffers table
#---------------------------------------------------------------------------------------------------
function CLEAN_NTTOFFERS_TABLE 
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE nttoffers | jq -r '.rows | length')
	nttoffer_ids=$(./cleos.sh get table $ACC $SCOPE nttoffers | jq -r '.rows[].assetid')

	if (( size > 0 ))
	  then
		echo "nttofferstable has $size data [$nttoffer_ids]"
		for id in ${nttoffer_ids}; do
			echo ${id}
			./cleos.sh push action $ACC cancelnttofr '["'${ACC}'", ["'${id}'"]]' -p $ACC
			echo "${id} is canceled"
		done
	  else
		echo "nttoffers table is empty"
	fi

	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE nttoffers | jq -r '.rows | length')

	sleep $WAIT_TIME
	if (( size > 0 ))
	  then

		size=$(./cleos.sh get table $ACC $SCOPE nttoffers | jq -r '.rows | length')

		if (( size > 0 ))
		  then
			echo "Error. nttoffers table is not fully cleaned. size: " $size 
			echo "offers_ids: " $(./cleos.sh get table $ACC $SCOPE  nttoffers | jq -r '.rows[].assetid')
			exit 1
		fi
	  fi
}


#---------------------------------------------------------------------------------------------------
# clean up offers table
#---------------------------------------------------------------------------------------------------
function CLEAN_OFFERS_TABLE 
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE offers | jq -r '.rows | length')
	offer_ids=$(./cleos.sh get table $ACC $SCOPE offers | jq -r '.rows[].assetid')

	if (( size > 0 ))
	  then
		echo "offers table has $size data [$offer_ids]"
		for id in ${offer_ids}; do
			echo ${id}
			./cleos.sh push action $ACC canceloffer '["'${ACC}'", ["'${id}'"]]' -p $ACC
			echo "${id} is canceled"
		done
	  else
		echo "offers table is empty"
	fi

	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE offers | jq -r '.rows | length')

	sleep $WAIT_TIME
	if (( size > 0 ))
	  then

		size=$(./cleos.sh get table $ACC $SCOPE offers | jq -r '.rows | length')

		if (( size > 0 ))
		  then
			echo "Error. offers table is not fully cleaned. size: " $size 
			echo "offers_ids: " $(./cleos.sh get table $ACC $SCOPE  offers | jq -r '.rows[].assetid')
			exit 1
		fi
	  fi
}

#---------------------------------------------------------------------------------------------------
# clean up snttassets table
#---------------------------------------------------------------------------------------------------
function CLEAN_NTTASSETS_TABLE
{
	local localacc=$1
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $localacc snttassets | jq -r '.rows | length')
	size=$(./cleos.sh get table $ACC $localacc snttassets | jq -r '.rows | length')
	nttasset_ids=$(./cleos.sh get table $ACC $localacc snttassets | jq -r '.rows[].id')

	if (( size > 0 ))
	  then
		echo "snttassets table has $size data [$nttasset_ids]"
		for id in ${nttasset_ids}; do
			echo ${id}
			./cleos.sh push action $ACC burnntt '["'${localacc}'", ["'${id}'"], "burntt"]' -p $localacc
			echo "nttasset: [${id}] is burned"
		done
	  else
		echo "Nothing to clean. snttasset table is empty"
	fi


	sleep $WAIT_TIME
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $localacc snttassets | jq -r '.rows | length')


	if (( size > 0 ))
	  then
		size=$(./cleos.sh get table $ACC $localacc snttassets | jq -r '.rows | length')

		if (( size > 0 ))
		  then
			echo "Error. snttasset table is not fully cleaned. size: " $size
			echo "asset_ids: " $(./cleos.sh get table $ACC $localacc snttassets | jq -r '.rows[].id')
			exit 1
		  fi

	  fi
}


#---------------------------------------------------------------------------------------------------
# clean up sassets table
#---------------------------------------------------------------------------------------------------
function CLEAN_ASSETS_TABLE
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows | length')
	size=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows | length')
	asset_ids=$(./cleos.sh get table $ACC $SCOPE  sassets | jq -r '.rows[].id')

	if (( size > 0 ))
	  then
		echo "sassets table has $size data [$asset_ids]"
		for id in ${asset_ids}; do
			echo ${id}
			./cleos.sh push action $ACC burn '["'${ACC}'", ["'${id}'"], "burn"]' -p $ACC
			echo "Asset: [${id}] is burned"
		done
	  else
		echo "Nothing to clean. sassets table is empty"
	fi


	sleep $WAIT_TIME
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows | length')


	if (( size > 0 ))
	  then
		size=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows | length')

		if (( size > 0 ))
		  then
			echo "Error. sasset table is not fully cleaned. size: " $size
			echo "asset_ids: " $(./cleos.sh get table $ACC $SCOPE  sassets | jq -r '.rows[].id')
			exit 1
		  fi

	  fi
}

#---------------------------------------------------------------------------------------------------
# clean up offerfs table
#---------------------------------------------------------------------------------------------------
function CLEAN_OFFERFS_TABLE 
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows | length')
	offerf_ids=$(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows[].id')

	if (( size > 0 ))
	  then
		echo "offerfs table has $size data [$offerf_ids]"
		for id in ${offerf_ids}; do
			echo ${id}
			./cleos.sh push action $ACC cancelofferf '["'${ACC}'", ["'${id}'"]]' -p $ACC
			echo "${id} is cancelled"
		done
	  else
		echo "offerfs table is empty"
	fi

	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows | length')


	if (( size > 0 ))
	  then
		size=$(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows | length')

		if (( size > 0 ))
		  then
			echo "Error. offerfs table is not fully cleaned. size: " $size 
			echo "offerf_ids: " $(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows[].id')
			exit 1
		  fi
	  fi
}

#---------------------------------------------------------------------------------------------------
# Fungible token accounts stats info
#---------------------------------------------------------------------------------------------------
function ft_account_stats_info
{
	sleep $WAIT_TIME
	account_item=$(./cleos.sh get table $ACC $SCOPE stat  -l 1000 | jq -r '.rows[] | select(.max_supply | contains("'${NAME_CURRENCY}'")) | .max_supply')

	if [ "$account_item" = "" ]
	then
		echo "Error. Currency was not created in stat table."
		exit 1;
	else
		echo "$NAME_CURRENCY was successfully added to stat table."
	fi
}

#---------------------------------------------------------------------------------------------------
# get balance
#---------------------------------------------------------------------------------------------------
function get_balance
{
	#./cleos.sh get table $ACC $SCOPE accounts -l 1000
	balance=$(./cleos.sh get table $ACC $SCOPE accounts -l 1000 | jq -r '.rows[] | select(.balance | contains("'${NAME_CURRENCY}'")) | .balance')
	balance=${balance//[[:alpha:]]} # trim name of currency which only character
	echo "balance: " $balance " " $NAME_CURRENCY
}

#---------------------------------------------------------------------------------------------------
# read one record from sassets table
#---------------------------------------------------------------------------------------------------
function READ_ONE_ASSET
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows | length')
	asset_ids=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows[].id')
	asset_author=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows[].author')

	if (( size > 1 ))
	  then
		size=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows | length')
		if (( size > 1 ))
	  	then
			echo "sassets table has more then one record. EXIT" ${size}
			exit 1;
		fi
	fi

	if (( size == 1 ))
	  then
		result_asset_id=$asset_ids
	  else
		echo "Error. sassets table is empty"
		./cleos.sh get table $ACC $SCOPE sassets
		exit 1
	fi
}

#---------------------------------------------------------------------------------------------------
# read one record from snttassets table
#---------------------------------------------------------------------------------------------------
function READ_ONE_NTTASSET
{
	local accscope=$1
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $accscope snttassets | jq -r '.rows | length')
	nttasset_ids=$(./cleos.sh get table $ACC $accscope snttassets | jq -r '.rows[].id')

	if (( size > 1 ))
	  then
		size=$(./cleos.sh get table $ACC $accscope snttassets | jq -r '.rows | length')
		if (( size > 1 ))
	  	then
			echo "snttassets table has more then one record. EXIT" ${size}
			exit 1;
		fi
	fi

	if (( size == 1 ))
	  then
		result_nttasset_id=$nttasset_ids
	  else
		echo "Error. snttassets table is empty"
		./cleos.sh get table $ACC $accscope snttassets
		exit 1
	fi
}


#---------------------------------------------------------------------------------------------------
# read one record from offer table
#---------------------------------------------------------------------------------------------------
function READ_ONE_OFFER
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE offers | jq -r '.rows | length')
	offer_ids=$(./cleos.sh get table $ACC $SCOPE offers | jq -r '.rows[].assetid')
	echo ${size}
	if (( size > 1 ))
	  then
		echo "offers table has more then one record. EXIT" ${size}
		exit 1;
	fi

	if (( size == 1 ))
	  then
		last_offer_result=${offer_ids}
		echo "offer table has one record: "$offer_ids
	  else
		echo "offer table is empty"
	fi
}


#---------------------------------------------------------------------------------------------------
# read one record from offerfs table
#---------------------------------------------------------------------------------------------------
function READ_ONE_OFFERF
{
	sleep $WAIT_TIME
	size=$(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows | length')
	offerf_ids=$(./cleos.sh get table $ACC $SCOPE offerfs | jq -r '.rows[].id')

	if (( size > 1 ))
	  then
		echo "offerfs table has more then one record. EXIT" ${size}
		exit 1;
	fi

	if (( size == 1 ))
	  then
		last_offerfs_result=${offerf_ids}
		echo "offerfs table has one record: " $last_offerfs_result
	  else
		echo "offerfs table is empty"
		exit 1
	fi
}

function PUSH_CREATE_NTTASSET
{
	local requireclaim=$1
	local owner=$2

	sleep $WAIT_TIME
	echo "create nttasset action"

	output=$(./cleos.sh push action ${ACC} createntt '{ "author":"'${ACC}'", "category":"'${CATEGORY}'", "owner":"'${owner}'" ,"idata":"idata","mdata":"mdata" , "requireclaim":'${requireclaim}'}' -p $ACC@active 2>&1)

	if [[ $output =~ "${ACC} <= ${ACC}" ]]
	then
	   echo "Success"
	else
	   echo "Error"
	   echo "Output: " $output
	   exit 1
	fi
}

function PUSH_CREATE_ASSET
{
	sleep $WAIT_TIME
	echo "create asset action"

	output=$(./cleos.sh push action ${ACC} create '{ "author":"'${ACC}'", "category":"'${CATEGORY}'", "owner":"'${ACC}'" ,"idata":"idata","mdata":"mdata" , "requireclaim":0}' -p $ACC@active 2>&1)

	if [[ $output =~ "${ACC} <= ${ACC}" ]]
	then
	   echo "Success"
	else
	   echo "Error"
	   echo "Output: " $output
	   exit 1
	fi
}

function TEST_CREATE_UPDATE_TRANSFER
{
	echo "----------------------------------- TEST_CREATE_UPDATE_TRANSFER ----------------------------------------------------------------"
	new_mdata="test_test_2345"

	# ----------------------------------- create test ----------------------------------------------------------------

	CLEAN_OFFERS_TABLE
	CLEAN_ASSETS_TABLE
	PUSH_CREATE_ASSET

	READ_ONE_ASSET
	asset_id=$result_asset_id

	# ----------------------------------- update test ----------------------------------------------------------------

	sleep $WAIT_TIME
	echo "update"
	./cleos.sh push action ${ACC} update '{ "author":"'${ACC}'", "owner":"'${ACC}'" , "assetid":'${asset_id}', "mdata":"'${new_mdata}'" }' -p $ACC@active

	sleep $WAIT_TIME
	mdata_updated=$(./cleos.sh get table $ACC $SCOPE sassets -l 1000 | jq -r '.rows[] | select(.id | contains("'${asset_id}'")) | .mdata')

	if [ "$mdata_updated" = "$new_mdata" ]
	then
		echo "Success. mdata was updated"
	else
		echo "new_mdata: " $new_mdata
		echo "mdata_updated: " $mdata_updated
		echo "Error. Update action failed. mdata values is $mdata_updated and it was not changed to $new_mdata"
		exit 1
	fi

	# ----------------------------------- transfer test ----------------------------------------------------------------
	sleep $WAIT_TIME
	echo "transfer"
	./cleos.sh push action ${ACC} transfer '{ "from":"'${ACC}'", "to":"'${ACC_TO}'" , "assetids":['$asset_id'], "memo":"memo" }' -p $ACC@active

	sleep $WAIT_TIME
	sleep $WAIT_TIME
	asset_from_account=$(./cleos.sh get table $ACC $ACC sassets  -l 1000 | jq -r '.rows[] | select(.id | contains("'${asset_id}'")) | .id')

	sleep $WAIT_TIME
	sleep $WAIT_TIME
	asset_to_account=$(./cleos.sh get table $ACC $ACC_TO sassets  -l 1000 | jq -r '.rows[] | select(.id | contains("'${asset_id}'")) | .id')


	if [ "$asset_id" = "$asset_to_account" ]
	then
		if [ "${asset_from_account}" = "" ]
		then
			echo "Success. Asset '${asset_id}' was successfully transfered from '${ACC}' to '${ACC_TO}'"
		else
			echo "asset_id: " ${asset_id}
			echo "asset_to_account: " ${asset_to_account}
			echo "asset_from_account: " ${asset_from_account}
			echo "Error. Asset was not completly transfered. "
			exit 1
		fi
	else
		echo "asset_id: " ${asset_id}
		echo "asset_to_account: " ${asset_to_account}
		echo "asset_from_account: " ${asset_from_account}

		echo "Error. Asset '${asset_id}' was not transfered from '${ACC}' to '${ACC_TO}'"
		exit 1
	fi

	CLEAN_ASSETS_TABLE
}

function get_period
{
	sleep $WAIT_TIME
	period=$(./cleos.sh get table $ACC $SCOPE delegates -l 1000 | jq -r '.rows[] | select(.assetid| contains("'$1'")) | .period')
	return $period
}

function is_delegate_removed
{
	datatest_item=$(./cleos.sh get table $ACC $SCOPE delegates -l 1000 | jq -r '.rows[] | select(.assetid| contains("'$1'")) | .assetid')

	if [ "$1" = "$datatest_item" ]
	then
		echo "Delegate $1 was not removed"
		echo "last_asset_result: " $1
		echo "datatest_item: " $datatest_item
		./cleos.sh get table $ACC $SCOPE delegates
		exit 1
	else
		echo "Delegate was successfully removed"
	fi
}

function is_delegate_inserted
{
	datatest_item=$(./cleos.sh get table $ACC $SCOPE delegates -l 1000 | jq -r '.rows[] | select(.assetid| contains("'$1'")) | .assetid')

	if [ "$1" = "$datatest_item" ]
	then
		echo "Delegate $1 was successfully inserted"
	else
		echo "last_asset_result: " $1
		echo "datatest_item: " $datatest_item
		exit 1
	fi
}

function TEST_UNDELEGATE_DELEGATE_DELEGATEMORE
{
	echo "----------------------------------- TEST_UNDELEGATE_DELEGATE_DELEGATEMORE ----------------------------------------------------------------"

	period1=1
	period2=1
	period12=$(bc <<< "$period1+$period2")

	CLEAN_ASSETS_TABLE
	CLEAN_DELEGATES_TABLE
	PUSH_CREATE_ASSET

	READ_ONE_ASSET
	asset_id=$result_asset_id

	PUSH_DELEGATE $asset_id ${period1} "test"

	#sleep $WAIT_TIME
	#echo "delegate"
	#./cleos.sh push action $ACC delegate '{"owner":"'${ACC}'", "to":"'${ACC_TO}'", "assetids":['${asset_id}'], "period":'${period1}', "memo":"test"}' -p $ACC

	# ----------------------------------------------- delegatemore testing ---------------------------------------------------------------------------------
	get_period $asset_id
	period_init=$?

	sleep $WAIT_TIME
	echo "delegatemore"
	./cleos.sh push action $ACC delegatemore '{"owner":"'${ACC}'", "assetidc":'${asset_id}', "period":'${period2}'}' -p $ACC

	get_period $asset_id
	period_after_delegatemore=$?

	if (( period12 = period_after_delegatemore ))
	then
		echo "Success. Period updated."
	else
		echo "Error. Period was not updated"
		echo "period_after_delegatemore: " $period_after_delegatemore 
		echo "get_period return: " $period_init
		echo "period12: " $period12
		./cleos.sh get table $ACC $SCOPE delegates -l 1000
		exit 1
	fi

	sleep $WAIT_TIME

	is_delegate_inserted $asset_id

	sleep $WAIT_TIME
	sleep $WAIT_TIME

	#---------------------------------------------- undelegate ----------------------------------------------------------------------------------

	echo "undelegate"
	./cleos.sh push action $ACC undelegate '{"owner":"'${ACC}'", "from":"'${ACC_TO}'", "assetids":['${asset_id}']}' -p $ACC

	is_delegate_removed $asset_id

	CLEAN_DELEGATES_TABLE
	CLEAN_ASSETS_TABLE
}

function TEST_OFFER
{
	echo "----------------------------------- TEST_OFFER ----------------------------------------------------------------"
	CLEAN_OFFERS_TABLE
	CLEAN_ASSETS_TABLE

	PUSH_CREATE_ASSET

	READ_ONE_ASSET
	asset_id=$result_asset_id

	sleep $WAIT_TIME
	echo "offer"
	./cleos.sh push action ${ACC} offer '{"owner":"'${ACC}'", "newowner":"'${ACC_TO}'", "assetids":['$asset_id'], "memo":"for test"}' -p $ACC

	READ_ONE_OFFER

	#CLEAN_OFFERS_TABLE
	#CLEAN_ASSETS_TABLE
}

function TEST_OFFERFS
{
	echo "----------------------------------- TEST_OFFERFS ----------------------------------------------------------------"
	CLEAN_OFFERFS_TABLE

	sleep $WAIT_TIME
	echo "offerf"
	./cleos.sh push action ${ACC} offerf '{"owner":"'${ACC}'", "newowner":"'${ACC_TO}'", "author":"'${ACC}'", "quantity":"5.20 '${NAME_CURRENCY}'", "memo":"for test"}' -p $ACC

	READ_ONE_OFFERF

	sleep $WAIT_TIME
	echo "cancelofferf"
	./cleos.sh push action ${ACC} cancelofferf '{"owner":"'${ACC}'", "ftofferids":['${last_offerfs_result}']}' -p $ACC

	CLEAN_OFFERFS_TABLE
}

function diff_balance
{
	sleep $WAIT_TIME
	local balance_init=$1
	local balance=$2
	local value=$3

	balance_diff=$(bc <<< "$balance-$balance_init")

	if (( $(echo "$balance_diff == $value" | bc -l) )); then
		echo "Balance was successfully updated"
	else
		echo "Error. Balance was not updated !!!"
		echo "balance_init: " $balance_init
		echo "balance: " $balance
		echo "value: " $value
		echo "balance_diff: " $balance_diff
		exit 1
	fi
}

function PUSH_ISSUEF
{
	sleep $WAIT_TIME
	./cleos.sh push action ${ACC} issuef '{"to":"'${ACC}'", "author":"'${ACC}'", "quantity":"'$1' '${NAME_CURRENCY}'", "memo":"test"}' -p $ACC
}


function TEST_CREATEF_ISSUEF_TRANSFERF_BURNF
{
	echo "----------------------------------- TEST_CREATEF_ISSUEF_TRANSFERF_BURNF ----------------------------------------------------------------"

	value=10.00
	#sleep $WAIT_TIME
	echo "createf"
	./cleos.sh push action ${ACC} createf '{"supply": "'${value}' '${NAME_CURRENCY}'", "author":"'${ACC}'", "maximum_supply":"1200000.00 '${NAME_CURRENCY}'", "authorctrl":0, "data":"dfd"}' -p $ACC@active
	ft_account_stats_info

	# ----------------------------------- issuef test ----------------------------------------------------------------
	sleep $WAIT_TIME
	get_balance
	balance_init=$balance
	echo "Inintial balance before issuef: "$balance_init

	PUSH_ISSUEF $value

	sleep $WAIT_TIME
	get_balance
	diff_balance $balance_init $balance $value

	# ----------------------------------- transferf test ----------------------------------------------------------------
	sleep $WAIT_TIME
	get_balance
	balance_init=$balance

	sleep $WAIT_TIME
	echo "transferf"

	value=5.20

	./cleos.sh push action ${ACC} transferf '{"from":"'${ACC}'", "to":"'${ACC_TO}'", "author":"'${ACC}'", "quantity":"'${value}' '${NAME_CURRENCY}'", "memo":"for test"}' -p $ACC

	sleep $WAIT_TIME
	get_balance
	diff_balance $balance_init $balance "-"$value

	# ----------------------------------- burnf test ----------------------------------------------------------------

	sleep $WAIT_TIME
	get_balance
	balance_init=$balance
	echo "Inintial balance before burnf: "$balance_init

	sleep $WAIT_TIME
	echo "burnf"

	value=1.00
	./cleos.sh push action $ACC burnf '["'${ACC}'", "'${ACC}'" "'${value}' '${NAME_CURRENCY}'", "test"]' -p $ACC@active

	sleep $WAIT_TIME
	get_balance
	diff_balance $balance_init $balance "-"$value
}


function TEST_AUTHORUPDATE
{
	echo "----------------------------------- TEST_AUTHORUPDATE ----------------------------------------------------------------"

	sleep $WAIT_TIME
	echo "authorupdate"
	dappinfotest="data_test_test"
	fieldtypes="fieldtypes_test_test"

	./cleos.sh push action $ACC authorupdate '{"author":"'${ACC}'", "dappinfo":"'${dappinfotest}'", "fieldtypes":"'${fieldtypes}'"}' -p $ACC

	sleep $WAIT_TIME
	dappinfotest_item=$(./cleos.sh get table $ACC $SCOPE authors -l 1000 | jq -r '.rows[] | select(.dappinfo | contains("'${dappinfo}'")) | .dappinfo')

	sleep $WAIT_TIME
	fieldtypes_item=$(./cleos.sh get table $ACC $SCOPE authors -l 1000 | jq -r '.rows[] | select(.fieldtypes | contains("'${fieldtypes}'")) | .fieldtypes')

	if [ "$dappinfotest_item" = "" ]
	then
		echo $dappinfotest_item
		echo $fieldtypes_item
		echo "Error. authors.dappinfo field was not updated during authorupdate."
		exit 1;
	else
		echo "dappinfo was successfully updated during authorupdate."
	fi

	if [ "$fieldtypes_item" = "" ]
	then
		echo $dappinfotest_item
		echo $fieldtypes_item
		echo "Error. authors.fieldtypes field was not updated during authorupdate."
		exit 1;
	else
		echo "fieldtypes was successfully updated during authorupdate."
	fi
}

function CHECK_ATTACHED_ASSET
{
	echo "----------------------------------- CHECK_ATTACHED_ASSET ----------------------------------------------------------------"
	local asset1=$1
	local asset2=$2
	local accto=$3

	sleep $WAIT_TIME
	container_id=$(./cleos.sh get table $ACC ${accto} sassets -l 1000 | jq -r '.rows[] | select(.id | contains("'${asset1}'")) | .container[].id')

	echo "container_id: " $container_id

	#sleep $WAIT_TIME
	#size=$(./cleos.sh get table $ACC ${accto} sassets -l 1000 | jq -r '.rows | length')

	if [ container_id = "" ]
	then
		echo "size: " $size
		echo "container_id: " $container_id
		echo "Error during attach asset [${asset2}] to asset [${asset1}]"
		exit 1
	else
		echo "Asset [${asset2}] was successfully attached to [${asset1}]"
	fi
}

function TEST_ATTACH_DETACH
{
	echo "----------------------------------- TEST_ATTACH_DETACH ----------------------------------------------------------------"
	CLEAN_ASSETS_TABLE
	PUSH_CREATE_ASSET
	PUSH_CREATE_ASSET

	sleep $WAIT_TIME
	asset_ids=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows[].id')

	array=($(echo "$asset_ids" | tr ' ' '\n'))

	sleep $WAIT_TIME
	./cleos.sh push action $ACC attach '["'$ACC'", '${array[0]}', ['${array[1]}']]' -p $ACC

	CHECK_ATTACHED_ASSET ${array[0]} ${array[1]} ${SCOPE}

	sleep $WAIT_TIME
	./cleos.sh push action $ACC detach '["'$ACC'", '${array[0]}', ['${array[1]}']]' -p $ACC

	sleep $WAIT_TIME

	size=$(./cleos.sh get table $ACC $SCOPE sassets -l 1000 | jq -r '.rows | length')
	echo "size: " $size

	if (( size = 2 ))
	then
		echo "Asset [${array[1]}] was successfully detached from [${array[0]}]"
	else
		echo "Error during detaching asset [${array[1]}] from asset [${array[0]}]"
		exit 1
	fi

	CLEAN_ASSETS_TABLE
}

function TEST_UPDATEF
{
	echo "----------------------------------- TEST_UPDATEF ----------------------------------------------------------------"
	data_text="test_test_test"

	sleep $WAIT_TIME
	data_old=$(./cleos.sh get table $ACC $SCOPE stat -l 1000 | jq -r '.rows[] | select(.max_supply | contains("'${NAME_CURRENCY}'")) | .data')

	sleep $WAIT_TIME
	./cleos.sh push action $ACC updatef '{"author":"'${ACC}'", "sym":"1,'${NAME_CURRENCY}'" ,"data":"'${data_text}'"}' -p $ACC

	sleep $WAIT_TIME
	data_new=$(./cleos.sh get table $ACC $SCOPE stat -l 1000 | jq -r '.rows[] | select(.max_supply | contains("'${NAME_CURRENCY}'")) | .data')

	if [ "$data_new" = "$data_text" ]
	then
		echo "Update command successfully : $data_updated"
	else
		echo "Error during update action. data = $data_new but should be data = $data_text"
		exit 1
	fi
}

function TEST_UPDATEVER
{
	echo "----------------------------------- TEST_UPDATEVER ----------------------------------------------------------------"
	sleep $WAIT_TIME
	./cleos.sh push action $ACC updatever '{ "version": "'$1'"}' -p $ACC@active

	sleep $WAIT_TIME
	data_new=$(./cleos.sh get table $ACC $SCOPE tokenconfigs | jq -r '.rows[] | select(.version | contains("'$1'")) | .version')

	if [ "$data_new" = "$1" ]
	then
		echo "updatever was successfully update version to $data_new"
	else
		echo "Error. updatever was not update version to $1. version = $data_new"
		exit 1
	fi
}

function TEST_ATTACHF_DETACHF
{
	echo "----------------------------------- TEST_ATTACHF_DETACHF ----------------------------------------------------------------"
	CLEAN_ASSETS_TABLE
	PUSH_CREATE_ASSET

	# ----------------------------------- save balance before attachf ------------------------------------------------
	sleep $WAIT_TIME
	get_balance
	balance_init=$balance
	# ------------------------------------------------------------------------------------------------------------------

	READ_ONE_ASSET
	asset_id=$result_asset_id

	value=1.00
	quantity="${value} ${NAME_CURRENCY}"

	echo "attachf"
	sleep $WAIT_TIME
	./cleos.sh push action $ACC attachf '{"owner":"'${ACC}'", "author":"'${ACC}'", "quantity":"'${value}' '${NAME_CURRENCY}'", "assetidc":'${asset_id}'}' -p $ACC

	# ----------------------------------- check balance after attachf --------------------------------------------------
	sleep $WAIT_TIME
	get_balance
	balance_after_attachf=$balance

	diff_balance $balance_init $balance "-"$value

	inserted_sasset_balance=$(CHECK_ASSET_CONTAINERF_FUNGIBLE_EMPTY "$asset_id")

	if [ "$quantity" = "$inserted_sasset_balance" ]
	then
		echo "Success. Attachf is attached item"
	else
		echo "balance after attachf: "$balance_after_attachf
		echo "inserted_sasset_balance: " $inserted_sasset_balance
		echo "Error. Attachf is not attached item"
		exit 1
	fi

	# ------------------------------------------ detachf ---------------------------------------------------------------

	./cleos.sh push action $ACC detachf '{"owner":"'${ACC}'", "author":"'${ACC}'", "quantity":"'${value}' '${NAME_CURRENCY}'", "assetidc":'${asset_id}'}' -p $ACC

	# ----------------------------------- check balance after detachf---------------------------------------------------
	sleep $WAIT_TIME
	get_balance
	balance_after_detachf=$balance
	diff_balance $balance_init $balance 0

	# ----------------------------------- check is in sasset empty containerf field--------------------------------------
	val_after=$(CHECK_ASSET_CONTAINERF_FUNGIBLE_EMPTY "$asset_id")
	
	if [ "$val_after" = "" ]
	then
		echo "Success. ${quantity} is successfully detached from '${asset_id}'"
	else
		echo "balance after attachf: "$balance_after_attachf
		echo "inserted_sasset_balance: " $inserted_sasset_balance
		echo "balance after attachf: "$balance_after_detachf
		echo "Error. ${quantity} is not detached from '${asset_id}'"
		exit 1
	fi
	# ------------------------------------------------------------------------------------------------------------------
}

function CHECK_ASSET_CONTAINERF_FUNGIBLE_EMPTY
{
	asset=$1
	sleep $WAIT_TIME
	balancef=$(./cleos.sh get table $ACC $SCOPE sassets -l 1000 | jq -r '.rows[] | select(.id | contains("'${asset}'")).containerf[].balance')
	
	if [ "$balancef" = "" ]
	then
		echo ""
	else
		echo $balancef
	fi
}

function TEST_END
{
	echo "----------------------------------- TEST DONE ----------------------------------------------------------------"
}

function TEST_DELEGATE_ATTACH_DETACH_UNDELEGATE
{
	CLEAN_ASSETS_TABLE
	CLEAN_DELEGATES_TABLE

	PUSH_CREATE_ASSET
	PUSH_CREATE_ASSET

	sleep $WAIT_TIME
	asset_ids=$(./cleos.sh get table $ACC $SCOPE sassets | jq -r '.rows[].id')

	array=($(echo "$asset_ids" | tr ' ' '\n'))

	sleep $WAIT_TIME
	./cleos.sh push action $ACC attach '["'$ACC'", '${array[0]}', ['${array[1]}']]' -p $ACC

	CHECK_ATTACHED_ASSET ${array[0]} ${array[1]} ${SCOPE}

	READ_ONE_ASSET
	asset_id=$result_asset_id
	
	PUSH_DELEGATE $result_asset_id 1 "test"

	CHECK_ATTACHED_ASSET ${array[0]} ${array[1]} ${ACC_TO}

	sleep $WAIT_TIME
	echo "detach"
	output=$(./cleos.sh push action $ACC detach '["'${ACC_TO}'", '${array[0]}', ['${array[1]}']]' -p $ACC_TO@active 2>&1 >/dev/null)

	if [[ $output =~ "Cannot detach from delegated. assetidc is delegated." ]]
	then
	   echo "Success. Detach is not allowed to delegated asset"
	else
	   echo "Error. Asset detached from delegated asset. Not allowed"
	   exit 1
	fi

	CLEAN_ASSETS_TABLE
}

function PUSH_DELEGATE {
	local asset_id=$1
	local period=$2
	local memo=$3

	sleep $WAIT_TIME
	./cleos.sh push action $ACC delegate '{"owner":"'${ACC}'", "to":"'${ACC_TO}'", "assetids":['${asset_id}'], "period":'${period}', "memo":"'${memo}'"}' -p $ACC
}


function CHECK_SUCCESS_EXECUTED_TRANSACTION
{
	local par1=$1
        echo "Output1: " $par1
	if [[ $1 =~ "${ACC} <= ${ACC}" ]]
	then
	   echo "Success."
	else
	   echo "Error"
	   echo "Output: " $1
	   exit 1
	fi
}

#------------------------------------------------------------------------------------------------------------------------------
# 2019-08-09 This test created after adding MEMO filed to SDELEGATE table
#------------------------------------------------------------------------------------------------------------------------------
function TEST_DELEGATE_MEMO_FIELD {

	echo "----------------------------------- TEST_DELEGATE_MEMO_FIELD ----------------------------------------------------------------"

	CLEAN_ASSETS_TABLE
	CLEAN_DELEGATES_TABLE

	PUSH_CREATE_ASSET

	READ_ONE_ASSET
	
	output=$(PUSH_DELEGATE $result_asset_id 1 "64_symbols_are_maximum_for_such_type_of_string_This_string1234567" 2>&1)

	if [[ $output =~ "Error. Size of memo cannot be bigger 64" ]]
	then
	   echo "Success. Memo should not be allowed be bigger then 64 symbols"
	else
	   echo "Error. Memo should not be allowed be bigger then 64 symbols. Not allowed!"
	   echo "Output: " $output
	   exit 1
	fi

	# executed transaction:
	CLEAN_DELEGATES_TABLE
	CLEAN_ASSETS_TABLE

	PUSH_CREATE_ASSET

	READ_ONE_ASSET
	echo "delegate"
	output=$(PUSH_DELEGATE $result_asset_id 1 "64_symbols_are_maximum_for_such_type_of_string_This_string123456" 2>&1)

	if [[ $output =~ "${ACC} <= ${ACC}" ]]
	then
	   echo "Success"
	else
	   echo "Error"
	   echo "Output: " $output
	   exit 1
	fi

	# executed transaction:
	CLEAN_DELEGATES_TABLE
	CLEAN_ASSETS_TABLE
}


function TEST_CREATENTT_CLAIMNTT_BURNNTT
{
	# createntt
	echo "----------------------------------- TEST_CREATENTT ----------------------------------------------------------------"
	new_mdata="test_test_2345"
	
	# ----------------------------------- create test ----------------------------------------------------------------
	
	CLEAN_NTTOFFERS_TABLE
	CLEAN_NTTASSETS_TABLE ${ACC}
	CLEAN_NTTASSETS_TABLE ${ACC_TO}

	PUSH_CREATE_NTTASSET 0 ${ACC}

	READ_ONE_NTTASSET ${ACC}
	nttasset_id=$result_nttasset_id

	# ----------------------------------- update test ----------------------------------------------------------------

	sleep $WAIT_TIME
	echo "update"
	./cleos.sh push action ${ACC} updatentt '{ "author":"'${ACC}'", "owner":"'${ACC}'" , "assetid":'${nttasset_id}', "mdata":"'${new_mdata}'" }' -p $ACC@active

	sleep $WAIT_TIME
	mdata_updated=$(./cleos.sh get table $ACC $SCOPE snttassets -l 1000 | jq -r '.rows[] | select(.id | contains("'${nttasset_id}'")) | .mdata')

	if [ "$mdata_updated" = "$new_mdata" ]
	then
		echo "Success. mdata was updated"
	else
		echo "new_mdata: " $new_mdata
		echo "mdata_updated: " $mdata_updated
		echo "Error. Update action failed. mdata values is $mdata_updated and it was not changed to $new_mdata"
		exit 1
	fi

	CLEAN_NTTASSETS_TABLE ${ACC}

	PUSH_CREATE_NTTASSET 1 ${ACC_TO}

	READ_ONE_NTTASSET ${ACC}
	nttasset_id=$result_nttasset_id
	
	sleep $WAIT_TIME
	
	size=$(./cleos.sh get table $ACC $SCOPE nttoffers | jq -r '.rows | length')
	nttoffer_ids=$(./cleos.sh get table $ACC $SCOPE nttoffers | jq -r '.rows[].assetid')

	if [ "$nttasset_id" = "$nttoffer_ids" ]
	then
		echo "Success. Offer successfully added"
	else
		echo "nttasset_id: " $nttasset_id
		echo "nttoffer_ids: " $nttoffer_ids
		echo "Error. Offer was not added."
		exit 1
	fi

	./cleos.sh push action ${ACC} claimntt '{ "claimer":"'${ACC_TO}'", "assetids":["'${nttasset_id}'"]  }' -p $ACC_TO@active

	READ_ONE_NTTASSET ${ACC_TO}
	nttasset_id=$result_nttasset_id

	if [ "$nttasset_id" = "$nttoffer_ids" ]
	then
		echo "Success. Claim successfully passed"
	else
		echo "nttasset_id: " $nttasset_id
		echo "nttoffer_ids: " $nttoffer_ids
		echo "Error. Offer was not added."
		exit 1
	fi

	CLEAN_NTTASSETS_TABLE ${ACC_TO}
}

function PUSH_CHANGEAUTHOR
{
	sleep $WAIT_TIME
	echo "changeauthor action"

	output=$(./cleos.sh push action ${ACC} changeauthor '{ "author":"'${ACC}'", "newauthor":"'${ACC_TO}'", "owner":"'${ACC}'" ,"assetids":['${result_asset_id}'],"memo":"mdata"}' -p $ACC@active 2>&1)

	if [[ $output =~ "${ACC} <= ${ACC}" ]]
	then
	   echo "Success"
	else
	   echo "Error"
	   echo "Output: " $output
	   exit 1
	fi
}

function TEST_CHANGEAUTHOR
{
	echo "----------------------------------- TEST_CHANGEAUTHOR ----------------------------------------------------------------"
	CLEAN_ASSETS_TABLE

	PUSH_CREATE_ASSET

	READ_ONE_ASSET

	PUSH_CHANGEAUTHOR

	READ_ONE_ASSET

	if [ "$asset_author" = "$ACC_TO" ]
	then
		echo "Success. changeauthor ACTION test was successfully passed"
	else
		echo "asset_author: " $asset_author
		echo "Error. Author was not changed"
		exit 1
	fi
}

unlock_wallet

# 2019-11-01 This test created after adding changeauthor action
TEST_CHANGEAUTHOR

# 2019-10-02 This test created after adding createntt claimntt burnntt actions
TEST_CREATENTT_CLAIMNTT_BURNNTT

TEST_UNDELEGATE_DELEGATE_DELEGATEMORE

TEST_AUTHORUPDATE

TEST_OFFER

TEST_OFFERFS

TEST_CREATEF_ISSUEF_TRANSFERF_BURNF

TEST_ATTACH_DETACH

TEST_ATTACHF_DETACHF

TEST_UPDATEF

TEST_UPDATEVER "666"

TEST_UPDATEVER "777"

TEST_CREATE_UPDATE_TRANSFER

TEST_UNDELEGATE_DELEGATE_DELEGATEMORE

# Test for Detach from Delegated Asset
TEST_DELEGATE_ATTACH_DETACH_UNDELEGATE

# 2019-08-09 This test created after adding MEMO filed to SDELEGATE table
TEST_DELEGATE_MEMO_FIELD

TEST_END

