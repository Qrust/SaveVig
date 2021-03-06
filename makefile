CC = colorgcc
CFLAGS = -Wall -Werror -lstdc++ -Wno-unused-variable
SRCS = Strong_vigenere_cipher.cpp hash.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = Strong_vigenere_cipher-bin

all: $(MAIN)

$(MAIN): $(SRCS)
	$(CC) -o $(MAIN) $(SRCS) $(CFLAGS)
%.o: %.cpp
	$(CC) -c $^  -o $@
clean:
	rm -rf *.o $(MAIN)

rebuild: clean $(MAIN)
