# 한국어  

# 심플에셋 (SimpleAssets)  

EOSIO 블록체인 상의 (대체 가능 코인과 불가능 코인—NFT를 모두 포함한) 디지털 자산을 위해, [Cryptolions](https://cryptolions.io/)가 고안해 낸 간편 표준  

Web: [http://simpleassets.io](http://simpleassets.io)  
Git: [https://github.com/CryptoLions/SimpleAssets](https://github.com/CryptoLions/SimpleAssets)  
Telegram: [https://t.me/simpleassets](https://t.me/simpleassets)  


소개 & 데모: [https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4](https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4)  

작성자들을 위한 이벤트 리시버 예시: [https://github.com/CryptoLions/SimpleAssets-EventReceiverExample](https://github.com/CryptoLions/SimpleAssets-EventReceiverExample)  

주의!!! 현재 CDT는 v1.6.x에서는 컴파일이 되지 않는 버그가 있습니다. v1.5.0 역시 abi 생성 시 "세그멘테이션 오류 (core dumped)" 버그가 발생합니다. 컨트랙트 컴파일 시 v1.5.0와 저희가 제공하는 abi를 사용하실 것을 추천합니다.  

이슈: https://github.com/EOSIO/eosio.cdt/issues/527  

---

심플에셋은 심플에셋 컨트렉트와 연결하여 사용합니다. 한마디로, Dapp을 위한 Dapp이라고 할 수 있겠습니다. 

정글 테스트넷: `simpleassets`  
EOS 메인넷: `simpleassets`  
MEETONE 메인넷: `smplassets.m`  
TELOS 메인넷: `simpleassets` 
PROTON: `simpleassets`
  
심플에셋은 다른 Dapp들이 자신의 디지털 자산을 관리하기 위해 호출할 수 있는 별개의 컨트랙트입니다. 이는 자산의 소유권이 신뢰할 수 있는 외부 권한자에 의해 관리될 수 있도록, 그리고 생성과 동시에 해당 Dapp이 자산의 mdata만을 관리할 수 있도록 하는 추가적인 보장책을 해당 Dapp 사용자들에게 제공합니다. 소유권과 관련한 모든 기능들은 외부에 존재하게 됩니다.  
  
현재 우리는, EOSIO 메인넷에 배포된 심플에셋의 업데이트를 큐레이팅할 DAC를 만드는 과정에 있습니다.  
  
관련 정보: 소유권 권한자 이해하기.  
  
자신의 NFT 관련 정보를 제 3자 시장에 게재하려면, regauthor 액션을 사용하세요.  
  
또는, 각 Dapp은 자신만의 심플에셋 사본을 배포할 수 있으며, 각 기능에 대해 더 큰 통제권을 가질 수 있도록 수정을 적용할 수 있습니다. 배포에 앞서, 심플에셋은 누군가가 자산을 생성하는 것을 막도록 수정되어야 합니다.  
---------------------------
  
## RAM 사용량  
  
NFT의 RAM 사용량은 idata와 mdata 필드에 얼마나 많은 데이터가 있는지에 의해 정해집니다. 두 영역 모두가 비어있다면, 각 NFT는 `276byte`를 차지합니다.  
  
idata와 mdata의 각 기호들은 1 byte 씩을 차지합니다.  
  
---------------------------
  
## 목차:  
1. [컨트랙트  액션](#컨트랙트-액션)  
2. [데이터 구조](#데이터-구조)  
3. [예시: 스마트  컨트랙트에서  심플에셋  사용하기](#예시:-스마트-컨트랙트에서-심플에셋-사용하기)  
4. [변경 로그](#변경-로그)  
  
---------------------------

# 컨트랙트  액션

각  파라미터에  대한  설명은  다음  링크에서  찾아볼  수  있습니다.
[https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp](https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp)

```
authorreg		( name author, string dappinfo, string fieldtypes, string priorityimg )
authorupdate		( name author, string dappinfo, string fieldtypes, string priorityimg )

# -- For Non-Fungible Tokens ---

create			(author, category, owner, idata, mdata, requireсlaim)
update			(author, owner, assetid, mdata)
transfer		(from, to , [assetid1,..,assetidn], memo)
burn			(owner, [assetid1,..,assetidn], memo)

offer			(owner, newowner, [assetid1,..,assetidn], memo)
canceloffer		(owner, [assetid1,..,assetidn])
claim			(claimer, [assetid1,..,assetidn])

regauthor		(name author, data, stemplate)
authorupdate		(author, data, stemplate)

delegate  		(owner, to, [assetid1,..,assetidn], period, memo)
undelegate  	        (owner, from, [assetid1,..,assetidn])
delegatemore		(owner, assetid, period)  

attach  		(owner, assetidc, [assetid1,..,assetidn])
detach  		(owner, assetidc, [assetid1,..,assetidn])

attachf  		(owner, author, quantity, assetidc)
detachf  		(owner, author, quantity, assetidc)

mdadd			(author, data)
mdupdate		(id, author, data) 
mdremove		(id)
mdaddlog		(id, author, data)

# -- For Fungible Tokens ---

createf  		(author, maximum_supply, authorctrl, data)
updatef 		(author, sym, data)
issuef  		(to, author, quantity, memo)
transferf 		(from, to, author, quantity, memo)
burnf 			(from, author, quantity, memo)
  
offerf 			(owner, newowner, author, quantity, memo)
cancelofferf 		(owner, [ftofferid1,...,ftofferidn])
claimf 			(claimer, [ftofferid1,...,ftofferidn])
 
openf  			(owner, author, symbol, ram_payer)
closef  		(owner, author, symbol)
```

 # -- For Non-Transferable Tokens (NTTs) ---

createntt		(author, category, owner, idata, mdata, requireсlaim)  
updatentt		(author, owner, assetid, mdata)  
burnntt		(owner, [assetid1,..,assetidn], memo)  
claimntt		(claimer, [assetid1,..,assetidn]) 

# 데이터  구조

## 자산

```
sasset {  
	uint64_t	id; 		// 양도와 검색에 이용되는 자산 ID;
	name		owner;  	// 자산 소유자 (소유자에 의해 변경 가능!!!);
	name		author;		// 자산 생성자 (게임 컨트랙트, 변경 불가);  
	name		category;	// 자산 종류, 생성자가 선택, 변경 불가;  
	string		idata;		// 변경 불가 자산 데이터. JSON이나 sha256로 스트링화 가능;
	string		mdata;		// 변경 가능 자산, 생성 시나 자산 업데이트 시 생성자에 의해 추가됨
					// JSON이나 sha256로 스트링화 가능;  
	sasset[]	container;	// 이 자산에 첨부된 다른 NFT
	account[]	containerf;	// 이 자산에 첨부된 FT
}  
```

// Markets에서 사용될 자산 이름 img desc에 대한 idata 혹은 mdata 정보를 포함시켜 주세요.

## 오퍼

```
offers {  
	uint64_t	assetid;	// 클레임을 위해 오퍼된 자산 ID; 
	name		owner;		// 자산 소유자;  
	name		offeredto;	// 이 자산을 클레임할 수 있는 사람 ; 
	uint64_t	cdate;		// 오퍼 생성일;  
}  
```

## 작성자

```
authors {  
	name		author;		// 자산을 생성하고 업데이트할 수 있는 자산 샌성자;  
	string		data;		// 개선된 디스플레이를 위해 마켓에서 사용되는 생성자의 데이터(json);
					// 추천: logo, info, url;  
	string		stemplate;	// 제 3자 시장을 위한 데이터(json) 스키마;
					// 키: mdata, idata의 키의 상태값;  
					// 추천값: txt, img, url, hide, webgl, mp3, video, timestamp;  
}  
```

## 위임자

```
delegates{  
	uint64_t	assetid;	// ```claim```을 위해 오퍼된 자산 ID;  
	name		owner;		// 자산 소유자;  
	name		delegatedto;	// 이 자산을 클레임할 수 있는 사람;  
	uint64_t	cdate;		// 오퍼 생성일;  
	uint64_t	period;		// 자산이 대여될 시간(초). 대여자는 이 시간이 만료될 때까지
					// 위임을 해제할 수 없음. 하지만 대출자는 언제든 반환 가능.
}  
```

## 화폐  통계 (대체  가능  토큰)

```
stat {  
	asset		supply;		// 토큰 공급 
	asset		max_supply;	// 토큰 최대 공급
	name		issuer;		// 대체 가능 토큰 생성자
	uint64_t	id;		// 이 토큰의 고유 ID
	bool		authorctrl;	// 이것이 참(1)이면 토큰 생성자(소유자가 아님)가 ```burn```과 ```transfer```를 할 수 있음.
	string		data;		// 스트링화 된 json. 다음 키를 포함할 것을 추천: `img`, `name`
}
```

## 계정 (대체 불가능 토큰)

```
accounts {  
	uint64_t	id;		// ```stat``` 테이블의 토큰 ID
	name		author;		// 토큰 생성자
	asset		balance;	// 토큰 잔고
}  
```

```
sofferf {
	uint64_t	id;		// ```claim```을 위한 ```offer```의 ID (자동으로 증가함) 
	name		author;		// FT 생성자
	name		owner;		// FT 소유자
	asset		quantity;	// 수량
	name		offeredto;	// 오퍼를 클레임할 수 있는 계정
	uint64_t	cdate;		// 오퍼 생성일
}
```

## NTT  
```
snttassets {  
	uint64_t	id; 		// NTT id used for claim or burn;  
	name		owner;  	// asset owner (mutable - by owner!!!);  
	name		author;		// asset author (game contract, immutable);  
	name		category;	// asset category, chosen by author, immutable;  
	string		idata;		// immutable assets data. Can be stringified JSON or just sha256 string;  
	string		mdata;		// mutable assets data, added on creation or asset update by author. Can be  
					// stringified JSON or just sha256 string;  
}  
```
  
```
nttoffers {
	uint64_t	id;		// id of the offer for claim (increments automatically) 
	name		author;		// ft author
	name		owner;		// ft owner
	asset		quantity;	// quantity
	name		offeredto;	// account who can claim the offer
	uint64_t	cdate;		// offer creation date
}
```    

## More Data 
```
moredata{
	uint64_t		id;		// id of the more data 
	name			author;	// author of the more data 
	string			data;	// more data
}
```

# 예시: 스마트  컨트랙트에서  심플에셋  사용하기

## 자산을 생성해 소유자 계정 ownerowner22으로 전송하기:

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name category = "weapon"_n;
name owner = "ownerowner22"_n;
string idata = "{\"power\": 10, \"speed\": 2.2, \"name\": \"Magic Sword\" }";
string mdata = "{\"color\": \"bluegold\", \"level\": 3, \"stamina\": 5, \"img\": \"https://bit.ly/2MYh8EA\" }";

action createAsset = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"create"_n,
		std::make_tuple( author, category, owner, idata, mdata,  0 )
);
createAsset.send();
```

## requireclaim 옵션으로 ownerowner22에 자산 생성하기:

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name category = "balls"_n;
name owner = "ownerowner22"_n;
string idata = "{\"radius\": 2, \"weigh\": 5, \"material\": \"rubber\", \"name\": \"Baseball\" }";
string mdata = "{\"color\": \"white\", \"decay\": 99, \"img\": \"https://i.imgur.com/QoTcosp.png\" }";

action createAsset = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"create"_n,
		std::make_tuple( author, category, owner, idata, mdata, 1 )
);
createAsset.send();
```

## 자산의  검색과  자산  정보  가져오기

1. 자산  구조에  관한 hpp 파일을  추가하십시오.

**주의! 현재 CDT는 v1.6.1에서는  컴파일이  되지  않는  버그가  있습니다.
v1.5.0 역시 abi generation에 한해 (객체 자신의 배열 포함 : std::vector<sasset> container;) "Segmentation fault (core dumped)" 버그가 발생합니다.**

```
TABLE account {
		uint64_t  	id;
		name  	  	author;
		asset  	  	balance;

		uint64_t primary_key()const {
			return id;
		}
};
typedef eosio::multi_index< "accounts"_n, account > accounts;

TABLE sasset {
		uint64_t  				id;
		name  					owner;
		name  					author;
		name  					category;
		string  				idata;
		string  				mdata;
		std::vector<sasset>  	container;
		std::vector<account>  	containerf;

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
```

2. 정보의  검색  및  사용

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
name author = get_self();
name owner = "lioninjungle"_n;
  
uint64_t assetid = 100000000000187

sassets assets(SIMPLEASSETSCONTRACT, owner.value);
auto idx = assets.find(assetid);

check(idx != assets.end(), "Asset not found or not yours");

check (idx->author == author, "Asset is not from this author");

auto idata = json::parse(idx->idata);  // for parsing json here is used nlohmann lib
auto mdata = json::parse(idx->mdata);  // https://github.com/nlohmann/json

check(mdata["cd"] < now(), "Not ready yet for usage");
```

  

## 자산  업데이트

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

auto mdata = json::parse(idxp->mdata);
mdata["cd"] = now() + 84600;

name author = get_self();
name owner = "ownerowner22"_n;
uint64_t assetid = 100000000000187;

action saUpdate = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"update"_n,
		std::make_tuple(author, owner, assetid, mdata.dump())
);
saUpdate.send();
```

  

## 자산  하나  전송하기

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

std::vector<uint64_t> assetids;
assetids.push_back(assetid);

string memo = "Transfer one asset";

action saUpdate = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"transfer"_n,
		std::make_tuple(from, to, assetids, memo)
);
saUpdate.send();
```

## 동일한  메모가  첨부된  자산  두  개를  동일한  수신자에게  전송하기

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

std::vector<uint64_t> assetids;
assetids.push_back(assetid1);
assetids.push_back(assetid2);

string memo = "Transfer two asset"

action saUpdate = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"transfer"_n,
		std::make_tuple(from, to, assetids, memo)
);
saUpdate.send();
```

## issue로 생성된 토큰에 대한 issuef (대체 가능 토큰)

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 100;
wood.symbol = symbol("WOOD", 0);

name author = get_self();
name to = "lioninjungle"_n;

std::string memo = "WOOD faucet";
action saRes1 = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"issuef"_n,
		std::make_tuple(to, author, wood, memo)
);
saRes1.send();
```

## authorctrl이 사용 가능한 경우의, 작성자에 의한 transferf (대체 가능 토큰)

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 20;
wood.symbol = symbol("WOOD", 0);

name from = "lioninjungle"_n;
name to = get_self();
name author = get_self();

std::string memo = "best WOOD";
action saRes1 = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"transferf"_n,
		std::make_tuple(from, to, author, wood, memo)
);
saRes1.send();
```

## authorctrl이 사용 가능한 경우의, 작성자에 의한 burnf (대체 가능 토큰)

```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

asset wood;
wood.amount = 20;
wood.symbol = symbol("WOOD", 0);

name author = get_self();
name from = "lioninjungle"_n;

std::string memo = "WOOD for oven";
action saRes1 = action(
		permission_level{author, "active"_n},
		SIMPLEASSETSCONTRACT,
		"burnf"_n,
		std::make_tuple(from, author, wood, memo)
);
saRes1.send();
```

## 변경 로그 v1.1.0

   * 코드 리펙터링
   * 위임 및 양도된 NFT를 위해 컨테이너라이징 된 NFT의 분리 수정
   * 대여한 NFT의 위임 기간을 연장할 수 있게 해주는 새로운 액션 delegatemore 추가
   * 외부(bash) 유닛 테스트 추가

## 변경  로그 v1.0.1

- 내부적으로 `create` 액션  히스토리  로그에  이용되는  새로운  파라미터 `requireclaim`가 `createlog` 액션에  추가됨.

## 변경  로그 v1.0.0

- 자기  자신에게  자산을  오퍼하는  소유자  블록  처리

## 변경  로그 v0.4.2

- `saeclaim` 이벤트의  포멧  변경: assetids 배열이 map <assetid, from>으로  대체됨

## 변경  로그 v0.4.1

- `create` 액션에 require_recipient(owner) 추가

## 변경  로그 v0.4.0

**대체  가능  코인  정보를  보다  쉽게  찾을  수  있게  됨 (대체  가능  코인이 scope author를  가짐):**

- FT를  위해 `account` 테이블에  새  필드 `author`가  추가됨. (대체  가능  코인  정보를  보다  쉽게  찾을  수  있음)

**추가된  대체  가능  코인  정보:**

- `currency_stats` 테이블에  새로운  필드 `data`가  추가됨 - 키 `img`, `name`을  포함할  수도  있는 json을  스트링화 (시장을  통한  개선된  디스플레이를  위해  권장)
- `createf` 액션에  새로운  파라미터 `data`가  추가됨
- FT `data`를  변경하기  위한  새로운  액션 `updatef`가  추가됨

  

**대체  가능  토큰  오퍼/클레임**

- `offer`/`claim` FT에  사용되는  새로운  테이블 `sofferf`가  추가됨
- 새로운  액션 `offerf`, `cancelofferf`, `claimf`가  추가됨
- 개방된  오퍼가  없을  경우 (내부) `closef`에  대해  검사

**자산  컨테이너라이징**
**주의! 현재 CDT는 v1.6.1에서는  컴파일이  되지  않는  버그가  있습니다.
   v1.5.0 역시 abi generation 시 "Segmentation fault (core dumped)" 버그가 발생합니다. 추천: 컨트랙트  컴파일  시 v1.5.0와  저희가  제공하는 abi를  사용하실  것을  추천합니다.**

- 다른 NFT 및 FT의  어태치와  디태치를  위해  새로운  필드 `container`와 `containerf`가 NFT 자산  구조에  추가됨
- 새로운  액션 `attach`, `detach`가  추가됨
- 새로운  액션 `attachf`, `detachf`가  추가됨

**기타**

- `global` 테이블  내  필드  이름  변경됨 `lastid` -> 'lnftid`, 'spare` -> `defid` (내부용)
- `soffer` 테이블  내 `offeredTo` 필드의  이름이 `offeredto`로  변경됨

## 변경  로그 v0.3.2

- `offer` 액션에 `memo` 파라미터  추가;
- `delegate` 액션에 `memo` 파라미터  추가;

## 변경  로그 v0.3.1

- NFT를  위한  내부  액션 `createlog`를  추가함. create 액션에  의해  사용되며, assetid를  기록하여  제 3자  탐색자가  새로운  자산 id와  기타  정보를  손쉽게  얻을  수  있도록  함.
- 새로운  싱글톤  테이블 `tokenconfigs`를  추가함. 이는  외부  컨트랙트  파스  액션과  테이블을  도움 (탈중앙화  교환소, 시장, 및  다수의  토큰을  사용하는  기타  컨트랙트에  유용).
시장, 교환소, 및  기타  유관  컨트랙트는  다음  코드를  이용해  이  정보를  열람할  수  있음.

```
Configs configs("simpleassets"_n, "simpleassets"_n.value);
configs.get("simpleassets"_n);
```

- 새로운  액션 `updatever`를  추가함. 이는  제 3자  지갑, 시장  등에  대해  본  버전으로  배포된  심플에셋을  업데이트함;
- 이벤트  알림과  관련한  새로운  예시: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample

## 변경  로그 v0.3.0

- 유예된  거래를  이용한  이벤트  알림이  추가됨. 자산  작성자는  자산의  생성, 전송, 클레임, 소각에  대한  알림을  받게됨. 알림을  받기  위해선  작성자  컨트랙트에  다음을  입력  해야함:

```
ACTION saecreate ( name owner, uint64_t assetid );
ACTION saetransfer ( name from, name to, std::vector<uint64_t>& assetids, std::string memo );
ACTION saeclaim  ( name account, std::vector<uint64_t>& assetids );
ACTION saeburn ( name account, std::vector<uint64_t>& assetids, std::string memo );
```

- `delegate` 액션과 `sdelegates` 테이블에  대한 `untildate` 파라미터가 `period` (초  단위)로  변경됨

## 변경  로그 v0.2.0

### 다소  변경된 eosio.token 컨트랙트를  이용하는  로직과  대체  가능  토큰  테이블  추가

- 새로운  액션  및  로직  추가: `createf`, `issuef`, `transferf`, `burnf`, `openf`, `closef`
- 새로운  테이블 `stat(supply, max_supply, issuer, id)`와 `accounts (id, balance)` 추가됨.
- stats 테이블(대체  가능  토큰들에  관한  정보)을  위한 scope가  작성자로  변경됨
- `accounts` 테이블의  기본  인덱스는 creates 액션에  생성되고 stats 테이블에  저장된 uniq id임.
- `authorctrl` 파라미터를  가진  대체  가능  코인을  위해 `stats` 테이블에 `createf` 액션이  추가됨. 이것이  참(1)인  경우, 토큰  작성자(소유자가  아님)가 burnf과 transferf를  할  수  있음. 생성  후에는  변경할  수  없음!
- 리카르디안  컨트랙트  업데이트
- 더  많은  예시  추가

## 변경  로그 v0.1.1

기타

- sdelagate 테이블  구조의  이름이 sdelegate로  변경됨 (오타)
- create 액션  파라미터의  이름이  변경됨: requireClaim -> require claim
- 모든  액션에  대한 assetID 액션  파라미터의  이름이 assetid로  변경됨

대출  자산

- sdelegate 테이블 - 새  필드  추가됨: luntildate
- delegate 액션에 untildate 파라미터  추가됨. 파라미터가  올바르게  입력된  경우 (0이거나  미래에) 액션은  단순  검사를  시행함.
- undelegate는 untildate에  이르기  전까지는  작동하지  않음 (이는  자산  대출에  최소  기간을  보장함).
- 위임된  경우, untiltime 보다  일찍  자산을  재전송(반납)하는  것을  허용 (대출자는  일찍  반납할  수  있는  옵션을  가짐)

배치  프로세싱

- 클레임(claim) 액션: assentid 파라미터가 assetsids의  배열로  변경됨. 복수  클레임  로직  추가됨.
- 오퍼(offer) 액션: assentid 파라미터가 assetsids의  배열로  변경됨. 복수  오퍼  로직  추가됨.
- 오퍼  취소(canceloffer) 액션: assentid 파라미터가 assetsids의  배열로  변경됨. 복수  취소  로직  추가됨.
- 전송(transfer) 액션: assentid 파라미터가 assetsids의  배열로  변경됨. 복수  전송  로직  추가됨.
- 소각(burn) 액션: assentid 파라미터가 assetsids의  배열로  변경됨. 복수  소각  로직  추가됨.
- 위임/위임취소(delegate/undelegate) 액션: assentid 파라미터가 assetsids의  배열로  변경됨. 복수  위임/위임취소  로직  추가

-   `sofferf` 用于`offer` / `calimFT`的新表

-   新的操作`offer`，`cancelofferf` 和 `claimf` 

-   对 `closef` 检查如果没有公开招股（内部）

**集装资产**

**警告!!! CDT目前有一个不被允许在v1.6.1上编译的漏洞。
1.5.0也有一个漏洞“Segmentation fault（core dumped）”，但只有abi生效。**

**建议：使用1.5.0进行合约编译，并使用我们的abi。**

-   nft资产结构中用于附加和分离其他NFT或FT的新字段“container”和“containerf”

-   新操作 `attach`，`detach`

-   新操作 `attachf`，`detach`

**杂项**

-   字段重命名 `lasted`- \>` lnftid`，`spare`- \> `defid`（内部用法）在表 `global` 中

-   字段 `providedTo` 在 `soffer` 表中重命名为 `offersto`
---
## 更改日志v0.3.2

-   为操作 `offer` 添加了 `memo` 参数;

-   为操作 `delegate` 添加了 `memo` 参数;
---
## 更改日志v0.3.1

-   增加了NFT的内部操作 `createlog` 。由创建操作用于记录资产ID，以便第三方资源管理器可以轻松获取新的资产ID和其他信息。

-   增加了新的单例表 `tokenconfigs`。它有助于外部合约正确解析操作和表格（对于分散交换，市场和使用多个代币的其他合约有用）。市场，交易所和其他依赖合约将能够使用以下代码查看此信息。
```
Configs configs("simpleassets"_n, "simpleassets"_n.value);

configs.get("simpleassets"_n);
```
-   增加了操作 `updatever`。它为第三方钱包，市场等更新了SimpleAstes部署的版本;

-   事件通知的新示例： <https://github.com/CryptoLions/SimpleAssets-EventReceiverExample>
---
## 更改日志v0.3.0

-   使用延迟事务添加了事件通知。资产作者将收到有关资产创建，转移，索赔或烧录的通知。要收到它，请为您的创建者合约添加以下操作：
```
ACTION saecreate ( name owner, uint64_t assetid );

ACTION saetransfer ( name from, name to, std::vector\<uint64_t\>& assetids,
std::string memo );

ACTION saeclaim ( name account, std::vector\<uint64_t\>& assetids );

ACTION saeburn ( name account, std::vector\<uint64_t\>& assetids, std::string
memo );
```
-   `untildate` 参数更改为 `period`（以秒为单位）的操作 `delegate` 和表 `sdelegates` 
---
## 更改日志v0.2.0

### 使用eosio.token合约添加了可替代代币（Fungible Token）表和逻辑，但有一些更改

-   新的操作和逻辑：`createf`，`issuef`，`transfer`，`burnf`，`openf`，`closef`

-   添加了新表 `stat(supply, max_supply, issuer, id)` 和 `accounts (id, balance)` 。

-   统计表的范围（关于可替代代币的信息）已更改为创建者

-   `accountstable` 的主索引是在 `create` f操作上创建的 `uniq id` 并存储在 `stats` 表中。

-   添加 `createf` 与 `parameter` 可替代代币操作 `authorctrl` 至 `stats` 表。如果为true（1）允许代币创建者（而不仅仅是所有者）使用burnf和transferf。创建后无法更改！

-   李嘉图合约已更新

-   以下有更多用法示例
---
## 更改日志v0.1.1

**杂项**

-  sdelagate 表结构重命名为 sdelegate（typo）

- 创建操作参数重命名：requireClaim - \> requireclaim

- assetID操作参数在所有要声明的操作中重命名

**借入资产**

- sdelegate表 - 添加了新字段：untildate

- 委托操作添加参数untildate。如果参数输入正确（零或将来），操作会进行简单检查。

- undelegate在不公开之前不会工作（这保证了资产贷款的最低期限）。

- 如果被委托，允许转移资产（返还）早于截至时间（借款人可以提前返还）

**批量处理**

- 声明操作（claim action）：assetid参数已更改为assetsids数组。添加了多个声明逻辑。

- 报价操作（offer action）：assetid参数已更改为assetsids数组。添加了多个提供逻辑。

- 取消报价操作（canceloffer action）：assetid参数已更改为assetsids数组。添加了多个取消逻辑。

- 传输操作（transfer action）：assetid参数已更改为assetsids数组。添加了多个资产转移逻辑。

- 烧录操作（burn action）：assetid参数已更改为assetsids数组。添加了多个刻录逻辑。

- 委托/非委托操作（delegate / undelegated action）：assetid参数已更改为assetsids数组。添加了多个委托/取消授权逻辑。
