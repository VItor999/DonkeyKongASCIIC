/***************************DONKEY KONG ASCII**********************************
   *************************DESENVOLVIDO POR******************************
                          Pedro Rodrigues de Lima
                                00288579
                                   &
                         Vitor Almeida de Carvalho
                                00289783

                                 Turma H
                                2017 / 1
    ***********************************************************************
                              MOTIVADO POR
                          Cláudio Rosito Jung
*******************************************************************************/

/*0-REFERENCIAS
Referência das cores da função textcolor() [Color] ------ [Light Color]
0 = Black -----------  8 = Gray
1 = Blue ------------  9 = Light Blue
2 = Green ----------- 10 = Light Green
3 = Aqua ------------ 11 = Light Aqua
4 = Red ------------- 12 = Light Red
5 = Purple ---------- 13 = Light Purple
6 = Yellow ---------- 14 = Light Yellow
7 = White ----------- 15 = Bright White
*/
//######################1-BIBILIOTECAS############################//

#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

//######################2-DEFINIÇÕES:############################//

// código  das teclas
#define SPACEBAR 32
#define CIMA 72
#define BAIXO 80
#define DIREITA 77
#define ESQUERDA 75
#define ESC 27
#define ENTER 13
// definições de itens do jogo
#define SCORE_INICIAL 1000      // Pontuação Inicial
#define VIDAS_INICIAIS 3        // Número de vidas inicial
#define X_PDR 3                 // Tamanho X padrão para impressão
#define Y_PDR 2                 // Tamanho Y padrão para impressão
#define MAX_OBJ 460             // Numero máximo de objetos possíveis
#define MAX_BARRIL_M 8          // Número máximo de barris móveis simultaneamente na tela
#define X_MAX_ESPELHO 61        // Tamanho X do espelho do jogo
#define Y_MAX_ESPELHO 48        // Tamanho Y do espelho do jogo
#define LINHAS_MAX 23           // Para save
#define COLUNAS_MAX 21          // Para save
#define Y_MAX 48                // Y max do prompt
#define X_MAX 60                // X max do prompt
// cores da função text_color()
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15
// códigos internos
#define ERRO -1
#define NOVOJOGO 0
#define CONTINUARJOGO 1
#define SAIR 3
#define DERROTA 3
#define VITORIA 4
#define PAUSE 5
#define JOGANDO 9
// defines de moventos  OBS.: As velocidades são inversamente proporcionais aos defines
#define GRAVIDADE 13           // Gravidade
#define VEL_MARIO 10           // Velocidade do Mário
#define VEL_BARRIL 10          // Velocidade dos barris móveis
#define VEL_GERA_BARRIL 150    // Velocidade de geração dos barris móveis
#define ATUALIZA_SCORE 150     // Velocidade de atualização de score
//#define DEBUG
#define PDIR 1
#define PESQ 2
#define PCIM 3
#define PBAI 4

//######################3-STRUCTS############################//
//Define estrutura central do programa.
typedef struct tipo_fase       // Struct TIPO_FASE utilizado na leitura dos arquivos binários
{
    char tipo;
    int linha_inicial;
    int coluna_inicial;
    float velocidade;
} TIPO_FASE;

typedef struct controle        // Struct contendo o espelho em tamanho real [46][60] e o espelho [23][20]
{
    char controle[Y_MAX_ESPELHO][X_MAX_ESPELHO]; // Usado em comparações de movimentação
    char save[LINHAS_MAX][COLUNAS_MAX];          // Usado para salvar e carregar os jogos
} CONTROLE;

typedef struct vetor           // Struct a parte para salvar tela
{
    char objeto[4];
} VETOR;

typedef struct jogo           // Struct principal do jogo, contém TUDO
{

    CONTROLE espelho_fixo;            // Espelho fixo
    TIPO_FASE vetor_objetos[MAX_OBJ]; // Vetor de objetos TIPO_FASE lido no bin
    TIPO_FASE mario;                  // Mario
    TIPO_FASE barril_movel[MAX_BARRIL_M];// Vetor TIPO_FASE com todos os barris móveis
    VETOR tela_salva[46][21];    // Para salvar a tela
    int mario_x_origem;               // Coordenadas iniciais do Mário, em caso de morte
    int mario_y_origem;
    int coluna_donk;                  // Coordenadas iniciais do Donkey Kong, para lançar os barris
    int linha_donk;
    int score_atual;                  // Score atual
    int score_max;                    // Score max
    int vidas;                        // Número de vidas
    int num_obj;                      // Número de objetos no mapa, para funções
    int conta_barril;                 // Número de barris móveis

} JOGO;
//#######################4-SKETCHES DE FUNÇÕES############################//

// Funções gerais do jogo
int controle_jogo(JOGO,TIPO_FASE[]); // Função de controle do jogo
int mainMenu();                      // Menu principal do jogo
JOGO novo_jogo(TIPO_FASE[],int);     // Seta os parâmetros para um jogo novo
void salva_jogo(JOGO);               // Salva o jogo
JOGO carrega_jogo();                 // Carrega o jogo
JOGO carrega_mapa();                 // Carrega o mapa
void jogo(JOGO);                     // Função principal do jogo, roda em um loop while
int menu_pausa(JOGO);                // Chama o menu de pause
void salva_tela(JOGO);               // Salva a tela

//Funções de impressões
// -> Telas
void vitoria(JOGO);                    // Imprime tela de vitória
void derrota(JOGO);                    // Imprime tela de derrota
void imprime_tela_inicial();           // Imprime tela inicial
void imprime_instrucoes();             // Imprime tela de instruções
void imprime_menu_pausa();             // Imprime tela de pause
// -> Mário
int move_mario(JOGO*,char,int*);       // Move o Mário uma vez
void apaga(int,int);                   // Apaga uma unidade do mapa em certas coordenadas
void posiciona_mario(int,int);         // Imprime o Mário em certas coordenadas
// -> Fase
void imprime_fase(TIPO_FASE[],int,int);// Imprime a fase utilizando o vetor de objetos{
void imprime_mario(int,int);           // Impressões de objetos{
void imprime_donk1(int,int);
void imprime_donk2(int,int);
void imprime_princesa(int,int);
void imprime_superficie(int,int);
void imprime_escFIM(int,int);
void imprime_escOK(int,int);
void imprime_escQ(int,int);
void imprime_barril_e(int,int);
void imprime_barril_m(int,int);
void imprime_rampadir(int,int);
void imprime_rampaesq(int,int);        // }}

// Mapeamento da fase e testes em geral
CONTROLE mapeia_fase(JOGO*, int,int);                          // Mapeia a fase no espelho
void preenche_espelho(CONTROLE*,int,int,char);   // Inicializa o espelho
CONTROLE escreve_espelho(CONTROLE,int,int,char);               // Escreve no espelho em certa localização
CONTROLE apaga_espelho(CONTROLE,int,int);                      // Apaga certa posição no espelho
CONTROLE restaura_escada(int,int,int,CONTROLE );               // Restaura as escadas caso fosse apagada pelo Mário ou por um barril móvel
int testa_morte_queda(int,int,JOGO);                           // Testa a morte por queda
int testa_vit_ou_der(CONTROLE,int,int,char);                   // Testa se o usuário ganhou/perdeu
void testa_queda(CONTROLE,int*,int*,int*,char*);               // Testa a queda do Mário
void testa_cima(CONTROLE, int*,int*,int*,char*);               // Testes de movimento do Mário{
void testa_direita(CONTROLE,int*,int*,int*,char*);
void testa_esquerda(CONTROLE,int*,int*,int*,char*);
void testa_baixo(CONTROLE,int*,int*,int*,char*);               // }
int testa_fim(CONTROLE,int,int);                               // Testa o fim do jogo
void prepara_barril(JOGO*);                                    // Prepara um barril móvel
void joga_barril(JOGO*);                                       // Joga um barril móvel e mapeia ele
int move_barril(JOGO*,int);                                    // Move os barris móveis
CONTROLE testa_direita_barril(CONTROLE,int *,int*,int*,char*); // Testes de movimento/movimentos dos barris{
CONTROLE testa_esquerda_barril(CONTROLE,int*,int*,int*,char*);
int deixa_descer_escada_b(CONTROLE,int,int);
int cai_barril(JOGO*,int);
CONTROLE desce_barril(CONTROLE,int*,int*,int*,char*);
int habilita_movimento(int,clock_t*,char);                      // }

// Funções ferramentas
void gera_tipo_fase(CONTROLE,int*,JOGO*);    // Gera um vetor TIPO_FASE[]
int ler_bin(TIPO_FASE[]);                    // Lê um arquivo .bin, preenche um vetor TIPO_FASE[] e retorna o número de objetos
void formatprompt();                         // Formata o prompt de comando para o tamanho certo
void hide_cursor();                          // Esconde o cursor do mouse
void gotoxy(int,int);                        // Move o cursor do mouse
void text_color(int);                        // Muda a cor do texto
void cor_escadas(int);                       // Colore as escadas
int setas();                                 // Retorna valores das setas
int tempo(clock_t,clock_t);                  // Retorna um intervalo de tempo

//######################5-MAIN############################//
int main()
{
    TIPO_FASE vetorobjetos[MAX_OBJ];  // Inicializa vetor que irá receber os dados do fase.bin
    JOGO controle_geral;  // Inicializa a variável geral
    int controle;         // Opção do menu
    do
    {
        controle=controle_jogo(controle_geral, vetorobjetos); // Chama a função de controle
    }
    while(controle!=SAIR); // Até que o retorno seja SAIR (#define)
    return 0;
}

//######################6-FUNÇÕES############################//
// Funções gerais do jogo
int controle_jogo(JOGO controle_geral, TIPO_FASE vetorobjetos[])  // Função recursiva multipla, eu chamo um cara,que chama outro e que me chama, até retornar SAIR
{
    int iOpcao,indfor,num_objetos;// Inicialização de variaveis de controle,score e indice do for de inicialização
    for (indfor=0; indfor<MAX_OBJ; indfor++) // Garante que não há lixo no campo tipo do TIPO_FASE
        vetorobjetos[indfor].tipo='Z';
    formatprompt(); // Seta tamanho e lingua
    num_objetos=ler_bin(vetorobjetos); // Chama leitura do binário
    iOpcao=mainMenu(); // Chama a rotina do menu principal, que retorna a opção selecionada pelo usuário.
    switch(iOpcao)  // Teste da opção
    {
    case NOVOJOGO: // Novo jogo
        controle_geral=novo_jogo(vetorobjetos,num_objetos); // Inicializa um novo jogo
        jogo(controle_geral); // Chama a função jogo, onde as coisas realmente acontecem
        break;
    case CONTINUARJOGO : // Carregar jogo
        controle_geral=carrega_mapa(); // Continua um jogo já inicializado
        controle_geral=carrega_jogo(controle_geral);
        jogo(controle_geral); // Chama a função jogo, onde as coisas realmente acontecem
        break;
    case SAIR: // Se sair, sai
        break;
    }
    return iOpcao; // Retorna a opção
}

int mainMenu() // Menu principal do jogo
{
    int x =24, y[] = {27, 31, 35,39};   // Coluna e linhas para exibir seta de opcao do menu
    int iIndicaOpcao = 0, saida = FALSE;    // Contador índice da seta, booleano sair para sair do menu
    imprime_tela_inicial();  // Tela de apresentaçao do jogo

    do // Faz ao menos uma vez
    {
        gotoxy(x, y[iIndicaOpcao]); // Move o cursor para (x, y[opcao])
        printf(">"); // Imprime a seta
        switch(setas()) // Chama a função setas()
        {
        case CIMA:
            if(iIndicaOpcao != 0) // Caso o usuário aperte direcional para cima
            {
                // Se o opcao não estiver na primeira posição
                gotoxy(x, y[iIndicaOpcao]);
                printf(" "); // Apaga a seta anterior
                iIndicaOpcao--; // Decrementa o contador
            }
            else if(iIndicaOpcao == 0)
            {
                // Se o contador estiver na primeira posição
                gotoxy(x, y[iIndicaOpcao]);
                printf(" "); // Apaga a seta anterior
                iIndicaOpcao = 3; // Define contador como 2 (última posição na tela)
            }
            break;

        case BAIXO:
            if(iIndicaOpcao != 3) // Caso o usuário aperte direcional para baixo
            {
                // Se o contador não estiver na última posição
                gotoxy(x, y[iIndicaOpcao]);
                printf(" "); // Apaga a seta anterior
                iIndicaOpcao++; // Incrementa o contador
            }
            else if(iIndicaOpcao == 3)
            {
                // Se o contador estiver na última posição
                gotoxy(x, y[iIndicaOpcao]);
                printf(" "); // Apaga a seta anterior
                iIndicaOpcao = 0; // Define o contador como 0 (primeira posição na tela)
            }
            break;
        case SPACEBAR:
        case ENTER:
            if (iIndicaOpcao==2) // Se o contador estiver em 2
            {
                imprime_instrucoes(); // Imprime as instruções
            }
            else
            {
                saida = TRUE; // Sai do loop
            }
            break;

        case ESC:
            if(iIndicaOpcao != 3) // Se o usuário teclar ESC (No caso seria um atalho para sair do programa rapidamente)
            {
                // Se o contador for diferente de 2
                gotoxy(x, y[iIndicaOpcao]);
                printf(" ");
                iIndicaOpcao = 3; // Move a seta para a última posição
            }
            else return SAIR;
            break;
        }

        Beep(500, 40); // B E E P

    }
    while(!saida);   // Enquanto "não sair"
    return iIndicaOpcao;
}

JOGO novo_jogo(TIPO_FASE array_obj[],int limite) // Começa um jogo novo
{
    system("cls");
    JOGO geral; // Declara uma variável JOGO
    int i;
    for (i=0; i<limite; i++)
        geral.vetor_objetos[i]=array_obj[i]; // Preenche o vetor_objetos interno da variável com o lido no .bin
    geral.vidas=VIDAS_INICIAIS; // Inicializa vidas, scores, número de barris
    geral.score_atual=SCORE_INICIAL;
    geral.score_max=SCORE_INICIAL;
    geral.num_obj=limite;
    geral.conta_barril=0;
    imprime_fase(array_obj,geral.num_obj,geral.vidas); // Imprime a fase
    geral.espelho_fixo=mapeia_fase(&geral,limite,NOVOJOGO); // Mapeia a fase
    return geral; // Retorna a variável tipo JOGO
}

void salva_jogo(JOGO mapa) // Salva o jogo em um arquivo .txt
{

    FILE *jogosalvo;
    char nome_save[40+5]; // Nome de até 40 caracteres + ".txt\0"
    int i,j;
    gotoxy(21, 29);
    printf("Nome do save: ");
    gotoxy(16, 31);
    fflush(stdin);
    gets(nome_save);
    strcat(nome_save, ".txt"); // Concatena o nome digitado ao tipo do arquivo
    jogosalvo=fopen(nome_save, "w"); // Abre um arquivo .txt em modo "write"
    for(i=0; i<LINHAS_MAX; i++)
    {
        for(j=0; j<COLUNAS_MAX; j++)
        {
            fputc(mapa.espelho_fixo.save[i][j],jogosalvo); // Utiliza o campo save[][] da struct CONTROLE
        }
    }
    /* Logo abaixo do mapa, o método salva informações necessárias para o carregamento posterior. Informações salvas na seguinte ordem:
    Coluna_Mario#Linha_Mario#Score_Atual#Vidas#Número_Barris#Coluna_Barril_0#Linha_Barril_0#Velocidade_Barril_0#...#Coluna_Barril_n#Linha_Barril_n#Velocidade_Barril_n */
    fprintf(jogosalvo,"%d#%d#%d#%d#%d",mapa.mario.coluna_inicial,mapa.mario.linha_inicial,mapa.score_atual,mapa.vidas,mapa.conta_barril);
    for(i=0;i<mapa.conta_barril;i++)
        fprintf(jogosalvo,"#%d#%d#%.0f",mapa.barril_movel[i].coluna_inicial,mapa.barril_movel[i].linha_inicial,mapa.barril_movel[i].velocidade);
    fclose(jogosalvo);
    salva_tela(mapa); // Salva a tela
    gotoxy(21, 35);
    printf("Jogo salvo!"); // Imprime mensagem para mostrar que a tela foi salva corretamente
}

JOGO carrega_jogo(JOGO carregado) // Carrega o jogo
{
    CONTROLE antigo=carregado.espelho_fixo; // Auxiliar para impressão do mapa
    system("cls");
    imprime_fase(carregado.vetor_objetos,carregado.num_obj,carregado.vidas); // Imprime a fase
    int i;
    if(carregado.conta_barril>0) // Se tiver barris moveis
        for(i=0;i<carregado.conta_barril;i++)
            imprime_barril_m(carregado.barril_movel[i].coluna_inicial,carregado.barril_movel[i].linha_inicial); // Imprime os barris móveis
    antigo=mapeia_fase(&carregado,carregado.num_obj,CONTINUARJOGO); // Mapeia a fase
    carregado.espelho_fixo=antigo; // Passa do auxiliar para o carregado
    apaga(carregado.mario_x_origem,carregado.mario_y_origem); // Apaga o Mário na origem
    posiciona_mario(carregado.mario.coluna_inicial,carregado.mario.linha_inicial); // Posiciona o Mário na posição lida no arquivo
    return carregado; // Retorna a variável do tipo JOGO
}

JOGO carrega_mapa() // Carrega o mapa
{
    FILE *load;
    JOGO loadgame; // Variável local loadgame que será retornada ao fim da função
    int numobjetos; // Número de objetos
    int i,j;  // Índices
    char *ptr; // Ponteiro que aponta para char, para a utilização da função strtok()
    char stringao[50]; // String que receberá os tokkens
    char key;
    char nome_load[40+5]; // Nome do arquivo a ser carregado 40 caracteres + ".txt\0"
    do
    {
        gotoxy(15,23);
        printf("Nome do arquivo (sem extensão): ");
        gotoxy(15, 25);
        fflush(stdin);
        gets(nome_load); // Lê o nome
        strcat(nome_load, ".txt"); // Concatena o nome com a extensão do arquivo
        load=fopen(nome_load, "r"); // Abre o arquivo em read

        if(!load)
        {
            system("cls");
            gotoxy(3, 27);
            printf("Erro na leitura, o arquivo não existe na pasta do jogo");
            gotoxy(18, 30);
            printf("Insira o nome do arquivo: ");
        }
    }while(!load); // Repete enquanto o usuário não abrir um arquivo que exista

   for(i=0; i<LINHAS_MAX; i++) // Percorre o arquivo texto
   {
        for(j=0; j<COLUNAS_MAX; j++)
        {
            loadgame.espelho_fixo.save[i][j]=getc(load); // Salva o espelho pequeno
        }
    }
    // O numero de bites deslocados é igual a matriz save + \n ao final
    fseek(load,0,SEEK_CUR); // Posiciona o cursor
    fgets(stringao,50,load); // Preenche o stringao
    ptr=strtok(stringao,"#"); // Começa a pegar os tokkens
    loadgame.mario.coluna_inicial=atoi(ptr); // Transforma de char para int/float
    ptr=strtok(NULL,"#"); // Continua de onde parou (por isso NULL)
    loadgame.mario.linha_inicial=atoi(ptr);
    ptr=strtok(NULL,"#");
    loadgame.score_atual=atoi(ptr);
    ptr=strtok(NULL,"#");
    loadgame.vidas=atoi(ptr);
    ptr=strtok(NULL,"#");
    loadgame.conta_barril=atoi(ptr);
    for(i=0;i<loadgame.conta_barril;i++) // Como o número de barris já foi lido, é feito um for
    {                                    // Para ler os dados dos barris móveis
        ptr=strtok(NULL,"#");
        loadgame.barril_movel[i].coluna_inicial=atoi(ptr);
        ptr=strtok(NULL,"#");
        loadgame.barril_movel[i].linha_inicial=atoi(ptr);
        ptr=strtok(NULL,"#");
        loadgame.barril_movel[i].velocidade=atof(ptr);
    }
    loadgame.score_max=SCORE_INICIAL;
    gera_tipo_fase(loadgame.espelho_fixo,&numobjetos,&loadgame); // Gera um TIPO_FASE para impressão
    loadgame.num_obj=numobjetos; // Preenche o num_obj interno da variável loadgame
    return loadgame; // Retorna tudo
}

void jogo(JOGO copia_controle)
{
    int condition = JOGANDO;            // Inicia jogando
    int eh_valido=FALSE;                // Flags para controle dos objetos{
    int flag_mario=TRUE;                // No caso os flags são indicadores do que se pode ou não fazer
    int flag_barril=TRUE;
    int flag_gravidade=TRUE;
    int flag_altera_score=TRUE;
    int flag_gera_barril=TRUE;
    int flag_pausa=FALSE;               // }
    int barril_pronto=FALSE;            // Variável que indica se o barril está pronto para ser jogado
    int status_pulo=-1;
    int coord_x =copia_controle.mario.coluna_inicial; // Auxiliar
    int coord_y =copia_controle.mario.linha_inicial;  // Auxiliar
    int indice_b;
    char key='z';                       // Tecla que vai ser lida para movimentação do Mário
    char reposicao='t';                 // Utilizado na função de restaurar escadas
    clock_t tempo_mario=clock();        // Tempos inicializados{
    clock_t tempo_score=clock();
    clock_t tempo_barril=clock();
    clock_t tempo_gravidade=clock();
    clock_t tempo_gera_barril=clock();  // }
    int condition_anterior;
    text_color(LIGHT_RED);
    gotoxy(0,-1);
    printf("SCORE / SCORE MAX %4d / %4d", copia_controle.score_atual,copia_controle.score_max); // Imprime o score novamente
    while(condition != SAIR && condition != VITORIA && condition != DERROTA) // While que fica rodando durante todo o jogo
    {
        flag_altera_score= habilita_movimento(flag_altera_score,&tempo_score,'Ç'); // Chama a função que controla os flags
        flag_barril=habilita_movimento(flag_barril,&tempo_barril,'B');
        flag_gravidade=habilita_movimento(flag_gravidade,&tempo_gravidade,'Z');
        flag_mario=habilita_movimento(flag_mario,&tempo_mario,'M');
        flag_gera_barril=habilita_movimento(flag_gera_barril,&tempo_gera_barril,'V');
        /* A partir de agora o jogo verifica todos os flags e faz as mudanças necessárias */
        if (flag_altera_score==TRUE && copia_controle.score_atual>0)  // Checa o flag. Se tem permissão para alterar, altera
        {
            copia_controle.score_atual=copia_controle.score_atual-10;  // Perdeu tempo = perdeu pontos
            text_color(LIGHT_RED);
            gotoxy(0,-1);
            printf("SCORE / SCORE MAX %4d / %4d", copia_controle.score_atual,copia_controle.score_max); // Imprime o score novamente
            flag_altera_score=FALSE; // Seta o flag para FALSE, pois já alterou
        }

        if (flag_gera_barril==TRUE && copia_controle.conta_barril<MAX_BARRIL_M)
        {
            // Se pode gerar e não está no número máximo de barris na tela. . .
            if (!barril_pronto)  // Se não estiver pronto
            {
                prepara_barril(&copia_controle); // Prepara o barril
                barril_pronto=TRUE;              // Barril será jogado no próximo ciclo
            }
            else // Senão
            {
                joga_barril(&copia_controle);    // Jogue
                barril_pronto=FALSE;             // Fazer novo barril no proximo ciclo
            }
            flag_gera_barril=FALSE;  // Liga espera de tempo para geração do proximo
        }

        if (flag_pausa==TRUE)   // Acionado ao retornar da pausa
        {
            gotoxy(0,-1); // Imprime o score novamente
            printf("SCORE / SCORE MAX %4d / %4d", copia_controle.score_atual,copia_controle.score_max);
            flag_pausa=FALSE; // Seta o flag para FALSE
        }

       if (flag_gravidade==TRUE)  // Se as condições dizem que a gravidade "deve atuar"
        {
            if (status_pulo<0)
            {   // E se o indicador do pulo é negativo, devo descer
                coord_x=copia_controle.mario.coluna_inicial; // Auxiliares com a posição do Mário
                coord_y=copia_controle.mario.linha_inicial;
                testa_queda((copia_controle.espelho_fixo),&coord_x,&coord_y,&eh_valido,&reposicao); // Testa se está em queda
                if(eh_valido) // Se pode se mover
                {
                    apaga((copia_controle.mario.coluna_inicial),(copia_controle.mario.linha_inicial)); // Apaga o Mário
                    posiciona_mario(coord_x,coord_y); // Posiciona o Mário nas novas coordenadas (que já foram alteradas na testa_queda())
                    copia_controle.mario.coluna_inicial=coord_x; // Por isso vão como ponteiros
                    copia_controle.mario.linha_inicial=coord_y;  // Atualiza as coordenadas de controle do mario
                }
            condition=testa_morte_queda(coord_y,condition,copia_controle);  // Vejo se minha queda foi mortal
            }
            else
            { // Caso o indicador de pulo ainda não seja negativo
                status_pulo++; // Soma mais um no indicador de pulo
                if (status_pulo<=3) // Se o indicador for menor ou igual a 3
                    condition= move_mario(&copia_controle,SPACEBAR,&status_pulo);// Testa se pode subir
                else if (status_pulo==(GRAVIDADE/3+1))status_pulo=-1; // Dessa forma o Mário sempre fica topo por no mínimo dois ciclos da gravidade
                    condition=JOGANDO;
            }
            if (condition!=DERROTA && copia_controle.conta_barril>0) // Se não perdeu e tem barris
            {
                for(indice_b=0;indice_b<=copia_controle.conta_barril-1;indice_b++) // Faz os barris cairem
                    condition=cai_barril(&copia_controle,indice_b);
            }
             flag_gravidade=FALSE; // Seta o flag da gravidade para FALSE

        }

        if (flag_mario==TRUE && kbhit()) // Se o Mário pode se mover e uma tecla foi apertada
        {
            key=getch();  // Lê o buffer e guarda na variável "key"
            fflush(stdin);// Limpa o buffer para as próximas execuções
            if (key==SPACEBAR && status_pulo<1) // Se a tecla for espaço e estiver no ar
                status_pulo++; // Incrementa ao status do pulo
            condition=move_mario(&copia_controle,key,&status_pulo);  // Chama a função que move o mário
            flag_mario=FALSE; // Como já moveu, altera o estado do flag para FALSE

        }

        if (status_pulo==-1 && condition!= PAUSE){
            condition_anterior=testa_vit_ou_der(copia_controle.espelho_fixo,coord_x,coord_y,'B'); // Testa se bateu no barril se não tiver pulando
          //  if (condition_anterior==DERROTA)condition==DERROTA;
        }

        if(flag_barril==TRUE && copia_controle.conta_barril>0 && condition!=DERROTA) // Se pode mover os barris, há barris e o jogador não perdeu
        {
            for(indice_b=0;indice_b<=copia_controle.conta_barril-1;indice_b++) // Percorre os barris
            condition=move_barril(&copia_controle,indice_b); // Move os barris
            flag_barril=FALSE; // Seta o flag para FALSE
        }
        switch(condition) // Testa a condição retornada nas funções
        {

        case JOGANDO: // Se estiver JOGANDO, passa para a próxima iteração
                     break;
        case DERROTA: // Se perder
                     if(copia_controle.vidas == 0) // Se não tiver mais vidas
                     {
                        derrota(copia_controle); // Chama a função derrota
                     }
                     else // Se tiver vidas
                     {
                        (copia_controle.vidas)--;  // Diminui uma vida
                        system("cls"); // Mostra uma tela informativa
                        gotoxy(20, 20);
                        text_color(LIGHT_RED);
                        printf(" Você perdeu uma vida");
                        gotoxy(20, 22);
                        printf("  Vidas restantes: %d",copia_controle.vidas);
                        Sleep(1500); // Pausa o jogo por um determinado tempo
                        system("cls");
                        imprime_fase(copia_controle.vetor_objetos, copia_controle.num_obj,copia_controle.vidas); // Reimprime a fase
                        copia_controle.mario.coluna_inicial=copia_controle.mario_x_origem; // Seta as coordenadas para a origem
                        copia_controle.mario.linha_inicial=copia_controle.mario_y_origem;
                        condition=JOGANDO; // Condição se torna JOGANDO com o intuito de passar para a próxima iteração
                        copia_controle.score_atual=copia_controle.score_atual-200; // Decrementa 200 pontos do score
                        gotoxy(0,-1); // Reimprime o score
                        printf("SCORE / SCORE MAX %4d / %4d", copia_controle.score_atual,copia_controle.score_max);
                        copia_controle.espelho_fixo=mapeia_fase(&copia_controle,copia_controle.num_obj,NOVOJOGO); // Mapeia a fase novamente e apaga os barris
                        copia_controle.conta_barril=0;

                     }
                     break;
        case VITORIA: copia_controle.score_atual+=500; // Se o jogador ganhar, ganha 500 pontos de bônus
                      vitoria(copia_controle); // Chama a tela de vitória
                      break;
        case PAUSE: condition=menu_pausa(copia_controle); // Se estiver em pausa, chama a função do menu de pause
                    system("cls");
                    if(condition!=SAIR) // Se "não sair"
                    {
                        imprime_fase(copia_controle.vetor_objetos, copia_controle.num_obj,copia_controle.vidas); // Reimprime a fase
                        apaga(copia_controle.mario_x_origem,copia_controle.mario_y_origem); // Apaga a primeira posição do Mário que será impressa com a fase
                        posiciona_mario(copia_controle.mario.coluna_inicial,copia_controle.mario.linha_inicial); // Somente reaproveitamento do struct para guardar as posições de jogo
                        gotoxy(0,-1); // Imprime o score
                        text_color(LIGHT_RED);
                        printf("SCORE / SCORE MAX %4d / %4d", copia_controle.score_atual,copia_controle.score_max);
                    }
                    break;
        }

    }
}

int menu_pausa(JOGO copia_controle) // Menu de pause
{
    system("cls");
    int x =19, y[] = {19, 21, 23}; // Coordenadas da seta
    int contador = 0, sair = FALSE,key,retorno; // Contador da seta e variáveis para controle
    imprime_menu_pausa(); // Lê o arquivo do pause
    do // Faz ao menos uma vez
    {
        gotoxy(x, y[contador]); // Move o cursor para (x, y[contador])
        printf(">"); // Imprime a seta
        key=getch();
        switch(key) // Testa os inputs do teclado
        {

        case 's': if(contador!=0)
                  {
                     gotoxy(x, y[contador]); // Move o cursor para (x, y[contador])
                     printf(" ");
                     contador=0;
                     Beep(500, 40);
                  }
                  else
                     salva_jogo(copia_controle); // Salva o jogo e mantém na tela de pause
                  break;

        case 'p': if(contador!=1)
                  {
                     gotoxy(x, y[contador]); // Move o cursor para (x, y[contador])
                     printf(" ");
                     contador=1;
                     Beep(500, 40);
                  }
                  else
                  {
                     retorno= JOGANDO;
                     sair=TRUE; // Retorna ao jogo
                  }
                  break;

        case ESC: if(contador!=2)
                  {
                     gotoxy(x, y[contador]); // Move o cursor para (x, y[contador])
                     printf(" ");
                     contador=2;
                     Beep(500, 40);
                  }
                  else
                  {
                     system("cls");
                     sair=TRUE;
                     retorno= SAIR; // Sai do jogo
                  }
                  break;
        }
    }
    while(!sair);  // Enquanto "não sair"
    return retorno;
}

void salva_tela(JOGO mapa) // Tira um "printscreen" da tela e salva em tela_salva.txt
{
    FILE *tela;
    int numbytesdesloc,i,x,y,in,jn,kn; // Índices, coordenadas...
    tela=fopen("tela_salva.txt","w+"); // Abre arquivo .txt em write+
    if (!(tela))
    {
        printf("ERRO ao salvar a tela");
    }
    else
    {
        fprintf(tela,"VIDAS %d\n",mapa.vidas); // Imprime o cabeçalho
        fprintf(tela,"SCORE ATUAL %4d / SCORE MAX %4d\n",mapa.score_atual,mapa.score_max);

        for (in=0;in<46;in++)
        {
            for (jn=0; jn<21; jn++)
            {   if (jn!=20)
                    strcpy(mapa.tela_salva[in][jn].objeto, "   "); // Preenche objeto com espaços
                else
                    strcpy(mapa.tela_salva[in][jn].objeto,"  \n"); // Ou quebra de linha no caso da última coluna
            }
        }

    for (i=0; i<mapa.num_obj; i++) // Percorre o vetor objetos e copia para objeto a unidade correspondente
    {
        switch(mapa.vetor_objetos[i].tipo)
        {
        case 'P':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"\\o/");
            strcpy(mapa.tela_salva[y+1][x].objeto,"/=\\");
            break;
        case 'K':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"}o{");
            strcpy(mapa.tela_salva[y+1][x].objeto,"/H\\");
            break;
        case 'G':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"-._");
            strcpy(mapa.tela_salva[y][x+1].objeto,"   ");
            strcpy(mapa.tela_salva[y+1][x].objeto,"   ");
            strcpy(mapa.tela_salva[y+1][x+1].objeto,"-._");
            break;
        case 'F':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"   ");
            strcpy(mapa.tela_salva[y][x+1].objeto,"_.-");
            strcpy(mapa.tela_salva[y+1][x].objeto,"_.-");
            strcpy(mapa.tela_salva[y+1][x+1].objeto,"   ");
            break;
        case 'S':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"XXX");
            strcpy(mapa.tela_salva[y+1][x].objeto,"XXX");
            break;
        case 'E':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"|-|");
            strcpy(mapa.tela_salva[y+1][x].objeto,"|-|");
            break;
        case 'Q':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"|-|");
            strcpy(mapa.tela_salva[y+1][x].objeto," -|");
            break;
        case 'H':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"|X|");
            strcpy(mapa.tela_salva[y+1][x].objeto,"|-|");
            break;
        case 'B':
            x=(mapa.vetor_objetos[i].coluna_inicial);
            y=(mapa.vetor_objetos[i].linha_inicial)*2;
            strcpy(mapa.tela_salva[y][x].objeto,"/-\\");
            strcpy(mapa.tela_salva[y+1][x].objeto,"\\Z/");
        }
    }
    x=((mapa.mario.coluna_inicial)/3); // O mapa e os barris são adicionados separadamente pois não estão no espelho
    y=(mapa.mario.linha_inicial);
    strcpy(mapa.tela_salva[y][x].objeto,"\\Ô/");
    strcpy(mapa.tela_salva[y+1][x].objeto,"/H\\");
    for(i=0;i<mapa.conta_barril;i++){
       x=((mapa.barril_movel[i].coluna_inicial)/3);
       y=(mapa.barril_movel[i].linha_inicial);
       strcpy(mapa.tela_salva[y][x].objeto,"/-\\");
       strcpy(mapa.tela_salva[y+1][x].objeto,"\\_/");
    }
    for (in=0;in<46;in++)
    {
       for (jn=0; jn<21; jn++)
       {
           fprintf(tela,"%s",mapa.tela_salva[in][jn].objeto); // Aqui objeto é realmente salvo no arquivo
       }
    }
       fclose(tela);
    }
}


// Funções de impressões
// -> Telas
void vitoria(JOGO parametros) // Imprime tela de vitória
{
    system("cls");
    text_color(LIGHT_AQUA);
    int c;
    FILE *vitoria;
    vitoria = fopen("vitoria.txt", "r"); // Abre um arquivo .txt
    if (vitoria)
    {
        while ((c = getc(vitoria)) != EOF)
            putchar(c); // Imprime os caracteres até chegar ao fim do arquivo
        fclose(vitoria);
    } // Imprime considerações finais
    gotoxy(5, 15);
    printf("Pontuação final: %d / Pontuação máxima: %d", parametros.score_atual, parametros.score_max);
    gotoxy(7, 17);
    printf("Pressione ESC para retornar ao menu. . .");
    while(c != ESC)
    {
        if(kbhit())
        {
            c=getch();
            if(c==ESC)
            {
                Beep(500, 40);
            }
        }
    }
    system("cls");
}

void derrota(JOGO parametros) // Imprime tela de derrota
{
    system("cls");
    text_color(BRIGHT_WHITE);
    int c;
    FILE *derrota;
    derrota = fopen("derrota.txt", "r"); // Abre um arquivo .txt
    if (derrota)
    {
        while ((c = getc(derrota)) != EOF)
            putchar(c); // Imprime todos os caracteres até chegar ao fim do arquivo
        fclose(derrota);
    } // Imprime considerações finais
    gotoxy(5, 15);
    printf("Pontuação final: %d / Pontuação máxima: %d", parametros.score_atual, parametros.score_max);
    gotoxy(7, 17);
    printf("Pressione ESC para retornar ao menu. . .");
    while(c != ESC)
    {
        if(kbhit())
        {
            c=getch();
            if(c==ESC)
            {
                Beep(500, 40);
            }
        }
    }
    system("cls");
}

void imprime_tela_inicial() // Imprime a tela inicial
{
    text_color(LIGHT_RED); // Cor vermelho claro
    system("cls"); // Limpa a tela
    int c;
    FILE *menu;
    menu = fopen("menu.txt", "r"); // Lê um arquivo de texto
    if (menu)
    {
        while ((c = getc(menu)) != EOF)
            putchar(c); // Imprime os caracteres do .txt
        fclose(menu);
    }
}

void imprime_instrucoes()
{
    system("cls"); // Limpa a tela
    text_color(LIGHT_GREEN); // Cor verde claro
    int c;
    FILE *info;
    info = fopen("menuinfo.txt", "r"); // Abre um .txt
    if (info)
    {
        while ((c = getc(info)) != EOF)
            putchar(c); // Imprime os caracteres do .txt
        fclose(info);
        getch();
    }
    else
    {
        printf("Erro ao apresentar as intruções, verifique a existencia do arquivo menuinfo.txt");
        printf("\nPressione qualquer tecla para retornar ao menu");
        getch();
    }
    imprime_tela_inicial(); // Volta à tela inicial
}

void imprime_menu_pausa() // Lê o arquivo do pause
{
    system("cls");
    Beep(500, 40);
    text_color(AQUA);
    int c;
    FILE *info;
    info = fopen("pausa.txt", "r"); // Lê um arquivo .txt
    if (info)
    {
        while ((c = getc(info)) != EOF)
            putchar(c); // Imprime os caracteres até terminar o arquivo
        fclose(info);
    }
    else
    {
        system("cls");
        printf("Erro ao apresentar as instruções, verifique a existencia do arquivo pausa.txt");
        printf("\nPressione qualquer tecla para retornar ao jogo");
        getch();
    }
}
// -> Mário
int move_mario(JOGO *parametros, char key,int *status_pulo) // Move o personagem principal Mário
{
    int coord_x= parametros->mario.coluna_inicial; // Coordenadas auxiliares
    int coord_y= parametros->mario.linha_inicial;
    int eh_valido=FALSE; // Variável que dita se é válido
    int pulando=*status_pulo; // Estado do pulo
    char restaura_mapa='Z';
    CONTROLE interno = parametros->espelho_fixo; // Espelho auxiliar
    key = toupper(key);   // Muda a tecla recebida para maiúsculo
    switch(key)
    {
    /*Os casos da tecla no caso de movimento são padronizados, porém cada um para seu respectivo lado*/
    case 'A': testa_esquerda(interno,&coord_x,&coord_y,&eh_valido,&restaura_mapa); // Chama a função que testa se há algo na esquerda
              switch(eh_valido)
              {
                 case TRUE: // Se for válido
                 parametros->mario.velocidade=PESQ; // O Mário está andando para a esquerda
                 apaga((parametros->mario.coluna_inicial),(parametros->mario.linha_inicial)); // Apaga Mário na tela
                 parametros->espelho_fixo=restaura_escada(PESQ,coord_x,coord_y,parametros->espelho_fixo);  // Restaura a escada
                 posiciona_mario(coord_x,coord_y);                                            // Mostra Mário na tela
                 parametros->mario.coluna_inicial=coord_x;  // Guarda nova coordenada mario
                 parametros->mario.linha_inicial=coord_y;   // Guarda nova coordenada mario
                 break;
                 case DERROTA: return DERROTA; break; // Se for DERROTA retorna DERROTA
                 case VITORIA: return VITORIA; break; // Se for VITÓRIA retorna VITÓRIA
              }
              break;

    case 'D': testa_direita(interno,&coord_x,&coord_y,&eh_valido,&restaura_mapa);
              switch(eh_valido)
              {
                 case TRUE:
                 apaga((parametros->mario.coluna_inicial),(parametros->mario.linha_inicial));
                 parametros->espelho_fixo=restaura_escada(PDIR,coord_x,coord_y,parametros->espelho_fixo);
                 posiciona_mario(coord_x,coord_y);
                 parametros->mario.coluna_inicial=coord_x;
                 parametros->mario.linha_inicial=coord_y;
                 break;
                 case DERROTA: return DERROTA; break;
                 case VITORIA: return VITORIA; break;
              }
              break;

    case 'W': testa_cima(interno,&coord_x,&coord_y,&eh_valido,&restaura_mapa);
              switch(eh_valido)
              {
                 case TRUE:
                 apaga((parametros->mario.coluna_inicial),(parametros->mario.linha_inicial));
                 parametros->espelho_fixo=restaura_escada(PCIM,coord_x,coord_y,parametros->espelho_fixo);
                 posiciona_mario(coord_x,coord_y);
                 parametros->mario.coluna_inicial=coord_x;
                 parametros->mario.linha_inicial=coord_y;
                 break;
              case DERROTA: return DERROTA; break;
              case VITORIA: return VITORIA; break;
              }
              break;

    case 'S': testa_baixo(interno,&coord_x,&coord_y,&eh_valido,&restaura_mapa);
              switch(eh_valido)
              {
                 case TRUE:
                 apaga((parametros->mario.coluna_inicial),(parametros->mario.linha_inicial));
                 parametros->espelho_fixo=restaura_escada(PBAI,coord_x,coord_y,parametros->espelho_fixo);
                 posiciona_mario(coord_x,coord_y);
                 parametros->mario.coluna_inicial=coord_x;
                 parametros->mario.linha_inicial=coord_y;
                 break;
                 case DERROTA: return DERROTA; break;
                 case VITORIA: return VITORIA; break;
              }
              break;
    case  'P': return PAUSE; // Se a tecla for 'P', retorna pause
    case SPACEBAR:if ((pulando)>=0 && pulando<=2) // Se não estiver no topo
                  {
                     if( parametros->espelho_fixo.controle[coord_y+2][coord_x]  =='S'  // Pulo na extrema direita
                     ||  parametros->espelho_fixo.controle[coord_y+3][coord_x  ]=='S'  // Sigo subindo na extrema direita
                     ||  parametros->espelho_fixo.controle[coord_y+2][coord_x+2]=='S'  // Pulo na extrema esquerda
                     ||  parametros->espelho_fixo.controle[coord_y+3][coord_x+2]=='S'  // Sigo subindo na extrema esquerda
                     ||  parametros->espelho_fixo.controle[coord_y+2][coord_x]  =='H'  // Pulo na escada
                     ||  parametros->espelho_fixo.controle[coord_y+3][coord_x]  =='H'
                     ||  parametros->espelho_fixo.controle[coord_y+2][coord_x+2]=='G'  //Pulo em rampas à esquerda
                     ||  parametros->espelho_fixo.controle[coord_y+3][coord_x+2]=='G'
                     ||  parametros->espelho_fixo.controle[coord_y+2][coord_x+2]=='F'  // Pulo em rampas à direita
                     ||  parametros->espelho_fixo.controle[coord_y+3][coord_x+2]=='F')
                     {
                        apaga(coord_x,coord_y); // Apaga o Mário
                        coord_y--;              // Sobe
                        parametros->espelho_fixo=restaura_escada(PCIM,coord_x,coord_y,parametros->espelho_fixo); // Restaura a escada
                        posiciona_mario(coord_x,coord_y); // Coloca o Mário
                        parametros->mario.coluna_inicial=coord_x; // Atualiza as coordenadas com as auxiliares
                        parametros->mario.linha_inicial=coord_y;
                     }
                 }
                break;
    }
    return JOGANDO; // Retorna JOGANDO (Continuar)
}

void apaga(int x,int y) // Apaga algo em (x,y)
{
    gotoxy(x, y);
    printf("   ");
    gotoxy(x, (y+1));
    printf("   ");
}

void posiciona_mario(int x,int y) // Imprime o Mário em (x,y)
{
    text_color(LIGHT_RED);
    gotoxy(x,y);
    printf("\\ô/");
    gotoxy(x, (y)+1);
    printf("/H\\");
    hide_cursor();
}
// -> Fase
void imprime_fase(TIPO_FASE vetor_objetos[],int qtd_objetos,int vidas) // Imprime o mapa
{
    int i; // Índices
    int x,y; // Coordenadas auxiliares
    for (i=0; i<qtd_objetos; i++) // Percorre todo o vetor de objetos
    {
        switch(vetor_objetos[i].tipo) // Switch com o tipo
        {

        case 'M':
            x=(vetor_objetos[i].coluna_inicial)*3; // Coordenada X -> É multiplicado pois o mapa é uma matriz [23][20]
            y=(vetor_objetos[i].linha_inicial)*2;  // E por isso as coordenadas normais não batem com o desejado para o mapa
            imprime_mario(x,y);                    // Imprime o objeto correspondente
            break;
        case 'P':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_princesa(x,y);
            break;
        case 'K':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_donk1(x,y);
            break;
        case 'G':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_rampaesq(x,y);
            break;
        case 'F':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_rampadir(x,y);
            break;
        case 'S':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_superficie(x,y);
            break;
        case 'E':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_escOK(x,y);
            break;
        case 'Q':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_escQ(x,y);
            break;
        case 'H':
            x=(vetor_objetos[i].coluna_inicial)*3;
            y=(vetor_objetos[i].linha_inicial)*2;
            imprime_escFIM(x,y);
            break;
        case 'B':
            if(vetor_objetos[i].velocidade>0) // Se o barril tiver velocidade
            {
                x=(vetor_objetos[i].coluna_inicial)*3;
                y=(vetor_objetos[i].linha_inicial)*2;
                imprime_barril_m(x,y); // Imprime barril móvel
            }
            else // Senão
            {
                x=(vetor_objetos[i].coluna_inicial)*3;
                y=(vetor_objetos[i].linha_inicial)*2;
                imprime_barril_e(x,y); // Imprime barril estático
            }
            break;
        }

    }

    gotoxy(0,-2); // Imprime número de vidas
    printf("VIDAS %d \n", vidas);
}

/* As funções "imprime_XXX" são todas iguais, mudando apenas a matriz de impressão e a cor */
void imprime_mario(int x, int y) // Imprime o Mário
{
    char M[Y_PDR][X_PDR]= {{'\\','ô','/'},{'/','H','\\'}}; // Matriz do mário
    int i,j; // Controle de linhas e colunas
    for (i=0; i<Y_PDR; i++) // Percorre linhas e colunas
    {
        gotoxy(x,y+i); // Move o cursor
        for (j=0; j<X_PDR; j++)
        {
            printf("%c", M[i][j]); // Imprime o caractere correspondente
        }
    }
}

void imprime_donk1(int x, int y)
{
    char D[Y_PDR][X_PDR]= {{'}','ô','{'},{'_','H','_'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            text_color(YELLOW);
            printf("%c", D[i][j]);
        }
    }
}

void imprime_donk2(int x, int y)
{
    char D[Y_PDR][X_PDR]= {{'\\','o','/'},{'/',' ','\\'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            text_color(YELLOW);
            printf("%c", D[i][j]);
        }
    }
}

void imprime_princesa(int x, int y)
{
    char P[Y_PDR][X_PDR]= {{'\\','o','/'},{'/','=','\\'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            text_color(LIGHT_PURPLE);
            printf("%c", P[i][j]);
        }
    }
}

void imprime_superficie(int x, int y)
{
    char S[Y_PDR][X_PDR]= {{'X','X','X'},{'X','X','X'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            text_color(LIGHT_RED);
            printf("%c", S[i][j]);
        }
    }
}

void imprime_escFIM(int x, int y)
{
    char H[Y_PDR][X_PDR]= {{'|','X','|'},{'|','-','|'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            cor_escadas(j);
            printf("%c", H[i][j]);
        }
    }
}

void imprime_escOK(int x, int y)
{
    char E[Y_PDR][X_PDR]= {{'|','-','|'},{'|','-','|'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            cor_escadas(j);
            printf("%c", E[i][j]);
        }
    }
}

void imprime_escQ(int x, int y)
{

    char Q[Y_PDR][X_PDR]= {{'|','-','|'},{' ','-','|'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            cor_escadas(j);
            printf("%c", Q[i][j]);
        }
    }
}

void imprime_barril_e(int x, int y)
{
    char B1[Y_PDR][X_PDR]= {{'/','^','\\'},{'\\','Z','/'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            text_color(GRAY);
            printf("%c", B1[i][j]);
        }
    }
}

void imprime_barril_m(int x, int y)
{
    char B2[Y_PDR][X_PDR]= {{'/','^','\\'},{'\\',' ','/'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR; j++)
        {
            text_color(GRAY);
            printf("%c", B2[i][j]);
        }
    }
}

void imprime_rampadir(int x, int y)
{
    char RD[Y_PDR][X_PDR*2]= {{' ',' ',' ','_','.','-'},{'_','.','-',' ',' ',' '}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR*2; j++)
        {
            text_color(LIGHT_RED);
            printf("%c", RD[i][j]);
        }
    }
}

void imprime_rampaesq(int x, int y)
{
    char RD[Y_PDR][X_PDR*2]= {{'-','.','_',' ',' ',' '},{' ',' ',' ','-','.','_'}};
    int i,j;
    for (i=0; i<Y_PDR; i++)
    {
        gotoxy(x,y+i);
        for (j=0; j<X_PDR*2; j++)
        {
            text_color(LIGHT_RED);
            printf("%c", RD[i][j]);
        }
    }
}


// Mapeamento da fase e testes em geral
CONTROLE mapeia_fase(JOGO *mapadojogo,int qtd_objetos, int flag) // Retorna um espelho dos objetos
{
    int i, x, y,i_linha,i_coluna,in,jn; // Índices de linhas e colunas e (x,y) auxiliares
    int indice_bM=0; // Índices de barris
    CONTROLE mapa;
    for (i_linha=0; i_linha<Y_MAX_ESPELHO; i_linha++) // Percorre linhas do espelho
    {
        for (i_coluna=0; i_coluna<X_MAX_ESPELHO; i_coluna++) // Percorre colunas do espelho
        {
            if (i_coluna==(X_MAX_ESPELHO-1))
                mapa.controle[i_linha][i_coluna]='\n'; // Se for no final da linha, coloca quebra de linha
            else                                       // Isso permite manter a forma do mapa
                mapa.controle[i_linha][i_coluna]=' ';  // Senão coloca um espaço que poderá ser preenchido
        }
    }
    for(in=0; in<LINHAS_MAX; in++) // Faz o mesmo para o espelho de save
    {
        for(jn=0; jn<COLUNAS_MAX; jn++)
        {
            mapa.save[in][jn]= ' ';
            if (jn== (COLUNAS_MAX-1))mapa.save[in][jn]='\n';
        }
    }
    mapadojogo->num_obj=qtd_objetos;
    for (i=0; i<qtd_objetos; i++) // Percorre os objetos
    {
        switch(mapadojogo->vetor_objetos[i].tipo) // Analisa cada caso do tipo
        {
        case 'M':
            y = (mapadojogo->vetor_objetos[i].linha_inicial);
            x = (mapadojogo->vetor_objetos[i].coluna_inicial);
            if (flag!=CONTINUARJOGO) // Se o usuário continuar o jogo
            {
                mapadojogo->mario.coluna_inicial=x*3; // Posiciona o Mário nas coordenadas lidas no arquivo
                mapadojogo->mario.linha_inicial=y*2;
            }
            else // Senão
            {
                mapadojogo->mario_x_origem=x*3; // Posiciona o Mário na origem
                mapadojogo->mario_y_origem=y*2;
            }
            mapa.save[y][x]='M';
            break;
        case 'P':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'P'); // No caso de objetos não móveis o espelho fixo (em tamanho real) é preenchido conforme encontra os caracteres
            mapa.save[y][x]='P';
            break;
        case 'K':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'K');
            mapa.save[y][x]='K';
            mapadojogo->coluna_donk=x*3;
            mapadojogo->linha_donk=y*2;
            break;
        case 'G':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'G');
            mapa.save[y][x]='G';
            break;
        case 'F':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'F');
            mapa.save[y][x]='F';
            break;
        case 'S':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'S');
            mapa.save[y][x]='S';
            break;
        case 'E':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'E');
            mapa.save[y][x]='E';
            break;
        case 'Q':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'Q');
            mapa.save[y][x]='Q';
            break;
        case 'H':
            y=(mapadojogo->vetor_objetos[i].linha_inicial);
            x=(mapadojogo->vetor_objetos[i].coluna_inicial);
            preenche_espelho(&mapa,x*3,y*2,'H');
            mapa.save[y][x]='H';
            break;
        case 'B':
            if(mapadojogo->vetor_objetos[i].velocidade==0)
            {
                y=(mapadojogo->vetor_objetos[i].linha_inicial);
                x=(mapadojogo->vetor_objetos[i].coluna_inicial);
                preenche_espelho(&mapa,x*3,y*2,'B');
                mapa.save[y][x]='B';
            }
            break;
        }

    }
    return mapa; // Retorna a variável local
}

void preenche_espelho(CONTROLE *mapa,int x,int y, char obj) // Preenche uma unidade no espelho
{
    int i,j;
    if (obj == 'F') // Casos das rampas
    {
        for (i=y; i<y+2; i++)
            for  (j=x; j<x+3; j++)
            {
                if (i==y)mapa -> controle[i][j+3] = obj;
                else mapa -> controle[i][j] = obj;
            }
    }
    else if (obj =='G')
    {
        for (i=y; i<y+2; i++)
            for  (j=x; j<x+3; j++)
            {
                if (i==y+1)mapa -> controle[i][j+3] = obj;
                else mapa -> controle[i][j] = obj;
            }
    }
    else
    {
        for (i=y; i<y+2; i++)              // EX.: Superfície:
            for (j=x; j<x+3; j++)          //        SSS        Uma sequência de caracteres desse tipo
                mapa->controle[i][j]=obj;  //        SSS        será escrita no espelho
    }
}

CONTROLE escreve_espelho(CONTROLE espelho_fixo, int coord_x,int coord_y,char para_imprimir)
{
    int i,j;
    j=coord_x;
    i=coord_y;
    for (i=0; i<Y_PDR; i++)
        for(j=0; j<X_PDR; j++)
            espelho_fixo.controle[i+coord_y][j+coord_x]=para_imprimir;
    return espelho_fixo;
}

CONTROLE apaga_espelho(CONTROLE espelho_fixo, int coord_x,int coord_y)
{
    int i,j;
    j=coord_x;
    i=coord_y;
    for (i=0; i<Y_PDR; i++)
        for(j=0; j<X_PDR; j++)
            espelho_fixo.controle[i+coord_y][j+coord_x]=' ';
    return espelho_fixo;
}

CONTROLE restaura_escada(int sentido,int x,int y,CONTROLE a) // Restaura escada apagada
{
    char teste;
    int testa=sentido;
    switch(testa)
    {
    case PCIM: if ((y%2)==1)
               {
                    y=y+1;
                    teste=a.save[y/2][x/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x,y);
                        a=escreve_espelho(a,x,y,teste);
                    }
                    else if (teste=='H')
                    {
                        imprime_escFIM(x,y);
                        a=escreve_espelho(a,x,y,teste);
                    }
                }
                else
                {
                    teste=a.save[(y+2)/2][x/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x,y+2);
                        a=escreve_espelho(a,x,y+2,teste);
                    }
                    else if (teste=='H')
                    {
                        imprime_escFIM(x,y+2);
                        a=escreve_espelho(a,x,y+2,teste);
                    }
                }
                break ;
    case PDIR: if(x%3==0)
               {
                    teste=a.save[y/2][(x-3)/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x-3,y);
                        a=escreve_espelho(a,x-3,y,teste);
                    }
                }
                else if(x%3==1)
                {
                    teste=a.save[y/2][(x-1)/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x-1,y);
                        a=escreve_espelho(a,x-1,y,teste);
                    }
                }
                else
                {
                    teste=a.save[y/2][(x-2)/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x-2,y);
                        a=escreve_espelho(a,x-2,y,teste);
                    }
                }
                break;
    case PESQ: if(x%3==0)
               {
                    teste=a.save[y/2][(x+3)/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x+3,y);
                        a=escreve_espelho(a,x+3,y,teste);
                    }
                }
                else if(x%3==1)
                {
                    teste=a.save[y/2][(x+2)/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x+2,y);
                        a=escreve_espelho(a,x+2,y,teste);
                    }
                }
                else
                {
                    teste=a.save[y/2][(x+1)/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x+1,y);
                        a=escreve_espelho(a,x+1,y,teste);
                    }
                }
                break;
    case PBAI: if ((y%2)==1)
               {
                    y=y-1;
                    teste=a.save[y/2][x/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x,y);
                        a=escreve_espelho(a,x,y,teste);
                    }
                    else if (teste=='H')
                    {
                        imprime_escFIM(x,y);
                        a=escreve_espelho(a,x,y,teste);
                    }
                }
                else
                {
                    teste=a.save[(y-2)/2][x/3];
                    if (teste=='E')
                    {
                        imprime_escOK(x,y-2);
                        a=escreve_espelho(a,x,y-2,teste);
                    }
                    else if (teste=='H')
                    {
                        imprime_escFIM(x,y-2);
                        a=escreve_espelho(a,x,y-2,teste);
                    }
                    else if (teste=='Q')
                    {
                        imprime_escQ(x,y-2);
                        a=escreve_espelho(a,x,y-2,teste);
                    }
                }

                break;
    }
    return a;
}

int testa_morte_queda(int y,int condition, JOGO teste) // Testa se a queda foi mortal
{
    if (y==44                                                                                                  // Se
            || teste.espelho_fixo.controle[teste.mario.linha_inicial+Y_PDR][teste.mario.coluna_inicial]== 'K'  // Bateu no Donkey Kong
            || teste.espelho_fixo.controle[teste.mario.linha_inicial+Y_PDR][teste.mario.coluna_inicial+1]== 'K'// Ou
            || teste.espelho_fixo.controle[teste.mario.linha_inicial+Y_PDR][teste.mario.coluna_inicial+2]== 'K'
            || teste.espelho_fixo.controle[teste.mario.linha_inicial+Y_PDR][teste.mario.coluna_inicial]== 'B'  // Bateu nos barris
            || teste.espelho_fixo.controle[teste.mario.linha_inicial+Y_PDR][teste.mario.coluna_inicial+1]== 'B'
            || teste.espelho_fixo.controle[teste.mario.linha_inicial+Y_PDR][teste.mario.coluna_inicial+2]== 'B')
    {
        return DERROTA; // Condição se torna DERROTA
    }
    else return condition; // Senão retorna a condição inalterada
}

int testa_vit_ou_der(CONTROLE posicao, int x, int y,char teste)
{
    int linha,coluna;
    coluna=x;
    linha=y;
    int flag_controle=0;
    if (      posicao.controle[linha][coluna+1]== teste
            ||  posicao.controle[linha][coluna+2]== teste
            ||  posicao.controle[linha][coluna]== teste // Caso atinja pela direita
            ||  posicao.controle[linha+1][coluna]== teste
            ||  posicao.controle[linha+1][coluna+1]== teste // Caso atinja por baixo
            ||  posicao.controle[linha+1][coluna+2]== teste)
        flag_controle=TRUE;
    if (flag_controle)
    {
        switch(teste)
        {
        case 'K':
        case 'B':
            return DERROTA;
            break;
        case 'P':
            return VITORIA;
            break;
        }
    }
    else return JOGANDO;
}

void testa_queda(CONTROLE posicao, int *x, int *y,int *flag,char *restaura) // Testa a queda de um objeto
{
    int linha,coluna; // Linhas e colunas
    coluna=*x; // Inicializadas com os valores passados na função
    linha=*y;

    if (   posicao.controle[linha+Y_PDR][coluna]   ==' '  // Se houver um espaço em baixo
        && posicao.controle[linha+Y_PDR][coluna+1] ==' '
        && posicao.controle[linha+Y_PDR][coluna+2] ==' ')
    {
        *restaura=posicao.controle[linha-1][coluna]; // Grava o que está em cima
        linha++; // Muda a cordenada para descer
        *x=coluna; // Atualiza as coordenadas do Mário
        *y=linha;
        *flag=TRUE;// Autoriza a cair (flag da função void JOGO)
    }

    else *flag=FALSE; // Senão, não autoriza cair
}

void testa_cima(CONTROLE posicao, int *x, int *y,int *flag,char *restaura)
{
    int linha,coluna;
    coluna=*x;
    linha=*y;
    int bPodeSubir=FALSE;
    if (    (     posicao.controle[linha-1][coluna]   == 'E'   // Todo corpo de escada acima de mim
               && posicao.controle[linha-1][coluna+1] == 'E'
               && posicao.controle[linha-1][coluna+2] == 'E'
            )  || posicao.controle[linha-1][coluna]   == 'H'   // Topo de escada acima de mim (ainda estou subindo)
               || posicao.controle[linha+1][coluna]   == 'H')  // Topo de escada nos meus pes (não terminei de subir)
    {
           bPodeSubir=TRUE;
    }

    if (bPodeSubir) {
       *restaura=posicao.controle[linha-1][coluna];
        linha--;
        *x=coluna;
        *y=linha;
        *flag=1;  // Movimento valido
    }
    else
        *flag=testa_fim(posicao,(coluna),linha-1);  // Verifique o que está acima
}

void testa_baixo(CONTROLE posicao, int *x, int *y,int *flag,char *restaura) // Verifica o que está em baixo
{
    int linha,coluna;
    coluna=*x;
    linha=*y;
    int bPodeDescer=FALSE;
    if (   posicao.controle[linha+2][coluna]   == 'S' // Superficie não autoriza descer
        || posicao.controle[linha+2][coluna+1] == 'S'
        || posicao.controle[linha+2][coluna+2] == 'S'
        || posicao.controle[linha+2][coluna]   == 'Q' // Escada quebrada não autoriza descer
        || posicao.controle[linha+2][coluna+1] == 'Q'
        || posicao.controle[linha+2][coluna+2] == 'Q'
        )
    {
        bPodeDescer=FALSE;
    }
    else if (    posicao.controle[linha+2][coluna]   == 'H'  // desce pelo topo de escada
              || posicao.controle[linha+2][coluna+1] == 'H'
              || posicao.controle[linha+2][coluna+2] == 'H'
              || posicao.controle[linha+2][coluna]   == 'E'  // desce pelo corpo de escada
              || posicao.controle[linha+2][coluna+1] == 'E'
              || posicao.controle[linha+2][coluna+2] == 'E')
    {
          bPodeDescer=TRUE;
    }
    if (bPodeDescer) {
        *restaura=posicao.controle[linha+2][coluna];
        linha++;
        *x=coluna;
        *y=linha;
        *flag=1;
    }
    else
        *flag=testa_fim(posicao,(coluna),linha+2);  // Verifica o que está abaixo
}

void testa_direita(CONTROLE posicao, int *x, int *y, int *flag,char *restaura)  // Verifica o que está na direita
{
    int linha,coluna;
    coluna=*x;
    linha=*y;
    int bPodeMover=FALSE;
    if (   posicao.controle[linha+2][coluna]   == 'H'  // Tem escada abaixo
        || posicao.controle[linha+2][coluna]   == 'S'  // Tem superficie abaixo
        || posicao.controle[linha+2][coluna+1] == 'S'
        || posicao.controle[linha+2][coluna+2] == 'S'
        || posicao.controle[linha+2][coluna]   == 'F'  // Tem rampa a direita para subir
        || posicao.controle[linha+2][coluna+2] == 'F'
        || posicao.controle[linha+2][coluna+1] == 'G'
        || posicao.controle[linha+2][coluna]   == ' '
        || posicao.controle[linha+3][coluna]   == ' ')
    {

        if  (  posicao.controle[linha][coluna+3]   == ' '
             &&posicao.controle[linha+1][coluna+3] == ' ')  // Nada a minha direita
        {
            bPodeMover=TRUE;
        }
        else
        {
            if (posicao.controle[linha][coluna+3] == 'E'
                && posicao.controle[linha+1][coluna+3] == 'E'
                && posicao.controle[linha+2][coluna+3] != 'E' )
            {
                *restaura='E';
                bPodeMover=TRUE;
            }
            else if (posicao.controle[linha+1][coluna+3] == 'F')   // Rampa à direita subo
            {
                linha--;
                bPodeMover=TRUE;
            }
            else if (posicao.controle[linha+3][coluna+1] == 'G')   // Rampa à esquerda deso
            {
                linha++;  //acho que eh mais
                bPodeMover=TRUE;
            }
        }

    }
    else if (posicao.controle[linha+3][coluna+1] == 'G'
             || posicao.controle[linha+3][coluna+1] == 'S'
             || posicao.controle[linha+3][coluna+1] == 'H'){
        if (posicao.controle[linha-1][coluna] !='E'){
        linha++;              // Desce, aumentando a linha
        bPodeMover=TRUE;
        }     // Pode mover
    }    // Rampa dir

     if (bPodeMover)
        {
            coluna++;
            *x=coluna;
            *y=linha;
            *flag=1;
        }
        else
            *flag=testa_fim(posicao,(coluna+3),linha);  // Verifique o que esta ao lado


}

void testa_esquerda(CONTROLE posicao, int *x, int *y,int *flag,char *restaura) // Verifica o que está na esquerda do Mário
{
    int linha,coluna; // Linhas e colunas auxiliares
    coluna=*x; // Troca dos valores para as auxiliares
    linha=*y;
    int bPodeMover=FALSE;
    // Primeiro vê se a esquerda não é problema
    if (   posicao.controle[linha+3][coluna+2] == 'H'    // Escada abaixo de mim, a esquerda
        || posicao.controle[linha+2][coluna  ] == 'S'    // Superficie
        || posicao.controle[linha+2][coluna+2] == 'S'    // Superficie
        || posicao.controle[linha+2][coluna  ] == 'G'    // Rampa esq
        || posicao.controle[linha+2][coluna+2] == 'G'    // Rampa esq
        || posicao.controle[linha+2][coluna+1] == 'F'    // Rampa dir
        || posicao.controle[linha+2][coluna]   == ' '
        || posicao.controle[linha+3][coluna]   == ' ')
    {

        if(posicao.controle[linha+1][coluna-1] == ' '
                &&posicao.controle[linha+1][coluna-1] == ' ')   // tudo limpo a esquerda
        {
            bPodeMover=TRUE;

        }
        else
        {
            if (posicao.controle[linha][coluna-1] == 'E'// há uma escada a esquerda
               && posicao.controle[linha+1][coluna-1] == 'E'
               && posicao.controle[linha+2][coluna-1] != 'E')    // para o pulo
            {
                *restaura='E';      // precisa restaurar o desenho
                bPodeMover=TRUE;    // pode mover
            }
            else if (posicao.controle[linha+1][coluna-1] == 'G')      // há uma rampa a esquerda, suba
            {
                linha--;              // suba, diminuindo a linha
                bPodeMover=TRUE;      // pode mover
            }
            else if (posicao.controle[linha+3][coluna-1] == 'F')      // há uma rampa a direita, desça
            {
                linha++;              // desce, aumentando a linha
                bPodeMover=TRUE;      // pode mover
            }
        }

    }
    else if (posicao.controle[linha+2][coluna+2] == 'F'){
        linha++;              // desce, aumentando a linha
        bPodeMover=TRUE;      // pode mover
    }    // rampa dir  ERA 2
    if (bPodeMover)
        {
            coluna--;    // vá para a esquerda
            *x=coluna;   // modifique as coordenadas do objeto
            *y=linha;    // modifique as coordenadas do objeto
            *flag=1;     // movimento deu certo
        }
    else
        *flag=testa_fim(posicao,(coluna-1),linha);
}

int testa_fim(CONTROLE posicao, int coluna, int linha)
{
      int resultado[3];
      int retorno;
      resultado[0]=testa_vit_ou_der(posicao,coluna,linha,'B');  // Bateu num barril
      resultado[1]=testa_vit_ou_der(posicao,coluna,linha,'K');  // Bateu no kong
      resultado[2]=testa_vit_ou_der(posicao,coluna,linha,'P');  // Bateu na princesa
      if (resultado[0]==JOGANDO && resultado[1]==JOGANDO && resultado[2]==JOGANDO) { // Não bateu em ninguém
          retorno=JOGANDO;  // Prosseguir
         }
      else if (resultado[2]==VITORIA) {  // Ganha se bateu na princesa
           retorno=VITORIA;
         }
      else retorno=DERROTA;
      return retorno;
 }

void prepara_barril(JOGO *jogo)
{
    int coord_x,coord_y;
    coord_x=jogo->coluna_donk;
    coord_y=jogo->linha_donk-2;
    imprime_donk2(jogo->coluna_donk,jogo->linha_donk);  // anima donkey kong
    imprime_barril_m(coord_x,coord_y);                  // Acima do donkey kong
    jogo->espelho_fixo=escreve_espelho(jogo->espelho_fixo,coord_x,coord_y,'B');

}

void joga_barril(JOGO *jogo)  // Ponteiro pois vamos mapear os barris moveis
{
    int indice ;
    int coord_x,coord_y;
    jogo->conta_barril=jogo->conta_barril+1;   // mais um barril no jogo
    indice=jogo->conta_barril-1;
    coord_x=jogo->coluna_donk;
    coord_y=jogo->linha_donk-2;
    jogo->espelho_fixo=apaga_espelho(jogo->espelho_fixo,coord_x,coord_y);  // apaga o barril do kong no espelho
    apaga(jogo->coluna_donk,jogo->linha_donk);                             // apaga o kong para baixar os bracos
    apaga(coord_x,coord_y);                                                // apaga o barril do kong na tela
    coord_x=jogo->coluna_donk+3;                                           // barril inicia ao lado do kong
    coord_y=jogo->linha_donk;
    imprime_donk1(jogo->coluna_donk,jogo->linha_donk);                     // reimprime o kong
    imprime_barril_m(coord_x,coord_y);                                     // imprime o barril
    jogo->espelho_fixo=escreve_espelho(jogo->espelho_fixo,coord_x,coord_y,'B');   // registra o barril no espelho
    jogo->barril_movel[indice].coluna_inicial=coord_x;
    jogo->barril_movel[indice].linha_inicial=coord_y;
    jogo->barril_movel[indice].velocidade=PDIR;         // sai pra direita
    jogo->barril_movel[indice].tipo='B';

}

int move_barril(JOGO *jogo,int indice_barril) // Move para os lados
{
    int coord_x,coord_y; // Auxiliares;
    int aux_x,aux_y; // Auxiliares das auxiliares
    int eh_valido=FALSE;
    int bDescerEscada=FALSE;
    char apagado='z';
    coord_x=jogo->barril_movel[indice_barril].coluna_inicial; // Inicizalizando auxiliares
    coord_y=jogo->barril_movel[indice_barril].linha_inicial;
    aux_x=coord_x;
    aux_y=coord_y;
    if (coord_x!=jogo->coluna_donk || (jogo->linha_donk-2)!=coord_y) // Se o barril não estiver em cima do Donkey Kong
    {
        if ((int)(jogo->barril_movel[indice_barril].velocidade)==PDIR) // Se está indo para a direita
        {
            bDescerEscada=deixa_descer_escada_b(jogo->espelho_fixo, coord_x, coord_y); // Chama a função que testa se o barril pode descer a escada
            if(bDescerEscada==TRUE) // Se deixar descer
            {
                jogo->barril_movel[indice_barril].velocidade=PBAI; // Muda a velocidade do barril
                return JOGANDO ; // Retorna JOGANDO
            }
            jogo->espelho_fixo=testa_direita_barril(jogo->espelho_fixo,&coord_x,&coord_y,&eh_valido,&apagado); // Testa se o barril pode ir para a direita
            if (eh_valido)
            { // Espelho já é apagado se válido, e coordenadas já são alteradas
                apaga(aux_x,aux_y);
                jogo->barril_movel[indice_barril].coluna_inicial = coord_x; // Alterando coordenadas no struct que guarda tudo
                jogo->barril_movel[indice_barril].linha_inicial  = coord_y;
                jogo->espelho_fixo=restaura_escada(PDIR,coord_x,coord_y,jogo->espelho_fixo); // Restaurando as escadas
                jogo->espelho_fixo=escreve_espelho(jogo->espelho_fixo,coord_x,coord_y,'B'); // Colocando o barril de volta no mapa de analise e na tela de jogo
                imprime_barril_m(coord_x,coord_y);
                if ((coord_x+2)==jogo->mario.coluna_inicial && coord_y==jogo->mario.linha_inicial) // Se o barril acertar o Mário num choque horizontal 100%
                     return DERROTA;
                else if ((coord_x+2)==jogo->mario.coluna_inicial && (coord_y+1)==jogo->mario.linha_inicial) // Acertei Mário num choque horizontal 50%
                     return DERROTA;
                if (coord_y==44) // Se atingir o "fim do mundo"
                {
                    apaga(aux_x,aux_y);
                    jogo->espelho_fixo=apaga_espelho(jogo->espelho_fixo,coord_x,coord_y); // Apaga o barril no espelho
                    jogo->barril_movel[indice_barril].coluna_inicial = jogo->coluna_donk+3; // Altera as coordenadas no struct que guarda tudo
                    jogo->barril_movel[indice_barril].linha_inicial  = jogo->linha_donk;
                }
            }
            else // Se encontrar alguma outra coisa, mudo de direção
                jogo->barril_movel[indice_barril].velocidade=PESQ;
        }
        else if ((int)(jogo->barril_movel[indice_barril].velocidade)==PESQ)// O mesmo se repete para a esquerda
        {
            if (bDescerEscada)
            {
                 jogo->barril_movel[indice_barril].velocidade=PBAI; // Mudo a direção para baixo e saio daqui
                 return JOGANDO;
            }
            jogo->espelho_fixo=testa_esquerda_barril(jogo->espelho_fixo,&coord_x,&coord_y,&eh_valido,&apagado);
            if (eh_valido)
            { // Espelho já é apagado se válido, e coordenadas já são alteradas
                apaga(aux_x,aux_y);
                jogo->barril_movel[indice_barril].coluna_inicial = coord_x; // Alterando coordenadas no struct que  guarda tudo
                jogo->barril_movel[indice_barril].linha_inicial  = coord_y;
                jogo->espelho_fixo=restaura_escada(PESQ,coord_x,coord_y,jogo->espelho_fixo); // Restaurando as escadas
                jogo->espelho_fixo=escreve_espelho(jogo->espelho_fixo,coord_x,coord_y,'B'); // Colocando o barril de volta no mapa de analise e na tela de jogo
                imprime_barril_m(coord_x,coord_y);
                if ((coord_x-1)==jogo->mario.coluna_inicial && coord_y==jogo->mario.linha_inicial) // Acertaria o mario num choque horizontal 100%
                     return DERROTA;
                else if ((coord_x-1)==jogo->mario.coluna_inicial && (coord_y+1)==jogo->mario.linha_inicial) // Acertaria mario num choque horizontal 50%
                     return DERROTA;
                 if (coord_y==44) // Atingi o "fim do mundo "
                {
                    apaga(aux_x,aux_y);
                    jogo->espelho_fixo=apaga_espelho(jogo->espelho_fixo,coord_x,coord_y);
                    jogo->barril_movel[indice_barril].coluna_inicial = jogo->coluna_donk+3; // Alterando coordenadas no struct que  guarda tudo
                    jogo->barril_movel[indice_barril].linha_inicial  = jogo->linha_donk;
                }
            }
            else
            {// Encontrei alguma outra coisa, mudo de direção
                jogo->barril_movel[indice_barril].velocidade=PDIR;
            }

        }
    }
    return JOGANDO;
}

CONTROLE testa_direita_barril(CONTROLE posicao, int *x, int *y, int *flag,char *restaura)  // Testa se o barril pode ir para a direita
{
    int linha,coluna;
    coluna=*x;
    linha=*y;
    int bPodeMover=FALSE;
    gotoxy(12,12);
    if (posicao.controle[linha+2][coluna] == 'H'
            || posicao.controle[linha+2][coluna] == 'S'
            || posicao.controle[linha+2][coluna] == ' '
            || posicao.controle[linha+2][coluna] == 'G'    // rampa dir

       )
    {
        if(posicao.controle[linha][coluna+X_PDR] == ' '
                &&posicao.controle[linha+1][coluna+X_PDR] == ' ')
        {
            bPodeMover=TRUE;

        }
        else
        {
            if (posicao.controle[linha][coluna+X_PDR] == 'E'
               || posicao.controle[linha][coluna+X_PDR] == 'E')
            {
                *restaura='E';
                bPodeMover=TRUE;
            }
            else if (posicao.controle[linha][coluna+X_PDR] == 'S')
                bPodeMover=FALSE;
                else if (posicao.controle[linha+3][coluna+1] == 'G')   // rampa a esquerda desço
            {
                linha++;  //acho que eh mais
                bPodeMover=TRUE;
            }
        }
    }

    if (bPodeMover)
    {
        posicao=apaga_espelho(posicao,*x,*y);
        coluna++;
        *x=coluna;
        *y=linha;
        *flag=1;
    }
    return posicao;

}

CONTROLE testa_esquerda_barril(CONTROLE posicao, int *x, int *y,int *flag,char *restaura) // Testa se o barril pode ir para a esquerda
{
    int linha,coluna;
    coluna=*x;
    linha=*y;
    int bPodeMover=FALSE;
    gotoxy(12,12);
    if (posicao.controle[linha+2][coluna] == 'H'
            || posicao.controle[linha+2][coluna] == 'S'
            || posicao.controle[linha+2][coluna] == ' '
            || posicao.controle[linha+2][coluna] == 'F'   // rampa dir
           )
    {
        if(posicao.controle[linha][coluna-1] == ' '
                &&posicao.controle[linha+1][coluna-1] == ' ')
        {
            bPodeMover=TRUE;

        }
        else
        {
            if (posicao.controle[linha][coluna-1] == 'E'
                || posicao.controle[linha+1][coluna-1] == 'E')
            {
                *restaura='E';
                bPodeMover=TRUE;
            }
            else if (posicao.controle[linha][coluna-1] == 'S')
                bPodeMover=0;
            else if (posicao.controle[linha+3][coluna-1] == 'F')      // há uma rampa a direita, desça
            {
                linha++;              // desce, aumentando a linha
                bPodeMover=TRUE;      // pode mover
            }
        }

    }
    if (bPodeMover)
    {
        posicao=apaga_espelho(posicao,*x,*y);
        coluna--;
        *x=coluna;
        *y=linha;
        *flag=1;
    }
    return posicao;
}

int deixa_descer_escada_b(CONTROLE posicao, int x, int y) // Testa se o barril pode descer a escada
{
    int linha,coluna; // Linhas e colunas auxiliares
    coluna=x; // Inicialização
    linha=y;

    int seed=time(NULL); // Inicialização da seed em toda a execução
    int max=3;
    int min=1;
    srand(seed); // Randomizando a seed
    int descer;
    int deixadescer=FALSE;
    if (posicao.controle[linha+Y_PDR][coluna] == 'H'
     && posicao.controle[linha+2][coluna+1] == 'H'
     && posicao.controle[linha+2][coluna+2] == 'H')
    {
        descer=rand() % (max + 1 - min) + min; // Um número aleatório entre min e max
        if (descer!=1) // Se o número for diferente de 1
            deixadescer=TRUE;
    }
    return deixadescer;
}

int cai_barril(JOGO *jogo,int indice_barril) // Faz os barris cairem
{
    int coord_x=jogo->barril_movel[indice_barril].coluna_inicial; // Coordenadas auxiliares
    int coord_y=jogo->barril_movel[indice_barril].linha_inicial;
    int aux_x=coord_x;
    int aux_y=coord_y;
    int eh_valido;
    int randomico;
    char apagado;
    if((int)(jogo->barril_movel[indice_barril].velocidade)==PBAI) // Se a velocidade for para baixo
    {
        if    (     jogo->espelho_fixo.controle[coord_y+2][coord_x  ] != 'S' // Se não for superfície ou rampa
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+1] != 'S' // Abaixo do barril
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+2] != 'S'
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x  ] != 'G'
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+1] != 'G'
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+2] != 'G'
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x  ] != 'F'
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+1] != 'F'
                &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+2] != 'F')
        {
            jogo->espelho_fixo=desce_barril(jogo->espelho_fixo,&coord_x,&coord_y,&eh_valido,&apagado); // Chama a função desce barril
            if (eh_valido)
            { // Espelho já é apagado se válido, e coordenadas já são alteradas
                apaga(aux_x,aux_y);
                jogo->barril_movel[indice_barril].coluna_inicial = coord_x; // Alterando coordenadas no struct que guarda tudo
                jogo->barril_movel[indice_barril].linha_inicial  = coord_y;
                jogo->espelho_fixo=restaura_escada(PBAI,coord_x,coord_y,jogo->espelho_fixo);// Restaura as escadas
                jogo->espelho_fixo=escreve_espelho(jogo->espelho_fixo,coord_x,coord_y,'B'); // Coloca o barril de volta no mapa de analise e na tela de jogo
                imprime_barril_m(coord_x,coord_y);
            }
        }
        else
        {// Se encontrar alguma outra coisa, muda de direção
            randomico=rand();
            if(randomico%2==0) // 50% de chance de ir para cada lado
                jogo->barril_movel[indice_barril].velocidade=PESQ; // Randomico pra direção esquerda
            else
                jogo->barril_movel[indice_barril].velocidade=PDIR; // Randomico pra direção direita
        }
    }
    else
    {
        if (    jogo->espelho_fixo.controle[coord_y+2][coord_x  ] == ' ' // Se tiver espaço em baixo
            &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+1] == ' '
            &&  jogo->espelho_fixo.controle[coord_y+2][coord_x+2] == ' ')
        {
            jogo->espelho_fixo=desce_barril(jogo->espelho_fixo,&coord_x,&coord_y,&eh_valido,&apagado); // Desce o barril
            if (eh_valido) // Se é válido
            {
                apaga(aux_x,aux_y);
                jogo->barril_movel[indice_barril].coluna_inicial = coord_x; // Altera coordenadas no struct que guarda tudo
                jogo->barril_movel[indice_barril].linha_inicial  = coord_y;
                jogo->espelho_fixo=restaura_escada(PBAI,coord_x,coord_y,jogo->espelho_fixo);// Restaura as escadas
                jogo->espelho_fixo=escreve_espelho(jogo->espelho_fixo,coord_x,coord_y,'B'); // Colocando o barril de volta no mapa de analise e na tela de jogo
                imprime_barril_m(coord_x,coord_y);
            }
            if (coord_y==44) // Se chegar ao "fim do mundo"
            {
                apaga(aux_x,aux_y+1);
                jogo->espelho_fixo=apaga_espelho(jogo->espelho_fixo,coord_x,coord_y);  // Apaga o barril do espelho
                jogo->barril_movel[indice_barril].coluna_inicial = jogo->coluna_donk+3;// Alterando coordenadas no struct que guarda tudo
                jogo->barril_movel[indice_barril].linha_inicial  = jogo->linha_donk;   // O barril é "resetado" para a origem
                jogo->barril_movel[indice_barril].velocidade=PDIR;
            }
        }
    }
}

CONTROLE desce_barril(CONTROLE posicao, int *x, int *y, int *flag,char *restaura) // Desce o barril
{
    int linha,coluna;
    coluna=*x;
    linha=*y;

        posicao=apaga_espelho(posicao,coluna,linha);
        linha++;
        *restaura=posicao.controle[linha][coluna];
        *x=coluna;
        *y=linha;
        *flag=TRUE;
    return posicao;
}

int habilita_movimento(int flag_teste, clock_t *relogio1 ,char atividade) // Avalia os flags
{
    int  iTempoAtividade, tempodecorrido;
    clock_t auxiliar =*relogio1; // Ponteiro pois é o tempo inicial
    clock_t relogio2;
    if (flag_teste!=TRUE)        // Avaliar tempo se flag não foi verdadeiro
    {
        relogio2=clock(); // Iguala relogio2 ao tempo atual
        tempodecorrido=tempo(auxiliar,relogio2); // Chama a função tempo
        switch(atividade) // Testa a atividade que varia de cada flag
        {
        case 'Z' :
            iTempoAtividade=GRAVIDADE; // Compara o tempo com o respectivo define
            break;
        case 'V' :
            iTempoAtividade=VEL_GERA_BARRIL;
            break;
        case 'B' :
            iTempoAtividade=VEL_BARRIL;
            break;
        case 'M' :
            iTempoAtividade=VEL_MARIO;
            break;
        case 'Ç' :
            iTempoAtividade=ATUALIZA_SCORE;
            break;
        }
        if (tempodecorrido>=iTempoAtividade)   // Se esgotou o tempo de espera dessa atividade, habilitar
        {
            flag_teste=TRUE;     // Avisar que habilitou o teste passado
            *relogio1=relogio2;  // Guardar a ultima hora em que testou

        }
    }
    return flag_teste;
}


//Funções ferramentas
void gera_tipo_fase(CONTROLE para_analise,int *qtdobjetos,JOGO *loadgame)
{
    TIPO_FASE vetor[MAX_OBJ];
    int contador=0;
    int in,jn;
    for(in=0; in<LINHAS_MAX; in++)
    {
        for(jn=0; jn<(COLUNAS_MAX-1); jn++) // -1 pois há o \n
        {
            if (para_analise.save[in][jn]!= ' ')
            {
                vetor[contador].tipo=para_analise.save[in][jn];
                vetor[contador].coluna_inicial=jn;
                vetor[contador].linha_inicial=in;
                loadgame->vetor_objetos[contador]=vetor[contador];
                contador++;
            }

        }
    }
    *qtdobjetos=contador;
}

int ler_bin(TIPO_FASE arrayobj[]) // Lê binário
{
    TIPO_FASE buffer; // Variável auxiliar
    FILE *fase; // Ponteiro arquivo
    int contador=0; // Contador de objetos

    fase=fopen("fase.bin","rb");
    if (!fase)
    {
        printf("Erro ao carregar a fase, veja se o arquivo fase.bin existe");
    }
    else
    {
        while(!feof(fase)) // Enquanto o arquivo não terminar
        {
            fread(&buffer,sizeof(TIPO_FASE),1,fase); // Lê e grava na variável buffer
            arrayobj[contador]=buffer;  // Iguala uma posição do vetor a buffer
            contador++; // Incrementa o contador
        }
    }
    fclose(fase);
    return contador; // Retorna o contador, que será o número de objetos
}

void formatprompt()
{
    // Formatação do prompt
    setlocale(LC_ALL, "Portuguese"); // Língua para português  //prompt em português para acentos e afins
    system("title Donkey Kong"); // Muda nome do prompt
    system("color 0C");                             // Cor normal do prompt
    system("mode 60, 48");                           // Modo para não exceder o buffer
    SMALL_RECT WinRect = {0, 0, X_MAX, Y_MAX};       // Formata o tamanho do prompt
    SMALL_RECT* WinSize = &WinRect;
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, WinSize);
    hide_cursor();
}

void hide_cursor() // Esconde o cursor default do prompt
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // Esconde o cursor do console
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy (int x, int y) // Move o cursor para (x,y)
{
    COORD coord = {0, 0};
    coord.X =x; // Coordenadas X e Y
    coord.Y = y+2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void text_color(int ForgC) // Função para colorir o texto
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void cor_escadas(int a) // Colore a escada
{
    if (a==1)text_color(GRAY);
    else
    {
        text_color(RED);
    }
}

int setas()  // Controle pelas setas direcionais
{
    int valor_seta; // Pode ser os valores definidos no cabeçalho

    do
        valor_seta = getch(); // Continua pegando caracteres
    while   (   valor_seta != CIMA
             && valor_seta != BAIXO
             && valor_seta != ENTER
             && valor_seta != ESC
             && valor_seta != SPACEBAR); // Espaço, ESC e ENTER incluidos

    return valor_seta; // Retorna a seta lida
}

int tempo(clock_t c0, clock_t c1) // Retorna a diferença de tempo entre c1 e c0
{
    double runtime_diff_ms;
    runtime_diff_ms= ((c1 - c0)*100)/ (CLOCKS_PER_SEC); // Valores para chegar a um número ótimo
    int retorno=(int)runtime_diff_ms;
    return retorno;
}
