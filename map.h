
#ifndef MAP_INCLUDED
#define MAP_INCLUDED   1

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>

bool equal( const std::string& s1, const std::string& s2 );
   //Returns true if s1 and s2 are equal

size_t hash( const std::string& s );
   //Returns computed hash value of s

class map
{  //Fields of the class:
   size_t map_size;
   double max_load_factor;

   using listofpairs = std::list< std::pair< std::string, unsigned int >> ;
      //For convinience 

   std::vector< listofpairs > buckets;

//Methods of the class:

public:  

   //both find functions find the needed key
   static listofpairs :: iterator  
      find( listofpairs& lst, const std::string& key );

   static listofpairs :: const_iterator 
      find( const listofpairs & lst, const std::string& key ); 

   //both getbucket functions finds needed bucket
   listofpairs& getbucket( const std::string& key )
      { return buckets[ hash( key ) % buckets. size( ) ]; }
   const listofpairs& getbucket( const std::string& key ) const
      { return buckets[ hash( key ) % buckets. size( ) ]; }

public:
  
   //Default constructor:
   map( size_t num_buckets = 4, double max_load_factor = 3.0 ) 
      : map_size{ 0 },
        max_load_factor{ max_load_factor },
        buckets{ std::vector< listofpairs > ( num_buckets ) }
   {
      if( num_buckets == 0 )
         throw std::runtime_error( "number of buckets cannot be zero" ); 
   }

   map( const map& m ) = default;
      //Copy constructor
 
   map& operator = ( const map& m ) = default; 
      //Assignment operator

   ~map() = default;
      //Destructor
 
   //Other constructor (from initializer list):
   map( std::initializer_list<std::pair< std::string, unsigned int >> init )
      : map( ) 
   {
      for( const auto& p : init )
         insert( p. first, p. second );
   }

   bool insert( const std::string& key, unsigned int val );
      //Does insertion; returns true if the insertion took place

   //Accessors:
   unsigned int& operator[] ( const std::string& key );
   unsigned int& at( const std::string& key ); 
   unsigned int at( const std::string& key ) const;
      
   bool contains_key( const std::string& key ) const;
      //Returns true if the hash map has key

   bool remove( const std::string& key );
      //Does remove; returns true if s was present

   //Returns a number of strings in the hash map:
   size_t size() const
   {
      return map_size;
   }

   //Returns true if the hash map empty:
   bool isempty() const
   {
      return map_size == 0;
   }

   //Computes a value of the load factor:
   double loadfactor() const 
   {
      return static_cast<double> ( map_size ) / buckets. size( );  
   }

   //Computes the stdev value:
   double standarddev() const;

   void clear();
      //Removes all the elements from the hash table
  
   void rehash( size_t newbucketsize );
      //Rehashes when load factor becomes bigger than max_load_factor

   void check_rehash( );
      //Calls rehash( ) if it is needed

   std::ostream& print( std::ostream& out ) const; 
      //Does a printing

   std::ostream& printstatistics( std::ostream& out ) const;
      //Prints the fields 
};

//Provides a nice printing:
inline std::ostream& operator << ( std::ostream& out, const map& m ) 
{
   return m. print( out );
}

#endif


