ex1:
	@gcc test_fork.c -o test_fork
	@./test_fork 1 2 3
	@rm test_fork

ex2:
	@gcc test_execl.c -o test_execl
	@./test_execl 1 2 3
	@rm test_execl

ex3:
	@gcc test_system.c -o test_system
	@./test_system 1 2 3
	@rm test_system

ex4:
	@gcc test_fork_wait.c -o test_fork_wait
	@./test_fork_wait
	@rm test_fork_wait