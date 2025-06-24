#include <iostream>
using namespace std;

#define MAX 100

// Estrutura para fila
struct Fila {
    int dados[MAX];
    int frente;
    int tras;
};

// Inicializa fila
void inicializarFila(Fila* f) {
    f->frente = 0;
    f->tras = -1;
}

// Verifica se fila est� vazia
bool filaVazia(Fila* f) {
    return f->frente > f->tras;
}

// Verifica se fila est� cheia
bool filaCheia(Fila* f) {
    return f->tras == MAX - 1;
}

// Enfileirar
bool enfileirar(Fila* f, int valor) {
    if (filaCheia(f)) return false;
    f->tras++;
    f->dados[f->tras] = valor;
    return true;
}

// Desenfileirar
bool desenfileirar(Fila* f, int* valor) {
    if (filaVazia(f)) return false;
    *valor = f->dados[f->frente];
    f->frente++;
    return true;
}

// Tamanho da fila
int tamanhoFila(Fila* f) {
    return f->tras - f->frente + 1;
}

// -------------------------
// Lista de guich�s
struct Guiche {
    int id;
    Fila senhasAtendidas;
    Guiche* prox;
};

// Adiciona novo guich� � lista
void adicionarGuiche(Guiche** lista, int id) {
    Guiche* novo = new Guiche;
    novo->id = id;
    inicializarFila(&novo->senhasAtendidas);
    novo->prox = *lista;
    *lista = novo;
}

// Busca guich� por id
Guiche* buscarGuiche(Guiche* lista, int id) {
    while (lista != NULL) {
        if (lista->id == id)
            return lista;
        lista = lista->prox;
    }
    return NULL;
}

// Conta guich�s abertos
int contarGuiches(Guiche* lista) {
    int cont = 0;
    while (lista != NULL) {
        cont++;
        lista = lista->prox;
    }
    return cont;
}

// Lista senhas atendidas por um guich�
void listarSenhasGuiche(Guiche* guiche) {
    if (guiche == NULL || filaVazia(&guiche->senhasAtendidas)) {
        cout << "Nenhuma senha atendida por este guich�.\n";
        return;
    }
    cout << "Senhas atendidas pelo guich� " << guiche->id << ": ";
    for (int i = guiche->senhasAtendidas.frente; i <= guiche->senhasAtendidas.tras; i++) {
        cout << guiche->senhasAtendidas.dados[i] << " ";
    }
    cout << endl;
}

// -------------------------
int main() {
    Fila* senhasGeradas = new Fila;
    inicializarFila(senhasGeradas);
    Guiche* listaGuiches = NULL;

    int senhaAtual = 0;
    int opcao;
    int senhaAtendida;
    int idGuiche;
    int totalAtendidas = 0;

    do {
        cout << "\n------ SISTEMA DE ATENDIMENTO 2.0 ------\n";
        cout << "0 - Sair\n";
        cout << "1 - Gerar senha\n";
        cout << "2 - Abrir guich�\n";
        cout << "3 - Realizar atendimento\n";
        cout << "4 - Listar senhas atendidas por guich�\n";
        cout << "Senhas aguardando: " << tamanhoFila(senhasGeradas) << " | Guich�s abertos: " << contarGuiches(listaGuiches) << "\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                senhaAtual++;
                if (enfileirar(senhasGeradas, senhaAtual))
                    cout << "Senha " << senhaAtual << " gerada com sucesso.\n";
                else
                    cout << "Fila cheia. N�o foi poss�vel gerar mais senhas.\n";
                break;

            case 2:
                cout << "Digite o ID do novo guich�: ";
                cin >> idGuiche;
                if (buscarGuiche(listaGuiches, idGuiche)) {
                    cout << "Guich� com esse ID j� existe.\n";
                } else {
                    adicionarGuiche(&listaGuiches, idGuiche);
                    cout << "Guich� " << idGuiche << " aberto com sucesso.\n";
                }
                break;

            case 3: {
                if (filaVazia(senhasGeradas)) {
                    cout << "Nenhuma senha aguardando atendimento.\n";
                    break;
                }
                cout << "Digite o ID do guich�: ";
                cin >> idGuiche;
                Guiche* g = buscarGuiche(listaGuiches, idGuiche);
                if (g == NULL) {
                    cout << "Guich� n�o encontrado.\n";
                    break;
                }
                if (desenfileirar(senhasGeradas, &senhaAtendida)) {
                    enfileirar(&g->senhasAtendidas, senhaAtendida);
                    totalAtendidas++;
                    cout << "Guich� " << idGuiche << " atendeu a senha " << senhaAtendida << endl;
                }
                break;
            }

            case 4: {
                cout << "Digite o ID do guich�: ";
                cin >> idGuiche;
                Guiche* g = buscarGuiche(listaGuiches, idGuiche);
                listarSenhasGuiche(g);
                break;
            }

            case 0:
                if (!filaVazia(senhasGeradas)) {
                    cout << "Ainda h� senhas para atender. N�o � poss�vel sair agora.\n";
                    opcao = -1;
                } else {
                    cout << "\nEncerrando o sistema...\n";
                    cout << "Total de senhas atendidas: " << totalAtendidas << endl;
                }
                break;

            default:
                cout << "Opcao invalida!\n";
        }

    } while (opcao != 0);

    // Libera��o de mem�ria
    delete senhasGeradas;
    while (listaGuiches != NULL) {
        Guiche* temp = listaGuiches;
        listaGuiches = listaGuiches->prox;
        delete temp;
    }

    return 0;
}
