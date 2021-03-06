/*  examples/miscellanea/dispatch_everything.c  */

#include <stdio.h>
#include <confini.h>

#define FALSE 0
#define TRUE 1

struct my_struct {

	char * my_date;
	char * my_ini_file;

};

static int print_ini_stats (IniStatistics * statistics, void * user_data) {

	struct my_struct * my_other = (struct my_struct *) user_data;

	printf(

		"Hi there!\n\n"
		"Today is %s.\n\n"
		"We decided to parse the INI file \"%s\".\n\n"
		"The file is %lu bytes large and contains %lu nodes.\n"
		"Please find below the output of each dispatch.\n",

		my_other->my_date, my_other->my_ini_file, statistics->bytes, statistics->members

	);

	return 0;

}

static int my_ini_listener (IniDispatch * dispatch, void * user_data) {

	printf(

		"\ndispatch_id: %lu\n"
		"format: {IniFormat}\n"
		"type: %d\n"
		"data: |%s|\n"
		"value: |%s|\n"
		"append_to: |%s|\n"
		"d_len: %lu\n"
		"v_len: %lu\n"
		"at_len: %lu\n",

		dispatch->dispatch_id,
		dispatch->type,
		dispatch->data,
		dispatch->value,
		dispatch->append_to,
		dispatch->d_len,
		dispatch->v_len,
		dispatch->at_len

	);

	/*  Check if this is an implicit key  */

	if (dispatch->value == INI_GLOBAL_IMPLICIT_VALUE) {

		printf("\n\tHey! This was an implicit value!\n");

	}

	return 0;

}

int main () {

	/*  The format of the INI file  */
	IniFormat my_format;

	/*  User data  */
	struct my_struct my_other;

	/*  Define the value to be shown in case of implicit keys, and its length  */
	ini_global_set_implicit_value("YES", 0);

	my_other.my_date = "Thursday September 22th, 2016";
	my_other.my_ini_file = "example.conf";

	/*  Use the default format as model for the new format  */
	my_format = INI_DEFAULT_FORMAT;

	/*  Enable implicit keys for this format  */
	my_format.implicit_is_not_empty = TRUE;

	/*  Load INI file  */
	if (load_ini_path("ini_files/example.conf", my_format, print_ini_stats, my_ini_listener, &my_other)) {

		fprintf(stderr, "Sorry, something went wrong :-(\n");
		return 1;

	}

	printf("\nFile \"%s\" has been parsed successfully.\n\n", my_other.my_ini_file);
	return 0;

}

