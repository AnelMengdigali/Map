
#include "map.h"

map::listofpairs::const_iterator //does not allow changes
map::find( const listofpairs& lst, const std::string& key )
{
   auto p = lst. begin( );
   while( p != lst. end( ) && !equal( p -> first, key )) //goes through the bucket till found the needed key or till reaches the end
      ++ p;
   return p; //return iterator to the found key or end(if the key was not found)
}

map::listofpairs::iterator //the same structure as above, but here no changes are allowed with the found item
map::find( listofpairs& lst, const std::string& key )
{
   auto p = lst. begin( );
   while( p != lst. end( ) && !equal( p -> first, key ))
      ++ p;
   return p;
}
    
bool map::contains_key( const std::string& key ) const 
{
   auto& b = getbucket(key); //finds the needed bucket by found index
   return find( b, key ) != b. end(); //returns true if the key was found
}

bool map::insert( const std::string& key, unsigned int val ) 
{
   auto &b = getbucket( key ); //finds the needed bucket by found index (the same operation is done by the same line of code)
   auto p = find( b, key ); //finds the needed key then returns iterator to it (the same operation is done by the same line of code)
   if( p == b. end() ) //checking if the iterator is not on the map
   { 
      b. push_back( { key, val } ); //inserting the pair
      ++ map_size; //incresing the size
      check_rehash(); //checking the load factor (the same operation is done by the same line of code)
      return true;
   }
   return false;
}

unsigned int& map::operator[] ( const std::string& key ) //
{
   auto &b = getbucket( key );
   auto p = find( b, key );
   if( p != b. end() ) //checking if the key was found
      return p -> second; //returning the reference for the second (the same operation is done by the same line of code)
   else //if the key was not found
   {
      b. push_back( { key, 0} ); //creation of the key-value pair
      ++ map_size;
      check_rehash();
      return b. back(). second; //returning the reference for the second
   }
}

unsigned int& map::at( const std::string& key )//
{
   auto &b = getbucket( key );
   auto p = find( b, key );
   if( p == b. end() ) //if the key was not found thrown the exception
      throw std::out_of_range( "at(): string not found" );
   else //key is in the map
      return p -> second;
}

unsigned int map::at( const std::string& key ) const 
{
   auto &b = getbucket( key );
   auto p = find( b, key );
   if( p == b. end() ) //if the key was not found thrown the exception
      throw std::out_of_range( "at(): string not found" );
   else //key is in the map
      return p -> second;
}

void map::rehash( size_t newbucketsize ) 
{
   if( newbucketsize < 4 )
      newbucketsize = 4;
   
   std::vector< listofpairs > newbuckets{ newbucketsize };
      //creation of the new buckets
   size_t i = 0;
   
   //Hashing all the elements from the old buckets into newbuckets
   while( i != buckets. size() )
   {
      for( const auto& el : buckets[i] )
      {
         size_t index;
         //Computing the index by hash function
         index = hash( el. first ) % newbuckets. size();
         newbuckets[ index ]. push_back( { el. first, el. second } );      
      }
      ++ i;
    }  

    buckets = newbuckets;   
}

void map::check_rehash( ) 
{
    double load_factor = loadfactor();
    
    //Chekcing the load factor, if it is more than max value does rising 
    if( load_factor > max_load_factor)
         rehash( 2 * buckets. size() );
}

bool map::remove( const std::string& key )
{
   auto &b = getbucket( key );
   auto p = find( b, key );

   if( p != b. end() ) 
   {
      b. erase( p ); //Removing the key-value pair through the iterator
      -- map_size; //Decresing the size
      return true;
   }      
   return false;
}

double map::standarddev( ) const  
{
   double sum = 0.0;
   double lf = loadfactor();
   
   //Computing the stdev
   for ( const auto& l : buckets )
   {
      double dif = l. size() - lf;
      sum += dif*dif;
   }

   return sqrt( sum / buckets. size( ));
}

void map::clear( ) 
{
   //Deleting all the elements
   for( auto& l : buckets )
      l. clear( ); 
   
   map_size = 0; 
}

bool equal( const std::string& s1, const std::string& s2 )
{ 
   //Checks for equality the two strings character by character
   if( s1. size() != s2. size() )  return false;
   
   else
   {
      for( size_t i = 0; i < s1. size(); ++ i )
      {
         if( tolower( s1[i] ) != tolower( s2[i] ) )
            return false;
      }
      return true;
   } 
}

size_t hash( const std::string& st )
{
   size_t hash_val = 0;
 
   //Computing the hash value
   for( size_t i = 0; i != st. size(); ++ i ) 
   {
      hash_val = ( ( tolower( st[i] ) ) + ( 349 * hash_val ) ); 
   } 
   return hash_val;
}

std::ostream& map::print( std::ostream& out ) const 
{
   size_t i = 0;
   
   while( i != buckets. size() ) //goes through every element and prints them
   {
      out << "buckets[ " << i << " ] : { ";
      for( listofpairs:: const_iterator it = buckets[i]. begin(); it != buckets[i]. end(); ++ it )
      {
         if( it != buckets[i]. begin() ) out << ", ";
         out << it -> first;
         out << "/"; 
         out << it -> second;
      }
      ++ i; 
      out << " }\n";
    }

    return out;
}

std::ostream& map::printstatistics( std::ostream& out ) const
{
   //Prints the values of the fields
   out << "set size =            " << size( ) << "\n";
   out << "load factor =         " << loadfactor( ) << " ( max = " << 
                                      max_load_factor << " )\n";
   out << "standard deviation =  " << standarddev( ) << "\n";
   return out;
}


