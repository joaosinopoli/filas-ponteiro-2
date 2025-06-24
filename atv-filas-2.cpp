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

// Verifica se fila está vazia
bool filaVazia(Fila* f) {
    return f->frente > f->tras;
}

// Verifica se fila está cheia
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
// Lista de guichês
struct Guiche {
    int id;
    Fila senhasAtendidas;
    Guiche* prox;
};

// Adiciona novo guichê à lista
void adicionarGuiche(Guiche** lista, int id) {
    Guiche* novo = new Guiche;
    novo->id = id;
    inicializarFila(&novo->senhasAtendidas);
    novo->prox = *lista;
    *lista = novo;
}

// Busca guichê por id
Guiche* buscarGuiche(Guiche* lista, int id) {
    while (lista != NULL) {
        if (lista->id == id)
            return lista;
        lista = lista->prox;
    }
    return NULL;
}

// Conta guichês abertos
int contarGuiches(Guiche* lista) {
    int cont = 0;
    while (lista != NULL) {
        cont++;
        lista = lista->prox;
    }
    return cont;
}

// Lista senhas atendidas por um guichê
void listarSenhasGuiche(Guiche* guiche) {
    if (guiche == NULL || filaVazia(&guiche->senhasAtendidas)) {
        cout << "Nenhuma senha atendida por este guichê.\n";
        return;
    }
    cout << "Senhas atendidas pelo guichê " << guiche->id << ": ";
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
        cout << "2 - Abrir guichê\n";
        cout << "3 - Realizar atendimento\n";
        cout << "4 - Listar senhas atendidas por guichê\n";
        cout << "Senhas aguardando: " << tamanhoFila(senhasGeradas) << " | Guichês abertos: " << contarGuiches(listaGuiches) << "\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                senhaAtual++;
                if (enfileirar(senhasGeradas, senhaAtual))
                    cout << "Senha " << senhaAtual << " gerada com sucesso.\n";
                else
                    cout << "Fila cheia. Não foi possível gerar mais senhas.\n";
                break;

            case 2:
                cout << "Digite o ID do novo guichê: ";
                cin >> idGuiche;
                if (buscarGuiche(listaGuiches, idGuiche)) {
                    cout << "Guichê com esse ID já existe.\n";
                } else {
                    adicionarGuiche(&listaGuiches, idGuiche);
                    cout << "Guichê " << idGuiche << " aberto com sucesso.\n";
                }
                break;

            case 3: {
                if (filaVazia(senhasGeradas)) {
                    cout << "Nenhuma senha aguardando atendimento.\n";
                    break;
                }
                cout << "Digite o ID do guichê: ";
                cin >> idGuiche;
                Guiche* g = buscarGuiche(listaGuiches, idGuiche);
                if (g == NULL) {
                    cout << "Guichê não encontrado.\n";
                    break;
                }
                if (desenfileirar(senhasGeradas, &senhaAtendida)) {
                    enfileirar(&g->senhasAtendidas, senhaAtendida);
                    totalAtendidas++;
                    cout << "Guichê " << idGuiche << " atendeu a senha " << senhaAtendida << endl;
                }
                break;
            }

            case 4: {
                cout << "Digite o ID do guichê: ";
                cin >> idGuiche;
                Guiche* g = buscarGuiche(listaGuiches, idGuiche);
                listarSenhasGuiche(g);
                break;
            }

            case 0:
                if (!filaVazia(senhasGeradas)) {
                    cout << "Ainda há senhas para atender. Não é possível sair agora.\n";
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

    // Liberação de memória
    delete senhasGeradas;
    while (listaGuiches != NULL) {
        Guiche* temp = listaGuiches;
        listaGuiches = listaGuiches->prox;
        delete temp;
    }

    return 0;
}
