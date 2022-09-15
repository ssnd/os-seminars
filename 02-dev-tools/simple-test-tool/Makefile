CFLAGS 			:= -Werror -Wall -Wextra -Wpedantic
OUTPUT_BIN 		:= sum
CC				:= gcc
PROGRAM 		:= sum.c
TEST_DIR		:= tests
TEST_INPUT		:= input
TEST_OUTPUT		:= output
TEST_EXPECTED	:= expected


execute: 
	./$(OUTPUT_BIN)

build: 
	$(CC) -o $(OUTPUT_BIN) $(CFLAGS) $(PROGRAM)

run_test_cases:
	for file in `ls $(TEST_DIR) | grep $(TEST_INPUT)`;         \
	do                                                         \
		test_no=`echo $$file | cut -c 6-`;                     \
		./$(OUTPUT_BIN) < $(TEST_DIR)/$$file                   \
		      > $(TEST_DIR)/$(TEST_OUTPUT)$$test_no;           \
	done;

run_checks:
	failed=0;                                                     \
	for file in `ls $(TEST_DIR) | grep $(TEST_INPUT)`; do         \
		test_no=`echo $$file | cut -c 6-`;                        \
		expected="$(TEST_DIR)/$(TEST_EXPECTED)$$test_no";         \
		actual="$(TEST_DIR)/$(TEST_OUTPUT)$$test_no";             \
		input=$(TEST_DIR)/$$file;                                 \
		echo "TEST $$test_no:\n";                                 \
		echo "  input: `cat $$input`";                            \
		echo "  output: `cat $$actual`";                          \
		echo "  expected: `cat $$actual`";                        \
		diff_result=`diff $$actual $$expected`;                   \
		if [ $$? -eq 0 ]; then                                    \
			echo "PASS\n";                                        \
		else                                                      \
			echo "\n$$diff_result\n";                             \
			echo "FAIL\n";                                        \
			failed=1;                                             \
		fi;                                                       \
	done;                                                         \

cleantestoutput:
	rm -f $(TEST_DIR)/$(TEST_OUTPUT)*

cleanbin:
	rm -f $(OUTPUT_BIN)

test: build run_test_cases run_checks clean

run: build execute

clean: cleantestoutput cleanbin
