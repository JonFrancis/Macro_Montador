# Montador

O Montador foi criado usando a linguagem C++, esse montador, traduz um código em assembly(um pouco mais simplificado, gerado para a aplicação a diciplina Software Básico) para linguagem de máquina.

## Requerimentos
A aplicação foi desenvolvida utilizando a linguagem C++.
Para compilar e executar o sistema:

- G++ version 11.3.0
- Linux version Ubuntu 22.04

## Execução
Após a compilação o  executável deve ser chamado de MONTADOR. E deve ter 3 modos de uso. Deve ser possível chamar por linha de comando da seguinte forma: .montador -<op> <arquivo>

  O arquivo de entrada deve ser indicado SEM extensão e o arquivo de saída deve MANTER o mesmo nome e mudar a extensão. Deve existir 3 formas de operação:
  - -p: A extensão de saída deve ser .PRE e somente deve se processar EQU e IF.
  - -m: A extensão da daída deve ser .MCR e somente deve se processar MACROS. A entrada vai ser a saída pré-processada.
  - -o: A extensão da saída deve ser .OBJ e deve seguir o formato indicado anteriormente. A entrada vai ser a saída das Macros.
  

## Requisitos do projeto

 ### Características

  - Aceitar Maiúsculas e Minúsculas.
  - A diretiva CONST deve aceitar positivos, negativos e hexa no formato 0x (ex: 0x12). No arquivo de saída OBJ tudo deve estar em decimal.
  - O comando COPY deve separar os argumentos po "," SEM espaço.
  - Desconsiderar todos os espaços, tabulações ou enter desnecessários.
  - Pode dar rótudo seguido de dois pontos e ENTER. O rótulo é considerado como linha seguinte.
  - SPACE pode aceitar argumento. Logo é possível fazer rótulos como X+2(sem espaços).
  - Aceitar comentários em qualquer parte do código iniciado por ";"(o comentário deve ser removido no pré-processamento de EQU e IF).  
  
 ### Verificações
  
  - Dois rótulos na mesma linha.
  - Rótulo não definido.
  - Dado não definido.
  - Quantidade de argumentos errada.
  - Seção TEXT faltante.
  - Erros léxicos (caracteres especiais ou números inicial nos rótulos).
  
## Especificação Completa em PDF

https://drive.google.com/file/d/1a_uSVcKNnAD7KsLIsG1SDBCKc45afvCg/view?usp=sharing
