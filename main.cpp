#include <iostream>
#include <random>
#include <time.h>
#include <cstdlib>

/// 1 2 3
/// 4 5 6
/// 7 8 9
///---------------------
/// 0: disponivel
/// 1: xizinho (Usuario)
/// 2: bolinha (Maquina)

using namespace std;

class Game
{
    private:
    int table[9]; /// [1,2,3]/[4,5,6]/[7,8,9]
    bool isFirstPlayUser;
    public:
    int firstPlayUser;
    Game()
    {
        for(int i=0; i<9; i++)
            table[i]=0;
        firstPlayUser=-1;
        isFirstPlayUser=true;
    }

    void print()
    {
        cout<<"\n\n\n\t\t";

        for(int i=0; i<3; i++)
        {
            cout<<"[";
            if(table[i]==0)
                cout<<"   ";
            else if(table[i]==1)
                cout<<" X ";
            else
                cout<<" O ";
            cout<<"]";
        }

        cout<<"\n\n\t\t";

        for(int i=3; i<6; i++)
        {
            cout<<"[";
            if(table[i]==0)
                cout<<"   ";
            else if(table[i]==1)
                cout<<" X ";
            else
                cout<<" O ";
            cout<<"]";
        }

        cout<<"\n\n\t\t";

        for(int i=6; i<9; i++)
        {
            cout<<"[";
            if(table[i]==0)
                cout<<"   ";
            else if(table[i]==1)
                cout<<" X ";
            else
                cout<<" O ";
            cout<<"]";
        }

        cout<<"\n\n\n\n";
    };

    bool markTable(int symbol, int position)
    {
        if(isEmpty(position))
        {
            table[position] = symbol;
            if(symbol==1 && isFirstPlayUser) ///salva a ultima jogado do usuario
            {
                isFirstPlayUser=false;
                firstPlayUser=position;
            }
            return true;
        }
        else
        {
            cout << "Ops! posicao ja marcada!";
            return false;
        }
    }

    bool isEmpty(int position)
    {
        if(table[position]==0)
            return true;
        return false;
    }

    int check2of3(int symbol) ///retorna a teceira posição para completar uma trinca (linha), se tiver disponivel
    {
        int lines[]= {0,1,2,3,4,5,6,7,8,0,3,6,1,4,7,2,5,8,0,4,8,2,4,6};
        int counter=0;
        int terceiro=-1;

        for(int i=0; i<24; i=i+3)
        {
            counter=0;
            terceiro=-1;
            for(int j=0; j<3; j++)
            {
                if(table[lines[i+j]]==symbol)
                {
                    counter++;
                }
                else if(table[lines[i+j]]==0) ///disponivel
                {
                    terceiro=lines[i+j];
                }
            }
            if(counter==2)
            {
                if(isEmpty(terceiro))
                    return terceiro;
            }
        }
        return -1;
    }

    bool isWinner(int symbol)
    {
        int lines[]= {0,1,2,3,4,5,6,7,8,0,3,6,1,4,7,2,5,8,0,4,8,2,4,6};

        for(int i=0; i<24; i=i+3)
        {
            if(symbol==table[lines[i]] && symbol==table[lines[i+1]] && symbol==table[lines[i+2]])
                return true;
        }
        return false;
    }

    bool isVelha()
    {
        for(int i=0; i<9; i++)
        {
            if(table[i]==0)
                return false;
        }
        return true;
    }

    bool isFirstPlay()
    {
        for(int i=0; i<9; i++)
        {
            if(table[i]!=0)
                return false;
        }
        return true;
    }
};

#ifdef _WIN32
    #include <windows.h>

    void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>

    void sleep(int milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif

int RandNumber(int num_mim, int num_max)
{
    srand(time(NULL));
    int numrand=(rand()%num_max+num_mim);///gera um numero entre min e max
    return numrand;
}

bool isCanto(int firstPosition)
{
    int cantos[] = {0,6,2,8};
    for(int i=0; i<4; i++)
    {
        if(firstPosition == cantos[i])
            return true;
    }
    return false;
}

bool isLado(int firstPosition)
{
    int lados[] = {7,1,3,5};
    if(firstPosition==4) ///meio
        return true;
    for(int i=0; i<4; i++)
    {
        if(firstPosition == lados[i])
            return true;
    }
    return false;
}

int SearchRandomEmptyField(Game &game)
{
    for(int i=0; i<9; i++)
    {
        if(game.isEmpty(i))
            return i;
    }
    return -1;
}

int DecideJogada(Game &game, int first)
{
    int symbol=2;
    bool isFirst = game.isFirstPlay();
    int cantos[] = {0,6,2,8};
    int lados[] = {7,1,3,5};
    int meio = 4;
    float pesos[] = {0.5, 0.4, 0.1, 0.4, 0.4, 0.2, 0.4, 0.6, 0.0};
    int rand=0;

    if(first==symbol) ///estratégia caso 1º jogador
    {
        if(isFirst)
        {
            rand=RandNumber(0,3);
            int indice = cantos[rand];
            if(game.isEmpty(indice))
            {
                return indice;
            }
        }
        else
        {
            for(int i=0; i<4; i++) ///jogada canto
            {
                if(game.isEmpty(cantos[i]))
                {
                    return cantos[i];
                }
            }

            for(int i=0; i<4; i++) ///jogada lado
            {
                if(game.isEmpty(lados[i]))
                {
                    return lados[i];
                }
            }

            if(game.isEmpty(meio)) ///jogada meio
            {
                return meio;
            }
        }
    }
    else ///estratégia caso 2º jogador
    {
        if(isCanto(game.firstPlayUser)) ///Se Oponente jogar canto
        {
            if(game.isEmpty(meio)) ///jogada meio
            {
                return meio;
            }

            for(int i=0; i<4; i++) ///jogada lado
            {
                if(game.isEmpty(lados[i]))
                {
                    return lados[i];
                }
            }

            for(int i=0; i<4; i++) ///jogada canto
            {
                if(game.isEmpty(cantos[i]))
                {
                    return cantos[i];
                }
            }
        }
        else ///Se Oponente jogar meio ou lado
        {
            for(int i=0; i<4; i++) ///jogada canto
            {
                if(game.isEmpty(cantos[i]))
                {
                    return cantos[i];
                }
            }

            for(int i=0; i<4; i++) ///jogada lado
            {
                if(game.isEmpty(lados[i]))
                {
                    return lados[i];
                }
            }

            if(game.isEmpty(meio)) ///jogada meio
            {
                return meio;
            }
        }
    }
    return -1;
}

bool JogadaPC(Game &game, int first)
{
    int mySymbol=2; ///simbolo padrao da maquina
    int oponenteSymbol=1;
    bool isMarked=false;
    int position=0;

    /// Verifica se a maquina pode ganhar na jogada atual
    position = game.check2of3(mySymbol);
    if(position >= 0)
    {
        isMarked = game.markTable(mySymbol,position);
        if(isMarked)
        {
            system("clear");
            cout << "===== JOGO DA VELHA ===== \n" << endl;
            game.print();
            cout << endl << "    Jogada da maquina - posicao: " << position+1 << endl;
            return true;
        }
    }

    /// Verifica se o oponente pode ganhar na proxima jogada, e tenta impedilo na jogada atual
    position = game.check2of3(oponenteSymbol);
    if(position >= 0)
    {
        isMarked = game.markTable(mySymbol,position);
        if(isMarked)
        {
            system("clear");
            cout << "===== JOGO DA VELHA ===== \n" << endl;
            game.print();
            cout << endl << "    Jogada da maquina - posicao: " << position+1 << endl;
            return true;
        }
    }


    /// aplica logica da jogada (Matriz de Transição)
    position = DecideJogada(game,first);
    if(position >= 0)
    {
        isMarked = game.markTable(mySymbol,position);
        if(isMarked)
        {
            system("clear");
            cout << "===== JOGO DA VELHA ===== \n" << endl;
            game.print();
            cout << endl << "    Jogada da maquina - posicao: " << position+1 << endl;
            return true;
        }
    }


    /// aplica logica da jogada (jogada aleatoria)
    /*
    position = SearchRandomEmptyField(game);
    if(position >= 0)
    {
        isMarked = game.markTable(mySymbol,position);
        if(isMarked)
        {
            system("clear");
            cout << "===== JOGO DA VELHA ===== \n" << endl;
            game.print();
            cout << endl << "    Jogada da maquina - posicao: " << position+1 << endl;
            return true;
        }
    }
    */

    return false;
}

int RandPlayerStart()
{
    srand(time(NULL));
    int numrand=(rand()%2+1);///gera um numero entre 1 e 2
    return numrand;
}

void JogadaUser(Game &game)
{
    int symbol=1; ///simbolo padrao do usuário
    string pos;
    bool isMarked=false;

    while(!isMarked)
    {
        cout << "\n";
        cout << "Escolha a posicao para marcar? " << endl;
        cin >> pos;
        int position = atoi(pos.c_str());
        if(position>=1 && position<=9)
        {
            position -= 1;
            isMarked = game.markTable(symbol,position);
        }
        else
            cout << "ALERTA: Escolha a posicao na faixa de 1 ate 9." << endl;
    }
}

void PlayGame(int turnStart)
{
    Game game;
    bool isEnd=false;
    int turn=turnStart;
    char pause;
    int first=turnStart;

    while(!isEnd)
    {
        system("clear");
        cout << "===== JOGO DA VELHA ===== \n" << endl;
        game.print();

        if(turn==1) ///turn do usuario
        {
            turn=2;
            cout << "SUA VEZ!  Simbolo 'X' Xizinho - Escolha uma posicao de 1 a 9: " << endl;
            JogadaUser(game);
        }
        else ///turn da maquina
        {
            turn=1;
            cout << "TURNO DA MAQUINA! Simbolo 'O' Bolinha: " << endl;
            sleep(2000);
            JogadaPC(game,first);
            sleep(2000);
            cout << "\n";
            cout << "Digite um CHARACTER qualquer e pressione ENTER para continuar..." << endl;
            cin >> pause;
            ///continue;
        }

        if(game.isWinner(1)) ///usuario
        {
            isEnd=true;
            cout << "  ==== VOCE VENCEU ====" << endl;
        }
        else if(game.isWinner(2)) ///maquina
        {
            isEnd=true;
            cout << "  ==== A MAQUINA VENCEU ====" << endl;
        }
        else if(game.isVelha())
        {
            isEnd=true;
            cout << "  ==== DEU VELHA ====" << endl;
        }
    }
}

void StartGame()
{
    char continuar = 'y';

    while(continuar=='y' || continuar=='s')
    {
        system("clear");
        PlayGame(RandPlayerStart());

        cout << "Deseja jogar novamente?  (y/n) ?" << endl;
        cin >> continuar;
    }
    cout << "Bye!!!" << endl;
}

int main()
{
    StartGame();
    return 0;
}
