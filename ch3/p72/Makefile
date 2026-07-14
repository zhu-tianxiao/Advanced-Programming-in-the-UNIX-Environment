process_limit: process_limit.c
	gcc process_limit.c -o process_limit

copy: copy.c ../error.c
	gcc copy.c ../error.c -o copy 

create_big_file: create_big_file.c
	gcc create_big_file.c -o create_big_file

time: time.c ../error.c
	gcc time.c ../error.c -o time

run_limit: process_limit
	./process_limit

run_copy: copy
	./copy < bigfile > /dev/null

run_create_big_file: create_big_file
	./create_big_file


run_time: time
	./time