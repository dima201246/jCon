#ifndef ___JCON
#define ___JCON

	extern void *(*Func_jsonCalloc)(size_t _num, size_t _size);
	extern char *(*Func_jsonStrndup)(const char *_str, size_t _len);


	int __jsonAddToObject(jsonObj_t **_obj_json,\
		const char *_str_key, size_t _len_str_key, \
		const void *_value, size_t _size_value,
		jsonValueType_t _type_obj);
	size_t __jsonObjDepth(const jsonObj_t *_obj_json);
	const jsonObj_t *__jsonGetObjByPath(const jsonObj_t *_obj_json, const char *_str_path);
#endif
