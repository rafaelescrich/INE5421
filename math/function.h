/* function.h
 * Função matemática qualquer, de qualquer domínio para qualquer imagem.
 *
 * Embora na matemática qualquer tipo de objeto possa ser usado como 
 * domínio, devido à forma como esta classe está implementada, apenas
 * domínios totalmente ordenados podem ser utilizados.
 *
 * Para implementar funções de várias variáveis, utilize std::tuple
 * ou std::pair; estas classes implementam comparação lexicográfica
 * dos elementos.
 */
#ifndef FUNCTION_H
#define FUNCTION_H

#include <exception>
#include <initializer_list>
#include <map>
#include <utility> // std::pair

namespace Math {

/* D é o tipo do domínio, I é o tipo da imagem. */
template< typename D, typename I >
class Function {
    std::map<D, I> values;

public:
    /* Constroi uma função vazia. */
    Function() = default;

    /* Constrói a função com os pares ordenados passados. */
    Function( std::initializer_list< std::pair<D, I> > );

    /* Recupera o valor da função na posição especificada.
     * Caso a posição não exista no domínio da função,
     * std::domain_error é lançado; o objeto não é alterado.
     *
     * Para garantir que operator() nunca lançe exceções, veja onDomain(). */
    I operator()( D ) const;

    /* Retorna true caso x pertença ao domínio desta função.
     * operator() lança exceções exatamente quando onDomain() retorna false. */
    bool onDomain( D x ) const;

    /* Adiciona o mapeamento de x para fx na função.
     * Caso x já esteja no domínio desta função, o mapeamento atual
     * será alterado para fx. */
    void insert( D x, I fx );
};

// Implementação

template< typename D, typename I >
Function<D, I>::Function( std::initializer_list< std::pair<D, I> > list ) :
    values( list.begin(), list.end() )
{}

template< typename D, typename I >
I Function<D, I>::operator()( D x ) const {
    auto iteratorToPair = values.find( x );
    if( iteratorToPair == values.end() )
        throw std::domain_error( "Element is not in domain of function." );
    return iteratorToPair->second;
}

template< typename D, typename I >
bool Function<D, I>::onDomain( D x ) const {
    auto iteratorToPair = values.find( x );
    return iteratorToPair != values.end();
}

template< typename D, typename I >
void Function<D, I>::insert( D x, I fx ) {
    values[x] = fx;
}

} // namespace Math

#endif // FUNCTION_H