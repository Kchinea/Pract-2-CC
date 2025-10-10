# Nombre del ejecutable
EXEC = pract-02

# Directorios
SRC_DIR = src
BUILD_DIR = build

# Encontrar todos los archivos .cc en el directorio de src
SRCS = $(wildcard $(SRC_DIR)/*.cc)

# Generar archivos .o correspondientes en el directorio de build
OBJS = $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SRCS))

# Comando del compilador
CXX = g++

# Flags del compilador
CXXFLAGS = -std=c++11 -Wall

# Regla por defecto
all: $(BUILD_DIR) $(EXEC)

# Compilar el ejecutable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Regla para compilar los archivos .o en el directorio build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear el directorio build si no existe
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpiar los archivos compilados
clean:
	rm -rf $(BUILD_DIR) $(EXEC)