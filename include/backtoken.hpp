
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <SimpleAssets.hpp>

using namespace eosio;
using namespace std;

class[[eosio::contract("backtoken")]] backtoken : public contract{
   public:
	using contract::contract;
	struct symbol_ex;
	struct token_ex;

	backtoken(name receiver, name code, datastream<const char*> ds)
	  : contract(receiver, code, ds) {}

	ACTION tokenprep(name owner, uint64_t nft_id, const vector<backtoken::symbol_ex>& symbols);
	using tokenprep_action = action_wrapper< "tokenprep"_n, &backtoken::tokenprep >;
#ifdef DEBUG
	ACTION delbtoken(uint64_t nft_id);
	using delbtoken_action = action_wrapper< "delbtoken"_n, &backtoken::delbtoken >;
#endif

	ACTION onburn(name owner, uint64_t nft_id);
	using onburn_action = action_wrapper< "onburn"_n, &backtoken::onburn >;

	ACTION getversion();
	using getversion_action = action_wrapper<"getversion"_n, &backtoken::getversion>;

	[[eosio::on_notify("simpleassets::transfer")]] void ontransfersa(name from, name to, vector<uint64_t>& assetids, string memo);
	[[eosio::on_notify("*::transfer")]] void ontransfer(name from, name to, asset quantity, string memo);
	[[eosio::on_notify("simpleassets::transferf")]] void ontransferf(name from, name to, name author, asset quantity, string memo);

	void send_tokens(name owner, const vector<backtoken::token_ex>& tokens, string memo);
	void process_memo(name from, name to, const vector<backtoken::token_ex> & tokens, const string& memo);

public:
	enum token_type { SA_NFT = 0, SA_FT = 1, TOKEN = 2 };

	typedef uint64_t token_type_t;

	struct symbol_ex
	{
		extended_symbol symbol_ex;
		token_type_t token_type;
		
		name get_contract() const 
		{
			return symbol_ex.get_contract();
		}

		symbol get_symbol() const
		{
			return symbol_ex.get_symbol();
		}
	};

	struct token_ex
	{
		uint64_t token_id;
		extended_asset quantity_ex;
		token_type_t token_type;

		symbol get_symbol() const
		{
			return quantity_ex.quantity.symbol;
		}

		bool is_token_valid() const
		{
			return token_id != 0;
		}

		uint64_t get_amount() const
		{
			return quantity_ex.quantity.amount;
		}

		asset get_quantity() const
		{
			return quantity_ex.quantity;
		}

		name get_contract() const
		{
			return quantity_ex.contract;
		}

		bool operator==(token_ex const & rhs) const
		{
			auto result = false;
			if (this->token_type == rhs.token_type)
			{
				if (this->is_nft())
				{
					result = this->token_id == rhs.token_id;
				}
				else if (this->is_ft() || this->is_token())
				{
					result = this->get_contract() == rhs.get_contract() && this->get_quantity() == rhs.get_quantity();
				}
			}
			return result;
		}

		void prepare_tokenex(const symbol_ex& sym)
		{
			switch (sym.token_type)
			{
				case SA_NFT:
				{
					set_nft(0);
					break;
				}
				case SA_FT:
				{
					extended_asset ext_asset{ 0, sym.symbol_ex };
					set_ft(ext_asset);
					break;
				}
				case TOKEN:
				{
					extended_asset ext_asset{ 0, sym.symbol_ex };
					set_token(ext_asset);
					break;
				}
			}
		}

		void set_token(extended_asset token_quantity_ex)
		{
			token_id = 0;
			token_type = TOKEN;
			quantity_ex = token_quantity_ex;
		}

		void set_token(name contract, asset quantity)
		{
			token_id = 0;
			token_type = TOKEN;
			quantity_ex.contract = contract;
			quantity_ex.quantity = quantity;
		}

		void set_ft(extended_asset ft_quantity_ex)
		{
			token_id = 0;
			token_type = SA_FT;
			quantity_ex = ft_quantity_ex;
		}

		void set_ft(name ft_author, asset ft_quantity)
		{
			token_id = 0;
			token_type = SA_FT;
			quantity_ex.contract = ft_author;
			quantity_ex.quantity = ft_quantity;
		}

		void set_nft(uint64_t tokenid)
		{
			token_id = tokenid;
			token_type = SA_NFT;
			quantity_ex = extended_asset();
		}

		bool is_ft() const
		{
			return (token_type == SA_FT);
		}

		bool is_token() const
		{
			return (token_type == TOKEN);
		}

		bool is_nft() const
		{
			return (token_type == SA_NFT);
		}

		string to_print_type() const
		{
			string result;
			switch (token_type)
			{
				case SA_NFT:
				{
					result = "SA_NFT";
					break;
				}
				case SA_FT:
				{
					result = "SA_FT";
					break;
				}
				case TOKEN:
				{
					result = "TOKEN";
					break;
				}
			}
			return result;
		}

		static vector<token_ex> token_to_tokenexs(name author, asset quantity) {

			vector<token_ex> result;
			token_ex token;
			token.set_token(author, quantity);
			result.emplace_back(token);

			return result;
		}

		static vector<token_ex> ft_to_tokenexs(name author, asset quantity) {

			vector<token_ex> result;
			token_ex token;
			token.set_ft(author, quantity);
			result.emplace_back(token);

			return result;
		}

		static vector<token_ex> nfts_to_tokenexs(vector<uint64_t>& assetids) {
			vector<token_ex> result;

			for (auto i = 0; i < assetids.size(); i++) {
				token_ex token;
				token.set_nft(assetids[i]);
				result.emplace_back(token);
			}

			return result;
		}

	};

		/*
		* Back token table
	*/
	TABLE sbacktoken{
		uint64_t	nft_id;
		vector<token_ex>	tokens;

		auto primary_key() const {
			return nft_id;
		}

		void prepare_tokens(const vector<symbol_ex>& symbols)
		{
			for (auto it_symbol_ex = symbols.begin(); it_symbol_ex != symbols.end(); it_symbol_ex++)
			{
				const auto& it = find_if(tokens.begin(), tokens.end(),
					[&](const token_ex& one_token) { return one_token.get_contract() == it_symbol_ex->get_contract()
					&& one_token.get_symbol() == it_symbol_ex->get_symbol()
					&& one_token.token_type == it_symbol_ex->token_type; });

				if (it == tokens.end()){
					token_ex tkn;
					tkn.prepare_tokenex(*it_symbol_ex);
					tokens.emplace_back(tkn);
				}
			}
		}

		void append_token(const token_ex& token)
		{
			if (token.token_type == SA_FT || token.token_type == TOKEN)
			{
				const auto author = token.quantity_ex.contract;
				const auto symbol = token.quantity_ex.quantity.symbol;
				const auto token_type = token.token_type;

				const auto& it = find_if(tokens.begin(), tokens.end(),
					[&](const token_ex& one_token) { return one_token.quantity_ex.contract == author
					&& one_token.get_symbol() == symbol
					&& one_token.token_type == token_type; });

				if (it != tokens.end())
				{
					it->quantity_ex.quantity.amount += token.quantity_ex.quantity.amount;
				}
				else
				{
					check(false, 
						"Prepare tokens before append. Tokenex for contract: " + author.to_string()
						+ " symbol : " + symbol.code().to_string() + " type : " + token.to_print_type() + " does not exist ");
				}

			}
			else if (token.token_type == SA_NFT)
			{
				const auto contract = token.quantity_ex.contract;
				const auto token_type = token.token_type;

				const auto& it = find_if(tokens.begin(), tokens.end(),
					[&](const token_ex& one_token) { return one_token.get_contract() == contract
					&& one_token.token_id == 0
					&& one_token.token_type == token_type; });

				if (it != tokens.end())
				{
					it->token_id = token.token_id;
				}
				else
				{
					check(false, "Prepare tokens before append. Token with type SA_NFT and token_id = 0 does no exist");
				}
			}
		}

		void append_tokens(const vector<token_ex>& append_tokens)
		{
			for (auto it_append_token = append_tokens.begin(); it_append_token != append_tokens.end(); it_append_token++)
			{
				append_token(*it_append_token);
			}
		}

	};
	typedef eosio::multi_index< "backtokens"_n, sbacktoken > backtokens;

	backtokens backtokent = { _self, _self.value };

};