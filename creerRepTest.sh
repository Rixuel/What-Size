#!/bin/bash

echo Creation rep_test1
mkdir rep_test1
cd rep_test1
echo "0---------0---------0---------0---------0--------" > fich1.txt
echo "0---------0--------" > fich2.txt
mkdir rep1
cd rep1
echo "0---------0---------0---------0---------0--------" > fich3.txt
echo "0---------0---------0---------0---------0--------" > fich4.txt
cd ..
ln -s rep1/fich4.txt lien_symb


echo Creation rep_test2
cd ..
mkdir rep_test2
cd rep_test2
echo "0---------0---------0---------0---------0--------" > fich1.txt
echo "0---------0--------" > fich2.txt
mkdir rep1
cd rep1
echo "0---------0---------0--------" > fichier3.txt
echo "0---------0---------0--------" > fichier4.txt
echo "0--------" > lien_physique1
mkdir rep11
cd rep11
ln ../lien_physique1 lien_physique2
cd ../..
mkdir rep2
cd rep2
echo "0---------0---------0---------0---------0---------0--------" > fichier5.txt
ln ../rep1/lien_physique1 lien_physique3