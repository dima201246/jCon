#include <stdio.h>
#include <jCon.h>

// const char *str_json = "[\"1\"]";
// const char *str_json = "[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\"]";
// const char *str_json = "[false          , 128, null]";
// const char *str_json = "[1,2,3,-4,5,6,7]";
// const char *str_json = "{\"key\":[1,2,\"3\",4,5,6,7]}";
// const char *str_json = "{\"key\":[\"12\", [\"12316\"]]}";
// const char *str_json = "{\"key\":[\"12\",\"34\",[\"12316\", \"12315\", [\"212\",\"234\",\"256\"], \"12314\", \"12313\", \"12312\"], \"56\", \"234d\", [\"16\", \"15\", \"14\", \"13\", \"12\"]], \"key2\":[\"212\",\"234\",\"256\"]}";
// const char *str_json = "{\"key\": \"value\", \"key2\": \"value2\", \"key3\": \"value3\", \"key4\": \"value4\"}";
// const char *str_json = "[ \"1\", \"3\", \"5\", {\"key\":\"value\"}, [\"2\", \"5\", \"6\", {\"key2\":\"value2\"}], []]";
// const char *str_json = "[ \"1\", \"3\", \"5\", [\"2\", \"5\", \"6\"], []]";
// const char *str_json = "[ \"1\", \"3\", \"5\"]";
// const char *str_json = "{}";
// const char *str_json = "{\"key\": \"value\", \"key2\": {\"key42\":\"value2\"}, \"key3\": \"value3\", \"key4\": \"value4\"}";
/*const char *str_json =
"{"\
	"\"ver\": 1,"\
	"\"empty_obj\": {}"\
"}";*/
const char *str_json =
"{"\
	"\"ver\": 1,"\
	"\"test_array\": [],"\
	"\"proto\": \"scheduleSyncReq\","\
	"\"ts\": 1580113855493763,"\
	"\"sid\": 216470325101216594277050222416296076944,"\
	"\"sender\": \"EDP\","\
	"\"empty_obj\": {},"\
	"\"receiver\": \"DAL-Schedule\","\
	"\"cmd\": \"setSchedule\","\
	"\"null\":null,"\
	"\"cmdParams\": {"\
		"\"schedules\": ["\
			"{\"ts\": 1580108580000000, \"cmd\": \"relayEnable\", \"cmdParams\": {}},"\
			"{\"ts\": 1580134200000000, \"cmd\": \"relayDisable\", \"cmdParams\": {}}],"\
		"\"from\": 1580108580000000,"\
		"\"to\": 1580134200000000,"\
		"\"initTs\": 1580113855480315,"\
		"\"schedType\": \"relaySchedule\","\
		"\"endpoints\": [\"19/relay/1\"]"\
	"}"\
"}";
/*const char *str_json =
"{"\
	"\"ver\": 1,"\
	"\"proto\": \"scheduleSyncReq\","\
	"\"ts\": 1580113855493763,"\
	"\"sid\": 216470325101216594277050222416296076944,"\
	"\"sender\": \"EDP\","\
	"\"receiver\": \"DAL-Schedule\","\
	"\"cmd\": \"setSchedule\","\
	"\"cmdParams\": {"\
		"\"schedules\": ["\
			"{\"ts\": 1580108580000000, \"cmd\": \"relayEnable\", \"cmdParams\": {}},"\
			"{\"ts\": 1580134200000000, \"cmd\": \"relayDisable\", \"cmdParams\": {}}],"\
		"\"from\": 1580108580000000,"\
		"\"to\": 1580134200000000,"\
		"\"initTs\": 1580113855480315,"\
		"\"schedType\": \"relaySchedule\""\
	"}"\
"}";*/


int testGenJson() {
	jsonObj_t *obj_json = NULL;
	jsonErr_t error = 0;

	error = jsonAddStr(&obj_json, "proto", "test_key_str", "test value");

	return 0;
}

int testArray() {
	jsonErr_t error = 0;

	jsonObj_t *obj_json = NULL;
	const jsonObj_t *ptr_obj_json = NULL;

	const char *str_json = "[{\"devType\":\"elfin\",\"devId\":\"12345\"},"\
		"{\"devType\":\"elfin1\",\"devId\":\"123451\"},"\
		"{\"devType\":\"elfin2\",\"devId\":\"123452\"}]";

	size_t index = 0;

	obj_json = jsonLoad(str_json, &error);

	for (index = 0; index < jsonGetArraySize(obj_json, NULL, &error); ++index)
	{
		ptr_obj_json = jsonGetObjInArray(obj_json, NULL, index, &error);

		printf("%s\n", jsonGetStr(jsonOpenObj(ptr_obj_json, NULL), "devType", &error));
	}

	jsonFree(obj_json);
}

int main(int argc, char const *argv[]) {
	jsonObj_t *test_obj_json = NULL;
	jsonObj_t *test_obj_json_2 = NULL;
	jsonErr_t error = 0;

/*	printf("Test JSON: \"%s\"\n\n", str_json);
	test_obj_json = jsonLoad(str_json);
	printf("##########################################################################################################\n");
	jsonShowTree(test_obj_json);*/

/*	printf("\"%s\"\n\"", str_json);
	printf("\nStr: \"%s\"\n", jsonGenStr(test_obj_json));*/

/*	printf("%s\n", jsonStrErr(jsonAddArray(&test_obj_json_2, NULL, NULL)));
	printf("%s\n", jsonStrErr(jsonAddStr(&test_obj_json_2, NULL, "test_key", "test value")));*/
	printf("%s\n", jsonStrErr(jsonAddObject(&test_obj_json_2, NULL, "testobj1")));
	printf("%s\n", jsonStrErr(jsonAddStr(&test_obj_json_2, "testobj1", "test_key_str", "test value")));
	printf("%s\n", jsonStrErr(jsonAddObject(&test_obj_json_2, "testobj1", "testobj2")));
	printf("%s\n", jsonStrErr(jsonAddNum(&test_obj_json_2, "testobj1:testobj2", "test_key", 15)));
	printf("%s\n", jsonStrErr(jsonAddObject(&test_obj_json_2, "testobj1:testobj2", "testobj3")));
	printf("%s\n", jsonStrErr(jsonAddArray(&test_obj_json_2, "testobj1:testobj2:testobj3", "test array")));
	printf("%s\n", jsonStrErr(jsonAddStr(&test_obj_json_2, "testobj1:testobj2:testobj3:test array", NULL, "test value")));
	printf("%s\n", jsonStrErr(jsonAddStr(&test_obj_json_2, "testobj1:testobj2:testobj3:test array", "test str 1", "test value 1")));
	printf("%s\n", jsonStrErr(jsonAddStr(&test_obj_json_2, "testobj1:testobj2:testobj3:test array", "test str 2", "test value 2")));
	printf("%s\n", jsonStrErr(jsonAddBool(&test_obj_json_2, "testobj1:testobj2:testobj3:test array", "BOOL true", 1)));
	printf("%s\n", jsonStrErr(jsonAddBool(&test_obj_json_2, "testobj1:testobj2:testobj3:test array", "bool false", 0)));
	printf("%s\n", jsonStrErr(jsonAddNull(&test_obj_json_2, "testobj1:testobj2:testobj3:test array", "bool false")));
	printf("\nStr: \"%s\"\n", jsonGenStr(test_obj_json_2));

	// jsonAddObject(&test_obj_json_2, NULL, "testobj4");

	// jsonAddStr(&test_obj_json_2, NULL, "test_key", "test value");
	// jsonAddStr(&test_obj_json_2, NULL, "test_key_2", "test value 2");
/*	jsonShowTree(test_obj_json_2);
	printf("%lu\n", jsonGetUL(test_obj_json_2, "testobj1:testobj2:test_key", &error));
	printf("%s\n", jsonStrErr(error));*/


	// printf("%s\n", jsonGetStr(test_obj_json, "cmdParams:schedules:1:cmd", NULL));
	// printf("%d\n", jsonGetInt(test_obj_json, "3", NULL));
	// printf("%s\n", jsonGetNumberAsStr(test_obj_json, "sid", NULL));
	// printf("%d\n", jsonGetObjInArray(test_obj_json, "key", 1, NULL)->__type);

	jsonFree(test_obj_json);
	jsonFree(test_obj_json_2);

	testArray();

	return 0;
}
