#!/bin/bash

# INI - Numero arquivo inicial
# FIM - Numero arquivo final

COUNTA=$1
FIM=$2

COUNTB=1
rm -f nomes-trocados.txt

while [ $COUNTA -le $FIM ]
do
	echo "Testando a existencia de input$COUNTA.tes"

	if [ -e "input$COUNTA.tes" ]; then
		echo "Movendo input$COUNTA.tes -> input$COUNTB.tes" >> nomes-trocados.txt
		mv input$COUNTA.tes input$COUNTB.tes
		#Incrementando o contador de projetos
		let COUNTB=COUNTB+1 
	fi
	let COUNTA=COUNTA+1 
done

