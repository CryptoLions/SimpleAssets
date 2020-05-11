# SimpleAssets  
*versión 1.4.1 del documento*

## Alcance:
1. [Introducción](#introduction)
2. [Acciones de contratos](#contract-actions)
3. [Estructuras de datos](#data-structures)
4. [EJEMPLOS: cómo usar Simple Assets en contratos inteligentes](#examples-how-to-use-simple-assets-in-smart-contracts)
5. [RegistroAutor](#authorreg)
6. [RegistroCambios](#change-logs)

---------------------------  

# Introducción

Un estándar simple para activos digitales en blockchains EOSIO: Tokens no fungibles (NFTs), Tokens fungibles (FTs) y Tokens no transferibles (NTTs).   
por [CryptoLions](https://CryptoLions.io)  

中文翻译: https://github.com/CryptoLions/SimpleAssets/blob/master/README_ZH.md  
한국어 번역: https://github.com/CryptoLions/SimpleAssets/blob/master/README_KR.md  
  
web: http://simpleassets.io  
Git: https://github.com/CryptoLions/SimpleAssets    
Telegram: https://t.me/simpleassets  
  
Intro & Demos:  https://medium.com/@cryptolions/introducing-simple-assets-b4e17caafaa4  

Ejemplo de recepción de eventos para autores: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample   

**ADVERTENCIA** La dependencia mínima en eosio.cdt ahora es v1.6.3.  

---------------------------  

Use Simple Assets realizando solicitudes al contrato de Simple Assets.  Esta es como una Dapp para Dapps.  

Jungle Testnet: **simpleassets**  

EOS Mainnet: **simpleassets**  
WAX Mainnet: **simpleassets**  
MEETONE Mainnet: **smplassets.m**  
TELOS Mainnet: **simpleassets**  
PROTON: **simpleassets**
  
Simple Assets es un contrato aparte que otras Dapps pueden solicitar para gestionar sus activos digitales. Este funciona como garantía adicional para usuarios de la Dapp de que la posesión de activos es administrada por una autoridad externa acreditada, y que una vez creada, la Dapp solo podrá gestionar la mdata del activo. Todas las funcionalidades relacionadas a la propiedad existen fuera del juego. 

Estamos en el proceso de crear un DAC que curará actualizaciones de Simple Assets luego de su implementación en EOSIO mainnet. 

Relacionado: entendiendo [autoridad de propiedad](https://medium.com/@cryptolions/digital-assets-we-need-to-think-about-ownership-authority-a2b0465c17f6).  
  
Para publicar información sobre sus NFTs a marketplaces de terceros, use la acción  ```authorreg```.
  
Alternativamente, las dapps pueden implementar una copia propia de Simple Assets y modificarla para tener mayor control de la funcionalidad. Antes de ser implementada, Simple Assets debe modificarse para prevenir que cualquiera pueda crear activos.

---------------------------
## Uso de RAM

El uso de RAM para NFTs depende de cuántos datos están almacenados en los campos i.data y mdata. Si se llegasen a vaciar, cada NFT requiere `276 bytes`.

Cada símbolo en idata y mdata es de +1 byte.

---------------------------
## Tokens Fungibles (FTs)

Las Dapps que necesitan Tokens Fungibles deben decidir entre usar el contrato estándar eosio.token y el contrato de Simple Assets. Estas son las diferencias:

En Simple Assets,

* El Alcance es el Autor en lugar del Símbolo
* La tabla de estadísticas también incluye datos adicionales sobre cada FT (ver [Estadísticas de monedas](#currency-stats-fungible-token) abajo).
* Para transferencias se deberá usar la acción ```transferf``` desde el contrato SA.
* Si el autor establece un indicador ```authorcontrol```, el autor podrá transferir/quemar/etc los FTs independientemente del consentimiento del usuario.
* La tabla que monitorea los FTs incluye el nombre de cuenta del autor, permitiendo a distintos dapps que tengan FTs con el mismo nombre. (Ejemplo: https://bloks.io/contract?tab=Tables&table=accounts&account=simpleassets&scope=bohdanbohdan&limit=100)


---------------------------
## Tokens No Transferibles (NTTs)

Los dos casos más comunes para NTTs son

* licencias que pueden ser otorgadas a una cuenta, pero no transferidas.
* premios dados a una cuenta en particular.

Las razones para usar NTTs son:

* los NTTs aparecen en exploradores de activos de terceros.
* Simple Assets se encarga de algunas funcionalidades.

Más sobre NTTs: https://medium.com/@cryptolions/introducing-non-transferable-tokens-ntts-2f1a532bf170

---------------------------

# Acciones del contrato

Aquí se puede encontrar una descripción de cada parámetro:
https://github.com/CryptoLions/SimpleAssets/blob/master/include/SimpleAssets.hpp 

```
authorreg		( name author, string dappinfo, string fieldtypes, string priorityimg )
authorupdate		( name author, string dappinfo, string fieldtypes, string priorityimg ) 


 # -- Para Tokens No Fungibles (NFTs)---
 
 create			(author, category, owner, idata, mdata, requireсlaim)  
 update			(author, owner, assetid, mdata)  
 transfer		(from, to , [assetid1,..,assetidn], memo)  
 burn			(owner, [assetid1,..,assetidn], memo)  
 
 offer			(owner, newowner, [assetid1,..,assetidn], memo)  
 canceloffer		(owner, [assetid1,..,assetidn])  
 claim			(claimer, [assetid1,..,assetidn])  
  
 delegate		(owner, to, [assetid1,..,assetidn], period, redelegate, memo)  
 undelegate		(owner, [assetid1,..,assetidn])  
 delegatemore		(owner, assetid, period)  
 
 attach			(owner, assetidc, [assetid1,..,assetidn])
 detach			(owner, assetidc, [assetid1,..,assetidn])
 
 attachf		(owner, author, quantity, assetidc)
 detachf		(owner, author, quantity, assetidc)
 
 # -- Para Tokens Fungibles (FTs) ---
 
 createf		(author, maximum_supply, authorctrl, data)
 updatef		(author, sym, data)
 issuef			(to, author, quantity, memo)
 transferf		(from, to, author, quantity, memo)
 burnf			(from, author, quantity, memo)

 offerf			(owner, newowner, author, quantity, memo)
 cancelofferf		(owner, [ftofferid1,...,ftofferidn])
 claimf			(claimer, [ftofferid1,...,ftofferidn])

 openf			(owner, author, symbol, ram_payer)
 closef			(owner, author, symbol)
 
 # -- Para Tokens No Transferibles (NTTs) ---

 createntt		(author, category, owner, idata, mdata, requireсlaim)  
 updatentt		(author, owner, assetid, mdata)  
 burnntt		(owner, [assetid1,..,assetidn], memo)  
 claimntt		(claimer, [assetid1,..,assetidn])  

```

# Estructuras de datos 
## Activos
```
sasset {  
	uint64_t	id; 		// identificación del activo usado para transferir y buscar;  
	name		owner;  	// propietario del activo (mutable - por el propietario!!!);  
	name		author;		// autor del activo (contrato del juego, inmutable);  
	name		category;	// categoría del activo, elegido por el autor, inmutable;  
	string		idata;		// datos inmutables de activos. Puede ser stringified JSON o solo sha256 string;  
	string		mdata;		// datos mutables de activos, añadidos en creación o actualización de activos por el autor. Puede ser  
					// stringified JSON o solo sha256 string;  
	sasset[]	container;	// otros NFTs adjuntos a este activo
	account[]	containerf;	// FTs adjuntos a este activo
}  
```
Para ayudar a exploradores de activos externos, recomendamos incluir los siguientes campos en `idata` o `mdata`: 
`name` (text) 
`img` (url to image file) 


## Ofertas 
```
offers {  
	uint64_t	assetid;	// identificación de activo ofrecida para tomar ; 
	name		owner;		// propietario del activo;  
	name		offeredto;	// quien puede tomar este activo ; 
	uint64_t	cdate;		// fecha de creación de oferta;  
}  
```

## Autores 
```
authors {  
	name	author;			// autor de activos, quien puede crear y actualizar activos;  

	string	data;			// datos del autor (json) serán usados por mercados para una mejor demostración;
					// recomendaciones: logo, info, url;  

	string	stemplate;		// esquema de datos (json) para avisar a mercados de terceros cómo mostrar cada campo del NFT.
					// llave: valores de estado, en la que la clave es la llave de mdata o idata;
					// valores recomendados: 
					// txt    | tipo predeterminado
					// url    | mostrar como URL cliqueable
					// img    | enlace al archivo img
					// webgl  | enlace al archivo webgl
					// mp3    | enlace al archivo mp3
					// video  | enlace al archivo de video
					// hide   | no mostrar
					// imgb   | imagen como string en formato binario
					// webglb | webgl binario
					// mp3b   | mp3 binario
					// videob | video binario

	string	imgpriority;		// Especifica campo de imagen primaria para categorías de NFTs. 
					//
					// Esta se usa cuando quiera que su imagen primaria de NFTs sea otra
					// que un URL a un campo de imagen específico en un campo img.  Además, le permite
					// crear categorías de NFTs con distintos campos de imágenes primarias.
					// 
					// la data es un strigified json.
					// llave: categorías de NFT.
					// valor: un campo de idata o mdata para usarse como imagen primaria para 
					// todos los NFTs de esa categoría.

}  
```

## Delegados 
```
delegates{  
	uint64_t	assetid;		// identificación del activo ofrecido para tomar;  
	name		owner;			// propietario del activo;  
	name		delegatedto;		// quien puede tomar este activo;  
	uint64_t	cdate;			// fecha de creación de oferta;  
	uint64_t	period;			// Tiempo en segundos que el activo será prestado. El prestador no puede dejar de delegar hasta 
						// que el periodo expire, sin embargo el receptor puede transferir de vuelta en cualquier momento.
	bool 		redelegate;		// delegar de nuevo es permitir más delegación a la cuenta o no.
	string		memo;			// memorandum de parámetros de acción. Máxima longitud es de 64.

}  
```

## Estadísticas de monedas (Tokens Fungibles)
```
stat {  
	asset		supply;		// Suministro de tokens
	asset		max_supply;	// Máximo suministro de tokens
	name		issuer;		// Autor de token fungible
	uint64_t	id;		// Identificación única de este token
	bool		authorctrl;	// si es verídico(1) permitir al autor del token (y no solo al propietario) a quemar y transferir.
	string		data;		// stringified json. Llaves recomendadas para incluir: `img`, `name`
}
```

## Cuenta (Token Fungible)  
```
accounts {  
	uint64_t	id;		// identificación del token, de tabla de estadísticas
	name		author;		// autor del token
	asset		balance;	// balance de tokens
}  
```

```
offerfs {
	uint64_t	id;		// identificación de la oferta para tomar (incrementa automáticamente) 
	name		author;		// autor de ft
	name		owner;		// propietario de ft
	asset		quantity;	// cantidad
	name		offeredto;	// cuenta que puede tomar la oferta
	uint64_t	cdate;		// fecha de creación de oferta
}
```    
  

## NTT  
```
snttassets {  
	uint64_t	id; 		// Identificación del NTT usado para tomar o quemar;  
	name		owner;  	// propietario del activo (mutable - por el propietario!!!);  
	name		author;		// autor del activo (contrato del juego, inmutable);  
	name		category;	// categoría del activo, elegida por autor, inmutable;  
	string		idata;		// datos inmutables de activos. Puede ser stringified JSON o solo sha256 string;  
	string		mdata;		// datos mutables de activos, añadidos con la creación o actualización de activos por el autor. Puede ser 
					// stringified JSON o solo sha256 string;  
}  
```
  
```
nttoffers {
	uint64_t	id;		// identificación de la oferta para tomar (incrementa automáticamente) 
	name		author;		// autor de ft
	name		owner;		// propietario de ft
	asset		quantity;	// cantidad
	name		offeredto;	// cuenta que puede tomar la oferta
	uint64_t	cdate;		// fecha de creación de oferta
}
```    

# EJEMPLOS: cómo usar Simple Assets en contratos inteligentes

## Crear un activo y transferirlo a la cuenta del propietario ownerowner22:
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
	std::make_tuple( author, category, owner, idata, mdata,	0 )
);
createAsset.send();	
```

## Crear activo con la opción requireclaim de ownerowner22:
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

## Buscar activo y obtener información de activos
1. Por favor, agregue en su archivo hpp la información sobre la estructura de los activos

```
TABLE account {
	uint64_t	id;
	name		author;
	asset		balance;

	uint64_t primary_key()const { 
		return id;
	}
};
typedef eosio::multi_index< "accounts"_n, account > accounts;

TABLE sasset {
	uint64_t		id;
	name			owner;
	name			author;
	name			category;
	string			idata;
	string			mdata;
	std::vector<sasset>	container;
	std::vector<account>	containerf;

			
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

2. Búsqueda y uso de información
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

## Actualización del activo
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

## Transferir un activo
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

## Transferir dos activos al mismo receptor con el mismo memorandum
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

## Token issuef (fungible) creado en cuestión
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

## transferf (fungible) por el autor si authorctrl está habilitado
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

## burnf (fungible) por el autor si authorctrl está habilitado
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

-----------------
# AuthorReg

## Acción authorreg
Los autores pueden registrar en la tabla authorreg para comunicarse con exploradores de activos externos, billeteras, y marketplaces.

```
ACTION authorreg( name author, string dappinfo, string fieldtypes, string priorityimg );
```

@param **author**     es la cuenta del autor que creará los activos.

@param **dappinfo**   es stringified JSON. Las recomendaciones incluyen: 
	name	 		- nombre de la aplicación
	company 		- nombre de la compañía
	logo 			- url a la imagen 
	url 			- url al sitio web del juego
	info			- descripción corta de la aplicación
 	defaultfee 		- 100x el % de cuota que le gustaría recolectar de marketplaces.  (por 2%, 200)

@param **fieldtypes** es stringified JSON con llave: valores del estado, en que la clave es la llave de mdata o idata y el
estado indica la forma recomendada de mostrar el campo. Valores recomendados:
	txt		- texto (predeterminado)
	url		- mostrar como URL cliqueable
	img		- enlace al archivo img
	webgl		- enlace al archivo webgl
	mp3		- enlace al archivo mp3
	video		- enlace al archivo del video
	hide		- no mostrar
	imgb 		- la imagen como un string en formato binario
	webglb		- webgl binario
	mp3b 		- mp3 binario
	videob 		- video binario
	timestamp	- marca de tiempo unix en segundos
	ipfs		- enlace ipfs

@param **priorityimg** es JSON que se asocia a la categoría NFT con el campo del nombre de idata or mdata
que especifica el campo de imagen principal por esa categoría de NFTs.  Este es probablemente un uso poco común y
puede dejarse en blanco. Si prefiere que una categoría de NFTs tenga un campo de imagen principal distinto al img, 
deberá usar "CATEGORY":"otherfieldname".  Un caso de uso común sería si quiere que webgls o algún otro formato
sea la imagen principal.

## Ejemplos de Cleos de authorreg y authorupdate


### authorreg
```
./cleos.sh.jungle push action simpleassets authorreg '["ilovekolobok", "{\"name\": \"Kolobok Breeding Game\", \"company\": \"CryptoLions\", \"info\": \"Breed your Kolobok\", \"logo\": \"https://imgs.cryptolions.io/logo_256.png\", \"url\": \"https://kolobok.io\", \"defaultfee\":200}", "{\"bdate\":\"timestamp\"},{\"cd\":\"timestamp\"},{\"img\":\"img\"},{\"st\":\"hide\"},{\"url\":\"url\"}", "{\"kolobok\":\"img\"},{\"*\":\"img\"}" ]' -p ilovekolobok
```

### authorupdate
```
./cleos.sh.jungle push action simpleassets authorupdate '["ilovekolobok", "{\"name\": \"Kolobok Breeding Game\", \"company\": \"CryptoLions\", \"info\": \"Breed your Kolobok\", \"logo\": \"https://imgs.cryptolions.io/logo_256.png\", \"url\": \"https://kolobok.io\", \"defaultfee\":200}", "{\"bdate\":\"timestamp\"},{\"cd\":\"timestamp\"},{\"img\":\"img\"},{\"st\":\"hide\"},{\"url\":\"url\"}", "{\"kolobok\":\"img\"},{\"*\":\"img\"}" ]' -p ilovekolobok
```

-----------------
# Registro de cambios

## Registro de cambios v1.4.1

- Campos y acciones renombradas en el Registro del Autor para una mayor claridad
        regauthor -> authorreg  
        data -> dappinfo  
        stemplate -> fieldtypes  
        imgpriority -> priorityimg  
- Documentación del Registro del Autor agregada al readme

## Registro de cambios v1.4.0
- activos delegados de nuevo. (prestador de activos puede permitir que sean prestados nuevamente)
- Nuevo parámetro `bool redelegate` agregado en acción de delegar, la que permite que se deleguen activos de nuevo.
- Nuevo campo `bool redelegate` agregado en la tabla `delegates` => requiere migración en caso de un contrato auto-ejecutado!!!
- En el parámetro de la acción `undelegate` fue removido el `from`. (la identidad del prestatario está disponible en la tabla de delegados) 
- Corrección de transferencias de matriz de activos vacía
- Mensajes de error con mayor claridad 
- Refactoring de códigos


## Registro de cambios v1.3.0
- Actualización con la última versión del Contract Development Toolkit (CDT v1.6.3).  
  (Resuelve este compilation [issue](https://github.com/EOSIO/eosio.cdt/issues/527))  
- refactoring menor del código.


## Registro de cambios v1.2.0
- TOKENS NO TRANSFERIBLES (NTTs) - nuevas tablas: snttassets y nttoffers
- nuevas acciones NTT: createntt, createnttlog, claimntt, updatentt, burnntt
- corrección de la acción delegatemore  (gracias a cc32d9)
- contratos ricardianos actualizados.
- tests externos para la lógica NTT agregados.


## Registro de cambios v1.1.3
- contratos ricardianos actualizados.
- corrección del issue de oferta de token fungible


## Registro de cambios v1.1.2
- agregado el campo de `string imgpriority` en la tabla `sauthor` y a las acciones `regauthor` y `authorupdate`
- IMPORTANTE:  Instancias auto-ejecutables de Simple Assets podrían necesitar migrar a la tabla regauthor (si se usan).


## Registro de cambios v1.1.1
- función optimizada de tomar/transferir/quemar
- Campo de memorandum agregado a la tabla de delegados.  (Esto permite a prestadores/juegos crear diferentes clases de activos prestados - eg. alto riesgo / bajo riesgo.)  En cuanto a una acción delegada, el memorandum del parámetro de acción es almacenado a este campo nuevo. máxima longitud de 64 caracteres.
- Tres nuevas unidades de prueba agregadas al memorandum de delegación.


## Registro de cambios v1.1.0
- Refactoring del código
- Corrección de separación de NFTs en contenedores para NFTs delegados y transferidos.  
- nueva acción delegatemore que permite extender el periodo del delegado por NFT en préstamo.  
- Agregadas unidades de prueba externas (bash)
  
## Registro de cambios v1.0.1
- nuevo parámetro `requireclaim` en la acción `createlog` que es usado internamente para el registro de historial de acciones `create`.


## Registro de cambios v1.0.0
- Bloqueo de ofertas de propietarios de activos a ellos mismos

## Registro de cambios v0.4.2
- cambio de formato de evento `saeclaim`: matriz de assetids reemplazada por mapa <assetid, from>

## Registro de cambios v0.4.1
- agregado require_recipient(owner) a la acción `create`


## Registro de cambios v0.4.0

**Encuentre fácilmente información del token fungible (los tokens fungibles tienen alcance de autor):**
- nuevo campo `author` en tabla de `account` por FT. (Hace más fácil encontrar información del token fungible)

**Más información del token fungible:**
- nuevo campo `data` en la tabla de `currency_stats` - stringify json que puede incluir llaves `img`, `name` (recomendado para mostrar mejor por mercados)
- nuevo parámetro `data` en la acción de `createf`
- nueva acción `updatef` para cambiar `data` del FT

**Ofrecer/tomar tokens fungibles**
- nueva tabla `sofferf` para usar `offer`/`claim` FT
- nuevas acciones `offerf`, `cancelofferf` y `claimf`
- en chequeo `closef` si no hay ofertas abiertas (internas)

**Activos en contenedores**  
- nuevos campos `container` y `containerf` en estructura de activos NFT para adjuntar y separar otros NFT o FT 
- nuevas acciones `attach`, `detach`
- nuevas acciones `attachf`, `detachf`

**misc**
- campos renombrados `lastid` -> `lnftid`, `spare`->`defid` (uso interno) en la tabla `global` 
- campo `offeredTo` renombrado a `offeredto` en la tabla `soffer`


## Registro de cambios v0.3.2
- Agregado el parámetro `memo` a la acción `offer`;  
- Agregado el parámetro `memo` a la acción `delegate`;


## Registro de cambios v0.3.1
- Agregada la acción interna `createlog` para NFT. Usada por la acción de crear registro assetid para que así exploradores externos puedan fácilmente obtener nuevas identificaciones de activos y otra información.
- Nueva tabla singelton `tokenconfigs` agregada. Ayuda a contratos externos analizar acciones y tablas correctamente (Útil para intercambios descentralizados, marketplaces y otros contratos que usen múltiples tokens).
  Marketplaces, intercambios y otros contratos de confianza tendrán visibilidad sobre esta información usando el siguiente código.
	```
	Configs configs("simpleassets"_n, "simpleassets"_n.value);
	configs.get("simpleassets"_n);
	```
- agregada la acción `updatever`. Esta actualiza la versión de implementación de este SimpleAssets por billeteras de terceros, marketplaces, etc;
- Nuevos ejemplos de notificaciones de Eventos: https://github.com/CryptoLions/SimpleAssets-EventReceiverExample  

## Registro de cambios v0.3.0
- Agregadas notificaciones de eventos usando transacciones diferidas. El autor de activos recibirá notificaciones sobre activos creados, transferidos, tomados o quemados. Para recibirlas por favor agregue la siguiente acción a su contrato de autor:  
	```
        ACTION saecreate   ( name owner, uint64_t assetid );  
        ACTION saetransfer ( name from, name to, std::vector<uint64_t>& assetids, std::string memo );  
        ACTION saeclaim    ( name account, std::vector<uint64_t>& assetids );  
        ACTION saeburn     ( name account, std::vector<uint64_t>& assetids, std::string memo );  
	```
- el parámetro `untildate` fue cambiado a `period` (en segundos) para las acciones `delegate` y la tabla `sdelegates`  


## Registro de cambios v0.2.0
### Agregadas tablas y lógica de Tokens Fungibles usando el contrato eosio.token pero con algunos cambios
- Nuevas acciones y lógica: `createf`, `issuef`, `transferf`, `burnf`, `openf`, `closef`
- agregadas nuevas tablas `stat(supply, max_supply, issuer, id)` y `accounts (id, balance)`. 
- alcance de tablas de estadísticas (info about fungible tokens) cambiado a autor
- índice primario para la tabla `accounts` e identificación única creada en la acción createf y almacenada en la tabla de estadísticas.
- agregada la acción  `createf` para tokens fungibles con el parámetro `authorctrl` a tabla de `stats`. Si es verdad(1) permite al autor del token (y no solo al propietario) a burnf y transferf. ¡No puede ser cambiado luego de su creación!
- Contratos ricardianos actualizados
- más ejemplos de uso abajo


## Registro de cambios v0.1.1
Misc
- estructura de la tabla sdelagate renombrada a sdelegate (errata)
- creación de parámetros de acción renombrada: requireClaim -> requireclaim
- parámetro de acción assetID renombrado en todas las acciones assetid

Activos en préstamo
- tabla sdelegate  - agregado nuevo campo: untildate
- agregado parámetro en acción de delegación untildate.  La acción hace una revisión sencilla si el parámetro fue introducido correctamente (ya sea cero o en el futuro).
- undelegate no funcionará hasta untildate (esto garantiza un término mínimo en préstamo del activo).
- permitir transferencia de vuelta de activos (retorno) si es delegado, antes de untiltime  (prestador tiene la opción de devolver antes de tiempo)

Procesamiento del lote
- acción claim: el parámetro assetid fue cambiado a matriz de assetsids. Lógica de tomares múltiples añadida.
- acción offer: el parámetro assetid fue cambiado a la matriz de assetsids. Lógica de ofertas múltiples añadida.
- acción canceloffer: el parámetro assetid fue cambiado a la matriz de assetsids. Lógica de cancelaciones múltiples añadida.
- acción transfer: el parámetro assetid fue cambiado a la matriz de assetsids. Lógica de transferencia de activos múltiple añadida.
- acción burn: el parámetro assetid fue cambiado a la matriz de assetsids. Lógica de quemar múltiple añadida.
- acción delegate/undelegate: el parámetro assetid fue cambiado a la matriz de assetsids. Lógica de delegation/undelegation  múltiple añadida.











