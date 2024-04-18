all: exercise1.ps-exe exercise2.ps-exe exercise3.ps-exe

exercise1.ps-exe: exercise1.bin
	python3 ../scripts/bin2exe.py exercise1.bin exercise1.ps-exe
	rm exercise1.bin

exercise1.bin:
	armips exercise1.s

exercise2.ps-exe: exercise2.bin
	python3 ../scripts/bin2exe.py exercise2.bin exercise2.ps-exe
	rm exercise2.bin

exercise2.bin:
	armips exercise2.s

exercise3.ps-exe: exercise3.bin
	python3 ../scripts/bin2exe.py exercise3.bin exercise3.ps-exe
	rm exercise3.bin

exercise3.bin:
	armips exercise3.s

clean:
	@-rm exercise1.bin exercise1.ps-exe exercise2.bin exercise2.ps-exe exercise3.bin exercise3.ps-exe
