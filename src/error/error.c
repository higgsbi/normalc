#include "error.h"

bool ok(Error error) {
	return error == OK;
}

bool err(Error error) {
	return error != OK;
}
