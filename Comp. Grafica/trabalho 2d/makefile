# ---- Compilador e flags ----
CXX      = g++
CXXFLAGS = -g -Wall -Wextra -O2 \
           -Isrc -Ilib/tinyxml2 \
           -MMD -MP
LDLIBS   = -lglut -lGL -lGLU

# ---- Diretórios ----
SRC_DIR = src
LIB_DIR = lib/tinyxml2
OBJ_DIR = build

# ---- Arquivos ----
APP_SRC = $(wildcard $(SRC_DIR)/*.cpp)
LIB_SRC = $(wildcard $(LIB_DIR)/*.cpp)

APP_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(APP_SRC))
LIB_OBJ = $(patsubst $(LIB_DIR)/%.cpp,$(OBJ_DIR)/lib/%.o,$(LIB_SRC))
OBJ     = $(APP_OBJ) $(LIB_OBJ)
DEP     = $(OBJ:.o=.d)

# ---- Executável (na raiz) ----
TARGET = trabalhocg

# ---- Regras ----
all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

# src/*.cpp  ->  build/*.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# lib/tinyxml2/*.cpp  ->  build/lib/*.o
$(OBJ_DIR)/lib/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Só compilar, sem rodar
build-only: $(TARGET)

# Rodar sem recompilar
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all build-only run clean

# Dependências geradas pelo -MMD
-include $(DEP)