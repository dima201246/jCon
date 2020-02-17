#include <stdio.h>
#include <stdlib.h>
#include <jCon.h>
#include <__jCon.h>


void *(*Func_jsonCalloc)(size_t _num, size_t _size) = calloc;
char *(*Func_jsonStrndup)(const char *_str, size_t _len) = strndup;


void __jsonStrUp(char *_str) {
	size_t index_str = 0;

	for (; _str[index_str] != '\0'; ++index_str)
	{
		if ((_str[index_str] >= 'a') && (_str[index_str] <= 'z'))
		{
			_str[index_str] += 'A' - 'a';
		}
	}
}


void __jsonRemoveSpaces(char *_str) {
	size_t index_str = 0;

	for (; _str[index_str] != '\0';)
	{
		if (_str[index_str] == ' ')
		{
			memcpy(_str, (_str + 1), strlen(_str) - 1);
			continue;
		}
		else
		{
			break;
		}

		index_str++;
	}

	for (index_str = (strlen(_str) - 1); index_str != 0; --index_str)
	{
		if (_str[index_str] == ' ')
		{
			_str[index_str] = '\0';
		}
		else
		{
			break;
		}
	}
}


uint8_t __jsonIsNumeric(const char *_str, size_t _len_str) {
	size_t index_str = 0;

	uint8_t flag_found_dot = 0;

	for (index_str = 0; index_str < _len_str; ++index_str)
	{
		if ((_str[index_str] < '0') || (_str[index_str] > '9'))
		{
			if ((index_str == 0) && ((_str[index_str] == '+') || (_str[index_str] == '-')))
			{
				continue;
			}

			if ((flag_found_dot == 0) && (_str[index_str] == '.'))
			{
				flag_found_dot = 1;

				continue;
			}

			return 0;
		}
	}

	return 1;
}


int __jsonAddToObject(jsonObj_t **_obj_json, const char *_str_key, size_t _len_str_key, \
	const void *_value, size_t _size_value, jsonValueType_t _type_obj)
{
	char buf[128] = {};

	uint8_t flag_not_copy = 0;

	jsonObj_t *ptr_obj_json = NULL;

	if (*_obj_json == NULL)
	{
		*_obj_json = (jsonObj_t *)Func_jsonCalloc(1, sizeof(jsonObj_t));

		if (*_obj_json == NULL)
		{
			return -1;
		}

		// (*_obj_json)->__ptr_head = *_obj_json;
		ptr_obj_json = (*_obj_json);
	}
	else
	{
		ptr_obj_json = *_obj_json;

		for (; ptr_obj_json->__ptr_next != NULL; ptr_obj_json = ptr_obj_json->__ptr_next);

		ptr_obj_json->__ptr_next = (jsonObj_t *)Func_jsonCalloc(1, sizeof(jsonObj_t));

		if (ptr_obj_json->__ptr_next == NULL)
		{
			return -2;
		}

		// ptr_obj_json->__ptr_next->__ptr_head = *_obj_json;
		ptr_obj_json = ptr_obj_json->__ptr_next;
	}

	if (_str_key != NULL)
	{
		ptr_obj_json->__str_key = Func_jsonStrndup(_str_key, _len_str_key);

		if (ptr_obj_json->__str_key == NULL)
		{
			free(ptr_obj_json);
			return -3;
		}
	}

	if (_value != NULL)
	{
		if (_type_obj == JSON_VALUE_ARRAY)
		{
			ptr_obj_json->__value = Func_jsonCalloc(sizeof(jsonObj_t), sizeof(void));
		}
		else
		{
			if (_type_obj == JSON_VALUE_UNKNOWN)
			{
				if (_size_value < 128)
				{
					snprintf(buf, 128, "%.*s", (int)_size_value, (const char *)_value);
					__jsonRemoveSpaces(buf);

					_size_value = strlen(buf);

					if ((_size_value >= 4) && (_size_value <= 5))
					{
						__jsonStrUp(buf);

						if (strcmp(buf, "TRUE") == 0)
						{
							flag_not_copy = 1;
							ptr_obj_json->__value = Func_jsonCalloc(1, sizeof(uint8_t));
							*(uint8_t *)ptr_obj_json->__value = 1;
							_type_obj = JSON_VALUE_BOOL;
							_size_value = 1;
						}

						if (strcmp(buf, "FALSE") == 0)
						{
							flag_not_copy = 1;
							ptr_obj_json->__value = Func_jsonCalloc(1, sizeof(uint8_t));
							*(uint8_t *)ptr_obj_json->__value = 0;
							_type_obj = JSON_VALUE_BOOL;
							_size_value = 1;
						}

						if (strcmp(buf, "NULL") == 0)
						{
							flag_not_copy = 1;
							ptr_obj_json->__value = Func_jsonCalloc(1, sizeof(void));	// Я не знаю почему, но без этого всё ломается...
							_type_obj = JSON_VALUE_NULL;
							_size_value = 0;
						}
					}
					else
					{
						if (__jsonIsNumeric(buf, strlen(buf)) == 1)
						{
							flag_not_copy = 1;
							ptr_obj_json->__value = Func_jsonStrndup(buf, _size_value);
							_type_obj = JSON_VALUE_NUMBER;
						}
						else
						{
							ptr_obj_json->__value = Func_jsonCalloc(sizeof(jsonObj_t), sizeof(void));
						}
					}
				}
			}
			else
			{
				if (_type_obj == JSON_VALUE_STR)
				{
					ptr_obj_json->__value = Func_jsonCalloc(_size_value + 1, sizeof(void));
				}
				else
				{
					ptr_obj_json->__value = Func_jsonCalloc(_size_value, sizeof(void));
				}
			}
		}

		if (ptr_obj_json->__value == NULL)
		{
			free(ptr_obj_json);
			return -4;
		}

		if (_type_obj ==  JSON_VALUE_ARRAY)
		{
			memcpy(ptr_obj_json->__value, _value, sizeof(jsonObj_t));
		}
		else
		{
			if (flag_not_copy == 0)
			{
				memcpy(ptr_obj_json->__value, _value, _size_value);
			}
		}

		ptr_obj_json->__size_value = _size_value;
	}

	ptr_obj_json->__type = _type_obj;
	// ptr_obj_json = ptr_obj_json->__ptr_head;

	return 0;
}


// Функция для исправления указателя на голову списка
/*int __jsonFixHead(jsonObj_t *_obj_json, jsonObj_t *_obj_json_head) {
	if (_obj_json == NULL)
	{
		return -1;
	}

	if (_obj_json_head == NULL)
	{
		return -2;
	}

	for (;;)
	{
		_obj_json->__ptr_head = _obj_json_head;

		if (_obj_json->__ptr_next != NULL)
		{
			_obj_json = _obj_json->__ptr_next;
		}
		else
		{
			break;
		}
	}

	return 0;
}*/


jsonObj_t *__jsonLoad(const char *_str_json, size_t _len_str_json, jsonError_t *_error) {
	jsonObj_t *obj_json = NULL;
	jsonObj_t *obj_json_children = NULL;	// Тут будет зиждется объект
	jsonObj_t *obj_json_tmp = NULL;	// Тут будет зиждется объект

	size_t index_in_json_str = 0;
	size_t len_key = 0;				// Размер извлекаемого ключа
	size_t len_value = 0;			// Размер извлекаемого значения
	size_t count_hooks = 0;			// Счётчик скобок, чтобы игнорировать их при чтении объекта

	uint8_t flag_found_separator = 0;		// Флаг чтения ключа
	uint8_t flag_found_start = 0;	// Флаг начало JSON-объекта
	// uint8_t flag_found_end = 0;		// Флаг окончания JSON-объекта
	uint8_t flag_read_key = 0;		// Флаг чтения ключа
	uint8_t flag_read_force_read = 0;	// Флаг-костыль для ситуаций, когда число последнее в массиве
	uint8_t flag_read_value = 0;	// Флаг чтения значения
	uint8_t flag_read_array = 0;	// Флаг чтения и обработки массива
	uint8_t flag_want_value = 0;	// Флаг ожидания значения
									// 	(выставляется после успешно прочитанного ключа)

	int res = 0;

	jsonValueType_t type_expected_value = JSON_VALUE_NONE;	// Ожидаемы тип считываемого значения

	char chr_open = '\0';
	char chr_close = '\0';

	const char *ptr_key = NULL;		// Указатель на начало извлекаемого ключа
	const char *ptr_value = NULL;	// Указатель на начало извлекаемого значения

	for (index_in_json_str = 0; index_in_json_str < _len_str_json; ++index_in_json_str)
	{
		// Если начало JSON-объекта не найдено, то пропускать
		if (flag_found_start == 0)
		{
			// Поиск начала JSON-объекта
			if (_str_json[index_in_json_str] == '{')
			{
				flag_found_start = 1;
			}

			if (_str_json[index_in_json_str] == '[')
			{
				flag_found_start = 1;
				flag_read_array = 1;
				flag_want_value = 1;
				flag_found_separator = 1;	// Сразу после знака "[" ожидается значение
			}

			continue;
		}

		// Обработка ключа 
		if ((flag_read_key == 0) &&\
			(flag_read_value == 0) &&\
			(flag_want_value == 0) &&\
			(flag_read_array == 0))
		{
			if (((_str_json[index_in_json_str] == '\"') || (_str_json[index_in_json_str] == '\'')))
			{
				chr_close = _str_json[index_in_json_str];
				flag_read_key = 1;	// Флаг начало чтения ключа

				if ((index_in_json_str + 1) != _len_str_json)
				{
					ptr_value = (const char *)(_str_json + index_in_json_str + 1);
					len_value = 1;
				}
				else
				{
					jsonFree(obj_json);
					return (NULL);
				}
			}

			continue;
		}

		// Обработка значения
		if ((flag_want_value == 1) && (flag_read_value == 0))
		{
			// Поиск разделителя ключа и значения
			if (flag_found_separator == 0)
			{
				if ((_str_json[index_in_json_str] == ']') && (flag_read_array == 1))
				{
					// flag_found_end = 1;
					res = __jsonAddToObject(&obj_json, NULL, 0,\
						(const void *)obj_json_children, sizeof(jsonObj_t),\
						JSON_VALUE_ARRAY);

					if (res == 0)
					{
						obj_json->__size_value = __jsonObjDepth(obj_json_children);
					}
					else
					{
						printf("asdfjanksjfksfng\n");
					}

					free(obj_json_children);

					break;
				}

				if (((_str_json[index_in_json_str] == ':') && (flag_read_array == 0)) ||\
					((_str_json[index_in_json_str] == ',') && (flag_read_array == 1)))
				{
					flag_found_separator = 1;

					continue;
				}
				else
				{
					continue;
				}
			}
			else
			{
				if (_str_json[index_in_json_str] == ' ')
				{
					continue;
				}
				else
				{
					len_value = 1;
					flag_read_value = 1;
					ptr_value = (const char *)(_str_json + index_in_json_str);

					// Поиск объекта
					if (_str_json[index_in_json_str] == '{')
					{
						#ifdef DEBUG
							printf("Found object\n");
						#endif

						type_expected_value = JSON_VALUE_OBJECT;
						chr_open = '{';
						chr_close = '}';

						continue;
					}

					// Поиск массива
					if (_str_json[index_in_json_str] == '[')
					{
						#ifdef DEBUG
							printf("Found array\n");
						#endif

						type_expected_value = JSON_VALUE_ARRAY;
						chr_open = '[';
						chr_close = ']';

						continue;
					}

					if ((_str_json[index_in_json_str] == '\"') ||\
						(_str_json[index_in_json_str] == '\''))
					{
						#ifdef DEBUG
							printf("Found string\n");
						#endif

						type_expected_value = JSON_VALUE_STR;
						chr_close = _str_json[index_in_json_str];

						if ((index_in_json_str + 1) != _len_str_json)
						{
							ptr_value = (const char *)(_str_json + index_in_json_str + 1);
						}
						else
						{
							jsonFree(obj_json);
							return NULL;
							// TODO: ошибка чтения значения
						}

						continue;
					}

					#ifdef DEBUG
						printf("Found unknown type\n");
					#endif

					type_expected_value = JSON_VALUE_UNKNOWN;
					chr_close = ',';

					// Костыль для чтения последнего элемента в массиве неизвестного типа
					if ((flag_read_array == 1) && ((index_in_json_str + 1) < _len_str_json) &&\
						(_str_json[index_in_json_str + 1] == ']') && (_str_json[index_in_json_str - 1] != '\\'))
					{
						flag_read_force_read = 1;
					}
					else
					{
						continue;
					}
				}
			}
		}

		// Чтение ключа
		if (flag_read_key == 1)
		{
			// Чтение до встречи знака открытия
			if (_str_json[index_in_json_str] != chr_close)
			{
				len_value++;
			}
			else
			{
				ptr_key = ptr_value;
				len_key = (len_value - 1);
				flag_want_value = 1;
				flag_read_key = 0;
				len_value = 0;
				chr_close = '\0';

				#ifdef DEBUG
					printf("Key: \"%.*s\"\n", (int)len_key, ptr_key);
				#endif
			}
		}

		// Чтение значения
		if (flag_read_value == 1)
		{
			// Условие для отброса объектов внутри объектов и массивов внутри массивов
			if ((chr_open != '\0') && ((type_expected_value == JSON_VALUE_ARRAY) ||\
										(type_expected_value == JSON_VALUE_OBJECT)) &&\
				(_str_json[index_in_json_str] == chr_open))
			{
				count_hooks++;
				len_value++;

				continue;
			}

			// Костыль для чтения последнего элемента в массиве неизвестного типа
			if ((flag_read_array == 1) && (flag_read_force_read == 0) &&\
				(_str_json[index_in_json_str] != chr_close) &&\
				(type_expected_value != JSON_VALUE_ARRAY) &&\
				((index_in_json_str + 1) < _len_str_json) && (_str_json[index_in_json_str + 1] == ']') &&\
				(_str_json[index_in_json_str - 1] != '\\'))
			{
				flag_read_force_read = 1;
				len_value++;
			}

			if (((_str_json[index_in_json_str] == chr_close) && (_str_json[index_in_json_str - 1] != '\\')) ||\
				(flag_read_force_read == 1))
			{
				if ((type_expected_value == JSON_VALUE_ARRAY) || (type_expected_value == JSON_VALUE_OBJECT))
				{
					if (count_hooks != 0)
					{
						count_hooks--;
						len_value++;

						continue;
					}

					len_value++;
				}

				#ifdef DEBUG
					if (flag_read_array == 1)
					{
						printf("IN ARRAY\n");
					}
				#endif

				if (type_expected_value == JSON_VALUE_STR)
				{
					len_value--;
				}

				if ((type_expected_value == JSON_VALUE_OBJECT) ||\
					(type_expected_value == JSON_VALUE_ARRAY))
				{
					#ifdef DEBUG
						printf("----------------------------->\n");
					#endif

					if (flag_read_array == 1)
					{
						obj_json_tmp = __jsonLoad(ptr_value, len_value, _error);

						if (obj_json_tmp == NULL)
						{
							__jsonAddToObject(&obj_json_children, ptr_key, len_key,\
								NULL, 0, type_expected_value);
						}
						else
						{
							if (type_expected_value == JSON_VALUE_ARRAY)
							{
								__jsonAddToObject(&obj_json_children, ptr_key, len_key,\
									(const void *)obj_json_tmp->__value, obj_json_tmp->__size_value,\
									type_expected_value);
								free(obj_json_tmp->__value);
							}

							if (type_expected_value == JSON_VALUE_OBJECT)
							{
								__jsonAddToObject(&obj_json_children, ptr_key, len_key,\
									(const void *)obj_json_tmp, sizeof(jsonObj_t),\
									type_expected_value);
							}

							free(obj_json_tmp);
						}
					}
					else
					{
						obj_json_children = __jsonLoad(ptr_value, len_value, _error);

						if (obj_json_children == NULL)
						{
							__jsonAddToObject(&obj_json, ptr_key, len_key,\
								NULL, 0, type_expected_value);
						}
						else
						{
							if (type_expected_value == JSON_VALUE_ARRAY)
							{
								__jsonAddToObject(&obj_json, ptr_key, len_key,\
									(const void *)obj_json_children->__value, obj_json_children->__size_value,\
									type_expected_value);
								free(obj_json_children->__value);
							}

							if (type_expected_value == JSON_VALUE_OBJECT)
							{
								__jsonAddToObject(&obj_json, ptr_key, len_key,\
									(const void *)obj_json_children, sizeof(jsonObj_t),\
									type_expected_value);
							}

							free(obj_json_children);
						}
					}

					#ifdef DEBUG
						printf("<-----------------------------\n");
					#endif
				}
				else
				{
					if (flag_read_array == 1)
					{
						__jsonAddToObject(&obj_json_children, ptr_key, len_key,\
							(const void *)ptr_value, len_value,\
							type_expected_value);
					}
					else
					{
						__jsonAddToObject(&obj_json, ptr_key, len_key,\
							(const void *)ptr_value, len_value,\
							type_expected_value);
					}
				}

				#ifdef DEBUG
					printf("Value: \"%.*s\"\n\n", (int)len_value, (char *)ptr_value);
				#endif

				if (flag_read_array == 1)
				{
					flag_want_value = 1;

					if ((type_expected_value != JSON_VALUE_UNKNOWN) ||\
						(flag_read_force_read == 1))
					{
						flag_found_separator = 0;
					}
				}
				else
				{
					flag_want_value = 0;
					flag_found_separator = 0;
				}

				flag_read_value = 0;
				chr_open = '\0';
				type_expected_value = JSON_VALUE_NONE;
			}
			else
			{
				len_value++;
			}
		}
	}

	// Если не найдено начало JSON-объекта, то вернуть ошибку
	if (flag_found_start == 0)
	{
		jsonFree(obj_json);

		obj_json = NULL;

		if (_error != NULL)
		{
			*_error = JSON_ERR_BAD_JSON;
		}
	}

	return obj_json;
}


/*jsonObj_t *jsonFileLoad(const char *_path_to_file) {
	jsonObj_t *obj_json = NULL;
	FILE *file_json = NULL;

	file_json = fopen(_path_to_file, "r");

	if (file_json == NULL)
	{
		obj_json = (jsonObj_t *)Func_jsonCalloc(1, sizeof(jsonObj_t));
		obj_json->error_code = JSON_ERR_OPEN_FILE;
		return obj_json;
	}

	// return jsonStrLoad();

	return NULL;
}
*/

void jsonFree(jsonObj_t *_obj_json) {
	jsonObj_t *ptr_next_obj_json = NULL;

	if (_obj_json == NULL)
	{
		return;
	}

	for (;; _obj_json = ptr_next_obj_json)
	{
		ptr_next_obj_json = _obj_json->__ptr_next;

		switch (_obj_json->__type) {
			case JSON_VALUE_OBJECT:
			{
				jsonFree(_obj_json->__value);
				free((void *)_obj_json->__str_key);
				free(_obj_json);
			}
			break;

			case JSON_VALUE_ARRAY: {
				jsonFree(_obj_json->__value);
				free((void *)_obj_json->__str_key);
				free(_obj_json);
			}
			break;

			default: {
				free(_obj_json->__value);
				free((void *)_obj_json->__str_key);
				free(_obj_json);
			}
		}

		if (ptr_next_obj_json == NULL)
		{
			break;
		}
	}
}


void jsonSetFuncRealloc(void *(*_func_calloc)(size_t _num, size_t _size),\
	char *(*_func_strndup)(const char *_str, size_t _len))
{
	Func_jsonCalloc = _func_calloc;
	Func_jsonStrndup = _func_strndup;
}

jsonObj_t *jsonLoad(const char *_str_json, jsonError_t *_error) {
	return __jsonLoad(_str_json, strlen(_str_json), _error);
}

const char *jsonStrErr(jsonError_t _err) {
	switch (_err) {
		case JSON_OK: {
			return "Success";
		}
		break;

		case JSON_ERR_OPEN_FILE: {
			return "Fail to open file";
		}
		break;

		case JSON_ERR_BAD_JSON: {
			return "Bad JSON-format";
		}
		break;

		case JSON_ERR_INTERNAL: {
			return "Internal error";
		}
		break;

		case JSON_ERR_ARGS: {
			return "Fail in arguments";
		}
		break;

		case JSON_ERR_MEMORY: {
			return "Fail memory";
		}
		break;

		case JSON_ERR_TYPE: {
			return "Wrong type";
		}
		break;

		case JSON_ERR_NOT_FOUND: {
			return "Element not found";
		}
		break;

		case JSON_ERR_OUT_OF_ARRAY: {
			return "Out of array";
		}
		break;

		case JSON_ERR_UNKNOWN: {
			return "Unknown error";
		}
		break;
	}

	return NULL;
}


void jsonGetVer(int *_int_major, int *_int_minor, int *_int_micro) {
	*_int_major = VER_MAJOR;
	*_int_minor = VER_MINOR;
	*_int_micro = VER_MICRO;
}

const jsonObj_t *jsonOpenObj(const jsonObj_t *_obj_json, jsonError_t *_error) {
	if (_obj_json != NULL)
	{
		if (_obj_json->__type == JSON_VALUE_OBJECT)
		{
			if (_error != NULL)
			{
				*_error = JSON_OK;
			}

			return (const jsonObj_t *)_obj_json->__value;
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
			*_error = JSON_ERR_BAD_JSON;
		}
	}

	return NULL;
}
