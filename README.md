# SAT Solver
SAT solver implementující DPLL algoritmus s unitární propagací.

## Požadavky
Pro sestavení projektu na všech platformách je vyžadován **CMake**  a **C++ kompilátor**.

### Linux

Sestavení:
1. Otevřete terminál a přejděte do složky s projektem.
2. Vytvořte build adresář: 
    mkdir build
    cd build
3. Spusťte cmake: cmake ..
4. Zkompilujte: cmake --build .

Spuštění:
./solver <input.cnf>
