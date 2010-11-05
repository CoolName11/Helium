//----------------------------------------------------------------------------------------------------------------------
// ConcurrentHashSet.inl
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

namespace Lunar
{
    /// Hash set key extract function.
    ///
    /// @param[in] rValue  Value from which to extract the key.
    ///
    /// @return  Constant reference to the key.
    template< typename T >
    const T& ConcurrentHashSetExtractKey< T >::operator()( const T& rValue ) const
    {
        return rValue;
    }

    /// Constructor.
    ///
    /// @param[in] bucketCount  Number of buckets to allocate in the hash table.
    template< typename Key, typename HashFunction, typename EqualKey, typename Allocator >
    ConcurrentHashSet< Key, HashFunction, EqualKey, Allocator >::ConcurrentHashSet( size_t bucketCount )
        : Super( bucketCount, HashFunction(), EqualKey() )
    {
    }

    /// Copy constructor.
    ///
    /// @param[in] rSource  Source hash set from which to copy.
    template< typename Key, typename HashFunction, typename EqualKey, typename Allocator >
    ConcurrentHashSet< Key, HashFunction, EqualKey, Allocator >::ConcurrentHashSet( const ConcurrentHashSet& rSource )
        : Super( rSource )
    {
    }

    /// Destructor.
    template< typename Key, typename HashFunction, typename EqualKey, typename Allocator >
    ConcurrentHashSet< Key, HashFunction, EqualKey, Allocator >::~ConcurrentHashSet()
    {
    }

    /// Assignment operator.
    ///
    /// @param[in] rSource  Source hash set from which to copy.
    ///
    /// @return  Reference to this object.
    template< typename Key, typename HashFunction, typename EqualKey, typename Allocator >
    ConcurrentHashSet< Key, HashFunction, EqualKey, Allocator >&
        ConcurrentHashSet< Key, HashFunction, EqualKey, Allocator >::operator=( const ConcurrentHashSet& rSource )
    {
        if( this != &rSource )
        {
            Super::operator=( rSource );
        }

        return *this;
    }
}
