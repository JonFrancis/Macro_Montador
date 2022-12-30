#include <iostream>
#include <bits/stdc++.h>
#include <map>
#include <stdio.h>
#include <string.h>
using namespace std;

map<string, pair<int,int>> opcode {
    {"ADD", make_pair(1,2)},
    {"SUB", make_pair(2,2)},
    {"MULT", make_pair(3,2)},
    {"MUL", make_pair(3,2)},
    {"DIV", make_pair(4,2)},
    {"JMP", make_pair(5,2)},
    {"JMPN", make_pair(6,2)},
    {"JMPP", make_pair(7,2)},
    {"JMPZ", make_pair(8,2)},
    {"COPY", make_pair(9,3)},
    {"LOAD", make_pair(10,2)},
    {"STORE", make_pair(11,2)},
    {"INPUT", make_pair(12,2)},
    {"OUTPUT", make_pair(13,2)},
    {"STOP", make_pair(14,1)},
    {"CONST", make_pair(-1,1)},
    {"SPACE", make_pair(0,-1)},
};

vector<pair<string, int>> tabela_simbolo;
vector<vector<string>> tudo;
vector<string> label_name;
map<string, int> tabela_simbolos;
vector<int> contador;
vector<vector<int>> codiu_final;
string objeto="", nome_prog;
bool e = 0;
map<string, string> dicionario_EQU;
map<string, string> cause5hora;

/*
    Funcao responsavel por converter uma string para maiusculo

    Esse funcao eh chamada para formatar todas as palavras(comando, label, chamadas) para que nao tenha problema com letras minusculas e maiusculas
*/
string strupr(string upper){
    string aux = "";
    for(int i = 0; i < upper.size(); i++){
        aux += toupper(upper[i]);
    }
    return aux;
}

/*
    Funcao responsavel por gerar o arquivo, sempre eh a ultima funcao chamada, a primeira string que le recebe contem o nome do arquivo, a segunda eh o
    tipo(.PRE, .MCR e .OBJ) e o ultimo argumento eh a string com o programa completo
*/
void cria_arquivo(string nome, string formato, string texto){
    FILE *arq;
    char num[80], text[1000];
    string path = nome+formato;
    strcpy(num, path.c_str());
    arq = fopen(num, "w+");

    if (arq == NULL){
        cout << "Erro ao criar arquivo" << endl;
        return;
    }

    strcpy(text, texto.c_str());

    if (fputs(text, arq) == EOF)
        cout << "Erro na Gravacao" << endl;

    fclose(arq);
}

/*
    A funcao prog eh sempre a primeira funcao chamada, ela é responsavel por formatar os dados lidos do arquivo, esses dados sao colocados
    em um vector<vector<string>> onde cada linha eh um vetor dentro de uma matriz e cada palavra eh uma string dentro desse vetor,
    assim facilitando para as outras funcoes.
*/
void prog(FILE *program){
    while(!feof(program)){
        vector<string> linha_comando = {};
        char linha[100];
        string result;
        result = fgets(linha, 100, program);
        int espaco = 0;
        string exemplo;
        int j = 0, contou = 0, dois_pontos = 0;
        for(int i = 0; i < result.size()-1; i++){
            if(result[i] == ';') break;
            if(result[i] != ' ' && result[i] != '\n' && result[i] != ','){
                exemplo += result[i];
                if(result[i] == ':'){
                    dois_pontos++;
                    if(dois_pontos > 1){
                        char erro[] = "Erro: dois rotulos na mesma linha\n";
                        cout << erro;
                        cria_arquivo("Log",".txt",erro);
                        e = 1;
                        return;
                    }
                }
            }else{
                if(exemplo != "" && exemplo != "\n" && exemplo != " ")
                    linha_comando.push_back(strupr(exemplo));
                exemplo = "";
            }
        }
        if(exemplo != " " && exemplo != "\n" && exemplo != ",")
            linha_comando.push_back(strupr(exemplo));
        tudo.push_back(linha_comando);
    }
}

/*
    Essa eh a funcao que faz o pré-processamento, ela identifica todos os EQU e subistitui por seu valor em todas as chamadas, identifica todos os IF e
    elimina o cabecalho em caso de do IF e a linha seguinte no caso do IF ser negado, para o caso do IF ser aceito ele elimnia apenas a linha do IF.
*/
void pre_processamento(){
    int flag_section = 0, flag_if = 0;
    string novo_arq;
    for(int  i = 0; i < tudo.size();i++){
        flag_if = 0;
        for(int j = 0; j < tudo[i].size(); j++){
            if(tudo[i][j] == "SECTION"){
                flag_section = 1;
            }
            else if(tudo[i][j] == "EQU" && flag_section == 0){
                dicionario_EQU[tudo[i][j-1]] = tudo[i][j+1];
            }else if(tudo[i][j] == "IF"){
                flag_if = 1;
                if(dicionario_EQU[tudo[i][j+1]+":"] == "0"){
                    i++;
                    break;
                }
            }else if(dicionario_EQU.find(tudo[i][j]+":") != dicionario_EQU.end()){
                tudo[i][j] = dicionario_EQU[tudo[i][j]+":"];
            }
            if(flag_section == 1 && flag_if == 0)
                novo_arq += string(tudo[i][j]) + " ";
        }
        if(flag_section == 1 && flag_if == 0){
            novo_arq += "\n";
        }
    }
    novo_arq.pop_back();
    cria_arquivo(nome_prog,".pre",novo_arq);
}

void segunda_passada(){
    for(int i = 0; i < tudo.size(); i++){
        vector<int> aux = {};
        int comando = 0;
        for(int j = 0; j < tudo[i].size(); j++){
            if(tudo[i][j] == "SECTION"){
                break;
            }else if(tudo[i][j][tudo[i][j].size()-1]== ':'){

            }else if(comando == 0){
                pair<int,int> soma = opcode.find(tudo[i][j])->second;
                if(soma.first == -1){
                    int k = stoi(tudo[i][j+1], nullptr, 10);
                    objeto += tudo[i][j+1] + " ";
                    aux.push_back(k);
                    break;
                }else{
                    if(soma.second == -1){
                        if(i == tudo.size()-1 || j == tudo[i].size()-2){
                            objeto += "0 ";
                            aux.push_back(0);
                        }else{
                            for(int k = 0; k < stoi(tudo[i][j+1], nullptr, 10); k++){
                                aux.push_back(0);
                                objeto += "0 ";
                            }
                        }
                        break;
                    }else{
                        aux.push_back(soma.first);
                        objeto += to_string(soma.first) + " ";
                    }
                }
                comando = 1;
            }else if(tudo[i][j] != ""){
                if(tabela_simbolos.find(tudo[i][j]) == tabela_simbolos.end()){
                    char erro[] = "Erro: rotulo/dado nao definido\n";
                    cout << erro;
                    cria_arquivo("Log",".txt",erro);
                    e = 1;
                    return;
                }
                else{
                    objeto += to_string(tabela_simbolos[tudo[i][j]]) + " ";
                    aux.push_back(tabela_simbolos[tudo[i][j]]);
                }
            }
        }
        codiu_final.push_back(aux);
    }
    cria_arquivo(nome_prog,".obj",objeto);
}

void primeira_passada(){
    int k = 0, comando = -1, text = 0;
    for(int i = 0; i < tudo.size(); i++){
        for(int j = 0; j < tudo[i].size(); j++){
            if(tudo[i][j] == "SECTION"){
                if(tudo[i].size()-1 > j){
                    if(tudo[i][j+1] == "TEXT")
                        text++;
                }
                break;
            }else if(tudo[i][j][tudo[i][j].size()-1]== ':'){
                if(tudo[i][j][0] < 65 || tudo[i][j][0] > 90){
                    char erro[] = "Erro: erro lexico\n";
                    cout << erro;
                    cria_arquivo("Log",".txt",erro);
                    e = 1;
                    return;
                }
                if(tudo[i][j] != ""){
                    label_name.push_back(tudo[i][j].substr(0,tudo[i][j].size()-1));
                    tabela_simbolos[tudo[i][j].substr(0,tudo[i][j].size()-1)] = k;
                }
            }else if(comando == -1){
                if(opcode.find(tudo[i][j]) == opcode.end()){
                    char erro[] = "Erro: instrucao ou diretiva inexistente\n";
                    cout << erro;
                    cria_arquivo("Log",".txt",erro);
                    e = 1;
                    return;
                }
                contador.push_back(k);
                pair<int,int> soma = opcode.find(tudo[i][j])->second;
                if(soma.second == -1){
                    if(i == tudo.size()-1 || j == tudo[i].size()-2){
                        k += 1;
                    }else{
                        k += stoi(tudo[i][j+1], nullptr, 10);
                    }
                }else{
                    k += soma.second;
                }
                comando = 1;
            }
        }
        comando = -1;
    }
    if(text == 0){
        char erro[] = "Erro: section text nao presente\n";
        cout << erro;
        cria_arquivo("Log",".txt",erro);
        e = 1;
        return;
    }

    for(int i = 0; i < label_name.size(); i++){
        cout << label_name[i] << " " << tabela_simbolos[label_name[i]] << endl;
    }

}

/*
    Essa eh a funcao criada para verificar as macros, quando uma macro eh gerada ela eh armazenada em um variavel cause5hora que eh um map<string, string>
    onde sua label eh usada para localizar esse map e o conteudo eh armazenado na string dentro do map.

    Em seguida verifica o resto do codigo, onde procura uma chamada para esse macro, e em seguida subistitui ela, existe dois casos, onde a macro possui
    um cabecalho e o caso onde nao possui, para eses dois casos o tratamento eh diferente, pois temos que substituir as variaveis dentro pelas variaveis
    que foram dadas ao chamar a macro e depois adicionar essa macro no codigo.
*/
void macros(){
    string novo_arq;
    int flag_teste = 0;
    for(int i = 0; i < tudo.size(); i++){
        flag_teste = 0;
        int primeiro = 0;
        for(int j = 0; j < tudo[i].size(); j++){
            if(tudo[i][j] == "MACRO"){
                novo_arq.pop_back();
                string label_aux = tudo[i][j-1];
                while(tudo[i][0] != "ENDMACRO"){
                    for(int k = 0;  k < tudo[i].size(); k++){
                        if(primeiro == 0){
                            k = j;
                            primeiro = 1;
                        }
                        cause5hora[label_aux] += tudo[i][k] + " ";
                    }
                    cause5hora[label_aux] += "\n";
                    i++;
                }
                cause5hora[label_aux].pop_back();
            }else if(cause5hora.find(tudo[i][j]+":") != cause5hora.end()){
                if(tudo[i].size()-1 == j){
                    int passou = 0;
                    string auxi = cause5hora[tudo[i][j]+":"];
                    for(int k = 0; k < auxi.size(); k++){
                        if(passou == 1){
                            novo_arq += auxi[k];
                        }
                        if(auxi[k] == '\n'){
                            passou = 1;
                        }
                    }
                }else{
                    string trecho_macro = "";
                    string auxi = cause5hora[tudo[i][j]+":"];
                    map<string, string> param;
                    string param_aux = "";
                    int flag_aux = 0;
                    int k = 0, passou = 0;
                    while(auxi[k] != '\n'){
                        if(auxi[k] == '&'){
                            flag_aux = 1;
                            param_aux += auxi[k];
                        }else if(auxi[k] == ' ' && flag_aux == 1){
                            flag_aux = 0;
                            param[param_aux] = tudo[i][j+1];
                            param_aux = "";
                            j++;
                        }else if(flag_aux == 1){
                            param_aux += auxi[k];
                        }else if(auxi[k] == '\n'){
                            param[param_aux] = tudo[i][j+1];
                            j++;
                            break;
                        }
                        k++;
                    }
                    param_aux = "";
                    flag_aux = 0;
                    for(k = 0; k < auxi.size(); k++){
                        if(auxi[k] == '&'){
                            flag_aux = 1;
                            param_aux += auxi[k];
                        }else if(auxi[k] == ' ' && flag_aux == 1){
                            flag_aux = 0;
                            trecho_macro += param[param_aux];
                            param_aux = "";
                        }else if(flag_aux == 1){
                            param_aux += auxi[k];
                        }else if(auxi[k] == '\n'){
                            trecho_macro += param[param_aux];
                            trecho_macro += "\n";
                        }else{
                            trecho_macro += auxi[k];
                        }
                        if(param.find(param_aux) != param.end() && flag_aux == 1){
                            flag_aux == 0;
                        }
                    }
                    for(int k = 0; k < trecho_macro.size(); k++){
                        if(passou == 1){
                            novo_arq += trecho_macro[k];
                        }
                        if(trecho_macro[k] == '\n'){
                            passou = 1;
                        }
                    }
                }

            }else{
                if((tudo[i][j][tudo[i][j].size()-1] != ':') || ((tudo[i][j][tudo[i][j].size()-1] == ':') && tudo[i][j+1] != "MACRO"))
                    novo_arq += tudo[i][j] + " ";
                else
                    flag_teste = 1;
            }
        }
        novo_arq += '\n';
    }
    novo_arq.pop_back();
    cria_arquivo(nome_prog,".mcr",novo_arq);
}

int main(int argc, char *argv[]){
    FILE *program;
    nome_prog = argv[2];
    char num[80];
    
    if(string(argv[1]) == "-p"){
        string path = string(argv[2])+".asm";
        strcpy(num, path.c_str());
        program = fopen(num, "rt");
        prog(program);
        pre_processamento();
    }
    else if(string(argv[1]) == "-m"){
        string path = string(argv[2])+".pre";
        strcpy(num, path.c_str());
        program = fopen(num, "rt");
        prog(program);
        macros();
    }
    else if(string(argv[1]) == "-o"){
        string path = string(argv[2])+".mcr";
        strcpy(num, path.c_str());
        program = fopen(num, "rt");
        prog(program);
        if (e) return 1;
        primeira_passada();
        if (e) return 1;
        segunda_passada();
        if (e) return 1;
    }
}