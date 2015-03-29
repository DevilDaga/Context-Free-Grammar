#pragma once
#include <vector>
#include <string>
#include <ctype.h>

#ifndef max
#define	max(a,b)			( (a) > (b) ) ? (a) : (b)
#endif // !max
#ifndef min
#define	min(a,b)			( (a) < (b) ) ? (a) : (b)
#endif // !min
#define LOOP(i,n)			for( size_t (i) = 0; (i) != (n); ++(i) )
#define NULLIFY(dest,size)	memset((dest), 0, (size) * sizeof (dest)[ 0 ] )

inline std::vector<std::string> split ( std::string str, char delimiter )
{
	size_t start = str.find_first_not_of ( delimiter ), end = start;
	std::vector<std::string> token_v;
	while ( start != std::string::npos )
	{
		end = str.find ( delimiter, start );
		token_v.push_back ( str.substr ( start, end - start ) );
		start = str.find_first_not_of ( delimiter, end );
	}
	return token_v;
}

inline std::string toUpper ( std::string str )
{
	std::string result = "";
	for each ( auto c in str )
		result.push_back ( toupper ( c ) );
	return result;
}

inline std::string increment ( std::string str )
{
	str = toUpper ( str );
	auto index = str.find_first_of ( "0123456789" );
	if ( index != std::string::npos )
	{
		auto name = str.substr ( 0, index );
		auto number = str.substr ( index );
		auto i_num = std::stoi ( number ) + 1U;
		number = std::to_string ( i_num );
		return ( name + number );
	}
	else
	{
		auto name = str;
		name.push_back ( '1' );
		return name;
	}
}

inline void replaceAll ( std::string& str, const std::string& from, const std::string& to )
{
	if ( from.empty ( ) )
		return;
	size_t start_pos = 0;
	while ( ( start_pos = str.find ( from, start_pos ) ) != std::string::npos )
	{
		str.replace ( start_pos, from.length ( ), to );
		start_pos += to.length ( ); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

template<typename T>
inline void replaceAll ( std::vector<T>& vec, std::vector<T> from, std::vector<T> to )
{
	while ( true )
	{
		auto it = search ( vec.begin ( ),
						   vec.end ( ),
						   from.begin ( ),
						   from.end ( )
						   );
		if ( it == vec.end ( ) )
		{
			updated.push_back ( vec );
			break;
		}
		it = vec.erase ( it, it + from.size ( ) );
		vec.insert ( it,
					 to.begin ( ),
					 to.end ( )
					 );
	}
}

template<typename T>
inline std::vector<T> append ( std::vector<T> original, std::vector<T> toAppend )
{
	original.insert (
		original.end ( ),
		toAppend.begin ( ),
		toAppend.end ( )
		);
	return original;
}

template<typename T>
inline bool contains ( std::vector<T> vec, T x )
{
	for each ( auto it in vec )
		if ( it == x )
			return true;
	return false;
}

template<typename T>
inline bool erase ( std::vector<T> vec, T x )
{
	LOOP ( i, vec.size ( ) )
	{
		if ( vec[ i ] == x )
		{
			vec.erase ( vec.begin ( ) + i );
			return true;
		}
	}
	return false;
}

template<typename T>
inline size_t count ( std::vector<T> vec, T x )
{
	size_t c = 0;
	for each ( auto it in vec )
		if ( it == x )
			++c;
	return c;
}