#include <stdio.h>
#include <string.h>

int CallDirectory(char *new, char *current) {
  return 0;
}

static int failed = 0;
void expect(int value, char *expectation) {
  if (!value) {
    printf("Failed expectation: %s\n", expectation);
    failed = 1;
  }
}

void test_existing() {
  char *current = strdup("foo");
  int exists = CallDirectory("bar", current);
  
  expect(exists, "CallDirectory returns true for existing directory");
  expect(strcmp("bar", current) == 0, "CallDirectory changes current directory on success");
}

void test_non_existing() {
  char *current = strdup("foo");
  int exists = CallDirectory("no-bar", current);
  
  expect(!exists, "CallDirectory returns false for non-existing directory");
  expect(strcmp("foo", current) == 0, "CallDirectory retains current directory on failure");
}

int main(void) {
  test_existing();
  test_non_existing();
  
  if (failed) {
    return -1;
  } else {
    return 0;
  }
}
