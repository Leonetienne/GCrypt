# Compile single-header
deggl -i ../GhettoCrypt/*.cpp --out GhettoCipher --loc --license-file ../LICENSE

# Verify that this builds cleanly via g++
echo "Verifying build..."
g++ GhettoCipher.cpp -c -S -o - -Wall > /dev/null
echo "If you saw no compiler errors, everything's good!"
