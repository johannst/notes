SRC = event.c
BIN = $(SRC:.c=)

all: $(BIN)

%: %.c
	bash $<

clean:
	$(RM) $(BIN)
