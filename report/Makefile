NAME 	 := report
TARGET 	 := ./$(NAME).pdf
MASDVI	 := ./src/tmp/$(NAME).dvi
MASTEX	 := ./src/$(NAME).tex
ALLDVI   := $(wildcard ./src/tmp/*.dvi)
SOURCES  := $(wildcard ./src/*.tex)
INCLUDE	 := $(wildcard ./include/*.tex)

$(TARGET): $(MASDVI) 
	dvipdfmx -o $(TARGET) $(MASDVI)

$(MASDVI): $(INCLUDE) $(SOURCES)
	platex -8bit -shell-escape -output-directory ./src $(MASTEX) 
	platex -8bit -shell-escape -output-directory ./src $(MASTEX) 
	mv src/*.dvi src/*.aux src/tmp/

all: $(TARGET)

clean:
	rm -f src/*.listing src/*.log src/*.fls src/*.out
	rm -rf src/_minted-report

test: $(TARGET)
	evince $(TARGET) 
	rm -rf src/_minted-report
	rm -f src/*.listing src/*.log src/*.fls src/*.out

.PHONY: all clean test twice
