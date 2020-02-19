#include <stdio.h>
#include <stdlib.h>
#include <jCon.h>
#include <__jCon.h>


uint8_t __jsonIsNumber(const char *_str, size_t _len_str) {
	size_t index_str = 0;

	for (index_str = 0; index_str < _len_str; ++index_str)
	{
		if ((_str[index_str] < '0') || (_str[index_str] > '9'))
		{
			return 0;
		}
	}

	return 1;
}


const jsonObj_t *__jsonGetObjByPath(const jsonObj_t *_obj_json, const char *_str_path) {
	size_t index_str_path = 0;
	size_t len_str_key = 0;
	size_t index_count = 0;
	size_t len_str_path = 0;

	uint8_t flag_last_key = 0;
	uint8_t flag_read_key = 0;
	uint8_t flag_is_number = 0;

	long long index_element_array = 0;

	char buf[128] = {};

	const char *ptr_str_key = 0;

	const jsonObj_t *ptr_obj_json = NULL;

	if (_str_path == NULL)
	{
		return NULL;
	}

	len_str_path = strlen(_str_path);

	if ((_obj_json == NULL) || (len_str_path == 0))
	{
		return NULL;
	}

	for (;; ++index_str_path)
	{
		if ((flag_read_key == 0) && (_str_path[index_str_path] != ' '))
		{
			ptr_str_key = _str_path + index_str_path;
			flag_read_key = 1;
			len_str_key = 0;
		}

		if (flag_read_key == 1)
		{
			if ((index_str_path == len_str_path) ||
				((_str_path[index_str_path] == ':') && ((index_str_path != 0) && (_str_path[index_str_path - 1] != '\\'))))
			{
				if (index_str_path == len_str_path)
				{
					flag_last_key = 1;
				}

				flag_is_number = __jsonIsNumber(ptr_str_key, len_str_key);

				if (flag_is_number == 1)
				{
					snprintf(buf, 128, "%.*s", (int)len_str_key, ptr_str_key);
					index_element_array = atoll(buf);
				}

				{
					for (;; _obj_json = _obj_json->__ptr_next)
					{
						// Ищем элемент в массиве
						if (flag_is_number == 1)
						{
							if ((_obj_json->__type == JSON_VALUE_ARRAY) &&\
								(_obj_json->__size_value > index_element_array))
							{
								_obj_json = (jsonObj_t *)_obj_json->__value;

								for (index_count = 0; index_count != index_element_array; ++index_count,\
									_obj_json = _obj_json->__ptr_next);

								if ((_obj_json->__type == JSON_VALUE_OBJECT) && (flag_last_key == 0))
								{
									_obj_json = (jsonObj_t *)_obj_json->__value;
								}

								if (flag_last_key == 1)
								{
									ptr_obj_json = _obj_json;
								}

								break;
							}
						}
						else
						{
							if ((_obj_json->__str_key != NULL) &&\
								(strncmp(_obj_json->__str_key, ptr_str_key, len_str_key) == 0))
							{
								if ((_obj_json->__type == JSON_VALUE_OBJECT) && (flag_last_key == 0))
								{
									_obj_json = (jsonObj_t *)_obj_json->__value;
								}

								if (flag_last_key == 1)
								{
									ptr_obj_json = _obj_json;
								}

								break;
							}
						}

						if (_obj_json->__ptr_next == NULL)
						{
							break;
						}
					}
				}

				len_str_key = 0;

				if ((index_str_path != len_str_path))
				{
					if ((index_str_path + 1) != len_str_path)
					{
						ptr_str_key = _str_path + index_str_path + 1;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				continue;
			}
			else
			{
				len_str_key++;
			}
		}
	}

	return ptr_obj_json;
}


const char *__jsonGetNum(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const jsonObj_t *ptr_obj_json = NULL;

	if (_error != NULL)
	{
		*_error = JSON_OK;
	}

	ptr_obj_json = __jsonGetObjByPath(_obj_json, _str_path);

	if (ptr_obj_json != NULL)
	{
		if (ptr_obj_json->__type == JSON_VALUE_NUMBER)
		{
			return (const char *)ptr_obj_json->__value;
		}
		else
		{
			if (_error != NULL)
			{
				*_error = JSON_ERR_TYPE;
			}
		}
	}
	else
	{
		if (_error != NULL)
		{
			*_error = JSON_ERR_NOT_FOUND;
		}
	}

	return NULL;
}


const char *jsonGetStr(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const jsonObj_t *ptr_obj_json = NULL;

	if (_error != NULL)
	{
		*_error = JSON_OK;
	}

	ptr_obj_json = __jsonGetObjByPath(_obj_json, _str_path);

	if (ptr_obj_json != NULL)
	{
		if (ptr_obj_json->__type == JSON_VALUE_STR)
		{
			return (const char *)ptr_obj_json->__value;
		}
		else
		{
			if (_error != NULL)
			{
				*_error = JSON_ERR_TYPE;
			}
		}
	}
	else
	{
		if (_error != NULL)
		{
			*_error = JSON_ERR_NOT_FOUND;
		}
	}

	return NULL;
}


const char * jsonGetNumberAsStr(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	return __jsonGetNum(_obj_json, _str_path, _error);
}


int jsonGetInt(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const char *str_num = NULL;

	str_num = __jsonGetNum(_obj_json, _str_path, _error);

	if (str_num != NULL)
	{
		return atoi(str_num);
	}

	return 0;
}


long long jsonGetLL(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const char *str_num = NULL;

	str_num = __jsonGetNum(_obj_json, _str_path, _error);

	if (str_num != NULL)
	{
		return atoll(str_num);
	}

	return 0;
}


unsigned long int jsonGetUL(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const char *str_num = NULL;

	str_num = __jsonGetNum(_obj_json, _str_path, _error);

	if (str_num != NULL)
	{
		return strtoul(str_num, NULL, 10);
	}

	return 0;
}


float jsonGetFloat(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const char *str_num = NULL;

	str_num = __jsonGetNum(_obj_json, _str_path, _error);

	if (str_num != NULL)
	{
		return atof(str_num);
	}

	return 0;
}


jsonValueType_t jsonGetType(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const jsonObj_t *ptr_obj_json = NULL;

	if (_error != NULL)
	{
		*_error = JSON_OK;
	}

	ptr_obj_json = __jsonGetObjByPath(_obj_json, _str_path);

	if (ptr_obj_json != NULL)
	{
		return ptr_obj_json->__type;
	}
	else
	{
		if (_error != NULL)
		{
			*_error = JSON_ERR_NOT_FOUND;
		}
	}

	return JSON_VALUE_NONE;
}


uint8_t jsonGetBool(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const jsonObj_t *ptr_obj_json = NULL;

	if (_error != NULL)
	{
		*_error = JSON_OK;
	}

	ptr_obj_json = __jsonGetObjByPath(_obj_json, _str_path);

	if (ptr_obj_json != NULL)
	{
		if (ptr_obj_json->__type == JSON_VALUE_BOOL)
		{
			return *(uint8_t *)ptr_obj_json->__value;
		}
		else
		{
			if (_error != NULL)
			{
				*_error = JSON_ERR_TYPE;
			}
		}
	}
	else
	{
		if (_error != NULL)
		{
			*_error = JSON_ERR_NOT_FOUND;
		}
	}

	return 0;
}


const jsonObj_t *jsonGetObjInArray(const jsonObj_t *_obj_json, const char *_str_path, size_t _index, jsonError_t *_error) {
	size_t index_element_array = 0;

	const jsonObj_t *ptr_obj_json = NULL;
	const jsonObj_t *ptr_obj_array_json = NULL;

	if (_error != NULL)
	{
		*_error = JSON_OK;
	}

	if ((_str_path == NULL) && (_obj_json != NULL) && (_obj_json->__type == JSON_VALUE_ARRAY))
	{
		ptr_obj_array_json = (const jsonObj_t *)_obj_json->__value;

		if (ptr_obj_array_json == NULL)
		{
			return NULL;
		}

		for (index_element_array = 0; index_element_array < _obj_json->__size_value; ++index_element_array)
		{
			if (index_element_array == _index)
			{
				return ptr_obj_array_json;
			}

			ptr_obj_array_json = ptr_obj_array_json->__ptr_next;
		}
	}

	ptr_obj_json = __jsonGetObjByPath(_obj_json, _str_path);

	if (ptr_obj_json != NULL)
	{
		if (ptr_obj_json->__type == JSON_VALUE_ARRAY)
		{
			if (ptr_obj_json->__size_value <= _index)
			{
				if (_error != NULL)
				{
					*_error = JSON_ERR_OUT_OF_ARRAY;
				}
			}
			else
			{
				ptr_obj_array_json = (const jsonObj_t *)ptr_obj_json->__value;

				if (ptr_obj_array_json == NULL)
				{
					return NULL;
				}

				for (index_element_array = 0; index_element_array < ptr_obj_json->__size_value; ++index_element_array)
				{
					if (index_element_array == _index)
					{
						return ptr_obj_array_json;
					}

					ptr_obj_array_json = ptr_obj_array_json->__ptr_next;
				}
			}
		}
		else
		{
			if (_error != NULL)
			{
				*_error = JSON_ERR_TYPE;
			}
		}
	}
	else
	{
		if (_error != NULL)
		{
			*_error = JSON_ERR_NOT_FOUND;
		}
	}

	return NULL;
}


size_t jsonGetArraySize(const jsonObj_t *_obj_json, const char *_str_path, jsonError_t *_error) {
	const jsonObj_t *ptr_obj_json = NULL;

	if (_error != NULL)
	{
		*_error = JSON_OK;
	}

	if ((_str_path == NULL) && (_obj_json != NULL) && (_obj_json->__type == JSON_VALUE_ARRAY))
	{
		return _obj_json->__size_value;
	}

	ptr_obj_json = __jsonGetObjByPath(_obj_json, _str_path);

	if (ptr_obj_json != NULL)
	{
		if (ptr_obj_json->__type == JSON_VALUE_ARRAY)
		{
			return ptr_obj_json->__size_value;
		}
		else
		{
			if (_error != NULL)
			{
				*_error = JSON_ERR_TYPE;
			}
		}
	}
	else
	{
		if (_error != NULL)
		{
			*_error = JSON_ERR_NOT_FOUND;
		}
	}

	return 0;
}
