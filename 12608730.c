/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2021                           **/
/**   <turma> - Prof. Luciano Antonio Digiampietri                  **/
/**                                                                 **/
/**   EP 1 - Lista Ligada de Produtos                               **/
/**                                                                 **/
/**   Nome: Amanda Gimenes Perellon        numero USP : 12608730    **/
/**                                                                 **/
/*********************************************************************/
#include "listadeprodutos.h"

PLISTA criarLista(){
  PLISTA res = (PLISTA) malloc(sizeof(LISTADEPRODUTOS));
  int x;
  for (x=0;x<NUMTIPOS;x++){
    res->LISTADELISTAS[x]=(PONT) malloc(sizeof(REGISTRO));
    res->LISTADELISTAS[x]->id=-1;
    res->LISTADELISTAS[x]->quantidade=0;
    res->LISTADELISTAS[x]->valorUnitario=0;
    res->LISTADELISTAS[x]->proxProd=NULL;
  }
  return res;
}


int tamanho(PLISTA l){
  int tam = 0;
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      atual = atual->proxProd;
      tam++;
    }
  }
  return tam;
}

PONT buscarID(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return atual;
      atual = atual->proxProd;
    }
  }
  return NULL;
}

PONT buscarIDTipo(PLISTA l, int id, int tipo){
  if (tipo<0 || tipo>=NUMTIPOS) return NULL;
  PONT atual = l->LISTADELISTAS[tipo]->proxProd;
  while (atual) {
    if (atual->id == id) return atual;
    atual = atual->proxProd;
  }
  return NULL;
}

void exibirLog(PLISTA f){
  int numElementos = tamanho(f);
  printf("Log lista [elementos: %i]\n", numElementos);
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    printf("  #TIPO: %i -> ", x);
    atual = f->LISTADELISTAS[x]->proxProd;
    while (atual){
      printf(" [%i;%i;%i;$%i]", atual->id, atual->quantidade, atual->valorUnitario, atual->quantidade*atual->valorUnitario);
      atual = atual->proxProd;
    }
    printf("\n");
  }
  printf("\n");
}

int consultarValorUnitario(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return atual->valorUnitario;
      atual = atual->proxProd;
    }
  }
  return 0;
}

PONT buscaElemento(PLISTA l, int id, PONT *ant){
    int x;
    PONT atual;
    for (x = 0; x < NUMTIPOS; x++){
        atual = l->LISTADELISTAS[x]->proxProd;
        *ant = l->LISTADELISTAS[x];
        while (atual){
            if (atual->id == id){
                return atual;
            }
            *ant = atual;
            atual = atual->proxProd;
        }
    }
    return NULL;
}

int buscaTipo(PLISTA l, int id){
    int x;
    PONT atual;
    for (x = 0; x < NUMTIPOS; x++){
        atual = l->LISTADELISTAS[x]->proxProd;
        while (atual){
            if (atual->id == id){
                return x;
            }
            atual = atual->proxProd;
        }
    }
}

bool inserirNovoProduto(PLISTA l, int id, int tipo, int quantidade, int valor)
{
    if (tipo < 0 || tipo >= NUMTIPOS){
        return false;
    }

    if (quantidade < 1 || valor < 1 || id < 1){
        return false;
    }

    if (buscarID(l, id) != NULL){
        return false;
    }

    int x;
    PONT ant = l->LISTADELISTAS[tipo];
    PONT atual = l->LISTADELISTAS[tipo]->proxProd;

    PONT novoElemento = (PONT)malloc(sizeof(REGISTRO));
    novoElemento->id = id;
    novoElemento->quantidade = quantidade;
    novoElemento->valorUnitario = valor;

    if (atual != NULL){
        while (atual){
            if ((quantidade * valor) <= (atual->quantidade * atual->valorUnitario)){
                ant->proxProd = novoElemento;
                novoElemento->proxProd = atual;
                return true;
            }else{
                ant = atual;
                atual = atual->proxProd;
            }
        }
    }else{
        ant->proxProd = novoElemento;
        novoElemento->proxProd = NULL;
        return true;
    }
}

bool removerItensDeUmProduto(PLISTA l, int id, int quantidade)
{
    if (buscarID(l, id) == NULL || quantidade < 1){
        return false;
    }

    PONT ant;
    int tipo;
    PONT elemento = buscaElemento(l, id, &ant);
    tipo = buscaTipo(l, id);

    if (quantidade > elemento->quantidade){
        return false;
    }

    elemento->quantidade -= quantidade;

    if (elemento->quantidade != 0){
        if (l->LISTADELISTAS[tipo]->proxProd->proxProd != NULL){
            ant->proxProd = elemento->proxProd;
            PONT antOrdena = l->LISTADELISTAS[tipo];
            PONT atualOrdena = l->LISTADELISTAS[tipo]->proxProd;
            while (atualOrdena){
                if (elemento->valorUnitario * elemento->quantidade <= atualOrdena->quantidade * atualOrdena->valorUnitario){
                    antOrdena->proxProd = elemento;
                    elemento->proxProd = atualOrdena;
                    return true;
                }else{
                    antOrdena = atualOrdena;
                    atualOrdena = atualOrdena->proxProd;
                }
            }
            if (atualOrdena == NULL){
                antOrdena->proxProd = elemento;
                elemento->proxProd = atualOrdena;
                return true;
            }
        }else{
            return true;
        }
    }else{
        ant->proxProd = elemento->proxProd;
        free(elemento);
        return true;
    }
}

bool atualizarValorDoProduto(PLISTA l, int id, int valor)
{
    if (buscarID(l, id) == NULL || valor <= 0){
        return false;
    }

    PONT ant;
    int tipo;
    PONT elemento = buscaElemento(l, id, &ant);
    tipo = buscaTipo(l, id);

    elemento->valorUnitario = valor;

    if (l->LISTADELISTAS[tipo]->proxProd->proxProd != NULL){
        ant->proxProd = elemento->proxProd;
        PONT antOrdena = l->LISTADELISTAS[tipo];
        PONT atualOrdena = l->LISTADELISTAS[tipo]->proxProd;
        while (atualOrdena){
            if ((elemento->valorUnitario * elemento->quantidade) <= (atualOrdena->quantidade * atualOrdena->valorUnitario)){
                antOrdena->proxProd = elemento;
                elemento->proxProd = atualOrdena;
                return true;
            }
            else{
                antOrdena = atualOrdena;
                atualOrdena = atualOrdena->proxProd;
            }
        }
        if (atualOrdena == NULL){
            antOrdena->proxProd = elemento;
            elemento->proxProd = atualOrdena;
            return true;
        }
    }
    else{
        return true;
    }
}