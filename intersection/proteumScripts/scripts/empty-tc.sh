#!/bin/bash

# INI - Numero arquivo inicial
# FIM - Numero arquivo final

COUNTA=$1
FIM=$2

while [ $COUNTA -le $FIM ]
do
	if [ ! -e "tec$COUNTA.tes" ]; then
		echo "Criando tec$COUNTA.tes"
		touch tec$COUNTA.tes
	fi
	let COUNTA=COUNTA+1 
done

