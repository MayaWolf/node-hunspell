#include <node.h>
#include <functional>
#include <node_object_wrap.h>
#include <uv.h>
#include "node_modules/hunspell/src/hunspell/hunspell.hxx"

using namespace v8;


class Spellchecker : public node::ObjectWrap {
public:
	static void Init(Local<FunctionTemplate> tpl) {
		NODE_SET_PROTOTYPE_METHOD(tpl, "add", [](auto args){ add(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "addDictionary", [](auto args){ add_dic(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "addWithAffix", [](auto args){ add_with_affix(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "analyze", [](auto args){ analyze(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "generate", [](auto args){ generate(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "remove", [](auto args){ remove(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "spell", [](auto args){ spell(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "stem", [](auto args){ stem(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "suffixSuggest", [](auto args){ suffix_suggest(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "suggest", [](auto args){ suggest(args, Execute); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "addSync", [](auto args){ add(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "addDictionarySync", [](auto args){ add_dic(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "addWithAffixSync", [](auto args){ add_with_affix(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "analyzeSync", [](auto args){ analyze(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "generateSync", [](auto args){ generate(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "removeSync", [](auto args){ remove(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "spellSync", [](auto args){ spell(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "stemSync", [](auto args){ stem(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "suffixSuggestSync", [](auto args){ suffix_suggest(args, ExecuteSync); });
		NODE_SET_PROTOTYPE_METHOD(tpl, "suggestSync", [](auto args){ suggest(args, ExecuteSync); });
	}

	static void New(const FunctionCallbackInfo<Value> &args) {
		Isolate* isolate = args.GetIsolate();
		HandleScope scope(isolate);
		int argl = args.Length();
		if(!args.IsConstructCall()) {
			isolate->ThrowException(Exception::Error(
					String::NewFromUtf8(isolate, "Use the new operator to create an instance of this object.")));
			return;
		}
		if(argl < 2) {
			isolate->ThrowException(
					Exception::Error(String::NewFromUtf8(isolate, "Constructor requires two arguments.")));
			return;
		}

		auto obj = new Spellchecker(args);
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}

private:
	Hunspell* hun;
	explicit Spellchecker(const FunctionCallbackInfo<Value> &args) {
		hun = new Hunspell(*String::Utf8Value(args[0]), *String::Utf8Value(args[1]));
	}

	~Spellchecker() override {
		delete hun;
	}

	static void add(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<int(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		auto word = *String::Utf8Value(args[0]);
		execute(args, [word](auto self) { return self->hun->add(word); });
	}

	static void add_dic(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<int(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		auto path = *String::Utf8Value(args[0]);
		execute(args, [path](auto self) { return self->hun->add_dic(path); });
	}

	static void add_with_affix(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<int(Spellchecker*)>)) {
		if(!CheckArgs(args, 2)) return;
		std::string word = *String::Utf8Value(args[0]), example = *String::Utf8Value(args[1]);
		execute(args, [word, example](auto self) { return self->hun->add_with_affix(word, example); });
	}

	static void analyze(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<std::vector<std::string>(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		std::string word = *String::Utf8Value(args[0]);
		execute(args, [word](auto self) { return self->hun->analyze(word); });
	}

	static void generate(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<std::vector<std::string>(Spellchecker*)>)) {
		if(!CheckArgs(args, 2)) return;
		std::string word = *String::Utf8Value(args[0]);
		std::string* word2;
		std::vector<std::string>* pl;
		if(args[1]->IsArray()) {
			auto vector = ToVector(args[1].As<Array>());
			pl = &vector;
		} else {
			std::string str = *String::Utf8Value(args[1]);
			word2 = &str;
		}
		execute(args, [word, word2, pl](auto self) {
			return word2 != nullptr ? self->hun->generate(word, *word2) : self->hun->generate(word, *pl);
		});
	}

	static void remove(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<int(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		std::string word = *String::Utf8Value(args[0]);
		execute(args, [word](auto self) {
			return self->hun->remove(word);
		});
	}

	static void spell(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<bool(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		std::string word = *String::Utf8Value(args[0]);
		execute(args, [word](auto self) {
			return self->hun->spell(word);
		});
	}

	static void stem(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<std::vector<std::string>(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		std::string* word;
		std::vector<std::string>* pl;
		if(args[0]->IsArray()) {
			auto vector = ToVector(args[0].As<Array>());
			pl = &vector;
		} else {
			std::string str = *String::Utf8Value(args[0]);
			word = &str;
		}
		execute(args, [word, pl](auto self) {
			return word != nullptr ? self->hun->stem(*word) : self->hun->stem(*pl);
		});
	}

	static void suffix_suggest(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<std::vector<std::string>(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		std::string word = *String::Utf8Value(args[0]);
		execute(args, [word](auto self) { return self->hun->suffix_suggest(word); });
	}

	static void suggest(const FunctionCallbackInfo<Value> &args, void (* execute)(const FunctionCallbackInfo<Value>&, std::function<std::vector<std::string>(Spellchecker*)>)) {
		if(!CheckArgs(args, 1)) return;
		std::string word = *String::Utf8Value(args[0]);
		execute(args, [word](auto self) { return self->hun->suggest(word); });
	}

	static bool CheckArgs(const FunctionCallbackInfo<Value> &args, int minArgs) {
		auto isolate = args.GetIsolate();
		if(args.Length() < minArgs) {
			isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Too few arguments provided.")));
			return false;
		}
		return true;
	}

	template<typename T>
	static void Execute(const FunctionCallbackInfo<Value> &args, std::function<T(Spellchecker*)> body) {
		auto spellchecker = ObjectWrap::Unwrap<Spellchecker>(args.Holder());
		struct dataT {
			T ret;
			Persistent<v8::Promise::Resolver>* persistent;
			std::function<T()> body;
		};
		auto isolate = args.GetIsolate();
		auto resolver = v8::Promise::Resolver::New(isolate);
		auto req = new uv_work_t;
		auto data = new dataT;
		data->persistent = new Persistent<v8::Promise::Resolver>(isolate, resolver);
		data->body = [body, spellchecker]() { return body(spellchecker); };
		req->data = data;
		args.GetReturnValue().Set(resolver->GetPromise());
		uv_queue_work(uv_default_loop(), req, [](auto req) {
			auto data = static_cast<dataT*>(req->data);
			auto ret = data->body();
			data->ret = ret;
		}, [](auto req, auto status) {
			auto isolate = Isolate::GetCurrent();
			HandleScope scope(isolate);
			auto data = static_cast<dataT*>(req->data);
			data->persistent->Get(isolate)->Resolve(GetValue(data->ret));
			delete data->persistent;
			delete data;
			delete req;
		});
	}

	template<typename T>
	static void ExecuteSync(const FunctionCallbackInfo<Value> &args, std::function<T(Spellchecker*)> body) {
		auto spellchecker = ObjectWrap::Unwrap<Spellchecker>(args.Holder());
		auto ret = body(spellchecker);
		args.GetReturnValue().Set(GetValue(ret));
	}

	static Local<Value> GetValue(int value) {
		return Integer::New(Isolate::GetCurrent(), value);
	}

	static Local<Value> GetValue(bool value) {
		return Boolean::New(Isolate::GetCurrent(), value);
	}

	static Local<Value> GetValue(std::vector<std::string> &vector) {
		auto isolate = Isolate::GetCurrent();
		auto array = Array::New(isolate, vector.size());
		for(auto i = 0; i < vector.size(); ++i)
			array->Set(i, String::NewFromUtf8(isolate, vector[i].c_str()));
		return array;
	}

	static std::vector<std::string> ToVector(Local<Array> array) {
		auto length = array->Length();
		auto vector = std::vector<std::string>(length);
		for(size_t i = 0; i < length; ++i)
			vector[i] = *String::Utf8Value(array->Get(i));
		return vector;
	}
};

void Init(Handle<Object> exports, Handle<Object> module) {
	Isolate* isolate = Isolate::GetCurrent();
	auto tpl = FunctionTemplate::New(isolate, Spellchecker::New);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	Spellchecker::Init(tpl);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Hunspell"));
	exports->Set(String::NewFromUtf8(isolate, "default"), tpl->GetFunction());
}

NODE_MODULE(hunspell, Init);