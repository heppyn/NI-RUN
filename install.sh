#!/bin/bash
echo "install g++"

apt-get --yes install g++

echo "compile interpreter"
echo ""

cd ./fml_ast_interpreter
make build_release

mv ./interpreter ../interpreter
cd ..

echo ""
echo "run interpreter with prepared example"
echo ""
./interpreter -r parsed.json