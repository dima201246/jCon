#ifndef _JCON
#define _JCON

	#include <stdint.h>
	#include <string.h>
	#include <stddef.h>

	typedef enum {
		JSON_OK,
		JSON_ERR_OPEN_FILE,
		JSON_ERR_BAD_JSON,
		JSON_ERR_INTERNAL,
		JSON_ERR_ARGS,
		JSON_ERR_MEMORY,
		JSON_ERR_TYPE,
		JSON_ERR_NOT_FOUND,
		JSON_ERR_OUT_OF_ARRAY,
		JSON_ERR_UNKNOWN
	} jsonErr_t;

	typedef enum {
		JSON_VALUE_NONE,
		JSON_VALUE_STR,
		JSON_VALUE_NUMBER,
		JSON_VALUE_OBJECT,
		JSON_VALUE_ARRAY,
		JSON_VALUE_BOOL,
		JSON_VALUE_NULL,
		JSON_VALUE_UNKNOWN
	} jsonValueType_t;

	typedef struct jsonObj_t {
		jsonValueType_t __type;
		const char *__str_key;
		void *__value;
		size_t __size_value;

		struct jsonObj_t *__ptr_next;
	} jsonObj_t;

	void jsonGetVer(int *_int_major, int *_int_minor, int *_int_micro);

	void jsonShowTree(const jsonObj_t *_obj_json);
	void jsonSetFuncRealloc(void *(*_func_calloc)(size_t _num, size_t _size),
		char *(*_func_strndup)(const char *_str, size_t _len));
	void jsonFree(jsonObj_t *_obj_json);
	jsonObj_t *jsonLoad(const char *_str_json, jsonErr_t *_error);

	const char *jsonGetStr(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	const char * jsonGetNumberAsStr(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	int jsonGetInt(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	long long jsonGetLL(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	jsonValueType_t jsonGetType(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	unsigned long int jsonGetUL(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	float jsonGetFloat(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	uint8_t jsonGetBool(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	const jsonObj_t *jsonGetObj(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);

	const jsonObj_t *jsonOpenObj(const jsonObj_t *_obj_json, jsonErr_t *_error);
	size_t jsonGetArraySize(const jsonObj_t *_obj_json, const char *_str_path, jsonErr_t *_error);
	const jsonObj_t *jsonGetObjInArray(const jsonObj_t *_obj_json, const char *_str_path, size_t _index, jsonErr_t *_error);

	char *jsonGenStr(jsonObj_t *_obj_json);

	const char *jsonStrErr(jsonErr_t _err);

	/*	Дубликатор объекта
			Описание:
				Дубликатор JSON-объекта

			Параметры:
				- jsonObj_t *_obj_json - JSON-объекта который необходимо дублировать

			Возвращаемое значение:
				Указатель на новый JSON-объект
	*/
	jsonObj_t *jsonDup(jsonObj_t *_obj_json);

	/*Add*/
	jsonErr_t jsonAddObject(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key);
	jsonErr_t jsonAddArray(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key);
	
	jsonErr_t jsonAddBool(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, uint8_t _bool_value);
	jsonErr_t jsonAddStr(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, const char *_str_value);
	jsonErr_t jsonAddNum(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, long long _int_value);
	jsonErr_t jsonAddNumByStr(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, const char *_str_value);
	jsonErr_t jsonAddNull(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key);
#endif
