#!/bin/bash

echo "INE5420-05208 (20152) - Computação Gráfica"
echo -e "Identificando os seus fontes em $(pwd) ..."
SOURCE_FILES=$(ls *.c *.cc *.cpp)
echo "Achei: $SOURCE_FILES"
echo -e "Compilando e linkando..."
#
# Teste para ver se tem as libs necessárias:
# dpkg -l libgtk* | grep -e '^i' | grep -e 'libgtk-*[0-9]'
#
echo "Usando: gcc `pkg-config --cflags gtk+-3.0 gtkmm-3.0`-o vpl_wexecution $SOURCE_FILES `pkg-config --libs gtk+-3.0 gtkmm-3.0` -rdynamic -lstdc++ -std=c++11 -lm"
echo "ERROS E AVISOS (se houver):"
gcc `pkg-config --cflags gtk+-3.0 gtkmm-3.0`-o vpl_wexecution $SOURCE_FILES `pkg-config --libs gtk+-3.0 gtkmm-3.0` -rdynamic -lstdc++ -std=c++11 -lm
