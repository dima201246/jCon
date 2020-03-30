#include <stdio.h>
#include <jCon.h>
#include <__jCon.h>


void __jsonGenTabs(size_t _num) {
	for (;_num != 0; --_num)
	{
		printf("\t");
	}
}


size_t __jsonObjDepth(const jsonObj_t *_obj_json) {
	size_t obj_depth = 0;

	if (_obj_json == NULL)
	{
		return 0;
	}

	for (;; _obj_json = _obj_json->__ptr_next)
	{
		obj_depth++;

		if (_obj_json->__ptr_next == NULL)
		{
			break;
		}
	}

	return obj_depth;
}


int __jsonGetSizeJson(const jsonObj_t *_obj_json, size_t *_size_json_str, jsonValueType_t _type_value) {
	if (_obj_json == NULL)
	{
		return -1;
	}


	if ((_type_value == JSON_VALUE_NONE) || (_type_value == JSON_VALUE_OBJECT))
	{
		*_size_json_str += 1;
	}

	for (;; _obj_json = _obj_json->__ptr_next)
	{
		if (_obj_json->__str_key != NULL)
		{
			*_size_json_str += strlen(_obj_json->__str_key) + 3;
		}

		switch (_obj_json->__type) {
			case JSON_VALUE_ARRAY: {
				*_size_json_str += 2;

				__jsonGetSizeJson((const jsonObj_t *)_obj_json->__value, _size_json_str, JSON_VALUE_ARRAY);
			}
			break;

			case JSON_VALUE_OBJECT: {
				if (_obj_json->__value == NULL)
				{
					*_size_json_str += 2;
				}
				else
				{
					__jsonGetSizeJson((const jsonObj_t *)_obj_json->__value, _size_json_str, JSON_VALUE_OBJECT);
				}
			}
			break;

			case JSON_VALUE_BOOL: {
				if (*(uint8_t *)_obj_json->__value == 1)
				{
					*_size_json_str += 4;
				}
				else
				{
					*_size_json_str += 5;
				}
			}
			break;

			case JSON_VALUE_NULL: {
				*_size_json_str += 4;
			}
			break;

			case JSON_VALUE_NUMBER: {
				*_size_json_str += _obj_json->__size_value;
			}
			break;

			default: {
				*_size_json_str += _obj_json->__size_value + 2;
			}
		}

		if (((_type_value == JSON_VALUE_NONE) ||\
			(_type_value == JSON_VALUE_ARRAY) ||\
			(_type_value == JSON_VALUE_OBJECT)) &&\
			(_obj_json->__ptr_next != NULL))
		{
			*_size_json_str += 1;
		}

		if (_obj_json->__ptr_next == NULL)
		{
			break;
		}
	}

	if ((_type_value == JSON_VALUE_NONE) || (_type_value == JSON_VALUE_OBJECT))
	{
		*_size_json_str += 1;
	}

	return 0;
}


void __jsonShowTree(const jsonObj_t *_obj_json, size_t _depth) {
	if (_obj_json == NULL)
	{
		return;
	}

	for (;; _obj_json = _obj_json->__ptr_next)
	{
		switch (_obj_json->__type) {
			case JSON_VALUE_ARRAY: {
				if (_obj_json->__str_key != NULL)
				{
					__jsonGenTabs(_depth);
					printf("Key: \"%s\"\n", _obj_json->__str_key);
				}

				__jsonGenTabs(_depth);
				printf("Size: %ld\n", _obj_json->__size_value);
				__jsonGenTabs(_depth);
				printf("Array: [\n");
				__jsonShowTree((const jsonObj_t *)_obj_json->__value, (_depth + 1));
				__jsonGenTabs(_depth);
				printf("]\n\n");
			}
			break;

			case JSON_VALUE_OBJECT: {
				if (_obj_json->__str_key != NULL)
				{
					__jsonGenTabs(_depth);
					printf("Key: \"%s\"\n", _obj_json->__str_key);
				}

				__jsonGenTabs(_depth);
				printf("Object: {\n");
				__jsonShowTree((const jsonObj_t *)_obj_json->__value, (_depth + 1));
				__jsonGenTabs(_depth);
				printf("}\n\n");
			}
			break;

			case JSON_VALUE_BOOL: {
				if (_obj_json->__str_key != NULL)
				{
					__jsonGenTabs(_depth);
					printf("Key: \"%s\"\n", _obj_json->__str_key);
				}

				__jsonGenTabs(_depth);
				printf("Bool: ");

				if (*(uint8_t *)_obj_json->__value == 1)
				{
					printf("true\n");
				}
				else
				{
					printf("false\n");
				}

				if (_obj_json->__ptr_next != NULL)
				{
					printf("\n");
				}
			}
			break;

			case JSON_VALUE_NULL: {
				if (_obj_json->__str_key != NULL)
				{
					__jsonGenTabs(_depth);
					printf("Key: \"%s\"\n", _obj_json->__str_key);
				}

				__jsonGenTabs(_depth);
				printf("NULL (wow it's is nothing!)\n");

				if (_obj_json->__ptr_next != NULL)
				{
					printf("\n");
				}
			}
			break;

			case JSON_VALUE_NUMBER: {
				if (_obj_json->__str_key != NULL)
				{
					__jsonGenTabs(_depth);
					printf("Key: \"%s\"\n", _obj_json->__str_key);
				}

				__jsonGenTabs(_depth);
				printf("Number: %s\n", (const char *)_obj_json->__value);

				if (_obj_json->__ptr_next != NULL)
				{
					printf("\n");
				}
			}
			break;

			default: {
				if (_obj_json->__str_key != NULL)
				{
					__jsonGenTabs(_depth);
					printf("Key: \"%s\"\n", _obj_json->__str_key);
				}

				__jsonGenTabs(_depth);
				printf("Value: \"%.*s\"\n", (int)_obj_json->__size_value, (char *)_obj_json->__value);
				__jsonGenTabs(_depth);
				printf("Type: %d\n", _obj_json->__type);

				if (_obj_json->__ptr_next != NULL)
				{
					printf("\n");
				}
			}
		}

		if (_obj_json->__ptr_next == NULL)
		{
			break;
		}
	}
}


char *__jsonGenStr(const jsonObj_t *_obj_json, jsonValueType_t _type_value, char *_ptr_to_str) {
	size_t size_json_str = 0;
	int res = 0;

	if (_obj_json == NULL)
	{
		return NULL;
	}

	if (_type_value == JSON_VALUE_NONE)
	{
		res = __jsonGetSizeJson(_obj_json, &size_json_str, JSON_VALUE_NONE);

		if (res != 0)
		{
			return NULL;
		}

		_ptr_to_str = (char *)Func_jsonCalloc((size_json_str + 1), sizeof(char));
	}

	if ((_type_value == JSON_VALUE_NONE) || (_type_value == JSON_VALUE_OBJECT))
	{
		strcat(_ptr_to_str, "{");
	}

	for (;; _obj_json = _obj_json->__ptr_next)
	{
		if (_obj_json->__str_key != NULL)
		{
			strcat(_ptr_to_str, "\"");
			strcat(_ptr_to_str, _obj_json->__str_key);
			strcat(_ptr_to_str, "\":");
		}

		switch (_obj_json->__type) {
			case JSON_VALUE_ARRAY: {
				strcat(_ptr_to_str, "[");
				__jsonGenStr((const jsonObj_t *)_obj_json->__value, JSON_VALUE_ARRAY, _ptr_to_str);
				strcat(_ptr_to_str, "]");
			}
			break;

			case JSON_VALUE_OBJECT: {
				if (_obj_json->__value == NULL)
				{
					strcat(_ptr_to_str, "{}");
				}
				else
				{
					__jsonGenStr((const jsonObj_t *)_obj_json->__value, JSON_VALUE_OBJECT, _ptr_to_str);
				}
			}
			break;

			case JSON_VALUE_BOOL: {
				if (*(uint8_t *)_obj_json->__value == 1)
				{
					strcat(_ptr_to_str, "true");
				}
				else
				{
					strcat(_ptr_to_str, "false");
				}
			}
			break;

			case JSON_VALUE_NULL: {
				strcat(_ptr_to_str, "null");
			}
			break;

			case JSON_VALUE_NUMBER: {
				strcat(_ptr_to_str, (const char *)_obj_json->__value);
			}
			break;

			default: {
				strcat(_ptr_to_str, "\"");
				strcat(_ptr_to_str, (const char *)_obj_json->__value);
				strcat(_ptr_to_str, "\"");
			}
		}

		if (((_type_value == JSON_VALUE_NONE) ||\
			(_type_value == JSON_VALUE_ARRAY) ||\
			(_type_value == JSON_VALUE_OBJECT)) &&\
			(_obj_json->__ptr_next != NULL))
		{
			strcat(_ptr_to_str, ",");
		}

		if (_obj_json->__ptr_next == NULL)
		{
			break;
		}
	}

	if ((_type_value == JSON_VALUE_NONE) || (_type_value == JSON_VALUE_OBJECT))
	{
		strcat(_ptr_to_str, "}");
	}

	return _ptr_to_str;
}


char *jsonGenStr(jsonObj_t *_obj_json) {
	return __jsonGenStr(_obj_json, JSON_VALUE_NONE, NULL);
}


jsonErr_t __jsonAdd(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, \
	const void *_value, size_t _size_value, size_t _index, jsonValueType_t _type_value)
{
	int res = 0;
	size_t index = 0;
	size_t len_str_key = 0;
	jsonObj_t *ptr_obj_json = NULL;

	if ((_type_value == JSON_VALUE_STR) && _value == NULL)
	{
		return JSON_ERR_ARGS;
	}

	if (_str_key != NULL)
	{
		len_str_key = strlen(_str_key);
	}

	// Обработка объекта состоящего из массива без ключа
	if ((*_obj_json != NULL) && ((*_obj_json)->__type == JSON_VALUE_ARRAY) && \
		((*_obj_json)->__str_key == NULL))
	{
		res = __jsonAddToObject((jsonObj_t **)&(*_obj_json)->__value, _str_key, len_str_key, _value, _size_value, _type_value);

		return JSON_OK;
	}

	if (((*_obj_json == NULL) && (_str_path == NULL)) || \
		((*_obj_json != NULL) && (_str_path == NULL)))
	{
		res = __jsonAddToObject(_obj_json, _str_key, len_str_key, _value, _size_value, _type_value);

		if (res != 0)
		{
			return JSON_ERR_UNKNOWN;
		}
	}
	else
	{
		ptr_obj_json = (jsonObj_t *)__jsonGetObjByPath(*_obj_json, _str_path);

		if (ptr_obj_json == NULL)
		{
			return JSON_ERR_NOT_FOUND;
		}

		if (((ptr_obj_json->__type != JSON_VALUE_OBJECT) && (ptr_obj_json->__type != JSON_VALUE_ARRAY)) || \
			((_index != 0) && (ptr_obj_json->__type != JSON_VALUE_ARRAY)))
		{
			return JSON_ERR_TYPE;
		}

		if (ptr_obj_json->__type == JSON_VALUE_ARRAY)
		{
			if (_index == 0)
			{
				// Не добавлять в массив ключ
				res = __jsonAddToObject((jsonObj_t **)&ptr_obj_json->__value, NULL, 0, _value, _size_value, _type_value);
			}
			else
			{
				if ((_index - 1) >= ptr_obj_json->__size_value)
				{
					return JSON_ERR_OUT_OF_ARRAY;
				}
				else
				{
					ptr_obj_json = (jsonObj_t *)ptr_obj_json->__value;

					for (index = 0; index != (_index - 1); ++index, ptr_obj_json = ptr_obj_json->__ptr_next);

					if (ptr_obj_json->__type != JSON_VALUE_OBJECT)
					{
						return JSON_ERR_TYPE;
					}

					return __jsonAdd((jsonObj_t **)&ptr_obj_json->__value, NULL, _str_key, _value, _size_value, 0, _type_value);
				}
			}
		}
		else
		{
			res = __jsonAddToObject((jsonObj_t **)&ptr_obj_json->__value, _str_key, len_str_key, _value, _size_value, _type_value);
		}

		if (res != 0)
		{
			return JSON_ERR_UNKNOWN;
		}

		if (ptr_obj_json->__type == JSON_VALUE_ARRAY)
		{
			ptr_obj_json->__size_value = __jsonObjDepth((const jsonObj_t *)ptr_obj_json->__value);
		}
	}

	return JSON_OK;
}


jsonErr_t jsonAddStr(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, const char *_str_value) {
	if (_str_value == NULL)
	{
		return JSON_ERR_ARGS;
	}

	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)_str_value, strlen(_str_value), 0, JSON_VALUE_STR);
}


jsonErr_t jsonAddObject(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key) {
	return __jsonAdd(_obj_json, _str_path, _str_key, NULL, 0, 0, JSON_VALUE_OBJECT);
}


jsonErr_t jsonAddNum(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, long long _int_value) {
	char buf[32] = {};

	sprintf(buf, "%lld", _int_value);

	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)buf, (strlen(buf) + 1), 0, JSON_VALUE_NUMBER);
}


jsonErr_t jsonAddNumByStr(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, const char *_str_value) {
	if (_str_value == NULL)
	{
		return JSON_ERR_ARGS;
	}

	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)_str_value, strlen(_str_value), 0, JSON_VALUE_NUMBER);
}


jsonErr_t jsonAddArray(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key) {
	return __jsonAdd(_obj_json, _str_path, _str_key, NULL, 0, 0, JSON_VALUE_ARRAY);
}


jsonErr_t jsonAddBool(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key, uint8_t _bool_value) {
	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)&_bool_value, 1, 0, JSON_VALUE_BOOL);
}


jsonErr_t jsonAddNull(jsonObj_t **_obj_json, const char *_str_path, const char *_str_key) {
	return __jsonAdd(_obj_json, _str_path, _str_key, NULL, 0, 0, JSON_VALUE_NULL);
}


jsonErr_t jsonAddStrToArray(jsonObj_t **_obj_json, const char *_str_path, size_t _index, const char *_str_key, const char *_str_value) {
	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)_str_value, strlen(_str_value), (_index + 1), JSON_VALUE_STR);
}


jsonErr_t jsonAddBoolToArray(jsonObj_t **_obj_json, const char *_str_path, size_t _index, const char *_str_key, uint8_t _bool_value) {
	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)&_bool_value, 1, (_index + 1), JSON_VALUE_BOOL);
}


jsonErr_t jsonAddNumToArray(jsonObj_t **_obj_json, const char *_str_path, size_t _index, const char *_str_key, long long _int_value) {
	char buf[32] = {};

	sprintf(buf, "%lld", _int_value);

	return __jsonAdd(_obj_json, _str_path, _str_key, (const void *)buf, (strlen(buf) + 1), (_index + 1), JSON_VALUE_NUMBER);
}


void jsonShowTree(const jsonObj_t *_obj_json) {
	__jsonShowTree(_obj_json, 0);
}
