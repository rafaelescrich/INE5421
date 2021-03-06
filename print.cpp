/* print.cpp
 * Implementação de print.h
 */

#include <cstdio>
#include "print.h"

using std::printf;
using std::set;
using std::pair;

void print( const DFA< int, char >& dfa ) {
    printf( "       " );
    for( char c : dfa.alphabet )
        printf( "%4c", c );
    printf( "\n" );
    for( int q : dfa.states ) {
        printf( "%s%s%4d", q == dfa.initialState ? "->" : "  ",
                           dfa.finalStates.count(q) == 0 ? " ":"*",
                           q );
        for( char c : dfa.alphabet )
            if( !dfa.delta.onDomain({q, c}) )
                printf( "   -" );
            else
                printf( "%4d", dfa.delta({q, c}) );
        printf( "\n" );
    }
}

void print( const DFA< set<int>, char >& dfa ) {
    printf( "             " );
    for( char c : dfa.alphabet )
        printf( "%10c", c );
    printf( "\n" );
    for( set<int> q : dfa.states ) {
        printf( "%s%s%10s", q == dfa.initialState ? "->" : "  ",
                            dfa.finalStates.count(q) == 0 ? " ":"*",
                            tostr( q ) );
        for( char c : dfa.alphabet )
            if( !dfa.delta.onDomain({q, c}) )
                printf( "         -" );
            else
                printf( "%10s", tostr( dfa.delta({q, c}) ) );
        printf( "\n" );
    }
}

void print( const DFA< pair<int, int>, char >& dfa ) {
    printf( "          " );
    for( char c : dfa.alphabet )
        printf( "%8c", c );
    printf( "\n" );
    for( pair<int, int> q : dfa.states ) {
        printf( "%s%s[%2d,%2d]", q == dfa.initialState ? "->" : "  ",
                            dfa.finalStates.count(q) == 0 ? " ":"*",
                            q.first, q.second );
        for( char c : dfa.alphabet )
            if( !dfa.delta.onDomain({q, c}) )
                printf( "         -" );
            else
                printf( " [%2d,%2d]", dfa.delta({q, c}).first, 
                                      dfa.delta({q, c}).second );
        printf( "\n" );
    }
}

void print( const NFA< int, char >& nfa ) {
    printf( "       " );
    for( char c : nfa.alphabet )
        printf( "%10c", c );
    printf( "\n" );
    for( int q : nfa.states ) {
        printf( "%s%s%4d", q == nfa.initialState ? "->" : "  ",
                           nfa.finalStates.count(q) == 0 ? " ":"*",
                           q );
        for( char c : nfa.alphabet )
            if( !nfa.delta.onDomain({q, c}) )
                printf( "         -" );
            else
                printf( "%10s", tostr( nfa.delta({q, c}) ) );
        printf( "\n" );
    }
}

void print( const NFAe< int, char >& nfa ) {
    printf( "       " );
    for( char c : nfa.alphabet )
        printf( "%10c", c );
    printf( "   epsilon\n" );
    for( int q : nfa.states ) {
        printf( "%s%s%4d", q == nfa.initialState ? "->" : "  ",
                           nfa.finalStates.count(q) == 0 ? " ":"*",
                           q );
        for( char c : nfa.alphabet )
            if( !nfa.delta.onDomain({q, c}) )
                printf( "         -" );
            else
                printf( "%10s", tostr( nfa.delta({q, c}) ) );

        if( !nfa.delta.onDomain({q, epsilon}) )
            printf( "         -" );
        else
            printf( "%10s", tostr( nfa.delta({q, epsilon}) ) );
        printf( "\n" );
    }
}

void printRightSide( const Production< int, char >& p ) {
    bool firstElement = true;
    for( auto& either : p.right ) {
        if( firstElement )
            firstElement = false;
        else
            printf( ", " );

        if( either.is<int>() )
            printf( "%d", either.getAs<int>() );
        else
            printf( "'%c'", either.getAs<char>() );
    }
}

void printRightSide( const Production< char, char >& p ) {
    for( auto& either : p.right )
        printf( "%c", either.getAs<char>() );
}

void printRightSide( const Production< std::string, std::string >& p ) {
    for( auto& either : p.right )
        printf( "%s ", either.getAs<std::string>().c_str() );
}

void print( const Grammar< char, char >& g ) {
    printf( "Start symbol: %c\n", g.startSymbol );
    const char * offset = "P = {";

    for( char nonTerminal : g.nonTerminals ) {
        auto range = g.productionsFrom( nonTerminal );
        if( range.empty() ) continue;

        printf( "%s%c -> ", offset, nonTerminal ); offset = "     ";
        const char * space = "";
        for( const auto& production : range ) {
            printf( "%s", space ); space = " | ";
            printRightSide( production );
        }
        printf( "\n" );
    }
    printf( "}\n" );
}

void print( const Grammar< int, char >& g ) {
    printf( "Start symbol: %d\n", g.startSymbol );
    const char * offset = "P = {";

    for( char nonTerminal : g.nonTerminals ) {
        auto range = g.productionsFrom( nonTerminal );
        if( range.empty() ) continue;

        printf( "%s%d -> ", offset, nonTerminal ); offset = "     ";
        const char * space = "";
        for( const auto& production : range ) {
            printf( "%s", space ); space = " | ";
            printRightSide( production );
        }
        printf( "\n" );
    }
    printf( "}\n" );
}

void print( const Grammar< std::string, std::string >& g ) {
    printf( "Start symbol: %s\n", g.startSymbol.c_str() );
    const char * offset = "P = {";

    for( auto nonTerminal : g.nonTerminals ) {
        auto range = g.productionsFrom( nonTerminal );
        if( range.empty() ) continue;

        printf( "%s%s -> ", offset, nonTerminal.c_str() ); offset = "     ";
        const char * space = "";
        for( const auto& production : range ) {
            printf( "%s", space ); space = "| ";
            printRightSide( production );
        }
        printf( "\n" );
    }
    printf( "}\n" );
}

void print( const TokenVector< char >& v ) {
    for( const auto& token : v )
        printf( "%-3.3s", tostr(token) );
    printf( "\n" );
}
typedef Either<char, Epsilon, Operator> EitherCEO;
typedef BinaryTree<EitherCEO>::const_iterator TreeIterator;

void print( const std::vector<TreeIterator>& vec ) {
    std::vector<TreeIterator> r;
    bool callAgain = false;
    int printSize = 32 / vec.size();
    if( printSize == 0 ) {
        printf( "Tree too deep\n" );
        return;
    }
    for( TreeIterator it : vec ) {
        if( !it ) {
            r.push_back( it ); r.push_back( it );
            printf( "%*.s", 2*printSize, "" );
            continue;
        }
        printf( "%*.*s%*.s", printSize, printSize, tostr(*it), printSize, "" );
        r.push_back( it.leftChild() );
        r.push_back( it.rightChild() );
        callAgain = true;
    }
    printf( "\n%s", vec.size() < 3 ? "\n" : "" );
    if( callAgain )
        print( r );
}

void print( const BinaryTree<EitherCEO>& tree ) {
    for( const auto& node : tree.raw() )
        printf( "{%i %i %i / %s} ", node.parent, node.leftChild, 
                node.rightChild, tostr(node.data) );
    printf( "\n" );
    print( std::vector<TreeIterator>{ tree.root() } );
}

void print( const std::set< EitherIterator >& set ) {
    const char * str = "";
    for( auto iterator : set ) {
        if( iterator )
            printf( "%s(%d,%s)", str, iterator.rawIndex(), tostr(*iterator) );
        else
            printf( "%s(null)", str );
        str = " ";
    }
}

void print( const std::pair<
                std::set< EitherIterator >,
                std::map< EitherIterator, std::set< EitherIterator > >
            > & pair )
{
    printf( "Initial: " );
    print( pair.first );
    printf( "\n" );
    for( auto inner : pair.second ) {
        printf( "[%s, ", tostr( *inner.first ) );
        print( inner.second );
        printf( "]\n" );
    }
}

const char * tostr( const Either<char, Epsilon, Operator, Parentheses>& e ) {
    if( e.is<Epsilon>() )
        return "&";
    if( e.is<Operator>() )
        switch( e.operator Operator() ) {
            case Operator::SigmaClosure:      return ":";
            case Operator::KleneeClosure:     return "*";
            case Operator::PositiveClosure:   return "+";
            case Operator::Optional:          return "?";
            case Operator::Concatenation:     return ".";
            case Operator::VerticalBar:       return "|";
        }
    if( e == Parentheses::Left )
        return "(";
    if( e == Parentheses::Right )
        return ")";

    static char v[] = "'x'";
    v[1] = e;
    return v;
}

const char * tostr( const Either<char, Epsilon, Operator>& e ) {
    if( e.is<Epsilon>() )
        return "&";
    if( e.is<Operator>() )
        switch( e.operator Operator() ) {
            case Operator::SigmaClosure:      return ":";
            case Operator::KleneeClosure:     return "*";
            case Operator::PositiveClosure:   return "+";
            case Operator::Optional:          return "?";
            case Operator::Concatenation:     return ".";
            case Operator::VerticalBar:       return "|";
        }

    static char v[] = "'x'";
    v[1] = e;
    return v;
}

const char * tostr( const set< int >& state ) {
    using std::sprintf;
    static char str[1024];

    str[0] = '\0';
    char * ptr = str;
    ptr += sprintf( ptr, "{" );

    bool firstPrint = true;
    for( int q : state )
        if( firstPrint ) {
            ptr += sprintf( ptr, "%d", q );
            firstPrint = false;
        } else
            ptr += sprintf( ptr, ", %d", q );

    sprintf( ptr, "}" );
    return str;
}
