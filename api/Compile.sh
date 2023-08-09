cd build
rm -rf *
echo "Starting Build Process (1/2)"
cmake ..
echo "Starting Build Process (2/2)"
cmake --build . --target install
echo "Done!"