all: graphics.ps-exe image.ps-exe

graphics.ps-exe: graphics.bin
	python3 ../scripts/bin2exe.py graphics.bin graphics.ps-exe
	rm graphics.bin

graphics.bin:
	armips graphics.s

image.ps-exe: image.bin
	python3 ../scripts/bin2exe.py image.bin image.ps-exe
	rm image.bin

image.bin:
	armips image.s

clean:
	-rm graphics.bin graphics.ps-exe image.bin image.ps-exe
