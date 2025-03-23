CXX = g++
CXXFLAGS = -Wall -Wextra
LDFLAGS = 
TARGET = programa

# Buscar archivos .cc en el directorio raíz y subdirectorios comunes (ej: src/, vehiculo/, zona/)
SRC_DIRS = . src vehiculo zona  # Agrega aquí tus subdirectorios
SRC = $(shell find $(SRC_DIRS) -name '*.cc' 2>/dev/null)

# Validar que haya archivos fuente
ifeq ($(SRC),)
  $(error No se encontraron archivos .cc en los directorios: $(SRC_DIRS))
endif

OBJ = $(patsubst %.cc, %.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET)
	find $(SRC_DIRS) -name '*.o' -delete

.PHONY: all clean