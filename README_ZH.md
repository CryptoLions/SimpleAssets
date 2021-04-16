
﻿﻿﻿﻿﻿﻿﻿﻿﻿简单资产（Simple Assets）
	 
文件版本更新于2020.07.29

=========================

## 目录

1.[简介](#简介)

   * [相关资源](###相关资源)
   * [代币类型](###代币类型)

2.[合约操作](#合约操作)

3.[数据结构](#数据结构)

4.[示例: 如何在智能合约中使用简单资产](#示例如何在智能合约中使用简单资产)

5.[创建者日志](#创建者日志)

6.[更新日志](#更新日志)

---
# 简介

一款EOSIO区块链上的数字资产标准，适用于不可替代代币（Non-Fungible Tokens，NFTs）、可替代代币(Fungible Tokens，FTs) 和 不可转让代币（Non-Transferable Tokens，NTTs），由[CryptoLions](https://CryptoLions.io)开发创建。    
  
**警告**现在最低基于eosio.cdt v1.6.3版。

---
通过调用简单资产（Simple Assets）合约来使用Simple Assets。这就像是Dapps的Dapp。 
 
丛林测试网:**`simpleassets`**  

EOS 主网: **`simpleassets`**  
WAX 主网： **`simpleassets`**  
MEETONE 主网: **`smplassets.m`**  
TELOS 主网: **`simpleassets`**   
PROTON 主网: **`simpleassets`**
EUROPECHAIN 主网: **`simpleassets`**

简单资产（Simple Assets）是一个独立的合约，其他Dapps可以直接调用它来管理自己的数字资产。这为Dapp用户提供了额外的保证，即资产的所有权由信誉良好的外部机构管理，并且一旦创建，Dapp只能管理资产的mdata部分。 所有与所有权相关的功能都存在于游戏之外。

[相关信息：理解所有权。](https://medium.com/@cryptolions/digital-assets-we-need-to-think-about-ownership-authority-a2b0465c17f6)

请使用`authorreg`操作发送自己的NFTs信息至第三方商城。

此外，dapps可以自行部署简单资产（Simple Assets）副本并进行修改以更好地使用控制其功能，但是这有可能会损害其与钱包以及某些EOSIO基础结构的兼容性。部署前，应修改简单资产（Simple Assets）以防止任何人创建资产。

---
### 相关资源
web: [http://simpleassets.io](http://simpleassets.io/)

Git: [https://github.com/CryptoLions/SimpleAssets](https://github.com/CryptoLions/SimpleAssets)

Telegram: [https://t.me/simpleassets](https://t.me/simpleassets)  

简介和演示：https://medium.com/\@cryptolions/introducing-simple-assets-b4e17caafaa4  

（对开发人员很重要）此处可以查阅每个参数的详细说明：https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp

创建者的事件接收器示例：https://github.com/CryptoLions/SimpleAssets-EventReceiverExample

---
### 代币类型

#### 不可替代代币（NFTs）
NFTs(Non-Fungible Tokens)是最常见的数字资产类型。它们用于表示唯一的代币资产类型。

**NFT结构**
简单资产的NFTs分为 **mdata**(创建者可以随时更新的数据，不需考虑所有权)和 **idata**(NFTs创建时设置的数据，永远不可更新)。

两者都是字符串化的JSONs。例如：`{\"key1\":\"some-string\", \"key2\":5}`

**Category**是一个可选字段，它让您更方便地对NFTs进行分组。Category名称必须小于或等于12个字符(a-z，1-5)。

**Offer/Claim** 与 **Transfer** 如果你交易转让一个NFT，则发送者将为RAM付费。或者，你可以简单地提供该NFT，申领用户将为其RAM付费。
(*注意：我们正在努力开发一种新功能，该功能允许NFT创建者保留大量RAM，这将节省用户间用于转让交易的费用。*)

*NFTs也译作非同质化代币*

**RAM使用情况**

NFTs的RAM使用量取决于idata和mdata字段中存储的数据量。如果它们都为空，则每个NFT占用276个字节。

idata和mdata中的每个符号都是+1字节。

#### 可替代代币(FTs)

需要FTs(Fungible Tokens)的Dapps应该在使用标准eosio.token合约和“简单资产（Simple Assets）”合约之间做出决定。 区别如下：

在简单资产中(Simple Assets)

* 范围(Scope)是创建者(Author)而不是符号(Symbol)。 
* 统计信息表(Stat table)还包含有关每个FT的其他数据(详见下面的货币统计信息[Currency Stats](#currency-stats-fungible-token))
* 对于交易转让(Transfers)，您需要使用简单资产合约中的`transferf`操作。
* 如果创建者设置了`authorcontrol`标志，则创建者可以独立于用户的同意授权而转让/刻录等用户的FTs。
* 追踪FTs表包含创建者的帐户名，从而允许在不同的dapps内具有相同名称的FTs(例如: https://bloks.io/contract?tab=Tables&table=accounts&account=simpleassets&scope=bohdanbohdan&limit=100)。

*(注意：FTs还具有**offer/claim**功能，可以作为**Transfers**的替代方法。对于FTs，发送者唯一要支付RAM的时间就是接收者之前从未持有过这些FTs。创建FT表大约使用300个字节。)*

#### 不可转让代币(NTTs)

不可转让代币(Non-Transferrable Tokens，NTTs)最可能的的两种用例是

* 可以授予帐户许可证，但该许可证不能转让。
* 向特定账户提供奖品和奖励。

使用不可转让代币(NTTs)的原因：

* NTTs出现在第三方资产浏览器中。
* 简单资产处理一些功能。

更多有关NTTs的信息: https://medium.com/@cryptolions/introducing-non-transferable-tokens-ntts-2f1a532bf170

---
# 合约操作

可在此处找到每个参数的说明：https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp

```
authorreg		( name author, string dappinfo, string fieldtypes, string priorityimg )
authorupdate		( name author, string dappinfo, string fieldtypes, string priorityimg )
setarampayer		( name author, name category, bool usearam ) 

# -- For Non-Fungible Tokens （NTFs） ---

create (author, category, owner, idata, mdata, requireсlaim)
update (author, owner, assetid, mdata)
transfer (from, to , [assetid1,..,assetidn], memo)
burn (owner, [assetid1,..,assetidn], memo)

offer (owner, newowner, [assetid1,..,assetidn], memo)
canceloffer (owner, [assetid1,..,assetidn])
claim (claimer, [assetid1,..,assetidn])

delegate        (owner, to, [assetid1,..,assetidn], period, memo)
undelegate (owner, from, [assetid1,..,assetidn])
delegatemore        (owner, assetid, period)  

attach (owner, assetidc, [assetid1,..,assetidn])
detach (owner, assetidc, [assetid1,..,assetidn])

attachf (owner, author, quantity, assetidc)
detachf (owner, author, quantity, assetidc)

mdadd (author, data)
mdupdate (id, author, data) 
mdremove (id)
mdaddlog (id, author, data)

# -- For Fungible Tokens (FTs)---

createf (author, maximum_supply, authorctrl, data)
updatef (author, sym, data)
issuef (to, author, quantity, memo)
transferf (from, to, author, quantity, memo)
burnf (from, author, quantity, memo)

offerf (owner, newowner, author, quantity, memo)
cancelofferf (owner, [ftofferid1,...,ftofferidn])
claimf (claimer, [ftofferid1,...,ftofferidn])

openf (owner, author, symbol, ram_payer)
closef (owner, author, symbol)

# -- For Non-Transferable Tokens (NTTs) ---

 createntt      (author, category, owner, idata, mdata, requireсlaim)  
 updatentt      (author, owner, assetid, mdata)  
 burnntt        (owner, [assetid1,..,assetidn], memo)  
 claimntt       (claimer, [assetid1,..,assetidn])
```

---
# 数据结构

## 资产
```
sasset {

uint64_t id;        // 用于转移和搜索的资产ID  
name owner;         // 资产所有者（可变更 — 由所有者决定!!!） 
name author;        //  资产创建者（游戏合约，不可变更）  
name category;      // 资产类别，由创建者选择，不可变更  
string idata;       // 不可变更资产数据。 它可以是字符串化“JSON”或只是字符串“sha256”  
string mdata;       // 可变资产数据，由创建者在创建或资产更新时添加。 它可以是字符串化“JSON”或只是字符串“sha256”  
sasset[] container;     // 其它NFTs(可替代和不可替代代币)附加到此资产  
account[] containerf;   // FTs(可替代代币)附加到此资产  

}
```
为帮助第三方资产浏览器，我们建议在`idata`或`mdata`请包含以下字段：//`name`（文本）//`img` (图像的网页链接) //以及 `desc`（文本描述）的信息。

---

## 报价
```
offers {

uint64_t assetid;   // 提供 claim 的资产ID  
name owner;         // 资产所有者  
name offeredto;     // 可以 claim 此资产的人  
uint64_t cdate;     // 创建日期  
}  
```

---

## 创建者
```
authors {

name author;            // 资产创建者，将能够创建和更新资产

string data;            // 创建者的数据（json）将被商城用于更好的展示
                // 建议：形象徽标（logo），信息，网址;

string  stemplate;      // 数据（json）模式将告诉第三方市场如何显示每个NFT field。
                //key：状态值，其中 key 是 idata 或 mdata 的字段名
                //建议格式：
                //txt | 默认类型
                //url | 显示可点击的网页链接
                //img | 链接至img文件
                //webgl | 链接至webgl文件
                //mp3 | 链接至mp3文件
                //video | 链接至视频文件
                //hide | 不显示
                //imgb | 图片作为二进制字符串
                //webglb | webgl二进制
                //mp3b | mp3二进制
                //videob | 视频二进制

string imgpriority;  //指定NFTs类别的主图像字段
                //
                // 这用于当您想把自己的NFTs主要图像移作他用
                // 不仅是一个指向image field中指定的img图片URL链接, 也允许您创建具有不同image field的NFTs类别
                //
                //数据是分层的json
                //value： 来自idata或mdata field, 将用作
                //所有类别NFTs的主要图像

}
```

---

## 委托
```
delegates{

uint64_t assetid;   // 提供用于claim的资产ID
name owner;         // 资产所有者
name delegatedto;   // claim该资产者
uint64_t cdate;     // 提供创建日期
uint64_t period;    // 借出资产时间（以秒为单位），借出人不可取消，直到期限到期
                    // 期限到期，但接收方可随时转回
bool redelegate;    // 是否允许更多重新委托账户
string memo;        // 合约参数备忘录，最长64字节

}
```

---

## 货币统计（可替代代币）
```
stat {

asset supply;       // 提供代币
asset max_supply;   // 提供最大量代币
name issuer;        // 可替代代币（Fungible token）创建者
uint64_t id;        // 此代币的唯一ID
bool authorctrl;    // 如果 true（1）允许代币创建者（而不仅仅是所有者）进行烧录和传输
string data;        // 字符串化的json。 建议的keys包括： img，name

}
```

---

## 账户（可替代代币）
```
accounts {

uint64_t id;    // 代币ID，来自 stat 表
name author;    // 代币创建者
asset balance;  // 代币余额

}
```

```
offerfs {

uint64_t id;    // claim 此 offer 的ID（自动递增）

name author;    // 可替代代币(fungible token)创建者
name owner;     // 可替代代币(fungible token)所有者
asset quantity; // 资产数量
name offeredto; // 可以 claim 此 offer 的帐户
uint64_t cdate; // offer创建日期

}
```

---

## 不可转让代币NTT
```
snttassets {  
    uint64_t    id;         // NTT id 用于 claim 或 burn;  
    name        owner;      // 资产所有者 (可变 - 基于所有者!!!);  
    name        author;     // 资产创建者 (游戏合约, 不可变);  
    name        category;   // 资产类别, 有创建者选择, 不可变;  
    string      idata;      // 不可变资产数据. 可以是 JSON 字符串化或只是 sha256 字符串;  
    string      mdata;      // 可变资产数据, 有创建者创建或更新资产时添加；
                            // 可以是 JSON 字符串化或只是 sha256 字符串;  
			    // 使用除JSON字符串之外的格式不会干扰简单资产的功能
			    // 但会损害与第三方的兼容性
			    // 与尝试分散资产的第三方浏览器的兼容性
} 
```

```
nttoffers {
    uint64_t    id;     // 用于 claim 的 offer id  (自动递增) 
    name        author;     // ft 创建者
    name        owner;      // ft 所有者
    asset       quantity;   // 数量
    name        offeredto;  // 可以 claim the offer 的账户
    uint64_t    cdate;      // offer 的创建日期
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
## 创建者RAM付款人
```
sarampayer{
      uint64_t 			id;
      name    			author;
      name    			category;
      bool    			usearam;
      uint64_t  		from_id;

      auto primary_key() const {
        return id;
      }

      uint64_t by_author() const {
        return author.value;
      }
    };
    
```

---
# 示例:如何在智能合约中使用简单资产

## 创建资产并转移到所有者帐户ownerowner22：
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
name author = get_self();
name category = "weapon"_n;
name owner = "ownerowner22"_n;

string idata = "{\\"power\\": 10, \\"speed\\": 2.2, \\"name\\": \\"Magic
Sword\\" }";

string mdata = "{\\"color\\": \\"bluegold\\", \\"level\\": 3, \\"stamina\\": 5,
\\"img\\": \\"https://bit.ly/2MYh8EA\\" }";

action createAsset = action(
permission_level{author, "active"_n},
SIMPLEASSETSCONTRACT,
"create"_n,
std::make_tuple( author, category, owner, idata, mdata, 0 )

);

createAsset.send();
```

## 使用ownerowner22的requireclaim选项创建资产：
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
name author = get_self();
name category = "balls"_n;
name owner = "ownerowner22"_n;

string idata = "{\\"radius\\": 2, \\"weigh\\": 5, \\"material\\": \\"rubber\\",
\\"name\\": \\"Baseball\\" }";

string mdata = "{\\"color\\": \\"white\\", \\"decay\\": 99, \\"img\\":
\\"https://i.imgur.com/QoTcosp.png\\" }";

action createAsset = action(
permission_level{author, "active"_n},
SIMPLEASSETSCONTRACT,
"create"_n,
std::make_tuple( author, category, owner, idata, mdata, 1 )

);

createAsset.send();
```

## 搜索资产并获取资产信息
1. 请添加有关资产结构的hpp文件信息。
```
TABLE account {

uint64_t id;
name author;
asset balance;
uint64_t primary_key()const {
return id;

}

};

typedef eosio::multi_index\< "accounts"_n, account \> accounts;

TABLE sasset {

uint64_t id;
name owner;
name author;
name category;
string idata;
string mdata;
std::vector\<sasset\> container;
std::vector\<account\> containerf;
auto primary_key() const {
return id;

}

uint64_t by_author() const {

return author.value;

}

};

typedef eosio::multi_index\< "sassets"_n, sasset,

eosio::indexed_by\< "author"_n, eosio::const_mem_fun\<sasset, uint64_t,
&sasset::by_author\> \>

\> sassets;
```

2. 搜索和使用信息
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
name author = get_self();
name owner = "lioninjungle"_n;
uint64_t assetid = 100000000000187
sassets assets(SIMPLEASSETSCONTRACT, owner.value);
auto idx = assets.find(assetid);
check(idx != assets.end(), "Asset not found or not yours");
check (idx-\>author == author, "Asset is not from this author");
auto idata = json::parse(idx-\>idata); // for parsing json here is used nlohmann
lib
auto mdata = json::parse(idx-\>mdata); // https://github.com/nlohmann/json
check(mdata["cd"] \< now(), "Not ready yet for usage");
```

## 更新资产
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
auto mdata = json::parse(idxp-\>mdata);
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

## 交易转让资产
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;

uint64_t assetid = 100000000000187;

std::vector<uint64_t> assetids;
assetids.push_back(assetid);

string memo = "Transfer one asset";

action saTransfer = action(
	permission_level{from, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(from, to, assetids, memo)
);
saTransfer.send();
```

## 将两个资产转移到具有相同备忘录的同一接收器
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;
name author = get_self();
name from = "lioninjungle"_n;
name to = "ohtigertiger"_n;
std::vector\<uint64_t\> assetids;
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

## 烧录资产
```
name SIMPLEASSETSCONTRACT = "simpleassets"_n;

name owner = "lioninjungle"_n;
uint64_t assetid1 = 100000000000187;
uint64_t assetid2 = 100000000000188;

std::vector<uint64_t> assetids;
assetids.push_back(assetid1);
assetids.push_back(assetid2);

string memo = "Transfer two asset"

action saBurn = action(
	permission_level{owner, "active"_n},
	SIMPLEASSETSCONTRACT,
	"transfer"_n,
	std::make_tuple(owner, assetids, memo)
);
saBurn.send();
```

## issuef创建代币问题（可替代代币）
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

## 如果启用了authorctrl，则由创建者转让代币（可替代）
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

## 如果启用了authorctrl，则由创建者烧录代币（可替代）
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
---
# 创建者日志

## authorreg操作
创建者可以在创建者注册表(authorreg)中进行注册，与第三方资产浏览器、钱包和市场进行连接通信。

`ACTION authorreg( name author, string dappinfo, string fieldtypes, string priorityimg );`

@param **author** 是将创建资产的创建者帐户

@param **dappinfo** 是字符串化的JSON。建议包含：名称-应用程序名称
company - 公司名称
logo - 网址图片
url - 游戏网址
info - 应用程序简介
defaultfee - 您要从市场上收取的费用的x%比的100x (例 2%, 200)

@param **fieldtypes** 是带有key：state值的字符串化JSON，其中key是来自 mdata 或 idata 的密钥，state指示显示字段的推荐方式。有关最新的推荐值，请查阅 https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp.
@param priorityimg 是JSON，它将NFT类别与 idata 或 mdata 中的字段名称相关联，该字段名称指定该NFTs类别的主图像字段。这可能是一种不常见的用例，可以空白。如果希望NFTs的类别具有除.img之外的主图像字段，则可以使用"CATEGORY":"otherfieldname"。如果您希望以webgls或其他格式作为主要图像格式，这是最可能的用例。

## authorreg和authorupdate的Cleos示例

**authorreg**
```
./cleos.sh.jungle push action simpleassets authorreg '["ilovekolobok", "{\"name\": \"Kolobok Breeding Game\", \"company\": \"CryptoLions\", \"info\": \"Breed your Kolobok\", \"logo\": \"https://imgs.cryptolions.io/logo_256.png\", \"url\": \"https://kolobok.io\", \"defaultfee\":200}", "{\"bdate\":\"timestamp\"},{\"cd\":\"timestamp\"},{\"img\":\"img\"},{\"st\":\"hide\"},{\"url\":\"url\"}", "{\"kolobok\":\"img\"},{\"*\":\"img\"}" ]' -p ilovekolobok
```

**authorupdate**
```
./cleos.sh.jungle push action simpleassets authorupdate '["ilovekolobok", "{\"name\": \"Kolobok Breeding Game\", \"company\": \"CryptoLions\", \"info\": \"Breed your Kolobok\", \"logo\": \"https://imgs.cryptolions.io/logo_256.png\", \"url\": \"https://kolobok.io\", \"defaultfee\":200}", "{\"bdate\":\"timestamp\"},{\"cd\":\"timestamp\"},{\"img\":\"img\"},{\"st\":\"hide\"},{\"url\":\"url\"}", "{\"kolobok\":\"img\"},{\"*\":\"img\"}" ]' -p ilovekolobok
```

---
# 更新日志

## 更新日志v1.6.0
* 添加了创建者RAM付款人选项
* 添加了setarampayer，dellampayer操作
* detach and detachf只针对创建者
* 备注增加至512
* 代码改进


## 更新日志v1.5.2
* 重新启用事件通知，执行如下操作：
saeburn, saeclaim, saetransfer, saechauthor, saecreate.
* 更改了通知的逻辑
如果需要将有关资产范围的变化通知给创建者的合约，请参考例子
https://github.com/CryptoLions/SimpleAssets-EventReceiverExample

---
## 更新日志v1.5.1
* 增加了burnlog, burnflog, burnnttlog
* 增加了对集装器中附加资产的烧录的限制
* 增加了对附加到委托资产的限制
* 代码更新
* SAE通知暂停使用

---
## 更新日志v1.5.0
* 增加了将SimpleAssets.hpp添加或包含至其他项目中的可能性。这有助于技术开发人员轻松地将简单资产(Simple Assets)集成整合至其它合约中
* 增加了技术开发人员功能sa_getnextid，用来轻松获取新创建资产的id
* 增加了更多数据功能(actions mdremove, mdupdate, mdaddlog, mdadd)。这提供了一个简单资产列表，该列表可以存储NFTs的其它或者重复的信息，并且保持最小占用RAM

---
## 更新日志v1.4.1
* 重命名"Author Registration"(创建者注册)中的字段和操作，以提高工作效率
regauthor -> authorreg
data -> dappinfo
stemplate -> fieldtypes
imgpriority -> priorityimg
* 在自述文件中添加了"Author Registration"(创建者注册)文档

---
## 更新日志v1.4.0
* 重新委托资产。（资产的借出人可以允许他们重新借出）
* 在委托动作中添加了新的参数`bool reelegate`，它允许资产重新委托
* 在表委托中添加了新的字段`bool reelegate`=>在自部署合约的情况下需要迁移！
* 取消删除操作中的参数，已从中删除。 （在委托人表中可以找到借款人的身份）
* 固定转移空资产数组
* 错误消息已改进，更加清晰
* 代码重构

---
## 更新日志v1.3.0
* 使用最新的合约开发工具包（CDT v1.6.3）进行升级（解决了此编译问题
* 次要代码重构

---
## 更新日志v1.2.0
* 不可转让代币（NTTs）-新tables: snttassets 和 nttoffers
* 新 NTT 操作: createntt, createnttlog, claimntt, updatentt, burnntt
* 委托更多修复操作（感谢 cc32d9）
* ricardian 合约更新
* 为NTT添加外部逻辑测试

---
## 更新日志v1.1.3
* ricardian 合约更新
* 可替代代币报价问题修复

---
## 更新日志v1.1.2
* 把`string imgpriority`字段添加至`sauthor`表以及`regauthor`和`authorupdate` 操作中
* 重要信息：自我部署简单资产可能需要迁移 regauthor 表（如果使用）

---
## 更新日志v1.1.1
* 优化 claim/transfer/burn 功能
* 备注字段已添加至delegates表（这允许出租者/游戏创建不同类别的借入资产-例如高风险/低风险）。在进行delegate操作时，操作参数的备忘录将存储至此新字段。最多64字节
* 为delegate备忘录添加三个新测试单元

---
## 更新日志v1.1.0
* 代码重构
* 修复了为委托和转让的NFTs的分离批量处理功能
* 新合约允许延长借用NFT的委托期限
* 增加了外部（bash）单元测试

## 更新日志v1.0.1
* `createlog`操作中的新参数`requireclaim`，用于`create`操作历史记录日志

---
## 更新日志v1.0.0
* 阻止所有者向自己提供资产

---
## 更新日志v0.4.2
* `saeclaim`事项的格式已更改：由 map <assetid，from>替换asseti数组

## 更新日志v0.4.1
* 添加了`require_recipient`（所有者）来执行`create`操作

---
## 更新日志v0.4.0

**轻松找到可替代代币的信息（可替换代币有创建者范围）：**
* FT的`account`表中的新字段`author`（更容易找到可替代代币信息）

**更多可替代代币信息**

* 新领域`data`中`currency_stats`的表-字符串化JSON其中可能包括键 `img`，`name`（建议最好通过市场显示）
* 新参数`data`在`createf`操作中
* updatef改变FT的新举措data

**提供/声明可替代的代币**

* `sofferf`用于`offer` / `calim` FT的新表
* 新的操作`ffer`，`cancelofferf` 和 `claimf`
对`closef`检查如果没有公开招股（内部）

**集装资产**

* nft资产结构中用于附加和分离其他NFT或FT的新字段`container`和`containerf`
* 新操作`attach`，`detach`
* 新操作`attachf`，`detach`

**杂项**

* 字段重命名`lasted`- >`lnftid`，`spare`- > `defid`（内部用法）在表 `global` 中
* 字段 `providedTo` 在 `soffer` 表中重命名为 `offersto`

---
## 更新日志v0.3.2
* 为操作`offer`添加了`memo`参数
* 为操作`delegate`添加了`memo`参数

---
## 更新日志v0.3.1
* 增加了NFT的内部操作`createlog`。由创建操作用于记录资产ID，以便第三方资源管理器可以轻松获取新的资产ID和其他信息。
* 增加了新的单例表`tokenconfigs`。它有助于外部合约正确解析操作和表格（对于分散交换，市场和使用多个代币的其他合约有用）。市场，交易所和其他依赖合约将能够使用以下代码查看此信息。
```
Configs configs("simpleassets"_n, "simpleassets"_n.value);

configs.get("simpleassets"_n);
```
* 增加了操作`updatever`。它为第三方钱包，市场等更新了SimpleAstes部署的版本;
* 事件通知的新示例： https://github.com/CryptoLions/SimpleAssets-EventReceiverExample

---
## 更新日志v0.3.0
* 使用延迟事务添加了事件通知。资产作者将收到有关资产创建，转移，索赔或烧录的通知。要收到它，请为您的创建者合约添加以下操作：
```
ACTION saecreate ( name owner, uint64_t assetid );

ACTION saetransfer ( name from, name to, std::vector\<uint64_t\>& assetids,
std::string memo );

ACTION saeclaim ( name account, std::vector\<uint64_t\>& assetids );

ACTION saeburn ( name account, std::vector\<uint64_t\>& assetids, std::string
memo );
```
* `untildate`参数更改为`period`（以秒为单位）的操作`delegate`和表`sdelegates`

---
## 更新日志v0.2.0
**使用eosio.token合约添加了可替代代币（Fungible Token）表和逻辑，但有一些更改**
* 新的操作和逻辑：`createf`，`issuef`，`transfer`，`burnf`，`openf`，`closef`
* 添加了新表 `stat(supply, max_supply, issuer, id) `和`accounts (id, balance)`
* 统计表的范围（关于可替代代币的信息）已更改为创建者
* `accountstable`的主索引是在`create f`操作上创建的 uniq id 并存储在 stats 表中。
* 添加`createf`与 parameter 可替代代币操作`authorctrl` 至`stats`表。如果为true（1）允许代币创建者（而不仅仅是所有者）使用burnf和transferf。创建后无法更改！
* 李嘉图合约已更新
* 以下有更多用法示例

---
## 更新日志v0.1.1
**杂项**
* sdelagate 表结构重命名为 sdelegate（typo）
* 创建操作参数重命名：requireClaim - > requireclaim
* assetID操作参数在所有要声明的操作中重命名

**借入资产**
* sdelegate表 - 添加了新字段：untildate
* 委托操作添加参数untildate。如果参数输入正确（零或将来），操作会进行简单检查。
* undelegate在不公开之前不会工作（这保证了资产贷款的最低期限）。
* 如果被委托，允许转移资产（返还）早于截至时间（借款人可以提前返还）

**批量处理**
* 声明操作（claim action）：assetid参数已更改为assetsids数组。添加了多个声明逻辑
* 报价操作（offer action）：assetid参数已更改为assetsids数组。添加了多个提供逻辑
* 取消报价操作（canceloffer action）：assetid参数已更改为assetsids数组。添加了多个取消逻辑
* 传输操作（transfer action）：assetid参数已更改为assetsids数组。添加了多个资产转移逻辑。
* 烧录操作（burn action）：assetid参数已更改为assetsids数组。添加了多个刻录逻辑。
* 委托/非委托操作（delegate / undelegated action）：assetid参数已更改为assetsids数组。添加了多个委托/取消授权逻辑。
